#include "sequence.h"

static void write_cameras( FILE *f, CalibSequenceContext *cc ); 

int main( int argc, char **argv )
{
 int i;
 FILE *fin, *fout, *fk;
 CalibSequenceContext *cc;
 SubSeqNode *aux;
 gsl_matrix *k;
 FeaturesList *fl;

 init_timer_random();
 k = gsl_matrix_alloc( 3, 3 );
 fk = fopen( argv[1], "r" );
 gsl_matrix_fscanf( fk, k );
 fin = fopen( argv[2], "r" );
 fout = fopen( argv[3], "w" );
 cc = calib_context_alloc( features_list_read( fin ) );
 seq_extrinsic_calibrate( cc, k );

 write_cameras( fout, cc ); 
 calib_context_dispose( cc );
 gsl_matrix_free( k );
 fclose( fin );
 fclose( fout );
 fclose( fk );
}


void write_cameras( FILE *f, CalibSequenceContext *cc )
{
 int i;
 SubSeqNode *aux;

 for(  aux = cc->sl; (aux != NULL) && (aux->next != NULL); aux = aux->next ){
   for( i = 0; i<aux->nframes - 1 ; i++ ){ 
     fprintf( f, "Frame %i\n", aux->first_frame + i );
     gsl_matrix_fprintf( f, aux->cl[i].r, "%f" );
     fprintf( f, "\n" );
     gsl_vector_fprintf( f, aux->cl[i].t, "%f" );
     fprintf( f, "\n" );
   }  
 }
} 

