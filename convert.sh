#!/bin/bash
NAME=`basename $1`
# Warning OUT name should be something like 0001.mp3
OUT="./out/01/$NAME"
echo " $1 --> $OUT" 
ffmpeg -i $1 -ab 128k $OUT
