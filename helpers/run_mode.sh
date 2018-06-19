# define function for switching to parallel processing (might be refined at some point)
#
# options e.g. ./run.sh <parallel>
# w/o argument fall back to serial mode

[[ -d log ]] || mkdir log

RUNSCHROOT="../helpers/run_schroot.sh"
RUNSCRIPT="../helpers/run.sh"
JOB_MAIL="kirill.grevtsov@desy.de"
OUTPUT_DIR="log/"
export here=$PWD

run() {
    if [[ "$RUNMODE" == "parallel" ]]; then
	updateCurrent
	[[ -z $MAXJOBS ]] && MAXJOBS=4
	while [[ $RM_CURRENT -ge $MAXJOBS ]]; do
	    sleep 5
	    updateCurrent
	done

	echo "enable parallel mode $period$slice"
	$@ > log/$LOGNAME.log &

    elif [[ "$RUNMODE" == "echo" ]]; then
	echo "enable echo mode"
	echo "$@"
    elif [[ "$RUNMODE" == "sbatch" ]]; then
	echo "enable sbatch mode"
	sbatch $RUNSCHROOT $RUNSCRIPT "$@"

    # ----------------------------------------------------------------
    # for grid engine batch system: split by period (NtupleDumper ONLY)
    # ----------------------------------------------------------------
    elif [[ "$RUNMODE" == "send2ge_NtupleDumper" ]]; then
        echo "enable grid engine mode for NtupleDumper (one job per period)"


        # determine slice
        args=("$@")
        var="0"
	var_c="_"
	sys_val=""
        for ((i=0; i<${#args[@]}; i++)); do
           if [[ "${args[i]}" == "-ps" ]]; then
               var=${args[i+1]}
	       var_c='_data'
           fi
           if [[ "${args[i]}" == "-s" ]]; then
               sys_val=${args[i+1]}
           fi
           if [[ "${args[i]}" == "-c" ]]; then
               var_c=${args[i+1]}
	  #     echo $var_c
              break
           fi
        done
	var+=$var_c
	var+=$sys_val
	#echo 'file = '$var
        # trick to identify if NtupleDumper is ran 
        if [[ "$var" == "0" ]]; then 
           echo "ERROR: send2ge applies to NtupleDumpler only. Exiting program ..."
           exit 1 
        fi

        # create job file
        PBSDIR="pbs_files"
        PBSFILE="$PBSDIR/$var.pbs"
        mkdir -p "$PBSDIR"
        cp ../setup.sh $PBSFILE 

        # adding some lines
        echo " " >> "$PBSFILE"
        echo "cd "$here >> "$PBSFILE"
        echo " " >> "$PBSFILE"
        echo "$@" >> "$PBSFILE"

        # create log directory
        OUTPUT_DIR="log/"
        mkdir -p "$OUTPUT_DIR" 

	#echo "here"
        # send job
	#qsub -P atlas -l cvmfs=1 -l h_rt=24:00:00 -l h_vmem=8000M -l h_fsize=80000M -M $JOB_MAIL -m a -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE
	#condor_qsub -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE
    # -----------------------------------------------------------------------
    # for grid engine batch system: split in period + syst (NtupleDumper ONLY)
    # ------------------------------------------------------------------------
    elif [[ "$RUNMODE" == "send2ge++_NtupleDumper" ]]; then
        echo "enable grid engine ++ mode for NtupleDumper (one job per period and per syst)"

        # determine position of -s flag
        args=("$@")
        s_index=0
        for ((j=0; j<${#args[@]}; j++)); do
           if [[ "${args[j]}" == "-s" ]]; then
              s_index=$j
              break
           fi
        done

        # determine slice
        var="0"
	var_c="_"
        ps_index=0
        d_flag=0
        for ((i=0; i<${#args[@]}; i++)); do
           if [[ "${args[i]}" == "-d" ]]; then
               d_flag=1
	       var_c='_data'
           fi
           if [[ "${args[i]}" == "-ps" ]]; then
              ps_index=$i
              var=${args[i+1]}
           fi
           if [[ "${args[i]}" == "-c" ]]; then
               var_c=${args[i+1]}
	  #     echo $var_c
              break
           fi
        done
	var+=$var_c

        # trick to identify if NtupleDumper is ran
        if [[ "$var" == "0" ]]; then 
           echo "ERROR: send2ge++ applies to NtupleDumpler only. Exiting program ..."
           exit 1 
        fi

        # loop on systematics
        for syst in $(getxAODsysts); do
           # for data, skip if it is different than nominal
           if [[ "$d_flag" == "1" ]] && [[ "$syst" != "FlavourTagging_Nominal" ]] ; then
	     echo "skipping running on systematics for data"
           else 
             # create 1 job file per systematic
             PBSDIR="pbs_files"
             PBSFILE="$PBSDIR"/"$var"_"$syst".pbs
             mkdir -p "$PBSDIR"
             cp ../setup.sh $PBSFILE 

             # adding some lines
             echo " " >> "$PBSFILE"
             echo "cd "$here >> "$PBSFILE"
             echo " " >> "$PBSFILE"
	     echo "${@:1:$s_index+1} $syst ${@:$ps_index+1} -split " >> "$PBSFILE"

             # create log directory
             OUTPUT_DIR="log/"
             mkdir -p "$OUTPUT_DIR" 

             # send job
             #qsub -P atlas -l cvmfs=1 -l h_rt=24:00:00 -l h_vmem=8000M -l h_fsize=80000M -M $JOB_MAIL -m a
	     #condor_qsub -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE
           fi
        done

    # -----------------------------------------------------------------------
    # for grid engine batch system: split in syst (Reweighting ONLY)
    # -----------------------------------------------------------------------
    elif [[ "$RUNMODE" == "send2ge_Reweighting" ]]; then
        echo "enable grid engine syst mode for reweighting (1 job per systematic and 2 bootstrap replicas ran per job for nominal)"

        # determine position of -s and -d flag
        args=("$@")
        s_index=0
        d_flag=0
        suffix="mc16"
        for ((j=0; j<${#args[@]}; j++)); do
           if [[ "${args[j]}" == "-d" ]]; then
              d_flag=1
              suffix="data"
           fi
           if [[ "${args[j]}" == "-s" ]]; then
              s_index=$j
              break
           fi
           if [[ "${args[j]}" == "-c" ]]; then
               var_c=${args[j+1]}
	       echo $var_c
              break
           fi
        done
	suffix+=$var_c

        # loop on systematics
        for syst in $(getxAODsystsAndOthers); do
           # for data, skip if it is different than nominal
           if [[ "$d_flag" == "1" ]] && [[ "$syst" != "FlavourTagging_Nominal" ]] ; then
	     echo "skipping running on systematics for data"
           else 
            if [[ "$syst" != "FlavourTagging_Nominal" ]] ; then
               # create 1 job file per systematic
               PBSDIR="pbs_files"
               PBSFILE="$PBSDIR"/"$suffix"_"$syst".pbs
               mkdir -p "$PBSDIR"
               cp ../setup.sh $PBSFILE 
               # adding some lines
               echo " " >> "$PBSFILE"
               echo "cd "$here >> "$PBSFILE"
               echo " " >> "$PBSFILE"
               echo "${@:1:$s_index+1} $syst -split 0" >> "$PBSFILE"
               # create log directory
               OUTPUT_DIR="log/"
               mkdir -p "$OUTPUT_DIR" 
               # send job
               #qsub -P atlas -l cvmfs=1 -l h_rt=6:00:00 -l h_vmem=4000M -l h_fsize=1000M -M $JOB_MAIL -m a -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE
             # bootstrap replica splitting
             else
               for k in {1..500} 
               do
                 # create 1 job file per systematic and per 2 bootstrap
                 PBSDIR="pbs_files"
                 PBSFILE="$PBSDIR"/"$suffix"_"$syst"_"$k".pbs
                 mkdir -p "$PBSDIR"
                 cp ../setup.sh $PBSFILE 
                 # adding some lines
                 echo " " >> "$PBSFILE"
		 echo "cd "$here >> "$PBSFILE"
		 echo " " >> "$PBSFILE"
                 echo "${@:1:$s_index+1} $syst -split $k" >> "$PBSFILE"
                 # create log directory
                 OUTPUT_DIR="log/"
                 mkdir -p "$OUTPUT_DIR" 
                 # send job
                 #qsub -P atlas -l cvmfs=1 -l h_rt=6:00:00 -l h_vmem=4000M -l h_fsize=1000M -M $JOB_MAIL -m a -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE
               done 
             fi

           fi
        done

    # -----------------------------------------------------------------------
    # for grid engine batch system: split in syst - NO BOOTSTRAP (Reweighting ONLY)
    # -----------------------------------------------------------------------
    elif [[ "$RUNMODE" == "send2ge_Reweighting_noBootstrap" ]]; then
        echo "enable grid engine syst mode for reweighting (1 job per systematic, no run on bootstrap replicas)"

        # determine position of -s and -d flag
        args=("$@")
        s_index=0
        d_flag=0
        suffix="mc"
	var_c=""
        for ((j=0; j<${#args[@]}; j++)); do
           if [[ "${args[j]}" == "-d" ]]; then
              d_flag=1
              suffix="data"
           fi
           if [[ "${args[j]}" == "-c" ]]; then
               var_c=${args[j+1]}
           fi
           if [[ "${args[j]}" == "-s" ]]; then
              s_index=$j
              break
           fi
        done

	echo $var_c
	alt_gen=""
	if [[ "$@" = *"W_H"* ]]; then
	    alt_gen="Herwig"
	fi
        # loop on systematics
        for syst in $(getxAODsystsAndOthers); do
           # for data, skip if it is different than nominal
           if [[ "$d_flag" == "1" ]] && [[ "$syst" != "FlavourTagging_Nominal" ]] ; then
	     echo "skipping running on systematics for data"
           elif [[ "$@" = *"W_H"* ]] && [[ "$syst" != "FlavourTagging_Nominal" ]] ; then
	     echo "skipping running on systematics for Herwig"
           else 
             # create 1 job file per systematic
             PBSDIR="pbs_files"
             PBSFILE="$PBSDIR"/"$suffix""$alt_gen"_"$var_c"_"$syst".pbs
             mkdir -p "$PBSDIR"
             cp ../setup.sh $PBSFILE 
             # adding some lines
             echo " " >> "$PBSFILE"
             echo "cd "$here >> "$PBSFILE"
             echo " " >> "$PBSFILE"
	     if [[ "$d_flag" == "1" ]] || [[ $@ = *"-f"* ]] ; then
		 echo "${@:1:$s_index+1} $syst -split 0" >> "$PBSFILE"
	     else
		 echo "${@:1:$s_index+1} $syst -f $(get_mc_ntupledumper $var_c $syst) -split 0" >> "$PBSFILE"
	     fi
             # create log directory
             OUTPUT_DIR="log/"
             mkdir -p "$OUTPUT_DIR" 
             # send job
             #qsub -P atlas -l cvmfs=1 -l h_rt=6:00:00 -l h_vmem=4000M -l h_fsize=8000M -M $JOB_MAIL -m a -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE
	     #condor_qsub -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE
           fi
        done

    # -----------------------------------------------------------------------
    # for grid engine batch system: no bootstrap (NtupleReader)
    # -----------------------------------------------------------------------
    elif [[ "$RUNMODE" == "send2ge_NtupleReader_noBootstrap" ]]; then
        echo "enable grid engine mode for NtupleReader (no run on bootstrap replicas)"

        # determine position of -s flag
        args=("$@")
        s_index=0
        d_flag=0
        suffix="mc"
	var_c=""
        for ((j=0; j<${#args[@]}; j++)); do
           if [[ "${args[j]}" == "-d" ]]; then
              d_flag=1
              suffix="data"
           fi
           if [[ "${args[j]}" == "-c" ]]; then
               var_c=${args[j+1]}
           fi
           if [[ "${args[j]}" == "-s" ]]; then
              s_index=$j
              break
           fi
        done

        # create job file
        PBSDIR="pbs_files"
        PBSFILE="$PBSDIR"/"$suffix"_"$var_c"_"${args[$s_index+1]}".pbs
        mkdir -p "$PBSDIR"
        cp ../setup.sh $PBSFILE 
        # adding some lines
        echo " " >> "$PBSFILE"
        echo "cd "$here >> "$PBSFILE"
        echo " " >> "$PBSFILE"
        #echo "$@" >> "$PBSFILE"
	#echo " " >> "$PBSFILE"
	if [[ "$d_flag" == "1" ]] ; then
	    echo "${@:1:$s_index+1} $syst -split 0" >> "$PBSFILE"
	else
	    echo "${@:1:$s_index+1} $syst -f $(get_mc_ntupledumper $var_c $syst) -split 0" >> "$PBSFILE"
	fi
        OUTPUT_DIR="log/"
        mkdir -p "$OUTPUT_DIR" 
        # send job
        #qsub -P atlas -l cvmfs=1 -l h_rt=12:00:00 -l h_vmem=6000M -l h_fsize=2000M -M $JOB_MAIL -m a -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE

    # -----------------------------------------------------------------------
    # for grid engine batch system: bootstrap splitting (NtupleReader)
    # -----------------------------------------------------------------------
    elif [[ "$RUNMODE" == "send2ge_NtupleReader" ]]; then
        echo "enable grid engine mode for NtupleReader (1 job per bootstrap)"

        # determine position of -s flag
        args=("$@")
        s_index=0
        d_flag=0
        suffix="mc"
	var_c=""
        for ((j=0; j<${#args[@]}; j++)); do
           if [[ "${args[j]}" == "-d" ]]; then
              d_flag=1
              suffix="data"
           fi
           if [[ "${args[j]}" == "-c" ]]; then
               var_c=${args[j+1]}
           fi
	   if [[ "${args[j]}" == "-s" ]]; then
              s_index=$j
              break
           fi
        done
        # normal execution
        if [[ "${args[$s_index+1]}" != "FlavourTagging_Nominal" ]] && [[ "${args[$s_index+1]}" != "subleadingjet" ]] ; then
          # create job file
          PBSDIR="pbs_files"
          PBSFILE="$PBSDIR"/"$suffix"_"${args[$s_index+1]}".pbs
          mkdir -p "$PBSDIR"
          cp ../setup.sh $PBSFILE 
          # adding some lines
          echo " " >> "$PBSFILE"
          echo "cd "$here >> "$PBSFILE"
          echo " " >> "$PBSFILE"
          echo "$@" >> "$PBSFILE"
          # create log directory
          OUTPUT_DIR="log/"
          mkdir -p "$OUTPUT_DIR" 
          # send job
          #qsub -P atlas -l cvmfs=1 -l h_rt=12:00:00 -l h_vmem=6000M -l h_fsize=2000M -M $JOB_MAIL -m a -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE

        # bootstrap replica splitting
        else
          for k in {0..1000} 
          do
            # create 1 job file per 1 bootstrap. index 0 is for nominal
            PBSDIR="pbs_files"
            PBSFILE="$PBSDIR"/"$suffix"_"${args[$s_index+1]}"_"$k".pbs
            mkdir -p "$PBSDIR"
            cp ../setup.sh $PBSFILE 
            # adding some lines
            echo " " >> "$PBSFILE"
            echo "cd "$here >> "$PBSFILE"
            echo " " >> "$PBSFILE"
	    echo "$@ -split $k" >> "$PBSFILE"
            # create log directory
            OUTPUT_DIR="log/"
            mkdir -p "$OUTPUT_DIR" 
            # send job
            qsub -P atlas -l cvmfs=1 -l h_rt=12:00:00 -l h_vmem=6000M -l h_fsize=2000M -M $JOB_MAIL -m a -e $OUTPUT_DIR -o $OUTPUT_DIR -cwd $PBSFILE
          done 
        fi

   # else

    else
	echo "enable serial mode"
	$@
    fi

}

updateCurrent(){
    RM_CURRENT=$(jobs -rp | wc -l)
}

# get dynamical all processed samples in NtupleDumper
PERIODFILE="../NtupleDumper/.periods.auto"
get_data_ntupledumper(){
    SAMPLES=""
    while read period; do
	SAMPLES+=" ../NtupleDumper/res/data$period.root"
    done < $PERIODFILE
    echo $SAMPLES
}

SLICEFILE="../NtupleDumper/.slices.auto"
get_mc_ntupledumper(){
    SAMPLES=""
    mcTYPE=$1
    isSplit='_'
    isSplit+=$2

    while read slice; do
	SAMPLES+=" ../NtupleDumper/res/mc16$mcTYPE"_"$slice$isSplit.root"
    done < $SLICEFILE
    echo $SAMPLES
#mc16a_JZ0W.root
#mc16d_JZ4W_FlavourTagging_JET_JER_SINGLE_NP__1up.root
}

SLICEFILE_H="../NtupleDumper/.slicesHERWIG.auto"
get_mcHERWIG_ntupledumper(){
    SAMPLES=""
    mcTYPE=$1
    isSplit='_'
    isSplit+=$2
    while read slice; do
	SAMPLES+=" ../NtupleDumper/res/mc16$mcTYPE"_"$slice$isSplit.root"
    done < $SLICEFILE_H
    echo $SAMPLES
}

SLICEFILE_S="../NtupleDumper/.slicesSHERPA.auto"
get_mcSHERPA_ntupledumper(){
    SAMPLES=""
    while read slice; do
	SAMPLES+=" ../NtupleDumper/res/mc$slice.root"
    done < $SLICEFILE_S
    echo $SAMPLES
}

getCleanSysts(){
    while read line; do
	case "$line" in \#*) continue ;; esac
	[[ -z "$line" ]] && continue

	echo "$line"
    done < ../config/systematics.txt    
}

getxAODsysts(){
    while read mode name type var; do
	if [[ "$mode" == "xAOD" ]];then
	    echo "$name"
	fi
    done < <(getCleanSysts)
}

getxAODsystsAndOthers(){
    while read mode name type var; do
	if [[ "$mode" == "xAOD" ]];then
	    echo "$name"
	fi

    done < <(getCleanSysts)
}
