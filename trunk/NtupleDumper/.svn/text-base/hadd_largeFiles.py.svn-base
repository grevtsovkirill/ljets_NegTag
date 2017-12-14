import ROOT 
import os, sys

print 'Merging for mcJZ%sW' % sys.argv[1]

print "Max tree size",ROOT.TTree.GetMaxTreeSize() 
ROOT.TTree.SetMaxTreeSize(200000000000) 
print "Updated tree size",ROOT.TTree.GetMaxTreeSize() 

rm = ROOT.TFileMerger(False) 
rm.SetFastMethod(True) 


path = 'res/'

file_output = 'res/mcJZ%sW.root' % sys.argv[1]

file_list = [] 
for path, dirs, files in os.walk(path):
  for filename in files:
    if ('mcJZ%sW_FlavourTagging_' % sys.argv[1]) in filename: file_list.append(path+filename)

print "Input file list:",file_list
print "Output file:",file_output

for F in file_list: 

    print "Adding ->",F 
    rm.AddFile(F) 

rm.OutputFile(file_output) 
rm.Merge()
