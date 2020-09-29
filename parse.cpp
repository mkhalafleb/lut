//
//  parse.cpp
//  lut
//
//  Created by Marwan Khalaf on 9/28/20.
//

#include "parse.h"
#include <optional>
#include <cstdint>
#include <sstream>
std::optional<uint16_t> parse_arguments (int argc, char **argv) {
  if (argv == nullptr) {
    return(std::nullopt);
  }
  if (argc != 2) {
    return(std::nullopt);
  }
  std::istringstream ss(argv[1]);
  uint16_t lutmask=0;
  ss  >> std::hex >> lutmask;
  if (!ss.eof() || ss.fail()) {
    return (std::nullopt);
  }
  
  return(std::optional<uint16_t>{lutmask});
}

