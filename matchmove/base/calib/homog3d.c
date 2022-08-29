#include "calib.h"

void calib_apply_3dhomog( gsl_matrix *r, gsl_matrix *h, gsl_matrix *s )
{
 int i;
 gsl_matrix *v1, *v2;

 v1 = gsl_matrix_alloc( 4, 1 );
 v2 = gsl_matrix_alloc( 4, 1 );
 for( i = 0; i < s->size1; i++ ){
   gsl_matrix_set( v1, 0, 0, gsl_matrix_get( s, i, 0 ) );
   gsl_matrix_set( v1, 1, 0, gsl_matrix_get( s, i, 1 ) );
   gsl_matrix_set( v1, 2, 0, gsl_matrix_get( s, i, 2 ) );
   gsl_matrix_set( v1, 3, 0, 1. );
   gsl_linalg_matmult( h, v1, v2 );
   gsl_matrix_set( r, i, 0, gsl_matrix_get(v2, 0, 0)/gsl_matrix_get(v2, 3, 0));
   gsl_matrix_set( r, i, 1, gsl_matrix_get(v2, 1, 0)/gsl_matrix_get(v2, 3, 0));
   gsl_matrix_set( r, i, 2, gsl_matrix_get(v2, 2, 0)/gsl_matrix_get(v2, 3, 0));
 }

 gsl_matrix_free(v1);
 gsl_matrix_free(v2);
}

