#!/bin/bash

# include run function
RUNMODE="$1"
MAXJOBS=6
source ../helpers/run_mode.sh

# create log dir
[[ -e log ]] || mkdir log

# Read ../config/systematics.txt line by line
while read mode name type var; do

    echo systematic: $name, mode: $mode
    # don't do anything for nomode, bootstrap and rewmode
    if [[ "$mode" == "nomode" ]] || [[ "$mode" == "bootstrap" ]] || [[ "$mode" == "rwmode" ]]; then
	continue
    fi

    # create directory for the syst.
    if [ ! -e res/$name ]; then
	echo creating res/$name
	mkdir res/$name
    fi

    LOGNAME="$mode$name"
    # for "dataperiod" only - run on data only period one by one
    #if [[ ${mode:0:6} == "doJets" ]] || [[ $mode == "xAOD2" ]] || [[ $mode == "xAOD" ]] && [[ $name != "FlavourTagging_Nominal" ]]; then
        # HERWIG
        if [[ $name == *"generator"* ]]; then
          echo ./NtupleReaderApp -s $name -m $mode -f $(get_mcHERWIG_ntupledumper)
          run ./NtupleReaderApp -s $name -m $mode -f $(get_mcHERWIG_ntupledumper)
        fi
    #fi	

done < <(getCleanSysts)

wait
