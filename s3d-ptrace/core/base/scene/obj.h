#ifndef OBJ_H
#define OBJ_H

#include <string.h>
#include "geom.h"
#include "csg.h"
#include "lang.h"


typedef struct GLvbuf {
  int           nvert;
  unsigned int  buffer;
  int           nind;
  unsigned int  indices;
  unsigned char ntex;
} GLvbuf;


typedef struct Shape {
  struct GLvbuf   *vb;
  union {
    struct Prim    *prim;
    struct CsgNode *tcsg;
    struct Hpoly   *hpols;
    struct iMesh   *mesh;
  } u;
} Shape;


typedef struct Object {
  struct Object   *next;
  struct Material *mat;
  int   id, type;
  union {
    struct Shape   *sh;
    struct Group   *gr;
  } u;
} Object;

#define shape_(OBJ) ((OBJ)->u.sh)
#define sh_vb_(OBJ) ((OBJ)->u.sh->vb)
#define prim_(OBJ) ((OBJ)->u.sh->u.prim)
#define mesh_(OBJ) ((OBJ)->u.sh->u.mesh)
#define hpols_(OBJ) ((OBJ)->u.sh->u.hpols)
#define tcsg_(OBJ) ((OBJ)->u.sh->u.tcsg)

#define group_(OBJ) ((OBJ)->u.gr)
#define xform_(OBJ) ((OBJ)->u.gr->t)
#define children_(OBJ) ((OBJ)->u.gr->children)

Object *obj_new(int type, void *v);
void obj_free(Object *o);
Object *obj_insert(Object *olist, Object *o);
Object *obj_list_insert(Object *olist, Object *l);
void obj_list_free(Object *ol);
int obj_list_size(Object *ol);
int obj_list_is_graph(Object *ol);

Val obj_parse(int c, Pval *pl);

void obj_transform_shape(Object *o, Matrix4 m, Matrix4 mi);
Shape *shape_new(int type, void *v);
void shape_free(int type, Shape *s);

#endif

