//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Dec 18 20:59:35 2012 by ROOT version 5.30/01
// from TTree d/recreate
// found on file: roo/std/a1_j1.root
//////////////////////////////////////////////////////////

#ifndef NtupleReader_h
#define NtupleReader_h


#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TClass.h>

#include "../config/subTagger.hpp"

class NtupleReader {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   TString         m_systematic;
   TString         m_mode;
   TString         m_reweighting_folder;
   TString         m_compaigne;
   TDirectory     *dir_subtagger;
   TDirectory     *dir_subsample;
   TDirectory     *dir_subflavour;
   TDirectory     *dir_bootstrap;


   // Declaration of leaf types

   int           nbootstrap;
   double        weight_bootstrap[1000]; //[nbootstrap]
   float         mc_evtweight;
   float         evtpuw;
   float         evtpuw_sys[2];
   float         evtJVTw;
   float         evtJVTw_sys[2];
   int           njets;
   int           njets_event;
   float         jetpt[2];   //[njets]
   float         jeteta[2];   //[njets]
   float         jetphi[2];   //[njets]
   int           flavor[2];   //[njets]
   int           jetpass[2];   //[njets]
   bool          isleading[2];   //[njets]

   int           ntrack_IP3DNeg[2];
   int           ntrack_IP3D[2];
   int           ntrack_IP2DNeg[2];
   int           ntrack_IP2D[2];
   int           ntrack_SV1Flip[2];
   int           ntrack_SV1[2];
   int           ntrack_JetFitterFlip[2];
   int           ntrack_JetFitter[2];

   float         data_evtweight[2];   //[njets]

   int           jetHasKShort[2];
   int           jetHasLambda[2];
   int           jetHasConversion[2];
   int           jetHasHadMatInt[2];

   // additional subtagger information
   std::map <std::string, float[2]> float_subtagger;
   std::map <std::string, int[2]> int_subtagger;
   std::map <std::string, double[2]> double_subtagger;

   // List of branches
   TBranch        *b_nbootstrap;   //!
   TBranch        *b_weight_bootstrap;   //!
   TBranch        *b_mc_evtweight;   //!
   TBranch        *b_evtpuw;   //!
   TBranch        *b_evtpuw_sys;   //!
   TBranch        *b_evtJVTw;   //!
   TBranch        *b_evtJVTw_sys;   //
   TBranch        *b_njets;   //!
   TBranch        *b_njets_event;   //!
   TBranch        *b_jetpt;   //!
   TBranch        *b_jeteta;   //!
   TBranch        *b_jetphi;   //!
   TBranch        *b_flavor;   //!
   TBranch        *b_jetpass;   //!
   TBranch        *b_isleading;   //!

   TBranch        *b_ntrack_IP3DNeg; //!
   TBranch        *b_ntrack_IP3D; //!
   TBranch        *b_ntrack_IP2DNeg; //!
   TBranch        *b_ntrack_IP2D; //!
   TBranch        *b_ntrack_SV1Flip; //!
   TBranch        *b_ntrack_SV1; //!
   TBranch        *b_ntrack_JetFitterFlip; //!
   TBranch        *b_ntrack_JetFitter; //!

   TBranch        *b_data_evtweight;   //!

   TBranch        *b_jetHasKShort; //!
   TBranch        *b_jetHasLambda; //!
   TBranch        *b_jetHasConversion; //!
   TBranch        *b_jetHasHadMatInt; //!

   NtupleReader(TTree *tree=0, TString="", TString="", TString="", TString="");
   virtual ~NtupleReader();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int bootstrap_bkeeper);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   /* virtual double DeltaR(double eta_one, double eta_two, double phi_one, double phi_two); */
};

#endif


#ifdef NtupleReader_cxx
NtupleReader::NtupleReader(TTree *tree, TString syst, TString mode, TString reweighting_folder, TString compaigne)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("roo/std/a1_j1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("roo/std/a1_j1.root");
      }
      f->GetObject("NtupleReader",tree);

   }
   m_systematic = syst;
   m_mode = mode;
   m_reweighting_folder = reweighting_folder;
   m_compaigne = compaigne;
   Init(tree);
}

