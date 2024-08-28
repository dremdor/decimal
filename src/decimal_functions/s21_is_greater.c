#include "../s21_decimal.h"

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int greater = 0;
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);

  s21_big_decimal val1 = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal val2 = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_normalize(value_1, value_2, &val1, &val2);
  if (sign1 == sign2) {
    int mem_diff = s21_big_compare(val1, val2);
    if (mem_diff > 0)
      greater = (sign1 == 0) ? 1 : 0;
    else if (mem_diff < 0)
      greater = (sign1 == 0) ? 0 : 1;
    else
      greater = 0;
  } else {
    if (s21_big_is_zero(val1) && s21_big_is_zero(val2))
      greater = 0;
    else
      greater = (sign1 == 0) ? 1 : 0;
  }
  return greater;
}