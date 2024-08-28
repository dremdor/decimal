#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = 0;
  // Проверка входного указателя на NULL
  if (dst == NULL) {
    error = 1;  // Ошибка: некорректный указатель
  } else {
    // Инициализируем структуру dst нулями
    memset(dst, 0, sizeof(s21_decimal));

    // Проверяем знак числа
    if (src < 0) {
      dst->bits[3] = 1u << 31;  // Устанавливаем бит знака в bits[3]
      src = -src;  // Преобразуем отрицательное число в положительное
    }

    // Записываем абсолютное значение числа в младшие 32 бита (bits[0])
    dst->bits[0] = (unsigned int)src;

    // Проверка переполнения: bits[1] и bits[2] должны оставаться нулями, т.к.
    // int помещается в 32 бита
    if ((dst->bits[1] != 0 || dst->bits[2] != 0)) {
      return 1;  // Ошибка конвертации: переполнение
    }
  }

  return error;  // Конвертация успешна
}
