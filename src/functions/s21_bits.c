#include "../s21_decimal.h"

int s21_adding_mantissa(buffer value_1, buffer value_2, buffer* result) {
  int bit_value_1, bit_value_2;
  int carry = 0, is_overflow = OK;
  for (int index_bit = 0; index_bit < SIZE_BUFFER; ++index_bit) {
    bit_value_1 = s21_get_bit_buff(value_1, index_bit);
    bit_value_2 = s21_get_bit_buff(value_2, index_bit);
    int sum = bit_value_1 + bit_value_2 + carry;
    carry = sum >> 1;
    sum &= 1;
    s21_set_bit_buff(result, index_bit, sum);
  }
  is_overflow = s21_is_overflow_buffer(result);
  return is_overflow;
}

int s21_subtracting_mantissa(buffer value_1, buffer value_2, buffer* result) {
  int bit_value_1, bit_value_2;
  int carry = 0;
  for (int index_bit = 0; index_bit < SIZE_BUFFER; ++index_bit) {
    bit_value_1 = s21_get_bit_buff(value_1, index_bit);
    bit_value_2 = s21_get_bit_buff(value_2, index_bit);
    int diff = bit_value_1 - bit_value_2 - carry;
    if (diff < 0) {
      diff += 2;
      carry = 1;
    } else
      carry = 0;

    s21_set_bit_buff(result, index_bit, diff);
  }
  return s21_is_overflow_buffer(result);
}

int s21_multiplication_mantissa(buffer value_1, buffer value_2,
                                buffer* result) {
  buffer tmp = {0};
  s21_init_buffer(&tmp);
  int bit_value_2;
  int is_overflow = 0;
  for (int index_bit = 0; index_bit < SIZE_BUFFER && !is_overflow;
       ++index_bit) {
    bit_value_2 = s21_get_bit_buff(value_2, index_bit);
    if (bit_value_2) {
      buffer temp = value_1;
      for (int i = 0; i < index_bit; i++) {
        if (s21_shift_left(&temp)) {
          is_overflow = 1;
          break;
        }
      }
      if (is_overflow || (is_overflow = s21_adding_mantissa(temp, tmp, &tmp)))
        break;
    }
  }
  is_overflow = s21_is_overflow_buffer(result);
  if (!is_overflow) *result = tmp;

  return is_overflow;
}

int s21_divide_mantissa(buffer value_1, buffer value_2, buffer* result) {
  int is_overflow = OK;
  buffer tmp_res = {0};
  s21_init_buffer(result);
  s21_init_buffer(&tmp_res);
  tmp_res = *result;
  buffer dividend = value_1;
  buffer divisor = value_2;

  for (int index_bit = SIZE_BUFFER - 1; index_bit >= 0; index_bit--) {
    buffer tmp = dividend, num;
    s21_init_buffer(&num);
    num.bits[0] = 1;
    is_overflow = s21_shift_right_by_power(&tmp, index_bit);
    if (s21_is_greater_or_equal_mantissa(tmp, divisor)) {
      is_overflow = s21_shift_left_by_power(&num, index_bit);
      is_overflow = s21_call_arithm_operation_extention(&s21_adding_mantissa,
                                                        &tmp_res, num);
      buffer tmp2 = divisor;
      is_overflow = s21_shift_left_by_power(&tmp2, index_bit);
      s21_subtracting_mantissa(dividend, tmp2, &dividend);
    }
  }
  is_overflow = s21_is_overflow_buffer(&tmp_res);
  if (!is_overflow) *result = tmp_res;

  return is_overflow;
}

int s21_mod_mantissa(buffer value_1, buffer value_2, buffer* result) {
  s21_init_buffer(result);
  int is_overflow = OK;
  buffer dividend = value_1;
  buffer divisor = value_2;
  buffer tmp;
  s21_init_buffer(&tmp);
  is_overflow = s21_divide_mantissa(dividend, divisor, result);
  is_overflow = s21_call_arithm_operation_extention(
      &s21_multiplication_mantissa, result, divisor);
  tmp = *result;
  s21_subtracting_mantissa(dividend, tmp, result);

  for (int i = 7; i >= 1; i--) result->bits[i] = 0;
  is_overflow = s21_is_overflow_buffer(result);
  return is_overflow;
}

int s21_call_arithm_operation_extention(s21_arithm_operation_extention func,
                                        buffer* value_1, buffer value_2) {
  int is_overflow = 0;
  buffer tmp_res = {0};
  s21_init_buffer(&tmp_res);
  is_overflow = func(*value_1, value_2, &tmp_res);
  tmp_res.exp = value_1->exp;
  tmp_res.sign = value_1->sign;
  *value_1 = tmp_res;

  return is_overflow;
}

void s21_write_sign_and_exp(s21_decimal* value, int ex, bool sign) {
  value->bits[S_BIT] = ex;
  s21_shift(16, value, 0, S_BIT);
  s21_set_sign(value, sign);
}

bool s21_check_bit(s21_decimal str, int ind, int number) {
  return (str.bits[ind] & (1 << number));
}

