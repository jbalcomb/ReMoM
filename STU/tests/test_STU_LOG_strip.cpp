/* This TU is compiled with STU_LOG_MIN_SEVERITY=LOG_SEV_INFO (set in CMakeLists.txt). LOG_TRACE and LOG_DEBUG must expand to the sizeof-noop form so that argument expressions are not evaluated. */

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
#include <sstream>
#include <string>


static std::string slurp_strip_log(const char * path)
{
	std::ifstream f(path, std::ios::in | std::ios::binary);
	std::ostringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

TEST(stu_log_strip_test, TraceCallDoesNotEvaluateArguments)
{
	int counter = 0;
	LOG_TRACE(LOG_CAT_GENERAL, "side-effect probe %d", ++counter);
	EXPECT_EQ(counter, 0);
}

TEST(stu_log_strip_test, DebugCallDoesNotEvaluateArguments)
{
	int counter = 0;
	LOG_DEBUG(LOG_CAT_GENERAL, "side-effect probe %d", ++counter);
	EXPECT_EQ(counter, 0);
}

TEST(stu_log_strip_test, InfoCallStillEvaluatesArguments)
{
	int counter = 0;
	STU_Log_Startup(NULL);
	LOG_INFO(LOG_CAT_GENERAL, "side-effect probe %d", ++counter);
	STU_Log_Shutdown();
	EXPECT_EQ(counter, 1);
}

TEST(stu_log_strip_test, StrippedSeveritiesProduceNoOutput)
{
	STU_Log_Startup(NULL);
	LOG_TRACE(LOG_CAT_GENERAL, "stripped trace");
	LOG_DEBUG(LOG_CAT_GENERAL, "stripped debug");
	LOG_INFO (LOG_CAT_GENERAL, "kept info");
	STU_Log_Shutdown();

	std::string contents = slurp_strip_log("remom_log_new.txt");
	EXPECT_EQ(contents.find("stripped trace"), std::string::npos);
	EXPECT_EQ(contents.find("stripped debug"), std::string::npos);
	EXPECT_NE(contents.find("kept info"),       std::string::npos);
}

TEST(stu_log_strip_test, CompileTimeFloorBeatsPermissiveIni)
{
	const char * ini_path = "test_log_strip_floor.ini";
	{
		std::ofstream f(ini_path);
		f << "[Logging]\nseverity_threshold = TRACE\n";
	}

	int counter = 0;
	STU_Log_Startup(ini_path);
	LOG_TRACE(LOG_CAT_GENERAL, "should still be stripped %d", ++counter);
	LOG_DEBUG(LOG_CAT_GENERAL, "also stripped %d",            ++counter);
	LOG_INFO (LOG_CAT_GENERAL, "info kept %d",                ++counter);
	STU_Log_Shutdown();
	std::remove(ini_path);

	EXPECT_EQ(counter, 1);  /* only INFO evaluated its argument */

	std::string contents = slurp_strip_log("remom_log_new.txt");
	EXPECT_EQ(contents.find("should still be stripped"), std::string::npos);
	EXPECT_EQ(contents.find("also stripped"),            std::string::npos);
	EXPECT_NE(contents.find("info kept"),                std::string::npos);
}
