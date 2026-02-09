
include(GoogleTest)

set(CMAKE_EXECUTE_PROCESS_COMMAND_ECHO STDOUT) # Set default to echo to STDOUT

add_executable(MOX_tests MOX_tests.cpp)

# # target_include_directories(TootLib_tests PRIVATE "${PROJECT_SOURCE_DIR}")
# # target_include_directories(TootLib_tests PRIVATE ${PROJECT_SOURCE_DIR}/TootLib)
# # target_include_directories(TootLib_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# target_include_directories(MOX_tests PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_SOURCE_DIR}/src/MOX
#     ${GTEST_INCLUDE_DIRS}
# )
target_include_directories(MOX_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
)
get_target_property(include_dirs MOX_tests INCLUDE_DIRECTORIES)
message(STATUS "INCLUDE_DIRECTORIES: ${include_dirs}")

target_link_libraries(MOX_tests PUBLIC
    GTest::gtest
    GTest::gtest_main
    SDL2::Core
    SDL2::Mixer
    momlib
    MOX
)
get_target_property(include_dirs MOX_tests INCLUDE_DIRECTORIES)
message(STATUS "INCLUDE_DIRECTORIES: ${include_dirs}")

# get_target_property(imported_location SDL2::Core IMPORTED_LOCATION)
# message(STATUS "IMPORTED_LOCATION: ${imported_location}")
# get_target_property(imported_location SDL2::Main IMPORTED_LOCATION)
# message(STATUS "IMPORTED_LOCATION: ${imported_location}")
# get_target_property(imported_location SDL2::Mixer IMPORTED_LOCATION)
# message(STATUS "IMPORTED_LOCATION: ${imported_location}")
# get_target_property(runtime_output_directory MOX_tests RUNTIME_OUTPUT_DIRECTORY)
# message(STATUS "RUNTIME_OUTPUT_DIRECTORY: ${runtime_output_directory}")
# [cmake] -- INCLUDE_DIRECTORIES: C:/STU/devel/ReMoM/src/MOX/tests;C:/STU/devel/ReMoM/src/MOX
# [cmake] -- INCLUDE_DIRECTORIES: C:/STU/devel/ReMoM/src/MOX/tests;C:/STU/devel/ReMoM/src/MOX
# [cmake] -- IMPORTED_LOCATION: C:/devellib/SDL2-2.32.2/lib/x64/SDL2.lib
# [cmake] -- IMPORTED_LOCATION: imported_location-NOTFOUND
# [cmake] -- IMPORTED_LOCATION: C:/devellib/SDL2_mixer-2.8.1/lib/x64/SDL2_mixer.lib
# [cmake] -- RUNTIME_OUTPUT_DIRECTORY: runtime_output_directory-NOTFOUND
# [cmake] -- executable_location: EXE_PATH-NOTFOUND
# [cmake] -- EXE_OUTPUT_DIR: 

# get_target_property(EXE_PATH MOX_tests LOCATION)
# [cmake] CMake Error at src/MOX/tests/CMakeLists.txt:38 (get_target_property):
# [cmake]   The LOCATION property may not be read from target "MOX_tests".  Use the
# [cmake]   target name directly with add_custom_command, or use the generator
# [cmake]   expression $<TARGET_FILE>, as appropriate.
# message(STATUS "executable_location: ${EXE_PATH}")
# 
# get_filename_component(EXE_OUTPUT_DIR ${EXE_PATH} DIRECTORY)
# message(STATUS "EXE_OUTPUT_DIR: ${EXE_OUTPUT_DIR}")

