#!/bin/bash

# -r N makes each image last 1/N seconds. change -r N value to -r 1/5 to make each image last 5 seconds. -r 2 to make each image last 0.5 seconds etc..
# change -start_number N to 0 to start from life_00000.png, or 100 to start from life_00100.png etc..
# see https://trac.ffmpeg.org/wiki/Slideshow
ffmpeg -start_number 0 -r 2 -i life_%05d.png -c:v libx264 -vf fps=25,format=yuv420p out.mp4
