@ECHO OFF
REM Microsoft C/C++ Optimizing Compiler (v19.28.29337) (x64)

if not exist build (mkdir build)

pushd build

@SET SRC_STU=..\src\STU\STU_CHK.C ..\src\STU\STU_DBG.C ..\src\STU\STU_UTIL.C 

cl /c %SRC_STU%

lib STU_CHK.obj STU_DBG.obj STU_UTIL.obj /OUT:STU.lib

@SET SRC_MOX=..\src\MOX\Allocate.C ..\src\MOX\CFG.C ..\src\MOX\DBG.C ..\src\MOX\DOS.C ..\src\MOX\EMM.C ..\src\MOX\Exit.C ..\src\MOX\Fields.C ..\src\MOX\file_ani.C ..\src\MOX\Fonts.C ..\src\MOX\GENDRAW.C ..\src\MOX\Graphics.C  ..\src\MOX\FLIC_Draw.C ..\src\MOX\Input.C ..\src\MOX\IO_Buffer.C ..\src\MOX\Keyboard.C ..\src\MOX\LBX_Load.C ..\src\MOX\LOADSAVE.C ..\src\MOX\Mouse.C ..\src\MOX\MOX_DAT.C ..\src\MOX\MOX_ITOA.C ..\src\MOX\MOX_SET.C ..\src\MOX\MOX_SND.C ..\src\MOX\MOX_T4.C ..\src\MOX\paragrph.C ..\src\MOX\random.C ..\src\MOX\SOUND.C ..\src\MOX\Util.C ..\src\MOX\Video.C ..\src\MOX\Video2.C

cl /c %SRC_MOX%

REM lib Allocate.obj Exit.obj FLIC_Draw.obj LBX_Load.obj MOX.obj MOX_DBG.obj /OUT:MOX.lib
lib Allocate.obj CFG.obj DBG.obj DOS.obj EMM.obj Exit.obj Fields.obj file_ani.obj Fonts.obj GENDRAW.obj Graphics.obj FLIC_Draw.obj Input.obj IO_Buffer.obj Keyboard.obj LBX_Load.obj LOADSAVE.obj Mouse.obj MOX_DAT.obj MOX_ITOA.obj MOX_SET.obj MOX_SND.obj MOX_T4.obj paragrph.obj random.obj SOUND.obj Util.obj Video.obj Video2.obj /OUT:MOX.lib

@SET SRC_PFL=..\src\win_MoM.cpp ..\src\win_PFL.cpp ..\src\MOM_PFL.C ..\src\win_CLK.C ..\src\win_DIR.C ..\src\win_EMS.C ..\src\win_Fonts.C ..\src\win_Input.C ..\src\win_KD.C ..\src\win_MD.C ..\src\win_Video.C

REM cl /c %SRC_PFL%

REM lib win_MoM.obj win_PFL.obj MOM_PFL.obj win_CLK.obj win_DIR.obj win_EMS.obj win_Fonts.obj win_Input.obj win_KD.obj win_MD.obj win_Video.obj /OUT:PFL.lib

@SET SRC_MOM=..\src\AdvsrScr.C ..\src\AIBUILD.C ..\src\AIDATA.C ..\src\AIDUDES.C ..\src\AIMOVE.C ..\src\AISPELL.C ..\src\ALLOC.C ..\src\ArmyList.C ..\src\CITYCALC.C ..\src\CityList.C ..\src\CITYSCAP.C ..\src\CityScr.C ..\src\City_ovr55.C ..\src\Combat.C ..\src\DIPLOMAC.C ..\src\EVENTS.C ..\src\Explore.C ..\src\Help.C ..\src\HIRE.C ..\src\Init.C ..\src\ItemMake.C ..\src\Items.C ..\src\ItemScrn.C ..\src\ItemView.C ..\src\Lair.C ..\src\LOADER.C ..\src\LoadScr.C ..\src\LVLMAKE.C ..\src\MagicScr.C ..\src\MainMenu.C ..\src\MainScr.C ..\src\MainScr_Maps.C ..\src\MOM.C ..\src\MOM_Data.C ..\src\MOM_DBG.C ..\src\MOM_SCR.C ..\src\MovePath.C ..\src\NewGame.C ..\src\NEXTTURN.C ..\src\Outpost.C ..\src\OverSpel.C ..\src\ProdScr.C ..\src\RACETYPE.C ..\src\REPORT.C ..\src\Roads.C ..\src\SBookScr.C ..\src\SCastScr.C ..\src\SCORE.C ..\src\Settings.C ..\src\SETTLE.C ..\src\special.C ..\src\Spellbook.C ..\src\SPLMASTR.C ..\src\Surveyor.C ..\src\Terrain.C ..\src\TEXTBOX.C ..\src\UnitList.C ..\src\UnitMove.C ..\src\UnitStat.C ..\src\UNITSTK.C ..\src\UnitType.C ..\src\UnitView.C ..\src\WIZVIEW.C ..\src\WZD_o059.C ..\src\WZD_o129.C ..\src\WZD_o143.C

