#include "../s21_decimal.h"

// void print_bits(int num) {
//   for (int i = 31; i >= 0; --i) {
//     printf("%d", (num >> i) & 1);
//   }
// }
//
// void print_decimal(s21_decimal dec) {
//   for (int i = 31; i >= 0; --i) {
//     if (i == 31 || i == 24 || i == 16 || i == 0)
//       printf("%d ", (dec.bits[3] >> i) & 1);
//     else
//       printf("%d", (dec.bits[3] >> i) & 1);
//   }
//   for (int i = 2; i >= 0; --i) {
//     print_bits(dec.bits[i]);
//   }
//   printf("\n");
// }
//
// void print_big_decimal(s21_big_decimal dec) {
//   for (int i = 7; i >= 0; --i) {
//     print_bits(dec.bits[i]);
//     printf("\n");
//   }
//   printf("\n");
// }

int s21_get_bit(int *bits, int index) {
  int bit_index = index % 32;
  int word_index = index / 32;
  return (bits[word_index] >> bit_index) & 1;
}

void s21_set_bit(int *bits, int index, int value) {
  int bit_index = index % 32;
  int word_index = index / 32;
  if (value) {
    bits[word_index] |= (1 << bit_index);
  } else {
    bits[word_index] &= ~(1 << bit_index);
  }
}

void s21_set_sign(s21_decimal *dec, int value) {
  s21_set_bit(dec->bits, 127, value);
}

void s21_set_scale(s21_decimal *dec, int scale) { dec->bits[3] |= scale << 16; }

void s21_set_null(int *bits) {
  for (int i = 0; i < (int)(sizeof(*bits) / sizeof(int)); ++i) {
    bits[i] = 0;
  }
}

int s21_get_sign(s21_decimal dec) { return (dec.bits[3] >> 31) & 1; }

int s21_get_scale(s21_decimal dec) {
  s21_set_sign(&dec, 0);
  return dec.bits[3] >> 16;
}

void s21_from_decimal_to_big(s21_decimal src, s21_big_decimal *dest) {
  memset(dest->bits, 0, sizeof(dest->bits));
  for (int i = 2; i >= 0; --i) {
    for (int j = 31; j >= 0; --j) {
      int index = i * 32 + j;
      int value = (src.bits[i] >> j) & 1;
      s21_set_bit(dest->bits, index, value);
    }
  }
}

void s21_from_big_to_decimal(s21_big_decimal src, s21_decimal *dest) {
  memset(dest->bits, 0, sizeof(dest->bits));
  for (int i = 2; i >= 0; --i) {
    for (int j = 31; j >= 0; --j) {
      int index = i * 32 + j;
      int value = (src.bits[i] >> j) & 1;
      s21_set_bit(dest->bits, index, value);
    }
  }
}

int s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  int carry = 0;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 32; ++j) {
      int index = i * 32 + j;
      int a = s21_get_bit(value_1.bits, index);
      int b = s21_get_bit(value_2.bits, index);

      int sum = a ^ b ^ carry;
      carry = (a & b) | (carry & (a ^ b));
      s21_set_bit(result->bits, index, sum);
    }
  }
  return carry;
}

int s21_big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  int error = 0;
  s21_big_decimal temp_result = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal temp_value = value_1;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 32; ++j) {
      int value = s21_get_bit(value_2.bits, i * 32 + j);
      if (value) {
        error = s21_big_add(temp_result, temp_value, &temp_result);
      }
      s21_shift_left(&temp_value);
    }
  }
  *result = temp_result;
  return error;
}

int s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  int error = 0;
  unsigned int borrow = 0;
  for (int i = 0; i < 8; ++i) {
    unsigned int val1 = value_1.bits[i];
    unsigned int val2 = value_2.bits[i];
    unsigned int temp = value_1.bits[i] - value_2.bits[i] - borrow;

    if (val1 < val2 + borrow) {
      borrow = 1;
    } else {
      borrow = 0;
    }
    result->bits[i] = temp;
  }
  return error;
}

s21_big_decimal s21_big_div(s21_big_decimal value_1, s21_big_decimal value_2,
                            s21_big_decimal *result) {
  s21_big_decimal remainder = value_1;
  s21_big_decimal quotient = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_set_null(result->bits);

  while (s21_big_compare(remainder, value_2) >= 0) {
    int shift = 0;
    s21_big_decimal value = value_2;
    s21_big_decimal shifted_value = value;

    s21_shift_left(&shifted_value);
    while (s21_big_compare(remainder, shifted_value) >= 0) {
      value = shifted_value;
      s21_shift_left(&shifted_value);
      ++shift;
    }
    s21_big_sub(remainder, value, &remainder);
    s21_set_bit(quotient.bits, shift, 1);
  }

  *result = quotient;
  return remainder;
}

void s21_normalize(s21_decimal value_1, s21_decimal value_2,
                   s21_big_decimal *val1, s21_big_decimal *val2) {
  int scale1 = s21_get_scale(value_1);
  int scale2 = s21_get_scale(value_2);
  int diff_scale = scale1 > scale2 ? (scale1 - scale2) : (scale2 - scale1);

  s21_from_decimal_to_big(value_1, val1);
  s21_from_decimal_to_big(value_2, val2);

  s21_big_decimal ten = {{0b1010, 0, 0, 0, 0, 0, 0, 0}};
  if (scale1 < scale2) {
    for (int i = 0; i < diff_scale; ++i) {
      s21_big_mul(*val1, ten, val1);
    }
  } else {
    for (int i = 0; i < diff_scale; ++i) {
      s21_big_mul(*val2, ten, val2);
    }
  }
}

