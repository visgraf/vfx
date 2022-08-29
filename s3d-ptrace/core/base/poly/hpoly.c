#include "poly.h"


Hpoly *hpoly_alloc(int n)
{
  Hpoly *p = NEWSTRUCT(Hpoly);
  p->n = n;
  p->v = NEWARRAY(n, Vector4);
  p->next = NULL;
  return p;
}

Hpoly *hpoly_transform(Hpoly *d, Matrix4 m)
{
  int i;
  for (i = 0; i < d->n; i++) {
    d->v[i] = v4_m4mult(d->v[i], m);
  }
  return d;
}

Hpoly *hpoly_homo(Hpoly *p)
{
  int i;
  for (i = 0; i < p->n; i++) {
    p->v[i].x /= p->v[i].w;
    p->v[i].y /= p->v[i].w;
    p->v[i].z /= p->v[i].w;
    p->v[i].w = 1.0;
  }
  return p;
}

Hpoly *hpoly_homoxform(Hpoly *p, Matrix4 m)
{
  int i;
  for (i = 0; i < p->n; i++) {
    p->v[i] = v4_m4mult(p->v[i], m);
    p->v[i].x /= p->v[i].w;
    p->v[i].y /= p->v[i].w;
    p->v[i].z /= p->v[i].w;
    p->v[i].w = 1.0;
  }
  return p;
}

Vector3 hpoly_normal(Hpoly *p)
{
  return v3_unit(v3_cross(v3_sub(v3_v4conv(p->v[1]), v3_v4conv(p->v[0])), 
			  v3_sub(v3_v4conv(p->v[2]), v3_v4conv(p->v[0]))));
}

Vector3 hpoly_centr(Hpoly *p)
{
  int i;  Vector3 c = v3_make(0,0,0);
  for (i = 0; i < p->n; i++)
    c = v3_add(c, v3_v4conv(p->v[i]));
  return v3_scale(1. / (p->n), c);
}

int hpoly_copy(Hpoly *s, Hpoly *d)
{
  int i;
  for (i = 0; i < s->n; i++)
    d->v[i] = s->v[i];
  return (d->n = s->n);
}

Hpoly *hpoly_insert(Hpoly *pl, Hpoly *p)
{
  p->next = pl;
  return p;
}

Inode *hpoly_intersect(Hpoly *p, Vector4 plane, Ray r)
{
  Real t = hpoly3_ray_inter(p, plane, r);
  if (t < RAY_EPS) {
    return NULL;
  } else {
    Vector3 n = v3_make(plane.x, plane.y, plane.z);
    return inode_alloc(t, n, (v3_dot(n, r.d) < 0));
  }
}

int hplist_lenght(Hpoly *l)
{
  int n = 0;
  while (l != NULL) {
    n++; l = l->next;
  }
  return n;
}

Hpoly *hplist_alloc(int n, int m)
{
  Hpoly *l = NULL;
  while (n--)
    l = hpoly_insert(l, hpoly_alloc(m));
  return l;
}

void hplist_free(Hpoly *l)
{
  Hpoly *q, *p = l;
  while (p != NULL) {
    q = p;
    p = p->next;
    efree(q->v);
    efree(q);
  }
}

Hpoly *hplist_transform(Hpoly *l, Matrix4 m)
{
  Hpoly *p;
  for (p = l; p != NULL; p = p->next)
    hpoly_transform(p, m);
  return l;
}

// --------- Hpoly3 ---------- //

#define MINUS_INFTY (-MAX_INT)

static Real hplane_ray_inter(Vector4 h, Ray r)
{
  Vector3 n = {h.x, h.y, h.z};
  Real denom = v3_dot(n, r.d);
  if (REL_EQ(denom, 0))
    return MINUS_INFTY;
  else 
    return -(v3_dot(n, r.o) + h.w) / denom;
}

#define PROJ_BASE(A, B, Q0_S, Q0_T, Q1_S, Q1_T, Q2_S, Q2_T) \
{ Real d = (Q1_S * Q2_T - Q2_S * Q1_T); \
       A = (Q0_S * Q2_T - Q2_S * Q0_T) / d;  \
       B = (Q1_S * Q0_T - Q0_S * Q1_T) / d;  \
}

