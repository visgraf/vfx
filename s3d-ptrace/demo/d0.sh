#!/bin/sh


ROOT=../
DATA=../data

BIN=$ROOT/platforms/linux/bin

echo "Coping hdrdome picture..."
cp $DATA/dome.pfm ./

echo "Generating Back Groud images..."
ffmpeg -i video.mp4 -vf "fps=30" frame%04d.png
counter=0
for file in frame*.png; do
  convert "$file" -endian lsb "bk${counter}.pfm"
  counter=$((counter + 1))
done
rm frame*.png




