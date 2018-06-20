#include <TH2.h>
#include <TH1.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <TFile.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>

using namespace std;

#include "hist_rel.h"
#include "../helpers/OutputHelper.hpp"
#include "../helpers/parser.hpp"

map<string, TH1F*> getHistograms(string filename){
  map<string, TH1F*> histograms;
  
}


void get_std_syst(string compaigne, string syst){
  map<string, TH1D*> h_out;
  auto kin_labels = getKinLabels();

  std::string nom_name = "raw_systematics/FlavourTagging_Nominal_db_"+compaigne+".root";
  std::string syst2 = syst;

  if(syst.substr(0,14)=="subleadingjet_")
  {
    nom_name = "raw_systematics/subleadingjet_db_"+compaigne+".root";
    syst2 = syst.substr(14);
  }

  TFile* f_nom = new TFile(nom_name.c_str(), "read");

  for (auto tagger: conf::tagger_list){
    int iwp = 0;
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second;
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      string wp_label = to_string(wpoint_title[iwp]);
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;

	string partial_identifier = "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);

        TH1* h_nom = (TH1D*)f_nom->Get((syst2 + partial_identifier).c_str());
	if (h_nom == 0) cout << "trying to get: " << syst2 + partial_identifier << ", but no histogram found" << endl;
	string identifier = "rel_sf" + partial_identifier;
	h_out[identifier] = (TH1D*)h_nom->Clone(identifier.c_str());
      }
      ++iwp;
    }
  }

  TFile* f_out = new TFile((string("rel_systematics/rel_sf_")+syst+"_"+compaigne+".root").c_str(), "recreate");
  for (auto histogram: h_out){
    histogram.second->Write();
  }
  f_out->Close();
  f_nom->Close();
}

void get_up_down(string compaigne, string syst, string key){

  map<string, TH1D*> h_out;
  auto kin_labels = getKinLabels();

  std::string nom_name = "raw_systematics/FlavourTagging_Nominal_db_"+compaigne+".root";
  if(syst.substr(0,14)=="subleadingjet_") nom_name = "raw_systematics/subleadingjet_db_"+compaigne+".root";
  TFile* f_nom = new TFile(nom_name.c_str(), "read");

  TFile* f_sup = new TFile(("raw_systematics/" + syst + "up_db_"+compaigne+".root").c_str(), "read");
  TFile* f_sdn = new TFile(("raw_systematics/" + syst + "down_db_"+compaigne+".root").c_str(), "read");

  for (auto tagger: conf::tagger_list){
    int iwp = 0;
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second;
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      string wp_label = to_string(wpoint_title[iwp]);
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;

	string partial_identifier = "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);

        TH1D* h_nom = (TH1D*)f_nom->Get((key + partial_identifier).c_str());
        TH1D* h_sup = (TH1D*)f_sup->Get((key + partial_identifier).c_str());
        TH1D* h_sdn = (TH1D*)f_sdn->Get((key + partial_identifier).c_str());
	if (h_nom == 0) cout << "trying to get: " << syst + partial_identifier << ", but no histogram found" << endl;

	string identifier_up = "rel_" + key + "_up" + partial_identifier;
	string identifier_down = "rel_" + key + "_down" + partial_identifier;

 	h_out[identifier_up] = (TH1D*) h_sup->Clone((string("rel_up_") + h_sup->GetName()).c_str()); 
 	h_out[identifier_down] = (TH1D*) h_sdn->Clone((string("rel_down_") + h_sdn->GetName()).c_str()); 
        h_out[identifier_up]->Add(h_nom, -1.0);
        h_out[identifier_down]->Add(h_nom, -1.0);

        h_out[identifier_up]->Divide(h_nom);
        h_out[identifier_down]->Divide(h_nom);
       
        for (int ibin=1; ibin<=h_nom->GetNbinsX(); ibin++)
        {
          double up = max(h_out[identifier_up]->GetBinContent(ibin), 
                          h_out[identifier_down]->GetBinContent(ibin));
                 up = max (up, 0.);
 
          double down = min(h_out[identifier_up]->GetBinContent(ibin),
                            h_out[identifier_down]->GetBinContent(ibin));
                 down = min(down, 0.);

          h_out[identifier_up]->SetBinContent(ibin, up);
          h_out[identifier_down]->SetBinContent(ibin, down);
        }

      }
      ++iwp;
    }
  }
  
  TFile* f_out = new TFile((string("rel_systematics/rel_")+key+string("_")+syst+"_"+compaigne+".root").c_str(), "recreate");
  for (auto histogram: h_out){
    histogram.second->Write();
  }

  f_out->Close();
  f_nom->Close();
  f_sup->Close();
  f_sdn->Close();
}


