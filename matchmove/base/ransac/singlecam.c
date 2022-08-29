#include "ransac.h"

static double reproj_error( gsl_matrix *proj1,  gsl_matrix *proj2, int index );
static void  apply_P( gsl_matrix *p, gsl_matrix *points, gsl_matrix *reprojs );


int ransac_singlecam( int *inliers, gsl_matrix *p, gsl_matrix *q, gsl_matrix *x, int n_iters, double tol  )
{
 int n, j, samples[6], *aux_inliers;
 int c, best_consensus = 0;
 gsl_matrix *qaux, *xaux, *q_inliers, *x_inliers;

 if( x->size1 > 5 ) 
   n = x->size1;
 else
   return 0;
 

 qaux = gsl_matrix_alloc(6,3);
 xaux = gsl_matrix_alloc(6,2);
 aux_inliers = (int*)malloc( n*sizeof(int) );


 for( j=0; j < n_iters; j++ ){
   get_random_choose( samples, 6, n-1 );
   get_samples( xaux, x, samples, 6 );
   get_samples( qaux, q, samples, 6 );

   calib_singlecam_dlt( p, qaux, xaux );

   if((c = consensus( aux_inliers, p, q, x, tol )) > best_consensus ) {
      best_consensus = c;
      memcpy( inliers, aux_inliers, n*sizeof(int));
   }

 }

 if( best_consensus > 5 ){
   q_inliers = gsl_matrix_alloc( best_consensus, 3 );
   x_inliers = gsl_matrix_alloc( best_consensus, 2 );
   get_inliers( q_inliers, q, inliers );
   get_inliers( x_inliers, x, inliers );
   calib_singlecam_dlt( p, q_inliers, x_inliers );
   gsl_matrix_free( q_inliers );
   gsl_matrix_free( x_inliers );
 }

 gsl_matrix_free( qaux );
 gsl_matrix_free( xaux );
 free( aux_inliers );
 return best_consensus;
}


int consensus( int *inliers, gsl_matrix *p, gsl_matrix *q, gsl_matrix *x, double tol )
{
  int i, ninliers = 0;
  gsl_matrix *x_proj;

  x_proj = gsl_matrix_alloc( x->size1, 2 );
  apply_P( x_proj, p, q );
  for( i = 0; i < x->size1; i++ ){
     if(  reproj_error( x, x_proj, i )   < tol ){
          ninliers++;
          inliers[i] = INLIER;
     }
     else
          inliers[i] = OUTLIER;
 }

 gsl_matrix_free( x_proj );
 return ninliers;
}


void  apply_P( gsl_matrix *reprojs, gsl_matrix *p, gsl_matrix *points )
{
 int i;
 gsl_vector *x, *x_proj;

 x =  gsl_vector_alloc( 3 );
 x_proj = gsl_vector_alloc( 2 );

 for( i = 0; i < points->size1; i++ ){
   gsl_matrix_get_row( x, points, i );
   calib_apply_P( p,  x,  x_proj );
   gsl_matrix_set_row( reprojs, i, x_proj );
 }

 gsl_vector_free( x );
 gsl_vector_free( x_proj );
}


double reproj_error( gsl_matrix *proj1,  gsl_matrix *proj2, int index )
{
 int i;
 double d = 0;
 gsl_vector *v1, *v2;

 v1 = gsl_vector_alloc( 2 );
 v2 = gsl_vector_alloc( 2 );
 gsl_matrix_get_row( v1, proj1, index );
 gsl_matrix_get_row( v2, proj2, index );
 gsl_vector_sub( v1, v2 );
 d = gsl_blas_dnrm2( v1 );

 gsl_vector_free( v1 );
 gsl_vector_free( v2 );
 return d;
}
