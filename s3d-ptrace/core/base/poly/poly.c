#include <string.h>
#include "poly.h"


iTri *itri_make(int v0, int  v1, int  v2, iMesh *mesh)
{
  iTri *f = NEWSTRUCT(iTri);
  f->v[0] = v0;  f->v[1] = v1;  f->v[2] = v2; 
  f->m = mesh;
  return f;
}

Hpoly *itri_to_hpoly3(Hpoly *p, iTri *t)
{
  p->n = 3;
  p->v[0] = v4_v3conv(imesh_point(t->m, t->v[0]));
  p->v[1] = v4_v3conv(imesh_point(t->m, t->v[1]));
  p->v[2] = v4_v3conv(imesh_point(t->m, t->v[2]));
  return p;
}

iTri *itri_insert(iTri *pl, iTri *p)
{
  p->next = pl;
  return p;
}

void itrilist_free(iTri *l)
{
  iTri *q, *p = l;
  while (p != NULL) {
    q = p;
    p = p->next;
    efree(q);
  }
}

iMesh *imesh_alloc(int n, Vector3 *pts, iTri *tlist)
{
  iMesh *m = NEWSTRUCT(iMesh);
  m->n = n;
  m->p = pts;
  m->t = tlist;
  m->nv = m->tx = m->du = m->dv = m->m = NULL;
  return m;

}

void imesh_free(iMesh *m)
{
  itrilist_free(m->t);
  efree(m->p);
  efree(m->nv);
  efree(m->tx);
  efree(m->du);
  efree(m->dv);
  efree(m->m);
  efree(m);
}

void imesh_transform(iMesh *m, Matrix4 t)
{
  int i;
  for (i=0; i<m->n; i++)
    m->p[i] = v3_m4mult(m->p[i], t);
}

void imesh_compute_normals(iMesh *m)
{
  int i; iTri *t; 
  Hpoly *p = hpoly_alloc(3);

  if (m->nv == NULL) 
    m->nv = NEWARRAY(m->n, Vector3);

  for (i=0; i<m->n; i++) 
    m->nv[i] = v3_make(0,0,0);
  for (t = m->t; t != NULL; t = t->next) {
    Vector3 n = hpoly_normal(itri_to_hpoly3(p, t));
    for (i = 0; i < 3; i++) 
      m->nv[t->v[i]] = v3_add(m->nv[t->v[i]], n);
  }
  for (i=0; i<m->n; i++) 
    m->nv[i] = v3_unit(m->nv[i]);

  hplist_free(p);
}

Vector3 imesh_point(iMesh *m, int k)
{
  return m->p[k];
}

Vector3 imesh_mpoint(iMesh *m, int k)
{
  return m->m[k];
}

Vector3 imesh_normal(iMesh *m, int k)
{
  return m->nv[k];
}

Vector3 imesh_texc(iMesh *m, int k)
{
  return m->tx[k];
}

Vector3 imesh_du(iMesh *m, int k)
{
  return m->du[k];
}

Vector3 imesh_dv(iMesh *m, int k)
{
  return m->dv[k];
}

Vector3 *vertlist_decode(Pval *plist, int *n)
{
  Pval *pl;
  int k;
  Vector3 *pts = NULL;

  for (pl = plist, k = 0; pl !=NULL; pl = pl->next, k++)
    ;
  if (k != 0)
    pts = NEWARRAY(k, Vector3);
  for (pl = plist, k = 0; pl !=NULL; pl = pl->next, k++)
    if (pl->val.type == V_PVL)
      pts[k] = pvl_to_v3(pl->val.u.v);
  *n = k;
  return pts;
}

iTri *itri_decode(Pval *pl, iMesh *m)
{
  double v[3];
  pvl_to_array(pl, v, 3);
  return itri_make((int)v[0], (int)v[1], (int)v[2], m);
}

iTri *itrilist_decode(Pval *plist, iMesh *m)
{
  Pval *pl;
  iTri *tl = NULL;

  for (pl = plist; pl != NULL; pl = pl->next) {
    if (pl->val.type == V_PVL)
      tl = itri_insert(tl, itri_decode(pl->val.u.v, m));
    else
      error("(trilist) syntax error");
  }
  return tl;
}

Val imesh_parse(int pass, Pval *pl)
{
  Val v = {V_NULL, {0}};

  if (pass == T_EXEC) {
    Pval *p;
    int n = -1, na = -1;
    Vector3 *pts = NULL,  *normals = NULL, *texc = NULL, *du = NULL, *dv = NULL;
    iMesh *m = NULL;

    for (p = pl; p !=NULL; p = p->next) {
      if (p->name == NULL) {
      	error("(mesh) syntax error");
      } else if (strcmp(p->name, "points") == 0 && p->val.type == V_PVL) {
	pts = vertlist_decode(p->val.u.v, &n);
      } else if (strcmp(p->name, "normals") == 0 && p->val.type == V_PVL) {
	normals = vertlist_decode(p->val.u.v, &na);
      } else if (strcmp(p->name, "texcoord") == 0 && p->val.type == V_PVL) {
	texc = vertlist_decode(p->val.u.v, &na);
      } else if (strcmp(p->name, "tangents") == 0 && p->val.type == V_PVL) {
	du = vertlist_decode(p->val.u.v, &na);
      } else if (strcmp(p->name, "bitangents") == 0 && p->val.type == V_PVL) {
	dv = vertlist_decode(p->val.u.v, &na);
      } else if (strcmp(p->name, "faces") == 0 && p->val.type == V_PVL && pts != NULL) {
	if (n == -1) 
	  error("(mesh) syntax error - 1");
	m = imesh_alloc(n, pts, NULL);
        m->t = itrilist_decode(p->val.u.v, m);
	m->nv = normals;
	m->tx = texc;
	m->du = du;
	m->dv = dv;
      } else error("(mesh) syntax error"); 
    }
    if (pts == NULL || m == NULL || m->t == NULL)
       error("(mesh) syntax error");
    v.type = V_TRIMESH;
    v.u.v = m;
  }
  return v;
}

void imesh_write(iMesh *mesh, FILE* fp)
{
  int i;
  iTri *t = mesh->t;
  
  fprintf(fp, "mesh {\n");
  fprintf(fp, "\t points = {\n");
  for (i = 0; i < mesh->n; i++)
    fprintf(fp, "\t\t { %g, %g, %g }, \n", mesh->p[i].x, mesh->p[i].y, mesh->p[i].z);
  fprintf(fp, "\t }, \n");
  fprintf(fp, "\t faces = {\n");
  while (t != NULL) {
    fprintf(fp, "\t\t { %d, %d, %d }", t->v[0], t->v[1], t->v[2]);
    if ((t = t->next) != NULL)
      fprintf(fp, ",\n");
  }
  fprintf(fp, "\n\t }, \n");
  fprintf(fp, "}\n");
}


