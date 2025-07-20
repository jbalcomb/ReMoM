


¿ CMD vs. VSC ?

rm -rf build/*
cmake -S . -B build
cmake --build build     (defaults to Debug)
cmake --build build --config Debug
cmake --build build --config Release

rm -rf build/* && cmake -S . -B build
rm -rf build/* && cmake -S . -B build && cmake --build build --config Debug && cmake --build build --config Release
rm -rf build/* && cmake -S . -B build && cmake --build build --config Debug && cmake --build build --config Release && cd build && ctest

CMake Error in CMakeLists.txt: No known features for CXX compiler
...because the project is C only, but GTest is C++ (need CXX)

  Building Custom Rule C:/STU/devel/ReMoM/tests/CMakeLists.txt
  hello_test.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\yvals_core.h(23,1): error C1189: #error:  error STL1003: Unexpected compiler, expected C++ compiler. [C:\STU\devel\ReMoM\build\tests\hello_test.vcxproj]
  (compiling source file '../../tests/hello_test.c')
  Building Custom Rule C:/STU/devel/ReMoM/tests/unit/CMakeLists.txt
  testLearning.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\yvals_core.h(23,1): error C1189: #error:  error STL1003: Unexpected compiler, expected C++ compiler. [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
  (compiling source file '../../../tests/unit/testLearning.c')


"C:\Program Files\CMake\bin\cmake.exe" -D TEST_TARGET=testLearning -D TEST_EXECUTABLE=C:/STU/devel/ReMoM/build/tests/unit/Debug/testLearning.exe -D TEST_EXECUTOR= -D TEST_WORKING_DIR=C:/STU/devel/ReMoM/build/tests/unit -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=testLearning_TESTS -D CTEST_FILE=C:/STU/devel/ReMoM/build/tests/unit/testLearning[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P "C:/Program Files/CMake/share/cmake-3.27/Modules/GoogleTestAddTests.cmake"



Project "C:\STU\devel\ReMoM\build\ALL_BUILD.vcxproj" (1) is building "C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj" (12) on node 1 (default targets).

PrepareForBuild:
  Creating directory "testLearning.dir\Release\".
  Structured output is enabled. The formatting of compiler diagnostics will reflect the error hierarchy. See https://aka.ms/cpp/structured-output for more details.
  Creating directory "C:\STU\devel\ReMoM\build\tests\unit\Release\".
  Creating directory "testLearning.dir\Release\testLearning.tlog\".

InitializeBuildStatus:
  Creating "testLearning.dir\Release\testLearning.tlog\unsuccessfulbuild" because "AlwaysCreate" was specified.
  Touching "testLearning.dir\Release\testLearning.tlog\unsuccessfulbuild".

CustomBuild:
  Building Custom Rule C:/STU/devel/ReMoM/tests/unit/CMakeLists.txt

VcpkgTripletSelection:
  Using triplet "x64-windows" from "C:\Python311\Lib\site-packages\vcpkgpip\vcpkg\installed\x64-windows\"
  Using normalized configuration "Release"

ClCompile:
  C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\HostX64\x64\CL.exe /c /IC:\STU\devel\ReMoM\src /IC:\STU\devel\ReMoM /IC:\STU\devel\ReMoM\src\MOX /IC:\STU\devel\ReMoM\src\STU /I"C:\Python311\Lib\site-packages\vcpkgpip\vcpkg\installed\x64-windows\include" /nologo /W1 /WX- /diagnostics:column /O2 /Ob2 /D _MBCS /D WIN32 /D _WINDOWS /D NDEBUG /D "CMAKE_INTDIR=\"Release\"" /Gm- /EHsc /MD /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /std:c++17 /Fo"testLearning.dir\Release\\" /Fd"testLearning.dir\Release\vc143.pdb" /external:W0 /Gd /TP /errorReport:queue  /external:I "C:/STU/devel/ReMoM/build/_deps/googletest-src/googletest/include" /external:I "C:/STU/devel/ReMoM/build/_deps/googletest-src/googletest" /external:I "C:/devellib/SDL2-2.32.2/include" /external:I "C:/devellib/SDL2_mixer-2.8.1/include" C:\STU\devel\ReMoM\tests\unit\testLearning.cpp
  testLearning.cpp

Link:
  C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\HostX64\x64\link.exe /ERRORREPORT:QUEUE /OUT:"C:\STU\devel\ReMoM\build\tests\unit\Release\testLearning.exe" /INCREMENTAL:NO /NOLOGO /LIBPATH:"C:\Python311\Lib\site-packages\vcpkgpip\vcpkg\installed\x64-windows\lib" /LIBPATH:"C:\Python311\Lib\site-packages\vcpkgpip\vcpkg\installed\x64-windows\lib\manual-link" ..\..\lib\Release\gtest_main.lib ..\..\src\Release\momlib.lib ..\..\src\MOX\Release\MOX.lib ..\..\src\STU\Release\STU.lib ..\..\lib\Release\gtest.lib "C:\devellib\SDL2_mixer-2.8.1\lib\x64\SDL2_mixer.lib" "C:\devellib\SDL2-2.32.2\lib\x64\SDL2main.lib" "C:\devellib\SDL2-2.32.2\lib\x64\SDL2.lib" kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib "C:\Python311\Lib\site-packages\vcpkgpip\vcpkg\installed\x64-windows\lib\*.lib" /MANIFEST /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /manifest:embed /PDB:"C:/STU/devel/ReMoM/build/tests/unit/Release/testLearning.pdb" /SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"C:/STU/devel/ReMoM/build/tests/unit/Release/testLearning.lib" /MACHINE:X64  /machine:x64 testLearning.dir\Release\testLearning.obj
MOX.lib(Fields.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(LVLMAKE.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells137.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ArmyList.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(GENDRAW.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ItemScrn.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Roads.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MagicScr.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(LoadScr.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells133.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(SPLMASTR.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(HIRE.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ItemMake.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ItemView.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ProdScr.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(SBookScr.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(DIPLOMAC.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Surveyor.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(WIZVIEW.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Settings.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MainMenu.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Combat.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Lair.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(REPORT.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spellbook.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(UnitList.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(UnitStat.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(CityScr.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(City_ovr55.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MainScr.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(SCastScr.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Outpost.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(AdvsrScr.obj) : error LNK2001: unresolved external symbol Get_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Input.obj) : error LNK2019: unresolved external symbol Release_Time referenced in function Get_Input_Delay [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Fields.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells137.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ArmyList.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(MOX_T4.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Util.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MagicScr.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(LoadScr.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(LVLMAKE.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells130.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(SPLMASTR.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(HIRE.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ItemScrn.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Roads.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(SBookScr.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(DIPLOMAC.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells131.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells133.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Settings.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MainMenu.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ItemView.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ProdScr.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Combat.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spellbook.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Surveyor.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(WIZVIEW.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(UnitList.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(UnitStat.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(CityScr.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(City_ovr55.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MainScr.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(SCastScr.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Outpost.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(AdvsrScr.obj) : error LNK2001: unresolved external symbol Release_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Input.obj) : error LNK2019: unresolved external symbol Mark_Time referenced in function Init_Mouse_Keyboard [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Fields.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells137.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ArmyList.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(MOX_T4.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Util.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MagicScr.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(LoadScr.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(LVLMAKE.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells130.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(SPLMASTR.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(HIRE.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ItemScrn.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Roads.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(SBookScr.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(DIPLOMAC.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells131.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spells133.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Settings.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MainMenu.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ItemView.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(ProdScr.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Combat.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Spellbook.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Surveyor.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(WIZVIEW.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(UnitList.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(UnitStat.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(CityScr.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(City_ovr55.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MainScr.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(SCastScr.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(Outpost.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(AdvsrScr.obj) : error LNK2001: unresolved external symbol Mark_Time [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MOM_PFL.obj) : error LNK2019: unresolved external symbol sdl2_renderer referenced in function Platform_Video_Update [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MOM_PFL.obj) : error LNK2019: unresolved external symbol sdl2_surface_RGB666 referenced in function Platform_Video_Update [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MOM_PFL.obj) : error LNK2019: unresolved external symbol sdl2_surface_ARGB8888 referenced in function Platform_Video_Update [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MOM_PFL.obj) : error LNK2019: unresolved external symbol sdl2_texture referenced in function Platform_Video_Update [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
momlib.lib(MOM_PFL.obj) : error LNK2019: unresolved external symbol sdl2_blit_rect referenced in function Platform_Video_Update [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Input.obj) : error LNK2019: unresolved external symbol Mouse_Button referenced in function Input_Box_Popup [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Input.obj) : error LNK2019: unresolved external symbol Mouse_Movement_Handler referenced in function Interpret_Mouse_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Input.obj) : error LNK2019: unresolved external symbol Mouse_Button_Handler referenced in function Interpret_Mouse_Input [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Input.obj) : error LNK2019: unresolved external symbol Keyboard_Status referenced in function Input_Box_Popup [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Input.obj) : error LNK2019: unresolved external symbol Read_Key referenced in function Input_Box_Popup [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Keyboard.obj) : error LNK2001: unresolved external symbol Read_Key [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(Mouse.obj) : error LNK2019: unresolved external symbol Set_Mouse_Position referenced in function Set_Pointer_Position [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
MOX.lib(random.obj) : error LNK2019: unresolved external symbol Read_System_Clock_Timer referenced in function Randomize [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
C:\STU\devel\ReMoM\build\tests\unit\Release\testLearning.exe : fatal error LNK1120: 15 unresolved externals [C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj]
Done Building Project "C:\STU\devel\ReMoM\build\tests\unit\testLearning.vcxproj" (default targets) -- FAILED.
Done Building Project "C:\STU\devel\ReMoM\build\ALL_BUILD.vcxproj" (default targets) -- FAILED.

Build FAILED.






jbalcomb@Velociraptor MINGW64 /c/STU/devel/ReMoM (current)
$ cmake -S . -B build
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.19045.
-- The C compiler identification is MSVC 19.44.35213.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- The CXX compiler identification is MSVC 19.44.35213.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found Python3: C:/Python311/python.exe (found version "3.11.0") found components: Interpreter
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
-- Found Threads: TRUE
-- Found SDL2: C:/devellib/SDL2-2.32.2/lib/x64/SDL2.lib (found version "2.32.2")
-- Found SDL2main: C:/devellib/SDL2-2.32.2/lib/x64/SDL2main.lib (found version "2.32.2")
-- Found SDL2_mixer: C:/devellib/SDL2_mixer-2.8.1/lib/x64/SDL2_mixer.lib (found version "2.8.1")
-- Configuring done (21.0s)
-- Generating done (0.3s)
-- Build files have been written to: C:/STU/devel/ReMoM/build



jbalcomb@Velociraptor MINGW64 /c/STU/devel/ReMoM (current)
$ cmake --build build
MSBuild version 17.14.14+a129329f1 for .NET Framework

  1>Checking Build System
  Building Custom Rule C:/STU/devel/ReMoM/src/MOX/CMakeLists.txt
...
  Generating Code...
  MOX.vcxproj -> C:\STU\devel\ReMoM\build\src\MOX\Debug\MOX.lib
  Building Custom Rule C:/STU/devel/ReMoM/src/STU/CMakeLists.txt
...
  Generating Code...
  STU.vcxproj -> C:\STU\devel\ReMoM\build\src\STU\Debug\STU.lib
  Building Custom Rule C:/STU/devel/ReMoM/build/_deps/googletest-src/googlemock/CMakeLists.txt
  gtest-all.cc
  gmock-all.cc
  Generating Code...
  gmock.vcxproj -> C:\STU\devel\ReMoM\build\lib\Debug\gmock.lib
  Building Custom Rule C:/STU/devel/ReMoM/build/_deps/googletest-src/googlemock/CMakeLists.txt
  gtest-all.cc
  gmock-all.cc
  gmock_main.cc
  Generating Code...
  gmock_main.vcxproj -> C:\STU\devel\ReMoM\build\lib\Debug\gmock_main.lib
  Building Custom Rule C:/STU/devel/ReMoM/build/_deps/googletest-src/googletest/CMakeLists.txt
  gtest-all.cc
  gtest.vcxproj -> C:\STU\devel\ReMoM\build\lib\Debug\gtest.lib
  Building Custom Rule C:/STU/devel/ReMoM/build/_deps/googletest-src/googletest/CMakeLists.txt
  gtest_main.cc
  gtest_main.vcxproj -> C:\STU\devel\ReMoM\build\lib\Debug\gtest_main.lib
  Building Custom Rule C:/STU/devel/ReMoM/src/CMakeLists.txt
...
  Generating Code...
  momlib.vcxproj -> C:\STU\devel\ReMoM\build\src\Debug\momlib.lib
  Building Custom Rule C:/STU/devel/ReMoM/src/CMakeLists.txt
...
  Generating Code...
  sdl2_ReMoM.vcxproj -> C:\STU\devel\ReMoM\build\src\Debug\sdl2_ReMoM.exe
  Building Custom Rule C:/STU/devel/ReMoM/CMakeLists.txt
