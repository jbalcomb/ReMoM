
cmake --build . --target clean

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON .
REM "C:\Program Files\CMake\bin\cmake.EXE" -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -SC:/STU/devel/ReMoM -Bc:/STU/devel/ReMoM/build -G Ninja

cmake --build .
