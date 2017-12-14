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

using namespace std;

#include "config/conf.hpp"

int main(){

  ifstream fin("config/systematic.text");
  vector<string> s_syst, s_mode;
  vector<int> ifun;
  map<string,string> systMap;
  string ones;
  while (getline(fin,ones)) {
    if (fin.eof()) break;

    if(ones.find("data_n")!=string::npos) continue;

    int j = ones.find(" ");
    int k = ones.find(":");

    if (j!=string::npos) s_syst.push_back(ones.substr(0,j));
    if (k!=string::npos) {
      s_mode.push_back(ones.substr(j+1,k-(j+1))); //substr(istart, ilength)
      systMap.insert(pair<string,string>(ones.substr(0,j),ones.substr(k+1)));
    }
  }
  const int nss = s_syst.size();
  cout << "Systematic: " << nss << endl;

  const int hundred = 100;

  TFile* fsys[nss];
  for(int is=0; is<nss; is++){
    fsys[is] = new TFile((string("output/rel_")+s_syst.at(is)+".root").c_str(), "read");
  }
  TFile* fval = new TFile("input/std_db.root", "read");

  map<string, TH1*> h_out;
  map<string, TH1*> h_SFs;
 for (auto tagger: conf::tagger_list){
   int iwp = 0;
   for (auto wp: conf::wpoint_map.find(tagger)->second){
     for (int ie = 0; ie<conf::n_eta; ++ie) {
       string partial_identifier_plots = "_" + tagger + "_w" + to_string(iwp) + "_eta" + to_string(ie+1);

       TH1* hval = (TH1*)fval->Get((string("sf")+partial_identifier_plots).c_str());
       TH1* hsdt = (TH1*)fval->Get((string("sys_datastat")+partial_identifier_plots).c_str());

       h_SFs[partial_identifier_plots] = (TH1*)hval->Clone((string("sf_stat")+partial_identifier_plots).c_str());
       h_out[partial_identifier_plots] = (TH1*)hval->Clone((string("rel_total")+partial_identifier_plots).c_str()); h_out[partial_identifier_plots]->Reset();

       cout<<tagger<<" wp "<<wp<<" ie "<<ie<<" val: "<< hval->GetEntries()<<", "<<hval->GetBinContent(1)<<" dt syst "<<hsdt->GetBinContent(1)<<endl;


       for(int ipt=0; ipt<conf::n_pt; ipt++){
	 h_SFs[partial_identifier_plots]->SetBinError(ipt+1, hval->GetBinContent(ipt+1)*hsdt->GetBinContent(ipt+1)); // data stat uncertainty
	 
	 double tot_syst = 0;
	 for(int is=0; is<nss; is++){
	   
	   TH1* hsys = (TH1*)fsys[is]->Get((string("rel_")+s_mode.at(is)+partial_identifier_plots).c_str());
	   tot_syst += pow(hsys->GetBinContent(ipt+1), 2);
	 }
	 tot_syst = sqrt(tot_syst);
	 h_out[partial_identifier_plots]->SetBinContent(ipt+1, tot_syst); // total systematic relative !!
       }
     }
   }
 }
 
 TFile* f_out = new TFile(string("output/rel_total.root").c_str(), "recreate");
 for (auto histogram: h_out){
   histogram.second->Write();
 }
 for (auto histogram: h_SFs){
   histogram.second->Write();
 }
 f_out->Close();
}
