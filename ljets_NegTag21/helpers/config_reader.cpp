#include "config_reader.hpp"

std::vector<TString> read_list(TString filename){

  std::vector<TString> config;
  std::ifstream in(filename);
  while (in) {
    TString mystring; in >> mystring;
    if (in.eof()) break;
    config.push_back(mystring);
  }
  return config;
}
std::vector<std::string> read_list_v2(std::string filename){

  std::vector<std::string> config;
  std::ifstream in(filename);
  while (in) {
    std::string mystring; in >> mystring;
    if (in.eof()) break;
    config.push_back(mystring);
  }
  return config;
}

