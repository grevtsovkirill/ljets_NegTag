#include <vector>
#include <iostream>
#include <TString.h>

std::vector<TString> get_arguments(const int argc, char* const argv[], int &counter){
  ++counter;
  std::vector<TString> arguments;
  while ( counter< argc){
    if (strncmp(argv[counter], "-", 1) == 0) break;
    std::cout  << " ++ " << argv[counter];
    arguments.push_back(argv[counter]);
    ++counter;
  }
  std::cout << std::endl;
  --counter;
  return arguments;
}

TString get_argument(const int argc, char* const argv[], int &counter){
  std::vector<TString> arguments;
  arguments = get_arguments(argc, argv, counter);
  if ( arguments.size()>1 ) {std::cout << "only 1 argument accepted!" << std::endl; return "";}
  if ( arguments.size()<1 ) {std::cout << "no argument provided!" << std::endl; return "";}
  return arguments[0];
}
