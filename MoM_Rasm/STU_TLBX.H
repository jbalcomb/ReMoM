#ifndef STU_TLBX_H
#define STU_TLBX_H

/*
    SimTex Universe - Test LBX

*/

/*
    MAGIC.EXE
        1st *.LBX Call:  main() |-> Hardware_Init() |-> VGA_DAC_Init() |-> LBXE_LoadSingle() |-> SA_Allocate_MemBlk()
            FONTS.LBX,0
            FONTS.LBX,1
        3rd *.LBX Call:  main() |-> VGA_LoadPalette() |-> LBXE_LoadReplace()
            FONTS.LBX,2
        4th *.LBX Call:  main() |-> GAME_LoadMainImages() |-> EMM_Load_LBX_File_1()
            MAINSCRN.LBX
        5th
            gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
        6th
            gsa_MAINSCRN_5_ScreenBottom = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);
        7th
            gsa_VORTEX_1_MenuContinue    = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);
        8th
            gsa_VORTEX_2_MenuHallOfFame  = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);
        9th
            gsa_VORTEX_3_MenuQuitToDOS   = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
        10th
            gsa_VORTEX_4_MenuNewGame     = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);
        11th
            gsa_VORTEX_5_MenuLoadGame    = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);
        12th
            EMM_Load_LBX_File_1(g_LbxNm_WIZARDS);
        13th-25th
            gsa_WIZARDS_0to13[itr_wizards] = LBXE_LoadSingle(g_LbxNm_WIZARDS, itr_wizards);
            WIZARDS.LBX,{0,...,12}
        26th
            EMM_Load_LBX_File_1(g_LbxNm_SPELLDAT);

*/

/*
    MAINSCRN.LBX
    Entry Index 0
    "MOMBACK", "MOM animated logo"
*/
// sa_FLIC_Header == TST_LBX_MAINSCRN_000.Segment_Address
// fp_FLIC_Header->Width != 320
// fp_FLIC_Header->Height != 41
// fp_FLIC_Header->Current_Frame != 0
// fp_FLIC_Header->Frame_Count != 20
// fp_FLIC_Header->Loop_Frame != 0
// fp_FLIC_Header->EMM_Handle_Number != 4
// fp_FLIC_Header->EMM_Logical_Page_Number != 0
// fp_FLIC_Header->EMM_Logical_Page_Offset != 704
// fp_FLIC_Header->Palette_Header_Offset != 102

/*
    MAINSCRN.LBX
    Entry Index 5
    "CASTLE", "MOM screen buttom"
*/
// sa_FLIC_Header == TST_LBX_MAINSCRN_005.Segment_Address
// fp_FLIC_Header->Width != 320
// fp_FLIC_Header->Height != 159
// fp_FLIC_Header->Current_Frame != 0
// fp_FLIC_Header->Frame_Count != 1
// fp_FLIC_Header->Loop_Frame != 0
// fp_FLIC_Header->EMM_Handle_Number != 4
// fp_FLIC_Header->EMM_Logical_Page_Number != 9
// fp_FLIC_Header->EMM_Logical_Page_Offset != 6416
// fp_FLIC_Header->Palette_Header_Offset != 0

#endif /* STU_TLBX_H */
