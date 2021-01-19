//
//  lutmask.h
//  lut
//
//  Created by Marwan Khalaf on 9/29/20.
//

#ifndef lutmask_h
#define lutmask_h

#include <array>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "lutdomain/lutdomain.h"

namespace lutmask {

class LutMask {
 public:

  // Since it is a 16 bit mask, the size is either 0, 1, 2, or 3
  // It creates a mask with the default domain of lutdomain::LutDomain
  LutMask(uint16_t mask, unsigned int size);

  LutMask CreateMaskIndependentOfPos(unsigned int pos) const;
  LutMask CreateMaskIndependentOfPosBar(unsigned int pos) const;
  LutMask InvertMask() const;

  bool IsLutMaskOnlyAndAtPos(unsigned int pos) const;
  bool IsSingleLiteralAtPos(unsigned int pos) const;
  bool IsSingleLiteral() const;
  bool IsSingleLiteralInv() const;

  bool IsIndependentOfPos(unsigned int pos) const;

  bool IsXorAtPos(unsigned int pos) const;

  std::vector<unsigned int> GetMetricVec() const;

  char GetCharDomainAtPos(unsigned int pos) const;

  bool IsGnd() const;
  bool IsVcc() const;

  bool operator==(const lutmask::LutMask &lut_mask) const;
  bool operator!=(const lutmask::LutMask &lut_mask) const;

 private:
  // 0x0001 = 2^1 - 1 = 2 ^ (2^0) - 1
  // 0x0003 = 2^2 - 1 = 2 ^ (2^1) - 1
  // 0x000F = 2^4 -1 = 2 ^ (2^2) - 1
  // 0x00FF = 2^8 - 1 = 2 ^ (2^3) - 1
  // 0xFFFF = 2^16 -1 = 2 ^ (2^4) - 1
  static constexpr std::array<uint16_t, lutdomain::LutDomain::MaxLutSize + 1>
      lut_size_mask = {0x0001, 0x0003, 0x000F, 0x00FF, 0xFFFF};

  // Mask is on a Look Up Table for values of
  // 0xFF00 is when D =1 and D = 0
  // 0XF0F0 is when C = 1 and C = 0 and so on
  static constexpr std::array<uint16_t, lutdomain::LutDomain::MaxLutSize>
      position_mask = {0xAAAA, 0xCCCC, 0xF0F0, 0xFF00};

  // When we shrink a mask, we need to change the domain so we need a new
  // constructor but that one can't be called from outside.
  LutMask(uint16_t mask, const lutdomain::LutDomain &domain);

  // Throws Exception if pos >= size;
  void ValidatePosToSize(unsigned int pos) const;

  uint16_t ShrinkMaskAtPos(uint16_t PosMask, unsigned int pos) const;

  uint16_t _lutmask;

  // For this LUT, What is the domain, D, C, B, A or D, B, A. What are the
  // connected inputs
  lutdomain::LutDomain domainc_;
};

}  // namespace lutmask

#endif /* lutmask_h */
