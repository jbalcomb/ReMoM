
SEEALSO:  Devel-GTest-ByHand.md

Not working...
...C & C++?
...linking?
...link C++ and C?
...linking dynamic and static?
...position independent code?
...Debug vs. Release?
...MSVC vs. CLANG vs. GCC?



set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# if(DO_TEST)
# GoogleTest requires at least C++17
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ¿ TODO  set(CXX_EXTENSIONS OFF) ?
enable_testing()
# endif()

add_subdirectory(tests)
