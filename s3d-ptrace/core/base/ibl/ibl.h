#ifndef IBL_H
#define IBL_H


#include "color.h"
#include "hdr.h"
#include "lang.h"
#include "shade.h"
#include "poly.h"
#include "obj.h"
#include <string.h>

#define REAL_RANDOM   ((double)rand()/RAND_MAX)

typedef struct HDRDome{
  Vector3 u;
  Vector3 v;
  Vector3 n;
  HDRImage *img;
}HDRDome;

typedef struct PolyShadow{
  struct PolyShadow *next;
  Hpoly *p;
  Material *m;
  HDRImage *img;
}PolyShadow;

extern HDRDome *hdr_dome;

/* HDR Dome Functions */

void hdrdome_init( void );
HDRDome *hdrdome_alloc( Vector3 orig, Vector3 south,
                        HDRImage *img );
void hdrdome_free( HDRDome *d );
Color hdrdome_value( HDRDome *d, Vector3 v );
Val hdrdome_parse(int pass, Pval *pl);


/* Poly Shadow Functions */

PolyShadow *plshadow_alloc( Material *m, Hpoly *p, HDRImage *img );
void plshadow_free( PolyShadow *ps );
Val plshadow_parse(int pass, Pval *plist);
Object *plshadow_to_obj( PolyShadow *ps );


#endif
