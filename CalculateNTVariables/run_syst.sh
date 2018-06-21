#!/bin/bash

# include run function
RUNMODE="$1"
MAXJOBS=5
source ../helpers/run_mode.sh

ALL_SYSTEMATICS=""
ALL_VARS=""
while read mode name type var; do

    if [[ "$type" == "updown" ]]; then
	if [[ ${name:${#name}-4:${#name}} == "down" ]]; then
	    XSYST=${name:0:${#name}-4}
	    run ./rel_syst -s $XSYST -t "$type" -v "$var" -c a
	    #run ./rel_syst -s $XSYST -t "$type" -v "$1" -c a
	    ALL_SYSTEMATICS+=" $XSYST"
	    ALL_VARS+=" $var"
	elif [[ ${name:${#name}-2:${#name}} == "up" ]]; then
	    continue
	else
	    echo "skipping $name"
	fi

#    elif [[ "$type" == "dataperiod" ]]; then
#	run ./rel_syst_C -s "$name" -t "$type" -p "$PERIODS" -v "$var"
#	ALL_SYSTEMATICS+=" $name"
#	ALL_VARS+=" $var"
    elif [[ "$type" == "no_rel" ]]; then
	continue
    else
	run ./rel_syst -s "$name" -t "$type" -v "$var" -c a
	#run ./rel_syst -s "$name" -t "$type" -v "$1" -c a
	ALL_SYSTEMATICS+=" $name"
	ALL_VARS+=" $var"
    fi

done < <(getCleanSysts)