NtupleReader::~NtupleReader()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NtupleReader::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t NtupleReader::LoadTree(Long64_t entry)
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

void NtupleReader::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
  
   fChain->SetBranchAddress("nbootstrap", &nbootstrap, &b_nbootstrap);
   fChain->SetBranchAddress("weight_bootstrap", weight_bootstrap, &b_weight_bootstrap);
   fChain->SetBranchAddress("mc_evtweight", &mc_evtweight, &b_mc_evtweight);
   fChain->SetBranchAddress("evtpuw", &evtpuw, &b_evtpuw);
   fChain->SetBranchAddress("evtpuw_sys", &evtpuw_sys, &b_evtpuw_sys);
   fChain->SetBranchAddress("evtJVTw", &evtJVTw, &b_evtJVTw);
   fChain->SetBranchAddress("evtJVTw_sys", &evtJVTw_sys, &b_evtJVTw_sys);
   fChain->SetBranchAddress("njets", &njets, &b_njets);
   fChain->SetBranchAddress("njets_event", &njets_event, &b_njets_event);
   fChain->SetBranchAddress("jetpt", jetpt, &b_jetpt);
   fChain->SetBranchAddress("jeteta", jeteta, &b_jeteta);
   fChain->SetBranchAddress("jetphi", jetphi, &b_jetphi);
   fChain->SetBranchAddress("flavor", flavor, &b_flavor);
   fChain->SetBranchAddress("jetpass", jetpass, &b_jetpass);
   fChain->SetBranchAddress("isleading", isleading, &b_isleading);
 
   fChain->SetBranchAddress("ntrack_IP3DNeg", ntrack_IP3DNeg, &b_ntrack_IP3DNeg);
   fChain->SetBranchAddress("ntrack_IP3D", ntrack_IP3D, &b_ntrack_IP3D);
   fChain->SetBranchAddress("ntrack_IP2DNeg", ntrack_IP2DNeg, &b_ntrack_IP2DNeg);
   fChain->SetBranchAddress("ntrack_IP2D", ntrack_IP2D, &b_ntrack_IP2D);
   fChain->SetBranchAddress("ntrack_SV1Flip", ntrack_SV1Flip, &b_ntrack_SV1Flip);
   fChain->SetBranchAddress("ntrack_SV1", ntrack_SV1, &b_ntrack_SV1);
   fChain->SetBranchAddress("ntrack_JetFitterFlip", ntrack_JetFitterFlip, &b_ntrack_JetFitterFlip);
   fChain->SetBranchAddress("ntrack_JetFitter", ntrack_JetFitter, &b_ntrack_JetFitter);

   fChain->SetBranchAddress("data_evtweight", data_evtweight, &b_data_evtweight);
 
   fChain->SetBranchAddress("jetHasKShort", jetHasKShort, &b_jetHasKShort);
   fChain->SetBranchAddress("jetHasLambda", jetHasLambda, &b_jetHasLambda);
   fChain->SetBranchAddress("jetHasConversion", jetHasConversion, &b_jetHasConversion);
   fChain->SetBranchAddress("jetHasHadMatInt", jetHasHadMatInt, &b_jetHasHadMatInt);

   for (const auto &name: subtagger::floats){
     fChain->SetBranchAddress(name.first.c_str(), &float_subtagger[name.first]);
   }

   for (const auto &name: subtagger::ints){
     fChain->SetBranchAddress(name.first.c_str(), &int_subtagger[name.first]);
   }

   for (const auto &name: subtagger::doubles){
     fChain->SetBranchAddress(name.first.c_str(), &double_subtagger[name.first]);
   }

   Notify();
}

Bool_t NtupleReader::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void NtupleReader::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t NtupleReader::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef NtupleReader_cxx
