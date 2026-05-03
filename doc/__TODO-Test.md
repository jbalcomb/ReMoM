

## Record & Replay

RMR script
HMS script
rmr2hms.exe

ReMoM
HeMoM  (headless ReMoM)
--record
--replay
--newgame
--scenario

## HeMoM  (headless ReMoM)
--newgame
--scenario

Testing needs to be deterministic, so the INI file holds a random number seed

cd c:/STU/devel/ReMoM/out/build/MSVC-headless-debug/bin/Debug
./HeMoM.exe --newgame c:/STU/devel/ReMoM/assets/test_worldgen.ini --scenario c:/STU/devel/ReMoM/assets/test_worldgen_smoke.hms

cd c:/STU/devel/ReMoM/out/build/MSVC-headless-debug/bin/Debug
./HeMoM.exe --newgame c:/STU/devel/ReMoM/assets/test_worldgen.ini --scenario c:/STU/devel/ReMoM/assets/test_worldgen_smoke.hms

--record
--replay

cd c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug && ./rmr2hms.exe next_turn.RMR next_turn.hms && cat next_turn.hms

e.g.,
cd c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug
ReMoMber.exe --record my_recording.rmr
...record some sequence of mouse moves, button clicks, keyboard presses
ReMoMber.exe --replay my_recording.rmr
...watch a playback of the recorded sequence

Use `--newgame my_newgame.ini` to automatically create a 'New Game', saved as the 'Continue' save
Use `--continue` to automatically launch with the 'Continue' save

## Canonical Scenarios
test_worldgen_smoke.hms — Smoke test: does world generation complete without crashing?
World generation + SAVE9.GAM creation happen in Config_Apply_And_Create_New_Game()
BEFORE Screen_Control() is entered. By the time this scenario runs, the game is already at the Main Screen with a valid save file written.
This scenario just tells the game to exit cleanly.
Usage:  HeMoM --newgame test_worldgen.ini --scenario test_worldgen_smoke.hms
Validation: - HeMoM exits with code 0 - SAVE9.GAM exists and is exactly 123300 bytes
cd c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug &&
.\HeMoM.exe --newgame test_worldgen.ini --scenario test_worldgen_smoke.hms


cd c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug &&
.\HeMoM.exe --newgame test_gameplay.ini --scenario test_worldgen_smoke.hms

Auto-Generate 'New Game', Launch 'Continue', handle 'Name Home City'

C:\STU\devel\ReMoM\out\build\MSVC-debug\bin\Debug\ReMoMber.exe

cd C:\STU\devel\ReMoM\out\build\MSVC-debug\bin\Debug\
.\HeMoM.exe --newgame C:\STU\devel\ReMoM\assets\test_gameplay.ini
.\ReMoMber.exe --continue



c:/STU/devel/ReMoM/assets/
c:/STU/devel/ReMoM/out/build/MSVC-debug/
c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/
C:\STU\devel\ReMoM\IDE\VS2022-SDL2\ReMoMber

...CMake copies c:/STU/devel/ReMoM/assets/ to c:/STU/devel/ReMoM/out/build/MSVC-debug/
...CMake copies c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/ to c:/STU/devel/ReMoM/out/build/MSVC-debug/

copy c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/MAGIC.SET :/STU/devel/ReMoM/assets/

