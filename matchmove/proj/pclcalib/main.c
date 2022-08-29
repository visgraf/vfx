#include "pcloud.h"
#include "cameras.h"

#define MAX_NCLOUDS 20

static void write_cloud( gsl_matrix *m );
                                
int main( int argc, char **argv )
{
 int i = 0;
 FILE *k_file, *cam_file;
 Cameras *cams;
 PointCloud pc[MAX_NCLOUDS];
 gsl_matrix *m;
 gsl_matrix *k;
 
 k = gsl_matrix_alloc( 3, 3 );
 k_file = fopen( argv[1], "r" );
 gsl_matrix_fscanf( k_file, k );
 cam_file = fopen( argv[2], "r" );
 cams = cameras_read( k, cam_file );
 
 while( !feof(stdin) )
   pcloud_read( &pc[i++], stdin ); 
 
 m = pcloud_calib( pc, cams, i );
 
 gsl_matrix_fprintf( stdout, m, "%f" );
 
 fclose( k_file );
 fclose( cam_file );
 gsl_matrix_free( k );
 gsl_matrix_free( m );
}



