#!/bin/sh


ROOT=..
DATA=../data

BIN=$ROOT/platforms/linux/bin


echo "Rendering frames..."

$BIN/mmove g7cam.txt cam_refined.txt 2 < $DATA/modeling.scn
convert out2.pfm out2.png
$BIN/mmove g7cam.txt cam_refined.txt 150 < $DATA/modeling.scn
convert out150.pfm out150.png
$BIN/mmove g7cam.txt cam_refined.txt 230 < $DATA/modeling.scn
convert out230.pfm out230.png

