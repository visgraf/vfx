#include "sequence.h"


static int is_inlier( SubSeqNode *s, gsl_vector *x, int index, double tol );


void subseq_recalibrate( SubSeqNode *s, double inlier_tol )
{
 int i,j,k, ninliers = 0;
 int *inliers;
 gsl_vector *x, *x1, *x2;
 gsl_matrix *p, *p1, *p2;

 inliers = NEWTARRAY( s->nfeatures, int );
 x = gsl_vector_alloc(3);
 x1 = gsl_vector_alloc(2);
 x2 = gsl_vector_alloc(2);
 p = gsl_matrix_alloc(3,4);
 p1 = gsl_matrix_alloc(3,4);
 p2 = gsl_matrix_alloc(3,4); 

 printf( "Recalibrating...\nInlier tol = %f pixel\n", inlier_tol );
 for( k = 0; k < s->nfeatures; k++ ){
   inliers[k] = OUTLIER;
   for( i = 0; i < s->nframes; i++ )
     for( j = 0; j < i; j++ ){ 
       calib_pmatrix_make( p1 , s->cl[i].k, s->cl[i].r, s->cl[i].t );
       calib_pmatrix_make( p2 , s->cl[j].k, s->cl[j].r, s->cl[j].t );
       gsl_matrix_get_row( x1, PROJS( s, i ), k );
       gsl_matrix_get_row( x2, PROJS( s, j ), k );
       calib_get_3dpoint( p1, p2, x1, x2, x );
       if( is_inlier( s, x, k, inlier_tol ) && (inliers[k] != INLIER) ){
         gsl_matrix_set_row( s->points, k, x );
         inliers[k] = INLIER;
         ninliers++;
         i = j = s->nframes;
       }  
      }
 }
 outliers_eliminate( s, inliers, ninliers );
 printf( "Number of inliers: %i\n", ninliers );

 free( inliers );
 gsl_vector_free( x );
 gsl_vector_free( x1 );
 gsl_vector_free( x2 );
 gsl_matrix_free( p );
 gsl_matrix_free( p1 );
 gsl_matrix_free( p2 );
}


int is_inlier( SubSeqNode *s, gsl_vector *x, int index, double tol )
{
 int i, n = 0;
 gsl_vector *x_proj, *x_real;
 gsl_matrix *p;

 x_proj = gsl_vector_alloc(2);
 x_real = gsl_vector_alloc(2);
 p = gsl_matrix_alloc(3, 4); 

 for( i = 0; i < s->nframes; i++ ){
    calib_pmatrix_make( p , s->cl[i].k, s->cl[i].r, s->cl[i].t );
    calib_apply_P( p, x, x_proj );
    gsl_matrix_get_row( x_real, PROJS( s, i ), index );
    gsl_vector_sub( x_proj, x_real );
    if( gsl_blas_dnrm2( x_proj ) < tol ) 
      n++;
 }

 gsl_vector_free( x_proj );
 gsl_vector_free( x_real );
 gsl_matrix_free( p );

 if( n == s->nframes )
   return TRUE;
 else 
   return FALSE;
}