@SET WIN_PFL=..\src\win_MoM.cpp ..\src\win_PFL.cpp ..\src\MOM_PFL.C ..\src\win_CLK.C ..\src\win_DIR.C ..\src\win_EMS.C ..\src\win_Fonts.C ..\src\win_Input.C ..\src\win_KD.C ..\src\win_MD.C ..\src\win_Video.C

cl /DUNDEFINE_UNICODE /DSTU_DEBUG /D__WIN32__ /RTCs /RTCu /W1 /wd4068 /Zc:strictStrings -FC /Fe: ReMoM.exe -I ..\src\STU -I ..\src -Zi %WIN_PFL% %SRC_MOM% user32.lib gdi32.lib winmm.lib MOX.lib STU.lib

REM cl /DUNDEFINE_UNICODE /DSTU_DEBUG /D__WIN32__ /RTCs /RTCu /W1 /wd4068 /Zc:strictStrings -FC /Fe: ReMoM.exe -I ..\src\STU -I ..\src -Zi ..\src\win_MoM.cpp ..\src\win_PFL.cpp ..\src\MOM_PFL.C ..\src\win_CLK.C ..\src\win_DIR.C ..\src\win_EMS.C ..\src\win_Fonts.C ..\src\win_Input.C ..\src\win_KD.C ..\src\win_MD.C ..\src\win_Video.C ..\src\AdvsrScr.C ..\src\AIBUILD.C ..\src\AIDATA.C ..\src\AIDUDES.C ..\src\AISPELL.C ..\src\ALLOC.C ..\src\ArmyList.C ..\src\CITYCALC.C ..\src\CityList.C ..\src\CITYSCAP.C ..\src\CityScr.C ..\src\City_ovr55.C ..\src\Combat.C ..\src\DIPLOMAC.C ..\src\EVENTS.C ..\src\Explore.C ..\src\Help.C ..\src\HIRE.C ..\src\Init.C ..\src\ItemMake.C ..\src\Items.C ..\src\ItemScrn.C ..\src\ItemView.C ..\src\Lair.C ..\src\LOADER.C ..\src\LoadScr.C ..\src\LVLMAKE.C ..\src\MagicScr.C ..\src\MainMenu.C ..\src\MainScr.C ..\src\MainScr_Maps.C ..\src\MOM.C ..\src\MOM_Data.C ..\src\MOM_DBG.C ..\src\MovePath.C ..\src\NewGame.C ..\src\NEXTTURN.C ..\src\Outpost.C ..\src\OverSpel.C ..\src\ProdScr.C ..\src\RACETYPE.C ..\src\REPORT.C ..\src\Roads.C ..\src\SBookScr.C ..\src\SCastScr.C ..\src\SCORE.C ..\src\Settings.C ..\src\SETTLE.C ..\src\special.C ..\src\Spellbook.C ..\src\SPLMASTR.C ..\src\Surveyor.C ..\src\Terrain.C ..\src\TEXTBOX.C ..\src\UnitList.C ..\src\UnitMove.C ..\src\UnitStat.C ..\src\UNITSTK.C ..\src\UnitType.C ..\src\UnitView.C ..\src\WIZVIEW.C ..\src\WZD_o059.C ..\src\WZD_o129.C ..\src\WZD_o143.C user32.lib gdi32.lib winmm.lib MOX.lib STU.lib

REM /wd4068  disable warning C4068: unknown pragma

popd
