#include "sequence.h"


void merge_outliers( int *out_inliers, int *inliers1, int *inliers2, int nfeatures )
{
 int j, k = 0;
 
 for( k = 0, j = 0; j < nfeatures ; j++ ){
   if( inliers1[j] == INLIER ){
     if( inliers2[k] == OUTLIER )
         out_inliers[j] = OUTLIER;	
     k++;
   }
 }
}


int get_ninliers( int *inliers, int n )
{
 int i, ninliers = 0;

 for( i = 0; i < n; i++ )
   if( inliers[i] == INLIER )
     ninliers++;

 return  ninliers;
}


void copy_projs_inliers( gsl_matrix *dst, gsl_matrix *src, int *inliers )
{
 int i,  k=0;

 for( i = 0; i < src->size1; i++ )
   if( inliers[i] == INLIER ){
     gsl_matrix_set( dst, k, 0, gsl_matrix_get( src, i , 0 ) );
     gsl_matrix_set( dst, k, 1, gsl_matrix_get( src, i , 1 ) );
     k++;
   }
}


void copy_points_inliers( gsl_matrix *dst, gsl_matrix *src, int *inliers )
{
 int i,  k=0;

 for( i = 0; i < src->size1; i++ )
   if( inliers[i] == INLIER ){
     gsl_matrix_set( dst, k, 0, gsl_matrix_get( src, i , 0 ) );
     gsl_matrix_set( dst, k, 1, gsl_matrix_get( src, i , 1 ) );
     gsl_matrix_set( dst, k, 2, gsl_matrix_get( src, i , 2 ) );
     k++;
   }
}


void  outliers_eliminate( SubSeqNode *s , int *inliers, int ninliers )
{
 int i, k = 0;
 gsl_matrix *projs, *points;

 for( i = 0; i < s->nframes; i++ ) {
   projs = gsl_matrix_alloc( ninliers, 2 );
   copy_projs_inliers( projs, PROJS( s, i ), inliers );
   if( WPROJS(s,i) != NULL )
     gsl_matrix_free( WPROJS(s,i) );
   WPROJS( s, i ) = projs;
 }

for( i = 0; i < s->nfeatures; i++ ){
   while( s->inliers[k] == OUTLIER )
     s->winliers[k++] = OUTLIER;
   if( inliers[i] == INLIER )
     s->winliers[k] = INLIER;
   else
     s->winliers[k] = OUTLIER;
   k++;
 }

 s->wnfeatures = ninliers;
 points =  gsl_matrix_alloc( ninliers, 3 );
 copy_points_inliers( points, s->points, inliers );
 if( s->wpoints != NULL )
    gsl_matrix_free( s->wpoints );
 s->wpoints = points;
}


 
