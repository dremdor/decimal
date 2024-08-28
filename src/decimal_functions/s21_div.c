#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  if (s21_is_zero(value_1)) {
    if (result != NULL) s21_set_null(result->bits);
  } else if (s21_is_zero(value_2)) {
    error = 3;
  } else {
    int scale1 = s21_get_scale(value_1);
    int scale2 = s21_get_scale(value_2);
    // int max_scale = scale1 > scale2 ? scale1 : scale2;
    int sign = s21_get_sign(value_1) ^ s21_get_sign(value_2);
    s21_set_null(result->bits);

    s21_big_decimal val1 = {{0, 0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal val2 = {{0, 0, 0, 0, 0, 0, 0, 0}};
    s21_normalize(value_1, value_2, &val1, &val2);

    s21_big_decimal res = {{0, 0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal rest = s21_big_div(val1, val2, &res);

    int scale = (scale1 - scale2 < 0) ? 0 : (scale1 - scale2);

    if (s21_big_is_zero(rest) == 0) {
      while (!s21_big_is_zero(rest) && scale <= 28) {
        s21_big_decimal ten = {{0b1010, 0, 0, 0, 0, 0, 0, 0}};
        s21_big_mul(res, ten, &res);
        s21_big_mul(rest, ten, &rest);

        s21_big_decimal temp = {{0, 0, 0, 0, 0, 0, 0, 0}};
        rest = s21_big_div(rest, val2, &temp);
        s21_big_add(res, temp, &res);

        ++scale;
      }
    }
    error |= s21_convert_to_fit(res, sign, scale, result);
  }
  if (error != 0 && result != NULL) s21_set_null(result->bits);
  return error;
}
