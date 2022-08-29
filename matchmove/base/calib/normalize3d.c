#include "calib.h"


void calib_get_3dnormalize_transform( gsl_matrix *h, gsl_matrix *s )
{
 int i, n;
 double d = 0, x0 = 0, y0 = 0, z0 = 0, scale;

 n = s->size1;
 for( i = 0; i < n; i++ ){
   x0 += gsl_matrix_get( s, i, 0);
   y0 += gsl_matrix_get( s, i, 1);
   z0 += gsl_matrix_get( s, i, 2);
 }
 x0 = x0/n;
 y0 = y0/n;
 z0 = z0/n;

 for( i = 0; i < n; i++ ){
   d += SQR(gsl_matrix_get(s, i, 0) - x0) +
        SQR(gsl_matrix_get(s, i, 1) - y0) +
        SQR(gsl_matrix_get(s, i, 2) - z0);
 }

 gsl_matrix_set_zero(h);
 scale = sqrt( 3*n/d );
 gsl_matrix_set( h, 0, 0, scale );
 gsl_matrix_set( h, 1, 1, scale );
 gsl_matrix_set( h, 2, 2, scale );
 gsl_matrix_set( h, 0, 3, -scale*x0 );
 gsl_matrix_set( h, 1, 3, -scale*y0 );
 gsl_matrix_set( h, 2, 3, -scale*z0 );
 gsl_matrix_set( h, 3, 3, 1. );
}

