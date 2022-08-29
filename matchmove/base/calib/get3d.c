#include "calib.h"


#define CREATE_ROW( R, P, X, J )  \
 gsl_matrix_get_row( aux1, P, 2 ); \
 gsl_vector_scale( aux1, gsl_vector_get( X, J ) ); \
 gsl_matrix_get_row( aux2, P, J ); \
 gsl_vector_sub( aux1, aux2 ); \
 gsl_matrix_set_row( a, R, aux1 );

void calib_get_3dpoint( gsl_matrix *p1, gsl_matrix *p2, gsl_vector *x1, gsl_vector *x2, gsl_vector *x )
{
 gsl_vector *aux1, *aux2, *s, *w, *r;
 gsl_matrix *a, *v;

 a = gsl_matrix_alloc( 4, 4 );
 v = gsl_matrix_alloc( 4, 4 );
 aux1 = gsl_vector_alloc( 4 );
 aux2 = gsl_vector_alloc( 4 );
 s = gsl_vector_alloc( 4 );
 w = gsl_vector_alloc( 4 );
 r = gsl_vector_alloc( 4 );

 CREATE_ROW( 0, p1, x1, 0 );
 CREATE_ROW( 1, p1, x1, 1 );
 CREATE_ROW( 2, p2, x2, 0 );
 CREATE_ROW( 3, p2, x2, 1 );

 gsl_linalg_SV_decomp( a, v, s, w );
 gsl_matrix_get_col( r, v, 3 );

 gsl_vector_set( x, 0, gsl_vector_get(r,0)/gsl_vector_get(r,3));
 gsl_vector_set( x, 1, gsl_vector_get(r,1)/gsl_vector_get(r,3));
 gsl_vector_set( x, 2, gsl_vector_get(r,2)/gsl_vector_get(r,3));

 gsl_matrix_free( a );
 gsl_matrix_free( v );
 gsl_vector_free( aux1 );
 gsl_vector_free( aux2 );
 gsl_vector_free( s );
 gsl_vector_free( w );
 gsl_vector_free( r );
}