void s21_shift(int length, s21_decimal* value, bool side,
               int ind) {  // side ? 0 - left : 1 - right
  if (!side)
    value->bits[ind] = (value->bits[ind] << length);
  else
    value->bits[ind] = (value->bits[ind] >> length);
}

int s21_get_exp(s21_decimal val) {
  s21_set_sign(&val, 1);
  s21_shift(16, &val, 1, S_BIT);
  int res = (int)val.bits[3] - 32768;
  return res == -32768 ? 0 : res;
}

int s21_get_bit(s21_decimal decimal, int index) {
  return (decimal.bits[INDEX_TETRAD(index)] >> INDEX_BIT(index)) & 1;
}

void s21_set_bit(s21_decimal* decimal, int index, bool bit) {
  if (bit == 0)
    decimal->bits[INDEX_TETRAD(index)] &= ~(1 << INDEX_BIT(index));
  else
    decimal->bits[INDEX_TETRAD(index)] |= 1 << INDEX_BIT(index);
}

int s21_get_bit_buff(buffer decimal, int index) {
  return (decimal.bits[INDEX_TETRAD(index)] >> INDEX_BIT(index)) & 1;
}

void s21_set_bit_buff(buffer* decimal, int index, int bit) {
  if (bit == 0)
    decimal->bits[INDEX_TETRAD(index)] &= ~(1 << INDEX_BIT(index));
  else
    decimal->bits[INDEX_TETRAD(index)] |= 1 << INDEX_BIT(index);
}

int s21_get_squared(s21_decimal decimal) {
  int sign = s21_get_sign(decimal);
  s21_set_sign(&decimal, 0);
  int squared = decimal.bits[3] >> 16;
  s21_set_sign(&decimal, sign);
  return squared;
}

void s21_set_squared(s21_decimal* decimal, int squared) {
  int sign = s21_get_sign(*decimal);
  decimal->bits[3] = squared;
  decimal->bits[3] <<= 16;
  s21_set_sign(decimal, sign);
}

int s21_get_sign(s21_decimal decimal) {
  return s21_get_bit(decimal, INDEX_SIGN);
}

void s21_set_sign(s21_decimal* decimal, bool sign) {
  s21_set_bit(decimal, INDEX_SIGN, sign);
}

int s21_has_zero(s21_decimal decimal) {
  int flag = 0;
  if ((decimal.bits[0] + decimal.bits[1] + decimal.bits[2]) == 0) flag = 1;
  return flag;
}

int s21_has_zero_buffer(buffer value) {
  int flag = 0, sum = 0;
  for (int i = 0; i < 6; ++i) sum += value.bits[i];
  if ((sum) == 0) flag = 1;

  return flag;
}

void s21_init_decimal(s21_decimal* decimal) {
  for (int i = 0; i <= 3; i++) decimal->bits[i] = 0;
}

int s21_shift_left(buffer* value) {
  int is_overflow = s21_get_bit_buff(*value, SIZE_BUFFER - 1);
  for (int i = SIZE_BUFFER - 1; i >= 0; i--) {
    if (i == 0)
      s21_set_bit_buff(value, i, 0);
    else
      s21_set_bit_buff(value, i, s21_get_bit_buff(*value, i - 1));
  }
  return is_overflow;
}

int s21_shift_right(buffer* value) {
  int is_overflow = s21_get_bit_buff(*value, 1);
  for (int i = 0; i < SIZE_BUFFER; i++) {
    if (i == SIZE_BUFFER - 1)
      s21_set_bit_buff(value, i, 0);
    else
      s21_set_bit_buff(value, i, s21_get_bit_buff(*value, i + 1));
  }
  return is_overflow;
}

int s21_shift_left_by_power(buffer* value, int num) {
  int err = 0;
  for (; num - 1 >= 0; num--) err = s21_shift_left(value);
  return err;
}

int s21_shift_right_by_power(buffer* value, int num) {
  int err = 0;
  for (; num - 1 >= 0; num--) err = s21_shift_right(value);
  return err;
}

void s21_normalization(buffer* value_1, buffer* value_2) {
  buffer* max_value = (value_1->exp > value_2->exp) ? value_1 : value_2;
  buffer* min_value = (value_1->exp < value_2->exp) ? value_1 : value_2;
  int e_max = max_value->exp;
  int e_min = min_value->exp;
  buffer ten = {0};
  s21_init_buffer(&ten);
  ten.bits[0] = 10;
  while (e_max != e_min) {
    if (e_min < EXP_MAX) {
      s21_call_arithm_operation_extention(&s21_multiplication_mantissa,
                                          min_value, ten);
      e_min++;
      min_value->exp = e_min;
    } else {
      s21_call_arithm_operation_extention(&s21_divide_mantissa, max_value, ten);
      e_max--;
      max_value->exp = e_max;
    }
  }
}

buffer s21_write_to_buffer(s21_decimal dec) {
  buffer buff = {0};
  s21_init_buffer(&buff);
  for (int i = 0; i < 3; i++) buff.bits[i] = dec.bits[i];
  buff.exp = s21_get_squared(dec);
  buff.sign = s21_get_sign(dec);
  return buff;
}

