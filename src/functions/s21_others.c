#include "../s21_decimal.h"

int s21_check_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }
int s21_check_exp(s21_decimal value) {
  int exp = 0;
  exp = (value.bits[3] >> 16) & 0x00ff;
  return exp;
}

int s21_check_decimal(s21_decimal value) {
  int result = 0;
  if ((value.bits[3] & 0x7f00ffff) == 0) result = 1;
  return result;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int exp = s21_check_exp(value);
  if (exp != 0) {
    s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
    bool sign = s21_check_sign(value);
    s21_truncate(value, result);
    if (sign && (exp != 0)) s21_sub(*result, one, result);
  } else
    *result = value;
  return 0;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (s21_check_decimal(value)) {
    *result = value;
    (*result).bits[3] ^= 0x80000000;
  } else
    err = 1;
  return err;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal ten = {{0x0000000a, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal five = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};
  buffer buff_ten = s21_write_to_buffer(ten);
  bool sign = s21_check_sign(value);
  int exp = s21_check_exp(value);
  if (exp != 0) {
    buffer buff_result = s21_write_to_buffer(value);
    buffer buff_mod;
    s21_div_by_ten(&buff_result, exp - 1);

    s21_mod_mantissa(buff_result, buff_ten, &buff_mod);
    s21_div_ten(&buff_result);

    *result = s21_write_to_decimal(buff_result);
    result->bits[3] = NULL_BITS;  // or = 0;
    s21_decimal mod = s21_write_to_decimal(buff_mod);
    if (s21_is_greater_or_equal(mod, five)) s21_add(*result, one, result);
    s21_set_sign(result, sign);
  } else
    *result = value;
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int sign = s21_get_sign(value);
  buffer buff_result = s21_write_to_buffer(value);
  int exp = s21_check_exp(value);
  s21_div_by_ten(&buff_result, exp);
  *result = s21_write_to_decimal(buff_result);
  s21_write_sign_and_exp(result, 0, sign);
  return 0;
}
