# rm -rf build/*
# cmake -S . -B build > _cmake_build.txt
# cmake --build build --config Debug --verbose > _cmake_build.txt
# ctest --test-dir build --rerun-failed --output-on-failure

add_library(TootLib STATIC TootLib.c)

# target_include_directories(TootLib PRIVATE "${PROJECT_SOURCE_DIR}")
target_include_directories(TootLib PRIVATE ${PROJECT_SOURCE_DIR}/TootLib)
# target_include_directories(TootLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# target_link_libraries(TootLib )

add_subdirectory(tests)
