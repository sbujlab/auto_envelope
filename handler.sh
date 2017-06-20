#!/bin/bash
#

for i in `seq 10 29`
do
	declare -i startPoint
	startPoint=1000*$i # +10000 if we want to start downstream
	#echo $startPoint
	qsub ./periPlot.sh $startPoint
done
