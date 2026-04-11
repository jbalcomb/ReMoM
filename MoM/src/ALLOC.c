/*
    MAGIC.EXE
        ovr057
    WIZARDS.EXE
        ovr152
*/

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */

#include "Combat.h"



// WZD dseg:7056                                                 BEGIN:  ovr152 - Initialized Data

// WZD dseg:7056
char EmmHndlNm_FIGUREX[] = "FIGUREX";
// WZD dseg:705E
char EmmHndlNm_TILEXXX[] = "TILEXXX";
// WZD dseg:7066
char EmmHndlNm_CONTXXX[] = "CONTXXX";

// WZD dseg:7066                                                 END:  ovr152 - Initialized Data



/*
    MGC o57p01 (1of1)
    WZD o152p01 (1of1)
    MoO2  Module: allocate
*/
// AKA MoM_Tables_Init()
// Allocate_Data_Space()
void Allocate_Data_Space(int16_t gfx_buff_nparas)
{
    
    // TODO  EmmHndl_FIGUREX = EMM_GetHandle(28, EmmHndlNm_FIGUREX, 1);
    // TODO  EmmHndl_TILEXXX = EMM_GetHandle( 3, EmmHndlNm_TILEXXX, 1);
    EmmHndl_FIGUREX = Allocate_Space(28672);  // 28 * 16384 EMM Page Size = 458752 B / 16 = 28672 PR
    EmmHndl_TILEXXX = Allocate_Space(3072);   //  3 * 16384 EMM Page Size =  49152 B / 16 =  3072 PR

    // MGC: 6100
    // WZD: 4600
    // 4600 * 16 = 73600 bytes
    //  515 * 16 = 8240 bytes
    // 73600 + 8240 = 81840 bytes
    _screen_seg = Allocate_Space(gfx_buff_nparas + 515);

    // lair_confirm_pict: 41 x 43 = 1763
    // ¿ MoO2  Module: MOX  data (0 bytes) _global_bitmap_seg  Address: 02:0018B2D0 ?
    GfxBuf_2400B = Allocate_Space(150);  // 150 PR  2400 B  ~ 48 x 48 = 2304 B w/ 6 headers ... 50 x 48 = 2400

// World_Data
// ¿ everything from a SAVE_GAM ? also, COMBAT.TMP ? 

    // 635 PR, 10160 B  / 198 = 51.3131 bytes per STATFIG pict?
    // GFX_Swap_Seg = Allocate_Space(635);
    // GFX_Swap_Seg = Allocate_Space(4092);
    // GFX_Swap_Seg = Allocate_Space(4092);
    // GFX_Swap_Seg = Allocate_Space((65536 * 3));
    g_graphics_cache_seg = Allocate_Space(65535);
    /*
        1024 * 1024 = 1048576
        1048576 / 16 = 65536
        Min EMS is 2700 KB
    */
    // used to load all 198 StatFigs from UNIT1.LBX and UNITS2.LBX 
    // ~ 198 * (18 * 16) = 198 * 288 = 57024 B
    /// lots of XREFs from City, Cmb, SplBk, ...

    // tmp_World_Data_Paras = 3519;
    tmp_World_Data_Paras = 3519;

    // 3519 paragraphs = 16 * 3519 = 56,304 bytes
    World_Data = Allocate_Space(tmp_World_Data_Paras);

    // also used for COMBAT.TMP
    // TODO figure out if/how there is a relationship between this and the check for file size 57764 in Load_SAVE_GAM()

    // should be 100 * 114 = 11400 but, it aint.
    // _CITIES = (struct s_CITY*)Allocate_First_Block(World_Data, 714);  // 714 PR, 11424 B
    _CITIES = (struct s_CITY*)Allocate_First_Block(World_Data, (((NUM_CITIES * sizeof(struct s_CITY)) / 16) + 1));  // 714 PR, 11424 B

    /*
        ...make a better note somewhere...
        WORLD_OVERRUN is for where OG-MoM does some bad math on the buffer
    */
    _world_maps = (uint8_t *)Allocate_Next_Block(World_Data, ( ((((NUM_PLANES * WORLD_SIZE) + WORLD_OVERRUN) * sizeof(int16_t)) / SZ_PARAGRAPH_B) + 2) );  // 602 PR, 9632 B  ... still don't know how they got the +2 for the headers. maybe header and subheader
    p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;

    UU_TBL_1 = Allocate_Next_Block(World_Data, 14);  // 14 PR, 224 B
    UU_TBL_2 = Allocate_Next_Block(World_Data, 14);  // 14 PR, 224 B

    _landmasses    = (uint8_t *)Allocate_Next_Block(World_Data, 302);  // 302 PR, 4832 B  ((2 * 60 * 40) / 16)

    /*
        same OVERRUN issue as _world_maps
    */
    _map_square_terrain_specials = (uint8_t *)Allocate_Next_Block(World_Data, ((((NUM_PLANES * WORLD_SIZE) + WORLD_OVERRUN) * sizeof(uint8_t)) / SZ_PARAGRAPH_B) + 2);   // 302 PR, 4832 B

    _map_square_flags = (uint8_t *)Allocate_Next_Block(World_Data, 302);   // 302 PR, 4832 B

    _square_explored  = (uint8_t *)Allocate_Next_Block(World_Data, 302);   // 302 PR, 4832 B


    square_scouted_p0 = (uint8_t *)Allocate_Next_Block(World_Data, 19);    // 19 PR, 304 B  ¿ (((2400 / 8) + 1) + 1) / 16) ?
    square_scouted_p1 = (uint8_t *)Allocate_Next_Block(World_Data, 19);    // 19 PR, 304 B

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
_ai_move_path_table      = SA_MK_FP0( EMM_EMMDATAH_AllocNext( 1033 ) );  // 1033 PR  16528 B

city_area_bits & city_area_shared_bits
¿ ~== _square_explored or square_scouted_p0/p1 ?
301 * 16 = 4816  ¿ 301 would be the byte count needed for one worlds worth of bits, +1 to cover the paragraph boundary, like with square_scouted_p0/p1 ?
60 * 40 = 2400 / 16 = 150 * 2 = 300

in Reset_City_Area_Bitfields()
it loops over 300 for city_area_bits and city_area_shared_bits
so, 300 PRs, + 1 for the SAMB header

*/

    movepath_cost_map = (struct s_MOVE_PATH *)Allocate_Space(632);
    movement_mode_cost_maps = (struct s_MOVE_MODE_COST_MAPS *)Allocate_Space(1802);  // 1802 PR; 28832 B; ~ 2 * 14400; 28800 / 16 = 1800 + 1 + 1

    city_area_shared_bits = (uint8_t *)Allocate_Space(WORLD_MAP_BITFIELD_SIZE + 1);
    city_area_bits = (uint8_t *)Allocate_Space(WORLD_MAP_BITFIELD_SIZE + 1);

    _ai_move_path_table = (struct s_AI_MOVE_PATH **)Allocate_Space(1033);  // 1033 * 16 = 16528 B


    /*
    
    */

    _HEROES2[0] = (struct s_HEROES *)Allocate_Space(28);  // 28 PR, 448 B
    _HEROES2[1] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
    _HEROES2[2] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
    _HEROES2[3] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
    _HEROES2[4] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B
    _HEROES2[5] = (struct s_HEROES *)Allocate_Space(27);  // 27 PR, 432 B


    _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 PR, 32448 B  32 B each, 1014 units max in a save game, but 2028 PR
    // NUM_UNITS 1009  32 * 1009 = 32288 B;  2028 PR would be 32448 B, so maybe the extra 14 units worth of PR is for COMBAT.TMP?
    // / 16 = 2018 PR, + 1 for the SAMB header, + 9 for COMBAT.TMP?

    // MoO2  global_combat_data
    global_battle_unit = (struct s_BATTLE_UNIT *)Allocate_Space(8);  // 8 PR, 128 B

    _NODES = (struct s_NODE *)Allocate_Space(92);  // 92 PR = 1472 B;  actual: 30 * sizeof(struct s_NODE) = 30 * 48 = 1440 B

    _FORTRESSES = (struct s_FORTRESS *)Allocate_Space(3);

    _TOWERS = (struct s_TOWER *)Allocate_Space(3);  // 3 PR, 48 B

    _LAIRS = (struct s_LAIR *)Allocate_Space(351);  // 351 PR, 5616 B

    events_table = (struct s_EVENT_DATA *)Allocate_Space(7);  // 7 PR, 112 B

    // ¿ MoO2  _officer_names ? ¿ Officer vs. Owned Officer ?
    hero_names_table = (struct s_INACTV_HERO *)Allocate_Space(37);  // 37 PR, 592 B  ... ~ (36) 16-byte structs

    _ITEMS = (struct s_ITEM *)Allocate_Space(433);  // 433 PR, 6928 B;  SAVE.GAM: 138 * 50 = 6900  ... 1 + ((138 * 50) + (SZ_PARAGRAPH - 1))

    TBL_Premade_Items = Allocate_Space(17);  // 17 PR, 272 B
    
    // AKA TBL_Spell_Data
    spell_data_table = (struct s_SPELL_DATA *)Allocate_Space(485);  // 485 PR, 7760 B; actual: 215 * 36 = 7740

    UnitDraw_WorkArea = Allocate_Space(60);  // 60 PR, 960 B
    
    // MoO2  Module: MOX  data (0 bytes) _global_music_handle  Address: 02:0018B17C
    SND_Music_Segment = Allocate_Space(350);  // 350 PR, 5600 B

    AI_MainWarConts[0] = ( (int16_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x17E8) );  // 12 bytes
    AI_MainWarConts[1] = ( (int16_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x17F4) );  // 12 bytes
    _ai_reevaluate_continents_countdown   = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1800) );  // 16 bytes
    Wiz5_Spell_28h     = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1810) );  // 20 bytes
    Wiz5_Spell_3Ch     = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1824) );  // 20 bytes
    Wiz5_Spell_50h     = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1838) );  // 20 bytes
    Wiz5_Spell_64h     = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x184C) );  // 20 bytes
    AI_NewColConts[0]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1860) );  // 20 bytes
    AI_NewColConts[1]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1874) );  // 20 bytes
    AI_NewColTgtXs[0]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1888) );  // 20 bytes
    AI_NewColTgtXs[1]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x189C) );  // 20 bytes
    AI_NewColTgtYs[0]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18B0) );  // 12 bytes
    AI_NewColTgtYs[1]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18BC) );  // 12 bytes
    AI_SCircle_Reevals = ( (int16_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18C8) );  // 16 bytes

    // TODO  EMM_ContXXX_H = EMM_GetHandle(4, cnst_EMM_ContH_Name, 1)
    EmmHndl_CONTXXX = Allocate_Space(4096);   //  4 * 16384 EMM Page Size = 65536 B / 16 = 4096 PR

}
