#include <stdio.h>
#include <math.h>
#include "defs.h"
#include "geom.h"
#include "poly.h"

#define LFT 0
#define RGT 1
#define TOP 2
#define BOT 3
#define FRT 4
#define BAK 5

#define LAST_P  5

#define MAXV 16
#define MAXD ((MAXV+1)*(BAK+1))


static Real plane_point_dist(int plane, Vector4 v)
{
  switch (plane) {
  case LFT: return (v.w + v.x);
  case RGT: return (v.w - v.x);
  case TOP: return (v.w + v.y);
  case BOT: return (v.w - v.y);
  case FRT: return (v.w + v.z);
  case BAK: return (v.w - v.z);
  }
  return -1;
}


#define DA(dd, i, p) dd[(p*MAXV)+i]

#define PLANE_CROSS(D0, D1) ((D0) * (D1) < 0.0)
#define ON_POSITIVE_SIDE(D1) ((D1) >= 0.0)

int pclip_store(int plane, Hpoly *s, Hpoly *d, double *dd)
{
  int i, k0=0, k1;
  double d0=0, d1;

  for (d->n = k1 = i = 0; i <= s->n ; i++,  k1 = (i == s->n)? 0 : i) {
    d1 = plane_point_dist(plane, s->v[k1]);
    DA(dd, i, plane) = d1;
    if (i != 0) {
      if (PLANE_CROSS(d0, d1))
	d->v[d->n++] = v4_add(s->v[k1], v4_scale(d1/(d1-d0), v4_sub(s->v[k0], s->v[k1])));
      if (ON_POSITIVE_SIDE(d1))
	d->v[d->n++] = s->v[k1];
    }
    d0 = d1;
    k0 = k1;
  }
  return (plane++ == LAST_P)? d->n : pclip_store(plane, d, s, dd);
}

int pclip_apply(int plane, Hpoly *s, Hpoly *d, double *dd)
{
  int i, k0=0, k1;
  double d0=0, d1;

  for (d->n = k1 = i = 0; i <= s->n ; i++,  k1 = (i == s->n)? 0 : i) {
    d1 = DA(dd, i, plane);
    if (i != 0) {
      if (PLANE_CROSS(d0, d1))
	d->v[d->n++] = v4_add(s->v[k1], v4_scale(d1/(d1-d0), v4_sub(s->v[k0], s->v[k1])));
      if (ON_POSITIVE_SIDE(d1))
	d->v[d->n++] = s->v[k1];
    }
    d0 = d1;
    k0 = k1;
  }
  return (plane++ == LAST_P)? d->n : pclip_apply(plane, d, s, dd);
}

int clip_copy(int (*clip_do)(), Hpoly *p, void (*dispose)(void*), double *dd)
{
  Vector4 vs[MAXV], vd[MAXV];
  Hpoly s = {NULL, 0, vs}, d = {NULL, 0, vd};

  hpoly_copy(p, &s);
  if (clip_do(0, &s, &d, dd) > p->n) {
    if (dispose != NULL)
      dispose(p->v);
    p->v = NEWARRAY(s.n, Vector4);
  }
  hpoly_copy(&s, p);
  return s.n;
}


int poly_clip(Hpoly *p, void (*dispose)(void*), int chain)
{
  double dd[MAXD];
  Hpoly *a;
  int n; 

  if (p->n > MAXV)
    error("(poly clip) too many vertices");

  n = clip_copy(pclip_store, p, dispose, dd);
  if (!chain)
    return n;
  for (a = p->next; a != NULL; a = a->next) 
    n = clip_copy(pclip_apply, a, dispose, dd);

  return n;
}
