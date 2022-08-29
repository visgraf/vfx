#include "main.h"

static HDRScene *s;
static Matrix4 mclip, mdpy;

#define MAX_NFRAMES 1000
#define MAX_PTRACE_SAMPLES 1000
#define PATH_SIZE 2

Color pfmimg_getc( HDRImage *img, int u, int v  );
void pfmimg_putc( HDRImage *img, int u, int v, Color c );


void main( int argc, char **argv )
{
 int i, first_frame, max_frame;
 char str[100];
 CamData *cd;
 HDRImage *aux;
 
 hdrdome_init();
 init_lang();
 s = hdrscene_read();
 aux = hdrimg_read( "bk0.pfm" );
 cd = cam_data_alloc( MAX_NFRAMES, aux->w, aux->h, .01, 1000. ); 
 mmove_read( cd, argv[1], argv[2] );
  
 
 if( argc > 3 ){
    first_frame = atoi( argv[3] );
    max_frame = first_frame + 1;
 }
 else{
    first_frame = 0;
    max_frame = cd->nframes;
 } 
   
 for( i = first_frame; i < max_frame; i++ ){
    printf( "frame %i\n", i );
    sprintf( str, "bk%i.pfm", i );
    s->plshadow->img = hdrimg_read( str );
    mmove_view( s->view, cd, i );
    render_frame( s );
    sprintf( str, "out%d.pfm", i ); 
    hdrimg_write( s->hdrimg, str ); 
    hdrimg_free( s->plshadow->img ); 
 }
  
 hdrimg_free( aux );
 cam_data_free( cd );
}


void render_frame( HDRScene *s )
{
 Color c, bk, c_aux, c_aux1, c_aux2;
 int u, v, smpl, ll_y, ur_y;
 Ray r;
 Object *obj1, *obj2;
 
 init_render();
 s->objs = graph_flatten(graph_transform(s->objs));
 
 ll_y = s->view->sc.ll.y;
 ur_y = s->view->sc.ur.y;
 
 obj1 = plshadow_to_obj( s->plshadow );
 obj1->next = s->objs;
 obj2 = plshadow_to_obj( s->plshadow );

 #pragma omp parallel for private(u, r, c, smpl, c_aux, c_aux2, c_aux1, bk )\
                                  shared(s, obj1, obj2) schedule(dynamic, 1)
  for (v = ll_y; v < ur_y; v += 1) {
    for (u = s->view->sc.ll.x; u < s->view->sc.ur.x; u += 1) {
      r = ray_unit(ray_transform(ray_view(u, v), mclip));
      c = c_make(0,0,0);
      for( smpl = 0; smpl < MAX_PTRACE_SAMPLES; smpl++ ){
         bk = pfmimg_getc( s->plshadow->img, u, v );
         c_aux = C_BLACK;
         if( (s->plshadow != NULL) && (s->plshadow->img != NULL) )
             c_aux = bk;
         if( hit_surface( r, s->objs ) ){
             c_aux = dome_trace_path(PATH_SIZE, PATH_SIZE, r, s->objs ); 
         }
         if( (hit_surface( r, obj1 )) && (!hit_surface( r, s->objs )) &&
             (s->plshadow != NULL) && (s->plshadow->img != NULL) ){
              c_aux1 = dome_trace_path(PATH_SIZE, PATH_SIZE, r, obj1 );
              c_aux2 = dome_trace_path(PATH_SIZE, PATH_SIZE, r, obj2 );
              c_aux = v3_add( bk, v3_sub( c_aux1, c_aux2 ) );
         }
         c = c_add( c, c_aux );
      }
      pfmimg_putc( s->hdrimg, u, v, c_scale(1./MAX_PTRACE_SAMPLES, c) );
    }
  }
}


Ray ray_view(int u, int v)
{
  Vector4 w = v4_m4mult(v4_make(u, v,s->view->sc.ur.z, 1), mdpy);
  return ray_make(v3_v4conv(v4_m4mult(v4_make(0, 0, 1, 0), mdpy)),
		  v3_make(w.x, w.y, w.z));
}


void init_render(void)
{
  mclip = m4_m4prod(s->view->Vinv, s->view->Cinv);
  mdpy = m4_m4prod(s->view->Pinv, s->view->Sinv);
}


Color pfmimg_getc( HDRImage *img, int u, int v  )
{
 return hdrimg_getc( img, u, img->h - v - 1 );
}


void pfmimg_putc( HDRImage *img, int u, int v, Color c )
{
 hdrimg_putc( img, u, img->h - v - 1, c );
}


void init_lang(void)
{
  lang_defun("hdrscene", hdrscene_parse);
  lang_defun("hdrdome", hdrdome_parse);
  lang_defun("view", view_parse);
  lang_defun("plastic", plastic_parse);
  lang_defun("primobj", obj_parse);
  lang_defun("sphere", sphere_parse);
  lang_defun("polyobj", obj_parse);
  lang_defun("trilist", htrilist_parse);
  lang_defun("group", group_parse);
  lang_defun("plshadow", plshadow_parse );
}

