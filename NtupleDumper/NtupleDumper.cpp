#define NtupleDumper_cxx
#include <TH2.h>
#include <TFile.h>
#include <TRandom3.h>
#include "conf.hpp"
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <cassert>
#include "NtupleDumper.h"


using namespace std;

void NtupleDumper::Loop()
{

  if (fChain == 0) return;

  // book histograms
  TH1F *h_cutflow = new TH1F("cutflow_"+m_systematic, "cutflow", 6, 0, 6);
  TH1F *h_pTavg = new TH1F("ptavg_"+m_systematic, "p_{T}^{avg}", 300, 0, 3000);
  TH1F *h_trackpTavg = new TH1F("trackptavg_"+m_systematic, "p_{T}^{avg}", 300, 0, 3000);
  TH1F *h_pTavg_ptlead = new TH1F("ptavg_ptlead_"+m_systematic, "p_{T}^{avg}/p_{T}^{lead}", 500, 0, 5);

  // for trigger check 
  TH2F *h2_triggers = new TH2F("triggers_"+m_systematic, "triggers firing in pT bins; lead jet p_{T}", 
			       conf::n_pt, conf::pt_lowedges, 
			       25, 0, 25);
  TH2F *h2_total_events = new TH2F("tot_evts_"+m_systematic, "", 
			       conf::n_pt, conf::pt_lowedges, 
			       25, 0, 25);
  TH2F *h2_tracktriggers = new TH2F("tracktriggers_"+m_systematic, "triggers firing in track pT bins; lead trackjet p_{T}", 
			       conf::n_pt, conf::pt_lowedges, 
			       25, 0, 25);
  TH2F *h2_total_trackevents = new TH2F("tot_track_evts_"+m_systematic, "", 
			       conf::n_pt, conf::pt_lowedges, 
			       25, 0, 25);

  njets = 2; // select only 2 leading jets
  ntrackjets = 2;  // select only 2 leading track jets

  // create tree
  cout << "creating tree" << endl;
  TTree *outtree = new TTree(m_systematic, "recreate");

  outtree->Branch("njets",&njets,"njets/I");
  outtree->Branch("ntrackjets",&ntrackjets,"ntrackjets/I");
  outtree->Branch("njets_event",&njets_event,"njets_event/I");
  outtree->Branch("ntrackjets_event",&ntrackjets_event,"ntrackjets_event/I");
  outtree->Branch("evtHLTj15ps",&evtHLTj15ps,"evtHLTj15ps/F");
  outtree->Branch("evtHLTj25ps",&evtHLTj25ps,"evtHLTj25ps/F");
  outtree->Branch("evtHLTj60ps",&evtHLTj60ps,"evtHLTj60ps/F");
  outtree->Branch("evtHLTj110ps",&evtHLTj110ps,"evtHLTj110ps/F");
  outtree->Branch("evtHLTj175ps",&evtHLTj175ps,"evtHLTj175ps/F");
  outtree->Branch("evtHLTj380ps",&evtHLTj380ps,"evtHLTj380ps/F");
  outtree->Branch("HLTj15",&HLTj15,"HLTj15/I");
  outtree->Branch("HLTj25",&HLTj25,"HLTj25/I");
  outtree->Branch("HLTj60",&HLTj60,"HLTj60/I");
  outtree->Branch("HLTj110",&HLTj110,"HLTj110/I");
  outtree->Branch("HLTj175",&HLTj175,"HLTj175/I");
  outtree->Branch("HLTj380",&HLTj380,"HLTj380/I");
  outtree->Branch("evtnum",&evtnum,"evtnum/I");
  outtree->Branch("runnum",&runnum,"runnum/I");
  outtree->Branch("evtpuw",&evtpuw,"evtpuw/F");
  outtree->Branch("evtpuw_sys",evtpuw_sys,"evtpuw_sys[njets]/F"); // njets used for commodities but up/down
  outtree->Branch("evtJVTw",&evtJVTw,"evtJVTw/F");
  outtree->Branch("evtJVTw_sys",evtJVTw_sys,"evtJVTw_sys[njets]/F");
  outtree->Branch("mc_evtnum",&mc_evtnum,"mc_evtnum/I");
  outtree->Branch("mc_channum",&mc_channum,"mc_channum/I");
  outtree->Branch("mc_evtweight",&mc_evtweight,"mc_evtweight/F");
  outtree->Branch("evt_averageInteractionsPerCrossing",&evt_averageInteractionsPerCrossing,"evt_averageInteractionsPerCrossing/F");

  outtree->Branch("jetpt", jetpt, "jetpt[njets]/F");
  outtree->Branch("jetphi",jetphi,"jetphi[njets]/F");
  outtree->Branch("jeteta",jeteta,"jeteta[njets]/F");
  outtree->Branch("jetpass",jetpass,"jetpass[njets]/I");
  outtree->Branch("isleading",isleading,"isleading[njets]/O");
  outtree->Branch("JVT",JVT,"JVT[njets]/F");
  outtree->Branch("clean",clean,"clean[njets]/I");
  outtree->Branch("tightBad",tightBad,"tightBad[njets]/I");
  outtree->Branch("flavor",flavor,"flavor[njets]/I");
  outtree->Branch("jetHasKShort",jetHasKShort, "jetHasKShort[njets]/I");
  outtree->Branch("jetHasLambda", jetHasLambda, "jetHasLambda[njets]/I");
  outtree->Branch("jetHasConversion", jetHasConversion, "jetHasConversion[njets]/I");
  outtree->Branch("jetHasHadMatInt", jetHasHadMatInt, "jetHasHadMatInt[njets]/I");
  outtree->Branch("jetIsDL1_Tagged", jetIsDL1_Tagged, "jetIsDL1_Tagged[njets]/I");
  outtree->Branch("jetIsDL1Flip_Tagged", jetIsDL1Flip_Tagged, "jetIsDL1Flip_Tagged[njets]/I");
//  outtree->Branch("jetIsDL1mu_Tagged", jetIsDL1mu_Tagged, "jetIsDL1mu_Tagged[njets]/I");
//  outtree->Branch("jetIsDL1Flipmu_Tagged", jetIsDL1Flipmu_Tagged, "jetIsDL1Flipmu_Tagged[njets]/I");
//  outtree->Branch("jetIsDL1rnn_Tagged", jetIsDL1rnn_Tagged, "jetIsDL1rnn_Tagged[njets]/I");
//  outtree->Branch("jetIsDL1Fliprnn_Tagged", jetIsDL1Fliprnn_Tagged, "jetIsDL1Fliprnn_Tagged[njets]/I");
  outtree->Branch("ntrack_IP3DNeg",ntrack_IP3DNeg,"ntrack_IP3DNeg[njets]/I");
  outtree->Branch("ntrack_IP3D",ntrack_IP3D,"ntrack_IP3D[njets]/I");
  outtree->Branch("ntrack_IP2DNeg",ntrack_IP2DNeg,"ntrack_IP2DNeg[njets]/I");
  outtree->Branch("ntrack_IP2D",ntrack_IP2D,"ntrack_IP2D[njets]/I");
  outtree->Branch("ntrack_SV1Flip",ntrack_SV1Flip,"ntrack_SV1Flip[njets]/I");
  outtree->Branch("ntrack_SV1",ntrack_SV1,"ntrack_SV1[njets]/I");
  outtree->Branch("ntrack_JetFitterFlip",ntrack_JetFitterFlip,"ntrack_JetFitterFlip[njets]/I");
  outtree->Branch("ntrack_JetFitter",ntrack_JetFitter,"ntrack_JetFitter[njets]/I");
  outtree->Branch("trackjetpt", trackjetpt, "trackjetpt[ntrackjets]/F");
  outtree->Branch("trackjetphi",trackjetphi,"jetphi[ntrackjets]/F");
  outtree->Branch("trackjeteta",trackjeteta,"trackjeteta[ntrackjets]/F");
  outtree->Branch("trackjetpass",trackjetpass,"trackjetpass[ntrackjets]/I");
  outtree->Branch("trackjetntrack",trackjetntrack,"trackjetntrack[ntrackjets]/I");

  outtree->Branch("trackisleading",trackisleading,"trackisleading[ntrackjets]/O");
  outtree->Branch("trackflavor",trackflavor,"trackflavor[ntrackjets]/I");
  outtree->Branch("trackjetHasKShort",trackjetHasKShort, "trackjetHasKShort[ntrackjets]/I");
  outtree->Branch("trackjetHasLambda", trackjetHasLambda, "trackjetHasLambda[ntrackjets]/I");
  outtree->Branch("trackjetHasConversion", trackjetHasConversion, "trackjetHasConversion[ntrackjets]/I");
  outtree->Branch("trackjetHasHadMatInt", trackjetHasHadMatInt, "trackjetHasHadMatInt[ntrackjets]/I");
//  outtree->Branch("trackjetIsDL1_Tagged", trackjetIsDL1_Tagged, "trackjetIsDL1_Tagged[ntrackjets]/I");
//  outtree->Branch("trackjetIsDL1Flip_Tagged", trackjetIsDL1Flip_Tagged, "tracketIsDL1Flip_Tagged[ntrackjets]/I");
//  outtree->Branch("trackjetIsDL1mu_Tagged", trackjetIsDL1mu_Tagged, "trackjetIsDL1mu_Tagged[ntrackjets]/I");
//  outtree->Branch("trackjetIsDL1Flipmu_Tagged", trackjetIsDL1Flipmu_Tagged, "trackjetIsDL1Flipmu_Tagged[ntrackjets]/I");
//  outtree->Branch("trackjetIsDL1rnn_Tagged", trackjetIsDL1rnn_Tagged, "trackjetIsDL1rnn_Tagged[ntrackjets]/I");
//  outtree->Branch("trackjetIsDL1Fliprnn_Tagged", trackjetIsDL1Fliprnn_Tagged, "trackjetIsDL1Fliprnn_Tagged[ntrackjets]/I");
  outtree->Branch("trackntrack_IP3DNeg",trackntrack_IP3DNeg,"trackntrack_IP3DNeg[ntrackjets]/I");
  outtree->Branch("trackntrack_IP3D",trackntrack_IP3D,"trackntrack_IP3D[ntrackjets]/I");
  outtree->Branch("trackntrack_IP2DNeg",trackntrack_IP2DNeg,"trackntrack_IP2DNeg[ntrackjets]/I");
  outtree->Branch("trackntrack_IP2D",trackntrack_IP2D,"trackntrack_IP2D[ntrackjets]/I");
  outtree->Branch("trackntrack_SV1Flip",trackntrack_SV1Flip,"trackntrack_SV1Flip[ntrackjets]/I");
  outtree->Branch("trackntrack_SV1",trackntrack_SV1,"trackntrack_SV1[ntrackjets]/I");
  outtree->Branch("trackntrack_JetFitterFlip",trackntrack_JetFitterFlip,"trackntrack_JetFitterFlip[ntrackjets]/I");
  outtree->Branch("trackntrack_JetFitter",trackntrack_JetFitter,"trackntrack_JetFitter[ntrackjets]/I");
  
  outtree->Branch("data_evtweight", data_evtweight, "data_evtweight[njets]/F");

  outtree->Branch("jettruthpt", jettruthpt, "jettruthpt[njets]/F");
  outtree->Branch("jettrutheta", jettrutheta, "jettrutheta[njets]/F");
  outtree->Branch("jettruthphi", jettruthphi, "jettruthphi[njets]/F");


  // for bootstrap
  // Random generator + branch declaration
  TRandom *r3 = new TRandom3();
  nbootstrap = conf::n_bootstrap; // number of bootstrap replicas
  if(m_systematic.EqualTo("FlavourTagging_Nominal"))
  {
    outtree->Branch("nbootstrap",&nbootstrap,"nbootstrap/I");
    outtree->Branch("weight_bootstrap",weight_bootstrap,"weight_bootstrap[nbootstrap]/D");
  }

  // tagger branches to be added to the tree
  for (auto const &name: subtagger::floats){
    float* a = new float[2];
    float_subtagger_out[name.first] = a;
    outtree->Branch(name.first.c_str(), a, (name.first + "[njets]/F").c_str());
  }
  for (auto const &name: subtagger::ints){
    int* a = new int[2];
    int_subtagger_out[name.first] = a;
    outtree->Branch(name.first.c_str(), a, (name.first + "[njets]/I").c_str());
  }
  for (auto const &name: subtagger::doubles){
    double* a = new double[2];
    double_subtagger_out[name.first] = a;
    outtree->Branch(name.first.c_str(), a, (name.first + "[njets]/D").c_str());
  }
// tagger branches to be added to the tree for track jets (Does this need to be done since njets = ntrackjets?)
  for (auto const &name: subtagger::floats_trackjet){
    float* a = new float[2];
    float_subtagger_trackjet_out[name.first] = a;
    outtree->Branch(name.first.c_str(), a, (name.first + "[ntrackjets]/F").c_str());
  }
  for (auto const &name: subtagger::ints_trackjet){
    int* a = new int[2];
    int_subtagger_trackjet_out[name.first] = a;
    outtree->Branch(name.first.c_str(), a, (name.first + "[ntrackjets]/I").c_str());
  }
  for (auto const &name: subtagger::doubles_trackjet){
    double* a = new double[2];
    double_subtagger_trackjet_out[name.first] = a;
    outtree->Branch(name.first.c_str(), a, (name.first + "[ntrackjets]/D").c_str());
  }
  // calculate xsec*filter eff. for sample
  double xsec_correction = 1.0;
  if (runmc){
    cout << "retrieving xsec" << endl;
    auto xsec = conf::xsec.find(m_period_slice);
    if ( xsec ==  conf::xsec.end()) cout << "####\ncould not find cross-section!\n####" << endl;
    else cout << "cross-section*filtereff: " << xsec->second<< endl;
    xsec_correction = xsec->second / m_total_events * 1e3 * conf::int_lumi; // should scale the sample to int lumi
  }

  // total number of entries of the tree
  Long64_t nentries = fChain->GetEntries();
  cout<<"total entries: "<<nentries<<endl;
  if ( nentries == 0) {
    cout << "skip file" << endl;
    return;
  }

  // initialization of some variables global to the whole sample
  const int ncuts = 5;  // number of cuts
  double nevents[ncuts] = {0}; // sum of weights
  double nevents_now[ncuts] = {0}; // unweighted number of events
  double nevents_puw[ncuts] = {0}; // sum of pile-up weights

  // cutvalues
  const double ptcut = 20.;
  const double etacut = 2.5;
  const double trackptcut = 10.;
  const double tracketacut = 2.5;
  const double trackntrackcut = 2;

  // event loop
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++){
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    // update number of processed events appearing on screen
    if (jentry%10000==0) { if(runmc) cout<<"running mc "; else cout<<"running data "; cout << jentry << '\r'; cout.flush(); }
	bool IsGoodEvent = true;
    // excludes event with no reco jets with warning.
    njets_event = jet_pt->size();
    ntrackjets_event = trackjet_pt->size();
	
    if ((njets_event < 2) || (ntrackjets_event < 2)) {
	IsGoodEvent = false;
      //std::cout << "Warning: event with no reco jet. Run/EventNumber: " << runnum << ", " << evtnum << std::endl; 
      continue;
    }

    // prepare weights
    double puweight = evtpuw;
    double JVTweight = evtJVTw;
    //mc_evtweight *= puweight; pu weight depends on systematic !!
    //mc_evtweight *= JVTweight; pu weight depends on systematic !!
    mc_evtweight *= xsec_correction; // -> mc_evtweight includes xsec/lumi weight
                                     // -> it is only MC gen weight in original Ntuples
    // 1.0 for data
    if(!runmc) 
    {
      mc_evtweight = 1.0;
      puweight = 1.0;
      JVTweight = 1.0;

      evtpuw = 1.0;
      evtJVTw = 1.0;
    }
    
    // find leading jet pt - BEFORE ANY CUTS APPLIED
    double pt1_noCut = 0.;
    double pttrack1_noCut = 0.;
    for (int i = 0; i< njets_event; ++i){
      if ((*jet_pt)[i]/1000.>pt1_noCut) pt1_noCut = (*jet_pt)[i]/1000.; // GeV
    }
    for (int i = 0; i< ntrackjets_event; ++i){
      if ((*trackjet_pt)[i]/1000.>pttrack1_noCut) pttrack1_noCut = (*trackjet_pt)[i]/1000.; // GeV
    }

    // fill histogram with trigger decisions - BEFORE ANY CUT APPLIED (removed te JVT weight from the track pt fill.  The other weights I should keep, correct?)
    for (auto trigger: trigger_names){
      h2_triggers->Fill(pt1_noCut, trigger.c_str(), puweight*mc_evtweight*JVTweight * *      trigger_decision[trigger]);
      h2_total_events->Fill(pt1_noCut, trigger.c_str(), mc_evtweight*puweight*JVTweight);
      h2_tracktriggers->Fill(pttrack1_noCut, trigger.c_str(), puweight*mc_evtweight * *      trigger_decision[trigger]);
      h2_total_trackevents->Fill(pttrack1_noCut, trigger.c_str(), mc_evtweight*puweight);
    }

    // start of cutflow (Not sure if we need tack jet cutflow?  Most of the cutflow has to do with back to back and jvt cuts in the calo jets, so for now I didn't touch it.)
    h_cutflow->Fill("C0:Input", mc_evtweight*puweight*JVTweight);

    nevents[0] += mc_evtweight*puweight*JVTweight;
    nevents_now[0]++;
    nevents_puw[0] += puweight; // already account for nevents computation

    // NPV requirement - cut already on Ntuples, should not remove events
    if (!evt_hasPV) continue; 
    h_cutflow->Fill("C1:NPV", mc_evtweight*puweight*JVTweight);

    nevents[1] += mc_evtweight*puweight*JVTweight;
    nevents_now[1]++;
    nevents_puw[1] += puweight;

    // event cleaning has been applied in ntuple production

    // select two leading jets and verify that they are back-to-back
    // count nb of reco jet > 20 GeV
    int j1 = -1, j2 = -1; // index of leading and subleading jet
    int tightBad1 = -1;
    double pt1 = 0, pt2 = 0; // pt of leading and subleading jet 
    int nj=0;

    for (int j = 0; j<njets_event; ++j) {

      double pt = (*jet_pt)[j]/1000.; // GeV
      double eta_abs = fabs((*jet_eta)[j]);

      if (pt<ptcut) continue;
      if (eta_abs > etacut) continue;
      //https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#MV2c20_tagger
      if ( pt < 60. && eta_abs < 2.4 && (*jet_JVT)[j] < 0.59) continue;        

      if (pt>pt1) {
    	j2 = j1; pt2 = pt1; 
    	j1 = j; pt1 = pt; 	
        tightBad1 = (*jet_tightBad)[j];
      } 
      else if (pt>pt2) {
    	j2 = j; pt2 = pt; 
      }
      nj++;
    }


    njets_event = nj; // number of reco jets in the event passing pT/eta/JVT cut



    // sanity check    
    if (pt1 < pt2) cout << "error " << pt1 << "<" << pt2 << endl;
 


    // select dijet events with leading passing tightBad
    if (j1<0 || j2<0 || tightBad1!=1 ) continue;
    h_cutflow->Fill("C2:dijet", mc_evtweight*puweight*JVTweight);

    nevents[2] += mc_evtweight*puweight*JVTweight;
    nevents_now[2]++;
    nevents_puw[2] += puweight;

    // select back-to-back di-jet
    double dphi = fabs((*jet_phi)[j1] - (*jet_phi)[j2]);
    if (dphi>M_PI) dphi = 2*M_PI - dphi;
    if (dphi<2.) continue;

    nevents[3] += mc_evtweight*puweight*JVTweight;
    nevents_now[3]++;
    nevents_puw[3] += puweight;

    h_cutflow->Fill("C3:back-to-back", mc_evtweight*puweight*JVTweight);

    // apply jet truth cleaning for MC
    // https://twiki.cern.ch/twiki/pub/AtlasProtected/JetStudies2012/JLacey_MC12_JZXW_weights_and_CleaningCuts.pdf
    jettruthpt[0] = -999;
    jettruthpt[1] = -999;
    jettrutheta[0] = -999;
    jettrutheta[1] = -999;
    jettruthphi[0] = -999;
    jettruthphi[1] = -999;

    if (runmc){

      // average pT of the two leading reco jets
      double pt_avg = ((*jet_pt)[j1]+(*jet_pt)[j2])/2./1000.;
      h_pTavg->Fill(pt_avg, mc_evtweight*puweight*JVTweight); 

      // find leading jet truth pT
      double pt_lead = -999;  
      double pt_sublead = -999;
      double eta_lead = -999;  
      double eta_sublead = -999;
      double phi_lead = -999;  
      double phi_sublead = -999;

      for (int j = 0; j<truthjet_pt->size(); j++) 
      {
        float jpt = (*truthjet_pt)[j];
        float jeta = (*truthjet_eta)[j];
        float jphi = (*truthjet_phi)[j];

        if (jpt/1000.>pt_lead)
        {
          pt_sublead = pt_lead;
          eta_sublead = eta_lead;
          phi_sublead = phi_lead;

          pt_lead = jpt/1000.;
          eta_lead = jeta;
          phi_lead = jphi;
        }

        else if(jpt/1000.>pt_sublead)
        {
          pt_sublead = jpt/1000.;
          eta_sublead = jeta;
          phi_sublead = jphi;
        }

      }

      jettruthpt[0] = pt_lead;
      jettruthpt[1] = pt_sublead;

      jettrutheta[0] = eta_lead;
      jettrutheta[1] = eta_sublead;

      jettruthphi[0] = phi_lead;
      jettruthphi[1] = phi_sublead;

      // calculate the truth cleaning discriminant    
      // lower cut set to 0 after talk with Jim Lacey
      double truth_cleaning_disc = pt_avg/pt_lead;
      h_pTavg_ptlead->Fill(truth_cleaning_disc, mc_evtweight*puweight*JVTweight);

      if(pt_lead<0 || pt_sublead<0 || truth_cleaning_disc < 0. || truth_cleaning_disc > 1.4 ) continue;
    }

    h_cutflow->Fill("C4:Truth-cleaning", mc_evtweight*puweight*JVTweight);
    
    nevents[4] += mc_evtweight*puweight*JVTweight;
    nevents_now[4]++;
    nevents_puw[4] += puweight;

    // ********************* SELECTION FINISHED **************** //

    // checks trigger firing and filling of the variables 
    nj=0;
    for (int j: {j1,j2}){

      int m_jetpass = 0;
      double ptj = jet_pt->at(j)/1000.;
      // check if trigger associated to pT bin has fired
      if (!check_trigger_pt_bin(ptj)) m_jetpass += 1;

      // check if ptj is out of the defined pT bins
      if(ptj >= conf::pt_lowedges[conf::n_pt]) m_jetpass += 2;  

      // check jet cleaning in xAOD      
      if((*jet_clean)[j]<=0) m_jetpass+=4;

      // Summary
      // m_jetpass == 0, good trigger fired, within range, cleaning ok
      // m_jetpass == 1, NO GOOD TRIGGER FIRED, within range, cleaning ok

      // m_jetpass == 2, good trigger fired, out of range, cleaning ok
      // m_jetpass == 3, NO GOOD TRIGGER FIRED, out of range, cleaning ok

      // m_jetpass > 3, jet do not pass xAOD cleaning 

      jetpt[nj] = ptj;
      jetphi[nj] = (*jet_phi)[j];
      jeteta[nj] = (*jet_eta)[j];
      jetpass[nj] = m_jetpass;

      if(j==j1) isleading[nj] = true;
      else      isleading[nj] = false;

      JVT[nj] = (*jet_JVT)[j];
      clean[nj] = (*jet_clean)[j];
      tightBad[nj] = (*jet_tightBad)[j];
      if(runmc) flavor[nj] = (*jet_truth)[j];
      else flavor[nj] = 0;

      jetHasKShort[nj] = (*jet_hasKShort)[j];
      jetHasLambda[nj] = (*jet_hasLambda)[j];
      jetHasConversion[nj] = (*jet_hasConversion)[j];
      jetHasHadMatInt[nj] = (*jet_hasHadMatInt)[j];
      jetIsDL1_Tagged[nj] = (*jet_IsDL1Tagged)[j];
      jetIsMV2_Tagged[nj] = (*jet_IsMV2Tagged)[j];
      jetIsDL1mu_Tagged[nj] = (*jet_IsDL1muTagged)[j];
      jetIsMV2mu_Tagged[nj] = (*jet_IsMV2muTagged)[j];
      jetIsDL1rnn_Tagged[nj] = (*jet_IsDL1rnnTagged)[j];
      jetIsMV2rnn_Tagged[nj] = (*jet_IsMV2rnnTagged)[j];
      ntrack_IP3DNeg[nj] = (*jet_IP3DNeg_ntrk)[j];
      ntrack_IP3D[nj] = (*jet_IP3D_ntrk)[j];
      ntrack_IP2DNeg[nj] = (*jet_IP2DNeg_ntrk)[j];
      ntrack_IP2D[nj] = (*jet_IP2D_ntrk)[j];
      ntrack_SV1Flip[nj] = (*jet_SV1Flip_ntrk)[j];
      ntrack_SV1[nj] = (*jet_SV1_ntrk)[j];
      ntrack_JetFitterFlip[nj] = (*jet_JetFitterFlip_ntrk)[j];
      ntrack_JetFitter[nj] = (*jet_JetFitter_ntrk)[j];

      // *_subtagger_out defined at the beginning
      for (auto &pair: float_subtagger_out){
	// cout << pair.first << endl;
	pair.second[nj] = (*float_subtagger[pair.first])[j];
      }
      for (auto &pair: int_subtagger_out){
	//cout << pair.first << endl;
	pair.second[nj] = (*int_subtagger[pair.first])[j];
      }
      for (auto &pair: double_subtagger_out){
	pair.second[nj] = (*double_subtagger[pair.first])[j];
      }

      // prescale for data only
      if(runmc) data_evtweight[nj] = 1.0;
      else data_evtweight[nj] = check_triggerPrescales_pt_bin(ptj);

      nj++; 
    }

    // bootstrap weights
    if(m_systematic.EqualTo("FlavourTagging_Nominal"))
    {
      r3->SetSeed(runnum*0.00789+evtnum*0.0063+0.0075);
      for(int ibootstrap=0; ibootstrap<nbootstrap; ibootstrap++) weight_bootstrap[ibootstrap] = r3->PoissonD(1);
    }

    // pu rew and JVT SF uncert
    if(!runmc)
    {
      evtpuw_sys[0] = 1;
      evtpuw_sys[1] = 1;
      evtJVTw_sys[0] = 1;
      evtJVTw_sys[1] = 1;
    }



// Track Jet loop


    int trackj1 = -1, trackj2 = -1; // index of leading and subleading jet
    double trackpt1 = 0, trackpt2 = 0; // pt of leading and subleading jet 
    int ntrackj=0;
    for (int j = 0; j<ntrackjets_event; ++j) {

      double trackpt = (*trackjet_pt)[j]/1000.; // GeV
      double etatrack_abs = fabs((*trackjet_eta)[j]);
      double trackjetntrack = ((*trackjet_ntrk)[j]);
      if (trackpt<trackptcut) continue;
      if (etatrack_abs > etacut) continue;
      if (trackjetntrack < trackntrackcut) continue;

                 

      if (trackpt>trackpt1) {
    	trackj2 = trackj1; trackpt2 = trackpt1; 
    	trackj1 = j; trackpt1 = trackpt; 	
      } 
      else if (trackpt>trackpt2) {
    	trackj2 = j; trackpt2 = trackpt; 
      }
      ntrackj++;
    }
    ntrackjets_event = ntrackj;  // number of reco track jets in the event passing pT/eta/ntrk cut

// sanity check track
   if (trackpt1 < trackpt2) cout << "error " << trackpt1 << "<" << trackpt2 << endl;



      // average pT of the two leading reco jets
      //double trackpt_avg = ((*trackjet_pt)[trackj1]+(*trackjet_pt)[trackj2])/2./1000.;
      //h_trackpTavg->Fill(trackpt_avg, mc_evtweight*puweight); 

        //}

      //}



 // checks trigger firing and filling of the variable
    ntrackj=0;
    for (int j: {trackj1,trackj2}){
      if (trackj1 == -1 || trackj2 == -1){ 
	IsGoodEvent = false;
	continue;}
      int m_trackjetpass = 0;
      double pttrackj = (*trackjet_pt)[j]/1000.;
      // check if trigger associated to pT bin has fired
      if (!check_trigger_pt_bin(pttrackj)) m_trackjetpass += 1;

      // check if pttrackj is out of the defined pT bins
      if(pttrackj >= conf::pt_lowedges[conf::n_pt]) m_trackjetpass += 2;  

       

      // Summary (Just removed cleaning part since there is no cleaning done on track jets.  Is this what needs to be done?
      // m_trackjetpass == 0, good trigger fired, within range, cleaning ok
      // m_trackjetpass == 1, NO GOOD TRIGGER FIRED, within range

      // m_trackjetpass == 2, good trigger fired, out of range
      // m_trackjetpass == 3, NO GOOD TRIGGER FIRED, out of range

      

      trackjetpt[ntrackj] = pttrackj;
      trackjetphi[ntrackj] = (*trackjet_phi)[j];
      trackjeteta[ntrackj] = (*trackjet_eta)[j];
      trackjetpass[ntrackj] = m_trackjetpass;
      trackjetntrack[ntrackj] = (*trackjet_ntrk)[j];
      if(j==trackj1) trackisleading[ntrackj] = true;
      else      trackisleading[ntrackj] = false;

      if(runmc) trackflavor[ntrackj] = (*trackjet_truth)[j];
      else trackflavor[ntrackj] = 0;

      trackjetHasKShort[ntrackj] = (*trackjet_hasKShort)[j];
      trackjetHasLambda[ntrackj] = (*trackjet_hasLambda)[j];
      trackjetHasConversion[ntrackj] = (*trackjet_hasConversion)[j];
      trackjetHasHadMatInt[ntrackj] = (*trackjet_hasHadMatInt)[j];
      trackjetIsDL1_Tagged[nj] = (*trackjet_IsDL1Tagged)[j];
      trackjetIsMV2_Tagged[nj] = (*trackjet_IsMV2Tagged)[j];
      trackjetIsDL1mu_Tagged[nj] = (*trackjet_IsDL1muTagged)[j];
      trackjetIsMV2mu_Tagged[nj] = (*trackjet_IsMV2muTagged)[j];
      trackjetIsDL1rnn_Tagged[nj] = (*trackjet_IsDL1rnnTagged)[j];
      trackjetIsMV2rnn_Tagged[nj] = (*trackjet_IsMV2rnnTagged)[j];
      trackntrack_IP3DNeg[ntrackj] = (*trackjet_IP3DNeg_ntrk)[j];
      trackntrack_IP3D[ntrackj] = (*trackjet_IP3D_ntrk)[j];
      trackntrack_IP2DNeg[ntrackj] = (*trackjet_IP2DNeg_ntrk)[j];
      trackntrack_IP2D[ntrackj] = (*trackjet_IP2D_ntrk)[j];
      trackntrack_SV1Flip[ntrackj] = (*trackjet_SV1Flip_ntrk)[j];
      trackntrack_SV1[ntrackj] = (*trackjet_SV1_ntrk)[j];
      trackntrack_JetFitterFlip[ntrackj] = (*trackjet_JetFitterFlip_ntrk)[j];
      trackntrack_JetFitter[ntrackj] = (*trackjet_JetFitter_ntrk)[j];

      // *_subtagger_out defined at the beginning
      for (auto &pair: float_subtagger_trackjet_out){
	// cout << pair.first << endl;
	pair.second[ntrackj] = (*float_subtagger_trackjet[pair.first])[j];
      }
      for (auto &pair: int_subtagger_trackjet_out){
	//cout << pair.first << endl;
	pair.second[ntrackj] = (*int_subtagger_trackjet[pair.first])[j];
      }
      for (auto &pair: double_subtagger_trackjet_out){
	pair.second[ntrackj] = (*double_subtagger_trackjet[pair.first])[j];
      }


      ntrackj++; 
    }
	if (IsGoodEvent) {
    outtree->Fill();}
  }

  for(int icut=0; icut<ncuts; icut++){
    cout<<icut<<": no w "<<nevents_now[icut]<<", pu w "<<nevents_puw[icut]<< " puw+mcw " <<nevents[icut]<<endl;

  }

}


bool NtupleDumper::check_trigger_pt_bin(double jetpt){

  // determine the pT bin
  int pt_bin = -1;
  for (int i = 0; i< conf::n_pt; ++i){
    if (jetpt < conf::pt_lowedges[i+1]){
      pt_bin = i;
      break;
    }
  }

  // check if not in reweighting range
  if (pt_bin == -1) return true;

  // check if trigger has fired
  // cout << "bin: " << pt_bin << " assoc. trigger: " << conf::bin_trigger[pt_bin] << " decision: " << *trigger_decision[conf::bin_trigger[pt_bin]] << endl;
  return *trigger_decision[conf::bin_trigger[pt_bin]];
}


float NtupleDumper::check_triggerPrescales_pt_bin(double jetpt){

  // determine the pT bin
  int pt_bin = -1;
  for (int i = 0; i< conf::n_pt; ++i){
    if (jetpt < conf::pt_lowedges[i+1]){
      pt_bin = i;
      break;
    }
  }

  // check if not in reweighting range
  if (pt_bin == -1) return -999;

  // return trigger prescales
  return *trigger_prescales[("eve_" + conf::bin_trigger[pt_bin] + "_ps")];
}

