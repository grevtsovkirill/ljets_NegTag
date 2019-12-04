#!/usr/bin/python
 
import os, math, sys, subprocess, argparse

parser = argparse.ArgumentParser(description='submission.')
parser.add_argument('--compaigne','-c', type=str,
                    help='compaigne: a or d')
parser.add_argument('--inType','-i', type=str,
                    help='type: data/ mc - P, H, S')
args = parser.parse_args()
compaigne=args.compaigne
inType=args.inType

if not compaigne and not inType: 
    print 'empty, specify arguments'
    sys.exit()

print 'Comp=',compaigne,', inType= ',inType
mclist =['P','H','S']
datalist = ['15','16','17','all']
loop_over =[]
if inType in mclist :
    loop_over = range(0,13)

if inType in datalist :
    print 'there'
    
for i in loop_over:
    var = "pbs_files/JZ"+str(loop_over[i])+"Wa_FlavourTagging_JET_EtaIntercalibration_NonClosure__1down.pbs"
    #print var
    os.system("python ../helpers/htc_one.py -i "+var)



