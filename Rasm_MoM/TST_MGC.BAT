@ECHO OFF

REM ######################################################################################
REM #
REM #  BCC.EXE - Borland C++ v3.00 (c)1991
REM #
REM       C:\BORLANDC\BIN\BCC.EXE -ml -nbcc_out -S -v -y @MRM.RSP
REM       C:\BORLANDC\BIN\BCC.EXE -ml -nout -v -y @MGC.RSP
REM #
REM #      -B
REM #          Compile via assembly
REM #      -c
REM #          Compile only
REM #      -D
REM #          Define macro
REM #      -nxxx
REM #          Output file directory
REM #      -M
REM #          Generate link map
REM #      -mx
REM #          Set Memory Model
REM #      -oxxx
REM #          Object file name
REM #      -Ox
REM #          Optimizations
REM #      -S
REM #          Produce assembly output
REM #      -v
REM #          Source level debugging
REM #      -y
REM #          Produce line number info
REM #
REM ######################################################################################

REM ######################################################################################
REM #
REM #  TASM.EXE - Turbo Assembler v3.0 (c)1991
REM #
REM #    /c
REM #         Generate cross-reference in listing
REM #    /e
REM #         Generates floating-point emulator instructions
REM #    /la  {/l, /la}
REM #         Shows high-level interface code in listing file
REM #    /m
REM #         Allow multiple passes to resolve forward references (default 5)
REM #    /ml  {/ml, /mx, /mu}
REM #         Treats symbols as case-sensitive
REM #    /t
REM #         Suppresses messages on successful assembly
REM #    /z
REM #         Displays source lines along with error messages
REM #    /zi  {/zi, /zd, /zn}
REM #         Function Enables debug information in object file
REM #
REM ######################################################################################

REM ######################################################################################
REM #
REM #  TLINK.EXE - Turbo Link v5.0 (c)1991
REM #
REM #    /c
REM #         Case significant in symbols
REM #    /l
REM #         creates a section in the .MAP file for source code line numbers [NOTE: Requires -y or -v and NOT -lx]
REM #    /s
REM #         creates a map file with segments, public symbols, the program start address, and adds a detailed segment map [NOTE: s > m > default < x]
REM #    /TDe 
REM #         Specify output file type: DOS image, EXE
REM #    /v
REM #         Full symbolic debug information
REM #    ,,,
REM #         generates the OBJ, LST, MAP, and XREF
REM #
REM ######################################################################################

SET STBC=C:\BORLANDC\BIN\BCC.EXE -B -c -ml -M -S -v -y
SET STTA=C:\BORLANDC\BIN\TASM.EXE /c /e /la /m /ml /t /z /zi
SET STTL=C:\BORLANDC\BIN\TLINK.EXE /c /l /s /TDe /v

RESCAN
CLS

REM DEL TST_MGC.EXE
REM DEL TST_MGC.MAP
REM DEL *C.ASM
REM DEL *C.LST
REM DEL *.OBJ



%STBC% MGC_MAIN.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% TST_MGC.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% MGC_DEF.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% MOM_DEF.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% ST_DBG.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% ST_DEF.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% ST_EMM.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% ST_GUI.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% ST_LBX.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% ST_SA.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% ST_SND.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% ST_VGA.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% STU_UTIL.C
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

%STBC% LbxLdEt1.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s05p01c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s05p02c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s05p05c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s06p01c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s06p02c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s06p04c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s07p04c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s07p05.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s07p06c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s07p07c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s07p08.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s07p09.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s08p07c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s08p08c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s08p15c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s08p16.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s08p19c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s09p01c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s09p02.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s09p03.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s09p04.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s09p05c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s09p06.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s09p07.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s10p01c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s10p10c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s10p15c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s10p16.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s10p17c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s12p01.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s12p02c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s12p03c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s12p04c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s12p05.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s12p06.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s12p07c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s12p08.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s12p07c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s13p01c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s13p11c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s13p12c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s13p15c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s13p16c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s13p20c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s13p31c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s13p32c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s14p01c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s14p03c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

tasm /c /e /la /m /ml /t /z /zi _s15p01.asm
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s19p14c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s24p05c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

bcc -B -c -ml -M -S -v -y _s24p06c.c
IF ERRORLEVEL 1 GOTO ERROR_HANDLER

REM %STBC% _s33p01c.c
REM IF ERRORLEVEL 1 GOTO ERROR_HANDLER

REM %STBC% _s34p65c.c
REM IF ERRORLEVEL 1 GOTO ERROR_HANDLER


tlink @TST_MGC.RSP



GOTO DONE

:ERROR_HANDLER
ECHO ERROR: compilation failed - skipping linking

:DONE
