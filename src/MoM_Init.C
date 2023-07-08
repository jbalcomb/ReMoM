
/*
    ~== {MGC main(), WZD main()}
    *.* |-> Screen_Control()

    MAGIC.EXE
    seg001
    s01p01
    WIZARDS.EXE
    seg001
    s01p01

    MoO2
        Module: INITGAME
        Module: init
*/

/*
Init_STGE()
Init_MoX()
Init_MoM()
Init_MGC()
Init_WZD()
*/

/*
    MGC
        s01p04  Load_MGC_Resources
        s01p07  Load_TERRSTAT
        s01p08  Load_SPELLDAT
    WZD
        o52p01  Load_WZD_Resources

OON XREF:

*/

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_Data.H"

#include "MOM_Init.H"
#include "MoM_main.H"

#include "Allocate.H"
#include "LBX_Load.H"
#include "MoX_DBG.H"
#include "MoX_CFG.H"
#include "MoX_GAM.H"
#include "MoX_RNG.H"
#include "MoX_SET.H"

#include "Fonts.H"  /* Load_Font_File() */
#include "Graphics.H"
#include "Input.H"  /* Init_Mouse_Keyboard() */
#include "Video.H"  /* Set_Page_Off() */

#include "MainMenu.H"
#include "MainScr.H"
#include "MainScr_Maps.H"



#define MOM_FONT_FILE "FONTS.LBX"
#define GAME_FONT_FILE MOM_FONT_FILE




// MGC s01p04
void Load_MGC_Resources(void);
// MGC s01p07
void Load_TERRSTAT();
// MGC s01p08
void Load_SPELLDAT();

// MGC o57p01
// WZD o152p01
void MoM_Tables_Init(int16_t gfx_buff_nparas);

// WZD o52p01
void Load_WZD_Resources(void);
// WZD o52p02
void UNIT_Upkeep_Reset(void);
// WZD o52p03
void Terrain_Init(void);

// Main_Screen_Load_Pictures();  // ; loads and processes the main screen graphic elements

// Load_Combat_Background_Bottom();  // ; creates a reserved EMM LBX header for the combat screen's bottom background

// Load_SPELLDAT();  // ; loads all records from SPELLDAT.LBX, overwriting the pointer to a previous allocation (8k wasted)

// Load_BUILDDAT();  // ; loads all records from BUILDDAT.LBX into a single allocation

// Load_Button_Sounds();  // ; loads the standard and left click sounds

// UNIT_Upkeep_Reset();  // ; resets the Upkeep Cost of Normal Units and Heroes  ; returns void (probably 4)





// ~== MGC & WZD main()
void MoM_Init(void)
{
    char font_file[] = GAME_FONT_FILE;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MoM_Init()\n", __FILE__, __LINE__);
#endif

    Load_CONFIG_MOM();

    Load_MAGIC_SET();

    /*
        MIDI Driver
        SFX Driver
    */

    // TODO  Check_For_Saved_Games()


    // MoO2  Module: init
    // Init_Drivers(GAME_FONT_FILE) |-> 
    // CRP_Empty_Exit_Fn2()
    // EMS_Startup()
    // VGA_SetModeY()
    Load_Font_File(font_file);
    // TODO  Audio_Init()
    Init_Mouse_Keyboard(1);
    Randomize();
    Set_Page_Off();  // initializes `current_video_page`


    Release_Version();
    Set_Global_ESC();


    // MoM_Tables_Init(6100);  // MGC  6100 PR * 16 B = 97600 bytes
    MoM_Tables_Init(4600);  // WZD  4600 PR * 16 B = 73600 bytes


    // TODO  TBL_Wizards.Banner+17E8h = BNR_Brown
    // TODO  GAME_SoM_Cast_By = ST_UNDEFINED


    Load_Palette(0, -1, 0);
    Apply_Palette();


    Load_MGC_Resources();  // MGC s01p04
    Load_WZD_Resources();  // WZD o52p01


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: MoM_Init()\n", __FILE__, __LINE__);
#endif
}

