#include "homol_point.h"
                                  
                                  
HomologPoint  *hp_alloc( int first_frame, int last_frame )  
{
 HomologPoint *hp = NEWSTRUCT(HomologPoint);
 
 hp->first_frame = first_frame;
 hp->last_frame = last_frame;
 hp->p = gsl_vector_alloc( 3 );
 hp->projs = gsl_matrix_alloc( last_frame - first_frame + 1, 2 ); 
 hp->is_inlier = NEWTARRAY( last_frame - first_frame + 1, Boolean );
 hp->ninliers = 0;
 
 return hp;
}     
       
       
void hp_free( HomologPoint *hp )
{
 gsl_vector_free( hp->p );
 gsl_matrix_free( hp->projs );
 free( hp );
}


Boolean hp_has_frame( HomologPoint *hp, int frame )
{
 if( (frame >= hp->first_frame ) && 
     (frame <= hp->last_frame ) ){
   return TRUE;   
 } 
 return FALSE;
}

Boolean hp_is_inlier( HomologPoint *hp, int frame )
{
 if( hp_has_frame( hp, frame ) &&  
     hp->is_inlier[frame - hp->first_frame] ){
   return TRUE;   
 }   
 return FALSE;
}


void hp_set_inlier(  HomologPoint *hp, int frame, Boolean b )
{
 hp->is_inlier[frame - hp->first_frame] = b;
}


void hp_set_proj( HomologPoint *hp, int frame, double x, double y )
{
 gsl_matrix_set( hp->projs, frame - hp->first_frame, 0, x );
 gsl_matrix_set( hp->projs, frame - hp->first_frame, 1, y );
}



void hp_get_proj( gsl_vector *v, HomologPoint *hp, int frame )
{
 gsl_vector_set( v, 0, gsl_matrix_get( hp->projs, frame - hp->first_frame, 0 ) );
 gsl_vector_set( v, 1, gsl_matrix_get( hp->projs, frame - hp->first_frame, 1 ) );
}


void hp_reconstruct_points( HomologPoint *hp, Cameras *c, double tol_error )
{
 double error;
 int i, j, ninliers, max_ninliers = 0;
 gsl_matrix *pi, *pj;
 gsl_vector *pt1, *pt2, *point;
 Boolean inliers[MAX_FRAMES_PER_FEATURE];
  
 pi = gsl_matrix_alloc( 3, 4 );
 pj = gsl_matrix_alloc( 3, 4 );
 pt1 = gsl_vector_alloc( 2 );
 pt2 = gsl_vector_alloc( 2 );
 point = gsl_vector_alloc( 3 );

 for( i = hp->first_frame; i <= hp->last_frame; i++ ){
    for( j = i+1; j <= hp->last_frame; j++ ){
      cameras_getP(pi, c, i);
      cameras_getP(pj, c, j); 
      
      gsl_vector_set( pt1, 0, gsl_matrix_get( hp->projs, i - hp->first_frame, 0 ) );
      gsl_vector_set( pt1, 1, gsl_matrix_get( hp->projs, i - hp->first_frame, 1 ) );
      gsl_vector_set( pt2, 0, gsl_matrix_get( hp->projs, j - hp->first_frame, 0 ) );
      gsl_vector_set( pt2, 1, gsl_matrix_get( hp->projs, j - hp->first_frame, 1 ) );
      
      calib_get_3dpoint( pi, pj, pt1, pt2, point ); 
      ninliers = hp_ninliers( hp, point, inliers, c, tol_error );
      if( ninliers > max_ninliers ){
         gsl_vector_memcpy( hp->p, point );
         max_ninliers = hp->ninliers = ninliers;
         adjust_inliers( hp, inliers );
      }
    }
 }
  
 gsl_matrix_free(pi);
 gsl_matrix_free(pj); 
 gsl_vector_free(pt1);
 gsl_vector_free(pt2);
 gsl_vector_free(point);
}


int hp_ninliers( HomologPoint *hp, gsl_vector *point, Boolean *inliers,
                 Cameras *c, double tol_error )
{
 double error;
 int i, ninliers = 0;
 gsl_vector *v, *p_proj;

 v = gsl_vector_alloc(2);
 p_proj = gsl_vector_alloc(2);
 
 for( i = hp->first_frame; i <= hp->last_frame; i++ ){
      cameras_apply( c, point, p_proj, i );
      gsl_vector_set( v, 0, gsl_matrix_get( hp->projs, i - hp->first_frame, 0 ));
      gsl_vector_set( v, 1, gsl_matrix_get( hp->projs, i - hp->first_frame, 1 ));
      gsl_vector_sub( v, p_proj );
      error = gsl_blas_dnrm2(v);
      if( error < tol_error ){
         ninliers++;
         hp_set_inlier( hp, i, TRUE );
      }
      else
         hp_set_inlier( hp, i, FALSE );
 }
 
 gsl_vector_free(v);
 gsl_vector_free(p_proj);
 return ninliers;
}


void adjust_inliers( HomologPoint *hp, Boolean *inliers )
{
 int i;

 for( i = 0; i < (hp->last_frame - hp->first_frame + 1); i++ )
      hp->is_inlier[i] = inliers[i];
}


double hp_reproj_error( HomologPoint *hp, int i, Cameras *c )
{
 double error = 0;
 gsl_vector *v, *p_proj;

 v = gsl_vector_alloc(2);
 p_proj = gsl_vector_alloc(2);

 cameras_apply( c, hp->p, p_proj, i );
 gsl_vector_set( v, 0, gsl_matrix_get( hp->projs, i - hp->first_frame, 0 ));
 gsl_vector_set( v, 1, gsl_matrix_get( hp->projs, i - hp->first_frame, 1 ));
 gsl_vector_sub( v, p_proj );
 error = gsl_blas_dnrm2(v);
 
 gsl_vector_free(v);
 gsl_vector_free(p_proj);
 
 return error;
}







