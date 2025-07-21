REM C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\HostX64\x64\CL.exe

cl^
 /c^
 /IC:\STU\devel\ReMoM\src^
 /IC:\STU\devel\ReMoM^
 /IC:\STU\devel\ReMoM\src\MOX^
 /IC:\STU\devel\ReMoM\src\STU^
 /I"C:\Python311\Lib\site-packages\vcpkgpip\vcpkg\installed\x64-windows\include"^
 /nologo^
 /W1^
 /WX-^
 /diagnostics:column^
 /O2^
 /Ob2^
 /D _MBCS^
 /D WIN32^
 /D _WINDOWS^
 /D NDEBUG^
 /D "CMAKE_INTDIR=\"Release\""^
 /Gm-^
 /EHsc^
 /MD^
 /GS^
 /fp:precise^
 /Zc:wchar_t^
 /Zc:forScope^
 /Zc:inline^
 /std:c++17^
 /Fo"testLearning.dir\Release\\"^
 /Fd"testLearning.dir\Release\vc143.pdb"^
 /external:W0^
 /Gd^
 /TP^
 /errorReport:queue^
 /external:I "C:/STU/devel/ReMoM/build/_deps/googletest-src/googletest/include"^
 /external:I "C:/STU/devel/ReMoM/build/_deps/googletest-src/googletest"^
 /external:I "C:/devellib/SDL2-2.32.2/include"^
 /external:I "C:/devellib/SDL2_mixer-2.8.1/include"^
 C:\STU\devel\ReMoM\tests\unit\testLearning.cpp
