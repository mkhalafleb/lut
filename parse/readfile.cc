//

#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "readfile.h"


namespace readfile {


std::string GetFullPath (const std::string &endpath, const std::string &dir_path) {
  const char *src_dir = std::getenv("TEST_SRCDIR");
  const char *work_dir = std::getenv("TEST_WORKSPACE");
  if ((src_dir == nullptr) || (work_dir == nullptr)) {
    return endpath;
  }

  std::filesystem::path basepath(src_dir);
  basepath /= std::filesystem::path(work_dir);
  basepath /= std::filesystem::path(dir_path);
  basepath /= std::filesystem::path(endpath);
  return(basepath.string());
}

std::unordered_map<std::string, std::string> ReadResultFile(const std::string &endrelative, const std::string &dir_path) {
  std::unordered_map<std::string, std::string> sop_map;
  std::string fullpath = readfile::GetFullPath(endrelative, dir_path);


  // Now open the file and read it
  std::ifstream infile; 
  infile.open(fullpath, std::ios::in); 

  if (!infile.is_open()) {
    return(sop_map);
  }

  while (infile.good() && (!infile.eof())) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof()) {
      continue;
    }
    // Split line on first space
    std::string key, sop;
    auto pos = line.find('\t');
    if (pos != std::string::npos) {
      key = line.substr(0, pos);
      sop = line.substr(pos+1, line.length());
      sop_map.emplace(key,sop);
    }
  }

  infile.close();
  return(sop_map);
}


}
