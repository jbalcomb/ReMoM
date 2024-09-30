@ECHO OFF
REM Microsoft C/C++ Optimizing Compiler (v19.28.29337) (x64)

if not exist build (mkdir build)

pushd build

@SET SRC_STU=..\src\STU\STU_CHK.C ..\src\STU\STU_DBG.C ..\src\STU\STU_UTIL.C 

cl /c %SRC_STU%

lib STU_CHK.obj STU_DBG.obj STU_UTIL.obj /OUT:STU.lib

@SET SRC_MOX=..\src\MoX\MOX_T4.C ..\src\MoX\Allocate.C ..\src\MoX\CFG.C ..\src\MoX\DBG.C ..\src\MoX\DOS.C ..\src\MoX\EMM.C ..\src\MoX\Exit.C ..\src\MoX\Fields.C ..\src\MoX\file_ani.C ..\src\MoX\Fonts.C ..\src\MoX\GENDRAW.C ..\src\MoX\Graphics.C  ..\src\MoX\FLIC_Draw.C ..\src\MoX\Input.C ..\src\MoX\IO_Buffer.C ..\src\MoX\Keyboard.C ..\src\MoX\LBX_Load.C ..\src\MoX\LOADSAVE.C ..\src\MoX\Mouse.C ..\src\MoX\MoX_Temp.C ..\src\MoX\paragrph.C ..\src\MoX\random.C ..\src\MoX\Util.C ..\src\MoX\Video.C ..\src\MoX\Video2.C

cl /c %SRC_MOX%

REM lib Allocate.obj Exit.obj FLIC_Draw.obj LBX_Load.obj MoX.obj MoX_DBG.obj /OUT:MoX.lib
lib MOX_T4.obj Allocate.obj CFG.obj DBG.obj DOS.obj EMM.obj Exit.obj Fields.obj file_ani.obj Fonts.obj GENDRAW.obj Graphics.obj  FLIC_Draw.obj Input.obj IO_Buffer.obj Keyboard.obj LBX_Load.obj LOADSAVE.obj Mouse.obj MoX_Temp.obj paragrph.obj random.obj Util.obj Video.obj Video2.obj /OUT:MoX.lib

@SET SRC_PFL=..\src\win_MoM.cpp ..\src\win_PFL.cpp ..\src\MoX_PFL.C ..\src\win_CLK.C ..\src\win_DIR.C ..\src\win_EMS.C ..\src\win_Fonts.C ..\src\win_Input.C ..\src\win_KD.C ..\src\win_MD.C ..\src\win_Video.C

REM cl /c %SRC_PFL%

REM lib win_MoM.obj win_PFL.obj MoX_PFL.obj win_CLK.obj win_DIR.obj win_EMS.obj win_Fonts.obj win_Input.obj win_KD.obj win_MD.obj win_Video.obj /OUT:PFL.lib

cl /DUNDEFINE_UNICODE /DSTU_DEBUG /D__WIN32__ /RTCs /RTCu /W1 /Zc:strictStrings -FC /Fe: MoM.exe -I ..\src\STU -I ..\src -Zi ..\src\win_MoM.cpp ..\src\win_PFL.cpp ..\src\MoM_PFL.C ..\src\win_CLK.C ..\src\win_DIR.C ..\src\win_EMS.C ..\src\win_Fonts.C ..\src\win_Input.C ..\src\win_KD.C ..\src\win_MD.C ..\src\win_Video.C ..\src\AdvsrScr.C ..\src\AIBUILD.C ..\src\AIDATA.C ..\src\AIDUDES.C ..\src\ALLOC.C ..\src\ArmyList.C ..\src\CITYCALC.C ..\src\CityList.C ..\src\CITYSCAP.C ..\src\CityScr.C ..\src\City_ovr55.C ..\src\Combat.C ..\src\DIPLOMAC.C ..\src\EVENTS.C ..\src\Explore.C ..\src\Help.C ..\src\HIRE.C ..\src\Init.C ..\src\ItemMake.C ..\src\Items.C ..\src\ItemScrn.C ..\src\ItemView.C ..\src\Lair.C ..\src\LOADER.C ..\src\LoadScr.C ..\src\LVLMAKE.C ..\src\MagicScr.C ..\src\MainMenu.C ..\src\MainScr.C ..\src\MainScr_Maps.C ..\src\MoM.C ..\src\MoM_DBG.C ..\src\MovePath.C ..\src\MoM_Data.C ..\src\NewGame.C ..\src\NEXTTURN.C ..\src\Outpost.C ..\src\ProdScr.C ..\src\RACETYPE.C ..\src\REPORT.C ..\src\Roads.C ..\src\SBookScr.C ..\src\SCastScr.C ..\src\SCORE.C ..\src\Settings.C ..\src\SETTLE.C ..\src\special.C ..\src\Spellbook.C ..\src\SOUND.C ..\src\SPLMASTR.C ..\src\Terrain.C ..\src\TEXTBOX.C ..\src\UnitList.C ..\src\UnitMove.C ..\src\UnitStat.C ..\src\UNITSTK.C ..\src\UnitType.C ..\src\UnitView.C ..\src\WIZVIEW.C ..\src\WZD_o059.C ..\src\WZD_o129.C ..\src\WZD_o143.C ..\src\STU\STU_CHK.C ..\src\STU\STU_DBG.C ..\src\STU\STU_UTIL.C user32.lib gdi32.lib winmm.lib MoX.lib

popd
