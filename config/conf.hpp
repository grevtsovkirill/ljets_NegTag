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
  //const std::string bin_trigger[n_pt] = {"HLT_j15", "HLT_j25", "HLT_j60", "HLT_j110", "HLT_j175", "HLT_j380", "HLT_j400", "HLT_j420"}; 
  const std::string bin_trigger[n_pt] = {"HLT_j15", "HLT_j25", "HLT_j60", "HLT_j110", "HLT_j175", "HLT_j420", "HLT_j420", "HLT_j420"}; 
 
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
  const std::vector<std::string> tagger_list = {"MV2c10","DL1"/*,"MV2c20"*/};

  const int n_tag = tagger_list.size();
  const std::map<std::string, std::tuple<int, double, double>> tag_hist = {
    { "MV2c10", std::make_tuple(100, -1.0, 1.0)},
    { "DL1", std::make_tuple(100, -6.0, 14.0)},
    //{ "MV2c20", std::make_tuple(100, -1.0, 1.0)},
  };

  /*
  std::map<std::string, std::tuple<int, double, double>> inputvariable_hist = {
    {"MV2c10", std::make_tuple(100, -1., 1.)},
    {"DL1", std::make_tuple(100, -6., 14.)},
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
  //*/

  // define working points
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarks#AntiKt4EMTopoJets - old
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarksRelease21
  //  const std::map <std::string, std::vector<double>> wpoint_map = {
  const std::map <std::string, std::vector<float>> wpoint_map = {
    {"MV2c10", {0.11,0.645,0.831,0.939}},
    {"DL1", {0.46,1.45,2.02,2.74}},
  };
  const std::map <std::string, std::vector<int>> wpoint_labels = {
    {"MV2c10", {85, 77, 70, 60}},
    {"DL1", {85, 77, 70, 60}},
    //{"FixedCut", {85, 77, 70, 60}},
  };


  // set the integrated luminosity [1/pb] of the data samples
  // modified by make_lists_and_lumi.sh
  //const double int_lumi = 32861.6 ; 
  //const double int_lumi = 36161.6 ; 
  const double int_lumi = 36184.86; 

  // get periods and slices automatically
  const std::vector<std::string> slice_list = read_list_v2("../NtupleDumper/.slices.auto");
  const std::vector<std::string> sliceHERWIG_list = read_list_v2("../NtupleDumper/.slicesHERWIG.auto");
  const std::vector<std::string> sliceSHERPA_list = read_list_v2("../NtupleDumper/.slicesSHERPA.auto");
  const std::vector<std::string> period_list = read_list_v2("../NtupleDumper/.periods.auto");

  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JetEtmissMC15
  const std::map <std::string, double> xsec = {
    {"JZ0W", 78420000  * 1}, // Pythia samples
    {"JZ1W", 78420000 * 0.00067143},
    {"JZ2W", 2433200 * 0.00033423},
    {"JZ3W", 26454 * 0.00032016},
    {"JZ4W", 254.63 * 0.00053138},
    {"JZ5W", 4.5535 * 0.00092409},
    {"JZ6W", 0.25753 * 0.00094092},
    {"JZ7W", 0.016215 * 0.0003928},
    {"JZ8W", 0.00062503 * 0.010176},
    {"JZ9W", 0.000019639 * 0.012076},
    {"JZ10W", 0.0000011962 * 0.0059087},
    {"JZ11W", 0.000000042259 * 0.0026761},
    {"JZ12W", 0.0000000010367  * 0.00042592},
    {"JZ0W_H",  78675000 * 0.98033}, 
    {"JZ1W_H",  78675000 * 0.019474},
    {"JZ2W_H",  1360500 * 0.013265}, 
    {"JZ3W_H",  18242 * 0.01328},
    {"JZ4W_H",  193.73 * 0.013879},
    {"JZ5W_H",  3.5079 * 0.014149},
    {"JZ6W_H",  0.19278 * 0.0090135},
    {"JZ7W_H",  0.01166 * 0.010663},
    {"JZ8W_H",  0.00043404 * 0.0097362},
    {"JZ9W_H",  0.000013472 * 0.01097},
    {"JZ10W_H", 0.00000081778 * 0.0054406},
    {"JZ11W_H", 0.000000029229 * 0.0029496},
    {"JZ1W_S",  20593000 * 0.085809},
    {"JZ2W_S",  107300 * 0.14181}, 
    {"JZ3W_S",  13075 * 0.01854},
    {"JZ4W_S",  96.077 * 0.027799},
    {"JZ5W_S",  2.7251 * 0.01842},
    {"JZ6W_S",  0.20863 * 0.0087423},
    {"JZ7W_S",  0.043734 * 0.0030974},
    {"JZ8W_S",  0.00033371 * 0.014567},
    {"JZ9W_S",  0.000058947 * 0.0031008},
    {"JZ10W_S", 0.0000055732 * 0.00095753},
    {"JZ11W_S", 0.00000012593 * 0.00072055},
    {"JZ12W_S", 0.000000001103 * 0.00047265}
    // {"JZ0W", 7.8420E+07 * 1.0240E+00}, // Pythia samples
    // {"JZ1W", 7.8420E+07 * 6.7198E-04},
    // {"JZ2W", 2433400 * 3.3264E-04},
    // {"JZ3W", 26454 * 3.1953E-04},
    // {"JZ4W", 254.64 * 5.3009E-04},
    // {"JZ5W", 4.5536 * 9.2325E-04},
    // {"JZ6W", .25752 * 9.4016E-04},
    // {"JZ7W", .016214 * 3.9282E-04},
    // {"JZ8W", .00062505 * 1.0162E-02},
    // {"JZ9W", .000019640 * 1.2054E-02},
    // {"JZ10W", .0000011961 * 5.8935E-03},
    // {"JZ11W", .000000042260 * 2.7015E-03},
    // {"JZ12W", .0000000010370 * 4.2502E-04},
    // {"JZ0W_H",  /*7.8514E+07*/ 43.5698 * 9.8374E-01}, // Jennet E Dickinson
    // {"JZ1W_H",  /*7.8514E+07*/ 46.6553 * 3.6905E-04},
    // {"JZ2W_H",  1.6777E+06 * 2.6121E-04}, // HERWIG++ samples (AMI)
    // {"JZ3W_H",  1.8118E+04 * 2.3539E-04},
    // {"JZ4W_H",  1.6755E+02 * 3.9276E-04},
    // {"JZ5W_H",  2.8717E+00 * 7.0652E-04},
    // {"JZ6W_H",  1.5757E-01 * 7.5887E-04},
    // {"JZ7W_H",  9.7880E-03 * 3.4609E-04},
    // {"JZ8W_H",  3.8667E-04 * 8.9708E-03},
    // {"JZ9W_H",  1.3056E-05 * 1.0164E-02},
    // {"JZ10W_H", 8.4140E-07 * 4.5845E-03},
    // {"JZ11W_H", 3.0273E-08 * 1.8749E-03},
    // {"JZ12W_H", 7.0212E-10 * 2.8761E-04}
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
