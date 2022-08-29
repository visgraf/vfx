
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "opengl.h"
#include "loadprogram.h"
#include "shadertable.h"

#define GL_MAXLIGHTS 8 
#define BUFFER_OFFSET(i) ((char *)0 + (i))

// textures
static int max_textures_gl = 2;

// time
static int timer_on = FALSE;
static double time_start = 0;
static double time_curr = 0;

// framebuffer object
static int render_to_fbo = FALSE;
static GLuint fboId;                   // ID of FBO
static GLuint texfbId;                 // ID of texture FBO
static GLuint rboId;                   // ID of Renderbuffer object


double gettime()
{
#ifdef _WIN32
  LARGE_INTEGER counter, freq;
  QueryPerformanceCounter(&counter);
  QueryPerformanceFrequency(&freq);
  return (1.0*counter.QuadPart)/(1.0*freq.QuadPart);
#else
  struct timeval v;
  gettimeofday(&v, (struct timezone *) NULL);
  return v.tv_sec + v.tv_usec/1.0e6;
#endif
}

void matrix4_to_glmat(GLfloat *g, Matrix4 m)
{
  g[0] = m.r1.x;  g[1] = m.r2.x;  g[2] = m.r3.x; g[3] = m.r4.x;
  g[4] = m.r1.y;  g[5] = m.r2.y;  g[6] = m.r3.y; g[7] = m.r4.y;
  g[8] = m.r1.z;  g[9] = m.r2.z;  g[10] = m.r3.z; g[11] = m.r4.z;
  g[12] = m.r1.w; g[13] = m.r2.w; g[14] = m.r3.w; g[15] = m.r4.w;
}

void setup_texture(Object *o)
{
  TmapInfo *ti = o->mat->tinfo;
  if (ti == NULL)
    return;
  if (ti->src->type == TSRC_IMAGE) {
    GLuint texture;
    Image *tmap = (Image *)(ti->src->texdata);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glGenTextures( 1, &texture );
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmap->w, tmap->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tmap->c);
    if (ti->code == TMAP_TILE) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    ti->src->gltex = texture;
  }
}

void init_fbo(Scene *s)
{
  // create a framebuffer object
  glGenFramebuffers(1, &fboId);
  glBindFramebuffer(GL_FRAMEBUFFER, fboId);

  // The texture 
  glGenTextures(1, &texfbId);
  glBindTexture(GL_TEXTURE_2D, texfbId);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, s->img->w, s->img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // The depth buffer
  glGenRenderbuffers(1, &rboId);
  glBindRenderbuffer(GL_RENDERBUFFER, rboId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, s->img->w, s->img->h);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

  // attach a texture to FBO color attachement point
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texfbId, 0);
	
  // check FBO status *TODO*

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void set_render_to_fbo_gl(int flag)
{
  render_to_fbo = flag;
}

void init_gl(Scene *s)
{
  GLint ntexu, ntexc;

  if( glewInit() != GLEW_OK ) 
    { printf( "[error] %s\n", glewGetErrorString( glewInit() ) ); return; }

  glGetIntegerv(GL_MAX_TEXTURE_COORDS, &ntexc);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ntexu);
  max_textures_gl = MAX(ntexc, ntexu) - 1;

  glClearColor( 0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  if (render_to_fbo)
    init_fbo(s);
  
  st_cleanup();
}

void setup_matrix_gl(GLint m_unif, Matrix4 mview, Matrix4 proj)
{
    GLfloat mat[16];
    matrix4_to_glmat(mat, m4_m4prod(proj,mview));
    glUniformMatrix4fv(m_unif, 1, 0, mat);
}

