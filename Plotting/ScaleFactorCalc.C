void ScaleFactorCalc() { 


	// Get input files
	TFile* f_data = new TFile("../HistoMaker/HistoFiles/dataSFhistosM.root", "read");
	TFile* f_mc = new TFile("../HistoMaker/HistoFiles/mcSFhistosM.root", "read");
	TFile* f_output = new TFile("../HistoMaker/HistoFiles/ScaleFactors.root", "Recreate");
	// Set up lists for bins, WPs, Flavors, and Tags
	const int n_pt = 8;
	const int n_eta = 2;
	const int n_WP = 4;
	const double pt_lowedges[n_pt+1] = {10, 15, 20, 50, 100, 150, 300, 500, 3000};
        const double eta_lowedges[n_eta+1] = {0, 1.2, 2.5};
	const std::string WP[n_WP+1] = {"Incl", "85", "77", "70", "60"};
        const std::string Tag[2] = {"", "Neg"};
        const std::string Flav[3] = {"l", "b", "c"};


	// All MC input histograms will follow the form: "mch_"+ptbin+"_"+etabin"+"_"+WP+"_"+Tag+"_"+Flav
        // All Data input histograms will follow the form: "datah_"+ptbin+"_"+etabin"+"_"+WP+"_"+Tag
	
	// Book flavor fraction histograms
	TH2D *h_fracl_mc = new TH2D("h_fracl_mc","", n_pt, pt_lowedges, n_eta, eta_lowedges);
	TH2D *h_fracb_mc = new TH2D("h_fracb_mc","", n_pt, pt_lowedges, n_eta, eta_lowedges);
	TH2D *h_fracc_mc = new TH2D("h_fracc_mc","", n_pt, pt_lowedges, n_eta, eta_lowedges);
 
	// Book scale factor hisograms (One for each WP, one for each eta bin
	TH1D *sfh_85_central = new TH1D("sfh_85_central", "Light jet Mistag Rate Scale Factor, 85% WP", n_pt, pt_lowedges);
        TH1D *sfh_77_central = new TH1D("sfh_77_central", "Light jet Mistag Rate Scale Factor, 77% WP", n_pt, pt_lowedges);
        TH1D *sfh_70_central = new TH1D("sfh_70_central", "Light jet Mistag Rate Scale Factor, 70% WP", n_pt, pt_lowedges);
        TH1D *sfh_60_central = new TH1D("sfh_60_central", "Light jet Mistag Rate Scale Factor, 60% WP", n_pt, pt_lowedges);
        TH1D *sfh_85_forward = new TH1D("sfh_85_forward", "Light jet Mistag Rate Scale Factor, Forward \u03B7, 85% WP", n_pt, pt_lowedges);
        TH1D *sfh_77_forward = new TH1D("sfh_77_forward", "Light jet Mistag Rate Scale Factor, Forward \u03B7, 77% WP", n_pt, pt_lowedges);
        TH1D *sfh_70_forward = new TH1D("sfh_70_forward", "Light jet Mistag Rate Scale Factor, Forward \u03B7, 70% WP", n_pt, pt_lowedges);
        TH1D *sfh_60_forward = new TH1D("sfh_60_forward", "Light jet Mistag Rate Scale Factor, Forward \u03B7, 60% WP", n_pt, pt_lowedges);
        std::ofstream text;
        text.open ("ValPrintouts.txt");



		for (int i=1; i < n_pt+1; i++){
        	for (int j=1; j < n_eta+1; j++){

		// Calculate inclusive sample numbers (No Working Points Tagged)

                std::string identifier_l = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_Incl__l" ;
                std::string identifier_c = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_Incl__c" ;
                std::string identifier_b = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_Incl__b" ;
                std::string identifier_data = "datah_" + std::to_string(i) + "_" + std::to_string(j) + "_Incl_" ;
                std::string identifier_data_neg = "datah_" + std::to_string(i) + "_" + std::to_string(j) + "_Incl_Neg" ;

		TH1D *h_l_Incl = (TH1D*)f_mc->Get(identifier_l.c_str());
		TH1D *h_c_Incl = (TH1D*)f_mc->Get(identifier_c.c_str());
      		TH1D *h_b_Incl = (TH1D*)f_mc->Get(identifier_b.c_str());
		TH1D *h_data_Incl = (TH1D*)f_data->Get(identifier_data.c_str());
                TH1D *h_data_Incl_Neg = (TH1D*)f_data->Get(identifier_data_neg.c_str());
		
		double e_l_inc = 0;
		double e_c_inc = 0;
		double e_b_inc = 0;
		double e_Incl_mc = 0;
		double e_Incl_data = 0;
		double e_Incl_data_neg = 0;
		double e_fc = 0;
		double e_fb = 0;
		int highbin = h_l_Incl->GetNbinsX();
		double N_l_mc = h_l_Incl->IntegralAndError(1, highbin, e_l_inc, "");
		double N_c_mc = h_c_Incl->IntegralAndError(1, highbin, e_c_inc, "");
		double N_b_mc = h_b_Incl->IntegralAndError(1, highbin, e_b_inc, "");
		double N_Incl_mc = N_l_mc + N_c_mc + N_b_mc;
		e_Incl_mc = sqrt(pow(e_l_inc,2) + pow(e_c_inc,2) + pow(e_b_inc,2));
		double N_Incl_data = h_data_Incl->IntegralAndError(1, highbin, e_Incl_data, "");
		double N_Incl_data_neg = h_data_Incl_Neg->IntegralAndError(1, highbin, e_Incl_data_neg, "");
                double fracc = N_c_mc/N_Incl_mc;
		e_fc = fracc * sqrt(pow(e_c_inc/N_c_mc,2) + pow(e_Incl_mc/N_Incl_mc,2));
                double fracb = N_b_mc/N_Incl_mc;
		e_fb = fracb * sqrt(pow(e_b_inc/N_b_mc,2) + pow(e_Incl_mc/N_Incl_mc,2));

			for (int k=1; k < n_WP+1; k++){
				
			// Get histograms to be used in scale factor calculations
				
                	std::string id_l = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "__l" ;
                	std::string id_c = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "__c" ;
                	std::string id_b = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "__b" ;
                	std::string id_l_neg = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "_Neg_l" ;
                	std::string id_c_neg = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "_Neg_c" ;
                	std::string id_b_neg = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "_Neg_b" ;
                        std::string id_data_neg = "datah_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "_Neg" ;
                        
			TH1D *data_neg = (TH1D*)f_data->Get(id_data_neg.c_str());
                	TH1D *mch_l = (TH1D*)f_mc->Get(id_l.c_str());
                	TH1D *mch_c = (TH1D*)f_mc->Get(id_c.c_str());
                	TH1D *mch_b = (TH1D*)f_mc->Get(id_b.c_str());
                	TH1D *mch_l_neg = (TH1D*)f_mc->Get(id_l_neg.c_str());
                	TH1D *mch_c_neg = (TH1D*)f_mc->Get(id_c_neg.c_str());
                	TH1D *mch_b_neg = (TH1D*)f_mc->Get(id_b_neg.c_str());
			text << "Working point and bins are " << WP[k] << "_" << i << "_" << j << std::endl;
			// Set up error values
			double e_l = 0;
			double e_c = 0;
			double e_b = 0;
                        double e_l_neg = 0;
                        double e_c_neg = 0;
                        double e_b_neg = 0;
			double e_Kll = 0;
			double e_epsc = 0;
			double e_epsb = 0;
			double e_Khf = 0;
			double e_epslneg = 0;
			double e_epslmcnom = 0;
			double e_data_neg = 0;
			double e_epsdataneg = 0;
			double e_SFl = 0;
                	double N_l_mc_wp = mch_l->IntegralAndError(1, highbin, e_l, "");
                	double N_c_mc_wp = mch_c->IntegralAndError(1, highbin, e_c, "" );
                	double N_b_mc_wp = mch_b->IntegralAndError(1, highbin, e_b, "");
                        double N_l_mc_neg_wp = mch_l_neg->IntegralAndError(1, highbin, e_l_neg, "");
                        double N_c_mc_neg_wp = mch_c_neg->IntegralAndError(1, highbin, e_c_neg, "" );
                        double N_b_mc_neg_wp = mch_b_neg->IntegralAndError(1, highbin, e_b_neg, "");
			double N_data_neg_wp = data_neg->IntegralAndError(1, highbin, e_data_neg, "");
			// Calculate K_{LL}
			double Kll = (N_l_mc_wp/N_l_mc_neg_wp);
			e_Kll = Kll * sqrt(pow(e_l/N_l_mc_wp,2) + pow(e_l_neg/N_l_mc_neg_wp,2));
			text << "Kll is " << Kll << "+-" << e_Kll << std::endl;
			
			// Calculate K_{HF}
			double epsc = (N_c_mc_neg_wp/N_c_mc);
			e_epsc = epsc * sqrt(pow(e_c/N_c_mc_wp,2) + pow(e_c_inc/N_c_mc,2));
                        double epsb = (N_b_mc_neg_wp/N_b_mc);
                        e_epsb = epsb * sqrt(pow(e_b/N_b_mc_wp,2) + pow(e_b_inc/N_b_mc,2));
			double epslneg = (N_l_mc_neg_wp/N_l_mc);
                        e_epslneg = epslneg * sqrt(pow(e_l_neg/N_l_mc_neg_wp,2) + pow(e_l_inc/N_l_mc,2));
			double Khf = 1./((fracc * (epsc/epslneg) + fracb * (epsb/epslneg)) + (1-(fracc+fracb)));
			e_Khf = Khf * sqrt(pow((fracc*(epsc/epslneg)) * sqrt(pow(e_fc/fracc,2) + pow(e_epsc/epsc,2) + pow(e_epslneg/epslneg,2)),2) + pow((fracb*(epsb/epslneg)) * sqrt(pow(e_fb/fracb,2) + pow(e_epsb/epsb,2) + pow(e_epslneg/epslneg,2)),2) + pow(e_fc,2) + pow(e_fb,2)); 
			text << "Khf is " << Khf << "+-" << e_Khf << std::endl;
			
			// Calculate eps_{l}^{MC, nominal}
			double epslmcnom = (N_l_mc_wp/N_l_mc);
			e_epslmcnom = epslmcnom * sqrt(pow(e_l/N_l_mc_wp,2) + pow(e_l_inc/N_l_mc,2));
			text << "epslmcnom is " << epslmcnom << "+-" << e_epslmcnom << std::endl;
                	
			// Calculate eps_{neg}^{Data}
			double epsdataneg = (N_data_neg_wp/N_Incl_data);
			e_epsdataneg = epsdataneg * sqrt(pow(e_data_neg/N_data_neg_wp,2) + pow(e_Incl_data/N_Incl_data,2));
			text << "epsdataneg is " << epsdataneg << "+-" << e_epsdataneg << std::endl;
			
			//Calculate scale factor dependent on pt, eta, and WP
			double SFl = (epsdataneg * Kll * Khf)/epslmcnom;
			e_SFl = SFl * sqrt(pow(e_epsdataneg/epsdataneg,2) + pow(e_Kll/Kll,2) + pow(e_Khf/Khf,2) + pow(e_epslmcnom/epslmcnom,2));
			text << "Scale Factor is " << SFl << "+-" << e_SFl << std::endl;			
			
			// Fill histogram bins
	              	h_fracl_mc->SetBinContent(i,j,N_l_mc/N_Incl_mc);
        	      	h_fracc_mc->SetBinContent(i,j,fracc);
			h_fracb_mc->SetBinContent(i,j,fracb);
			if (k == 1){
				if (j == 1){
					sfh_85_central->SetBinContent(i,SFl);
					sfh_85_central->SetBinError(i, e_SFl);
					}
				else{
                                        sfh_85_forward->SetBinContent(i,SFl);
				        sfh_85_forward->SetBinError(i, e_SFl);
					}
 
				}	
                        if (k == 2){
                                if (j == 1){
                                        sfh_77_central->SetBinContent(i,SFl);
                                        sfh_77_central->SetBinError(i, e_SFl);
                                        }
                                else{
                                        sfh_77_forward->SetBinContent(i,SFl);
                                        sfh_77_forward->SetBinError(i, e_SFl);
                                        }
				}
                        if (k == 3){
                                if (j == 1){
                                        sfh_70_central->SetBinContent(i,SFl);
                                        sfh_70_central->SetBinError(i, e_SFl);
                                        }
                                else{
                                        sfh_70_forward->SetBinContent(i,SFl);
                                        sfh_70_forward->SetBinError(i, e_SFl);
                                        }

                                }
                        if (k == 4){
                                if (j == 1){
                                        sfh_60_central->SetBinContent(i,SFl);
                                        sfh_60_central->SetBinError(i, e_SFl);
                                        }
                                else{
                                        sfh_60_forward->SetBinContent(i,SFl);
                                        sfh_60_forward->SetBinError(i, e_SFl);
                                        }

                                }



			} // End WP Loop
	}} // End Pt/eta loops


                        text.close();



    sfh_85_central->SetLineColor(kBlue+1);
    sfh_85_central->SetMarkerColor(kBlue+1);
    sfh_85_central->SetXTitle("P_{T} [GeV]");
    sfh_85_central->SetYTitle("Scale Factor");
    sfh_85_central->SetTitle("Light Flavour Mistag Scale Factor, 85% WP");
    sfh_85_central->GetXaxis()->SetLabelSize(0.02);
    sfh_85_central->GetXaxis()->SetTitleSize(0.02);
    sfh_85_central->GetYaxis()->SetLabelSize(0.03);
    sfh_85_central->GetYaxis()->SetTitleSize(0.03);
    sfh_85_central->GetYaxis()->SetTitleOffset(1.10);
    sfh_85_central->GetYaxis()->SetNdivisions(505);
    sfh_85_forward->SetLineColor(kRed+1);
    sfh_85_forward->SetMarkerColor(kRed+1);
    sfh_85_forward->SetXTitle("P_{T} [GeV]");
    sfh_85_forward->SetYTitle("Scale Factor");
    sfh_85_forward->SetTitle("Light Flavour Mistag Scale Factor, 85% WP");
    sfh_85_forward->GetXaxis()->SetLabelSize(0.02);
    sfh_85_forward->GetXaxis()->SetTitleSize(0.02);
    sfh_85_forward->GetYaxis()->SetLabelSize(0.03);
    sfh_85_forward->GetYaxis()->SetTitleSize(0.03);
    sfh_85_forward->GetYaxis()->SetTitleOffset(1.10);
    sfh_85_forward->GetYaxis()->SetNdivisions(505);
    sfh_77_central->SetLineColor(kBlue+1);
    sfh_77_central->SetMarkerColor(kBlue+1);
    sfh_77_central->SetXTitle("P_{T} [GeV]");
    sfh_77_central->SetYTitle("Scale Factor");
    sfh_77_central->SetTitle("Light Flavour Mistag Scale Factor, 77% WP");
    sfh_77_central->GetXaxis()->SetLabelSize(0.02);
    sfh_77_central->GetXaxis()->SetTitleSize(0.02);
    sfh_77_central->GetYaxis()->SetLabelSize(0.03);
    sfh_77_central->GetYaxis()->SetTitleSize(0.03);
    sfh_77_central->GetYaxis()->SetTitleOffset(1.10);
    sfh_77_central->GetYaxis()->SetNdivisions(505);
    sfh_77_forward->SetLineColor(kRed+1);
    sfh_77_forward->SetMarkerColor(kRed+1);
    sfh_77_forward->SetXTitle("P_{T} [GeV]");
    sfh_77_forward->SetYTitle("Scale Factor");
    sfh_77_forward->SetTitle("Light Flavour Mistag Scale Factor, 77% WP");
    sfh_77_forward->GetXaxis()->SetLabelSize(0.02);
    sfh_77_forward->GetXaxis()->SetTitleSize(0.02);
    sfh_77_forward->GetYaxis()->SetLabelSize(0.03);
    sfh_77_forward->GetYaxis()->SetTitleSize(0.03);
    sfh_77_forward->GetYaxis()->SetTitleOffset(1.10);
    sfh_77_forward->GetYaxis()->SetNdivisions(505);
    sfh_70_central->SetLineColor(kBlue+1);
    sfh_70_central->SetMarkerColor(kBlue+1);
    sfh_70_central->SetXTitle("P_{T} [GeV]");
    sfh_70_central->SetYTitle("Scale Factor");
    sfh_70_central->SetTitle("Light Flavour Mistag Scale Factor, 70% WP");
    sfh_70_central->GetXaxis()->SetLabelSize(0.02);
    sfh_70_central->GetXaxis()->SetTitleSize(0.02);
    sfh_70_central->GetYaxis()->SetLabelSize(0.03);
    sfh_70_central->GetYaxis()->SetTitleSize(0.03);
    sfh_70_central->GetYaxis()->SetTitleOffset(1.10);
    sfh_70_central->GetYaxis()->SetNdivisions(505);
    sfh_70_forward->SetLineColor(kRed+1);
    sfh_70_forward->SetMarkerColor(kRed+1);
    sfh_70_forward->SetXTitle("P_{T} [GeV]");
    sfh_70_forward->SetYTitle("Scale Factor");
    sfh_70_forward->SetTitle("Light Flavour Mistag Scale Factor, 70% WP");
    sfh_70_forward->GetXaxis()->SetLabelSize(0.02);
    sfh_70_forward->GetXaxis()->SetTitleSize(0.02);
    sfh_70_forward->GetYaxis()->SetLabelSize(0.03);
    sfh_70_forward->GetYaxis()->SetTitleSize(0.03);
    sfh_70_forward->GetYaxis()->SetTitleOffset(1.10);
    sfh_70_forward->GetYaxis()->SetNdivisions(505);
    sfh_60_central->SetLineColor(kBlue+1);
    sfh_60_central->SetMarkerColor(kBlue+1);
    sfh_60_central->SetXTitle("P_{T} [GeV]");
    sfh_60_central->SetYTitle("Scale Factor");
    sfh_60_central->SetTitle("Light Flavour Mistag Scale Factor, 60% WP");
    sfh_60_central->GetXaxis()->SetLabelSize(0.02);
    sfh_60_central->GetXaxis()->SetTitleSize(0.02);
    sfh_60_central->GetYaxis()->SetLabelSize(0.03);
    sfh_60_central->GetYaxis()->SetTitleSize(0.03);
    sfh_60_central->GetYaxis()->SetTitleOffset(1.10);
    sfh_60_central->GetYaxis()->SetNdivisions(505);
    sfh_60_forward->SetLineColor(kRed+1);
    sfh_60_forward->SetMarkerColor(kRed+1);
    sfh_60_forward->SetXTitle("P_{T} [GeV]");
    sfh_60_forward->SetYTitle("Scale Factor");
    sfh_60_forward->SetTitle("Light Flavour Mistag Scale Factor, 60% WP");
    sfh_60_forward->GetXaxis()->SetLabelSize(0.02);
    sfh_60_forward->GetXaxis()->SetTitleSize(0.02);
    sfh_60_forward->GetYaxis()->SetLabelSize(0.03);
    sfh_60_forward->GetYaxis()->SetTitleSize(0.03);
    sfh_60_forward->GetYaxis()->SetTitleOffset(1.10);
    sfh_60_forward->GetYaxis()->SetNdivisions(505);

    TLegend* legend = new TLegend(0.680201,0.661807,0.880702,0.806947);
    legend->SetTextFont(42);
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);  // no border
    legend->SetFillColor(0);   // Legend background should be white
    legend->SetTextSize(0.02); // Increase entry font size!
    legend->AddEntry(sfh_85_central,"Central eta","LPE");
    legend->AddEntry(sfh_85_forward,"Forward eta","LPE");

    TPaveText *pt = new TPaveText(0.158772,0.751259,0.279073,0.819908,"brNDC");
    pt->SetBorderSize(0);
    pt->SetFillColor(0);
    pt->SetTextSize(0.025);
    pt->SetTextFont(72);
    pt->AddText("ATLAS");
    TPaveText *pt2 = new TPaveText(0.317794,0.750768,0.438095,0.821053,"brNDC");
    pt2->SetBorderSize(0);
    pt2->SetFillColor(0);
    pt2->SetTextSize(0.025);
    pt2->SetTextFont(42);
    pt2->AddText("Internal");
    TPaveText *pt3 = new TPaveText(0.252757,0.663959,0.373058,0.814335,"brNDC");
    pt3->SetBorderSize(0);
    pt3->SetFillColor(0);
    pt3->SetTextSize(0.015);
    pt3->SetTextFont(42);
    pt3->AddText("#sqrt{s} = 13 TeV, 3.2 + 32.9 fb^{-1}, rel21");

	gStyle->SetOptStat(0);

        TCanvas *c85 = new TCanvas("85", "", 800, 900);
        c85->Draw();
        c85->cd();
	c85->SetLogx();
        sfh_85_central->SetMaximum(1.5);
        sfh_85_central->SetMinimum(0.8);
	sfh_85_central->Draw("");
	sfh_85_forward->Draw("same");
	legend->Draw();
	pt->Draw();
	pt2->Draw();
	pt3->Draw();
	f_output->cd();
	c85->Write();
	c85->SaveAs("Control_plots/85.jpg");

        TCanvas *c77 = new TCanvas("77", "", 800, 900);
        c77->Draw();
        c77->cd();
	c77->SetLogx();
        sfh_77_central->SetMaximum(1.5);
        sfh_77_central->SetMinimum(0.5);
        sfh_77_central->Draw("");
        sfh_77_forward->Draw("same");
        legend->Draw();
        pt->Draw();
        pt2->Draw();
        pt3->Draw();
        f_output->cd();
        c77->Write();
        c77->SaveAs("Control_plots/77.jpg");

        TCanvas *c70 = new TCanvas("70", "", 800, 900);
        c70->Draw();
        c70->cd();
	c70->SetLogx();
        sfh_70_central->SetMaximum(2);
        sfh_70_central->SetMinimum(0.5);
        sfh_70_central->Draw("");
        sfh_70_forward->Draw("same");
        legend->Draw();
        pt->Draw();
        pt2->Draw();
        pt3->Draw();
        f_output->cd();
        c70->Write();
        c70->SaveAs("Control_plots/70.jpg");

        TCanvas *c60 = new TCanvas("60", "", 800, 900);
        c60->Draw();
        c60->cd();
	c60->SetLogx();
        sfh_60_central->SetMaximum(2);
        sfh_60_central->SetMinimum(0.2);
        sfh_60_central->Draw("");
        sfh_60_forward->Draw("same");
        legend->Draw();
        pt->Draw();
        pt2->Draw();
        pt3->Draw();
        f_output->cd();
        c60->Write();
        c60->SaveAs("Control_plots/60.jpg");


//f_output->Close();





}




















