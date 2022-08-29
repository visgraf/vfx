#include "sequence.h"

static int  reproj_inliers( int *out_inliers, int *in_inliers, int n_in_inliers,
                            SubSeqNode *s, gsl_matrix *k);
static int is_intrinsic_inlier( gsl_matrix *p, gsl_matrix *k, double tol );
static double matrix33_distancy( gsl_matrix *k, gsl_matrix *kref );
static void reconstruct( SubSeqNode *s, gsl_matrix *k );
static double projs_distancy( SubSeqNode *s, int frame1, int frame2 );


void subseq_dlt_calib( SubSeqNode *s, gsl_matrix *k )
{
 int i;
 gsl_vector *t;
 gsl_matrix  *k_real, *r, *p;

 t = gsl_vector_alloc( 3 );
 r = gsl_matrix_alloc( 3, 3 );
 k_real = gsl_matrix_alloc( 3, 3 );
 p = gsl_matrix_alloc( 3, 4 );

 reconstruct( s, k );
 for(  i = 0; i < s->nframes; i++ ){
   calib_singlecam_dlt(  p,  s->wpoints, WPROJS( s, i ) );
   calib_camfactor( k_real, r, t,  p );
   camera_set( &(s->cl[i]) , k, r, t );
 }

 gsl_vector_free( t );
 gsl_matrix_free( r );
 gsl_matrix_free( k_real );
 gsl_matrix_free( p );
}


void reconstruct( SubSeqNode *s, gsl_matrix *k )
{
  int i, j, frame1, frame2, ninliers, ransac_ninliers, max_ninliers = -1;
  int *inliers, *ransac_inliers, *max_inliers;
  gsl_matrix *max_points;
     
  max_points = gsl_matrix_alloc(  s->nfeatures ,3 );
  inliers = (int*)malloc( s->nfeatures*sizeof(int) );
  max_inliers = (int*)malloc( s->nfeatures*sizeof(int) );
  ransac_inliers = (int*)malloc( s->nfeatures*sizeof(int) );

  for( i = s->nframes - 1; i > 0; i-- ){
    for( j = 0; j < i; j++ ) {
      if( projs_distancy( s, i, j ) > MIN_MOV_DIST2 ){
       
        ransac_ninliers = ransac_reconstruct( ransac_inliers, s->points, PROJS( s,i ), PROJS( s,j ),
                          k, s->cc->ransac_iterations, s->cc->ransac_inliers_tol  );
       
        if( (ransac_ninliers > 5) && (ransac_ninliers > max_ninliers) ){
           ninliers = reproj_inliers( inliers, ransac_inliers, ransac_ninliers, s, k );

           if( ninliers > max_ninliers ){
             max_ninliers = ninliers;
	     memcpy( max_inliers, inliers, s->nfeatures * sizeof( int ) );
	     gsl_matrix_memcpy( max_points , s->points ); 
             frame1 = i;
             frame2 = j;
           }
        }
        printf( "(%i, %i)  r:%i nin:%i, max: ( %i, %i )%i\n", i, j, ransac_ninliers, ninliers, frame1, frame2, max_ninliers  );
     }
    }
 }

 gsl_matrix_memcpy( s->points, max_points ); 
 outliers_eliminate( s , max_inliers, max_ninliers );

 gsl_matrix_free( max_points );
 free( inliers );
 free( max_inliers );
 free( ransac_inliers );
}


int reproj_inliers( int *out_inliers, int *in_inliers, int in_ninliers, SubSeqNode *s, gsl_matrix *k )
{
 int i, *proj_inliers, out_ninliers;
 gsl_matrix *projs, *reprojs, *inline_points, *p;
 
 p = gsl_matrix_alloc( 3, 4 );
 projs = gsl_matrix_alloc( in_ninliers, 2 );
 reprojs = gsl_matrix_alloc( in_ninliers, 2 );
 inline_points =  gsl_matrix_alloc( in_ninliers, 3 );
 proj_inliers = (int*)malloc( in_ninliers*sizeof(int) );
 
 copy_points_inliers( inline_points, s->points, in_inliers );
 memcpy( out_inliers, in_inliers, s->nfeatures * sizeof( int ) );
 for( i = 0; i < s->nframes; i++ ) {
   copy_projs_inliers( projs, PROJS( s, i ), in_inliers );
   ransac_singlecam( proj_inliers, p, inline_points, projs, s->cc->ransac_iterations, s->cc->reproj_inliers_tol );
   merge_outliers( out_inliers, in_inliers, proj_inliers, s->nfeatures );
 }

 out_ninliers = get_ninliers( out_inliers, s->nfeatures );
 if( out_ninliers > 5 ){ 
   gsl_matrix_free( projs );
   gsl_matrix_free( inline_points );
   projs = gsl_matrix_alloc( out_ninliers, 2 );
   inline_points =  gsl_matrix_alloc( out_ninliers, 3 );
   copy_points_inliers( inline_points, s->points, out_inliers );
   for( i = 0; i < s->nframes; i++ ){
     copy_projs_inliers( projs, PROJS( s, i ), out_inliers );
     calib_singlecam_dlt( p, inline_points, projs );
     if( !is_intrinsic_inlier(p, k, INTRISIC_DIST_TOL) ){
       out_ninliers = -1;	
       break;
     }
   }
 }

 gsl_matrix_free( p );
 gsl_matrix_free( projs );
 gsl_matrix_free( reprojs );
 gsl_matrix_free( inline_points );
 free( proj_inliers );
 return out_ninliers;
}


int is_intrinsic_inlier( gsl_matrix *p, gsl_matrix *kref, double tol )
{
 double d;
 gsl_matrix *k, *r;
 gsl_vector *t;

 k = gsl_matrix_alloc( 3, 3 );
 r = gsl_matrix_alloc( 3, 3 );
 t = gsl_vector_alloc( 3 );
 
 calib_camfactor( k, r, t, p );
 d = matrix33_distancy( k, kref ); 
   
 gsl_matrix_free( k );
 gsl_matrix_free( r );
 gsl_vector_free( t );
 
 if( d < tol )
   return 1;
 else  
   return 0;
} 


#ifndef MAX
  #define MAX(U,V) (U>V?U:V)
#endif

double matrix33_distancy( gsl_matrix *k, gsl_matrix *kref )
{
 double e = 0;
 int i, j;
 
 for( i = 0; i < 3; i++ )
   for( j = 0; j < 3; j++ )
     e = MAX( e, fabs( gsl_matrix_get(k, i, j )- gsl_matrix_get( kref, i, j )) );
 
 return e;    
}


double projs_distancy( SubSeqNode *s, int frame1, int frame2 )
{
 gsl_vector *v1, *v2;
 int i, ninliers = 0;
 double d = 0;
 
 v1 = gsl_vector_alloc(2);
 v2 = gsl_vector_alloc(2);
 for( i = 0; i < s->nfeatures; i++ ){
   gsl_matrix_get_row( v1, PROJS(s,frame1), i );  
   gsl_matrix_get_row( v2, PROJS(s,frame2), i );
   gsl_vector_sub( v1, v2 );
   d += gsl_blas_dnrm2( v1 );
 }  

 gsl_vector_free( v1 );
 gsl_vector_free( v2 ); 
 return  d/s->nfeatures; 
}



