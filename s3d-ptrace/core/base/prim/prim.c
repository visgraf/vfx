#include "prim.h"


Prim *prim_instance(PrimFuncs *f)
{
  return f->instance(f);
}

void prim_destroy(Prim *p)
{
   (*p->f->destroy)(p);
}

Box3d prim_bbox(Prim *p)
{
  return (*p->f->bbox)(p);
}

int prim_classify(Prim *p, Vector3 q)
{
  return (*p->f->classify)(p, q);
}

Vector3 prim_point(Prim *p, Real u, Real v)
{
  return (*p->f->point)(p, u, v);
}

Vector3 prim_normal(Prim *p, Real u, Real v)
{
  return (*p->f->normal)(p, u, v);
}

Vector3 prim_gradient(Prim *p, Vector3 q)
{
  return (*p->f->gradient)(p, q);
}

Inode *prim_intersect(Prim *p, Ray r)
{
  return (*p->f->intersect)(p, r);
}

Prim *prim_transform(Prim *p, Matrix4 md, Matrix4 mi)
{
  return (*p->f->transform)(p, md, mi);
}

iMesh *prim_uv_decomp(Prim *p, Real level)
{
  return (*p->f->uv_decomp)(p, level);
}

Vector3 prim_texc(Prim *p, Real u, Real v)
{
  return (*p->f->texc)(p, u, v);
}

Vector3 prim_du(Prim *p, Real u, Real v)
{
  return (*p->f->du)(p, u, v);
}

Vector3 prim_dv(Prim *p, Real u, Real v)
{
  return (*p->f->dv)(p, u, v);
}

Matrix4 prim_local(Prim *p)
{
  return (*p->f->local)(p);
}

Vector3 prim_inv_map(Prim *p, Vector3 q)
{
  return (*p->f->inv_map)(p, q);
}

int prim_type(Prim *p)
{
  return (*p->f->type)(p);
}

void prim_write(Prim *p, FILE *fp)
{
   (*p->f->write)(p, fp);
}

void prim_draw(Prim *p, int axis)
{
   (*p->f->draw)(p, axis);
}

iMesh *prim_mesh(Prim *p, iMesh *m)
{
  int i;
  if (m->p == NULL)
    m->p = NEWARRAY(m->n, Vector3);
  if (m->nv == NULL)
    m->nv = NEWARRAY(m->n, Vector3);
  if (m->tx == NULL)
    m->tx = NEWARRAY(m->n, Vector3);
  if (m->du == NULL)
    m->du = NEWARRAY(m->n, Vector3);
  if (m->dv == NULL)
    m->dv = NEWARRAY(m->n, Vector3);
  for (i=0; i < m->n; i++) {
    Vector3 u = m->m[i];
    m->p[i] = prim_point(p, u.x, u.y);
    m->nv[i] = prim_normal(p, u.x, u.y);
    m->tx[i] = prim_texc(p, u.x, u.y);
    m->du[i] = prim_du(p, u.x, u.y);
    m->dv[i] = prim_dv(p, u.x, u.y);
  }
  return m;
}
