
cmake --build . --target clean

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON .

cmake --build .
