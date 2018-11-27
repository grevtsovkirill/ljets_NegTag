//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 11 09:59:23 2012 by ROOT version 5.30/02
// from TTree atree/atree
// found on file: mc/user.yuj.0509.mc12_8TeV.147912.Pythia8_AU2CT10_jetjet_JZ2W.merge.NTUP_BTAGSLIM.e1126_s1469_s1470_r3542_r3549_p1075.120610143808/user.yuj.008103._00002.output.root
//////////////////////////////////////////////////////////

#ifndef NtupleDumper_h
#define NtupleDumper_h

#include <TROOT.h>
#include <TChain.h>
#include <TH1D.h>
#include <TFile.h>
#include <iostream>
#include <vector>
#include <config_reader.hpp>
#include "subTagger.hpp"

class NtupleDumper {
public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  TString m_systematic;
  std::string m_period_slice;
  double m_sum_of_weights;
  double m_total_events;
  double m_events_sample;
  bool runmc;
  
  //---------------Declaration of leaf types---------------//
  // calo jets
  std::vector<float> *jet_pt;
  std::vector<float> *jet_phi;
  std::vector<float> *jet_eta;
  std::vector<float> *jet_jvt;
  //std::vector<int>   *jet_clean;
  //std::vector<int>   *jet_tightBad;
  std::vector<int>   *jet_truthflav;

  /* std::vector<int>   *jet_hasKShort; */
  /* std::vector<int>   *jet_hasLambda; */
  /* std::vector<int>   *jet_hasConversion; */
  /* std::vector<int>   *jet_hasHadMatInt; */

  /*
  // calo jets - track multiplicity studies
  std::vector<int>   *jet_IP3DNeg_ntrk; // jet_IP3DNeg_nTracks (IP3DNeg input)
  std::vector<int>   *jet_IP3D_ntrk; // jet_IP3D_nTracks (IP3D input)
  std::vector<int>   *jet_IP2DNeg_ntrk; // jet_IP2DNeg_nTracks (IP2DNeg input)
  std::vector<int>   *jet_IP2D_ntrk; // jet_IP2D_nTracks (IP2D input)
  std::vector<int>   *jet_SV1Flip_ntrk; // jet_SV1Flip_nTracks (SV1Flip input)
  std::vector<int>   *jet_SV1_ntrk; // jet_SV1_nTracks (SV1 input)
  std::vector<int>   *jet_JetFitterFlip_ntrk; // jet_JetFitterFlip_nTracks (JetFitterFlip input)
  std::vector<int>   *jet_JetFitter_ntrk; // jet_JetFitter_nTracks (JetFitter input)
  //*/


  // truth jets
  /*
  std::vector<float> *truthjet_pt;
  std::vector<float> *truthjet_phi;
  std::vector<float> *truthjet_eta;
  std::vector<int> *truthjet_pdgId;
  //*/

  // additional subtagger information
  // see ../conf/subTagger.hpp
  std::map <std::string, std::vector<float>*> float_subtagger;
  std::map <std::string, std::vector<int>*> int_subtagger;
  std::map <std::string, std::vector<double>*> double_subtagger;

  std::map<std::string, float*> float_subtagger_out;
  std::map<std::string, int*> int_subtagger_out;
  std::map<std::string, double*> double_subtagger_out;

  // trigger and map the trigger decision branches to strings to have easier access
  int *evt_HLT_j15, *evt_HLT_j25, *evt_HLT_j60, *evt_HLT_j110, *evt_HLT_j175, *evt_HLT_j380, *evt_HLT_j400, *evt_HLT_j420; // output tree variables
  // + prescales
  float *evt_HLT_j15_ps, *evt_HLT_j25_ps, *evt_HLT_j60_ps, *evt_HLT_j110_ps, *evt_HLT_j175_ps, *evt_HLT_j380_ps, *evt_HLT_j400_ps, *evt_HLT_j420_ps;

  std::map <std::string, int*> trigger_decision = {
    {"HLT_j15", evt_HLT_j15},
    {"HLT_j25", evt_HLT_j25},
    {"HLT_j60", evt_HLT_j60},
    {"HLT_j110", evt_HLT_j110},
    {"HLT_j175", evt_HLT_j175},
    //{"HLT_j380", evt_HLT_j380},
    //{"HLT_j400", evt_HLT_j400},
    {"HLT_j420", evt_HLT_j420}
  };

