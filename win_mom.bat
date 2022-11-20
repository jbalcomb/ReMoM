@ECHO OFF

if not exist build (mkdir build)

pushd build

cl /D__WIN32__ -FC -I ..\MoM_Rasm\STU -Zi ..\MoM_Rasm\win32_mom.cpp user32.lib gdi32.lib

popd
