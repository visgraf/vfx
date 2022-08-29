#ifndef RELAX_POINTS_H
#define RELAX_POINTS_H

#include "homol_point.h"
#include "cameras.h"


void relax_points( gsl_vector *xout, gsl_vector *x, HomologPoint *hp, Cameras *c );
void relax_optimize( gsl_multifit_fdfsolver *s );

             
#endif
