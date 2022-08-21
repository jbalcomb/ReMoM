.suffixes: .cpp .c .asm

.c.obj:
    BCC -B -c -DDEBUG=1 -ml -M -S -Tm2 -U__VSCode__ -v -y $<

.cpp.obj:
    BCC -B -c -DDEBUG=1 -ml -M -S -U__VSCode__ -v -y $<

.asm.obj:
    TASM /c /dDEBUG=1 /e /la /m2 /ml /t /z /zi $<



TST_MGC.EXE: TST_MGC.C _s27a.c TST_MGC.OBJ Y4_VL.OBJ TST_STGE.OBJ TST_CRSR.OBJ TST_FLI.OBJ STU_DBG.OBJ STU_UTIL.OBJ STU_VGA.OBJ MGC_MAIN.OBJ MGC_DEF.OBJ MOM_DEF.OBJ ST_CRSR.OBJ ST_CTRL.OBJ ST_DBG.OBJ ST_DEF.OBJ ST_EMM.OBJ ST_GUI.OBJ ST_HLP.OBJ ST_LBX.OBJ ST_SA.OBJ ST_SCRN.OBJ ST_SND.OBJ ST_TXT.OBJ ST_SND.OBJ ST_VGA.OBJ FileSize.obj LbxLdEt1.obj CR_Draw.obj CR_Store.obj CR_Resto.obj _o57p01c.obj _o60p01c.obj _o60p02c.obj _s01p03c.obj _s01p04c.obj _s01p05c.obj _s01p06c.obj _s01p07c.obj _s01p08c.obj _s02p01.obj _s02p03.obj _s02p05.obj _s03p05c.obj _s04p01c.obj _s05p01c.obj _s05p02c.obj _s05p05c.obj _s06p01c.obj _s06p02c.obj _s06p04c.obj _s07p04c.obj _s07p05.obj _s07p06c.obj _s07p07c.obj _s07p08.obj _s07p09.obj _s08p07c.obj _s08p08c.obj _s08p12c.obj _s08p14c.obj _s08p15c.obj _s08p16.obj _s08p19c.obj _s09p01c.obj _s09p02.obj _s09p03.obj _s09p05a.obj _s09p06a.obj _s09p07.obj _s10p01c.obj _s10p02c.obj _s10p03c.obj _s10p04c.obj _s10p10c.obj _s10p11c.obj _s10p12c.obj _s10p13c.obj _s10p15c.obj _s10p16.obj _s10p17c.obj _s12p01.obj _s12p02c.obj _s12p03c.obj _s12p04c.obj _s12p05.obj _s12p06c.obj _s12p07c.obj _s12p08c.obj _s12p10.obj _s12p11c.obj _s12p12c.obj _s13p01c.obj _s13p03c.obj _s13p04c.obj _s13p11c.obj _s13p12c.obj _s13p13c.obj _s13p14c.obj _s13p15c.obj _s13p16c.obj _s13p20c.obj _s13p25c.obj _s13p26c.obj _s13p27c.obj _s13p31c.obj _s13p32c.obj _s14p01c.obj _s14p03c.obj _s14p04c.obj _s14p05c.obj _s15p01.obj _s16p01a.obj _s17p06c.obj _s17p24c.obj _s17p35c.obj _s17p36.obj _s17p37.obj _s17p39.obj _s17p40.obj _s17p41.obj _s17p42.obj _s18p01.obj _s18p02.obj _s18p03.obj _s18p05.obj _s18p06.obj _s18p07.obj _s19p11c.obj _s19p14c.obj _s20p01a.obj _s20p05c.obj _s21p01c.obj _s21p07c.obj _s22p07.obj _s22p22c.obj _s24p05c.obj _s24p06c.obj _s26p01.obj _s26p02.obj _s26p03.obj _s26p04a.obj _s26p05.obj _s26p06.obj _s26p07.obj _s27.obj _s27a.obj _s27p03c.obj _s28p02.obj _s28p11a.obj _s28p14c.obj _s28p15.obj _s28p16c.obj _s33p01c.obj _s33p02.obj _s33p03c.obj _s33p04c.obj _s33p05.obj _s33p06.obj _s33p07c.obj _s33p09.obj _s33p10.obj _s33p11.obj _s33p12.obj _s33p13c.obj _s33p14c.obj _s33p16.obj _s33p18c.obj _s33p19c.obj _s33p17.obj _s33p20.obj _s33p21.obj _s33p22c.obj _s33p23c.obj _s33p24c.obj _s33p25c.obj _s33p26c.obj _s33p29.obj _s33p30.obj _s33p31.obj _s33p32a.obj _s33p33.obj _s33p34.obj _s33p35.obj _s33p37.obj _s33p37c.obj _s33p38.obj _s33p38c.obj _s33p39.obj _s33p40.obj _s34p01c.obj _s34p08c.obj _s34p12c.obj _s34p13c.obj _s34p22c.obj _s34p25.obj _s34p26.obj _s34p47.obj _s34p49c.obj _s34p57c.obj _s34p65c.obj _s34p66c.obj _s34p67c.obj _s34p71c.obj _s34p72c.obj _s34p73c.obj _s34p76c.obj _s34p79c.obj _s34p81c.obj _s34p82c.obj _s34p83c.obj _s34p85c.obj
    TLINK @TST_MGCD.RSP
