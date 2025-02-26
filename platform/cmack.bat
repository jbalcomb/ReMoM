@ECHO OFF

cmake --build . --target clean

REM cmake .
REM cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON -G Ninja "C:\\path\\to\\source" 
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON .

cmake --build .
