#ifndef HIER_H
#define HIER_H

#include "lang.h"
#include "sdltypes.h"
#include "prim.h"
#include "csg.h"
#include "poly.h"
#include "scene.h"
#include "stack.h"

typedef void (*Gfun)(Object *o, Stack4 *st, void *d);
typedef void (*Sfun)(Object *o, Stack4 *st, Scene *s);

#define XFORM_AFFINE 0
#define XFORM_PARAM 1

#define RX 0
#define RY 1
#define RZ 2
#define TX 3
#define TY 4
#define TZ 5
#define SX 6
#define SY 7
#define SZ 8

typedef void (*Pfun)(double *p, int n, void *info);

typedef struct Transform {
  int type;
  double p[9];
  Matrix4 m, mi;
  Pfun p_cb;
  void *info;
} Transform;

typedef struct Group {
  struct Transform *t;
  struct Object  *children;
} Group; 


Val xaffine_parse(int pass, Pval *p);
Val xparam_parse(int pass, Pval *p);
void xform_push();
void xform_pop();
Matrix4 xform_ctm();
Matrix4 xform_ctmi();
Transform *xform_new(int type);
void xform_free(Transform *t);
Transform *xform_setmat(Transform *t, Matrix4 m, Matrix4 mi);
Transform *xform_setpar(Transform *t, double *p);
Transform *xform_updatepar(Transform *t);
Transform *xform_applypar_m(Transform *t);
Transform *xform_applypar_mi(Transform *t);
void xform_reset(Transform *t);
Matrix4 xform_m(Transform *t);
Matrix4 xform_mi(Transform *t);

Val group_parse(int pass, Pval *pl);
Group *group_new(Object *ol, Transform *t);
void group_free(Group *gr);

Val graph_parse(int pass, Pval *pl);
void graph_apply(Object *objs, Gfun, Sfun, void *data);
Object *graph_transform(Object *ol);
Object *graph_flatten(Object *ol);

#endif
