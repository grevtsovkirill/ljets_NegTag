#!/bin/bash

# include run function and sample getter
RUNMODE="$1"
source ../helpers/run_mode.sh

# create log directory
[[ -e log ]] || mkdir log

ALL_SYSTS=""
# for syst in $(getxAODsysts); do
#     ALL_SYSTS+=" $syst"
# done

# # Add PU SF uncertainty
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

sys=FlavourTagging_Nominal
#rew_d/rew_a
run ./CreateNTrackHistogramsApp -d -f $(get_data_ntupledumper) -c "1516" -s "FlavourTagging_Nominal" 
#run ./CreateNTrackHistogramsApp -c "d" -s $sys
#run ./CreateNTrackHistogramsApp -f $(get_mcHERWIG_ntupledumper d $sys) -c "d" -s $sys


#echo "run data"
#run ./CreateNTrackHistogramsApp -d -f $(get_data_ntupledumper) -c "17" -s "FlavourTagging_Nominal"
#echo "run mc Pythia, Nominal only"
#run ./CreateNTrackHistogramsApp -f $(get_mc_ntupledumper d) -c "d" -s "FlavourTagging_Nominal"  #$ALL_SYSTS
#echo "run mc"
#run ./CreateNTrackHistogramsApp -f $(get_mc_ntupledumper) -s $ALL_SYSTS
#echo 'run HERWIG mc'
#run ./CreateNTrackHistogramsApp -f $(get_mcHERWIG_ntupledumper) -s "FlavourTagging_Nominal" 
#echo 'run SHERPA mc'
#run ./CreateNTrackHistogramsApp -f $(get_mcSHERPA_ntupledumper) -s "FlavourTagging_Nominal" 

wait

#if [[ "$RUNMODE" == "send2ge_Reweighting" ]]; then
#    echo "Please wait for the end of the jobs and run:"
#    echo "root -l -b -q calculate_jetntrackreweighting.C"
#    echo "root -l -b -q data_mc_comparison_pteta_rew.cpp"
#else
#    echo "Calculate reweighting."
#    run root -l -q -b calculate_jetntrackreweighting.C
#    echo "Creating Data-MC comparison plots after PtEta reweighting(standard binning)."
#    run root -l -q -b data_mc_comparison_pteta_rew.cpp
#fi
