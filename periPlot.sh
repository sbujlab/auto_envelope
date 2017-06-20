#!/bin/bash
#
#$ -cwd
#$ -j y
#$ -S /bin/bash
startPoint=$1
export DISPLAY=localhost:0.0
arg="peripheralPlotPoints_sep.c($startPoint)"
root "$arg"