Real hpoly3_ray_inter(Hpoly *p, Vector4 plane, Ray r)
{
  Vector3 q0, q1, q2;
  Real t, a=0, b=0;

  t = hplane_ray_inter(plane, r);
  if (t < RAY_EPS)
    return MINUS_INFTY;

  q0 = v3_sub(ray_point(r, t), v3_v4conv(p->v[0]));
  q1 = v3_sub(v3_v4conv(p->v[1]), v3_v4conv(p->v[0]));
  q2 = v3_sub(v3_v4conv(p->v[2]), v3_v4conv(p->v[0]));

  switch (max3_index(fabs(plane.x), fabs(plane.y), fabs(plane.z))) {
  case 1:
    PROJ_BASE(a, b, q0.y, q0.z, q1.y, q1.z, q2.y, q2.z); break;
  case 2:
    PROJ_BASE(a, b, q0.x, q0.z, q1.x, q1.z, q2.x, q2.z); break;
  case 3:
    PROJ_BASE(a, b, q0.x, q0.y, q1.x, q1.y, q2.x, q2.y); break;
  }
  if ((a >= 0 && b >= 0 && (a+b) <= 1))
    return t;
  else
    return MINUS_INFTY;
}

Hpoly *hpoly3_make(Vector4 v0, Vector4  v1, Vector4  v2)
{
  Hpoly *p = hpoly_alloc(3);
  p->v[0] = v0;  p->v[1] = v1;  p->v[2] = v2; 
  return p;
}

Hpoly *hpoly3_set(Hpoly *p, Vector4 v0, Vector4  v1, Vector4  v2)
{
  p->n = 3;
  p->v[0] = v0;  p->v[1] = v1;  p->v[2] = v2; 
  return p;
}

Vector4 hpoly3_plane(Hpoly *p)
{
  Vector3 n = hpoly_normal(p);
  Real d = -v3_dot(n, v3_v4conv(p->v[0]));
  return v4_make(n.x, n.y, n.z, d);
}

Real hpoly3_area(Hpoly *p)
{
  return v3_norm(v3_cross(v3_sub(v3_v4conv(p->v[1]),v3_v4conv(p->v[0])), 
			  v3_sub(v3_v4conv(p->v[2]),v3_v4conv(p->v[0]))))/2;
}

void hpoly3_write(Hpoly *p, FILE* fp)
{
  fprintf(fp, "{{%g, %g, %g}, ",  p->v[0].x, p->v[0].y, p->v[0].z);
  fprintf(fp, " {%g, %g, %g}, ",  p->v[1].x, p->v[1].y, p->v[1].z);
  fprintf(fp, " {%g, %g, %g}}\n", p->v[2].x, p->v[2].y, p->v[2].z);
}

Hpoly *hpoly3_decode(Pval *plist)
{
  Pval *pl;
  int k;

  for (pl = plist, k = 0; pl !=NULL; pl = pl->next, k++)
    ;
  if (k != 3) {
    error("(poly3) wrong number of vertices");
    return NULL;
  } else {
    Hpoly *t = hpoly_alloc(3);
    for (pl = plist, k = 0; pl !=NULL; pl = pl->next, k++)
      if (pl->val.type == V_PVL)
	t->v[k] = pvl_to_v4(pl->val.u.v);
      else
	error("(hpoly3) error in vertex");
    return t;
  }
}

void htrilist_write(Hpoly *tlist, FILE* fp)
{
  Hpoly *p = tlist;
  
  fprintf(fp, "trilist {\n");
  while (p != NULL) {
    hpoly3_write(p, fp);
    if ((p = p->next) != NULL)
      fprintf(fp, ",\n");
  }
  fprintf(fp, "}\n");
}


Val htrilist_parse(int pass, Pval *plist)
{
  Val v = {V_NULL, {0}};

  if (pass == T_EXEC) {
    Pval *pl;
    Hpoly *tl = NULL;

    for (pl = plist; pl != NULL; pl = pl->next) {
      if (pl->val.type == V_PVL)
	tl = hpoly_insert(tl, hpoly3_decode(pl->val.u.v));
      else
	error("(trilist) syntax error");
    }
    v.type = V_HPOLYLIST;
    v.u.v = tl;
  }
  return v;
}


int max3_index(Real d1, Real d2, Real d3)
{
  if (d1 > d2)
    return (d1 > d3)? 1 : 3;
  else
    return (d2 > d3)? 2 : 3;
}
