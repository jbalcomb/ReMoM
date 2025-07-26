


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


Code Coverage
https://www.labri.fr/perso/fleury/posts/programming/using-cmake-googletests-and-gcovr-in-a-c-project.html

install SDL2 for CPack
https://martin-fieber.de/blog/cmake-cpack-cross-platform-distributables/#setup-cpack
# src/platform/windows/CMakeLists.txt
# Copy .dll files on Windows to the target MyApp build folder.
# For development:
add_custom_command(TARGET MyApp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:SDL2::SDL2>
  $<TARGET_FILE_DIR:MyApp>)
# For distribution:
install(FILES $<TARGET_FILE:SDL2::SDL2>
  DESTINATION ${CMAKE_INSTALL_BINDIR})

Error copying file "C:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/gtest_main.dll;C:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/gtest.dll
CMake Error copying file gtest.dll


https://stackoverflow.com/questions/69978314/cmake-with-gtest-on-windows-build-starts-test-but-shared-libs-cannot-be-found
Now, the real meat that makes testing work for the various shared libraries is this:
if (WIN32)
    add_custom_command(
        TARGET unittest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:unittest> $<TARGET_FILE_DIR:unittest>
        COMMAND_EXPAND_LISTS
    )
endif ()
This uses a new feature in CMake 3.21: the $<TARGET_RUNTIME_DLLS:> generator expression. On DLL platforms, it expands to the transitive list of DLLs on which the given target depends. In this case, it will be Google Test's two libraries (gtest.dll, gtest_main.dll), TinyXml2's library (tinyxml2.dll), and the internal myLib.dll.
This is passed to cmake -E copy_if_different to place these DLLs next to the unittest binary. The destination directory is given by $<TARGET_FILE_DIR:unittest>.
The custom command is guarded by if (WIN32) because $<TARGET_RUNTIME_DLLS:unittest> will be empty otherwise. In that case, the cmake -E copy_if_different command won't get enough arguments and will fail at build time.
Finally COMMAND_EXPAND_LISTS makes sure that the semicolon-delimited list returned by $<TARGET_RUNTIME_DLLS:unittest> is split into multiple arguments, rather than being passed as a single argument with escaped semicolons.
https://github.com/alexreinking/so69978314/blob/main/CMakeLists.txt


https://alexreinking.com/blog/building-a-dual-shared-and-static-library-with-cmake.html
https://github.com/alexreinking/SharedStaticStarter/



https://discourse.cmake.org/t/librarys-cmake-generates-dll-applications-cmake-wants-lib/408

# install(
#     TARGETS MyLib
#     LIBRARY DESTINATION ${destination}/lib
#     RUNTIME DESTINATION ${destination}/lib
#     COMPONENT Libraries)
# install(
#     FILES mylib.h
#     DESTINATION ${destination}/include
#     COMPONENT Headers)
Probably an even better solution would be for MyLib to produce the appropriate config files. In any case: in typical scenario on windows you need *.lib for linking and *.dll for running.
.lib files are installed as ARCHIVE targets, not as LIBRARY targets. So adding an ARCHIVE DESTINATION ${destination}/lib argument to your install() command should help.
https://crascit.com/2019/10/16/cppcon-2019-deep-cmake-for-library-authors/

¿ DLL ?
https://batuhankoc.medium.com/use-cmake-to-create-and-use-dynamic-libraries-5f6498417b3c



Main_Screen_Draw_Summary_Window()
    int16_t mana = 0;
    int16_t food = 0;
    int16_t gold = 0;
    Player_Resource_Income_Total(_human_player_idx, &gold, &food, &mana);
        Player_Magic_Power_Income_Total(&mana_income, &food_income, &gold_income, player_idx);

        mana_expense = Player_Armies_And_Enchantments_Mana_Upkeep(player_idx);
        gold_expense = Player_Armies_Gold_Upkeep(player_idx);
        food_expense = Player_Armies_Food_Upkeep(player_idx);



...getting insane value...
Auto_Move_Ship()
    battle_units[battle_unit_idx].target_cgx = (Last_Target_X + Facing_X_Offset);
    battle_units[battle_unit_idx].target_cgy = (Last_Target_Y + Facing_Y_Offset);
    ...negative...
    CMB_Path_Xs,Ys looks sane
    Origin_X,Y looks sane
    Last_Target_X,Y seems bad
    ...come from passed in Dest_X,Y
    ...if target, updated from battle_units[target_battle_unit_idx].cgx,cgy
origin 14,10



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
