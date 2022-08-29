#ifndef CAMERAS_H
#define CAMERAS_H

#include "defs.h"
#include "calib.h"

#define MAX_CAMERAS 3000

typedef struct Cameras{
  gsl_matrix *k;
  gsl_matrix *r[MAX_CAMERAS];
  gsl_vector *t[MAX_CAMERAS];
  int ncams;
} Cameras;


Cameras *cameras_alloc( gsl_matrix *k, gsl_matrix *r[MAX_CAMERAS], 
                        gsl_vector *t[MAX_CAMERAS], int ncams );
Cameras *cameras_read( gsl_matrix *k, FILE *f );
void cameras_free( Cameras *c );
void cameras_apply( Cameras *c, gsl_vector *p, gsl_vector *proj, int i );
void cameras_getP( gsl_matrix *p, Cameras *c, int i ); 

#endif
