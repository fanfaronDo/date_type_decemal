#include "../s21_decimal.h"

int s21_is_less(s21_decimal first, s21_decimal second) {
  bool result = 1, neg_flag = 0;
  if (s21_is_equal(first, second)) {
    result = 0;
  } else {
    buffer f = s21_write_to_buffer(first);
    buffer s = s21_write_to_buffer(second);
    s21_normalization(&f, &s);
    if (f.sign > s.sign)
      result = 1;
    else if (f.sign == s.sign) {
      if (f.sign == s.sign && s.sign == 1) neg_flag = 1;
      bool flag = 0;
      for (int i = 7; i >= 0; i--) {
        if (!flag) {
          if (f.bits[i] < s.bits[i])
            flag = 1;
          else if (f.bits[i] > s.bits[i]) {
            flag = 1;
            result = 0;
          }
        }
      }
    } else
      result = 0;
  }
  if (neg_flag) result = !result;
  return result;
}

int s21_is_greater(s21_decimal first, s21_decimal second) {
  return (s21_is_less(second, first));
}

int s21_is_less_or_equal(s21_decimal first, s21_decimal second) {
  return ((s21_is_equal(first, second)) || (s21_is_less(first, second)));
}

int s21_is_greater_or_equal(s21_decimal first, s21_decimal second) {
  return ((s21_is_equal(first, second)) | (s21_is_greater(first, second)));
}
int s21_is_equal(s21_decimal first, s21_decimal second) {
  buffer f = s21_write_to_buffer(first);
  buffer s = s21_write_to_buffer(second);
  s21_normalization(&f, &s);
  bool res = 1;
  if (f.sign == s.sign && f.exp == s.exp) {
    for (int i = 0; i < 8; i++) {
      if (f.bits[i] == s.bits[i]) {
        res *= 1;
      } else
        res = 0;
    }
  } else
    res = 0;
  return res;
}

int s21_is_not_equal(s21_decimal first, s21_decimal second) {
  return (!(s21_is_equal(first, second)));
}

int s21_is_less_mantissa(buffer value_1, buffer value_2) {
  int high_bit = 0;
  for (int index_bit = SIZE_BUFFER - 1; index_bit >= 0; index_bit--) {
    int bit_value_1 = s21_get_bit_buff(value_1, index_bit);
    int bit_value_2 = s21_get_bit_buff(value_2, index_bit);
    if (bit_value_1 ^ bit_value_2) {
      high_bit = bit_value_2;
      break;
    }
  }
  return high_bit;
}

int s21_is_equal_mantissa(buffer value_1, buffer value_2) {
  int high_bit = 1;
  for (int index_bit = SIZE_BUFFER - 1; index_bit >= 0; index_bit--) {
    int bit_value_1 = s21_get_bit_buff(value_1, index_bit);
    int bit_value_2 = s21_get_bit_buff(value_2, index_bit);
    if (bit_value_1 != bit_value_2) {
      high_bit = 0;
      break;
    }
  }
  return high_bit;
}

int s21_is_less_or_equal_mantissa(buffer value_1, buffer value_2) {
  return s21_is_less_mantissa(value_1, value_2) ||
         s21_is_equal_mantissa(value_1, value_2);
}

int s21_is_greater_mantissa(buffer value_1, buffer value_2) {
  return !s21_is_less_or_equal_mantissa(value_1, value_2);
}

int s21_is_greater_or_equal_mantissa(buffer value_1, buffer value_2) {
  return s21_is_greater_mantissa(value_1, value_2) ||
         s21_is_equal_mantissa(value_1, value_2);
}
