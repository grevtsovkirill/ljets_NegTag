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
    LOGNAME+="data"
    echo ./NtupleReaderApp -d -s $name -m $mode -f $(get_data_ntupledumper)
    run ./NtupleReaderApp -d -s $name -m $mode -f $(get_data_ntupledumper)
    
done < <(getCleanSysts)

wait
