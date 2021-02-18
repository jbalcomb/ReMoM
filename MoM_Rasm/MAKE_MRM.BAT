@ECHO OFF

REM -R Generate ObjectBrowser information
REM -S Produce .ASM output file

REM -v Source debugging on
REM -y Line numbers on

REM -3 Generate 80386 instructions

REM -Ol Enable loop optimizations
REM -G Select code for speed
REM -G- Select code for size (default)

REM local virtual tables

REM -Y Enable overlay code generation
REM -Yo Overlay the compiled files

REM -Tstring Pass string as an option to TASM or assembler specified with-E
REM -lx Pass option x to the linker
REM /l line numbers
REM /m map
REM /s detailed map
REM /v debugging information

REM Options BCC passes to TASM
REM /D__MODEL__ /D__LANG__ /mb FLOATOPT


REM tasm.exe /ml larg.asm
REM tcc.exe -mlarge large.c larg.obj

REM %TASM% /M /MX /Q /T /D__LARGE__   c0, %objdir%\c0l

REM TASM /ml Rasm_MoM.ASM
REM TASM /ml ReqdMem.ASM
REM TASM /ml FindFile.ASM
REM BCC -ml MoM_Rdx Rasm_MoM.OBJ ReqdMem.OBJ FindFile.OBJ

REM TASM /M /MX /Q /T /D__LARGE__ c0, c0l
REM TASM /ml Rasm_MoM.ASM
REM TASM /ml ReqdMem.ASM
REM TASM /ml FindFile.ASM
REM BCC -mlarge MoM_Rdx.CPP C0L.OBJ Rasm_MoM.OBJ ReqdMem.OBJ FindFile.OBJ

RESCAN



REM BCC -S -ml MoM_Rdx Rasm_MoM.ASM ReqdMem.ASM FindFile.ASM
REM BCC -ml MoM_Rdx Rasm_MoM.ASM ReqdMem.ASM FindFile.ASM

REM BCC -ml -S -v -y MoM_Rdx Rasm_MoM.ASM seg001\f010109.ASM seg004\f040101.ASM seg005\f050207.ASM seg006\f060206.ASM ReqdMem.ASM FindFile.ASM
BCC -ll -lm -ls -lv -ml -v -y @MRM.RSP

REM BCC -ml -S -v -y ReqdMem.ASM
REM BCC -ml -S -v -y FindFile.ASM
REM BCC -ml -S -v -y Rasm_MoM.ASM
REM BCC -ml -S -v -y seg001\f010109.ASM
REM BCC -ml -S -v -y seg004\f040101.ASM
REM BCC -ml -S -v -y seg005\f050207.ASM
REM BCC -ml -S -v -y seg006\f060206.ASM



REM BCC -ml -v -y MRM MRM_A.ASM
REM BCC -ml -v -y -T"" MRM MRM_A.ASM

REM C:\BORLANDC\BIN\BCC.EXE -ml -S -v -y MRM.CPP
REM C:\BORLANDC\BIN\TASM.EXE /zi MRM_A.ASM
REM C:\BORLANDC\BIN\TLINK.EXE /l /m /s /TDe /v C0L.OBJ MRM.OBJ MRM_A.OBJ, MRM.EXE, MRM.MAP, CL.LIB EMU.LIB GRAPHICS.LIB MATHL.LIB OVERLAY.LIB
REM C:\BORLANDC\BIN\TLINK.EXE /l /m /s /TDe /v C0L.OBJ MRM.OBJ MRM_A.OBJ, MRM.EXE, MRM.MAP, CL.LIB

REM TLINK objfiles, exefile, mapfile, lib files, deffile
