//
//  lutmask.cc
//  lut
//
//  Created by Marwan Khalaf on 9/29/20.
//

#include <exception>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>
#include "lutmask.h"


namespace lutmask {

LutMask::LutMask(uint16_t mask, unsigned int size) : _lutmask(mask), domainc_(size, lutdomain::LutDomain::DefaultDomain) {

  std::string o_range("");
  
  if (size > lutdomain::LutDomain::MaxLutSize) {
    // Throw Exception, class can't be constructed
    o_range += "Illgeal Mask Size: " + std::to_string(size) + " , Maximum is " + std::to_string(lutdomain::LutDomain::MaxLutSize) ;
    throw std::out_of_range(o_range);
  }
  if (mask > lutmask::LutMask::lut_size_mask[size]) {
    std::ostringstream ss;
    ss << std::hex << mask;
    o_range += "Illegal Mask: " + ss.str() + " with mask size: " + std::to_string(size);
    throw std::out_of_range(o_range);
  }
  // Check if the mask fits within size.
}
  
LutMask::LutMask(uint16_t mask, const lutdomain::LutDomain &domain) : _lutmask(mask), domainc_(domain) {};
  

  
bool LutMask::IsGnd() const {
  return(_lutmask == 0);
}
  
bool LutMask::IsVcc() const {

  // Shrink the full mask to the current size.
  uint16_t vcc_mask = std::numeric_limits<uint16_t>::max();
  vcc_mask &= lut_size_mask[domainc_.GetSize()];

  return ((_lutmask & vcc_mask) == vcc_mask);
}
  
void LutMask::ValidatePosToSize(unsigned int pos) const {
  if (pos >= domainc_.GetSize()) {
    std::string error("");
    error += "Can't Create independent mask at pos " + std::to_string(pos) + " With size " + std::to_string(domainc_.GetSize());
    throw std::logic_error (error);
  }
}
  
uint16_t LutMask::ShrinkMaskAtPos(uint16_t PosMask, unsigned int pos) const {
  unsigned int shifter = 0x1;
  unsigned int small_shifter = 0x1;
  uint16_t work_mask = 0;
  unsigned int i;

  unsigned int shift_time = (0x1 << domainc_.GetSize());

  for (i=0; i < shift_time; i++)
  {
    if (PosMask & shifter)
    {
      if (_lutmask & shifter)
      {
        work_mask |= small_shifter;
      }
      small_shifter <<= 1;
    }
    shifter <<= 1;
  }
  return (work_mask);
}

  
lutmask::LutMask LutMask::CreateMaskIndependentOfPos(unsigned int pos) const {
  lutmask::LutMask::ValidatePosToSize(pos);
  // In Shannon's Decomposition, a function g(x) = xf(x) + x'f(x');
  // This Function basically computes f(x) by setting x = 1 in g(x)
  uint16_t work_mask = LutMask::ShrinkMaskAtPos(LutMask::position_mask[pos], pos);

  // Shrink domain by 1
  return (lutmask::LutMask(work_mask,  domainc_.ShrinkAtPos(pos)));
}
  
lutmask::LutMask LutMask::CreateMaskIndependentOfPosBar(unsigned int pos) const {
  lutmask::LutMask::ValidatePosToSize(pos);
    // In Shannon's Decomposition, a function g(x) = xf(x) + x'f(x');
    // This Function basically computes f(x') by setting x = 0 in g(x)
  uint16_t work_mask = LutMask::ShrinkMaskAtPos(~LutMask::position_mask[pos], pos);

  return (lutmask::LutMask(work_mask,  domainc_.ShrinkAtPos(pos)));
}

bool LutMask::IsSingleLiteralAtPos(unsigned int pos) const {

  lutmask::LutMask mask_at_pos = CreateMaskIndependentOfPos(pos);
  lutmask::LutMask mask_at_pos_bar = CreateMaskIndependentOfPosBar(pos);


  if (mask_at_pos.IsGnd() && mask_at_pos_bar.IsVcc()) {
    return(true);
  }

  if (mask_at_pos.IsVcc() && mask_at_pos_bar.IsGnd()) {
    return(true);
  }


  return (false);
}
  
std::vector<unsigned int> LutMask::GetMetricVec() const {
  std::vector<unsigned int> m(domainc_.GetSize(), 0);
  return(m);
};
  
char LutMask::GetCharDomainAtPos(unsigned int pos) const {
  return(domainc_.GetDomain().at(pos));
}


bool LutMask::IsSingleLiteral() const {
  return (domainc_.GetSize() == 1);
}

bool LutMask::IsSingleLiteralInv() const {
  return(IsSingleLiteral() && (_lutmask == 0x1));
}

bool LutMask::IsLutMaskOnlyAndAtPos(unsigned int pos) const {
  lutmask::LutMask mask_at_pos = CreateMaskIndependentOfPos(pos);
  lutmask::LutMask mask_at_pos_bar = CreateMaskIndependentOfPosBar(pos);

  return (mask_at_pos.IsGnd() || mask_at_pos_bar.IsGnd());

}

bool LutMask::IsIndependentOfPos(unsigned int pos) const {
  lutmask::LutMask mask_at_pos = CreateMaskIndependentOfPos(pos);
  lutmask::LutMask mask_at_pos_bar = CreateMaskIndependentOfPosBar(pos);
  return(mask_at_pos == mask_at_pos_bar);
}

bool LutMask::operator==(const lutmask::LutMask &lut_mask) const {
  
  bool equal = (_lutmask == lut_mask._lutmask);
  equal &= (domainc_ == lut_mask.domainc_);
  return (equal);
}

bool LutMask::operator!=(const lutmask::LutMask &lut_mask) const {
  return(!(*this == lut_mask));
}

lutmask::LutMask LutMask::InvertMask() const {
  return(lutmask::LutMask((~_lutmask) & LutMask::lut_size_mask[domainc_.GetSize()], domainc_));
}

bool LutMask::IsXorAtPos(unsigned int pos) const {
  lutmask::LutMask mask_at_pos = CreateMaskIndependentOfPos(pos);
  lutmask::LutMask mask_at_pos_bar = CreateMaskIndependentOfPosBar(pos);

  if (mask_at_pos.IsGnd() || mask_at_pos_bar.IsGnd()) {
    return(false);
  }
 
  lutmask::LutMask invert_mask_at_pos_bar = mask_at_pos_bar.InvertMask();

  return(mask_at_pos == invert_mask_at_pos_bar);

}



}  // namespace lutmask
