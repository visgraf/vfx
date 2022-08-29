#include "ba.h"

#define ANG_EPS 1e-6


void ba_axis_angle_to_r( gsl_matrix *r, gsl_vector *axis_angle )
{
 double a1, a2, a3, s, c, angle;
 gsl_vector *a;

 a = gsl_vector_alloc(3);

 angle =  gsl_blas_dnrm2( axis_angle );

 if( angle <= ANG_EPS )
    gsl_matrix_set_identity(r);
 else{
   s = sin(angle);
   c = cos(angle);
   gsl_vector_memcpy( a, axis_angle );
   gsl_vector_scale( a, 1./angle );

   a1 = gsl_vector_get( a, 0 );
   a2 = gsl_vector_get( a, 1 );
   a3 = gsl_vector_get( a, 2 );

   gsl_matrix_set( r, 0, 0, a1*a1 + c*( 1 - a1*a1 ) );
   gsl_matrix_set( r, 1, 0, a1*a2*( 1 - c ) + a3*s );
   gsl_matrix_set( r, 2, 0, a1*a3*( 1 - c ) - a2*s );

   gsl_matrix_set( r, 0, 1, a1*a2*( 1 - c ) - a3*s );
   gsl_matrix_set( r, 1, 1, a2*a2 + c*( 1 - a2*a2 ) );
   gsl_matrix_set( r, 2, 1, a2*a3*( 1 - c ) + a1*s );

   gsl_matrix_set( r, 0, 2, a1*a3*( 1 - c ) + a2*s );
   gsl_matrix_set( r, 1, 2, a2*a3*( 1 - c ) - a1*s );
   gsl_matrix_set( r, 2, 2, a3*a3 + c*( 1 - a3*a3 ) );
 }

 gsl_vector_free(a);
}


void ba_r_to_axis_angle( gsl_vector *axis_angle, gsl_matrix *r )
{
 double s_doub, c_doub, angle;
 gsl_vector *w, *s;
 gsl_matrix *m, *v;

 m = gsl_matrix_alloc(3,3);
 v = gsl_matrix_alloc(3,3);
 s = gsl_vector_alloc(3);
 w = gsl_vector_alloc(3);

 gsl_matrix_set_identity( m );
 gsl_matrix_scale( m, -1. );
 gsl_matrix_add( m, r );

 gsl_linalg_SV_decomp( m, v, s,w );
 gsl_matrix_get_col( axis_angle, v, 2 );
 gsl_vector_scale( axis_angle, 1/gsl_blas_dnrm2( axis_angle ) );

 s_doub = gsl_vector_get( axis_angle, 0 )*( gsl_matrix_get( r, 2,1 ) - gsl_matrix_get( r, 1, 2 )) +
          gsl_vector_get( axis_angle, 1 )*( gsl_matrix_get( r, 0,2 ) - gsl_matrix_get( r, 2, 0 )) +
          gsl_vector_get( axis_angle, 2 )*( gsl_matrix_get( r, 1,0 ) - gsl_matrix_get( r, 0, 1 ));

 c_doub = gsl_matrix_get( r, 0,0 ) + gsl_matrix_get( r, 1, 1 ) + gsl_matrix_get( r, 2, 2 ) - 1;
 angle = atan2( s_doub, c_doub );

 gsl_vector_scale( axis_angle, angle );

 gsl_matrix_free(m);
 gsl_matrix_free(v);
 gsl_vector_free(s);
 gsl_vector_free(w);
}

