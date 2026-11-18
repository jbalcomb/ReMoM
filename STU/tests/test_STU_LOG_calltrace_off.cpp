/* This TU forces STU_CALL_TRACE off: TRACE_ENTER / TRACE_EXIT must expand to the sizeof-noop form -- no output, no argument evaluation -- whatever the build config. */

#include <gtest/gtest.h>

#undef STU_CALL_TRACE
#define STU_CALL_TRACE 0

#ifdef __cplusplus
extern "C" {
#endif
#include "../src/STU_LOG.h"
#ifdef __cplusplus
}
#endif

#include <fstream>
#include <sstream>
#include <string>


static std::string slurp_calltrace_off_log(const char * path)
{
	std::ifstream f(path, std::ios::in | std::ios::binary);
	std::ostringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

TEST(stu_log_calltrace_off_test, CallTraceOffProducesNoOutput)
{
	int counter = 0;
	STU_Log_Startup(NULL);
	TRACE_ENTER_NAMED((++counter, "off_probe"));
	TRACE_EXIT_NAMED("off_probe");
	LOG_INFO(LOG_CAT_GENERAL, "calltrace-off info kept");
	STU_Log_Shutdown();

	EXPECT_EQ(counter, 0);

	std::string contents = slurp_calltrace_off_log("remom_log_new.txt");
	EXPECT_EQ(contents.find("off_probe"),                 std::string::npos);
	EXPECT_NE(contents.find("calltrace-off info kept"),   std::string::npos);
}
