


¿ CMD vs. VSC ?

rm -rf build/*
cmake -S . -B build
cmake --build build     (defaults to Debug)
cmake --build build --config Debug
cmake --build build --config Release

rm -rf build/* && cmake -S . -B build
rm -rf build/* && cmake -S . -B build && cmake --build build --config Debug && cmake --build build --config Release
rm -rf build/* && cmake -S . -B build && cmake --build build --config Debug && cmake --build build --config Release && cd build && ctest
