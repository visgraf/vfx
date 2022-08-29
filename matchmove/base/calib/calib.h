#ifndef CALIB_H
#define CALIB_H

#include "defs.h"
#include <math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>


/* Constructors */
gsl_matrix *calib_kmatrix_alloc( double f1, double f2, double s, double u, double v );
void calib_fmatrix_make( gsl_matrix *f, gsl_matrix *a, gsl_matrix *b );
void calib_pmatrix_make( gsl_matrix *p, gsl_matrix *k, gsl_matrix *r, gsl_vector *t );
void calib_ematrix_make( gsl_matrix *e, gsl_matrix *f, gsl_matrix *k );


/* Camera Estimation Functions */
void calib_ematrix_get_all_rt( gsl_matrix *e, gsl_matrix *r1, gsl_matrix *r2, gsl_vector *t );
void calib_ematrix_get_RT( gsl_matrix *k, gsl_matrix *e, gsl_vector *x1, gsl_vector *x2, gsl_matrix *r, gsl_vector *tr );
void calib_ematrix_get_P( gsl_matrix *k, gsl_matrix *e, gsl_vector *x1, gsl_vector *x2, gsl_matrix *P );
void calib_fmatrix_dlt( gsl_matrix *f, gsl_matrix *b, gsl_matrix *a );
void calib_singlecam( gsl_matrix *p, gsl_matrix *q, gsl_matrix *x );
void calib_singlecam_dlt( gsl_matrix *p, gsl_matrix *q, gsl_matrix *x );


/* Reconstruction  Functions */
void calib_get_3dpoint( gsl_matrix *p1, gsl_matrix *p2, gsl_vector *x1, gsl_vector *x2, gsl_vector *x );
void calib_dlt_reconstruct(  gsl_matrix *points, gsl_matrix *projs1, gsl_matrix *projs2,  gsl_matrix *k  );


/* Misc */
void calib_rt_transform( gsl_vector *xcam, gsl_matrix *r, gsl_vector *t, gsl_vector *x );
void calib_fmatrix_enforce( gsl_matrix *r, gsl_matrix *f );
void calib_get_normalize_transform( gsl_matrix *h,  gsl_matrix *hinv, gsl_matrix *s );
void calib_get_3dnormalize_transform( gsl_matrix *h, gsl_matrix *s );
void calib_fmatrix_singular_enforce( gsl_matrix *r, gsl_matrix *f );
void calib_apply_homog( gsl_matrix *r, gsl_matrix *h, gsl_matrix *s );
void calib_apply_3dhomog( gsl_matrix *r, gsl_matrix *h, gsl_matrix *s );
void calib_apply_P( gsl_matrix *p, gsl_vector *x, gsl_vector *x_proj );
void calib_change_ref( gsl_matrix *ref_a1, gsl_matrix *ref_a2, gsl_matrix *ref_b1, gsl_matrix *ref_b2 );
void calib_camfactor( gsl_matrix *k, gsl_matrix *r, gsl_vector *t, gsl_matrix *p );

#endif