  std::map <std::string, float*> trigger_prescales = {
    {"eve_HLT_j15_ps", evt_HLT_j15_ps},
    {"eve_HLT_j25_ps", evt_HLT_j25_ps},
    {"eve_HLT_j60_ps", evt_HLT_j60_ps},
    {"eve_HLT_j110_ps", evt_HLT_j110_ps},
    {"eve_HLT_j175_ps", evt_HLT_j175_ps},
    //{"eve_HLT_j380_ps", evt_HLT_j380_ps},
    //{"eve_HLT_j400_ps", evt_HLT_j400_ps},
    {"eve_HLT_j420_ps", evt_HLT_j420_ps}
  };

  // ad hoc vector to get sorted trigger plot...
  //std::vector <std::string> trigger_names = {"HLT_j15", "HLT_j25", "HLT_j60", "HLT_j110", "HLT_j175", "HLT_j380", "HLT_j400", "HLT_j420"};
  std::vector <std::string> trigger_names = {"HLT_j15", "HLT_j25", "HLT_j60", "HLT_j110", "HLT_j175","HLT_j420"};

  // event-wide
  int     evtnum; // eve_num (= eve_mc_num for MC)
  int     runnum; // eve_run
  float   evtpuw; // eve_pu_w
  float   evtpuw_sys[2]; // eve_pu_w_up/down
  int     evt_hasPV; // eve_hasPV
  float   evt_averageInteractionsPerCrossing; // eve_averageInteractionsPerCrossing;

  // MC only
  float   evtJVTw; // eve_JvtSF
  float   evtJVTw_sys[2]; // eve_JvtSF_up/down
  int     mc_evtnum; // eve_mc_num
  int     mc_channum; // eve_mc_chan
  float   mc_evtweight; // eve_mc_w



  //-------------Output tree variables---------------//
  // TO BE INITIALIZED TO 0
  // selected jets
  float jetpt[2];
  float jetphi[2];
  float jeteta[2];
  int   jetpass[2];
  bool  isleading[2];
  float JVT[2];
  int   clean[2];
  int   tightBad[2];
  int   flavor[2];
  int   jetHasKShort[2];
  int   jetHasLambda[2];
  int   jetHasConversion[2];
  int   jetHasHadMatInt[2];
  int   ntrack_IP3DNeg[2];
  int   ntrack_IP3D[2];
  int   ntrack_IP2DNeg[2];
  int   ntrack_IP2D[2];
  int   ntrack_SV1Flip[2];
  int   ntrack_SV1[2];
  int   ntrack_JetFitterFlip[2];
  int   ntrack_JetFitter[2];
  float data_evtweight[2]; 

  // leading truth jets
  float jettruthpt[2];
  float jettrutheta[2];
  float jettruthphi[2];
  int jettruthpdgId[2];

  // extra
  int    njets_event;
  int    njets;
  int    nbootstrap;
  double weight_bootstrap[1000]; //[nbootstrap]

  NtupleDumper(TTree *tree=0, TString="nominal", bool runmc=true);
  virtual ~NtupleDumper();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  //virtual TTree*     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);

  bool check_trigger_pt_bin(double);
  float check_triggerPrescales_pt_bin(double);
};

#endif

#ifdef NtupleDumper_cxx
NtupleDumper::NtupleDumper(TTree *tree, TString syst, bool tmp_runmc)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  if (tree == 0) {
    std::cout << "trying to access tree" << std::endl;
  }
   m_systematic = syst;
   runmc = tmp_runmc;
   Init(tree);
   std::cout << "accessed tree - " << m_systematic << std::endl;
}

