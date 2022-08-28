@ECHO OFF

if not exist build (mkdir build)

pushd build

cl -FC -Zi ..\MoM_Rasm\win32_mom.cpp user32.lib gdi32.lib

popd