char *fragp_init_lights(Light *lights, const char *fragp)
{
  char *out, buf[10000];
  int i, nlights; Light *l;

  for (i=0, l=lights; l!=NULL; i++, l=l->next)
    ;
  nlights = (i < GL_MAXLIGHTS) ? i : GL_MAXLIGHTS;
  sprintf(buf, "struct lightsource {  vec3 amb, col;  vec3 pos, dir;  float expo, cutoff, att0, att1, att2; };\n");
  sprintf(buf+strlen(buf), " int nlights; int ltype[%d]; lightsource light[%d]; \n", nlights, nlights);
  sprintf(buf+strlen(buf), " void set_lights() { nlights=%d;", nlights);
  for (i=0, l=lights; i < nlights; i++, l=l->next) {
    sprintf(buf+strlen(buf), " ltype[%d]=%d;", i, l->type);
    sprintf(buf+strlen(buf), " light[%d].amb = vec3(%f,%f,%f);", i, l->ambient*l->color.x, l->ambient*l->color.y,l->ambient*l->color.z);
    sprintf(buf+strlen(buf), " light[%d].col = vec3(%f,%f,%f);", i, l->intensity*l->color.x, l->intensity*l->color.y, l->intensity*l->color.z);
    sprintf(buf+strlen(buf), " light[%d].pos = vec3(%f,%f,%f);", i, l->loc.x, l->loc.y, l->loc.z);
    sprintf(buf+strlen(buf), " light[%d].dir = vec3(%f,%f,%f);", i, l->dir.x, l->dir.y, l->dir.z);
    sprintf(buf+strlen(buf), " light[%d].expo = %f;", i, l->distr);
    sprintf(buf+strlen(buf), " light[%d].cutoff = %f;", i, l->cutoff);
    sprintf(buf+strlen(buf), " light[%d].att0 = %f;", i, l->att0);
    sprintf(buf+strlen(buf), " light[%d].att1 = %f;", i, l->att1);
    sprintf(buf+strlen(buf), " light[%d].att2 = %f;", i, l->att2);
  }
  sprintf(buf+strlen(buf), "}");
  asprintf(&out, "%s\n%s", buf, fragp);
  return out;
}

void setup_programs_gl(Scene *s, Object *objs)
{
  int k, n; Object *o;

  for (k = 0, o = objs; o != NULL; k++, o = o->next) {
    switch (o->type) {
    case V_TRIMESH: case V_PRIM: {
      GLuint pid = st_lookup(o->mat->shd.name);
      if (pid == 0) {
	char *buf = fragp_init_lights(s->lights, o->mat->shd.fragp);
	pid = LoadProgram_GL(o->mat->shd.vertp, buf);
	efree(buf);
	st_install(o->mat->shd, pid);
      }
      o->mat->shd.id = pid;
    }
      break;
    case V_GROUP:
      setup_programs_gl(s, children_(o));
      break;
    default: error("(setup_prog_gl) wrong type");
    }
  }
}

void setup_scene_gl(Scene *s)
{
  init_gl(s);
  setup_programs_gl(s, s->objs);
}

void load_objects_gl(Object *objs)
{
  Object *o; iMesh *m;

  for (o = objs; o != NULL; o = o->next) {
    setup_texture(o);
    switch (o->type) {
    case V_TRIMESH:
      if (mesh_(o)->nv == NULL)
	imesh_compute_normals(mesh_(o)); 
      vbo_store_mesh_gl(o, mesh_(o)); 
      break;
    case V_PRIM:
      m = prim_mesh(prim_(o), prim_uv_decomp(prim_(o), 1.));
      vbo_store_mesh_gl(o, m); 
      break;
    case V_GROUP:
      load_objects_gl(children_(o));
      break;
    default: error("(load_obj_gl) wrong type");
    }
  }
}

void delete_shaders_gl(Item *sp)
{
  int i; GLsizei cnt, maxc = 16;
  GLuint shd[16];

  glUseProgram(0);
  glGetAttachedShaders(sp->s.id, maxc, &cnt, &(shd[0]));
  for (i=0; i<cnt; i++){
    glDeleteShader(shd[i]);
    glDetachShader(sp->s.id, shd[i]);
  }
  glDeleteProgram(sp->s.id);
}

void cleanup_programs_gl()
{
  st_apply(delete_shaders_gl);
}

void close_objs_gl(Object *objs)
{
  Object *o;
  for (o = objs; o != NULL ; o = o->next) {
    switch (o->type) {
    case V_TRIMESH: case V_PRIM: {
      TmapInfo *ti = o->mat->tinfo;
      if (ti != NULL && ti->src->type == TSRC_IMAGE) {
	GLuint texture = ti->src->gltex;
	glDeleteTextures( 1, &texture);
      }
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &(sh_vb_(o)->buffer));
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &(sh_vb_(o)->indices));

      glVertexPointer(3, GL_FLOAT, 0, NULL); 
      glNormalPointer(GL_FLOAT, 0, NULL);
      glClientActiveTexture(GL_TEXTURE0); 
      glTexCoordPointer(3, GL_FLOAT, 0, NULL);
      glClientActiveTexture(GL_TEXTURE1); 
      glTexCoordPointer(3, GL_FLOAT, 0, NULL);
      glClientActiveTexture(GL_TEXTURE2); 
      glTexCoordPointer(3, GL_FLOAT, 0, NULL);
    }
      break;
    case V_GROUP:
      close_objs_gl(children_(o));
      break;
    default: error("(close_obj_gl) wrong type");
    }
  }
}

