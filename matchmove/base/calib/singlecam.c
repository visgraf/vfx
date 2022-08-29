
#include "calib.h"


void calib_singlecam_dlt( gsl_matrix *p, gsl_matrix *q, gsl_matrix *x )
{
 gsl_matrix *h, *hinv, *homog3d, *paux, *paux_homog3d, *qn, *xn;
  
 h = gsl_matrix_alloc( 3, 3 );
 hinv = gsl_matrix_alloc( 3, 3 );
 homog3d = gsl_matrix_alloc( 4, 4 );
 paux = gsl_matrix_alloc( 3, 4 );
 paux_homog3d = gsl_matrix_alloc( 3, 4 );
 qn = gsl_matrix_alloc( q->size1, 3 );
 xn = gsl_matrix_alloc( x->size1, 2 );
  
 calib_get_normalize_transform( h, hinv, x );
 calib_get_3dnormalize_transform( homog3d, q );
 calib_apply_homog( xn, h, x );
 calib_apply_3dhomog( qn, homog3d, q );
 calib_singlecam( paux, qn, xn );
 gsl_linalg_matmult( paux, homog3d, paux_homog3d );
 gsl_linalg_matmult( hinv, paux_homog3d, p );

 gsl_matrix_free( h );
 gsl_matrix_free( hinv );
 gsl_matrix_free( homog3d );
 gsl_matrix_free( paux );
 gsl_matrix_free( paux_homog3d );
 gsl_matrix_free( qn );
 gsl_matrix_free( xn );
}


void calib_singlecam( gsl_matrix *p, gsl_matrix *q, gsl_matrix *x )
{
 int n, i, j;
 double xi, yi, qXi, qYi, qZi;
 gsl_matrix *u, *v;
 gsl_vector *w, *s, *aux;

 n = x->size1;
 u = gsl_matrix_alloc( 2*n, 12 );
 v = gsl_matrix_alloc( 12, 12 );
 s = gsl_vector_alloc( 12 );
 aux = gsl_vector_alloc( 12 );
 w = gsl_vector_alloc( 12 );
 
 gsl_matrix_set_zero(u);
 for( i = 0; i < n; i++ ){
   xi = gsl_matrix_get( x, i, 0 );
   yi = gsl_matrix_get( x, i, 1 );
   qXi = gsl_matrix_get( q, i, 0 );
   qYi = gsl_matrix_get( q, i, 1 );
   qZi = gsl_matrix_get( q, i, 2 );
   gsl_matrix_set( u, 2*i, 0, qXi );
   gsl_matrix_set( u, 2*i, 1, qYi );
   gsl_matrix_set( u, 2*i, 2, qZi );
   gsl_matrix_set( u, 2*i, 3, 1 );
   gsl_matrix_set( u, 2*i + 1, 4, qXi );
   gsl_matrix_set( u, 2*i + 1, 5, qYi );
   gsl_matrix_set( u, 2*i + 1, 6, qZi );
   gsl_matrix_set( u, 2*i + 1, 7, 1 );
   gsl_matrix_set( u, 2*i, 8, -xi*qXi );
   gsl_matrix_set( u, 2*i, 9, -xi*qYi ); 
   gsl_matrix_set( u, 2*i, 10, -xi*qZi );
   gsl_matrix_set( u, 2*i, 11, -xi );
   gsl_matrix_set( u, 2*i + 1, 8, -yi*qXi );
   gsl_matrix_set( u, 2*i + 1, 9, -yi*qYi ); 
   gsl_matrix_set( u, 2*i + 1, 10, -yi*qZi );
   gsl_matrix_set( u, 2*i + 1, 11, -yi );
 }   
 gsl_linalg_SV_decomp( u, v, s, w );
 gsl_matrix_get_col( aux, v, 11 );
   
 for( j=0; j < 3; j++ )
   for( i=0; i < 4; i++ )
     gsl_matrix_set( p, j, i, gsl_vector_get( aux, 4*j + i ) );  
         
 gsl_matrix_free(u);
 gsl_matrix_free(v);
 gsl_vector_free(s);
 gsl_vector_free(aux);
 gsl_vector_free(w);
}

 