// get relative systematic w.r.t std, varying "key" (eg. sf, data_epsl, etc)
void get_rel_syst(string compaigne, string syst, string key){
  map<string, TH1D*> h_out;
  auto kin_labels = getKinLabels();

  std::string nom_name = "raw_systematics/FlavourTagging_Nominal_db_"+compaigne+".root";
  if(syst.substr(0,14)=="subleadingjet_" ||
     syst=="notrackrew_subleadingjet") nom_name = "raw_systematics/subleadingjet_db_"+compaigne+".root";
  TFile* f_nom = new TFile(nom_name.c_str(), "read");

  TFile* f_sys = new TFile(("raw_systematics/" + syst + "_db_"+compaigne+".root").c_str(), "read");

  for (auto tagger: conf::tagger_list){
    int iwp = 0;
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second;
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      string wp_label = to_string(wpoint_title[iwp]);
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;

	string partial_identifier = "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);

	TH1D* h_nom = (TH1D*)f_nom->Get((key+partial_identifier).c_str());
        TH1D* h_sys = (TH1D*)f_sys->Get((key+partial_identifier).c_str());
 	h_out[partial_identifier] = hist_rel(h_nom, h_sys);
      }
      ++iwp;
    }
  }

  TFile* f_out = new TFile((string("rel_systematics/rel_")+key+string("_")+syst+"_"+compaigne+".root").c_str(), "recreate");
  for (auto histogram: h_out){
    histogram.second->Write();
  }

  f_out->Close();
  f_nom->Close();
  f_sys->Close();
}



