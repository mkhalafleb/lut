#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include "parse.h"
#include <optional>
#include <cstdio>
#include <exception>
#include "lutmask.h"

int position_mask[4] = {0xAAAA, 0xCCCC, 0xF0F0, 0xFF00};
int lut_size_mask[5] = {0x0001, 0x0003, 0x000F, 0x00FF, 0xFFFF};
            /* 4, 3, 2, 1, 0 */

int calculate_best_pivot(int, int);


int make_mask_independent_of(int lut_mask, int size, int pos)
{
  int shifter = 0x1;
  int small_shifter = 0x1;
  int work_mask = 0;
  int i;

  
  int shift_time = 0x1;
   shift_time <<= size;

  for (i=0; i < shift_time; i++)
  {
    if (position_mask[pos] & shifter)
    {
      if (lut_mask & shifter)
      {
        work_mask |= small_shifter;
      }
      small_shifter <<= 1;
    }
    shifter <<= 1;
  
  }
  return(work_mask);
}

int make_mask_independent_of_bar(int lut_mask, int size, int pos)
{
  int shifter = 0x1;
  int work_mask = 0;
  int small_shifter = 0x1;
  int i;

  int position_work_mask = (~position_mask[pos]) & lut_size_mask[size];

  int shift_time = 0x1;
  shift_time <<= size;


  for (i=0; i < shift_time; i++)
  {
    if (position_work_mask & shifter)
    {
      if (lut_mask & shifter)
      {
        work_mask |= small_shifter;
      }
      small_shifter <<= 1;
    }
    shifter <<=1;
  }
  return(work_mask);
}

bool is_xor
(
  int lut_mask,
  int size,
  int pos
)
{
  bool is_xor = false;

  int fx, fx_bar;

  fx = make_mask_independent_of(lut_mask, size, pos);
  fx_bar = make_mask_independent_of_bar(lut_mask, size, pos);

  if ((fx == 0) || (fx_bar == 0))
  {
    is_xor = false;
  }
  else if ((fx ^ fx_bar) == (-1 & lut_size_mask[size-1]))
  {
    /* is fx = !fx' */
    /* -1 & lut_size_mask[size] == VCC */
    is_xor = true;
  }


  return(is_xor);
}


bool is_mask_vcc(int lut_mask, int size)
{
  int vcc_mask = -1;  /* 0xFFF */
  bool is_vcc = false;

  vcc_mask &= lut_size_mask[size];

  if ((lut_mask & vcc_mask) == vcc_mask)
  {
    is_vcc = true;
  }

  return(is_vcc);
}

bool is_mask_gnd(int lut_mask, int size)
{
  bool is_gnd = false;

  if (lut_mask == 0)
  {
    is_gnd = true;
  }

  return(is_gnd);
}




bool is_independent_of
(
  int lut_mask,
  int size,
  int pos
)
{

  bool independent = false;
  int fx, fx_bar;

  fx = make_mask_independent_of(lut_mask, size, pos);
  fx_bar = make_mask_independent_of_bar(lut_mask, size, pos);
  if (fx == fx_bar)
  {
    independent = true;
  }
  
  return (independent);

}

int find_overall_lit(int lut_mask, int size, int pos)
{

  int lit = 0;
  int mask_fx_bar;
  int mask_fx;
  int best_pivot;


  if ((is_mask_vcc(lut_mask, size)) || (is_mask_gnd(lut_mask, size)))
  {
    lit = 0;
  }
  else if (size == 1)
  {
    /* There is 1 literal */
    lit = 1;

  }
  else
  {
    mask_fx = make_mask_independent_of(lut_mask, size, pos);
    mask_fx_bar = make_mask_independent_of_bar(lut_mask, size, pos);

    if (is_xor(lut_mask, size, pos))
    {
      best_pivot = calculate_best_pivot(mask_fx_bar, size-1);
      lit = 1 + find_overall_lit(mask_fx_bar, size-1, best_pivot);
    }
    else if (is_mask_vcc(mask_fx, size-1)
        || is_mask_gnd(mask_fx, size-1))
    {
      best_pivot = calculate_best_pivot(mask_fx_bar, size-1);
      lit = 1 + find_overall_lit(mask_fx_bar, size-1, best_pivot);
    }
    else if (is_mask_vcc(mask_fx_bar, size-1)
        || is_mask_gnd(mask_fx_bar, size-1))
    {
      best_pivot = calculate_best_pivot(mask_fx, size-1);
      lit = 1 + find_overall_lit(mask_fx, size-1, best_pivot);
    }
    else
    {
      best_pivot = calculate_best_pivot(mask_fx, size-1);
      lit = 2 + find_overall_lit(mask_fx, size-1, best_pivot);
      best_pivot = calculate_best_pivot(mask_fx_bar, size-1);
      lit = lit + find_overall_lit(mask_fx_bar, size-1, best_pivot);
      
    }
  }

  return(lit);

}


