## mcHistoMaker.py
## Cut and storing of interesting trackjet kinematics (release 21)
## \author Nathan Grieser
from ROOT import *
from math import *
with open('mcRFL.txt') as f:
	mylist = f.read().splitlines()
GeV = 1000.

def main():

	for rootfile in mylist:

            # Define pt and eta bin ranges


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
		hMV2c10_l  = TH1F( 'hMV2c10_l', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
        	hMV2c10F_l  = TH1F( 'hMV2c10F_l', 'Track Jet MV2c10Flip Weight, light jets', 100, -1, 1)        
		hMV2c10_b  = TH1F( 'hMV2c10_b', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10F_b  = TH1F( 'hMV2c10F_b', 'Track Jet MV2c10Flip Weight, b jets', 100, -1, 1)
		hMV2c10_c  = TH1F( 'hMV2c10_c', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
        	hMV2c10F_c  = TH1F( 'hMV2c10F_c', 'Track Jet MV2c10Flip Weight, c jets', 100, -1, 1)
                hDL1_l  = TH1F( 'hDL1_l', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1F_l  = TH1F( 'hDL1F_l', 'Track Jet DL1Flip Weight, light jets', 50, -5, 5)
                hDL1_b  = TH1F( 'hDL1_b', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1F_b  = TH1F( 'hDL1F_b', 'Track Jet DL1Flip Weight, b jets', 50, -5, 5)
                hDL1_c  = TH1F( 'hDL1_c', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1F_c  = TH1F( 'hDL1F_c', 'Track Jet DL1Flip Weight, c jets', 50, -5, 5)
                hMV2c10_l_highpt  = TH1F( 'hMV2c10_l_highpt', 'Track Jet MV2c10 Weight, light jets, highpt', 100, -1, 1)
                hMV2c10F_l_highpt  = TH1F( 'hMV2c10F_l_highpt', 'Track Jet MV2c10Flip Weight, light jets, highpt', 100, -1, 1)        
                hMV2c10_b_highpt  = TH1F( 'hMV2c10_b_highpt', 'Track Jet MV2c10 Weight, b jets, highpt', 100, -1, 1)
                hMV2c10F_b_highpt  = TH1F( 'hMV2c10F_b_highpt', 'Track Jet MV2c10Flip Weight, b jets, highpt', 100, -1, 1)
                hMV2c10_c_highpt  = TH1F( 'hMV2c10_c_highpt', 'Track Jet MV2c10 Weight, c jets, highpt', 100, -1, 1)
                hMV2c10F_c_highpt  = TH1F( 'hMV2c10F_c_highpt', 'Track Jet MV2c10Flip Weight, c jets, highpt', 100, -1, 1)
		hMV2c10_l_lowpt  = TH1F( 'hMV2c10_l_lowpt', 'Track Jet MV2c10 Weight, light jets, lowpt', 100, -1, 1)
                hMV2c10F_l_lowpt  = TH1F( 'hMV2c10F_l_lowpt', 'Track Jet MV2c10Flip Weight, light jets, lowpt', 100, -1, 1)        
                hMV2c10_b_lowpt  = TH1F( 'hMV2c10_b_lowpt', 'Track Jet MV2c10 Weight, b jets, lowpt', 100, -1, 1)
                hMV2c10F_b_lowpt  = TH1F( 'hMV2c10F_b_lowpt', 'Track Jet MV2c10Flip Weight, b jets, lowpt', 100, -1, 1)
                hMV2c10_c_lowpt = TH1F( 'hMV2c10_c_lowpt', 'Track Jet MV2c10 Weight, c jets, lowpt', 100, -1, 1)
                hMV2c10F_c_lowpt  = TH1F( 'hMV2c10F_c_lowpt', 'Track Jet MV2c10Flip Weight, c jets, lowpt', 100, -1, 1)
                hMV2c10_l_1_1  = TH1F( 'hMV2c10_l_1_1', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_2_1  = TH1F( 'hMV2c10_l_2_1', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_3_1  = TH1F( 'hMV2c10_l_3_1', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_4_1  = TH1F( 'hMV2c10_l_4_1', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_5_1  = TH1F( 'hMV2c10_l_5_1', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_6_1  = TH1F( 'hMV2c10_l_6_1', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_7_1  = TH1F( 'hMV2c10_l_7_1', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_1_2  = TH1F( 'hMV2c10_l_1_2', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_2_2  = TH1F( 'hMV2c10_l_2_2', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_3_2  = TH1F( 'hMV2c10_l_3_2', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_4_2  = TH1F( 'hMV2c10_l_4_2', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_5_2  = TH1F( 'hMV2c10_l_5_2', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_6_2  = TH1F( 'hMV2c10_l_6_2', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_l_7_2  = TH1F( 'hMV2c10_l_7_2', 'Track Jet MV2c10 Weight, light jets', 100, -1, 1)
                hMV2c10_b_1_1  = TH1F( 'hMV2c10_b_1_1', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_2_1  = TH1F( 'hMV2c10_b_2_1', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_3_1  = TH1F( 'hMV2c10_b_3_1', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_4_1  = TH1F( 'hMV2c10_b_4_1', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_5_1  = TH1F( 'hMV2c10_b_5_1', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_6_1  = TH1F( 'hMV2c10_b_6_1', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_7_1  = TH1F( 'hMV2c10_b_7_1', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_1_2  = TH1F( 'hMV2c10_b_1_2', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_2_2  = TH1F( 'hMV2c10_b_2_2', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_3_2  = TH1F( 'hMV2c10_b_3_2', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_4_2  = TH1F( 'hMV2c10_b_4_2', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_5_2  = TH1F( 'hMV2c10_b_5_2', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_6_2  = TH1F( 'hMV2c10_b_6_2', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_b_7_2  = TH1F( 'hMV2c10_b_7_2', 'Track Jet MV2c10 Weight, b jets', 100, -1, 1)
                hMV2c10_c_1_1  = TH1F( 'hMV2c10_c_1_1', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_2_1  = TH1F( 'hMV2c10_c_2_1', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_3_1  = TH1F( 'hMV2c10_c_3_1', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_4_1  = TH1F( 'hMV2c10_c_4_1', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_5_1  = TH1F( 'hMV2c10_c_5_1', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_6_1  = TH1F( 'hMV2c10_c_6_1', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_7_1  = TH1F( 'hMV2c10_c_7_1', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_1_2  = TH1F( 'hMV2c10_c_1_2', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_2_2  = TH1F( 'hMV2c10_c_2_2', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_3_2  = TH1F( 'hMV2c10_c_3_2', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_4_2  = TH1F( 'hMV2c10_c_4_2', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_5_2  = TH1F( 'hMV2c10_c_5_2', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_6_2  = TH1F( 'hMV2c10_c_6_2', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
                hMV2c10_c_7_2  = TH1F( 'hMV2c10_c_7_2', 'Track Jet MV2c10 Weight, c jets', 100, -1, 1)
		hDL1      = TH1F( 'hDL1' , 'Leading Track Jet DL1 Weight', 50, -5, 5)
                hDL1F     = TH1F( 'hDL1F', 'Leading Track Jet DL1Flip Weight', 50, -5, 5)
                hDL1_l_1_1  = TH1F( 'hDL1_l_1_1', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_2_1  = TH1F( 'hDL1_l_2_1', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_3_1  = TH1F( 'hDL1_l_3_1', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_4_1  = TH1F( 'hDL1_l_4_1', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_5_1  = TH1F( 'hDL1_l_5_1', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_6_1  = TH1F( 'hDL1_l_6_1', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_7_1  = TH1F( 'hDL1_l_7_1', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_1_2  = TH1F( 'hDL1_l_1_2', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_2_2  = TH1F( 'hDL1_l_2_2', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_3_2  = TH1F( 'hDL1_l_3_2', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_4_2  = TH1F( 'hDL1_l_4_2', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_5_2  = TH1F( 'hDL1_l_5_2', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_6_2  = TH1F( 'hDL1_l_6_2', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_l_7_2  = TH1F( 'hDL1_l_7_2', 'Track Jet DL1 Weight, light jets', 50, -5, 5)
                hDL1_b_1_1  = TH1F( 'hDL1_b_1_1', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_2_1  = TH1F( 'hDL1_b_2_1', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_3_1  = TH1F( 'hDL1_b_3_1', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_4_1  = TH1F( 'hDL1_b_4_1', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_5_1  = TH1F( 'hDL1_b_5_1', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_6_1  = TH1F( 'hDL1_b_6_1', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_7_1  = TH1F( 'hDL1_b_7_1', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_1_2  = TH1F( 'hDL1_b_1_2', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_2_2  = TH1F( 'hDL1_b_2_2', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_3_2  = TH1F( 'hDL1_b_3_2', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_4_2  = TH1F( 'hDL1_b_4_2', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_5_2  = TH1F( 'hDL1_b_5_2', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_6_2  = TH1F( 'hDL1_b_6_2', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_b_7_2  = TH1F( 'hDL1_b_7_2', 'Track Jet DL1 Weight, b jets', 50, -5, 5)
                hDL1_c_1_1  = TH1F( 'hDL1_c_1_1', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_2_1  = TH1F( 'hDL1_c_2_1', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_3_1  = TH1F( 'hDL1_c_3_1', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_4_1  = TH1F( 'hDL1_c_4_1', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_5_1  = TH1F( 'hDL1_c_5_1', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_6_1  = TH1F( 'hDL1_c_6_1', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_7_1  = TH1F( 'hDL1_c_7_1', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_1_2  = TH1F( 'hDL1_c_1_2', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_2_2  = TH1F( 'hDL1_c_2_2', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_3_2  = TH1F( 'hDL1_c_3_2', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_4_2  = TH1F( 'hDL1_c_4_2', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_5_2  = TH1F( 'hDL1_c_5_2', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_6_2  = TH1F( 'hDL1_c_6_2', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
                hDL1_c_7_2  = TH1F( 'hDL1_c_7_2', 'Track Jet DL1 Weight, c jets', 50, -5, 5)
	
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
	
			tjflav = ""
			if (nominal.trackflavor[ltjindex] == 0):
				tjflav = "l"
			if (nominal.trackflavor[ltjindex] == 4):
				tjflav = "c"
			if (nominal.trackflavor[ltjindex] == 5):
				tjflav = "b"

			ltweight = 1.#weight * data_weight[leadingtrackjetindex]
                	weighthj15 = 1.#weight * nominal.evtHLTj15ps
                        weighthj25 = 1.#weight * nominal.evtHLTj25ps
                        weighthj60 = 1.#weight * nominal.evtHLTj60ps
                        weighthj110 = 1.#weight * nominal.evtHLTj110ps
                        weighthj175 = 1.#weight * nominal.evtHLTj175ps
                 	weighthj380 = 1.#weight * nominal.evtHLTj380ps

            			#Filling of histograms




                        if ((nominal.HLTj25 == 1) and ((nominal.trackjetpt[ltjindex] < 20.) and (nominal.trackjetpt[ltjindex] > 10.))):
                        	hpT.Fill(nominal.trackjetpt[ltjindex], weight)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weight)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weight)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weight)
                        	hNjet.Fill(nominal.ntrackjets_event, weight)
                                hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)

                        	if (tjflav == "l" ):
                        		hMV2c10_l.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        		hMV2c10F_l.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
					hDL1_l.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
					hDL1F_l.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                        	if (tjflav == "b" ):
                        		hMV2c10_b.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        		hMV2c10F_b.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
					hDL1_b.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
					hDL1F_b.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                        	if (tjflav == "c" ):
                        		hMV2c10_c.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        		hMV2c10F_c.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_c.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_c.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)

                                if (tjflav == "l" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_l_1_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_l_1_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                                        else:
                                                hMV2c10_l_1_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_l_1_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                                elif (tjflav == "b" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_b_1_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_b_1_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        else:
                                                hMV2c10_b_1_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_b_1_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                elif (tjflav == "c" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_c_1_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_c_1_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        else:
                                                hMV2c10_c_1_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_c_1_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                        if ((nominal.HLTj60 == 1) and ((nominal.trackjetpt[ltjindex] < 50.) and (nominal.trackjetpt[ltjindex] > 20.))):
                        	hpT.Fill(nominal.trackjetpt[ltjindex], weight)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weight)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weight)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weight)
                        	hNjet.Fill(nominal.ntrackjets_event, weight)
                                hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "l" ):
                                        hMV2c10_l.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_l.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_l.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_l.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "b" ):
                                        hMV2c10_b.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_b.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_b.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_b.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "c" ):
                                        hMV2c10_c.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_c.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_c.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_c.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)

                                if (tjflav == "l" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_l_2_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_l_2_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                                        else:
                                                hMV2c10_l_2_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_l_2_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                                elif (tjflav == "b" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_b_2_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_b_2_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        else:
                                                hMV2c10_b_2_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_b_2_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                elif (tjflav == "c" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_c_2_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_c_2_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        else:
                                                hMV2c10_c_2_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_c_2_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                        if ((nominal.HLTj110 == 1) and ((nominal.trackjetpt[ltjindex] < 100.) and (nominal.trackjetpt[ltjindex] > 50.))):
                        	hpT.Fill(nominal.trackjetpt[ltjindex], weight)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weight)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weight)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weight)
                        	hNjet.Fill(nominal.ntrackjets_event, weight)
                                hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "l" ):
                                        hMV2c10_l.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_l.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_l.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_l.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "b" ):
                                        hMV2c10_b.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_b.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_b.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_b.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "c" ):
                                        hMV2c10_c.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_c.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_c.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_c.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)

                                if (tjflav == "l" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_l_3_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_l_3_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        else:
                                                hMV2c10_l_3_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_l_3_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                                elif (tjflav == "b" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_b_3_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_b_3_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        else:
                                                hMV2c10_b_3_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_b_3_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                elif (tjflav == "c" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_c_3_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_c_3_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        else:
                                                hMV2c10_c_3_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_c_3_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                        if ((nominal.HLTj175 == 1) and ((nominal.trackjetpt[ltjindex] < 150.) and (nominal.trackjetpt[ltjindex] > 100.))):
				hpT.Fill(nominal.trackjetpt[ltjindex], weight)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weight)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weight)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weight)
                        	hNjet.Fill(nominal.ntrackjets_event, weight)
                                hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "l" ):
                                        hMV2c10_l.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_l.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_l.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_l.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "b" ):
                                        hMV2c10_b.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_b.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_b.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_b.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "c" ):
                                        hMV2c10_c.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_c.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_c.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_c.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)

                                if (tjflav == "l" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_l_4_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_l_4_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                                        else:
                                                hMV2c10_l_4_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_l_4_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

                                elif (tjflav == "b" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_b_4_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_b_4_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        else:
                                                hMV2c10_b_4_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_b_4_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                elif (tjflav == "c" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_c_4_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_c_4_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        else:
                                                hMV2c10_c_4_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                hDL1_c_4_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	
                        if ((nominal.HLTj380 == 1) and (nominal.trackjetpt[ltjindex] > 150.)):
                        	hpT.Fill(nominal.trackjetpt[ltjindex], weight)
                        	heta.Fill(nominal.trackjeteta[ltjindex], weight)
                        	hphi.Fill(nominal.trackjetphi[ltjindex], weight)
                        	hMV2c10.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        	hMV2c10F.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                        	hntrk.Fill(nominal.trackjetntrk[ltjindex], weight)
                        	hNjet.Fill(nominal.ntrackjets_event, weight)
                                hDL1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                hDL1F.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "l" ):
                                        hMV2c10_l.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_l.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_l.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_l.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "b" ):
                                        hMV2c10_b.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_b.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_b.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_b.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)
                                if (tjflav == "c" ):
                                        hMV2c10_c.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        hMV2c10F_c.Fill(nominal.trackjet_MV2c10Flip[ltjindex], weight)
                                        hDL1_c.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
                                        hDL1F_c.Fill(nominal.trackjet_DL1Flip_w[ltjindex], weight)


                                if (nominal.trackjetpt[ltjindex] < 300.):
	                                if (tjflav == "l" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_l_5_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_l_5_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	
	                                        else:
	                                                hMV2c10_l_5_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_l_5_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	
	                                elif (tjflav == "b" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_b_5_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_b_5_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	                                        else:
	                                                hMV2c10_b_5_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_b_5_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	                                elif (tjflav == "c" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_c_5_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_c_5_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	                                        else:
	                                                hMV2c10_c_5_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_c_5_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

				elif ((nominal.trackjetpt[ltjindex] > 300.) and (nominal.trackjetpt[ltjindex] < 500.)):
	                                if (tjflav == "l" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_l_6_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_l_6_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	
	                                        else:
	                                                hMV2c10_l_6_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_l_6_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	
	                                elif (tjflav == "b" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_b_6_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_b_6_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	                                        else:
	                                                hMV2c10_b_6_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_b_6_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	                                elif (tjflav == "c" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_c_6_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_c_6_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	                                        else:
	                                                hMV2c10_c_6_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_c_6_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)

				elif (nominal.trackjetpt[ltjindex] > 500.):
	                                if (tjflav == "l" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_l_7_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_l_7_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	
	                                        else:
	                                                hMV2c10_l_7_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_l_7_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	
	                                elif (tjflav == "b" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_b_7_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_b_7_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	                                        else:
	                                                hMV2c10_b_7_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_b_7_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	                                elif (tjflav == "c" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_c_7_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_c_7_1.Fill(nominal.trackjet_DL1_w[ltjindex], weight)
	                                        else:
	                                                hMV2c10_c_7_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                                hDL1_c_7_2.Fill(nominal.trackjet_DL1_w[ltjindex], weight)




		studyfile.Close()
		save_me.Write()
		save_me.Close()


		print "Done with "+ rootfile

if __name__ =='__main__':
	main()




