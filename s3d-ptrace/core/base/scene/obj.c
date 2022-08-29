#include "lang.h"
#include "sdltypes.h"
#include "obj.h"
#include "hier.h"
#include "shade.h"

#ifdef SHADE_LIB
#include "shade.h"
#define SET_MAT_DEFAULT(o)     {(o)->mat = mat_default();}
#else
#define SET_MAT_DEFAULT(o) {;}
#endif

// global object id
int obj_id = 0; 


Object *obj_new(int type, void *v)
{
  Object *o = NEWSTRUCT(Object);
  o->next = NULL;
  SET_MAT_DEFAULT(o);
  o->id = obj_id++;
  o->type = type;
  if (type == V_GROUP)
    o->u.gr = v;
  else
    o->u.sh = v;
  return o;
}

void obj_free(Object *o)
{
  if (o->type ==  V_GROUP)
    group_free(o->u.gr);
  else
    shape_free(o->type, o->u.sh);
  mat_free(o->mat);
  efree(o);
}

Object *obj_insert(Object *olist, Object *o)
{
  o->next = olist;
  return o;
}

Object *obj_list_insert(Object *olist, Object *l)
{
  Object *t, *o = l;
  while (o != NULL) {
    t = o; o = o->next;
    olist = obj_insert(olist, t);
  }
  return olist;
}

void obj_list_free(Object *ol)
{
  Object *t, *o = ol;
  while (o != NULL) {
    t = o; o = o->next;
    obj_free(t);
  }
}

int obj_list_size(Object *ol)
{
  Object *o; int n;
  for (n = 0, o = ol; o != NULL; n++, o = o->next)
    ;
  return n;
}

int obj_list_is_graph(Object *ol)
{
  Object *o;
  for (o = ol; o != NULL; o = o->next)
    if (o->type == V_GROUP)
      return TRUE;
  return FALSE;
}

Val obj_parse(int c, Pval *pl)
{
  Val v = {V_NULL, {0}};
  if (c == T_EXEC) { Pval *p;
    Object *o = obj_new(0, shape_new(0, NULL));
    for (p = pl; p !=NULL; p = p->next) {
      if (p->name == NULL) {
      	error("(object) syntax error");
      } else if (strcmp(p->name, "material") == 0 && p->val.type == V_MATERIAL) {
        o->mat = p->val.u.v;
      } else if (strcmp(p->name, "shape") == 0 && p->val.type == V_PRIM) {
        o->type = V_PRIM; o->u.sh->u.prim = p->val.u.v;
      } else if (strcmp(p->name, "shape") == 0 && p->val.type == V_HPOLYLIST) {
        o->type = V_HPOLYLIST; o->u.sh->u.hpols = p->val.u.v;
      } else if (strcmp(p->name, "shape") == 0 && p->val.type == V_TRIMESH) {
        o->type = V_TRIMESH; o->u.sh->u.mesh = p->val.u.v;
      } else if (strcmp(p->name, "shape") == 0 && p->val.type == V_CSG_NODE) {
        o->type = V_CSG_NODE; o->u.sh->u.tcsg = p->val.u.v;
      } else error("(object) syntax error"); 
    }
    if (o->mat == NULL)
      SET_MAT_DEFAULT(o);
    v.type = V_OBJECT;
    v.u.v = o;
  }
  return v;
}


void obj_transform_shape(Object *o, Matrix4 m, Matrix4 mi)
{
  switch (o->type) {
  case V_PRIM: prim_transform(o->u.sh->u.prim, m, mi); break;
  case V_CSG_NODE: csg_transform(o->u.sh->u.tcsg, m, mi); break;
  case V_HPOLYLIST: hplist_transform(o->u.sh->u.hpols, m); break;
  case V_TRIMESH: imesh_transform(o->u.sh->u.mesh, m); break;
  }
}

Shape *shape_new(int type, void *v)
{
  Shape *s = NEWSTRUCT(Shape);
  s->vb = NULL;
  switch (type) {
  case V_CSG_NODE: s->u.tcsg = v; break;
  case V_PRIM: s->u.prim = v; break;
  case V_HPOLYLIST:  s->u.hpols = v; break;
  case V_TRIMESH:  s->u.mesh = v; break;
  case 0: break;
  default: error("(newshape) wrong type");
  }
  return s;
}

void shape_free(int type, Shape *s)
{
  switch (type) {
  case V_PRIM: prim_destroy(s->u.prim); break;
  case V_CSG_NODE: csg_destroy(s->u.tcsg); break;
  case V_HPOLYLIST: hplist_free(s->u.hpols); break;
  case V_TRIMESH: imesh_free(s->u.mesh); break;
  default: error("(freeshape) wrong type");
  }
  efree(s->vb);
  efree(s);
}

