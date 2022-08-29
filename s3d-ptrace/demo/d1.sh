#!/bin/sh


ROOT=../
DATA=../data

BIN=$ROOT/platforms/linux/bin


echo "Rendering using Path Tracing..."
$BIN/ptrace < $DATA/cornell.scn > output.png



