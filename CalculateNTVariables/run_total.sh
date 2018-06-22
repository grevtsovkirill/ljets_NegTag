#!/bin/bash

source ../helpers/run_mode.sh

input=$1
if [[ -n "$input" ]]; then
    ALL_VARS=$input
else
    ALL_VARS="sf kll khf eps_d eps_neg_d"
fi

ALL_VARS1="sf kll khf eps_d eps_neg_d"
ALL_SYSTEMATICS=""
while read mode name type var; do    
    if [[ "$type" == "updown" ]]; then
	if [[ ${name:${#name}-4:${#name}} == "down" ]]; then
	    XSYST=${name:0:${#name}-4}
	    ALL_SYSTEMATICS+=" $XSYST"
	    ALL_VARS+=" $var"
	    #ALL_VARS+=" $1"
	elif [[ ${name:${#name}-2:${#name}} == "up" ]]; then
	    continue
	else
	    echo "skipping $name"
	fi
#    elif [[ "$type" == "dataperiod" ]]; then
#	ALL_SYSTEMATICS+=" $name"
#	ALL_VARS+=" $var"
    elif [[ "$type" == "no_rel" ]]; then
	continue
    elif [[ "$type" == "std" ]]; then
	ALL_SYSTEMATICS+=" $name"
	ALL_VARS+=" $var"
    else
	ALL_SYSTEMATICS+=" $name"
	ALL_VARS+=" $var"
	#ALL_VARS+=" $1"
    fi
    
done < <(getCleanSysts)

echo ' -v '$ALL_VARS
./total_syst -s $ALL_SYSTEMATICS -v $ALL_VARS -c a
