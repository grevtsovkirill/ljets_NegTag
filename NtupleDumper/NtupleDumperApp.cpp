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
  TString outputpath;
  outputpath = "root://eosatlas//eos/user/m/mstrauss/BTagDumpedNtuples/Test21/DumpedNtuples/";
  if (runmc){
    cout << "## running mc " << period_slice[0] << " ##" << endl;
    filename = outputpath + "/mc" + period_slice[0] + ".root";
    // if -split mode activated, create one file per systematic
    if(file_per_syst) filename = outputpath +  "/mc" + period_slice.at(0) + "_" + systematics.at(0) + ".root";
 } 
  
  else{
    cout << "## running data " << period_slice[0] << " ##" << endl;
    filename = outputpath + "/data" + period_slice[0] + ".root";
    // if -split mode activated, create one file per systematic
    if(file_per_syst) filename = outputpath +  "/data" + period_slice.at(0) + "_" + systematics.at(0) + ".root";
  }
 

  TH1D* h_event_count = 0;

  std::vector<TChain*> tchains;
  for (auto syst: systematics){
    TChain * tchain = new TChain(syst);
    tchains.push_back(tchain);
  }

    for (auto file: files) {
	TFile *f = TFile::Open(file);
	if ((!f) || (f->IsZombie())){
	   std::cout << "(no file || file is zombie) skip file: " << file << std::endl;
           delete f;
	   continue;
	}
	TH1D *h; 
	f->GetObject("MetaData_EventCount", h);
	if ((!h) || (h->GetBinContent(3) < 1)){
	  std::cout << "(no histogram || 0 entries in histogram) skip file: " << file << std::endl;
          delete f;
	  continue;
	}
	TTree *t;
	f->GetObject("FlavourTagging_Nominal", t);
	if ((!t) || (t->GetEntries() < 1)) {
	   std::cout << "(no tree || 0 entires in tree) skip file " << file << std::endl;
	  delete f;
	  continue;
	}
	if (h_event_count) h_event_count->Add(h);
	else {
	   h->SetDirectory(0);
	   h->SetName("evcount");
	   h_event_count = h;
	}
	delete f;
	
	std::cout << "use file: " << file << std::endl;
	for (auto chain: tchains) chain->Add(file);
}

  TFile *histofile = new TFile(filename, "RECREATE");
  for (auto syst: systematics){
    TTree* mytree = (TTree*)gROOT->FindObject(syst);
    NtupleDumper* dumper = new NtupleDumper(mytree, syst, runmc);
    dumper->m_period_slice = period_slice[0];
    cout << "Initial number of events: " << h_event_count->GetBinContent(1) << " Initial sum of event weights: " << h_event_count->GetBinContent(4) << " Number of events sample: " << h_event_count->GetBinContent(3) << endl;
    if ( h_event_count != 0) {
      dumper->m_sum_of_weights = h_event_count->GetBinContent(4);
      dumper->m_total_events = h_event_count->GetBinContent(1);
      dumper->m_events_sample = h_event_count->GetBinContent(3);
    }
    dumper->Loop();
  }

  // save stuff
  histofile->Write(0,TObject::kOverwrite);
  histofile->Close();

  return 0;
}
