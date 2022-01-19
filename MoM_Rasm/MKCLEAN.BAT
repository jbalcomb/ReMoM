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

DEL TST_MGC.EXE
DEL TST_MGC.MAP
