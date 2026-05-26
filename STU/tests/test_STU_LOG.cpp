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

	std::string contents = slurp_log_file("remom_log_new.txt");
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
	std::remove("remom_log_new.txt");
	log_pump();
	std::ifstream f("remom_log_new.txt");
	EXPECT_FALSE(f.good());
}

TEST(stu_log_test, MessageFormatHasAllFields)
{
	log_init(NULL);
	LOG_INFO(LOG_CAT_AIMOVE, "shape test message");
	log_shutdown();

	std::string contents = slurp_log_file("remom_log_new.txt");
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

	std::string contents = slurp_log_file("remom_log_new.txt");
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

	std::string contents = slurp_log_file("remom_log_new.txt");
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

	std::ifstream f("remom_log_new.txt", std::ios::ate | std::ios::binary);
	std::streamsize size_after_one_pump = f.tellg();
	EXPECT_LE(size_after_one_pump, (std::streamsize)(4096 + 256));  /* 4 KB cap + small slack for line boundary */
	EXPECT_GT(size_after_one_pump, (std::streamsize)2048);          /* should have drained meaningfully */

	for (int i = 0; i < 50; ++i)
	{
		log_pump();
	}
	log_shutdown();

	std::string contents = slurp_log_file("remom_log_new.txt");
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

	std::string contents = slurp_log_file("remom_log_new.txt");
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

	std::string contents = slurp_log_file("remom_log_new.txt");
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

	std::string contents = slurp_log_file("remom_log_new.txt");
	EXPECT_EQ(contents.find("aimove should be filtered"), std::string::npos);
	EXPECT_NE(contents.find("combat should appear"),      std::string::npos);
	EXPECT_NE(contents.find("general should appear"),     std::string::npos);
}

TEST(stu_log_test, IniMissingFileFallsBackToDefaults)
{
	log_init("definitely_does_not_exist_xyz.ini");
	LOG_INFO(LOG_CAT_GENERAL, "default config works");
	log_shutdown();

	std::string contents = slurp_log_file("remom_log_new.txt");
	EXPECT_NE(contents.find("default config works"), std::string::npos);
}

static void write_fixture_log(const char * path, const char * marker)
{
	std::ofstream f(path);
	f << "fixture content: " << marker << "\n";
}

TEST(stu_log_test, RotationWithAllThreeFixturesShifts)
{
	/* Pre-stage all three files with distinguishable contents, ensure shutdown so files are closed first. */
	log_shutdown();
	write_fixture_log("remom_log_previous.txt", "OLD_PREVIOUS");
	write_fixture_log("remom_log_current.txt",  "OLD_CURRENT");
	write_fixture_log("remom_log_new.txt",      "OLD_NEW");

	log_init(NULL);
	LOG_INFO(LOG_CAT_GENERAL, "post-rotation marker");
	log_shutdown();

	std::string previous = slurp_log_file("remom_log_previous.txt");
	std::string current  = slurp_log_file("remom_log_current.txt");
	std::string fresh    = slurp_log_file("remom_log_new.txt");

	EXPECT_NE(previous.find("OLD_CURRENT"), std::string::npos) << "previous should contain OLD_CURRENT after rotation";
	EXPECT_NE(current.find("OLD_NEW"),      std::string::npos) << "current should contain OLD_NEW after rotation";
	EXPECT_NE(fresh.find("post-rotation marker"), std::string::npos) << "new should contain the fresh message";
	EXPECT_EQ(previous.find("OLD_PREVIOUS"), std::string::npos)   << "OLD_PREVIOUS should have been deleted";
}

TEST(stu_log_test, RotationWithOnlyNewBecomesCurrent)
{
	log_shutdown();
	std::remove("remom_log_previous.txt");
	std::remove("remom_log_current.txt");
	write_fixture_log("remom_log_new.txt", "LONE_NEW");

	log_init(NULL);
	LOG_INFO(LOG_CAT_GENERAL, "after lone-new rotation");
	log_shutdown();

	std::string current = slurp_log_file("remom_log_current.txt");
	std::string fresh   = slurp_log_file("remom_log_new.txt");

	EXPECT_NE(current.find("LONE_NEW"), std::string::npos);
	EXPECT_NE(fresh.find("after lone-new rotation"), std::string::npos);
}

TEST(stu_log_test, RotationWithNoPriorFilesSucceeds)
{
	log_shutdown();
	std::remove("remom_log_previous.txt");
	std::remove("remom_log_current.txt");
	std::remove("remom_log_new.txt");

	log_init(NULL);
	LOG_INFO(LOG_CAT_GENERAL, "first-ever run");
	log_shutdown();

	std::string fresh = slurp_log_file("remom_log_new.txt");
	EXPECT_NE(fresh.find("first-ever run"), std::string::npos);

	std::ifstream prev("remom_log_previous.txt");
	std::ifstream curr("remom_log_current.txt");
	EXPECT_FALSE(prev.good()) << "previous should not exist on first-ever run";
	EXPECT_FALSE(curr.good()) << "current should not exist on first-ever run";
}

TEST(stu_log_test, LogFatalDrainsEntireRingPastPumpCap)
{
	log_init(NULL);
	/* Each message ~170 bytes; 1500 messages ≈ 255 KB — well past the 4 KB pump cap. */
	const int N = 1500;
	for (int i = 0; i < N; ++i)
	{
		LOG_INFO(LOG_CAT_GENERAL, "fatal_drain_msg %05d %s", i,
			"PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PADDING_PAD");
	}
	LOG_FATAL(LOG_CAT_GENERAL, "fatal terminator");
	/* NO log_shutdown, NO log_pump — LOG_FATAL's log_flush_all must drain everything past the 4 KB cap. */

	std::string contents = slurp_log_file("remom_log_new.txt");
	int info_count = 0;
	for (const std::string & line : split_lines(contents))
	{
		if (line.find("fatal_drain_msg ") != std::string::npos)
		{
			++info_count;
		}
	}
	EXPECT_EQ(info_count, N) << "all N messages should be on disk after LOG_FATAL";
	EXPECT_NE(contents.find("fatal terminator"), std::string::npos);

	log_shutdown();
}

TEST(stu_log_test, AtexitFlushesAfterCleanExit)
{
	ASSERT_EXIT({
		log_init(NULL);
		LOG_INFO(LOG_CAT_GENERAL, "atexit probe message");
		std::exit(0);
	}, ::testing::ExitedWithCode(0), ".*");

	std::string contents = slurp_log_file("remom_log_new.txt");
	EXPECT_NE(contents.find("atexit probe message"), std::string::npos);
}

/* gtest wraps test bodies in __try/__except on Windows, catching SEH before any SetUnhandledExceptionFilter-registered filter sees it. Disabling catch_exceptions for just this test lets the OS dispatch the access violation to STU_LOG's filter. Same flag is harmless on POSIX. */
TEST(stu_log_test, CrashHandlerWritesMarkerOnFatalSignal)
{
	const bool saved_catch = ::testing::GTEST_FLAG(catch_exceptions);
	::testing::GTEST_FLAG(catch_exceptions) = false;

	ASSERT_DEATH({
		log_init(NULL);
		LOG_INFO(LOG_CAT_GENERAL, "before crash marker");
		volatile int * p = (int *)0;
		*p = 42;
	}, ".*");

	::testing::GTEST_FLAG(catch_exceptions) = saved_catch;

	std::string contents = slurp_log_file("remom_log_new.txt");
	EXPECT_NE(contents.find("before crash marker"), std::string::npos);
	EXPECT_NE(contents.find("[CRASH]"), std::string::npos);
}
