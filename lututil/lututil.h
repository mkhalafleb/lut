
#include <string>
#include "lutmask/lutmask.h"

#ifndef LUT_LUTUTIL_H
#define LUT_LITUTIL_H
namespace lututil {

std::string print_mask(const lutmask::LutMask &mask, unsigned int pos, char *domain_char);

unsigned int calculate_best_pivot(const lutmask::LutMask &mask);

unsigned int find_overall_lit(const lutmask::LutMask &mask, unsigned int pos);

void calculate_new_domain(char *domain_char, int size, int pos);

std::string generate_sop(const lutmask::LutMask &mask);


} // namespace lututil

#endif
