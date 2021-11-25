.suffixes: .cpp .c .asm

.c.obj:
    BCC -B -c -ml -M -S -v -y $<

.c.obj:
    BCC -B -c -ml -M -S -v -y $<

.asm.obj:
    TASM /c /e /la /m /ml /t /z /zi $<



TST_MGC.EXE: TST_MGC.C TST_MGC.OBJ MGC_DEF.OBJ MOM_DEF.OBJ ST_DBG.OBJ ST_DEF.OBJ ST_EMM.OBJ ST_GUI.OBJ ST_LBX.OBJ ST_SA.OBJ ST_SND.OBJ ST_VGA.OBJ STU_UTIL.OBJ LbxLdEt1.obj _s05p01c.obj _s05p02c.obj _s05p05c.obj _s06p01c.obj _s06p02c.obj _s06p04c.obj _s07p04c.obj _s07p05.obj _s07p06c.obj _s07p07c.obj _s07p08.obj _s07p09.obj _s08p07c.obj _s08p08c.obj _s08p15c.obj _s08p16.obj _s08p19c.obj _s09p01c.obj _s09p02.obj _s09p03.obj _s09p04.obj _s09p05c.obj _s09p06.obj _s09p07.obj _s10p01c.obj _s10p10c.obj _s10p15c.obj _s10p16.obj _s10p17c.obj _s12p01.obj _s12p02c.obj _s12p03c.obj _s12p04c.obj _s12p05.obj _s12p06.obj _s12p07c.obj _s12p08.obj _s13p01c.obj _s13p11c.obj _s13p12c.obj _s13p15c.obj _s13p16c.obj _s13p20c.obj _s13p31c.obj _s13p32c.obj _s14p03c.obj _s14p01c.obj _s15p01.obj _s19p14c.obj _s24p05c.obj _s24p06c.obj
    TLINK @TST_MGC.RSP
