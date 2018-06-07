/*
  Steering macro to produce systematics and filtered samples
  Options:
    -d (is data)
    -s (systematic, will be outputfolder)
    -f (space separated list of input files)
    -m (mode)
    -p (dataperiod, when using mode dataperiod)
*/
#include "NtupleReader.h"
#include "parser.hpp"
#include "config_reader.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
using namespace std;

bool runmc = true;

int main(int argc, char* argv[]) {

  TString systematic = "";
  std::vector<TString> files;
  TString mode = "";
  TString period = "";
  std::vector<TString> reco_comp;

  bool bootstrap_flag = false;
  TString bootstrap_index = "0";

  for ( int i1 = 1; i1 < argc; ++i1){ // start at 1 (0 script name)
    if (strcmp(argv[i1], "-d")==0){
      runmc = false;
    }
    else if (strcmp(argv[i1], "-s")==0){
      cout << "=== selected systematic ===" << endl;
      systematic = get_argument(argc, argv, i1);
      cout << "=== end of systematics ===" << endl;
    }
    else if (strcmp(argv[i1], "-f")==0){
      cout << "=== selected files ===" << endl;
      files = get_arguments(argc, argv, i1);
      cout << "=== end of files ===" << endl;
    }
    else if (strcmp(argv[i1], "-m")==0){
      cout << "=== selected mode ===" << endl;
      mode = get_argument(argc, argv, i1);
      cout << "=== end of modes ===" << endl;
    }
    else if (strcmp(argv[i1], "-p")==0){
      cout << "=== selected period ===" << endl;
      period = get_argument(argc, argv, i1);
      if (period == "") return 1;
      cout << "=== end of periods ===" << endl;
    }
    else if (strcmp(argv[i1], "-split")==0)
    {
        cout << "=== 1 file per bootstrap created ===" << endl;
        bootstrap_flag = true;
        bootstrap_index = get_argument(argc, argv, i1);
    } 
    else if (strcmp(argv[i1], "-c")==0){
      reco_comp = get_arguments(argc, argv, i1);;
      cout << "=== mc16a / mc16d  ===" << endl;
    }
    else {
      cout << "argument not recognized: " << argv[i1] << endl;
    }
  }
  if (systematic == "") {
    cout << "please choose systematic!" << endl;
    return 1;
  }

  TString filename;
  if (runmc){
    filename = "res/" + systematic + "/mc_"+reco_comp[0]+".root";
    if(bootstrap_flag) filename = "res/" + systematic + "/mc_"+reco_comp[0]+"_" + bootstrap_index + ".root";
  }
  else{
    filename = "res/" + systematic + "/data.root";
    if(bootstrap_flag) filename = "res/" + systematic + "/data_" + bootstrap_index + ".root";
  }
  TFile histofile(filename, "RECREATE");
  TDirectory* dir_subtagger = histofile.mkdir("subTagger");
  TDirectory* dir_subsample = histofile.mkdir("subSamples");
  TDirectory* dir_subflavour = histofile.mkdir("subFlavour");
  TDirectory* dir_bootstrap = histofile.mkdir("bootstrap");


  TString treename;
  TString reweighting_folder;
  if ( mode == "xAOD"){
    treename = systematic;
    reweighting_folder = systematic;
  }
  else if ( mode == "xAOD2"){
    treename = systematic(14,1000);
    reweighting_folder = systematic(14,1000);
  }
  else if ( systematic.Contains("PRW_DATASF") || systematic.Contains("JVT_effSF") ||
            systematic.Contains("conversions") || systematic.Contains("hadronic") ||
            systematic.Contains("longlivedparticles") )
  {
    treename = "FlavourTagging_Nominal";
    reweighting_folder = systematic;
    if( systematic.Contains("subleadingjet_")) reweighting_folder = systematic(14,1000);
  }
  else{
    treename = "FlavourTagging_Nominal";
    reweighting_folder = "FlavourTagging_Nominal";
  }

  cout << "Read tree: "<< treename << endl;
  TChain* tchain = new TChain(treename);
  for (auto file: files){
    tchain->Add(file);
  }

  // bootstrap replica bookkeeper
  int bootstrap_bkeeper = 0;
  const char* bootstrap_bkeeper_char = bootstrap_index.Data();
  std::stringstream converter;
  converter << bootstrap_bkeeper_char;
  converter >> bootstrap_bkeeper;

  histofile.cd();
  TTree* mytree = (TTree*)gROOT->FindObject(treename);
  NtupleReader* t = new NtupleReader(mytree, systematic, mode, reweighting_folder);
  t->dir_subtagger = dir_subtagger;
  t->dir_subsample = dir_subsample;
  t->dir_subflavour = dir_subflavour;
  t->dir_bootstrap = dir_bootstrap;
  t->Loop(bootstrap_bkeeper);

  // save stuff
  histofile.Write();
  histofile.Close();

  return 0;
}
