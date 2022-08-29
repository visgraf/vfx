#include "ransac.h"

static int consensus( int *inliers, gsl_matrix *f, gsl_matrix *a, gsl_matrix *b, double tol );
static void get_homologus( gsl_matrix *x1, gsl_matrix *x2, gsl_matrix *a, gsl_matrix *b, int i );


int ransac_fmatrix_make( int *inliers, gsl_matrix *f, gsl_matrix *a, gsl_matrix *b, int n_iters, double tol )
{
 int n, j, samples[9], *aux_inliers;
 int c, best_consensus = 0;
 gsl_matrix *aux1, *aux2, *a_inliers, *b_inliers;
 
 n = a->size1;
 aux1 = gsl_matrix_alloc(9,2);
 aux2 = gsl_matrix_alloc(9,2);
 aux_inliers = (int*)malloc( n*sizeof(int) );
 
 for( j=0; j < n_iters; j++ ){
   get_random_choose( samples, 9, n-1 );
   get_samples( aux1, a, samples, 9 );
   get_samples( aux2, b, samples, 9 );
   calib_fmatrix_dlt( f, aux1, aux2 );
   if((c = consensus( aux_inliers, f, a, b, tol )) > best_consensus ) {
      best_consensus = c;
      memcpy( inliers, aux_inliers, n*sizeof(int));
   }
 }
 
 if( best_consensus > 8 ){
   a_inliers = gsl_matrix_alloc( best_consensus, 2 ); 
   b_inliers = gsl_matrix_alloc( best_consensus, 2 );
   get_inliers( a_inliers, a, inliers );
   get_inliers( b_inliers, b, inliers );
   calib_fmatrix_dlt( f, a_inliers, b_inliers );
   gsl_matrix_free( a_inliers );
   gsl_matrix_free( b_inliers ); 
 } 

 gsl_matrix_free( aux1 );
 gsl_matrix_free( aux2 );
 free( aux_inliers );
 return best_consensus;
}


int consensus( int *inliers, gsl_matrix *f, gsl_matrix *a, gsl_matrix *b, double tol )
{
  int i, ninliers = 0;
  gsl_matrix *x1, *x2, *u, *error;

  x1 = gsl_matrix_alloc(3,1);
  x2 = gsl_matrix_alloc(1,3);
  u = gsl_matrix_alloc(3,1);
  error = gsl_matrix_alloc(1,1);

 for( i = 0; i < a->size1; i++ ){
     get_homologus( x1, x2, a, b, i  );
     gsl_linalg_matmult( f, x1, u );
     gsl_linalg_matmult( x2, u, error );
     if(  fabs( gsl_matrix_get( error, 0, 0 ))  < tol ){
          ninliers++;
          inliers[i] = INLIER;
     }
     else
          inliers[i] = OUTLIER;
 }

 gsl_matrix_free( x1 );
 gsl_matrix_free( x2 );
 gsl_matrix_free( u );
 gsl_matrix_free( error );
 return ninliers;
}


void get_homologus( gsl_matrix *x1, gsl_matrix *x2, gsl_matrix *a, gsl_matrix *b, int i )
{
 gsl_matrix_set( x1, 0, 0, gsl_matrix_get(  a, i, 0 ) );
 gsl_matrix_set( x1, 1, 0, gsl_matrix_get(  a, i, 1 ) );
 gsl_matrix_set( x1, 2, 0, 1. );

 gsl_matrix_set( x2, 0, 0, gsl_matrix_get(  b, i, 0 ) );
 gsl_matrix_set( x2, 0, 1, gsl_matrix_get(  b, i, 1 ) );
 gsl_matrix_set( x2, 0, 2, 1. );
}



