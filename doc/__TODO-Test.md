


https://google.github.io/googletest/quickstart-cmake.html

https://www.labri.fr/perso/fleury/posts/programming/using-cmake-googletests-and-gcovr-in-a-c-project.html


C:/STU/devel/ReMoM/build/src/MOX/tests/Debug/MOX_tests.exe --gtest_list_tests

C:\STU\devel\ReMoM>C:/STU/devel/ReMoM/build/src/MOX/tests/Debug/MOX_tests.exe --gtest_list_tests
Running main() from C:\STU\devel\ReMoM\build\_deps\googletest-src\googletest\src\gtest_main.cc
MOX_test.
  MOX_One_Plus_One
  MOX_String_To_Upper
  MOX_String_To_Lower


Use this program to figure that out: https://github.com/lucasg/Dependencies – 
drescherjm
 CommentedApr 1, 2023 at 21:07
MSYS2 env ldd.exe and ntldd.exe both can be used to trace DLL dependencies. So far, I think the problem should come from CTest ... – 
KCC
 CommentedApr 2, 2023 at 6:34




MainScr_Maps.c
MainScr_Maps.h
## Center_Map()
Main_Screen()
if(abs(input_field_idx) == _minimap_grid_field)
    Reduced_Map_Coords(&reduced_map_window_wx, &reduced_map_window_wy, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);
    _prev_world_x = reduced_map_window_wx + _minimap_grid_x;  // ...is the 'wx' of the clicked square
    _prev_world_y = reduced_map_window_wy + _minimap_grid_y;  // ...is the 'wy' of the clicked square
    _map_x = _prev_world_x;
    _map_y = _prev_world_y;
    Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);
    assert(_prev_world_x >= WORLD_XMIN && _prev_world_x <= WORLD_XMAX);  /*  0 & 59 */
    assert(_prev_world_y >= WORLD_YMIN && _prev_world_y <= WORLD_YMAX);  /*  0 & 39 */
    assert(_map_x >= WORLD_XMIN && _map_x <= WORLD_XMAX);  /*  0 & 59 */
    assert(_map_y >= WORLD_YMIN && _map_y <= WORLD_YMAX);  /*  0 & 39 */



https://cfd.university/learn/automating-cfd-solver-and-library-compilation-using-cmake/how-to-add-external-libraries-into-your-cfd-solver-using-cmake/#aioseo-the-preferred-way-using-a-package-manager

To create a default profile, run the following command

conan profile detect --force

This will give us some sensible default settings. On Windows, for example, this is what I am getting as my default profile:

[settings]
arch=x86_64
build_type=Release
compiler=msvc
compiler.cppstd=20
compiler.runtime=dynamic
compiler.version=193
os=Windows
[settings]
arch=x86_64
build_type=Release
compiler=msvc
compiler.cppstd=20
compiler.runtime=dynamic
compiler.version=193
os=Windows

C:\>conan profile detect --force
detect_api: Found msvc 17

Detected profile:
[settings]
arch=x86_64
build_type=Release
compiler=msvc
compiler.cppstd=14
compiler.runtime=dynamic
compiler.version=194
os=Windows

WARN: This profile is a guess of your environment, please check it.
WARN: The output of this command is not guaranteed to be stable and can change in future Conan versions.
WARN: Use your own profile files for stability.
Saving detected profile to C:\Users\jbalcomb\.conan2\profiles\default


To install all dependencies (gtest, in this case), open a console and head to the root-level directory of this project. Then, execute the following command (if you do not have a dev profile, remove the -pr dev flag from the command):
conan install . -pr=dev --output-folder=build --build=missing



Compiling the tests
At this point, we have gone over the compilation step quite a few times, and hopefully, this will look very familiar to you by now.
To execute the configuration step, execute the command shown below:
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -G Ninja ..
If you are using a different generator, use that instead, or omit the -G altogether, but be sure to use the same strategy in Conan, as the created toolchain file by Conan will assume a specific generator to be used, as we have discussed above at length. Once the project is configured, we can build it with the usual command
cmake --build . --config Debug
You should now see the testComplexNumbers.exe (Windows) or testComplexNumbers (UNIX) file within your build/ directory. If you used MSBuild, Ninja-multiconfig, or Xcode, you will see it in the Debug/ folder. The actual location doesn’t matter, though, as we will not execute the test ourselves. For that, we have to turn to CTest, finally.
Running CTest on the command line
Since we have already defined all tests in the tests/unit/CMakeLists.txt file, CTest already knows about all executables and can run them for us. To do so, simply invoke
ctest
on the command line, and CTest will start running all of our tests. Below is the output of CTest on my system:


cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -G Ninja-multiconfig ..
