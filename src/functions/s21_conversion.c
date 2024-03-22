#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  bool err = 0;
  if (dst) {
    int sign = s21_check_bit(src, S_BIT, INDEX_SIGN) ? -1 : 1;
    int number, ex;
    if (src.bits[0] > MAX_INT)
      err = 1;
    else {
      ex = s21_get_exp(src);
      number = floor((src.bits[0]) / pow(10, ex));
      *dst = number * sign;
    }
  } else
    err = 1;
  return err;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  bool err = 0;
  if (dst) {
    if (inMM(src)) {
      bool sign = src >= 0 ? 0 : 1;
      if (sign) s21_set_sign(dst, sign);
      dst->bits[0] = abs(src);
    } else
      err = 1;
  } else
    err = 1;
  return err;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
  bool err = 0, sign = 0;
  int ex = 0;
  long int base = fabs(src);
  //
  if (fabs(src) > 7922816251426433759 || (src > 0 && src < 1e-28) ||
      (isinf(src) || isnan(src) || !(dst)))
    err = 1;
  else if (src != 0) {
    if (src < 0) {
      sign = 1;
      src *= -1;
    }
    if (!((base) == src)) {
      while (src - ((float)base / (long int)(pow(10, ex))) != 0) {
        ex++;
        base = src * (long int)(pow(10, ex));
      }
    }
    dst->bits[0] = base;
    s21_write_sign_and_exp(dst, ex, sign);
  }
  return err;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  bool err = 0;
  if (dst) {
    uint64_t buf = 0;
    int sign = s21_check_bit(src, S_BIT, INDEX_SIGN) ? -1 : 1;
    buf = src.bits[0] + src.bits[1] * 4294967295 + src.bits[2] * 4294967295;
    *dst = (((float)buf) / pow(10, s21_get_exp(src))) * sign;
  } else {
    *dst = 0;
    err = 1;
  }
  return err;
}
