#!/bin/bash

# include run function and sample getter
RUNMODE="$1"
source ../helpers/run_mode.sh

# create log directory
[[ -e log ]] || mkdir log

ALL_SYSTS=""
for syst in $(getxAODsysts); do
    ALL_SYSTS=""
#ALL_SYSTS+=" $syst"
done

# Add PU SF uncertainty
# ALL_SYSTS+=" FlavourTagging_PRW_DATASF__1down"
# ALL_SYSTS+=" FlavourTagging_PRW_DATASF__1up"
# # Add JVT uncertainty
# ALL_SYSTS+=" FlavourTagging_JVT_effSF__1down"
# ALL_SYSTS+=" FlavourTagging_JVT_effSF__1up"
# # Add Conversion uncertainty
# ALL_SYSTS+=" conversions__1down"
# ALL_SYSTS+=" conversions__1up"
# # Add Hadronic interactions uncertainty
# ALL_SYSTS+=" hadronic__1down"
# ALL_SYSTS+=" hadronic__1up"
# # Add LL uncertainty
# ALL_SYSTS+=" longlivedparticles"

#echo 'run data'
#run ./CreateHistogramsApp -d -f $(get_data_ntupledumper) -c "1516" -s "FlavourTagging_Nominal" 
echo 'run mc nominal only'
run ./CreateHistogramsApp -f $(get_mc_ntupledumper d) -c "d" -s "FlavourTagging_Nominal"
#echo 'run mc'
#run ./CreateHistogramsApp -f $(get_mc_ntupledumper) -s $ALL_SYSTS
#echo 'run HERWIG mc'
#run ./CreateHistogramsApp -f $(get_mcHERWIG_ntupledumper) -s "FlavourTagging_Nominal" 
#echo 'run SHERPA mc'
#run ./CreateHistogramsApp -f $(get_mcSHERPA_ntupledumper) -s "FlavourTagging_Nominal" 

#wait
#
#if [[ "$RUNMODE" == "send2ge_Reweighting" ]]; then
#    echo "Please wait for the end of the jobs and run:"
#    echo "root -l -b -q calculate_jetptetareweighting.C"
#    echo "root -l -b -q data_mc_comparison_mc_weight.cpp"
#else
#    echo "Calculate reweighting."
#    run root -l -b -q calculate_jetptetareweighting.C
#    echo "Creating Data-MC comparison plots with MC weights applied."
#    run root -l -b -q data_mc_comparison_mc_weight.cpp
#fi
