#include "s21_decimal_test.h"

START_TEST(s21_mul_test_1) {
  s21_decimal a = {.bits = {-1, -1, -1, 0}};  // 79228162514264337593543950335
  s21_decimal b = {.bits = {1, 0, 0, -2147483648}};  // -1
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  // ,

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)-2147483648);
}
END_TEST

START_TEST(s21_mul_test_2) {
  s21_decimal a = {.bits = {-2017942635, 47083, 0, 65536}};  // 20222222222222.9
  s21_decimal b = {.bits = {0, 0, 0, 0}};                    // 0
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_3) {
  s21_decimal a = {.bits = {292342, 0, 0, 327680}};  // 2.92342
  s21_decimal b = {.bits = {0, 0, 0, 0}};            // 0
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_4) {
  s21_decimal a = {.bits = {292342, 0, 0, -2147155968}};  // -2.92342
  s21_decimal b = {.bits = {781413, 0, 0, -2147483648}};  // -781413
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)806572558);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)53);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)327680);
}
END_TEST

START_TEST(s21_mul_test_5) {
  s21_decimal a = {
      .bits = {-1158315456, -197194979, 33, 0}};  // 626342352523521000000
  s21_decimal b = {.bits = {781413, 0, 0, -2147483648}};  // -781413
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)-1347460288);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)-177876063);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)26532164);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)-2147483648);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_6) {
  s21_decimal a = {
      {-1, -1, -1, -2147483648}};             // -79228162514264337593543950335
  s21_decimal b = {.bits = {-1, -1, -1, 0}};  // 79228162514264337593543950335
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 2);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_7) {
  s21_decimal a = {.bits = {-1, -1, -1, 0}};  // 79228162514264337593543950335
  s21_decimal b = {.bits = {-1, -1, -1, 0}};  // 79228162514264337593543950335
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_8) {
  s21_decimal a = {.bits = {1015837740, 287445, 0, 0}};  // 1234567890236460
  s21_decimal b = {.bits = {-2045900063, 28744, 0, 0}};  // 123456789023457
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_9) {
  s21_decimal a = {
      .bits = {-1345409089, 146074521, 0, 0}};  // 627385293423423423
  s21_decimal b = {
      {1668895137, 1064696021, 0, -2147483648}};  // -4572834592045224353
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 2);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_10) {
  s21_decimal a = {.bits = {5, 0, 0, -2147483648}};  // -5
  s21_decimal b = {.bits = {0, 0, 0, 0}};            // 0
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_11) {
  s21_decimal a = {.bits = {351661334, 63675, 0, 0}};  // 273482394234134
  s21_decimal b = {.bits = {7, 0, 0, 0}};              // 7
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)-1833337958);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)445725);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_12) {
  s21_decimal a = {.bits = {61237812, 0, 0, -2147024896}};  // -6.1237812
  s21_decimal b = {.bits = {81230311, 0, 0, 196608}};       // 81230.311
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)-183998228);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)1158184);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)-2146828288);
}
END_TEST

START_TEST(s21_mul_test_13) {
  s21_decimal a = {.bits = {17403, 0, 0, 0}};              // 17403
  s21_decimal b = {.bits = {4789282, 0, 0, -2147352576}};  // -47892.82
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)1743496022);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)19);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)-2147352576);
}
END_TEST

START_TEST(s21_mul_test_14) {
  s21_decimal a = {.bits = {72342342, 0, 0, 524288}};  // 0.72342342
  s21_decimal b = {.bits = {2, 0, 0, 1703936}};  // 0.00000000000000000000000002
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)145);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)1835008);
}
END_TEST

START_TEST(s21_mul_test_15) {
  s21_decimal a = {
      .bits = {610781734, 168435, 0, 983040}};    // 0.723423427283494
  s21_decimal b = {.bits = {784, 0, 0, 196608}};  // 0.784
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)2111509600);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)132053151);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)1179648);
}
END_TEST

START_TEST(s21_mul_test_16) {
  s21_decimal a = {.bits = {7938402, 0, 0, 0}};               // 7938402
  s21_decimal b = {.bits = {1830490002, 0, 0, -2147155968}};  // -18304.90002
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)-1654667292);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)3383300);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)-2147155968);
}
END_TEST

START_TEST(s21_mul_test_17) {
  s21_decimal a = {.bits = {2024072662, 18483, 0, 589824}};  // 79385.904604630
  s21_decimal b = {.bits = {1265466682, 55512, 0, 524288}};  // 2384234.90002234
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)1739821180);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)3802310573);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)1026059902);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)1114112);
}
END_TEST

START_TEST(s21_mul_test_18) {
  s21_decimal a = {
      {809805404, 16957601, 0, -2147483648}};            // -72832342523422300
  s21_decimal b = {.bits = {-1998155945, 28268, 0, 0}};  // 121412432334679
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 2);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_19) {
  s21_decimal a = {
      {809805404, 16957601, 0, -2147483648}};           // -72832342523422300
  s21_decimal b = {.bits = {652204261, 150968, 0, 0}};  // -648403274946789
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 2);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_20) {
  s21_decimal a = {.bits = {-1318475665, 151177, 0, 0}};  // 649303247399023
  s21_decimal b = {
      .bits = {652206196, 1509, 0, -2147221504}};  // -648175785.5860
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)166883916);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)2748916214);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)228150095);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)2147745792);
}
END_TEST

