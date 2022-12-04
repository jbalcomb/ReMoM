.suffixes: .cpp .c .asm

.c.obj:
    BCC -B -c -D__DOS16__=1 -IMAINMENU -ISTU -ml -Tm2 $<
#    BCC -B -c -ml -M -S -Tm2 -v -y $<
#    BCC -B -c -ml -M -S -Tm2 -UDEBUG -USTU_DEBUG -UTEST -USTU_TEST -U__VSCODE__ -v -y $<
#    BCC -B -c -DSTU_DEBUG=1 -D__DOS16__=1 -ml -M -S -Tm2 -U__VSCODE__ -v -y $<

.cpp.obj:
    BCC -B -c -D__DOS16__=1 -IMAINMENU -ISTU -ml -Tm2 $<
#    BCC -B -c -ml -M -S -Tm2 -v -y $<
#    BCC -B -c -ml -M -S -Tm2 -UDEBUG -USTU_DEBUG -UTEST -USTU_TEST -U__VSCODE__ -v -y $<
#    BCC -B -c -DSTU_DEBUG=1 -D__DOS16__=1 -ml -M -S -U__VSCODE__ -v -y $<

.asm.obj:
    TASM /m2 /ml /t $<
#    TASM /c /e /la /m2 /ml /t /z /zi $<
#    TASM /c /dSTU_DEBUG=1 /d__DOS16__=1 /e /la /m2 /ml /t /z /zi $<



WZD.EXE: WZD.OBJ WZD_main.OBJ WZD_scrn.OBJ WZD_DEF.OBJ MOM_DEF.OBJ ST_DEF.OBJ .\MAINMENU\MAINMENU.OBJ ST_CRSR.OBJ ST_CTRL.OBJ ST_DBG.OBJ ST_EMM.OBJ ST_EXIT.OBJ ST_FLIC.OBJ ST_GUI.OBJ ST_HLP.OBJ ST_LBX.OBJ ST_RND.OBJ ST_SA.OBJ ST_SCRN.OBJ ST_SND.OBJ ST_SND.OBJ ST_TXT.OBJ ST_QUIT.OBJ ST_VGA.OBJ MoX_DBG.OBJ MoX_EXIT.OBJ MoX_LBX.OBJ MoX_SA.OBJ dos_DIR.OBJ _o57p01c.obj _s02p01.obj _s02p03.obj _s02p05.obj seg014.obj _s15p01.obj _s16p01a.obj _s17p06c.obj _s17p24c.obj _s17p35c.obj _s17p36.obj _s17p37.obj _s17p39.obj _s17p40.obj _s17p41.obj _s17p42.obj _s18p01.obj _s18p02.obj _s18p03.obj _s18p05.obj _s18p06.obj _s18p07.obj seg019.obj seg020.obj seg021.obj seg022.obj seg024.obj seg026.obj _s26p06.obj _s26p07.obj _s27p02.obj seg028.obj _s33p01c.obj _s33p02.obj _s33p03c.obj _s33p04c.obj _s33p05.obj _s33p06.obj _s33p07c.obj _s33p09.obj _s33p10a.obj _s33p11a.obj _s33p12.obj _s33p13c.obj _s33p14c.obj _s33p16.obj _s33p18c.obj _s33p19c.obj _s33p17.obj _s33p20.obj _s33p21.obj _s33p22c.obj _s33p23c.obj _s33p24c.obj _s33p25c.obj _s33p26c.obj _s33p29.obj _s33p30.obj _s33p31.obj _s33p32a.obj _s33p33.obj _s33p34.obj _s33p35.obj _s33p37.obj _s33p37c.obj _s33p38.obj _s33p38c.obj _s33p39.obj _s33p40.obj _s34p01c.obj _s34p08c.obj _s34p12c.obj _s34p13c.obj _s34p22c.obj _s34p25.obj _s34p26.obj _s34p47.obj _s34p49c.obj _s34p57c.obj _s34p65c.obj _s34p66c.obj _s34p67c.obj _s34p71c.obj _s34p72c.obj _s34p73c.obj _s34p76c.obj _s34p79c.obj _s34p81c.obj _s34p82c.obj _s34p83c.obj _s34p85c.obj
    TLINK @MK_WZD.RSP
