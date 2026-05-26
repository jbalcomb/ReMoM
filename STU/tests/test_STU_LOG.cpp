#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../src/STU_LOG.h"
#ifdef __cplusplus
}
#endif

#include <cstdio>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>


static std::string slurp_log_file(const char * path)
{
	std::ifstream f(path, std::ios::in | std::ios::binary);
	std::ostringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

static std::vector<std::string> split_lines(const std::string & s)
{
	std::vector<std::string> out;
	std::istringstream iss(s);
	std::string line;
	while (std::getline(iss, line))
	{
		out.push_back(line);
	}
	return out;
}

TEST(stu_log_test, RoundTripNMessagesAppearInOrder)
{
	const int N = 100;

	log_init();
	for (int i = 0; i < N; ++i)
	{
		LOG_INFO(LOG_CAT_GENERAL, "round_trip_msg %d", i);
	}
	log_shutdown();

	std::string contents = slurp_log_file("remom_log.txt");
	ASSERT_FALSE(contents.empty());

	int found = 0;
	int last_idx = -1;
	for (const std::string & line : split_lines(contents))
	{
		std::string::size_type p = line.find("round_trip_msg ");
		if (p == std::string::npos)
		{
			continue;
		}
		int idx = -1;
		if (std::sscanf(line.c_str() + p, "round_trip_msg %d", &idx) == 1)
		{
			EXPECT_GT(idx, last_idx);
			last_idx = idx;
			++found;
		}
	}
	EXPECT_EQ(found, N);
}

TEST(stu_log_test, NoOutputWhenPumpHasNoInit)
{
	std::remove("remom_log.txt");
	log_pump();
	std::ifstream f("remom_log.txt");
	EXPECT_FALSE(f.good());
}

TEST(stu_log_test, MessageFormatHasAllFields)
{
	log_init();
	LOG_INFO(LOG_CAT_AIMOVE, "shape test message");
	log_shutdown();

	std::string contents = slurp_log_file("remom_log.txt");
	std::vector<std::string> lines = split_lines(contents);
	ASSERT_FALSE(lines.empty());

	const std::string & line = lines.back();
	EXPECT_TRUE(std::regex_search(line, std::regex(R"(^\[\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z\] )"))) << line;
	EXPECT_TRUE(std::regex_search(line, std::regex(R"(test_STU_LOG\.cpp:\d+ )"))) << line;
	EXPECT_NE(line.find("[INFO ]"),                       std::string::npos) << line;
	EXPECT_NE(line.find("[AIMOVE  ]"),                    std::string::npos) << line;
	EXPECT_NE(line.find(": shape test message"),          std::string::npos) << line;
}

TEST(stu_log_test, AllSixSeveritiesProduceOutput)
{
	log_init();
	LOG_TRACE(LOG_CAT_GENERAL, "trace msg");
	LOG_DEBUG(LOG_CAT_GENERAL, "debug msg");
	LOG_INFO (LOG_CAT_GENERAL, "info msg");
	LOG_WARN (LOG_CAT_GENERAL, "warn msg");
	LOG_ERROR(LOG_CAT_GENERAL, "error msg");
	/* LOG_FATAL handled in its own test — it flushes synchronously and would shut the file early if mixed here. */
	log_shutdown();

	std::string contents = slurp_log_file("remom_log.txt");
	EXPECT_NE(contents.find("[TRACE]"), std::string::npos);
	EXPECT_NE(contents.find("[DEBUG]"), std::string::npos);
	EXPECT_NE(contents.find("[INFO ]"), std::string::npos);
	EXPECT_NE(contents.find("[WARN ]"), std::string::npos);
	EXPECT_NE(contents.find("[ERROR]"), std::string::npos);
	EXPECT_NE(contents.find("trace msg"), std::string::npos);
	EXPECT_NE(contents.find("error msg"), std::string::npos);
}

TEST(stu_log_test, LogFatalFlushesWithoutPump)
{
	log_init();
	LOG_FATAL(LOG_CAT_GENERAL, "fatal flush test");
	/* DELIBERATELY no log_pump() or log_shutdown() — LOG_FATAL must flush on its own. */

	std::string contents = slurp_log_file("remom_log.txt");
	EXPECT_NE(contents.find("[FATAL]"), std::string::npos);
	EXPECT_NE(contents.find("fatal flush test"), std::string::npos);

	log_shutdown();
}
