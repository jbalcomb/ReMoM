
# SEEALSO MoX/test/CMakeLists.xtx ...notes, experimentation, etc.

include(GoogleTest)

add_executable(MOM_tests MOM_tests.cpp)

target_include_directories(MOM_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(MOM_tests PUBLIC
    GTest::gtest
    GTest::gtest_main
    SDL2::Core
    SDL2::Mixer
    momlib
    MOX
)

get_target_property(imported_location SDL2::Core IMPORTED_LOCATION)
get_filename_component(imported_location_dir ${imported_location} DIRECTORY)
message(STATUS "imported_location_dir: ${imported_location_dir}")
# file(COPY "${imported_location_dir}/SDL2.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

set(SDL2_dll ${imported_location_dir})
message(STATUS "SDL2_dll: ${SDL2_dll}")
string(APPEND SDL2_dll "/SDL2.dll")
message(STATUS "SDL2_dll: ${SDL2_dll}")

if(EXISTS "${SDL2_dll}")
    message("The file exists: ${SDL2_dll}")
    # Perform actions if the file exists
else()
    message("The file does not exist: ${SDL2_dll}")
    # Perform actions if the file does not exist
endif()

if(WIN32)
    add_custom_command(
        TARGET MOM_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${SDL2_dll}
            $<TARGET_FILE_DIR:MOM_tests>
        COMMAND_EXPAND_LISTS
        COMMENT "WTF ... copy SDL2.dll"
    )
endif()

get_target_property(imported_location SDL2::Mixer IMPORTED_LOCATION)
get_filename_component(imported_location_dir ${imported_location} DIRECTORY)
message(STATUS "imported_location_dir: ${imported_location_dir}")
# file(COPY "${imported_location_dir}/SDL2_mixer.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

set(SDL2_mixer_dll ${imported_location_dir})
message(STATUS "SDL2_mixer_dll: ${SDL2_mixer_dll}")
string(APPEND SDL2_mixer_dll "/SDL2_mixer.dll")
message(STATUS "SDL2_mixer_dll: ${SDL2_mixer_dll}")

if(EXISTS "${SDL2_mixer_dll}")
    message("The file exists: ${SDL2_mixer_dll}")
    # Perform actions if the file exists
else()
    message("The file does not exist: ${SDL2_mixer_dll}")
    # Perform actions if the file does not exist
endif()

if(WIN32)
    add_custom_command(
        TARGET MOM_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${SDL2_mixer_dll}
            $<TARGET_FILE_DIR:MOM_tests>
        COMMAND_EXPAND_LISTS
        COMMENT "WTF ... copy SDL2_mixer.dll"
    )
endif()

add_custom_command(TARGET MOM_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MOM_tests> $<TARGET_RUNTIME_DLLS:MOM_tests> $<TARGET_FILE_DIR:MOM_tests>
)

gtest_discover_tests(MOM_tests)
