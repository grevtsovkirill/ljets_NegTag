#!/bin/bash

# include run function
RUNMODE="$1"
MAXJOBS=5
source ../helpers/run_mode.sh

PERIODS=""
# while read mode syst; do
while read mode name type var; do

    [[ "$mode" == "nomode" ]] && continue

    if [[ "$name" == "dataperiod" ]]; then
	while read period; do
	    run ./CalculateVariables -s $name$period
	    PERIODS+=" $period"
	done < $PERIODFILE
    else
    run ./CalculateVariables -s $name
    fi

done < <(getCleanSysts)

