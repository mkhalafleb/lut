//
//  parse.h
//  lut
//
//  Created by Marwan Khalaf on 9/28/20.
//

#ifndef PARSE_H
#define PARSE_H

#include <optional>
#include <limits> 
#include <string>

namespace lut_arg_parser {

std::string validate_arguments (int argc, char **argv);
  
std::optional<uint16_t> parse_hex (const std::string &hex_string);
  
}

#endif /* PARSE_H */
