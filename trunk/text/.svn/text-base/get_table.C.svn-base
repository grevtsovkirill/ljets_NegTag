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
#include <iomanip>

using namespace std;

int main(){

  ifstream fin("config/systematic.text");
  vector<string> s_syst, s_mode;
  vector<int> ifun;
  map<string,string> systMap;
  string ones;
  while (getline(fin,ones)) {
    if (fin.eof()) break;
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
    fsys[is] = new TFile((string("roo/rel_")+s_syst.at(is)+".root").c_str(), "read");
  }
  TFile* fval = new TFile("input/std_db.root", "read");

#include "config/taggers.h"
#include "config/kine.h"

  FILE * pFile;

  for (int jt = 0; jt<nt; ++jt) {
    for (int iw = 0; iw<nw; ++iw) {
      double weight = dw[jt*nw+iw];
      if (weight<-998.) continue;
      if (!docalib[jt*nw+iw]) continue;

      ostringstream ss; ss <<  setprecision(9) << weight; 
      string filename = "texts/mistag_negative_" + string(tags[jt]) + "_w" + string(ss.str()) + "_" + s_operation[jt*nw+iw] + "_LCJVF_winter2013_v2.txt"; 
      pFile = fopen (filename.c_str(),"w");

      for (int ie = 0; ie<ne; ++ie) {
	int jh = (jt*nw+iw)*ne+ie;

	string sel = string(tags[jt])+"_w"+char('0'+iw)+"_eta"+char('1'+ie);
        //cout<<jt<<" iw "<<iw<<" ie "<<ie<<" sel "<< sel << endl;

        TH1* hval = (TH1*)fval->Get((string("sf_")+sel).c_str());
        TH1* hsdt = (TH1*)fval->Get((string("sys_datastat_")+sel).c_str());
        
        cout<<jt<<" iw "<<iw<<" ie "<<ie<<" val: "<< hval->GetEntries()<<", "<<hval->GetBinContent(1)<<" dt syst "<<hsdt->GetBinContent(1)<<endl;

        fprintf(pFile, "Analysis(negative tags,light,%s, %s, AntiKt4TopoLCJVF){ \n", (string(tags_ex[jt]).c_str()), (string(ss.str())).c_str());
        for(int ipt=0; ipt<nk; ipt++){
          fprintf(pFile, "  bin(%3d<pt<%3d, %2.1f<abseta<%2.1f)\n", int(xk[ipt]), int(xk[ipt+1]), xe[ie], xe[ie+1]);
          fprintf(pFile, "  {\n");
          fprintf(pFile, "    central_value(%6.4f,%6.4f)\n", hval->GetBinContent(ipt+1), hval->GetBinContent(ipt+1) * hsdt->GetBinContent(ipt+1)); //data stat inserted here

          for(int is=0; is<nss; is++){

            string nickname = s_syst.at(is);
            map<string,string>::iterator inm = systMap.find(s_syst.at(is));
            if (inm!=systMap.end()) nickname = inm->second;

            TH1* hsys = (TH1*)fsys[is]->Get((string("rel_")+s_mode.at(is)+"_"+sel).c_str());
            if     (is <=1) fprintf(pFile, "    meta_data(%s,%7.0f,%5.1f)\n", nickname.c_str(), hsys->GetBinContent(ipt+1), hsys->GetBinError(ipt+1));
            else if(is ==2) fprintf(pFile, "    usys(%s,%5.2f%%)\n", nickname.c_str(), hsys->GetBinContent(ipt+1) * hundred );
            else            fprintf(pFile, "    sys(%s,%5.2f%%)\n",  nickname.c_str(), hsys->GetBinContent(ipt+1) * hundred );
          }
          fprintf(pFile, "  }\n");
        }
      }
    }
  }
}
