//
//  lutmask.h
//  lut
//
//  Created by Marwan Khalaf on 9/29/20.
//

#ifndef lutmask_h
#define lutmask_h

#include <cstdint>

namespace lutmask {
  
class LutMask {
  
public:
  static const unsigned int MaxLutSize = 4U;
  // 0x0001 = 2^1 - 1 = 2 ^ (2^0) - 1
  // 0x0003 = 2^2 - 1 = 2 ^ (2^1) - 1
  // 0x000F = 2^4 -1 = 2 ^ (2^2) - 1
  // 0x00FF = 2^8 - 1 = 2 ^ (2^3) - 1
  // 0xFFFF = 2^16 -1 = 2 ^ (2^4) - 1
  static constexpr uint16_t lut_size_mask[LutMask::MaxLutSize+1] = {0x0001, 0x0003, 0x000F, 0x00FF, 0xFFFF};
  
  // Since it is a 16 bit mask, the size is either 0, 1, 2, or 3
  LutMask(uint16_t mask, unsigned int size);
  
  bool IsGnd() const;
  bool IsVcc() const;
  
private:
  uint16_t _lutmask;
  unsigned int _size;
    
    
};
  
}

#endif /* lutmask_h */
