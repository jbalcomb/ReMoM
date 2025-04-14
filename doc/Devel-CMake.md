
# CMake


SEEALSO:  VSC && CMake





add_library()
add_executable()
add_subdirectory()

include_directories()
target_include_directories()

### ¿ target_include_directories() vs. include_directories() ?



target_compile_options()
target_compile_definitions()
target_compile_features()



## ¿ DEBUG ?

get_target_property()
message()



## FetchContent

include(FetchContent)
¿ Set(FETCHCONTENT_QUIET FALSE) ?
~ Options
    set(BUILD_SHARED_LIBS OFF)
    option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
FetchContent_Declare()
FetchContent_MakeAvailable()

e.g., SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG        release-2.32.0
)
FetchContent_MakeAvailable(SDL2)
¿ set(SDL_INCLUDE_DIR "${SDL2_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/SDL2") ?