START_TEST(s21_mul_test_21) {
  s21_decimal a = {.bits = {0, 0, 0, 0}};  // 0
  s21_decimal b = {.bits = {0, 0, 0, 0}};  // 0
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_22) {
  s21_decimal a = {.bits = {292342, 0, 0, -2147155968}};  // -2.92342
  s21_decimal b = {.bits = {781413, 0, 0, -2147483648}};  // -781413
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)806572558);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)53);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)327680);
}
END_TEST

START_TEST(s21_mul_test_23) {
  s21_decimal a = {.bits = {999999999, 0, 0, 0}};  // 999999999
  s21_decimal b = {.bits = {999999999, 0, 0, 0}};  // 999999999
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)808348673);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)232830643);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_24) {
  s21_decimal a = {
      .bits = {-1, -1, -78293423, 0}};  // 77783903795975396563441680383
  s21_decimal b = {.bits = {-2147483648, -2147483648, 628902378,
                            0}};  // 11601201223536716331618402304
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_25) {
  s21_decimal a = {
      {-1, -1, -78293423, -2147483648}};  // -77783903795975396563441680383
  s21_decimal b = {.bits = {-2147483648, -2147483648, 628902378,
                            -2147483648}};  // -11601201223536716331618402304
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 1);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_26) {
  s21_decimal a = {.bits = {1, 0, 0, -2147483648}};  // -1
  s21_decimal b = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)0);
}
END_TEST

START_TEST(s21_mul_test_27) {
  s21_decimal a = {.bits = {8, 0, 0, 0}};  // 8
  s21_decimal b = {
      .bits = {1, 0, 0, 1835008}};  // 0.0000000000000000000000000001
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)8);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)1835008);
}
END_TEST

START_TEST(s21_mul_test_28) {
  s21_decimal a = {.bits = {10, 0, 0, 0}};  // 10
  s21_decimal b = {
      .bits = {1, 0, 0, 1835008}};  // 0.0000000000000000000000000001
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)10);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)1835008);
}
END_TEST

START_TEST(s21_mul_test_29) {
  s21_decimal a = {.bits = {8, 0, 0, 0}};            // 8
  s21_decimal b = {.bits = {1, 0, 0, -2147483648}};  // -1
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)8);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)-2147483648);
}
END_TEST

START_TEST(s21_mul_test_30) {
  s21_decimal a = {.bits = {10, 0, 0, 0}};           // 10
  s21_decimal b = {.bits = {1, 0, 0, -2147483648}};  // -1
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)10);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)0);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)-2147483648);
}
END_TEST

START_TEST(s21_mul_test_31) {
  s21_decimal a = {.bits = {1, 0, 0, -2147483648}};  // -1
  s21_decimal b = {.bits = {-1, -1, -1, 0}};  // 79228162514264337593543950335
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)-2147483648);
}
END_TEST

START_TEST(s21_mul_test_32) {
  s21_decimal a = {.bits = {1, 0, 0, 0}};  // 1
  s21_decimal b = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)-2147483648);
}
END_TEST

START_TEST(s21_mul_test_33) {
  s21_decimal a = {.bits = {-1, -1, -1, 0}};    // 79228162514264337593543950335
  s21_decimal b = {.bits = {1, 0, 0, 196608}};  // 0.001
  s21_decimal res_bits = {.bits = {0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(res_bits.bits[0], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[1], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[2], (uint32_t)-1);
  ck_assert_uint_eq(res_bits.bits[3], (uint32_t)196608);
}
END_TEST

Suite *s21_mul_suite(void) {
  Suite *s = suite_create("s21_mul");
  TCase *o = tcase_create("mul");

  tcase_add_test(o, s21_mul_test_1);
  tcase_add_test(o, s21_mul_test_2);
  tcase_add_test(o, s21_mul_test_3);
  tcase_add_test(o, s21_mul_test_4);
  tcase_add_test(o, s21_mul_test_5);
  tcase_add_test(o, s21_mul_test_6);
  tcase_add_test(o, s21_mul_test_7);
  tcase_add_test(o, s21_mul_test_8);
  tcase_add_test(o, s21_mul_test_9);
  tcase_add_test(o, s21_mul_test_10);
  tcase_add_test(o, s21_mul_test_11);
  tcase_add_test(o, s21_mul_test_12);
  tcase_add_test(o, s21_mul_test_13);
  tcase_add_test(o, s21_mul_test_14);
  tcase_add_test(o, s21_mul_test_15);
  tcase_add_test(o, s21_mul_test_16);
  tcase_add_test(o, s21_mul_test_17);
  tcase_add_test(o, s21_mul_test_18);
  tcase_add_test(o, s21_mul_test_19);
  tcase_add_test(o, s21_mul_test_20);
  tcase_add_test(o, s21_mul_test_21);
  tcase_add_test(o, s21_mul_test_22);
  tcase_add_test(o, s21_mul_test_23);
  tcase_add_test(o, s21_mul_test_24);
  tcase_add_test(o, s21_mul_test_25);
  tcase_add_test(o, s21_mul_test_26);
  tcase_add_test(o, s21_mul_test_27);
  tcase_add_test(o, s21_mul_test_28);
  tcase_add_test(o, s21_mul_test_29);
  tcase_add_test(o, s21_mul_test_30);
  tcase_add_test(o, s21_mul_test_31);
  tcase_add_test(o, s21_mul_test_32);
  tcase_add_test(o, s21_mul_test_33);

  suite_add_tcase(s, o);
  return s;
}