/*
    WIZARDS.EXE
    seg001
    
    ovr051
    ovr052
    ovr060
    ovr063
    ovr067

*/
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_Data.H"

#include "MoM_main.H"

#include "MoX_CFG.H"
#include "MoX_DBG.H"
#include "MoX_Data.H"
#include "UNITTYPE.H"
#include "MoX_SET.H"
#include "MoX_GAM.H"

#include "Allocate.H"
#include "Explore.H"
#include "Fields.H"
#include "Fonts.H"
#include "Graphics.H"
#include "Input.H"
#include "LBX_Load.H"
#include "Mouse.H"
#include "Video.H"

#include "LoadScr.H"
#include "MainMenu.H"
#include "MainScr.H"
#include "MainScr_Maps.H"

#include "MoM.H"  /* g_State_Run */

// #define STU_DEBUG 1
#ifdef STU_DEBUG
#include "STU_DBG.H"
#include "TST_GameData.H"
#endif
#include "TST_GameState.H"



int16_t current_screen;

// WZD dseg:9C8E
// ...unitialized data...after MSG's, before TBL's...
int16_t PageFlipEffect;

// uint8_t g_Video_Back_Buffer[64000];

uint8_t g_Palette[768];
uint8_t g_Palette_XBGR[1024];





// MGC s01p01
// WZD s01p01
// MoM_Init:  void MoM_main(void)
void MoM_Main(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MoM_Main()\n", __FILE__, __LINE__);
#endif

    // PLATFORM  MoM_Init()


    // MoO2: Draw_Logos()


    // TODO  c0argv == 'J','E','N','N','Y'
    // TODO  j_GAME_PlayIntro()


    // MoO2: Init_Credits() <-| Load_Credits() <-| Main_Menu_Screen()
    // TODO  j_Init_Credits()
    // TODO  SND_Stop_Music()
    // TODO  MainMenu_Music_Seg = LBX_Load(music_lbx, MUSIC_Main_Menu)
    // TODO  SND_PlayFile(MainMenu_Music_Seg)
    // TODO  Main_Menu_Screen_Control()


    current_screen = scr_Main_Menu_Screen;

    // PLATFORM  Screen_Control();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: MoM_Main()\n", __FILE__, __LINE__);
#endif
}

