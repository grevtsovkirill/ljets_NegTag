#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TSystem.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "string.h"
using namespace std;

#include "../config/conf.hpp"
#include "../helpers/OutputHelper.hpp"
#include "../helpers/parser.hpp"

// compute efficiencies
void calculate_epsilon(TH1D* Hists, double& val, double wp){

  TH1* hist = Hists; // classic weighted histogram
  val = 0;

  // first bin to be included. Should be the lower edge of the bin (see NtupleReader).
  int ix = hist->GetXaxis()->FindBin(wp);  
  double lowEdge = hist->GetBinLowEdge(ix);
  if (wp - lowEdge > 0) cout << "----> WARNING: difference in wp and bin edge: " << wp << "\t" << lowEdge <<endl;

  // Total number of bins
  int nBins = hist->GetNbinsX();

  double numerator = hist->Integral(ix, nBins); 
  double denominator = hist->Integral();

  if (denominator == 0) 
  {
    cout << "----> ERROR: histogram error in 0, wp is " << wp << endl; 
    return;
  }

  val = numerator/denominator; // efficiency computation
  if (val > 1) cout << "----> ERROR: efficiency > 1: " << val <<endl;
  if (val < 0) cout << "----> ERROR: efficiency < 0: " << val <<endl;
}




// Function execute in main
int path_eps(string sfold="std", int bootstrap_bkeeper=0)
{
  auto kin_labels = getKinLabels(); // take [ipt][ieta] and returns a string "ptXetaY"

  auto flavors = conf::flav_list; flavors.push_back(""); // flavour_list. add "" for inclusive.

  //---- Get Nominal MC HF fraction in pT/eta if generator uncertainty
  TFile *f_fracHF; 
  TH2D *h_fracc_Pythia;
  TH2D *h_fracb_Pythia;
  if(sfold.find("generator")!= std::string::npos)
  {
    std::string fracHF_path = "../TemplateFit/template_fit.root.pTetaCorrections";
    if(sfold.find("subleadingjet_") != std::string::npos) fracHF_path = "../TemplateFit/template_fit.root.subldg.pTetaCorrections";
    f_fracHF = new TFile(fracHF_path.c_str(),"read");
    h_fracc_Pythia = (TH2D*)f_fracHF->Get("h_fracc_mc");
    h_fracb_Pythia = (TH2D*)f_fracHF->Get("h_fracb_mc");
  }

  //---- Get external weights for doJetsExternal uncertainties
  //  depend on flavour and mode and pT/eta
  map<int, map<int, map<string, map<string, TH1D*>>>> h_rew_rewmode_pt_eta_f_m;
  TFile *f_MCbased;
  if(sfold.find("d0smearing") != std::string::npos || 
     sfold.find("z0smearing") != std::string::npos || 
     sfold.find("faketracks") != std::string::npos || 
     sfold.find("trackrecoeff") != std::string::npos )
  {
    //std::string fileName_MCbased = "../external/effPlot_fineEta_wC_FixedCutBEff_ALL.root";
    std::string fileName_MCbased = "../external/effPlot_FixedCutBEff_ALL_dijetPy8_25M.root";
    f_MCbased = new TFile(fileName_MCbased.c_str(), "read");
    std::string base_histoName_LF = "mv2c10wp_LF_";
    std::string base_histoName_LF_flip= "mv2c10wpFlip_LF_";
    std::string base_histoName_C= "mv2c10wp_C_";
    std::string base_histoName_C_flip= "mv2c10wpFlip_C_";
    std::string base_histoName_B= "mv2c10wp_B_";
    std::string base_histoName_B_flip= "mv2c10wpFlip_B_";

    for (auto p_pt: kin_labels) 
    {
      for (auto p_eta: p_pt.second)
      {
        std::string prefix="cen";
        if(p_eta.second==2) prefix="frw";        
    
        prefix += "Eta_pt_"; 

        std::string suffix="";
        if(sfold.find("d0smearing")!= std::string::npos) suffix = "RES_D0_MEAS"; 
        else if(sfold.find("z0smearing")!= std::string::npos) suffix = "RES_Z0_MEAS"; 
        else if(sfold.find("faketracks")!= std::string::npos) suffix = "FAKE_RATE_LOOSE"; 
        //else if(sfold=="trackrecoeff") suffix = "TO_BE_FILLED"; PLACE HOLDER 
        std::string histoName_LF = prefix + base_histoName_LF + suffix + "_var";
        std::string histoName_LF_flip = prefix + base_histoName_LF_flip + suffix + "_var";
        std::string histoName_C = prefix + base_histoName_C + suffix + "_var";
        std::string histoName_C_flip = prefix + base_histoName_C_flip + suffix + "_var";
        std::string histoName_B = prefix + base_histoName_B + suffix + "_var";
        std::string histoName_B_flip = prefix + base_histoName_B_flip + suffix + "_var";

        std::cout << histoName_LF << std::endl;

        h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"][""] = ((TH2D*)f_MCbased->Get(histoName_LF.c_str()))->ProjectionY((histoName_LF + to_string(p_pt.first)).c_str(),p_pt.first,p_pt.first);

        h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"][""] = ((TH2D*)f_MCbased->Get(histoName_C.c_str()))->ProjectionY((histoName_C + to_string(p_pt.first)).c_str(),p_pt.first,p_pt.first);

        h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"][""] = ((TH2D*)f_MCbased->Get(histoName_B.c_str()))->ProjectionY((histoName_B + to_string(p_pt.first)).c_str(),p_pt.first,p_pt.first);

        h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"]["neg"] = ((TH2D*)f_MCbased->Get(histoName_LF_flip.c_str()))->ProjectionY((histoName_LF_flip + to_string(p_pt.first)).c_str(),p_pt.first,p_pt.first);

        h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"]["neg"] = ((TH2D*)f_MCbased->Get(histoName_C_flip.c_str()))->ProjectionY((histoName_C_flip + to_string(p_pt.first)).c_str(),p_pt.first,p_pt.first);

        h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"]["neg"] = ((TH2D*)f_MCbased->Get(histoName_B_flip.c_str()))->ProjectionY((histoName_B_flip + to_string(p_pt.first)).c_str(),p_pt.first,p_pt.first);

        // set weight to 1 if less than 2 sigma away from 1
        for(int ibin=1; ibin<h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"][""]->GetNbinsX()+1; ibin++)
        {
          if(h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"][""]->GetBinContent(ibin)==0 || abs(1-h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"][""]->GetBinContent(ibin)) < 2*h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"][""]->GetBinError(ibin)) h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"][""]->SetBinContent(ibin, 1.);

          if(h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"][""]->GetBinContent(ibin)==0 || abs(1-h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"][""]->GetBinContent(ibin)) < 2*h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"][""]->GetBinError(ibin)) h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"][""]->SetBinContent(ibin, 1.);

          if(h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"][""]->GetBinContent(ibin)==0 || abs(1-h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"][""]->GetBinContent(ibin)) < 2*h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"][""]->GetBinError(ibin)) h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"][""]->SetBinContent(ibin, 1.);

          if(h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"]["neg"]->GetBinContent(ibin)==0 || abs(1-h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"]["neg"]->GetBinContent(ibin)) < 2*h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"]["neg"]->GetBinError(ibin)) h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["l"]["neg"]->SetBinContent(ibin, 1.);

          if(h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"]["neg"]->GetBinContent(ibin)==0 || abs(1-h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"]["neg"]->GetBinContent(ibin)) < 2*h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"]["neg"]->GetBinError(ibin)) h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["c"]["neg"]->SetBinContent(ibin, 1.);

          if(h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"]["neg"]->GetBinContent(ibin)==0 || abs(1-h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"]["neg"]->GetBinContent(ibin)) < 2*h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"]["neg"]->GetBinError(ibin)) h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first]["b"]["neg"]->SetBinContent(ibin, 1.);

        }

      }
    }
  }


  //------MAP OF THE TAGGER HISTOGRAMS IN MC------// 
  // [ipt] [ieta] [flav] [tagger] [mode("" or neg)] - only weighted "w"
  // bootstrap
  // [ipt] [ieta] [incl] [tagger] [neg)] - only weighted "w"
  // [ipt] [ieta] [l] [tagger] [mode("" or neg)] - only weighted "w"

  // MC
  map<int, map<int, map<string, map<string, map<string, TH1D*>>>>> h_mc;
  map<int, map<int, map<string, map<string, map<string, TH1D*>>>>> h_mc_bootstrap_mc;
  map<int, map<int, map<string, map<string, map<string, TH1D*>>>>> h_mc_bootstrap_data;
  TFile* ff;
  if(sfold.find("mcstat")==std::string::npos && sfold.find("datastat")==std::string::npos)
  {
    std::string ff_name = "res/" + sfold + "/mc.root";
    if(sfold.find("d0smearing") != std::string::npos || 
       sfold.find("z0smearing") != std::string::npos || 
       sfold.find("faketracks") != std::string::npos || 
       sfold.find("trackrecoeff") != std::string::npos )
    {
      ff_name = "res/FlavourTagging_Nominal/mc.root";
      if(sfold.find("subleadingjet") != std::string::npos) ff_name = "res/subleadingjet/mc.root";
    }

    ff = new TFile(ff_name.c_str()); // MC file
    for (auto tagger: conf::tagger_list) {
      for (auto p_pt: kin_labels) {
    
        int ipt = p_pt.first; // pT bin number

        for (auto p_eta: p_pt.second){

          int ieta = p_eta.first; // eta bin number

          for (string mode: {"","neg"}){
            for (auto flav: flavors) {
            
              // Apply reweighting for rewmode systematics for LF
              // inclusive must be recomputed
              if( tagger=="MV2c10" && (sfold.find("d0smearing") != std::string::npos || 
                                       sfold.find("z0smearing") != std::string::npos || 
                                       sfold.find("faketracks") != std::string::npos || 
                                       sfold.find("trackrecoeff") != std::string::npos ))
              {
                if(flav!="")
                { 
                  h_mc[p_pt.first][p_eta.first][flav][tagger][mode] =
                  (TH1D*)(ff->Get((mode + "w_" + flav + "_" + p_eta.second + "_" + tagger).c_str()));
                  h_mc[p_pt.first][p_eta.first][flav][tagger][mode]->Multiply(h_rew_rewmode_pt_eta_f_m[p_pt.first][p_eta.first][flav][mode]);
                }
                // inclusive comes after l,c and b have been computed
                else if(flav=="")
                {
                  h_mc[p_pt.first][p_eta.first][flav][tagger][mode] = (TH1D*)h_mc[p_pt.first][p_eta.first]["l"][tagger][mode]->Clone((mode + "w_" + flav + "_" + p_eta.second + "_" + tagger).c_str());
                  h_mc[p_pt.first][p_eta.first][flav][tagger][mode]->Add(h_mc[p_pt.first][p_eta.first]["c"][tagger][mode]);
                  h_mc[p_pt.first][p_eta.first][flav][tagger][mode]->Add(h_mc[p_pt.first][p_eta.first]["b"][tagger][mode]);
                }
              }     
              // HERWIG ++ inclusive -> same HF than Pythia
              else if( sfold.find("generator")!= std::string::npos && flav=="")
              {
                  // Total normalization from l, b and c
                  double Ntot = h_mc[p_pt.first][p_eta.first]["l"][tagger][mode]->Integral() + h_mc[p_pt.first][p_eta.first]["c"][tagger][mode]->Integral() + h_mc[p_pt.first][p_eta.first]["b"][tagger][mode]->Integral();
                  // inclusive HERWIG histo with Pythia HF fractions
                  h_mc[p_pt.first][p_eta.first][flav][tagger][mode] = (TH1D*)h_mc[p_pt.first][p_eta.first]["c"][tagger][mode]->Clone((mode + "w_" + flav + "_" + p_eta.second + "_" + tagger).c_str());
                  h_mc[p_pt.first][p_eta.first][flav][tagger][mode]->Scale(h_fracc_Pythia->GetBinContent(ipt, ieta)*Ntot/h_mc[p_pt.first][p_eta.first][flav][tagger][mode]->Integral());
                  h_mc[p_pt.first][p_eta.first][flav][tagger][mode]->Add(h_mc[p_pt.first][p_eta.first]["b"][tagger][mode], h_fracb_Pythia->GetBinContent(ipt, ieta)*Ntot/h_mc[p_pt.first][p_eta.first]["b"][tagger][mode]->Integral());
                  h_mc[p_pt.first][p_eta.first][flav][tagger][mode]->Add(h_mc[p_pt.first][p_eta.first]["l"][tagger][mode], (1-h_fracb_Pythia->GetBinContent(ipt, ieta)-h_fracc_Pythia->GetBinContent(ipt, ieta))*Ntot/h_mc[p_pt.first][p_eta.first]["l"][tagger][mode]->Integral());
              }
              // regular case
              else h_mc[p_pt.first][p_eta.first][flav][tagger][mode] = (TH1D*)(ff->Get((mode + "w_" + flav + "_" + p_eta.second + "_" + tagger).c_str()));

            } // end loop flavors
          }
        }
      }
    }
  } 
  else
  {
    std::string ff_name="";
    if(sfold.find("subleadingjet")!= std::string::npos) ff_name="res/subleadingjet/mc.root";
    else                               ff_name="res/FlavourTagging_Nominal/mc.root";  

    ff = new TFile(ff_name.c_str()); // MC file
    for (auto tagger: conf::tagger_list) {
      for (auto p_pt: kin_labels) {
        for (auto p_eta: p_pt.second){
          for (string mode: {"","neg"}){
            for (auto flav: {"l",""}) {
              if(flav=="" && mode=="") continue; // no need of eps_incl, only eps^neg_incl needed
              if(sfold.find("mcstat")!=std::string::npos) h_mc_bootstrap_mc[p_pt.first][p_eta.first][flav][tagger][mode] =
              (TH1D*)(ff->Get(("bootstrap/" + mode + "w_" + flav + "_" + p_eta.second + "_" + tagger + "_mc_" + to_string(bootstrap_bkeeper)).c_str()));
              else if(sfold.find("datastat")!=std::string::npos) h_mc_bootstrap_data[p_pt.first][p_eta.first][flav][tagger][mode] = (TH1D*)(ff->Get(("bootstrap/" + mode + "w_" + flav + "_" + p_eta.second + "_" + tagger + "_data_" + to_string(bootstrap_bkeeper)).c_str())); // change in mc due to reweighting factors
            }
          }
        }
      }
    }
  }

 
  // DATA: [ipt] [ieta] [tagger] ONLY 
  // bootstrap
  // [ipt] [ieta] [tagger]
  map<int, map<int, map<string, TH1D*>>> h_data_neg;
  map<int, map<int, map<string, TH1D*>>> h_data_neg_bootstrap_data;
  TFile* ff_data;
  if(sfold.find("datastat")==std::string::npos)
  {
    std::string ff_name2 = "res/" + sfold + "/data.root";
    if(sfold=="mcstat" || sfold=="d0smearing" || sfold=="z0smearing" || sfold=="faketracks" || sfold=="trackrecoeff") ff_name2 = "res/FlavourTagging_Nominal/data.root";
    else if(sfold=="mcstat_subleadingjet" || sfold=="subleadingjet_d0smearing" || sfold=="subleadingjet_z0smearing" || sfold=="subleadingjet_faketracks" || sfold=="subleadingjet_trackrecoeff") ff_name2 = "res/subleadingjet/data.root";
    ff_data = new TFile(ff_name2.c_str()); // Data file
    for (auto tagger: conf::tagger_list) {
      for (auto p_pt: kin_labels) {
        for (auto p_eta: p_pt.second){
            h_data_neg[p_pt.first][p_eta.first][tagger] = 
              (TH1D*)(ff_data->Get(("negw__" + p_eta.second + "_" + tagger ).c_str()));
        }
      }
    }
  } 
  else
  {
    std::string ff_data_name;
    if(sfold=="datastat") ff_data_name="res/FlavourTagging_Nominal/data.root";
    else if (sfold=="datastat_subleadingjet") ff_data_name="res/subleadingjet/data.root";
    ff_data = new TFile(ff_data_name.c_str()); // Data file
    for (auto tagger: conf::tagger_list) {
      for (auto p_pt: kin_labels) {
        for (auto p_eta: p_pt.second){
              h_data_neg_bootstrap_data[p_pt.first][p_eta.first][tagger] = 
                (TH1D*)(ff_data->Get(("bootstrap/negw__" + p_eta.second + "_" + tagger  + "_data_" + to_string(bootstrap_bkeeper)).c_str()));
        }
      }
    }
  }

 
  // inclusive negtag histograms with varied c/b fractions ONLY FOR NOMINAL
  TFile *f_data_fraction;
  //TFile *f_data_fraction_noCorr;
  if(sfold=="FlavourTagging_Nominal" || sfold=="subleadingjet")
  {
    // get fraction variations - only valid for MV2c10
    //"with_mcstat" includes histo with data (returned by the fit) and MC stat uncertainties (bootstrap)
    // "template_fit" includes only unc. from data (returned by the fit)
    std::string data_fraction_path = "../TemplateFit/with_mcstat.root.pTetaCorrections";
    if(sfold=="subleadingjet") data_fraction_path = "../TemplateFit/with_mcstat.root.subldg.pTetaCorrections";

    f_data_fraction = new TFile(data_fraction_path.c_str(),"read");
    TH2D *h_fracb = (TH2D*)f_data_fraction->Get("h_fracb_data_updated");
    TH2D *h_fracc = (TH2D*)f_data_fraction->Get("h_fracc_data_updated");
    TH2D *h_fracb_var = (TH2D*)f_data_fraction->Get("h_fracb_var_updated"); 
    TH2D *h_fracc_var = (TH2D*)f_data_fraction->Get("h_fracc_var_updated"); 
    TH2D *h_chi2 = (TH2D*)f_data_fraction->Get("h_chi2_updated"); 
/*
    f_data_fraction_noCorr = new TFile("../TemplateFit/with_mcstat.root.noCorrections","read");
    TH2D *h_fracb_noCorr = (TH2D*)f_data_fraction_noCorr->Get("h_fracb_data_updated");
    TH2D *h_fracc_noCorr = (TH2D*)f_data_fraction_noCorr->Get("h_fracc_data_updated");
    TH2D *h_fracb_var_noCorr = (TH2D*)f_data_fraction_noCorr->Get("h_fracb_var_updated"); 
    TH2D *h_fracc_var_noCorr = (TH2D*)f_data_fraction_noCorr->Get("h_fracc_var_updated"); 
    TH2D *h_chi2_noCorr = (TH2D*)f_data_fraction_noCorr->Get("h_chi2_updated"); 
*/
    for (auto tagger: conf::tagger_list){
      for (auto p_pt: kin_labels){
        for (auto p_eta: p_pt.second){

          string partial_identifier = p_eta.second + "_" + tagger;

          TH1D* h_negl = h_mc[p_pt.first][p_eta.first]["l"][tagger]["neg"];
          TH1D* h_negc = h_mc[p_pt.first][p_eta.first]["c"][tagger]["neg"];
          TH1D* h_negb = h_mc[p_pt.first][p_eta.first]["b"][tagger]["neg"];
          
          // get number of entries
          double sl = h_negl->Integral();  // switched to Integral() 
          double sc = h_negc->Integral();
          double sb = h_negb->Integral();

          // variation significance
          if(2*h_fracb_var->GetBinError(p_pt.first, p_eta.first) > abs(h_fracb_var->GetBinContent(p_pt.first, p_eta.first))) h_fracb->SetBinContent(p_pt.first, p_eta.first, sb/(sl+sc+sb) );
          if(2*h_fracc_var->GetBinError(p_pt.first, p_eta.first) > abs(h_fracc_var->GetBinContent(p_pt.first, p_eta.first))) h_fracc->SetBinContent(p_pt.first, p_eta.first, sc/(sl+sc+sb) );
/*
          if(2*h_fracb_var_noCorr->GetBinError(p_pt.first, p_eta.first) > abs(h_fracb_var_noCorr->GetBinContent(p_pt.first, p_eta.first))) h_fracb_noCorr->SetBinContent(p_pt.first, p_eta.first, sb/(sl+sc+sb) );
          if(2*h_fracc_var_noCorr->GetBinError(p_pt.first, p_eta.first) > abs(h_fracc_var_noCorr->GetBinContent(p_pt.first, p_eta.first))) h_fracc_noCorr->SetBinContent(p_pt.first, p_eta.first, sc/(sl+sc+sb) );
*/
          TH2D *h_fracc_var_chosen = h_fracc_var;
          TH2D *h_fracb_var_chosen = h_fracb_var;

          TH2D *h_fracc_chosen = h_fracc;
          TH2D *h_fracb_chosen = h_fracb;
/*
          // TEMPORARY
          // chose if one uses the corrected results based on chi2
          if(h_chi2_noCorr->GetBinContent(p_pt.first, p_eta.first)<0.7*h_chi2->GetBinContent(p_pt.first, p_eta.first) || h_chi2->GetBinContent(p_pt.first, p_eta.first)==0)
          {
            h_fracc_chosen = h_fracc_noCorr;
            h_fracb_chosen = h_fracb_noCorr;
            h_fracc_var_chosen = h_fracc_var_noCorr;
            h_fracb_var_chosen = h_fracb_var_noCorr;
            std::cout << "c/b fraction variations, fit with no corrections chosen for: pT: " << p_pt.first << ", eta: " << p_eta.first << std::endl;
          }
*/
          // TEMPORARY
          if(h_fracc_var_chosen->GetBinContent(p_pt.first, p_eta.first) > 2) 
          {
             h_fracc_var_chosen->SetBinContent(p_pt.first, p_eta.first, 2);
             h_fracc_chosen->SetBinContent(p_pt.first, p_eta.first, 3*sc/(sl+sc+sb));
          }

          std::cout << "pT: " << p_pt.first << ", eta: " << p_eta.first << std::endl;
          std::cout << "fracb variation: " <<  h_fracb_var_chosen->GetBinContent(p_pt.first, p_eta.first) << std::endl;
          std::cout << "fracc variation: " <<  h_fracc_var_chosen->GetBinContent(p_pt.first, p_eta.first) << std::endl;

          // add variation histograms
          TH1D* h_negHFv = (TH1D*)h_negl->Clone((string("HFnega_")+partial_identifier).c_str());
          h_mc[p_pt.first][p_eta.first]["HFv"][tagger]["neg"] = h_negHFv;
          h_negHFv->Scale( (1 - h_fracc_chosen->GetBinContent(p_pt.first, p_eta.first) 
                            - h_fracb_chosen->GetBinContent(p_pt.first, p_eta.first)) );
          h_negHFv->Add(h_negc, h_fracc_chosen->GetBinContent(p_pt.first, p_eta.first)*(sl+sb+sc)/sc ); 
          h_negHFv->Add(h_negb, h_fracb_chosen->GetBinContent(p_pt.first, p_eta.first)*(sl+sb+sc)/sb ); 
        }
      }
    }
  }

  // PLOTS vs pT as a function of [tagger][wp][ieta][prefix]
  map<std::string, map<double, map<int, map< std::string, TH1D*>>>> h_plots;

  // prefixes with LF(l)/INCLUSIVE(a)/DATA(d) efficiencies, 
  // kll, khf, sf, 
  // variations: frac_c on eps_a_neg, frac_b on eps_a_neg, c/b neg-efficiencies on eps_a_neg
  vector<string> plot_prefixes = {"eps_l", "eps_l_neg", "eps_a_neg", "eps_neg_d", "eps_d", "kll", "khf", "sf"};

  if(sfold=="FlavourTagging_Nominal" || sfold=="subleadingjet")
  {
    plot_prefixes.push_back("fracHF");
    plot_prefixes.push_back("eps_c_neg");
    plot_prefixes.push_back("eps_b_neg");
  }

  for (auto tagger: conf::tagger_list){
    int iwp = 0; // incremented 

    // array with the WP names (85, 77 ,etc)
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second; 
    // array with the cuts to be applied to the tagger to get the corresponding WPs
    auto wpoint_cuts = conf::wpoint_map.find(tagger)->second; 

    for (auto wp: wpoint_cuts){
      string wp_label = to_string(wpoint_title[iwp]);
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;
        string eta_title;
	if (ieta == 1) eta_title = "central";
	else eta_title = "forward";

	for (auto prefix: plot_prefixes){

          string identifier = prefix + "_" + tagger + "_w" + wp_label + "_eta" + to_string(ieta);
          if(sfold.find("mcstat")!=std::string::npos) identifier += "_mcstat_" + to_string(bootstrap_bkeeper);
          else if(sfold.find("datastat")!=std::string::npos) identifier += "_datastat_" + to_string(bootstrap_bkeeper);

	  string title = tagger + " WP = " + wp_label + "%, eta region " + eta_title;
	    
          h_plots[tagger][wp][ieta][prefix] = new TH1D(identifier.c_str(), title.c_str(), conf::n_pt, conf::pt_lowedges);
	}
      }
      ++iwp;
    }
  }

  // calculate all histograms
  // PLOTS vs pT as a function of [tagger][wp][ieta][prefix]
  for (auto tagger: conf::tagger_list){

    int iwp = 0; // incremented 
    // array with the WP names (85, 77 ,etc)
    auto wpoint_title = conf::wpoint_labels.find(tagger)->second; 
    // array with the cuts to be applied to the tagger to get the corresponding WPs
    auto wpoint_cuts = conf::wpoint_map.find(tagger)->second; 

    for (auto wp: wpoint_cuts){
      for (auto p_pt: kin_labels){
	for (auto p_eta: p_pt.second){
	  auto cur_histo = h_plots[tagger][wp][p_eta.first];

          //--------------MC (neg) mis-tag rates--------------//
          // done for all	
 
          // mis-tag rate light jets (MC) 
          // neg-tag rate light jets (MC)
          // incl neg-tag rate (MC)
	  double eps_l=0;
	  double eps_l_neg=0; 
	  double eps_a_neg=0; 

          if(sfold.find("mcstat")!=std::string::npos)
          {
            calculate_epsilon(h_mc_bootstrap_mc[p_pt.first][p_eta.first]["l"][tagger][""], eps_l, wp);
            calculate_epsilon(h_mc_bootstrap_mc[p_pt.first][p_eta.first]["l"][tagger]["neg"], eps_l_neg, wp);
            calculate_epsilon(h_mc_bootstrap_mc[p_pt.first][p_eta.first][""][tagger]["neg"], eps_a_neg, wp);
          }
          else if(sfold.find("datastat")!=std::string::npos)
          {
            calculate_epsilon(h_mc_bootstrap_data[p_pt.first][p_eta.first]["l"][tagger][""], eps_l, wp);
            calculate_epsilon(h_mc_bootstrap_data[p_pt.first][p_eta.first]["l"][tagger]["neg"], eps_l_neg, wp);
            calculate_epsilon(h_mc_bootstrap_data[p_pt.first][p_eta.first][""][tagger]["neg"], eps_a_neg, wp);
          }
          else
          {
	    calculate_epsilon(h_mc[p_pt.first][p_eta.first]["l"][tagger][""], eps_l, wp);
	    calculate_epsilon(h_mc[p_pt.first][p_eta.first]["l"][tagger]["neg"], eps_l_neg, wp);
	    calculate_epsilon(h_mc[p_pt.first][p_eta.first][""][tagger]["neg"], eps_a_neg, wp);
          }

	  cur_histo["eps_l"]->SetBinContent(p_pt.first, eps_l);
	  cur_histo["eps_l"]->SetBinError(p_pt.first, 0.);
	  cur_histo["eps_l_neg"]->SetBinContent(p_pt.first, eps_l_neg);
	  cur_histo["eps_l_neg"]->SetBinError(p_pt.first, 0.);
	  cur_histo["eps_a_neg"]->SetBinContent(p_pt.first, eps_a_neg);
	  cur_histo["eps_a_neg"]->SetBinError(p_pt.first, 0.);

          //--------------Data neg mis-tag rates--------------//
          // done for all	

          // neg-tag rate on data 
	  double eps_neg_d=0; 
          if(sfold.find("datastat")!=std::string::npos) calculate_epsilon(h_data_neg_bootstrap_data[p_pt.first][p_eta.first][tagger], eps_neg_d, wp);
          else calculate_epsilon(h_data_neg[p_pt.first][p_eta.first][tagger], eps_neg_d, wp);

	  cur_histo["eps_neg_d"]->SetBinContent(p_pt.first, eps_neg_d);
	  cur_histo["eps_neg_d"]->SetBinError(p_pt.first, 0.);

          //--------------Data corrected mis-tag rates--------------//
          // done for all

          // correction to account for the expected differences in pos/neg tag
          // due to long-lived particles (K_S, Lambda, etc), MC
	  double kll = -999; 
	  if (eps_l_neg > 0) kll = eps_l/eps_l_neg;

	  cur_histo["kll"]->SetBinContent(p_pt.first, kll);
	  cur_histo["kll"]->SetBinError(p_pt.first, 0.);
	 
          // correction factor for c and b contamination (MC) 
	  double khf = -999; 
	  if (eps_a_neg > 0) khf = eps_l_neg/eps_a_neg;
	  cur_histo["khf"]->SetBinContent(p_pt.first, khf);
	  cur_histo["khf"]->SetBinError(p_pt.first, 0.);

          // corrected tag rate from data
	  double eps_d = khf*kll*eps_neg_d; // estimated mis-tag rate data
	  cur_histo["eps_d"]->SetBinContent(p_pt.first, eps_d);
	  cur_histo["eps_d"]->SetBinError(p_pt.first, 0.);

          //-----------------FINAL SF----------------//

          // SF : DENOMINATOR, eps_l FROM NOMINAL MC WITH NO TRACK REWEIGHTING ("notrackrew")
          // NEED TO BE CAREFUL IF NEW REWEIGHTING ARE APPLIED
	  double sf = 0;
          double eps_l_NOMINAL = 0;
          if(sfold=="notrackrew" || sfold=="notrackrew_subleadingjet") eps_l_NOMINAL = eps_l;
          else
          {
            //std::cout << "SF computation required eps_l for nominal MC, looking in raw_systematics/notrackrew_db.root" << std::endl;
            std::string histo_name =  cur_histo["eps_l"]->GetName();

            if(sfold.find("mcstat")!=std::string::npos)
            {
              size_t pos = histo_name.find("mc");
              histo_name.erase(pos-1, histo_name.size()-1);
            }
            else if(sfold.find("datastat")!=std::string::npos)
            {
              size_t pos = histo_name.find("data");
              histo_name.erase(pos-1, histo_name.size()-1);
            }
            //std::cout << histo_name << std::endl;

            std::string f_ntrack_name = "raw_systematics/notrackrew_db.root";
            if(sfold.find("subleadingjet")!=std::string::npos) f_ntrack_name = "raw_systematics/notrackrew_subleadingjet_db.root";
            TFile *f_ntrack = new TFile(f_ntrack_name.c_str(),"read");
            TH1D *h_eps_l_ntrack = (TH1D*)f_ntrack->Get(histo_name.c_str());
            eps_l_NOMINAL = h_eps_l_ntrack->GetBinContent(p_pt.first);
            //std::cout << eps_l_NOMINAL << std::endl;
            f_ntrack->Close();
          }

          if(eps_l_NOMINAL==0) sf = 0; // can happen for very tight WP (no stat) 
	  else sf = eps_d/eps_l_NOMINAL;
	  cur_histo["sf"]->SetBinContent(p_pt.first, sf);
	  cur_histo["sf"]->SetBinError(p_pt.first, 0.);


          //---------------HF variations---------------//
          // only for FlavourTagging_Nominal and subleadingjet
          if(sfold=="FlavourTagging_Nominal" || sfold=="subleadingjet")
          {
            //std::cout << p_pt.first << " " << p_eta.first << " " << tagger << " " << wp << std::endl; 
	    //----------------------------------------------------------------------
	    // effect of variation of b/c jet fractions in eps_a_neg: determined from data.
	    double eps_a_neg_HF=0;
	    calculate_epsilon(h_mc[p_pt.first][p_eta.first]["HFv"][tagger]["neg"], eps_a_neg_HF, wp);

            // relative variation of the measurement
	    double relval_HF = 0;
	    if (eps_a_neg>0) 
            {
              relval_HF = (eps_a_neg/eps_a_neg_HF)-1;
	    }

	    cur_histo["fracHF"]->SetBinContent(p_pt.first, relval_HF);
	    cur_histo["fracHF"]->SetBinError(p_pt.first, 0.);

	    //----------------------------------------------------------------------
            // effect of variation of b/c jet negative tagging efficiencies
	    double releff_c = 0, releff_b = 0;
	   
            // c and b tag nominal efficiencies 
            double eps_c_neg = 0;
	    calculate_epsilon(h_mc[p_pt.first][p_eta.first]["c"][tagger]["neg"], eps_c_neg, wp);
	    double eps_b_neg = 0;
	    calculate_epsilon(h_mc[p_pt.first][p_eta.first]["b"][tagger]["neg"], eps_b_neg, wp);

	    TH1D* h_negl = h_mc[p_pt.first][p_eta.first]["l"][tagger]["neg"];
	    TH1D* h_negc = h_mc[p_pt.first][p_eta.first]["c"][tagger]["neg"];
	    TH1D* h_negb = h_mc[p_pt.first][p_eta.first]["b"][tagger]["neg"];

	    // get number of entries
	    double sl = h_negl->Integral(); // switched to Integral
	    double sc = h_negc->Integral();
	    double sb = h_negb->Integral();
	  
            // nominal c and b fractions and consistency check
	    double fc = sc/(sl+sc+sb);
	    double fb = sb/(sl+sc+sb);

//            double eps_a_neg_test = fc*eps_c_neg + fb*eps_b_neg + (1-fb-fc)*eps_l_neg;
//            std::cout << eps_a_neg_test << " and " << eps_a_neg << " should be equal. Otherwise, please worry." << endl; 
	  
	    // variations of eps_c_neg and eps_b_neg
            double eps_a_neg_ctag= fc*(1+conf::d_eps_c[iwp])*eps_c_neg + fb*eps_b_neg + (1-fb-fc)*eps_l_neg; 
            double eps_a_neg_btag = fc*eps_c_neg + fb*(1+conf::d_eps_b[iwp])*eps_b_neg + (1-fc-fb)*eps_l_neg; 
            
            // effect on measurement
	    releff_c = (eps_a_neg/eps_a_neg_ctag)-1; 
	    releff_b = (eps_a_neg/eps_a_neg_btag)-1;

	    cur_histo["eps_c_neg"]->SetBinContent(p_pt.first, releff_c);
	    cur_histo["eps_c_neg"]->SetBinError(p_pt.first, 0.);
	    cur_histo["eps_b_neg"]->SetBinContent(p_pt.first, releff_b);
	    cur_histo["eps_b_neg"]->SetBinError(p_pt.first, 0.);
          }
	}
      }
      iwp++;
    }
  }
  std::string f1_name = "raw_systematics/" + sfold + "_db";
  if(sfold.find("mcstat")!=std::string::npos || sfold.find("datastat")!=std::string::npos) f1_name += "_" + to_string(bootstrap_bkeeper) + ".root";
  else f1_name += ".root";

  TFile *f1 = new TFile(f1_name.c_str(), "RECREATE");
  for (auto tagger: conf::tagger_list){
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;
	for (auto prefix: plot_prefixes){
	  h_plots[tagger][wp][ieta][prefix]->Write();
	}
      }
    }
  }
  f1->Close();

  // deleting - painful with TH1D* in map ...
  for (auto tagger: conf::tagger_list){
    for (auto wp: conf::wpoint_map.find(tagger)->second){
      for (auto p_eta: kin_labels[1]){
	auto ieta = p_eta.first;
	for (auto prefix: plot_prefixes){
	  delete h_plots[tagger][wp][ieta][prefix];
	}
      }
    }
  }

  for(auto const& ent1 : h_mc) 
  {
    for(auto const& ent2 : ent1.second) 
    {
      for(auto const& ent3 : ent2.second) 
      {
        for(auto const& ent4 : ent3.second) 
        {
          for(auto const& ent5 : ent4.second) delete h_mc[ent1.first][ent2.first][ent3.first][ent4.first][ent5.first]; 
        }
      }
    }
  }

  for(auto const& ent1 : h_mc_bootstrap_mc) 
  {
    for(auto const& ent2 : ent1.second) 
    {
      for(auto const& ent3 : ent2.second) 
      {
        for(auto const& ent4 : ent3.second) 
        {
          for(auto const& ent5 : ent4.second) delete h_mc_bootstrap_mc[ent1.first][ent2.first][ent3.first][ent4.first][ent5.first]; 
        }
      }
    }
  }

  for(auto const& ent1 : h_mc_bootstrap_data) 
  {
    for(auto const& ent2 : ent1.second) 
    {
      for(auto const& ent3 : ent2.second) 
      {
        for(auto const& ent4 : ent3.second) 
        {
          for(auto const& ent5 : ent4.second) delete h_mc_bootstrap_data[ent1.first][ent2.first][ent3.first][ent4.first][ent5.first]; 
        }
      }
    }
  }

  for(auto const& ent1 : h_data_neg) 
  {
    for(auto const& ent2 : ent1.second) 
    {
      for(auto const& ent3 : ent2.second) delete h_data_neg[ent1.first][ent2.first][ent3.first]; 
    }
  }

  for(auto const& ent1 : h_data_neg_bootstrap_data) 
  {
    for(auto const& ent2 : ent1.second) 
    {
      for(auto const& ent3 : ent2.second) delete h_data_neg_bootstrap_data[ent1.first][ent2.first][ent3.first]; 
    }
  }


  ff->Close();
  ff_data->Close();
  if(sfold.find("d0smearing") != std::string::npos || 
     sfold.find("z0smearing") != std::string::npos || 
     sfold.find("faketracks") != std::string::npos || 
     sfold.find("trackrecoeff") != std::string::npos ) f_MCbased->Close(); 
  if(sfold=="FlavourTagging_Nominal" || sfold=="subleadingjet")
  {
    f_data_fraction->Close();
    //f_data_fraction_noCorr->Close();
  }

  h_rew_rewmode_pt_eta_f_m.clear();
  h_mc.clear();
  h_mc_bootstrap_mc.clear();
  h_mc_bootstrap_data.clear();
  h_data_neg.clear();
  h_data_neg_bootstrap_data.clear();
  h_plots.clear();

  if(sfold.find("generator")!= std::string::npos) f_fracHF->Close();

}

int main(int argc, char* argv[]) {

  cout << "calculate analysis variables" << endl;
  TString systematic="std";

  for ( int i1 = 1; i1 < argc; ++i1){ // start at 1 (0 script name)
    if (strcmp(argv[i1], "-s")==0){
        systematic = get_argument(argc, argv, i1);
      }
    else {
      cout << "argument not recognized: " << argv[i1] << endl;
    }
  }

  cout<<"working on " << systematic <<endl;

  if(systematic.Contains("mcstat") || systematic.Contains("datastat"))
  {
    for(int ibootstrap=1; ibootstrap<1001; ibootstrap++)
     {
        std::cout << ibootstrap << std::endl;
        path_eps(systematic.Data(),ibootstrap);
     }
  }
  else path_eps(systematic.Data());

  return 0;
}
