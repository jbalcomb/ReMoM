cmake_minimum_required(VERSION 3.22)

project(
    game
    VERSION "0.0.1"
    DESCRIPTION "Reassembly of Master of Magic v1.31 (Simtex, 1995)"
    LANGUAGES C CXX
)

add_subdirectory(src)





# cmake -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..

# DOME
# cmake --build . --target clean
# cmake .
# cmake --build .
