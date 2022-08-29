#ifndef ZSORT_H
#define ZSORT_H

#include "lang.h"
#include "poly.h"
#include "prim.h"
#include "obj.h"
#include "list.h"

typedef struct Zdatum {
  Real          zmax;
  Hpoly          *l;
  Object       *o;
} Zdatum;

Item *z_select(List *p);
List *z_sort(List *p);

#endif
