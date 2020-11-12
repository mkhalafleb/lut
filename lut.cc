#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <exception>
#include <iostream>
#include <optional>

#include "lutmask/lutmask.h"
#include "parse/parse.h"
#include "lututil/lututil.h"





int main(int argc, char **argv) {
  int new_pos;
  char domain_char[4] = {'A', 'B', 'C', 'D'};

  auto sarg = lut_arg_parser::validate_arguments(argc, argv);

  std::optional<uint16_t> input_lutmask = lut_arg_parser::parse_hex(sarg);

  // If the arguments are illegal, it is a nullopt

  if (!input_lutmask) {
    printf("Illegal arguments\n");
    printf("Usage: lut <mask> (hex)\n");
    return (-1);
  }

  try {
    //   lutmask::LutMask (lut_mask, 5);
    //   lutmask::LutMask (0xF001, 2);
    std::string string_mask(""); 
    lutmask::LutMask mask(*input_lutmask, 4);
    if (mask.IsVcc()) {
      string_mask = "1";
    } else if (mask.IsGnd()) {
      string_mask = "0";
    } else {
      new_pos = lututil::calculate_best_pivot(mask);
      string_mask = lututil::print_mask(mask, new_pos, &domain_char[0]);
    }
    std::cout << string_mask << std::endl;
  } catch (std::out_of_range &out_of_range) {
    std::cout << out_of_range.what() << std::endl;
  } catch (std::logic_error &logic_error) {
    std::cout << logic_error.what() << std::endl;
  }
  return (0);
}
