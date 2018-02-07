#include <TH2.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <TFile.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include "TLatex.h"
#include "TLine.h"
#include "TPave.h"
#include "TPad.h"
#include "TMarker.h"

using namespace std;

#include "hist_rel.h"
#include "../helpers/OutputHelper.hpp"
#include "../helpers/parser.hpp"

using namespace std;

// --------------------ATLAS LABEL
void ATLASLabel(Double_t x,Double_t y,const char* text,Color_t color)
{
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);

  double delx = 0.115*696*gPad->GetWh()/(472*gPad->GetWw());

  l.DrawLatex(x,y,"ATLAS");
  if (text) {
    TLatex p;
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.DrawLatex(x+delx,y,text);
    //    p.DrawLatex(x,y,"#sqrt{s}=900GeV");
  }
}

// ---------------------MAIN
int main(int argc, char* argv[]) {

  // Get arguments
  cout << "calculate total systematics" << endl;
  vector<TString> systematics;
  vector<TString> vars;
  for ( int i1 = 1; i1 < argc; ++i1){ // start at 1 (0 script name)
    if (strcmp(argv[i1], "-s")==0){
        systematics = get_arguments(argc, argv, i1);
      }
    else if (strcmp(argv[i1], "-v")==0){
        vars = get_arguments(argc, argv, i1);
      }
    else {
      cout << "argument not recognized: " << argv[i1] << endl;
    }
  }

  cout << " " << std::endl; 
  cout << " " << std::endl; 

  // Print systematics
  cout << "systematics used: "; 
  for (auto sys: systematics){
    cout << sys << " ";
  }
  cout << endl;

  // Get Sys files
  map<TString, TFile*> sys_files;
  for(int isys=0; isys<systematics.size(); isys++)
  {
    TString sys = systematics[isys];
    if(vars[isys]=="sfonly") sys_files[sys] = new TFile("rel_systematics/rel_sf_"+sys+".root", "read");
    else                     sys_files[sys] = new TFile("rel_systematics/rel_" + vars[isys] + "_" +sys+".root", "read");
  }

  auto kin_labels = getKinLabels();

  // nominal values
  TFile* fval = new TFile("raw_systematics/FlavourTagging_Nominal_db.root", "read");
  TFile* fval2 = new TFile("raw_systematics/subleadingjet_db.root", "read");

  // output file
  TFile* f_out = new TFile("tot_systematics/total_" + vars[0] + ".root", "recreate");

  // histograms
  map<string, TCanvas*> c_out_final;
  map<string, TH1D*> h_out_nom;
  map<string, TH1D*> h_out_syst_up;
  map<string, TH1D*> h_out_syst_down;
  map<string, TH1D*> h_out_calib_up;
  map<string, TH1D*> h_out_calib_down;
  map<string, TH1D*> h_out_stat;
  map<string, TH1D*> h_out_tot_up;
  map<string, TH1D*> h_out_tot_down;

  // loop start
  for (auto tagger: conf::tagger_list){
    std::cout << "***************" << tagger << "***************" << std::endl;
    int iwp = 0;
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second;
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      string wp_label = to_string(wpoint_title[iwp]);
      std::cout << "***************" << wp_label << " WP ***************" << std::endl;

      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;
        std::cout << "*************** ETA " << ieta << "***************" << std::endl;
        
	
	string partial_identifier = "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);

        // nominal value (SF, KLL, KHF, eps_d_neg)
	TH1D* hval = (TH1D*)fval->Get(vars[0]+partial_identifier);
	TH1D* hval2 = (TH1D*)fval2->Get(vars[0]+partial_identifier); // for first pT bin

        // total stat
        h_out_stat[partial_identifier] = (TH1D*)hval->Clone((string("rel_stat_total")+partial_identifier).c_str()); 
	h_out_stat[partial_identifier]->Reset();

        // total syst_up
        h_out_syst_up[partial_identifier] = (TH1D*)hval->Clone((string("rel_syst_up_total")+partial_identifier).c_str()); 
	h_out_syst_up[partial_identifier]->Reset();

        // total syst_down
        h_out_syst_down[partial_identifier] = (TH1D*)hval->Clone((string("rel_syst_down_total")+partial_identifier).c_str()); 
	h_out_syst_down[partial_identifier]->Reset();

        // calib up
        h_out_calib_up[partial_identifier] = (TH1D*)hval->Clone((string("rel_calib_up_total")+partial_identifier).c_str()); 
	h_out_calib_up[partial_identifier]->Reset();

        // calib down
        h_out_calib_down[partial_identifier] = (TH1D*)hval->Clone((string("rel_calib_down_total")+partial_identifier).c_str()); 
	h_out_calib_down[partial_identifier]->Reset();

        // total uncertainty (up)
        h_out_tot_up[partial_identifier] = (TH1D*)hval->Clone((string("rel_tot_up")+partial_identifier).c_str()); 
	h_out_tot_up[partial_identifier]->Reset();

        // total uncertainty (down)
        h_out_tot_down[partial_identifier] = (TH1D*)hval->Clone((string("rel_tot_down")+partial_identifier).c_str()); 
	h_out_tot_down[partial_identifier]->Reset();

        // final nominal
        h_out_nom[partial_identifier] = (TH1D*)hval->Clone(vars[0]+"_nom"+partial_identifier); 

        // Loop on pT bins
	for (int ipt=0; ipt<conf::n_pt; ++ipt){

          double tot_syst_up = 0;
          double tot_syst_down = 0;
          double tot_calib_up = 0;
          double tot_calib_down = 0;
          double tot_stat = 0;

          // loop on uncertainties
          for (int i=0; i< systematics.size(); ++i){
    
            // **********************PRELIMINARIES******************* //

            // if not first pT bin, skip "subleadingjet_" unc. (JES unc. for subleading)
            if(ipt!=0 && systematics.at(i)(0,14)=="subleadingjet_") continue;

            // if first pT bin, skip "FlavourTagging_JET" unc. (JES unc. for leading)
            if(ipt==0 && systematics.at(i)(0,15)=="FlavourTagging_") continue;
                             // skip generator, conversions, hadronic, longlived, d0smearing, z0smearing, faketracks, trackrecoeff (subleadingjet_ versions used)
            if(ipt==0 && (systematics.at(i)=="generator" || systematics.at(i)(0,11)=="conversions" ||
                     systematics.at(i)(0,8)=="hadronic" || systematics.at(i)=="longlivedparticles" ||
                     systematics.at(i)=="d0smearing" || systematics.at(i)=="z0smearing" || 
                     systematics.at(i)=="faketracks" || systematics.at(i)=="trackrecoeff" ||
                     systematics.at(i)=="fracHF" || 
                     systematics.at(i)=="eps_c_neg" ||
                     systematics.at(i)=="eps_b_neg") ) continue;

            // skip uncertainty related to N track reweighting
            if(systematics.at(i)=="notrackrew" || systematics.at(i)=="notrackrew_subleadingjet") continue;

            // for eps_neg_d and kll, skip "std" (= HF) uncertainties (fracHF, eps_c_neg, eps_b_neg)
            if(!vars[0].Contains("khf") && !vars[0].Contains("sf") && !vars[0].Contains("eps_d") && vars[i].Contains("sfonly")) 
            {
              std::cout << systematics.at(i) << " SKIPPED" << std::endl;
              continue; 
            } 

            // skip leading/subleadingjet uncertainty except for sf
            // makes sense only for SF (denominator is different)
            if(!vars[0].Contains("sf") && systematics.at(i)=="subleadingjet") continue;

            // **********************STATISTICAL UNCERTAINTY******************* //

            // stat for first bin 
            if(systematics.at(i)=="mcstat_subleadingjet" || systematics.at(i)=="datastat_subleadingjet")
            {
              if(ipt==0)
              {
	        TH1* hstat = (TH1*)sys_files[systematics[i]]->Get("rel_" + vars[i] + partial_identifier);
	        if (!hstat)
	          cout << "----> no histogram found for " << systematics[i] << endl;
	        tot_stat += pow(hstat->GetBinContent(ipt+1), 2);
              }
            }
            // stat EXCEPT FOR FIRST BIN
            else if(systematics.at(i)=="mcstat" || systematics.at(i)=="datastat")
            { 
              if(ipt!=0)
              {
	        TH1* hstat2 = (TH1*)sys_files[systematics[i]]->Get("rel_" + vars[i] + partial_identifier);
	        if (!hstat2)
	          cout << "----> no histogram found for " << systematics[i] << endl;
	        tot_stat += pow(hstat2->GetBinContent(ipt+1), 2);
              }
            }

            // **********************SYSTEMATIC UNCERTAINTY******************* //

            // syst_up/down
            else if(systematics.at(i).Contains("conversions") ||
                    systematics.at(i).Contains("hadronic") ||
                    (systematics.at(i).Contains("FlavourTagging") && !systematics.at(i).Contains("FlavourTagging_JET_JER_SINGLE_NP__1up")) )
            {
	      TH1D* hsys_up = (TH1D*)sys_files[systematics[i]]->Get("rel_up_" + vars[i] + partial_identifier);
	      TH1D* hsys_down = (TH1D*)sys_files[systematics[i]]->Get("rel_down_" + vars[i] + partial_identifier);
	      if (!hsys_up || !hsys_down)
	        cout << "----> no histogram found for " << systematics[i] << endl;
	      tot_syst_up += pow( max(0., hsys_up->GetBinContent(ipt+1)), 2);
	      tot_syst_up += pow( max(0., hsys_down->GetBinContent(ipt+1)), 2);

	      tot_syst_down += pow( min(0., hsys_down->GetBinContent(ipt+1)), 2);
	      tot_syst_down += pow( min(0., hsys_up->GetBinContent(ipt+1)), 2);

              if(systematics.at(i).Contains("FlavourTagging_JET"))
              {
                tot_calib_up += pow(  max(0., hsys_up->GetBinContent(ipt+1)), 2);
                tot_calib_up += pow(  max(0., hsys_down->GetBinContent(ipt+1)), 2);
                tot_calib_down += pow(  min(0., hsys_up->GetBinContent(ipt+1)), 2);
                tot_calib_down += pow(  min(0., hsys_down->GetBinContent(ipt+1)), 2);
              }
 
              //std::cout << systematics.at(i) << " " << hsys_up->GetBinContent(ipt+1) << " " << hsys_down->GetBinContent(ipt+1) << std::endl;
           }
            // other syst
            else
            {
              TH1D *hsys;
              if(vars[i]!="sfonly") hsys = (TH1D*)sys_files[systematics[i]]->Get("rel_" + vars[i] + partial_identifier); 
              else                  hsys = (TH1D*)sys_files[systematics[i]]->Get((string("rel_sf") + partial_identifier).c_str());

	      if (!hsys)
	        cout << "----> no histogram found for " << systematics[i] << endl;

              // symmetrized: JER, subleadingjet, faketracks, eps_HF_neg, generator
              if(systematics.at(i).Contains("FlavourTagging_JET_JER_SINGLE_NP__1up") ||
                 systematics.at(i)=="faketracks" ||
                 systematics.at(i)=="eps_b_neg" || 
                 systematics.at(i)=="eps_c_neg") 
              {
	        tot_syst_up += pow(hsys->GetBinContent(ipt+1), 2);
	        tot_syst_down += pow(hsys->GetBinContent(ipt+1), 2);
              }
              // 4% flat/symmetrized uncertainty for subleadingjet
              else if(systematics.at(i)=="subleadingjet")
              {
	        tot_syst_up += pow(0.05, 2);
	        tot_syst_down += pow(0.05, 2);
              }
              // 0% extra gen dependence uncertainty
              else if(systematics.at(i).Contains("generator"))
              {
	        tot_syst_up += pow(0., 2);
	        tot_syst_down += pow(0., 2);
              }
              // don't symmetrize: long-lived, smearing, frac_HF 
              else
              {
                tot_syst_up += pow( max(hsys->GetBinContent(ipt+1),0.), 2);
                tot_syst_down += pow( min(hsys->GetBinContent(ipt+1),0.), 2);
              }
            }

	  } // end loop on uncertainties
	  tot_syst_up = sqrt(tot_syst_up);
	  tot_syst_down = -sqrt(tot_syst_down);
	  tot_calib_up = sqrt(tot_calib_up);
	  tot_calib_down = -sqrt(tot_calib_down);
	  tot_stat = sqrt(tot_stat);
       
	  cout << "pt bin: " << ipt << "\t SF: " << hval->GetBinContent(ipt+1) << " \ttot syst (+, rel): " << tot_syst_up << " \ttot syst (-, rel): " << tot_syst_down << "\ttot stat (mc+data): " << tot_stat <<  endl;

          // write uncertainties
	  h_out_syst_up[partial_identifier]->SetBinContent(ipt+1, tot_syst_up); // total systematic relative !!
	  h_out_syst_down[partial_identifier]->SetBinContent(ipt+1, tot_syst_down); // total systematic relative !!
	  h_out_calib_up[partial_identifier]->SetBinContent(ipt+1, tot_calib_up); // total systematic relative !!
	  h_out_calib_down[partial_identifier]->SetBinContent(ipt+1, tot_calib_down); // total systematic relative !!
	  h_out_stat[partial_identifier]->SetBinContent(ipt+1, tot_stat); // total statistics relative !!
	  h_out_tot_up[partial_identifier]->SetBinContent(ipt+1, sqrt(tot_stat*tot_stat+tot_syst_up*tot_syst_up)); // total relative !!
	  h_out_tot_down[partial_identifier]->SetBinContent(ipt+1, -sqrt(tot_stat*tot_stat+tot_syst_down*tot_syst_down)); // total relative !!

          // write nominal with stat uncertainties
          // IF FIRST PT BIN, use hval2
          if(ipt==0)
          { 
            h_out_nom[partial_identifier]->SetBinContent(ipt+1, hval2->GetBinContent(ipt+1));
            h_out_nom[partial_identifier]->SetBinError(ipt+1, tot_stat*hval2->GetBinContent(ipt+1));
          }
          else h_out_nom[partial_identifier]->SetBinError(ipt+1, tot_stat*hval->GetBinContent(ipt+1));

          //if(ipt==0) std::cout << "TEST" << h_out_nom[partial_identifier]->GetBinError(1) << " " <<  h_out_tot_up[partial_identifier]->GetBinContent(1) << " " << h_out_syst_up[partial_identifier]->GetBinContent(1) << std::endl; 

	} // end loop on pT bins

        // Canvas
        TH1D *h_tmp_tot_up = (TH1D*)h_out_nom[partial_identifier]->Clone((string("tmp_tot_up") + partial_identifier).c_str());
        TH1D *h_tmp_tot_down = (TH1D*)h_out_nom[partial_identifier]->Clone((string("tmp_tot_down") + partial_identifier).c_str());

        h_tmp_tot_up->Multiply(h_out_tot_up[partial_identifier]);
        h_tmp_tot_down->Multiply(h_out_tot_down[partial_identifier]);
        h_tmp_tot_up->Add(h_out_nom[partial_identifier]);
        h_tmp_tot_down->Add(h_out_nom[partial_identifier]);

        h_tmp_tot_up->SetLineColor(kGreen-8);
        h_tmp_tot_up->SetFillColor(kGreen-8);
        h_tmp_tot_up->SetStats(0);
        h_tmp_tot_up->GetXaxis()->SetLabelSize(0.05);
        h_tmp_tot_up->GetYaxis()->SetLabelSize(0.05);
        h_tmp_tot_up->GetYaxis()->SetTitleSize(0.05);
        h_tmp_tot_up->GetXaxis()->SetTitle("p_{T}^{jet} [GeV]");
        h_tmp_tot_up->GetXaxis()->SetTitleSize(0.05);
        h_tmp_tot_up->GetXaxis()->SetTitleOffset(0.70);
        h_tmp_tot_up->SetTitle("");

        h_out_nom[partial_identifier]->SetStats(0);
        h_out_nom[partial_identifier]->GetXaxis()->SetLabelSize(0.05);
        h_out_nom[partial_identifier]->GetYaxis()->SetLabelSize(0.05);
        h_out_nom[partial_identifier]->GetYaxis()->SetTitleSize(0.05);
        h_out_nom[partial_identifier]->GetXaxis()->SetTitle("p_{T}^{jet} [GeV]");
        h_out_nom[partial_identifier]->GetXaxis()->SetTitleSize(0.05);
        h_out_nom[partial_identifier]->GetXaxis()->SetTitleOffset(0.70);
        h_out_nom[partial_identifier]->SetTitle("");

        // Custom w.r.t var
        std::string title = "Mistag rate scale factor";
        double max = 3.18;
        double min = 0.85;
        if(vars[0]=="kll")
        {
          title = "MC LF corrections";
          max = 20.;
          min = 1.;
        }
        else if(vars[0]=="khf")
        {
          title = "MC HF corrections";
          max = 1.1;
          min = 0.;
        }
        else if(vars[0]=="eps_neg_d")
        {
          title = "Data negative tag efficiency";
          max = 0.12; 
          min = 0.; 
        } 
        else if(vars[0]=="eps_d")
        {
          title = "LF mistag rate";
          max = 0.2; 
          min = 0.; 
        } 
        h_tmp_tot_up->SetMaximum(max);
        h_tmp_tot_up->SetMinimum(min);
        h_tmp_tot_up->GetYaxis()->SetTitle(title.c_str());

        h_out_nom[partial_identifier]->SetMaximum(max);
        h_out_nom[partial_identifier]->SetMinimum(min);
        h_out_nom[partial_identifier]->GetYaxis()->SetTitle(title.c_str());

        //       
        h_tmp_tot_down->SetLineColor(kGreen-8);
        h_tmp_tot_down->SetFillColor(10);

        h_out_nom[partial_identifier]->SetLineWidth(2); 
        h_out_nom[partial_identifier]->SetLineColor(kBlack); 
        h_out_nom[partial_identifier]->SetMarkerStyle(20); 
        h_out_nom[partial_identifier]->SetMarkerSize(1.2); 

        TLegend* legend = new TLegend(0.13467,0.679325,0.335244,0.829114);
        legend->SetBorderSize(0);  // no border
        legend->SetFillColor(0);   // Legend background should be white
        legend->SetTextSize(0.04); // Increase entry font size

        std::string legend_entry_var;
        if(vars[0]=="sf") legend_entry_var = "Scale factor";
        else if(vars[0]=="kll") legend_entry_var = "K_{ll}^{MC}";
        else if(vars[0]=="khf") legend_entry_var = "K_{HF}^{MC}";
        else if(vars[0]=="eps_neg_d") legend_entry_var = "#epsilon^{neg}_{data}";
        else if(vars[0]=="eps_d") legend_entry_var = "Neg. tag method";

        std::string legend_entry_1 = legend_entry_var + " (stat. unc.)";
        std::string legend_entry_2 = legend_entry_var + "  (stat. + syst. unc.)";
        legend->AddEntry(h_out_nom[partial_identifier],legend_entry_1.c_str(), "LPE");
        if(!vars[0].Contains("eps_neg_d")) legend->AddEntry(h_tmp_tot_up,legend_entry_2.c_str(), "F");

        TPaveText *pt = new TPaveText(0.631805,0.71097,0.750716,0.860759,"brNDC");
        pt->SetBorderSize(0);
        pt->SetFillColor(0);
        pt->SetTextSize(0.04);
        pt->SetTextFont(42);
        std::string eta_region = "| #eta^{jet} | < 1.2";
        if(ieta==2) eta_region = "1.2 < | #eta^{jet} | < 2.5";
        //pt->AddText("#sqrt{s} = 13 TeV, 0.02 (prescaled) to 36100 pb  ^{-1} (unprescaled)");
	pt->AddText("#sqrt{s} = 13 TeV, 36.1 fb^{-1}, rel21");
        pt->AddText((tagger + string(", #epsilon_{b} = ") + wp_label + string("%    ") +  eta_region).c_str());
/*         
        TPaveText *pt2 = new TPaveText(0.191977,0.677215,0.310888,0.827004,"brNDC");
        pt2->SetBorderSize(0);
        pt2->SetFillColor(0);
        pt2->SetTextSize(0.04);
        pt2->SetTextFont(72);
        pt2->AddText("2016 configuration");
*/
        c_out_final[partial_identifier] = new TCanvas((string("canvas") + partial_identifier).c_str());
        c_out_final[partial_identifier]->SetLogx();
        c_out_final[partial_identifier]->SetTicky();
        c_out_final[partial_identifier]->SetTickx();

        // no systematic for eps_neg_data
        if(!vars[0].Contains("eps_neg_d"))
        {
          h_tmp_tot_up->Draw("HIST");
          h_tmp_tot_down->Draw("HISTSAME");
        }
        // add Pythia predictions for eps_d only
        if(vars[0].Contains("eps_d"))
        {
          TH1D *h_pythia = (TH1D*)h_out_nom[partial_identifier]->Clone((string("pytia_") + partial_identifier).c_str());
          // Pythia nominal is eps_d / SF
          // total_sf.root MUST EXIST
          TFile *f_sf = new TFile("tot_systematics/total_sf.root", "READ");       
          TH1D *h_sf = (TH1D*)f_sf->Get(("sf_nom" + partial_identifier).c_str());          

          h_pythia->Divide(h_sf);          
          h_pythia->SetLineColor(kRed+1);
          h_pythia->Draw("HISTSAME"); 
          legend->AddEntry(h_pythia, "Pythia 8 MC", "L");
        }

        h_out_nom[partial_identifier]->Draw("SAME");

        legend->Draw();
        pt->Draw();
//        pt2->Draw();
        ATLASLabel(0.15,0.83, "Internal",kBlack);
        h_tmp_tot_up->Draw("AXISSAME"); 
      }
      ++iwp;
    }
  }

  f_out->cd();
  for (auto histogram: h_out_syst_up){
    histogram.second->Write();
  }
  for (auto histogram: h_out_syst_down){
    histogram.second->Write();
  }
  for (auto histogram: h_out_calib_up){
    histogram.second->Write();
  }
  for (auto histogram: h_out_calib_down){
    histogram.second->Write();
  }
  for (auto histogram: h_out_stat){
    histogram.second->Write();
  }
  for (auto histogram: h_out_nom){
    histogram.second->Write();
  }
  for (auto histogram: h_out_tot_up){
    histogram.second->Write();
  }
  for (auto histogram: h_out_tot_down){
    histogram.second->Write();
  }
  for (auto canvas: c_out_final){
    canvas.second->Write();
  }
  f_out->Close();
  
  return 0;
}
