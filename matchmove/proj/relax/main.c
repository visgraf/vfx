#include "rcontext.h"
#include "ba.h"
#include "relax_points.h"
#include "relax_cameras.h"

#define NRELAX 9
#define NCYCLES 10    /* Number of complete relax until changing tol */
#define FIRST_RANSAC_TOL 10.
#define TOL_STEP 1.


static void relax( RelaxationContext *rc, Cameras *cam, int niters,
            double first_tol, double tol_step );
static void optimize( RelaxationContext *rc, Cameras *cam );
static void refine_inliers( HomologPoint *hp, Cameras *c , double tol );


void main( int argc, char **argv )
{
 RelaxationContext *rc;
 HomologPoint *hp;
 Cameras *cam;
 int i, nframes;
 gsl_matrix *k;
 int m[200];
 
 k = gsl_matrix_alloc(3,3);

 FILE *kf = fopen( argv[1], "r" );
 FILE *cam_f = fopen( argv[3], "r" );
 FILE *fout = fopen( argv[4], "w" );
  
 gsl_matrix_fscanf( kf, k );
 cam = cameras_read( k, cam_f );
 rc = rc_alloc( argv[2], cam, 10. );
   
 relax( rc, cam, NRELAX, FIRST_RANSAC_TOL, TOL_STEP );
 
 for( i = 0; i < cam->ncams; i++ ){
   fprintf( fout, "Frame %i\n", i );
   gsl_matrix_fprintf( fout, rc->c->r[i], "%f" );
   fprintf( fout, "\n" );
   gsl_vector_fprintf( fout, rc->c->t[i], "%f" );
   fprintf( fout, "\n" );
 }
 
 fclose( fout );
 fclose( kf );
 fclose( cam_f );
 gsl_matrix_free(k);
}


void relax( RelaxationContext *rc, Cameras *cam, int niters, 
            double first_tol, double tol_step )
{
  int i, j, total_ninliers = 0;
  double tol = first_tol;
  
  for( j = 0; j < niters; j++ ){
     for( i = 0; i < NCYCLES; i++ ){
        optimize( rc, cam );
     }
     printf( "MAX REPROJ ERROR: %lf\n", tol );
     
     for( i=0; i < rc->npoints; i++ ){
       refine_inliers( rc->hp[i], cam, tol );
       printf( "%i: [%i,%i], %i ninliers = %i\n", i, rc->hp[i]->first_frame, 
                      rc->hp[i]->last_frame,  
                      rc->hp[i]->last_frame - rc->hp[i]->first_frame + 1, 
                      rc->hp[i]->ninliers );  
       total_ninliers += rc->hp[i]->ninliers;
     }
     printf( "Total NINLIERS = %i\n", total_ninliers );
     tol -= tol_step;
  } 
}    


void optimize( RelaxationContext *rc, Cameras *cam )
{
 int i;
 gsl_vector *xpts, *xcams;
 
 xpts= gsl_vector_alloc(3);
 xcams = gsl_vector_alloc(6);
 
 for( i = 0; i < rc->npoints; i++ ){
    printf( "point %i \n", i  ); 
    relax_points( xpts, rc->hp[i]->p, rc->hp[i], cam );
    gsl_vector_memcpy( rc->hp[i]->p, xpts ); 
 }
 
 for( i=0; i < rc->c->ncams; i++ ){
    printf( "camera %i \n", i  );
    relax_cameras( xcams, rc, i );
    adjust_relax_context( rc, xcams, i );
 }   
 
 gsl_vector_free(xpts);
 gsl_vector_free(xcams);
}


void refine_inliers( HomologPoint *hp, Cameras *c , double tol )
{
 int i;
 hp->ninliers = 0;
 
 for( i = hp->first_frame; i <= hp->last_frame; i++ )
   if( hp_reproj_error( hp, i, c ) < tol ){
      hp_set_inlier( hp, i, TRUE );
      hp->ninliers++;  
   }
   else
      hp_set_inlier( hp, i, FALSE );
}