// WZD s01p02
// ~== MGC s01p03  Menu_Screen_Control()
void Screen_Control(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Screen_Control()\n", __FILE__, __LINE__);
#endif

    Clear_Fields();
    Set_Mouse_List(1, mouse_list_default);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: current_screen: %d\n", __FILE__, __LINE__, current_screen);
#endif

    switch(current_screen)
    {
        case scr_Main_Menu_Screen:
        {
            DLOG("case scr_Main_Menu:");
            Load_Palette(2, -1, 0);
            Apply_Palette();
            // TODO  Main_Menu_Screen_Control();
            Main_Menu_Screen();
        } break;

        case scr_Continue:
        {
            DLOG("case scr_Continue:");
            // BEGIN: WZD main()
            Load_SAVE_GAM(-1);  // SAVETEST.GAM
            Loaded_Game_Update();
            // END: WZD main()
            current_screen = scr_Main_Screen;
        } break;

        case scr_Load_Screen:
        {
            DLOG("case scr_Load_Game:");
            // TODO  Load_Screen();
        } break;

        // case scr_New_Game:
        // {
        //     DLOG("case scr_New_Game:");
        //     New_Game_Screen();
        // } break;

        case scr_Quit_To_DOS:
        {
            DLOG("case scr_Quit_To_DOS:");
            g_State_Run = ST_FALSE;
        } break;
        // case scr_Hall_Of_Fame:
        // {
        //     DLOG("case scr_Hall_Of_Fame:");
        //     Hall_Of_Fame_Screen();
        // } break;
        // case scr_Settings:
        // {
        //     DLOG("case scr_Settings:");
        //     Settings_Screen();
        // } break;
    // scr_City = 100,
    // scr_Load = 101,
    // scr_Armies = 102,
    // scr_Cities = 103,
    // scr_Quit = 104,
        case scr_Main_Screen:
        {
            DLOG("case scr_MainGame:");
            // BEGIN: WZD main()
            // WZD  main()  j_Load_SAVE_GAM(8)
            // WZD  main()  j_Load_WZD_Resources()
            Load_Palette(0, -1, 0);  // NOTE(JimBalcomb,20230111): this is the only Load_Palette() leading to the Main_Screen()
            // VGA_SetShades_Grey0()
            // Set_Button_Down_Offsets(1, 1)
            // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1)
            Apply_Palette();
            // Fade_In()
            // Clear_Fields()
            // Loaded_Game_Update_WZD()
            // END: WZD main()

            Set_Outline_Color(0);
            Set_Alias_Color(0);
            Set_Font_Style1(0, 0, 0, 0);

            Set_Page_Off();
            Fill(0, 0, 319, 199, 7);
            Set_Page_On();
            Fill(0, 0, 319, 199, 5);
            Set_Page_Off();

            Main_Screen();
        } break;
    // scr_Magic = 106,
    // scr_RoadBuilding = 107,
    // scr_Production = 108,
    // scr_Items = 109,
    // scr_NextTurn = 110,
    // /* ?default? */
    // scr_Spellbook = 112,
    // /* ?default? */
    // scr_Advisor = 114,
    // scr_Diplomac = 115

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Screen_Control()\n", __FILE__, __LINE__);
#endif

}



// WZD s01p03
// ; executes a VGA page flip using a special effect for
// ; screen transition set through VGA_PageFlipEffect,
// ; which is zeroed out afterward:
// ;   0 - simple flip, no extra effect
// ;   1 - left to right cut (not used in the game)
// ;   2 - black cut with the new image fading in
// ;   3 - mosaic effect
// ;   4 - grow out from a tile (not used in the game)
// ;   5 - writes out the DAC, but DOES NOT do a page flip
// Default: Apply_Palette(); Toggle_Pages();
// WZD s01p03
// ¿ MoO2: Set_Palette_Changes() ?
void PageFlip_FX(void)
{

    switch(PageFlipEffect)
    {
        case 0:
        {
            Apply_Palette();
            Toggle_Pages();  // |-> Page_Flip()
        } break;
        case 1:
        {
            // RP_VGA_CutRight();
        } break;
        case 2:
        {
            // Toggle_Pages();  // |-> Page_Flip()
            // Fade_In();
        } break;
        case 3:
        {
            // Apply_Palette();
            // VGA_MosaicFlip();  // |-> GUI_PageFlip() |-> Page_Flip()
        } break;
        case 4:
        {
            // RP_VGA_GrowOutFlip(RP_GUI_GrowOutLeft, RP_GUI_GrowOutTop, RP_GUI_GrowOutFrames, _screen_seg + 400)             
        } break;
        default:
        {
            // Apply_Palette();
        } break;
    }

    PageFlipEffect = 0;  // DEDUCE: PageFlipEffect 0 is the default PageFlipEffect

}



// WZD o51p01
// _main() |-> Loaded_Game_Update_WZD() |-> GAME_Overland_Init()
// Load_Screen |-> [WZD ovr160] Loaded_Game_Update() |-> GAME_Overland_Init()
/*
    Does this not feel like ~ Init Main Screen?
    There are a few oddities...
        city buildings
        staff lock flags
        nominal skill calc
        ! City Recalculate All - Main Screen needs that for the Resources Window
        GFX_Swap_Cities(), which has a terribly misleading name, loads other screens stuff
        ? G_WLD_StaticAssetRfrsh() Meh. More oddities...

*/
void GAME_Overland_Init(void)
{
    int16_t itr_cities;
    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GAME_Overland_Init()\n", __FILE__, __LINE__);
#endif


    G_WLD_StaticAssetRfrsh();


    itr_cities = 0;
    while(itr_cities++ < _cities)
    {
        _CITIES[itr_cities].buildings.None = 0; // ? ~ enum City Building Status B_Replaced;
    }


    itr_units = 0;
    while(itr_units++ < _units)
    {
        if(_UNITS[itr_units].world_plane == 2) {_UNITS[itr_units].world_plane = 0; }

        // NOTE: the DASM thinks world_plane is passed here as well, but IsPassableTower() makes no xref to it
        if(IsPassableTower(_UNITS[itr_units].world_x, _UNITS[itr_units].world_y) == ST_TRUE)
        {
            _UNITS[itr_units].In_Tower = ST_TRUE;
        }
        else
        {
            _UNITS[itr_units].In_Tower = ST_FALSE;
        }

    }


    // TODO  skill_staff_lock_flag = 0;
    // TODO  mana_staff_lock_flag = 0;
    // TODO  research_staff_lock_flag = 0;

    // _players[0].Nominal_Skill = Calc_Nominal_Skill(0);

    // NIU?  CRP_OVL_MapWindowX = 0;
    // NIU?  CRP_OVL_MapWindowY = 0;
    _prev_world_x = 0;
    _prev_world_y = 0;
    // _curr_world_x = 0;
    // _curr_world_y = 0;
    _map_x = 0;
    _map_y = 0;

    // j_RP_WIZ_ReturnZero(_human_player_idx)

    _unit = 0;  // 0: None / No Unit

    // TODO(JimBalcomb,20230629): validate the SAVE_GAM data for _FORTRESSES
    // // _active_world_x = _FORTRESSES[0].world_x;
    // // _active_world_y = _FORTRESSES[0].world_y;
    // OVL_Map_CenterX = _FORTRESSES[0].world_x;
    // OVL_Map_CenterY = _FORTRESSES[0].world_y;
    OVL_Map_CenterX = 24;
    OVL_Map_CenterY = 16;

    _unit_window_start_x = 247;
    _unit_window_start_y = 79;

    // TODO(JimBalcomb,20230629): validate the SAVE_GAM data for _FORTRESSES
    // _world_plane = _FORTRESSES[0].world_plane;
    // _map_plane = _FORTRESSES[0].world_plane;  // TODO(JimBalcomb,20230614): Why is this getting set to 100?
    _map_plane = 0;


    TILE_VisibilityUpdt();


    Allocate_Reduced_Map();

    // Center_Map(&_curr_world_x, &_curr_world_y, _FORTRESSES[0].world_x, _FORTRESSES[0].world_y, _world_plane);
    // TODO(JimBalcomb,20230629): validate the SAVE_GAM data for _FORTRESSES
    // Center_Map(&_map_x, &_map_y, _FORTRESSES[0].world_x, _FORTRESSES[0].world_y, _map_plane);
    Center_Map(&_map_x, &_map_y, 24, 16, 0);

    // TODO  Set_Unit_Draw_Priority();
    // TODO  Reset_Stack_Draw_Priority();
    // TODO  Set_Entities_On_Map_Window();

    GFX_Swap_Cities();

    // TODO  j_WIZ_NextIdleStack(_human_player_idx, &_curr_world_x, &_curr_world_y, &_world_plane)
    // ; selects the next idle stack of the specified player,
    // ; if any, moving any other stacks marked as going to
    // ; while iterating over them

    // j_RP_Empty_Load_FnA

    // j_CTY_RecalculateAll
    // ; calls CTY_Recalculate for all cities  ; (with all its BUGs)


    // TODO  if(Check_Release_Version() != ST_FALSE)
    // TODO  {
    // TODO      // WZD s22p05
    // TODO      RNG_SetSeed(0x2737, 0);  // LFSR_LO, LFSR_HI
    // TODO      // ; sets the linear feedback shift register
    // TODO  }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GAME_Overland_Init()\n", __FILE__, __LINE__);
#endif
}

// WZD o51p02
// Loaded_Game_Update_WZD()  |-> GAME_Overland_Init()
// Loaded_Game_Update()    |-> GAME_Overland_Init()
// GAME_Overland_Init()  |-> G_WLD_StaticAssetRfrsh()
// NOTE: no XREFs to j_G_WLD_StaticAssetRfrsh()
void G_WLD_StaticAssetRfrsh(void)
{
    int16_t itr_cities;
    int16_t itr_players;

//    RNG_TimerSeed();
//    LFSR_HI = 0;
//    LFSR_LO = 0x03E8;
//    if(!Check_Release_Version())
//    {
//        RNG_SetSeed(LFSR_LO, LFSR_HI)
//    }

    all_units_moved = ST_FALSE;
    // G_OVL_MapVar4 = 1;  // ? ST_TRUE ?
    // Reset_Active_Stack_Draw()
    _map_plane = 0;  // Arcanus

//    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
//    {
//        CTY_Recalculate(itr_cities);
//    }

//    for(itr_players = 0; itr_players < _num_players; itr_players++)
//    {
//        WIZ_RefreshResearch(itr_players);
//    }

//    _WIZ_SetPowerBases();

//    SBK_SomePageSaveVar = 0;
//    CMB_SpellBookPage = 0;
//    SBK_Candidate_Page = 0;

}

// WZD o67p15
int16_t IsPassableTower(int16_t world_x, int16_t world_y)
{
    int16_t itr_towers;
    int16_t is_passible_tower;
    int16_t active_planar_seal;

    is_passible_tower = ST_FALSE;

    itr_towers = 0;
    while(itr_towers++ < TOWER_COUNT_MAX)
    {
        if(world_x == _TOWERS[itr_towers].world_x && world_y == _TOWERS[itr_towers].world_y)
        {
            is_passible_tower = ST_TRUE;
        }
    }

    if(is_passible_tower == ST_TRUE)
    {
        if(Check_Planar_Seal() == ST_TRUE)
        {
            is_passible_tower = ST_FALSE;
        }
    }

    return is_passible_tower;
}

// WZD o63p15
int16_t Check_Planar_Seal(void)
{
    int16_t itr_players;
    int16_t active_planar_seal;

    active_planar_seal = ST_FALSE;

    itr_players = 0;
    while(itr_players++ < _num_players)
    {

        if(active_planar_seal == ST_FALSE)
        {
            if(_players[itr_players].Globals.Planar_Seal == ST_TRUE)
            {
                active_planar_seal = ST_TRUE;
            }
        }

    }

    return active_planar_seal;
}



/*
    WIZARDS.EXE  ovr052
*/

// WZD o52p20
void GFX_Swap_Reset(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GFX_Swap_Reset()\n", __FILE__, __LINE__);
#endif

    Allocate_First_Block(GFX_Swap_Seg, 1);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GFX_Swap_Reset()\n", __FILE__, __LINE__);
#endif
}

// WZD o52p22
// AKA GFX_Swap_AppendUnits()
void Load_Unit_StatFigs(void)
{
    uint16_t itr_unit_types;
#ifdef STU_DEBUG
    SAMB_ptr pict_seg;
    int16_t pict_seg_width;
    int16_t pict_seg_height;
    uint16_t itr_pict_seg;
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Unit_StatFigs()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: GFX_Swap_Seg: %p\n", __FILE__, __LINE__, GFX_Swap_Seg);
#endif


    for(itr_unit_types = 0; itr_unit_types < 120; itr_unit_types++)
    {
        _unit_type_table[itr_unit_types].pict_seg = LBX_Reload_Next("UNITS1", itr_unit_types, GFX_Swap_Seg);
    }

    for(itr_unit_types = 120; itr_unit_types < 198; itr_unit_types++)
    {
        _unit_type_table[itr_unit_types].pict_seg = LBX_Reload_Next("UNITS2", itr_unit_types-120, GFX_Swap_Seg);
    }
    
// #ifdef STU_DEBUG
//     for(itr_unit_types = 0; itr_unit_types < 198; itr_unit_types++)
//     {
//         pict_seg = _unit_type_table[itr_unit_types].pict_seg;
//         pict_seg_width = GET_2B_OFS(pict_seg, 0);
//         pict_seg_height = GET_2B_OFS(pict_seg, 2);
//     // // dbg_prn("DEBUG: [%s, %d]: pict_seg: %p\n", __FILE__, __LINE__, pict_seg);
//     // dbg_prn("DEBUG: [%s, %d]: pict_seg_width: %d\n", __FILE__, __LINE__, pict_seg_width);
//     // dbg_prn("DEBUG: [%s, %d]: pict_seg_height: %d\n", __FILE__, __LINE__, pict_seg_height);
// 
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg,  0): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 0));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg,  2): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 2));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg,  4): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 4));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg,  6): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 6));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg,  8): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 8));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg, 10): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 10));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg, 12): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 12));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg, 14): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 14));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg, 16): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 16));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg, 18): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 18));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg, 20): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 20));
//         dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(pict_seg, 22): %02X\n", __FILE__, __LINE__, GET_2B_OFS(pict_seg, 22));
// 
//         if(GET_2B_OFS(pict_seg, 0) != 18)
//         {
//             dbg_prn("DEBUG: [%s, %d]: FAILURE: Load_Unit_StatFigs(): %u\n", __FILE__, __LINE__, itr_unit_types);
//         }
//         if(GET_2B_OFS(pict_seg, 2) != 16)
//         {
//             dbg_prn("DEBUG: [%s, %d]: FAILURE: Load_Unit_StatFigs(): %u\n", __FILE__, __LINE__, itr_unit_types);
//         }
// 
// 
//     }
// #endif

