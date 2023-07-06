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
#include "Fields.H"
#include "Fonts.H"
#include "Explore.H"
#include "Input.H"
#include "LBX_Load.H"
#include "Mouse.H"
#include "Video.H"

#include "MainMenu.H"
#include "MainScr.H"
#include "MainScr_Maps.H"

// #define STU_DEBUG 1
#ifdef STU_DEBUG
#include "STU_DBG.H"
#include "TST_GameData.H"
#endif
#include "TST_GameState.H"


#define MOM_FONT_FILE "FONTS.LBX"
#define GAME_FONT_FILE MOM_FONT_FILE


uint8_t g_State_Run;

int16_t g_Current_Screen;

// WZD dseg:9C8E
// ...unitialized data...after MSG's, before TBL's...
int16_t PageFlipEffect;

// uint8_t g_Video_Back_Buffer[64000];

uint8_t g_Palette[768];
uint8_t g_Palette_XBGR[1024];





void Screen_Control(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Screen_Control()\n", __FILE__, __LINE__);
#endif

    Clear_Fields();
    Set_Mouse_List(1, mouse_list_default);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: g_Current_Screen: %d\n", __FILE__, __LINE__, g_Current_Screen);
#endif

    switch(g_Current_Screen)
    {
        case scr_Main_Menu_Screen:
        {
            DLOG("case scr_Main_Menu:");
            Load_Palette(2, -1, 0);
            Apply_Palette();
            Main_Menu_Screen();
        } break;

        case scr_Continue:
        {
            DLOG("case scr_Continue:");
            // // Load_SAVE_GAM(9);
            // Load_SAVE_GAM(-1);  // SAVETEST.GAM
            // // TST_Validate_GameData();
            // // Main_Screen();
            g_Current_Screen = scr_Main_Screen;
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
            // Load_SAVE_GAM(-1);  // SAVETEST.GAM
            // WZD main()  _main+32F
            Load_Palette(0, -1, 0);
            Apply_Palette();

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



void MoM_main(void)
{
    char font_file[] = GAME_FONT_FILE;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MoM_main()\n", __FILE__, __LINE__);
#endif


    // ?
    // Init_STGE()
    // Init_MGC()
    // Init_WZD()
    // Init_MoM()
    // ?

    // Init_STGE()
    // |-> Init_Drivers(GAME_FONT_FILE) |-> Load_Font_File(font_file)
    Load_Font_File(font_file);
    Init_Mouse_Keyboard(1);
    Release_Version();
    Set_Global_ESC();
    MoM_Tables_Init(4600);  // 4600 * 16 = 73600 bytes
    // NOTE(JimBalcomb,20230111): this load game is only here because 'Continue' is the default behavior of WIZARDS.EXE
    // Load_SAVE_GAM(8);
    Load_SAVE_GAM(-1);  // SAVETEST.GAM
    Load_Init_MainGame();  // ovr052

    // g_EmmHndl_OVERXYZ ... __OvrInitEms() ... EMS Unallocated Raw Page Count

    // NOTE(JimBalcomb,20230111): this is the only Load_Palette() leading to the Main_Screen()
    Load_Palette(0, -1, 0);
    // VGA_SetShades_Grey0()
    // GUI_SetBtnClickDir()
    // VGA_SetBlinkColor()
    Apply_Palette();
    // Fade_In()

    // _current_screen = scr_Main_Screen
    // TBL_Wizards.Banner+17E8h = BNR_Brown
    // Clear_Fields()
    WZD_Startup_MainGame();
    // GAME_RazeCity = 0
    // player_idx = 0
    PageFlipEffect = 0;
    _unit_stack_count = 0;
    // GAME_TimeStopTracker = 0

    // GAME_SoM_Cast_By = ST_UNDEFINED
    // Screen_Control()
    // RP_Empty_Exit_Fn1()



    // Read_CONFIG_MOM();
    // // Write_CONFIG_MOM();
    // Read_MAGIC_SET();
    // // Write_MAGIC_SET();
    // // Read_SAVE_GAM();
    // // Write_SAVE_GAM();

    Load_CONFIG_MOM();
    Load_MAGIC_SET();
    Load_SAVE_GAM(-1);
    // TODO(JimBalcomb,20230629): add test code for load and save SAVE_GAM
    // Save_SAVE_GAM(0);
#ifdef STU_DEBUG
    Export_GameData();
#endif


    // HERE:  draw_page_num = 0; current_video_page = 0xA000;
    // TODO  Fill(0,0,319,199);  // Clear Off-Screen
    // TODO  Set_Page_On()
    // TODO  Fill(0,0,319,199);  // Clear On-Screen
    // TODO  Set_Page_Off()
    draw_page_num = 0;
    current_video_page = video_page_buffer[0];
    Set_Page_Off();


    Load_Palette(0, -1, 0);
    // TODO  Apply_Palette()

    g_Current_Screen = scr_Main_Menu_Screen;

    g_State_Run = 1;  // ST_TRUE


    // DBG  Main_Menu_Load_Pictures();
    // DBG  // FLIC_Draw(int x_start, int y_start, SAMB_ptr p_FLIC_File)
    // DBG  FLIC_Draw(0, 0, mainmenu_top);
    // DBG  FLIC_Draw(0, 41, mainmenu_bot);
    // DBG  int menu_x_start = 123;
    // DBG  int menu_y_start = 141;
    // DBG  int menu_shift = 0;
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + 12), mainmenu_l);
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + (12 * menu_shift)), mainmenu_c);
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + 24), mainmenu_n);
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + 36), mainmenu_h);
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + 48), mainmenu_q);
    // DBG  STU_Export_VBB_To_BMP32();

    Set_Page_Off();
    Fill(0, 0, 319, 199, 7);
    Set_Page_On();
    Fill(0, 0, 319, 199, 5);
    Set_Page_Off();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: MoM_main()\n", __FILE__, __LINE__);
