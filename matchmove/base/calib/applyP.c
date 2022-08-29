#include "calib.h"

void calib_apply_P( gsl_matrix *p, gsl_vector *x, gsl_vector *x_proj )
{
 gsl_matrix *x_homog;
 gsl_matrix *proj_homog;

 x_homog = gsl_matrix_alloc( 4, 1 );
 proj_homog = gsl_matrix_alloc( 3, 1 );

 gsl_matrix_set( x_homog, 0, 0, gsl_vector_get(x,0) );
 gsl_matrix_set( x_homog, 1, 0, gsl_vector_get(x,1) );
 gsl_matrix_set( x_homog, 2, 0, gsl_vector_get(x,2) );
 gsl_matrix_set( x_homog, 3, 0, 1. );
 gsl_linalg_matmult( p, x_homog, proj_homog );
 gsl_vector_set( x_proj, 0, gsl_matrix_get( proj_homog, 0, 0 )/
                            gsl_matrix_get( proj_homog, 2, 0 ) );
 gsl_vector_set( x_proj, 1, gsl_matrix_get( proj_homog, 1, 0 )/
                            gsl_matrix_get( proj_homog, 2, 0 ) );

 gsl_matrix_free( x_homog );
 gsl_matrix_free( proj_homog );
}



