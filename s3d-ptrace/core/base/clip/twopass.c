#include <stdio.h>
#include <math.h>
#include "defs.h"
#include "geom.h"
#include "poly.h"
#include "clip.h"

static void poly_split(Hpoly *p,int i0,int i1,int i2,Real t,Hpoly *pa,Hpoly *pb);
static int classify_vert(Hpoly *p, Real h);
int hclip(Real h, Hpoly *p, void (*render)(), void (*plfree)());


int hither_clip(Real h, Hpoly *p, void (*render)(), void (*plfree)())
{
  if (cull_poly4(h, p)) {
    plfree(p); return FALSE;
  } else {
    return hclip(h, p, render, plfree);
  }
}

#define EDGE_CROSS_Z(P, V0, V1, H) \
                       ((REL_LT((P)->v[V0].z, H) && REL_GT((P)->v[V1].z, H)) \
                     || (REL_GT((P)->v[V0].z, H) && REL_LT((P)->v[V1].z, H)))


int hclip(Real h, Hpoly *p, void (*render)(), void (*plfree)())
{
  Hpoly *pa, *pb, *a, *b;
  int n, i0=0, i1=0, i2=0;
  double t;

  switch (classify_vert(p, h)) {
  case 0: plfree(p); return FALSE;
  case 3: render(p); return TRUE;
  case 2:  case 1:
    if (EDGE_CROSS_Z(p, 0, 1, h)) {
      i0 = 0; i1 = 1; i2 = 2;
    } else if (EDGE_CROSS_Z(p, 1, 2, h)) {
      i0 = 1; i1 = 2; i2 = 0;
    } else if (EDGE_CROSS_Z(p, 2, 0, h)) {
      i0 = 2; i1 = 0; i2 = 1;
    }
  }
  t = (p->v[i1].z - h) / (p->v[i1].z - p->v[i0].z);

  a = pa = hplist_alloc(n = hplist_lenght(p), 3);
  b = pb = hplist_alloc(n, 3);
  while (p != NULL) {
    poly_split(p, i0, i1, i2, t, pa, pb);
    p = p->next; pa = pa->next; pb = pb->next;
  }
  return hclip(h, a, render, plfree) | hclip(h, b, render, plfree);
}


static void poly_split(Hpoly *p, int i0, int i1, int i2, Real t, Hpoly *pa, Hpoly *pb)
{
  Vector4 vm = v4_add(p->v[i1], v4_scale(t, v4_sub(p->v[i0], p->v[i1])));

  pa->v[0] = p->v[i1]; pa->v[1] = p->v[i2];
  pa->v[2] = pb->v[0] = vm;
  pb->v[1] = p->v[i2]; pb->v[2] = p->v[i0];
}

static int classify_vert(Hpoly *p, Real h)
{
  int i, k = 0;
  for (i = 0; i < 3; i++)
    if (REL_LT(p->v[i].z, h))  k++;
  for (i = 0; i < 3; i++)
    if (k > 0 && REL_EQ(p->v[i].z, h))  k++;
  return k;
}


