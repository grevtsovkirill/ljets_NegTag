#!/bin/bash
##
## Slurm parameters
##
#SBATCH --partition=lsschaile
#SBATCH --export=NONE  # do not export current environemnt to job (recommended) 
##SBATCH --get-user-env
#SBATCH --mem=2000mb
#SBATCH --time=01:29:59
#SBATCH -o /home/b/Balthasar.Schachtner/tmp/slurm-test.out.%N.%j   # out file name (in submission dir)
#SBATCH -J negTag

date
hostname

echo schroot -c slc6 -p -- /bin/bash  "$@"
schroot -c slc6 -p -- /bin/bash "$@"

date
