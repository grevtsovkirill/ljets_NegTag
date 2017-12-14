#include <TH2.h>
#include <TH1.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <TFile.h>
#include <stdio.h>
#include <string.h>
#include <map>

#include <TROOT.h>
#include "AtlasUtils.C"
#include "AtlasStyle.C"

using namespace std;

#include "../config/conf.hpp"
// #include "config/kine.h"
#include "../helpers/OutputHelper.hpp"
#include "../helpers/config_reader.cpp"
#include "../helpers/parser.hpp"

vector<string> variables = {"sf","kll","khf","eps_l","eps_d","eps_l_neg","eps_a_neg"};
map<string, string> variables_ytitle = {
  {"sf", "SF"},
  {"kll","k_{ll}"},
  {"khf","k_{hf}"},
  {"eps_l","#epsilon^{MC}_{l}"},
  {"eps_d","#epsilon^{Data}_{l}"},
  {"eps_l_neg","#epsilon^{MC neg}_{l}"},
  {"eps_a_neg","#epsilon^{MC neg}_{incl}"}
};

int main(int argc, char* argv[]) {
  vector<string> systematics;
  for (int i = 1; i< argc; ++i){
    systematics.push_back(argv[i]);
  }
  cout << "systematics used: "; 
  for (auto sys: systematics){
    cout << sys << " ";
  }
  cout << endl;

  //gROOT->LoadMacro("AtlasStyle.C+");
  SetAtlasStyle();
  //gROOT->LoadMacro("AtlasUtils.C+");
  gStyle -> SetOptStat(0);

  auto kin_labels = getKinLabels();

  // ifstream fin("config/var.txt");
  // vector<string> s_var, s_ytitle;
  // while (fin) {
  //   string thevar, theytitle;
  //   fin >> thevar >> theytitle;
  //   if (fin.eof()) break;
  //   s_var.push_back(thevar);
  //   s_ytitle.push_back(theytitle);
  // }
  // const int nvar = s_var.size();
  // cout << "Vars: " << nvar << endl;


  TFile* fsys = new TFile("../CalculateNTVariables/total.root", "read");
  TFile* fval = new TFile("../CalculateNTVariables/raw_systematics/FlavourTagging_Nominal_db.root", "read");

  for (auto tagger: conf::tagger_list){
    int iwp = 0;
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second;
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      string wp_label = to_string(wpoint_title[iwp]);
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;
	string eta_title;
	if (ieta == 1) eta_title = "central";
	else eta_title = "forward";

	string partial_identifier = "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);

        TH1* hsdt = (TH1*)fval->Get((string("datastat")+partial_identifier).c_str());
        TH1* hsys = (TH1*)fsys->Get((string("rel_total")+partial_identifier).c_str());
	for (auto var: variables){
          bool use_totsyst = false;
          if ( var == "sf" || var == "data_epsl" ) use_totsyst = true; 
          TH1* hval = (TH1*)fval->Get((var+partial_identifier).c_str());
	  cout << hval->Integral() << endl;
	  // get range and set error
          double Ymax = -99999999, Ymin = 99999999;
          for(int ib=1; ib<=hval->GetNbinsX(); ib++){
            double m_sf = hval->GetBinContent(ib);
            double m_stat = m_sf * hsdt->GetBinContent(ib);
            double m_syst = m_sf * hsys->GetBinContent(ib);
  
            hval->SetBinError(ib, m_stat);
            hsys->SetBinContent(ib, m_sf);
            hsys->SetBinError(ib, m_syst);
  
            if((m_sf+m_syst)>Ymax) Ymax = m_sf+m_syst;
            if((m_sf-m_syst)<Ymin) Ymin = m_sf-m_syst;
          }

	  // do the plotting
          TCanvas* cc = new TCanvas((string("c_sf_")+partial_identifier).c_str(),(string("c_sf_")+partial_identifier).c_str());
          cc->SetLogx();
          hsys->SetFillColor(kGreen-8);
          hsys->GetXaxis()->SetTitle("jet pT [GeV]");
          hsys->GetXaxis()->SetMoreLogLabels();
          hsys->GetXaxis()->SetTitleOffset(1.2);
          hsys->GetYaxis()->SetTitleOffset(1.2);
          hsys->GetYaxis()->SetTitle(variables_ytitle[var].c_str());
          hsys->SetMaximum(Ymax*1.2);
          hsys->SetMinimum(Ymin*0.8);
          hval->GetXaxis()->SetTitle("jet pT [GeV]");
          hval->GetXaxis()->SetMoreLogLabels();
          hval->GetXaxis()->SetTitleOffset(1.2);
          hval->GetYaxis()->SetTitleOffset(1.2);
          hval->GetYaxis()->SetTitle(variables_ytitle[var].c_str());
          hval->SetMaximum(Ymax*1.2);
          hval->SetMinimum(Ymin*0.8);
          hval->SetLineColor(kBlue);
          hval->SetMarkerStyle(8);   
          hval->SetMarkerSize(1.0); 
          hval->SetLineWidth(3);     
	  
          if(use_totsyst){ 
            hsys->Draw("e2");
            hval->Draw("e0same");      
            //l->AddEntry(hsys,(thevar+" (stat+syst)").c_str(),"f");    
          }else{
            hval->Draw("e0");      
          }
	  string title = tagger + " WP = " + wp_label + "%, eta region " + eta_title;

	  myText(0.23,0.87, kBlack, title.c_str());
/*
          l->AddEntry(hval,(thevar+" (stat)").c_str(),     "lp");    
          l->Draw();
*/
          cc->Modified();
  
          //cc->Print((string("eps_plots/var_")+var+"_"+partial_identifier+".eps").c_str());
          //cc->Print((string("pdf_plots/var_")+var+"_"+partial_identifier+".pdf").c_str());
          cc->Print((string("png_plots/var_")+var+"_"+partial_identifier+".png").c_str());

          delete hval;
        }
        delete hsys;
        delete hsdt;
      }
      ++iwp;
    }
  }

  // MV2c20 plots
  TFile* fmc   = new TFile("../CalculateNTVariables/res/FlavourTagging_Nominal/mc.root",   "read");
  TFile* fdata = new TFile("../CalculateNTVariables/res/FlavourTagging_Nominal/data.root", "read");
  TFile* ff;

  const std::vector<std::string> flav_list = {"", "l", "c", "b"};

  map<int, map<int, map<string, map<string, map<string, TH1F*>>>>> h_tagger;

  for (auto tagger: conf::tagger_list) {
    for (auto p_pt: kin_labels) {
      for (auto p_eta: p_pt.second){
        for (string mode: {"","neg"}){

          for (auto flav: flav_list) {
            if (flav=="") ff = fdata;
            else ff = fmc;
            h_tagger[p_pt.first][p_eta.first][flav][tagger][mode] =
              (TH1F*)(ff->Get((mode + "w_" + flav + "_" + p_eta.second + "_" + tagger).c_str()));
            //std::cout<<"dav -> mode="<<mode<<" flav="<<flav<<" p_eta.second="<<p_eta.second<<" tagger="<<tagger<<std::endl;
          }

          // do plot
          string partial_identifier = "_" + mode + "_" + p_eta.second + "_" + tagger;

          TCanvas* cc = new TCanvas((string("c_tagger_")+partial_identifier).c_str(),(string("c_tagger_")+partial_identifier).c_str());
          
          // pad1
          double b_marg_pad1, t_marg_pad1;
          //b_marg_pad1 = 0.41; t_marg_pad1 = 0.05; // big
          b_marg_pad1 = 0.35; t_marg_pad1 = 0.05;
          TPad *pad1 = new TPad("pad1","pad1",0,0,1,1);
          pad1->SetBottomMargin(b_marg_pad1);
          pad1->SetTopMargin(t_marg_pad1);
          pad1->Draw();
          pad1->cd();
          gPad->SetLogy();
          //
          h_tagger[p_pt.first][p_eta.first]["l"][tagger][mode]->SetFillColor(kRed);
          h_tagger[p_pt.first][p_eta.first]["c"][tagger][mode]->SetFillColor(kGreen);
          h_tagger[p_pt.first][p_eta.first]["b"][tagger][mode]->SetFillColor(kBlue);
          THStack *hs = new THStack("hs","");
          hs->Add(h_tagger[p_pt.first][p_eta.first]["b"][tagger][mode],"hist");
          hs->Add(h_tagger[p_pt.first][p_eta.first]["c"][tagger][mode],"hist");
          hs->Add(h_tagger[p_pt.first][p_eta.first]["l"][tagger][mode],"hist");
          //
          int maxi=h_tagger[p_pt.first][p_eta.first][""][tagger][mode]->GetMaximum();
          h_tagger[p_pt.first][p_eta.first][""][tagger][mode]->SetMaximum(maxi*5);
          //int mini=h_tagger[p_pt.first][p_eta.first][""][tagger][mode]->GetMinimum();
          //h_tagger[p_pt.first][p_eta.first][""][tagger][mode]->SetMinimum(mini/5);
          //h_tagger[p_pt.first][p_eta.first][""][tagger][mode]->GetXaxis()->SetTitle((tagger + mode).c_str());
          h_tagger[p_pt.first][p_eta.first][""][tagger][mode]->Draw("E1");
          hs->Draw("same");
          h_tagger[p_pt.first][p_eta.first][""][tagger][mode]->Draw("E1same");
          // text
          TLatex *tex = new TLatex(0.52,0.88,"ATLAS"); tex->SetNDC(); tex->SetTextFont(72); tex->SetLineWidth(2); tex->Draw();
          tex = new TLatex(0.65,0.88,"Internal");      tex->SetNDC(); tex->SetTextFont(42); tex->SetLineWidth(2); tex->Draw();
          //tex = new TLatex(0.15,0.82,"#int L dt = 3.21 fb^{-1}, #sqrt{s}=13 TeV"); tex->SetNDC(); tex->SetTextFont(72); tex->SetTextSize(0.04); tex->SetLineWidth(2); tex->Draw();
          stringstream s_pt; s_pt << conf::pt_lowedges[p_pt.first-1] << "<p_{T}(GeV)<" << conf::pt_lowedges[p_pt.first]; string str_pt = s_pt.str();
          tex = new TLatex(0.52,0.84,str_pt.c_str()); tex->SetNDC(); tex->SetTextFont(72); tex->SetTextSize(0.03); tex->SetLineWidth(2); tex->Draw();
          stringstream s_eta; s_eta << conf::eta_lowedges[p_eta.first-1] << "<|#eta|<" << conf::eta_lowedges[p_eta.first]; string str_eta = s_eta.str();
          tex = new TLatex(0.52,0.80,str_eta.c_str()); tex->SetNDC(); tex->SetTextFont(72); tex->SetTextSize(0.03); tex->SetLineWidth(2); tex->Draw();
          // legend
          TLegend *leg_pad = new TLegend(0.82, 0.72, 0.9, 0.93);
          leg_pad->AddEntry(h_tagger[p_pt.first][p_eta.first]["l"][tagger][mode], "l");
          leg_pad->AddEntry(h_tagger[p_pt.first][p_eta.first]["c"][tagger][mode], "c");
          leg_pad->AddEntry(h_tagger[p_pt.first][p_eta.first]["b"][tagger][mode], "b");
          leg_pad->AddEntry(h_tagger[p_pt.first][p_eta.first][""][tagger][mode],  "Data");
          leg_pad->SetFillColor(0); leg_pad->SetLineWidth(2); leg_pad->Draw();


          // pad2
          double pad2_n1, pad2_n2, pad2_n3, pad2_n4;
          //pad2_n1 = 0; pad2_n2 = 0; pad2_n3 = 1; pad2_n4 = 0.4; // big
          pad2_n1 = 0; pad2_n2 = 0; pad2_n3 = 1; pad2_n4 = 0.34;
          double b_marg_pad2, t_marg_pad2;
          //b_marg_pad2 = 0.35; t_marg_pad2 = 0; // big
          b_marg_pad2 = 0.41; t_marg_pad2 = 0;
          TPad *pad2 = new TPad("pad2","pad2",pad2_n1, pad2_n2, pad2_n3, pad2_n4);
          pad2->SetTopMargin(t_marg_pad2);
          pad2->SetBottomMargin(b_marg_pad2);
          pad2->Draw();
          pad2->cd(); pad2->cd()->SetGrid();
          // make histo of b,c,l MC
          TH1F* h_sum_mc;
          h_sum_mc = (TH1F*)h_tagger[p_pt.first][p_eta.first]["b"][tagger][mode]->Clone();
          h_sum_mc->Add(h_tagger[p_pt.first][p_eta.first]["c"][tagger][mode],1.0);
          h_sum_mc->Add(h_tagger[p_pt.first][p_eta.first]["l"][tagger][mode],1.0);
          // make necessary histos
          TH1F* h_ratio;
          h_ratio = (TH1F*)h_tagger[p_pt.first][p_eta.first][""][tagger][mode]->Clone();
          h_ratio->Divide(h_sum_mc);
          //float diff = fabs(1.-h_ratio->GetMaximum())*1.05;
          float diff = 0.5;
          //
          TH1F* h_1;
          h_1 = (TH1F*)h_tagger[p_pt.first][p_eta.first][""][tagger][mode]->Clone();
          for (int i = 1; i < h_1->GetNbinsX()+1; i++)
          {
            h_1->SetBinContent(i,1.0);
            h_1->SetBinError(i,0.0);
          }
          h_1->SetLineColor(kRed);
          // draw
          h_1->GetXaxis()->SetLabelFont(63); //font in pixels
          h_1->GetXaxis()->SetLabelSize(20); //in pixels
          h_1->GetYaxis()->SetLabelFont(60); //font in pixels
          h_1->GetXaxis()->SetTitleFont(63);
          h_1->GetXaxis()->SetTitleSize(20);
          h_1->GetXaxis()->SetTitleOffset(4);
          h_1->GetXaxis()->SetTitle((tagger + mode).c_str());
          h_1->GetYaxis()->SetTitleFont(63);
          h_1->GetYaxis()->SetTitleSize(16);
          h_1->GetYaxis()->SetTitleOffset(0.9);
          h_1->GetYaxis()->SetTitle("DATA / MC");
          h_1->SetMaximum(1.+diff);
          h_1->SetMinimum(1.-diff);
          h_1->Draw("L");
          h_ratio->Draw("epsame");

          // save plot
          //cc->Print((string("eps_plots/tagger_")+partial_identifier+".eps").c_str());
          //cc->Print((string("pdf_plots/tagger_")+partial_identifier+".pdf").c_str());
          cc->Print((string("png_plots/tagger_")+partial_identifier+".png").c_str());
          // clean
          delete hs;
        }
      }
    }
  }

}
