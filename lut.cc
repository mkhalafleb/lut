#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <exception>
#include <iostream>
#include <optional>

#include "lutmask/lutmask.h"
#include "parse/parse.h"


unsigned int calculate_best_pivot(const lutmask::LutMask &mask);

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

void print_mask(const lutmask::LutMask mask, unsigned int pos, char *domain_char) {
  /* print function as xf(x) + x'f(x') */
  unsigned int new_pos = mask.Size() - 1;
  char char_name = domain_char[pos];
  char saved_domain[4];
  bool only_and = false;
  bool is_xor_only = false;
  bool single_lit = false;


  memcpy(&saved_domain[0], domain_char, mask.Size() * sizeof(char));


  if (mask.IsSingleLiteral()) {
    assert(!mask.IsVcc());
    assert(!mask.IsGnd());
    /* Either print x or x' */
    if (!mask.IsSingleLiteralInv()) {
      printf("%c", char_name);
    } else {
      printf("%c'", char_name);
    }
  } else if (mask.IsXorAtPos(pos)) {
    lutmask::LutMask mask_fx_bar = mask.CreateMaskIndependentOfPosBar(pos);
    new_pos = calculate_best_pivot(mask_fx_bar);
    calculate_new_domain(domain_char, mask.Size(), pos);

    is_xor_only = mask_fx_bar.IsXorAtPos(new_pos);
    single_lit = mask_fx_bar.IsSingleLiteralAtPos(new_pos);

    if ((!is_xor_only) && (!single_lit))
    {
      printf("%c $ (", char_name);
    } else {
      printf("%c $ ", char_name);
    }
    print_mask(mask_fx_bar, new_pos, domain_char);
    memcpy(domain_char, &saved_domain[0], mask.Size() * sizeof(char));
    if ((!is_xor_only) && (!single_lit))
    {
      printf(")");
    }
  } else if (!mask.IsIndependentOfPos(pos)) {
    /* Compute F(x) and F(x') */
    lutmask::LutMask mask_fx = mask.CreateMaskIndependentOfPos(pos);
    lutmask::LutMask mask_fx_bar = mask.CreateMaskIndependentOfPosBar(pos);

    /* print xf(x) */
    if (!mask_fx.IsGnd()) {
      if (mask_fx.IsVcc()) {
        printf("%c", char_name);
      } else {
        new_pos = calculate_best_pivot(mask_fx);
        calculate_new_domain(domain_char, mask.Size(), pos);
        only_and = mask_fx.IsLutMaskOnlyAndAtPos(new_pos);

        if (mask_fx_bar.IsVcc()) {
          /* DO not print x */
        } else {
          if (!only_and) {
            printf("%c & (", char_name);
          } else {
            printf("%c & ", char_name);
          }
        }
        print_mask(mask_fx, new_pos, domain_char);
        memcpy(domain_char, &saved_domain[0], mask.Size() * sizeof(char));
        if ((!only_and) && (!mask_fx_bar.IsVcc())) {
          printf(")");
        }
      }
      if (!mask_fx_bar.IsGnd()) {
        printf(" + ");
      }
    }

    /* Now print x' f(x') */
    if (!mask_fx_bar.IsGnd()) {
      new_pos = calculate_best_pivot(mask_fx_bar);
      calculate_new_domain(domain_char, mask.Size(), pos);
      only_and = mask_fx_bar.IsLutMaskOnlyAndAtPos(new_pos);
      if (mask_fx_bar.IsVcc())  {
        printf("%c'", char_name);
      } else {
        if (mask_fx.IsVcc()) {
          /* Do not print x' */
        } else {
          if (!only_and) {
            printf("%c' & (", char_name);
          } else {
            printf("%c' & ", char_name);
          }
        }
        print_mask(mask_fx_bar, new_pos, domain_char);
        memcpy(domain_char, &saved_domain[0], mask.Size() * sizeof(char));
        if ((!only_and) && (!mask_fx.IsVcc())) {
          printf(")");
        }
      }
    }
  } else {
    /* fx = fx' */
    lutmask::LutMask mask_fx = mask.CreateMaskIndependentOfPos(pos);
    new_pos = calculate_best_pivot(mask_fx);
    calculate_new_domain(domain_char, mask.Size(), pos);
    print_mask(mask_fx, new_pos, domain_char);
    memcpy(domain_char, &saved_domain[0], mask.Size() * sizeof(char));
  }
}

int main(int argc, char **argv) {
  int new_pos;
  char domain_char[4] = {'A', 'B', 'C', 'D'};

  auto sarg = lut_arg_parser::validate_arguments(argc, argv);

  std::optional<uint16_t> input_lutmask = lut_arg_parser::parse_hex(sarg);

  // If the arguments are illegal, it is a nullopt

  if (!input_lutmask) {
    printf("Illegal arguments\n");
    printf("Usage: lut <mask> (hex)\n");
    return (-1);
  }

  try {
    //   lutmask::LutMask (lut_mask, 5);
    //   lutmask::LutMask (0xF001, 2);
    lutmask::LutMask mask(*input_lutmask, 4);
    if (mask.IsVcc()) {
      printf("1\n");
    } else if (mask.IsGnd()) {
      printf("0\n");
    } else {
      new_pos = calculate_best_pivot(mask);
      print_mask(mask, new_pos, &domain_char[0]);
      printf("\n");
    }
  } catch (std::out_of_range &out_of_range) {
    std::cout << out_of_range.what() << std::endl;
  } catch (std::logic_error &logic_error) {
    std::cout << logic_error.what() << std::endl;
  }
  return (0);
}
