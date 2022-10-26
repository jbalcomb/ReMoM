
# J:\STU\DBWD\developc\dos-utils-master\zlib\msdos\Makefile.tc

# Memory Model
MODEL=L
# C Compiler
CC = BCC.EXE
# ASM Compiler
CA = TASM.EXE
# Linker
LD = TLINK.EXE

# Compiler Flags
CFLAGS = 
# Linker Flags
LDFLAGS = 

.suffixes: .cpp .c .asm

.c.obj:
    BCC -B -c -DSTU_DEBUG=1 -DSTU_TEST=1 -D__DOS16__=1 -ml -M -S -Tm2 -U__VSCODE__ -v -y $<
#    BCC -B -c -DSTU_DEBUG=1 -DSTU_TEST=1 -D__DOS16__=1 -g1 -j1 -ml -M -S -Tm2 -U__VSCODE__ -v -y $<
#    BCC -B -c -ml -M -S -Tm2 -U__VSCODE__ -v -y $<

.cpp.obj:
    BCC -B -c -DSTU_DEBUG=1 -DSTU_TEST=1 -D__DOS16__=1 -ml -M -S -U__VSCODE__ -v -y $<
#    BCC -B -c -DSTU_DEBUG=1 -DSTU_TEST=1 -D__DOS16__=1 -g1 -j1 -ml -M -S -U__VSCODE__ -v -y $<
#    BCC -B -c -ml -M -S -U__VSCODE__ -v -y $<

.asm.obj:
    TASM /c /dSTU_DEBUG=1 /dSTU_TEST=1 /d__DOS16__=1 /e /la /m2 /ml /t /z /zi $<
#    TASM /c /e /la /m2 /ml /t /z /zi $<

# OBJ1 = adler32.obj compress.obj crc32.obj deflate.obj gzio.obj infback.obj
# OBJ2 = inffast.obj inflate.obj inftrees.obj trees.obj uncompr.obj zutil.obj
# OBJP1 = +adler32.obj+compress.obj+crc32.obj+deflate.obj+gzio.obj+infback.obj
# OBJP2 = +inffast.obj+inflate.obj+inftrees.obj+trees.obj+uncompr.obj+zutil.obj

# OBJ_s34 = _s34p01c.obj _s34p08c.obj _s34p12c.obj _s34p13c.obj _s34p22c.obj _s34p25.obj _s34p26.obj _s34p47.obj _s34p49c.obj _s34p57c.obj _s34p65c.obj _s34p66c.obj _s34p67c.obj _s34p71c.obj _s34p72c.obj _s34p73c.obj _s34p76c.obj _s34p79c.obj _s34p81c.obj _s34p82c.obj _s34p83c.obj _s34p85c.obj
# OBJP_s34 = _s34p01c.obj+_s34p08c.obj+_s34p12c.obj+_s34p13c.obj+_s34p22c.obj+_s34p25.obj+_s34p26.obj+_s34p47.obj+_s34p49c.obj+_s34p57c.obj+_s34p65c.obj+_s34p66c.obj+_s34p67c.obj+_s34p71c.obj+_s34p72c.obj+_s34p73c.obj+_s34p76c.obj+_s34p79c.obj+_s34p81c.obj+_s34p82c.obj+_s34p83c.obj+_s34p85c.obj


