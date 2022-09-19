@ECHO OFF

REM ########################################
REM #####
REM ##### Dispoable Artifacts
REM #####
REM ##### BC, BCC, TASM, TLINK
REM #####
REM ##### ASM, EXE, LST, MAP, OBJ
REM #####
REM ########################################


DEL *.LST
DEL *.OBJ


REM ########################################
REM #####
REM #####  BCC - Per .C
REM #####
REM ########################################

REM No-Workie!! DEL *C.ASM
REM DEL ???????C.ASM
REM DEL ??????C.ASM
REM DEL ?????C.ASM
REM DEL ????C.ASM
REM DEL ???C.ASM
REM DEL ??C.ASM
REM DEL ?C.ASM
DEL MGC_DEF.ASM
DEL MGC_MAIN.ASM
DEL MOM_DEF.ASM
DEL ST_CRSR.ASM
DEL ST_CTRL.ASM
DEL ST_DBG.ASM
DEL ST_DEF.ASM
DEL ST_EMM.ASM
DEL ST_FLIC.ASM
DEL ST_GUI.ASM
DEL ST_HLP.ASM
DEL ST_LBX.ASM
DEL ST_SA.ASM
DEL ST_SCRN.ASM
DEL ST_SND.ASM
DEL ST_TXT.ASM
DEL ST_TYPE.ASM
DEL ST_VGA.ASM
DEL STU_DBG.ASM
DEL STU_UTIL.ASM
DEL STU_VGA.ASM
DEL TST_MGC.ASM


REM ########################################
REM #####
REM #####  TASM - Per .ASM
REM #####
REM ########################################

DEL *.MAP


REM ########################################
REM #####
REM #####  TLNIK - Per .EXE
REM #####
REM ########################################

REM #####  TST_MGC  #####
REM DEL TST_MGC.EXE
REM DEL TST_MGC.MAP

REM #####  TST  #####
REM DEL TST.EXE
REM DEL TST.MAP

REM #####  MGC  #####
REM DEL MGC.EXE
REM DEL MGC.MAP
