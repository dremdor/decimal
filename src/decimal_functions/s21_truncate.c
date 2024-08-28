#include "../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  int scale = s21_get_scale(value);
  if (scale != 0) {
    s21_big_decimal val1 = {{0, 0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal ten = {{0b1010, 0, 0, 0, 0, 0, 0, 0}};
    s21_from_decimal_to_big(value, &val1);

    while (scale > 0) {
      s21_big_div(val1, ten, &val1);
      --scale;
    }
    s21_from_big_to_decimal(val1, result);
  } else
    *result = value;
  s21_set_sign(result, s21_get_sign(value));
  s21_set_scale(result, 0);
  return error;
}