int s21_is_zero(s21_decimal value) {
  int zero = 1;
  for (int i = 0; i < 3 && zero; ++i) {
    if (value.bits[i] != 0) zero = 0;
  }
  return zero;
}

int s21_big_is_zero(s21_big_decimal value) {
  int zero = 1;
  for (int i = 0; i < 8 && zero; ++i) {
    if (value.bits[i] != 0) zero = 0;
  }
  return zero;
}

void s21_shift_left(s21_big_decimal *value) {
  unsigned int carry = 0;
  for (int i = 0; i < 8; ++i) {
    unsigned int next_carry = (value->bits[i] & (1 << 31)) ? 1 : 0;
    value->bits[i] <<= 1;
    value->bits[i] |= carry;
    carry = next_carry;
  }
}

// int s21_big_greater_or_equal(s21_big_decimal value_1, s21_big_decimal
// value_2) {
//   return s21_big_is_greater(value_1, value_2) ||
//          s21_big_is_equal(value_1, value_2);
// }
//
// int s21_big_is_equal(s21_big_decimal value_1, s21_big_decimal value_2) {
//   int equal = 0;
//
//   if (s21_big_compare(value_1, value_2) == 0) {
//     equal = 1;
//   }
//
//   return equal;
// }
//
// int s21_big_is_greater(s21_big_decimal value_1, s21_big_decimal value_2) {
//   int greater = 0;
//   int mem_diff = s21_big_compare(value_1, value_2);
//   if (mem_diff > 0)
//     greater = 1;
//   else
//     greater = 0;
//   return greater;
// }

int s21_big_compare(s21_big_decimal value_1, s21_big_decimal value_2) {
  int diff = 0;

  for (int i = 7; i >= 0 && diff == 0; --i) {
    for (int j = 31; j >= 0 && diff == 0; --j) {
      int index = i * 32 + j;
      int bit_1 = s21_get_bit(value_1.bits, index);
      int bit_2 = s21_get_bit(value_2.bits, index);
      if (bit_1 != bit_2) diff = bit_1 - bit_2;
    }
  }

  return diff;
}

int s21_convert_to_fit(s21_big_decimal value, int sign, int scale,
                       s21_decimal *result) {
  int error = 0;
  int dot_scale = 0;
  s21_big_decimal rest = {0};
  s21_big_decimal ten = {{0b1010, 0, 0, 0, 0, 0, 0, 0}};
  if (result != NULL) s21_set_null(result->bits);
  if (scale > 28) {
    dot_scale = scale - 28;
    rest = s21_big_truncate(value, &dot_scale, &value);
    dot_scale -= 1;
    s21_big_truncate(rest, &dot_scale, &rest);
  }
  while (scale >= 0 && !s21_big_is_fit(value)) {
    rest = s21_big_div(value, ten, &value);
    scale -= 1;
  }
  value = s21_big_bank_round(value, rest, 1);
  if (scale < 0 || !s21_big_is_fit(value)) {
    if (sign || (s21_big_is_zero(value) && !s21_big_is_zero(rest))) {
      error = 2;
    } else
      error = 1;
  } else if (error == 0 && result != NULL) {
    s21_from_big_to_decimal(value, result);
    s21_set_scale(result, scale);
    s21_set_sign(result, sign);
  }
  return error;
}

int s21_big_is_fit(s21_big_decimal value) {
  int is_fit = 1;
  for (int i = 7; i >= 3 && is_fit; --i) {
    if (value.bits[i] != 0) is_fit = 0;
  }
  return is_fit;
}

s21_big_decimal s21_big_bank_round(s21_big_decimal value, s21_big_decimal rest,
                                   int scale) {
  s21_big_decimal half = {{0b101, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal one = {{0b1, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_normalize(half, scale, &half);

  if (s21_big_compare(rest, half) > 0) {
    s21_big_add(value, one, &value);
  } else if (s21_big_compare(rest, half) == 0) {
    if (s21_get_bit(value.bits, 0)) {
      s21_big_add(value, one, &value);
    }
  }
  return value;
}

void s21_big_normalize(s21_big_decimal value, int shift,
                       s21_big_decimal *result) {
  s21_set_null(result->bits);
  s21_big_decimal ten = {{0b1010, 0, 0, 0, 0, 0, 0, 0}};

  *result = value;
  for (int i = 0; i < shift; ++i) {
    s21_big_mul(*result, ten, result);
  }
}

s21_big_decimal s21_big_truncate(s21_big_decimal value, int *scale,
                                 s21_big_decimal *result) {
  s21_set_null(result->bits);
  s21_big_decimal rest = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal ten = {{0b1010, 0, 0, 0, 0, 0, 0, 0}};
  if (*scale != 0) {
    while (*scale > 0) {
      rest = s21_big_div(value, ten, result);
      *scale -= 1;
    }
  } else
    *result = value;
  return rest;
}
