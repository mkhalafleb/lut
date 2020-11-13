
#include <string>
#include <cstring>
#include <cassert>
#include "lututil.h"
#include "lutmask/lutmask.h"


namespace lututil {

unsigned int calculate_best_pivot(const lutmask::LutMask &mask) {
  unsigned int best_pivot;
  unsigned int i;
  unsigned int num_lit[4];
  unsigned int smallest;

  if (mask.IsSingleLiteral()) {
    return(0);
  }
  /* initialize literals */
  memset(&num_lit[0], 0, mask.Size() * sizeof(int));

  for (i = 0; i < mask.Size(); i++) {
    num_lit[i] = find_overall_lit(mask, i);
  }

  smallest = 0;
  best_pivot = 0;

  for (i = 0; i < mask.Size(); i++) {
    if ((num_lit[i] < smallest) || (smallest == 0)) {
     if (num_lit[i] != 0) {
        smallest = num_lit[i];
        best_pivot = i;
      }
    }
  }
  return (best_pivot);
}


std::string print_mask(const lutmask::LutMask &mask, unsigned int pos, char *domain_char) {
  /* print function as xf(x) + x'f(x') */
  unsigned int new_pos = mask.Size() - 1;
  char char_name = domain_char[pos];
  char saved_domain[4];
  bool only_and = false;
  bool is_xor_only = false;
  bool single_lit = false;


  std::memcpy(&saved_domain[0], domain_char, mask.Size() * sizeof(char));


  std::string string_mask("");
  if (mask.IsSingleLiteral()) {
    assert(!mask.IsVcc());
    assert(!mask.IsGnd());
    /* Either print x or x' */
    string_mask += std::string(1, char_name);
    if (mask.IsSingleLiteralInv()) {
      string_mask += "'";

    }
    return(string_mask);
  } 

  if (mask.IsXorAtPos(pos)) {
    lutmask::LutMask mask_fx_bar = mask.CreateMaskIndependentOfPosBar(pos);
    new_pos = calculate_best_pivot(mask_fx_bar);
    calculate_new_domain(domain_char, mask.Size(), pos);

    is_xor_only = mask_fx_bar.IsXorAtPos(new_pos);
    single_lit = mask_fx_bar.IsSingleLiteralAtPos(new_pos);


    string_mask += std::string(1, char_name) + " $ ";
    if ((!is_xor_only) && (!single_lit)) {
      string_mask += "(";
    }
    
    string_mask += print_mask(mask_fx_bar, new_pos, domain_char);
    std::memcpy(domain_char, &saved_domain[0], mask.Size() * sizeof(char));
    if ((!is_xor_only) && (!single_lit))
    {
      string_mask += ")";
    }
    return (string_mask);
  }

  if (!mask.IsIndependentOfPos(pos)) {
    /* Compute F(x) and F(x') */
    lutmask::LutMask mask_fx = mask.CreateMaskIndependentOfPos(pos);
    lutmask::LutMask mask_fx_bar = mask.CreateMaskIndependentOfPosBar(pos);

    /* print xf(x) */
    if (!mask_fx.IsGnd()) {
      if (mask_fx.IsVcc()) {
        string_mask += std::string(1, char_name);
      } else {
        new_pos = calculate_best_pivot(mask_fx);
        calculate_new_domain(domain_char, mask.Size(), pos);
        only_and = mask_fx.IsLutMaskOnlyAndAtPos(new_pos);

        if (mask_fx_bar.IsVcc()) {
          /* DO not print x */
        } else {
          string_mask += std::string(1, char_name);
          if (!only_and) {
            string_mask += " & ("; 
          } else {
            string_mask += " & "; 
          }
        }
        string_mask += print_mask(mask_fx, new_pos, domain_char);
        std::memcpy(domain_char, &saved_domain[0], mask.Size() * sizeof(char));
        if ((!only_and) && (!mask_fx_bar.IsVcc())) {
          string_mask += ")";
        }
      }
      if (!mask_fx_bar.IsGnd()) {
        string_mask += " + ";
      }
    }

    /* Now print x' f(x') */
    if (!mask_fx_bar.IsGnd()) {
      new_pos = calculate_best_pivot(mask_fx_bar);
      calculate_new_domain(domain_char, mask.Size(), pos);
      only_and = mask_fx_bar.IsLutMaskOnlyAndAtPos(new_pos);
      if (mask_fx_bar.IsVcc())  {
        string_mask += std::string(1, char_name);
        string_mask += "'";
      } else {
        if (mask_fx.IsVcc()) {
          /* Do not print x' */
        } else {
          string_mask += std::string(1, char_name);
          string_mask += "'";
          if (!only_and) {
            string_mask += " & (";
          } else {
            string_mask += " & ";
          }
        }
        string_mask += print_mask(mask_fx_bar, new_pos, domain_char);
        std::memcpy(domain_char, &saved_domain[0], mask.Size() * sizeof(char));
        if ((!only_and) && (!mask_fx.IsVcc())) {
          string_mask += ")";
        }
      }
    }
  } else {
    /* fx = fx' */
    lutmask::LutMask mask_fx = mask.CreateMaskIndependentOfPos(pos);
    new_pos = calculate_best_pivot(mask_fx);
    calculate_new_domain(domain_char, mask.Size(), pos);
    string_mask += print_mask(mask_fx, new_pos, domain_char);
    std::memcpy(domain_char, &saved_domain[0], mask.Size() * sizeof(char));
  }
  return(string_mask);

}


unsigned int find_overall_lit(const lutmask::LutMask &mask , unsigned int pos) {
  unsigned int lit = 0;
  unsigned int best_pivot;

  if (mask.IsVcc() || mask.IsGnd()) {
    return(0);
  }

  if (mask.IsSingleLiteral()) {
    return(1);
  }

  lutmask::LutMask mask_fx = mask.CreateMaskIndependentOfPos(pos);
  lutmask::LutMask mask_fx_bar = mask.CreateMaskIndependentOfPosBar(pos);

  if (mask.IsXorAtPos(pos)) {
    best_pivot = calculate_best_pivot(mask_fx_bar);
    lit = 1 + find_overall_lit(mask_fx_bar, best_pivot);
  } else if (mask_fx.IsVcc() || mask_fx.IsGnd()) {
    best_pivot = calculate_best_pivot(mask_fx_bar);
    lit = 1 + find_overall_lit(mask_fx_bar, best_pivot);
  } else if (mask_fx_bar.IsVcc() || mask_fx_bar.IsGnd()) {
    best_pivot = calculate_best_pivot(mask_fx);
    lit = 1 + find_overall_lit(mask_fx, best_pivot);
  } else {
    best_pivot = calculate_best_pivot(mask_fx);
    lit = 2 + find_overall_lit(mask_fx,best_pivot);
    best_pivot = calculate_best_pivot(mask_fx_bar);
    lit += find_overall_lit(mask_fx_bar, best_pivot);
  }
  return (lit);
}

/* returns integer from 0 to size */

void calculate_new_domain(char *domain_char, int size, int pos) {
  int i;
  int j;

  char temp_domain[4];

  memset(&temp_domain[0], '0', size * sizeof(char));
  j = 0;
  for (i = 0; i < size; i++) {
    if (i != pos) {
      temp_domain[j] = domain_char[i];
      j++;
    }
  }

  assert(j == size - 1);
  /* shrunk by one size */
  memcpy(domain_char, &temp_domain[0], (size - 1) * sizeof(char));
}

std::string generate_sop (const lutmask::LutMask  &mask) {

  char domain_char[4] = {'A', 'B', 'C', 'D'};


  if (mask.IsVcc()) {
    return("1");
  }

  if (mask.IsGnd()) {
    return("0");
  }

  unsigned int new_pos = lututil::calculate_best_pivot(mask);
  return(lututil::print_mask(mask, new_pos, &domain_char[0]));

}


} // namespace lututil
