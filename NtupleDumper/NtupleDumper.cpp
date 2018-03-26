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

  TH1F *h_trackpTavg = new TH1F("trackptavg_"+m_systematic, "p_{T}^{avg}", 300, 0, 3000);

  // for trigger check 


  TH2F *h2_tracktriggers = new TH2F("tracktriggers_"+m_systematic, "triggers firing in track pT bins; lead trackjet p_{T}", 
			       conf::n_pt, conf::pt_lowedges, 
			       25, 0, 25);
  TH2F *h2_total_trackevents = new TH2F("tot_track_evts_"+m_systematic, "", 
			       conf::n_pt, conf::pt_lowedges, 
			       25, 0, 25);


  ntrackjets = 1;  // select only leading track jet

  // create tree
  cout << "creating tree" << endl;
  TTree *outtree = new TTree(m_systematic, "recreate");


  outtree->Branch("ntrackjets",&ntrackjets,"ntrackjets/I");
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
  outtree->Branch("evtpuw_sys",evtpuw_sys,"evtpuw_sys[ntrackjets]/F"); // njets used for commodities but up/down
  outtree->Branch("evtJVTw",&evtJVTw,"evtJVTw/F");
  outtree->Branch("evtJVTw_sys",evtJVTw_sys,"evtJVTw_sys[ntrackjets]/F");
  outtree->Branch("mc_evtnum",&mc_evtnum,"mc_evtnum/I");
  outtree->Branch("mc_channum",&mc_channum,"mc_channum/I");
  outtree->Branch("mc_evtweight",&mc_evtweight,"mc_evtweight/F");
  outtree->Branch("evt_averageInteractionsPerCrossing",&evt_averageInteractionsPerCrossing,"evt_averageInteractionsPerCrossing/F");


  outtree->Branch("trackjetpt", trackjetpt, "trackjetpt[ntrackjets]/F");
  outtree->Branch("trackjetphi",trackjetphi,"trackjetphi[ntrackjets]/F");
  outtree->Branch("trackjeteta",trackjeteta,"trackjeteta[ntrackjets]/F");
  outtree->Branch("trackjetntrk",trackjetntrk,"trackjetntrk[ntrackjets]/I");
  outtree->Branch("trackflavor",trackflavor,"trackflavor[ntrackjets]/I");
  outtree->Branch("trackjetHasKShort",trackjetHasKShort, "trackjetHasKShort[ntrackjets]/I");
  outtree->Branch("trackjetHasLambda", trackjetHasLambda, "trackjetHasLambda[ntrackjets]/I");
  outtree->Branch("trackjetHasConversion", trackjetHasConversion, "trackjetHasConversion[ntrackjets]/I");
  outtree->Branch("trackjetHasHadMatInt", trackjetHasHadMatInt, "trackjetHasHadMatInt[ntrackjets]/I");
  outtree->Branch("trackjetIsDL1_Tagged", trackjetIsDL1_Tagged, "trackjetIsDL1_Tagged[ntrackjets]/I");
  outtree->Branch("trackjetIsDL1Flip_Tagged", trackjetIsDL1Flip_Tagged, "tracketIsDL1Flip_Tagged[ntrackjets]/I");
  outtree->Branch("trackjetIsMV2_Tagged", trackjetIsMV2_Tagged, "trackjetIsMV2_Tagged[ntrackjets]/I");
  outtree->Branch("trackjetIsMV2Flip_Tagged", trackjetIsMV2Flip_Tagged, "tracketIsMV2Flip_Tagged[ntrackjets]/I");
