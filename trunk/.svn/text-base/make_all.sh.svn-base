#!/bin/bash

source setup.sh

# choose here the setup
#source make_lists_and_lumi.sh 15
source make_lists_and_lumi.sh 16
#source make_lists_and_lumi.sh 1516

# usual sequence of code runs
for folder in NtupleDumper GetPtEtaWeights GetNTrackWeights ApplyAllWeights NtupleReader CalculateNTVariables CreatePlots; do
    pushd $folder
    if [ -e Makefile ]; then
	make clean
	make
	if [ $? != 0 ]; then
	    echo make failed in $folder!
	    break
	fi
    fi
    popd
done
