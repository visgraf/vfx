#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

#define SQR(X) ((X)*(X)) 


static double min_distancy( Point2f p2, vector<Point2f> p1 ); 

int main(int argc, char **argv)
{
 VideoCapture capture(argv[1]);
   
  Mat old_frame, old_gray;
  vector<Point2f> p0, p1, p2;
  int npoints = atoi(argv[2]);
  double features_dist = atof(argv[3]);

  capture >> old_frame;
  cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);
  goodFeaturesToTrack(old_gray, p0, npoints, 0.00001, features_dist );
  std::cout << "Features per frame = " << argv[2] << std::endl;
  std::cout << "Frame 0"  << std::endl;
  for(uint i = 0; i < p0.size(); i++)
      std::cout << p0[i].x << " " << p0[i].y << " 0"  << std::endl;
  Mat mask = Mat::zeros(old_frame.size(), old_frame.type());
  int k = 1;
  while(true){
      Mat frame, frame_gray;
      capture >> frame;
      if (frame.empty())
            break;
      std::cout << "Frame " << k++ << std::endl;
      cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        vector<uchar> status;
        vector<float> err;
        TermCriteria criteria = TermCriteria((TermCriteria::COUNT) 
                                + (TermCriteria::EPS), 30, 0.01);
        calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err );
        vector<Point2f> good_new;
        for(uint i = 0; i < p0.size(); i++)
        {
            if(status[i] == 1) {
                good_new.push_back(p1[i]);
                circle(frame, p1[i], 2, Scalar(0,0,255), -1);
                std::cout << p1[i].x << " " << p1[i].y << " 0"  << std::endl;
            }
            else {
                std::cout << p1[i].x << " " << p1[i].y << " -1"  << std::endl;
                int n = 0;
                do{
                  goodFeaturesToTrack(old_gray, p2, n+1, 0.00001, 4 );
                  if( (min_distancy( p2[n], good_new ) > features_dist) &&
                      (min_distancy( p2[n], p1 ) > features_dist) ){ 
                         good_new.push_back( p2[n] ); 
                         n = -1;  
                  }
                  n++;
                }
                while( n != 0 );
            } 
        }  
        Mat img;
        add(frame, mask, img);
        imshow("Frame", img);
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
        old_gray = frame_gray.clone();
        p0 = good_new;
    }
}


static double min_distancy( Point2f p2, vector<Point2f> p1 )
{
 Point2f p;
 double dist, min_dist = 1000000;

 for( int i = 0; i < p1.size(); i++ ){
   dist = SQR(p2.x - p1[i].x) + SQR(p2.y - p1[i].y);
   if( dist < min_dist )
      min_dist = dist;
 }
 return min_dist;
}
