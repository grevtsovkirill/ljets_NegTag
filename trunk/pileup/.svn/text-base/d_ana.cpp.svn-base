#include "d.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

bool runmc = true;
bool full_ntuple = true;
int npvfix = -1;
int npvmin = -1;
int ptfix = 0;
bool select_trigger = false;

bool dopvmatch = true;
bool doJESuncert = false;


int main(int argc, char* argv[]) {

  int i1 = 1;
  if (argc<=i1) return 1;
  if (strcmp(argv[i1],"-d")==0) {
    runmc = false;
    ++i1;
    if (argc<=i1) return 1;
  }
  if (strcmp(argv[i1],"-s")==0) {
    full_ntuple = false;
    ++i1;
    if (argc<=i1) return 1;
  }
  if (strcmp(argv[i1],"-t")==0) {
    select_trigger = true;
    ++i1;
    if (argc<=i1) return 1;
  }
  if (strcmp(argv[i1],"-npvfix")==0) {
    ++i1;
    if (argc<=i1) return 1;
    npvfix = atoi(argv[i1]);
    ++i1;
    if (argc<=i1) return 1;
  }
  if (strcmp(argv[i1],"-npvmin")==0) {
    ++i1;
    if (argc<=i1) return 1;
    npvmin = atoi(argv[i1]);
    ++i1;
    if (argc<=i1) return 1;
  }
  if (strcmp(argv[i1],"-ptfix")==0) {
    ++i1;
    if (argc<=i1) return 1;
    ptfix = atoi(argv[i1]);
    ++i1;
    if (argc<=i1) return 1;
  }

  if (strcmp(argv[i1],"-nopvmatch")==0) {
    dopvmatch = false;
    ++i1;
    if(argc<=i1) return 1;
  }

  if (strcmp(argv[i1],"-doJESuncert")==0) {
    doJESuncert = true;
    ++i1;
    if(argc<=i1) return 1;
  }

  //TFile histofile("a1.root","RECREATE");

  TChain* tt = new TChain("atree");
  for (int i=i1; i<argc; ++i) {
    if (strncmp(argv[i],"list:",5)==0) {
      string s(argv[i]+5);
      ifstream in(s.c_str());
      while (in) {
        string ss; in >> ss;
        if (in.eof()) break;
        tt->Add(ss.c_str());
      }
    } else {
      tt->Add(argv[i]);
    }
  }

  TTree* mytree = (TTree*)gROOT->FindObject("atree");
  d* t = new d(mytree);
  t->Loop();

  // save stuff
//  histofile.Write();
//  histofile.Close();

  return 0;
}
