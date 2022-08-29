#include "sequence.h"

static int get_n_tracked_features( FeaturesList *fl );


SubSeqNode *subseq_node_alloc( CalibSequenceContext *cc, FeaturesList *fl )
{
 int i, j, k, n_tracked_features;
 SubSeqNode *sn;
 Features *aux;
  
 n_tracked_features = get_n_tracked_features( fl );
 sn = NEWSTRUCT( SubSeqNode );  
 sn->cc = cc;
 sn->nframes = fl->nframes;
 sn->nfeatures = n_tracked_features;
 sn->first_frame = fl->head->frame;
 sn->last_frame = fl->tail->frame;
 sn->inliers = NEWTARRAY( cc->nfeatures, int );
 sn->points = gsl_matrix_alloc( n_tracked_features, 3 );
 sn->fp = NEWTARRAY( fl->nframes, FrameProjs );
 sn->winliers = NEWTARRAY( cc->nfeatures, int );
 sn->wpoints = NULL;
 sn->wfp = NEWTARRAY( fl->nframes, FrameProjs );
 sn->wnfeatures = 0;
 sn->cl = NEWTARRAY( fl->nframes, Camera );
   
 for( aux = fl->head, i = 0; aux != NULL; aux = aux->next, i++ ){
   k = 0;
   sn->wfp[i].projs = NULL;
   sn->fp[i].projs = gsl_matrix_alloc( n_tracked_features, 2 );
   camera_fields_alloc( &sn->cl[i] );
   for( j = 0; j < cc->nfeatures; j++ ){
     if( fl->tail->status[j] == FEATURE_TRACKED ){ 
       gsl_matrix_set( sn->fp[i].projs, k, 0, aux->p[j].x );
       gsl_matrix_set( sn->fp[i].projs, k, 1, aux->p[j].y );
       k++;    
       sn->inliers[j] = INLIER;
     }
     else
       sn->inliers[j] = OUTLIER;   
   } 
 }   
 
 sn->next = NULL; 
 return sn;
}


void camera_fields_alloc( Camera *cam )
{
 cam->k = gsl_matrix_alloc( 3, 3 );
 cam->r = gsl_matrix_alloc( 3, 3 );
 cam->t = gsl_vector_alloc( 3 );
}


void camera_fields_dispose( Camera *cam )
{
 gsl_matrix_free( cam->k );
 gsl_matrix_free( cam->r );
 gsl_vector_free( cam->t );
}


void subseq_node_dispose(  SubSeqNode *sn ) 
{
 int i;
 
 for( i = 0; i < sn->nframes; i++ ){
    camera_fields_dispose( &sn->cl[i] );
    gsl_matrix_free( sn->fp[i].projs ); 
 }
    
 gsl_matrix_free( sn->points );
 free( sn->fp );
 free( sn->cl );
 free( sn );
}


int get_n_tracked_features( FeaturesList *fl )
{
 int i, k=0;
 
 for( i = 0; i < fl->tail->nfeatures; i++ )
   if( fl->tail->status[i] == FEATURE_TRACKED )
    k++;
  
 return k;
}

