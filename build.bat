@ECHO OFF

mkdir build
pushd build

REM cl ..\MoM_Rasm\win32_mom.cpp
REM cl -Zi ..\MoM_Rasm\win32_mom.cpp user32.lib
REM cl -Zi ..\MoM_Rasm\win32_mom.cpp user32.lib gdi32.lib
cl -FC -Zi ..\MoM_Rasm\win32_mom.cpp user32.lib gdi32.lib

popd
