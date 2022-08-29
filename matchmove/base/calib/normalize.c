#include "calib.h"


#define SQRT2 1.414213

void calib_get_normalize_transform( gsl_matrix *h, gsl_matrix *hinv, gsl_matrix *s )
{
 int i, n;
 double d = 0, x0 = 0, y0 = 0, scale;

 n = s->size1;
 for( i = 0; i < n; i++ ){
   x0 += gsl_matrix_get( s, i, 0);
   y0 += gsl_matrix_get( s, i, 1);
 }
 x0 = x0/n;
 y0 = y0/n;

 for( i = 0; i < n; i++ ){
   d += SQR(gsl_matrix_get(s, i, 0) - x0) +
        SQR(gsl_matrix_get(s, i, 1) - y0);
 }

 gsl_matrix_set_zero(h);
 scale = sqrt( 2*n/d );
 gsl_matrix_set( h, 0, 0, scale );
 gsl_matrix_set( h, 1, 1, scale );
 gsl_matrix_set( h, 0, 2, -scale*x0 );
 gsl_matrix_set( h, 1, 2, -scale*y0 );
 gsl_matrix_set( h, 2, 2, 1. );

 if( hinv != NULL ){
   gsl_matrix_set_zero(hinv);
   gsl_matrix_set( hinv, 0, 0, 1/scale );
   gsl_matrix_set( hinv, 1, 1, 1/scale );
   gsl_matrix_set( hinv, 0, 2, x0 );
   gsl_matrix_set( hinv, 1, 2, y0 );
   gsl_matrix_set( hinv, 2, 2, 1. );
 }
}
