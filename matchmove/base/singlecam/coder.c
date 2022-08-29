#include "singlecam.h"

static double kvalues[5];

/* free_kparams controls the intrinsic parameters that must be optimized by the Levenberg-Marquadt */
static int free_kparams[] = {1,0,1,1,1};


gsl_vector* singlecam_param_alloc( void )
{
 int i, n = 0;

 for( i=0; i<5; i++ )
   if( free_kparams[i] != 0 )
     n++;

 return  gsl_vector_alloc( 6 + n );
}


void singlecam_get_krt( gsl_matrix *k, gsl_vector *r, gsl_vector *t, const gsl_vector *x )
{
 int i, j, p = 0, q = 0;

 gsl_matrix_set_identity( k );

 gsl_vector_set( r, 0, gsl_vector_get( x, 0 ) );
 gsl_vector_set( r, 1, gsl_vector_get( x, 1 ) );
 gsl_vector_set( r, 2, gsl_vector_get( x, 2 ) );
 gsl_vector_set( t, 0, gsl_vector_get( x, 3 ) );
 gsl_vector_set( t, 1, gsl_vector_get( x, 4 ) );
 gsl_vector_set( t, 2, gsl_vector_get( x, 5 ) );

 for( i=0; i < 2; i++ )
   for( j = i; j < 3; j++ ){
     if( free_kparams[p] == 1 ){
       gsl_matrix_set( k, i, j, gsl_vector_get( x, p+6 ) ); 
       p++;
     }
     else
       gsl_matrix_set( k, i, j, kvalues[q++] );
   } 
}


void singlecam_set_krt( gsl_vector *x, gsl_matrix *k, gsl_vector *r, gsl_vector *t )
{
 int i, j, p = 0, q = 0;

 gsl_vector_set( x, 0, gsl_vector_get( r, 0 ) );
 gsl_vector_set( x, 1, gsl_vector_get( r, 1 ) );
 gsl_vector_set( x, 2, gsl_vector_get( r, 2 ) );
 gsl_vector_set( x, 3, gsl_vector_get( t, 0 ) );
 gsl_vector_set( x, 4, gsl_vector_get( t, 1 ) );
 gsl_vector_set( x, 5, gsl_vector_get( t, 2 ) );

 
 for( i=0; i < 2; i++ )
   for( j = i; j < 3; j++ ){
     if( free_kparams[p] == 1 ){
       gsl_vector_set( x, p+6, gsl_matrix_get( k, i, j ) );
       p++;
     }
     else
       kvalues[q++] = gsl_matrix_get( k, i, j ); 
   } 
}


void singlecam_get_camera( gsl_matrix *camera, const gsl_vector *x )
{
 gsl_vector *h, *t;
 gsl_matrix *k, *r;

 h = gsl_vector_alloc(3);
 t = gsl_vector_alloc(3);
 k = gsl_matrix_alloc(3,3);
 r = gsl_matrix_alloc(3,3);

 singlecam_get_krt( k, h, t, x );
 ba_axis_angle_to_r( r, h );
 calib_pmatrix_make( camera, k, r, t );

 gsl_vector_free(h);
 gsl_vector_free(t);
 gsl_matrix_free(k);
 gsl_matrix_free(r);
}

