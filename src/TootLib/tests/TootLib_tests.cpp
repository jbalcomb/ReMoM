#include "TootLib.h"

#include <gtest/gtest.h>

#include "gtest/gtest-spi.h" // For CaptureStdout and GetCapturedStdout

TEST(TootLib_test, FooPrintsCorrectMessage)
{

    // Start capturing stdout
    testing::internal::CaptureStdout();

    // Call a function that prints to stdout
    // std::cout << "Hello, World!"; 
    Hello_World();

    // Get captured output
    std::string captured_output = testing::internal::GetCapturedStdout();

    // Assert that the captured output matches the expected string
    ASSERT_EQ("Hello, World\n", captured_output);

}
