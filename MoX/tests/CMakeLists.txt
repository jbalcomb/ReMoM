
# NOTEO(JimBalcomb,20260217): I moved all my CTest/GTest scraps to C:\STU\devel\ReMoM\doc\#CMake\Devel-CTest-GTest.md

include(GoogleTest)

set(CMAKE_EXECUTE_PROCESS_COMMAND_ECHO STDOUT) # Set default to echo to STDOUT

add_executable(MOX_tests MOX_tests.cpp)
target_include_directories(MOX_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GTEST_INCLUDE_DIRS})
target_link_libraries(MOX_tests PUBLIC GTest::gtest GTest::gtest_main SDL2::Core SDL2::Mixer momlib MOX )

add_executable(test_Util test_Util.cpp)
target_include_directories(test_Util PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GTEST_INCLUDE_DIRS})
target_link_libraries(test_Util PUBLIC GTest::gtest GTest::gtest_main SDL2::Core SDL2::Mixer momlib MOX )

add_executable(test_Allocate test_Allocate.cpp)
target_include_directories(test_Allocate PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GTEST_INCLUDE_DIRS})
target_link_libraries(test_Allocate PUBLIC GTest::gtest GTest::gtest_main SDL2::Core SDL2::Mixer momlib MOX )

if(WIN32)
    add_custom_command(
        TARGET MOX_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_DLL_FILE_PATH}
            $<TARGET_FILE_DIR:MOX_tests>
        COMMAND_EXPAND_LISTS
        COMMENT "./MoX/tests: Copying SDL2.dll"
    )
endif()

if(WIN32)
    add_custom_command(
        TARGET MOX_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${SDL2_MIXER_DLL_FILE_PATH}
            $<TARGET_FILE_DIR:MOX_tests>
        COMMAND_EXPAND_LISTS
        COMMENT "./MoX/tests: Copying SDL2_mixer.dll"
    )
endif()

file(GLOB LBX_FILE_LIST "${CMAKE_SOURCE_DIR}/*.lbx")
    add_custom_command(
        TARGET MOX_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LBX_FILE_LIST}
            $<TARGET_FILE_DIR:MOX_tests>
        COMMAND_EXPAND_LISTS
        COMMENT "./MoX/tests: Copying LBX files..."
)

gtest_discover_tests(MOX_tests)
gtest_discover_tests(test_Util)
gtest_discover_tests(test_Allocate)
