#include "sequence.h"

static void seq_subseq_list_create(  CalibSequenceContext *cc );
static void seq_insert_subseq(  CalibSequenceContext *cc, SubSeqNode *s );
static void seq_adjust_cameras(  CalibSequenceContext *cc  );


void seq_extrinsic_calibrate( CalibSequenceContext *cc, gsl_matrix *k  )
{
 SubSeqNode *aux;

 seq_subseq_list_create( cc  );

 printf( "Video Fragments\n" );
 for(  aux = cc->sl; aux != NULL; aux = aux->next )
   printf( "Frame: %i to %i nfeat = %i\n", aux->first_frame, aux->last_frame, aux->nfeatures );
 printf( "Calibration... The last fragment will be discarded\n");
 for(  aux = cc->sl; (aux != NULL) && (aux->next != NULL); aux = aux->next ){
     printf( "Frame: %i to %i\n", aux->first_frame, aux->last_frame ); 
     subseq_extrinsic_calibrate( aux, k  );
 }     
 seq_adjust_cameras( cc );
}


void seq_subseq_list_create(  CalibSequenceContext *cc )
{
 features_list_begin_frag( cc->fl );
 
 do{
    seq_insert_subseq( cc, subseq_node_alloc( cc, 
                       features_list_get_next_frag( cc->fl,
                       FRAGMENT_NFRAMES, MIN_MOV_DIST ) ));     
 }while( !features_list_end_frag() );
}


void seq_insert_subseq(  CalibSequenceContext *cc, SubSeqNode *s )
{
 static SubSeqNode *tail = NULL;
 
 if( tail == NULL ){
   tail = s;
   cc->sl = s; 
 }  
 else{
   tail->next = s;
   tail = s;
 }
}


void seq_adjust_cameras(  CalibSequenceContext *cc  )
{
 double scale = 1., scale_vector[MAX_NUMBER_OF_SUBSEQ];
 int i, n, j = 0;
 SubSeqNode *aux;
 gsl_matrix *id, *ref_a1, *ref_a2, *ref_b1, *ref_b2;
 
 ref_a1 = gsl_matrix_alloc( 3, 4 );
 ref_a2 = gsl_matrix_alloc( 3, 4 );
 ref_b1 = gsl_matrix_alloc( 3, 4 );
 ref_b2 = gsl_matrix_alloc( 3, 4 );
 id = gsl_matrix_alloc( 3,3 );
 gsl_matrix_set_identity(id);
 
 get_scale_vector( scale_vector, cc );
 for( aux = cc->sl; (aux != NULL) && (aux->next != NULL); aux = aux->next, j++ ){
   n = aux->nframes - 1;
   scale *= scale_vector[j];
   gsl_vector_scale( aux->next->cl[0].t , scale );
   calib_pmatrix_make( ref_a1, id, aux->next->cl[0].r, aux->next->cl[0].t );
   gsl_vector_scale( aux->next->cl[0].t , 1./scale );
   calib_pmatrix_make( ref_a2, id, aux->cl[n].r, aux->cl[n].t );
   for( i = 0; i < aux->next->nframes; i++ ){  
      gsl_vector_scale( aux->next->cl[i].t , scale );
      calib_pmatrix_make( ref_b1, id, aux->next->cl[i].r, aux->next->cl[i].t );
      calib_change_ref( ref_a1, ref_a2, ref_b1, ref_b2  );
      extract_rt( aux->next->cl[i].r, aux->next->cl[i].t, ref_b2 );  
   }
 }

 gsl_matrix_free( id );
 gsl_matrix_free( ref_a1 );
 gsl_matrix_free( ref_a2 );
 gsl_matrix_free( ref_b1 );
 gsl_matrix_free( ref_b2 );
}


void extract_rt( gsl_matrix *r, gsl_vector *t, gsl_matrix *ref )
{
 int i, j;
 
 for( i=0; i<3; i++ )
   for( j=0; j<3; j++ )
     gsl_matrix_set( r, i, j, gsl_matrix_get( ref, i, j ) );
     
  gsl_matrix_get_col( t, ref, 3 );   
}
