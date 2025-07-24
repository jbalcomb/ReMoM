
include(GoogleTest)

add_executable(TootLib_tests TootLib_tests.cpp)

# target_include_directories(TootLib_tests PRIVATE "${PROJECT_SOURCE_DIR}")
# target_include_directories(TootLib_tests PRIVATE ${PROJECT_SOURCE_DIR}/TootLib)
# target_include_directories(TootLib_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TootLib_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/TootLib)

target_link_libraries(TootLib_tests GTest::gtest_main TootLib)

gtest_discover_tests(TootLib_tests)
