
# NOTEO(JimBalcomb,20260217): I moved all my CTest/GTest scraps to C:\STU\devel\ReMoM\doc\#CMake\Devel-CTest-GTest.md

include(GoogleTest)

set(CMAKE_EXECUTE_PROCESS_COMMAND_ECHO STDOUT) # Set default to echo to STDOUT

add_executable(MOX_tests MOX_tests.cpp)
target_include_directories(MOX_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GTEST_INCLUDE_DIRS})
if(NOT DISABLE_AUDIO)
#     target_link_libraries(MOX_tests PUBLIC GTest::gtest GTest::gtest_main GTest::gmock SDL2::Core SDL2::Mixer momlib MOX )
else()
    target_link_libraries(MOX_tests PUBLIC GTest::gtest GTest::gtest_main GTest::gmock SDL2::Core momlib MOX )
endif()

add_executable(test_Util test_Util.cpp)
target_include_directories(test_Util PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GTEST_INCLUDE_DIRS})
# target_link_libraries(test_Util PUBLIC GTest::gtest GTest::gtest_main GTest::gmock SDL2::Core SDL2::Mixer momlib MOX )
if(NOT DISABLE_AUDIO)
#     target_link_libraries(test_Util PUBLIC GTest::gtest GTest::gtest_main SDL2::Core SDL2::Mixer momlib MOX )
else()
    target_link_libraries(test_Util PUBLIC GTest::gtest GTest::gtest_main SDL2::Core momlib MOX )
endif()

add_executable(test_Allocate test_Allocate.cpp)
target_include_directories(test_Allocate PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GTEST_INCLUDE_DIRS})
if(NOT DISABLE_AUDIO)
#     target_link_libraries(test_Allocate PUBLIC GTest::gtest GTest::gtest_main GTest::gmock SDL2::Core SDL2::Mixer momlib MOX )
else()
    target_link_libraries(test_Allocate PUBLIC GTest::gtest GTest::gtest_main GTest::gmock SDL2::Core momlib MOX )
endif()

ReMoM_Add_Target_Support_Files(MOX_tests
    COPY_SDL2
    COPY_SDL2_MIXER
    COPY_LBX
    COPY_SDL2D
    COPY_GMOCK
)

ReMoM_Add_Target_Support_Files(test_Util
    COPY_SDL2
    COPY_SDL2D
)

ReMoM_Add_Target_Support_Files(test_Allocate
    COPY_SDL2
    COPY_SDL2D
)

# add_custom_command(
#     TARGET MOX_tests
#     POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         $<TARGET_RUNTIME_DLLS:MOX_tests>
#         $<TARGET_FILE_DIR:MOX_tests>
#     COMMENT "./MoX/tests: Copying 'Target Runtime DLLs'..."
# )
# # error ... "C:\Program Files\CMake\bin\cmake.exe" -E copy_if_different  C:/STU/devel/ReMoM/out/build/MSVC-debug/MoX/tests/Debug

gtest_discover_tests(MOX_tests
    PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)
gtest_discover_tests(test_Util
    PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)
gtest_discover_tests(test_Allocate
    PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)