/* returns integer from 0 to size */
int calculate_best_pivot(int lut_mask, int size)
{
  int best_pivot;
  bool found_xor = false;
  int i;
  int num_lit[4];
  int smallest;


  if (size == 1)
  {
    best_pivot = 0;
  }
  else
  {
    /* initialize literals */
    memset(&num_lit[0], 0, size * sizeof(int));

    for (i=0;  i < size; i++)
    {
      num_lit[i] = find_overall_lit(lut_mask, size, i);
    }

    smallest = 0;
    best_pivot = 0;

    for (i = 0; i < size ; i++)
    {
      if (   (num_lit[i] < smallest)
        || (smallest == 0) )
      {
        if (num_lit[i] != 0)
        {
          smallest = num_lit[i];
          best_pivot = i;
        }
      }
    }
  }
  
  return(best_pivot);
}


bool is_lut_mask_only_and(int lut_mask, int size, int pos)
{
  bool only_and = false;

  int mask_fx, mask_fx_bar;

  mask_fx = make_mask_independent_of(lut_mask, size, pos);
  mask_fx_bar = make_mask_independent_of_bar(lut_mask, size, pos);

  if ((is_mask_gnd(mask_fx, size-1)) || (is_mask_gnd(mask_fx_bar, size-1)))
  {
    only_and = true;
  }
  return(only_and);
}

bool is_single_literal(int lut_mask, int size, int pos)
{
  bool single_literal = false;
  int mask_fx, mask_fx_bar;

  mask_fx = make_mask_independent_of(lut_mask, size, pos);
  mask_fx_bar = make_mask_independent_of_bar(lut_mask, size, pos);

  if (((is_mask_gnd(mask_fx, size-1)) && (is_mask_vcc(mask_fx_bar, size-1)))
    || ((is_mask_gnd(mask_fx_bar, size-1)) && (is_mask_vcc(mask_fx, size-1))))
  {
    single_literal = true;
  }
  return(single_literal);
}


void calculate_new_domain(char *domain_char, int size, int pos)
{
  int i;
  int j;

  char temp_domain[4];

  memset(&temp_domain[0], '0', size * sizeof(char));
  j = 0;
  for (i = 0; i < size; i++)
  {
    if (i != pos)
    {
      temp_domain[j] = domain_char[i];
      j++;
    }
  }

  assert(j == size - 1);
  /* shrunk by one size */
  memcpy(domain_char, &temp_domain[0], (size - 1) * sizeof(char));

  
}