//  outtree->Branch("trackjetIsDL1mu_Tagged", trackjetIsDL1mu_Tagged, "trackjetIsDL1mu_Tagged/I");
//  outtree->Branch("trackjetIsDL1Flipmu_Tagged", trackjetIsDL1Flipmu_Tagged, "trackjetIsDL1Flipmu_Tagged/I");
//  outtree->Branch("trackjetIsDL1rnn_Tagged", trackjetIsDL1rnn_Tagged, "trackjetIsDL1rnn_Tagged/I");
//  outtree->Branch("trackjetIsDL1Fliprnn_Tagged", trackjetIsDL1Fliprnn_Tagged, "trackjetIsDL1Fliprnn_Tagged/I");
  outtree->Branch("trackntrack_IP3DNeg",trackntrack_IP3DNeg,"trackntrack_IP3DNeg[ntrackjets]/I");
  outtree->Branch("trackntrack_IP3D",trackntrack_IP3D,"trackntrack_IP3D[ntrackjets]/I");
  outtree->Branch("trackntrack_IP2DNeg",trackntrack_IP2DNeg,"trackntrack_IP2DNeg[ntrackjets]/I");
  outtree->Branch("trackntrack_IP2D",trackntrack_IP2D,"trackntrack_IP2D[ntrackjets]/I");
  outtree->Branch("trackntrack_SV1Flip",trackntrack_SV1Flip,"trackntrack_SV1Flip[ntrackjets]/I");
  outtree->Branch("trackntrack_SV1",trackntrack_SV1,"trackntrack_SV1[ntrackjets]/I");
  outtree->Branch("trackntrack_JetFitterFlip",trackntrack_JetFitterFlip,"trackntrack_JetFitterFlip[ntrackjets]/I");
  outtree->Branch("trackntrack_JetFitter",trackntrack_JetFitter,"trackntrack_JetFitter[ntrackjets]/I");
  outtree->Branch("trackdata_evtweight", trackdata_evtweight, "trackdata_evtweight[ntrackjets]/F");




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


  // cutvalues
  const double trackptcut = 10.;
  const double tracketacut = 2.5;
  const double trackntrackcut = 2;
  const double ptcut = 20.;
  const double etacut = 2.5;

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
     ntrackjets_event = trackjet_pt->size();
     njets_event = jet_pt->size();
	
    if ( ntrackjets_event < 1 || njets_event < 0) {
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

    double pttrack1_noCut = 0.;

    for (int i = 0; i< ntrackjets_event; ++i){
      if ((*trackjet_pt)[i]/1000.>pttrack1_noCut) pttrack1_noCut = (*trackjet_pt)[i]/1000.; // GeV
    }

    // fill histogram with trigger decisions - BEFORE ANY CUT APPLIED 
    for (auto trigger: trigger_names){
      h2_tracktriggers->Fill(pttrack1_noCut, trigger.c_str(), puweight*mc_evtweight * *     trigger_decision[trigger]);
      h2_total_trackevents->Fill(pttrack1_noCut, trigger.c_str(), mc_evtweight*puweight);
    }




    // NPV requirement - cut already on Ntuples, should not remove events


    // event cleaning has been applied in ntuple production

    // select two leading jets and verify that they are back-to-back
    // count nb of reco jet > 20 GeV
    int j1 = -1, j2 = -1; // index of leading and subleading jet
    double pt1 = 0, pt2 = 0; // pt of leading and subleading jet 
    int nj=0;

    for (int j = 0; j<ntrackjets_event; ++j) {

      double pt = (*trackjet_pt)[j]/1000.; // GeV
      double eta_abs = fabs((*trackjet_eta)[j]);
      int ntracks = (*trackjet_ntrk)[j];
      if (pt<trackptcut) continue;
      if (eta_abs > tracketacut) continue;
      if (ntracks < trackntrackcut) continue;
        

      if (pt>pt1) {
    	j2 = j1; pt2 = pt1; 
    	j1 = j; pt1 = pt; 	

      } 
      else if (pt>pt2) {
    	j2 = j; pt2 = pt; 
      }
      nj++;
    }


    ntrackjets_event = nj; // number of reco jets in the event passing pT/eta/JVT cut
    // sanity check    
    if (pt1 < pt2) cout << "error " << pt1 << "<" << pt2 << endl;
     // Make sure there is at least one track jet passing cuts.
    if (j1<0){
	IsGoodEvent = false;
	 continue;}
      


    // Find leading calo jet index
    int caloj1 = -1, caloj2 = -1; // index of leading and subleading jet
    double calopt1 = 0, calopt2 = 0; // pt of leading and subleading jet 
    int ncaloj=0;

    for (int j = 0; j<njets_event; ++j) {

      double calopt = (*jet_pt)[j]/1000.; // GeV
      double caloeta_abs = fabs((*jet_eta)[j]);

      if (calopt<ptcut) continue;
      if (caloeta_abs > etacut) continue;
      //https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#MV2c20_tagger
      if ( calopt < 60. && caloeta_abs < 2.4 && (*jet_JVT)[j] < 0.59) continue;

      if (calopt>calopt1) {
        caloj2 = caloj1; calopt2 = calopt1;
        caloj1 = j; calopt1 = calopt;
      }
      else if (calopt>calopt2) {
        caloj2 = j; calopt2 = calopt;
      }
      ncaloj++;
    }
	if ((caloj1 < 0) || (caloj2 < 1)){
	IsGoodEvent=false;
	continue;
	}
    // MC truth cleaning
    jettruthpt[0] = -999;
    jettruthpt[1] = -999;
    jettrutheta[0] = -999;
    jettrutheta[1] = -999;
    jettruthphi[0] = -999;
    jettruthphi[1] = -999;
    jettruthpdgId[0] = -999;
    jettruthpdgId[1] = -999;

    if (runmc){
      double pt_avg = ((*jet_pt)[caloj1]+(*jet_pt)[caloj2])/2./1000.;
      double pt_lead = -999;
      double pt_sublead = -999;
      double eta_lead = -999;
      double eta_sublead = -999;
      double phi_lead = -999;
      double phi_sublead = -999;
      int pdgId_lead = -999;
      int pdgId_sublead = -999;


      for (int j = 0; j<truthjet_pt->size(); j++)
      {
        float jpt = (*truthjet_pt)[j];
        float jeta = (*truthjet_eta)[j];
        float jphi = (*truthjet_phi)[j];
        int jpdgId = (*truthjet_pdgId)[j];

        if (jpt/1000.>pt_lead)
        {
          pt_sublead = pt_lead;
          eta_sublead = eta_lead;
          phi_sublead = phi_lead;
          pdgId_sublead = pdgId_lead;

          pt_lead = jpt/1000.;
          eta_lead = jeta;
          phi_lead = jphi;
          pdgId_lead = jpdgId;
        }

        else if(jpt/1000.>pt_sublead)
        {
          pt_sublead = jpt/1000.;
          eta_sublead = jeta;
          phi_sublead = jphi;
          pdgId_sublead = jpdgId;
        }

      }

      jettruthpt[0] = pt_lead;
      jettruthpt[1] = pt_sublead;

      jettrutheta[0] = eta_lead;
      jettrutheta[1] = eta_sublead;

      jettruthphi[0] = phi_lead;
      jettruthphi[1] = phi_sublead;

      jettruthpdgId[0] = pdgId_lead;
      jettruthpdgId[1] = pdgId_sublead;

      double truth_cleaning_disc = pt_avg/pt_lead;

      if(pt_lead<0 || pt_sublead<0 || truth_cleaning_disc < 0. || truth_cleaning_disc > 1.4 ){
	IsGoodEvent=false;			
	 continue;
    }
	}

    double dphi = fabs((*jet_phi)[caloj1] - (*trackjet_phi)[j1]);
    if (dphi>M_PI) dphi = 2*M_PI - dphi;
    if (dphi<2.){
	IsGoodEvent=false;
	 continue;
	}


    // ********************* SELECTION FINISHED **************** //

    // checks trigger firing and filling of the variables 
    nj=0;
    for (int j : {j1}){

      //int m_jetpass = 0;
      //double ptj = jet_pt->at(0)/1000.;
      //// check if trigger associated to pT bin has fired
      //if (!check_trigger_pt_bin(ptj)) m_jetpass += 1;

      //// check if ptj is out of the defined pT bins
      //if(ptj >= conf::pt_lowedges[conf::n_pt]) m_jetpass += 2;

      //// check jet cleaning in xAOD      
      //if((*jet_clean)[0]<=0) m_jetpass+=4;

      // Summary
      // m_jetpass == 0, good trigger fired, within range, cleaning ok
      // m_jetpass == 1, NO GOOD TRIGGER FIRED, within range, cleaning ok

      // m_jetpass == 2, good trigger fired, out of range, cleaning ok
      // m_jetpass == 3, NO GOOD TRIGGER FIRED, out of range, cleaning ok

      // m_jetpass > 3, jet do not pass xAOD cleaning 

      //cout << "Jetpass = " << m_jetpass << endl; 
      //if (m_jetpass > 0) {
      //  IsGoodEvent = false;
      //  continue;
      //}
      double ptj = jet_pt->at(0)/1000.;
      double trackptj = trackjet_pt->at(j)/1000.;

      trackjetpt[nj] = trackptj;
      trackjetphi[nj] = (*trackjet_phi)[j];
      trackjeteta[nj] = (*trackjet_eta)[j];
      trackjetntrk[nj] = (*trackjet_ntrk)[j];


      if(runmc) trackflavor[nj] = (*trackjet_truth)[j];
      else trackflavor[nj] = 0;

      trackjetHasKShort[nj] = (*trackjet_hasKShort)[j];
      trackjetHasLambda[nj] = (*trackjet_hasLambda)[j];
      trackjetHasConversion[nj] = (*trackjet_hasConversion)[j];
      trackjetHasHadMatInt[nj] = (*trackjet_hasHadMatInt)[j];
      trackjetIsDL1_Tagged[nj] = (*trackjet_isDL1_Tagged)[j];
      trackjetIsDL1Flip_Tagged[nj] = (*trackjet_isDL1Flip_Tagged)[j];
      trackjetIsMV2_Tagged[nj] = (*trackjet_isMV2_Tagged)[j];
      trackjetIsMV2Flip_Tagged[nj] = (*trackjet_isMV2Flip_Tagged)[j];

      //trackjetIsDL1mu_Tagged[nj] = (*trackjet_IsDL1muTagged)[j];
      //trackjetIsDL1Flipmu_Tagged[nj] = (*trackjet_IsDL1FlipmuTagged)[j];
      //trackjetIsDL1rnn_Tagged[nj] = (*trackjet_IsDL1rnnTagged)[j];
      //trackjetIsDL1Fliprnn_Tagged[nj] = (*trackjet_IsDL1FliprnnTagged)[j];
      trackntrack_IP3DNeg[nj] = (*trackjet_IP3DNeg_ntrk)[j];
      trackntrack_IP3D[nj] = (*trackjet_IP3D_ntrk)[j];
      trackntrack_IP2DNeg[nj] = (*trackjet_IP2DNeg_ntrk)[j];
      trackntrack_IP2D[nj] = (*trackjet_IP2D_ntrk)[j];
      trackntrack_SV1Flip[nj] = (*trackjet_SV1Flip_ntrk)[j];
      trackntrack_SV1[nj] = (*trackjet_SV1_ntrk)[j];
      trackntrack_JetFitterFlip[nj] = (*trackjet_JetFitterFlip_ntrk)[j];
      trackntrack_JetFitter[nj] = (*trackjet_JetFitter_ntrk)[j];

      // *_subtagger_out defined at the beginning
      for (auto &pair: float_subtagger_trackjet_out){
  // cout << pair.first << endl;
  pair.second[nj] = (*float_subtagger_trackjet[pair.first])[j];
      }
      for (auto &pair: int_subtagger_trackjet_out){
  //cout << pair.first << endl;
  pair.second[nj]= (*int_subtagger_trackjet[pair.first])[j];
      }
      for (auto &pair: double_subtagger_trackjet_out){
  pair.second[nj] = (*double_subtagger_trackjet[pair.first])[j];
      }

      // prescale for data only
      if(runmc) trackdata_evtweight[nj] = 1.0;
      else trackdata_evtweight[nj] = check_triggerPrescales_pt_bin(ptj);

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
      evtJVTw_sys[0] = 1;
    }



// Track Jet loop


	if (IsGoodEvent) {
    outtree->Fill();}
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

