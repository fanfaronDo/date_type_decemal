#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL_BITS (unsigned int)0x000000000
#define EXP_NULL (unsigned int)0x800000000
#define SIZE_DECIMAL 96
#define SIZE_TETRAD 32
#define INDEX_TETRAD(index) index / SIZE_TETRAD
#define INDEX_BIT(index) index % SIZE_TETRAD
#define INDEX_SIGN (SIZE_DECIMAL - 1) + SIZE_TETRAD
#define S_BIT 3  //бит с экспонентой и знаком
#define MAX_INT 2147483647
#define MIN_INT -2147483648
#define inMM(x) (x > MIN_INT || x < MAX_INT)
#define EXP_MAX 28
#define SIZE_BUFFER SIZE_TETRAD * 8
#define SIGN 1
#define NO_SIGN 0
#define isNAN(x) (x != x)

typedef struct {         // bits[0] младшие 32 бита
  unsigned int bits[4];  // bits[1] средние 32 бита
} s21_decimal;           // bits[2] старшие 32 бита

enum status_code {
  OK = 0,
  IS_INF_POS = 1,
  IS_INF_NEG = 2,
  DEV_ZERO = 3,
  CONVERTER_ERROR = 1,
};

typedef struct {
  unsigned int bits[8];
  int exp;
  bool sign;
} buffer;

// bits[3] содержит коэффициент масштабирования и знак
/*
 about bits[3]
- Биты от 0 до 15, младшее слово, не используются и должны быть равны нулю
- Биты с 16 по 23 должны содержать показатель степени от 0 до 28, который
указывает степень 10 для разделения целого числа
- Биты с 24 по 30 не используются и должны быть равны нулю
- Бит 31 содержит знак; 0 означает положительный, а 1 означает отрицательный
*/

//вспомогательные функции
bool s21_check_bit(s21_decimal str, int ind, int number);
// void s21_set_bit(s21_decimal *value, int ind, int number);
void s21_shift(int length, s21_decimal* value, bool side, int ind);
void s21_write_sign_and_exp(s21_decimal* value, int ex, bool sign);
int s21_get_exp(s21_decimal val);
// glory
void s21_set_sign(s21_decimal* decimal, bool sign);
int s21_adding_mantissa(buffer value_1, buffer value_2, buffer* result);
int s21_add_calculate(int is_overflow, buffer value_1, buffer value_2,
                      buffer* result);
int s21_subtracting_mantissa(buffer value_1, buffer value_2, buffer* result);
int s21_multiplication_mantissa(buffer value_1, buffer value_2, buffer* result);
int s21_divide_mantissa(buffer value_1, buffer value_2, buffer* result);
int s21_prepare_for_calculation(buffer* value_1, buffer* value_2);
void s21_calculate_divided(buffer value_1, buffer value_2, buffer* result);
int s21_divided_buffer(buffer value_1, buffer value_2, buffer* result);
int s21_mod_mantissa(buffer value_1, buffer value_2, buffer* result);
int s21_get_bit(s21_decimal decimal, int index);
void s21_set_bit(s21_decimal* decimal, int index, bool bit);
int s21_get_bit_buff(buffer decimal, int index);
void s21_set_bit_buff(buffer* decimal, int index, int bit);
int s21_get_squared(s21_decimal decimal);
void s21_set_squared(s21_decimal* decimal, int squared);
int s21_get_sign(s21_decimal decimal);
int s21_has_zero(s21_decimal decimal);
int s21_has_zero_buffer(buffer value);
int s21_has_one_buffer(buffer value);
int s21_shift_left(buffer* decimal);
int s21_shift_right(buffer* decimal);
void s21_init_decimal(s21_decimal* decimal);
void s21_init_buffer(buffer* buff);
int s21_shift_left_by_power(buffer* decimal, int num);
int s21_shift_right_by_power(buffer* decimal, int num);
void s21_normalization(buffer* value_1, buffer* value_2);
void s21_bank_rounding(buffer* value);
int s21_get_amount_shift(buffer value);
int s21_get_last_bit(buffer r);
void s21_find_sign_zero_add(buffer value_1, buffer value_2, buffer* result);
buffer s21_write_to_buffer(s21_decimal dec);
s21_decimal s21_write_to_decimal(buffer value);
int s21_is_even_number(buffer num);
int s21_is_overflow_decimal(buffer buff);
buffer get_ten();
buffer get_one();
void s21_normalize_div(buffer* result);
int get_last(buffer value);

int s21_is_overflow_buffer(buffer* result);
typedef int s21_arithm_operation_extention(buffer, buffer, buffer*);
int s21_call_arithm_operation_extention(s21_arithm_operation_extention func,
                                        buffer* value_1, buffer value_2);
int s21_is_less_mantissa(buffer value_1, buffer value_2);
int s21_is_equal_mantissa(buffer value_1, buffer value_2);
int s21_is_less_or_equal_mantissa(buffer value_1, buffer value_2);
int s21_is_greater_mantissa(buffer value_1, buffer value_2);
int s21_is_greater_or_equal_mantissa(buffer value_1, buffer value_2);
void s21_find_highest_bit(buffer v1, buffer v2, int* bit_1, int* bit_2);
int s21_equation_bits(buffer* value_1, buffer* value_2);
void s21_mul_ten(buffer* value);
void s21_mul_by_ten(buffer* value, int n);
void s21_div_ten(buffer* value);
void s21_div_by_ten(buffer* value, int n);
int s21_get_highest_bit(buffer dec);
int s21_offset_left(buffer* value, int value_offset);
int s21_div_only_bits(buffer value_1, buffer value_2, buffer* buf,
                      buffer* result);

// sult
int s21_check_sign(s21_decimal value);
int s21_check_exp(s21_decimal value);
int s21_check_decimal(s21_decimal value);

// Arithmetic Operators
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// Convertors and parsers
int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_float_to_decimal(float src, s21_decimal* dst);
int s21_from_decimal_to_int(s21_decimal src, int* dst);
int s21_from_decimal_to_float(s21_decimal src, float* dst);

// Comparison Operators
int s21_is_less(s21_decimal num1, s21_decimal num2);
int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_greater(s21_decimal num1, s21_decimal num2);
int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_equal(s21_decimal num1, s21_decimal num2);
int s21_is_not_equal(s21_decimal num1, s21_decimal num2);

// Others
int s21_negate(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_floor(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