// #ifdef STU_DEBUG
//     // pict_seg = _unit_type_table[0].pict_seg;  // Dwarf / BRAX
//     // pict_seg = _unit_type_table[109].pict_seg;
//     // pict_seg = _unit_type_table[119].pict_seg;
//     // pict_seg = _unit_type_table[197].pict_seg;
//     pict_seg = _unit_type_table[34].pict_seg;  // Trireme
//     pict_seg_width = GET_2B_OFS(pict_seg, 0);
//     pict_seg_height = GET_2B_OFS(pict_seg, 2);
//     dbg_prn("DEBUG: [%s, %d]: pict_seg: %p\n", __FILE__, __LINE__, pict_seg);
//     dbg_prn("DEBUG: [%s, %d]: pict_seg_width: %d\n", __FILE__, __LINE__, pict_seg_width);
//     dbg_prn("DEBUG: [%s, %d]: pict_seg_height: %d\n", __FILE__, __LINE__, pict_seg_height);
//     // for(itr_pict_seg = 0; itr_pict_seg < 204; itr_pict_seg++)
//     // for(itr_pict_seg = 0; itr_pict_seg < 33; itr_pict_seg++)
//     // for(itr_pict_seg = 0; itr_pict_seg < 233; itr_pict_seg++)
//     // for(itr_pict_seg = 0; itr_pict_seg < 177; itr_pict_seg++)
//     for(itr_pict_seg = 0; itr_pict_seg < 225; itr_pict_seg++)
//     {
//         dbg_prn("DEBUG: [%s, %d]: pict_seg[%u]: %02X\n", __FILE__, __LINE__, itr_pict_seg, *(pict_seg + itr_pict_seg));
//     }
// #endif


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_Unit_StatFigs()\n", __FILE__, __LINE__);
#endif

}

