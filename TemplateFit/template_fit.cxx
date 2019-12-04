#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TSystem.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "string.h"
using namespace std;

#include "../config/conf.hpp"
#include "../helpers/OutputHelper.hpp"
#include "../helpers/parser.hpp"
#include "../AtlasStyle/AtlasStyle.C"

int template_fit(string compaigne="a",bool flag_corrections=1, bool leading_jet=1, int bootstrap_index=0)
{
   // Load ATLAS style
   SetAtlasStyle();

  //**********OUTPUT*********///
  std::string output_name = "template_fit_"+compaigne+".root";
  if(!leading_jet)      output_name += ".subldg";

  if(!flag_corrections) output_name += ".noCorrections";
  else                  output_name += ".pTetaCorrections";

  if(bootstrap_index!=0) output_name = "bootstrap/" + output_name + "_" + to_string(bootstrap_index);

  TFile *f_out = new TFile(output_name.c_str(),"RECREATE");

  //**********INPUT***********//
  //TFile *f_MCbased = TFile::Open("../external/effPlot_fineEta_wC_FixedCutBEff_ALL.root");
  TFile *f_MCbased = TFile::Open("../external/effPlot_FixedCutBEff_ALL_dijetPy8_25M.root");

  std::string      f_data_name = "../NtupleReader/res/FlavourTagging_Nominal/data_"+compaigne+".root";
  if(!leading_jet) f_data_name = "../NtupleReader/res/subleadingjet/data_"+compaigne+".root";

  std::string      f_mc_name = "../NtupleReader/res/FlavourTagging_Nominal/mc_"+compaigne+".root";
  if(!leading_jet) f_mc_name = "../NtupleReader/res/subleadingjet/mc_"+compaigne+".root";

  TFile *f_data = new TFile(f_data_name.c_str(),"READ");
  TFile *f_mc = new TFile(f_mc_name.c_str(),"READ");
  TFile *f_mc_HERWIG = new TFile(("../NtupleReader/res/generator/mc_"+compaigne+".root").c_str(),"READ");

  //**********VARIABLES***********//
  // observable
  RooRealVar* MV2c10_output = new RooRealVar("MV2c10_output","MV2c10_output",-1.0,1.0);

  // fitting parameters
  RooRealVar* N_l = new RooRealVar("N_l","N_l", 100000., 0., 1000000000000000.);
  RooRealVar* N_c = new RooRealVar("N_c","N_c", 100000., 0., 1000000000000000.);
  RooRealVar* N_b = new RooRealVar("N_b","N_b", 100000., 0., 1000000000000000.);

  RooFormulaVar *fracl = new RooFormulaVar("fracl","","@0/(@0+@1+@2)",RooArgList(*N_l,*N_c,*N_b));
  RooFormulaVar *fracc = new RooFormulaVar("fracc","","@0/(@0+@1+@2)",RooArgList(*N_c,*N_l,*N_b));
  RooFormulaVar *fracb = new RooFormulaVar("fracb","","@0/(@0+@1+@2)",RooArgList(*N_b,*N_c,*N_l));

  //**********FINAL HISTOS***********//
  f_out->cd();
  TH2D *h_fracl_mc = new TH2D("h_fracl_mc","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
  TH2D *h_fracl_mc_HERWIG = new TH2D("h_fracl_mc_HERWIG","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
  TH2D *h_fracl_data = new TH2D("h_fracl_data","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);

  TH2D *h_fracc_mc = new TH2D("h_fracc_mc","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
  TH2D *h_fracc_mc_HERWIG = new TH2D("h_fracc_mc_HERWIG","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
  TH2D *h_fracc_data = new TH2D("h_fracc_data","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);

  TH2D *h_fracb_mc = new TH2D("h_fracb_mc","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
  TH2D *h_fracb_mc_HERWIG = new TH2D("h_fracb_mc_HERWIG","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
  TH2D *h_fracb_data = new TH2D("h_fracb_data","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);

  TH2D *h_fracb_var = new TH2D("h_fracb_var","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
  TH2D *h_fracc_var = new TH2D("h_fracc_var","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);

  TH2D *h_chi2 = new TH2D("h_chi2","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
  TH2D *h_chi2_ROOT = new TH2D("h_chi2_ROOT","", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);

  //**********LOOP ON THE pT/eta BINS***********//
  std::map<int, std::map<int, std::string>> kin_labels = getKinLabels(); // take [ipt][ieta] and returns a string "ptXetaY"

  std::cout << "HHHHHHHHHHHHHHHHHHHHHHHHHH================="<< std::endl;

  for (auto tagger: conf::tagger_list){
    for (auto p_pt: kin_labels){
      for (auto p_eta: p_pt.second){
	std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAA================="<< p_eta.second<< std::endl;
      }
    }
  }
  

  for (auto p_pt: kin_labels){

    int ipt = p_pt.first; // pT bin number

    for (auto p_eta: p_pt.second){

      int ieta = p_eta.first; // eta bin number

      // identifiers
      std::string identifier_l = "w_l_" + p_eta.second + "_MV2c10";
      std::string identifier_c = "w_c_" + p_eta.second + "_MV2c10";
      std::string identifier_b = "w_b_" + p_eta.second + "_MV2c10";
      std::string identifier_incl = "w__" + p_eta.second + "_MV2c10";
 
      std::string identifier_l_mc = identifier_l;
      std::string identifier_c_mc = identifier_c;
      std::string identifier_b_mc = identifier_b;
      if(bootstrap_index!=0)
      {
        identifier_l_mc = "bootstrap/" + identifier_l_mc + "_mc_" + to_string(bootstrap_index);
        identifier_c_mc = "bootstrap/" + identifier_c_mc + "_mc_" + to_string(bootstrap_index);
        identifier_b_mc = "bootstrap/" + identifier_b_mc + "_mc_" + to_string(bootstrap_index);
      }

      // get data (inclusive flavour only)
      TH1D *h_data = (TH1D*)f_data->Get(identifier_incl.c_str());
      RooDataHist* dh_data = new RooDataHist("dh_data","",*MV2c10_output,h_data);

      // get model (MC, l, b, c, incl)
      TH1D *h_l = (TH1D*)f_mc->Get(identifier_l_mc.c_str());
      TH1D *h_c = (TH1D*)f_mc->Get(identifier_c_mc.c_str());
      TH1D *h_b = (TH1D*)f_mc->Get(identifier_b_mc.c_str());

      double N_l_mc = h_l->Integral();
      double N_c_mc = h_c->Integral();
      double N_b_mc = h_b->Integral();
      double N_inc_mc = N_l_mc + N_c_mc + N_b_mc;

      h_fracl_mc->SetBinContent(ipt,ieta,N_l_mc/N_inc_mc);
      h_fracc_mc->SetBinContent(ipt,ieta,N_c_mc/N_inc_mc);
      h_fracb_mc->SetBinContent(ipt,ieta,N_b_mc/N_inc_mc);

      // MC stat uncertainty from bootstrap (mcstat accounted only, dominant)
      // for FRAC MC
      TH1D *h_fracl_bootstrap = new TH1D((string("h_fracl_bootstrap_")+identifier_l).c_str(),"",100000.,0., 1.);
      TH1D *h_fracc_bootstrap = new TH1D((string("h_fracc_bootstrap_")+identifier_c).c_str(),"",100000.,0., 1.);
      TH1D *h_fracb_bootstrap = new TH1D((string("h_fracb_bootstrap_")+identifier_b).c_str(),"",100000.,0., 1.);
      /*
      for(int ibootstrap=1; ibootstrap<1001; ibootstrap++)
      {
        //std::cout << ibootstrap << std::endl;       

        std::string bootstrap_identifier_l = "bootstrap/" + identifier_l + "_mc_" + 
                                             to_string(ibootstrap); 
        std::string bootstrap_identifier_c = "bootstrap/" + identifier_c + "_mc_" + 
                                             to_string(ibootstrap); 
        std::string bootstrap_identifier_b = "bootstrap/" + identifier_b + "_mc_" + 
                                             to_string(ibootstrap); 
        std::string bootstrap_identifier_incl = identifier_incl + "_mc_" + to_string(ibootstrap); 

        TH1D *h_l_bootstrap = (TH1D*)f_mc->Get(bootstrap_identifier_l.c_str());
        TH1D *h_c_bootstrap = (TH1D*)f_mc->Get(bootstrap_identifier_c.c_str());
        TH1D *h_b_bootstrap = (TH1D*)f_mc->Get(bootstrap_identifier_b.c_str());
        TH1D *h_incl_bootstrap = (TH1D*)h_l_bootstrap->Clone(bootstrap_identifier_incl.c_str());
        h_incl_bootstrap->Add(h_c_bootstrap);
        h_incl_bootstrap->Add(h_b_bootstrap);

        h_fracl_bootstrap->Fill(h_l_bootstrap->Integral()/h_incl_bootstrap->Integral());
        h_fracc_bootstrap->Fill(h_c_bootstrap->Integral()/h_incl_bootstrap->Integral());
        h_fracb_bootstrap->Fill(h_b_bootstrap->Integral()/h_incl_bootstrap->Integral());
      }
      */

      // MC uncertainty on fractions (bootstrap mean - nominal + bootstrap RMS)
      // only MC bootstrap (effect of data stat over reweighting neglected) 
      double mean_l = h_fracl_mc->GetBinContent(ipt,ieta);
      double mean_c = h_fracc_mc->GetBinContent(ipt,ieta);
      double mean_b = h_fracb_mc->GetBinContent(ipt,ieta);
  
      double bootstrap_mean_l = h_fracl_bootstrap->GetMean();
      double bootstrap_mean_c = h_fracc_bootstrap->GetMean();
      double bootstrap_mean_b = h_fracb_bootstrap->GetMean();

      double bootstrap_rms_l = h_fracl_bootstrap->GetRMS();
      double bootstrap_rms_c = h_fracc_bootstrap->GetRMS();
      double bootstrap_rms_b = h_fracb_bootstrap->GetRMS();

      double final_l = sqrt((mean_l-bootstrap_mean_l)*(mean_l-bootstrap_mean_l)+bootstrap_rms_l*bootstrap_rms_l);
      double final_c = sqrt((mean_c-bootstrap_mean_c)*(mean_c-bootstrap_mean_c)+bootstrap_rms_c*bootstrap_rms_c);
      double final_b = sqrt((mean_b-bootstrap_mean_b)*(mean_b-bootstrap_mean_b)+bootstrap_rms_b*bootstrap_rms_b);

      h_fracl_mc->SetBinError(ipt,ieta,final_l);
      h_fracc_mc->SetBinError(ipt,ieta,final_c);
      h_fracb_mc->SetBinError(ipt,ieta,final_b);

      // HERWIG fractions for LEADING JET ONLY (no bootstrap available)
      TH1D *h_l_HERWIG = (TH1D*)f_mc_HERWIG->Get(identifier_l.c_str());
      TH1D *h_c_HERWIG = (TH1D*)f_mc_HERWIG->Get(identifier_c.c_str());
      TH1D *h_b_HERWIG = (TH1D*)f_mc_HERWIG->Get(identifier_b.c_str());
      TH1D *h_incl_HERWIG = (TH1D*)f_mc_HERWIG->Get(identifier_incl.c_str());

      double N_l_mc_HERWIG = h_l_HERWIG->Integral();
      double N_c_mc_HERWIG = h_c_HERWIG->Integral();
      double N_b_mc_HERWIG = h_b_HERWIG->Integral();
      double N_inc_mc_HERWIG = h_incl_HERWIG->Integral();

      h_fracl_mc_HERWIG->SetBinContent(ipt,ieta,N_l_mc_HERWIG/N_inc_mc_HERWIG);
      h_fracc_mc_HERWIG->SetBinContent(ipt,ieta,N_c_mc_HERWIG/N_inc_mc_HERWIG);
      h_fracb_mc_HERWIG->SetBinContent(ipt,ieta,N_b_mc_HERWIG/N_inc_mc_HERWIG);

      // apply correct d0/z0 smearing
      if(flag_corrections)
      {
        // find pT/eta bin
        std::string eta_label = "cenEta_pt_";
        if(ieta==2) eta_label = "frwEta_pt_";

        // final names
        std::string base_histoName_LF_d0 = eta_label + "mv2c10wp_LF_RES_D0_MEAS_var";
        std::string base_histoName_C_d0  = eta_label + "mv2c10wp_C_RES_D0_MEAS_var";
        std::string base_histoName_B_d0  = eta_label + "mv2c10wp_B_RES_D0_MEAS_var";
        std::string base_histoName_LF_z0 = eta_label + "mv2c10wp_LF_RES_Z0_MEAS_var";
        std::string base_histoName_C_z0  = eta_label + "mv2c10wp_C_RES_Z0_MEAS_var";
        std::string base_histoName_B_z0  = eta_label + "mv2c10wp_B_RES_Z0_MEAS_var";        

        TH1D *h_rew_LF_d0 = (TH1D*)((TH2D*)f_MCbased->Get(base_histoName_LF_d0.c_str()))->ProjectionY("h_rew_LF_d0",ipt,ipt);
        TH1D *h_rew_C_d0 = (TH1D*)((TH2D*)f_MCbased->Get(base_histoName_C_d0.c_str()))->ProjectionY("h_rew_C_d0",ipt,ipt);
        TH1D *h_rew_B_d0 = (TH1D*)((TH2D*)f_MCbased->Get(base_histoName_B_d0.c_str()))->ProjectionY("h_rew_B_d0",ipt,ipt);
        TH1D *h_rew_LF_z0 = (TH1D*)((TH2D*)f_MCbased->Get(base_histoName_LF_z0.c_str()))->ProjectionY("h_rew_LF_z0",ipt,ipt);
        TH1D *h_rew_C_z0 = (TH1D*)((TH2D*)f_MCbased->Get(base_histoName_C_z0.c_str()))->ProjectionY("h_rew_C_z0",ipt,ipt);
        TH1D *h_rew_B_z0 = (TH1D*)((TH2D*)f_MCbased->Get(base_histoName_B_z0.c_str()))->ProjectionY("h_rew_B_z0",ipt,ipt);

        // set weight to 1 if less than 2 sigma away from 1
        for(int ibin=1; ibin<h_rew_LF_d0->GetNbinsX()+1; ibin++)
        {
          if(h_rew_LF_d0->GetBinContent(ibin)==0 || abs(1-h_rew_LF_d0->GetBinContent(ibin))<2*h_rew_LF_d0->GetBinError(ibin)) h_rew_LF_d0->SetBinContent(ibin, 1.);
          if(h_rew_C_d0->GetBinContent(ibin)==0 || abs(1-h_rew_C_d0->GetBinContent(ibin))<2*h_rew_C_d0->GetBinError(ibin)) h_rew_C_d0->SetBinContent(ibin, 1.);
          if(h_rew_B_d0->GetBinContent(ibin)==0 || abs(1-h_rew_B_d0->GetBinContent(ibin))<2*h_rew_B_d0->GetBinError(ibin)) h_rew_B_d0->SetBinContent(ibin, 1.);
          if(h_rew_LF_z0->GetBinContent(ibin)==0 || abs(1-h_rew_LF_z0->GetBinContent(ibin))<2*h_rew_LF_z0->GetBinError(ibin)) h_rew_LF_z0->SetBinContent(ibin, 1.);
          if(h_rew_C_z0->GetBinContent(ibin)==0 || abs(1-h_rew_C_z0->GetBinContent(ibin))<2*h_rew_C_z0->GetBinError(ibin)) h_rew_C_z0->SetBinContent(ibin, 1.);
          if(h_rew_B_z0->GetBinContent(ibin)==0 || abs(1-h_rew_B_z0->GetBinContent(ibin))<2*h_rew_B_z0->GetBinError(ibin)) h_rew_B_z0->SetBinContent(ibin, 1.);
        }

        h_rew_LF_d0->Multiply(h_rew_LF_z0);
        h_rew_B_d0->Multiply(h_rew_B_z0);
        h_rew_C_d0->Multiply(h_rew_C_z0);

        h_l->Multiply(h_rew_LF_d0);
        h_b->Multiply(h_rew_B_d0);
        h_c->Multiply(h_rew_C_d0);
      }

      // define h_incl AFTER corrections
      std::string identifier_incl_mc = identifier_incl + "_mc";
      TH1D *h_incl = (TH1D*)h_l->Clone(identifier_incl_mc.c_str());
      h_incl->Add(h_c);
      h_incl->Add(h_b);

      // build RooFit objects
      RooDataHist* dh_l = new RooDataHist("dh_l","",*MV2c10_output,h_l);
      RooDataHist* dh_c = new RooDataHist("dh_c","",*MV2c10_output,h_c);
      RooDataHist* dh_b = new RooDataHist("dh_b","",*MV2c10_output,h_b);
      RooDataHist* dh_incl = new RooDataHist("dh_incl","",*MV2c10_output,h_incl);

      RooHistPdf* pdf_l = new RooHistPdf("pdf_l","",*MV2c10_output,*dh_l,0);
      RooHistPdf* pdf_c = new RooHistPdf("pdf_c","",*MV2c10_output,*dh_c,0);
      RooHistPdf* pdf_b = new RooHistPdf("pdf_b","",*MV2c10_output,*dh_b,0);

      RooExtendPdf* extPdf_l = new RooExtendPdf("extPdf_l","",*pdf_l,*N_l);
      RooExtendPdf* extPdf_c = new RooExtendPdf("extPdf_c","",*pdf_c,*N_c);
      RooExtendPdf* extPdf_b = new RooExtendPdf("extPdf_b","",*pdf_b,*N_b);

      RooAddPdf *model = new RooAddPdf("model","",RooArgList(*extPdf_l,*extPdf_c,*extPdf_b));

      // pdf sanity check and plot
      model->fitTo(*dh_incl);

      std::string histo_check_identifier = "_check_" + p_eta.second + "_MV2c10";
      TH1D *h_l_check = (TH1D*)h_l->Clone((string("h_l")+histo_check_identifier).c_str());
      TH1D *h_c_check = (TH1D*)h_c->Clone((string("h_c")+histo_check_identifier).c_str());
      TH1D *h_b_check = (TH1D*)h_b->Clone((string("h_b")+histo_check_identifier).c_str());
      h_l_check->Scale(N_l->getValV()/h_l_check->Integral());
      h_c_check->Scale(N_c->getValV()/h_c_check->Integral());
      h_b_check->Scale(N_b->getValV()/h_b_check->Integral());

      TH1D *h_incl_check = (TH1D*)h_l_check->Clone((string("h_incl")+histo_check_identifier).c_str());
      h_incl_check->Add(h_c_check);
      h_incl_check->Add(h_b_check);

      TH1D *h_ratio_check = (TH1D*)h_incl_check->Clone((string("h_ratio")+histo_check_identifier).c_str());
      h_ratio_check->Divide(h_incl);
      h_ratio_check->SetMaximum(1.2);
      h_ratio_check->SetMinimum(0.8);

      h_l_check->SetLineColor(kOrange-8);
      h_c_check->SetLineColor(kBlue+1);
      h_b_check->SetLineColor(kGreen-8);
      h_incl_check->SetLineColor(kRed+1);


      f_out->cd();
      std::string plot_check_identifier = "c_check_" + p_eta.second + "_MV2c10";
      TCanvas *c_check = new TCanvas(plot_check_identifier.c_str());
      c_check->Divide(1,2);

      c_check->cd(2);
      gPad->SetPad(0,0,0.9,0.35);
      h_ratio_check->Draw("HIST");

      c_check->cd(1);
      gPad->SetPad(0,0.35,0.9,0.9);
      h_incl->Draw("LPE");
      h_incl_check->Draw("HISTSAME");
      h_l_check->Draw("HISTSAME");
      h_c_check->Draw("HISTSAME");
      h_b_check->Draw("HISTSAME");
      c_check->Write();

      // fit of data and plot - uncertainty from fit (data stat. only)
      RooFitResult* datafit_results = model->fitTo(*dh_data, RooFit::Save());

      h_fracl_data->SetBinContent(ipt,ieta,fracl->getValV());
      h_fracl_data->SetBinError(ipt,ieta,fracl->getPropagatedError(*datafit_results));

      h_fracc_data->SetBinContent(ipt,ieta,fracc->getValV());
      h_fracc_data->SetBinError(ipt,ieta,fracc->getPropagatedError(*datafit_results));

      h_fracb_data->SetBinContent(ipt,ieta,fracb->getValV());
      h_fracb_data->SetBinError(ipt,ieta,fracb->getPropagatedError(*datafit_results));

      // variations - uncertainty from fit (data stat. only)
      double fracc_var = (h_fracc_data->GetBinContent(ipt,ieta)/h_fracc_mc->GetBinContent(ipt,ieta))-1;
      double fracb_var = (h_fracb_data->GetBinContent(ipt,ieta)/h_fracb_mc->GetBinContent(ipt,ieta))-1;

      double fracc_var_err = fracc_var*(h_fracc_data->GetBinError(ipt,ieta)/h_fracc_data->GetBinContent(ipt,ieta)); 
      double fracb_var_err = fracb_var*(h_fracb_data->GetBinError(ipt,ieta)/h_fracb_data->GetBinContent(ipt,ieta)); 

      h_fracc_var->SetBinContent(ipt,ieta,fracc_var);
      h_fracc_var->SetBinError(ipt,ieta,fracc_var_err);
      h_fracb_var->SetBinContent(ipt,ieta,fracb_var);
      h_fracb_var->SetBinError(ipt,ieta,fracb_var_err);

      std::string histo_datafit_identifier = "_datafit_" + p_eta.second + "_MV2c10";
      TH1D *h_l_datafit = (TH1D*)h_l->Clone((string("h_l")+histo_datafit_identifier).c_str());
      TH1D *h_c_datafit = (TH1D*)h_c->Clone((string("h_c")+histo_datafit_identifier).c_str());
      TH1D *h_b_datafit = (TH1D*)h_b->Clone((string("h_b")+histo_datafit_identifier).c_str());
      h_l_datafit->Scale(N_l->getValV()/h_l_datafit->Integral());
      h_c_datafit->Scale(N_c->getValV()/h_c_datafit->Integral());
      h_b_datafit->Scale(N_b->getValV()/h_b_datafit->Integral());

      TH1D *h_incl_datafit = (TH1D*)h_l_datafit->Clone((string("h_incl")+histo_datafit_identifier).c_str());
      h_incl_datafit->Add(h_c_datafit);
      h_incl_datafit->Add(h_b_datafit);

      // chi2 - Roofit
      RooChi2Var chi2 ("chi2", "chi2", *model, *dh_data, RooFit::Extended(kTRUE)); 
      double chi2_val = chi2.getValV(); // only data stat. accounted
      double dof = 0;
      int ibin;
      for(ibin=1; ibin<h_data->GetNbinsX()+1; ibin++)
      {
        if(h_data->GetBinContent(ibin)!=0) dof++;
      }
      dof -= 3; // 3 fitted parameters
      h_chi2->SetBinContent(ipt,ieta,chi2_val/dof);

      // chi2 - ROOT
      double chi2_val_ROOT = h_data->Chi2Test(h_incl_datafit, "WWPCHI2");
      h_chi2_ROOT->SetBinContent(ipt,ieta,chi2_val_ROOT/dof);

      // ratio and cosmetics
      TH1D *h_ratio_datafit = (TH1D*)h_data->Clone((string("h_ratio")+histo_datafit_identifier).c_str());
      h_ratio_datafit->Divide(h_incl_datafit);
      h_ratio_datafit->SetMaximum(1.5);
      h_ratio_datafit->SetMinimum(0.5);
      for(ibin=1; ibin<h_ratio_datafit->GetNbinsX()+1; ibin++) h_ratio_datafit->SetBinError(ibin, h_data->GetBinError(ibin)/h_incl_datafit->GetBinContent(ibin));
      h_ratio_datafit->SetYTitle("Data/Model");

      h_l_datafit->SetLineColor(kOrange-8);
      h_c_datafit->SetLineColor(kBlue+1);
      h_b_datafit->SetLineColor(kGreen-8);
      h_incl_datafit->SetLineColor(kRed+1);

      f_out->cd();
      std::string plot_datafit_identifier = "c_datafit_" + p_eta.second + "_MV2c10";
      std::string plot_datafit_identifier1 = "p1_datafit_" + p_eta.second + "_MV2c10";
      std::string plot_datafit_identifier2 = "p2_datafit_" + p_eta.second + "_MV2c10";
      TCanvas *c_datafit = new TCanvas(plot_datafit_identifier.c_str());
      TPad *pad1 = new TPad(plot_datafit_identifier1.c_str(), "",0.0,0.3,1.0,1.0);
      TPad *pad2 = new TPad(plot_datafit_identifier2.c_str(), "",0.0,0.0,1.0,0.3);
      pad1->Draw();
      pad2->Draw();

      pad1->cd();
      h_data->Draw("LPE");
      h_incl_datafit->Draw("HISTSAME");
      h_l_datafit->Draw("HISTSAME");
      h_c_datafit->Draw("HISTSAME");
      h_b_datafit->Draw("HISTSAME");

      pad2->cd();
      h_ratio_datafit->Draw("LPE");

      c_datafit->Write();
      h_data->Write();
    }//end for (auto p_eta: p_pt.second)
  }//end  (auto p_pt: kin_labels)


 f_out->Write();
 return 0;
}
