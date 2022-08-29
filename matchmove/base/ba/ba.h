#ifndef BA_H
#define BA_H

#include "calib.h"
#include <gsl/gsl_multifit_nlin.h>

#define LM_MAX_ITERATIONS 50 
#define LM_EPS 1e-2

#ifndef MAX
#define MAX(U,V) (U>V?U:V)
#endif


typedef struct BundleAdjustData{
  gsl_matrix *projs;
  int n_cameras;
  int n_points;
} BundleAdjustData;


/* Extrinsic parameter Bundle Adjust */
gsl_vector *ba_ext_param_alloc( int n_cameras, int n_points );
void ba_ext_exec( gsl_vector *xout, gsl_vector *xin, gsl_matrix *projs, gsl_matrix *k );
void ba_ext_get_rt( gsl_vector *r, gsl_vector *t, const gsl_vector *x, int n_points, int cam_index );
void ba_ext_get_camera( gsl_matrix *camera, const gsl_vector *x, int n_cams, int cam_index, gsl_matrix *k );
void ba_ext_set_camera( gsl_vector *x, gsl_vector *r, gsl_vector *t, int n_points, int cam_index );
double ba_ext_reproj_error( const gsl_vector *x, gsl_matrix *projs, int cam_index, int point_index );


/* ba.c */
void ba_get_proj( gsl_vector *prj, gsl_matrix *projs, int cam_index, int point_index );
void ba_set_proj( gsl_matrix *projs, gsl_vector *prj, int cam_index, int point_index );
void ba_get_point( gsl_vector *v, const gsl_vector *x, int point_index );
void ba_set_point( gsl_vector *x, gsl_vector *v, int point_index );
void ba_optimize( gsl_multifit_fdfsolver *s );
void ba_set_lm_max_iterations( int n );


/* Rotations */
void ba_axis_angle_to_r( gsl_matrix *r, gsl_vector *axis_angle );
void ba_r_to_axis_angle( gsl_vector *axis_angle, gsl_matrix *r );


#endif
