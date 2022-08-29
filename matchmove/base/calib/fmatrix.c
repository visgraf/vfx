#include "calib.h"


void calib_fmatrix_dlt( gsl_matrix *f, gsl_matrix *b, gsl_matrix *a )
{
 gsl_matrix *h1, *h2, *b2, *a2, *f1, *f1_h1, *f1_sing, *f1_sing_h1;

 h1 = gsl_matrix_alloc( 3, 3 );
 h2 = gsl_matrix_alloc( 3, 3 );
 b2 = gsl_matrix_alloc( b->size1, 2 );
 a2 = gsl_matrix_alloc( a->size1, 2 );
 f1 = gsl_matrix_alloc( 3, 3 );
 f1_sing = gsl_matrix_alloc( 3, 3 );
 f1_h1 = gsl_matrix_alloc( 3, 3 );
 f1_sing_h1 = gsl_matrix_alloc( 3, 3 );

 calib_get_normalize_transform( h1, NULL, b );
 calib_get_normalize_transform( h2, NULL, a );
 calib_apply_homog( b2, h1, b );
 calib_apply_homog( a2, h2, a );
 
 calib_fmatrix_make( f1, b2, a2 ); 
 calib_fmatrix_singular_enforce( f1_sing, f1 );
 gsl_linalg_matmult( f1_sing, h1, f1_sing_h1 );
 gsl_matrix_transpose( h2 );
 gsl_linalg_matmult( h2, f1_sing_h1, f );

 gsl_matrix_free( h1 );
 gsl_matrix_free( h2 );
 gsl_matrix_free( b2 );
 gsl_matrix_free( a2 );
 gsl_matrix_free( f1 );
 gsl_matrix_free( f1_h1 );
 gsl_matrix_free( f1_sing );
 gsl_matrix_free( f1_sing_h1 );
}


/* 8-points algorithm */
void calib_fmatrix_make( gsl_matrix *f ,gsl_matrix *b, gsl_matrix *a )
{
 int i, n;
 gsl_matrix *u, *v;
 gsl_vector *s, *x, *w;

 n = b->size1;
 u = gsl_matrix_alloc( n, 9 );
 v = gsl_matrix_alloc( 9, 9 );
 s = gsl_vector_alloc( 9 );
 x = gsl_vector_alloc( 9 );
 w = gsl_vector_alloc( 9 );

 for( i=0; i<n; i++ ){
   gsl_matrix_set( u, i, 0, gsl_matrix_get(a,i,0)*gsl_matrix_get(b,i,0) );
   gsl_matrix_set( u, i, 1, gsl_matrix_get(a,i,0)*gsl_matrix_get(b,i,1) );
   gsl_matrix_set( u, i, 2, gsl_matrix_get(a,i,0) );
   gsl_matrix_set( u, i, 3, gsl_matrix_get(a,i,1)*gsl_matrix_get(b,i,0) );
   gsl_matrix_set( u, i, 4, gsl_matrix_get(a,i,1)*gsl_matrix_get(b,i,1) );
   gsl_matrix_set( u, i, 5, gsl_matrix_get(a,i,1) );
   gsl_matrix_set( u, i, 6, gsl_matrix_get(b,i,0) );
   gsl_matrix_set( u, i, 7, gsl_matrix_get(b,i,1) );
   gsl_matrix_set( u, i, 8, 1. );
 }

 gsl_linalg_SV_decomp( u, v, s, w );
 gsl_matrix_get_col( x, v, 8);

 for( i=0; i<9; i++ )
   gsl_matrix_set( f, i/3 , i%3 , gsl_vector_get( x,i ));

 gsl_matrix_free( u );
 gsl_matrix_free( v );
 gsl_vector_free( s );
 gsl_vector_free( x );
 gsl_vector_free( w );
}


/* singular enforcement */
void calib_fmatrix_singular_enforce( gsl_matrix *r, gsl_matrix *f )
{
 gsl_vector *s, *w;
 gsl_matrix *u, *v, *sm, *usm;

 u = gsl_matrix_alloc(3,3);
 v = gsl_matrix_alloc(3,3);
 s = gsl_vector_alloc(3);
 sm = gsl_matrix_alloc(3,3);
 usm = gsl_matrix_alloc(3,3);
 w = gsl_vector_alloc(3);

 gsl_matrix_memcpy( u, f );
 gsl_linalg_SV_decomp( u, v, s, w );
 gsl_matrix_set_zero( sm );
 gsl_matrix_set( sm, 0, 0, gsl_vector_get( s, 0 ) );
 gsl_matrix_set( sm, 1, 1, gsl_vector_get( s, 1 ) );
 gsl_matrix_set( sm, 2, 2, 0. );

 gsl_linalg_matmult( u , sm , usm );
 gsl_matrix_transpose( v );
 gsl_linalg_matmult( usm, v, r );

 gsl_matrix_free(u);
 gsl_matrix_free(v);
 gsl_vector_free(s);
 gsl_matrix_free(sm);
 gsl_matrix_free(usm);
 gsl_vector_free(w);
}
