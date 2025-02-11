#include "framework.h"
#include "mock.h"

#include <stdint.h>
#include <stdio.h>
#include <limits.h>

static uint8_t u8_buff;
static uint16_t u16_buff;

// mock funtions for recording testing
static void uninterested_func(void) {
  MOCK_RECORD();
}

static void func(void) {
  MOCK_RECORD();
}

static uint8_t func_ret(void) {
  MOCK_RECORD_RET(uint8_t);
}

static void func_param_1(uint16_t param) {
  MOCK_RECORD_1_PARAM(uint16_t, param);
}

static uint32_t func_param_1_ret(uint64_t param) {
  MOCK_RECORD_1_PARAM_RET(uint64_t, param, uint32_t);
}

static void func_param_2(int8_t param_1, int16_t param_2) {
  MOCK_RECORD_2_PARAM(int8_t, param_1, int16_t, param_2);
}

static int32_t func_param_2_ret(int64_t param_1, char param_2) {
  MOCK_RECORD_2_PARAM_RET(int64_t, param_1, char, param_2, int32_t);
}

static uint8_t *func_ptr(uint16_t* param) {
  MOCK_RECORD_1_PARAM_RET(uint16_t*, param, uint8_t*);
}

// dummy functions to call mocks
static void test_func(void) {
  func();
  uninterested_func();

  uint8_t u8 = func_ret();
  if (u8 != UINT8_MAX) {
    printf("Wrong ret of func_ret(), actual: %u, expected: %u\n", u8, UINT8_MAX);
    exit(1);
  }

  func_param_1(UINT16_MAX);
  func_param_1(UINT16_MAX / 2);

  uint32_t u32 = func_param_1_ret(UINT64_MAX);
  if (u32 != UINT32_MAX) {
    printf("Wrong ret of func_param_1_ret(), actual: %u, expected: %u\n", u32, UINT32_MAX);
    exit(1);
  }

  func_param_2(INT8_MIN, INT16_MIN);

  int32_t i32 = func_param_2_ret(INT64_MIN, CHAR_MAX);
  if (i32 != INT32_MIN) {
    printf("Wrong ret of func_param_2_ret(), actual: %u, expected: %u\n", i32, INT32_MIN);
    exit(1);
  }

  uninterested_func();

  uint8_t *ptr = func_ptr(&u16_buff);
  if (ptr != &u8_buff) {
    printf("Wrong ret of func_param_2_ret(), mismatching pointers\n");
    exit(1);
  }
}

static void test_other_func(void) {
  uninterested_func();
  func_param_1(UINT16_MAX);
}

// TCs
static bool tc_test(void) {
  TEST_BEGIN();

  mock_uninterested_call("uninterested_func");

  MOCK_EXPECT("func", "%s", "");

  uint8_t u8 = UINT8_MAX;
  MOCK_EXPECT_RET("func_ret", uint8_t, u8, "%s", "");

  uint16_t u16 = UINT16_MAX;
  MOCK_EXPECT_1_PARAM("func_param_1", uint16_t, u16, "%s", "");
  MOCK_EXPECT_1_PARAM("func_param_1", mock_skip, MOCK_SKIP_T, "%s", "");

  uint32_t u32 = UINT32_MAX;
  uint64_t u64 = UINT64_MAX;
  MOCK_EXPECT_1_PARAM_RET("func_param_1_ret", uint64_t, u64, uint32_t, u32, "%s", "");

  int8_t i8 = INT8_MIN;
  int16_t i16 = INT16_MIN;
  MOCK_EXPECT_2_PARAM("func_param_2", int8_t, i8, int16_t, i16, "%s", "");

  int32_t i32 = INT32_MIN;
  int64_t i64 = INT64_MIN;
  char c = CHAR_MAX;
  MOCK_EXPECT_2_PARAM_RET("func_param_2_ret", int64_t, i64, char, c, int32_t, i32, "%s", "");

  uint8_t *u8_ptr = &u8_buff;
  uint16_t *u16_ptr = &u16_buff;
  MOCK_EXPECT_1_PARAM_RET("func_ptr", uint16_t*, u16_ptr, uint8_t*, u8_ptr, "%s", "");

  test_func();

  TEST_CHECK_MOCK();

  MOCK_EXPECT_1_PARAM("func_param_1", uint16_t, u16, "%s", "");

  test_other_func();

  TEST_ASSERT_EQ_WITH_MOCK(UINT64_MAX, UINT64_MAX, "matching");

  MOCK_EXPECT_1_PARAM("func_param_1", uint16_t, u16, "%s", "");

  test_other_func();

  TEST_ASSERT_EQ(12, 12, "matching");

  TEST_ASSERT_BOOL(true);

  TEST_END();
}

static bool tc_other_test(void) {
  TEST_BEGIN();

  MOCK_EXPECT("uninterested_func", "%s", "");

  uint16_t u16 = UINT16_MAX;
  MOCK_EXPECT_1_PARAM("func_param_1", uint16_t, u16, "%s", "");

  test_other_func();

  TEST_END();
}

int main(void) {
  TEST_EVALUATE_INIT();
  TEST_EVALUATE(tc_test());
  TEST_EVALUATE(tc_other_test());
  TEST_EVALUATE_END();
}
