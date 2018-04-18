#!/bin/bash

# include run function
RUNMODE="$1"
source ../helpers/run_mode.sh

# create output dir
[[ -e res ]] || mkdir res
[[ -e log ]] || mkdir log

PERIODFILE=".periods.auto"
SLICEFILE=".slices.auto"
SLICEFILE_H=".slicesHERWIG.auto"
SLICEFILE_S=".slicesSHERPA.auto"
SYSTFILE=".systematics.auto"
rm $PERIODFILE $SLICEFILE $SLICEFILE_H $SLICEFILE_S $SYSTFILE 2> /dev/null
#rm log/* 2> /dev/null

ALL_SYSTS=""
for syst in $(getxAODsysts); do
    ALL_SYSTS+=" $syst"
    echo $syst >> $SYSTFILE
done
# # run Pythia
while read slice files; do
   case "$slice" in \#*) continue ;; esac
   [ -z "$slice" ] && continue
   LOGNAME=$slice
   #run ./NtupleDumperApp -s $ALL_SYSTS -ps $slice -f $files
   run ./NtupleDumperApp -s "FlavourTagging_Nominal" -ps $slice -f $files -c "a"
   echo $slice >> $SLICEFILE
done < ../config/mc_files_pythia.txt
#done < ../config/mc_files_pythiaNEW.txt
slice=""

# # ##run mc (HERWIG) - cannot be run in send2ge++ mode at the moment
# while read slice files; do
#    case "$slice" in \#*) continue ;; esac
#    [ -z "$slice" ] && continue
#    LOGNAME=$slice
#    run ./NtupleDumperApp -s "FlavourTagging_Nominal" -ps $slice -f $files
#    echo $slice >> $SLICEFILE_H
# done < ../config/mc_files_16_herwig.txt
# slice=""

##run mc (SHERPA) - cannot be run in send2ge++ mode at the moment
# while read slice files; do
#    case "$slice" in \#*) continue ;; esac
#    [ -z "$slice" ] && continue
#    LOGNAME=$slice
#    run ./NtupleDumperApp -s "FlavourTagging_Nominal" -ps $slice -f $files
#    echo $slice >> $SLICEFILE_S
# done < ../config/mc_files_16_sherpa.txt
# slice=""

#run data
# while read period files; do
#     case "$period" in \#*) continue ;; esac
#     [ -z "$period" ] && continue
#     LOGNAME=$period
#     run ./NtupleDumperApp -d -s "FlavourTagging_Nominal" -ps $period -f $files
#     echo $period >> $PERIODFILE
# #done < ../config/data_files.txt
# done < ../config/data_files_16.txt
# period=""

wait
echo "done"
