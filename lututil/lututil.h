
#include <string>

#include "lutmask/lutmask.h"

#ifndef LUT_LUTUTIL_H
#define LUT_LUTUTIL_H
namespace lututil {

std::string print_mask(const lutmask::LutMask &mask, unsigned int pos);

unsigned int calculate_best_pivot(const lutmask::LutMask &mask);

unsigned int find_overall_lit(const lutmask::LutMask &mask, unsigned int pos);

std::string generate_sop(const lutmask::LutMask &mask);

}  // namespace lututil

#endif
