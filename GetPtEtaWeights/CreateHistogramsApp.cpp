/*
  Steering macro for plots of the kinematic of the two jets
  Options:
    -d (data)
    -f (space separated list of input files)
*/


#include <parser.hpp>
#include <config_reader.hpp>

#include <TRint.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include <CreateHistograms.h>

using namespace std;

bool runmc = true;
bool file_per_syst = false;
bool HERWIG = false;
bool SHERPA = false;

int main(int argc, char* argv[]) 
{

  std::vector<TString> bootstrap_index;
  std::vector<TString> systematics;
  std::vector<TString> files;
  std::vector<TString> compagnie;

  // read arguments from command line
  for ( int i1 = 1; i1 < argc; ++i1)
    { // start at 1 (0 script name)
      if (strcmp(argv[i1], "-d")==0)
	{
	  runmc = false;
	}
      else if (strcmp(argv[i1], "-f")==0)
	{
	  cout << "=== selected files ===" << endl;
	  files = get_arguments(argc, argv, i1);
          if(files.at(0).Contains("W_H")) HERWIG = true;
          if(files.at(0).Contains("W_S")) SHERPA = true;
	  cout << "=== end of files ===" << endl;
	}
      else if (strcmp(argv[i1], "-s")==0)
	{
	  cout << "=== selected systematics ===" << endl;
	  systematics = get_arguments(argc, argv, i1);
	  cout << "=== end of systematics ===" << endl;
	}
      else if (strcmp(argv[i1], "-split")==0)
        {
          file_per_syst = true;
          if(HERWIG) file_per_syst = false;
          if(SHERPA) file_per_syst = false;
          cout << "=== 1 file per systematic created ===" << endl;
	  bootstrap_index = get_arguments(argc, argv, i1);
        } 
      else if (strcmp(argv[i1], "-c")==0)
	{
	  cout << "=== select compagnie ===" << endl;
	  compagnie = get_arguments(argc, argv, i1);
	}
      else {
	cout << "argument not recognized: " << argv[i1] << endl;
      }
    }

  // if (runmc)
  //   systematics = read_list("../config/xAOD_systematics.txt");
  // else
  //   systematics = {"FlavourTagging_Nominal"};

  TString filename;
  TString extraGen="";
  if (runmc)
    {
      cout << "## running mc ##" << endl;
      if(HERWIG) extraGen="_HERWIG";//filename = "mc_HERWIG.root";
      if(SHERPA) extraGen="_SHERPA";//filename = "mc_SHERPA.root";
      filename = "mc"+extraGen+"_"+compagnie.at(0)+".root";
      if(file_per_syst) filename = "mc"+extraGen+"_"+compagnie.at(0)+"_" + systematics.at(0) + "_" + bootstrap_index.at(0) + ".root";
    }
  else
    {
      cout << "## running data ##" << endl;
      filename = "data"+compagnie.at(0)+".root";
      if(file_per_syst) filename = "data"+compagnie.at(0)+"_" + systematics.at(0) + "_" + bootstrap_index.at(0) + ".root";
    }

  TFile* histofile = new TFile(filename, "RECREATE");
  std::vector<TChain*> tchains;

  for (auto syst: systematics)
    {
      TChain * tchain;
      if(syst.Contains("PRW_DATASF") || syst.Contains("JVT_effSF") ||
         syst.Contains("conversions") || syst.Contains("hadronic") ||
         syst.Contains("longlivedparticles")) tchain = new TChain("FlavourTagging_Nominal");
      else                            tchain = new TChain(syst);
      tchains.push_back(tchain);
    }

  for ( auto file: files)
    {
      TFile::Open(file);

      for (auto chain: tchains)
	{
	  chain->Add(file);      
	}
    }

  for (auto syst: systematics)
    {
      // bootstrap replica bookkeeper
      int bootstrap_bkeeper = 0;
      if (bootstrap_index.size()>0)
      {
        const char* bootstrap_bkeeper_char = bootstrap_index.at(0).Data();
        std::stringstream converter;
        converter << bootstrap_bkeeper_char;
        converter >> bootstrap_bkeeper;
      }

      TDirectory* myDir = histofile->mkdir(syst);
      myDir->cd();
      TTree* mytree;
      if(syst.Contains("PRW_DATASF") || syst.Contains("JVT_effSF") ||
         syst.Contains("conversions") || syst.Contains("hadronic") ||
         syst.Contains("longlivedparticles")) mytree = (TTree*)gROOT->FindObject("FlavourTagging_Nominal");
      else                                    mytree = (TTree*)gROOT->FindObject(syst);

      //TTreePerfStats *ps = new TTreePerfStats("ioperf",mytree);

      CreateHistograms* hc = new CreateHistograms(mytree, syst);
      hc->Loop(bootstrap_bkeeper);

      //ps->Finish();
      //ps->Print();
    }

    // save stuff
    histofile->Write();
    histofile->Close();
    delete histofile;
    return 0;
}
