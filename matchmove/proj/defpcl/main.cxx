#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

float scale;
Mat image;
int nframes;
int npoints;
int frame_id;
vector<Mat> video;
Point p[20];

static void load_video( vector<Mat> &video, char *fname );
static void on_trackbar( int, void* );
static void CallBackFunc(int event, int x, int y, int flags, void* userdata);
static int get_ncameras( FILE *f );


int main( int argc, char **argv )
{
   scale = atof( argv[4] );
   load_video( video, argv[1] );
   npoints = atoi( argv[2] );
   FILE *cam_file = fopen( argv[3], "r" ); 
   int nframes = get_ncameras( cam_file ); 
   fclose( cam_file );
   frame_id = 0;
   
   imshow( "PCloud", video[frame_id] );
   char TrackbarName[50];
   sprintf( TrackbarName, "%d Frames", nframes );
   createTrackbar( TrackbarName, "PCloud", &frame_id, nframes-1, on_trackbar );
   on_trackbar( frame_id, 0 );
   setMouseCallback("PCloud", CallBackFunc, NULL);
   
   for(;;){
       imshow( "PCloud", video[frame_id] );
       char c = waitKey();
       if( c == ' ' ){
         cout << "Frame " << frame_id << endl;
         cout << "Npoints " << npoints << endl;
         for( int i = 0; i < npoints; i++ )
            cout << p[i].x/scale << " " << p[i].y/scale << endl;
         cout << endl;
       }
       else if(c == 27 )
         exit(0);
       else if( getWindowProperty( "PCloud", WND_PROP_VISIBLE) == -1 )
           exit(0);
   }
   return 0;
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
         resize(frame, *resized, Size(scale*frame.cols, scale*frame.rows),
                                      INTER_LINEAR);
         video.push_back(*resized);
      }
 }while(!frame.empty());
}


void on_trackbar( int, void* )
{
  imshow( "PCloud", video[frame_id] );
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
 static int i = 0;
 char str[2];
 
 if( i == 0 )
    image = video[frame_id].clone();

 if( event == EVENT_LBUTTONDOWN ){
    imshow( "PCloud", video[frame_id] );  
    line( image, Point( x - 5, y ), Point( x + 5, y ), Scalar( 0, 0, 255 ),  2, 8 );
    line( image, Point( x, y - 5 ), Point( x, y + 5 ), Scalar( 0, 0, 255 ),  2, 8 );
    sprintf( str, "%i", i+1 );
    putText( image, str, Point( x-15, y -10), FONT_HERSHEY_DUPLEX, 1, Scalar(0,0,255), 2 ); 
    imshow( "PCloud", image );
    p[i].x = x;
    p[i].y = y;
    i++; 
 }
 
 if( i == npoints )
   i = 0;
}


int get_ncameras( FILE *f )
{
 int i = 0;
 float r, t; 
 
 while( !feof(f) ){
   fscanf( f, "Frame %i\n", &i );
   fscanf( f, "%f%f%f%f%f%f%f%f%f\n", 
               &r, &r, &r,  &r, &r, &r, &r, &r, &r );
   fscanf( f, "%f%f%f\n", &t, &t, &t );
   i++;
 }
 return i;
} 
 



