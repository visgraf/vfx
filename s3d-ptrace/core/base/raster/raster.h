#ifndef RASTER_H
#define RASTER_H

#include "geom.h"
#include "prim.h"
#include "poly.h"

typedef void Paint(Vector4 v,int n,int lv,Real lt,int rv,Real rt,Real st,void* d);

void scan_hpoly(Hpoly *p, Paint *pfun, void *pdata);
void scan_shpoly3(Hpoly *p, int n, Paint *pfun, void *pdata);
Vector4 seg_bilerp(Hpoly *p, int n, Real t, int lv, Real lt, int rv, Real rt);

void scan_prim(Prim *p, void (*paint)());

void subdiv_line(Vector3 p, Vector3 q, void (*paint)());

void subdiv_boxline(Box3d bb, Vector3 p, Vector3 q, void (*paint)());

#endif

