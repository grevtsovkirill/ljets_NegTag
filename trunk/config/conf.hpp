#ifndef CONF_HPP
#define CONF_HPP

#include <map>
#include <vector>
#include <tuple>
#include <algorithm>
#include "../helpers/config_reader.hpp"

// using the namespace conf for common definitions only use fixed values and const type
namespace conf{

  // bootstrap replicas
  const int n_bootstrap = 1000;

  // define here the pt, eta binning to have only one definition
  // define here trigger associated to certain bin

  const int n_pt = 8;

  // new config
  const double pt_lowedges[n_pt+1] = {20, 60, 100, 200, 300, 500, 750, 1000, 3000};
  // ATL-COM-DAQ-2016-087. 
  const std::string bin_trigger[n_pt] = {"HLT_j15", "HLT_j25", "HLT_j60", "HLT_j110", "HLT_j175", "HLT_j380", "HLT_j380", "HLT_j380"}; 
 
  const int n_eta = 2;                                      // current eta bin
  const double eta_lowedges[n_eta+1] = {0.0, 1.2, 2.5};     // (unchanged)
/*
  // thin eta binning
  const int n_eta = 10;                                                                                 
  const double eta_lowedges[n_eta+1] = {0.0, 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 1.75, 2.0, 2.25, 2.50}; 
*/
  const int n_kin = n_pt * n_eta;

  // HF systematics
  const double d_eps_c[6] = {0.2, 0.4, 0.4, 1.0, 0, 0}; // 8 TeV used to be 0.4 flat. 
                                                        // now one for 85%, 77%, 70%, 60%
                                                        // see external/hist_36300_Wc_onx_Data_OsSs.root

  const double d_eps_b[6] = {0.15, 0.15, 0.15, 0.2, 0, 0}; // 8 TeV used to be 0.2 flat. 
                                                           // now one for 85%, 77%, 70%, 60% 
                                                           // see ATL-COM-PHYS-2016-1796

  // jet flavor
  const std::vector<std::string> flav_list = {"l", "c", "b"};

  // define here the taggers to have only one definition
  const std::vector<std::string> tagger_list = {"MV2c10"/*,"MV2c20"*/};

  const int n_tag = tagger_list.size();
  const std::map<std::string, std::tuple<int, double, double>> tag_hist = {
    { "MV2c10", std::make_tuple(100, -1.0, 1.0)},
    { "MV2c20", std::make_tuple(100, -1.0, 1.0)},
  };

  std::map<std::string, std::tuple<int, double, double>> inputvariable_hist = {
    {"MV2c10", std::make_tuple(100, -1., 1.)},
    {"MV2c20", std::make_tuple(100, -1., 1.)},
    {"IP2D_logpbpc",std::make_tuple(100,-5.,5.)},
    {"IP2D_logpbpu",std::make_tuple(100,-5.,5.)},
    {"IP2D_logpcpu",std::make_tuple(100,-5.,5.)},
    {"IP3D_logpbpc",std::make_tuple(100,-5.,5.)},
    {"IP3D_logpbpu",std::make_tuple(100,-5.,5.)},
    {"IP3D_logpcpu",std::make_tuple(100,-5.,5.)},
    { "SV1_masssvx", std::make_tuple(100, -1000., 9000.)},
    { "SV1_efracsvx", std::make_tuple(120, -1.2, 1.2)},
    { "SV1_NGTinSvx", std::make_tuple(25, -1.5, 23.5)},
    { "SV1_N2Tpair", std::make_tuple(100, -0.5, 99.5)},
    { "JetFitter_significance3d", std::make_tuple(100, -0., 100.)},
    { "JetFitter_mass", std::make_tuple(100, 0., 10000.)},
    { "JetFitter_energyFraction", std::make_tuple(100, 0., 1.0)},
    { "JetFitter_nTracksAtVtx", std::make_tuple(30, -0.5, 29.5)},
    { "JetFitter_N2Tpair", std::make_tuple(100, 0., 100.)},
    { "JetFitter_nSingleTracks", std::make_tuple(10, -0.5, 9.5)},
    { "JetFitter_dR", std::make_tuple(30, 0., 1.5)}
  };

  // define working points
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarks#AntiKt4EMTopoJets
  const std::map <std::string, std::vector<double>> wpoint_map = {
    {"MV2c20", {0.0206,0.4803,0.7110,0.8867,0.9594,0.9963}},
    {"MV2c10", {0.1758475,0.645925,0.8244273,0.934906,0.9769329,0.9977155}},
  };
  const std::map <std::string, std::vector<int>> wpoint_labels = {
    {"MV2c20", {85, 77, 70, 60, 50, 30}},
    {"MV2c10", {85, 77, 70, 60, 50, 30}},
  };

  // set the integrated luminosity [1/pb] of the data samples
  // modified by make_lists_and_lumi.sh
  const double int_lumi = 36470.16 ; 

  // get periods and slices automatically
  const std::vector<std::string> slice_list = read_list_v2("../NtupleDumper/.slices.auto");
  const std::vector<std::string> sliceHERWIG_list = read_list_v2("../NtupleDumper/.slicesHERWIG.auto");
  const std::vector<std::string> period_list = read_list_v2("../NtupleDumper/.periods.auto");

  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JetEtmissMC15
  const std::map <std::string, double> xsec = {
    {"JZ0W", 7.8420E+07 * 1.0240E+00}, // Pythia samples
    {"JZ1W", 7.8420E+07 * 6.7198E-04},
    {"JZ2W", 2433400 * 3.3264E-04},
    {"JZ3W", 26454 * 3.1953E-04},
    {"JZ4W", 254.64 * 5.3009E-04},
    {"JZ5W", 4.5536 * 9.2325E-04},
    {"JZ6W", .25752 * 9.4016E-04},
    {"JZ7W", .016214 * 3.9282E-04},
    {"JZ8W", .00062505 * 1.0162E-02},
    {"JZ9W", .000019640 * 1.2054E-02},
    {"JZ10W", .0000011961 * 5.8935E-03},
    {"JZ11W", .000000042260 * 2.7015E-03},
    {"JZ12W", .0000000010370 * 4.2502E-04},
    {"JZ0W_H",  /*7.8514E+07*/ 43.5698 * 9.8374E-01}, // Jennet E Dickinson
    {"JZ1W_H",  /*7.8514E+07*/ 46.6553 * 3.6905E-04},
    {"JZ2W_H",  1.6777E+06 * 2.6121E-04}, // HERWIG++ samples (AMI)
    {"JZ3W_H",  1.8118E+04 * 2.3539E-04},
    {"JZ4W_H",  1.6755E+02 * 3.9276E-04},
    {"JZ5W_H",  2.8717E+00 * 7.0652E-04},
    {"JZ6W_H",  1.5757E-01 * 7.5887E-04},
    {"JZ7W_H",  9.7880E-03 * 3.4609E-04},
    {"JZ8W_H",  3.8667E-04 * 8.9708E-03},
    {"JZ9W_H",  1.3056E-05 * 1.0164E-02},
    {"JZ10W_H", 8.4140E-07 * 4.5845E-03},
    {"JZ11W_H", 3.0273E-08 * 1.8749E-03},
    {"JZ12W_H", 7.0212E-10 * 2.8761E-04}
  };
  // const std::map <std::string, double> mc_threshold = {
  //   {"JZ0W", 20},
  //   {"JZ1W", 60},
  //   {"JZ2W", 160},
  //   {"JZ3W", 400},
  //   {"JZ4W", 800},
  //   {"JZ5W", 1300}
  // };

};
#endif //CONF_HPP
