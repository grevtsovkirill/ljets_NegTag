#define NtupleReader_cxx
#include "NtupleReader.h"
#include <TH2.h>
#include <TFile.h>
#include <TSystem.h>
#include <TString.h>
#include <TRandom3.h>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <cassert>
#include <tuple>


#include "../config/conf.hpp"
#include "../helpers/OutputHelper.hpp"

using namespace std;

extern bool runmc;
const int debug =2;

// Complete a binning bin given min and max values
std::vector<double> extendBinRange(const std::vector<double> &bin_edges, double min = -1.0, double max = 1.0){
  std::vector<double> new_edges = bin_edges;

  auto it = new_edges.begin();
  new_edges.insert(it, min);
  new_edges.push_back(max);
  
  return new_edges;
}


//template<class T>

template< typename T > struct MyTagger
{
    // attributes   
  std::string name;
  T* wptr;
  T* wnegptr;
  TH1D* h;
  TH1D* hneg;
  // constructors
  //MyTagger(std::string aName, float* aWptr, float* aWnegptr)
  //MyTagger(std::string aName, TYPE aWptr, TYPE aWnegptr)
  MyTagger() {} 
  MyTagger(std::string aName, T* aWptr, T* aWnegptr)
  {
    name = aName;       // tagger name
    wptr = aWptr;       // weight
    wnegptr = aWnegptr; // wight of flipped version
  }    
  
  // init function
  void init() {
    // binning 
    //old = should start at -1 and end at +1.
    //new = retrieve from conf.hpp boarders
    auto tuple = conf::tag_hist.find(name)->second;
    std::vector<double> bins=extendBinRange(conf::wpoint_map.find(name)->second,std::get<1>(tuple),std::get<2>(tuple));
    
    if(debug == 2){ std::cout<< "Name = "<< name << ",  extendBinRange: "<<  (conf::wpoint_map.find(name)->second[1])  <<std::endl;  	std::cout<<"  , tag_hist " <<  std::get<0>(tuple) <<std::endl;    }
    // weight and negative weight tagger histogram 
    h = new TH1D(name.c_str(), name.c_str(), bins.size()-1, &(bins[0]));
    hneg = new TH1D((name+"neg").c_str(), (name + "neg").c_str(), bins.size()-1, &(bins[0]));
    h->Sumw2();
    hneg->Sumw2();
  }
  ~MyTagger() {}  // destructor
  
};



    // Main function
