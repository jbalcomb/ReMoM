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

	log_init(NULL);
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
	log_init(NULL);
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
	log_init(NULL);
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
	log_init(NULL);
	LOG_FATAL(LOG_CAT_GENERAL, "fatal flush test");
	/* DELIBERATELY no log_pump() or log_shutdown() — LOG_FATAL must flush on its own. */

	std::string contents = slurp_log_file("remom_log.txt");
	EXPECT_NE(contents.find("[FATAL]"), std::string::npos);
	EXPECT_NE(contents.find("fatal flush test"), std::string::npos);

	log_shutdown();
}

static void write_ini_file(const char * path, const char * body)
{
	std::ofstream f(path);
	f << body;
}

TEST(stu_log_test, PumpCapsDrainAt4KB)
{
	log_init(NULL);
	/* Each message ~95-byte body + ~70-byte header ≈ 170 bytes. 200 messages ≈ 34 KB — well over 4 KB cap. */
	const int N = 200;
	for (int i = 0; i < N; ++i)
	{
		LOG_INFO(LOG_CAT_GENERAL, "pump_cap_msg %04d %s", i,
			"PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PAD");
	}
	log_pump();

	std::ifstream f("remom_log.txt", std::ios::ate | std::ios::binary);
	std::streamsize size_after_one_pump = f.tellg();
	EXPECT_LE(size_after_one_pump, (std::streamsize)(4096 + 256));  /* 4 KB cap + small slack for line boundary */
	EXPECT_GT(size_after_one_pump, (std::streamsize)2048);          /* should have drained meaningfully */

	for (int i = 0; i < 50; ++i)
	{
		log_pump();
	}
	log_shutdown();

	std::string contents = slurp_log_file("remom_log.txt");
	int found = 0;
	for (const std::string & line : split_lines(contents))
	{
		if (line.find("pump_cap_msg ") != std::string::npos)
		{
			++found;
		}
	}
	EXPECT_EQ(found, N);
}

TEST(stu_log_test, RingOverflowEmitsDropMarker)
{
	log_init(NULL);
	/* Each message ~170 bytes; ring is 2 MB. Write enough to overflow without any pump in between. */
	const int N = 20000;
	for (int i = 0; i < N; ++i)
	{
		LOG_INFO(LOG_CAT_GENERAL, "overflow_msg %05d %s", i,
			"PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PAD");
	}
	log_pump();
	log_shutdown();

	std::string contents = slurp_log_file("remom_log.txt");
	std::string::size_type marker_pos = contents.find("[LOGGER] ");
	ASSERT_NE(marker_pos, std::string::npos);

	unsigned long n_dropped = 0;
	int parsed = std::sscanf(contents.c_str() + marker_pos, "[LOGGER] %lu messages dropped", &n_dropped);
	EXPECT_EQ(parsed, 1);
	EXPECT_GT(n_dropped, 0u);
	EXPECT_LT(n_dropped, (unsigned long)N);
}

TEST(stu_log_test, IniSeverityThresholdFiltersBelowThreshold)
{
	const char * ini_path = "test_log_sev.ini";
	write_ini_file(ini_path, "[Logging]\nseverity_threshold = WARN\n");

	log_init(ini_path);
	LOG_INFO (LOG_CAT_GENERAL, "info should be filtered");
	LOG_WARN (LOG_CAT_GENERAL, "warn should appear");
	LOG_ERROR(LOG_CAT_GENERAL, "error should appear");
	log_shutdown();
	std::remove(ini_path);

	std::string contents = slurp_log_file("remom_log.txt");
	EXPECT_EQ(contents.find("info should be filtered"), std::string::npos);
	EXPECT_NE(contents.find("warn should appear"),      std::string::npos);
	EXPECT_NE(contents.find("error should appear"),     std::string::npos);
}

TEST(stu_log_test, IniCategoryMaskFiltersDisabledCategory)
{
	const char * ini_path = "test_log_cat.ini";
	write_ini_file(ini_path, "[Logging]\nAIMOVE = false\nCOMBAT = true\n");

	log_init(ini_path);
	LOG_INFO(LOG_CAT_AIMOVE, "aimove should be filtered");
	LOG_INFO(LOG_CAT_COMBAT, "combat should appear");
	LOG_INFO(LOG_CAT_GENERAL, "general should appear");
	log_shutdown();
	std::remove(ini_path);

	std::string contents = slurp_log_file("remom_log.txt");
	EXPECT_EQ(contents.find("aimove should be filtered"), std::string::npos);
	EXPECT_NE(contents.find("combat should appear"),      std::string::npos);
	EXPECT_NE(contents.find("general should appear"),     std::string::npos);
}

TEST(stu_log_test, IniMissingFileFallsBackToDefaults)
{
	log_init("definitely_does_not_exist_xyz.ini");
	LOG_INFO(LOG_CAT_GENERAL, "default config works");
	log_shutdown();

	std::string contents = slurp_log_file("remom_log.txt");
	EXPECT_NE(contents.find("default config works"), std::string::npos);
}
