#include <map>
#include <string>
#include <iostream>
#include "../config/conf.hpp"

std::map<int, std::map<int, std::string>> getKinLabels(){
  std::map<int, std::map<int, std::string>> labels;
  for (int ipt = 1; ipt<=conf::n_pt; ++ipt) {
    for (int ieta = 1; ieta<=conf::n_eta; ++ieta) {
      labels[ipt][ieta] = std::string("pt")+std::to_string(ipt)+"eta"+std::to_string(ieta);
    }
  }
  return labels;
}

int getPtBin(const double pt){
  if (pt >= conf::pt_lowedges[0] && pt < conf::pt_lowedges[conf::n_pt]){
    for (int i = 1; i <= conf::n_pt; ++i){
      if (pt < conf::pt_lowedges[i]) return i;
    }
  }
  std::cout << std::endl;
  std::cout << "!!! pt out of range: " << pt << std::endl;
  return -1;
}

int getEtaBin(const double eta){
  double ieta = fabs(eta);
  if (ieta >= conf::eta_lowedges[0] && ieta <= conf::eta_lowedges[conf::n_eta]){
    for (int i = 1; i <= conf::n_eta; ++i){
      if (ieta <= conf::eta_lowedges[i]) return i;
    }
  }
  std::cout << "!!! fabs(eta) out of range: " << ieta << std::endl;
  return -1;
}


std::map<int, std::map<int, std::string>> getTrackKinLabels(){
  std::map<int, std::map<int, std::string>> track_labels;
  for (int itrackpt = 1; itrackpt<=conf::n_pt; ++itrackpt) {
    for (int itracketa = 1; itracketa<=conf::n_eta; ++itracketa) {
      track_labels[itrackpt][itracketa] = std::string("trackpt")+std::to_string(itrackpt)+"tracketa"+std::to_string(itracketa);
    }
  }
  return track_labels;
}

int getTrackPtBin(const double trackpt){
  if (trackpt >= conf::pt_lowedges[0] && trackpt < conf::pt_lowedges[conf::n_pt]){
    for (int i = 1; i <= conf::n_pt; ++i){
      if (trackpt < conf::pt_lowedges[i]) return i;
    }
  }
  std::cout << std::endl;
  std::cout << "!!! trackpt out of range: " << trackpt << std::endl;
  return -1;
}

int getTrackEtaBin(const double tracketa){
  double itracketa = fabs(tracketa);
  if (itracketa >= conf::eta_lowedges[0] && itracketa <= conf::eta_lowedges[conf::n_eta]){
    for (int i = 1; i <= conf::n_eta; ++i){
      if (itracketa <= conf::eta_lowedges[i]) return i;
    }
  }
  std::cout << "!!! fabs(tracketa) out of range: " << itracketa << std::endl;
  return -1;
}

