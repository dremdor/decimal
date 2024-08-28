#include <limits.h>
#include <stdio.h>

#include "s21_decimal.h"

int main() {
  s21_decimal value1 = {{0, 0b11111111111111111111111111111110, 0b1,
                         0b00000000000111000000000000000000}};
  s21_decimal value2 = {{0b111, 0b1, 0b110, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal answer = {
      {0b00101001101110010010011100000000, 0b00000101111101011110000100000000,
       0b00100011110000110100011000000000, 0b111000000000000000000}};

  int error = s21_add(value1, value2, &result);
  printf("%d\n", error);
  print_decimal(result);
  print_decimal(answer);
  return 0;
}
