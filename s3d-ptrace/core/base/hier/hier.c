#include "stack.h"
#include "hier.h"


static void transform_objects(Pval *pl);
static Object *collect_objects(Pval *pl);


Val group_parse(int pass, Pval *pl)
{
  Val v = {V_NULL, {0}};	
  switch (pass) {
  case T_PREP:
    xform_push();
    break;
  case T_EXEC:
    transform_objects(pl);
    xform_pop();
    v.u.v = collect_objects(pl);
    v.type = V_GROUP;
    break;
  default: break;
  }
  return v;
}

static void transform_objects(Pval *pl)
{
  Pval *p;
  for (p = pl; p != NULL; p = p->next)
    if (p->val.type == V_OBJECT) 
      obj_transform_shape(p->val.u.v, xform_ctm(), xform_ctmi());
}

static Object *collect_objects(Pval *pl)
{
  Pval *p; Object *olist = NULL;
  for (p = pl; p != NULL; p = p->next) {
    if (p->val.type == V_OBJECT)
      olist = obj_insert(olist, p->val.u.v);
    else if (p->val.type == V_GROUP)
      olist = obj_list_insert(olist, p->val.u.v);
  }
  return olist;
}
