#include "../AtlasStyle/AtlasStyle.C"
#include<math.h>



void FracPlotter() { 
	SetAtlasStyle();
	gStyle->SetPaintTextFormat("2.2f%");
	gStyle->SetNumberContours(6);
	TFile *f_mc = new TFile("../HistoMaker/HistoFiles/ConHistoFileFileM.root", "READ");
	TFile *f_out = new TFile("../HistoFiles/Fracs.root", "RECREATE");

	const int n_pt = 7;
	const double pt_lowedges[n_pt+1] = {10, 20, 50, 100, 150, 300, 500, 3000};

	const int n_eta = 2;                                      // current eta bin
    const double eta_lowedges[n_eta+1] = {0.0, 1.2, 2.5};     // (unchanged)


  TH2D *h_fracl_mc = new TH2D("h_fracl_mc","", n_pt, pt_lowedges, n_eta, eta_lowedges);
  TH2D *h_fracb_mc = new TH2D("h_fracb_mc","", n_pt, pt_lowedges, n_eta, eta_lowedges);
  TH2D *h_fracc_mc = new TH2D("h_fracc_mc","", n_pt, pt_lowedges, n_eta, eta_lowedges);






for (int i=1; i < n_pt+1; i++){
	for (int j=1; j < n_eta+1; j++){
		std::string identifier_l = "hMV2c10_l_" + std::to_string(i) + "_" + std::to_string(j);
		std::string identifier_b = "hMV2c10_b_" + std::to_string(i) + "_" + std::to_string(j);
		std::string identifier_c = "hMV2c10_c_" + std::to_string(i) + "_" + std::to_string(j);

		TH1D *h_l = (TH1D*)f_mc->Get(identifier_l.c_str());
		TH1D *h_c = (TH1D*)f_mc->Get(identifier_c.c_str());
		TH1D *h_b = (TH1D*)f_mc->Get(identifier_b.c_str());
	      	h_l->Sumw2();
		h_c->Sumw2();
		h_b->Sumw2();
		int highbin = h_l->GetNbinsX();
		double N_l_error = 0.;
		double N_c_error = 0.;
		double N_b_error = 0.;
		double N_l_mc = h_l->IntegralAndError(1, highbin, N_l_error, "");
	      	double N_c_mc = h_c->IntegralAndError(1, highbin, N_c_error, "" );
	      	double N_b_mc = h_b->IntegralAndError(1, highbin, N_b_error, "");
		double N_inc_error = sqrt((N_l_error * N_l_error) + (N_c_error * N_c_error) + (N_b_error * N_b_error));
	      	double N_inc_mc = N_l_mc + N_c_mc + N_b_mc;
	      	h_fracl_mc->SetBinContent(i,j,N_l_mc/N_inc_mc);
		h_fracl_mc->SetBinError(i, j, (N_l_mc/N_inc_mc) * sqrt(pow(N_inc_error/N_inc_mc,2) + pow(N_l_error/N_l_mc,2))); 
	      	h_fracc_mc->SetBinContent(i,j,N_c_mc/N_inc_mc);
                h_fracc_mc->SetBinError(i, j, (N_c_mc/N_inc_mc) * sqrt(pow(N_inc_error/N_inc_mc,2) + pow(N_c_error/N_c_mc,2)));
	      	h_fracb_mc->SetBinContent(i,j,N_b_mc/N_inc_mc);
                h_fracb_mc->SetBinError(i, j, (N_b_mc/N_inc_mc) * sqrt(pow(N_inc_error/N_inc_mc,2) + pow(N_b_error/N_b_mc,2)));
		
		}


	}
		//h_fracc_mc->Sumw2();
		//h_fracb_mc->Sumw2();
		h_fracc_mc->Scale(100.);
		h_fracb_mc->Scale(100.);
	    h_fracc_mc->SetXTitle("p_{T}^{jet} [GeV]");
	    h_fracb_mc->SetXTitle("p_{T}^{jet} [GeV]");
	    h_fracc_mc->SetYTitle("#eta^{jet}");
	    h_fracb_mc->SetYTitle("#eta^{jet}");
	    h_fracc_mc->GetYaxis()->SetTitleOffset(1.0);
	    h_fracb_mc->GetYaxis()->SetTitleOffset(1.0);
	    h_fracc_mc->SetZTitle("c-fraction (PYTHIA)");
	    h_fracb_mc->SetZTitle("b-fraction (PYTHIA)");
	    h_fracc_mc->GetZaxis()->SetTitleOffset(0.8);
	    h_fracb_mc->GetZaxis()->SetTitleOffset(0.8);
	    h_fracc_mc->SetBarOffset(0.2);
	    h_fracb_mc->SetBarOffset(0.2);
	    h_fracc_mc->SetMarkerSize(1.5);
	    h_fracb_mc->SetMarkerSize(1.5);
	    h_fracc_mc->SetMarkerColor(kBlack);
	    h_fracb_mc->SetMarkerColor(kBlack);

	  TCanvas *c1_fracc = new TCanvas("c1_fracc","",1200,600);
	  c1_fracc->SetLogx();
	  h_fracc_mc->Draw("TEXTCOLZE");
	  c1_fracc->Write();
	  c1_fracc->SaveAs("Control_plots/jpg/Cfrac.jpg");

	  TCanvas *c1_fracb = new TCanvas("c1_fracb","",1200,600);
	  c1_fracb->SetLogx();
	  h_fracb_mc->Draw("TEXTCOLZE");
	  c1_fracb->Write();
	  c1_fracb->SaveAs("Control_plots/jpg/Bfrac.jpg");

	}
