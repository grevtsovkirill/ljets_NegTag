#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <fstream>
#include <vector>
#include "TString.h"

std::vector<TString> read_list(TString);
std::vector<std::string> read_list_v2(std::string);

#endif //CONFIG_READER_HPP
