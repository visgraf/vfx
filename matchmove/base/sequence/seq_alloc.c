#include "sequence.h"

CalibSequenceContext *calib_context_alloc( FeaturesList *fl )
{
 CalibSequenceContext *cc;

 cc = NEWSTRUCT( CalibSequenceContext );
 cc->fl = fl;
 cc->sl = NULL;
 cc->cl = NULL;
 cc->nframes = fl->nframes;
 cc->nfeatures = fl->head->nfeatures;
 cc->ransac_iterations = RANSAC_NITERS;
 cc->ransac_inliers_tol =  .01 ;
 cc->reproj_inliers_tol = RANSAC_REPROJ_TOL ;

 return cc;
}


void calib_context_dispose( CalibSequenceContext *cc )
{
 features_list_dispose( cc->fl );
 free( cc );
}