void close_gl(Scene *s)
{
  close_objs_gl(s->objs);
  cleanup_programs_gl();
}


GLfloat *alloc_set_buffer(int nv, Vector3 *p)
{
  int i,j;
  GLfloat *buf = emalloc(nv*3*sizeof(GLfloat));
  for (i=j=0; i<nv; i++, j+=3) {
    if (p != NULL) {
      buf[j+0] =  p[i].x;  buf[j+1] = p[i].y;  buf[j+2] = p[i].z;
    } else {
      buf[j+0] = 0;  buf[j+1] = 0;  buf[j+2] = 0;
    }
  }
  return buf;
}

void vbo_store_mesh_gl(Object *o, iMesh *m)
{
  int nv, ni, i;
  GLsizei vsiz, nsiz, tsiz, dusiz, dvsiz;
  GLfloat *verts = NULL;
  GLfloat *norms = NULL;
  GLfloat *texc = NULL;
  GLfloat *du = NULL;
  GLfloat *dv = NULL;
  GLuint *inds = NULL;
  GLenum glerror;
  unsigned char *dest = NULL;
  iTri *t; 

  if (sh_vb_(o) == NULL)
    sh_vb_(o) = NEWSTRUCT(GLvbuf);
  ResetError_GL();
  // set vertices and attributes
  sh_vb_(o)->nvert = nv = m->n;
  verts = alloc_set_buffer(nv, m->p);
  vsiz = nv*3*sizeof(GLfloat);
  nsiz = tsiz = dusiz = dvsiz = 0;
  if (m->nv) {
    nsiz = nv*3*sizeof(GLfloat);
    norms = alloc_set_buffer(nv, m->nv);
  }
  sh_vb_(o)->ntex = 0;
  if (m->tx || TRUE) { 
    sh_vb_(o)->ntex++; tsiz = nv*3*sizeof(GLfloat);
    texc =  alloc_set_buffer(nv, m->tx); 
  }
  if (m->du || TRUE) { 
    sh_vb_(o)->ntex++; dusiz = nv*3*sizeof(GLfloat);
    du =  alloc_set_buffer(nv, m->du); 
  }
  if (m->dv || TRUE) { 
    sh_vb_(o)->ntex++; dvsiz = nv*3*sizeof(GLfloat);
    dv =  alloc_set_buffer(nv, m->dv); 
  }
  // set triangles
  for (ni = 0, t = m->t; t != NULL; t = t->next)
    ni+=3;
  sh_vb_(o)->nind = ni;
  inds =  NEWTARRAY(ni, GLuint);
  for (ni = 0, t = m->t; t != NULL; t = t->next, ni+=3) {
    inds[ni] = t->v[0]; inds[ni+1] = t->v[1]; inds[ni+2] = t->v[2]; 
  }
  glGenBuffers(1, &(sh_vb_(o)->buffer));
  glBindBuffer(GL_ARRAY_BUFFER, sh_vb_(o)->buffer);
  CheckError_GL("vbo verts", sh_vb_(o)->buffer, 0);
  glBufferData(GL_ARRAY_BUFFER, vsiz+nsiz+tsiz+dusiz+dvsiz, NULL, GL_STATIC_DRAW);
  glerror = glGetError();
  dest = (unsigned char *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  if (glerror == GL_NO_ERROR && dest) {
    memcpy(dest, (void *)verts, vsiz);
    memcpy(dest+vsiz, (void *)norms, nsiz);
    memcpy(dest+vsiz+nsiz, (void *)texc, tsiz);
    memcpy(dest+vsiz+nsiz+tsiz, (void *)du, dusiz);
    memcpy(dest+vsiz+nsiz+tsiz+dusiz, (void *)dv, dvsiz);
    glUnmapBuffer(GL_ARRAY_BUFFER); 
  } else 
    error("vbo data alloc");
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &(sh_vb_(o)->indices));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sh_vb_(o)->indices);
  CheckError_GL("vbo inds", 0, 0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ni*sizeof(GLuint), NULL, GL_STATIC_DRAW);
  glerror = glGetError();
  dest = (unsigned char *) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
  if (glerror == GL_NO_ERROR && dest) {
    memcpy(dest, (void *)inds, ni*sizeof(GLuint));
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER); 
  } else
    error("vbo ind alloc");
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  efree(verts); efree(norms); efree(inds); efree(texc); efree(du); efree(dv);
}


