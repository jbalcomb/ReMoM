cmake_minimum_required(VERSION 3.14)

project(platform)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# prior to CMake 3.18, set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
# ...it has to be set prior to the invocation of cmake...
# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 /path/to/src
# ...explicitly specify a generator like this:  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 /path/to/src -G Ninja

include(FetchContent)

# Set(FETCHCONTENT_QUIET FALSE)

set(SDL_DISABLE_NO_INFO ON)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG        release-2.32.0
)
FetchContent_MakeAvailable(SDL2)



# https://stackoverflow.com/questions/71487302/how-to-build-sdl-image-from-fetchcontent
# FetchContent_Declare(
#     SDL2_IMAGE
#     GIT_REPOSITORY  https://github.com/libsdl-org/SDL_image
#     GIT_TAG         release-2.8.5
#     GIT_PROGRESS    TRUE
# )
# 
# FetchContent_MakeAvailable(SDL2_IMAGE)
# set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2main SDL2-static SDL2_image)
# target_include_directories("${PROJECT_NAME}" PRIVATE include)
# target_link_libraries("${PROJECT_NAME}" PRIVATE ${SDL_LIBRARIES})
# cmake --build ./build --config debug --target ALL_BUILD --parallel

# https://github.com/libsdl-org/SDL_ttf



# # https://stackoverflow.com/questions/75551185/cmake-using-sdl2-and-sdl2-image-via-fetchcontent
# # START ADDITION
# set(SDL2IMAGE_INSTALL OFF)
# set(BUILD_SHARED_LIBS FALSE)
# # END ADDITION



# https://github.com/OpenXcom/OpenXcom/blob/master/CMakeLists.txt
# if ( NOT SDL_FOUND )
#     message ( FATAL_ERROR "Can't find SDL which is required" )
# else ()
#     include_directories ( SYSTEM ${SDL_INCLUDE_DIR} )
#     message ( "found SDL ${SDL_MAJOR}.${SDL_MINOR}.${SDL_MICRO} (${SDL_LIBRARY_DIRS}:${SDL_INCLUDE_DIR})" )
# endif ()
message("SDL_MAJOR_VERSION: ${SDL_MAJOR_VERSION}")
message("SDL_MINOR_VERSION: ${SDL_MINOR_VERSION}")
message("SDL_MICRO_VERSION: ${SDL_MICRO_VERSION}")
message("SDL_SOURCE_VERSION: ${SDL_SOURCE_VERSION}")
message("SDL_VERSION: ${SDL_VERSION}")
message("SDL_REVISION: ${SDL_REVISION}")
message("SDL2_SOURCE_DIR: ${SDL2_SOURCE_DIR}")
message("SDL2_BINARY_DIR: ${SDL2_BINARY_DIR}")
message("SDL_LIBRARY_DIRS: ${SDL_LIBRARY_DIRS}")
message("SDL_INCLUDE_DIR: ${SDL_INCLUDE_DIR}")
message("SDL_INCLUDE_DIRS: ${SDL_INCLUDE_DIRS}")
message("CMAKE_INSTALL_INCLUDEDIR: ${CMAKE_INSTALL_INCLUDEDIR}")
message("PACKAGE_CMAKE_INSTALL_FULL_INCLUDEDIR: ${PACKAGE_CMAKE_INSTALL_FULL_INCLUDEDIR}")
message("CMAKE_INSTALL_INCLUDEDIR/SDL2: ${CMAKE_INSTALL_INCLUDEDIR}/SDL2")
set(SDL_INCLUDE_DIR "${SDL2_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/SDL2")
#set(SDL2_INCLUDE_DIRS "@PACKAGE_CMAKE_INSTALL_FULL_INCLUDEDIR@;@PACKAGE_CMAKE_INSTALL_FULL_INCLUDEDIR@/SDL2")
message("SDL_INCLUDE_DIR: ${SDL_INCLUDE_DIR}")

