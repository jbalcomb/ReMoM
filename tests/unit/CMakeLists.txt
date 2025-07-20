
include(GoogleTest)

add_executable(testLearning testLearning.cpp)
# add_executable(testLearning testLearning.cpp ${CMAKE_SOURCE_DIR}/src/MainScr_Maps.c)

# target_include_directories(testLearning PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(testLearning PRIVATE ${CMAKE_SOURCE_DIR}/src)

# target_link_libraries(testLearning GTest::gtest_main)
target_link_libraries(testLearning GTest::gtest_main momlib MOX STU)

gtest_discover_tests(testLearning)
