#ifndef POLY_H
#define POLY_H

#include "sdltypes.h"
#include "geom.h"
#include "ray.h"
#include "lang.h"

#define IDX2(u,v,w) ((u) + ((v) * (w)))

typedef struct Hpoly {
  struct Hpoly *next;
  int          n;
  Vector4     *v;
} Hpoly;

typedef struct iTri {
  struct iTri  *next;
  struct iMesh *m;
  int          v[3];
} iTri;

typedef struct iMesh {
  int        n;
  Vector3   *p, *nv, *tx, *du, *dv, *m;
  iTri      *t;
} iMesh;

Hpoly *hpoly_alloc(int n);
int    hpoly_copy(Hpoly *d, Hpoly *s);
Hpoly *hpoly_transform(Hpoly *d, Matrix4 m);
Hpoly *hpoly_homoxform(Hpoly *p, Matrix4 m);
Hpoly *hpoly_homo(Hpoly *p);
Vector3 hpoly_normal(Hpoly *p);
Vector3 hpoly_centr(Hpoly *p);
Hpoly *hpoly_insert(Hpoly *pl, Hpoly *p);
int hpoly_copy(Hpoly *s, Hpoly *d);
Inode *hpoly_intersect(Hpoly *p, Vector4 plane, Ray r);

int hplist_lenght(Hpoly *l);
Hpoly *hplist_alloc(int n, int m);
void hplist_free(Hpoly *l);
Hpoly *hplist_transform(Hpoly *l, Matrix4 m);

Hpoly *hpoly3_make(Vector4 v0, Vector4  v1, Vector4  v2);
Real hpoly3_ray_inter(Hpoly *p, Vector4 plane, Ray r);
Vector4 hpoly3_plane(Hpoly *p);
Real hpoly3_area(Hpoly *p);
Hpoly *hpoly3_set(Hpoly *p, Vector4 v0, Vector4  v1, Vector4  v2);
void hpoly3_write(Hpoly *p, FILE* fp);

iTri *itri_make(int v0, int v1, int v2, iMesh *mesh);
Hpoly *itri_to_hpoly3(Hpoly *p, iTri *t);
iTri *itri_insert(iTri *pl, iTri *p);
void itrilist_free(iTri *l);

iMesh *imesh_alloc(int n, Vector3 *pts, iTri *tlist);
void imesh_free(iMesh *m);
void imesh_transform(iMesh *m, Matrix4 t);
void imesh_compute_normals(iMesh *m);

Vector3 imesh_point(iMesh *m, int k);
Vector3 imesh_mpoint(iMesh *m, int k);
Vector3 imesh_normal(iMesh *m, int k);
Vector3 imesh_texc(iMesh *m, int k);
Vector3 imesh_du(iMesh *m, int k);
Vector3 imesh_dv(iMesh *m, int k);

Vector3 *vertlist_decode(Pval *plist, int *n);
iTri *itri_decode(Pval *pl, iMesh *mesh);
iTri *itrilist_decode(Pval *plist, iMesh *mesh);

Val imesh_parse(int pass, Pval *plist);
void imesh_write(iMesh *mesh, FILE* fp);

Val htrilist_parse(int pass, Pval *plist);
void htrilist_write(Hpoly *tlist, FILE* fp);

int max3_index(Real d1, Real d2, Real d3);

#endif
