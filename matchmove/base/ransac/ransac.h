#ifndef RANSAC_H
#define RANSAC_H

#include <defs.h>
#include <string.h>
#include "random.h"
#include "calib.h"

#define INLIER 1
#define OUTLIER 0

int ransac_fmatrix_make(  int *inliers, gsl_matrix *f, gsl_matrix *a, gsl_matrix *b,  int n_iters, double tol );
int ransac_reconstruct(  int *inliers,  gsl_matrix *points, gsl_matrix *projs1, gsl_matrix *projs2, gsl_matrix *k,  int n_iters, double tol );
int ransac_singlecam( int *inliers, gsl_matrix *p, gsl_matrix *q, gsl_matrix *x, int n_iters, double tol  );
int ransac_extrinsic_singlecam( int *inliers, gsl_matrix *p, gsl_matrix *q, gsl_matrix *x, int n_iters, double tol, gsl_matrix *k );

/* misc */
void get_random_choose( int *samples, int nsamples, int n );
void get_inliers( gsl_matrix *dst, gsl_matrix *src, int *inliers );
void get_samples( gsl_matrix *aux, gsl_matrix *m, int *samples, int n );

#endif
