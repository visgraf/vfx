#include "ransac.h"


void get_random_choose( int *samples, int nsamples, int n )
{
 int i, r, k = 0;
 while( k< nsamples ){
   r = (int)( n * uniform_random() + .5 );
   for( i = 0; (samples[i] != r) && (i<k); i++ );
   if( i==k ){
     samples[k] = r;
     k++;
   }
 }
}


void get_inliers( gsl_matrix *dst, gsl_matrix *src, int *inliers )
{
 int i, j, k = 0;

 for( i = 0; i < src->size1; i++ ){
   if( inliers[i] == INLIER ){
      for( j = 0; j < src->size2; j++ )
           gsl_matrix_set( dst, k, j, gsl_matrix_get( src, i, j ) );
      k++;
   }
 }
}


void get_samples( gsl_matrix *aux, gsl_matrix *m, int *samples, int n )
{
 int i, j;

 for( j=0; j < m->size2; j++ )
    for( i = 0; i < n; i++ )
       gsl_matrix_set( aux, i, j, gsl_matrix_get( m, samples[i], j ) );
}


