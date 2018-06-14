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
const int debug=0;
extern bool runmc;

void CreateHistograms::Loop(int bootstrap_bkeeper=0)
{
  const double pi = M_PI, twopi = 2*pi;

  if (fChain == 0) return;

  int currentTree = -1;

  // Control plots

  //----------event-wide----------//
  TH1D* h_njets_event_mc_w; 
  TH1D* h_averageInteractionsPerCrossing_mc_w;

  //----------leading jet----------//
  // 1D - kinematics
  TH1D* h_pt1_mc_w; 
  TH1D* h_eta1_mc_w; 
  TH1D* h_pt_thin1_mc_w; 
  TH1D* h_eta_thin1_mc_w; 
  TH1D* h_phi1_mc_w; 
  // 1D - taggers and subtaggers
  std::vector<TH1D*> h_float_subtagger1_mc_w;
  std::vector<TH1D*> h_int_subtagger1_mc_w;
  std::vector<TH1D*> h_double_subtagger1_mc_w;
  // 1D - ntrk
  TH1D* h_ntrk_IP3DNeg1_mc_w;
  TH1D* h_ntrk_IP3D1_mc_w;
  TH1D* h_ntrk_IP2DNeg1_mc_w;
  TH1D* h_ntrk_IP2D1_mc_w;
  TH1D* h_ntrk_SV1Flip1_mc_w;
  TH1D* h_ntrk_SV11_mc_w;
  TH1D* h_ntrk_JetFitterFlip1_mc_w;
  TH1D* h_ntrk_JetFitter1_mc_w;

  // 2D - kinematics
  TH2D* h_pteta1_mc_w; 
  // 2D - ntrk
  TH2D* h_ntrk_IP3DNeg_pt1_mc_w;
  TH2D* h_ntrk_IP3D_pt1_mc_w;
  TH2D* h_ntrk_IP2DNeg_pt1_mc_w;
  TH2D* h_ntrk_IP2D_pt1_mc_w;
  TH2D* h_ntrk_SV1Flip_pt1_mc_w;
  TH2D* h_ntrk_SV1_pt1_mc_w;
  TH2D* h_ntrk_JetFitterFlip_pt1_mc_w;
  TH2D* h_ntrk_JetFitter_pt1_mc_w;

  //----------subleading jet----------//
  // 1D - kinematics
  TH1D* h_pt2_mc_w; 
  TH1D* h_eta2_mc_w; 
  TH1D* h_pt_thin2_mc_w; 
  TH1D* h_eta_thin2_mc_w; 
  TH1D* h_phi2_mc_w; 
  // 1D - taggers and subtaggers
  std::vector<TH1D*> h_float_subtagger2_mc_w;
  std::vector<TH1D*> h_int_subtagger2_mc_w;
  std::vector<TH1D*> h_double_subtagger2_mc_w;
  // 1D - ntrk
  TH1D* h_ntrk_IP3DNeg2_mc_w;
  TH1D* h_ntrk_IP3D2_mc_w;
  TH1D* h_ntrk_IP2DNeg2_mc_w;
  TH1D* h_ntrk_IP2D2_mc_w;
  TH1D* h_ntrk_SV1Flip2_mc_w;
  TH1D* h_ntrk_SV12_mc_w;
  TH1D* h_ntrk_JetFitterFlip2_mc_w;
  TH1D* h_ntrk_JetFitter2_mc_w;

  // 2D - kinematics
  TH2D* h_pteta2_mc_w; 
  // 2D - ntrk
  TH2D* h_ntrk_IP3DNeg_pt2_mc_w;
  TH2D* h_ntrk_IP3D_pt2_mc_w;
  TH2D* h_ntrk_IP2DNeg_pt2_mc_w;
  TH2D* h_ntrk_IP2D_pt2_mc_w;
  TH2D* h_ntrk_SV1Flip_pt2_mc_w;
  TH2D* h_ntrk_SV1_pt2_mc_w;
  TH2D* h_ntrk_JetFitterFlip_pt2_mc_w;
  TH2D* h_ntrk_JetFitter_pt2_mc_w;


  // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
  if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
  {
    //----------event-wide----------//
    h_njets_event_mc_w = new TH1D("njets_event_mc_w", "", 20, 0, 20); 
    h_averageInteractionsPerCrossing_mc_w = new TH1D("averageInteractionsPerCrossing_mc_w", "", 100, 0, 100); 

    //----------leading jet----------//
    // 1D - kinematics
    h_pt1_mc_w = new TH1D("pt1_mc_w", "", conf::n_pt, conf::pt_lowedges); 
    h_eta1_mc_w = new TH1D("eta1_mc_w", "", conf::n_eta, conf::eta_lowedges);
    h_pt_thin1_mc_w = new TH1D("pt_thin1_mc_w", "", 600, 0., 3000.); 
    h_eta_thin1_mc_w = new TH1D("eta_thin1_mc_w", "", 25, 0., 2.5);
    h_phi1_mc_w = new TH1D("phi1_mc_w", "", 60, -3.16, 3.16);
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
    // 1D - ntrk
    h_ntrk_IP3DNeg1_mc_w = new TH1D("ntrk_IP3DNeg1_mc_w","",25, 0, 25);
    h_ntrk_IP3D1_mc_w = new TH1D("ntrk_IP3D1_mc_w","",25, 0, 25);
    h_ntrk_IP2DNeg1_mc_w = new TH1D("ntrk_IP2DNeg1_mc_w","",25, 0, 25);
    h_ntrk_IP2D1_mc_w = new TH1D("ntrk_IP2D1_mc_w","",25, 0, 25);
    h_ntrk_SV1Flip1_mc_w = new TH1D("ntrk_SV1Flip1_mc_w","",25, 0, 25);
    h_ntrk_SV11_mc_w = new TH1D("ntrk_SV11_mc_w","",25, 0, 25);
    h_ntrk_JetFitterFlip1_mc_w = new TH1D("ntrk_JetFitterFlip1_mc_w","",25, 0, 25);
    h_ntrk_JetFitter1_mc_w = new TH1D("ntrk_JetFitter1_mc_w","",25, 0, 25);

    // 2D - kinematics
    h_pteta1_mc_w = new TH2D("pteta1_mc_w", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
    // 2D - ntrk
    h_ntrk_IP3DNeg_pt1_mc_w = new TH2D("ntrk_IP3DNeg_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP3D_pt1_mc_w = new TH2D("ntrk_IP3D_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2DNeg_pt1_mc_w = new TH2D("ntrk_IP2DNeg_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2D_pt1_mc_w = new TH2D("ntrk_IP2D_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1Flip_pt1_mc_w = new TH2D("ntrk_SV1Flip_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1_pt1_mc_w = new TH2D("ntrk_SV1_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitterFlip_pt1_mc_w = new TH2D("ntrk_JetFitterFlip_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitter_pt1_mc_w = new TH2D("ntrk_JetFitter_pt1_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);

    //----------subleading jet----------//
    // 1D - kinematics
    h_pt2_mc_w = new TH1D("pt2_mc_w", "", conf::n_pt, conf::pt_lowedges); 
    h_eta2_mc_w = new TH1D("eta2_mc_w", "", conf::n_eta, conf::eta_lowedges);
    h_pt_thin2_mc_w = new TH1D("pt_thin2_mc_w", "", 600, 0., 3000.); 
    h_eta_thin2_mc_w = new TH1D("eta_thin2_mc_w", "", 25, 0., 2.5);
    h_phi2_mc_w = new TH1D("phi2_mc_w", "", 60, -3.16, 3.16);
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
    // 1D - ntrk
    h_ntrk_IP3DNeg2_mc_w = new TH1D("ntrk_IP3DNeg2_mc_w","",25, 0, 25);
    h_ntrk_IP3D2_mc_w = new TH1D("ntrk_IP3D2_mc_w","",25, 0, 25);
    h_ntrk_IP2DNeg2_mc_w = new TH1D("ntrk_IP2DNeg2_mc_w","",25, 0, 25);
    h_ntrk_IP2D2_mc_w = new TH1D("ntrk_IP2D2_mc_w","",25, 0, 25);
    h_ntrk_SV1Flip2_mc_w = new TH1D("ntrk_SV1Flip2_mc_w","",25, 0, 25);
    h_ntrk_SV12_mc_w = new TH1D("ntrk_SV12_mc_w","",25, 0, 25);
    h_ntrk_JetFitterFlip2_mc_w = new TH1D("ntrk_JetFitterFlip2_mc_w","",25, 0, 25);
    h_ntrk_JetFitter2_mc_w = new TH1D("ntrk_JetFitter2_mc_w","",25, 0, 25);

    // 2D - kinematics
    h_pteta2_mc_w = new TH2D("pteta2_mc_w", "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
    // 2D - ntrk
    h_ntrk_IP3DNeg_pt2_mc_w = new TH2D("ntrk_IP3DNeg_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP3D_pt2_mc_w = new TH2D("ntrk_IP3D_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2DNeg_pt2_mc_w = new TH2D("ntrk_IP2DNeg_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_IP2D_pt2_mc_w = new TH2D("ntrk_IP2D_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1Flip_pt2_mc_w = new TH2D("ntrk_SV1Flip_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_SV1_pt2_mc_w = new TH2D("ntrk_SV1_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitterFlip_pt2_mc_w = new TH2D("ntrk_JetFitterFlip_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);
    h_ntrk_JetFitter_pt2_mc_w = new TH2D("ntrk_JetFitter_pt2_mc_w","",25, 0, 25, conf::n_pt, conf::pt_lowedges);

    // Correct uncertainties for weighted histograms
    h_njets_event_mc_w->Sumw2();
    h_averageInteractionsPerCrossing_mc_w->Sumw2();

    h_pt1_mc_w->Sumw2();
    h_eta1_mc_w->Sumw2();
    h_pt_thin1_mc_w->Sumw2();
    h_eta_thin1_mc_w->Sumw2();
    h_phi1_mc_w->Sumw2();

    h_ntrk_IP3DNeg1_mc_w->Sumw2();
    h_ntrk_IP3D1_mc_w->Sumw2();
    h_ntrk_IP2DNeg1_mc_w->Sumw2();
    h_ntrk_IP2D1_mc_w->Sumw2();
    h_ntrk_SV1Flip1_mc_w->Sumw2();
    h_ntrk_SV11_mc_w->Sumw2();
    h_ntrk_JetFitterFlip1_mc_w->Sumw2();
    h_ntrk_JetFitter1_mc_w->Sumw2();

    h_pteta1_mc_w->Sumw2();

    h_ntrk_IP3DNeg_pt1_mc_w->Sumw2();
    h_ntrk_IP3D_pt1_mc_w->Sumw2();
    h_ntrk_IP2DNeg_pt1_mc_w->Sumw2();
    h_ntrk_IP2D_pt1_mc_w->Sumw2();
    h_ntrk_SV1Flip_pt1_mc_w->Sumw2();
    h_ntrk_SV1_pt1_mc_w->Sumw2();
    h_ntrk_JetFitterFlip_pt1_mc_w->Sumw2();
    h_ntrk_JetFitter_pt1_mc_w->Sumw2();

    h_pt2_mc_w->Sumw2();
    h_eta2_mc_w->Sumw2();
    h_pt_thin2_mc_w->Sumw2();
    h_eta_thin2_mc_w->Sumw2();
    h_phi2_mc_w->Sumw2();
 
    h_ntrk_IP3DNeg2_mc_w->Sumw2();
    h_ntrk_IP3D2_mc_w->Sumw2();
    h_ntrk_IP2DNeg2_mc_w->Sumw2();
    h_ntrk_IP2D2_mc_w->Sumw2();
    h_ntrk_SV1Flip2_mc_w->Sumw2();
    h_ntrk_SV12_mc_w->Sumw2();
    h_ntrk_JetFitterFlip2_mc_w->Sumw2();
    h_ntrk_JetFitter2_mc_w->Sumw2();
   
    h_pteta2_mc_w->Sumw2();

    h_ntrk_IP3DNeg_pt2_mc_w->Sumw2();
    h_ntrk_IP3D_pt2_mc_w->Sumw2();
    h_ntrk_IP2DNeg_pt2_mc_w->Sumw2();
    h_ntrk_IP2D_pt2_mc_w->Sumw2();
    h_ntrk_SV1Flip_pt2_mc_w->Sumw2();
    h_ntrk_SV1_pt2_mc_w->Sumw2();
    h_ntrk_JetFitterFlip_pt2_mc_w->Sumw2();
    h_ntrk_JetFitter_pt2_mc_w->Sumw2();
  }


  //************************************************ //
  //               Bootstrap Replicas                //
  //************************************************ //

  TH2D* array_h_pteta1_mc_w[2];
  TH2D* array_h_pteta2_mc_w[2];

  if (m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
  {
     for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
     {
       array_h_pteta1_mc_w[ibootstrap] = new TH2D( (string("pteta1_mc_w_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
       array_h_pteta2_mc_w[ibootstrap] = new TH2D(  (string("pteta2_mc_w_") + to_string((bootstrap_bkeeper-1)*2+ibootstrap)).c_str(), "", conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);

       array_h_pteta1_mc_w[ibootstrap]->Sumw2();
       array_h_pteta2_mc_w[ibootstrap]->Sumw2();
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
      if(debug == 1 && jentry>1000){
	cout << "first 1k events for debugging. stop here!"<< endl;
	break;
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

      if(debug==1) cout << "njets = "<< njets << ", jetpt0="<<jetpt[0]<< " " << ", jetpt1="<<jetpt[1]<< ". "<< endl;
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

	  double eta_abs = abs(jeteta[j]);

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

	  if(debug == 1) cout <<" in jet loop, entering LEADING: isleading["<<j<<"]="<<isleading[j]<< endl ;
          // ---------- leading jet ---------- //
          // jetpass - use leading prescale
	  if(isleading[j] && jetpass_leading==0 && jetpass_subleading<2 )
	    {
              // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
              if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
              {
                h_njets_event_mc_w->Fill(njets_event, weight);
                h_averageInteractionsPerCrossing_mc_w->Fill(evt_averageInteractionsPerCrossing, weight);

		if(debug == 1) cout <<"          LEADING: !m_systematic.EqualTo(FlavourTagging_Nominal) || bootstrap_bkeeper<=1: fill h_pt1_mc_w = "<<ptj<<", weight= "<< weight << endl ;
 
  	        h_pt1_mc_w->Fill(ptj, weight);
  	        h_eta1_mc_w->Fill(eta_abs, weight);
	        h_pt_thin1_mc_w->Fill(ptj, weight);
	        h_eta_thin1_mc_w->Fill(eta_abs, weight);
  	        h_phi1_mc_w->Fill(jetphi[j], weight);

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

                h_ntrk_IP3DNeg1_mc_w->Fill(ntracks_IP3DNeg, weight);
                h_ntrk_IP3D1_mc_w->Fill(ntracks_IP3D, weight);
                h_ntrk_IP2DNeg1_mc_w->Fill(ntracks_IP2DNeg, weight);
                h_ntrk_IP2D1_mc_w->Fill(ntracks_IP2D, weight);
                h_ntrk_SV1Flip1_mc_w->Fill(ntracks_SV1Flip, weight);
                h_ntrk_SV11_mc_w->Fill(ntracks_SV1, weight);
                h_ntrk_JetFitterFlip1_mc_w->Fill(ntracks_JetFitterFlip, weight);
                h_ntrk_JetFitter1_mc_w->Fill(ntracks_JetFitter, weight);

  	        h_pteta1_mc_w->Fill(ptj, eta_abs, weight);

                h_ntrk_IP3DNeg_pt1_mc_w->Fill(ntracks_IP3DNeg, ptj, weight);
                h_ntrk_IP3D_pt1_mc_w->Fill(ntracks_IP3D, ptj, weight);
                h_ntrk_IP2DNeg_pt1_mc_w->Fill(ntracks_IP2DNeg, ptj, weight);
                h_ntrk_IP2D_pt1_mc_w->Fill(ntracks_IP2D, ptj, weight);
                h_ntrk_SV1Flip_pt1_mc_w->Fill(ntracks_SV1Flip, ptj, weight);
                h_ntrk_SV1_pt1_mc_w->Fill(ntracks_SV1, ptj, weight);
                h_ntrk_JetFitterFlip_pt1_mc_w->Fill(ntracks_JetFitterFlip, ptj, weight);
                h_ntrk_JetFitter_pt1_mc_w->Fill(ntracks_JetFitter, ptj, weight);
              }

              // bootstrap replicas
              if (m_systematic.EqualTo("FlavourTagging_Nominal") && bootstrap_bkeeper>0)
              {
                for(int ibootstrap=0; ibootstrap<2; ibootstrap++)
                {
                  array_h_pteta1_mc_w[ibootstrap]->Fill(ptj, eta_abs, weight*weight_bootstrap[(bootstrap_bkeeper-1)*2+ibootstrap]);
                }
	      }

	    } 
          // ---------- subleading jet ---------- //
	  else if(!isleading[j])
	    {
	      if(debug == 1) cout <<"       entering  S U B -  LEADING: (!isleading["<<j<<"] "<< isleading[j]  << endl;
		
	      if(debug == 1) cout <<"          SUBLEADING: getPtBin(ptj) = "<<getPtBin(ptj)<<", ptj= "<< ptj << endl;
              // if subleading in the first bin, consider only events triggered by the leading in the second bin for 100% trigger efficiency
              if(getPtBin(ptj)==1)
              {
		if(debug == 1) cout <<"      if subleading in the first bin, consider only events triggered by the leading in the second bin for 100% trigger efficiency "  << endl;
	  
                // Get the pT of the leading
                int index_leading = 0;
                if(j==0) index_leading = 1; 
                else     index_leading = 0;
                double pt_leading = jetpt[index_leading];
                //consider only subleading with leading in a higher bin
		if(debug == 1) cout <<"          SUBLEADING: getPtBin(pt_leading) = "<< getPtBin(pt_leading) <<", pt_leading= "<<pt_leading<< " should not be=2(continue); jetpass_leading ="<< jetpass_leading<<", jetpass_subleading = "<< jetpass_subleading << "-> tricky cut: jetpass_leading!=0 || jetpass_subleading>1) continue"<<endl;
	      if(debug == 1) cout <<"          weight before prescale = "<<weight<< endl;

                if(getPtBin(pt_leading)!=2) continue; // prescales should not be mixed
                if(jetpass_leading!=0 || jetpass_subleading>1) continue;
                // use leading jet prescale
                weight *= data_evtweight[index_leading]/data_evtweight[j];
		
		if(debug == 1) cout <<"          sbld - end of getPtBin(ptj)==1; use leading jet prescale= data_evtweight[index_leading]/data_evtweight[j] "<< data_evtweight[index_leading]/data_evtweight[j] <<", w= "<< weight << endl;

              }
              // otherwise use subleading prescale, again not to mix different precales
	      else if(jetpass_subleading!=0 || jetpass_leading>1) continue;
	      if(debug == 1) cout <<"        pass else if(jetpass_subleading!=0 || jetpass_leading>1)  ; entering !m_systematic.Equa stuff" << endl;
              // nominal histo filled except if bootstrap_bkeeper<=1 or isSyst
              if (!m_systematic.EqualTo("FlavourTagging_Nominal") || bootstrap_bkeeper<=1 )
              {
		if(debug == 1) cout <<"        @S @U @B-  LEADING: !m_systematic.EqualTo(FlavourTagging_Nominal) || bootstrap_bkeeper<=1: fill h_pt2_mc_w = "<<ptj<<", weight= "<< weight << endl ;

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