// WZD o52p30
void GFX_Swap_Cities(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GFX_Swap_Cities()\n", __FILE__, __LINE__);
#endif

    GFX_Swap_Reset();
    Load_Unit_StatFigs();  // LBX_Reload_Next(); UNITS_.LBX, _unit_type_table, GFX_Swap_Seg
    // GFX_SwapAppendItems();
    // GFX_Swap_AppndCtScap();
    // City_Screen_Load_Pictures();
    // Spellbook_Load_Small_Pictures();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GFX_Swap_Cities()\n", __FILE__, __LINE__);
#endif
}



/*
    WZD ovr060
*/

// WZD o60p07
void TILE_VisibilityUpdt(void)
{
    int16_t itr_players;
    int16_t itr_units;
    int16_t tmp_unit_enchantments_hiword;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        // TODO(JimBalcomb,2023075): figure out the indexing in the Dasm - doesn't look like array of struct  also, this'll set the neutral player?
        _players[itr_players + 1].Dipl.Contacted[0] = 1;

        if(_players[itr_players + 1].Globals.Nature_Awareness != ST_FALSE)
        {
            for(itr_units = 0; itr_units < _units; itr_units++)
            {
                if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
                {
                    if(_UNITS[itr_units].owner_idx != itr_players)
                    {
                        if(_UNITS[itr_units].owner_idx != NEUTRAL_PLAYER_IDX)
                        {
                            // BUG: only checks enchantment, not ability or item
                            tmp_unit_enchantments_hiword = _UNITS[itr_units].Enchants_HI;  // // ; enum UE_FLAGS_H
                            // UE_Invisibility 0x8000
                            if( (tmp_unit_enchantments_hiword & 0x8000) != 0 )
                            {
                                _players[itr_players].Dipl.Contacted[_UNITS[itr_units].owner_idx] = 1;
                                _players[_UNITS[itr_units].owner_idx].Dipl.Contacted[itr_players] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    Update_Scouted_And_Contacted();
    Validate_Square_Scouted(18,11,0);

}
