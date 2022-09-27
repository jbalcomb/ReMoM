.suffixes: .cpp .c .asm

.c.obj:
    BCC -B -c -ml -Tm2 $<
#    BCC -B -c -ml -M -S -Tm2 -v -y $<
#    BCC -B -c -ml -M -S -Tm2 -UDEBUG -USTU_DEBUG -UTEST -USTU_TEST -U__VSCode__ -v -y $<
#    BCC -B -c -DSTU_DEBUG=1 -D__DOS16__=1 -ml -M -S -Tm2 -U__VSCode__ -v -y $<

.cpp.obj:
    BCC -B -c -ml -Tm2 $<
#    BCC -B -c -ml -M -S -Tm2 -v -y $<
#    BCC -B -c -ml -M -S -Tm2 -UDEBUG -USTU_DEBUG -UTEST -USTU_TEST -U__VSCode__ -v -y $<
#    BCC -B -c -DSTU_DEBUG=1 -D__DOS16__=1 -ml -M -S -U__VSCode__ -v -y $<

.asm.obj:
    TASM /m2 /ml /t $<
#    TASM /c /e /la /m2 /ml /t /z /zi $<
#    TASM /c /dSTU_DEBUG=1 /d__DOS16__=1 /e /la /m2 /ml /t /z /zi $<



MGC.EXE: MGC.OBJ ST_DEF.OBJ MOM_DEF.OBJ MGC_DEF.OBJ ST_CRSR.OBJ ST_CTRL.OBJ ST_DBG.OBJ ST_EMM.OBJ ST_FLIC.OBJ ST_GUI.OBJ ST_HLP.OBJ ST_LBX.OBJ ST_SA.OBJ ST_SCRN.OBJ ST_SND.OBJ ST_SND.OBJ ST_TXT.OBJ ST_TYPE.OBJ ST_VGA.OBJ _o57p01c.obj _o60p01c.obj _o60p02c.obj seg001.obj _s02p01.obj _s02p03.obj _s02p05.obj _s03p05c.obj _s04p01c.obj seg005.obj seg006.obj _s10p10c.obj _s10p11c.obj _s10p12c.obj _s10p13c.obj seg014.obj _s15p01.obj _s16p01a.obj _s17p06c.obj _s17p24c.obj _s17p35c.obj _s17p36.obj _s17p37.obj _s17p39.obj _s17p40.obj _s17p41.obj _s17p42.obj _s18p01.obj _s18p02.obj _s18p03.obj _s18p05.obj _s18p06.obj _s18p07.obj seg019.obj seg020.obj seg021.obj _s22p07.obj _s22p22c.obj _s24p05c.obj _s24p06c.obj _s26p01c.obj _s26p02c.obj _s26p03.obj _s26p04a.obj _s26p05.obj _s26p06.obj _s26p07.obj _s27.obj _s27a.obj _s27p03c.obj seg028.obj _s28p02a.obj _s28p14c.obj _s28p15a.obj _s28p16c.obj _s33p01c.obj _s33p02.obj _s33p03c.obj _s33p04c.obj _s33p05.obj _s33p06.obj _s33p07c.obj _s33p09.obj _s33p10a.obj _s33p11a.obj _s33p12.obj _s33p13c.obj _s33p14c.obj _s33p16.obj _s33p18c.obj _s33p19c.obj _s33p17.obj _s33p20.obj _s33p21.obj _s33p22c.obj _s33p23c.obj _s33p24c.obj _s33p25c.obj _s33p26c.obj _s33p29.obj _s33p30.obj _s33p31.obj _s33p32a.obj _s33p33.obj _s33p34.obj _s33p35.obj _s33p37.obj _s33p37c.obj _s33p38.obj _s33p38c.obj _s33p39.obj _s33p40.obj _s34p01c.obj _s34p08c.obj _s34p12c.obj _s34p13c.obj _s34p22c.obj _s34p25.obj _s34p26.obj _s34p47.obj _s34p49c.obj _s34p57c.obj _s34p65c.obj _s34p66c.obj _s34p67c.obj _s34p71c.obj _s34p72c.obj _s34p73c.obj _s34p76c.obj _s34p79c.obj _s34p81c.obj _s34p82c.obj _s34p83c.obj _s34p85c.obj
    TLINK @MK_MGC.RSP


# MK_MGC.RSP
# /c /l /s /TDe /v +
# C:\BORLANDC\LIB\C0L.OBJ +
# MGC.OBJ +
# ...
# _s34p85c.obj, +
# MGC.EXE, +
# MGC.MAP, +
# C:\BORLANDC\LIB\CL.LIB