// includes statistical uncertainty on the uncertainty for subleadingjet uncertainty.
void get_subleadingjet_syst(string compaigne, string syst, string key){
  auto kin_labels = getKinLabels();

  // file with central values
  TFile* f_central = new TFile((string("rel_systematics/rel_")+key+string("_")+syst+"_"+compaigne+".root").c_str(), "update");

  std::string f_nom_data_name = "raw_systematics/datastat_db_"+compaigne+".root";
  std::string f_sys_data_name = "raw_systematics/datastat_subleadingjet_db_"+compaigne+".root";

  std::string f_nom_mc_name = "raw_systematics/mcstat_db_"+compaigne+".root";
  std::string f_sys_mc_name = "raw_systematics/mcstat_subleadingjet_db_"+compaigne+".root";

  TFile* f_nom_data = new TFile(f_nom_data_name.c_str(), "read");
  TFile* f_sys_data = new TFile(f_sys_data_name.c_str(), "read");

  TFile* f_nom_mc = new TFile(f_nom_mc_name.c_str(), "read");
  TFile* f_sys_mc = new TFile(f_sys_mc_name.c_str(), "read");

  for (auto tagger: conf::tagger_list){
    int iwp = 0;
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second;
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      string wp_label = to_string(wpoint_title[iwp]);
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;

        if(tagger=="MV2c20") continue; // skip MV2c20 to keep good performance

	string partial_identifier = "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);

        // get central values
	string identifier = "rel_" + key + "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);
	TH1D* h_central = (TH1D*)f_central->Get(identifier.c_str());

        // bootstrap histo: 1 per pT bin and 1 for MC, 1 for data 
        TH1D *h_mc_bootstrap[h_central->GetNbinsX()];
        TH1D *h_data_bootstrap[h_central->GetNbinsX()];

        std::string h_data_bootstrap_baseName = key + "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta) + "_data_bin";
        std::string h_mc_bootstrap_baseName = key + "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta) + "_mc_bin";

        f_central->cd();
        TDirectory *dir_bootstrap = f_central->mkdir((key + partial_identifier).c_str());

        // loop on pT bins 
        for(int ibin=0; ibin<h_central->GetNbinsX(); ibin++)
        {
          std::string h_data_bootstrap_name = h_data_bootstrap_baseName + to_string(ibin+1);
          std::string h_mc_bootstrap_name = h_mc_bootstrap_baseName + to_string(ibin+1);
          double nbin = 400.;

          double min = std::min(h_central->GetBinContent(ibin+1)*-1., h_central->GetBinContent(ibin+1)*3.);
          double max = std::max(h_central->GetBinContent(ibin+1)*-1., h_central->GetBinContent(ibin+1)*3.);
          if(abs(h_central->GetBinContent(ibin+1))<0.01)
          {
            nbin = 1000.;
            min = -0.5;
            max = 0.5;
          }

          dir_bootstrap->cd();
          h_data_bootstrap[ibin] = new TH1D(h_data_bootstrap_name.c_str(),"",nbin,min,max);
          h_mc_bootstrap[ibin] = new TH1D(h_mc_bootstrap_name.c_str(),"",nbin,min, max);
        }

        // filling the bootstrap histo
        std::string h_syst_mc_baseName = key + "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta) + "_mcstat_";
        std::string h_syst_data_baseName = key + "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta) + "_datastat_";

        for(int ibootstrap=1; ibootstrap<1001; ibootstrap++)
        {
          //std::cout << ibootstrap << std::endl;
          std::string h_syst_mc_name = h_syst_mc_baseName + to_string(ibootstrap);
          std::string h_syst_data_name = h_syst_data_baseName + to_string(ibootstrap);

          TH1D* h_sys_data = (TH1D*)f_sys_data->Get(h_syst_data_name.c_str());
          TH1D* h_nom_data = (TH1D*)f_nom_data->Get(h_syst_data_name.c_str());

          TH1D* h_sys_mc = (TH1D*)f_sys_mc->Get(h_syst_mc_name.c_str());
          TH1D* h_nom_mc = (TH1D*)f_nom_mc->Get(h_syst_mc_name.c_str());

          for(int ibin=0; ibin<h_central->GetNbinsX(); ibin++)
          {
            h_data_bootstrap[ibin]->Fill( (h_sys_data->GetBinContent(ibin+1) - h_nom_data->GetBinContent(ibin+1)) / h_nom_data->GetBinContent(ibin+1) );
            h_mc_bootstrap[ibin]->Fill( (h_sys_mc->GetBinContent(ibin+1) - h_nom_mc->GetBinContent(ibin+1)) / h_nom_mc->GetBinContent(ibin+1) );
          }
        }
        dir_bootstrap->Write();

        // final uncertainty
        f_central->cd();
        double data_uncertainty[h_central->GetNbinsX()];
        double mc_uncertainty[h_central->GetNbinsX()];
        for(int ibin=0; ibin<h_central->GetNbinsX(); ibin++)
        {
          double data_mean_nom = h_central->GetBinContent(ibin+1)-h_data_bootstrap[ibin]->GetMean();
          double data_rms = h_data_bootstrap[ibin]->GetRMS();
          data_uncertainty[ibin] = sqrt( data_mean_nom*data_mean_nom + data_rms*data_rms);

          double mc_mean_nom = h_central->GetBinContent(ibin+1)-h_mc_bootstrap[ibin]->GetMean();
          double mc_rms = h_mc_bootstrap[ibin]->GetRMS();
          mc_uncertainty[ibin] = sqrt( mc_mean_nom*mc_mean_nom + mc_rms*mc_rms);

          if(abs(data_mean_nom)>data_rms) cout << "WARNING in data bootstrap, mean-nom > bootstrap rms, i.e. " << data_mean_nom << " vs " << data_rms << " for: " << tagger << ", " << wp_label << " WP, eta " << to_string(ieta) << ", pT " << ibin+1 << std::endl;
          if(abs(mc_mean_nom)>mc_rms) cout << "WARNING in mc bootstrap, mean-nom > bootstrap rms, i.e. " << mc_mean_nom << " vs " << mc_rms << " for: " << tagger << ", " << wp_label << " WP, eta " << to_string(ieta) << ", pT " << ibin+1 << std::endl;

          h_central->SetBinError(ibin+1, sqrt(data_uncertainty[ibin]*data_uncertainty[ibin] + 
                                              mc_uncertainty[ibin]*mc_uncertainty[ibin]) );

          delete h_data_bootstrap[ibin];
          delete h_mc_bootstrap[ibin];

        }
          
        h_central->Write();
          
      }
      ++iwp;
    }
  }

  f_central->Close();
  f_nom_data->Close();
  f_sys_data->Close();
  f_nom_mc->Close();
  f_sys_mc->Close();
}



