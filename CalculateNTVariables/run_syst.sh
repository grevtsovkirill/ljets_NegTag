#!/bin/bash

# include run function
RUNMODE="$1"
MAXJOBS=5
source ../helpers/run_mode.sh

input=$1
if [[ -n "$input" ]]; then
    ALL_VARS=$input
else
    ALL_VARS="sf kll khf eps_d eps_neg_d"
fi

comp="a"

ALL_SYSTEMATICS=""
for var1 in $ALL_VARS; do
    while read mode name type var; do
	
	if [[ "$type" == "updown" ]]; then
	    if [[ ${name:${#name}-4:${#name}} == "down" ]]; then
		XSYST=${name:0:${#name}-4}
		run ./rel_syst -s $XSYST -t "$type" -v "$var1" -c $comp
	    #run ./rel_syst -s $XSYST -t "$type" -v "$1" -c $comp
		ALL_SYSTEMATICS+=" $XSYST"
	    #ALL_VARS+=" $var"
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
	    run ./rel_syst -s "$name" -t "$type" -v "$var1" -c $comp
	#run ./rel_syst -s "$name" -t "$type" -v "$1" -c $comp
	    ALL_SYSTEMATICS+=" $name"
	#ALL_VARS+=" $var"
	fi
	
    done < <(getCleanSysts)
    echo ""
    echo "****************************************"
    echo "Done with "$var1
    echo "****************************************"
    echo ""

done