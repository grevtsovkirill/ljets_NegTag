#!/bin/bash

export here=$PWD
gen="$1"
mkdir -p ./HTC/CMD
mkdir -p ./HTC/LOGS

if [ $# -eq 2 ];then
chan=$2
fi


if [[ -z  $chan ]];then
    array=($(seq 13 14))
else
    array=($(ls -d pbs_files/*data.pbs))
fi



echo "${array[*]}"
echo $chan
COUNTER=0
condor=''
for i in ${array[*]};
do
    if [[ -z  $chan ]];then
	condor=HTC/CMD/condor_MC_"$gen""$chan"-"$i".cmd
	echo "MC"
	echo "executable = pbs_files/JZ"$i"W"$gen".pbs "             > $condor
	COUNTER=$i
    else
	let COUNTER=COUNTER+1
	condor=HTC/CMD/condor_data_"$gen""$chan"-"$COUNTER".cmd
 	echo "data"
	echo "executable = "$i" "             > $condor
    fi
 
    echo "should_transfer_files = YES "      >> $condor
    echo "when_to_transfer_output = ON_EXIT "      >> $condor
    echo "universe        = vanilla "         >> $condor
    echo "output          = "HTC/LOGS/$gen""$chan"_"$COUNTER".out "   >> $condor
    echo "error           = "HTC/LOGS/$gen""$chan"_"$COUNTER".err "   >> $condor
    echo "log             = "HTC/LOGS/$gen""$chan"_"$COUNTER".log "   >> $condor
    echo "notification    = never "          >> $condor
    echo "queue "                            >> $condor

    condor_submit $condor
    sleep 3s
    #echo $condor
done #<${filename}
# exit