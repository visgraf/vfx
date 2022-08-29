#include "rt.h"


Inode *ray_intersect(Object *olist, Ray r)
{
  Object *o; Hpoly *p;
  Inode *l = NULL, *i = NULL;

  for (o = olist; o != NULL; o = o->next ) {
    p  = (o->type == V_HPOLYLIST)? hpols_(o) : NULL;
    do {
      switch (o->type) {
      case V_CSG_NODE:
	l = csg_intersect(tcsg_(o), r); break;
      case V_PRIM:
	l = prim_intersect(prim_(o), r); break;
      case V_HPOLYLIST:
	if (p != NULL) {
	  l = hpoly_intersect(p, hpoly3_plane(p), r);
	  p = p->next;
	} break;
      }
      if (l == NULL)
	continue;
      if (i == NULL) {
	i = inode_alloc(l->t, l->n, l->enter);
	i->m = o->mat; i->o = o;
      }
      if (l->t < i->t) {
	i->t = l->t; i->n = l->n; i->enter = l->enter;
	i->m = o->mat; i->o = o;
      }
      inode_free(l);
    } while (p != NULL);
  }
  for (l = i; l != NULL; l = l->next) {
    l->m = i->m;
    l->o = i->o;
  }
  return i;
}
