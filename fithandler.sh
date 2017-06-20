#!/bin/bash
#
begin=""
step="10"
end=""
flag=0
store=""
posFlag=-999999

while read X Y; do
    if [ $flag -eq 0 ] 
    then
	if [ $X -eq $posFlag ] 
	then
	    store=$Y
	    flag=1
	fi
    elif [ $flag -eq 1 ]
    then
	if [ $X -eq $posFlag ] 
        then
	    echo "$X is equal to $posFlag"
            end=$Y
	    begin="$store"
    #        echo "$begin $step $end"
            qsub ./gFit.sh $begin $step $end
	    store="$end"
	fi
    else 
	continue
    fi
done<vertex_storage.txt