// get statistical uncertainty from bootstrap replicas
void get_bootstrap_syst(string compaigne, string syst, string key){
  map<string, TH1D*> h_out;
  auto kin_labels = getKinLabels();

  TFile* f_out = new TFile((string("rel_systematics/rel_")+key+string("_")+syst+"_"+compaigne+".root").c_str(), "recreate");

  std::string f_name = "raw_systematics/FlavourTagging_Nominal_db_"+compaigne+".root";
  if(syst.find("subleading")!=std::string::npos) f_name = "raw_systematics/subleadingjet_db_"+compaigne+".root";
  TFile* f_nom = new TFile(f_name.c_str(), "read");

  TFile* f_sys = new TFile(("raw_systematics/" + syst + "_db_"+compaigne+".root").c_str(), "read");

  for (auto tagger: conf::tagger_list){
    int iwp = 0;
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second;
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      string wp_label = to_string(wpoint_title[iwp]);
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;

	string partial_identifier = "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);

	TH1D* h_nom = (TH1D*)f_nom->Get((key+partial_identifier).c_str());

        // bootstrap histo: 1 per pT bin 
        TH1D *h_bootstrap[h_nom->GetNbinsX()];
        std::string h_bootstrap_baseName = key + partial_identifier + "_bin";

        f_out->cd();
        TDirectory *dir_bootstrap = f_out->mkdir((key + partial_identifier).c_str());
 
        for(int ibin=0; ibin<h_nom->GetNbinsX(); ibin++)
        {
          std::string h_bootstrap_name = h_bootstrap_baseName + to_string(ibin+1);

          double nbin = 400.;
          double min = std::min(h_nom->GetBinContent(ibin+1)*-1., h_nom->GetBinContent(ibin+1)*3.);
          double max = std::max(h_nom->GetBinContent(ibin+1)*-1., h_nom->GetBinContent(ibin+1)*3.);
          if(abs(h_nom->GetBinContent(ibin+1))<0.01)
          {
            nbin = 1000.;
            min = -0.5;
            max = 0.5;
          }
          dir_bootstrap->cd();
          h_bootstrap[ibin] = new TH1D(h_bootstrap_name.c_str(),"",nbin,min,max);
        }

        // filling the bootstrap histo
        std::string syst_short = syst;
        if(syst.find("subleading")!=std::string::npos) syst_short.erase(syst_short.find("_subleading"));

        std::string h_syst_baseName = key + partial_identifier + "_" + syst_short + "_";

        for(int ibootstrap=1; ibootstrap<1001; ibootstrap++)
        {
          //std::cout << ibootstrap << std::endl;
          std::string h_syst_name = h_syst_baseName + to_string(ibootstrap);
          TH1D* h_sys = (TH1D*)f_sys->Get(h_syst_name.c_str());
          for(int ibin=0; ibin<h_nom->GetNbinsX(); ibin++) h_bootstrap[ibin]->Fill(h_sys->GetBinContent(ibin+1));
        }
        dir_bootstrap->Write();
 
        // final uncertainty
        std::string identifier = "rel_" + key + partial_identifier;
        h_out[identifier] = (TH1D*)h_nom->Clone((string("rel_") + key + partial_identifier).c_str()); 

        double uncertainty[h_nom->GetNbinsX()];
        for(int ibin=0; ibin<h_nom->GetNbinsX(); ibin++)
        {

          double mean_nom = h_nom->GetBinContent(ibin+1)-h_bootstrap[ibin]->GetMean();
          double rms = h_bootstrap[ibin]->GetRMS();
          uncertainty[ibin] = sqrt( mean_nom*mean_nom + rms*rms) / h_nom->GetBinContent(ibin+1);

          if(abs(mean_nom)>rms) cout << "WARNING in bootstrap, mean-nom > bootstrap rms, i.e. " << mean_nom << " vs " << rms << " for: " << tagger << ", " << wp_label << " WP, eta " << to_string(ieta) << ", pT " << ibin+1 << std::endl;

          h_out[identifier]->SetBinContent(ibin+1, uncertainty[ibin]);

          // TEMPORARY UNC SET TO 0
          //h_out[identifier]->SetBinContent(ibin+1, 0);

          delete h_bootstrap[ibin];
        }

      }
      ++iwp;
    }
  }

  f_out->cd();
  for (auto histogram: h_out){
    histogram.second->Write();
  }

  f_out->Close();
  f_nom->Close();
  f_sys->Close();
}



