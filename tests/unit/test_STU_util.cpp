#include <gtest/gtest.h>
#include <string.h>
#include "../../STU/src/STU_UTIL.h"

TEST(STU_util, get_datetime_writes_iso8601)
{
    char buf[64] = {0};
    get_datetime(buf);
    // Non-empty
    ASSERT_GT(strlen(buf), 0u);
    // Basic ISO8601 checks: contains 'T' and ends with 'Z'
    ASSERT_NE(strchr(buf, 'T'), nullptr);
    size_t len = strlen(buf);
    ASSERT_GT(len, 1u);
    ASSERT_EQ(buf[len-1], 'Z');
}
