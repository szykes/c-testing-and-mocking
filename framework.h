#ifndef C_TESTING_FRAMEWORK_H_
#define C_TESTING_FRAMEWORK_H_

#include "mock.h"

#include <stdbool.h>

#define log_error(format, ...) __own_log(__func__, __LINE__, "ERROR", format, ##__VA_ARGS__)
#define log_fail(format, ...) __own_log(__func__, __LINE__, "FAIL", format, ##__VA_ARGS__)
#define log_info(format, ...) __own_log(__func__, __LINE__, "INFO", format, ##__VA_ARGS__)
#define log_test(format, ...) __own_log("", 0, "TEST", format, ##__VA_ARGS__)

void __own_log(const char *func, unsigned int line, const char *lvl, const char *format, ...);

#define TEST_BEGIN() \
  log_test("%s", "--------------------------------------------------------"); \
  log_test("TC: %s", __func__);						\
  bool is_succeeded = true;						\
  mock_clear_calls();

#define TEST_ASSERT_EQ(actual, expected, msg)				\
  if (actual != expected) {						\
    log_fail("not equal, msg: '%s': actual: %d, expected: %d", msg, actual, expected); \
    is_succeeded = false;						\
  }

#define TEST_ASSERT_EQ_WITH_MOCK(actual, expected, msg)			\
  TEST_ASSERT_EQ(actual, expected, msg);				\
  TEST_CHECK_MOCK();

#define TEST_ASSERT_BOOL(value)						\
  if (!value) {								\
    log_fail("%s","not true");						\
    is_succeeded = false;						\
  }

#define TEST_CHECK_MOCK()						\
  if (!mock_is_succeeded()) {						\
    log_fail("%s", "Test FAILED!!!!!");					\
    exit(1);								\
  } else {								\
    mock_clear_calls();							\
  }

#define TEST_END()							\
  {									\
    mock_clear_all_uninterested_calls();				\
    bool is_mock_succeeded = mock_is_succeeded();			\
    if (is_succeeded && is_mock_succeeded) {				\
      log_test("%s", "Test succeeded");					\
      return true;							\
    }									\
    log_fail("%s", "Test FAILED!!!!!");					\
    exit(1);								\
  } while(0);

#define TEST_EVALUATE_INIT()						\
  bool is_succeeded = true;						\
  mock_init();

#define TEST_EVALUATE(value)						\
  if (!value) {								\
    is_succeeded = false;						\
  }

#define TEST_EVALUATE_END()						\
  log_test("%s", "--------------------------------------------------------"); \
  if (is_succeeded) {							\
    log_test("%s", "SUM: ALL Tests succeeded");				\
    return 0;								\
  }									\
  log_fail("%s", "SUM: Some Test(s) FAILED!!!!!");			\
  return 1;


#endif // C_TESTING_FRAMEWORK_H_
