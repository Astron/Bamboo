// Filename: test_helper.h
#include <cxxtest/TestSuite.h>

#define expect(expr) TS_ASSERT(expr)
#define assert(expr) \
    TS_ASSERT(expr); \
    return

#define expect_equals(x,y) TS_ASSERT_EQUALS(x,y)
#define assert_equals(x,y) \
    TS_ASSERT_EQUALS(x,y); \
    return