void print_mask(int lut_mask, int size, int pos, char *domain_char)
{

  /* print function as xf(x) + x'f(x') */
  int mask_fx;
  int mask_fx_bar;
  int new_pos = size - 1;
  char char_name = domain_char[pos];
  char saved_domain[4];
  bool only_and = false;
  bool is_xor_only = false;
  bool single_lit = false;


  assert((size <= 4) && (size >= 0));
  assert((pos <= size) && (pos >= 0));

  memcpy(&saved_domain[0], domain_char, size * sizeof(char));

  if (size == 1)
  {
    assert(!is_mask_vcc(lut_mask, size));
    assert(!is_mask_gnd(lut_mask, size));
    /* Either print x or x' */
    if (lut_mask == 0x2)
    {
      printf("%c", char_name);
    }
    else
    {
      printf("%c'", char_name);
    }
  }
  else if (is_xor(lut_mask, size, pos))
  {

    mask_fx_bar = make_mask_independent_of_bar(lut_mask, size, pos);
    new_pos = calculate_best_pivot(mask_fx_bar, size - 1);
    calculate_new_domain(domain_char, size, pos);

    is_xor_only = is_xor(mask_fx_bar, size-1, new_pos);
    single_lit = is_single_literal(mask_fx_bar, size-1, new_pos);

    if ((!is_xor_only) && (!single_lit))
        
    {
      printf("%c $ (", char_name);
    }
    else
    {
      printf("%c $ ", char_name);
      
    }
    print_mask(mask_fx_bar, size - 1, new_pos, domain_char);
    memcpy(domain_char, &saved_domain[0], size * sizeof(char));
    if ((!is_xor_only) && (!single_lit))
          
    {
      printf(")");
    }

  }
  else if (!is_independent_of(lut_mask, size, pos))
  {
    /* Compute F(x) and F(x') */
    mask_fx = make_mask_independent_of(lut_mask, size, pos);
    mask_fx_bar = make_mask_independent_of_bar(lut_mask, size, pos);

      

    /* print xf(x) */
    if (!is_mask_gnd(mask_fx, size - 1))
    {
      if (is_mask_vcc(mask_fx, size - 1))
      {
        printf("%c", char_name);
      }
      else
      {
        new_pos = calculate_best_pivot(mask_fx, size - 1);
        calculate_new_domain(domain_char, size, pos);
        only_and = is_lut_mask_only_and(mask_fx, size-1, new_pos);

        if (is_mask_vcc(mask_fx_bar, size - 1))
        {
          /* DO not print x */
        }
        else
        {
          if (!only_and)
          {
            printf("%c & (", char_name);
          }
          else
          {
            printf("%c & ", char_name);
          }

        }
        print_mask(mask_fx, size - 1, new_pos, domain_char);
        memcpy(domain_char, &saved_domain[0], size * sizeof(char));
        if ((!only_and) && (!is_mask_vcc(mask_fx_bar, size - 1)))
        {
          printf(")");
        }
      }
      if (!is_mask_gnd(mask_fx_bar, size - 1))
      {
        printf(" + ");
      }
    }


    /* Now print x' f(x') */
    if (!is_mask_gnd(mask_fx_bar, size - 1))
    {
      new_pos = calculate_best_pivot(mask_fx_bar, size -1);
      calculate_new_domain(domain_char, size, pos);
      only_and = is_lut_mask_only_and(mask_fx_bar, size-1, new_pos);
      if (is_mask_vcc(mask_fx_bar, size - 1))
      {
        printf("%c'", char_name);
      }
      else
      {
        if (is_mask_vcc(mask_fx, size - 1))
        {
          /* Do not print x' */
        }
        else
        {
          if (!only_and)
          {
            printf("%c' & (", char_name);
          }
          else
          {
            printf("%c' & ", char_name);
          }
        }
        print_mask(mask_fx_bar, size - 1, new_pos, domain_char);
        memcpy(domain_char, &saved_domain[0], size * sizeof(char));
        if ((!only_and) && (!is_mask_vcc(mask_fx, size -1)))
        {
          printf(")");
        }
      }
    }
  }
  else
  {
    /* fx = fx' */
    mask_fx = make_mask_independent_of(lut_mask, size, pos);
    new_pos = calculate_best_pivot(mask_fx, size - 1);
    calculate_new_domain(domain_char, size, pos);
    print_mask(mask_fx, size - 1, new_pos, domain_char);
    memcpy(domain_char, &saved_domain[0], size * sizeof(char));
  }


}



int main
(
  int argc,
  char **argv
)
{
  int lut_mask;
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
    // casting for now and later will clean up
    lut_mask = static_cast <int> (*input_lutmask);
    lutmask::LutMask (lut_mask, 5);
    printf("LUTMASK %x\n", lut_mask);
    if (is_mask_vcc(lut_mask, 4))
    {
      printf("1\n");
    }
    else if (is_mask_gnd(lut_mask, 4))
    {
      printf("0\n");
    }
    else
    {
      new_pos = calculate_best_pivot(lut_mask, 4);
      print_mask(lut_mask, 4, new_pos, &domain_char[0]);
      printf("\n");
    }
  } catch (std::out_of_range &out_of_range) {
    std::cout << out_of_range.what() << std::endl;
  }
  return(0);
}
