#!/usr/bin/python
 
import os, math, sys, subprocess, argparse
#systList= ['FlavourTagging_Nominal','subleadingjet']
#systList= ['subleadingjet','notrackrew_subleadingjet']
#'notrackrew',
systList= ['generator','FlavourTagging_JET_EtaIntercalibration_NonClosure__1down','FlavourTagging_JET_EtaIntercalibration_NonClosure__1up','FlavourTagging_JET_GroupedNP_1__1down','FlavourTagging_JET_GroupedNP_1__1up','FlavourTagging_JET_GroupedNP_2__1down','FlavourTagging_JET_GroupedNP_2__1up','FlavourTagging_JET_GroupedNP_3__1down','FlavourTagging_JET_GroupedNP_3__1up','FlavourTagging_JET_RelativeNonClosure_MC16__1down','FlavourTagging_JET_RelativeNonClosure_MC16__1up','FlavourTagging_JET_JER_SINGLE_NP__1up','FlavourTagging_JET_JvtEfficiency__1up','conversions__1up','conversions__1down','hadronic__1up','hadronic__1down','longlivedparticles']
for i in systList:
    print i
    #os.system("python loop.py -s "+i+" -t mc")
    #os.system("python loop.py -s subleadingjet_"+i+" -t data -b 0")
    #os.system("python loop.py -s "+i+" -t data -b 0")
    #os.system("python ../merger.py -s "+i)
    #os.system("python ../link_to_data.py -s "+i)
    os.system("python ../link_to_data.py -s subleadingjet_"+i)
