#ifndef SINGLECAM_H
#define SINGLECAM_H 

#include "ba.h"

typedef struct SingleCamData{
  gsl_matrix *points;
  gsl_matrix *projs;
}SingleCamData;


void singlecam_nlin_calib( gsl_vector *xout, gsl_vector *x,  gsl_matrix *points, gsl_matrix *projs );

/* Encoder e Decoder Functions */
gsl_vector* singlecam_param_alloc( void );
void singlecam_get_krt( gsl_matrix *k, gsl_vector *r, gsl_vector *t, const gsl_vector *x );
void singlecam_set_krt( gsl_vector *x, gsl_matrix *k, gsl_vector *r, gsl_vector *t );
void singlecam_get_camera( gsl_matrix *camera, const gsl_vector *x );
void singlecam_get_point( gsl_vector *v , const gsl_vector *x, int point_index );
void singlecam_set_point( gsl_vector *x, gsl_vector *v, int point_index );

#endif
