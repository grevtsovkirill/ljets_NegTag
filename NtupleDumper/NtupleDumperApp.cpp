/*
  Steering macro to produce sklimmed NTuple
  Options:
    -d (data)
    -ps (data period / mc slice, e.g. C / JWZ0)
    -s (space separated list of systematics; all uses list from config/xAOD_systematics.txt)
    -f (space separated list of input files)
*/

#include <parser.hpp>
// #include <config_reader.hpp>

#include <TROOT.h>
#include <TString.h>
#include <TFile.h>
#include <TH1D.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "NtupleDumper.h"

using namespace std;

bool runmc = true;

int main(int argc, char* argv[]) {

  std::vector<TString> systematics;
  std::vector<TString> files;
  std::vector<TString> period_slice;
  std::vector<TString> reco_comp;
  bool runmc = true;
  bool file_per_syst = false;

  for ( int i1 = 1; i1 < argc; ++i1){ // start at 1 (0 script name)
    if (strcmp(argv[i1], "-d")==0){
      runmc = false;
    }
    else if (strcmp(argv[i1], "-s")==0){
      cout << "=== selected systematics ===" << endl;
      systematics = get_arguments(argc, argv, i1);
      cout << "=== end of systematics ===" << endl;
    }
    else if (strcmp(argv[i1], "-f")==0){
      cout << "=== selected files ===" << endl;
      files = get_arguments(argc, argv, i1);
      cout << "=== end of files ===" << endl;
    }
    else if (strcmp(argv[i1], "-ps")==0){
      cout << "=== selected period/slice ===" << endl;
      period_slice = get_arguments(argc, argv, i1);
      cout << "=== end of period/slice ===" << endl;
    }
    else if (strcmp(argv[i1], "-split")==0){
      file_per_syst = true;
      cout << "=== 1 file per systematic created ===" << endl;
    }
    else if (strcmp(argv[i1], "-c")==0){
      reco_comp = get_arguments(argc, argv, i1);;
      cout << "=== mc16a / mc16d  ===" << endl;
    }
    else {
      cout << "argument not recognized: " << argv[i1] << endl;
    }
  }

  if (systematics.size()<1) {
    cout << "use default systematic: FlavourTagging_Nominal" <<endl;
    systematics.push_back("FlavourTagging_Nominal");
  }

  if (period_slice.size() != 1) { cout << "provide exactly 1 period/slice" << endl; return 0;}

  TString filename;
  if (runmc){
    cout << "## running mc " << period_slice[0] << " ##" << endl;
    filename = "res/mc16"+reco_comp[0]+"_" + period_slice[0] + ".root";
    // if -split mode activated, create one file per systematic
    if(file_per_syst) filename = "res/mc16"+reco_comp[0]+"_" + period_slice.at(0) + "_" + systematics.at(0) + ".root";
  }
  else{
    cout << "## running data " << period_slice[0] << " ##" << endl;
    filename = "res/data" + period_slice[0] + ".root";
    // if -split mode activated, create one file per systematic
    if(file_per_syst) filename = "res/data" + period_slice.at(0) + "_" + systematics.at(0) + ".root";
  }
 

  TH1D* h_event_count = 0;

  std::vector<TChain*> tchains;
  for (auto syst: systematics){
    TChain * tchain = new TChain(syst);
    tchains.push_back(tchain);
  }

  for ( auto file: files){
    // cout << file << endl;
    int nbranches, entries;
    TFile *f=TFile::Open(file);
    TH1D *h = (TH1D*)f->Get("MetaData_EventCount");
    if ( h_event_count == 0){
      h_event_count = (TH1D*)h->Clone("evcount");
      h_event_count->SetDirectory(0); // otherwise histogram gets deleted...
    }
    else{
      h_event_count->Add(h);
    }
    int metadata_entries = h->GetBinContent(3); 
    TTree* nom_tree = (TTree*)f->Get("FlavourTagging_Nominal");
    if (nom_tree) entries = nom_tree->GetEntries();
    else entries = 0;
    // nbranches = ((TTree*)f->Get("FlavourTagging_Nominal"))->GetListOfBranches()->GetEntries();
    if (metadata_entries < 1) cout << "(0 entries) skip file: " << file << endl;
    else if (entries == 0){
      cout << "(0 entries in tree || no nominal tree) skip file: " << file << endl;
    }
    else{
      cout << " use file: " << file << endl;
      for (auto chain: tchains){
	chain->Add(file);      
      }
    }
    if (entries != metadata_entries){
      cout << "***** entries tree and entries metadata do not match: " << entries << "\t" << metadata_entries << endl;
    }
    f->Close();
    delete f;
  }

  //TFile *histofile = new TFile(filename, "RECREATE");
  for (auto syst: systematics){
    TFile *histofile = new TFile(filename, "RECREATE");
    TTree* mytree = (TTree*)gROOT->FindObject(syst);
    NtupleDumper* dumper = new NtupleDumper(mytree, syst, runmc);
    dumper->m_period_slice = period_slice[0];
    cout << "Initial number of events: " << h_event_count->GetBinContent(1) << " Initial sum of event weights: " << h_event_count->GetBinContent(4) << " Number of events sample: " << h_event_count->GetBinContent(3) << endl;
    if ( h_event_count != 0) {
      dumper->m_sum_of_weights = h_event_count->GetBinContent(4);
      dumper->m_total_events = h_event_count->GetBinContent(1);
      dumper->m_events_sample = h_event_count->GetBinContent(3);
    }
    //TTree *T = new TTree("T","title");
    //dumper->Loop();
    TTree *T;
    T = dumper->Loop();
    //histofile = mytree->GetCurrentFile(); 
    histofile->Write();
    //histofile->Map(); 
    histofile->Close();
    //T->Delete();
    histofile->Delete();
  }
  //cout << "which tree to save: "<< endl;
  // save stuff
  ////histofile = dumper->GetCurrentFile();
  //histofile->Write(0,TObject::kOverwrite);
  //histofile->Close();

  return 0;
}
