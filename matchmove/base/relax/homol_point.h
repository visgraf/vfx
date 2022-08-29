#ifndef HOMOL_POINT_H
#define HOMOL_POINT_H

#include "cameras.h"
#include "calib.h"

#define MAX_FRAMES_PER_FEATURE 1000

typedef struct HomologPoint{
 int first_frame;  
 int last_frame;
 gsl_vector *p;
 gsl_matrix *projs;
 Boolean *is_inlier;
 int ninliers;
 int index;
} HomologPoint;

             
HomologPoint  *hp_alloc( int first_frame, int last_frame );  
void hp_free( HomologPoint *hp ); 
Boolean hp_has_frame( HomologPoint *hp, int frame );  
Boolean hp_is_inlier( HomologPoint *hp, int frame );
void hp_set_inlier(  HomologPoint *hp, int frame, Boolean b );
void hp_set_proj( HomologPoint *hp, int frame, double x, double y );
void hp_get_proj( gsl_vector *v, HomologPoint *hp, int frame );
void hp_reconstruct_points( HomologPoint *hp, Cameras *c, double to_error );
int hp_ninliers( HomologPoint *hp, gsl_vector *point, Boolean *inliers,
                 Cameras *c, double tol_error );
void adjust_inliers( HomologPoint *hp, Boolean *inliers );
double hp_reproj_error( HomologPoint *hp, int i, Cameras *c );   

#endif
