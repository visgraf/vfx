#include <math.h>
#include "calib.h"


static void vector_cross( gsl_vector *r, gsl_vector *v1, gsl_vector *v2 );


void calib_camfactor( gsl_matrix *k, gsl_matrix *r, gsl_vector *t, gsl_matrix *p )
{
 int i;
 double lambda, v0, u0, s, f1, f2, t1, t2, t3;
 gsl_matrix *p_til;
 gsl_vector *a1, *a2, *a3, *aux, *r1;

 p_til = gsl_matrix_alloc( 3, 4 );
 a1 = gsl_vector_alloc(3);
 a2 = gsl_vector_alloc(3);
 a3 = gsl_vector_alloc(3);
 aux = gsl_vector_alloc(3);
 r1 = gsl_vector_alloc(3);

 lambda = sqrt( SQR(gsl_matrix_get( p, 2, 0 )) +
                SQR(gsl_matrix_get( p, 2, 1 )) +
                SQR(gsl_matrix_get( p, 2, 2 )) );

 gsl_matrix_memcpy( p_til, p );
 gsl_matrix_scale( p_til, 1/lambda );

 for( i = 0; i < 3; i++ ){
   gsl_vector_set( a1, i, gsl_matrix_get( p_til, 0, i ) );
   gsl_vector_set( a2, i, gsl_matrix_get( p_til, 1, i ) );
   gsl_vector_set( a3, i, gsl_matrix_get( p_til, 2, i ) );
 }

 gsl_matrix_set_row( r, 2, a3 );
 gsl_blas_ddot( a2, a3, &v0 );
 gsl_blas_ddot( a1, a3, &u0 );

 gsl_vector_memcpy( aux, a3 );
 gsl_vector_scale( aux, -v0 );
 gsl_vector_add( aux, a2 ); 
 f2 = gsl_blas_dnrm2( aux );
 gsl_vector_scale( aux, 1/f2 );
 gsl_matrix_set_row( r, 1, aux );
 gsl_blas_ddot( a1, aux, &s );
 vector_cross( r1 ,aux, a3 );
 gsl_matrix_set_row( r, 0, r1 );
 gsl_blas_ddot( a1, r1, &f1 );
 
 t3 = gsl_matrix_get( p_til, 2, 3 );
 t2 = (gsl_matrix_get( p_til, 1, 3 ) - v0*t3)/f2;
 t1 = (gsl_matrix_get( p_til, 0, 3) - u0*t3 - s*t2 )/f1;

 if( f1 < 0 ){
   t2 = -t2;
   t3 = -t3;
   f1 = -f1;
   gsl_vector_scale( a3, -1 );
   gsl_matrix_set_row( r, 2, a3 );
   gsl_matrix_get_row( aux, r, 1 );
   gsl_vector_scale( aux, -1 );
   gsl_matrix_set_row( r, 1, aux ); 
 }   
   
 gsl_vector_set( t, 0, t1 );
 gsl_vector_set( t, 1, t2 );
 gsl_vector_set( t, 2, t3 );
  
 gsl_matrix_set_identity(k);
 gsl_matrix_set( k, 0, 0, f1 );
 gsl_matrix_set( k, 1, 1, f2 );
 gsl_matrix_set( k, 0, 1, s );
 gsl_matrix_set( k, 0, 2, u0 );
 gsl_matrix_set( k, 1, 2, v0 ); 

 gsl_matrix_free( p_til );
 gsl_vector_free( a1 );
 gsl_vector_free( a2 );
 gsl_vector_free( a3 );
 gsl_vector_free( aux );
 gsl_vector_free( r1 );
}

void vector_cross( gsl_vector *r, gsl_vector *v1, gsl_vector *v2 )
{
 double v10, v11, v12,
        v20, v21, v22;
		
 v10 = gsl_vector_get( v1, 0 );
 v11 = gsl_vector_get( v1, 1 );
 v12 = gsl_vector_get( v1, 2 );
 v20 = gsl_vector_get( v2, 0 );
 v21 = gsl_vector_get( v2, 1 );
 v22 = gsl_vector_get( v2, 2 );

 gsl_vector_set( r, 0, v11*v22 - v12*v21 );
 gsl_vector_set( r, 1, v12*v20 - v10*v22 );
 gsl_vector_set( r, 2, v10*v21 - v11*v20 );
}

