#ifndef P_LIGHT_H
#define P_LIGHT_H

#define DIRECT_LIGHT_NSMPLS 3

#include "color.h"
#include "poly.h"
#include "ibl.h"


typedef struct PolyLight{
  struct PolyLight *next;
  Hpoly *p;
  Color c;
} PolyLight;

PolyLight *plight_head;

void init_plight_list( void );
void plight_free( PolyLight *head );
PolyLight *plight_alloc( Color c, Hpoly *p );
void plight_insert( PolyLight *pl );
Ray plight_sample( PolyLight *p, Color *c );
Real plight_area( PolyLight *p );
Inode *plight_intersect( PolyLight *p, Ray r, Color *c );
Val plight_parse(int pass, Pval *pl);

Ray poly3_sample( Hpoly *p );

#endif