/*
// get relative systematic on multi- parallel inputs (eg. data periods )
void get_syst_dataperiod(string s_sys, vector<TString> periods, string key){

  map<string, TH1*> h_out;
  auto kin_labels = getKinLabels();

  vector<TFile*> f_sn;

  for (auto period: periods){
    f_sn.push_back(new TFile("raw_systematics/dataperiod"+period+"_db_"+compaigne+".root", "read"));
  }

  for (auto tagger: conf::tagger_list){
    int iwp = 0;
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second;
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      string wp_label = to_string(wpoint_title[iwp]);
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;

	string partial_identifier = "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);
	vector<TH1*> h_sn;
	for (auto file: f_sn){
	  h_sn.push_back((TH1*)file->Get((key + partial_identifier).c_str()));
	}
	string identifier = "rel_" + key + "_" + partial_identifier;
	auto it = h_out.find(identifier);
	if ( it != h_out.end())
	  cout << "----> overwriting " << identifier << endl;
	h_out[identifier] = hist_rel_n(h_sn);
      }
      ++iwp;
    }
  }

  TFile* f_out = new TFile((string("rel_systematics/rel_")+s_sys+"_"+compaigne+".root").c_str(), "recreate");
  for (auto histogram: h_out){
    histogram.second->Write();
  }

  f_out->Close();
  for (auto file: f_sn){
    file->Close();
  }
}
*/


int main(int argc, char* argv[]) {

  cout << "calculate relative systematics" << endl;
  TString systematic="stdS";
  TString type="stdT";
  TString var="stdV";
  vector<TString> periods;
  TString compaigne="def";


  for ( int i1 = 1; i1 < argc; ++i1){ // start at 1 (0 script name)
    if (strcmp(argv[i1], "-s")==0){
        systematic = get_argument(argc, argv, i1);
      }
    else if (strcmp(argv[i1], "-t")==0){
        type = get_argument(argc, argv, i1);
      }
    else if (strcmp(argv[i1], "-v")==0){
        var = get_argument(argc, argv, i1);
      }
    else if (strcmp(argv[i1], "-p")==0){
        periods = get_arguments(argc, argv, i1);
      }
    else if (strcmp(argv[i1], "-c")==0){
        compaigne = get_argument(argc, argv, i1);
    }
    else {
      cout << "argument not recognized: " << argv[i1] << endl;
    }
  }

  cout<<"working on " << systematic << ", for compaigne "<< compaigne <<endl;


  // std uncertainty computed only once for all
  if(type=="std") var="sfonly";

  if (type == "std"){
    get_std_syst(compaigne.Data(),systematic.Data());
  }
  else if (type == "updown"){
    get_up_down(compaigne.Data(),systematic.Data(), var.Data());
  }
  else if (type == "rel"){
    get_rel_syst(compaigne.Data(),systematic.Data(), var.Data());
    // special case for subleading: add the uncertainty on the uncertainty from bootstrap
    // makes sense only for final SF (different denominator)
    if(systematic=="subleadingjet" && var=="sf") get_subleadingjet_syst(compaigne.Data(),systematic.Data(), var.Data());
  }
  else if (type == "bootstrap"){
    get_bootstrap_syst(compaigne.Data(),systematic.Data(), var.Data());
  }
  else if (type == "dataperiod"){
//    get_syst_dataperiod(systematic.Data(), periods, var.Data());
  }
  else{
    cout << "----> type not recognized: " << type << endl;
  }

  return 0;
}
