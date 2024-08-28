#include "../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  int max_scale = s21_get_scale(value_1) > s21_get_scale(value_2)
                      ? s21_get_scale(value_1)
                      : s21_get_scale(value_2);
  int sign = s21_get_sign(value_1) ^ s21_get_sign(value_2);
  s21_set_null(result->bits);

  s21_big_decimal val1 = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal val2 = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_normalize(value_1, value_2, &val1, &val2);
  s21_big_decimal res = {{0, 0, 0, 0, 0, 0, 0, 0}};

  error = s21_big_mul(val1, val2, &res);

  error |= s21_convert_to_fit(res, sign, max_scale, result);
  if (error != 0 && result != NULL) s21_set_null(result->bits);
  s21_set_sign(result, sign);
  s21_set_scale(result, max_scale);
  return error;
}