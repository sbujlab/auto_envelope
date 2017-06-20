#!/bin/bash
#
#$ -cwd
#$ -j y
#$ -S /bin/bash
startPoint=$1
stepSize=$2
endPoint=$3
export DISPLAY=localhost:0.0
arg="envelopeFitLoop_batch.c($startPoint,$stepSize,$endPoint)++"
root "$arg"
