#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include "fileStructure.hpp"
#define ACTIVE 1
#define DEACTIVE 0
#define SUCCESS true
#define FAILURE false
extern std::string currHeadPosition;
extern std::map<std::string, std::vector<std::string>>mp;
extern std::map<std::string, int>functions;
// extern fs::path root_dir = ".";
