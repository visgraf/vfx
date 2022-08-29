#include "ba.h"


gsl_vector* ba_ext_param_alloc( int n_cameras, int n_points )
{
 return gsl_vector_alloc( 3*n_points + 6*n_cameras );
}


void ba_ext_get_rt( gsl_vector *r, gsl_vector *t, const gsl_vector *x, int n_points, int cam_index )
{
 gsl_vector_set( r, 0, gsl_vector_get( x, 3*n_points + 6*cam_index ));
 gsl_vector_set( r, 1, gsl_vector_get( x, 3*n_points + 6*cam_index + 1 ));
 gsl_vector_set( r, 2, gsl_vector_get( x, 3*n_points + 6*cam_index + 2 ));
 gsl_vector_set( t, 0, gsl_vector_get( x, 3*n_points + 6*cam_index + 3));
 gsl_vector_set( t, 1, gsl_vector_get( x, 3*n_points + 6*cam_index + 4 ));
 gsl_vector_set( t, 2, gsl_vector_get( x, 3*n_points + 6*cam_index + 5 ));
}


void ba_ext_get_camera( gsl_matrix *camera, const gsl_vector *x, int n_points, int cam_index, gsl_matrix *k )
{
 gsl_vector *h, *t;
 gsl_matrix *r;

 h = gsl_vector_alloc(3);
 t = gsl_vector_alloc(3);
 r = gsl_matrix_alloc(3,3);

 ba_ext_get_rt( h, t, x, n_points, cam_index );
 ba_axis_angle_to_r( r, h );
 calib_pmatrix_make( camera, k, r, t );

 gsl_vector_free(h);
 gsl_vector_free(t);
 gsl_matrix_free(r);
}


void ba_ext_set_camera( gsl_vector *x, gsl_vector *r, gsl_vector *t, int n_points, int cam_index )
{
 gsl_vector_set( x, 3*n_points + 6*cam_index, gsl_vector_get( r, 0 ));
 gsl_vector_set( x, 3*n_points + 6*cam_index + 1, gsl_vector_get( r, 1 ));
 gsl_vector_set( x, 3*n_points + 6*cam_index + 2, gsl_vector_get( r, 2 ));
 gsl_vector_set( x, 3*n_points + 6*cam_index + 3, gsl_vector_get( t, 0 ));
 gsl_vector_set( x, 3*n_points + 6*cam_index + 4, gsl_vector_get( t, 1 ));
 gsl_vector_set( x, 3*n_points + 6*cam_index + 5, gsl_vector_get( t, 2 ));
}
