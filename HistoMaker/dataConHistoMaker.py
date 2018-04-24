## trackjetstudy.py
## Cut and storing of interesting trackjet kinematics (release 20.7)
## \author Nathan Grieser
from ROOT import *
from math import *
with open('dataRFL.txt') as f:
	mylist = f.read().splitlines()
GeV = 1000.

def main():

	for j in mylist:
		rootfile = j

		print "Creating Histograms"
		save_me =  TFile("HistoFiles/ConHistoFile" + rootfile + ".root",'recreate')
	
		# Book histograms of interesting kinematics.
		hpT       = TH1F( 'hpT', 'Leading Track Jet p_{T}', 600, 0, 3000)
		heta      = TH1F( 'heta', 'Leading Track Jet eta', 100, -4, 4)
	        hphi      = TH1F( 'hphi', 'Leading Track Jet phi', 100, -4, 4)
		hMV2c10   = TH1F( 'hMV2c10', 'Track Jet MV2c10 Weight', 100, -1, 1)
        	hMV2c10F  = TH1F( 'hMV2c10F', 'Track Jet MV2c10Flip Weight', 100, -1, 1)
        	hntrk     = TH1F( 'hntrk', 'Leading Track Jet Track Multiplicity', 30, 0, 30)
        	hNjet	  = TH1F( 'hNjet' , 'Leading Track Jet Multiplicity', 15, 0, 15)
		hDL1	  = TH1F( 'hDL1' , 'Leading Track Jet DL1 Weight', 100, -10, 10)
		hDL1F	  = TH1F( 'hDL1F', 'Leading Track Jet DL1Flip Weight', 100, -10, 10)



		studyfile = TFile("../DumpedNtuplestest/" + rootfile + ".root")
		nominal = studyfile.Get("FlavourTagging_Nominal")

		
		entries = nominal.GetEntriesFast()
		print entries
		num_events = 0.

		for jentry in xrange(entries):
			if nominal.LoadTree(jentry) < 0:
				break

			if nominal.GetEntry(jentry) <= 0:
				continue

			# Print to make sure the script is running still
			if jentry % 50000 == 0:
				print "Event", jentry, "/", entries

			data_weight = nominal.trackdata_evtweight[0]
			mc_weight = nominal.mc_evtweight
			PU_weight = nominal.evtpuw
			JVT_weight = nominal.evtJVTw
			weight = mc_weight * PU_weight * JVT_weight #*data_weight
			
								
			num_events += 1.
			

			ltjindex = 0 #Index of the leading track jet.

			ltweight = 1.#weight * data_weight[leadingtrackjetindex]
            		weightj15 = 1.#weight * nominal.evtHLTj15ps
            		weightj25 = weight * nominal.evtHLTj25ps
            		weightj60 = weight * nominal.evtHLTj60ps
            		weightj110 = weight * nominal.evtHLTj110ps
            		weightj175 = weight * nominal.evtHLTj175ps
            		weightj380 = weight * nominal.evtHLTj380ps

            			#Filling of histograms

                        if ((nominal.HLTj25 == 1) and ((nominal.trackjetpt[ltjindex] < 20.) and (nominal.trackjetpt[ltjindex] > 10.))):
                        	hpT.Fill(nominal.trackjetpt[ltjindex], weightj25)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weightj25)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weightj25)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weightj25)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weightj25)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weightj25)
                        	hNjet.Fill(nominal.ntrackjets_event, weightj25)
				hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weightj25)
				hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weightj25)

                        if ((nominal.HLTj60 == 1) and ((nominal.trackjetpt[ltjindex] < 50.) and (nominal.trackjetpt[ltjindex] > 20.))):
                        	hpT.Fill(nominal.trackjetpt[ltjindex], weightj60)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weightj60)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weightj60)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weightj60)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weightj60)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weightj60)
                        	hNjet.Fill(nominal.ntrackjets_event, weightj60)
                                hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weightj60)
                                hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weightj60)

                        if ((nominal.HLTj110 == 1) and ((nominal.trackjetpt[ltjindex] < 100.) and (nominal.trackjetpt[ltjindex] > 50.))):
                        	hpT.Fill(nominal.trackjetpt[ltjindex], weightj110)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weightj110)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weightj110)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weightj110)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weightj110)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weightj110)
                                hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weightj110)
                                hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weightj110)
                        	hNjet.Fill(nominal.ntrackjets_event, weightj110)

                        if ((nominal.HLTj175 == 1) and ((nominal.trackjetpt[ltjindex] < 150.) and (nominal.trackjetpt[ltjindex] > 100.))):
                        	hpT.Fill(nominal.trackjetpt[ltjindex], weightj175)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weightj175)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weightj175)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weightj175)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weightj175)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weightj175)
                        	hNjet.Fill(nominal.ntrackjets_event, weightj175)
                                hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weightj175)
                                hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weightj175)

                        if ((nominal.HLTj380 == 1) and (nominal.trackjetpt[ltjindex] > 150.)):
                        	hpT.Fill(nominal.trackjetpt[ltjindex], weightj380)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weightj380)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weightj380)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weightj380)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weightj380)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weightj380)
                        	hNjet.Fill(nominal.ntrackjets_event, weightj380)
				hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weightj380)
                                hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weightj380)






		studyfile.Close()
		save_me.Write()
		save_me.Close()


		print "Done with "+ rootfile

if __name__ =='__main__':
	main()




