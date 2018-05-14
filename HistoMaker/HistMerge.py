## mcHistMerge.py
## Merge histograms from different data periods (r21)
## \author Nathan Grieser
from ROOT import *
from math import *
with open('mcRFL.txt') as f1:
	mcRFL = f1.read().splitlines()
with open('mcHL.txt') as f5:
        mcConHL = f5.read().splitlines()
with open('mcSFHistoList.txt') as f2:
    	mcSFHL = f2.read().splitlines()
with open('dataRFL.txt') as f3:
        dataRFL = f3.read().splitlines()
with open('DataSFHistList.txt') as f4:
        dataSFHL = f4.read().splitlines()
with open('dataHL.txt') as f4:
        dataConHL = f4.read().splitlines()

mc = True #Choose data/MC
suffix = ""
prefix = ""
# Only one of these should be set as true:
Control = True 
ScaleFac = False 
if Control:
	mcHL = mcConHL
	dataHL = dataConHL
	prefix = "ConHistoFile"
if ScaleFac:
	mcHL = mcSFHL
	dataHL = dataSFHL
	suffix = "SFhistos"


def main():
	if mc:
		save_me =  TFile("HistoFiles/mc" + prefix + suffix + "M16d.root","RECREATE")
		h_baserootfile = TFile("HistoFiles/" + prefix + "mcJZ0Wd" + suffix + ".root") # This should be the first file that would be in your root file list
		for histname in mcHL:
			baseHist = h_baserootfile.Get(histname)
			for index, filename in enumerate(mcRFL): 
				h_rootfile = TFile("HistoFiles/" + prefix + filename + suffix + ".root")
				if index == 0: 
					pass
				print filename
				print histname 
	    			addHist = h_rootfile.Get(histname)
				baseHist.Add(addHist, 1.0)
	    			h_rootfile.Close()
			baseHist.SetDirectory(save_me)
    		save_me.Write()
    		save_me.Close()           


	if (mc == False):
		save_me =  TFile("HistoFiles/data" + prefix + suffix + "M16.root",'recreate')
		h_baserootfile = TFile("HistoFiles/" + prefix + "dataA16" + suffix + ".root") # This should be the first file that would be in your root file list
		for histname in dataHL: # Set up base histograms to be added to
			baseHist = h_baserootfile.Get(histname)
        		for index, filename in enumerate(dataRFL): # Skip first file as it is the base
                		if index == 0: 
					pass
                		h_rootfile = TFile("HistoFiles/" + prefix + filename + suffix + ".root")
				print filename 
				print histname
                		addHist = h_rootfile.Get(histname)
                		baseHist.Add(addHist, 1.0)
                		h_rootfile.Close()
        		baseHist.SetDirectory(save_me)
		save_me.Write(histname)
		save_me.Close()



















if __name__ =='__main__':
    main()



