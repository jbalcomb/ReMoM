
# add_subdirectory(unit)

include(GoogleTest)

add_executable(hello_test hello_test.cpp)

target_link_libraries(hello_test GTest::gtest_main)

gtest_discover_tests(hello_test)

# Test mocks library (used by isolated unit tests that don't link whole MoM/MOX)
add_library(test_mocks STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/mocks/MockSDL.c
	${CMAKE_CURRENT_SOURCE_DIR}/mocks/MockLBX.c
)

target_include_directories(test_mocks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

add_executable(mock_example_test ${CMAKE_CURRENT_SOURCE_DIR}/unit/mock_example_test.cpp)
target_link_libraries(mock_example_test PRIVATE GTest::gtest_main test_mocks)
target_include_directories(mock_example_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
gtest_discover_tests(mock_example_test)

# Representative unit tests
add_executable(test_STU_util ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_STU_util.cpp)
target_link_libraries(test_STU_util PRIVATE GTest::gtest_main STU)
target_include_directories(test_STU_util PRIVATE ${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
gtest_discover_tests(test_STU_util)

add_executable(test_LBX_mock ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_LBX_mock.cpp)
target_link_libraries(test_LBX_mock PRIVATE GTest::gtest_main test_mocks)
target_include_directories(test_LBX_mock PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/mocks ${PROJECT_SOURCE_DIR}/src/MOX)
gtest_discover_tests(test_LBX_mock)

add_executable(test_ArmyList_symbol ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_ArmyList_symbol.cpp)
target_link_libraries(test_ArmyList_symbol PRIVATE GTest::gtest_main momlib)
target_include_directories(test_ArmyList_symbol PRIVATE ${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
gtest_discover_tests(test_ArmyList_symbol)

add_executable(test_CFG_Load ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_CFG_Load.cpp)
target_link_libraries(test_CFG_Load PRIVATE GTest::gtest_main MOX)
target_include_directories(test_CFG_Load PRIVATE ${PROJECT_SOURCE_DIR}/src/MOX ${PROJECT_SOURCE_DIR}/src)
gtest_discover_tests(test_CFG_Load)
