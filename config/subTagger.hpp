#ifndef SUBTAGGER_HPP
#define SUBTAGGER_HPP
#include <map>
// defining here the names and types of the subtagger variables
// inputs from CxAOD
namespace subtagger{
  const std::map <std::string, std::tuple<int, double, double>> floats = {
    {"jet_MV2c10",std::make_tuple(100, -1.0, 1.0)},
    {"jet_MV2c10Flip",std::make_tuple(100, -1.0, 1.0)},
    // subtaggers 
    // IPXD, IPXDNeg
    {"jet_IP2D_logpbpu",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP2D_logpbpc",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP2D_logpcpu",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP3D_logpbpu",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP3D_logpbpc",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP3D_logpcpu",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP2DNeg_logpbpu",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP2DNeg_logpbpc",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP2DNeg_logpcpu",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP3DNeg_logpbpu",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP3DNeg_logpbpc",std::make_tuple(70, -30.0, 40.0)},
    {"jet_IP3DNeg_logpcpu",std::make_tuple(70, -30.0, 40.0)},
    // SV1, SV1Flip
    {"jet_SV1_masssvx",std::make_tuple(100, 0.0, 10000.0)},
    {"jet_SV1_efracsvx",std::make_tuple(20, 0.0, 1.0)},
    {"jet_SV1_Lxy",std::make_tuple(700, -100, 600)},
    {"jet_SV1_L3d",std::make_tuple(700, -100, 600)},
    {"jet_SV1_normdist",std::make_tuple(6000, 0., 600)},
    {"jet_SV1_deltaR",std::make_tuple(50, 0, 5)},
    {"jet_SV1Flip_masssvx",std::make_tuple(100, 0.0, 10000.0)},
    {"jet_SV1Flip_efracsvx",std::make_tuple(20, 0.0, 1.0)},
    {"jet_SV1Flip_Lxy",std::make_tuple(700, -100, 600)},
    {"jet_SV1Flip_L3d",std::make_tuple(700, -100, 600)},
    {"jet_SV1Flip_normdist",std::make_tuple(6000, 0., 600)},
    {"jet_SV1Flip_deltaR",std::make_tuple(50, 0., 5.)},
    // JetFitter, JetFitterFlip
    {"jet_JetFitter_mass",std::make_tuple(100, 0, 10000)},
    {"jet_JetFitter_significance3d",std::make_tuple(600, 0, 600)},
    {"jet_JetFitter_energyFraction",std::make_tuple(100,0.0,1.0)},
    {"jet_JetFitter_dRFlightDir",std::make_tuple(50, 0, 5)},
    {"jet_JetFitterFlip_mass",std::make_tuple(100, 0, 10000)},
    {"jet_JetFitterFlip_significance3d",std::make_tuple(600, 0, 600)},
    {"jet_JetFitterFlip_energyFraction",std::make_tuple(100,0.0,1.0)},
    {"jet_JetFitterFlip_dRFlightDir",std::make_tuple(50, 0, 5)}
  };
  const std::map <std::string, std::tuple<int, double, double>> ints = {
    // SV1, SV1Flip
    {"jet_SV1_N2Tpair",std::make_tuple(102, -2.5, 99.5)},
    {"jet_SV1_NGTinSvx",std::make_tuple(102, -2.5, 99.5)},
    {"jet_SV1Flip_N2Tpair",std::make_tuple(102, -2.5, 99.5)},
    {"jet_SV1Flip_NGTinSvx",std::make_tuple(102, -2.5, 99.5)},
    // JetFitter, JetFitterFlip
    {"jet_JetFitter_N2Tpair",std::make_tuple(102, -2.5, 99.5)},
    {"jet_JetFitter_nSingleTracks",std::make_tuple(102, -2.5, 99.5)},
    {"jet_JetFitter_nVTX",std::make_tuple(102, -2.5, 99.5)},
    {"jet_JetFitter_nTracksAtVtx",std::make_tuple(102, -2.5, 99.5)},
    {"jet_JetFitterFlip_N2Tpair",std::make_tuple(102, -2.5, 99.5)},
    {"jet_JetFitterFlip_nSingleTracks",std::make_tuple(102, -2.5, 99.5)},
    {"jet_JetFitterFlip_nVTX",std::make_tuple(102, -2.5, 99.5)},
    {"jet_JetFitterFlip_nTracksAtVtx",std::make_tuple(102, -2.5, 99.5)},
  };

  const std::map <std::string, std::tuple<int, double, double>> doubles = {
    {"jet_DL1_w",std::make_tuple(100, -6.0, 14.0)},
    {"jet_DL1Flip_w",std::make_tuple(100, -6.0, 14.0)},
  };

}
#endif
