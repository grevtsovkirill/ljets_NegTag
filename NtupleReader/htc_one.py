#!/usr/bin/python
 
import os, math, sys, subprocess, argparse
import datetime

parser = argparse.ArgumentParser(description='submission.')
parser.add_argument('--infile','-i', type=str,
                    help='')
args = parser.parse_args()
infile=args.infile
ts =datetime.datetime.now().strftime("%m_%d_%y%H_%M_%S")
if not infile: 
    print 'empty, specify arguments'
    sys.exit()
infile0=infile
infile= infile.replace('pbs_files/','')
infile= infile.replace('.pbs','')

if not os.path.exists("HTC/CMD"):
    os.makedirs("HTC/CMD")
if not os.path.exists("HTC/LOGS"):
    os.makedirs("HTC/LOGS")

filename0='HTC/CMD/'+infile+'_'+ts+'.cmd'
print filename0
outfile = open(filename0, 'w')
command = []
command.append("executable = "+infile0)
command.append("should_transfer_files = YES")
command.append("when_to_transfer_output = ON_EXIT")
command.append("universe        = vanilla ")
command.append("output          = HTC/LOGS/"+infile+"_"+ts+".out ")
command.append("error          = HTC/LOGS/"+infile+"_"+ts+".err ")
command.append("log          = HTC/LOGS/"+infile+"_"+ts+".log ")
command.append("notification    = never ")
command.append("queue  ")
#command.append
outfile.write('\n'.join(command))
outfile.close()
os.system("condor_submit "+filename0)
