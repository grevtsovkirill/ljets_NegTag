#include "string.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "../config/conf.hpp"
#include "../AtlasStyle/AtlasStyle.C"
#include "../AtlasStyle/AtlasUtils.C"
#include "../helpers/config_reader.cpp" // included to use the read_list() function.

void DrawPlots(TFile* f_da, TFile* f_mc){ // Plotting results only for Nominal Branch.

  SetAtlasStyle();
  gStyle->SetOptStat(0);   

  const int nj = 2;
  // Lopp on leading and sub-leading jet.
  for(int ij=0; ij < nj; ij++){
    string hname = string("FlavourTagging_Nominal/ntrk_IP3D") + char('1'+ij) + string("_pteta_rew");
    TCanvas* c1 = new TCanvas( (string("c_")+hname).c_str(), (string("c_")+hname).c_str(), 0, 0, 600, 700);

    c1->Divide(1,2);
    c1->cd(1)->SetPad(0,0.25, 1.0, 1.0);
    c1->cd(1)->SetLogy();

    TH1D* hmc1 = (TH1D*)f_mc->Get(hname.c_str()); 
    hmc1->SetLineColor(kBlue);
    hmc1->SetLineWidth(3);
    //hmc1->SetXTitle("nTrack per jet");
    hmc1->SetYTitle("#jets");
    hmc1->GetXaxis()->SetMoreLogLabels();
    hmc1->Draw();
  
    TH1D* hda1 = (TH1D*)f_da->Get(hname.c_str()); 
    hda1->SetLineColor(95);
    hda1->SetLineWidth(3);
    hda1->Draw("same");
    //Double_t ymax = hmc1->GetBinContent(hmc1->GetMaximumBin());
    //Double_t yda = hda1->GetBinContent(hda1->GetMaximumBin());
    //if(yda>ymax) ymax = yda;
    // hmc1->SetMaximum(2e6);
    // hmc1->SetMinimum(1e4);
 
    TLegend *pl2 = new TLegend(0.75, 0.75, 0.9, 0.85); 
    if (ij == 0) pl2->SetHeader("leading jet");
    else  pl2->SetHeader("subleading jet");
    pl2->SetTextSize(0.035); 
    pl2->SetFillColor(0);  pl2->SetBorderSize(0);
    pl2->SetShadowColor(0);
    TLegendEntry *ple2 = pl2->AddEntry(hda1, "data", "l"); 
    pl2->AddEntry(hmc1, "mc", "l"); 
    pl2->Draw();
  
    TH1D *h3 = (TH1D *)hda1->Clone("ratio_ntrack");
    h3->Divide(hmc1);
    h3->GetYaxis()->SetTitle("ratio (data/MC) ");
    h3->GetYaxis()->SetTitleOffset(0.3);
    h3->GetYaxis()->SetTitleSize(0.1);
    //cc->cd(2);
    c1->cd(2)->SetPad(0, 0.0, 1.0, 0.3);
    h3->SetMaximum(2.0);
    h3->SetMinimum(0.3);
    h3->SetXTitle("nTrack per jet");
    h3->GetXaxis()->SetTitleOffset(0.5);
    h3->GetXaxis()->SetTitleSize(0.12);
    //h3->GetXaxis()->SetTitle(xtitle.c_str());
    h3->Draw("e0");

    TLine* l1 = new TLine(0.0, 1.0, 26, 1.0);
    l1->SetLineStyle(3);
    l1->SetLineColor(kBlue);
    l1->SetLineWidth(2);
    l1->Draw("same");

    string sfold = "plot/";
    c1->Print( (sfold+hname+"_datamc.eps").c_str());
    c1->Print( (sfold+hname+"_datamc.pdf").c_str());
  }

  //  f_output->Close();
}

void calculate_jetntrackreweighting_HERWIG( string dname="data_FlavourTagging_Nominal_0.root",string mname="mc_FlavourTagging_Nominal_Pythia.root")
{
 // separate reweighting for leading and next-to-leading pt jet in pt:eta
  const int nj = 2;
  auto systematics = read_list("../NtupleDumper/.systematics.auto");
  //systematics.push_back("FlavourTagging_PRW_DATASF__1down");
  //systematics.push_back("FlavourTagging_PRW_DATASF__1up");

  // input
  TFile* f_da = new TFile(dname.c_str());
  TFile* f_mc = new TFile(mname.c_str());

  // output
  TFile *f_output = new TFile("reweight_ntrack2D_HERWIG.root", "RECREATE");

  // ntrk_IP3D/pT 2D histo
  TH2D* hmc[nj];
  TH2D* hda[nj];
  TH2D* hratio[nj];

  for(auto syst: systematics)
    {
      if(syst!="FlavourTagging_Nominal") continue;
      cout << "#####\n# Working on: " << syst << "\n#####" << endl;
      // Create a directory for each systematic in the root file.
      TDirectory *subdir = f_output->mkdir(syst);
      subdir->cd();
      
      // loop on jet
      for(int ij=0; ij < nj; ij++)
	{
	  string hname = string(syst) + string("/ntrk_IP3D_pt") + char('1'+ij)+ string("_pteta_rew");
	  cout << "1\n";
	  cout << "string is: " << hname << endl;
	  hmc[ij] = (TH2D*)f_mc->Get(hname.c_str()); 
	  cout << "2\n";
	  hda[ij] = (TH2D*)f_da->Get( (string("FlavourTagging_Nominal/ntrk_IP3D_pt")+char('1'+ij)+string("_pteta_rew")).c_str() ); // Data has only the nominal Branch.
	  cout << "3\n";

	  hratio[ij] = (TH2D*)hda[ij]->Clone( (string("datamc_ratio")+char('1'+ij)).c_str() );
	  cout << "4\n";
	  hratio[ij]->Divide(hmc[ij]);
	  cout << "5\n";
	  
	  // if hmc or hmc has empty content, set SF to 1 (i.e. no reweighting)
	  for(int i=1; i<hmc[ij]->GetNbinsX()+1; i++)
	    {
	      for(int j=1; j<hmc[ij]->GetNbinsY()+1; j++)
		{
		  if(hmc[ij]->GetBinContent(i,j)==0 || hda[ij]->GetBinContent(i,j)==0)
		    {
		      hratio[ij]->SetBinContent(i,j,1.);
		      hratio[ij]->SetBinError(i,j,0.);
		    }
		}
	    }

	  cout << "6\n";
	  hratio[ij]->Write();
    
	}  // end of looping on jets.

    }      // end of looping on systematics.

  f_output->Close();
  cout << "2\n";

  DrawPlots(f_da, f_mc);

}
