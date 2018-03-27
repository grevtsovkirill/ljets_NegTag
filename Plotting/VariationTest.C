#include <TRandom.h>





void VariationTest(){


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
        const std::string WP[n_WP] = {"85", "77", "70", "60"};
        const std::string Tag[2] = {"", "Neg"};
        const std::string Flav[3] = {"l", "b", "c"};
	TRandom3 *r1 = new TRandom3(62292);
	

	// Since it is rare that you NEED to make all taggers' scale factors simultaneously,
	// here is where you set which tagger you want to calculate scale factors for.
	// Multiple can be booked in ../HistoMaker/sfHistoMaker.cpp, so select accordingly.

	TString WhatTagger = "MV2c10";

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

                for (int i=1; i < n_pt + 1; i++){
                for (int j=1; j < n_eta + 1; j++){
                for (int k=0; k < n_WP; k++){
	                std::string id_l = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "__l" ;
                        std::string id_c = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "__c" ;
                        std::string id_b = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +"_" + WhatTagger + "__b" ;
                        std::string id_l_neg = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] + "_" + WhatTagger + "_Neg_l" ;
                        std::string id_c_neg = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "_Neg_c" ;
                        std::string id_b_neg = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "_Neg_b" ;
                        std::string id_data_neg = "datah_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "_Neg" ;
                        std::string id_l_f = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "__l_Fail" ;
                        std::string id_c_f = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "__c_Fail" ;
                        std::string id_b_f = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "__b_Fail" ;
                        std::string id_l_neg_f = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "_Neg_l_Fail" ;
                        std::string id_c_neg_f = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "_Neg_c_Fail" ;
                        std::string id_b_neg_f = "mch_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "_Neg_b_Fail" ;
                        std::string id_data_neg_f = "datah_" + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k] +  "_" + WhatTagger + "_Neg_Fail" ;

                        TH1D *data_neg = (TH1D*)f_data->Get(id_data_neg.c_str());
                        TH1D *mch_l = (TH1D*)f_mc->Get(id_l.c_str());
                        TH1D *mch_c = (TH1D*)f_mc->Get(id_c.c_str());
                        TH1D *mch_b = (TH1D*)f_mc->Get(id_b.c_str());
                        TH1D *mch_l_neg = (TH1D*)f_mc->Get(id_l_neg.c_str());
                        TH1D *mch_c_neg = (TH1D*)f_mc->Get(id_c_neg.c_str());
                        TH1D *mch_b_neg = (TH1D*)f_mc->Get(id_b_neg.c_str());
                        TH1D *data_neg_f = (TH1D*)f_data->Get(id_data_neg_f.c_str());
                        TH1D *mch_l_f = (TH1D*)f_mc->Get(id_l_f.c_str());
                        TH1D *mch_c_f = (TH1D*)f_mc->Get(id_c_f.c_str());
                        TH1D *mch_b_f = (TH1D*)f_mc->Get(id_b_f.c_str());
                        TH1D *mch_l_neg_f = (TH1D*)f_mc->Get(id_l_neg_f.c_str());
                        TH1D *mch_c_neg_f = (TH1D*)f_mc->Get(id_c_neg_f.c_str());
                        TH1D *mch_b_neg_f = (TH1D*)f_mc->Get(id_b_neg_f.c_str());

                        text << "Working point and bins are " << WP[k] << "_" << i << "_" << j << std::endl;
                        
                int highbin = mch_l->GetNbinsX();

// Numbers to be varied:
// These will be taken from the corresponding histogram for each pt/eta/wp
double e_nlp = 0;
double e_nlps = 0;
double e_ncp = 0;
double e_nbp = 0;
double e_nlf = 0;
double e_ncf = 0;
double e_nbf = 0;
double N_l_p = mch_l_neg->IntegralAndError(1, highbin, e_nlp, "");
double N_l_p_S = mch_l->IntegralAndError(1, highbin, e_nlps, "");;
double N_c_p = mch_c_neg->IntegralAndError(1, highbin, e_ncp, "");
double N_b_p = mch_b_neg->IntegralAndError(1, highbin, e_nbp, "");
double N_l_f = mch_l_neg_f->IntegralAndError(1, highbin, e_nlf, "");
double N_c_f = mch_c_neg_f->IntegralAndError(1, highbin, e_ncf, "");
double N_b_f = mch_b_neg_f->IntegralAndError(1, highbin, e_nbf, "");
double N_d_p = data_neg->Integral();
double N_d_f = data_neg_f->Integral();


// Composite values that need to be used and not varied:

double N_incl_l = N_l_p + N_l_f;
double N_incl_c = N_c_p + N_c_f;
double N_incl_b = N_b_p + N_b_f;
double N_incl_d = N_d_p + N_d_f;
double N_incl_mc_p = N_l_p + N_c_p + N_b_p;
double N_incl_mc_f = N_l_f + N_c_f + N_b_f;
double N_incl_mc = N_incl_mc_p + N_incl_mc_f;
double e_Ninclmc = sqrt(pow(e_ncp,2) + pow(e_nlp,2) + pow(e_nbp,2) + pow(e_ncf,2) + pow(e_nbf,2) + pow(e_nlf,2));
double fracc = N_incl_c / N_incl_mc;
double fracb = N_incl_b / N_incl_mc;

// Define histogram for each wp/eta/pt to be filled with tmp h_epsldata
	std::map<TString,TH1D*> h_el;
	TString varhistname = "h_el_ " + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k];
	h_el[varhistname] = new TH1D(varhistname, varhistname, 10000, 0, 0.5);
        TString varthistnamekll = "h_kll_ " + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k];
        h_el[varthistnamekll] = new TH1D(varthistnamekll, varthistnamekll, 10000, 0, 10);
        TString varthistnamekhf = "h_khf_ " + std::to_string(i) + "_" + std::to_string(j) + "_" + WP[k];
        h_el[varthistnamekhf] = new TH1D(varthistnamekhf, varthistnamekhf, 10000, 0, 10);
	 

