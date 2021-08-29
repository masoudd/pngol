#!/bin/bash

ffmpeg -start_number 0 -r 2 -i life_%05d.png out.gif
