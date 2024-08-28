#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  if (dst == NULL) {
    error = 1;  // Ошибка конвертации, указатель на результат равен NULL
  } else {
    // Инициализируем структуру dst нулями
    memset(dst, 0, sizeof(s21_decimal));

    if (src == 0.0f) {
      error = 0;  // Результат уже инициализирован нулями, возвращаем успех
    } else {
      // Проверяем знак числа
      if (src < 0) {
        dst->bits[3] = 1u << 31;  // Устанавливаем бит знака
        src =
            -src;  // Преобразуем в положительное число для дальнейших расчетов
      }

      // Находим масштаб (количество десятичных разрядов)
      int scale = 0;
      while ((src - (int)src) != 0 && scale < 28) {
        src *= 10.0f;
        scale++;
      }

      // Округляем число
      src = roundf(src);

      // Проверяем диапазон чисел, если значение слишком велико для s21_decimal
      if (src >= powf(2, 96)) {
        error = 1;  // Ошибка конвертации, число слишком велико
      } else {
        // Записываем значение в bits[0], bits[1], bits[2]
        dst->bits[0] = (unsigned int)src;
        src -= dst->bits[0];
        dst->bits[1] = (unsigned int)(src / powf(2, 32));
        src -= dst->bits[1] * powf(2, 32);
        dst->bits[2] = (unsigned int)(src / powf(2, 64));

        // Записываем масштаб в bits[3]
        dst->bits[3] |= (scale << 16);
      }
    }
  }

  return error;
}
