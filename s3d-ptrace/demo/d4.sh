#!/bin/sh


ROOT=../
DATA=../data

BIN=$ROOT/platforms/linux/bin
SCN=$DATA/scn


echo "Rendering frames."
echo "It will take more than 24Hs running on a Core-i7 processor"

$BIN/mmove g7cam.txt cam_refined.txt < $DATA/modeling.scn

