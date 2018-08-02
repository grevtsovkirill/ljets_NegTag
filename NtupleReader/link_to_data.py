#!/usr/bin/python

import os, math, sys, subprocess, argparse
parser = argparse.ArgumentParser(description='submission.')
parser.add_argument('--systematics','-s', type=str,
                    help='systtype')
args = parser.parse_args()
syst_name=args.systematics
comp="d"

## prepare correct file names:
# find . -name "data_d__0.root" |  xargs rename data_d__0.root data_d.root data_d__0.*
# find . -name "mc_d__0.root"| xargs rename mc_d__0.root mc_d.root mc_d__0.*

toReplaceFile="FlavourTagging_Nominal"
print syst_name
if "subleading" in syst_name:
    toReplaceFile="subleadingjet"
    print "link to subleading"

if syst_name == "FlavourTagging_Nominal":
    print "no neeed for link for nominal"
    sys.exit()

os.chdir(syst_name)
print os.getcwd()
os.system("ln -s ../"+toReplaceFile+"/data_"+comp+".root data_"+comp+".root")
os.system("ls .")
os.chdir("../")
