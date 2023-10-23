
#include "MoX.H"
#include "STU_DBG.H"

/*
    MGC o57p01 (1of1)
    WZD o152p01 (1of1)
*/
// AKA MoM_Tables_Init()
// Allocate_Data_Space()
void Allocate_Data_Space(int16_t gfx_buff_nparas)
{
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MoM_Tables_Init()\n", __FILE__, __LINE__);
#endif


// TODO  EmmHndl_FIGUREX = EMM_GetHandle(28, EmmHndlNm_FIGUREX, 1);  // Page_Count, Handle_Name, Reserved_Flag
// TODO  EmmHndl_TILEXXX = EMM_GetHandle(3, EmmHndlNm_TILEXXX, 1);  // Page_Count, Handle_Name, Reserved_Flag

    // MGC: 6100
    // WZD: 4600
    // 4600 * 16 = 73600 bytes
    //  515 * 16 = 8240 bytes
    // 73600 + 8240 = 81840 bytes
    _screen_seg = Allocate_Space(gfx_buff_nparas + 515);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: _screen_seg: %p\n", __FILE__, __LINE__, _screen_seg);
#endif

    // 150 * 16 = 2400 bytes
    // TODO  GUI_SmallWork_IMG = Allocate_Space(150);
    // ; 96h LBX_Alloc_Space paragraphs
    // ; used for building GUI notification images (although
    // ; the pointer variable is also used for building combat
    // ; figure images, after which it is reassigned)


// World_Data
// ¿ everything from a SAVE_GAM ? also, COMBAT.TMP ? 

    // 635 paragraphs = 16 * 635 = 10,160 bytes
    // GFX_Swap_Seg = Allocate_Space(635);
    GFX_Swap_Seg = Allocate_Space(4092);
    // used to load all 198 StatFigs from UNIT1.LBX and UNITS2.LBX 
    /// lots of XREFs from City, Cmb, SplBk, ...

    // tmp_World_Data_Paras = 3519;
    tmp_World_Data_Paras = 3519;

    // 3519 paragraphs = 16 * 3519 = 56,304 bytes
    World_Data = Allocate_Space(tmp_World_Data_Paras);

    // also used for COMBAT.TMP
    // TODO figure out if/how there is a relationship between this and the check for file size 57764 in Load_SAVE_GAM()

    // 714 paragraphs = 16 * 714 = 11,424 bytes
    _CITIES = (struct s_CITY *)Allocate_First_Block(World_Data, 714);   // 714 Paragraphs, 11424 Bytes
#ifdef STU_DEBUG
    DBG_ORIG__CITIES = _CITIES;
#endif
    _world_maps = (uint8_t *)Allocate_Next_Block(World_Data, 602);         // 602 Paragraphs, 9632 Bytes
    DBG_ORIG__world_maps = _world_maps;
    UU_TBL_1 = Allocate_Next_Block(World_Data, 14);             // 14 Paragraphs, 224 Bytes
    DBG_ORIG_UU_TBL_1 =  UU_TBL_1;
    UU_TBL_2 = Allocate_Next_Block(World_Data, 14);             // 14 Paragraphs, 224 Bytes
    DBG_ORIG_UU_TBL_2 =  UU_TBL_2;

    TBL_Landmasses = (uint8_t *)Allocate_Next_Block(World_Data, 302);      // 302 Paragraphs, 4832 Bytes
    DBG_ORIG_TBL_Landmasses = TBL_Landmasses;
    TBL_Terr_Specials = (uint8_t *)Allocate_Next_Block(World_Data, 302);   // 302 Paragraphs, 4832 Bytes
    DBG_ORIG_TBL_Terr_Specials = TBL_Terr_Specials;
    TBL_Terrain_Flags = (uint8_t *)Allocate_Next_Block(World_Data, 302);   // 302 Paragraphs, 4832 Bytes
    DBG_ORIG_TBL_Terrain_Flags = TBL_Terrain_Flags;
    TBL_Scouting = (uint8_t *)Allocate_Next_Block(World_Data, 302);        // 302 Paragraphs, 4832 Bytes
    DBG_ORIG_TBL_Scouting = TBL_Scouting;

    square_scouted_p0 = (uint8_t *)Allocate_Next_Block(World_Data, 19);   // 19 Paragraphs, 304 Bytes
    square_scouted_p1 = (uint8_t *)Allocate_Next_Block(World_Data, 19);    // 19 Paragraphs, 304 Bytes

    World_Data_Extra = Allocate_Next_Block(World_Data, Get_Free_Blocks(World_Data) - 1);

/*
929 = (3519 - 714 - 602 - 14 - 14 - 302 - 302 - 302 - 302 - 19 - 19)
2590 = 714 + 602 + 14+14 + 302+302+302+302 + 19+19
929 = (3519 - 2590)

1174 - 929 = 245
     1  Allocate_First_Block()
    10  Allocate_Next_Block()

SA_GET_SIZE(SAMB_head): 3519
SA_GET_USED(SAMB_head): 2345
3519 - 2345 = 1174

BEGIN: Allocate_First_Block( SAMB_head = 000001B4252A35EC, size = 714 )
SA_GET_SIZE(SAMB_head): 3519
SA_GET_USED(SAMB_head): 716

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 602 )
SA_GET_USED(SAMB_head): 716
SA_GET_USED(SAMB_head): 1063

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 14 )
SA_GET_USED(SAMB_head): 1063
SA_GET_USED(SAMB_head): 1078

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 14 )
SA_GET_USED(SAMB_head): 1078
SA_GET_USED(SAMB_head): 1093

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 302 )
SA_GET_USED(SAMB_head): 1093
SA_GET_USED(SAMB_head): 1396

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 302 )
SA_GET_USED(SAMB_head): 1396
SA_GET_USED(SAMB_head): 1699

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 302 )
SA_GET_USED(SAMB_head): 1699
SA_GET_USED(SAMB_head): 2258

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 302 )
SA_GET_USED(SAMB_head): 2258
SA_GET_USED(SAMB_head): 2305

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 19 )
SA_GET_USED(SAMB_head): 2305
SA_GET_USED(SAMB_head): 2325

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 19 )
SA_GET_USED(SAMB_head): 2325
SA_GET_USED(SAMB_head): 2345

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 1174 )
SA_GET_USED(SAMB_head): 2345

*/


    /*
    
    */
    // mov     ax, 632                         ; rewritten in the overland Djikstra patch
    // call    EMM_EMMDATAH_AllocFirst         ; clears the EMM Data block, then creates an LBX
    // mov     [word ptr TBL_TempMoveMap_EMS], ax ; 278h EMMData paragraphs
    // 
    // mov     ax, 1802
    // call    EMM_EMMDATAH_AllocNext          ; creates an LBX allocation into the EMM Data block
    // mov     [word ptr TBL_MoveMaps_EMS], ax ; 70Ah EMMData paragraphs
    // 
    // mov     ax, 301
    // call    EMM_EMMDATAH_AllocNext          ; creates an LBX allocation into the EMM Data block
    // mov     [word ptr TBL_SharedTiles_EMS@], ax ; bitflag table of the map tiles that are in the
    // 
    // mov     ax, 301
    // call    EMM_EMMDATAH_AllocNext          ; creates an LBX allocation into the EMM Data block
    // mov     [word ptr TBL_Catchments_EMS@], ax ; bitflag table of the map tiles that are in the
    // 
    // mov     ax, 1033
    // call    EMM_EMMDATAH_AllocNext          ; creates an LBX allocation into the EMM Data block
    // mov     [word ptr TBL_OvlMovePathsEMS@], ax ; 409h EMMData paragraphs

    TBL_TempMoveMap_EMS = Allocate_Space(632);
    TBL_MoveMaps_EMS = Allocate_Space(1802);  // 1802 PR, 28832 B
    DBG_ORIG_TBL_MoveMaps_EMS = TBL_MoveMaps_EMS;
    TBL_SharedTiles_EMS = Allocate_Space(301);
    TBL_Catchments_EMS = Allocate_Space(301);
    TBL_OvlMovePaths_EMS = Allocate_Space(1033);

    /*
    
    */



    p0_heroes = Allocate_Space(28);  // 28 paragraphs = 448 bytes
    DBG_ORIG_p0_heroes = p0_heroes;
    p1_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    DBG_ORIG_p1_heroes = p1_heroes;
    p2_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    DBG_ORIG_p2_heroes = p2_heroes;
    p3_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    DBG_ORIG_p3_heroes = p3_heroes;
    p4_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    DBG_ORIG_p4_heroes = p4_heroes;
    p5_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    DBG_ORIG_p5_heroes = p5_heroes;

    p_heroes[0] = (struct s_HERO *)p0_heroes;
    p_heroes[1] = (struct s_HERO *)p1_heroes;
    p_heroes[2] = (struct s_HERO *)p2_heroes;
    p_heroes[3] = (struct s_HERO *)p3_heroes;
    p_heroes[4] = (struct s_HERO *)p4_heroes;
    p_heroes[5] = (struct s_HERO *)p5_heroes;



    _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 paragraphs = 32448 bytes
    DBG_ORIG__UNITS = _UNITS;

    Active_Unit = (struct s_BU_REC *)Allocate_Space(8);  // 8 paragraphs = 128 bytes

    TBL_Nodes = (struct s_NODE *)Allocate_Space(92);  // 92 PR = 1472 B;  actual: 30 * sizeof(struct s_NODE) = 30 * 48 = 1440 B
    DBG_ORIG_TBL_Nodes = TBL_Nodes;
    _FORTRESSES = (struct s_FORTRESS *)Allocate_Space(3);  // 3 PR = 48 B;  actual: 6 * sizeof(struct s_FORTRESS) = 24
    DBG_ORIG__FORTRESSES = _FORTRESSES;
    _TOWERS = (struct s_TOWER *)Allocate_Space(3);  // 3 paragraphs = 48 bytes
    DBG_ORIG__TOWERS = _TOWERS;
    _LAIRS = (struct s_LAIR *)Allocate_Space(351);  // 351 paragraphs = 5616 bytes
    DBG_ORIG__LAIRS = _LAIRS;
    _events_table = (uint8_t *)Allocate_Space(7);  // 7 paragraphs = 112 bytes
    DBG_ORIG__events_table = _events_table;
    TBL_Hero_Names = Allocate_Space(37);
    DBG_ORIG_TBL_Hero_Names = TBL_Hero_Names;
    TBL_Items = (struct s_ITEM *)Allocate_Space(433);
    DBG_ORIG_TBL_Items = TBL_Items;
    TBL_Premade_Items = Allocate_Space(17);
    DBG_ORIG_TBL_Premade_Items = TBL_Premade_Items;
    
    // AKA TBL_Spell_Data
    spell_data_table = Allocate_Space(485);  // 485 PR  7760 B; actual: 215 * 36 = 7740

    UnitDraw_WorkArea = Allocate_Space(60);  // 60 paragraphs = 960 bytes
    
    // SND_Music_Segment = Allocate_Space(350);


// ?
// mov     [AI_Arc_MainWarConts@], (offset TBL_Wizards.Spells_Known+17E8h)
// mov     [AI_Myr_MainWarConts@], (offset TBL_Wizards.Spells_Known+17F4h)
// mov     [AI_CONTX_Reevals@], (offset TBL_Wizards.Spells_Known+1800h)
// mov     [Wiz5_Spell_28h@], (offset TBL_Wizards.Spells_Known+1810h)
// mov     [Wiz5_Spell_3Ch@], (offset TBL_Wizards.Spells_Known+1824h)
// mov     [Wiz5_Spell_50h@], (offset TBL_Wizards.Spells_Known+1838h)
// mov     [Wiz5_Spell_64h@], (offset TBL_Wizards.Spells_Known+184Ch)
// mov     [AI_Arc_NewColConts@], (offset TBL_Wizards.Spells_Known+1860h)
// mov     [AI_Myr_NewColConts@], (offset TBL_Wizards.Spells_Known+1874h)
// mov     [AI_Arc_NewColTgtXs@], (offset TBL_Wizards.Spells_Known+1888h)
// mov     [AI_Myr_NewColTgtXs@], (offset TBL_Wizards.Spells_Known+189Ch)
// mov     [AI_Arc_NewColTgtYs@], (offset TBL_Wizards.Spells_Known+18B0h)
// mov     [AI_Myr_NewColTgtYs@], (offset TBL_Wizards.Spells_Known+18BCh)
// mov     [AI_SCircle_Reevals@], (offset TBL_Wizards.Spells_Known+18C8h)
// ?

    // EMM_ContXXX_H = EMM_GetHandle(4, cnst_EMM_ContH_Name, 1)


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: MoM_Tables_Init()\n", __FILE__, __LINE__);
#endif
}