for (int n = 0; n < 10000; n++){

// Produce random values based on poisson distribution from mean of observed value.
	double tmp_nlp = r1->PoissonD(N_l_p);
	double tmp_ncp = r1->PoissonD(N_c_p);
	double tmp_nbp = r1->PoissonD(N_b_p);
	double tmp_nlf = r1->PoissonD(N_l_f);
	double tmp_ncf = r1->PoissonD(N_c_f);
	double tmp_nbf = r1->PoissonD(N_b_f);
	double tmp_ndp = r1->PoissonD(N_d_p);
	double tmp_ndf = r1->PoissonD(N_d_f);
	double tmp_nlpS = r1->PoissonD(N_l_p_S);
// Define the composite variables for after variation.
	double tmp_N_incl_l = tmp_nlp + tmp_nlf;
	double tmp_N_incl_c = tmp_ncp + tmp_ncf;
	double tmp_N_incl_b = tmp_nbp + tmp_nbf;
	double tmp_N_incl_d = tmp_ndp + tmp_ndf;
	double tmp_N_incl_mc_p = tmp_nlp + tmp_ncp + tmp_nbp;
	double tmp_N_incl_mc_f = tmp_nlf + tmp_ncf + tmp_nbf;
	double tmp_N_incl_mc = tmp_N_incl_mc_p + tmp_N_incl_mc_f;
	double tmp_fracb = tmp_N_incl_b / tmp_N_incl_mc;
	double tmp_fracc = tmp_N_incl_c / tmp_N_incl_mc;
	// Calculate tmp values of Negative tag variables.

	double tmp_Kll = (tmp_nlpS)/(tmp_nlp);
	//std::cout << "Kll is :" <<  tmp_Kll << std::endl;
	double tmp_epsc = (tmp_ncp/tmp_N_incl_c);
	double tmp_epsb = (tmp_nbp/tmp_N_incl_b);
	double tmp_epsl = (tmp_nlp/tmp_N_incl_l);
	double tmp_Khf = 1./((tmp_fracc * (tmp_epsc/tmp_epsl) + tmp_fracb * (tmp_epsb/tmp_epsl)) + (1-(tmp_fracc+tmp_fracb)));
	//std::cout << "Khf is :" << tmp_Khf << std::endl;
	double tmp_epsd = (tmp_ndp/tmp_N_incl_d);
	//std::cout << "Epsd is: " << tmp_epsd << endl;
	double tmp_el = tmp_epsd * tmp_Khf * tmp_Kll;
	//std::cout << "Epsl is :" << tmp_el << std::endl;
	h_el[varhistname]->Fill(tmp_el);
	h_el[varhistnamekll]->Fill(tmp_Kll);
	h_el[varhistnamekhf]->Fill(tmp_Khf);

}
	double e_el = h_el[varhistname]->GetRMS();
	double el_mean = h_el[varhistname]->GetMean();
        text << "el is " << el_mean << "+-" << e_el << std::endl;
        double e_kll = h_el[varhistnamekll]->GetRMS();
        double kll_mean = h_el[varhistnamekll]->GetMean();
        text << "Kll is " << kll_mean << "+-" << e_kll << std::endl;
        double e_khf = h_el[varhistnamekhf]->GetRMS();
        double khf_mean = h_el[varhistnamekhf]->GetMean();
        text << "Khf is " << khf_mean << "+-" << e_khf << std::endl;

	double epslmcnom = (N_l_p_S/N_incl_l);
	double e_epslmc = epslmcnom * sqrt(pow(e_Ninclmc/N_incl_mc,2) + pow(e_nlps/N_l_p_S,2));
        text << "epslmcnom is " << epslmcnom << "+-" << e_epslmc << std::endl;
	double SFl = (el_mean/epslmcnom);
	double e_SFl = SFl * sqrt(pow(e_epslmc/epslmcnom,2) + pow(e_el/el_mean,2));
	text << "Scale Factor is " << SFl << "+-" << e_SFl << std::endl;
                // Fill histogram bins
                h_fracc_mc->SetBinContent(i,j,fracc);
                h_fracb_mc->SetBinContent(i,j,fracb);
                if (k == 0){
                        if (j == 1){
                               sfh_85_central->SetBinContent(i,SFl);
                               sfh_85_central->SetBinError(i, e_SFl);
                                        }
                        else{
                               sfh_85_forward->SetBinContent(i,SFl);
                               sfh_85_forward->SetBinError(i, e_SFl);
                                        }
                            }
                if (k == 1){
                        if (j == 1){
                               sfh_77_central->SetBinContent(i,SFl);
                               sfh_77_central->SetBinError(i, e_SFl);
                                        }
                        else{
                               sfh_77_forward->SetBinContent(i,SFl);
                               sfh_77_forward->SetBinError(i, e_SFl);
                                        }
                             }
                  if (k == 2){
                          if (j == 1){
                                 sfh_70_central->SetBinContent(i,SFl);
                                 sfh_70_central->SetBinError(i, e_SFl);
                                        }
                          else{
                                 sfh_70_forward->SetBinContent(i,SFl);
                                 sfh_70_forward->SetBinError(i, e_SFl);
                                        }
                                }
                  if (k == 3){
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

	f_output->Write();
//f_output->Close();




}