OBJ_STU = STU_DBG.OBJ STU_TST.OBJ STU_UTIL.OBJ STU_VGA.OBJ
OBJ_MGC = MGC_DEF.OBJ
OBJ_MoM = MOM_DEF.OBJ
OBJ_ST = ST_DEF.OBJ ST_CRSR.OBJ ST_CTRL.OBJ ST_DBG.OBJ ST_EMM.OBJ ST_EXIT.OBJ ST_FLIC.OBJ ST_GUI.OBJ ST_HLP.OBJ ST_LBX.OBJ ST_SA.OBJ ST_SCRN.OBJ ST_SND.OBJ ST_TXT.OBJ ST_TYPE.OBJ ST_QUIT.OBJ ST_VGA.OBJ
OBJ_o57 = _o57p01c.obj
OBJ_o60 = _o60p01c.obj _o60p02c.obj
OBJ_s01 = seg001.obj
OBJ_s02 = _s02p01.obj _s02p03.obj _s02p05.obj _s03p05c.obj
OBJ_s04 = _s04p01c.obj
# OBJ_s05 = ST_EXIT
# OBJ_s06 = ST_QUIT
# OBJ_s10 = ST_LBX
OBJ_s14 = seg014.obj
OBJ_s15 = _s15p01.obj
OBJ_s16 = _s16p01a.obj
OBJ_s17 = _s17p06c.obj _s17p24c.obj _s17p35c.obj _s17p36.obj _s17p37.obj _s17p39.obj _s17p40.obj _s17p41.obj _s17p42.obj
OBJ_s18 = _s18p01.obj _s18p02.obj _s18p03.obj _s18p05.obj _s18p06.obj _s18p07.obj
OBJ_s19 = seg019.obj
OBJ_s20 = seg020.obj
OBJ_s21 = seg021.obj
OBJ_s22 = seg022.obj
OBJ_s24 = seg024.obj
OBJ_s26 = _s26p01c.obj _s26p02c.obj _s26p03.obj _s26p04a.obj _s26p05.obj _s26p06.obj _s26p07.obj
OBJ_s27 = seg027.obj _s27.obj _s27a.obj
OBJ_s28 = seg028.obj
OBJ_s33 = _s33p01c.obj _s33p02.obj _s33p03c.obj _s33p04c.obj _s33p05.obj _s33p06.obj _s33p07c.obj _s33p09.obj _s33p10a.obj _s33p11a.obj _s33p12.obj _s33p13c.obj _s33p14c.obj _s33p16.obj _s33p18c.obj _s33p19c.obj _s33p17.obj _s33p20.obj _s33p21.obj _s33p22c.obj _s33p23c.obj _s33p24c.obj _s33p25c.obj _s33p26c.obj _s33p29.obj _s33p30.obj _s33p31.obj _s33p32a.obj _s33p33.obj _s33p34.obj _s33p35.obj _s33p37.obj _s33p37c.obj _s33p38.obj _s33p38c.obj _s33p39.obj _s33p40.obj
OBJ_s34 = _s34p01c.obj _s34p08c.obj _s34p12c.obj _s34p13c.obj _s34p22c.obj _s34p25.obj _s34p26.obj _s34p47.obj _s34p49c.obj _s34p57c.obj _s34p65c.obj _s34p66c.obj _s34p67c.obj _s34p71c.obj _s34p72c.obj _s34p73c.obj _s34p76c.obj _s34p79c.obj _s34p81c.obj _s34p82c.obj _s34p83c.obj _s34p85c.obj

# targets
all: TST.EXE

# _s34.OBJ: _s34p01c.c _s34p08c.c _s34p12c.c _s34p13c.c _s34p22c.c _s34p25.asm _s34p26.asm _s34p47.asm _s34p49c.c _s34p57c.c _s34p65c.c _s34p66c.c _s34p67c.c _s34p71c.c _s34p72c.c _s34p73c.c _s34p76c.c _s34p79c.c _s34p81c.c _s34p82c.c _s34p83c.c _s34p85c.c
# STU_DBG.OBJ: STU_DBG.C ST_DBG.H
# ovl060.obj: _o60p01c.c _o60p02c.c Why does this do nothing at all?
# seg014.obj: seg014.C seg014.H

TST.EXE: TST_MGC.OBJ $(OBJ_STU) $(OBJ_MGC) $(OBJ_MoM) $(OBJ_ST) $(OBJ_o57) $(OBJ_o60) $(OBJ_s01) $(OBJ_s02) $(OBJ_s04) $(OBJ_s14) $(OBJ_s15) $(OBJ_s16) $(OBJ_s17) $(OBJ_s18) $(OBJ_s19) $(OBJ_s20) $(OBJ_s21) $(OBJ_s22) $(OBJ_s24) $(OBJ_s26) $(OBJ_s27) $(OBJ_s28) $(OBJ_s33) $(OBJ_s34)
    TLINK @MK_TST.RSP
