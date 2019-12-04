#define ApplyAllWeights_cxx
#include "ApplyAllWeights.h"
#include <TH2.h>
#include <TFile.h>
#include <TSystem.h>
#include <TString.h>
#include <TRandom3.h>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <cassert>

#include "../config/conf.hpp"
#include "../helpers/OutputHelper.hpp"

using namespace std;
const int debug=0;

extern int runmc;

void ApplyAllWeights::Loop(int bootstrap_bkeeper=0)
{

  const double pi = M_PI, twopi = 2*pi;

  if (fChain == 0) return;

  int currentTree = -1;

  // Control plots

  //----------event-wide----------//
  TH1D* h_njets_event_all_rew; 
  TH1D* h_averageInteractionsPerCrossing_all_rew;

  //----------leading jet----------//
  // 1D - kinematics
  TH1D* h_pt1_all_rew; 
  TH1D* h_eta1_all_rew; 
  TH1D* h_pt_thin1_all_rew; 
  TH1D* h_eta_thin1_all_rew; 
  TH1D* h_phi1_all_rew; 
  // 1D - taggers and subtaggers
  std::vector<TH1D*> h_float_subtagger1_all_rew;
  std::vector<TH1D*> h_int_subtagger1_all_rew;
  std::vector<TH1D*> h_double_subtagger1_all_rew;
  // 1D - ntrk
  TH1D* h_ntrk_IP3DNeg1_all_rew;
  TH1D* h_ntrk_IP3D1_all_rew;
  TH1D* h_ntrk_IP2DNeg1_all_rew;
  TH1D* h_ntrk_IP2D1_all_rew;
  TH1D* h_ntrk_SV1Flip1_all_rew;
  TH1D* h_ntrk_SV11_all_rew;
  TH1D* h_ntrk_JetFitterFlip1_all_rew;
  TH1D* h_ntrk_JetFitter1_all_rew;

  // 2D - kinematics
  TH2D* h_pteta1_all_rew; 
  // 2D - ntrk
  TH2D* h_ntrk_IP3DNeg_pt1_all_rew;
  TH2D* h_ntrk_IP3D_pt1_all_rew;
  TH2D* h_ntrk_IP2DNeg_pt1_all_rew;
  TH2D* h_ntrk_IP2D_pt1_all_rew;
  TH2D* h_ntrk_SV1Flip_pt1_all_rew;
  TH2D* h_ntrk_SV1_pt1_all_rew;
  TH2D* h_ntrk_JetFitterFlip_pt1_all_rew;
  TH2D* h_ntrk_JetFitter_pt1_all_rew;

  //----------subleading jet----------//
  // 1D - kinematics
  TH1D* h_pt2_all_rew; 
  TH1D* h_eta2_all_rew; 
  TH1D* h_pt_thin2_all_rew; 
  TH1D* h_eta_thin2_all_rew; 
  TH1D* h_phi2_all_rew; 
  // 1D - taggers and subtaggers
  std::vector<TH1D*> h_float_subtagger2_all_rew;
  std::vector<TH1D*> h_int_subtagger2_all_rew;
  std::vector<TH1D*> h_double_subtagger2_all_rew;
  // 1D - ntrk
  TH1D* h_ntrk_IP3DNeg2_all_rew;
  TH1D* h_ntrk_IP3D2_all_rew;
  TH1D* h_ntrk_IP2DNeg2_all_rew;
  TH1D* h_ntrk_IP2D2_all_rew;
  TH1D* h_ntrk_SV1Flip2_all_rew;
  TH1D* h_ntrk_SV12_all_rew;
  TH1D* h_ntrk_JetFitterFlip2_all_rew;
  TH1D* h_ntrk_JetFitter2_all_rew;

  // 2D - kinematics
  TH2D* h_pteta2_all_rew; 
  // 2D - ntrk
  TH2D* h_ntrk_IP3DNeg_pt2_all_rew;
  TH2D* h_ntrk_IP3D_pt2_all_rew;
  TH2D* h_ntrk_IP2DNeg_pt2_all_rew;
  TH2D* h_ntrk_IP2D_pt2_all_rew;
  TH2D* h_ntrk_SV1Flip_pt2_all_rew;
  TH2D* h_ntrk_SV1_pt2_all_rew;
  TH2D* h_ntrk_JetFitterFlip_pt2_all_rew;
  TH2D* h_ntrk_JetFitter_pt2_all_rew;

// nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
  if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
  {
    //----------event-wide----------//
    h_njets_event_all_rew = new TH1D("njets_event_all_rew", "", 20, 0, 20); 
    h_averageInteractionsPerCrossing_all_rew = new TH1D("averageInteractionsPerCrossing_all_rew", "", 100, 0, 100); 

    //----------leading jet----------//
    // 1D - kinematics
    h_pt1_all_rew = new TH1D("pt1_all_rew", "", conf::n_pt, conf::pt_lowedges); 
    h_eta1_all_rew = new TH1D("eta1_all_rew", "", conf::n_eta, conf::eta_lowedges);
    h_pt_thin1_all_rew = new TH1D("pt_thin1_all_rew", "", 600, 0., 3000.); 
    h_eta_thin1_all_rew = new TH1D("eta_thin1_all_rew", "", 25, 0., 2.5);
    h_phi1_all_rew = new TH1D("phi1_all_rew", "", 60, -3.16, 3.16);
    // 1D - taggers and subtaggers
    for (const auto &name: subtagger::floats){
      TH1D *h_tmp = new TH1D((name.first + "1_all_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_float_subtagger1_all_rew.push_back(h_tmp);
    }
    for (const auto &name: subtagger::ints){
      TH1D *h_tmp = new TH1D((name.first + "1_all_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_int_subtagger1_all_rew.push_back(h_tmp);
    }
    for (const auto &name: subtagger::doubles){
      TH1D *h_tmp = new TH1D((name.first + "1_all_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_double_subtagger1_all_rew.push_back(h_tmp);
    }
    // 1D - ntrk
    h_ntrk_IP3DNeg1_all_rew = new TH1D("ntrk_IP3DNeg1_all_rew","",25, 0, 25);
    h_ntrk_IP3D1_all_rew = new TH1D("ntrk_IP3D1_all_rew","",25, 0, 25);
    h_ntrk_IP2DNeg1_all_rew = new TH1D("ntrk_IP2DNeg1_all_rew","",25, 0, 25);
    h_ntrk_IP2D1_all_rew = new TH1D("ntrk_IP2D1_all_rew","",25, 0, 25);
    h_ntrk_SV1Flip1_all_rew = new TH1D("ntrk_SV1Flip1_all_rew","",25, 0, 25);
    h_ntrk_SV11_all_rew = new TH1D("ntrk_SV11_all_rew","",25, 0, 25);
    h_ntrk_JetFitterFlip1_all_rew = new TH1D("ntrk_JetFitterFlip1_all_rew","",25, 0, 25);
    h_ntrk_JetFitter1_all_rew = new TH1D("ntrk_JetFitter1_all_rew","",25, 0, 25);

    // 2D - kinematics
    h_pteta1_all_rew = new TH2D("pteta1_all_rew", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
    // 2D - ntrk
    h_ntrk_IP3DNeg_pt1_all_rew = new TH2D("ntrk_IP3DNeg_pt1_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP3D_pt1_all_rew = new TH2D("ntrk_IP3D_pt1_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2DNeg_pt1_all_rew = new TH2D("ntrk_IP2DNeg_pt1_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2D_pt1_all_rew = new TH2D("ntrk_IP2D_pt1_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1Flip_pt1_all_rew = new TH2D("ntrk_SV1Flip_pt1_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1_pt1_all_rew = new TH2D("ntrk_SV1_pt1_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitterFlip_pt1_all_rew = new TH2D("ntrk_JetFitterFlip_pt1_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitter_pt1_all_rew = new TH2D("ntrk_JetFitter_pt1_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);

    //----------subleading jet----------//
    // 1D - kinematics
    h_pt2_all_rew = new TH1D("pt2_all_rew", "", conf::n_pt, conf::pt_lowedges); 
    h_eta2_all_rew = new TH1D("eta2_all_rew", "", conf::n_eta, conf::eta_lowedges);
    h_pt_thin2_all_rew = new TH1D("pt_thin2_all_rew", "", 600, 0., 3000.); 
    h_eta_thin2_all_rew = new TH1D("eta_thin2_all_rew", "", 25, 0., 2.5);
    h_phi2_all_rew = new TH1D("phi2_all_rew", "", 60, -3.16, 3.16);
    // 1D - taggers and subtaggers
    for (const auto &name: subtagger::floats){
      TH1D *h_tmp = new TH1D((name.first + "2_all_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_float_subtagger2_all_rew.push_back(h_tmp);
    }
    for (const auto &name: subtagger::ints){
      TH1D *h_tmp = new TH1D((name.first + "2_all_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_int_subtagger2_all_rew.push_back(h_tmp);
    }
    for (const auto &name: subtagger::doubles){
      TH1D *h_tmp = new TH1D((name.first + "2_all_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_double_subtagger2_all_rew.push_back(h_tmp);
    }
    // 1D - ntrk
    h_ntrk_IP3DNeg2_all_rew = new TH1D("ntrk_IP3DNeg2_all_rew","",25, 0, 25);
    h_ntrk_IP3D2_all_rew = new TH1D("ntrk_IP3D2_all_rew","",25, 0, 25);
    h_ntrk_IP2DNeg2_all_rew = new TH1D("ntrk_IP2DNeg2_all_rew","",25, 0, 25);
    h_ntrk_IP2D2_all_rew = new TH1D("ntrk_IP2D2_all_rew","",25, 0, 25);
    h_ntrk_SV1Flip2_all_rew = new TH1D("ntrk_SV1Flip2_all_rew","",25, 0, 25);
    h_ntrk_SV12_all_rew = new TH1D("ntrk_SV12_all_rew","",25, 0, 25);
    h_ntrk_JetFitterFlip2_all_rew = new TH1D("ntrk_JetFitterFlip2_all_rew","",25, 0, 25);
    h_ntrk_JetFitter2_all_rew = new TH1D("ntrk_JetFitter2_all_rew","",25, 0, 25);

    // 2D - kinematics
    h_pteta2_all_rew = new TH2D("pteta2_all_rew", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
    // 2D - ntrk
    h_ntrk_IP3DNeg_pt2_all_rew = new TH2D("ntrk_IP3DNeg_pt2_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP3D_pt2_all_rew = new TH2D("ntrk_IP3D_pt2_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2DNeg_pt2_all_rew = new TH2D("ntrk_IP2DNeg_pt2_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2D_pt2_all_rew = new TH2D("ntrk_IP2D_pt2_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1Flip_pt2_all_rew = new TH2D("ntrk_SV1Flip_pt2_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1_pt2_all_rew = new TH2D("ntrk_SV1_pt2_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitterFlip_pt2_all_rew = new TH2D("ntrk_JetFitterFlip_pt2_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitter_pt2_all_rew = new TH2D("ntrk_JetFitter_pt2_all_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);

    // Correct uncertainties for weighted histograms
    h_njets_event_all_rew->Sumw2();
    h_averageInteractionsPerCrossing_all_rew->Sumw2();

    h_pt1_all_rew->Sumw2();
    h_eta1_all_rew->Sumw2();
    h_pt_thin1_all_rew->Sumw2();
    h_eta_thin1_all_rew->Sumw2();
    h_phi1_all_rew->Sumw2();

    h_ntrk_IP3DNeg1_all_rew->Sumw2();
    h_ntrk_IP3D1_all_rew->Sumw2();
    h_ntrk_IP2DNeg1_all_rew->Sumw2();
    h_ntrk_IP2D1_all_rew->Sumw2();
    h_ntrk_SV1Flip1_all_rew->Sumw2();
    h_ntrk_SV11_all_rew->Sumw2();
    h_ntrk_JetFitterFlip1_all_rew->Sumw2();
    h_ntrk_JetFitter1_all_rew->Sumw2();

    h_pteta1_all_rew->Sumw2();

    h_ntrk_IP3DNeg_pt1_all_rew->Sumw2();
    h_ntrk_IP3D_pt1_all_rew->Sumw2();
    h_ntrk_IP2DNeg_pt1_all_rew->Sumw2();
    h_ntrk_IP2D_pt1_all_rew->Sumw2();
    h_ntrk_SV1Flip_pt1_all_rew->Sumw2();
    h_ntrk_SV1_pt1_all_rew->Sumw2();
    h_ntrk_JetFitterFlip_pt1_all_rew->Sumw2();
    h_ntrk_JetFitter_pt1_all_rew->Sumw2();

    h_pt2_all_rew->Sumw2();
    h_eta2_all_rew->Sumw2();
    h_pt_thin2_all_rew->Sumw2();
    h_eta_thin2_all_rew->Sumw2();
    h_phi2_all_rew->Sumw2();
 
    h_ntrk_IP3DNeg2_all_rew->Sumw2();
    h_ntrk_IP3D2_all_rew->Sumw2();
    h_ntrk_IP2DNeg2_all_rew->Sumw2();
    h_ntrk_IP2D2_all_rew->Sumw2();
    h_ntrk_SV1Flip2_all_rew->Sumw2();
    h_ntrk_SV12_all_rew->Sumw2();
    h_ntrk_JetFitterFlip2_all_rew->Sumw2();
    h_ntrk_JetFitter2_all_rew->Sumw2();
   
    h_pteta2_all_rew->Sumw2();

    h_ntrk_IP3DNeg_pt2_all_rew->Sumw2();
    h_ntrk_IP3D_pt2_all_rew->Sumw2();
    h_ntrk_IP2DNeg_pt2_all_rew->Sumw2();
    h_ntrk_IP2D_pt2_all_rew->Sumw2();
    h_ntrk_SV1Flip_pt2_all_rew->Sumw2();
    h_ntrk_SV1_pt2_all_rew->Sumw2();
    h_ntrk_JetFitterFlip_pt2_all_rew->Sumw2();
    h_ntrk_JetFitter_pt2_all_rew->Sumw2();
 
  }


  // ************************************************ //
  //                Bootstrap Replicas                //
  // ************************************************ //

  // not used so far

  TH2D* array_data_h_ntrk_pt1[2];
  TH2D* array_data_h_ntrk_pt2[2];
  TH2D* array_mc_h_ntrk_pt1[2];
  TH2D* array_mc_h_ntrk_pt2[2];

  if ( m_systematic.EqualTo("FlavourTagging_Nominal")  && bootstrap_bkeeper>0)
  {
     for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
     {
       array_data_h_ntrk_pt1[ibootstrap] = new TH2D( (string("ntrk_pt1_all_rew_data_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", 25, 0, 25, conf::n_pt, conf::pt_lowedges);

       array_data_h_ntrk_pt2[ibootstrap] = new TH2D( (string("ntrk_pt2_all_rew_data_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", 25, 0, 25, conf::n_pt, conf::pt_lowedges);

       array_data_h_ntrk_pt1[ibootstrap]->Sumw2();
       array_data_h_ntrk_pt2[ibootstrap]->Sumw2();

       if(runmc)
       {
         array_mc_h_ntrk_pt1[ibootstrap] = new TH2D( (string("ntrk_pt1_all_rew_mc_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", 25, 0, 25, conf::n_pt, conf::pt_lowedges);
         array_mc_h_ntrk_pt2[ibootstrap] = new TH2D( (string("ntrk_pt2_all_rew_mc_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", 25, 0, 25, conf::n_pt, conf::pt_lowedges);
         array_mc_h_ntrk_pt1[ibootstrap]->Sumw2();
         array_mc_h_ntrk_pt2[ibootstrap]->Sumw2();
       }

     }

  }  

  // ************************************************ //
  //           End of bootstrap definition            //
  // ************************************************ //

  // pT/eta reweighting
  TFile* frew_pt_eta;
  std::cout<< "Compaigne = "<< m_compagine << std::endl;

  std::string frew_pt_eta_name = "../GetPtEtaWeights/rew_"+m_compagine+".root";
  if(!m_alt.empty()) frew_pt_eta_name = "../GetPtEtaWeights/rew_"+m_compagine+"_HERWIG.root";
  std::cout<< " input pT/eta reweighting:  "<< frew_pt_eta_name << ", alt_gen = "<<m_alt << std::endl;

  frew_pt_eta = new TFile(frew_pt_eta_name.c_str(), "read");
  TH2D* h_rew[2];
  for(int ij=0; ij<2; ij++)
    {
      h_rew[ij] = (TH2D*)frew_pt_eta->Get( (string(m_systematic + "/ptscale")+char('1'+ij)).c_str() );
    }

  // NTrack reweighting (ntracks_IP3DNeg so far) 
  TFile* frew_ntrack_pt;
  std::string frew_ntrack_pt_name = "../GetNTrackWeights/reweight_ntrack2D_"+m_compagine+".root";
  if(!m_alt.empty()) frew_ntrack_pt_name = "../GetNTrackWeights/reweight_ntrack2D_"+m_compagine+"_HERWIG.root"; 
  frew_ntrack_pt = new TFile(frew_ntrack_pt_name.c_str(), "read"); 
  std::cout<< " input Ntrk reweighting:  "<< frew_ntrack_pt_name << std::endl;
  TH2D* h_rew_ntrack[2];
  for(int ij=0; ij<2; ij++)
    {
      h_rew_ntrack[ij] = (TH2D*)frew_ntrack_pt->Get( (string(m_systematic + "/datamc_ratio")+char('1'+ij)).c_str() );
    }


  // Reweighting for bootstrap
  // pT/eta
  TH2D* array_data_h_rew[2][2];
  TH2D* array_mc_h_rew[2][2];

  if ( m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
  {
     for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
     {
       for(int ij=0; ij<2; ij++){
         array_data_h_rew[ij][ibootstrap] = (TH2D*)frew_pt_eta->Get( (string(m_systematic + "/ptscale") + char('1'+ij) + string("_data_")+to_string((bootstrap_bkeeper-1)*2+ibootstrap) ).c_str() );
         array_mc_h_rew[ij][ibootstrap] = (TH2D*)frew_pt_eta->Get( (string(m_systematic + "/ptscale") + char('1'+ij) + string("_mc_")+to_string((bootstrap_bkeeper-1)*2+ibootstrap) ).c_str() );
       }
     }
  }

  // NTrack
  TH2D* array_data_h_rew_ntrack[2][2];
  TH2D* array_mc_h_rew_ntrack[2][2];

  if ( m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
  {
     for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
     {
       for(int ij=0; ij<2; ij++){
         array_data_h_rew_ntrack[ij][ibootstrap] = (TH2D*)frew_ntrack_pt->Get( (string(m_systematic + "/datamc_ratio") + char('1'+ij) + string("_data_")+to_string((bootstrap_bkeeper-1)*2+ibootstrap) ).c_str() );
         array_mc_h_rew_ntrack[ij][ibootstrap] = (TH2D*)frew_ntrack_pt->Get( (string(m_systematic + "/datamc_ratio") + char('1'+ij) + string("_mc_")+to_string((bootstrap_bkeeper-1)*2+ibootstrap) ).c_str() );
       }
     }
  }


  // use double since integer truncates non-integer weights
  // Long64_t nevt1_data = 0, nevt1_mc = 0;
  // Long64_t nevt1_data_now = 0, nevt1_mc_now = 0;
  double nevt1_data = 0, nevt1_mc = 0;
  double nevt1_data_now = 0, nevt1_mc_now = 0;
  int events_lost = 0, events_passed = 0;

  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if(jentry ==0) cout<<" all: "<<nentries<<", ientry by loadTree: "<<ientry<<endl;
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    if (jentry%10000==0) {
      cout << jentry << '\r'; cout.flush();
    }

    // identify the leading/subleading jet for cut on jetpass
    int jetpass_leading = -999;
    int jetpass_subleading = -999;
    if(isleading[0])
    {
      jetpass_leading = jetpass[0]; 
      jetpass_subleading = jetpass[1]; 
    }
    else
    {
      jetpass_leading = jetpass[1];
      jetpass_subleading = jetpass[0];
    }

    for (int j = 0; j<njets; ++j) {

      if(isleading[j]){

        if(runmc) nevt1_mc_now += mc_evtweight;
        else      nevt1_data_now++;
      }

      double weight = mc_evtweight * data_evtweight[j];
      // pu reweighting
      if(runmc && !m_systematic.Contains("PRW_DATASF")) weight *= evtpuw;
      else if (m_systematic.EqualTo("FlavourTagging_PRW_DATASF__1up")) weight *= evtpuw_sys[0];
      else if (m_systematic.EqualTo("FlavourTagging_PRW_DATASF__1down")) weight *= evtpuw_sys[1];
      else if (runmc) std::cout << "WARNING: no pileup reweighting applied" << std::endl;
      // JVT SF
      if(runmc && !m_systematic.Contains("JVT_effSF")) weight *= evtJVTw;
      else if (m_systematic.EqualTo("FlavourTagging_JVT_effSF__1up")) weight *= evtJVTw_sys[0];
      else if (m_systematic.EqualTo("FlavourTagging_JVT_effSF__1down")) weight *= evtJVTw_sys[1];
      else if (runmc) std::cout << "WARNING: no JVT SF applied" << std::endl;
      // conversions syst
      if(runmc && m_systematic.EqualTo("conversions__1up") && jetHasConversion[j]) weight *=1.1;
      else if(runmc && m_systematic.EqualTo("conversions__1down") && jetHasConversion[j]) weight *=0.9;
      // hadronic syst
      if(runmc && m_systematic.EqualTo("hadronic__1up") && jetHasHadMatInt[j]) weight *=1.1; 
      else if(runmc && m_systematic.EqualTo("hadronic__1down") && jetHasHadMatInt[j]) weight *=0.9; 
      // longlived
      if(runmc && m_systematic.EqualTo("longlivedparticles") && (jetHasKShort[j] || jetHasLambda[j])) weight *=1.3;

      // bootstrap weight
      double weight_data[2];
      double weight_mc[2];
      weight_data[0] = weight;
      weight_data[1] = weight;
      weight_mc[0] = weight;
      weight_mc[1] = weight;

      // pT/eta reweighting -> nominal
      // Ntracks in case of good significance (> 2 sigma) -> nominal
      if (runmc) 
      {
        int ilead = isleading[j] ? 0 : 1;
	int ix = h_rew[ilead]->GetXaxis()->FindBin(jetpt[j]);
	int iy = h_rew[ilead]->GetYaxis()->FindBin( abs(jeteta[j]) );
	double reweight_pt_eta = h_rew[ilead]->GetBinContent(ix,iy);
	weight *= reweight_pt_eta;

	int jx = h_rew_ntrack[ilead]->GetXaxis()->FindBin(ntrack_IP3D[j]);
	int jy = h_rew_ntrack[ilead]->GetYaxis()->FindBin(jetpt[j]);
	double reweight_ntrack_pt = h_rew_ntrack[ilead]->GetBinContent(jx,jy);

        if( abs(1-h_rew_ntrack[ilead]->GetBinContent(jx, jy)) > 2*h_rew_ntrack[ilead]->GetBinError(jx, jy) ) weight *= reweight_ntrack_pt;

	if(debug == 16){
	  cout << "reweight_pt_eta = "<< reweight_pt_eta<<", reweight_ntrack_pt= "<< reweight_ntrack_pt<< endl;
	}

	if (reweight_pt_eta * reweight_ntrack_pt == 0) ++events_lost;
	else ++events_passed;
      }

      // pT/eta and Ntrack reweighting for bootstrap

      if ( m_systematic.EqualTo("FlavourTagging_Nominal")  && bootstrap_bkeeper>0 )
      {
        for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
        {
          if (runmc)
          {
            int ilead = isleading[j] ? 0 : 1;
            int ix = h_rew[ilead]->GetXaxis()->FindBin( jetpt[j] );
            int iy = h_rew[ilead]->GetYaxis()->FindBin( abs(jeteta[j]) );

            // datatstat
            weight_data[ibootstrap] *= array_data_h_rew[ilead][ibootstrap]->GetBinContent(ix,iy);
            // mcstat
            weight_mc[ibootstrap]   *= array_mc_h_rew[ilead][ibootstrap]->GetBinContent(ix,iy);

            int jx = h_rew_ntrack[ilead]->GetXaxis()->FindBin(ntrack_IP3D[j]);
            int jy = h_rew_ntrack[ilead]->GetYaxis()->FindBin(jetpt[j]);

            // 3 sigma significance
            if( abs(1-h_rew_ntrack[ilead]->GetBinContent(jx, jy)) > 2*h_rew_ntrack[ilead]->GetBinError(jx, jy) )
            {
              // datatstat
              weight_data[ibootstrap] *= array_data_h_rew_ntrack[ilead][ibootstrap]->GetBinContent(jx,jy);
              // mcstat
              weight_mc[ibootstrap] *= array_mc_h_rew_ntrack[ilead][ibootstrap]->GetBinContent(jx,jy);
            }
          }
        }
      }


      double ptj = jetpt[j];
      int ntracks_IP3DNeg = ntrack_IP3DNeg[j];
      int ntracks_IP3D = ntrack_IP3D[j];
      int ntracks_IP2DNeg = ntrack_IP2DNeg[j];
      int ntracks_IP2D = ntrack_IP2D[j];
      int ntracks_SV1Flip = ntrack_SV1Flip[j];
      int ntracks_SV1 = ntrack_SV1[j];
      int ntracks_JetFitterFlip = ntrack_JetFitterFlip[j];
      int ntracks_JetFitter = ntrack_JetFitter[j];

      double eta_abs = abs(jeteta[j]);

      // ---------- leading jet ---------- //
      // jetpass - use leading prescale
      if(isleading[j] && jetpass_leading==0 && jetpass_subleading<2 )
	{
	  if(runmc) nevt1_mc += weight;
	  else      nevt1_data++;

	  if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
	    {
              h_njets_event_all_rew->Fill(njets_event, weight);
              h_averageInteractionsPerCrossing_all_rew->Fill(evt_averageInteractionsPerCrossing, weight);

              h_pt1_all_rew->Fill(ptj, weight);
              h_eta1_all_rew->Fill(eta_abs, weight);
              h_pt_thin1_all_rew->Fill(ptj, weight);
              h_eta_thin1_all_rew->Fill(eta_abs, weight);
              h_phi1_all_rew->Fill(jetphi[j], weight);

              int k=0;
              for (const auto &name: subtagger::floats){
                h_float_subtagger1_all_rew.at(k)->Fill(float_subtagger[name.first][j], weight);
                k++;
              }

              k=0;
              for (const auto &name: subtagger::ints){
                h_int_subtagger1_all_rew.at(k)->Fill(int_subtagger[name.first][j], weight);
                k++;
              }

              k=0;
              for (const auto &name: subtagger::doubles){
                h_double_subtagger1_all_rew.at(k)->Fill(double_subtagger[name.first][j], weight);
                k++;
              }

              h_ntrk_IP3DNeg1_all_rew->Fill(ntracks_IP3DNeg, weight);
              h_ntrk_IP3D1_all_rew->Fill(ntracks_IP3D, weight);
              h_ntrk_IP2DNeg1_all_rew->Fill(ntracks_IP2DNeg, weight);
              h_ntrk_IP2D1_all_rew->Fill(ntracks_IP2D, weight);
              h_ntrk_SV1Flip1_all_rew->Fill(ntracks_SV1Flip, weight);
              h_ntrk_SV11_all_rew->Fill(ntracks_SV1, weight);
              h_ntrk_JetFitterFlip1_all_rew->Fill(ntracks_JetFitterFlip, weight);
              h_ntrk_JetFitter1_all_rew->Fill(ntracks_JetFitter, weight);

              h_pteta1_all_rew->Fill(ptj, eta_abs, weight);

              h_ntrk_IP3DNeg_pt1_all_rew->Fill(ntracks_IP3DNeg, ptj, weight);
              h_ntrk_IP3D_pt1_all_rew->Fill(ntracks_IP3D, ptj, weight);
              h_ntrk_IP2DNeg_pt1_all_rew->Fill(ntracks_IP2DNeg, ptj, weight);
              h_ntrk_IP2D_pt1_all_rew->Fill(ntracks_IP2D, ptj, weight);
              h_ntrk_SV1Flip_pt1_all_rew->Fill(ntracks_SV1Flip, ptj, weight);
              h_ntrk_SV1_pt1_all_rew->Fill(ntracks_SV1, ptj, weight);
              h_ntrk_JetFitterFlip_pt1_all_rew->Fill(ntracks_JetFitterFlip, ptj, weight);
              h_ntrk_JetFitter_pt1_all_rew->Fill(ntracks_JetFitter, ptj, weight);	      

	    }

	  // bootstrap replicas
            if (m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
             {
                for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
                {
                  //for mc
                  if(runmc)
                  {
                    //datatstat
                    array_data_h_ntrk_pt1[ibootstrap]->Fill(ntracks_IP3DNeg, ptj, weight_data[ibootstrap]);
                    //mcstat
                    array_mc_h_ntrk_pt1[ibootstrap]->Fill(ntracks_IP3DNeg, ptj, weight_mc[ibootstrap]*weight_bootstrap[(bootstrap_bkeeper-1)*2+ibootstrap]);
                  }
                  // for data
                  else 
                  {
                    // mcstat requires nominal data
                    // datastat
                    array_data_h_ntrk_pt1[ibootstrap]->Fill(ntracks_IP3DNeg, ptj, weight_data[ibootstrap]*weight_bootstrap[(bootstrap_bkeeper-1)*2+ibootstrap]);
                  }
                }
	    }

        }
      // ---------- subleading jet ---------- //
      else if(!isleading[j])
	{
          // if subleading in the first bin, consider only events triggered by the leading in the second bin for 100% trigger efficiency
          if(getPtBin(ptj)==1)
          {
            // Get the pT of the leading
            int index_leading = 0;
            if(j==0) index_leading = 1; 
            else     index_leading = 0;
            double pt_leading = jetpt[index_leading];
            //consider only subleading with leading in a higher bin
            if(getPtBin(pt_leading)!=2) continue; // prescales should not be mixed
            if(jetpass_leading!=0 || jetpass_subleading>1) continue;
            // use leading jet prescale
            weight *= data_evtweight[index_leading]/data_evtweight[j];
            weight_data[0] *= data_evtweight[index_leading]/data_evtweight[j];
            weight_data[1] *= data_evtweight[index_leading]/data_evtweight[j];
            weight_mc[0] *= data_evtweight[index_leading]/data_evtweight[j];
            weight_mc[0] *= data_evtweight[index_leading]/data_evtweight[j];
          }
          // otherwise use subleading prescale, again not to mix different precales
          else if(jetpass_subleading!=0 || jetpass_leading>1) continue;
	  
	  // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
	  if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
	    {
              h_pt2_all_rew->Fill(ptj, weight);
              h_eta2_all_rew->Fill(eta_abs, weight);
              h_pt_thin2_all_rew->Fill(ptj, weight);
              h_eta_thin2_all_rew->Fill(eta_abs, weight);
              h_phi2_all_rew->Fill(jetphi[j], weight);
      
              int k=0;
              for (const auto &name: subtagger::floats){
                h_float_subtagger2_all_rew.at(k)->Fill(float_subtagger[name.first][j], weight);
                k++;
              }
      
              k=0;
              for (const auto &name: subtagger::ints){
                h_int_subtagger2_all_rew.at(k)->Fill(int_subtagger[name.first][j], weight);
                k++;
              }
      
              k=0;
              for (const auto &name: subtagger::doubles){
                h_double_subtagger2_all_rew.at(k)->Fill(double_subtagger[name.first][j], weight);
                k++;
              }
      
              h_ntrk_IP3DNeg2_all_rew->Fill(ntracks_IP3DNeg, weight);
              h_ntrk_IP3D2_all_rew->Fill(ntracks_IP3D, weight);
              h_ntrk_IP2DNeg2_all_rew->Fill(ntracks_IP2DNeg, weight);
              h_ntrk_IP2D2_all_rew->Fill(ntracks_IP2D, weight);
              h_ntrk_SV1Flip2_all_rew->Fill(ntracks_SV1Flip, weight);
              h_ntrk_SV12_all_rew->Fill(ntracks_SV1, weight);
              h_ntrk_JetFitterFlip2_all_rew->Fill(ntracks_JetFitterFlip, weight);
              h_ntrk_JetFitter2_all_rew->Fill(ntracks_JetFitter, weight);
      
              h_pteta2_all_rew->Fill(ptj, eta_abs, weight);
      
              h_ntrk_IP3DNeg_pt2_all_rew->Fill(ntracks_IP3DNeg, ptj, weight);
              h_ntrk_IP3D_pt2_all_rew->Fill(ntracks_IP3D, ptj, weight);
              h_ntrk_IP2DNeg_pt2_all_rew->Fill(ntracks_IP2DNeg, ptj, weight);
              h_ntrk_IP2D_pt2_all_rew->Fill(ntracks_IP2D, ptj, weight);
              h_ntrk_SV1Flip_pt2_all_rew->Fill(ntracks_SV1Flip, ptj, weight);
              h_ntrk_SV1_pt2_all_rew->Fill(ntracks_SV1, ptj, weight);
              h_ntrk_JetFitterFlip_pt2_all_rew->Fill(ntracks_JetFitterFlip, ptj, weight);
              h_ntrk_JetFitter_pt2_all_rew->Fill(ntracks_JetFitter, ptj, weight); 	     
	    }

	  // bootstrap replicas
             if (m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
             {
                for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
                {
                  //for mc
                  if(runmc)
                  {
                    //datatstat
                    array_data_h_ntrk_pt2[ibootstrap]->Fill(ntracks_IP3DNeg, ptj, weight_data[ibootstrap]);
                    //mcstat
                    array_mc_h_ntrk_pt2[ibootstrap]->Fill(ntracks_IP3DNeg, ptj, weight_mc[ibootstrap]*weight_bootstrap[(bootstrap_bkeeper-1)*2+ibootstrap]);
                  }
                  // for data
                  else 
                  {
                    // mcstat requires nominal data
                    // datastat
                    array_data_h_ntrk_pt2[ibootstrap]->Fill(ntracks_IP3DNeg, ptj, weight_data[ibootstrap]*weight_bootstrap[(bootstrap_bkeeper-1)*2+ibootstrap]);
                  }
                }
	    }

	}
    }
  }
  cout<<"check nevt: mc "<<nevt1_mc<<" =? data "<<nevt1_data<<"; mc weight before reweighting "<<nevt1_mc_now<<endl;
  if (runmc) cout << "number of MC events lost/all: " << events_lost << "/" << events_lost + events_passed << " (" << (double)events_lost/(events_lost+events_passed) * 100 << "%)" << endl;

}
