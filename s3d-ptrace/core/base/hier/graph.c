#include "stack.h"
#include "hier.h"

static Object *graph_collect_objects(Pval *pl);


Val graph_parse(int pass, Pval *pl)
{
  Val v = {V_NULL, {0}};	
  switch (pass) {
  case T_PREP:
    xform_push();
    break;
  case T_EXEC: { Pval *p;
      Transform *t = NULL; int nt = 0;
      for (p = pl; p != NULL; p = p->next)
	if (p->val.type == V_TRANSFORM) 
	  { t = p->val.u.v; nt++; }
      if (nt > 1) {
	error("(group) only one transform block allowed");
      } else if (t == NULL) {
	t = xform_new(XFORM_AFFINE);
	t = xform_setmat(t, m4_ident(), m4_ident());
      }
      xform_pop();
      v.u.v = obj_new(V_GROUP, group_new(graph_collect_objects(pl), t));
      v.type = V_OBJECT;
    }
    break;
  default: break;
  }
  return v;
}

static Object *graph_collect_objects(Pval *pl)
{
  Pval *p; Object *olist = NULL;
  for (p = pl; p != NULL; p = p->next) {
    if (p->val.type == V_OBJECT) 
      olist = obj_insert(olist, p->val.u.v);
  }
  return olist;
}


void graph_exec_apply(Stack4 *st, Object *objs, void (*gfun)(), void (*sfun)(), void *data)
{
  Object *o;
  for (o = objs; o != NULL; o = o->next) {
    if (o->type == V_GROUP) {
      s4_push(st);
      gfun(o, st, data);
      graph_exec_apply(st, children_(o), gfun, sfun, data);
      s4_pop(st);
    } else {
      sfun(o, st, data);
    }
  }
}

void graph_apply(Object *objs, void (*gfun)(), void (*sfun)(), void *data)
{
  Stack4 *st = s4_initstack(MAX_STK_DEPTH);
  graph_exec_apply(st, objs, gfun, sfun, data);
  s4_free(st);
}


Object *graph_flatten(Object *ol)
{
  Object *t, *rl = NULL, *o = ol;
  while (o != NULL) {
    t = o; o = o->next;
    if (t->type == V_GROUP) {
      rl = obj_list_insert(rl, graph_flatten(children_(t)));
      children_(t) = NULL; obj_free(t);
    } else {
      rl = obj_insert(rl, t);
    }
  }
  return rl;
}

void g_conc_reset_matrix(Object *o, Stack4 *st, void *d)
{
  Matrix4 T = xform_m(xform_(o));
  Matrix4 Ti = xform_mi(xform_(o));
  s4_concmat(st, &(T), &(Ti));
  xform_reset(xform_(o));
}
void g_xform_shape(Object *o, Stack4 *st, void *d)
{
  obj_transform_shape(o, s4_getmat(st), s4_getimat(st));
}

Object *graph_transform(Object *ol)
{
  graph_apply(ol, (Gfun)(g_conc_reset_matrix), (Sfun)(g_xform_shape), NULL);
  return ol;
}

