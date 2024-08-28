#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int error = 0;
  int sign = s21_get_sign(value);
  s21_decimal temp = {{0, 0, 0, 0}};
  s21_truncate(value, &temp);
  if (sign == 1 && s21_is_not_equal(value, temp)) {
    s21_decimal one = {{0b1, 0, 0, 0}};
    error = s21_sub(temp, one, &temp);
  }
  *result = temp;
  s21_set_sign(result, sign);
  s21_set_scale(result, 0);
  return error;
}