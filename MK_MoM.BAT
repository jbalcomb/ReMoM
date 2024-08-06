@ECHO OFF
REM Microsoft C/C++ Optimizing Compiler (v19.28.29337) (x64)

if not exist build (mkdir build)

pushd build

@SET SRC_MOX=..\src\MoX\Allocate.C ..\src\MoX\Exit.C ..\src\MoX\FLIC_Draw.C ..\src\MoX\LBX_Load.C ..\src\MoX.C ..\src\MoX\MoX_DBG.C

cl /c %SRC_MOX%

lib Allocate.obj Exit.obj FLIC_Draw.obj LBX_Load.obj MoX.obj MoX_DBG.obj /OUT:MoX.lib


@SET SRC_PFL=..\src\win_MoM.cpp ..\src\win_PFL.cpp ..\src\MoX_PFL.C ..\src\win_CLK.C ..\src\win_DIR.C ..\src\win_EMS.C ..\src\win_Fonts.C ..\src\win_Input.C ..\src\win_KD.C ..\src\win_MD.C ..\src\win_Video.C

REM cl /c %SRC_PFL%

REM lib win_MoM.obj win_PFL.obj MoX_PFL.obj win_CLK.obj win_DIR.obj win_EMS.obj win_Fonts.obj win_Input.obj win_KD.obj win_MD.obj win_Video.obj /OUT:PFL.lib


cl /DUNDEFINE_UNICODE /DSTU_DEBUG /D__WIN32__ /RTCs /RTCu /W1 /Zc:strictStrings -FC /Fe: MoM.exe -I ..\src\STU -I ..\src -Zi ..\src\win_MoM.cpp ..\src\win_PFL.cpp ..\src\MoX_PFL.C ..\src\win_CLK.C ..\src\win_DIR.C ..\src\win_EMS.C ..\src\win_Fonts.C ..\src\win_Input.C ..\src\win_KD.C ..\src\win_MD.C ..\src\win_Video.C ..\src\AdvsrScr.C ..\src\ALLOC.C ..\src\ArmyList.C ..\src\CITYCALC.C ..\src\CityList.C ..\src\CITYSCAP.C ..\src\CityScr.C ..\src\City_ovr55.C ..\src\Combat.C ..\src\DIPLOMAC.C ..\src\Explore.C ..\src\Fields.C ..\src\Fonts.C ..\src\GENDRAW.C ..\src\Graphics.C ..\src\Help.C ..\src\HIRE.C ..\src\Init.C ..\src\Input.C ..\src\ItemMake.C ..\src\Items.C ..\src\ItemScrn.C ..\src\ItemView.C ..\src\Keyboard.C ..\src\Lair.C ..\src\LOADER.C ..\src\LoadScr.C ..\src\MagicScr.C ..\src\MainMenu.C ..\src\MainScr.C ..\src\MainScr_Maps.C ..\src\MoM.C ..\src\Mouse.C ..\src\MovePath.C ..\src\MoX_CFG.C ..\src\MoX_CMB.C ..\src\MoX_Data.C ..\src\MoX_DOS.C ..\src\MoX_GAM.C ..\src\MoX_RNG.C ..\src\MoX_UnOrg.C ..\src\MoX_Util.C ..\src\NewGame.C ..\src\NEXTTURN.C ..\src\Outpost.C ..\src\paragrph.C ..\src\ProdScr.C ..\src\RACETYPE.C ..\src\Roads.C ..\src\SBookScr.C ..\src\SCastScr.C ..\src\SCORE.C ..\src\Settings.C ..\src\SETTLE.C ..\src\special.C ..\src\Spellbook.C ..\src\Terrain.C ..\src\TEXTBOX.C ..\src\UnitList.C ..\src\UnitMove.C ..\src\UnitStat.C ..\src\UNITSTK.C ..\src\UnitType.C ..\src\UnitView.C  ..\src\Video.C ..\src\Video2.C ..\src\WIZVIEW.C ..\src\WZD_o059.C ..\src\WZD_o143.C ..\src\STU\STU_DBG.C  ..\src\STU\STU_UTIL.C user32.lib gdi32.lib winmm.lib MoX.lib

popd
