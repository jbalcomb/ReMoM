@ECHO OFF

IF "%COMSPEC%"=="Z:\COMMAND.COM" ECHO Running in DOSBox...


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
DEL MAINMENU\*.OBJ

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

REM #####  TST  #####
DEL TST.EXE

REM #####  MGC  #####
DEL MGC.EXE
