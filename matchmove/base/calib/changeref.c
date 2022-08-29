#include "calib.h"


void calib_change_ref( gsl_matrix *ref_a1, gsl_matrix *ref_a2,
                       gsl_matrix *ref_b1, gsl_matrix *ref_b2  )
{
 int i,j;
 gsl_matrix *ra1, *ra2, *rb1, *rb2,
            *ta1, *ta2, *tb1, *tb2, *raux, *r, *t;

 r = gsl_matrix_alloc( 3, 3 );
 raux = gsl_matrix_alloc( 3, 3 );
 ra1 = gsl_matrix_alloc( 3, 3 );
 ra2 = gsl_matrix_alloc( 3, 3 );
 rb1 = gsl_matrix_alloc( 3, 3 );
 rb2 = gsl_matrix_alloc( 3, 3 );
 ta1 = gsl_matrix_alloc( 3, 1 );
 ta2 = gsl_matrix_alloc( 3, 1 );
 tb1 = gsl_matrix_alloc( 3, 1 );
 t = gsl_matrix_alloc( 3, 1 );
 
 for( i=0; i<3; i++ )
   for( j=0; j<3; j++ ){
     gsl_matrix_set( ra1, i, j, gsl_matrix_get( ref_a1, i, j ) );
     gsl_matrix_set( ra2, i, j, gsl_matrix_get( ref_a2, i, j ) );
     gsl_matrix_set( rb1, i, j, gsl_matrix_get( ref_b1, i, j ) );
   }

 for( i=0; i<3; i++ ){
   gsl_matrix_set( ta1, i, 0, gsl_matrix_get( ref_a1, i, 3 ) );
   gsl_matrix_set( ta2, i, 0, gsl_matrix_get( ref_a2, i, 3 ) );
   gsl_matrix_set( tb1, i, 0, gsl_matrix_get( ref_b1, i, 3 ) );
 }

 gsl_matrix_transpose( ra1 );
 gsl_linalg_matmult( rb1, ra1, raux );
 gsl_linalg_matmult( raux, ra2, r );
 gsl_matrix_sub( ta2, ta1 ); 
 gsl_linalg_matmult( raux, ta2, t );
 gsl_matrix_add( t, tb1 );

 for( i=0; i<3; i++ )
   for( j=0; j<3; j++ )
     gsl_matrix_set( ref_b2, i, j, gsl_matrix_get( r, i, j ) );

 for( i=0; i<3; i++ )
   gsl_matrix_set( ref_b2, i, 3, gsl_matrix_get( t, i, 0 ) );

 gsl_matrix_free(r);
 gsl_matrix_free(raux);
 gsl_matrix_free(ra1);
 gsl_matrix_free(ra2);
 gsl_matrix_free(rb1);
 gsl_matrix_free(rb2);
 gsl_matrix_free(ta1);
 gsl_matrix_free(ta2);
 gsl_matrix_free(tb1);
 gsl_matrix_free(t);
}
