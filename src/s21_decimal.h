#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <math.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int bits[8];
} s21_big_decimal;

// Базовые функции
void s21_print_bits(int num);
void s21_print_decimal(s21_decimal dec);
void s21_print_big_decimal(s21_big_decimal dec);
int s21_get_bit(int *bits, int index);
void s21_set_bit(int *bits, int index, int value);
void s21_set_sign(s21_decimal *dec, int value);
void s21_set_scale(s21_decimal *dec, int scale);
void s21_set_null(int *bits);
int s21_get_sign(s21_decimal dec);
int s21_get_scale(s21_decimal dec);
void s21_from_decimal_to_big(s21_decimal src, s21_big_decimal *dest);
void s21_from_big_to_decimal(s21_big_decimal src, s21_decimal *dest);
void s21_normalize(s21_decimal value_1, s21_decimal value_2,
                   s21_big_decimal *val1, s21_big_decimal *val2);
int s21_is_zero(s21_decimal value);
int s21_big_is_zero(s21_big_decimal value);
void s21_shift_left(s21_big_decimal *value);
int s21_big_greater_or_equal(s21_big_decimal value1, s21_big_decimal value2);
int s21_big_is_equal(s21_big_decimal value_1, s21_big_decimal value_2);
int s21_big_is_greater(s21_big_decimal value_1, s21_big_decimal value_2);
int s21_big_compare(s21_big_decimal value_1, s21_big_decimal value_2);
int s21_convert_to_fit(s21_big_decimal value, int sign, int scale,
                       s21_decimal *result);
int s21_big_is_fit(s21_big_decimal value);
s21_big_decimal s21_big_bank_round(s21_big_decimal value, s21_big_decimal rest,
                                   int scale);
void s21_big_normalize(s21_big_decimal value, int shift,
                       s21_big_decimal *result);
s21_big_decimal s21_big_truncate(s21_big_decimal value, int *scale,
                                 s21_big_decimal *result);

// Арифметические операторы
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
s21_big_decimal s21_big_div(s21_big_decimal value_1, s21_big_decimal value_2,
                            s21_big_decimal *result);

// Операторы сравнения
int s21_is_less(s21_decimal, s21_decimal);           // Меньше <
int s21_is_less_or_equal(s21_decimal, s21_decimal);  // Меньше или равно <=
int s21_is_greater(s21_decimal, s21_decimal);  // Больше >
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  // Больше или равно >=
int s21_is_equal(s21_decimal, s21_decimal);      // Равно ==
int s21_is_not_equal(s21_decimal, s21_decimal);  // Равно !=

// Преобразователи
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result);
// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result);
// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result);
// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result);

#endif
