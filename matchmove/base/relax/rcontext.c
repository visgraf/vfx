#include "rcontext.h"

static int get_nframes( char *fname );


int get_nframes( char *fname )
{
 FILE *fin;
 int i, nfeatures, status, frame_id, nframes = 0;
 double coordx, coordy;
 fin = fopen( fname, "r" );
 
 fscanf( fin, "Features per frame = %i\n", &nfeatures );
 while( !feof(fin) ){
     for( i = 0; i < nfeatures; i++ ){
       fscanf( fin, "Frame %i\n", &frame_id );
       fscanf( fin, "%lf %lf %i\n", &coordx, &coordy, &status );
     }
     nframes++;
 }
 fclose(fin);
 return nframes;
}


#define NOT_TRACKED -1

 RelaxationContext *rc_alloc( char *fname, Cameras *c, double tol_error )
{
 FILE *fin;
 int i, j=0, k=0, nfeatures, nframes, status, 
     frame_id=0, last_frame = 0; 
 double coordx, coordy;
 RelaxationContext *rc;
 
 rc = NEWSTRUCT( RelaxationContext);
 nframes = get_nframes( fname );
 fin = fopen( fname, "r" );
 fscanf( fin, "Features per frame = %i\n", &nfeatures );
 rc->xproj = gsl_matrix_alloc( nframes, nfeatures );
 rc->yproj = gsl_matrix_alloc( nframes, nfeatures ); 
 
 for( i = 0; i < MAX_FEATURES; i++ )
   rc->last_features[i] = NOT_TRACKED; 
   
 while( !feof(fin) && (frame_id <= c->ncams) ) {
     for( i = 0; i < nfeatures; i++ ){
       fscanf( fin, "Frame %i\n", &frame_id );
       fscanf( fin, "%lf %lf %i\n", &coordx, &coordy, &status );
       gsl_matrix_set( rc->xproj, frame_id, i, coordx );
       gsl_matrix_set( rc->yproj, frame_id, i, coordy );
       
       if( rc->last_features[i] == NOT_TRACKED )
           rc->last_features[i] =  frame_id;    
    
       if( ( frame_id - rc->last_features[i] > MIN_HOMOL_SEGMENT ) &&
           (( frame_id == c->ncams ) ||  (status != FEATURE_TRACKED )) ){
           
            rc->hp[j] = hp_alloc( rc->last_features[i], frame_id - 1 );
            rc->last_features[i] = frame_id - 1;
            rc_set_all_projs( rc, j, i );
            hp_reconstruct_points( rc->hp[j], c, tol_error );
            
            if( rc->hp[j]->ninliers > MIN_HOMOL_INLIERS ){
              printf( "%i: [%i,%i]: %i ninliers: %i\n", k++, rc->hp[j]->first_frame,
                       rc->hp[j]->last_frame, rc->hp[j]->last_frame - rc->hp[j]->first_frame + 1,
                       rc->hp[j]->ninliers );
              j++;
            }  
              
         }
    }
 } 
 
 
 rc->c = c;
 rc->npoints = j;
 return rc;
}


void  rc_set_all_projs( RelaxationContext *rc, int j, int feature )
{
 int i;

 for( i = rc->hp[j]->first_frame; i <= rc->hp[j]->last_frame; i++ ){
      hp_set_proj( rc->hp[j], i,  
                   gsl_matrix_get( rc->xproj, i, feature ),
                   gsl_matrix_get( rc->yproj, i, feature ) ) ;

 }
}


void rc_free( RelaxationContext *rc )
{
 int i;
 
 for( i = 0; i < rc->npoints; i++ )
   hp_free( rc->hp[i] );
 
 cameras_free( rc->c );
}

