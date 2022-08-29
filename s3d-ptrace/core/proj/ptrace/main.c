#include "main.h"

static Scene *s;
//static RContext *rc;
static Matrix4 mclip, mdpy;

#define MAX_PTRACE_SAMPLES 200
#define PATH_SIZE 3


int main(int argc, char **argv)
{
  Color c, c_aux;
  int u, v, smpl, ll_y, ur_y;
  Ray r;
  
  init_lang();
  s = scene_read();
  init_render(s);
  s->objs = graph_flatten(graph_transform(s->objs));
  
  ll_y = s->view->sc.ll.y;
  ur_y = s->view->sc.ur.y;
   
#pragma omp parallel for private(u, r, c, smpl, c_aux) shared(s) schedule(dynamic, 1) 
  for (v = ll_y; v < ur_y; v += 1) {
    for (u = s->view->sc.ll.x; u < s->view->sc.ur.x; u += 1) {
      r = ray_unit(ray_transform(ray_view(u, v), mclip));
      c = c_make(0,0,0);
      for( smpl = 0; smpl < MAX_PTRACE_SAMPLES; smpl++ ){
         c_aux = trace_path(PATH_SIZE, PATH_SIZE, r, s->objs );
         c = c_add( c, c_aux );
      }
      img_putc(s->img, u, v, 
                col_dpymap( c_scale(1./MAX_PTRACE_SAMPLES, c))
      );
    }
  }
  img_write(s->img,"stdout",0);
  exit(0);
}

Ray ray_view(int u, int v)
{
  Vector4 w = v4_m4mult(v4_make(u, v,s->view->sc.ur.z, 1), mdpy);
  return ray_make(v3_v4conv(v4_m4mult(v4_make(0, 0, 1, 0), mdpy)),
		  v3_make(w.x, w.y, w.z));
}

void init_render(Scene *s)
{
  mclip = m4_m4prod(s->view->Vinv, s->view->Cinv);
  mdpy = m4_m4prod(s->view->Pinv, s->view->Sinv);

  //rc_sset(rc = NEWSTRUCT(RContext), s->view, s->lights, s->img);
}

void init_lang(void)
{
 
  lang_defun("scene", scene_parse);
  lang_defun("view", view_parse);
  lang_defun("dist_light", distlight_parse);
  lang_defun("plastic", plastic_parse);
  lang_defun("primobj", obj_parse);
  lang_defun("sphere", sphere_parse);
  lang_defun("polyobj", obj_parse);
  lang_defun("trilist", htrilist_parse);
  lang_defun("group", group_parse);
  lang_defun("hdrdome", hdrdome_parse);
  lang_defun("polylight", plight_parse);
}
