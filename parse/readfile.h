//
//  readfile.h
//
//

#ifndef LUT_PARSE_READFILE_H
#define LUT_PARSE_READFILE_H

#include <string>
#include <unordered_map>

namespace readfile {

std::string GetFullPath(const std::string &endpath);

std::unordered_map<std::string, std::string> ReadResultFile(
    const std::string &filename, const std::string &dir_path);
}  // namespace readfile

#endif /* READFILE_H */
