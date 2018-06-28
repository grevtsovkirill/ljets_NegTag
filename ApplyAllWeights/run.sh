#!/bin/bash

# include run function and sample getter
RUNMODE="$1"
source ../helpers/run_mode.sh

# create log directory
[[ -e log ]] || mkdir log

ALL_SYSTS=""
for syst in $(getxAODsysts); do
ALL_SYSTS+=" $syst"
done

# Add PU SF uncertainty
ALL_SYSTS+=" FlavourTagging_PRW_DATASF__1down"
ALL_SYSTS+=" FlavourTagging_PRW_DATASF__1up"
# Add JVT uncertainty
ALL_SYSTS+=" FlavourTagging_JVT_effSF__1down"
ALL_SYSTS+=" FlavourTagging_JVT_effSF__1up"
# Add Conversion uncertainty
ALL_SYSTS+=" conversions__1down"
ALL_SYSTS+=" conversions__1up"
# Add Hadronic interactions uncertainty
ALL_SYSTS+=" hadronic__1down"
ALL_SYSTS+=" hadronic__1up"
# Add LL uncertainty
ALL_SYSTS+=" longlivedparticles"

#echo "run data"
#run ./ApplyAllWeightsApp -d -f $(get_data_ntupledumper) -s "FlavourTagging_Nominal"
#echo "run mc"
#run ./ApplyAllWeightsApp -f $(get_mc_ntupledumper) -s $ALL_SYSTS
echo 'run HERWIG mc'
run ./ApplyAllWeightsApp -f $(get_mcHERWIG_ntupledumper) -s "FlavourTagging_Nominal"
#echo "run mc"
#run ./ApplyAllWeightsApp -f $(get_mc_ntupledumper) -s "FlavourTagging_Nominal"

wait

#if [[ "$RUNMODE" == "send2ge_Reweighting" ]]; then
    echo "Please wait for the end of the jobs and run:"
    echo "root -l -b -q data_mc_comparison_all_rew.cpp"
#else
#    echo "Creating Data-MC comparison plots after all reweighting(standard binning)."
#    run root -l -q -b data_mc_comparison_all_rew.cpp
#fi
