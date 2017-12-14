index=0
arr[0]=0

find ./bootstrap/ -name 'template_fit*' -size -9M | while read line; do
   echo "Processing file '$line'"
   NUMBER=$(echo $line | tr -dc '0-9')

   if echo ${arr[@]} | grep -q -w ${NUMBER}; then 
     echo "${NUMBER} skipped because already resent"
   else
     arr[index]=${NUMBER}
     index=${index}+1
     PBSFILE="run_bootstrap.sh ${NUMBER}"
     echo ${PBSFILE}
     qsub -P atlas -l cvmfs=1 -l h_rt=3:00:00 -l h_vmem=1000M -l h_fsize=100M -e /nfs/dust/atlas/user/saimpert/log/btag_TemplateFit -o /nfs/dust/atlas/user/saimpert/log/btag_TemplateFit -cwd ${PBSFILE}
     sleep 2 
   fi
done
