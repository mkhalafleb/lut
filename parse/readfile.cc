//

#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>
#include "readfile.h"


namespace readfile {


std::string GetFullPath (const std::string &endpath) {
  const char *src_dir = std::getenv("TEST_SRCDIR");
  const char *work_dir = std::getenv("TEST_WORKSPACE");
  if ((src_dir == nullptr) || (work_dir == nullptr)) {
    return endpath;
  }

  std::filesystem::path basepath(src_dir);
  basepath /= std::filesystem::path(work_dir);
  basepath /= std::filesystem::path("readfile");
  basepath /= std::filesystem::path(endpath);
  return(basepath.string());
}

std::unordered_map<std::string, std::string> ReadResultFile(const std::string &endrelative) {
  std::unordered_map<std::string, std::string> sop_map;
  std::string fullpath = readfile::GetFullPath(endrelative);


  // Now open the file and read it
   
  return(sop_map);
}


}
