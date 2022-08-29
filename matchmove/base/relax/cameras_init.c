#include "cameras_init.h"


int get_npoints( RelaxationContext *rc, int frame )
{
 int j, s = 0;

 for( j = 0; j < rc->npoints; j++ )
   if( hp_is_inlier( rc->hp[j], frame ) )
    s++;

 return s;
}



void relax_cameras( RelaxationContext *rc, double tol, int n_iters )
{
 int i,j, s, npoints, *inliers;
 gsl_matrix *projs, *points, *p, *k, *r;
 gsl_vector *t;
 
 p = gsl_matrix_alloc( 3, 4 );
 k = gsl_matrix_alloc( 3, 3 );
  
 for( i = 0; i < rc->c->ncams; i++ ){
   npoints = get_npoints( rc, i );
   points = gsl_matrix_alloc( npoints, 3 );
   projs = gsl_matrix_alloc( npoints, 2 ); 
   set_points( points, i, rc );
   set_projs( projs, i, rc );
   inliers = NEWTARRAY( npoints, int );
   ransac_singlecam( inliers, p, points, projs, n_iters, tol );
   r = gsl_matrix_alloc( 3, 3 ); 
   t = gsl_vector_alloc( 3 );
   calib_camfactor( k, r, t, p );
   rc->c->r[i] = r;
   rc->c->t[i] = t;
   
   printf( "K camera %i\n", i );
   gsl_matrix_fprintf( stdout, k, "%f" );
   printf( "\n" ); 
   
   gsl_matrix_free(projs);
   gsl_matrix_free(points);
   free( inliers );
 }
 
 gsl_matrix_free(p);
 gsl_matrix_free(k);
}


void  set_points( gsl_matrix *points, int frame, RelaxationContext *rc )
{
 int j, s = 0;
 gsl_vector *p;
 
 for( j = 0; j < rc->npoints; j++ )
    if( hp_is_inlier( rc->hp[j], frame ) ){
      p = rc->hp[j]->p;
      gsl_matrix_set( points, s, 0, gsl_vector_get(p,0) ); 
      gsl_matrix_set( points, s, 1, gsl_vector_get(p,1) );
      gsl_matrix_set( points, s, 2, gsl_vector_get(p,2) );
      s++;
    }
}


void set_projs( gsl_matrix *projs, int frame, RelaxationContext *rc )
{
 int j, s = 0;
 gsl_vector *p;
 
 p = gsl_vector_alloc(2);

  for( j = 0; j < rc->npoints; j++ )
    if( hp_is_inlier( rc->hp[j], frame ) ){
      hp_get_proj( p, rc->hp[j], frame );
      gsl_matrix_set( projs, s, 0, gsl_vector_get(p, 0) );
      gsl_matrix_set( projs, s, 1, gsl_vector_get(p, 1) ); 
      s++;
    }
    
 gsl_vector_free(p);
}