# https://github.com/libsdl-org/SDL/blob/release-2.32.0/CMakeLists.txt
# set(SDL_MAJOR_VERSION 2)
# set(SDL_MINOR_VERSION 32)
# set(SDL_MICRO_VERSION 0)
# set(SDL_VERSION "${SDL_MAJOR_VERSION}.${SDL_MINOR_VERSION}.${SDL_MICRO_VERSION}")
# ...
# if (NOT DEFINED BUILD_SHARED_LIBS)
# ...
# ... ${SDL2_SOURCE_DIR}/src/audio/dummy/*.c) ... list(APPEND SOURCE_FILES ${DUMMYAUDIO_SOURCES}) ...
# ...
# elseif(WINDOWS)
#   find_program(WINDRES windres)
# 
#   check_c_source_compiles("
#     #include <windows.h>
#     int main(int argc, char **argv) { return 0; }" HAVE_WIN32_CC)
# 
#   file(GLOB CORE_SOURCES ${SDL2_SOURCE_DIR}/src/core/windows/*.c)
#   list(APPEND SOURCE_FILES ${CORE_SOURCES})
# ...
# ${SDL2_BINARY_DIR}
# ...
# # Copy all non-generated headers to "${SDL2_BINARY_DIR}/include/SDL2"
# file(GLOB SDL2_INCLUDE_FILES ${SDL2_SOURCE_DIR}/include/*.h)
# ...
# if(CMAKE_STATIC_LIBRARY_PREFIX STREQUAL "" AND CMAKE_STATIC_LIBRARY_SUFFIX STREQUAL ".lib")
#   # Avoid conflict between the dll import library and the static library
#   set(sdl_static_libname "SDL2-static")
# else()
#   set(sdl_static_libname "SDL2")
# endif()
# ...
# if(NOT WINDOWS_STORE AND NOT SDL2_DISABLE_SDL2MAIN)
#   target_include_directories(SDL2main BEFORE
#     PRIVATE "${SDL2_BINARY_DIR}/include"
#     PRIVATE "${SDL2_BINARY_DIR}/include/SDL2"
#     PRIVATE "${SDL2_BINARY_DIR}/include-config-$<LOWER_CASE:$<CONFIG>>/SDL2"
#   )
#   target_include_directories(SDL2main PUBLIC "$<BUILD_INTERFACE:${SDL2_BINARY_DIR}/include>" $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/SDL2>)
# ...
# install(FILES ... DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDL2)
# ...



# Which one *should* we do/use?  include_directories(SYSTEM ${SDL_INCLUDE_DIR})



add_executable(${PROJECT_NAME}
    main.cpp
)



target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} SDL2 SDL2main)

# set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2main SDL2-static)
# target_include_directories("${PROJECT_NAME}" PRIVATE include)
# target_link_libraries("${PROJECT_NAME}" PRIVATE ${SDL_LIBRARIES})

# target_include_directories(platform PUBLIC
#     ${PROJECT_SOURCE_DIR}/src
#     ${PROJECT_SOURCE_DIR}/src/MOX
#     ${PROJECT_SOURCE_DIR}/src/STU
# )

# target_link_libraries(platform gamelib)


# [proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -SC:/STU/devel/ReMoM -Bc:/STU/devel/ReMoM/build -G "Visual Studio 17 2022" -T host=x64 -A x64
# [cmake] -- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.19045.
# [cmake] -- The C compiler identification is MSVC 19.39.33522.0
# [cmake] -- The CXX compiler identification is MSVC 19.39.33522.0
# [cmake] -- Detecting C compiler ABI info
# [cmake] -- Detecting C compiler ABI info - done
# [cmake] -- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.39.33519/bin/Hostx64/x64/cl.exe - skipped
# [cmake] -- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.39.33519/bin/Hostx64/x64/cl.exe - skipped


# maybe add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
# or, `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON`

# add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
# ¿ compile_commands.json ?
# Squiggles went away after following instructions above AND ensuring that my settings included "C_Cpp.intelliSenseEngine": "Tag Parser" – 
# You may need to restart VSCode after modifying c_cpp_properties.json (inserting compileCommands).

get_target_property(include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
message(STATUS "Include directories for ${PROJECT_NAME}: ${include_dirs}")
