#!/bin/bash

for i in {1..1000}
do
   PBSFILE="run_bootstrap.sh $i"
   qsub -P atlas -l cvmfs=1 -l h_rt=3:00:00 -l h_vmem=4000M -l h_fsize=100M -M $JOB_MAIL -m a -e /nfs/dust/atlas/user/saimpert/log/btag_TemplateFit -o /nfs/dust/atlas/user/saimpert/log/btag_TemplateFit -cwd ${PBSFILE}
done

