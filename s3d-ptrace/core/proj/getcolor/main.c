#include "main.h"

static HDRScene *s;
static Matrix4 mclip, mdpy;

#define MAX_NFRAMES 1000
#define MAX_PTRACE_SAMPLES  100
#define PATH_SIZE 1
#define MAX_N 10000

void main( int argc, char **argv )
{
  Real sl;
  Color bk, c_aux, c_med, bk_med;
  int u, v, smpl, ll_y, ur_y;
  long int n;
  Ray r;
  Object *obj;
  CamData *cd;
  HDRImage *aux;
  char str[50];
  
  srand(time(NULL));
  hdrdome_init();
  init_lang();
  s = hdrscene_read();
  aux = hdrimg_read( "bk0.pfm" );
  cd = cam_data_alloc( MAX_NFRAMES, aux->w, aux->h, .01, 1000. ); 
  mmove_read( cd, argv[1], argv[2] );
  mmove_view( s->view, cd, atoi( argv[3] ) );
  init_render();
  
  sprintf( str, "bk%i.pfm", atoi( argv[3] ) );
  s->plshadow->img = hdrimg_read( str ); 
  
  ll_y = s->view->sc.ll.y;
  ur_y = s->view->sc.ur.y;
  
  obj = plshadow_to_obj( s->plshadow );
  
  n = 0;
  c_med = C_BLACK;
  bk_med = C_BLACK;
  for(v = ll_y; v < ur_y; v += 1) {
     for (u = s->view->sc.ll.x; u < s->view->sc.ur.x; u += 1) {
       r = ray_unit(ray_transform(ray_view(u, v), mclip));
       bk = hdrimg_getc( s->plshadow->img, u, v );
       for( smpl = 0; smpl < MAX_PTRACE_SAMPLES; smpl++ ){
          if( (hit_surface( r, obj )) && (!hit_surface( r, s->objs )) &&
              (s->plshadow != NULL) && (s->plshadow->img != NULL) &&
              (n < MAX_N) ){
                 c_aux = dome_trace_path(PATH_SIZE, PATH_SIZE, r, obj );
                 c_med = v3_add( v3_scale( ((Real)n)/(n+1), c_med ),
                                 v3_scale( 1./(n+1), c_aux )); 
                 bk_med = v3_add( v3_scale( ((Real)n)/(n+1), bk_med ),
                                 v3_scale( 1./(n+1), bk ));  
                 n++;
          } 
       }
     }
 }

 if(s->plshadow != NULL)  
   printf( "%lf %lf %lf\n", (bk_med.x/c_med.x)*s->plshadow->m->c.x,
                            (bk_med.y/c_med.y)*s->plshadow->m->c.y,
                            (bk_med.z/c_med.z)*s->plshadow->m->c.z ); 
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
