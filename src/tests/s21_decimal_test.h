#ifndef __TESTS_S21_DECIMAL_TEST_H__
#define __TESTS_S21_DECIMAL_TEST_H__

#include <check.h>
#include <time.h>
#include <unistd.h>

#include "../s21_decimal.h"

typedef struct {
  uint32_t sign : 1;
  uint32_t t2_ : 7;
  uint32_t expo : 8;
  uint32_t t1_ : 16;
} s21_decimal_info;

s21_decimal_info *get_info(s21_decimal *dec);

Suite *s21_add_suite(void);
Suite *s21_sub_suite(void);
Suite *s21_mul_suite(void);
Suite *s21_div_suite(void);
Suite *s21_from_int_to_decimal_suite(void);
Suite *s21_from_decimal_to_int_suite(void);
Suite *s21_from_float_to_decimal_suite(void);
Suite *s21_from_decimal_to_float_suite(void);
Suite *s21_truncate_suite(void);
Suite *s21_negate_suite(void);
Suite *s21_floor_suite(void);
Suite *s21_round_suite(void);
Suite *s21_is_equal_suite(void);
Suite *s21_is_greater_or_equal_suite(void);
Suite *s21_is_greater_suite(void);
Suite *s21_is_less_or_equal_suite(void);
Suite *s21_is_less_suite(void);
Suite *s21_is_not_equal_suite(void);

void run_test(void);
void run_testcase(Suite *testcase);

#endif  // __TESTS_S21_DECIMAL_TEST_H__