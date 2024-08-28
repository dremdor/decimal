#include "../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int error = 0;
  int sign = s21_get_sign(value);

  s21_decimal temp = {{0, 0, 0, 0}};
  error |= s21_truncate(value, &temp);

  s21_decimal frac_part = {{0, 0, 0, 0}};
  s21_decimal half = {{0b101, 0, 0, 0b10000000000000000}};
  error |= s21_sub(value, temp, &frac_part);
  s21_set_sign(&frac_part, 0);
  s21_set_sign(&half, 0);

  if (s21_is_greater_or_equal(frac_part, half)) {
    s21_decimal one = {{0b1, 0, 0, 0}};
    if (sign == 1)
      error |= s21_sub(temp, one, &temp);
    else
      error |= s21_add(temp, one, &temp);
  }

  *result = temp;
  s21_set_sign(result, sign);
  s21_set_scale(result, 0);
  return error;
}