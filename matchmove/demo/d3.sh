#!/bin/sh


echo "Calibrating HDR camera 3D points..."
../bin/pclcalib ../data/g7cam.txt cam_refined.txt  < ../data/calib_video_projs.txt > calib_points.txt
echo "Six points camera calibrating..."
../bin/singlecam  ../data/calib_projs.txt calib_points.txt



