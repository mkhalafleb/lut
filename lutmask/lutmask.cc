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
  
  _domain.reserve(LutMask::MaxLutSize);
  
  _domain.resize(LutMask::MaxLutSize);
  
  // Create the Default Domain
  std::copy(default_domain, default_domain + LutMask::MaxLutSize, _domain.begin());

  if (size > LutMask::MaxLutSize) {
    // Throw Exception, class can't be constructed
    o_range += "Illgeal Mask Size: " + std::to_string(size) + " , Maximum is " + std::to_string(LutMask::MaxLutSize) ;
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
  
  
LutMask::LutMask(uint16_t mask, unsigned int size, const std::vector<char>  &new_domain) : LutMask::LutMask(mask, size) {
  _domain.resize(new_domain.size());
  std::copy(new_domain.begin(), new_domain.end(), _domain.begin());
}

  
bool LutMask::IsGnd() const {
  return(_lutmask == 0);
}
  
bool LutMask::IsVcc() const {

  // Shrink the full mask to the current size.
  uint16_t vcc_mask = std::numeric_limits<uint16_t>::max();
  vcc_mask &= lut_size_mask[_domain.size()];

  return ((_lutmask & vcc_mask) == vcc_mask);
}
  
void LutMask::ValidatePosToSize(unsigned int pos) const {
  if (pos >= _domain.size()) {
    std::string error("");
    error += "Can't Create independent mask at pos " + std::to_string(pos) + " With size " + std::to_string(_domain.size());
    throw std::logic_error (error);
  }
}
  
uint16_t LutMask::ShrinkMaskAtPos(uint16_t PosMask, unsigned int pos) const {
  unsigned int shifter = 0x1;
  unsigned int small_shifter = 0x1;
  uint16_t work_mask = 0;
  unsigned int i;

  unsigned int shift_time = (0x1 << _domain.size());

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
  std::vector<char> new_domain(_domain);
  new_domain.erase(new_domain.begin()+pos);
  return (lutmask::LutMask(work_mask, new_domain.size(), new_domain));
}
  
lutmask::LutMask LutMask::CreateMaskIndependentOfPosBar(unsigned int pos) const {
  lutmask::LutMask::ValidatePosToSize(pos);
    // In Shannon's Decomposition, a function g(x) = xf(x) + x'f(x');
    // This Function basically computes f(x') by setting x = 0 in g(x)
  uint16_t work_mask = LutMask::ShrinkMaskAtPos(~LutMask::position_mask[pos], pos);
  std::vector<char> new_domain(_domain);
  new_domain.erase(new_domain.begin()+pos);

  return (lutmask::LutMask(work_mask, new_domain.size(), new_domain));
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
  std::vector<unsigned int> m(_domain.size(), 0);
  return(m);
};
  
char LutMask::GetCharDomainAtPos(unsigned int pos) const {
  return(_domain.at(pos));
}


bool LutMask::IsSingleLiteral() const {
  return (_domain.size() == 1);
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
  equal &= std::equal(_domain.begin(), _domain.end(), lut_mask._domain.begin());
  return (equal);
}

bool LutMask::operator!=(const lutmask::LutMask &lut_mask) const {
  return(!(*this == lut_mask));
}

lutmask::LutMask LutMask::InvertMask() const {
  return(lutmask::LutMask((~_lutmask) & LutMask::lut_size_mask[_domain.size()], _domain.size(), _domain));
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
