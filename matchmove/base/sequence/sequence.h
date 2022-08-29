#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string.h>
#include <gsl/gsl_min.h>
#include "featureslist.h"
#include "ransac.h"
#include "ba.h"

#define RANSAC_REPROJ_TOL 15.
#define RANSAC_NITERS 60
#define FRAGMENT_NFRAMES 60
#define MIN_MOV_DIST 20.
#define MIN_MOV_DIST2 4.
#define INTRISIC_DIST_TOL 100
#define INTERFRAME_REPROJ_TOL 20.
#define MAX_INTERFRAG_NINLIERS 500
#define MAX_NUMBER_OF_SUBSEQ 100

#define   PROJS(S,J)   (S)->fp[ J ].projs
#define   WPROJS(S,J)  (S)->wfp[ J ].projs

typedef struct Camera{
  gsl_matrix *k, *r;
  gsl_vector *t;
}Camera;


typedef struct FrameProjs{
   gsl_matrix *projs;
}FrameProjs;


typedef struct SubSeqNode{
  struct CalibSequenceContext *cc;
  struct SubSeqNode *next;
  int nframes;                      /* Number of frames */
  int nfeatures;                    /* Number of features per frame */
  int first_frame, last_frame;      /* Interval in the FeatureList */
  int *inliers;
  gsl_matrix *points;               /* 3d points */
  FrameProjs *fp;                   /* Projections */ 
  int *winliers;		     /* Work Inliers */
  gsl_matrix *wpoints;              /* Work 3d points */
  FrameProjs *wfp;                  /* Work projections */
  int wnfeatures;                   /* Number of Work Features */
  Camera *cl;
}SubSeqNode;


typedef struct CalibSequenceContext{
  FeaturesList *fl;
  SubSeqNode *sl;
  Camera *cl;
  int nframes;
  int nfeatures;
  int ransac_iterations;
  double ransac_inliers_tol;
  double reproj_inliers_tol;
}CalibSequenceContext;


/* constructors & destructors */
void camera_fields_alloc( Camera *cam );
void camera_fields_dispose( Camera *cam );

CalibSequenceContext *calib_context_alloc( FeaturesList *fl );
void calib_context_dispose( CalibSequenceContext *cc );

SubSeqNode *subseq_node_alloc( CalibSequenceContext *cc, FeaturesList *fl );
void subseq_node_dispose(  SubSeqNode *sn );

void camera_set( Camera *c, gsl_matrix *k, gsl_matrix *r, gsl_vector *t );

/* calib operation */
void seq_extrinsic_calibrate( CalibSequenceContext *cc, gsl_matrix *k  );

/* sub sequence operations */
void subseq_extrinsic_calibrate( SubSeqNode *s, gsl_matrix *k );
void subseq_dlt_calib( SubSeqNode *s, gsl_matrix *k );
void subseq_extrinsic_ba( SubSeqNode *s, gsl_matrix *k );
void subseq_recalibrate( SubSeqNode *s, double inlier_tol );

/* inliers & outliers operations */
void merge_outliers( int *out_inliers, int *inliers1, int *inliers2, int nfeatures );
int get_ninliers( int *inliers, int n );
void copy_projs_inliers( gsl_matrix *dst, gsl_matrix *src, int *inliers );
void copy_points_inliers( gsl_matrix *dst, gsl_matrix *src, int *inliers );
void  outliers_eliminate( SubSeqNode *s , int *inliers, int ninliers );

/* scale adjust function */
void get_scale_vector( double *scale_vector, CalibSequenceContext *cc ); 

/* misc */
void extract_rt( gsl_matrix *r, gsl_vector *t, gsl_matrix *ref );

#endif


