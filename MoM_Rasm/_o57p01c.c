// _o57p01c.c LBX_Tables_Init
// MOM_DEF.H

#include "ST_HEAD.H"
#include "ST_EMM.H"
#include "ST_SA.H"
#include "MOM_DEF.H"

/*
    Where do these get used?
    Am I using any of them, presently?

*/
void MoM_Tables_Init(int gfx_buff_nparas)
{
    SAMB_addr tmp_gsa_World_Data;
    int tmp_Free_gsa_World_Data;
    SAMB_ptr tmp_gfp_World_Data;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: MoM_Tables_Init()\n", __FILE__, __LINE__);
#endif

    g_EmmHndl_FIGUREX = EMM_GetHandle(28, EmmHndlNm_FIGUREX, 1);

    g_EmmHndl_TILEXXX = EMM_GetHandle(3, EmmHndlNm_TILEXXX, 1);

    // TODO(JimBalcomb,20220907): move whatever this 3519 is to a manifest constant
    g_World_Data_Paras = 3519;                                          // 3519 paragraphs = 16 * 3519 = 56,304 bytes

    // TODO(JimBalcomb,20220907): move whatever this 515 is to a manifest constant
    gsa_Sandbox = FP_SEG(SA_Allocate_Space(515 + gfx_buff_nparas));     //  515 paragraphs = 16 *  515 =   8,240 bytes
                                                                        // 6100 paragraphs = 16 * 6100 =  97,600 bytes
                                                                        // 6615 paragraphs = 16 * 6615 = 105,840 bytes

    gsa_GUI_SmallWork_IMG = FP_SEG(SA_Allocate_Space(150));             // 150 paragraphs = 16 * 150 = 2,400 bytes

    gsa_Gfx_Swap = FP_SEG(SA_Allocate_Space(635));                      // 635 paragraphs = 16 * 635 = 10,160 bytes

    tmp_gsa_World_Data = FP_SEG(SA_Allocate_Space(g_World_Data_Paras)); // 3519 paragraphs = 16 * 3519 = 56,304 bytes
    // NOTE(JimBalcomb,20220911): gsa_World_Data IS ONLY XREF'd in MoM_Tables_Init, in MAGIC.EXE
    gsa_World_Data = tmp_gsa_World_Data;
    tmp_gfp_World_Data = MK_FP(tmp_gsa_World_Data,0);

    gfp_TBL_Cities = SA_Alloc_First(tmp_gfp_World_Data, 714);           // 714 paragraphs = 16 * 714 = 11,424 bytes

    gfp_TBL_Maps = SA_Alloc_Next(tmp_gfp_World_Data, 602);              // 602 paragraphs = 16 * 602 = 9632 bytes

    gfp_UU_TBL_2 = SA_Alloc_Next(tmp_gfp_World_Data, 14);               // 14 paragraphs = 16 * 14 = 224 bytes
    gfp_UU_TBL_1 = SA_Alloc_Next(tmp_gfp_World_Data, 14);               // 14 paragraphs = 16 * 14 = 224 bytes

    gfp_TBL_Landmasses = SA_Alloc_Next(tmp_gfp_World_Data, 302);        // 302 paragraphs = 16 * 302 = 4832 bytes
    gfp_TBL_Terr_Specials = SA_Alloc_Next(tmp_gfp_World_Data, 302);     // 302 paragraphs = 16 * 302 = 4832 bytes
    gfp_TBL_TerrainFlags = SA_Alloc_Next(tmp_gfp_World_Data, 302);      // 302 paragraphs = 16 * 302 = 4832 bytes
    gfp_TBL_Scouting = SA_Alloc_Next(tmp_gfp_World_Data, 302);          // 302 paragraphs = 16 * 302 = 4832 bytes

    gfp_CRP_Visibility_Arcanus = SA_Alloc_Next(tmp_gfp_World_Data, 19); // 19 paragraphs = 16 * 19 = 304 bytes
    gfp_CRP_Visibility_Myrror = SA_Alloc_Next(tmp_gfp_World_Data, 19);  // 19 paragraphs = 16 * 19 = 304 bytes

    tmp_Free_gsa_World_Data = SA_GetFree(tmp_gfp_World_Data);
    tmp_Free_gsa_World_Data = tmp_Free_gsa_World_Data - 1;  // Minus One? Why? What - byte/segment/paragraph?
    
    // ? g_World_Data_Paras = 3519; 3519 - (714 + 602 + 14 + 14 + 302 + 302 + 302 + 302 + 302 + 19 + 19) = 3519 - 2892 = 627; 627 * 16 = 10,032 bytes
    gsa_World_Data_Extra = FP_SEG(SA_Alloc_Next(tmp_gfp_World_Data, tmp_Free_gsa_World_Data));

    /*
        ? Allocate EMM FP ?
    */
    gfp_CRP_TBL_TempMoveMap_EMM = EMM_EMMDATAH_AllocFirst(632);         //  632 paragraphs = 16 * 632 = 10112 bytes
    gfp_TBL_MoveMaps_EMM = EMM_EMMDATAH_AllocNext(1602);                // 1602 paragraphs = 16 * 1602 = 25632 bytes
    gfp_CRP_TBL_SharedTiles_EMM = EMM_EMMDATAH_AllocNext(301);          //  301 paragraphs = 16 * 301 = 4816 bytes
    gfp_CRP_TBL_Catchments_EMM = EMM_EMMDATAH_AllocNext(301);           //  301 paragraphs = 16 * 301 = 4816 bytes
    gfp_CRP_TBL_OvlMovePathsEMM = EMM_EMMDATAH_AllocNext(1033);         // 1033 paragraphs = 16 * 1033 = 16528 bytes

    /*

    */
    gfp_TBL_HeroStats_P0 = SA_Allocate_Space(28);                       //   28 paragraphs = 16 * 28 = 448 bytes
    gfp_TBL_HeroStats_P1 = SA_Allocate_Space(27);                       //   27 paragraphs = 16 * 27 = 432 bytes
    gfp_TBL_HeroStats_P2 = SA_Allocate_Space(27);                       //   27 paragraphs = 16 * 27 = 432 bytes
    gfp_TBL_HeroStats_P3 = SA_Allocate_Space(27);                       //   27 paragraphs = 16 * 27 = 432 bytes
    gfp_TBL_HeroStats_P4 = SA_Allocate_Space(27);                       //   27 paragraphs = 16 * 27 = 432 bytes
    gfp_TBL_HeroStats_P5 = SA_Allocate_Space(27);                       //   27 paragraphs = 16 * 27 = 432 bytes

    /*

    */
    gfp_TBL_Units = SA_Allocate_Space(2028);                            // 2028 paragraphs = 16 * 2028 = 32448 bytes
    gfp_CRP_Active_Unit = SA_Allocate_Space(8);                         //    8 paragraphs = 16 * 8 = 128 bytes
    gfp_TBL_Nodes = SA_Allocate_Space(92);                              //   92 paragraphs = 16 * 92 = 1472 bytes
    gfp_TBL_Fortresses = SA_Allocate_Space(3);                          //    3 paragraphs = 16 * 3 = 48 bytes
    gfp_TBL_Towers = SA_Allocate_Space(3);                              //    3 paragraphs = 16 * 3 = 48 bytes
    gfp_TBL_Encounters = SA_Allocate_Space(351);                        //  351 paragraphs = 16 * 351 = 5616 bytes
    gfp_TBL_Events = SA_Allocate_Space(7);                              //    7 paragraphs = 16 * 7 = 112 bytes
    gfp_TBL_Hero_Names = SA_Allocate_Space(37);                         //   37 paragraphs = 16 * 37 = 592 bytes
    gfp_TBL_Items = SA_Allocate_Space(433);                             //  433 paragraphs = 16 * 433 = 6928 bytes
    gfp_TBL_Premade_Items = SA_Allocate_Space(17);                      //   17 paragraphs = 16 * 17 = 272 bytes
    gfp_TBL_Spell_Data = SA_Allocate_Space(485);                        //  485 paragraphs = 16 * 485 = 7760 bytes
    gsa_CRP_UnitDraw_WorkArea = FP_SEG(SA_Allocate_Space(60));          //   60 paragraphs = 16 * 60 = 960 bytes
    gsa_CRP_SND_Music_Segment = FP_SEG(SA_Allocate_Space(350));         //  350 paragraphs = 16 * 350 = 5600 bytes


// mov [Wiz5_Spell_00h@], (offset TBL_Wizards.Spells_Known+17E8h)
// mov [Wiz5_Spell_0Ch@], (offset TBL_Wizards.Spells_Known+17F4h)
// mov [Wiz5_Spell_18h@], (offset TBL_Wizards.Spells_Known+1800h)
// mov [Wiz5_Spell_28h@], (offset TBL_Wizards.Spells_Known+1810h)
// mov [Wiz5_Spell_3Ch@], (offset TBL_Wizards.Spells_Known+1824h)
// mov [Wiz5_Spell_50h@], (offset TBL_Wizards.Spells_Known+1838h)
// mov [Wiz5_Spell_64h@], (offset TBL_Wizards.Spells_Known+184Ch)
// mov [Wiz5_Spell_78h@], (offset TBL_Wizards.Spells_Known+1860h)
// mov [Wiz5_Spell_8Ch@], (offset TBL_Wizards.Spells_Known+1874h)
// mov [Wiz5_Spell_A0h@], (offset TBL_Wizards.Spells_Known+1888h)
// mov [Wiz5_Spell_B4h@], (offset TBL_Wizards.Spells_Known+189Ch)
// mov [Wiz5_Spell_C8h@], (offset TBL_Wizards.Spells_Known+18B0h)
// mov [Wiz5_Spell_D4h@], (offset TBL_Wizards.Spells_Known+18BCh)
// mov [Wiz5_Spell_E0h@], (offset TBL_Wizards.Spells_Known+18C8h)

    g_EmmHndl_CONTXXX = EMM_GetHandle(4, EmmHndlNm_CONTXXX, 1);

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: MoM_Tables_Init()\n", __FILE__, __LINE__);
#endif

}
