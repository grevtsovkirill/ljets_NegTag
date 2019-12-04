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
    if [ $name == "dataperiod" ]; then
	rmdir res/$name
	LOGNAME+="data"
	while read period; do
	    pname=$name$period
	    if [ ! -e res/$pname ]; then
		echo creating res/$pname
		mkdir res/$pname
	    fi
	    LOGNAME+="$pname"
	    run ./NtupleReaderApp -d -s $pname -p $period -f ../NtupleDumper/res/data$period.root
	    rm -f res/$pname/mc.root
	    ln -s ../FlavourTagging_Nominal/mc.root res/$pname/mc.root
	done < $PERIODFILE
    # for xAOD or doJets and != nominal, run MC only
    elif [[ ${mode:0:6} == "doJets" ]] || [[ $mode == "xAOD2" ]] || [[ $mode == "xAOD" ]] && [[ $name != "FlavourTagging_Nominal" ]]; then
        # HERWIG
        if [[ $name == *"generator"* ]]; then
           #echo ./NtupleReaderApp -c "a" -s $name -m $mode -f $(get_mcHERWIG_ntupledumper)
            run ./NtupleReaderApp -c "a" -s $name -m $mode -f $(get_mcHERWIG_ntupledumper)
        else 
	    #echo ./NtupleReaderApp -c "a" -s $name -m $mode -f $(get_mc_ntupledumper d)
	    run ./NtupleReaderApp -c "a" -s $name -m $mode -f $(get_mc_ntupledumper d) 
        fi
        # use data from FlavourTagging_Nominal except if it contains "subleadingjet"
 	rm -f res/$name/data.root
        if [[ $name == *"subleadingjet"* ]]; then
            ln -s ../subleadingjet/data.root res/$name/data.root
        else 
	    ln -s ../FlavourTagging_Nominal/data.root res/$name/data.root
        fi
    # remaining systematic (doSubLeadJet) and FlavourTagging_Nominal: run data and MC
    else
	echo ./NtupleReaderApp -c "a" -s $name -m $mode -f $(get_mc_ntupledumper $name)
	run ./NtupleReaderApp -c "a" -s $name -m $mode 
	LOGNAME+="data"
	echo ./NtupleReaderApp -c "a" -d -s $name -m $mode -f $(get_data_ntupledumper)
	run ./NtupleReaderApp -c "a" -d -s $name -m $mode -f $(get_data_ntupledumper)
    fi

done < <(getCleanSysts)

wait
