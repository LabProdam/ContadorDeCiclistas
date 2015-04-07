#!/bin/bash

bin/CycloTracker -D 0 --address faria_lima-5125 --record out.avi -S /dev/video1 -t /dev/ttyACM0 > Cyclo.log 2>&1  &

sleep 5

while true; do
ffmpeg -f video4linux2 -video_size 640x480 -framerate 20 -i /dev/video1 -f pulse -ac 1 -i default -vcodec libx264 -s 640x480 -preset medium -acodec libmp3lame -ar 44100 -threads 1 -qscale 3 -b:a 128k -b:v 100k -maxrate 100k -minrate 100k -g 40 -bufsize 100k -pix_fmt yuv420p -f flv rtmp://a.rtmp.youtube.com/live2/LabProdam.h8fr-0yht-zf0u-ags8

sleep 5
done
