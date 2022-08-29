#ifndef REL_CONTEXT_H
#define REL_CONTEXT_H

#include "homol_point.h"
#include "cameras.h"
#include "ba.h"
#include "featureslist.h"

#define MAX_POINTS 2000
#define MAX_FEATURES 1000
#define MIN_HOMOL_SEGMENT 45
#define MIN_HOMOL_INLIERS 45

typedef struct RelaxationContext{
  HomologPoint *hp[MAX_POINTS];
  int npoints;
  Cameras *c;
  gsl_matrix *xproj;
  gsl_matrix *yproj;
  int last_features[MAX_FEATURES];
} RelaxationContext;



RelaxationContext *rc_alloc( char *fname, Cameras *c, double tol_error );
void  rc_set_all_projs( RelaxationContext *rc, int j, int feature );
void rc_free( RelaxationContext *rc );
void rc_set_proj( RelaxationContext *rc, int point, int camera, double x, double y );
void rc_points_optimize( RelaxationContext *rc );
void rc_camera_optimize( RelaxationContext *rc );
void rc_reconstruct_points( RelaxationContext *rc, double tol_error );
void rc_relax( RelaxationContext *rc, int max_iter, double tol );
void rc_reconstruct_points( RelaxationContext *rc, double tol_error );
void rc_points_optimize( RelaxationContext *rc );
void rc_camera_optimize( RelaxationContext *rc );


 
 


#endif
