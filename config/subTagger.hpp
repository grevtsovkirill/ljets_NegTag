#ifndef SUBTAGGER_HPP
#define SUBTAGGER_HPP
#include <map>
// defining here the names and types of the subtagger variables
namespace subtagger{
    // Track versions
   const std::map <std::string, std::tuple<int, double, double>> floats_trackjet = {
    {"trackjet_MV2c10",std::make_tuple(100, -1.0, 1.0)},
    {"trackjet_MV2c10Flip",std::make_tuple(100, -1.0, 1.0)},
//    {"trackjet_DL1mu_w",std::make_tuple(100, -5.0, 5)},
//    {"trackjet_DL1rnn_w",std::make_tuple(100, -5.0, 5)},
//    {"trackjet_DL1muFlip_w",std::make_tuple(100, -5.0, 5)},
//    {"trackjet_DL1rnnFlip_w",std::make_tuple(100, -5.0, 5)},
    // subtaggers 
    // IPXD, IPXDNeg
    //{"trackjet_IP2D_logpbpu",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP2D_logpbpc",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP2D_logpcpu",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP3D_logpbpu",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP3D_logpbpc",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP3D_logpcpu",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP2DNeg_logpbpu",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP2DNeg_logpbpc",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP2DNeg_logpcpu",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP3DNeg_logpbpu",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP3DNeg_logpbpc",std::make_tuple(70, -30.0, 40.0)},
    //{"trackjet_IP3DNeg_logpcpu",std::make_tuple(70, -30.0, 40.0)},
    // SV1, SV1Flip
    //{"trackjet_SV1_masssvx",std::make_tuple(100, 0.0, 10000.0)},
    //{"trackjet_SV1_efracsvx",std::make_tuple(20, 0.0, 1.0)},
    //{"trackjet_SV1_Lxy",std::make_tuple(700, -100, 600)},
    //{"trackjet_SV1_L3d",std::make_tuple(700, -100, 600)},
    //{"trackjet_SV1_normdist",std::make_tuple(6000, 0., 600)},
    //{"trackjet_SV1_deltaR",std::make_tuple(50, 0, 5)},
    //{"trackjet_SV1Flip_masssvx",std::make_tuple(100, 0.0, 10000.0)},
    //{"trackjet_SV1Flip_efracsvx",std::make_tuple(20, 0.0, 1.0)},
    //{"trackjet_SV1Flip_Lxy",std::make_tuple(700, -100, 600)},
    //{"trackjet_SV1Flip_L3d",std::make_tuple(700, -100, 600)},
    //{"trackjet_SV1Flip_normdist",std::make_tuple(6000, 0., 600)},
    //{"trackjet_SV1Flip_deltaR",std::make_tuple(50, 0., 5.)},
    // JetFitter, JetFitterFlip
    //{"trackjet_JetFitter_mass",std::make_tuple(100, 0, 10000)},
    //{"trackjet_JetFitter_significance3d",std::make_tuple(600, 0, 600)},
    //{"trackjet_JetFitter_energyFraction",std::make_tuple(100,0.0,1.0)},
    //{"trackjet_JetFitter_dRFlightDir",std::make_tuple(50, 0, 5)},
    //{"trackjet_JetFitterFlip_mass",std::make_tuple(100, 0, 10000)},
    //{"trackjet_JetFitterFlip_significance3d",std::make_tuple(600, 0, 600)},
    //{"trackjet_JetFitterFlip_energyFraction",std::make_tuple(100,0.0,1.0)},
    //{"trackjet_JetFitterFlip_dRFlightDir",std::make_tuple(50, 0, 5)},
  };
const std::map <std::string, std::tuple<int, double, double>> ints_trackjet = { 
   // SV1, SV1Flip
    //{"trackjet_SV1_N2Tpair",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_SV1_NGTinSvx",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_SV1Flip_N2Tpair",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_SV1Flip_NGTinSvx",std::make_tuple(102, -2.5, 99.5)},

    // JetFitter, JetFitterFlip
    //{"trackjet_JetFitter_N2Tpair",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_JetFitter_nSingleTracks",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_JetFitter_nVTX",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_JetFitter_nTracksAtVtx",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_JetFitterFlip_N2Tpair",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_JetFitterFlip_nSingleTracks",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_JetFitterFlip_nVTX",std::make_tuple(102, -2.5, 99.5)},
    //{"trackjet_JetFitterFlip_nTracksAtVtx",std::make_tuple(102, -2.5, 99.5)},

  };

  const std::map <std::string, std::tuple<int, double, double>> doubles_trackjet = {
    {"trackjet_DL1Flip_w",std::make_tuple(100, -10.0, 10)},  
    {"trackjet_DL1_w",std::make_tuple(100, -10.0, 10)},
};

}
#endif
