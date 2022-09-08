// _o57p01c.c LBX_Tables_Init
// MOM_DEF.H

#include "ST_EMM.H"
#include "ST_SA.H"
#include "MOM_DEF.H"

void MoM_Tables_Init(int gfx_buff_nparas)
{
    unsigned int tmp_gsa_World_Data;
    int tmp_Free_gsa_World_Data;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: MoM_Tables_Init()\n", __FILE__, __LINE__);
#endif

    g_EmmHndl_FIGUREX = EMM_GetHandle(28, EmmHndlNm_FIGUREX, 1);

    g_EmmHndl_TILEXXX = EMM_GetHandle(3, EmmHndlNm_TILEXXX, 1);

    // TODO(JimBalcomb,20220907): move whatever this 3519 is to a manifest constant
    g_World_Data_Paras = 3519;                                  // 3519 paragraphs = 16 * 3519 = 56,304 bytes

    // TODO(JimBalcomb,20220907): move whatever this 515 is to a manifest constant
    gsa_Sandbox = FP_SEG(SA_Allocate_Space(515 + gfx_buff_nparas));     //  515 paragraphs = 16 *  515 =   8,240 bytes
                                                                // 6100 paragraphs = 16 * 6100 =  97,600 bytes
                                                                // 6615 paragraphs = 16 * 6615 = 105,840 bytes

    gsa_GUI_SmallWork_IMG = FP_SEG(SA_Allocate_Space(150));             // 150 paragraphs = 16 * 150 = 2,400 bytes

    gsa_Gfx_Swap = FP_SEG(SA_Allocate_Space(635));                      // 635 paragraphs = 16 * 635 = 10,160 bytes

    tmp_gsa_World_Data = FP_SEG(SA_Allocate_Space(g_World_Data_Paras)); // 3519 paragraphs = 16 * 3519 = 56,304 bytes
    gsa_World_Data = tmp_gsa_World_Data;
    
    gfp_TBL_Cities = (void far *) SA_Alloc_First(MK_FP(tmp_gsa_World_Data,0), 714);   // 714 paragraphs = 16 * 714 = 11,424 bytes

    gfp_TBL_Maps = (void far *) SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), 602);      // 602 paragraphs = 16 * 602 = 9632 bytes

    gfp_UU_TBL_2 = (void far *) SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), 14);      // 14 paragraphs = 16 * 14 = 224 bytes
    gfp_UU_TBL_1 = (void far *) SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), 14);      // 14 paragraphs = 16 * 14 = 224 bytes

    gfp_TBL_Landmasses = (void far *) SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), 302);      // 302 paragraphs = 16 * 302 = 4832 bytes
    gfp_TBL_Terr_Specials = (void far *) SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), 302);      // 302 paragraphs = 16 * 302 = 4832 bytes
    gfp_TBL_TerrainFlags = (void far *) SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), 302);      // 302 paragraphs = 16 * 302 = 4832 bytes
    gfp_TBL_Scouting = (void far *) SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), 302);      // 302 paragraphs = 16 * 302 = 4832 bytes

    gfp_CRP_Visibility_Arcanus = (void far *) SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), 19);      // 19 paragraphs = 16 * 19 = 304 bytes
    gfp_CRP_Visibility_Myrror = (void far *) SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), 19);      // 19 paragraphs = 16 * 19 = 304 bytes

    tmp_Free_gsa_World_Data = SA_GetFree(MK_FP(tmp_gsa_World_Data,0));
    tmp_Free_gsa_World_Data = tmp_Free_gsa_World_Data - 1;
    
    gsa_World_Data_Extra = FP_SEG(SA_Alloc_Next(MK_FP(tmp_gsa_World_Data,0), tmp_Free_gsa_World_Data));

    /*
        ? Allocate EMM FP ?
    */
    gfp_CRP_TBL_TempMoveMap_EMM = (void far *) EMM_EMMDATAH_AllocFirst(632);      // 632 paragraphs = 16 * 632 = 10112 bytes

    gfp_TBL_MoveMaps_EMM = (void far *) EMM_EMMDATAH_AllocNext(1602);      // 1602 paragraphs = 16 * 1602 = 25632 bytes

    gfp_CRP_TBL_SharedTiles_EMM = (void far *) EMM_EMMDATAH_AllocNext(301);      // 301 paragraphs = 16 * 301 = 4816 bytes
    gfp_CRP_TBL_Catchments_EMM = (void far *) EMM_EMMDATAH_AllocNext(301);      // 301 paragraphs = 16 * 301 = 4816 bytes

    gfp_CRP_TBL_OvlMovePathsEMM = (void far *) EMM_EMMDATAH_AllocNext(1033);      // 1033 paragraphs = 16 * 1033 = 16528 bytes

    /*

    */
    gfp_TBL_HeroStats_P0 = (void far *) SA_Allocate_Space(28);      // 28 paragraphs = 16 * 28 = 448 bytes
    gfp_TBL_HeroStats_P1 = (void far *) SA_Allocate_Space(27);      // 27 paragraphs = 16 * 27 = 432 bytes
    gfp_TBL_HeroStats_P2 = (void far *) SA_Allocate_Space(27);      // 27 paragraphs = 16 * 27 = 432 bytes
    gfp_TBL_HeroStats_P3 = (void far *) SA_Allocate_Space(27);      // 27 paragraphs = 16 * 27 = 432 bytes
    gfp_TBL_HeroStats_P4 = (void far *) SA_Allocate_Space(27);      // 27 paragraphs = 16 * 27 = 432 bytes
    gfp_TBL_HeroStats_P5 = (void far *) SA_Allocate_Space(27);      // 27 paragraphs = 16 * 27 = 432 bytes

    /*

    */

    gfp_TBL_Units = (void far *) SA_Allocate_Space(2028);        // 2028 paragraphs = 16 * 2028 = 32448 bytes

    gfp_CRP_Active_Unit = (void far *) SA_Allocate_Space(8);     // 8 paragraphs = 16 * 8 = 128 bytes

    gfp_TBL_Nodes = (void far *) SA_Allocate_Space(92);          // 92 paragraphs = 16 * 92 = 1472 bytes

    gfp_TBL_Fortresses = (void far *) SA_Allocate_Space(3);      // 3 paragraphs = 16 * 3 = 48 bytes

    gfp_TBL_Towers = (void far *) SA_Allocate_Space(3);          // 3 paragraphs = 16 * 3 = 48 bytes

    gfp_TBL_Encounters = (void far *) SA_Allocate_Space(351);    // 351 paragraphs = 16 * 351 = 5616 bytes

    gfp_TBL_Events = (void far *) SA_Allocate_Space(7);          // 7 paragraphs = 16 * 7 = 112 bytes

    gfp_TBL_Hero_Names = (void far *) SA_Allocate_Space(37);     // 37 paragraphs = 16 * 37 = 592 bytes

    gfp_TBL_Items = (void far *) SA_Allocate_Space(433);         // 433 paragraphs = 16 * 433 = 6928 bytes

    gfp_TBL_Premade_Items = (void far *) SA_Allocate_Space(17);  // 17 paragraphs = 16 * 17 = 272 bytes

    gfp_TBL_Spell_Data = (void far *) SA_Allocate_Space(485);    // 485 paragraphs = 16 * 485 = 7760 bytes

    gsa_CRP_UnitDraw_WorkArea = FP_SEG(SA_Allocate_Space(60));      // 60 paragraphs = 16 * 60 = 960 bytes

    gsa_CRP_SND_Music_Segment = FP_SEG(SA_Allocate_Space(350));     // 350 paragraphs = 16 * 350 = 5600 bytes

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: MoM_Tables_Init()\n", __FILE__, __LINE__);
#endif

}

// mov ax, 3Ch
// push ax
// call 
// mov [@], ax

// mov ax, 15Eh
// push ax
// call LBX_Alloc_Space
// mov [], ax

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
// mov ax, 1
// push ax
// mov ax, offset cnst_EMM_ContH_Name
// push ax
// mov ax, 4
// push ax
// call EMM_CreateHandle
// add sp, 6
// mov [EMM_ContXXX_H], ax
