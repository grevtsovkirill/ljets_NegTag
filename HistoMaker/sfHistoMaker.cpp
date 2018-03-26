#include <TH2.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <TBranch.h>

void sfHistoMaker(){
	const TString DumpedMC[12] = {"JZ0W", "JZ1W", "JZ2W", "JZ3W", "JZ4W", "JZ5W", "JZ6W", "JZ7W", "JZ8W", "JZ10W", "JZ11W", "JZ12W"};
	const TString DumpedDT[10] = {"D15", "G15", "E15", "F15", "H15", "I16", "Ia16", "L16", "La16", "J15"};
        const int n_DT = 10;
	const int n_MC = 12;
        const int n_pt = 8;
        const int n_eta = 2;
        const int n_WP = 4;
	const int n_taggers = 2;
	const int n_flav = 3;
        const double pt_lowedges[n_pt+1] = {10, 15, 20, 50, 100, 150, 300, 500, 3000};
        const double eta_lowedges[n_eta+1] = {0, 1.2, 2.5};
        const TString WP[n_WP] = {"85", "77", "70", "60"};
        const TString Tagger[n_taggers] = {"MV2c10" , "DL1"}
	const TString Tag[2] = {"", "Neg"};
        const TString Flav[n_flav] = {"l", "b", "c"};
	bool mc = false; // Set either data or MC

// Set up loop over mc files.
if (mc){
for (int i = 0; i < n_MC; i++){
	
	TString mcfile = DumpedMC[i];
	TFile* f_mc = new TFile("../DumpedNtuples/mc" + mcfile + ".root", "read");
	TTree* nominal = (TTree*)f_mc->Get("FlavourTagging_Nominal");
	TFile* f_output = new TFile("HistoFiles/mc" + mcfile + "SFhistos.root", "recreate");
	int entries = nominal->GetEntriesFast();
	std::cout << "There are " << entries << "in slice " << DumpedMC[i] << std::endl;



	float mc_evtweight;
	float evtpuw;
	float evtJVTw;
	float trackjetpt[1];
	float trackjeteta[1];
	int trackflavor[1];
	int HLTj25;
	int HLTj60;
	int HLTj110;
	int HLTj175;
	int HLTj380;
	float trackjet_MV2c10[1];
	float trackjet_MV2c10Flip[1];
	float trackjet_DL1_w[1];
	float trackjet_DL1Flip_w[1];
	int trackjet_isMV2_Tagged[1];
	int trackjet_isMV2_Tagged[1];
	int trackjet_isDL1_Tagged[1];
	int trackjet_isDL1Flip_Tagged[1];	
	
	TBranch *b_mc_evtweight;
	TBranch *b_evtpuw;
	TBranch *b_evtJVTw;
	TBranch *b_trackjetpt;
	TBranch *b_trackjeteta;
	TBranch *b_trackflavor;
	TBranch *b_HLTj25;
        TBranch *b_HLTj60;
        TBranch *b_HLTj110;
        TBranch *b_HLTj175;
        TBranch *b_HLTj380;
	TBranch *b_trackjet_MV2c10;
	TBranch *b_trackjet_MV2c10Flip;
	TBranch *b_trackjet_DL1_w;
	TBranch *b_trackjet_DL1Flip_w;
	TBranch *b_trackjet_isMV2_Tagged;
	TBranch *b_trackjet_isMV2Flip_Tagged;
	TBranch *b_trackjet_isDL1_Tagged;
	TBranch	*b_trackjet_isDL1Flip_Tagged;


        nominal->SetBranchAddress("mc_evtweight", &mc_evtweight, &b_mc_evtweight);
        nominal->SetBranchAddress("evtpuw", &evtpuw, &b_evtpuw);
        nominal->SetBranchAddress("evtJVTw", &evtJVTw, &b_evtJVTw);
        nominal->SetBranchAddress("trackjetpt", &trackjetpt, &b_trackjetpt);
        nominal->SetBranchAddress("trackjeteta", &trackjeteta, &b_trackjeteta);
        nominal->SetBranchAddress("trackflavor", &trackflavor, &b_trackflavor);
        nominal->SetBranchAddress("HLTj25", &HLTj25, &b_HLTj25);
        nominal->SetBranchAddress("HLTj60", &HLTj60, &b_HLTj60);
        nominal->SetBranchAddress("HLTj110", &HLTj110, &b_HLTj110);
        nominal->SetBranchAddress("HLTj175", &HLTj175, &b_HLTj175);
        nominal->SetBranchAddress("HLTj380", &HLTj380, &b_HLTj380);
        nominal->SetBranchAddress("trackjet_MV2c10", &trackjet_MV2c10, &b_trackjet_MV2c10);
        nominal->SetBranchAddress("trackjet_MV2c10Flip", &trackjet_MV2c10Flip, &b_trackjet_MV2c10Flip);
        nominal->SetBranchAddress("trackjet_DL1_w", &trackjet_DL1_w, &b_trackjet_DL1_w);
        nominal->SetBranchAddress("trackjet_DL1Flip_w", &trackjet_DL1Flip_w, &b_trackjet_DL1Flip_w);
	nominal->SetBranchAddress("trackjet_isMV2_Tagged", &trackjet_isMV2_Tagged, &b_trackjet_isMV2_Tagged);
        nominal->SetBranchAddress("trackjet_isMV2Flip_Tagged", &trackjet_isMV2Flip_Tagged, &b_trackjet_isMV2Flip_Tagged);
        nominal->SetBranchAddress("trackjet_isDL1_Tagged", &trackjet_isDL1_Tagged, &b_trackjet_isDL1_Tagged);
        nominal->SetBranchAddress("trackjet_isDL1Flip_Tagged", &trackjet_isDL1Flip_Tagged, &b_trackjet_isDL1Flip_Tagged);

// Book the zillion histos we will need for scale factor calculations.
// One histogram is needed for each pt, eta, WP, and tag.`
// In MC, we need one of these for every flavor as well.  For control plots, use mc(data)HistMaker.py
        std::ofstream mcHL;
        mcHL.open ("mcSFHistoList.txt");
	std::map<TString,TH1D*> mch_ptetaWPtagF;
	for (int i = 1; i < n_pt + 1; i++){
	for (int j = 1; j < n_eta +1; j++){
	for (int k = 0; k < n_WP; k++){
	for (int l = 0; l < n_taggers; l++{
	for (int m = 0; m < 2; m++){
	for (int n = 0; n < n_flav; n++){
		TString histname = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "_" + Tagger[l] +  "_" + Tag[m] + "_" + Flav[n];
		TString histnamefail = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "_" + Tagger[l] + "_" + Tag[m] + "_" + Flav[n] + "_Fail";
		mcHL << histnamefail << std::endl;
		mcHL << histname << std::endl;
		mch_ptetaWPtagF[histname] = new TH1D(histname, histname, 20, -1, 1);
		mch_ptetaWPtagF[histnamefail] = new TH1D(histnamefail, histnamefail, 20, -1, 1);
		mch_ptetaWPtagF[histnamefail]->Sumw2();
		mch_ptetaWPtagF[histname]->Sumw2();
		}}}}}} // End histo booking loop	
	mcHL.close();



	// Loop over events and fill histograms
	for (int j = 0; j < entries; j++){
		if (j % 100000 == 0){ 
			std::cout << "Event  " << j << "/" << entries << std::endl; }
		nominal->GetEntry(j);
	        int Triggers[n_pt] = {HLTj25, HLTj25, HLTj60, HLTj110, HLTj175, HLTj380, HLTj380, HLTj380};

		// Set up needed variables/weights
		int ltj = 0; //Index for leading track jet
		float mc_weight = mc_evtweight;
		float PU_weight = evtpuw;
		float JVT_weight = evtJVTw;
		float mcw = mc_weight * PU_weight * JVT_weight;

		// Check pT bin
		float jetpt = trackjetpt[ltj];
		int pt_bin = 0;
		bool GoodTriggerEvent = true;
		int trigger = 0;
                if (jetpt > pt_lowedges[n_pt]){
                        continue;
                } // Skip jets that are above top pt bin edge.
		for (int i = 1; i < n_pt+1; ++i){
			if (jetpt <= pt_lowedges[i]){
			GoodTriggerEvent = true;
			pt_bin = i;
			int trigger = Triggers[i-1];
			if (trigger != 1){
				GoodTriggerEvent = false;
				}
			break;
				}
			    }
			  
		if (!GoodTriggerEvent)  { 
			//std::cout << "This event failed with Pt " << jetpt << "and pt_bin " << pt_bin<< "and trigger " << trigger << std::endl;
			continue;
			} // Skip event if trigger doesn't match pt bin or if low MC slice and pT is too high.
		// Check eta bin
		float jeteta = trackjeteta[ltj];
                int eta_bin = 0;
                for (int i = 1; i < n_eta+1; ++i){
                        if (jeteta < eta_lowedges[i]){
                        eta_bin = i;
                        break;
                            }
                          }
		// Check flavor
		TString flav = "";
		if (trackflavor[ltj] == 0){
                        }        flav = "l";
		if (trackflavor[ltj] == 4){
                                flav = "c";
			}
		if (trackflavor[ltj] == 5){
                                flav = "b";
			}

		// Determine Pass/Fail based on WP/Tagger
		std::map<TString,bool> PassTag;
		std::map<TString,bool> PassNegTag;
		for (int n = 0; n < n_WP; n++){
		for (int m = 0; m < n_taggers; m++){
			TString wp_name = WP[n];
			TString tag_name = Tagger[m];
			bool tagged = false;
			TString passfail = wp_name + "_" + tag_name;
			PassTag[passfail] = tagged;
			PassNegTag[passfail] = tagged;
			}} // End Pass/Fail loop.
		
		// Set up definition of passing or failing tags. 
		// THIS NEEDS TO BE ADJUSTED BY HAND WHEN ADDING NEW TAGGERS.
		// I propse that we leave this to be adjusted by hand, as the definitions to
		// "is tagged" changed quite a bit.
		// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarksRelease21
		int isMV2c10 = trackjet_isMV2_Tagged[ltj];
		int isMV2c10F = trackjet_isMV2Flip_Tagged[ltj];
		int isDL1 = trackjet_isDL1_Tagged[ltj];
		int isDL1F = trackjet_isDL1Flip_Tagged[ltj];
		if (isMV2c10 *= 2){
			(PassTag[85_MV2c10]) = true;}
		if (isMV2c10 *= 3){ 
			(PassTag[77_MV2c10]) = true;}
		if (isMV2c10 *= 5){
			(PassTag[70_MV2c10]) = true;}
		if (isMV2c10 *= 7){
			(PassTag[60_MV2c10]) = true;}
                if (isMV2c10F *= 2){
                        (PassNegTag[85_MV2c10]) = true;}
                if (isMV2c10F *= 3){ 
                        (PassNegTag[77_MV2c10]) = true;}
                if (isMV2c10F *= 5){
                        (PassNegTag[70_MV2c10]) = true;}
                if (isMV2c10F *= 7){
                        (PassNegTag[60_MV2c10]) = true;}
                if (isDL1 *= 2){
                        (PassTag[85_DL1]) = true;}
                if (isDL1 *= 3){   
                        (PassTag[77_DL1]) = true;}
                if (isDL1 *= 5){
                        (PassTag[70_DL1]) = true;}
                if (isDL1 *= 7){
                        (PassTag[60_DL1]) = true;}
                if (isDL1F *= 2){
                        (PassNegTag[85_DL1]) = true;}
                if (isDL1F *= 3){    
                        (PassNegTag[77_DL1]) = true;}
                if (isDL1F *= 5){
                        (PassNegTag[70_DL1]) = true;}
                if (isDL1F *= 7){
                        (PassNegTag[60_DL1]) = true;}


		// Determine which histograms to fill.  Tags must be true, using the pt and flavor bins from above as well.
		//std::cout << "Jet pT is  " << jetpt << std::endl;
		//std::cout << "PTbin  " << pt_bin << std::endl;
		//std::cout << "Etabin  " << eta_bin << std::endl;
		//std::cout << "Flavor   " << flav << std::endl;	
		for (int k = 0; k < n_WP; k++){
		for (int i = 0; i < n_taggers){
			TString wpname = WP[k];
			TString tagname = Tagger[i];
			TString PassFail = wpname + "_" + tagname;
			float filler = -99;
			float negfiller = -99;
			if (tagname == "MV2c10"){
				filler = trackjet_MV2c10[ltj];
				negfiller = trackjet_MV2c10Flip[ltj];}
			if (tagname == "DL1"){
				filler = trackjet_DL1[ltj];
				negfiller = trackjet_DL1Flip[ltj];}
			if (PassTag[PassFail] == true){
				TString fillname = "mch_" + std::to_string(pt_bin) + "_" + std::to_string(eta_bin) + "_" + WP[k] + "_" + Tagger[i] + "__" + flav;
				mch_ptetaWPtagF[fillname]->Fill(filler, mcw);}
			if (PassNegTag[PassFail] == true){
                                TString negfillname = "mch_" + std::to_string(pt_bin) + "_" + std::to_string(eta_bin) + "_" + WP[k] + "_" + Tagger[i] + "_Neg_" + flav;
                                mch_ptetaWPtagF[negfillname]->Fill(negfiller, mcw);}
                        if (PassTag[PassFail] == false){
                                TString fillname = "mch_" + std::to_string(pt_bin) + "_" + std::to_string(eta_bin) + "_" + WP[k] + "_" + Tagger[i] + "__" + flav + "_Fail";
                                mch_ptetaWPtagF[fillname]->Fill(filler, mcw);}
                        if (PassNegTag[PassFail] == false){
                                TString negfillname = "mch_" + std::to_string(pt_bin) + "_" + std::to_string(eta_bin) + "_" + WP[k] + "_" + Tagger[i] + "_Neg_" + flav + "_Fail";
                                mch_ptetaWPtagF[negfillname]->Fill(negfiller, mcw);}
                }} // End of filling loop.
			

}

		f_output->Write();
		f_output->Close();
}
}


if (!mc){
for (int i = 0; i < n_DT; i++){

        TString datafile = DumpedDT[i];
        TFile* f_data = new TFile("../DumpedNtuples/data" + datafile + ".root", "read");
        TTree* nominal = (TTree*)f_data->Get("FlavourTagging_Nominal");
        TFile* f_outputdt = new TFile("HistoFiles/data" + datafile + "SFhistos.root", "recreate");
        int entries = nominal->GetEntriesFast();
        std::cout << "There are  " << entries << "  events in period " << DumpedDT[i] << std::endl;



        float mc_evtweight;
        float evtpuw;
        float evtJVTw;
	float trackdata_evtweight[1];
        float trackjetpt[1];
        float trackjeteta[1];
        int trackflavor[1];
        int HLTj25;
        int HLTj60;
        int HLTj110;
        int HLTj175;
        int HLTj380;
	float evtHLTj25ps;
        float evtHLTj60ps;
        float evtHLTj110ps;
        float evtHLTj175ps;
        float evtHLTj380ps;
        float trackjet_MV2c10[1];
        float trackjet_MV2c10Flip[1];
        float trackjet_DL1_w[1];
        float trackjet_DL1Flip_w[1];
        int trackjet_isMV2_Tagged[1];
        int trackjet_isMV2_Tagged[1];
        int trackjet_isDL1_Tagged[1];
        int trackjet_isDL1Flip_Tagged[1];

        TBranch *b_mc_evtweight;
        TBranch *b_evtpuw;
        TBranch *b_evtJVTw;
	TBranch *b_trackdata_evtweight;
        TBranch *b_trackjetpt;
        TBranch *b_trackjeteta;
        TBranch *b_trackflavor;
        TBranch *b_HLTj25;
        TBranch *b_HLTj60;
        TBranch *b_HLTj110;
        TBranch *b_HLTj175;
        TBranch *b_HLTj380;
	TBranch *b_evtHLTj25ps;
        TBranch *b_evtHLTj60ps;
        TBranch *b_evtHLTj110ps;
        TBranch *b_evtHLTj175ps;
        TBranch *b_evtHLTj380ps;
        TBranch *b_trackjet_MV2c10;
        TBranch *b_trackjet_MV2c10Flip;
        TBranch *b_trackjet_DL1_w;
        TBranch *b_trackjet_DL1Flip_w;
        TBranch *b_trackjet_isMV2_Tagged;
        TBranch *b_trackjet_isMV2Flip_Tagged;
        TBranch *b_trackjet_isDL1_Tagged;
        TBranch *b_trackjet_isDL1Flip_Tagged;


        nominal->SetBranchAddress("mc_evtweight", &mc_evtweight, &b_mc_evtweight);
        nominal->SetBranchAddress("evtpuw", &evtpuw, &b_evtpuw);
        nominal->SetBranchAddress("evtJVTw", &evtJVTw, &b_evtJVTw);
        nominal->SetBranchAddress("trackdata_evtweight", &trackdata_evtweight, &b_trackdata_evtweight);
        nominal->SetBranchAddress("trackjetpt", &trackjetpt, &b_trackjetpt);
        nominal->SetBranchAddress("trackjeteta", &trackjeteta, &b_trackjeteta);
        nominal->SetBranchAddress("trackflavor", &trackflavor, &b_trackflavor);
        nominal->SetBranchAddress("HLTj25", &HLTj25, &b_HLTj25);
        nominal->SetBranchAddress("HLTj60", &HLTj60, &b_HLTj60);
        nominal->SetBranchAddress("HLTj110", &HLTj110, &b_HLTj110);
        nominal->SetBranchAddress("HLTj175", &HLTj175, &b_HLTj175);
        nominal->SetBranchAddress("HLTj380", &HLTj380, &b_HLTj380);
	nominal->SetBranchAddress("evtHLTj25ps", &evtHLTj25ps, &b_evtHLTj25ps);
        nominal->SetBranchAddress("evtHLTj60ps", &evtHLTj60ps, &b_evtHLTj60ps);
        nominal->SetBranchAddress("evtHLTj110ps", &evtHLTj110ps, &b_evtHLTj110ps);
        nominal->SetBranchAddress("evtHLTj175ps", &evtHLTj175ps, &b_evtHLTj175ps);
        nominal->SetBranchAddress("evtHLTj380ps", &evtHLTj380ps, &b_evtHLTj380ps);
        nominal->SetBranchAddress("trackjet_MV2c10", &trackjet_MV2c10, &b_trackjet_MV2c10);
        nominal->SetBranchAddress("trackjet_MV2c10Flip", &trackjet_MV2c10Flip, &b_trackjet_MV2c10Flip);
        nominal->SetBranchAddress("trackjet_DL1_w", &trackjet_DL1_w, &b_trackjet_DL1_w);
        nominal->SetBranchAddress("trackjet_DL1Flip_w", &trackjet_DL1Flip_w, &b_trackjet_DL1Flip_w);
        nominal->SetBranchAddress("trackjet_isMV2_Tagged", &trackjet_isMV2_Tagged, &b_trackjet_isMV2_Tagged);
        nominal->SetBranchAddress("trackjet_isMV2Flip_Tagged", &trackjet_isMV2Flip_Tagged, &b_trackjet_isMV2Flip_Tagged);
        nominal->SetBranchAddress("trackjet_isDL1_Tagged", &trackjet_isDL1_Tagged, &b_trackjet_isDL1_Tagged);
        nominal->SetBranchAddress("trackjet_isDL1Flip_Tagged", &trackjet_isDL1Flip_Tagged, &b_trackjet_isDL1Flip_Tagged);



	//Fill histograms and write histogram list
	
	std::ofstream dataHL;
	dataHL.open ("DataSFHistList.txt");
        std::map<TString,TH1D*> datah_ptetaWPtag;
        for (int i = 1; i < n_pt + 1; i++){
        for (int j = 1; j < n_eta +1; j++){
        for (int k = 0; k < n_WP; k++){
	for (int l = 0; l < n_taggers; l++){
        for (int m = 0; m < 2; m++){
                TString histname = "datah_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "_" + Tagger[l] + "_" + Tag[m];
               TString histnamefail = "datah_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "_" + Tagger[l] + "_" + Tag[m] + "_Fail";
		dataHL << histnamefail << std::endl;
		dataHL << histname << std::endl;
		datah_ptetaWPtag[histnamefail] = new TH1D(histnamefail, histnamefail, 20, -1, 1);
                datah_ptetaWPtag[histname] = new TH1D(histname, histname, 20, -1, 1);
		datah_ptetaWPtag[histnamefail]->Sumw2();
                datah_ptetaWPtag[histname]->Sumw2();
                }}}}} // End histo booking loop 
	dataHL.close();


        for (int j = 0; j < entries; j++){
                if (j % 100000 == 0){
                        std::cout << "Event  " << j << "/" << entries << std::endl; }
                nominal->GetEntry(j);
                int Triggers[n_pt] = {HLTj25, HLTj25, HLTj60, HLTj110, HLTj175, HLTj380, HLTj380, HLTj380};
                float Prescales[n_pt] = {evtHLTj25ps, evtHLTj25ps, evtHLTj60ps, evtHLTj110ps, evtHLTj175ps, evtHLTj380ps, evtHLTj380ps, evtHLTj380ps};
                int ltj = 0; //Index for leading track jet
                float mc_weight = mc_evtweight;
                float PU_weight = evtpuw;
                float JVT_weight = evtJVTw;
                float mcw = mc_weight * PU_weight * JVT_weight;

                float jetpt = trackjetpt[ltj];
                int pt_bin = 0;
		float prescale = 0;
		bool GoodTriggerEvent = true;
		if (jetpt > pt_lowedges[n_pt]){
			continue;
		} // Skip jets that are above top pt bin edge.
                for (int i = 1; i < n_pt+1; ++i){
                        if (jetpt <= pt_lowedges[i]){
			GoodTriggerEvent = true;
                        pt_bin = i;
                        int trigger = Triggers[i-1];
			//std::cout << trigger << i-1 << endl;
			prescale = Prescales[i-1];
                        if (trigger != 1){
                                GoodTriggerEvent = false;
                                }
                        break;
				}
                            }
                float dw = mc_weight + prescale;          
                if (!GoodTriggerEvent)  {
			//std::cout << "This event failed with Pt " << jetpt << " and bin " << pt_bin << " and trigger " << HLTj25 << "_" << HLTj60 << "_" << HLTj110 << "_" << HLTj175 << "_" << HLTj380 << "_" << failed_events << std::endl;
                        continue;
                        } // Skip event if trigger doesn't match pt bin or if low MC slice and pT is too high.

                float jeteta = trackjeteta[ltj];
                int eta_bin = 0;
                for (int i = 1; i < n_eta+1; ++i){
                        if (jeteta < eta_lowedges[i]){
                        eta_bin = i;
                        break;
                            }
                          }

                // Determine Pass/Fail based on WP/Tagger
                std::map<TString,bool> PassTag;
                std::map<TString,bool> PassNegTag;
                for (int n = 0; n < n_WP; n++){
                for (int m = 0; m < n_taggers; m++){
                        TString wp_name = WP[n];
                        TString tag_name = Tagger[m];
                        bool tagged = false;
                        TString passfail = wp_name + "_" + tag_name;
                        PassTag[passfail] = tagged;
                        PassNegTag[passfail] = tagged;
                        }} // End Pass/Fail loop.



                // Set up definition of passing or failing tags. 
                // THIS NEEDS TO BE ADJUSTED BY HAND WHEN ADDING NEW TAGGERS.
                // I propse that we leave this to be adjusted by hand, as the definitions to
                // "is tagged" changed quite a bit.
                // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarksRelease21
                int isMV2c10 = trackjet_isMV2_Tagged[ltj];
                int isMV2c10F = trackjet_isMV2Flip_Tagged[ltj];
                int isDL1 = trackjet_isDL1_Tagged[ltj];
                int isDL1F = trackjet_isDL1Flip_Tagged[ltj];
                if (isMV2c10 *= 2){
                        (PassTag[85_MV2c10]) = true;}
                if (isMV2c10 *= 3){
                        (PassTag[77_MV2c10]) = true;}
                if (isMV2c10 *= 5){
                        (PassTag[70_MV2c10]) = true;}
                if (isMV2c10 *= 7){
                        (PassTag[60_MV2c10]) = true;}
                if (isMV2c10F *= 2){
                        (PassNegTag[85_MV2c10]) = true;}
                if (isMV2c10F *= 3){
                        (PassNegTag[77_MV2c10]) = true;}
                if (isMV2c10F *= 5){
                        (PassNegTag[70_MV2c10]) = true;}
                if (isMV2c10F *= 7){
                        (PassNegTag[60_MV2c10]) = true;}
                if (isDL1 *= 2){
                        (PassTag[85_DL1]) = true;}
                if (isDL1 *= 3){
                        (PassTag[77_DL1]) = true;}
                if (isDL1 *= 5){
                        (PassTag[70_DL1]) = true;}
                if (isDL1 *= 7){
                        (PassTag[60_DL1]) = true;}
                if (isDL1F *= 2){
                        (PassNegTag[85_DL1]) = true;}
                if (isDL1F *= 3){
                        (PassNegTag[77_DL1]) = true;}
                if (isDL1F *= 5){
                        (PassNegTag[70_DL1]) = true;}
                if (isDL1F *= 7){
                        (PassNegTag[60_DL1]) = true;}

                //std::cout << "PTbin  " << pt_bin << "and pt " << jetpt << std::endl;
                //std::cout << "Etabin  " << eta_bin << std::endl;
                for (int k = 0; k < n_WP; k++){
                for (int i = 0; i < n_taggers){
                        TString wpname = WP[k];
                        TString tagname = Tagger[i];
                        TString PassFail = wpname + "_" + tagname;
                        float filler = -99;
                        float negfiller = -99;
                        if (tagname == "MV2c10"){
                                filler = trackjet_MV2c10[ltj];
                                negfiller = trackjet_MV2c10Flip[ltj];}
                        if (tagname == "DL1"){
                                filler = trackjet_DL1[ltj];
                                negfiller = trackjet_DL1Flip[ltj];}

			if (PassTag[WP[k]] == true){
                                TString fillname = "datah_" + std::to_string(pt_bin) + "_" + std::to_string(eta_bin) + "_" + WP[k] + "_" + Tagger[i] + "_";
                                datah_ptetaWPtag[fillname]->Fill(filler, dw);}
                        if (PassNegTag[WP[k]] == true){
                                TString negfillname = "datah_" + std::to_string(pt_bin) + "_" + std::to_string(eta_bin) + "_" + WP[k] + "_" + Tagger[i] + "_Neg";
                                datah_ptetaWPtag[negfillname]->Fill(negfiller, dw);}
                        if (PassTag[WP[k]] == false){
                                TString fillname = "datah_" + std::to_string(pt_bin) + "_" + std::to_string(eta_bin) + "_" + WP[k] + "_" + Tagger[i] + "__Fail";
                                datah_ptetaWPtag[fillname]->Fill(filler, dw);}
                        if (PassNegTag[WP[k]] == false){
                                TString negfillname = "datah_" + std::to_string(pt_bin) + "_" + std::to_string(eta_bin) + "_" + WP[k] + "_" + Tagger[i] + "_Neg_Fail";
                                datah_ptetaWPtag[negfillname]->Fill(negfiller, dw);}

                }}

}

                f_outputdt->Write();
                f_outputdt->Close();

}
}

}
