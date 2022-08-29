#!/bin/sh


echo "Selecting points..."
../bin/defpcl ../data/video.mp4 3 cam_refined.txt  1 > points2d.txt
echo "Debug..."
../bin/debug ../data/g7cam.txt cam_refined.txt ../data/video.mp4 < points2d.txt
echo "Point Cloud Calib"
../bin/pclcalib ../data/g7cam.txt cam_refined.txt  < points2d.txt > points3d.txt	



