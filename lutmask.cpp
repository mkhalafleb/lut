//
//  lutmask.cpp
//  lut
//
//  Created by Marwan Khalaf on 9/29/20.
//

#include <exception>
#include <string>
#include "lutmask.h"


namespace lutmask {
  
LutMask::LutMask(int16_t mask, unsigned int size) : _lutmask(mask), _size(size) {
  
  if (size > LutMask::MaxLutSize) {
    // Throw Exception, class can't be constructed
    std::string o_range("");
    o_range += "Illgeal Mask Size: " + std::to_string(size) + " , Maximum is " + std::to_string(LutMask::MaxLutSize) ;
    throw std::out_of_range(o_range);
  }
  
  // Check if the mask fits within size.
}
}
