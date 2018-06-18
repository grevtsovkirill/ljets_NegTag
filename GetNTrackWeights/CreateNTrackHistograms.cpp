#define CreateNTrackHistograms_cxx
#include "CreateNTrackHistograms.h"
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

using namespace std;
#include "../helpers/OutputHelper.hpp"

extern bool runmc;

void CreateNTrackHistograms::Loop(int bootstrap_bkeeper=0)
{

  const double pi = M_PI, twopi = 2*pi;

  if (fChain == 0) return;

  int currentTree = -1;

  // Control plots

  //----------event-wide----------//
  TH1D* h_njets_event_pteta_rew; 
  TH1D* h_averageInteractionsPerCrossing_pteta_rew;

  //----------leading jet----------//
  // 1D - kinematics
  TH1D* h_pt1_pteta_rew; 
  TH1D* h_eta1_pteta_rew; 
  TH1D* h_pt_thin1_pteta_rew; 
  TH1D* h_eta_thin1_pteta_rew; 
  TH1D* h_phi1_pteta_rew; 
  // 1D - taggers and subtaggers
  std::vector<TH1D*> h_float_subtagger1_pteta_rew;
  std::vector<TH1D*> h_int_subtagger1_pteta_rew;
  std::vector<TH1D*> h_double_subtagger1_pteta_rew;
  // 1D - ntrk
  TH1D* h_ntrk_IP3DNeg1_pteta_rew;
  TH1D* h_ntrk_IP3D1_pteta_rew;
  TH1D* h_ntrk_IP2DNeg1_pteta_rew;
  TH1D* h_ntrk_IP2D1_pteta_rew;
  TH1D* h_ntrk_SV1Flip1_pteta_rew;
  TH1D* h_ntrk_SV11_pteta_rew;
  TH1D* h_ntrk_JetFitterFlip1_pteta_rew;
  TH1D* h_ntrk_JetFitter1_pteta_rew;

  // 2D - kinematics
  TH2D* h_pteta1_pteta_rew; 
  // 2D - ntrk
  TH2D* h_ntrk_IP3DNeg_pt1_pteta_rew;
  TH2D* h_ntrk_IP3D_pt1_pteta_rew;
  TH2D* h_ntrk_IP2DNeg_pt1_pteta_rew;
  TH2D* h_ntrk_IP2D_pt1_pteta_rew;
  TH2D* h_ntrk_SV1Flip_pt1_pteta_rew;
  TH2D* h_ntrk_SV1_pt1_pteta_rew;
  TH2D* h_ntrk_JetFitterFlip_pt1_pteta_rew;
  TH2D* h_ntrk_JetFitter_pt1_pteta_rew;

  //----------subleading jet----------//
  // 1D - kinematics
  TH1D* h_pt2_pteta_rew; 
  TH1D* h_eta2_pteta_rew; 
  TH1D* h_pt_thin2_pteta_rew; 
  TH1D* h_eta_thin2_pteta_rew; 
  TH1D* h_phi2_pteta_rew; 
  // 1D - taggers and subtaggers
  std::vector<TH1D*> h_float_subtagger2_pteta_rew;
  std::vector<TH1D*> h_int_subtagger2_pteta_rew;
  std::vector<TH1D*> h_double_subtagger2_pteta_rew;
  // 1D - ntrk
  TH1D* h_ntrk_IP3DNeg2_pteta_rew;
  TH1D* h_ntrk_IP3D2_pteta_rew;
  TH1D* h_ntrk_IP2DNeg2_pteta_rew;
  TH1D* h_ntrk_IP2D2_pteta_rew;
  TH1D* h_ntrk_SV1Flip2_pteta_rew;
  TH1D* h_ntrk_SV12_pteta_rew;
  TH1D* h_ntrk_JetFitterFlip2_pteta_rew;
  TH1D* h_ntrk_JetFitter2_pteta_rew;

  // 2D - kinematics
  TH2D* h_pteta2_pteta_rew; 
  // 2D - ntrk
  TH2D* h_ntrk_IP3DNeg_pt2_pteta_rew;
  TH2D* h_ntrk_IP3D_pt2_pteta_rew;
  TH2D* h_ntrk_IP2DNeg_pt2_pteta_rew;
  TH2D* h_ntrk_IP2D_pt2_pteta_rew;
  TH2D* h_ntrk_SV1Flip_pt2_pteta_rew;
  TH2D* h_ntrk_SV1_pt2_pteta_rew;
  TH2D* h_ntrk_JetFitterFlip_pt2_pteta_rew;
  TH2D* h_ntrk_JetFitter_pt2_pteta_rew;

  // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
  if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
  {
    //----------event-wide----------//
    h_njets_event_pteta_rew = new TH1D("njets_event_pteta_rew", "", 20, 0, 20); 
    h_averageInteractionsPerCrossing_pteta_rew = new TH1D("averageInteractionsPerCrossing_pteta_rew", "", 100, 0, 100); 

    //----------leading jet----------//
    // 1D - kinematics
    h_pt1_pteta_rew = new TH1D("pt1_pteta_rew", "", conf::n_pt, conf::pt_lowedges); 
    h_eta1_pteta_rew = new TH1D("eta1_pteta_rew", "", conf::n_eta, conf::eta_lowedges);
    h_pt_thin1_pteta_rew = new TH1D("pt_thin1_pteta_rew", "", 600, 0., 3000.); 
    h_eta_thin1_pteta_rew = new TH1D("eta_thin1_pteta_rew", "", 25, 0., 2.5);
    h_phi1_pteta_rew = new TH1D("phi1_pteta_rew", "", 60, -3.16, 3.16);
    // 1D - taggers and subtaggers
    for (const auto &name: subtagger::floats){
      TH1D *h_tmp = new TH1D((name.first + "1_pteta_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_float_subtagger1_pteta_rew.push_back(h_tmp);
    }
    for (const auto &name: subtagger::ints){
      TH1D *h_tmp = new TH1D((name.first + "1_pteta_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_int_subtagger1_pteta_rew.push_back(h_tmp);
    }
    for (const auto &name: subtagger::doubles){
      TH1D *h_tmp = new TH1D((name.first + "1_pteta_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_double_subtagger1_pteta_rew.push_back(h_tmp);
    }
    // 1D - ntrk
    h_ntrk_IP3DNeg1_pteta_rew = new TH1D("ntrk_IP3DNeg1_pteta_rew","",25, 0, 25);
    h_ntrk_IP3D1_pteta_rew = new TH1D("ntrk_IP3D1_pteta_rew","",25, 0, 25);
    h_ntrk_IP2DNeg1_pteta_rew = new TH1D("ntrk_IP2DNeg1_pteta_rew","",25, 0, 25);
    h_ntrk_IP2D1_pteta_rew = new TH1D("ntrk_IP2D1_pteta_rew","",25, 0, 25);
    h_ntrk_SV1Flip1_pteta_rew = new TH1D("ntrk_SV1Flip1_pteta_rew","",25, 0, 25);
    h_ntrk_SV11_pteta_rew = new TH1D("ntrk_SV11_pteta_rew","",25, 0, 25);
    h_ntrk_JetFitterFlip1_pteta_rew = new TH1D("ntrk_JetFitterFlip1_pteta_rew","",25, 0, 25);
    h_ntrk_JetFitter1_pteta_rew = new TH1D("ntrk_JetFitter1_pteta_rew","",25, 0, 25);

    // 2D - kinematics
    h_pteta1_pteta_rew = new TH2D("pteta1_pteta_rew", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
    // 2D - ntrk
    h_ntrk_IP3DNeg_pt1_pteta_rew = new TH2D("ntrk_IP3DNeg_pt1_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP3D_pt1_pteta_rew = new TH2D("ntrk_IP3D_pt1_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2DNeg_pt1_pteta_rew = new TH2D("ntrk_IP2DNeg_pt1_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2D_pt1_pteta_rew = new TH2D("ntrk_IP2D_pt1_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1Flip_pt1_pteta_rew = new TH2D("ntrk_SV1Flip_pt1_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1_pt1_pteta_rew = new TH2D("ntrk_SV1_pt1_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitterFlip_pt1_pteta_rew = new TH2D("ntrk_JetFitterFlip_pt1_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitter_pt1_pteta_rew = new TH2D("ntrk_JetFitter_pt1_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);

    //----------subleading jet----------//
    // 1D - kinematics
    h_pt2_pteta_rew = new TH1D("pt2_pteta_rew", "", conf::n_pt, conf::pt_lowedges); 
    h_eta2_pteta_rew = new TH1D("eta2_pteta_rew", "", conf::n_eta, conf::eta_lowedges);
    h_pt_thin2_pteta_rew = new TH1D("pt_thin2_pteta_rew", "", 600, 0., 3000.); 
    h_eta_thin2_pteta_rew = new TH1D("eta_thin2_pteta_rew", "", 25, 0., 2.5);
    h_phi2_pteta_rew = new TH1D("phi2_pteta_rew", "", 60, -3.16, 3.16);
    // 1D - taggers and subtaggers
    for (const auto &name: subtagger::floats){
      TH1D *h_tmp = new TH1D((name.first + "2_pteta_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_float_subtagger2_pteta_rew.push_back(h_tmp);
    }
    for (const auto &name: subtagger::ints){
      TH1D *h_tmp = new TH1D((name.first + "2_pteta_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_int_subtagger2_pteta_rew.push_back(h_tmp);
    }
    for (const auto &name: subtagger::doubles){
      TH1D *h_tmp = new TH1D((name.first + "2_pteta_rew").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_double_subtagger2_pteta_rew.push_back(h_tmp);
    }
    // 1D - ntrk
    h_ntrk_IP3DNeg2_pteta_rew = new TH1D("ntrk_IP3DNeg2_pteta_rew","",25, 0, 25);
    h_ntrk_IP3D2_pteta_rew = new TH1D("ntrk_IP3D2_pteta_rew","",25, 0, 25);
    h_ntrk_IP2DNeg2_pteta_rew = new TH1D("ntrk_IP2DNeg2_pteta_rew","",25, 0, 25);
    h_ntrk_IP2D2_pteta_rew = new TH1D("ntrk_IP2D2_pteta_rew","",25, 0, 25);
    h_ntrk_SV1Flip2_pteta_rew = new TH1D("ntrk_SV1Flip2_pteta_rew","",25, 0, 25);
    h_ntrk_SV12_pteta_rew = new TH1D("ntrk_SV12_pteta_rew","",25, 0, 25);
    h_ntrk_JetFitterFlip2_pteta_rew = new TH1D("ntrk_JetFitterFlip2_pteta_rew","",25, 0, 25);
    h_ntrk_JetFitter2_pteta_rew = new TH1D("ntrk_JetFitter2_pteta_rew","",25, 0, 25);

    // 2D - kinematics
    h_pteta2_pteta_rew = new TH2D("pteta2_pteta_rew", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
    // 2D - ntrk
    h_ntrk_IP3DNeg_pt2_pteta_rew = new TH2D("ntrk_IP3DNeg_pt2_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP3D_pt2_pteta_rew = new TH2D("ntrk_IP3D_pt2_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2DNeg_pt2_pteta_rew = new TH2D("ntrk_IP2DNeg_pt2_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2D_pt2_pteta_rew = new TH2D("ntrk_IP2D_pt2_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1Flip_pt2_pteta_rew = new TH2D("ntrk_SV1Flip_pt2_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1_pt2_pteta_rew = new TH2D("ntrk_SV1_pt2_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitterFlip_pt2_pteta_rew = new TH2D("ntrk_JetFitterFlip_pt2_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitter_pt2_pteta_rew = new TH2D("ntrk_JetFitter_pt2_pteta_rew","",25, 0, 25, conf::n_pt, conf::pt_lowedges);

    // Correct uncertainties for weighted histograms
    h_njets_event_pteta_rew->Sumw2();
    h_averageInteractionsPerCrossing_pteta_rew->Sumw2();

    h_pt1_pteta_rew->Sumw2();
    h_eta1_pteta_rew->Sumw2();
    h_pt_thin1_pteta_rew->Sumw2();
    h_eta_thin1_pteta_rew->Sumw2();
    h_phi1_pteta_rew->Sumw2();

    h_ntrk_IP3DNeg1_pteta_rew->Sumw2();
    h_ntrk_IP3D1_pteta_rew->Sumw2();
    h_ntrk_IP2DNeg1_pteta_rew->Sumw2();
    h_ntrk_IP2D1_pteta_rew->Sumw2();
    h_ntrk_SV1Flip1_pteta_rew->Sumw2();
    h_ntrk_SV11_pteta_rew->Sumw2();
    h_ntrk_JetFitterFlip1_pteta_rew->Sumw2();
    h_ntrk_JetFitter1_pteta_rew->Sumw2();

    h_pteta1_pteta_rew->Sumw2();

    h_ntrk_IP3DNeg_pt1_pteta_rew->Sumw2();
    h_ntrk_IP3D_pt1_pteta_rew->Sumw2();
    h_ntrk_IP2DNeg_pt1_pteta_rew->Sumw2();
    h_ntrk_IP2D_pt1_pteta_rew->Sumw2();
    h_ntrk_SV1Flip_pt1_pteta_rew->Sumw2();
    h_ntrk_SV1_pt1_pteta_rew->Sumw2();
    h_ntrk_JetFitterFlip_pt1_pteta_rew->Sumw2();
    h_ntrk_JetFitter_pt1_pteta_rew->Sumw2();

    h_pt2_pteta_rew->Sumw2();
    h_eta2_pteta_rew->Sumw2();
    h_pt_thin2_pteta_rew->Sumw2();
    h_eta_thin2_pteta_rew->Sumw2();
    h_phi2_pteta_rew->Sumw2();
 
    h_ntrk_IP3DNeg2_pteta_rew->Sumw2();
    h_ntrk_IP3D2_pteta_rew->Sumw2();
    h_ntrk_IP2DNeg2_pteta_rew->Sumw2();
    h_ntrk_IP2D2_pteta_rew->Sumw2();
    h_ntrk_SV1Flip2_pteta_rew->Sumw2();
    h_ntrk_SV12_pteta_rew->Sumw2();
    h_ntrk_JetFitterFlip2_pteta_rew->Sumw2();
    h_ntrk_JetFitter2_pteta_rew->Sumw2();
   
    h_pteta2_pteta_rew->Sumw2();

    h_ntrk_IP3DNeg_pt2_pteta_rew->Sumw2();
    h_ntrk_IP3D_pt2_pteta_rew->Sumw2();
    h_ntrk_IP2DNeg_pt2_pteta_rew->Sumw2();
    h_ntrk_IP2D_pt2_pteta_rew->Sumw2();
    h_ntrk_SV1Flip_pt2_pteta_rew->Sumw2();
    h_ntrk_SV1_pt2_pteta_rew->Sumw2();
    h_ntrk_JetFitterFlip_pt2_pteta_rew->Sumw2();
    h_ntrk_JetFitter_pt2_pteta_rew->Sumw2();

    
  }


  // ************************************************ //
  //                Bootstrap Replicas                //
  // ************************************************ //
 
  TH2D* array_data_h_ntrk_pt1[2];
  TH2D* array_data_h_ntrk_pt2[2]; 
  TH2D* array_mc_h_ntrk_pt1[2];
  TH2D* array_mc_h_ntrk_pt2[2]; 

  if ( m_systematic.EqualTo("FlavourTagging_Nominal")  && bootstrap_bkeeper>0)
  {
     for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
     {
       array_data_h_ntrk_pt1[ibootstrap] = new TH2D( (string("ntrk_pt1_pteta_rew_data_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", 25, 0, 25, conf::n_pt, conf::pt_lowedges);

       array_data_h_ntrk_pt2[ibootstrap] = new TH2D( (string("ntrk_pt2_pteta_rew_data_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", 25, 0, 25, conf::n_pt, conf::pt_lowedges);

       array_data_h_ntrk_pt1[ibootstrap]->Sumw2();
       array_data_h_ntrk_pt2[ibootstrap]->Sumw2();

       if(runmc)
       {
         array_mc_h_ntrk_pt1[ibootstrap] = new TH2D( (string("ntrk_pt1_pteta_rew_mc_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", 25, 0, 25, conf::n_pt, conf::pt_lowedges);
         array_mc_h_ntrk_pt2[ibootstrap] = new TH2D( (string("ntrk_pt2_pteta_rew_mc_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", 25, 0, 25, conf::n_pt, conf::pt_lowedges);
         array_mc_h_ntrk_pt1[ibootstrap]->Sumw2();
         array_mc_h_ntrk_pt2[ibootstrap]->Sumw2();
       }

     }

  }

  // ************************************************ //
  //           End of bootstrap definition            //
  // ************************************************ //

  // pT/eta reweighting
  TFile* frew;
  std::cout<< "Compaigne = "<< m_compagine << std::endl;
  std::string frew_name = "../GetPtEtaWeights/rew_"+m_compagine+".root";
  if(m_HERWIG) frew_name = "../GetPtEtaWeights/rew_"+m_compagine+"_HERWIG.root";
  if(m_SHERPA) frew_name = "../GetPtEtaWeights/rew_"+m_compagine+"_SHERPA.root";

  frew = new TFile(frew_name.c_str(),"read");
  TH2D* h_rew[2];
  for(int ij=0; ij<2; ij++){
    h_rew[ij] = (TH2D*)frew->Get( (string(m_systematic + "/ptscale")+char('1'+ij)).c_str() );
  }

  // Reweighting for bootstrap
  TH2D* array_data_h_rew[2][2];
  TH2D* array_mc_h_rew[2][2];

  if ( m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
  {
     for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
     {
       for(int ij=0; ij<2; ij++){
         array_data_h_rew[ij][ibootstrap] = (TH2D*)frew->Get( (string(m_systematic + "/ptscale") + char('1'+ij) + string("_data_")+to_string((bootstrap_bkeeper-1)*2+ibootstrap) ).c_str() );
         array_mc_h_rew[ij][ibootstrap] = (TH2D*)frew->Get( (string(m_systematic + "/ptscale") + char('1'+ij) + string("_mc_")+to_string((bootstrap_bkeeper-1)*2+ibootstrap) ).c_str() );
       }
     }
  }

  // use double since integer truncates non-integer weights
  // Long64_t nevt1_data = 0, nevt1_mc = 0;
  // Long64_t nevt1_data_now = 0, nevt1_mc_now = 0;
  double nevt1_data = 0, nevt1_mc = 0;
  double nevt1_data_now = 0, nevt1_mc_now = 0;
  int events_lost = 0, events_passed = 0;

  // Start to loop on the event of a chain/tree
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
  {
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

      double weight = mc_evtweight*data_evtweight[j];
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

      // bootstrap weights
      double weight_data[2];
      double weight_mc[2];

      weight_data[0] = weight;
      weight_data[1] = weight;
      weight_mc[0] = weight;
      weight_mc[1] = weight;

      // pT/eta reweighting -> nominal 
      if (runmc) 
      {
        int ilead = isleading[j] ? 0 : 1;
      	int ix = h_rew[ilead]->GetXaxis()->FindBin( jetpt[j] );
      	int iy = h_rew[ilead]->GetYaxis()->FindBin( abs(jeteta[j]) );
      	double reweight = h_rew[ilead]->GetBinContent(ix,iy);
      	weight *= reweight;
      	if (reweight == 0) ++events_lost;
      	else ++events_passed;
      }

      // pT/eta reweighting -> bootstrap 
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

          // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst	  
	  if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
	    {
              h_njets_event_pteta_rew->Fill(njets_event, weight);
              h_averageInteractionsPerCrossing_pteta_rew->Fill(evt_averageInteractionsPerCrossing, weight);

              h_pt1_pteta_rew->Fill(ptj, weight);
              h_eta1_pteta_rew->Fill(eta_abs, weight);
              h_pt_thin1_pteta_rew->Fill(ptj, weight);
              h_eta_thin1_pteta_rew->Fill(eta_abs, weight);
              h_phi1_pteta_rew->Fill(jetphi[j], weight);

              int k=0;
              for (const auto &name: subtagger::floats){
                h_float_subtagger1_pteta_rew.at(k)->Fill(float_subtagger[name.first][j], weight);
                k++;
              }

              k=0;
              for (const auto &name: subtagger::ints){
                h_int_subtagger1_pteta_rew.at(k)->Fill(int_subtagger[name.first][j], weight);
                k++;
              }

              k=0;
              for (const auto &name: subtagger::doubles){
                h_double_subtagger1_pteta_rew.at(k)->Fill(double_subtagger[name.first][j], weight);
                k++;
              }

              h_ntrk_IP3DNeg1_pteta_rew->Fill(ntracks_IP3DNeg, weight);
              h_ntrk_IP3D1_pteta_rew->Fill(ntracks_IP3D, weight);
              h_ntrk_IP2DNeg1_pteta_rew->Fill(ntracks_IP2DNeg, weight);
              h_ntrk_IP2D1_pteta_rew->Fill(ntracks_IP2D, weight);
              h_ntrk_SV1Flip1_pteta_rew->Fill(ntracks_SV1Flip, weight);
              h_ntrk_SV11_pteta_rew->Fill(ntracks_SV1, weight);
              h_ntrk_JetFitterFlip1_pteta_rew->Fill(ntracks_JetFitterFlip, weight);
              h_ntrk_JetFitter1_pteta_rew->Fill(ntracks_JetFitter, weight);

              h_pteta1_pteta_rew->Fill(ptj, eta_abs, weight);

              h_ntrk_IP3DNeg_pt1_pteta_rew->Fill(ntracks_IP3DNeg, ptj, weight);
              h_ntrk_IP3D_pt1_pteta_rew->Fill(ntracks_IP3D, ptj, weight);
              h_ntrk_IP2DNeg_pt1_pteta_rew->Fill(ntracks_IP2DNeg, ptj, weight);
              h_ntrk_IP2D_pt1_pteta_rew->Fill(ntracks_IP2D, ptj, weight);
              h_ntrk_SV1Flip_pt1_pteta_rew->Fill(ntracks_SV1Flip, ptj, weight);
              h_ntrk_SV1_pt1_pteta_rew->Fill(ntracks_SV1, ptj, weight);
              h_ntrk_JetFitterFlip_pt1_pteta_rew->Fill(ntracks_JetFitterFlip, ptj, weight);
              h_ntrk_JetFitter_pt1_pteta_rew->Fill(ntracks_JetFitter, ptj, weight);	      
	    }
	  

	  // bootstrap replicas
          // REWEIGHTING BASED ON ntracks_IP3DNeg FOR NOW 
	  if (m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
          {
	    for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
            {
              // for mc
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
                // mcstat required nominal data
                // datatstat
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
          weight_mc[1] *= data_evtweight[index_leading]/data_evtweight[j];
        }
        // otherwise use subleading prescale, again not to mix different precales
        else if(jetpass_subleading!=0 || jetpass_leading>1) continue;

        // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
	if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
	{
          h_pt2_pteta_rew->Fill(ptj, weight);
          h_eta2_pteta_rew->Fill(eta_abs, weight);
          h_pt_thin2_pteta_rew->Fill(ptj, weight);
          h_eta_thin2_pteta_rew->Fill(eta_abs, weight);
          h_phi2_pteta_rew->Fill(jetphi[j], weight);
  
          int k=0;
          for (const auto &name: subtagger::floats){
            h_float_subtagger2_pteta_rew.at(k)->Fill(float_subtagger[name.first][j], weight);
            k++;
          }
  
          k=0;
          for (const auto &name: subtagger::ints){
            h_int_subtagger2_pteta_rew.at(k)->Fill(int_subtagger[name.first][j], weight);
            k++;
          }
  
          k=0;
          for (const auto &name: subtagger::doubles){
            h_double_subtagger2_pteta_rew.at(k)->Fill(double_subtagger[name.first][j], weight);
            k++;
          }
  
          h_ntrk_IP3DNeg2_pteta_rew->Fill(ntracks_IP3DNeg, weight);
          h_ntrk_IP3D2_pteta_rew->Fill(ntracks_IP3D, weight);
          h_ntrk_IP2DNeg2_pteta_rew->Fill(ntracks_IP2DNeg, weight);
          h_ntrk_IP2D2_pteta_rew->Fill(ntracks_IP2D, weight);
          h_ntrk_SV1Flip2_pteta_rew->Fill(ntracks_SV1Flip, weight);
          h_ntrk_SV12_pteta_rew->Fill(ntracks_SV1, weight);
          h_ntrk_JetFitterFlip2_pteta_rew->Fill(ntracks_JetFitterFlip, weight);
          h_ntrk_JetFitter2_pteta_rew->Fill(ntracks_JetFitter, weight);
  
          h_pteta2_pteta_rew->Fill(ptj, eta_abs, weight);
  
          h_ntrk_IP3DNeg_pt2_pteta_rew->Fill(ntracks_IP3DNeg, ptj, weight);
          h_ntrk_IP3D_pt2_pteta_rew->Fill(ntracks_IP3D, ptj, weight);
          h_ntrk_IP2DNeg_pt2_pteta_rew->Fill(ntracks_IP2DNeg, ptj, weight);
          h_ntrk_IP2D_pt2_pteta_rew->Fill(ntracks_IP2D, ptj, weight);
          h_ntrk_SV1Flip_pt2_pteta_rew->Fill(ntracks_SV1Flip, ptj, weight);
          h_ntrk_SV1_pt2_pteta_rew->Fill(ntracks_SV1, ptj, weight);
          h_ntrk_JetFitterFlip_pt2_pteta_rew->Fill(ntracks_JetFitterFlip, ptj, weight);
          h_ntrk_JetFitter_pt2_pteta_rew->Fill(ntracks_JetFitter, ptj, weight); 	     
	}

	// bootstrap replicas
	if (m_systematic.EqualTo("FlavourTagging_Nominal")  && bootstrap_bkeeper>0)
	{
	  for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
          {
            // for mc
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
              // mcstat uses nominal data
              // datatstat
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
