#!/bin/bash

source ../helpers/run_mode.sh

ALL_SYSTEMATICS=""
ALL_VARS=""
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

echo './total_syst -s '$ALL_SYSTEMATICS' -v '$ALL_VARS' -c a'
./total_syst -s $ALL_SYSTEMATICS -v $ALL_VARS -c a
