#!/usr/bin/python
 
import os, math, sys, subprocess, argparse

parser = argparse.ArgumentParser(description='Look up the path to each file of a sample and create a list to it.\nIMPORTANT NOTE: You need a valid grid token and rucio set up for this script, this is not checked by the script.')
parser.add_argument('--inputfile','-i', type=str,
                    help='File with list of grid samples')
args = parser.parse_args()
in_file=args.inputfile

if not in_file: 
    in_file = 'test.list'
    print 'empty, use test.list'



#if content.find("mc16") 
ref = [
#--------------------------
["D15",   276073, 276954], #  51.6461 pb-1 (GRL starts at 276262)
["E15",   278727, 279928], #  435.605 pb-1                       
["F15",   279932, 280422], #  301.693 pb-1                       
["G15",   280423, 281075], #  724.399 pb-1                       
["H15",   281130, 281411], #  264.362 pb-1                       
["J15",   282625, 284484], #  1435.25 pb-1                       
# total_15 = 3212.96 pb-1                                        
#--------------------------                                      
#["H16",   305359, 305359], # low mu run not use                 
["A16",   297730, 300287],  #  552.738 pb-1 -> run<297730 not in GRL
["B16",   300345, 300908],  # 1949.59  pb-1                         
["C16",   301912, 302393],  # 2872.72  pb-1                         
["D16",   302737, 303560],  # 4626.62  pb-1                         
["E16",   303638, 303892],  # 1481.01  pb-1                         
["F16",   303943, 304494],  # 3398.89  pb-1                         
["G16",   305291, 306714],  # 3827.95  pb-1                         
["I16",   307124, 308084],  # 5802.86  pb-1                         
["K16",   309311, 309759],  # 2198.23  pb-1                         
["L16",   310015, 311481],  # 6151.19  pb-1                         
# -> total_16 = 32861.6 pb-1 (Sum=32861.798)                                                                                                                                             
#-------------------------------------------                        
["A17",   324320, 325558],  #                                       
["B17",   325713, 328393],  #                                       
["C17",   329385, 330470],  #                                       
["D17",   330857, 332304],  #                                       
["E17",   332720, 334779],  #                                       
["F17",   334842, 335290],  #                                       
#["G17",   335302, 335302],  # high mu run, not used for physics    
["H17",   336497, 336782],  #                                       
["I17",   336832, 337833],  #                                       
["K17",   338183, 340453],  #                                       
#["N17",   341257, 341649],  # low mu run, not used                 
# -> total_17 = 43593.8 pb-1 (Sum=76155.598) 
# MC samples - Pythia
["JZ0W",  361020, 361020],
["JZ1W",  361021, 361021],
["JZ2W",  361022, 361022],
["JZ3W",  361023, 361023],
["JZ4W",  361024, 361024],
["JZ5W",  361025, 361025],
["JZ6W",  361026, 361026],
["JZ7W",  361027, 361027],
["JZ8W",  361028, 361028],
["JZ9W",  361029, 361029],
["JZ10W", 361030, 361030],
["JZ11W", 361031, 361031],
["JZ12W", 361032, 361032],

]


y_input=''
def lineExtractor(y_input):
        p = subprocess.Popen("rucio list-file-replicas --rse DESY-HH_LOCALGROUPDISK "+y_input,shell=True,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        line3=''
        for line in p.stdout.readlines():
            if "+" in line or  "SCOPE" in line: 
                continue
            line1 = line[line.find('2880/dq2'):]
            line2= line1.replace('\n','')
            line2= line2.replace('|','')
            line2= line2.replace(' ','')
            line2= line2.replace('2880/','/pnfs/desy.de/atlas/')
            line3+=line2+' '
        return line3

isMC = False
p1516 = False
p17 = False
isData = False
                                                     
fname = in_file
array_of_slices = []
import rucio.client
ruc = rucio.client.Client()

slice_files = []
list_dsid_file = []

counter = 0
with open(fname) as f:
    content = f.readlines()
    for x in content:
        y= x.replace('\n','')
        if 'mc16' in y : 
            isMC = True
            if 'mc16a' in y : p1516 = True
            if 'mc16d' in y : p17 = True
        if 'data17' in y : 
            isData = True
            p17 = True
        if ('data16' in y) or ('data15' in y) : 
            isData = True  
            p1516 = True 
                
        print y
        dsid = y[y.find('13TeV.')+6:y.find('.DAOD')]
        if isData : 
            dsid = y[y.find('13TeV.')+8:y.find('.DAOD')]
        print 'dsid=',dsid

        list_dsid_file.append((dsid,y))
        if isMC:
            for i_ref in ref : 
                if int(dsid) >= i_ref[1] and int(dsid) <= i_ref[2] :
                    slice_name=i_ref[0]
                #print i_ref[0]
                    break
                slice=slice_name+'\t'+' '+lineExtractor(y) #.join(slice_files)
                array_of_slices.append(slice)
                print 'over line',y #,'\n slice: ',slice

#print slice_files

print isMC,p17,p1516,isData

if isData:
    for i_ref in ref : 
        if 'JZ' not in i_ref[0]: 
            line_per_period=''
            for x in list_dsid_file:
            # for x in list_dsid_file:
            #     if
                if int(x[0])>= i_ref[1] and int(x[0])<=i_ref[2]:
                    #print 'in slice',i_ref[0]
                    line_per_period+=lineExtractor(x[1])
                    #print x
                
            if len(line_per_period)>3:
                #print 'b',line_per_period,'e'
                slice_name=i_ref[0]
                slice=slice_name+'\t'+' '+line_per_period #.join(slice_files)                                                                                                                                                                
                array_of_slices.append(slice)
            #print 'next',i_ref[0]
            #for i 

# recreate file
if isData and p1516      : filename0="data_files_15and16.txt"
if isData and p17      : filename0="data_files_17.txt"
if isMC and p1516      : filename0="mc16a_files.txt"
if isMC and p17      : filename0="mc16d_files.txt"

outfile = open(filename0, 'w')
outfile.write('\n'.join(array_of_slices))
outfile.close()



