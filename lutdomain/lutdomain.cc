//
//  lutdomain.cc
//  lut
//
//  Created by Marwan Khalaf on 9/29/20.
//

#include "lutdomain.h"


namespace lutdomain {

LutDomain::LutDomain(unsigned int size, const std::array<char, LutDomain::MaxLutSize> &domain):size_(size), domain_(domain) {} ;


bool LutDomain::operator==(const LutDomain &domain) const {
  return( (size_ == domain.size_) && (domain_ == domain.domain_) );
}

unsigned int LutDomain::GetSize() const {
  return (size_);
}

std::array<char, LutDomain::MaxLutSize> LutDomain::GetDomain() const {
  return (domain_);
}

bool LutDomain::operator!=(const LutDomain &domain) const {
  return(!(*this == domain));
}




}  // namespace lutdomain
