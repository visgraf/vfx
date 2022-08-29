#include <stdio.h>
#include "shade.h"
#include "raster.h"


Color point_shade(Vector3 p, Vector3 n, Vector3 v, RContext *rc, Material *m)
{
  return (*m->luminance)(rc_set(rc, v3_unit(v3_sub(v, p)), p, n, m));
}

Color point_texshade(Vector3 p, Vector3 n, Vector3 v, RContext *rc, Material *m, Prim *s)
{
  Vector3 t, du, dv, u;
  u = prim_inv_map(s, p);
  t = prim_texc(s, u.x, u.y); 
  du = v3_unit(prim_du(s, u.x, u.y));
  dv = v3_unit(prim_dv(s, u.x, u.y));
  return (*m->luminance)(rc_tset(rc, v3_unit(v), p, n, t, du, dv, m));
}

Color flat_shade(Hpoly *p, Vector3 v, RContext *rc, Material *m)
{
  Vector3 c = hpoly_centr(p);
  Vector3 n = hpoly_normal(p);
  return (*m->luminance)(rc_set(rc, v3_unit(v3_sub(v, c)), c, n, m));
}


void *flat_setshade(FlatData *d, Hpoly *p, Vector3 v, RContext *rc, Material *m)
{
  d->img = rc->img;
  d->c = flat_shade(p, v, rc, m);
  return d;
}

void flat_paint(Vector4 p, int n, int lv, Real lt, int rv, Real rt, Real st, void *data)
{
  FlatData *d = data;
  img_putc(d->img, p.x, p.y, col_dpymap(d->c));
}


void *gouraud_set(GouraudData *g, Hpoly *c, Image *i)
{
  g->img = i;
  g->cols = c;
  return (void *)g;
}

void gouraud_shade(Hpoly *c, Hpoly *p, Hpoly *n, Vector3 v, RContext *rc, Material *m)
{
  int i;
  for (i = 0; i < p->n; i++)
    c->v[i] = v4_v3conv((*m->luminance)(rc_set(rc, v3_unit(v3_sub(v, v3_v4conv(p->v[i]))), 
					       v3_v4conv(p->v[i]), v3_v4conv(n->v[i]), m)));
}

void gouraud_paint(Vector4 p, int n, int lv, Real lt, int rv, Real rt, Real st,void *data)
{
  GouraudData *d = data;
  Vector3 c = v3_v4conv(seg_bilerp(d->cols, n, st, lv, lt, rv, rt));
  img_putc(d->img, p.x, p.y, col_dpymap(c));
}

void *phong_set(PhongData *d, Hpoly *p, Hpoly *n, Vector3 v, RContext *rc, Material *m)
{
  d->pnts = p;
  d->norms = n;
  d->v = v;
  d->rc = rc;
  d->rc->m = m;
  return (void *)d;
}

void phong_shadepaint(Vector4 p, int n, int lv, Real lt, int rv, Real rt, Real st, void *data)
{
  PhongData *d = data;
  Vector3 pv = v3_v4conv(seg_bilerp(d->pnts, n, st, lv, lt, rv, rt));
  Vector3 pn = v3_unit(v3_v4conv(seg_bilerp(d->norms, n, st, lv, lt, rv, rt)));
  Color c = point_shade(pv, pn, d->v, d->rc, d->rc->m);
  img_putc(d->rc->img, p.x, p.y, col_dpymap(c));
}
