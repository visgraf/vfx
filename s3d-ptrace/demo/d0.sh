#!/bin/sh


ROOT=../
DATA=../data

BIN=$ROOT/platforms/linux/bin

echo "Coping hdrdome picture..."
cp $DATA/dome.pfm ./
echo "Generating Back Groud images..."
convert video.mp4 -endian lsb bk%d.pfm



