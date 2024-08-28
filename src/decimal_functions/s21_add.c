#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  int max_scale = s21_get_scale(value_1) > s21_get_scale(value_2)
                      ? s21_get_scale(value_1)
                      : s21_get_scale(value_2);
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  int sign = 0;
  s21_set_null(result->bits);
  s21_big_decimal val1 = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal val2 = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_normalize(value_1, value_2, &val1, &val2);
  s21_big_decimal res = {{0, 0, 0, 0, 0, 0, 0, 0}};

  if (sign1 == sign2) {
    sign = sign1;
    error = s21_big_add(val1, val2, &res);
  } else {
    int cmp = s21_big_compare(val1, val2);
    if (cmp > 0) {
      error = s21_big_sub(val1, val2, &res);
      sign = sign1;
    } else if (cmp < 0) {
      error = s21_big_sub(val2, val1, &res);
      sign = sign2;
    } else {
      res = (s21_big_decimal){{0, 0, 0, 0, 0, 0, 0, 0}};
      sign = 0;
    }
  }

  error |= s21_convert_to_fit(res, sign, max_scale, result);
  // from_big_to_decimal(res, result);
  if (error != 0 && result != NULL) s21_set_null(result->bits);
  s21_set_sign(result, sign);
  s21_set_scale(result, max_scale);
  return error;
}