// MGC s01p04
/*
    cache MAINSCR.LBX
    ~ MainMenu_Screen_Load_Pictures()
    cache WIZARDS.LBX
    ~ Load Wizard Portraits - Magic Screen / New Game Screen
    cache SPELLDAT.LBX
*/
void Load_MGC_Resources(void)
{
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_MGC_Resources()\n", __FILE__, __LINE__);
#endif

    // TODO  EMM_Load_LBX_File_1("MAINSCR")
    mainmenu_top = LBX_Load(mainscrn_lbx_file, 0);
    mainmenu_c   = LBX_Load(vortex_lbx_file, 1);
    mainmenu_h   = LBX_Load(vortex_lbx_file, 2);
    mainmenu_q   = LBX_Load(vortex_lbx_file, 3);
    mainmenu_n   = LBX_Load(vortex_lbx_file, 4);
    mainmenu_l   = LBX_Load(vortex_lbx_file, 5);
    mainmenu_bot = LBX_Load(mainscrn_lbx_file, 5);

    // TODO  EMM_Load_LBX_File_1("WIZARDS")

    // HERE: ~== MoO2 Module NEWGAME Reload_Newgame_Screen()
    for(itr = 0; itr < 14; itr++)
    {
        wizard_portrait_segs[itr] = LBX_Load("WIZARDS", itr);  // AKA gsa_WIZARDS_0to13
    }

    // TODO  EMM_Load_LBX_File_1("SPELLDAT")

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_MGC_Resources()\n", __FILE__, __LINE__);
#endif
}


// WZD o52p01
// drake178: LBX_Tables_LoadMain()
// AKA Load_Init_MainGame
void Load_WZD_Resources(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_WZD_Resources()\n", __FILE__, __LINE__);
#endif

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

// fxn_o52p04();
// Main_Screen_Load_Pictures();  // ; loads and processes the main screen graphic elements
// fxn_o52p08();
// fxn_o52p10();
// fxn_o52p07();
// fxn_o52p11();
// drake178: LBX_Assign_Cmbt_BG()
// Load_Combat_Background_Bottom();  // ; creates a reserved EMM LBX header for the combat screen's bottom background
// fxn_o52p12();
// fxn_o52p14():
// fxn_o52p15();
// Load_SPELLDAT();  // ; loads all records from SPELLDAT.LBX, overwriting the pointer to a previous allocation (8k wasted)
// drake178: j_LBX_Load_Bldng_Data
// Load_BUILDDAT();  // ; loads all records from BUILDDAT.LBX into a single allocation
// fxn_o52p18();
// drake178: LBX_Load_Click_SFX
// Load_Button_Sounds();  // ; loads the standard and left click sounds
// UNIT_Upkeep_Reset();  // ; resets the Upkeep Cost of Normal Units and Heroes  ; returns void (probably 4)

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_WZD_Resources()\n", __FILE__, __LINE__);
#endif
}

// WZD o52p02
void UNIT_Upkeep_Reset(void)
{

}

// WZD o52p03
/*
    All (Movement) Map
        TERRAIN.LBX, Entry 1,2
        TERRSTAT.LBX, Entry 1, Records 1x196
        MAPBACK.LBX

    Allocate_Space()
        Map_Square_WorkArea
        Warp_Node_WorkArea
*/
void Terrain_Init(void)
{
    int16_t itr;

    // 
    Map_Square_WorkArea = Allocate_Space(70);  // ; 70 * 16 = 1120 bytes

    terrain_lbx_001 = LBX_Load(terrain_lbx_file, 1);
    terrain_lbx_002 = LBX_Load(terrain_lbx_file, 2);
    // ehn_terrain_lbx = EMM_Load_LBX_File("TERRAIN.LBX", 1);
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
            mineral_site_segs[itr] = LBX_Load(mapback_lbx_file, 77 + itr);
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

        UU_hunters_lodge_seg = LBX_Load(mapback_lbx_file, 90);     // ; single-loaded image, called "hunter's lodge"
        IMG_OVL_Nightshade = LBX_Load(mapback_lbx_file, 91);    // ; reserved EMM header pointer for a single image
        IMG_OVL_WildGame = LBX_Load(mapback_lbx_file, 92);      // ; reserved EMM header pointer for a single image
        
        node_warped_seg = LBX_Load(mapback_lbx_file, 93);    // ; reserved EMM header pointer for a single image

}


/*
    MGC o57p01 (1of1)
    WZD o152p01 (1of1)
*/
void MoM_Tables_Init(int16_t gfx_buff_nparas)
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

    tmp_World_Data_Paras = 3519;
    // 3519 paragraphs = 16 * 3519 = 56,304 bytes
    World_Data = Allocate_Space(tmp_World_Data_Paras);
    // also used for COMBAT.TMP
    // TODO figure out if/how there is a relationship between this and the check for file size 57764 in Load_SAVE_GAM()

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