void display_obj_gl(Object *o, Scene *s, Stack4 *st)
{
    int l = 1;
    GLint time, dtex, eye, pos, norm, texc, vdu, vdv, mvp;
    GLsizei vsiz = sh_vb_(o)->nvert*3*sizeof(GLfloat);
    
    glUseProgram(o->mat->shd.id);
 
    set_material_gl(o->mat);

    mvp = glGetUniformLocation(o->mat->shd.id, "ModelViewProjectionMatrix");
    if (st != NULL)
        setup_matrix_gl(mvp, m4_m4prod(s->view->V, s4_getmat(st)), m4_m4prod(s->view->P, s->view->C));
    
    dtex = glGetUniformLocation(o->mat->shd.id, "color_tex");
    glUniform1i(dtex, 0);
    
    time = glGetUniformLocation(o->mat->shd.id, "time");
    glUniform1f(time, (float) (time_curr - time_start));
    
    eye = glGetUniformLocation(o->mat->shd.id, "eye");
    glUniform3f(eye,(float)(s->view->center.x),(float)(s->view->center.y),(float)(s->view->center.z));
    
    glBindBuffer(GL_ARRAY_BUFFER, sh_vb_(o)->buffer);
    
    pos = glGetAttribLocation(o->mat->shd.id, "vertpos");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    norm = glGetAttribLocation(o->mat->shd.id, "vertnormal");
    glEnableVertexAttribArray(norm);
    glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(l*vsiz) );

    if (sh_vb_(o)->ntex >= 1) { l++;
      texc = glGetAttribLocation(o->mat->shd.id, "verttexc");
      glEnableVertexAttribArray(texc);
      glVertexAttribPointer(texc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(l*vsiz) );
    }
    if (sh_vb_(o)->ntex >= 2) { l++;
      vdu = glGetAttribLocation(o->mat->shd.id, "vertdu");
      glEnableVertexAttribArray(vdu);
      glVertexAttribPointer(vdu, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(l*vsiz) );
    }
    if (sh_vb_(o)->ntex >= 3) { l++;
      vdv = glGetAttribLocation(o->mat->shd.id, "vertdv");
      glEnableVertexAttribArray(vdv);
      glVertexAttribPointer(vdv, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(l*vsiz) );
    }

    if (o->mat->tinfo != NULL) {
        TmapInfo *ti = (TmapInfo *) o->mat->tinfo;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ti->src->gltex);
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, sh_vb_(o)->indices);
    glDrawElements( GL_TRIANGLES, sh_vb_(o)->nind, GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
}

void gr_xform_gl(Object *o, Stack4 *st, void *d) 
{
  Matrix4 T = xform_m(xform_(o));
  Matrix4 Ti = xform_mi(xform_(o));
  s4_concmat(st, &(T), &(Ti));
}

void gr_display_gl(Object *o, Stack4 *st, Scene *s)
{
  display_obj_gl(o, s, st);
}

void display_gl(Scene *s)
{
  if (!timer_on) {
    timer_on = TRUE;
    time_start = time_curr = gettime();
  } else time_curr = gettime();
  
  ResetError_GL();
  
  if (render_to_fbo)
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  graph_apply(s->objs, gr_xform_gl, gr_display_gl, (void *)s);
  
  if (render_to_fbo) {
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, s->img->w, s->img->h, GL_RGB, GL_UNSIGNED_BYTE, s->img->c);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

void col_apply_weight(double w, GLfloat *gl_c, Color col)
{
  gl_c[0] = w*RED(col); gl_c[1] = w*GRN(col); gl_c[2] = w*BLU(col);
}

void set_material_gl(Material *m)
{
    GLint uloc;
    GLfloat m_par[4] = {1, 1, 1, 1};

    uloc = glGetUniformLocation(m->shd.id, "matambient");
    col_apply_weight(m->ka, m_par, m->c);
    glUniform3f(uloc, m_par[0], m_par[1], m_par[2]);
 
    uloc = glGetUniformLocation(m->shd.id, "matdiffuse");
    col_apply_weight(m->kd, m_par, m->c);
    glUniform3f(uloc, m_par[0], m_par[1], m_par[2]);
    
    uloc = glGetUniformLocation(m->shd.id, "matspecular");    
    col_apply_weight(m->ks, m_par, m->s);
    glUniform3f(uloc, m_par[0], m_par[1], m_par[2]);
    
    uloc = glGetUniformLocation(m->shd.id, "mattransmit");
    col_apply_weight(m->kt, m_par, m->s);
    glUniform3f(uloc, m_par[0], m_par[1], m_par[2]);
    
    uloc = glGetUniformLocation(m->shd.id, "matshininess");    
    glUniform1f(uloc, m->se);
}