NtupleDumper::~NtupleDumper()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NtupleDumper::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t NtupleDumper::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void NtupleDumper::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
   if (runmc) std::cout << "running mc" << std::endl;
   else std::cout << "running data" << std::endl;

  // calo jets  
  jet_pt = new std::vector<float>;
  jet_phi = new std::vector<float>;
  jet_eta = new std::vector<float>;
  jet_jvt = new std::vector<float>;
  jet_truthflav = new std::vector<int>;

  /* jet_hasKShort = new std::vector<int>; */
  /* jet_hasLambda = new std::vector<int>; */
  /* jet_hasConversion = new std::vector<int>; */
  /* jet_hasHadMatInt = new std::vector<int>; */

  // calo jets - track multiplicity studies
  /*
  jet_IP3DNeg_ntrk = new std::vector<int>; // jet_IP3DNeg_nTracks (IP3DNeg input)
  jet_IP3D_ntrk = new std::vector<int>; // jet_IP3D_nTracks (IP3D input)
  jet_IP2DNeg_ntrk = new std::vector<int>; // jet_IP2DNeg_nTracks (IP2DNeg input)
  jet_IP2D_ntrk = new std::vector<int>; // jet_IP2D_nTracks (IP2D input)
  jet_SV1Flip_ntrk = new std::vector<int>; // jet_SV1Flip_nTracks (SV1Flip input)
  jet_SV1_ntrk = new std::vector<int>; // jet_SV1_nTracks (SV1 input)
  jet_JetFitterFlip_ntrk = new std::vector<int>; // jet_JetFitterFlip_nTracks (JetFitterFlip input)
  jet_JetFitter_ntrk = new std::vector<int>; // jet_JetFitter_nTracks (JetFitter input)
  //*/

  // truth jets
  /*
  truthjet_pt = new std::vector<float>;
  truthjet_phi = new std::vector<float>;
  truthjet_eta = new std::vector<float>;
  truthjet_pdgId = new std::vector<int>;
  //*/

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   // calo jets / truth jets
   /*
   if(runmc){
     fChain->SetBranchAddress("truthjet_pt", &truthjet_pt);
     fChain->SetBranchAddress("truthjet_eta", &truthjet_eta);
     fChain->SetBranchAddress("truthjet_phi", &truthjet_phi);
     fChain->SetBranchAddress("truthjet_pdgId", &truthjet_pdgId);
   }
   //*/
   fChain->SetBranchAddress("jet_pt", &jet_pt);
   fChain->SetBranchAddress("jet_phi", &jet_phi);
   fChain->SetBranchAddress("jet_eta", &jet_eta);
   fChain->SetBranchAddress("jet_jvt", &jet_jvt);
   /*
   fChain->SetBranchAddress("jet_IP3DNeg_ntrk", &jet_IP3DNeg_ntrk);
   fChain->SetBranchAddress("jet_IP3D_ntrk", &jet_IP3D_ntrk);
   fChain->SetBranchAddress("jet_IP2DNeg_ntrk", &jet_IP2DNeg_ntrk);
   fChain->SetBranchAddress("jet_IP2D_ntrk", &jet_IP2D_ntrk);
   fChain->SetBranchAddress("jet_SV1Flip_ntrk", &jet_SV1Flip_ntrk);
   fChain->SetBranchAddress("jet_SV1_ntrk", &jet_SV1_ntrk);
   fChain->SetBranchAddress("jet_JetFitter_ntrk", &jet_JetFitter_ntrk);
   fChain->SetBranchAddress("jet_JetFitterFlip_ntrk", &jet_JetFitterFlip_ntrk);
   //*/
   fChain->SetBranchAddress("jet_truthflav", &jet_truthflav);

   /* fChain->SetBranchAddress("jet_hasKShort", &jet_hasKShort); */
   /* fChain->SetBranchAddress("jet_hasLambda", &jet_hasLambda); */
   /* fChain->SetBranchAddress("jet_hasConversion", &jet_hasConversion); */
   /* fChain->SetBranchAddress("jet_hasHadMatInt", &jet_hasHadMatInt); */

   for (auto &pair: trigger_decision){
     pair.second = new int;
     fChain->SetBranchAddress(pair.first.c_str(), pair.second);
   }
 
   for (auto &pair: trigger_prescales){
     pair.second = new float;
     fChain->SetBranchAddress(pair.first.c_str(), pair.second);
   }
   
   for (const auto &name: subtagger::floats){
     float_subtagger[name.first] = new std::vector<float>;
     fChain->SetBranchAddress(name.first.c_str(), &float_subtagger[name.first]);
   }
   for (const auto &name: subtagger::ints){
     int_subtagger[name.first] = new std::vector<int>;
     fChain->SetBranchAddress(name.first.c_str(), &int_subtagger[name.first]);
   }
   for (const auto &name: subtagger::doubles){
     double_subtagger[name.first] = new std::vector<double>;
     fChain->SetBranchAddress(name.first.c_str(), &double_subtagger[name.first]);
   }

   // event wide
   // MC only
   fChain->SetBranchAddress("weight_jvt", &evtJVTw);
   fChain->SetBranchAddress("weight_jvt_UP", &evtJVTw_sys[0]);
   fChain->SetBranchAddress("weight_jvt_DOWN", &evtJVTw_sys[1]);
   fChain->SetBranchAddress("eventNumber", &mc_evtnum);
   fChain->SetBranchAddress("mcChannelNumber", &mc_channum);
   fChain->SetBranchAddress("weight_mc", &mc_evtweight);
   fChain->SetBranchAddress("weight_pileup", &evtpuw);
   fChain->SetBranchAddress("weight_pileup_UP", &evtpuw_sys[0]);
   fChain->SetBranchAddress("weight_pileup_DOWN", &evtpuw_sys[1]);
   // All  
   fChain->SetBranchAddress("eventNumber", &evtnum);
   fChain->SetBranchAddress("runNumber", &runnum);
   //fChain->SetBranchAddress("eve_hasPV", &evt_hasPV);
   fChain->SetBranchAddress("mu", &evt_averageInteractionsPerCrossing);

   // Output tree variables to be initialized to 0
   jetpt[0] = -99.;
   jetpt[1] = -99.;

   jetphi[0] = -99;
   jetphi[1] = -99;

   jeteta[0] = -99;
   jeteta[1] = -99;

   jetpass[0] = -99;
   jetpass[1] = -99;

   isleading[0] = -99;
   isleading[1] = -99;

   JVT[0] = -99;
   JVT[1] = -99;

   clean[0] = -99;
   clean[1] = -99;

   tightBad[0] = -99;
   tightBad[1] = -99;

   flavor[0] = -99;
   flavor[1] = -99;

   jetHasKShort[0] = -99;
   jetHasKShort[1] = -99;

   jetHasLambda[0] = -99;
   jetHasLambda[1] = -99;

   jetHasConversion[0] = -99;
   jetHasConversion[1] = -99;

   jetHasHadMatInt[0] = -99;
   jetHasHadMatInt[1] = -99;

   ntrack_IP3DNeg[0] = -99;
   ntrack_IP3DNeg[1] = -99;

   ntrack_IP3D[0] = -99;
   ntrack_IP3D[1] = -99;

   ntrack_IP2DNeg[0] = -99;
   ntrack_IP2DNeg[1] = -99;

   ntrack_IP2D[0] = -99;
   ntrack_IP2D[1] = -99;

   ntrack_SV1Flip[0] = -99;
   ntrack_SV1Flip[1] = -99;

   ntrack_SV1[0] = -99;
   ntrack_SV1[1] = -99;

   ntrack_JetFitterFlip[0] = -99;
   ntrack_JetFitterFlip[1] = -99;

   ntrack_JetFitter[0] = -99;
   ntrack_JetFitter[1] = -99;

   data_evtweight[0] = -99;
   data_evtweight[1] = -99;
   /*
   jettruthpt[0] = -99;
   jettruthpt[1] = -99;

   jettrutheta[0] = -99;
   jettrutheta[1] = -99;
  
   jettruthphi[0] = -99;
   jettruthphi[1] = -99;
   
   jettruthpdgId[0] = -99;
   jettruthpdgId[1] = -99;
   //*/   
   njets_event = -99;
   njets = 2; // 2 jets selected
   nbootstrap = -99;
   for(int ibootstrap=0; ibootstrap<1000; ibootstrap++) weight_bootstrap[ibootstrap] = -99;

   Notify();
}

Bool_t NtupleDumper::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void NtupleDumper::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t NtupleDumper::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef NtupleDumper_cxx
