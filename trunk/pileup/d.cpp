#define d_cxx
#include "d.h"
#include <TH2.h>
#include <TFile.h>

#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <cassert>

//#include "JetUncertainties/MultijetJESUncertaintyProvider.h"
#include "PileupReweighting/TPileupReweighting.h"

using namespace std;


//#define PASS_BTRIG
#define DIJET_SELECTION

#define USE_NEW_QUALITY
#define NTRPV_CUT
#define REMOVE_JX_TAILS

#define PV_MATCH

extern bool runmc;
extern bool full_ntuple;
extern int npvfix,npvmin;
extern int ptfix;
extern bool select_trigger;
extern bool dopvmatch;
extern bool doJESuncert;

//JESUncertaintyProvider myJES("AntiKt4EMJESTopoJets");

//Root::TPileupReweighting m_pileupTool = new Root::TPileupReweighting("nameOfTool");
//Root::TPileupReweighting *m_pileupTool;

void d::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L d.C
//      Root > d t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) return;


  fChain->SetBranchStatus("*",0);  // disable all branches
  fChain->SetBranchStatus("runnum",1); 
  fChain->SetBranchStatus("avgIntPerXing",1); 
  if(runmc){
   avgIntPerXing *= 1.11;
   cout<<"running MC adding a factor 1.11 as recommanded for data 2012. "<<endl;
  }

  TObjArray *tmp = fChain->GetListOfBranches();
  bool LostInfo = false;
  if(runmc){
   if (tmp->FindObject("mc_channum")   != 0) fChain->SetBranchStatus("mc_channum",1);   else LostInfo = true;
   if (tmp->FindObject("mc_evtweight") != 0) fChain->SetBranchStatus("mc_evtweight",1); else LostInfo = true;
  }
  // book histograms
  TH1F* h_cutflow = new TH1F("cutflow", "cutflow", 25, -0.5, 24.5);
  TH1F* h_avgInt = new TH1F("avgInt", "avgInt", 100, 0, 100);

  int currentTree = -1;
  int mcid = 0;
  Long64_t nentries = fChain->GetEntries();


  //myJES.init();

  Root::TPileupReweighting *m_pileupTool;
  m_pileupTool = new Root::TPileupReweighting("nameOfTool");
  m_pileupTool->UsePeriodConfig("MC12a"); //for MC11b change the string to "MC11b"...or for other configurations, use AddPeriod to manually define it
  m_pileupTool->AddBinning("myCustomWeight",100,0,100); //this, for example, creates a histogram with 100 bins from 0 to 50. You can also do variable bin widths...see the other  AddBinning methods
  m_pileupTool->Initialize();

  cout<<"Total number of events: "<< nentries<<endl;

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    // check if tree number changed
    if (currentTree != fCurrent) {
      currentTree = fCurrent;
      string s = ((TChain*)fChain)->GetFile()->GetName();
      if (runmc) {
	size_t i = s.find("JZ");
	if (i!=string::npos &&i>=2) mcid = s[i+2]-'0';
      } else {
	size_t i = s.rfind("data");
	mcid = -1;
      }
    }

    const int nmcs = 8;
    int mc_channum_init[nmcs] = {147910, 147911, 147912, 147913, 147914, 147915, 147916, 147917};
    double mc_xsec_rew[nmcs] = {7.2850e+07, 7.2850e+07, 2.6359e+04, 5.4419e+02, 6.4453e+00, 3.9740e-02, 4.1609e-04, 4.0636e-05};
    for(int im=0; im<nmcs; im++){mc_xsec_rew[im] = 1.0 / mc_xsec_rew[im];}
 


//IMPORTANT: For data, use mc_channel_number = -1 and mcevt_weight = 1
//myVariable is anything you want, e.g. averageIntPerXing
//m_pileupTool->Fill("myCustomWeight",RunNumber,mc_channel_number,mcevt_weight[0][0],myVariable);

    int mc_channel_number = -1; 
    float mc_event_weight = 1;
    if(runmc){
      if(LostInfo){ 
        mc_channel_number = mc_channum_init[mcid]; 
        mc_event_weight = 1.0; 
        if(jentry <1){
          cout<<"mc channel number not found, set to: "<<mc_channum_init[mcid]<<endl;
          cout<<"mc event weight not found, set to: 1.0"<<endl;
        }
      }
      else{
        mc_channel_number = mc_channum;
        mc_event_weight = mc_evtweight;
      }
      if(jentry<1) cout<<"mc_chan: "<<mc_channum<<endl;
    }

    if(runmc) mc_event_weight *= mc_xsec_rew[mcid];
    m_pileupTool->Fill("myCustomWeight",runnum,mc_channel_number,mc_event_weight,avgIntPerXing);

    h_avgInt->Fill(avgIntPerXing, mc_event_weight);

    if (jentry%10000==0) { cout << jentry << '\r'; cout.flush(); }
    //if (jentry%10000==0) { cout << jentry << endl;}
    h_cutflow->Fill(0.0);

  }

  m_pileupTool->WriteToFile("prw_output.root"); //or whatever filename


}
