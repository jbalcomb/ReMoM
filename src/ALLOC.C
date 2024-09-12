
#include "MoX.H"

/*
    MGC o57p01 (1of1)
    WZD o152p01 (1of1)
*/
// AKA MoM_Tables_Init()
// Allocate_Data_Space()
void Allocate_Data_Space(int16_t gfx_buff_nparas)
{
    
// TODO  EmmHndl_FIGUREX = EMM_GetHandle(28, EmmHndlNm_FIGUREX, 1);  // Page_Count, Handle_Name, Reserved_Flag
// TODO  EmmHndl_TILEXXX = EMM_GetHandle(3, EmmHndlNm_TILEXXX, 1);  // Page_Count, Handle_Name, Reserved_Flag

    // MGC: 6100
    // WZD: 4600
    // 4600 * 16 = 73600 bytes
    //  515 * 16 = 8240 bytes
    // 73600 + 8240 = 81840 bytes
    _screen_seg = Allocate_Space(gfx_buff_nparas + 515);

    GfxBuf_2400B = Allocate_Space(150);  // 150 PR  2400 B

// World_Data
// ¿ everything from a SAVE_GAM ? also, COMBAT.TMP ? 

    // 635 paragraphs = 16 * 635 = 10,160 bytes
    // GFX_Swap_Seg = Allocate_Space(635);
    // GFX_Swap_Seg = Allocate_Space(4092);
    // GFX_Swap_Seg = Allocate_Space(4092);
    // GFX_Swap_Seg = Allocate_Space((65536 * 3));
    GFX_Swap_Seg = Allocate_Space(65535);
    /*
        1024 * 1024 = 1048576
        1048576 / 16 = 65536
        Min EMS is 2700 KB
    */
    // used to load all 198 StatFigs from UNIT1.LBX and UNITS2.LBX 
    /// lots of XREFs from City, Cmb, SplBk, ...

    // tmp_World_Data_Paras = 3519;
    tmp_World_Data_Paras = 3519;

    // 3519 paragraphs = 16 * 3519 = 56,304 bytes
    World_Data = Allocate_Space(tmp_World_Data_Paras);

    // also used for COMBAT.TMP
    // TODO figure out if/how there is a relationship between this and the check for file size 57764 in Load_SAVE_GAM()

    // 714 paragraphs = 16 * 714 = 11,424 bytes
    // _CITIES = (struct s_CITY *)Allocate_First_Block(World_Data, 714);   // 714 Paragraphs, 11424 Bytes
    _CITIES = (struct s_CITY*)Allocate_First_Block(World_Data, 714);

    _world_maps = (uint8_t *)Allocate_Next_Block(World_Data, 602);         // 602 Paragraphs, 9632 Bytes

    UU_TBL_1 = Allocate_Next_Block(World_Data, 14);             // 14 Paragraphs, 224 Bytes

    UU_TBL_2 = Allocate_Next_Block(World_Data, 14);             // 14 Paragraphs, 224 Bytes


    TBL_Landmasses = (uint8_t *)Allocate_Next_Block(World_Data, 302);      // 302 Paragraphs, 4832 Bytes

    TBL_Terr_Specials = (uint8_t *)Allocate_Next_Block(World_Data, 302);   // 302 Paragraphs, 4832 Bytes

    _map_square_flags = (uint8_t *)Allocate_Next_Block(World_Data, 302);   // 302 Paragraphs, 4832 Bytes

    TBL_Scouting = (uint8_t *)Allocate_Next_Block(World_Data, 302);        // 302 Paragraphs, 4832 Bytes


    square_scouted_p0 = (uint8_t *)Allocate_Next_Block(World_Data, 19);    // 19 Paragraphs, 304 Bytes  ¿ (((2400 / 8) + 1) + 1) / 16) ?
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

only usage of EMM_EMMDATAH_AllocFirst() / EMM_EMMDATAH_AllocNext()

632 + 1802 + 301 + 301 + 1033 = 4069 * 16 = 65,104  (65536 - 65104 = 432 / 16 = 27)

movepath_cost_map        = SA_MK_FP0( EMM_EMMDATAH_AllocFirst( 632 ) );  //  632 PR  10112 B
movement_mode_cost_maps  = SA_MK_FP0( EMM_EMMDATAH_AllocNext( 1802 ) );  // 1802 PR  28832 B
city_area_shared_bits    = SA_MK_FP0( EMM_EMMDATAH_AllocNext(  301 ) );  //  301 PR   4816 B
city_area_bits           = SA_MK_FP0( EMM_EMMDATAH_AllocNext(  301 ) );  //  301 PR   4816 B
TBL_OvlMovePathsEMS      = SA_MK_FP0( EMM_EMMDATAH_AllocNext( 1033 ) );  // 1033 PR  16528 B

city_area_bits & city_area_shared_bits
¿ ~== TBL_Scouting or square_scouted_p0/p1 ?
301 * 16 = 4816  ¿ 301 would be the byte count needed for one worlds worth of bits, +1 to cover the paragraph boundary, like with square_scouted_p0/p1 ?
60 * 40 = 2400 / 16 = 150 * 2 = 300

in Reset_City_Area_Bitfields()
it loops over 300 for city_area_bits and city_area_shared_bits
so, 300 PRs, + 1 for the SAMB header

*/

    movepath_cost_map = (struct s_MOVE_PATH *)Allocate_Space(632);
    movement_mode_cost_maps = (struct s_MOVE_MODE_COST_MAPS *)Allocate_Space(1802);

    city_area_shared_bits = (uint8_t *)Allocate_Space(WORLD_MAP_BITFIELD_SIZE + 1);
    city_area_bits = (uint8_t *)Allocate_Space(WORLD_MAP_BITFIELD_SIZE + 1);

    TBL_OvlMovePaths_EMS = Allocate_Space(1033);


    /*
    
    */
// DELETE      p0_heroes = Allocate_Space(28);  // 28 paragraphs = 448 bytes
// DELETE      p1_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
// DELETE      p2_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
// DELETE      p3_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
// DELETE      p4_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
// DELETE      p5_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
// DELETE  
// DELETE      p_heroes[0] = (struct s_HERO *)p0_heroes;
// DELETE      p_heroes[1] = (struct s_HERO *)p1_heroes;
// DELETE      p_heroes[2] = (struct s_HERO *)p2_heroes;
// DELETE      p_heroes[3] = (struct s_HERO *)p3_heroes;
// DELETE      p_heroes[4] = (struct s_HERO *)p4_heroes;
// DELETE      p_heroes[5] = (struct s_HERO *)p5_heroes;
// DELETE  
// DELETE      _HEROES = (struct s_HERO **)p0_heroes;
// DELETE      _HEROES2 = p0_heroes;

    _HEROES2[0] = (struct s_HEROES *)Allocate_Space(28);  // 28 paragraphs = 448 bytes
    _HEROES2[1] = (struct s_HEROES *)Allocate_Space(27);  // 27 paragraphs = 432 bytes
    _HEROES2[2] = (struct s_HEROES *)Allocate_Space(27);  // 27 paragraphs = 432 bytes
    _HEROES2[3] = (struct s_HEROES *)Allocate_Space(27);  // 27 paragraphs = 432 bytes
    _HEROES2[4] = (struct s_HEROES *)Allocate_Space(27);  // 27 paragraphs = 432 bytes
    _HEROES2[5] = (struct s_HEROES *)Allocate_Space(27);  // 27 paragraphs = 432 bytes


    _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 paragraphs = 32448 bytes

    // MoO2  global_combat_data
    global_battle_unit = (struct s_BATTLE_UNIT *)Allocate_Space(8);  // 8 paragraphs = 128 bytes

    _NODES = (struct s_NODE *)Allocate_Space(92);  // 92 PR = 1472 B;  actual: 30 * sizeof(struct s_NODE) = 30 * 48 = 1440 B

    _FORTRESSES = (struct s_FORTRESS *)Allocate_Space(3);

    _TOWERS = (struct s_TOWER *)Allocate_Space(3);  // 3 paragraphs = 48 bytes

    _LAIRS = (struct s_LAIR *)Allocate_Space(351);  // 351 paragraphs = 5616 bytes

    // events_table = (struct s_EVENT_DATA *)Allocate_Space(7);  // 7 paragraphs = 112 bytes
    events_table = (struct s_EVENT_DATA *)Allocate_Space(7);

    // ¿ MoO2  _officer_names ? ¿ Officer vs. Owned Officer ?
    hero_names_table = (struct s_INACTV_HERO *)Allocate_Space(37);  // 37 PR  592 B  ... ~ (36) 16-byte structs

    _ITEMS = (struct s_ITEM *)Allocate_Space(433);  // 433 PR  6928 B;  SAVE.GAM: 138 * 50 = 6900  ... 1 + ((138 * 50) + (SZ_PARAGRAPH - 1))

    TBL_Premade_Items = Allocate_Space(17);
    
    // AKA TBL_Spell_Data
    spell_data_table = (struct s_SPELL_DATA *)Allocate_Space(485);  // 485 PR  7760 B; actual: 215 * 36 = 7740

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

}
