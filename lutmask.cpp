//
//  lutmask.cpp
//  lut
//
//  Created by Marwan Khalaf on 9/29/20.
//

#include <exception>
#include <string>
#include <limits>
#include <sstream>
#include "lutmask.h"


namespace lutmask {
  
LutMask::LutMask(uint16_t mask, unsigned int size) : _lutmask(mask), _size(size) {
  
  std::string o_range("");

  if (size > LutMask::MaxLutSize) {
    // Throw Exception, class can't be constructed
    o_range += "Illgeal Mask Size: " + std::to_string(size) + " , Maximum is " + std::to_string(LutMask::MaxLutSize) ;
    throw std::out_of_range(o_range);
  }
  
  if (mask > ((1U << size)  - 1U)) {
    std::ostringstream ss;
    ss  << std::hex << mask;
    o_range += "Illegal Mask: " + ss.str() + " with mask size: " + std::to_string(size);
    throw std::out_of_range(o_range);
  }
  // Check if the mask fits within size.
}
  
bool LutMask::IsGnd() const {
  return(_lutmask == 0);
}
  
bool LutMask::IsVcc() const {

  // Shrink the full mask to the current size.
  uint16_t vcc_mask = std::numeric_limits<uint16_t>::max();
    
  vcc_mask &= lut_size_mask[_size];

  return ((_lutmask & vcc_mask) == vcc_mask);
}
  
  
}
