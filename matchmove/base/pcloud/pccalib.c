#include "pcloud.h"


static void reconstruct_point( gsl_vector *xout, PointCloud *pc, 
                               Cameras *cam, int nframes, int point );
static Real reproj_error( gsl_vector *x,  Cameras *cams, PointCloud *pc, 
                          int point, int nframes );
                               

gsl_matrix* pcloud_calib( PointCloud *pc, Cameras *cam, int nframes )
{
 gsl_vector *x, *xout;
 gsl_matrix *output;
 int point, npoints;
 
 npoints = pcloud_get_npoints(pc);
 
 output = gsl_matrix_alloc( npoints, 3 );
 x = gsl_vector_alloc( 3 );
 xout = gsl_vector_alloc( 3 );
 
 for( point = 0; point < npoints; point++ ){
    reconstruct_point( x, pc, cam, nframes, point );
    pcloud_nlin_optimize( xout, x, cam, pc, nframes, point );
    gsl_matrix_set_row( output, point, xout );
 }    
 
 gsl_vector_free(x);
 gsl_vector_free(xout);
 
 return output;
}


void reconstruct_point( gsl_vector *xout, PointCloud *pc, Cameras *cam,
                        int nframes, int point )
{
 int i, j;
 double error, min_reproj_error = MAX_FLOAT;
 gsl_vector *v1, *v2, *x;
 gsl_matrix *p1, *p2;
 
 p1 = gsl_matrix_alloc( 3, 4 );
 p2 = gsl_matrix_alloc( 3, 4 );
 v1 = gsl_vector_alloc( 2 );
 v2 = gsl_vector_alloc( 2 );
 x = gsl_vector_alloc( 3 );

 for( i = 0; i < nframes; i++ )
   for( j = i+1; j < nframes; j++ ){
      calib_pmatrix_make( p1, cam->k, cam->r[pc[i].frame], cam->t[pc[i].frame] );
      calib_pmatrix_make( p2, cam->k, cam->r[pc[j].frame], cam->t[pc[j].frame] );
      pcloud_get_point( v1, &pc[i], point );
      pcloud_get_point( v2, &pc[j], point );
      calib_get_3dpoint( p1, p2, v1, v2, x );  
      if( (error = reproj_error( x, cam, pc, point, nframes )) < min_reproj_error ){
         min_reproj_error = error; 
         gsl_vector_memcpy( xout, x );
      }
   }  
   
  gsl_matrix_free( p1 ); 
  gsl_matrix_free( p2 );
  gsl_vector_free( v1 );
  gsl_vector_free( v2 ); 
  gsl_vector_free( x ); 
}


Real reproj_error( gsl_vector *x,  Cameras *cams, PointCloud *pc, int point, int nframes )
{
 int j;
 Real error = 0;
 gsl_vector *x_proj, *p;
 
 x_proj = gsl_vector_alloc(2);
 p = gsl_vector_alloc(2);
 
 for( j = 0; j < nframes; j++ )
   if( pc[j].frame < cams->ncams ){
      cameras_apply( cams, x, x_proj, pc[j].frame );
      pcloud_get_point( p, &pc[j], point );
      gsl_vector_sub( x_proj, p );
      error += gsl_blas_dnrm2( x_proj );
   }
 
 gsl_vector_free( x_proj ); 
 gsl_vector_free( p ); 
 return error;
}
