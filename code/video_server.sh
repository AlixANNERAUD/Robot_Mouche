#!/bin/bash

libcamera-vid -t 0 -n --width 720 --height 480 --framerate 15 -o - | cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8090}' :demux=h264