s21_decimal s21_write_to_decimal(buffer value) {
  s21_decimal res = {0};
  s21_init_decimal(&res);
  for (int i = 0; i < 3; ++i) res.bits[i] = value.bits[i];
  s21_set_squared(&res, value.exp);
  s21_set_sign(&res, value.sign);
  return res;
}

void s21_init_buffer(buffer* buff) {
  for (int i = 0; i < 6; i++) buff->bits[i] = 0;
  buff->exp = 0;
  buff->sign = 0;
}

int s21_is_overflow_decimal(buffer buff) {
  return (buff.bits[3] || buff.bits[4] || buff.bits[5] || buff.bits[6] ||
          buff.bits[7])
             ? 1
             : 0;
}

void s21_bank_rounding(buffer* value) {
  while ((s21_is_overflow_decimal(*value) && value->exp > 0) ||
         value->exp > 28) {
    buffer ten = get_ten(), one = get_one(), d_mod = {0};
    d_mod = *value;
    s21_call_arithm_operation_extention(&s21_mod_mantissa, &d_mod, ten);
    s21_call_arithm_operation_extention(&s21_divide_mantissa, value, ten);
    if (d_mod.bits[0] > 5) {
      s21_call_arithm_operation_extention(&s21_adding_mantissa, value, one);
    } else if (d_mod.bits[0] == 5) {
      if (!s21_is_even_number(*value))
        s21_call_arithm_operation_extention(&s21_adding_mantissa, value, one);
    }
    value->exp -= 1;
  }
}

int s21_is_overflow_buffer(buffer* result) {
  return s21_get_bit_buff(*result, SIZE_BUFFER - 1);
}

int s21_has_one_buffer(buffer value) {
  buffer one = {0};
  s21_init_buffer(&one);
  one.bits[0] = 1;
  return s21_is_equal_mantissa(value, one);
}

int s21_get_highest_bit(buffer dec) {
  int i = SIZE_BUFFER - 1;
  while (i) {
    if (s21_get_bit_buff(dec, i)) break;
    i--;
  }
  return i;
}

int s21_get_last_bit(buffer r) {
  if (!s21_has_zero_buffer(r))
    for (int i = 0; i < SIZE_BUFFER; i++) {
      if (s21_get_bit_buff(r, i)) return i;
    }
  return -1;
}

void s21_mul_ten(buffer* value) {
  int exp = value->exp, sign = value->sign;
  buffer tmp1 = *value;
  buffer tmp2 = *value;
  s21_init_buffer(value);
  s21_shift_left(&tmp1);
  s21_shift_left_by_power(&tmp2, 3);
  s21_adding_mantissa(tmp1, tmp2, value);
  value->exp = exp;
  value->sign = sign;
}

void s21_div_ten(buffer* value) {
  buffer ten = get_ten();
  s21_call_arithm_operation_extention(&s21_divided_buffer, value, ten);
}

buffer get_ten() {
  buffer ten = {0};
  s21_init_buffer(&ten);
  ten.bits[0] = 10u;
  return ten;
}

buffer get_one() {
  buffer one = {0};
  s21_init_buffer(&one);
  one.bits[0] = 1u;
  return one;
}

void s21_div_by_ten(buffer* value, int n) {
  for (int i = 0; i < n; i++) s21_div_ten(value);
}

void s21_mul_by_ten(buffer* value, int n) {
  for (int i = 0; i < n; ++i) s21_mul_ten(value);
}

int s21_is_even_number(buffer num) { return s21_get_bit_buff(num, 0) == 0; }

void s21_normalize_div(buffer* result) {
  buffer result_tmp = *result;
  int exp = result_tmp.exp;
  int flag = 0;
  if (s21_is_overflow_decimal(result_tmp) ||
      (get_last(result_tmp) % 10 == 0 && exp > 0) || exp > 28)
    flag = 1;
  while (flag && exp > 0) {
    int two_last = get_last(result_tmp);
    int remainder = two_last % 10, even = two_last / 10;
    s21_call_arithm_operation_extention(&s21_divided_buffer, &result_tmp,
                                        get_ten());
    if (even % 2 == 0 && remainder == 5) remainder--;
    if (remainder >= 5)
      s21_call_arithm_operation_extention(&s21_adding_mantissa, &result_tmp,
                                          get_one());
    exp--;
    if (exp > 28 && s21_has_zero_buffer(result_tmp))
      flag = 0;
    else if (s21_is_overflow_decimal(result_tmp))
      flag = 1;
    else if (get_last(result_tmp) % 10 == 0 && exp > 0)
      flag = 1;
    else if (exp > 28)
      flag = 1;
    else
      flag = 0;
  }
  result_tmp.exp = exp;
  *result = result_tmp;
}

int get_last(buffer value) {
  buffer temp = value;
  buffer one_hand = {0};
  s21_init_buffer(&one_hand);
  one_hand.bits[0] = 100u;

  s21_divided_buffer(temp, one_hand, &temp);
  s21_call_arithm_operation_extention(&s21_multiplication_mantissa, &temp,
                                      one_hand);
  s21_subtracting_mantissa(value, temp, &temp);

  return temp.bits[0];
}