
# SDL2-2.32.2
# SDL2_mixer-2.8.1

# sudo apt-get install libsdl2-dev libsdl2-2.0-0

# mkdir build
# cd build
# cmake ..
# make
# sudo make install

cmake --build . --target clean

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON .

cmake --build .
