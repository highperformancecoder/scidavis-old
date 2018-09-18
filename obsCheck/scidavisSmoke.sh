#!/bin/bash
set -e
Xvfb :10 &
export DISPLAY=:10
scidavis -h
jobs -p|while read pid; do kill -KILL $pid; done
