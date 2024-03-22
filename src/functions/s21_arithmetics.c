#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = OK, is_overflow = OK;

  s21_init_decimal(result);
  buffer value_1_buffer = s21_write_to_buffer(value_1);
  buffer value_2_buffer = s21_write_to_buffer(value_2);
  buffer result_buffer = s21_write_to_buffer(*result);

  if ((s21_has_zero_buffer(value_1_buffer) ||
       s21_has_zero_buffer(value_2_buffer)))
    s21_find_sign_zero_add(value_1_buffer, value_2_buffer, &result_buffer);
  else
    err = s21_add_calculate(is_overflow, value_1_buffer, value_2_buffer,
                            &result_buffer);

  s21_bank_rounding(&result_buffer);

  if (!s21_is_overflow_decimal(result_buffer)) {
    *result = s21_write_to_decimal(result_buffer);
  } else
    err = (result_buffer.sign) ? IS_INF_NEG : IS_INF_POS;

  return err;
}

int s21_add_calculate(int is_overflow, buffer value_1_buffer,
                      buffer value_2_buffer, buffer *result_buffer) {
  if (value_1_buffer.exp != value_2_buffer.exp)
    s21_normalization(&value_1_buffer, &value_2_buffer);
  if (value_1_buffer.sign == value_2_buffer.sign) {
    is_overflow =
        s21_adding_mantissa(value_1_buffer, value_2_buffer, result_buffer);
    result_buffer->sign = value_1_buffer.sign | value_1_buffer.sign;
  } else if (s21_is_less_or_equal_mantissa(value_1_buffer, value_2_buffer)) {
    is_overflow =
        s21_subtracting_mantissa(value_2_buffer, value_1_buffer, result_buffer);
    if (s21_has_zero_buffer(*result_buffer))
      result_buffer->sign = value_1_buffer.sign;
    else
      result_buffer->sign = value_2_buffer.sign;
  } else {
    is_overflow =
        s21_subtracting_mantissa(value_1_buffer, value_2_buffer, result_buffer);
    result_buffer->sign = value_1_buffer.sign;
  }
  result_buffer->exp = value_1_buffer.exp;
  if (s21_has_zero_buffer(*result_buffer)) s21_init_buffer(result_buffer);

  return is_overflow;
}

void s21_find_sign_zero_add(buffer value_1, buffer value_2, buffer *result) {
  int is_zero_value_1 = s21_has_zero_buffer(value_1);
  int is_zero_value_2 = s21_has_zero_buffer(value_2);
  if (is_zero_value_1 && is_zero_value_2) {
    if (value_1.exp <= value_2.exp) {
      result->sign = value_1.sign;
      result->exp = value_2.exp;
    } else {
      result->sign = value_2.sign;
      result->exp = value_1.exp;
    }
  } else if (is_zero_value_1 && !is_zero_value_2) {
    *result = value_2;
  } else if (!is_zero_value_1 && is_zero_value_2) {
    *result = value_1;
  }
}
// работает через функцию s21_add
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = OK, is_overflow = OK;

  s21_set_sign(&value_2, !s21_get_sign(value_2));
  s21_init_decimal(result);
  buffer value_1_buffer = s21_write_to_buffer(value_1);
  buffer value_2_buffer = s21_write_to_buffer(value_2);
  buffer result_buffer = s21_write_to_buffer(*result);

  if ((s21_has_zero_buffer(value_1_buffer) ||
       s21_has_zero_buffer(value_2_buffer)))
    s21_find_sign_zero_add(value_1_buffer, value_2_buffer, &result_buffer);
  else
    err = s21_add_calculate(is_overflow, value_1_buffer, value_2_buffer,
                            &result_buffer);

  s21_bank_rounding(&result_buffer);

  if (!s21_is_overflow_decimal(result_buffer)) {
    *result = s21_write_to_decimal(result_buffer);
  } else
    err = (result_buffer.sign) ? IS_INF_NEG : IS_INF_POS;

  return err;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = OK;
  s21_init_decimal(result);
  buffer value_1_buffer = s21_write_to_buffer(value_1);
  buffer value_2_buffer = s21_write_to_buffer(value_2);
  buffer result_buffer = s21_write_to_buffer(*result);

  if (!s21_has_zero_buffer(value_1_buffer) ||
      !s21_has_zero_buffer(value_2_buffer)) {
    s21_multiplication_mantissa(value_1_buffer, value_2_buffer, &result_buffer);
  }
  if (!s21_has_zero_buffer(result_buffer)) {
    result_buffer.exp = (value_1_buffer.exp + value_2_buffer.exp);
    result_buffer.sign = value_1_buffer.sign ^ value_2_buffer.sign;
  }
  s21_bank_rounding(&result_buffer);

  if (!s21_is_overflow_decimal(result_buffer))
    *result = s21_write_to_decimal(result_buffer);
  else
    err = (result_buffer.sign) ? IS_INF_NEG : IS_INF_POS;

  return err;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = OK;
  s21_init_decimal(result);
  buffer value_1_buffer = s21_write_to_buffer(value_1);
  buffer value_2_buffer = s21_write_to_buffer(value_2);
  buffer result_buffer = s21_write_to_buffer(*result);
  int sign = (value_1_buffer.sign != value_2_buffer.sign) ? SIGN : NO_SIGN;

  if (s21_has_one_buffer(value_2_buffer)) {
    result_buffer = value_1_buffer;
    result_buffer.sign = sign;
    *result = s21_write_to_decimal(result_buffer);
  } else if (s21_has_zero_buffer(value_2_buffer)) {
    err = DEV_ZERO;
    s21_init_decimal(result);
  } else {
    s21_calculate_divided(value_1_buffer, value_2_buffer, &result_buffer);
    result_buffer.sign = sign;
    if (!s21_is_overflow_decimal(result_buffer)) {
      if (s21_has_zero_buffer(result_buffer)) result_buffer.sign = NO_SIGN;
      *result = s21_write_to_decimal(result_buffer);
    } else
      err = (result_buffer.sign) ? IS_INF_NEG : IS_INF_POS;
  }

  return err;
}

void s21_calculate_divided(buffer value_1, buffer value_2, buffer *result) {
  s21_mul_by_ten(&value_1, 35);
  value_1.exp += 35;
  s21_divided_buffer(value_1, value_2, result);
  s21_normalize_div(result);
}

int s21_divided_buffer(buffer value_1, buffer value_2, buffer *result) {
  s21_init_buffer(result);
  int exp_new = abs(value_1.exp - value_2.exp);
  int f = 0;
  buffer value_2_tmp = value_2;
  int diff_shift =
      (s21_get_highest_bit(value_1) - s21_get_highest_bit(value_2));

  if (diff_shift > 0) s21_shift_left_by_power(&value_2_tmp, diff_shift);

  while (s21_is_greater_or_equal_mantissa(value_2_tmp, value_2)) {
    f = 1;
    if (s21_is_greater_or_equal_mantissa(value_1, value_2_tmp)) {
      s21_set_bit_buff(result, diff_shift, 1);
      s21_call_arithm_operation_extention(&s21_subtracting_mantissa, &value_1,
                                          value_2_tmp);
    } else {
      s21_set_bit_buff(result, diff_shift, 0);
    }
    diff_shift--;
    s21_shift_right(&value_2_tmp);
  }
  if (f) result->exp = exp_new;

  return 0;
}
