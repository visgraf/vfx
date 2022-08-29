#!/bin/sh


ROOT=../
DATA=../data

BIN=$ROOT/platforms/linux/bin

echo "Get polyshadow diffuse color"
$BIN/getcolor g7cam.txt cam_refined.txt 127 < $DATA/modeling.scn



