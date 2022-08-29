#include <stdio.h>
#include "lang.h"
#include "gener.h"


iMesh *gener_affine(int n, Vector3 *g, int m, Matrix4 *h)
{
  int u, v;
  Vector3 *pts = NEWARRAY(n*m, Vector3);
  iTri *tl = NULL;
  iMesh *mesh = NULL;

  for (u = 0; u < n; u++) {
    for (v = 0; v < m; v++) {
      pts[IDX2(u,v,n)] = v3_m4mult(g[u], h[v]);
    }
  }
  mesh = imesh_alloc(n*m, pts, NULL);
  for (u = 0; u < n; u++) {
    for (v = 0; v < m; v++) {
      if (u == 0 || v == 0)
	continue;
      tl = itri_insert(tl, itri_make(IDX2((u-1),(v-1),n),  IDX2(u,(v-1),n), IDX2((u-1),v,n), mesh));
      tl = itri_insert(tl, itri_make(IDX2((u-1),v,n), IDX2(u,(v-1),n), IDX2(u,v,n), mesh));
    }
  }
  mesh->t = tl;
  return mesh;
}


Matrix4 m4_compxform(int k, char *t, Real **p, int j)
{
  int i;
  Matrix4 m = m4_ident();

  for (i = 0; i < k; i++) {
    switch (t[i]) {
    case G_TX: m = m4_m4prod(m4_translate(p[i][j], 0, 0), m); break;
    case G_TY: m = m4_m4prod(m4_translate(0, p[i][j], 0), m); break;
    case G_TZ: m = m4_m4prod(m4_translate(0, 0, p[i][j]), m); break;
    case G_RX: m = m4_m4prod(m4_rotate('x', p[i][j]), m); break;
    case G_RY: m = m4_m4prod(m4_rotate('y', p[i][j]), m); break;
    case G_RZ: m = m4_m4prod(m4_rotate('z', p[i][j]), m); break;
    case G_SX: m = m4_m4prod(m4_scale(p[i][j], 1, 1), m); break;
    case G_SY: m = m4_m4prod(m4_scale(1, p[i][j], 1), m); break;
    case G_SZ: m = m4_m4prod(m4_scale(1, 1, p[i][j]), m); break;
    default: break;
    }
  }
  return m;
}


Matrix4 *affine_group(int l, int m, char *t, Real **p)
{
  int v;
  Matrix4 *h = NEWARRAY(m, Matrix4);
  for (v = 0; v < m; v++)
    h[v] = m4_compxform(l, t, p, v);
  return h;
}
