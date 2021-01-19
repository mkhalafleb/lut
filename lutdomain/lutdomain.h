//
//  lutdomain.h
//  lut
//
//  Created by Marwan Khalaf on 9/29/20.
//

#ifndef lutdomain_h
#define lutdomain_h

#include <array>

namespace lutdomain {

class LutDomain {
 public:
  static const unsigned int MaxLutSize = 4U;

  static constexpr std::array<char, LutDomain::MaxLutSize> DefaultDomain = {
      'A', 'B', 'C', 'D'};

  LutDomain(unsigned int size,
            const std::array<char, LutDomain::MaxLutSize> &domain);

  unsigned int GetSize() const;

  char GetDomainAtPos(unsigned int pos) const;

  LutDomain ShrinkAtPos(unsigned int pos) const;

  bool operator==(const lutdomain::LutDomain &lut_mask) const;
  bool operator!=(const lutdomain::LutDomain &lut_mask) const;

 private:
  unsigned int size_;
  std::array<char, LutDomain::MaxLutSize> domain_;
};

}  // namespace lutdomain

#endif /* lutdomain_h */
