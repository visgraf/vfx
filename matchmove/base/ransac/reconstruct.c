#include "ransac.h"

int ransac_reconstruct(  int *inliers,  gsl_matrix *points,
                         gsl_matrix *projs1, gsl_matrix *projs2, gsl_matrix *k,
                         int n_iters, double tol )
{
 int i, ninliers;
 gsl_matrix *f, *e, *p, *pid, *id;
 gsl_vector *zero, *x1, *x2, *x;

 f = gsl_matrix_alloc( 3, 3 );
 e = gsl_matrix_alloc( 3, 3 );
 p = gsl_matrix_alloc( 3, 4 );
 pid = gsl_matrix_alloc( 3, 4 );
 id = gsl_matrix_alloc( 3, 3 );
 zero = gsl_vector_alloc( 3 );
 x1 = gsl_vector_alloc( 2 );
 x2 = gsl_vector_alloc( 2 );
 x = gsl_vector_alloc( 3 );

 gsl_matrix_set_identity( id );
 gsl_vector_set_zero( zero );

 ninliers = ransac_fmatrix_make(  inliers, f, projs1 , projs2, n_iters, tol );
 if( ninliers > 8 ){
   calib_ematrix_make( e, f, k );
   for( i=0; inliers[i] == OUTLIER;  i++);
   gsl_matrix_get_row( x1, projs1, i );
   gsl_matrix_get_row( x2, projs2, i );
   calib_ematrix_get_P( k, e, x1, x2, p );
   calib_pmatrix_make( pid, k, id, zero );

   for( i = 0; i < projs1->size1; i++ ) {
      gsl_matrix_get_row( x1, projs1, i );
      gsl_matrix_get_row( x2, projs2, i );
      calib_get_3dpoint( pid, p, x1, x2, x  );
      gsl_matrix_set( points, i, 0, gsl_vector_get( x, 0 ) );
      gsl_matrix_set( points, i, 1, gsl_vector_get( x, 1 ) );
      gsl_matrix_set( points, i, 2, gsl_vector_get( x, 2 ) );
   }
 }  

 gsl_matrix_free( f );
 gsl_matrix_free( e );
 gsl_matrix_free( p );
 gsl_matrix_free( pid );
 gsl_matrix_free( id );
 gsl_vector_free( zero );
 gsl_vector_free( x1 );
 gsl_vector_free( x2 );
 gsl_vector_free( x );
 return  ninliers;
}