#endif

}


void MoM_Tables_Init(int16_t gfx_buff_nparas)
{
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MoM_Tables_Init()\n", __FILE__, __LINE__);
#endif


// TODO  EmmHndl_FIGUREX = EMM_GetHandle(28, EmmHndlNm_FIGUREX, 1);  // Page_Count, Handle_Name, Reserved_Flag
// TODO  EmmHndl_TILEXXX = EMM_GetHandle(3, EmmHndlNm_TILEXXX, 1);  // Page_Count, Handle_Name, Reserved_Flag

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





    // 635 paragraphs = 16 * 635 = 10,160 bytes
    // GFX_Swap_Seg = Allocate_Space(635);
    GFX_Swap_Seg = Allocate_Space(4092);


    tmp_World_Data_Paras = 3519;
    // 3519 paragraphs = 16 * 3519 = 56,304 bytes
    World_Data = Allocate_Space(tmp_World_Data_Paras);
    // 714 paragraphs = 16 * 714 = 11,424 bytes
    _CITIES = (struct s_CITY *)Allocate_First_Block(World_Data, 714);   // 714 Paragraphs, 11424 Bytes
    // 602 paragraphs = 16 * 602 = 9632 bytes
    _world_maps = Allocate_Next_Block(World_Data, 602);         // 602 Paragraphs, 9632 Bytes

    UU_TBL_1 = Allocate_Next_Block(World_Data, 14);             // 14 Paragraphs, 224 Bytes
    UU_TBL_2 = Allocate_Next_Block(World_Data, 14);             // 14 Paragraphs, 224 Bytes

    TBL_Landmasses = (uint8_t *)Allocate_Next_Block(World_Data, 302);      // 302 Paragraphs, 4832 Bytes
    TBL_Terr_Specials = (uint8_t *)Allocate_Next_Block(World_Data, 302);   // 302 Paragraphs, 4832 Bytes
    TBL_Terrain_Flags = (uint8_t *)Allocate_Next_Block(World_Data, 302);   // 302 Paragraphs, 4832 Bytes
    TBL_Scouting = (uint8_t *)Allocate_Next_Block(World_Data, 302);        // 302 Paragraphs, 4832 Bytes

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
    TBL_MoveMaps_EMS = Allocate_Space(1802);
    TBL_SharedTiles_EMS = Allocate_Space(301);
    TBL_Catchments_EMS = Allocate_Space(301);
    TBL_OvlMovePaths_EMS = Allocate_Space(1033);

    /*
    
    */



    p0_heroes = Allocate_Space(28);  // 28 paragraphs = 448 bytes
    p1_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    p2_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    p3_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    p4_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    p5_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes


    _UNITS = (struct s_UNIT *)Allocate_Space(2028);  // 2028 paragraphs = 32448 bytes

    Active_Unit = Allocate_Space(8);  // 8 paragraphs = 128 bytes
    
    TBL_Nodes = (struct s_NODE *)Allocate_Space(92);  // 92 PR = 1472 B;  actual: 30 * sizeof(struct s_NODE) = 30 * 48 = 1440 B
    
    _FORTRESSES = (struct s_FORTRESS *)Allocate_Space(3);  // 3 PR = 48 B;  actual: 6 * sizeof(struct s_FORTRESS) = 24
    
    _TOWERS = (struct s_TOWER *)Allocate_Space(3);  // 3 paragraphs = 48 bytes
    
    TBL_Lairs = (struct s_LAIR *)Allocate_Space(351);  // 351 paragraphs = 5616 bytes
    
    _events_table = (uint8_t *)Allocate_Space(7);  // 7 paragraphs = 112 bytes
    
    TBL_Hero_Names = Allocate_Space(37);
    
    TBL_Items = Allocate_Space(433);
    
    TBL_Premade_Items = Allocate_Space(17);
    
    // TBL_Spell_Data = Allocate_Space(485);
    
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


// WZD s01p06
void WZD_Startup_MainGame(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WZD_Startup_MainGame()\n", __FILE__, __LINE__);
#endif

// mov     [GAME_RazeCity], 0
    _human_player_idx = HUMAN_PLAYER_IDX;
    PageFlipEffect = 0;
// call    j_CTY_CatchmentRefresh          ; clears and recalculates the bitfield lookup tables

    GAME_Overland_Init();

// call    j_RP_GAME_UnitTypesUpdate       ; refreshes the Upkeep and Sound_FX fields of the unit
// call    j_LD_CTY_ResRefresh             ; Legacy Development function, can be removed
// call    j_LD_MAP_TFUnk40_Eval           ; not sure what this resource is or would have been,
// call    j_CTY_CheckMinFarmers           ; ensures that every city has at least the minimum

    _unit_stack_count = 0;

// call    j_SND_PlayBkgrndTrack           ; stops all sound playback and, if background music is

    GFX_Swap_Cities();

// call    j_CTY_ResetRoadConns            ; resets the road connection bitfields of every city
// call    j_GAME_DeleteDeadUnits          ; deletes all removed units from the unit table,
// call    j_AI_ResetUnitMoves             ; marks all AI units that are on patrol, building a

    // for(itr = 1; itr < NUM_PLAYERS; itr++)
    // {
    //     TBL_Wizards[itr].Avg_Unit_Value = 0;
    // }

// j_GAME_NextHumanStack
// ; a wrapper for WIZ_NextIdleStack hard-coded for the
// ; human player and surrounded by two visibility updates
// ; is this really necessary?

    // if(game_data.Difficulty == _Intro) { magic_set.Random_Events = ST_FALSE; }

    // for(itr = 0; itr < 100; itr++)
    // {
    //     mov     ax, si
    //     mov     dx, 118
    //     imul    dx
    //     les     bx, [TBL_OvlMovePathsEMS@]
    //     add     bx, ax
    //     mov     [word ptr es:bx], e_ST_UNDEFINED
    // }

    // CRP_UNIT_OverlandPath = ST_UNDEFINED

    // j_CONTX_CreateChains()
    // j_CONTX_CreateLChains()

    // for(itr = 0; itr < NUM_PLAYERS; itr++)
    // {
    //     mov     ax, si
    //     shl     ax, 1
    //     mov     bx, [AI_CONTX_Reevals@]         ; 16 bytes
    //     add     bx, ax
    //     mov     [word ptr bx], 0
    // }

    // GAME_TimeStopTracker = 0
    // for(itr = 0; itr < NUM_PLAYERS; itr++)
    // {
    //     if(TBL_Wizards[itr].Globals.Time_Stop > 0) { GAME_TimeStopTracker = itr + 1 }
    // }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WZD_Startup_MainGame()\n", __FILE__, __LINE__);
#endif
}


// WZD o51p01
// _main() |-> WZD_Startup_MainGame() |-> GAME_Overland_Init()
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
// WZD_Startup_MainGame()  |-> GAME_Overland_Init()
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


// WZD o52p01
// drake178: LBX_Tables_LoadMain()
void Load_Init_MainGame(void)
{

// Load_MAGIC_SET();

// EMM_Load_LBX_File_1(rsc01_MAIN_LBX)
// EMM_Load_LBX_File_1(rsc02_BACKGRND_LBX);
// EMM_Load_LBX_File_1(rsc03_UNITS1_LBX);
// EMM_Load_LBX_File_1(rsc04_UNITS2_LBX);
// EMM_Load_LBX_File_1(rsc05_UNITVIEW_LBX);
// EMM_Load_LBX_File_1(rsc06_SPECIAL_LBX);
// EMM_Load_LBX_File_1(rsc07_SPECIAL2_LBX);
// EMM_Load_LBX_File_1(rsc08_ITEMS_LBX);
// EMM_Load_LBX_File_1(rsc09_CHRIVER_LBX);
// EMM_Load_LBX_File_1(rsc0A_ITEMISC_LBX);
// EMM_Load_LBX_File_1(rsc0B_CITYSCAP_LBX);
// EMM_Load_LBX_File_1(rsc0C_MAPBACK_LBX);
// EMM_Load_LBX_File_1(rsc0D_CMBMAGIC_LBX);
// EMM_Load_LBX_File_1(rsc0E_CMBTCITY_LBX);
// EMM_Load_LBX_File_1(rsc0F_CITYWALL_LBX);

    Terrain_Init();
    // ; processes and loads the TERRAIN, TERRSTAT, and
    // ; MAPBACK.LBX files, except the actual terrain tiles
    // ; (or movement translation tables), although it does
    // ; load the entire TERRAIN.LBX file into an EMS handle

// fxn_o52p04();
// Main_Screen_Load_Pictures();  // ; loads and processes the main screen graphic elements
// fxn_o52p08();
// fxn_o52p10();
// fxn_o52p07();
// fxn_o52p11();
// LBX_Assign_Cmbt_BG();  // ; creates a reserved EMM LBX header for the combat screen's bottom background
// fxn_o52p12();
// fxn_o52p14():
// fxn_o52p15();
// Load_SPELLDAT();  // ; loads all records from SPELLDAT.LBX, overwriting the pointer to a previous allocation (8k wasted)
// Load_BUILDDAT();  // ; loads all records from BUILDDAT.LBX into a single allocation
// fxn_o52p18();
// LBX_Load_Click_SFX();  // ; loads the standard and left click sounds
// UNIT_Upkeep_Reset();  // ; resets the Upkeep Cost of Normal Units and Heroes  ; returns void (probably 4)
}

// WZD o52p02
void UNIT_Upkeep_Reset(void)
{

}

// WZD o52p03
/*
    All Terrain
        TERRAIN.LBX, TERRSTAT.LBX, MAPBACK.LBX

*/
void Terrain_Init(void)
{
    int16_t itr;

    terrain_lbx_001 = LBX_Load(terrain_lbx_file, 1);
    gsa_OVL_Tile_WorkArea = Allocate_Space(70);  // ; 70 * 16 = 1120 bytes
    terrain_lbx_002 = LBX_Load(terrain_lbx_file, 2);
    // g_EmmHndl_TERRAIN = EMM_Load_LBX_File("TERRAIN.LBX", 1);
    // HACK: no EMM, so just load entry and monkey with offset adjustments
    terrain_lbx_000 = LBX_Load(terrain_lbx_file, 0);


//     // TBL_Unrest = LBX_Load_Data("TERRSTAT.LBX", 1, 0, 1, 196);
//     // ; 14 individual pointers, one to each row of the table
//     // for(itr = 1; itr < 14; itr++)
//     // {
//     //     TBL_Unrest[itr] = TBL_Unrest[itr - 1] + (14 * 16);
//     // }


    // Loop MAPBACK 0 to 13:
    for(itr = 1; itr < 15; itr++)
    {
        itr--;
        unexplored_mask_seg[itr] = LBX_Load(rsc0C_MAPBACK_LBX, itr);  // ; array of 14 reserved EMM header pointers
        itr++;
    }


    // Loop MAPBACK 14 to 19:
    for(itr = 0; itr < 6; itr++)
    {
        _unit_colored_backgrounds_seg[itr] = LBX_Load(mapback_lbx_file, 14 + itr);
    }

    IMG_OVL_Walled_City = LBX_Load(mapback_lbx_file, 20);   // ; reserved EMM header pointer for a 5 frame image
    IMG_OVL_NoWall_City = LBX_Load(mapback_lbx_file, 21);   // ; reserved EMM header pointer for a 5 frame image

    tower_unowned_seg = LBX_Load(mapback_lbx_file, 69);     // ; reserved EMM header pointer for a single image
    tower_owned_seg = LBX_Load(mapback_lbx_file, 70);       // ; reserved EMM header pointer for a single image

// 71  SITES    mound
    IMG_OVL_EZ_Cave = LBX_Load(mapback_lbx_file, 71);       // ; reserved EMM header pointer for a single image
// 74  SITES    ruins
    IMG_OVL_EZ_Dung = LBX_Load(mapback_lbx_file, 74);       // ; reserved EMM header pointer for a single image
// 72  SITES    temple
    IMG_OVL_EZ_ATmpl = LBX_Load(mapback_lbx_file, 72);      // ; reserved EMM header pointer for a single image
// 73  SITES    keep
    IMG_OVL_EZ_AKeep = LBX_Load(mapback_lbx_file, 73);      // ; reserved EMM header pointer for a single image
// 71  SITES    mound
    IMG_OVL_EZ_MLair = LBX_Load(mapback_lbx_file, 71);      // ; reserved EMM header pointer for a single image
// 74  SITES    ruins
    IMG_OVL_EZ_Ruins = LBX_Load(mapback_lbx_file, 74);      // ; reserved EMM header pointer for a single image
// 75  SITES    fallen temple
    IMG_OVL_EZ_FTmpl = LBX_Load(mapback_lbx_file, 75);      // ; reserved EMM header pointer for a single image
//     EZ_Cave,
//     EZ_Dungeon,
//     EZ_Ancient_Temple,
//     EZ_Keep,
//     EZ_Monster_Lair,
//     EZ_Ruins,
//     EZ_Fallen_Temple
// SAMB_ptr IMG_OVL_EZ_Cave;                   // ; reserved EMM header pointer for a single image
// // WZD dseg:CBC6
// SAMB_ptr IMG_OVL_EZ_Dung;                   // ; reserved EMM header pointer for a single image
// // WZD dseg:CBC8
// SAMB_ptr IMG_OVL_EZ_ATmpl;                   // ; reserved EMM header pointer for a single image
// // WZD dseg:CBCA
// SAMB_ptr IMG_OVL_EZ_AKeep;                   // ; reserved EMM header pointer for a single image
// // WZD dseg:CBCC
// SAMB_ptr IMG_OVL_EZ_MLair;                   // ; reserved EMM header pointer for a single image
// // WZD dseg:CBCE
// SAMB_ptr IMG_OVL_EZ_Ruins;                   // ; reserved EMM header pointer for a single image
// // WZD dseg:CBD0
// SAMB_ptr IMG_OVL_EZ_FTmpl;                   // ; reserved EMM header pointer for a single image

        /*
            MAPBACK 78 to 86:
            78  SITES   coal
            79  SITES   iron
            80  SITES   silver
            81  SITES   gold
            82  SITES   gems
            83  SITES   mithril
            84  SITES   adamantium
            85  SITES   quork
            86  SITES   crysx
        */
        for(itr = 1; itr < 10; itr++)
        {
            _mineral_sites_seg[itr] = LBX_Load(mapback_lbx_file, 77 + itr);
        }


//     UU_IMG_OVL_Mud = LBX_Load(mapback_lbx_file, 76);
//     // ; reserved EMM header pointer (unused in the game?)

        IMG_OVL_Corruption = LBX_Load(mapback_lbx_file, 77);    // ; reserved EMM header pointer for a single image

//     UU_IMG_OVL_Empty1 = LBX_Load(mapback_lbx_file, 87);
//     // ; single-loaded image, called "mine" in the file
//     UU_IMG_OVL_Empty2 = LBX_Load(mapback_lbx_file, 88);
//     // ; single-loaded image, called "lumber camp" in the file


        road_lone_seg = LBX_Load(mapback_lbx_file, 45);  // ; reserved EMM header pointer for a single image  ; no connection normal road

        // Loop MAPBACK 46 to 53:
        for(itr = 0; itr < 8; itr++)
        {
            roads_seg[itr] = LBX_Load(mapback_lbx_file, 46 + itr);  // ; array of 8 reserved EMM header pointers  ; clockwise 1-connection roads starting up
        }

        eroad_lone_seg = LBX_Load(mapback_lbx_file, 54);  // ; reserved EMM header pointer for a 6 frame animation  ; no connection enchanted road
        // Loop MAPBACK 55 to 62:
        for(itr = 0; itr < 8; itr++)
        {
            eroads_seg[itr] = LBX_Load(mapback_lbx_file, 55 + itr);  // ; array of 8 reserved EMM header pointers  ; for 6 frame animations  ; clockwise 1-connection e.roads starting up
        }


        Warp_Node_WorkArea = Allocate_Space(24);               // ; used to save and manipulate the tile graphics to ; display the warp node effect

        // Loop MAPBACK 63 to 67:
        for(itr = 0; itr < 5; itr++)
        {
            node_auras_seg[itr] = LBX_Load(mapback_lbx_file, 63 + itr);  // ; array of 5 reserved EMM header pointers for // ; 6 frame animations
        }


//     UU_IMG_OVL_WorkMark = LBX_Load(mapback_lbx_file, 89);
//     // ; single-loaded 6 frame animation, unused afaik
//     // ; (blue tile frame called "city work area")

        UU_IMG_OVL_Empty3 = LBX_Load(mapback_lbx_file, 90);     // ; single-loaded image, called "hunter's lodge"
        IMG_OVL_Nightshade = LBX_Load(mapback_lbx_file, 91);    // ; reserved EMM header pointer for a single image
        IMG_OVL_WildGame = LBX_Load(mapback_lbx_file, 92);      // ; reserved EMM header pointer for a single image
        
        node_warped_seg = LBX_Load(mapback_lbx_file, 93);    // ; reserved EMM header pointer for a single image

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
