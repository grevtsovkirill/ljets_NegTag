#include "../AtlasStyle/AtlasStyle.C"


void FlavorCompPlotter(){ 
	SetAtlasStyle();


  	TFile *f_mc = new TFile("../HistoMaker/HistoFiles/mcConHistoFileM.root", "read");
  	TFile *f_output = new TFile("Control_plots/cplotsflavcomp.root", "recreate");


  	std::vector<std::string> histo_var = {"hMV2c10_l", "hMV2c10F_l", "hMV2c10_b", "hMV2c10F_b", "hMV2c10_c", "hMV2c10F_c"};

		bool flag_logy = true;
		bool flag_logx = false;

			std::string histo_lightF_name = histo_var.at(1);
			TH1D *h_lF = (TH1D*) f_mc->Get(histo_lightF_name.c_str());

			std::string histo_bjetF_name = histo_var.at(3);
			TH1D *h_bF = (TH1D*) f_mc->Get(histo_bjetF_name.c_str());
			
			std::string histo_cjetF_name = histo_var.at(5);
			TH1D *h_cF = (TH1D*) f_mc->Get(histo_cjetF_name.c_str());
				
			std::string histo_light_name = histo_var.at(0);
			TH1D *h_l = (TH1D*) f_mc->Get(histo_light_name.c_str());
			
			std::string histo_bjet_name = histo_var.at(2);
			TH1D *h_b = (TH1D*) f_mc->Get(histo_bjet_name.c_str());
			
			std::string histo_cjet_name = histo_var.at(4);
			TH1D *h_c = (TH1D*) f_mc->Get(histo_cjet_name.c_str());
			
			// Normalize all flavors to 1
			Double_t norm = 1.;
			h_l->Scale(norm/h_l->Integral());
			h_lF->Scale(norm/h_lF->Integral());
			h_c->Scale(norm/h_c->Integral());
			h_cF->Scale(norm/h_cF->Integral());
			h_b->Scale(norm/h_b->Integral());
			h_bF->Scale(norm/h_bF->Integral());
	// Pretty art things
    h_l->SetLineColor(kGreen+1);
    h_l->SetMarkerColor(kGreen+1);
    h_l->SetXTitle("MV2c10 Weight");
    h_l->SetYTitle("Events [Normalized]");
    h_l->SetTitle("");
    h_l->GetXaxis()->SetLabelSize(0.04);
    h_l->GetXaxis()->SetTitleSize(0.04);
    h_l->GetYaxis()->SetLabelSize(0.06);
    h_l->GetYaxis()->SetTitleSize(0.06);
    h_l->GetYaxis()->SetTitleOffset(1.30);
    h_l->GetYaxis()->SetNdivisions(505);			
    h_lF->SetLineColor(kGreen+1);
    h_lF->SetMarkerColor(kGreen+1);
    h_lF->SetXTitle("MV2c10Flip Weight");
    h_lF->SetYTitle("Events [Normalized]");
    h_lF->SetTitle("");
    h_lF->GetXaxis()->SetLabelSize(0.04);
    h_lF->GetXaxis()->SetTitleSize(0.04);
    h_lF->GetYaxis()->SetLabelSize(0.06);
    h_lF->GetYaxis()->SetTitleSize(0.06);
    h_lF->GetYaxis()->SetTitleOffset(1.30);
    h_lF->GetYaxis()->SetNdivisions(505);
    h_b->SetLineColor(kRed+1);
    h_b->SetMarkerColor(kRed+1);
    h_b->SetXTitle("MV2c10 Weight");
    h_b->SetYTitle("Events [Normalized]");
    h_b->SetTitle("");
    h_b->GetXaxis()->SetLabelSize(0.04);
    h_b->GetXaxis()->SetTitleSize(0.04);
    h_b->GetYaxis()->SetLabelSize(0.06);
    h_b->GetYaxis()->SetTitleSize(0.06);
    h_b->GetYaxis()->SetTitleOffset(1.30);
    h_b->GetYaxis()->SetNdivisions(505);			
    h_bF->SetLineColor(kRed+1);
    h_bF->SetMarkerColor(kRed+1);
    h_bF->SetXTitle("MV2c10Flip Weight");
    h_bF->SetYTitle("Events [Normalized]");
    h_bF->SetTitle("");
    h_bF->GetXaxis()->SetLabelSize(0.04);
    h_bF->GetXaxis()->SetTitleSize(0.04);
    h_bF->GetYaxis()->SetLabelSize(0.06);
    h_bF->GetYaxis()->SetTitleSize(0.06);
    h_bF->GetYaxis()->SetTitleOffset(1.30);
    h_bF->GetYaxis()->SetNdivisions(505);
    h_c->SetLineColor(kBlue+1);
    h_c->SetMarkerColor(kBlue+1);
    h_c->SetXTitle("MV2c10 Weight");
    h_c->SetYTitle("Events [Normalized]");
    h_c->SetTitle("");
    h_c->GetXaxis()->SetLabelSize(0.04);
    h_c->GetXaxis()->SetTitleSize(0.04);
    h_c->GetYaxis()->SetLabelSize(0.06);
    h_c->GetYaxis()->SetTitleSize(0.06);
    h_c->GetYaxis()->SetTitleOffset(1.30);
    h_c->GetYaxis()->SetNdivisions(505);			
    h_cF->SetLineColor(kBlue+1);
    h_cF->SetMarkerColor(kBlue+1);
    h_cF->SetXTitle("MV2c10Flip Weight");
    h_cF->SetYTitle("Events [Normalized]");
    h_cF->SetTitle("");
    h_cF->GetXaxis()->SetLabelSize(0.04);
    h_cF->GetXaxis()->SetTitleSize(0.04);
    h_cF->GetYaxis()->SetLabelSize(0.06);
    h_cF->GetYaxis()->SetTitleSize(0.06);
    h_cF->GetYaxis()->SetTitleOffset(1.30);
    h_cF->GetYaxis()->SetNdivisions(505);


    // Define ratio plots
	std::string histo_ratio_name1 = "ratio_Bl";
	TH1D *h_ratiobl = (TH1D*)h_b->Clone(histo_ratio_name1.c_str());
	h_ratiobl->Divide(h_l);
    	std::string histo_ratio_name2 = "ratio_Cl";
	TH1D *h_ratiocl = (TH1D*)h_c->Clone(histo_ratio_name2.c_str());
	h_ratiocl->Divide(h_l);
    	std::string histo_ratio_name1F = "ratio_BlF";
	TH1D *h_ratioblF = (TH1D*)h_bF->Clone(histo_ratio_name1F.c_str());
	h_ratioblF->Divide(h_lF);
    	std::string histo_ratio_name2F = "ratio_ClF";
	TH1D *h_ratioclF = (TH1D*)h_cF->Clone(histo_ratio_name2F.c_str());
	h_ratioclF->Divide(h_lF);
	std::string histo_ratio_name1sf = "ratio_Bsf";
	TH1D *h_ratiobsf = (TH1D*)h_bF->Clone(histo_ratio_name1sf.c_str());
	h_ratiobsf->Divide(h_b);
	std::string histo_ratio_name2sf = "ratio_Csf";
	TH1D *h_ratiocsf = (TH1D*)h_cF->Clone(histo_ratio_name2sf.c_str());
	h_ratiocsf->Divide(h_c);
	std::string histo_ratio_name3sf = "ratio_Lsf";
	TH1D *h_ratiolsf = (TH1D*)h_lF->Clone(histo_ratio_name3sf.c_str());
	h_ratiolsf->Divide(h_l);
	//h_ratiobl->Scale(norm/h_ratiobl->Integral(), "width");
	//h_ratiocl->Scale(norm/h_ratioblF->Integral(), "width");
	//h_ratioblF->Scale(norm/h_ratiocl->Integral(), "width");


    h_ratiocl->SetLineColor(kBlue+1);
    h_ratiocl->SetMarkerColor(kBlue+1);
    h_ratiocl->SetXTitle("MV2c10 Weight");
    h_ratiocl->SetYTitle("Heavy/Light");
    h_ratiocl->SetTitle("");
    h_ratiocl->GetXaxis()->SetLabelSize(0.04);
    h_ratiocl->GetXaxis()->SetTitleSize(0.04);
    h_ratiocl->GetYaxis()->SetLabelSize(0.06);
    h_ratiocl->GetYaxis()->SetTitleSize(0.06);
    h_ratiocl->GetYaxis()->SetTitleOffset(1.30);
    h_ratiocl->GetYaxis()->SetNdivisions(505);			
    h_ratioclF->SetLineColor(kBlue+1);
    h_ratioclF->SetMarkerColor(kBlue+1);
    h_ratioclF->SetXTitle("MV2c10Flip Weight");
    h_ratioclF->SetYTitle("Heavy/Light");
    h_ratioclF->SetTitle("");
    h_ratioclF->GetXaxis()->SetLabelSize(0.04);
    h_ratioclF->GetXaxis()->SetTitleSize(0.04);
    h_ratioclF->GetYaxis()->SetLabelSize(0.06);
    h_ratioclF->GetYaxis()->SetTitleSize(0.06);
    h_ratioclF->GetYaxis()->SetTitleOffset(1.30);
    h_ratioclF->GetYaxis()->SetNdivisions(505);
    h_ratiobl->SetLineColor(kRed+1);
    h_ratiobl->SetMarkerColor(kRed+1);
    h_ratiobl->SetXTitle("MV2c10 Weight");
    h_ratiobl->SetYTitle("Heavy/Light");
    h_ratiobl->SetTitle("");
    h_ratiobl->GetXaxis()->SetLabelSize(0.04);
    h_ratiobl->GetXaxis()->SetTitleSize(0.04);
    h_ratiobl->GetYaxis()->SetLabelSize(0.06);
    h_ratiobl->GetYaxis()->SetTitleSize(0.06);
    h_ratiobl->GetYaxis()->SetTitleOffset(1.30);
    h_ratiobl->GetYaxis()->SetNdivisions(505);			
    h_ratioblF->SetLineColor(kRed+1);
    h_ratioblF->SetMarkerColor(kRed+1);
    h_ratioblF->SetXTitle("MV2c10Flip Weight");
    h_ratioblF->SetYTitle("Heavy/Light");
    h_ratioblF->SetTitle("");
    h_ratioblF->GetXaxis()->SetLabelSize(0.04);
    h_ratioblF->GetXaxis()->SetTitleSize(0.04);
    h_ratioblF->GetYaxis()->SetLabelSize(0.06);
    h_ratioblF->GetYaxis()->SetTitleSize(0.06);
    h_ratioblF->GetYaxis()->SetTitleOffset(1.30);
    h_ratioblF->GetYaxis()->SetNdivisions(505);
    h_ratiobsf->SetLineColor(kRed+1);
    h_ratiobsf->SetMarkerColor(kRed+1);
    h_ratiobsf->SetXTitle("MV2c10 Weight");
    h_ratiobsf->SetYTitle("Flip/Standard");
    h_ratiobsf->SetTitle("");
    h_ratiobsf->GetXaxis()->SetLabelSize(0.04);
    h_ratiobsf->GetXaxis()->SetTitleSize(0.04);
    h_ratiobsf->GetYaxis()->SetLabelSize(0.06);
    h_ratiobsf->GetYaxis()->SetTitleSize(0.06);
    h_ratiobsf->GetYaxis()->SetTitleOffset(1.30);
    h_ratiobsf->GetYaxis()->SetNdivisions(505);
    h_ratiocsf->SetLineColor(kBlue+1);
    h_ratiocsf->SetMarkerColor(kBlue+1);
    h_ratiocsf->SetXTitle("MV2c10 Weight");
    h_ratiocsf->SetYTitle("Flip/Standard");
    h_ratiocsf->SetTitle("");
    h_ratiocsf->GetXaxis()->SetLabelSize(0.04);
    h_ratiocsf->GetXaxis()->SetTitleSize(0.04);
    h_ratiocsf->GetYaxis()->SetLabelSize(0.06);
    h_ratiocsf->GetYaxis()->SetTitleSize(0.06);
    h_ratiocsf->GetYaxis()->SetTitleOffset(1.30);
    h_ratiocsf->GetYaxis()->SetNdivisions(505);
    h_ratiolsf->SetLineColor(kGreen+1);
    h_ratiolsf->SetMarkerColor(kGreen+1);
    h_ratiolsf->SetXTitle("MV2c10 Weight");
    h_ratiolsf->SetYTitle("Flip/Standard");
    h_ratiolsf->SetTitle("");
    h_ratiolsf->GetXaxis()->SetLabelSize(0.04);
    h_ratiolsf->GetXaxis()->SetTitleSize(0.04);
    h_ratiolsf->GetYaxis()->SetLabelSize(0.06);
    h_ratiolsf->GetYaxis()->SetTitleSize(0.06);
    h_ratiolsf->GetYaxis()->SetTitleOffset(1.30);
    h_ratiolsf->GetYaxis()->SetNdivisions(505);



    TLegend* legend = new TLegend(0.580201,0.561807,0.780702,0.706947);
    legend->SetTextFont(42);
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);  // no border
    legend->SetFillColor(0);   // Legend background should be white
    legend->SetTextSize(0.04); // Increase entry font size!
    legend->AddEntry(h_l,"Light jets","LPE");
    legend->AddEntry(h_b,"B jets","LPE");
    legend->AddEntry(h_c,"C jets","LPE");

    TLegend* legendr = new TLegend(0.580201,0.561807,0.780702,0.706947);
    legendr->SetTextFont(42);
    legendr->SetFillStyle(0);
    legendr->SetBorderSize(0);  // no border
    legendr->SetFillColor(0);   // Legend background should be white
    legendr->SetTextSize(0.04); // Increase entry font size!
    legendr->AddEntry(h_ratiobl,"b/l","LPE");
    legendr->AddEntry(h_ratiocl,"c/l","LPE");

    TLegend* legendrsf = new TLegend(0.580201,0.561807,0.780702,0.706947);
    legendrsf->SetTextFont(42);
    legendrsf->SetFillStyle(0);
    legendrsf->SetBorderSize(0);  // no border
    legendrsf->SetFillColor(0);   // Legend background should be white
    legendrsf->SetTextSize(0.04); // Increase entry font size!
    legendrsf->AddEntry(h_ratiobsf,"B jets","LPE");
    legendrsf->AddEntry(h_ratiocsf,"C jets","LPE");
    legendrsf->AddEntry(h_ratiolsf,"Light jets", "LPE");

    TPaveText *pt = new TPaveText(0.508772,0.851259,0.629073,0.919908,"brNDC");
    pt->SetBorderSize(0);
    pt->SetFillColor(0);
    pt->SetTextSize(0.04);
    pt->SetTextFont(72);
    pt->AddText("ATLAS");
    TPaveText *pt2 = new TPaveText(0.617794,0.850768,0.738095,0.921053,"brNDC");
    pt2->SetBorderSize(0);
    pt2->SetFillColor(0);
    pt2->SetTextSize(0.04);
    pt2->SetTextFont(42);
    pt2->AddText("Internal");
    TPaveText *pt3 = new TPaveText(0.602757,0.713959,0.723058,0.864335,"brNDC");
    pt3->SetBorderSize(0);
    pt3->SetFillColor(0);
    pt3->SetTextSize(0.04);
    pt3->SetTextFont(42);
    pt3->AddText("#sqrt{s} = 13 TeV, 3.2 + 32.9 fb^{-1}, rel21");

	TCanvas *c = new TCanvas("FlavorComp", "", 800, 900);
	c->Draw();
	c->cd();
	if(flag_logy) c->SetLogy();
    if(flag_logx) c->SetLogx();
    h_b->Draw("hist");
    h_c->Draw("hist SAME");
    h_c->Draw("AXISSAME");
    h_l->Draw("hist SAME");
    h_l->Draw("AXISSAME");
    legend->Draw();
    pt->Draw();
    pt2->Draw();
    pt3->Draw();
    f_output->cd();
    c->Write();
    c->SaveAs(("Control_plots/jpg/FlavorComp.jpg"));

	TCanvas *cF = new TCanvas("FlavorComp_Flip", "", 800, 900);
	cF->Draw();
	cF->cd();
	if(flag_logy) cF->SetLogy();
    if(flag_logx) cF->SetLogx();
    h_bF->Draw("hist");
    h_cF->Draw("hist SAME");
    h_cF->Draw("AXISSAME");
    h_lF->Draw("hist SAME");
    h_lF->Draw("AXISSAME");
    legend->Draw();
    pt->Draw();
    pt2->Draw();
    pt3->Draw();
    f_output->cd();
    cF->Write();
    cF->SaveAs(("Control_plots/jpg/FlavorComp_Flip.jpg"));

	TCanvas *cr = new TCanvas("FlavorRatio", "", 800, 900);
	cr->Draw();
	cr->cd();
	if(flag_logy) cr->SetLogy();
    if(flag_logx) cr->SetLogx();
    h_ratiobl->Draw("hist");
    h_ratiocl->Draw("hist SAME");
    h_ratiocl->Draw("AXISSAME");
    legendr->Draw();
    pt->Draw();
    pt2->Draw();
    pt3->Draw();
    f_output->cd();
    cr->Write();
    cr->SaveAs(("Control_plots/jpg/FlavorRatio.jpg"));

	TCanvas *crF = new TCanvas("FlavorRatio_Flip", "", 800, 900);
	crF->Draw();
	crF->cd();
	if(flag_logy) crF->SetLogy();
    if(flag_logx) crF->SetLogx();
    h_ratioblF->Draw("hist");
    h_ratioclF->Draw("hist SAME");
    h_ratioclF->Draw("AXISSAME");
    legendr->Draw();
    pt->Draw();
    pt2->Draw();
    pt3->Draw();
    f_output->cd();
    crF->Write();
    crF->SaveAs(("Control_plots/jpg/FlavorRatio_Flip.jpg"));

	TCanvas *crsf = new TCanvas("TaggerRatio", "", 800, 900);
	crsf->Draw();
	crsf->cd();
    if(flag_logx) crsf->SetLogx();
    h_ratiobsf->SetAxisRange(0, 10, "Y");
    h_ratiobsf->Draw("hist");
    h_ratiocsf->Draw("hist SAME");
    h_ratiocsf->Draw("AXISSAME");
    h_ratiolsf->Draw("hist SAME");
    h_ratiolsf->Draw("AXISSAME");
    legendrsf->Draw();
    pt->Draw();
    pt2->Draw();
    pt3->Draw();
    f_output->cd();
    crsf->Write();
    crsf->SaveAs(("Control_plots/jpg/TaggerRatio.jpg"));




}
