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
  
  // Since it is a 16 bit mask, the size is either 0, 1, 2, or 3
  LutMask(int16_t mask, unsigned int size);
    
};
  
}

#endif /* lutmask_h */
