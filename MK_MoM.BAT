@ECHO OFF

if not exist build (mkdir build)

pushd build

cl /DSTU_DEBUG /D__WIN32__ -FC /Fe: MoM.exe -I ..\src\STU -I ..\src -Zi ..\src\Win32_Main.cpp ..\src\Win32_Init.cpp ..\src\Win32_Evnt.cpp ..\src\win_DIR.C ..\src\MoM_main.C ..\src\LBX_Load.C ..\src\Allocate.C ..\src\FLIC_Draw.C ..\src\Fields.C ..\src\Fonts.C ..\src\Input.C ..\src\Keyboard.C ..\src\Mouse.C ..\src\MoX_DBG.C ..\src\MoX_CFG.C ..\src\MoX_Data.C ..\src\MoX_GAM.C ..\src\MoX_SET.C ..\src\Video.C ..\src\MainMenu.C ..\src\STU\STU_DBG.C ..\src\STU\STU_UTIL.C ..\src\STU\STU_BMP.C user32.lib gdi32.lib winmm.lib

popd