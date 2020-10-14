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


namespace lut_arg_parser {

std::string validate_arguments (int argc, char **argv) {
  if (argc != 2) {
    return(std::string());
  }
  return(std::string(argv[1]));
}


std::optional<uint16_t> parse_hex (const std::string  &hex_string ) {
  std::istringstream ss(hex_string);
  uint16_t lutmask=0;
  ss  >> std::hex >> lutmask;
  if (!ss.eof() || ss.fail()) {
    return (std::nullopt);
  }
  return(std::optional<uint16_t>{lutmask});
}

}
