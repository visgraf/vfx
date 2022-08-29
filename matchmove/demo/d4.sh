#!/bin/sh


S3D_DEM=../../s3d-v64-2021/test/demo

echo "Coping files..."
cp ../data/video.mp4  $S3D_DEM
cp cam_refined.txt  $S3D_DEM
cp points3d.txt $S3D_DEM	
cp r $S3D_DEM
cp ../data/g7cam.txt $S3D_DEM

