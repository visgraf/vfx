#include <stdio.h>
#include <math.h>
#include <string.h>
#include "shade.h"


Val distlight_parse(int pass, Pval *pl)
{
  Val v = {V_NULL, {0}};
  if (pass == T_EXEC) {
    Light *l = NEWSTRUCT(Light);
    l->type = LIGHT_DISTANT;
    l->color = C_WHITE;
    l->ambient = pvl_get_num(pl, "ambient", .1);
    l->intensity = pvl_get_num(pl, "intensity", .9);
    l->dir = v3_unit(pvl_get_v3(pl, "direction", v3_make(1,1,1)));
    l->loc = v3_make(0,0,0);
    l->cutoff = l->distr = l->att0 = l->att1 = l->att2 = 0;
    l->transport = distantlight;
    l->tinfo = NULL;
    v.type = V_LIGHT;
    v.u.v = l;
  }
  return v;
}

Val plastic_parse(int pass, Pval *pl)
{
  Val v = {V_NULL, {0}};
  if (pass == T_EXEC) {
    Material *m = NEWSTRUCT(Material);
    m->c = pvl_get_v3(pl, "d_col", C_WHITE);
    m->s = pvl_get_v3(pl, "s_col", C_BLACK);
    m->ka = pvl_get_num(pl, "ka", .1);
    m->kd = pvl_get_num(pl, "kd", .9);
    m->ks = pvl_get_num(pl, "ks", 0);
    m->kt = pvl_get_num(pl, "kt", 0);
    m->se = pvl_get_num(pl, "se", 10);
    m->ir = pvl_get_num(pl, "ir", 1);
    m->luminance = plastic;
    m->tinfo = NULL;
    m->shd = parse_shader(pl, "phong_plastic"); 
    v.type = V_MATERIAL;
    v.u.v = m;
  }
  return v;
}

Shader parse_shader(Pval *pl, char *defshader)
{
  Pval *p = pl;
  while (p != NULL) {
    if (p->name && strcmp(p->name,"shader") == 0) {
      if (p->val.type == V_SHADER_FILE)
	return read_shaderfiles(p->val.u.v);
      else if (p->val.type == V_SHADER_DEF) 
	return lookup_shader(p->val.u.v);
    }
    p = p->next;
  }
  // set default
  return lookup_shader(defshader);
}

Material *mat_default()
{
  Material *m = NEWSTRUCT(Material);
  m->c = C_WHITE;
  m->ka = .1;
  m->kd = .9;
  m->s = C_BLACK;
  m->ks = 0;
  m->kt = 0;
  m->se = 10;
  m->ir = 1;
  m->luminance = matte;
  m->tinfo = NULL;
  m->shd = make_shader("phong_matte", phong_vertp, phong_matte_fragp);
  return m;
}

void mat_free(Material *mat)
{
  efree(mat->tinfo); 
  efree(mat->shd.vertp);
  efree(mat->shd.fragp);
  efree(mat);
}
