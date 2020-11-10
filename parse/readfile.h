//
//  readfile.h
//
//

#ifndef READFILE_H
#define READFILE_H

#include <string>
#include <unordered_map>


namespace readfile {

std::string GetFullPath (const std::string &endpath);

std::unordered_map<std::string, std::string> ReadResultFile(const std::string &filename);
}

#endif /* READFILE_H */
