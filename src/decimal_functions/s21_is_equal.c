#include "../s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int equal = 0;

  s21_big_decimal val1 = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal val2 = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_normalize(value_1, value_2, &val1, &val2);

  if ((s21_get_sign(value_1) == s21_get_sign(value_2) &&
       s21_big_compare(val1, val2) == 0) ||
      (s21_big_is_zero(val1) && s21_big_is_zero(val2))) {
    equal = 1;
  }

  return equal;
}