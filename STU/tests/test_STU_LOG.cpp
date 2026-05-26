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


static std::string slurp_log_file(const char * path)
{
	std::ifstream f(path, std::ios::in | std::ios::binary);
	std::ostringstream ss;
	ss << f.rdbuf();
	return ss.str();
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
	std::istringstream iss(contents);
	std::string line;
	while (std::getline(iss, line))
	{
		int idx = -1;
		if (std::sscanf(line.c_str(), "[%*[^]]] round_trip_msg %d", &idx) == 1)
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
