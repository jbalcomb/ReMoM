#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../src/STU_UTIL.h"
#ifdef __cplusplus
}
#endif

#include <cstdio>
#include <ctime>


TEST(get_datetime_test, ProducesIso8601UtcFormat)
{
	char datetime[21] = { 0 };

	get_datetime(datetime);

	EXPECT_EQ(datetime[4], '-');
	EXPECT_EQ(datetime[7], '-');
	EXPECT_EQ(datetime[10], 'T');
	EXPECT_EQ(datetime[13], ':');
	EXPECT_EQ(datetime[16], ':');
	EXPECT_EQ(datetime[19], 'Z');
	EXPECT_EQ(datetime[20], '\0');

	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	int parsed = std::sscanf(datetime, "%4d-%2d-%2dT%2d:%2d:%2dZ", &year, &month, &day, &hour, &minute, &second);
	ASSERT_EQ(parsed, 6);

	EXPECT_GE(year, 1970);
	EXPECT_GE(month, 1);
	EXPECT_LE(month, 12);
	EXPECT_GE(day, 1);
	EXPECT_LE(day, 31);
	EXPECT_GE(hour, 0);
	EXPECT_LE(hour, 23);
	EXPECT_GE(minute, 0);
	EXPECT_LE(minute, 59);
	EXPECT_GE(second, 0);
	EXPECT_LE(second, 60);
}


TEST(get_datetime_test, IsCloseToCurrentUtcTime)
{
	char datetime[21] = { 0 };
	const std::time_t before = std::time(NULL);

	get_datetime(datetime);

	const std::time_t after = std::time(NULL);

	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	int parsed = std::sscanf(datetime, "%4d-%2d-%2dT%2d:%2d:%2dZ", &year, &month, &day, &hour, &minute, &second);
	ASSERT_EQ(parsed, 6);

	std::tm utc_tm = {};
	utc_tm.tm_year = year - 1900;
	utc_tm.tm_mon = month - 1;
	utc_tm.tm_mday = day;
	utc_tm.tm_hour = hour;
	utc_tm.tm_min = minute;
	utc_tm.tm_sec = second;
	utc_tm.tm_isdst = 0;

#ifdef _WIN32
	const std::time_t produced = _mkgmtime(&utc_tm);
#else
	const std::time_t produced = timegm(&utc_tm);
#endif

	ASSERT_NE(produced, static_cast<std::time_t>(-1));
	EXPECT_GE(produced, before - 2);
	EXPECT_LE(produced, after + 2);
}
