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
#include <TLeaf.h>
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
    cout << "use default systematic: nominal" <<endl;
    systematics.push_back("nominal");
  }

  if (period_slice.size() != 1) { cout << "provide exactly 1 period/slice" << endl; return 0;}

  TString filename;
  if (runmc){
    cout << "## running mc " << period_slice[0] << " ##" << endl;
    filename = "res/mc16"+reco_comp[0]+"_" + period_slice[0] + ".root";
    // if -split mode activated, create one file per systematic
    if(file_per_syst) filename = "res/s20p_mc16"+reco_comp[0]+"_" + period_slice.at(0) + "_" + systematics.at(0) + ".root";
  }
  else{
    cout << "## running data " << period_slice[0] << " ##" << endl;
    filename = "res/data" + period_slice[0] + ".root";
    // if -split mode activated, create one file per systematic
    if(file_per_syst) filename = "res/data" + period_slice.at(0) + "_" + systematics.at(0) + ".root";
    
    
  }
  cout << "## FName " << filename << " ##" << endl;
  
  TH1D* h_event_count = 0;

  std::vector<TChain*> tchains;
  for (auto syst: systematics){
    TChain * tchain = new TChain(syst);
    tchains.push_back(tchain);
  }
  
  double read_sum_of_weights=0;
  double read_total_events=0;

  for ( auto file: files){
    cout << file << endl;
    int nbranches, entries;
    
    TFile *f=TFile::Open(file);
    
    TTree* sumw_tree = (TTree*)f->Get("sumWeights");
    //sumw_tree->Draw("totalEventsWeighted")
    TH1F *h1 = new TH1F("h1", "h1 title", 100, 0, 4.4);
    sumw_tree->Draw("totalEventsWeighted>>h1");
    sumw_tree->Draw("totalEvents>>h1");
    cout << "  = = = read totalEventsWeighted = "<<  ((TTree*)f->Get("sumWeights"))->GetLeaf("totalEventsWeighted")->GetValue(0) << endl;
    cout << "  = = = read totalEvents = "<<  ((TTree*)f->Get("sumWeights"))->GetLeaf("totalEvents")->GetValue(0) << endl;
    read_sum_of_weights += ((TTree*)f->Get("sumWeights"))->GetLeaf("totalEventsWeighted")->GetValue(0);
    read_total_events += ((TTree*)f->Get("sumWeights"))->GetLeaf("totalEventsWeighted")->GetValue(0);
    //sumWeights->GetLeaf("totalEvents")->GetValue(0)
    //cout << read_sum_of_weights << endl;
    TTree* nom_tree = (TTree*)f->Get("nominal");
    if (nom_tree) entries = nom_tree->GetEntries();
    else entries = 0;
    // nbranches = ((TTree*)f->Get("FlavourTagging_Nominal"))->GetListOfBranches()->GetEntries();
    //if (metadata_entries < 1) cout << "(0 entries) skip file: " << file << endl;
    if (entries == 0){
      cout << "(0 entries in tree || no nominal tree) skip file: " << file << endl;
    }
    else{
      cout << " use file: " << file << endl;
      for (auto chain: tchains){
	chain->Add(file);      
      }
    }

    f->Close();
    delete f;
  }

  TFile *histofile = new TFile(filename, "RECREATE");
  for (auto syst: systematics){
    TTree* mytree = (TTree*)gROOT->FindObject(syst);
    NtupleDumper* dumper = new NtupleDumper(mytree, syst, runmc);
    cout << "NtupleDumperApp.cpp: filename= "<<filename <<"    loop over systs = "<< syst << endl; 
    dumper->m_period_slice = period_slice[0];
    //cout << "Initial number of events: " << h_event_count->GetBinContent(1) << " Initial sum of event weights: " << h_event_count->GetBinContent(4) << " Number of events sample: " << h_event_count->GetBinContent(3) << endl;
    dumper->m_sum_of_weights = read_sum_of_weights;
    //sumWeights->GetLeaf("totalEventsWeighted")->GetValue(0)
    dumper->m_total_events = read_total_events;
    dumper->m_events_sample =0;
    //sumWeights->GetLeaf("totalEvents")->GetValue(0)
    //dumper->m_events_sample = h_event_count->GetBinContent(3);
    //
    //}
    
    //TTree *T = new TTree("T","title");
    dumper->Loop();
    //TTree *T;
    //T = dumper->Loop();
    //histofile = mytree->GetCurrentFile(); 
    //histofile->Write();
    //histofile->Map(); 
    //histofile->Close();
    //T->Delete();
    //histofile->Delete();
  }
  //cout << "which tree to save: "<< endl;
  // save stuff
  ////histofile = dumper->GetCurrentFile();

  //histofile->Write(0,TObject::kOverwrite);
  histofile->Write();
  histofile->Close();
  
  return 0;
}
