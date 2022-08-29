#!/bin/sh


echo "Tracking points..."
../bin/track ../data/video.mp4 150 10 > f.txt
echo "Calibrating cameras..."
../bin/calib ../data/g7cam.txt f.txt cameras.txt
echo "Relaxing Solution..."
../bin/relax ../data/g7cam.txt f.txt cameras.txt cam_refined.txt


