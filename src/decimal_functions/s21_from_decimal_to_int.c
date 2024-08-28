#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  s21_decimal tmp = {0};

  s21_truncate(src, &tmp);
  //
  int result = tmp.bits[0];
  //  printf("31 bit: %d", src.bits[0] >> 30);
  if (tmp.bits[1] != 0 || tmp.bits[2] != 0 || (result >> 31)) {
    error = 1;  // Если старшие биты установлены или 31-й бит, то ошибка
                // переполнения
    *dst = 0;
  } else if (s21_get_sign(tmp) == 1 && tmp.bits[0]) {
    *dst = -result;
  } else if (s21_get_sign(tmp) != 1 && tmp.bits[0]) {
    *dst = result;
  }

  return error;
}
