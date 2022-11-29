@ECHO OFF

if not exist build (mkdir build)

pushd build

cl /D__WIN32__ -FC -I ..\MoM_Rasm\STU -Zi ..\MoM_Rasm\Win32_WinMain.C user32.lib gdi32.lib

popd
