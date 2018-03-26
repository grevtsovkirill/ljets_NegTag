## trackjetstudy.py
## Cut and storing of interesting trackjet kinematics (release 20.7)
## \author Nathan Grieser
from ROOT import *
from math import *
with open('mcRFL.txt') as f:
	mylist = f.read().splitlines()
GeV = 1000.

def main():

	for j in mylist:
		rootfile = j

		print "Creating Histograms"
		save_me =  TFile("HistoFiles/FracFile" + rootfile + ".root",'recreate')

		Pt_edges = [10, 20, 50, 100, 150, 300, 500, 1000]
            	Eta_edges = [0, 1.2]
	
		# Book histograms of interesting kinematics.
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

		studyfile = TFile("../DumpedNtuples/" + rootfile + ".root")
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
			elif (nominal.trackflavor[ltjindex] == 4):
				tjflav = "c"
			elif (nominal.trackflavor[ltjindex] == 5):
				tjflav = "b"

            		weightj25 = weight# * nominal.evtHLTj25ps
            		weightj60 = weight# * nominal.evtHLTj60ps
            		weightj110 = weight# * nominal.evtHLTj110ps
            		weightj175 = weight# * nominal.evtHLTj175ps
            		weightj380 = weight# * nominal.evtHLTj380ps

            			#Filling of histograms

                        if (((rootfile == "mcJZ0W") or (rootfile == "mcJZ1W") or (rootfile == "mcJZ2W")) and nominal.trackjetpt[ltjindex]>100.):
                                continue


                        if ((nominal.HLTj25 == 1) and ((nominal.trackjetpt[ltjindex] < 20.) and (nominal.trackjetpt[ltjindex] > 10.))):
                        	if (tjflav == "l" ):
					if (nominal.trackjeteta[ltjindex] < 1.2):
                        			hMV2c10_l_1_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
					else:
						hMV2c10_l_1_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        	elif (tjflav == "b" ):
					if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_b_1_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_b_1_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        	elif (tjflav == "c" ):
					if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_c_1_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_c_1_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        if ((nominal.HLTj60 == 1) and ((nominal.trackjetpt[ltjindex] < 50.) and (nominal.trackjetpt[ltjindex] > 20.))):
                                if (tjflav == "l" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_l_2_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_l_2_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                elif (tjflav == "b" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_b_2_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_b_2_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                elif (tjflav == "c" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_c_2_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_c_2_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        if ((nominal.HLTj110 == 1) and ((nominal.trackjetpt[ltjindex] < 100.) and (nominal.trackjetpt[ltjindex] > 50.))):
                                if (tjflav == "l" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_l_3_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_l_3_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                elif (tjflav == "b" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_b_3_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_b_3_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                elif (tjflav == "c" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_c_3_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_c_3_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        if ((nominal.HLTj175 == 1) and ((nominal.trackjetpt[ltjindex] < 150.) and (nominal.trackjetpt[ltjindex] > 100.))):
				if (tjflav == "l" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_l_4_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_l_4_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                elif (tjflav == "b" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_b_4_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_b_4_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                elif (tjflav == "c" ):
                                        if (nominal.trackjeteta[ltjindex] < 1.2):
                                                hMV2c10_c_4_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        else:
                                                hMV2c10_c_4_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                        if ((nominal.HLTj380 == 1) and (nominal.trackjetpt[ltjindex] > 150.)):
				if (nominal.trackjetpt[ltjindex] < 300.):
					if (tjflav == "l" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_l_5_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                        else:
	                                                hMV2c10_l_5_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                elif (tjflav == "b" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_b_5_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                        else:
	                                                hMV2c10_b_5_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                elif (tjflav == "c" ):
	                                        if (nominal.trackjeteta[ltjindex] < 1.2):
	                                                hMV2c10_c_5_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
	                                        else:
	                                                hMV2c10_c_5_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                elif ((nominal.trackjetpt[ltjindex] > 300.) and (nominal.trackjetpt[ltjindex] < 500.)):
                                        if (tjflav == "l" ):
                                                if (nominal.trackjeteta[ltjindex] < 1.2):
                                                        hMV2c10_l_6_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                else:
                                                        hMV2c10_l_6_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        elif (tjflav == "b" ):
                                                if (nominal.trackjeteta[ltjindex] < 1.2):
                                                        hMV2c10_b_6_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                else:
                                                        hMV2c10_b_6_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        elif (tjflav == "c" ):
                                                if (nominal.trackjeteta[ltjindex] < 1.2):
                                                        hMV2c10_c_6_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                else:
                                                        hMV2c10_c_6_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                elif (nominal.trackjetpt[ltjindex] > 500.):
                                        if (tjflav == "l" ):
                                                if (nominal.trackjeteta[ltjindex] < 1.2):
                                                        hMV2c10_l_7_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                else:
                                                        hMV2c10_l_7_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        elif (tjflav == "b" ):
                                                if (nominal.trackjeteta[ltjindex] < 1.2):
                                                        hMV2c10_b_7_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                else:
                                                        hMV2c10_b_7_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                        elif (tjflav == "c" ):
                                                if (nominal.trackjeteta[ltjindex] < 1.2):
                                                        hMV2c10_c_7_1.Fill(nominal.trackjet_MV2c10[ltjindex], weight)
                                                else:
                                                        hMV2c10_c_7_2.Fill(nominal.trackjet_MV2c10[ltjindex], weight)







		studyfile.Close()
		save_me.Write()
		save_me.Close()


		print "Done with "+ rootfile

if __name__ =='__main__':
	main()




