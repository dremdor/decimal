#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (dst == NULL) {
    return 1;  // Ошибка конвертации, указатель на результат равен NULL
  }

  // Извлекаем значение из bits[0], bits[1], и bits[2]
  double value = (double)src.bits[0];
  value += (double)src.bits[1] * (double)(1U << 31) *
           2.0;  // Заменяем сдвиг на умножение
  value +=
      (double)src.bits[2] * (double)(1U << 31) * 2.0 * (double)(1U << 31) * 2.0;

  // Извлекаем масштаб (scale) и применяем его
  int scale = (src.bits[3] >> 16) & 0xFF;
  while (scale-- > 0) {
    value /= 10.0;
  }

  // Проверяем и применяем знак
  if (src.bits[3] & (1u << 31)) {
    value = -value;
  }

  *dst = (float)value;  // Записываем результат в указанный адрес

  return 0;  // Успешная конвертация
}