void NtupleReader::Loop(int bootstrap_bkeeper=0)
{



  //--------------------------Start of the NtupleReader::Loop()----------------------------

  if (fChain == 0) return;
  int currentTree = -1;
  string period_slice;

  //-----PREAMBULE STUFF----//
  // flavour_list. add "" for inclusive.
  auto flavors = conf::flav_list; flavors.push_back("");

  // Label bookkeeping
  const int nk = conf::n_kin; // total nb of bin, n_pt*n_eta
  auto kin_labels = getKinLabels(); // take [ipt][ieta] and returns a string "ptXetaY"

  // tagger declaration
  std::map<string, typename MyTagger> tagger_map; // map of the tagger objects
  
  //tagger_map["MV2c10"] = MyTagger("MV2c10", float_subtagger["jet_MV2c10"], float_subtagger["jet_MV2c10Flip"]);
  tagger_map["DL1"] = MyTagger<double>("DL1", double_subtagger["jet_DL1_w"], double_subtagger["jet_DL1Flip_w"]);
  
  if(debug == 2) std::cout << "  map of the tagger objects: " << float_subtagger["jet_MV2c10"] <<  "  "<< float_subtagger["jet_MV2c10"][0] <<  "  "<< float_subtagger["jet_MV2c10"][1] <<  "  "<< float_subtagger["jet_MV2c10"][2] << std::endl;
  //std::map <std::string, float[2]> float_subtagger;

  for (auto &tagger: tagger_map) tagger.second.init();


  //---- WEIGHTS -----//
  // Cumulative Weight bookeeping: [ipt][ieta]["",gen or raw] returns the corresponding weight
  map<int, map<int, map<string, double>>> weightpTeta_map;
  for (auto p_pt: kin_labels){
    for (auto p_eta: p_pt.second){
      weightpTeta_map[p_pt.first][p_eta.first][""] = 0.0; // used weights (i.e. gen+reweighting)
      weightpTeta_map[p_pt.first][p_eta.first]["gen"] = 0.0; // mc generator weights wo kin reweighting
      weightpTeta_map[p_pt.first][p_eta.first]["raw"] = 0.0; // raw event count
    }
  }

  // histograms containing weight after reweighting
  TH1D* h_wjet1 = new TH1D("wjet1", "wjet1", 100, 0,10);
  TH1D* h_wjet2 = new TH1D("wjet2", "wjet2", 100, 0,10);


  // ------Classify type of jets------
  //ks: +1, fake: +2, interaction: +4, conversion +8
  TH1D* h_jetCategory  = new TH1D("h_jetCategory", "h_jetCategory", 16, 0,16);


  //------MAP OF THE TAGGER HISTOGRAMS------// 
  // [ipt] [ieta] [flav] [tagger] [mode("" or neg)] - only weighted "w"
  map<int, map<int, map<string, map<string, map<string, TH1D*>>>>> h_pt_eta_f_t_m;
  if (bootstrap_bkeeper==0)
  {
    for (auto p_pt: kin_labels){
      for (auto p_eta: p_pt.second){
        for (auto flav: flavors) {
          for (auto p_tag: tagger_map) {
            for (string mode: {"", "neg"}){
              string identifier = mode + "w_" + flav + "_" + p_eta.second + "_" + p_tag.first;
              h_pt_eta_f_t_m[p_pt.first][p_eta.first][flav][p_tag.first][mode] = (TH1D*)p_tag.second.h->Clone(identifier.c_str());
            }
          }
        }
      }
    }
  }

  // bootstrap only what is needed "w", 1 bootstrap per job
  // [ipt] [ieta] [incl] [tagger] [neg)] [bootstrap_type]
  // [ipt] [ieta] [l] [tagger] [mode("" or neg)] [bootstrap_type]
  map<int, map<int, map<string, map<string, map<string, map<string, TH1D*>>>>>> h_pt_eta_f_t_m_bt;
  if(bootstrap_bkeeper>0)
  {
    for (auto p_pt: kin_labels){
      for (auto p_eta: p_pt.second){
        for (auto flav: flavors) {
          for (auto p_tag: tagger_map) {
            for (string mode: {"", "neg"}){
              for (string bootstrap_type: {"data", "mc"}){

                if(flav=="" && mode=="") continue; // no need of eps_incl, only eps^neg_incl needed
                if(!runmc && bootstrap_type=="mc") continue; // only one histo needed for data (no reweighting)
                if(!runmc && (flav=="b" || flav=="c")) continue; // required for template fit for mc only
                if(runmc && (flav=="b" || flav=="c") && mode=="neg") continue; // positive tag only

                string identifier = mode + "w_" + flav + "_" + p_eta.second + "_" + p_tag.first + "_" + bootstrap_type + "_" + to_string(bootstrap_bkeeper);
                h_pt_eta_f_t_m_bt[p_pt.first][p_eta.first][flav][p_tag.first][mode][bootstrap_type] = (TH1D*)p_tag.second.h->Clone(identifier.c_str());
                h_pt_eta_f_t_m_bt[p_pt.first][p_eta.first][flav][p_tag.first][mode][bootstrap_type]->SetDirectory(dir_bootstrap);
              }
            }
          }
        }
      }
    }
  }

  //------MAP OF THE OTHER TAGGER HISTOGRAMS------// - not considered for bootstrap 
  // [tagger name][flavor]
  map<string, map<string, TH1D*>> h_subtagger;
  if(bootstrap_bkeeper==0)
  {
    for (auto flav: conf::flav_list){
      for (const auto &name: subtagger::floats){
        int nbins;
        double binmin, binmax;
        std::tie(nbins, binmin, binmax) = name.second;
        h_subtagger[flav][name.first] = new TH1D((name.first+"_"+flav).c_str(), (name.first+"_"+flav).c_str(), nbins, binmin, binmax);
        h_subtagger[flav][name.first]->SetDirectory(dir_subtagger);
      }
      for (const auto &name: subtagger::ints){
        int nbins;
        double binmin, binmax;
        std::tie(nbins, binmin, binmax) = name.second;
        h_subtagger[flav][name.first] = new TH1D((name.first+"_"+flav).c_str(), (name.first+"_"+flav).c_str(), nbins, binmin, binmax);
        h_subtagger[flav][name.first]->SetDirectory(dir_subtagger);
      }
      for (const auto &name: subtagger::doubles){
        int nbins;
        double binmin, binmax;
        std::tie(nbins, binmin, binmax) = name.second;
        h_subtagger[flav][name.first] = new TH1D((name.first+"_"+flav).c_str(), (name.first+"_"+flav).c_str(), nbins, binmin, binmax);
        h_subtagger[flav][name.first]->SetDirectory(dir_subtagger);
      }
    }// flav
  }

  //------MAP OF THE PT/ETA HISTOGRAMS------// - not considered for bootstrap
  map<string, TH2D*> h2_pteta;
  if(bootstrap_bkeeper==0)
  {
    for(int ij=0; ij<2; ij++){
      // period/slice dependent
      vector<string> subsample_list = runmc ? conf::slice_list : conf::period_list;
      for( auto subsample: subsample_list){
        string identifier = string("pteta")+to_string(1+ij)+"_"+ subsample;
        h2_pteta[identifier] = new TH2D(identifier.c_str(),"",conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
        h2_pteta[identifier]->SetDirectory(dir_subsample);
      }
      // flavour dependent
      for (auto flav: conf::flav_list) {
        string identifier = string("pteta")+to_string(1+ij)+"_"+ flav;
        h2_pteta[identifier] = new TH2D(identifier.c_str(),"",conf::n_pt, conf::pt_lowedges, conf::n_eta, conf::eta_lowedges);
        h2_pteta[identifier]->SetDirectory(dir_subflavour);
      }
    }
  }
  
  //---- Get pT/eta weight histograms---- 
  std::string rew_filename = "../GetPtEtaWeights/rew_d.root";
  if(m_systematic.Contains("generator")) rew_filename = "../GetPtEtaWeights/rew_HERWIG.root";
  TFile* frew = new TFile(rew_filename.c_str());
  TH2D* h_rew[2];
  TH2D* h_rew_bootstrap_data[2]; // datastat
  TH2D* h_rew_bootstrap_mc[2]; // mcstat
  for(int ij=0; ij<2; ij++){
    h_rew[ij] = (TH2D*)frew->Get( (string(m_reweighting_folder + "/ptscale")+char('1'+ij)).c_str() );
    // get bootstrap histo
    if(m_systematic=="FlavourTagging_Nominal" || m_systematic=="subleadingjet")
    {
      if(bootstrap_bkeeper>0)
      {
        h_rew_bootstrap_data[ij] = (TH2D*)frew->Get( (string(m_reweighting_folder + "/ptscale")+char('1'+ij)+string("_data_")+to_string(bootstrap_bkeeper-1)).c_str());
        h_rew_bootstrap_mc[ij]   = (TH2D*)frew->Get( (string(m_reweighting_folder + "/ptscale")+char('1'+ij)+string("_mc_")+to_string(bootstrap_bkeeper-1)).c_str());
      }
    }
  }
  //---- Get Ntrack weight histogram (now applied by default) -----
  std::string rew_ntrack_filename = "../GetNTrackWeights/reweight_ntrack2D_d.root";
  if(m_systematic.Contains("generator")) rew_ntrack_filename = "../GetNTrackWeights/reweight_ntrack2D_HERWIG.root";
  TFile* frew_ntrack = new TFile(rew_ntrack_filename.c_str());
  TH2D* h_rew_ntrk[2];
  TH2D* h_rew_ntrk_bootstrap_data[2]; // datastat
  TH2D* h_rew_ntrk_bootstrap_mc[2]; // mcstat
  for(int ij=0; ij<2; ij++){
      h_rew_ntrk[ij] = (TH2D*)frew_ntrack->Get( (string(m_reweighting_folder + "/datamc_ratio")+char('1'+ij)).c_str() );
      // get bootstrap histo
      if(m_systematic=="FlavourTagging_Nominal" || m_systematic=="subleadingjet")
      {
        if(bootstrap_bkeeper>0)
        {
          h_rew_ntrk_bootstrap_data[ij] = (TH2D*)frew_ntrack->Get( (string(m_reweighting_folder + "/datamc_ratio")+char('1'+ij)+string("_data_")+to_string(bootstrap_bkeeper-1)).c_str());
          h_rew_ntrk_bootstrap_mc[ij] = (TH2D*)frew_ntrack->Get( (string(m_reweighting_folder + "/datamc_ratio")+char('1'+ij)+string("_mc_")+to_string(bootstrap_bkeeper-1)).c_str());
        }
      }
  }

  // ------------- EVENT LOOP STARTS ---------- //
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if(jentry ==0) cout<<" all: "<<nentries<<", ientry by loadTree: "<<ientry<<endl;
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    // determine current tree
    if (currentTree != fCurrent) {
      currentTree = fCurrent;
      string s = ((TChain*)fChain)->GetFile()->GetName();
      // Check it is in NtupleDumper .auto files
      if (runmc) {
	for (auto slice: conf::slice_list){
	  size_t i = s.find(slice);
	  if (i!=string::npos ) {
	    period_slice = slice;
	    break;
	  }
	}
      } else {
	for (auto period: conf::period_list){
	  size_t i = s.find("data" + period + ".root");
	  if (i!=string::npos ) {
	    period_slice = period;
	    break;
	  }
	}
      }
    }
    // some printing
    if (jentry%10000==0) { if(runmc)cout<<"running mc "; else cout<<"running data "; cout << jentry << " period/slice: " << period_slice << '\r'; cout.flush(); }

    // Cut for phi test
    int the_phi=12; bool good_phi=false;
    if (the_phi==0  && ( -3.2<=jetphi[0] && jetphi[0]<-2.5 )) good_phi=true;
    if (the_phi==1  && ( -2.5<=jetphi[0] && jetphi[0]<-2.  )) good_phi=true;
    if (the_phi==2  && (  -2.<=jetphi[0] && jetphi[0]<-1.5 )) good_phi=true;
    if (the_phi==3  && ( -1.5<=jetphi[0] && jetphi[0]<-1.  )) good_phi=true;
    if (the_phi==4  && (  -1.<=jetphi[0] && jetphi[0]<-0.5 )) good_phi=true;
    if (the_phi==5  && ( -0.5<=jetphi[0] && jetphi[0]<0.   )) good_phi=true;
    if (the_phi==6  && (   0.<=jetphi[0] && jetphi[0]<0.5  )) good_phi=true;
    if (the_phi==7  && (  0.5<=jetphi[0] && jetphi[0]<1.   )) good_phi=true;
    if (the_phi==8  && (   1.<=jetphi[0] && jetphi[0]<1.5  )) good_phi=true;
    if (the_phi==9  && (  1.5<=jetphi[0] && jetphi[0]<2.   )) good_phi=true;
    if (the_phi==10 && (   2.<=jetphi[0] && jetphi[0]<2.5  )) good_phi=true;
    if (the_phi==11 && (  2.5<=jetphi[0] && jetphi[0]<3.2  )) good_phi=true;
    if (the_phi==12) good_phi=true; // inclusive phi
    if (!good_phi) continue;

    // identify the leading/subleading jet for cut on jetpass
    int jetpass_leading = -999;
    int jetpass_subleading = -999;
    if(isleading[0])
    {
      jetpass_leading = jetpass[0]; 
      jetpass_subleading = jetpass[1]; 
    }
    else
    {
      jetpass_leading = jetpass[1];
      jetpass_subleading = jetpass[0];
    }

    // Jet Loop
    for (int j = 0; j<njets; ++j) {

      double ptj = jetpt[j];
      double abs_eta = fabs(jeteta[j]);

      // SANITY CHECK (NtupleDumper requirement cuts pt<20GeV)
      if (ptj < 20){ cout<<"Error: some jets with pT < 20 GeV filled the histograms" << std::endl; continue;}

      // event weight
      double weight = mc_evtweight * data_evtweight[j];
      // pu reweighting
      if(runmc && !m_systematic.Contains("PRW_DATASF")) weight *= evtpuw;
      else if (m_systematic.Contains("FlavourTagging_PRW_DATASF__1up")) weight *= evtpuw_sys[0];
      else if (m_systematic.Contains("FlavourTagging_PRW_DATASF__1down")) weight *= evtpuw_sys[1];
      else if (runmc) std::cout << "WARNING: no pileup reweighting applied" << std::endl;
      // JVT SF
      if(runmc && !m_systematic.Contains("JVT_effSF")) weight *= evtJVTw;
      else if (m_systematic.Contains("FlavourTagging_JVT_effSF__1up")) weight *= evtJVTw_sys[0];
      else if (m_systematic.Contains("FlavourTagging_JVT_effSF__1down")) weight *= evtJVTw_sys[1];
      else if (runmc) std::cout << "WARNING: no JVT SF applied" << std::endl;
      // conversions syst
      if(runmc && m_systematic.Contains("conversions__1up") && jetHasConversion[j]) weight *=1.1;
      else if(runmc && m_systematic.Contains("conversions__1down") && jetHasConversion[j]) weight *=0.9;
      // hadronic syst
      if(runmc && m_systematic.Contains("hadronic__1up") && jetHasHadMatInt[j]) weight *=1.1; 
      else if(runmc && m_systematic.Contains("hadronic__1down") && jetHasHadMatInt[j]) weight *=0.9; 
      // longlived
      if(runmc && m_systematic.Contains("longlivedparticles") && (jetHasKShort[j] || jetHasLambda[j])) weight *=1.3;

      double weight_bootstrap_data = weight; // weight for datatstat
      double weight_bootstrap_mc = weight; // weight for mcstat
      if(bootstrap_bkeeper>0)
      {
        weight_bootstrap_mc *= weight_bootstrap[bootstrap_bkeeper-1]; // useless when running on data
        if(!runmc) weight_bootstrap_data *= weight_bootstrap[bootstrap_bkeeper-1]; // useful for both data and MC. weight_bootstrap applied only when running on data
      }

      // Loop only on the leading jet by default
      if(!m_systematic.Contains("subleadingjet") && !isleading[j]) continue;
      if(m_systematic.Contains("subleadingjet") && isleading[j]) continue;

      // jetpass cut
      if (isleading[j] && (jetpass_leading!=0 || jetpass_subleading>1)) continue; // for leading jet
      else if(!isleading[j])
      {
        // if subleading in the first bin, consider only events triggered by the leading in the second bin for 100% trigger efficiency
        if(getPtBin(ptj)==1)
        {
          // Get the pT of the leading
          int index_leading = 0;
          if(j==0) index_leading = 1; 
          else     index_leading = 0;
          double pt_leading = jetpt[index_leading];
          //consider only subleading with leading in a higher bin
          if(getPtBin(pt_leading)!=2) continue; // prescales should not be mixed
          if(jetpass_leading!=0 || jetpass_subleading>1) continue;
          // use leading jet prescale
          weight *= data_evtweight[index_leading]/data_evtweight[j];
          weight_bootstrap_data *= data_evtweight[index_leading]/data_evtweight[j];
          weight_bootstrap_mc *= data_evtweight[index_leading]/data_evtweight[j];
        }
        // use subleading prescale
        else if(jetpass_subleading!=0 || jetpass_leading>1) continue;
      } 
 
      // apply reweighting pT/eta/nTrack for MC
      // reweighting applied only for MC
      if (runmc) 
      {
        int ilead = isleading[j] ? 0 : 1;
	int ix = h_rew[ilead]->GetXaxis()->FindBin(jetpt[j]);
	int iy = h_rew[ilead]->GetYaxis()->FindBin(abs_eta);
	weight *= h_rew[ilead]->GetBinContent(ix,iy);
        if (bootstrap_bkeeper>0) 
        {
          weight_bootstrap_mc *= h_rew_bootstrap_mc[ilead]->GetBinContent(ix,iy);
          weight_bootstrap_data *= h_rew_bootstrap_data[ilead]->GetBinContent(ix,iy);
        }

        // ntrack reweighting applied by default
        // PROTECTION FOR WEIGHT COMPATIBLE WITH 0 added. 
        // the *nominal* weight must be > 2 sigma away from 1 to be applied.

        // reweighting based on IP3DNeg for now
        int ntracks_IP3DNeg = ntrack_IP3DNeg[j];
	if (!m_systematic.Contains("notrackrew")) 
        {
	  int itrx = h_rew_ntrk[ilead]->GetXaxis()->FindBin(ntracks_IP3DNeg);
	  int itry = h_rew_ntrk[ilead]->GetYaxis()->FindBin(jetpt[j]);
         
          if( abs(1-h_rew_ntrk[ilead]->GetBinContent(itrx, itry)) > 2*h_rew_ntrk[ilead]->GetBinError(itrx, itry) )
          {
	    weight *= h_rew_ntrk[ilead]->GetBinContent(itrx, itry);
            if (bootstrap_bkeeper>0)
            {
              weight_bootstrap_mc *= h_rew_ntrk_bootstrap_mc[ilead]->GetBinContent(itrx,itry);
              weight_bootstrap_data *= h_rew_ntrk_bootstrap_data[ilead]->GetBinContent(itrx,itry);
            }
          }
	}
      } // end if runmc

      // Fill up the weight map
      int ptBin = getPtBin(jetpt[j]);
      int etaBin = getEtaBin(jeteta[j]);

      weightpTeta_map[ptBin][etaBin][""] += weight;
      weightpTeta_map[ptBin][etaBin]["gen"] += mc_evtweight;
      ++weightpTeta_map[ptBin][etaBin]["raw"];

      // determine jet category
      // weights already modified earlier
      int ijetCategory = 0;
      bool has_longlived = jetHasKShort[j] || jetHasLambda[j]; 
      bool has_conversion = jetHasConversion[j];
      bool has_interaction = jetHasHadMatInt[j];

      if (has_longlived) ijetCategory += 1;
      if (has_conversion) ijetCategory += 8;
      if (has_interaction) ijetCategory += 4; 
      if(bootstrap_bkeeper==0) h_jetCategory->Fill(ijetCategory, weight);

      // Jet Flavour truth in MC
      int jf = 0;
      string flav = "";
      if (runmc) {
	jf = flavor[j];
	if (jf == 0) flav = "l";
	else if (jf == 4) flav = "c";
	else if (jf == 5) flav = "b";
      }
      else flav = "l"; // data is "l" by default
      if (jf<0){ cout<<"Error jet flavor less than 0: "<<jf<<endl; continue;}
      if (jf>5){ continue;} // skip tau-jets in MC

      if(bootstrap_bkeeper==0)
      {
        // PT/ETA HISTOGRAM FILLING
        // slice dep.
        if(isleading[j]) h2_pteta[string("pteta")+to_string(1)+"_"+ period_slice]->Fill(jetpt[j], abs_eta, weight);
        else h2_pteta[string("pteta")+to_string(2)+"_"+ period_slice]->Fill(jetpt[j], abs_eta, weight);
        // weight histogram
        if(bootstrap_bkeeper==0)
        {
          if(isleading[j]) h_wjet1->Fill(weight);
          else             h_wjet2->Fill(weight);
        }
        // flavor dep
        if(isleading[j]) h2_pteta[string("pteta")+to_string(1)+"_"+ flav]->Fill(jetpt[j], abs_eta, weight);
        else h2_pteta[string("pteta")+to_string(2)+"_"+ flav]->Fill(jetpt[j], abs_eta, weight);

        // TAGGER HISTOGRAM FILLING

        // loop on the taggers
        for (auto tagger: tagger_map){
          // weight and negative weight
          double w = (tagger.second.wptr)[j];
          double wneg = (tagger.second.wnegptr)[j];
	  if(debug == 1) std::cout << " weight and negative weight for pt("<< ptBin<<"),eta("<< etaBin<<"), fl("<< flav<<"), tagger("<< tagger.first<<"): "<< w <<" , " << wneg<< std::endl;

          tagger.second.h->Fill(w, weight); 
          tagger.second.hneg->Fill(wneg, weight);

          h_pt_eta_f_t_m[ptBin][etaBin][flav][tagger.first][""]->Fill(w, weight);
          h_pt_eta_f_t_m[ptBin][etaBin][flav][tagger.first]["neg"]->Fill(wneg, weight);
          // flavor incl.
          h_pt_eta_f_t_m[ptBin][etaBin][""][tagger.first][""]->Fill(w, weight);
          h_pt_eta_f_t_m[ptBin][etaBin][""][tagger.first]["neg"]->Fill(wneg, weight);
        } // tagger loop

        // OTHER TAGGER HISTOGRAM FILLING
        for (const auto &name: subtagger::floats){
          h_subtagger[flav][name.first]->Fill(float_subtagger[name.first][j], weight);
        }
        for (const auto &name: subtagger::ints){
          h_subtagger[flav][name.first]->Fill(int_subtagger[name.first][j], weight);
        }
        for (const auto &name: subtagger::doubles){
          h_subtagger[flav][name.first]->Fill(double_subtagger[name.first][j], weight);
        }
      }

      else
      {

        // bootstrap versions
        // loop on the taggers
        for (auto tagger: tagger_map)
        {
          // weight and negative weight
          double w = (tagger.second.wptr)[j];
          double wneg = (tagger.second.wnegptr)[j];
          //tagger.second.h->Fill(w, weight); 
          //tagger.second.hneg->Fill(wneg, weight);

          if(flav=="l")
          {
            h_pt_eta_f_t_m_bt[ptBin][etaBin][flav][tagger.first][""]["data"]->Fill(w, weight_bootstrap_data);
            h_pt_eta_f_t_m_bt[ptBin][etaBin][flav][tagger.first]["neg"]["data"]->Fill(wneg, weight_bootstrap_data);
            if(runmc)
            {
              h_pt_eta_f_t_m_bt[ptBin][etaBin][flav][tagger.first][""]["mc"]->Fill(w, weight_bootstrap_mc);
              h_pt_eta_f_t_m_bt[ptBin][etaBin][flav][tagger.first]["neg"]["mc"]->Fill(wneg, weight_bootstrap_mc);
            }
          }
          
          else if((flav=="b" || flav=="c") && runmc)
          {
            h_pt_eta_f_t_m_bt[ptBin][etaBin][flav][tagger.first][""]["data"]->Fill(w, weight_bootstrap_data);
            h_pt_eta_f_t_m_bt[ptBin][etaBin][flav][tagger.first][""]["mc"]->Fill(w, weight_bootstrap_mc);
          }

          // flavor incl. for negative algo
          h_pt_eta_f_t_m_bt[ptBin][etaBin][""][tagger.first]["neg"]["data"]->Fill(wneg, weight_bootstrap_data);
          if(runmc)
          {
            h_pt_eta_f_t_m_bt[ptBin][etaBin][""][tagger.first]["neg"]["mc"]->Fill(wneg, weight_bootstrap_mc);
          }
        } // tagger loop
      } // end bootstrap


    } // end jet loop
  } // end event loop

}