message(STATUS "MoX tests: CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "MoX tests: CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "MoX tests: CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
message(STATUS "MoX tests: CMAKE_CONFIGURATION_TYPE: ${CMAKE_CONFIGURATION_TYPE}")
# Nope. message(STATUS "<CONFIG>: $<CONFIG>")
get_target_property(imported_location SDL2::Core IMPORTED_LOCATION)
get_filename_component(imported_location_dir ${imported_location} DIRECTORY)
message(STATUS "MoX tests: imported_location_dir: ${imported_location_dir}")
# file(COPY "${imported_location_dir}/SDL2.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
if(WIN32)
    add_custom_command(
        TARGET MOX_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${imported_location_dir}/SDL2.dll
            $<TARGET_FILE_DIR:MOX_tests>
    )
endif()
get_target_property(imported_location SDL2::Mixer IMPORTED_LOCATION)
get_filename_component(imported_location_dir ${imported_location} DIRECTORY)
message(STATUS "MoX tests: imported_location_dir: ${imported_location_dir}")
# file(COPY "${imported_location_dir}/SDL2_mixer.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
if(WIN32)
    add_custom_command(
        TARGET MOX_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            ${imported_location_dir}/SDL2_mixer.dll
            $<TARGET_FILE_DIR:MOX_tests>
    )
endif()
# if(WIN32)
#     add_custom_command(
#         TARGET MOX_tests POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         $<TARGET_RUNTIME_DLLS:MOX_tests>
#         $<TARGET_FILE_DIR:MOX_tests>
#         COMMAND_EXPAND_LISTS
#     )
# endif()



# test code from ReMoMber
# # if(WIN32)
# #     add_custom_command(
# #         TARGET "ReMoMber" POST_BUILD
# #         COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:ReMoMber>"
# #         "$<TARGET_RUNTIME_DLLS:ReMoMber>" USES_TERMINAL COMMAND_EXPAND_LISTS
# #     )
# # endif()
# # # [build]   TARGET 'SDL2::Main' is IMPORTED and does not build here.
# # if(WIN32)
# #     add_custom_command(
# #         TARGET "SDL2::Main" POST_BUILD
# #         COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:SDL2::Main>"
# #         "$<TARGET_RUNTIME_DLLS:SDL2::Main>" USES_TERMINAL COMMAND_EXPAND_LISTS
# #     )
# # endif()



# add_compile_options("$<$<CONFIG:Release,MinSizeRel>:-fmacro-prefix-map=${CMAKE_SOURCE_DIR}=;-fomit-frame-pointer>" "$<$<CONFIG:Debug>:-Og;-fnoinline>")

# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy 
#         ${imported_location_dir}/SDL2_mixer.dll
#         $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )

# # build-time copy
# file(COPY "${DLL_PATH}" DESTINATION "${EXE_OUTPUT_DIR}")
# # post-build-time copy
# add_custom_command(
#     TARGET MyApp POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLL_PATH}" "${EXE_OUTPUT_DIR}"
#     COMMENT "Copying DLL for SDL2"
# )

# fails if no DLLs are required
# add_custom_command(TARGET MOX_tests POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:MOX_tests> $<TARGET_FILE_DIR:MOX_tests>
# )
# hack to avoid failure if no DLLs are required
add_custom_command(TARGET MOX_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MOX_tests> $<TARGET_RUNTIME_DLLS:MOX_tests> $<TARGET_FILE_DIR:MOX_tests>
)
# add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>.
# )

file(GLOB LBX_FILE_LIST "${CMAKE_SOURCE_DIR}/*.lbx")
add_custom_command(
    TARGET MOX_tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LBX_FILE_LIST}
        $<TARGET_FILE_DIR:MOX_tests>
    COMMAND_EXPAND_LISTS
    COMMENT "MoX tests: Copying LBX files..."
)

gtest_discover_tests(MOX_tests)
# add_test(NAME MOX_tests COMMAND MOX_tests)
# # add_custom_command(TARGET MOX_test POST_BUILD COMMAND ctest --output-on-failure)
# 
# # add_custom_command(TARGET exe POST_BUILD
# #   COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:exe> $<TARGET_RUNTIME_DLLS:exe>
# #   COMMAND_EXPAND_LISTS
# # )
