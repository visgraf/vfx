#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
extern "C"{
  #include "pcloud.h"
  #include "cameras.h"
}

using namespace std;
using namespace cv;

#define MAX_NCLOUDS 20

void debug( gsl_matrix *m, Cameras *cams,
            PointCloud *pc, vector<Mat> &video );      
void load_video( vector<Mat> &video, char *fname );

static void write_cloud( gsl_matrix *m );
                                
int main( int argc, char **argv )
{
 int i = 0;
 FILE *k_file, *cam_file;
 Cameras *cams;
 PointCloud pc[MAX_NCLOUDS];
 gsl_matrix *m;
 gsl_matrix *k;
 vector<Mat> video;
 FILE *fd;
 
 k = gsl_matrix_alloc( 3, 3 );
 k_file = fopen( argv[1], "r" );
 gsl_matrix_fscanf( k_file, k );
 cam_file = fopen( argv[2], "r" );
 cams = cameras_read( k, cam_file );
 load_video( video, argv[3] );
 
 while( !feof(stdin) )
   pcloud_read( &pc[i++], stdin ); 
 
 m = pcloud_calib( pc, cams, i );
 debug( m, cams, pc, video );
  
 fclose( k_file );
 fclose( cam_file );
 gsl_matrix_free( k );
 gsl_matrix_free( m );
}


void debug( gsl_matrix *m, Cameras *cams, PointCloud *pc, vector<Mat> &video )
{
 int i, j;
 gsl_vector *v, *proj[3];
 
 v = gsl_vector_alloc(3);
 
 for( i = 0; i < 3; i++ )
    proj[i] = gsl_vector_alloc(2);

 for( int frame = 0; frame < 150; frame++ ){ 
   printf( "%i\n", frame );
   for( i = 0; i < m->size1; i++ ){
       gsl_matrix_get_row( v, m, i );
       cameras_apply( cams, v, proj[i], frame ); 
   }
   line( video[frame], Point( gsl_vector_get( proj[0], 0 ), gsl_vector_get( proj[0], 1 ) ),
                       Point( gsl_vector_get( proj[1], 0 ), gsl_vector_get( proj[1], 1 ) ),
                       Scalar( 0, 0, 255 ), 1 );
   line( video[frame], Point( gsl_vector_get( proj[1], 0 ), gsl_vector_get( proj[1], 1 ) ),
                       Point( gsl_vector_get( proj[2], 0 ), gsl_vector_get( proj[2], 1 ) ),
                       Scalar( 0, 0, 255 ), 1 );
   line( video[frame], Point( gsl_vector_get( proj[2], 0 ), gsl_vector_get( proj[2], 1 ) ),
                       Point( gsl_vector_get( proj[0], 0 ), gsl_vector_get( proj[0], 1 ) ),
                       Scalar( 0, 0, 255 ), 1 ); 
   imshow( "Debug", video[frame] );
   waitKey(25);
 }                   
} 


void load_video( vector<Mat> &video, char *fname )
{
 Mat frame;
 Mat  *resized;
 VideoCapture capture(fname);
 do{
      resized = new Mat();
      capture >> frame;
      if( !frame.empty() ){
         resize(frame, *resized, Size(frame.cols, frame.rows),
                                      INTER_LINEAR);
         video.push_back(*resized);
      }
 }while(!frame.empty());
}


