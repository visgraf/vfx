#include "hier.h"

Group *group_new(Object *ol, Transform *t)
{
  Group *g = NEWSTRUCT(Group);
  g->t = t;
  g->children = ol;
  return g;
}

void group_free(Group *gr)
{
  obj_list_free(gr->children);
  xform_free(gr->t);
  efree(gr);
}

