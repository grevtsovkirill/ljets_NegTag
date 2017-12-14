#define CreateHistograms_cxx
#include "CreateHistograms.h"
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
#include <fstream>
#include <sstream>
#include <stdio.h>

#include "../config/conf.hpp"
#include "../helpers/OutputHelper.hpp"

using namespace std;

extern bool runmc;

void CreateHistograms::Loop(int bootstrap_bkeeper=0)
{
  const double pi = M_PI, twopi = 2*pi;

  if (fChain == 0) return;

  int currentTree = -1;

  // Control plots

  //----------event-wide----------//
  TH1D* h_njets_event_mc_w;
  TH1D* h_ntrackjets_event_mc_w;
  TH1D* h_averageInteractionsPerCrossing_mc_w;

  //----------leading jet----------//
  // 1D - kinematics
  TH1D* h_pt1_mc_w; 
  TH1D* h_eta1_mc_w; 
  TH1D* h_pt_thin1_mc_w; 
  TH1D* h_eta_thin1_mc_w; 
  TH1D* h_phi1_mc_w;
  TH1D* h_trackpt1_mc_w;
  TH1D* h_tracketa1_mc_w;
  TH1D* h_trackpt_thin1_mc_w;
  TH1D* h_tracketa_thin1_mc_w;
  TH1D* h_trackphi1_mc_w; 
  // 1D - taggers and subtaggers
  std::vector<TH1D*> h_float_subtagger1_mc_w;
  std::vector<TH1D*> h_int_subtagger1_mc_w;
  std::vector<TH1D*> h_double_subtagger1_mc_w;
  std::vector<TH1D*> h_float_subtagger1_track_mc_w;
  std::vector<TH1D*> h_int_subtagger1_track_mc_w;
  std::vector<TH1D*> h_double_subtagger1_track_mc_w;

  // 1D - ntrk
  TH1D* h_ntrk_IP3DNeg1_mc_w;
  TH1D* h_ntrk_IP3D1_mc_w;
  TH1D* h_ntrk_IP2DNeg1_mc_w;
  TH1D* h_ntrk_IP2D1_mc_w;
  TH1D* h_ntrk_SV1Flip1_mc_w;
  TH1D* h_ntrk_SV11_mc_w;
  TH1D* h_ntrk_JetFitterFlip1_mc_w;
  TH1D* h_ntrk_JetFitter1_mc_w;
  TH1D* h_trackntrk_IP3DNeg1_mc_w;
  TH1D* h_trackntrk_IP3D1_mc_w;
  TH1D* h_trackntrk_IP2DNeg1_mc_w;
  TH1D* h_trackntrk_IP2D1_mc_w;
  TH1D* h_trackntrk_SV1Flip1_mc_w;
  TH1D* h_trackntrk_SV11_mc_w;
  TH1D* h_trackntrk_JetFitterFlip1_mc_w;
  TH1D* h_trackntrk_JetFitter1_mc_w;
  // 2D - kinematics
  TH2D* h_pteta1_mc_w; 
  TH2D* h_trackpteta1_mc_w;
  // 2D - ntrk
  TH2D* h_ntrk_IP3DNeg_pt1_mc_w;
  TH2D* h_ntrk_IP3D_pt1_mc_w;
  TH2D* h_ntrk_IP2DNeg_pt1_mc_w;
  TH2D* h_ntrk_IP2D_pt1_mc_w;
  TH2D* h_ntrk_SV1Flip_pt1_mc_w;
  TH2D* h_ntrk_SV1_pt1_mc_w;
  TH2D* h_ntrk_JetFitterFlip_pt1_mc_w;
  TH2D* h_ntrk_JetFitter_pt1_mc_w;
  TH2D* h_trackntrk_IP3DNeg_pt1_mc_w;
  TH2D* h_trackntrk_IP3D_pt1_mc_w;
  TH2D* h_trackntrk_IP2DNeg_pt1_mc_w;
  TH2D* h_trackntrk_IP2D_pt1_mc_w;
  TH2D* h_trackntrk_SV1Flip_pt1_mc_w;
  TH2D* h_trackntrk_SV1_pt1_mc_w;
  TH2D* h_trackntrk_JetFitterFlip_pt1_mc_w;
  TH2D* h_trackntrk_JetFitter_pt1_mc_w;
  //----------subleading jet----------//
  // 1D - kinematics
  TH1D* h_pt2_mc_w; 
  TH1D* h_eta2_mc_w; 
  TH1D* h_pt_thin2_mc_w; 
  TH1D* h_eta_thin2_mc_w; 
  TH1D* h_phi2_mc_w;
  TH1D* h_trackpt2_mc_w;
  TH1D* h_tracketa2_mc_w;
  TH1D* h_trackpt_thin2_mc_w;
  TH1D* h_tracketa_thin2_mc_w;
  TH1D* h_trackphi2_mc_w; 
  // 1D - taggers and subtaggers
  std::vector<TH1D*> h_float_subtagger2_mc_w;
  std::vector<TH1D*> h_int_subtagger2_mc_w;
  std::vector<TH1D*> h_double_subtagger2_mc_w;
  std::vector<TH1D*> h_float_subtagger2_track_mc_w;
  std::vector<TH1D*> h_int_subtagger2_track_mc_w;
  std::vector<TH1D*> h_double_subtagger2_track_mc_w;

  // 1D - ntrk
  TH1D* h_ntrk_IP3DNeg2_mc_w;
  TH1D* h_ntrk_IP3D2_mc_w;
  TH1D* h_ntrk_IP2DNeg2_mc_w;
  TH1D* h_ntrk_IP2D2_mc_w;
  TH1D* h_ntrk_SV1Flip2_mc_w;
  TH1D* h_ntrk_SV12_mc_w;
  TH1D* h_ntrk_JetFitterFlip2_mc_w;
  TH1D* h_ntrk_JetFitter2_mc_w;
  TH1D* h_trackntrk_IP3DNeg2_mc_w;
  TH1D* h_trackntrk_IP3D2_mc_w;
  TH1D* h_trackntrk_IP2DNeg2_mc_w;
  TH1D* h_trackntrk_IP2D2_mc_w;
  TH1D* h_trackntrk_SV1Flip2_mc_w;
  TH1D* h_trackntrk_SV12_mc_w;
  TH1D* h_trackntrk_JetFitterFlip2_mc_w;
  TH1D* h_trackntrk_JetFitter2_mc_w;

  // 2D - kinematics
  TH2D* h_pteta2_mc_w; 
  TH2D* h_trackpteta2_mc_w;
  // 2D - ntrk
  TH2D* h_ntrk_IP3DNeg_pt2_mc_w;
  TH2D* h_ntrk_IP3D_pt2_mc_w;
  TH2D* h_ntrk_IP2DNeg_pt2_mc_w;
  TH2D* h_ntrk_IP2D_pt2_mc_w;
  TH2D* h_ntrk_SV1Flip_pt2_mc_w;
  TH2D* h_ntrk_SV1_pt2_mc_w;
  TH2D* h_ntrk_JetFitterFlip_pt2_mc_w;
  TH2D* h_ntrk_JetFitter_pt2_mc_w;
  TH2D* h_trackntrk_IP3DNeg_pt2_mc_w;
  TH2D* h_trackntrk_IP3D_pt2_mc_w;
  TH2D* h_trackntrk_IP2DNeg_pt2_mc_w;
  TH2D* h_trackntrk_IP2D_pt2_mc_w;
  TH2D* h_trackntrk_SV1Flip_pt2_mc_w;
  TH2D* h_trackntrk_SV1_pt2_mc_w;
  TH2D* h_trackntrk_JetFitterFlip_pt2_mc_w;
  TH2D* h_trackntrk_JetFitter_pt2_mc_w;


  // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
  if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
  {
    //----------event-wide----------//
    h_njets_event_mc_w = new TH1D("njets_event_mc_w", "", 20, 0, 20); 
    h_ntrackjets_event_mc_w = new TH1D("ntrackjets_event_mc_w", "", 20, 0, 20);
    h_averageInteractionsPerCrossing_mc_w = new TH1D("averageInteractionsPerCrossing_mc_w", "", 100, 0, 100); 

    //----------leading jet----------//
    // 1D - kinematics
    h_pt1_mc_w = new TH1D("pt1_mc_w", "", conf::n_pt, conf::pt_lowedges); 
    h_eta1_mc_w = new TH1D("eta1_mc_w", "", conf::n_eta, conf::eta_lowedges);
    h_pt_thin1_mc_w = new TH1D("pt_thin1_mc_w", "", 600, 0., 3000.); 
    h_eta_thin1_mc_w = new TH1D("eta_thin1_mc_w", "", 25, 0., 2.5);
    h_phi1_mc_w = new TH1D("phi1_mc_w", "", 60, -3.16, 3.16);
    h_trackpt1_mc_w = new TH1D("trackpt1_mc_w", "", conf::n_pt, conf::pt_lowedges);
    h_tracketa1_mc_w = new TH1D("tracketa1_mc_w", "", conf::n_eta, conf::eta_lowedges);
    h_trackpt_thin1_mc_w = new TH1D("trackpt_thin1_mc_w", "", 600, 0., 3000.);
    h_tracketa_thin1_mc_w = new TH1D("tracketa_thin1_mc_w", "", 25, 0., 2.5);

    // 1D - taggers and subtaggers
    for (const auto &name: subtagger::floats){
      TH1D *h_tmp = new TH1D((name.first + "1_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_float_subtagger1_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::ints){
      TH1D *h_tmp = new TH1D((name.first + "1_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_int_subtagger1_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::doubles){
      TH1D *h_tmp = new TH1D((name.first + "1_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_double_subtagger1_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::floats_trackjet){
      TH1D *h_tmp = new TH1D((name.first + "1_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_float_subtagger1_track_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::ints_trackjet){
      TH1D *h_tmp = new TH1D((name.first + "1_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_int_subtagger1_track_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::doubles_trackjet){
      TH1D *h_tmp = new TH1D((name.first + "1_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_double_subtagger1_track_mc_w.push_back(h_tmp);
    }

    // 1D - ntrk
    h_ntrk_IP3DNeg1_mc_w = new TH1D("ntrk_IP3DNeg1_mc_w","",25, 0, 25);
    h_ntrk_IP3D1_mc_w = new TH1D("ntrk_IP3D1_mc_w","",25, 0, 25);
    h_ntrk_IP2DNeg1_mc_w = new TH1D("ntrk_IP2DNeg1_mc_w","",25, 0, 25);
    h_ntrk_IP2D1_mc_w = new TH1D("ntrk_IP2D1_mc_w","",25, 0, 25);
    h_ntrk_SV1Flip1_mc_w = new TH1D("ntrk_SV1Flip1_mc_w","",25, 0, 25);
    h_ntrk_SV11_mc_w = new TH1D("ntrk_SV11_mc_w","",25, 0, 25);
    h_ntrk_JetFitterFlip1_mc_w = new TH1D("ntrk_JetFitterFlip1_mc_w","",25, 0, 25);
    h_ntrk_JetFitter1_mc_w = new TH1D("ntrk_JetFitter1_mc_w","",25, 0, 25);
    h_trackntrk_IP3DNeg1_mc_w = new TH1D("trackntrk_IP3DNeg1_mc_w","",25, 0, 25);
    h_trackntrk_IP3D1_mc_w = new TH1D("trackntrk_IP3D1_mc_w","",25, 0, 25);
    h_trackntrk_IP2DNeg1_mc_w = new TH1D("trackntrk_IP2DNeg1_mc_w","",25, 0, 25);
    h_trackntrk_IP2D1_mc_w = new TH1D("trackntrk_IP2D1_mc_w","",25, 0, 25);
    h_trackntrk_SV1Flip1_mc_w = new TH1D("trackntrk_SV1Flip1_mc_w","",25, 0, 25);
    h_trackntrk_SV11_mc_w = new TH1D("tracckntrk_SV11_mc_w","",25, 0, 25);
    h_trackntrk_JetFitterFlip1_mc_w = new TH1D("trackntrk_JetFitterFlip1_mc_w","",25, 0, 25);
    h_trackntrk_JetFitter1_mc_w = new TH1D("trackntrk_JetFitter1_mc_w","",25, 0, 25);

    // 2D - kinematics
    h_pteta1_mc_w = new TH2D("pteta1_mc_w", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
    h_trackpteta1_mc_w = new TH2D("trackpteta1_mc_w", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);

    // 2D - ntrk
    h_ntrk_IP3DNeg_pt1_mc_w = new TH2D("ntrk_IP3DNeg_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP3D_pt1_mc_w = new TH2D("ntrk_IP3D_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2DNeg_pt1_mc_w = new TH2D("ntrk_IP2DNeg_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2D_pt1_mc_w = new TH2D("ntrk_IP2D_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1Flip_pt1_mc_w = new TH2D("ntrk_SV1Flip_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1_pt1_mc_w = new TH2D("ntrk_SV1_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitterFlip_pt1_mc_w = new TH2D("ntrk_JetFitterFlip_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitter_pt1_mc_w = new TH2D("ntrk_JetFitter_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_IP3DNeg_pt1_mc_w = new TH2D("trackntrk_IP3DNeg_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_IP3D_pt1_mc_w = new TH2D("trackntrk_IP3D_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_IP2DNeg_pt1_mc_w = new TH2D("trackntrk_IP2DNeg_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_IP2D_pt1_mc_w = new TH2D("trackntrk_IP2D_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_SV1Flip_pt1_mc_w = new TH2D("trackntrk_SV1Flip_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_SV1_pt1_mc_w = new TH2D("trackntrk_SV1_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_JetFitterFlip_pt1_mc_w = new TH2D("trackntrk_JetFitterFlip_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_JetFitter_pt1_mc_w = new TH2D("trackntrk_JetFitter_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);

    //----------subleading jet----------//
    // 1D - kinematics
    h_pt2_mc_w = new TH1D("pt2_mc_w", "", conf::n_pt, conf::pt_lowedges); 
    h_eta2_mc_w = new TH1D("eta2_mc_w", "", conf::n_eta, conf::eta_lowedges);
    h_pt_thin2_mc_w = new TH1D("pt_thin2_mc_w", "", 600, 0., 3000.); 
    h_eta_thin2_mc_w = new TH1D("eta_thin2_mc_w", "", 25, 0., 2.5);
    h_phi2_mc_w = new TH1D("phi2_mc_w", "", 60, -3.16, 3.16);
    h_trackpt2_mc_w = new TH1D("trackpt2_mc_w", "", conf::n_pt, conf::pt_lowedges);
    h_tracketa2_mc_w = new TH1D("tracketa2_mc_w", "", conf::n_eta, conf::eta_lowedges);
    h_trackpt_thin2_mc_w = new TH1D("trackpt_thin2_mc_w", "", 600, 0., 3000.);
    h_tracketa_thin2_mc_w = new TH1D("tracketa_thin2_mc_w", "", 25, 0., 2.5);
    h_trackphi2_mc_w = new TH1D("trackphi2_mc_w", "", 60, -3.16, 3.16);
    // 1D - taggers and subtaggers
    for (const auto &name: subtagger::floats){
      TH1D *h_tmp = new TH1D((name.first + "2_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_float_subtagger2_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::ints){
      TH1D *h_tmp = new TH1D((name.first + "2_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_int_subtagger2_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::doubles){
      TH1D *h_tmp = new TH1D((name.first + "2_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_double_subtagger2_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::floats_trackjet){
      TH1D *h_tmp = new TH1D((name.first + "2_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_float_subtagger2_track_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::ints_trackjet){
      TH1D *h_tmp = new TH1D((name.first + "2_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_int_subtagger2_track_mc_w.push_back(h_tmp);
    }
    for (const auto &name: subtagger::doubles_trackjet){
      TH1D *h_tmp = new TH1D((name.first + "2_mc_w").c_str(),"",std::get<0>(name.second), std::get<1>(name.second), std::get<2>(name.second));
      h_tmp->Sumw2();
      h_double_subtagger2_track_mc_w.push_back(h_tmp);
    }

    // 1D - ntrk
    h_ntrk_IP3DNeg2_mc_w = new TH1D("ntrk_IP3DNeg2_mc_w","",25, 0, 25);
    h_ntrk_IP3D2_mc_w = new TH1D("ntrk_IP3D2_mc_w","",25, 0, 25);
    h_ntrk_IP2DNeg2_mc_w = new TH1D("ntrk_IP2DNeg2_mc_w","",25, 0, 25);
    h_ntrk_IP2D2_mc_w = new TH1D("ntrk_IP2D2_mc_w","",25, 0, 25);
    h_ntrk_SV1Flip2_mc_w = new TH1D("ntrk_SV1Flip2_mc_w","",25, 0, 25);
    h_ntrk_SV12_mc_w = new TH1D("ntrk_SV12_mc_w","",25, 0, 25);
    h_ntrk_JetFitterFlip2_mc_w = new TH1D("ntrk_JetFitterFlip2_mc_w","",25, 0, 25);
    h_ntrk_JetFitter2_mc_w = new TH1D("ntrk_JetFitter2_mc_w","",25, 0, 25);
    h_trackntrk_IP3DNeg2_mc_w = new TH1D("trackntrk_IP3DNeg2_mc_w","",25, 0, 25);
    h_trackntrk_IP3D2_mc_w = new TH1D("trackntrk_IP3D2_mc_w","",25, 0, 25);
    h_trackntrk_IP2DNeg2_mc_w = new TH1D("trackntrk_IP2DNeg2_mc_w","",25, 0, 25);
    h_trackntrk_IP2D2_mc_w = new TH1D("trackntrk_IP2D2_mc_w","",25, 0, 25);
    h_trackntrk_SV1Flip2_mc_w = new TH1D("trackntrk_SV1Flip2_mc_w","",25, 0, 25);
    h_trackntrk_SV12_mc_w = new TH1D("trackntrk_SV12_mc_w","",25, 0, 25);
    h_trackntrk_JetFitterFlip2_mc_w = new TH1D("trackntrk_JetFitterFlip2_mc_w","",25, 0, 25);
    h_trackntrk_JetFitter2_mc_w = new TH1D("trackntrk_JetFitter2_mc_w","",25, 0, 25);
    // 2D - kinematics
    h_pteta2_mc_w = new TH2D("pteta2_mc_w", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
    h_trackpteta2_mc_w = new TH2D("trackpteta2_mc_w", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);

    // 2D - ntrk
    h_ntrk_IP3DNeg_pt2_mc_w = new TH2D("ntrk_IP3DNeg_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP3D_pt2_mc_w = new TH2D("ntrk_IP3D_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2DNeg_pt2_mc_w = new TH2D("ntrk_IP2DNeg_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2D_pt2_mc_w = new TH2D("ntrk_IP2D_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1Flip_pt2_mc_w = new TH2D("ntrk_SV1Flip_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1_pt2_mc_w = new TH2D("ntrk_SV1_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitterFlip_pt2_mc_w = new TH2D("ntrk_JetFitterFlip_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitter_pt2_mc_w = new TH2D("ntrk_JetFitter_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_IP3DNeg_pt2_mc_w = new TH2D("trackntrk_IP3DNeg_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_IP3D_pt2_mc_w = new TH2D("trackntrk_IP3D_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_IP2DNeg_pt2_mc_w = new TH2D("trackntrk_IP2DNeg_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_IP2D_pt2_mc_w = new TH2D("trackntrk_IP2D_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_SV1Flip_pt2_mc_w = new TH2D("trackntrk_SV1Flip_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_SV1_pt2_mc_w = new TH2D("trackntrk_SV1_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_JetFitterFlip_pt2_mc_w = new TH2D("trackntrk_JetFitterFlip_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_trackntrk_JetFitter_pt2_mc_w = new TH2D("trackntrk_JetFitter_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);


    // Correct uncertainties for weighted histograms
    h_njets_event_mc_w->Sumw2();
    h_averageInteractionsPerCrossing_mc_w->Sumw2();
    h_ntrackjets_event_mc_w->Sumw2();

    h_pt1_mc_w->Sumw2();
    h_eta1_mc_w->Sumw2();
    h_pt_thin1_mc_w->Sumw2();
    h_eta_thin1_mc_w->Sumw2();
    h_phi1_mc_w->Sumw2();
    h_trackpt1_mc_w->Sumw2();
    h_tracketa1_mc_w->Sumw2();
    h_trackpt_thin1_mc_w->Sumw2();
    h_tracketa_thin1_mc_w->Sumw2();
    h_trackphi1_mc_w->Sumw2();

    h_ntrk_IP3DNeg1_mc_w->Sumw2();
    h_ntrk_IP3D1_mc_w->Sumw2();
    h_ntrk_IP2DNeg1_mc_w->Sumw2();
    h_ntrk_IP2D1_mc_w->Sumw2();
    h_ntrk_SV1Flip1_mc_w->Sumw2();
    h_ntrk_SV11_mc_w->Sumw2();
    h_ntrk_JetFitterFlip1_mc_w->Sumw2();
    h_ntrk_JetFitter1_mc_w->Sumw2();
    h_trackntrk_IP3DNeg1_mc_w->Sumw2();
    h_trackntrk_IP3D1_mc_w->Sumw2();
    h_trackntrk_IP2DNeg1_mc_w->Sumw2();
    h_trackntrk_IP2D1_mc_w->Sumw2();
    h_trackntrk_SV1Flip1_mc_w->Sumw2();
    h_trackntrk_SV11_mc_w->Sumw2();
    h_trackntrk_JetFitterFlip1_mc_w->Sumw2();
    h_trackntrk_JetFitter1_mc_w->Sumw2(); 

    h_pteta1_mc_w->Sumw2();
    h_trackpteta1_mc_w->Sumw2();

    h_ntrk_IP3DNeg_pt1_mc_w->Sumw2();
    h_ntrk_IP3D_pt1_mc_w->Sumw2();
    h_ntrk_IP2DNeg_pt1_mc_w->Sumw2();
    h_ntrk_IP2D_pt1_mc_w->Sumw2();
    h_ntrk_SV1Flip_pt1_mc_w->Sumw2();
    h_ntrk_SV1_pt1_mc_w->Sumw2();
    h_ntrk_JetFitterFlip_pt1_mc_w->Sumw2();
    h_ntrk_JetFitter_pt1_mc_w->Sumw2();
    h_trackntrk_IP3DNeg_pt1_mc_w->Sumw2();
    h_trackntrk_IP3D_pt1_mc_w->Sumw2();
    h_trackntrk_IP2DNeg_pt1_mc_w->Sumw2();
    h_trackntrk_IP2D_pt1_mc_w->Sumw2();
    h_trackntrk_SV1Flip_pt1_mc_w->Sumw2();
    h_trackntrk_SV1_pt1_mc_w->Sumw2();
    h_trackntrk_JetFitterFlip_pt1_mc_w->Sumw2();
    h_trackntrk_JetFitter_pt1_mc_w->Sumw2();

    h_pt2_mc_w->Sumw2();
    h_eta2_mc_w->Sumw2();
    h_pt_thin2_mc_w->Sumw2();
    h_eta_thin2_mc_w->Sumw2();
    h_phi2_mc_w->Sumw2();
    h_trackpt2_mc_w->Sumw2();
    h_tracketa2_mc_w->Sumw2();
    h_trackpt_thin2_mc_w->Sumw2();
    h_tracketa_thin2_mc_w->Sumw2();
    h_trackphi2_mc_w->Sumw2();
 
    h_ntrk_IP3DNeg2_mc_w->Sumw2();
    h_ntrk_IP3D2_mc_w->Sumw2();
    h_ntrk_IP2DNeg2_mc_w->Sumw2();
    h_ntrk_IP2D2_mc_w->Sumw2();
    h_ntrk_SV1Flip2_mc_w->Sumw2();
    h_ntrk_SV12_mc_w->Sumw2();
    h_ntrk_JetFitterFlip2_mc_w->Sumw2();
    h_ntrk_JetFitter2_mc_w->Sumw2();
    h_trackntrk_IP3DNeg2_mc_w->Sumw2();
    h_trackntrk_IP3D2_mc_w->Sumw2();
    h_trackntrk_IP2DNeg2_mc_w->Sumw2();
    h_trackntrk_IP2D2_mc_w->Sumw2();
    h_trackntrk_SV1Flip2_mc_w->Sumw2();
    h_trackntrk_SV12_mc_w->Sumw2();
    h_trackntrk_JetFitterFlip2_mc_w->Sumw2();
    h_trackntrk_JetFitter2_mc_w->Sumw2();
 
    h_pteta2_mc_w->Sumw2();
    h_trackpteta2_mc_w->Sumw2();

    h_ntrk_IP3DNeg_pt2_mc_w->Sumw2();
    h_ntrk_IP3D_pt2_mc_w->Sumw2();
    h_ntrk_IP2DNeg_pt2_mc_w->Sumw2();
    h_ntrk_IP2D_pt2_mc_w->Sumw2();
    h_ntrk_SV1Flip_pt2_mc_w->Sumw2();
    h_ntrk_SV1_pt2_mc_w->Sumw2();
    h_ntrk_JetFitterFlip_pt2_mc_w->Sumw2();
    h_ntrk_JetFitter_pt2_mc_w->Sumw2();
    h_trackntrk_IP3DNeg_pt2_mc_w->Sumw2();
    h_trackntrk_IP3D_pt2_mc_w->Sumw2();
    h_trackntrk_IP2DNeg_pt2_mc_w->Sumw2();
    h_trackntrk_IP2D_pt2_mc_w->Sumw2();
    h_trackntrk_SV1Flip_pt2_mc_w->Sumw2();
    h_trackntrk_SV1_pt2_mc_w->Sumw2();
    h_trackntrk_JetFitterFlip_pt2_mc_w->Sumw2();
    h_trackntrk_JetFitter_pt2_mc_w->Sumw2();

  }


  //************************************************ //
  //               Bootstrap Replicas                //
  //************************************************ //

  TH2D* array_h_pteta1_mc_w[2];
  TH2D* array_h_pteta2_mc_w[2];
  TH2D* array_h_trackpteta1_mc_w[2];
  TH2D* array_h_trackpteta2_mc_w[2];

  if (m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
  {
     for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
     {
       array_h_pteta1_mc_w[ibootstrap] = new TH2D( (string("pteta1_mc_w_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
       array_h_pteta2_mc_w[ibootstrap] = new TH2D(  (string("pteta2_mc_w_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
	array_h_trackpteta1_mc_w[ibootstrap] = new TH2D( (string("trackpteta1_mc_w_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
       array_h_trackpteta2_mc_w[ibootstrap] = new TH2D(  (string("trackpteta2_mc_w_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
       array_h_pteta1_mc_w[ibootstrap]->Sumw2();
       array_h_pteta2_mc_w[ibootstrap]->Sumw2();
       array_h_trackpteta1_mc_w[ibootstrap]->Sumw2();
       array_h_trackpteta2_mc_w[ibootstrap]->Sumw2();

     }

  }

  // ************************************************ //
  //           End of bootstrap definition            //
  // ************************************************ //


  // Start to loop on the event of a chain/tree
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if(jentry ==0) cout<<" all: "<<nentries<<", ientry by loadTree: "<<ientry<<endl;
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      if (jentry%10000==0) 
	{
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

      for (int j = 0; j<njets; ++j) 
	{
	  double ptj = jetpt[j];
	  int ntracks_IP3DNeg = ntrack_IP3DNeg[j];
	  int ntracks_IP3D = ntrack_IP3D[j];
	  int ntracks_IP2DNeg = ntrack_IP2DNeg[j];
	  int ntracks_IP2D = ntrack_IP2D[j];
	  int ntracks_SV1Flip = ntrack_SV1Flip[j];
	  int ntracks_SV1 = ntrack_SV1[j];
	  int ntracks_JetFitterFlip = ntrack_JetFitterFlip[j];
	  int ntracks_JetFitter = ntrack_JetFitter[j];
          double trackptj = trackjetpt[j];
          int trackntracks_IP3DNeg = trackntrack_IP3DNeg[j];
          int trackntracks_IP3D = trackntrack_IP3D[j];
          int trackntracks_IP2DNeg = trackntrack_IP2DNeg[j];
          int trackntracks_IP2D = trackntrack_IP2D[j];
          int trackntracks_SV1Flip = trackntrack_SV1Flip[j];
          int trackntracks_SV1 = trackntrack_SV1[j];
          int trackntracks_JetFitterFlip = trackntrack_JetFitterFlip[j];
          int trackntracks_JetFitter = trackntrack_JetFitter[j];

	  double eta_abs = abs(jeteta[j]);
	  double tracketa_abs = abs(trackjeteta[j]);

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

          // ---------- leading jet ---------- //
          // jetpass - use leading prescale
	  if(isleading[j] && jetpass_leading==0 && jetpass_subleading<2 )
	    {
              // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
              if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
              {
                h_njets_event_mc_w->Fill(njets_event, weight);
                h_ntrackjets_event_mc_w->Fill(ntrackjets_event, weight);
		h_averageInteractionsPerCrossing_mc_w->Fill(evt_averageInteractionsPerCrossing, weight);
 
  	        h_pt1_mc_w->Fill(ptj, weight);
  	        h_eta1_mc_w->Fill(eta_abs, weight);
	        h_pt_thin1_mc_w->Fill(ptj, weight);
	        h_eta_thin1_mc_w->Fill(eta_abs, weight);
  	        h_phi1_mc_w->Fill(jetphi[j], weight);
                h_trackpt1_mc_w->Fill(trackptj, weight);
                h_tracketa1_mc_w->Fill(tracketa_abs, weight);
                h_trackpt_thin1_mc_w->Fill(trackptj, weight);
                h_tracketa_thin1_mc_w->Fill(tracketa_abs, weight);
                h_trackphi1_mc_w->Fill(trackjetphi[j], weight);

                int k=0;
                for (const auto &name: subtagger::floats){
                  h_float_subtagger1_mc_w.at(k)->Fill(float_subtagger[name.first][j], weight);
                  k++;
                }

                k=0;
                for (const auto &name: subtagger::ints){
                  h_int_subtagger1_mc_w.at(k)->Fill(int_subtagger[name.first][j], weight);
                  k++;
                }

                k=0;
                for (const auto &name: subtagger::doubles){
                  h_double_subtagger1_mc_w.at(k)->Fill(double_subtagger[name.first][j], weight);
                  k++;
                }
		k=0;
		for (const auto &name: subtagger::floats_trackjet){
                  h_float_subtagger1_track_mc_w.at(k)->Fill(float_subtagger_trackjet[name.first][j], weight);
                  k++;
                }

                k=0;
                for (const auto &name: subtagger::ints_trackjet){
                  h_int_subtagger1_track_mc_w.at(k)->Fill(int_subtagger_trackjet[name.first][j], weight);
                  k++;
                }

                k=0;
                for (const auto &name: subtagger::doubles_trackjet){
                  h_double_subtagger1_track_mc_w.at(k)->Fill(double_subtagger_trackjet[name.first][j], weight);
                  k++;
                }

                h_ntrk_IP3DNeg1_mc_w->Fill(ntracks_IP3DNeg, weight);
                h_ntrk_IP3D1_mc_w->Fill(ntracks_IP3D, weight);
                h_ntrk_IP2DNeg1_mc_w->Fill(ntracks_IP2DNeg, weight);
                h_ntrk_IP2D1_mc_w->Fill(ntracks_IP2D, weight);
                h_ntrk_SV1Flip1_mc_w->Fill(ntracks_SV1Flip, weight);
                h_ntrk_SV11_mc_w->Fill(ntracks_SV1, weight);
                h_ntrk_JetFitterFlip1_mc_w->Fill(ntracks_JetFitterFlip, weight);
                h_ntrk_JetFitter1_mc_w->Fill(ntracks_JetFitter, weight);
                h_trackntrk_IP3DNeg1_mc_w->Fill(trackntracks_IP3DNeg, weight);
                h_trackntrk_IP3D1_mc_w->Fill(trackntracks_IP3D, weight);
                h_trackntrk_IP2DNeg1_mc_w->Fill(trackntracks_IP2DNeg, weight);
                h_trackntrk_IP2D1_mc_w->Fill(trackntracks_IP2D, weight);
                h_trackntrk_SV1Flip1_mc_w->Fill(trackntracks_SV1Flip, weight);
                h_trackntrk_SV11_mc_w->Fill(trackntracks_SV1, weight);
                h_trackntrk_JetFitterFlip1_mc_w->Fill(trackntracks_JetFitterFlip, weight);
                h_trackntrk_JetFitter1_mc_w->Fill(trackntracks_JetFitter, weight);

  	        h_pteta1_mc_w->Fill(ptj, eta_abs, weight);
		h_trackpteta1_mc_w->Fill(trackptj, tracketa_abs, weight);

                h_ntrk_IP3DNeg_pt1_mc_w->Fill(ntracks_IP3DNeg, ptj, weight);
                h_ntrk_IP3D_pt1_mc_w->Fill(ntracks_IP3D, ptj, weight);
                h_ntrk_IP2DNeg_pt1_mc_w->Fill(ntracks_IP2DNeg, ptj, weight);
                h_ntrk_IP2D_pt1_mc_w->Fill(ntracks_IP2D, ptj, weight);
                h_ntrk_SV1Flip_pt1_mc_w->Fill(ntracks_SV1Flip, ptj, weight);
                h_ntrk_SV1_pt1_mc_w->Fill(ntracks_SV1, ptj, weight);
                h_ntrk_JetFitterFlip_pt1_mc_w->Fill(ntracks_JetFitterFlip, ptj, weight);
                h_ntrk_JetFitter_pt1_mc_w->Fill(ntracks_JetFitter, ptj, weight);
                h_trackntrk_IP3DNeg_pt1_mc_w->Fill(trackntracks_IP3DNeg, trackptj, weight);
                h_trackntrk_IP3D_pt1_mc_w->Fill(trackntracks_IP3D, trackptj, weight);
                h_trackntrk_IP2DNeg_pt1_mc_w->Fill(trackntracks_IP2DNeg, trackptj, weight);
                h_trackntrk_IP2D_pt1_mc_w->Fill(trackntracks_IP2D, trackptj, weight);
                h_trackntrk_SV1Flip_pt1_mc_w->Fill(trackntracks_SV1Flip, trackptj, weight);
                h_trackntrk_SV1_pt1_mc_w->Fill(trackntracks_SV1, trackptj, weight);
                h_trackntrk_JetFitterFlip_pt1_mc_w->Fill(trackntracks_JetFitterFlip, trackptj, weight);
                h_trackntrk_JetFitter_pt1_mc_w->Fill(trackntracks_JetFitter, trackptj, weight);

              }

              // bootstrap replicas
              if (m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
              {
                for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
                {
                  array_h_pteta1_mc_w[ibootstrap]->Fill(ptj, eta_abs, weight*weight_bootstrap[(bootstrap_bkeeper-1)*2+ibootstrap]);
                array_h_trackpteta1_mc_w[ibootstrap]->Fill(trackptj, tracketa_abs, weight*weight_bootstrap[(bootstrap_bkeeper-1)*2+ibootstrap]);
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

              }
              // otherwise use subleading prescale, again not to mix different precales
              else if(jetpass_subleading!=0 || jetpass_leading>1) continue;

              // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
              if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
              {
 
  	        h_pt2_mc_w->Fill(ptj, weight);
  	        h_eta2_mc_w->Fill(eta_abs, weight);
	        h_pt_thin2_mc_w->Fill(ptj, weight);
	        h_eta_thin2_mc_w->Fill(eta_abs, weight);
  	        h_phi2_mc_w->Fill(jetphi[j], weight);

                int k=0;
                for (const auto &name: subtagger::floats){
                  h_float_subtagger2_mc_w.at(k)->Fill(float_subtagger[name.first][j], weight);
                  k++;
                }

                k=0;
                for (const auto &name: subtagger::ints){
                  h_int_subtagger2_mc_w.at(k)->Fill(int_subtagger[name.first][j], weight);
                  k++;
                }

                k=0;
                for (const auto &name: subtagger::doubles){
                  h_double_subtagger2_mc_w.at(k)->Fill(double_subtagger[name.first][j], weight);
                  k++;
                }

                h_ntrk_IP3DNeg2_mc_w->Fill(ntracks_IP3DNeg, weight);
                h_ntrk_IP3D2_mc_w->Fill(ntracks_IP3D, weight);
                h_ntrk_IP2DNeg2_mc_w->Fill(ntracks_IP2DNeg, weight);
                h_ntrk_IP2D2_mc_w->Fill(ntracks_IP2D, weight);
                h_ntrk_SV1Flip2_mc_w->Fill(ntracks_SV1Flip, weight);
                h_ntrk_SV12_mc_w->Fill(ntracks_SV1, weight);
                h_ntrk_JetFitterFlip2_mc_w->Fill(ntracks_JetFitterFlip, weight);
                h_ntrk_JetFitter2_mc_w->Fill(ntracks_JetFitter, weight);

  	        h_pteta2_mc_w->Fill(ptj, eta_abs, weight);

                h_ntrk_IP3DNeg_pt2_mc_w->Fill(ntracks_IP3DNeg, ptj, weight);
                h_ntrk_IP3D_pt2_mc_w->Fill(ntracks_IP3D, ptj, weight);
                h_ntrk_IP2DNeg_pt2_mc_w->Fill(ntracks_IP2DNeg, ptj, weight);
                h_ntrk_IP2D_pt2_mc_w->Fill(ntracks_IP2D, ptj, weight);
                h_ntrk_SV1Flip_pt2_mc_w->Fill(ntracks_SV1Flip, ptj, weight);
                h_ntrk_SV1_pt2_mc_w->Fill(ntracks_SV1, ptj, weight);
                h_ntrk_JetFitterFlip_pt2_mc_w->Fill(ntracks_JetFitterFlip, ptj, weight);
                h_ntrk_JetFitter_pt2_mc_w->Fill(ntracks_JetFitter, ptj, weight);

              }

              // bootstrap replicas
              if (m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
              {
                for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
                {
                  array_h_pteta2_mc_w[ibootstrap]->Fill(ptj, eta_abs, weight*weight_bootstrap[(bootstrap_bkeeper-1)*2+ibootstrap]);
                }
	      }

	    }
	}
    }
} // end of the Loop member function.
