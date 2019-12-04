#!/bin/bash

# include run function and sample getter
RUNMODE="$1"
source ../helpers/run_mode.sh

# create log directory
[[ -e log ]] || mkdir log

sys=FlavourTagging_Nominal

#echo "run data"
#run ./ApplyAllWeightsApp -d -f $(get_data_ntupledumper) -c "a" -s "FlavourTagging_Nominal"
#echo 'run HERWIG mc'
#run ./ApplyAllWeightsApp -f $(get_mcHERWIG_ntupledumper a $sys) -c a -s $sys 
#echo "run mc"
run ./ApplyAllWeightsApp -f $(get_mc_ntupledumper a $sys) -c a -s $sys

#run ./ApplyAllWeightsApp -c "a" -s $sys

