#!/usr/bin/python
 
import os, math, sys, subprocess, argparse

from time import sleep

parser = argparse.ArgumentParser(description='submission.')
parser.add_argument('--inType','-t', type=str,
                    help='type: data/ mc')
parser.add_argument('--systematics','-s', type=str,
                    help='systtype')
parser.add_argument('--bootstrap','-b', type=str,
                    help='bootstrap ')
args = parser.parse_args()
inType=args.inType
syst_name=args.systematics
bootstrap=args.bootstrap


fname='test1.txt'
#
cwd = os.getcwd()

#difference  ../NtupleDumper/res/
#mc16a_ JZ11W _FlavourTagging_Nominal.root 
#data B16 .root
d_option = ""
fname_1 = ""
fname_2 = ""
comp = "d"
mode_type = "xAOD"

#syst_name = "FlavourTagging_Nominal"
#syst_name = "generator"
systList= ['generator','notrackrew','notrackrew_subleadingjet','subleadingjet_generator','conversions__1up','conversions__1down','hadronic__1up','hadronic__1down','longlivedparticles']


if inType == 'data':
    fname='../NtupleDumper/.periods.auto'
    d_option = "-d"
    fname_1 = "data"
    fname_2 = ""

if inType == "mc":
    fname='../NtupleDumper/.slices.auto'
    d_option = ""
    fname_1 = "mc16"+comp+"_"
    fname_2 = "_"+syst_name

if not inType:
    print 'specify arguments'
    sys.exit()

filename0=""

if syst_name in systList:
    print "here"
    mode_type = "doJets"
    fname_2 = "_FlavourTagging_Nominal"

if "generator" in syst_name:
    fname="../NtupleDumper/.slicesHERWIG.auto" 

#if syst_name == "subleadingjet":
if "subleadingjet" in syst_name:
    mode_type = "doSubLeadJet"
    fname_2 = "_FlavourTagging_Nominal"
    if inType == 'data':
        fname_2 = ""

if  "mcstat" in syst_name or "datastat" in syst_name:
    mode_type = "bootstrap"
    fname_2 = "_FlavourTagging_Nominal"
    if inType == 'data':
        fname_2 = ""

def create_pbs(i):
    filename0='pbs_files/'+fname_1+syst_name+'_'+comp+'_'+i+'.pbs'
    outfile = open(filename0, 'w')
    command = []
    command.append("#!/bin/sh  ")
    command.append("export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase")
    command.append("source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh")
    command.append("cd "+cwd)
    command.append("lsetup root")
    #var="./NtupleReaderApp -c a -p "+i+" "+d_option+" -s "+syst_name+"  -m "+mode_type+" -f ../NtupleDumper/res/"+fname_1+i+fname_2+".root "
    var="./NtupleReaderApp -c "+comp+" "+d_option+" -s "+syst_name+"  -m "+mode_type+" "+files_list
    command.append(var)
    outfile.write('\n'.join(command))
    outfile.close()
    print var
    #os.system("python ../helpers/htc_one.py -i "+filename0)
    sub_var="python ../helpers/htc_one.py -i "
    #sub_var= "qsub -P atlas -e log/ -o log/ -cwd "
    #os.system(sub_var+filename0)


def bootstrap_create_pbs(j,files_list):
    bs_option="-split "+j        
    filename0='pbs_files/'+fname_1+syst_name+'_'+comp+'_'+j+'.pbs'
    outfile = open(filename0, 'w')
    command = []
    command.append("#!/bin/sh  ")
    command.append("export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase")
    command.append("source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh")
    command.append("cd "+cwd)
    command.append("lsetup root")
    var="./NtupleReaderApp -c "+comp+" "+d_option+" -s "+syst_name+"  -m "+mode_type+" "+files_list+" "+bs_option
    command.append(var)
    outfile.write('\n'.join(command))
    outfile.close()
    print var
    #os.system("python ../helpers/htc_one.py -i "+filename0)
    sub_var="python ../helpers/htc_one.py -i "
    #sub_var= "qsub -P atlas -e log/ -o log/ -cwd "
    os.system(sub_var+filename0)
    


if bootstrap:
    print 'bootstrap '
    files_list="-f "
    files_path="../NtupleDumper/res/"
    with open(fname) as f:
        cont =f.read().splitlines()
        for i in cont:
            files_list+=files_path+fname_1+i+fname_2+".root "
   # print files_list
    if "0" in bootstrap:
        bootstrap_create_pbs(str(0),files_list)
    else:
        for j in range(0,1001):
            bootstrap_create_pbs(str(j),files_list)
            sleep(1)
else:
    with open(fname) as f:
        cont =f.read().splitlines()
        for i in cont:
            print i
            i_var=i
            create_pbs(i_var)

#./NtupleReaderApp -c a -p D15 -d -s FlavourTagging_Nominal  -m xAOD -f ../NtupleDumper/res/dataD15.root