copy c:/STU/devel/ReMoM/assets/*.* c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/

copy c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/SAVE1.GAM c:/STU/devel/ReMoM/assets/
copy c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/MAGIC.SET :/STU/devel/ReMoM/assets/

cd c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/
.\HeMoM.exe --newgame test_gameplay.ini
.\ReMoMber.exe --continue --record test-gameplay.rmr
.\HeMoM.exe --newgame test_gameplay.ini
.\ReMoMber.exe --continue --replay test-gameplay.rmr
.\rmr2hms test-gameplay.rmr test-gameplay.hms
.\HeMoM.exe --newgame test_gameplay.ini
.\ReMoMber.exe --continue --scenario test-gameplay.hms

cd c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/
.\HeMoM.exe --newgame test_gameplay.ini
.\HeMoM.exe --continue --scenario test-gameplay.hms

cd c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/
.\ReMoMber.exe --continue --scenario test-gameplay.hms --record test-gameplay2.rmr



Three PRDs drafted under doc/PRDs/:

File	Layer	Enforcement	Speed
PRD-VSCode-Task-Safe-To-Push.md	IDE / on-demand	none (manual)	fastest after first run
PRD-Git-Hook-Pre-Push-Release-Check.md	Local git	local, bypassable	medium
PRD-Github-Actions-Release-Check.md	GitHub CI	absolute	slowest, runs in parallel
Each is self-contained — same problem statement framed for its layer, distinct goals/non-goals, explicit acceptance criteria, dependencies cross-linked. They're independent: any one can ship without the others, and the three together form layered defense.

A few things I deliberately left as "TBD" or "open question" rather than decided in the PRDs:

Task name. check: safe-to-push vs. release-build: verify. The first reads better but overstates what the check covers.
Hook scope. Whether to gate on every push or only main-bound. Affects speed → bypass discipline.
Hook OS coverage. v1 is Windows-only by virtue of the MSVC-release preset. Listed as a follow-up rather than baked in.
Required-status-check on main. Stage gate — don't flip it until the workflow has stabilized.
Worth your read-through before any of them get implemented. Anything you'd want changed in the framing before we move to implementation?



TRACE.LOG, test-gameplay.rmr, and test-gameplay-RECORD.log

grep -E "WIN_KEY|REC_CAP|KBD_READ" TRACE.LOG | grep -A5 "vk=0x43"   # for 'C' key



cd c:/STU/devel/ReMoM/out/build/MSVC-debug
cd c:/STU/devel/ReMoM/out/build/MSVC-debug/bin/Debug/
.\HeMoM.exe --newgame test_gameplay.ini
copy .\SAVE9.GAM .\SAVE1.GAM
.\savedump.exe SAVE9.GAM SAVE9_gameplay.txt
.\ReMoMber.exe --continue --record test-gameplay.rmr
.\ReMoMber.exe --continue --replay test-gameplay.rmr
.\rmr2hms test-gameplay.rmr test-gameplay.hms

...the continue save has now been overwritten, so we need to reload another copy...

.\HeMoM.exe --newgame test_gameplay.ini
.\ReMoMber.exe --continue --scenario test-gameplay-byhand.hms

copy .\SAVE1.GAM .\SAVE9.GAM
.\ReMoMber.exe --loadsave SAVE1.GAM --scenario test-gameplay-byhand.hms



.\savedump.exe SAVE1.GAM SAVE1_gameplay.txt

--dump-save SAVE1.GAM


PS C:\STU\devel\ReMoM\out\build\MSVC-debug> ./savedump.exe SAVE9.GAM SAVE9_gameplay.txt
[HeMoM SaveDump] Wrote: SAVE9_gameplay.txt
savedump: wrote SAVE9_gameplay.txt
PS C:\STU\devel\ReMoM\out\build\MSVC-debug> 

.\ReMoMber.exe --continue --record test-gameplay.rmr
.\rmr2hms test-gameplay.rmr test-gameplay.hms

ReMoMber.exe --replay test-gameplay.rmr
rmr2hms test-gameplay.rmr test-gameplay.hms


#   ReMoMber --continue --scenario test_continue_save.hms
# Used with:  ReMoMber --newgame [ReMoM.ini]
#             ReMoMber --headless --newgame ReMoM.ini --replay game.RMR

### test_continue_.hms
used to launch a new-game continue game-save and start from turn 0
currently handles the 'Name Home City' box then sits idle
### test_continue_pre_next_turn.hms
used to launch a new-game continue game-save and start from turn 0
currently handles the 'Name Home City' box, puts the starting units on patrol, then sits idle
### test_continue_save.hms
used to test from --continue through save-and-quit; post-op test uses output from --dump-save SAVE1.GAM
### test_continue_next_turn.hms

### scenario_tail_save_and_quit.hms
assumes 'Main Screen'; saves the current game in save-slot-1
Notes:
  - The save slot name comes from the $SAVE_NAME built-in (expanded to "AHP-YYYYMMDDHHMM" by Artificial_Human_Player at scenario-load time).
  - Coordinates are game-space (320x200).



# ReMoM.ini — New Game Configuration  (example/template)



## Scenario Authoring Work-Flow
    Here's where we landed:
        One-time setup:
            cmake --workflow --preset MSVC-debug
            OR
            cmake --preset MSVC-debug
            cmake --build out/build/MSVC-debug --config Debug
            Builds HeMoM, ReMoMber, savedump, rmr2hms, tests — all into out/build/MSVC-debug/bin/Debug/ alongside SDL DLLs and game assets.
        Per-scenario authoring loop:
            1. Generate deterministic baseline (in bin/Debug/):
                ./HeMoM.exe --newgame /path/to/your.ini
                Writes SAVE9.GAM + SAVE9.txt. Exits cleanly.
            2. Snapshot baseline dump (so you can diff later):
                ./savedump.exe SAVE9.GAM SAVE9_baseline.txt
            3. Play interactively + record:
                ./ReMoMber.exe --continue --record next_turn.RMR
                Loads SAVE9.GAM, drops you into the game. Play your turn, hit popups, save to a slot, quit normally so the .RMR finalizes.
            4. Translate recording to a draft scenario:
                ./rmr2hms.exe next_turn.RMR next_turn.hms
                Hand-clean the output — fix any double-key collapses (e.g. "ll"), pick sensible wait granularities, add comments.
            5. Dump and diff post-session save:
                ./savedump.exe SAVE1.GAM SAVE1_postturn.txt
                diff SAVE9_baseline.txt SAVE1_postturn.txt > next_turn_diff.txt
                Read the diff, pick which fields are the meaningful state changes, write them into assert_<name>.txt.
            6. Verify the scenario replays the same outcome:
                ./HeMoM.exe --newgame /path/to/your.ini
                ./HeMoM.exe --continue --scenario next_turn.hms --dump-save SAVE1.GAM
                ./check_save_fields.exe SAVE1.txt assert_<name>.txt
            7. Wire into CTest — add to tests/CMakeLists.txt following the HeMoM_NextTurn_* pattern (Setup fixture → Run → Assertions).
                tools/record_next_turn.sh already automates 1–5 for the next-turn case. It still has stale references to the old two-build-dir layout — worth updating to match the unified single-dir reality if you want to keep using it as the canonical example.



cd c:/STU/devel/ReMoM/out/build/MSVC-debug
./HeMoM.exe --newgame test_worldgen.ini
./ReMoMber.exe --continue
./savedump.exe SAVE9.GAM
./rmr2hms.exe next_turn.RMR
./check_save_fields.exe SAVE9.txt assert_continue_save.txt
Everything lives in c:/STU/devel/ReMoM/out/build/MSVC-debug/: all 7 .exe files, both SDL DLLs, all .LBX assets, CONFIG.MOM, .ini and .hms scenario files, PLAYING.md.

Confirmed — actually working:

HeMoM --newgame test_worldgen.ini → writes SAVE9.GAM (123300 bytes), exits clean
savedump SAVE9.GAM → writes SAVE9.txt (1.7 MB)
check_save_fields SAVE9.txt <assert> → runs (correctly reports 5 failures vs the post-continue assertions, since SAVE9 is the pre-continue baseline)
All 7 .exe files present in out/build/MSVC-debug/
rmr2hms runs, prints usage
Correction to my earlier example: the assert_*.txt files live in c:/STU/devel/ReMoM/tests/, not in the install dir. So that line should be:

./check_save_fields.exe SAVE9.txt c:/STU/devel/ReMoM/tests/assert_continue_save.txt
I didn't run ReMoMber (it'd open a GUI window mid-conversation), but the .exe is there.






## Translate RMR script to HMS script

Tool is working. 3778 frames → 42 actions, output is valid HMS that matches the recorded session: city-name typing, Patrol click, Next Turn ('n'), research picker click at (54,38), then the save+quit interactions.

One caveat in the output worth flagging: "Claudesvile" has one 'l' instead of two. That's a known limitation of my key-event detection — when you press the same key twice in fast succession, the recording may show key0 unchanged across consecutive frames without key_pressed going to 0 between them, and my dedupe logic collapses them into one event. Consecutive identical keys ("ll", "tt", "00", etc.) need a hand-fix.

A real fix would be to look at delta_ms or the position within the keyboard buffer queue to detect "same key, new press" — but the .RMR format only records the latest pending key, not press/release transitions per-frame. Best path is to add a comment in the output for runs of the same key, or have the user spot-check.

The tool is in tools/rmr2hms.c, builds as rmr2hms target, follows the same CLI pattern as savedump (./rmr2hms input.RMR [output.hms]). Use it as a starting point and hand-clean the output before saving as a canonical scenario.







add gcov
[ctest] No coverage info files for CMake project C:/STU/devel/ReMoM. No coverage data will be analyzed for this project.
set(CMAKE_BUILD_TYPE "Debug")
target_compile_options(your_target PRIVATE -g -O0 -coverage -fprofile-arcs -ftest-coverage)
target_link_options(your_target PRIVATE -coverage -lgcov)



Test-Scenario: Color-Cycling; Combat on a Node



text file of all function names
...with filename/module
...with signature
...as csv,tsv,fixed-width
...
...non standard / edge cases
...
...as a markdown checklist
...
...as something that can be fed to GHCP to work through unit-test test-cases for every single function

test file of chains of function calls
graph semantic triples file of function calls

list of all symbols in a source code file
...yeild/generator

What does VS GHCP mean by "Code search" in it's prompt responses?
...`get_file()`, `run_in_terminal()`, `get_terminal_output()`, `get_output_window_logs()`







TODO
prompt each of the big four to write a github copilot instructions

prompt each of the big four to write a guiderails document for a C project
prompt each of the big four to write a guiderails document for a classic C project
prompt each of the big four to write a guiderails document for a classic C game project
...that uses CMake, CTest, GTest, CPack, SDL2, SDL2 Mixer
...in both Visual studio and Visual Studio Code

prompt each of the big four to write a prompt template for ...





https://cmake.org/cmake/help/latest/manual/ctest.1.html

ctest --list-presets
Available test presets:
  "MSVC-debug" - VS2022 amd64 Debug

ctest --preset MSVC-debug --list-tests

ctest --preset=MSVC-debug -R "STR_ListSeparator_test.SingleItem_NoSeparator"

ctest --preset=MSVC-debug -R "STR_ListSeparator_test"

cmake --workflow=MSVC-debug && ctest --preset=MSVC-debug -R "City_Maximum_Size_NewGame_test"





$ ctest --preset MSVC-debug --list-tests
Test project C:/STU/devel/ReMoM/out/build/MSVC-debug
      Start  1: MOX_test.MOX_One_Plus_One
 1/17 Test  #1: MOX_test.MOX_One_Plus_One ................................   Passed    0.16 sec
      Start  2: MOX_test.MOX_String_To_Upper
 2/17 Test  #2: MOX_test.MOX_String_To_Upper .............................   Passed    0.02 sec
      Start  3: MOX_test.MOX_String_To_Lower
 3/17 Test  #3: MOX_test.MOX_String_To_Lower .............................   Passed    0.03 sec
      Start  4: MOM_test.MOM_Create_Unit
 4/17 Test  #4: MOM_test.MOM_Create_Unit .................................   Passed    0.21 sec
      Start  5: MOM_test.MOM_Kill_Unit_kt0
 5/17 Test  #5: MOM_test.MOM_Kill_Unit_kt0 ...............................   Passed    0.03 sec
      Start  6: MOM_test.MOM_Kill_Unit_kt1
 6/17 Test  #6: MOM_test.MOM_Kill_Unit_kt1 ...............................   Passed    0.02 sec
      Start  7: MOM_test.MOM_Kill_Unit_kt2
 7/17 Test  #7: MOM_test.MOM_Kill_Unit_kt2 ...............................   Passed    0.03 sec
      Start  8: MOM_test.MOM_Delete_Dead_Units
 8/17 Test  #8: MOM_test.MOM_Delete_Dead_Units ...........................   Passed    0.03 sec
      Start  9: STR_ListSeparator_test.SingleItem_NoSeparator
 9/17 Test  #9: STR_ListSeparator_test.SingleItem_NoSeparator ............   Passed    0.03 sec
      Start 10: STR_ListSeparator_test.TwoItems_AndSeparator
10/17 Test #10: STR_ListSeparator_test.TwoItems_AndSeparator .............   Passed    0.03 sec
      Start 11: STR_ListSeparator_test.ThreeItems_CommaAndAnd
11/17 Test #11: STR_ListSeparator_test.ThreeItems_CommaAndAnd ............   Passed    0.02 sec
      Start 12: STR_ListSeparator_test.FourItems_MultipleCommasThenAnd
12/17 Test #12: STR_ListSeparator_test.FourItems_MultipleCommasThenAnd ...   Passed    0.03 sec
      Start 13: STR_ListSeparator_test.ListSizeIncrementsCorrectly
13/17 Test #13: STR_ListSeparator_test.ListSizeIncrementsCorrectly .......   Passed    0.04 sec
      Start 14: STR_ListSeparator_test.FirstItem_DestUnchanged
14/17 Test #14: STR_ListSeparator_test.FirstItem_DestUnchanged ...........   Passed    0.03 sec
      Start 15: STR_ListSeparator_test.MiddleItem_CommaAppended
15/17 Test #15: STR_ListSeparator_test.MiddleItem_CommaAppended ..........   Passed    0.03 sec
      Start 16: STR_ListSeparator_test.LastItem_AndAppended
16/17 Test #16: STR_ListSeparator_test.LastItem_AndAppended ..............   Passed    0.03 sec
      Start 17: HelloTest.BasicAssertions
17/17 Test #17: HelloTest.BasicAssertions ................................   Passed    0.07 sec

100% tests passed, 0 tests failed out of 17

Total Test time (real) =   0.95 sec


STR_ListSeparator_test

ctest -C Debug -R "^YourTestExecutableName$"

-R <regex>, --tests-regex <regex>
Run tests matching regular expression.

This option tells CTest to run only the tests whose names match the given regular expression.

ctest --preset=MSVC-debug -R "MySuite.MyTest1"




...
[workflow] Running ctest for the MSVC-debug test preset of the workflow step.
...
[proc] Executing command: "C:\Program Files\CMake\bin\ctest.exe" -T test --output-on-failure -R ^MOX_test\.MOX_One_Plus_One$


# Configure
cmake -S . -B build

# Build the test target
cmake --build build --target test_STR_ListSeparator

# Run with CTest
cd build
ctest -R STR_ListSeparator --output-on-failure



Note: The test target test_STR_ListSeparator needs to be added to your CMakeLists.txt first, following the pattern in tests\unit\CMakeLists.txt:
```
add_executable(test_STR_ListSeparator test_STR_ListSeparator.cpp
    ${MOMLIB_SOURCES}
    ${SDL2_SOURCES}
    ${MOX_SOURCES}
    ${STU_SOURCES}
)

target_include_directories(test_STR_ListSeparator PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(test_STR_ListSeparator GTest::gtest_main momlib MOX STU)

gtest_discover_tests(test_STR_ListSeparator)
```






Run One GTest, for de-bugging
cd /c/STU/devel/ReMoM/out/build/MSVC-debug
ctest -R MOM_Kill_Unit_1 -VV





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





## ISSUE: ReMoM needs cleaner logging

might be time for a new approach to the logging, since you keep getting confused, and we're mixing up a lot of different purposes

Agreed. Before I touch anything, let me make sure I understand the purposes we're mixing — I'd rather get the taxonomy right than invent another layer on top of the mess.

From what I've seen in the tree, there are at least these distinct things currently sharing stderr/dbg_prn/trc_prn/MOUSE_LOG/printf:

Original-game DBG trace — dbg_prn / trc_prn calls sprinkled in reconstructed code to mirror what the disassembly is doing. Long-lived, mostly faithful-to-dasm debugging.
Platform / input low-level trace — IKI, FIELDADD, SCR, MENU, MOUSE_LOG. Pre-existing instrumentation for input/field/screen transitions.
HeMoM artificial human player trace — [HeMoM Player] action dispatch + wait timer logs. Test-infrastructure, only meaningful in headless scenario runs.
HeMoM test-probe trace — the new [NEXTTURN] BEGIN/END/phase stuff I just added. Ad-hoc "which function is eating time" probes that come and go per investigation.
User-facing errors / asserts — things that should always print regardless of build config.
And they all currently go to some mix of stderr, DEBUG.LOG, TRACE.LOG, with overlapping #ifdef STU_DEBUG gates.

Before I propose a new scheme, a few questions so I don't guess wrong:

Q1. Do you want these split into separate log files (e.g. HEMOM.LOG, NEXTTURN.LOG, INPUT.LOG) or separate tagged channels in one file that can be grep'd?

Q2. For the HeMoM/test-probe category (#3 + #4) specifically — should that stuff never go to stderr (so CTest output stays clean) and never go to DEBUG.LOG/TRACE.LOG (so it doesn't pollute the original-game dasm trace)? I.e., a dedicated HeMoM log that is only headless-test instrumentation?

Q3. The ad-hoc probe category (#4 — the PHASE macro, BEGIN/END wrappers) — do you want a permanent primitive for "time this call and log it" so future probes are one-line drop-ins, or should each probe just use whatever the new logging primitive is?

Q4. Should the new scheme be runtime-gated (env var, .ini flag) so you can turn individual channels on/off per run, or stay compile-time #ifdef-gated like now?

Once I know the answers I can sketch the concrete API (function names, file destinations, gating) before writing any code.



## ISSUE: Artificial_Human_Player needs non-stick 'Next Turn'

FUUUUU!!!!!  Players_Check_Spell_Research()

Good — the picture is clear. Inside Next_Turn_Proc the end-of-turn block fires several interactive surfaces against the human player:

NEXTTURN.c:291 Chancellor_Screen__WIP(0) — gated by magic_set.end_of_turn_summary
NEXTTURN.c:326 City_Built_Building_Message(...) — per completed building
NEXTTURN.c:359,1004,2172 Warn0(...) — modal warnings
NEXTTURN.c:364 City_Screen__WIP() — full city screen re-entry
Later: Merchant_Popup, Hire_Merc_Popup, Hire_Hero_Popup, Hero_LevelUp_Popup
All of them spin their own local input loop, so the artificial human player's only lever is to keep feeding keystrokes blind and hope the right hotkey lands — which is exactly the mess we've been fighting.

Here are a few ways to manage this in headless tests. Two concrete, two for context:

Idea 1 — Suppress at the source with a headless_mode gate.
Wrap each interactive end-of-turn call in if (headless_mode == ST_FALSE) (or a dedicated hemom_suppress_popups flag set by HeMoM at init). The bookkeeping around them (advancing MSG_Building_Complete, setting current_screen, _map_plane restore) still runs; only the modal UI call is skipped. Pros: deterministic, no input-timing dependency, test runs in ~1 second. Cons: we're no longer exercising the popup code paths — the test proves the turn calculation but not that popups would have rendered.

Idea 2 — Turn the flags off in the .ini / save.
magic_set.end_of_turn_summary = ST_FALSE kills Chancellor immediately. Most "spam" popups are gated by magic_set.* or game-state conditions (new building, hired hero, city alert). A test-only magic_set preset + a known-quiet save (no buildings completing turn 1, no heroes offered, no merchants) makes the whole block a no-op by construction. Pros: zero code change, purely data-driven. Cons: fragile — any later save drift can silently resurrect a popup, and you can't test any popup path this way.

Idea 3 — A HeMoM "auto-dismiss" hook inside the popup primitives.
Warn0, City_Built_Building_Message, Chancellor_Screen__WIP, the *_Popup family all eventually call into Fields + input. Add a single hook at the top of each: if (hemom_auto_dismiss) { inject_escape(); return; } — or more surgically, a hook in the input dispatcher that, when hemom_auto_dismiss is set and no scripted action is pending, synthesizes ESC/ENTER. Pros: exercises real popup entry/exit, no scenario needs to know they exist. Cons: more touch points; need to pick a per-popup "benign dismissal" (ESC on Warn0, ENTER on Chancellor, etc.).

Idea 4 — State-aware waits in the scenario (the TODO already filed).
Instead of wait 2s, wait_until screen=Main_Screen or wait_until field=next_turn_button visible. The player stays idle across however many popups fire; once the game is back on the Main Screen the scenario proceeds. Pros: no game-code changes, tests the real interactive path end-to-end. Cons: biggest infra lift, and the scenarios must still script each popup dismissal (or combine with Idea 3 for a "dismiss-all" helper).

My recommendation: Idea 1 for the turn-over smoke test (fast, deterministic, exactly what the current scenario wants — get to turn 1 and dump the save), and Idea 3 + Idea 4 later when you want tests that actually cover popups. Idea 2 alone is too fragile to rely on.



## ISSUE: Artificial_Human_Player needs input field 



## ISSUE: Artificial_Human_Player needs state-aware waits

**Problem:**
The artificial human player's `wait <duration>` actions are wall-clock based.  They idle until `Platform_Get_Millies()` reaches a target.  This is fragile when the wait is meant to bracket a long-running game operation whose duration is not known up front.

The first concrete failure: in `test_continue_next_turn.hms`, the action sequence is `key N` (Next Turn) → `wait 10s` → `key G` → ...  Empirically, `Next_Turn_Calc()` takes ~11 seconds for a 4-opponent game on the first turn.  The 10s wall-clock wait elapses while the AI is still processing, and the post-wait actions (`key G`, `click 80 54`, etc.) fire *during* `Next_Turn_Proc()`.  Those keys/clicks land on whatever transient screen state happens to be active mid-turn-processing, and the scenario loses sync with the game.

The current workaround is to bump the wait to `15s` or `20s` — guessing high enough that the AI is always done.  This is fragile (a slower machine, or a longer-running turn deeper in the game, breaks the test) and slow (every test pays the worst-case wait time on every run).

**Proposed fix: state-aware waits.**
Add new scenario actions that block the artificial human player until a game-state condition becomes true:

```
wait_screen <name>      block until current_screen == scr_<name>
wait_turn <N>           block until game_turn == N
wait_field <name>       block until field <name> exists in p_fields[]
                        (means: until the named button/hotkey is on-screen)
```

Implementation sketch:
- Add new action types `act_WAIT_SCREEN`, `act_WAIT_TURN`, `act_WAIT_FIELD`
- Each holds a target value (screen enum, turn number, or field name)
- The frame callback checks the condition each tick; advances when true
- Add a safety timeout (e.g. 30s wall clock) so a typo can't hang the test forever
- The action's debug log shows the current value vs the target on each poll

Benefits:
- Tests don't pay worst-case wait time — they advance as soon as the AI/turn is done
- Tests don't break when game logic gets faster or slower
- Scenarios become more readable: `wait_turn 1` is more meaningful than `wait 20s`

**Example after the change:**
```
key N                   # Next Turn
wait_turn 1             # block until the turn counter advances
key G                   # safe to open the Game menu now
```

**Related:**
- `current_screen` is `int16_t` in `MoX/src/MOX_T4.h` — already accessible to the player
- `_turn` is `int16_t` in MoM globals — need extern in Artificial_Human_Player.c
- `p_fields[]` and `fields_count` are accessible (HeMoM already iterates them in `HeMoM_Replay_Log_Field_Hit`)
- The screen-name lookup table can mirror the existing `e_SCREENS` enum from `MoM_SCR.h`

**Priority:** Medium.  The wall-clock workaround is good enough to keep building tests, but every test that includes a Next Turn or other long-running operation will need to be revisited when this lands.
