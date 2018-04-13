#include "../AtlasStyle/AtlasStyle.C"



void ControlPlotter() { 
	SetAtlasStyle();

	TFile *f_data = new TFile("../HistoMaker/HistoFiles/dataConHistoFileM.root", "read");
  	TFile *f_mc = new TFile("../HistoMaker/HistoFiles/mcConHistoFileM.root", "read");
  	TFile *f_output = new TFile("Control_plots/cplots.root", "recreate");


  	std::vector<std::string> histo_var = {"hpT", "heta", "hphi", "hntrk","hNjet", "hMV2c10", "hMV2c10F"};
  	std::vector<std::string> histo_Xlabels = {"p_{T}^{Trackjet} [GeV]", "#eta^{Trackjet}", "#phi^{jet} [rad.]", "N_{trk}^{Trackjet}","N_{Trackjet}", "MV2c10 Weight", "MV2c10Flip Weight"};

for(int ivar=0; ivar<histo_var.size(); ivar++){
	
	bool flag_logx = false;
	if (histo_var.at(ivar)=="hpT") flag_logx = true;

	bool flag_logy = true;

	std::string histo_name = histo_var.at(ivar);

	TH1D *h_tmp = (TH1D*)f_data->Get(histo_name.c_str());
	std::string histo_data_name = "data_" + histo_var.at(ivar);
	TH1D *h_data = (TH1D*)h_tmp->Clone(histo_data_name.c_str());

	h_tmp = (TH1D*)f_mc->Get(histo_name.c_str());
	std::string histo_mc_name = "mc_" + histo_var.at(ivar);
	TH1D *h_mc = (TH1D*)h_tmp->Clone(histo_mc_name.c_str());
	h_tmp =NULL;
	h_data->Scale(h_mc->Integral()/h_data->Integral());
	if(histo_var.at(ivar)=="hMV2c10"||"hMV2c10Flip"||
							"hMV2c10_c"||"hMV2c10F_c"
							"hMV2c10_b"||"hMV2c10F_b")
	{
		h_data->GetXaxis()->SetRangeUser(-1,1);
        h_mc->GetXaxis()->SetRangeUser(-1,1);
	}

	if(histo_var.at(ivar)=="hpT")
	{
		h_data->GetXaxis()->SetRangeUser(0,3000);
        h_mc->GetXaxis()->SetRangeUser(0,3000);
	}

	if(histo_var.at(ivar)=="heta")
	{
		h_data->GetXaxis()->SetRangeUser(-2.5,2.5);
        h_mc->GetXaxis()->SetRangeUser(-2.5,2.5);
	}

	if(histo_var.at(ivar)=="hphi")
	{
		h_data->GetXaxis()->SetRangeUser(-3.5,3.5);
        h_mc->GetXaxis()->SetRangeUser(-3.5,3.5);
	}

	if(histo_var.at(ivar)=="hntrk")
	{
		h_data->GetXaxis()->SetRangeUser(0,30);
        h_mc->GetXaxis()->SetRangeUser(0,30);
	}

	if(histo_var.at(ivar)=="hNjet")
	{
	h_data->GetXaxis()->SetRangeUser(0,15);
        h_mc->GetXaxis()->SetRangeUser(0,15);
	}

    if(flag_logy){      
        h_data->SetMaximum(h_data->GetMaximum()*10000);
        h_mc->SetMaximum(h_mc->GetMaximum()*10000);
      }	
      if(!flag_logy){      
        h_data->SetMaximum(h_data->GetMaximum()*1.3);
        h_mc->SetMaximum(h_mc->GetMaximum()*1.3);
      }

	std::string histo_ratio_name = "ratio_" + histo_var.at(ivar);
	TH1D *h_ratio = (TH1D*)h_data->Clone(histo_ratio_name.c_str());
	h_ratio->Divide(h_mc);
	if(histo_var.at(ivar)=="hpT"||histo_var.at(ivar)=="hntrk"||histo_var.at(ivar)=="hNjet"){
	h_ratio->SetMaximum(4);
	h_ratio->SetMinimum(0);}
	else {
	h_ratio->SetMaximum(1.4);
        h_ratio->SetMinimum(.6);}	


	// cosmetics
    h_mc->SetLineColor(kRed+1);
    h_mc->SetMarkerColor(kRed+1);
    h_mc->SetXTitle(histo_Xlabels.at(ivar).c_str());
    h_mc->SetYTitle("Events");
    h_mc->SetTitle("");
    h_mc->GetXaxis()->SetLabelSize(0);
    h_mc->GetXaxis()->SetTitleSize(0);
    h_mc->GetYaxis()->SetLabelSize(0.06);
    h_mc->GetYaxis()->SetTitleSize(0.06);
    h_mc->GetYaxis()->SetTitleOffset(1.30);
    h_mc->GetYaxis()->SetNdivisions(505);

    h_ratio->SetXTitle(histo_Xlabels.at(ivar).c_str());
    h_ratio->SetYTitle("Data / MC");
    h_ratio->SetTitle("");
    h_ratio->GetYaxis()->SetLabelSize(0.12);
    h_ratio->GetYaxis()->SetTitleSize(0.12);
    h_ratio->GetYaxis()->SetTitleOffset(0.60);
    h_ratio->GetYaxis()->SetNdivisions(505);
    h_ratio->GetXaxis()->SetLabelSize(0.12);
    h_ratio->GetXaxis()->SetTitleSize(0.12);
    h_ratio->GetXaxis()->SetTitleOffset(1.36);

    TLegend* legend = new TLegend(0.580201,0.561807,0.780702,0.706947);
    legend->SetTextFont(42);
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);  // no border
    legend->SetFillColor(0);   // Legend background should be white
    legend->SetTextSize(0.04); // Increase entry font size!
    legend->AddEntry(h_data,"Data","LPE");
    legend->AddEntry(h_mc,"Pythia MC","LPE");

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
    
    std::string canvas_name = "c_" + histo_var.at(ivar);
    std::string pad1_name = "pad1_" + histo_var.at(ivar);
    std::string pad2_name = "pad2_" + histo_var.at(ivar);
    TCanvas *c = new TCanvas(canvas_name.c_str(), "", 800, 900);
    TPad* pad1 = new TPad(pad1_name.c_str(),"", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0.1);
    TPad* pad2 = new TPad(pad2_name.c_str(),"", 0, 0.01, 1, 0.35);
    pad2->SetBottomMargin(0.35);
    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    if(flag_logy) pad1->SetLogy();
    if(flag_logx) pad1->SetLogx();

    h_mc->Draw("LPE");
    h_data->Draw("LPESAME");
    h_data->Draw("AXISSAME");
    legend->Draw();
    pt->Draw();
    pt2->Draw();
    pt3->Draw();

    pad2->cd();
    if(flag_logx) pad2->SetLogx();
    h_ratio->Draw("LPE");
    h_ratio->Draw("AXISSAME");

    f_output->cd();
    c->Write();
    c->SaveAs(("Control_plots/jpg/"+canvas_name+".jpg").c_str());

	

std::cout << "OK" << std::endl ;
   }
f_output->Write();
//f_output->Close();
}





































