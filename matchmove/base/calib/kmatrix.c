#include "calib.h"


gsl_matrix *calib_kmatrix_alloc( double f1, double f2, double s, double u, double v )
{
 gsl_matrix *r;

 r = gsl_matrix_alloc( 3, 3 );
 gsl_matrix_set_zero(r);
 gsl_matrix_set( r, 0, 0, f1 );
 gsl_matrix_set( r, 0, 1, s );
 gsl_matrix_set( r, 0, 2, u );
 gsl_matrix_set( r, 1, 1, f2 );
 gsl_matrix_set( r, 1, 2, v );
 gsl_matrix_set( r, 2, 2, 1.);

 return r;
}



