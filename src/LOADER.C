
/*

MGC s01p04
Load_MGC_Resources()

*/
/*
    WIZARDS.EXE
        ovr052
            34

WZD o52p01
Load_WZD_Resources()

WZD o52p02
UNIT_Upkeep_Reset()

WZD o52p03
Terrain_Init()

// WZD o52p04
// WZD o52p05
// WZD o52p06
// WZD o52p07
// WZD o52p08
// WZD o52p09
// WZD o52p10
// WZD o52p11
// WZD o52p12
// WZD o52p13
// WZD o52p14
// WZD o52p15
// WZD o52p16
// WZD o52p17
// WZD o52p18
// WZD o52p19

WZD o52p20
void GFX_Swap_Reset()

WZD o52p21

WZD o52p22
Load_Unit_StatFigs()

// WZD o52p23
// WZD o52p24
// WZD o52p25
// WZD o52p26
// WZD o52p27
// WZD o52p28

// WZD o52p29
void Spellbook_Load_Small_Pictures(void);

WZD o52p30
GFX_Swap_Cities()

// WZD o52p31
void GFX_Swap_Overland(void);

// WZD o52p32
// WZD o52p33

// WZD o52p34
// GFX_Swap_Combat()

*/



#include "MoM.H"



// WZD dseg:291E                            BEGIN: ovr052

// WZD dseg:291E                            Load/Init Main Game

// WZD dseg:291E rsc01_MAIN_LBX db 'MAIN.LBX',0        
// WZD dseg:2927 rsc02_BACKGRND_LBX db 'BACKGRND.LBX',0
// WZD dseg:2934 rsc03_UNITS1_LBX db 'UNITS1.LBX',0    
// WZD dseg:293F rsc04_UNITS2_LBX db 'UNITS2.LBX',0    
// WZD dseg:294A rsc05_UNITVIEW_LBX db 'UNITVIEW.LBX',0
// WZD dseg:2957 rsc06_SPECIAL_LBX db 'SPECIAL.LBX',0  
// WZD dseg:2963 rsc07_SPECIAL2_LBX db 'SPECIAL2.LBX',0
// WZD dseg:2970 rsc08_ITEMS_LBX db 'ITEMS.LBX',0      
// WZD dseg:297A rsc09_CHRIVER_LBX db 'CHRIVER.LBX',0  
// WZD dseg:2986 rsc0A_ITEMISC_LBX db 'ITEMISC.LBX',0  
// WZD dseg:2992 rsc0B_CITYSCAP_LBX db 'CITYSCAP.LBX',0

// WZD dseg:299F
char rsc0C_MAPBACK_LBX[] = "MAPBACK.LBX";

// dseg:29AB rsc0D_CMBMAGIC_LBX db 'CMBMAGIC.LBX',0
// dseg:29B8 rsc0E_CMBTCITY_LBX db 'CMBTCITY.LBX',0
// dseg:29C5 rsc0F_CITYWALL_LBX db 'CITYWALL.LBX',0

// dseg:29D2                                Init_Terrain()

// WZD dseg:29D2
char terrain_lbx_file[] = "TERRAIN.LBX";

// WZD dseg:29DE
char terrstat_lbx_file[] = "TERRSTAT";

// WZD dseg:29E7
char mapback_lbx_file[] = "MAPBACK";

// WZD dseg:29EF                            Menu_Screen_Load_Pictures()

// WZD dseg:29EF
char main_lbx_file[] = "MAIN";

// WZD dseg:29F4
char backgrnd_lbx_file[] = "BACKGRND";

// WZD dseg:29FD
char soundfx_lbx_file__ovr052[] = "SOUNDFX";

// WZD dseg:2A05
char spelldat_lbx_file[] = "SPELLDAT.LBX";

// WZD dseg:2A12
char builddat_lbx_file[] = "BUILDDAT.LBX";

// WZD dseg:2A1F
char units1_lbx_file[] = "UNITS1";

// WZD dseg:2A26
char units2_lbx_file[] = "UNITS2";
// WZD dseg:2A2D

char cmbmagic_lbx_file[] = "CMBMAGIC";

// WZD dseg:2A36
char cmbtcity_lbx_file[] = "CMBTCITY";

// WZD dseg:2A3F
char chriver_lbx_file[] = "CHRIVER";

// WZD dseg:2A47
char citywall_lbx_file[] = "CITYWALL";

// WZD dseg:2A50
char cityscap_lbx_file[] = "CITYSCAP";

// WZD dseg:2A59
char unitview_lbx_file[] = "UNITVIEW";

// WZD dseg:2A62
char special_lbx_file[] = "SPECIAL";

// WZD dseg:2A6A
char special2_lbx_file[] = "SPECIAL2";

// WZD dseg:2A73
char items_lbx_file[] = "ITEMS";

// WZD dseg:2A79
char itemisc_lbx_file[] = "ITEMISC";

// WZD dseg:2A81
char spellbook_small__lbx_file[] = "special2";

// WZD dseg:2A81                            END: ovr052



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

    Load_SPELLDAT();  // loads all records from SPELLDAT.LBX, overwriting the pointer to a previous allocation (8k wasted)

    Load_BUILDDAT();  // loads all records from BUILDDAT.LBX

// fxn_o52p18();
// drake178: LBX_Load_Click_SFX
// Load_Button_Sounds();  // ; loads the standard and left click sounds


    Units_Upkeeps();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_WZD_Resources()\n", __FILE__, __LINE__);
#endif
}


// WZD o52p02
// drake178: UNIT_Upkeep_Reset()
/*
    UNIT_Upkeep_Reset()
    XREF:
        NX_j_UNIT_Upkeep_Reset()
        Load_WZD_Resources()

*/
void Units_Upkeeps(void)
{

    int16_t itr;  // _CX_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Units_Upkeeps()\n", __FILE__, __LINE__);
#endif

    // TODO(JimBalcomb,20231127): figure out where/what this value is that it is using to iterate over
    
    // C4 1E 2C 91                                     les     bx, [spell_data_table]
    // 26 39 8F 64 1C                                  cmp     [es:bx+1C64h], cx
    // 7F BB                                           jg      short loc_460B7
    // AKA
    // C4 1E 2C 91                                     les     bx, [spell_data_table]
    // 26 39 8F 64 1C                                  cmp     [es:(_unit_type_table.pict_seg+1AB8h)+bx], cx
    // 7F BB                                           jg      short loc_460B7
    // ...
    // dseg:912C
    // + 1C64
    // 912C + 1C64 = AD90  44,432

    for(itr = 0; itr < 198; itr++)
    {
        if(itr >= 0x23 /* Trireme */)
        {
            _unit_type_table[itr].Upkeep = ((_unit_type_table[itr].Cost + 49) / 50);  // Dasm is doing a ceil(), somehow?
        }
        else
        {
            _unit_type_table[itr].Upkeep = ((_unit_type_table[itr].Cost + 99) / 100);  // Dasm is doing a ceil(), somehow?
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Units_Upkeeps()\n", __FILE__, __LINE__);
#endif

}

// WZD o52p03
/*
    All (Movement) Map
        TERRAIN.LBX   Entry 1,2
        TERRSTAT.LBX  Entry 1, Records 1x196
        MAPBACK.LBX   ¿ *.* ?

    Allocate_Space()
        Map_Square_WorkArea
        Warp_Node_WorkArea

... Allocate / Load / Init ...
... Terrain / Map Data ...
... Movement / World ...
*/
void Terrain_Init(void)
{
    SAMB_ptr tmp_sa_TBL_Unrest = 0;
    int16_t itr = 0;
    uint64_t tmp_address = 0;

    // 
    Map_Square_WorkArea = Allocate_Space(70);  // 70 PR, 1120 B

    m_terrain_lbx_001 = (uint16_t *)LBX_Load(terrain_lbx_file, 1);
    m_terrain_lbx_002 = LBX_Load(terrain_lbx_file, 2);
    // ehn_terrain_lbx = EMM_Load_LBX_File("TERRAIN.LBX", 1);
    // HACK:  no EMM, so just load entry and monkey with offset adjustments
    // TODO  make this an allocate and reload and set another pointer to the offset, so it looks like it did and dont need the math elsewhere - e.g., Draw_Map_Terrain()
    m_terrain_lbx_000 = LBX_Load(terrain_lbx_file, 0);  // 676416 B


    tmp_sa_TBL_Unrest = LBX_Load_Data(terrstat_lbx_file, 1, 0, 1, 196);
    TBL_Unrest[0] = (int8_t *)tmp_sa_TBL_Unrest;
    for(itr = 1; itr < NUM_RACES; itr++)
    {
        // ¿¿¿
        TBL_Unrest[itr] = ((int8_t *)TBL_Unrest[(itr - 1)]) + NUM_RACES;
        // ???
    }


    // Loop MAPBACK 0 to 13:
    for(itr = 1; itr < 15; itr++)
    {
        itr--;
        unexplored_mask_seg[itr] = LBX_Load(rsc0C_MAPBACK_LBX, itr);
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
    tower_owned_seg   = LBX_Load(mapback_lbx_file, 70);     // ; reserved EMM header pointer for a single image


    // MAPBACK.LBX, 071     SITES    mound
    // MAPBACK.LBX, 074     SITES    ruins
    // MAPBACK.LBX, 072     SITES    temple
    // MAPBACK.LBX, 073     SITES    keep
    // MAPBACK.LBX, 071     SITES    mound
    // MAPBACK.LBX, 074     SITES    ruins
    // MAPBACK.LBX, 075     SITES    fallen temple

    IMG_OVL_EZ_Cave  = LBX_Load(mapback_lbx_file, 71);  // ~ EZ_Cave
    IMG_OVL_EZ_Dung  = LBX_Load(mapback_lbx_file, 74);  // ~ EZ_Dungeon
    IMG_OVL_EZ_ATmpl = LBX_Load(mapback_lbx_file, 72);  // ~ EZ_Ancient_Temple
    IMG_OVL_EZ_AKeep = LBX_Load(mapback_lbx_file, 73);  // ~ EZ_Keep
    IMG_OVL_EZ_MLair = LBX_Load(mapback_lbx_file, 71);  // ~ EZ_Monster_Lair
    IMG_OVL_EZ_Ruins = LBX_Load(mapback_lbx_file, 74);  // ~ EZ_Ruins
    IMG_OVL_EZ_FTmpl = LBX_Load(mapback_lbx_file, 75);  // ~ EZ_Fallen_Temple


    // MAPBACK.LBX, 078     SITES   coal
    // MAPBACK.LBX, 079     SITES   iron
    // MAPBACK.LBX, 080     SITES   silver
    // MAPBACK.LBX, 081     SITES   gold
    // MAPBACK.LBX, 082     SITES   gems
    // MAPBACK.LBX, 083     SITES   mithril
    // MAPBACK.LBX, 084     SITES   adamantium
    // MAPBACK.LBX, 085     SITES   quork
    // MAPBACK.LBX, 086     SITES   crysx

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
        
        IMG_OVL_Nightshade = LBX_Load(mapback_lbx_file, 91);
        IMG_OVL_WildGame = LBX_Load(mapback_lbx_file, 92);
        
        node_warped_seg = LBX_Load(mapback_lbx_file, 93);

}


// WZD o52p04
// fxn_o52p04()


// WZD o52p05
void Main_Screen_Load_Pictures(void)
{
    int16_t itr;

    main_background_seg = LBX_Load(main_lbx_file,  0);

    main_game_button   = LBX_Load(main_lbx_file, 1);
    main_spells_button = LBX_Load(main_lbx_file, 2);
    main_armies_button = LBX_Load(main_lbx_file, 3);
    main_cities_button = LBX_Load(main_lbx_file, 4);
    main_magic_button  = LBX_Load(main_lbx_file, 5);
    main_info_button   = LBX_Load(main_lbx_file, 6);
    main_plane_button  = LBX_Load(main_lbx_file, 7);

    main_done_button   = LBX_Load(main_lbx_file, 8);
    main_patrol_button = LBX_Load(main_lbx_file, 9);
    main_wait_button   = LBX_Load(main_lbx_file, 10);
    main_build_button  = LBX_Load(main_lbx_file, 11);
    main_purify_button = LBX_Load(main_lbx_file, 42);
    main_meld_button   = LBX_Load(main_lbx_file, 49);

    main_lock_done_button   = LBX_Load(main_lbx_file, 12);
    main_lock_patrol_button = LBX_Load(main_lbx_file, 13);
    main_lock_wait_button   = LBX_Load(main_lbx_file, 14);
    main_lock_build_button  = LBX_Load(main_lbx_file, 15);
    main_lock_purify_button = LBX_Load(main_lbx_file, 43);

    main_white_medal_icon       = LBX_Load(main_lbx_file, 51);
    main_gold_medal_icon        = LBX_Load(main_lbx_file, 52);
    main_red_medal_icon         = LBX_Load(main_lbx_file, 53);
    main_magic_weapon_icon      = LBX_Load(main_lbx_file, 54);
    main_mithril_weapon_icon    = LBX_Load(main_lbx_file, 55);
    main_adamantium_weapon_icon = LBX_Load(main_lbx_file, 56);

    move_sail_icon   = LBX_Load(main_lbx_file, 18);
    move_swim_icon   = LBX_Load(main_lbx_file, 19);
    move_mt_icon     = LBX_Load(main_lbx_file, 20);
    move_forest_icon = LBX_Load(main_lbx_file, 21);
    move_fly_icon    = LBX_Load(main_lbx_file, 22);
    move_path_icon   = LBX_Load(main_lbx_file, 23);
    move_astral_icon = LBX_Load(main_lbx_file, 36);
    move_wind_icon   = LBX_Load(main_lbx_file, 37);
    move_boot_icon   = LBX_Load(main_lbx_file, 38);

    movement_mode_icons[0] = LBX_Load(main_lbx_file, 18);
    movement_mode_icons[1] = LBX_Load(main_lbx_file, 19);
    movement_mode_icons[2] = LBX_Load(main_lbx_file, 20);
    movement_mode_icons[3] = LBX_Load(main_lbx_file, 21);
    movement_mode_icons[4] = LBX_Load(main_lbx_file, 22);
    movement_mode_icons[5] = LBX_Load(main_lbx_file, 23);
    movement_mode_icons[6] = LBX_Load(main_lbx_file, 36);
    movement_mode_icons[7] = LBX_Load(main_lbx_file, 37);
    movement_mode_icons[8] = LBX_Load(main_lbx_file, 38);
    // movement_mode_icons[9] ¿ drake178: "Cavalry" ?


    for(itr = 0; itr < 9; itr++)
    {
        unit_backgrounds[itr] = LBX_Load(main_lbx_file, 24 + itr);
    }

    // MAIN.LBX, 058    DESELECT    next turn button..
    // MAIN.LBX, 034    DESELECT    deselect backgrnd
    // MAIN.LBX, 035    DESELECT    next turn backgrnd
    next_turn_button_seg     = LBX_Load(main_lbx_file, 58);
    deselect_background      = LBX_Load(main_lbx_file, 34);
    next_turn_background_seg = LBX_Load(main_lbx_file, 35);

    // MAIN.LBX, 040    MAINCAST    cast background
    // MAIN.LBX, 041    CASTCNCL    cast cancel button
    // MAIN.LBX, 047    C&RBORDR    cast button border
    cast_background    = LBX_Load(main_lbx_file, 40);
    cast_cancel_button = LBX_Load(main_lbx_file, 41);
    cast_button_border = LBX_Load(main_lbx_file, 47);

    // MAIN.LBX, 044    DESELECT    button blockout
    deselect_button_blockout = LBX_Load(main_lbx_file, 44);

    // MAIN.LBX, 045    MAINROAD    road background
    // MAIN.LBX, 046    CASTCNCL    road ok button
    // MAIN.LBX, 048    C&RBORDR    road button border
    road_background    = LBX_Load(main_lbx_file, 45);
    road_ok_button     = LBX_Load(main_lbx_file, 46);
    road_button_border = LBX_Load(main_lbx_file, 48);

    // MAIN.LBX, 057    MAINSRVY    survey backgrnd
    survey_background = LBX_Load(main_lbx_file, 57);

    // BACKGRND.LBX, 04  MAGEVIEW    wizardview backgrn
    mirror_screen_background_seg = LBX_Load(backgrnd_lbx_file, 4);

    // MAIN.LBX, 050    GOTO        goto booty
    goto_booty_icon = LBX_Load(main_lbx_file, 50);


    // MAIN.LBX, 059    LILEVENT    blue
    // MAIN.LBX, 060    LILEVENT    red
    // MAIN.LBX, 061    LILEVENT    grn
    // MAIN.LBX, 062    LILEVENT    bad
    // MAIN.LBX, 063    LILEVENT    good
    // MAIN.LBX, 064    LILEVENT    short
    for(itr = 0; itr < 6; itr++)
    {
        main_lilevent_icons[itr] = LBX_Load(main_lbx_file, (59 + itr));
    }

}


// WZD o52p06
/*
    loads all pics for City Screen
    and houses for Outpost Screen
    and confirmation box for Lair
*/
void City_Screen_Load_Pictures(void)
{
    int16_t itr;  // _SI_

    // BACKGRND.LBX,  6  CITYBACK  city backgrnd
    // BACKGRND.LBX,  7  CITYBUTT  city buy button
    // BACKGRND.LBX,  8  CITYBUTT  city change button
    // BACKGRND.LBX,  9  CITYBUTT  city ok button
    // BACKGRND.LBX, 10  BLDGRASS  newbuildnotifygras
    // BACKGRND.LBX, 11  CITYBLD   reqd city resource
    // BACKGRND.LBX, 12  CITYBLD   full city resource
    // BACKGRND.LBX, 13  CITYBITS  city block out
    // BACKGRND.LBX, 14  CITYBITS  lock city buy
    // BACKGRND.LBX, 15  CITYBUTT  city spell up arro
    // BACKGRND.LBX, 16  CITYBUTT  city spell dn arro

    city_background_seg              = LBX_Reload_Next(backgrnd_lbx_file, 6, GFX_Swap_Seg);
    city_buy_button_seg              = LBX_Reload_Next(backgrnd_lbx_file, 7, GFX_Swap_Seg);
    city_change_button_seg           = LBX_Reload_Next(backgrnd_lbx_file, 8, GFX_Swap_Seg);
    city_ok_button_seg               = LBX_Reload_Next(backgrnd_lbx_file, 9, GFX_Swap_Seg);
    city_new_build_notify_grass_seg  = LBX_Reload_Next(backgrnd_lbx_file, 10, GFX_Swap_Seg);
    city_reqd_resource_seg           = LBX_Reload_Next(backgrnd_lbx_file, 11, GFX_Swap_Seg);
    city_full_resource_seg           = LBX_Reload_Next(backgrnd_lbx_file, 12, GFX_Swap_Seg);
    city_block_out_seg               = LBX_Reload_Next(backgrnd_lbx_file, 13, GFX_Swap_Seg);
    city_lock_buy_button_seg         = LBX_Reload_Next(backgrnd_lbx_file, 14, GFX_Swap_Seg);
    city_spell_up_arrow_button_seg   = LBX_Reload_Next(backgrnd_lbx_file, 15, GFX_Swap_Seg);
    city_spell_dn_arrow_button_seg   = LBX_Reload_Next(backgrnd_lbx_file, 16, GFX_Swap_Seg);

    // BACKGRND.LBX, 24  REDBUTT  red button
    red_button_seg = LBX_Reload_Next(backgrnd_lbx_file, 24, GFX_Swap_Seg);

    // BACKGRND.LBX, 45  CITYICON  worker barbarian
    // BACKGRND.LBX, 46  CITYICON  worker beastman
    // BACKGRND.LBX, 47  CITYICON  worker darkelf
    // BACKGRND.LBX, 48  CITYICON  worker draconian
    // BACKGRND.LBX, 49  CITYICON  worker dwarf
    // BACKGRND.LBX, 50  CITYICON  worker gnoll
    // BACKGRND.LBX, 51  CITYICON  worker halfling
    // BACKGRND.LBX, 52  CITYICON  worker highelf
    // BACKGRND.LBX, 53  CITYICON  worker highmen
    // BACKGRND.LBX, 54  CITYICON  worker klackon
    // BACKGRND.LBX, 55  CITYICON  worker lizardman
    // BACKGRND.LBX, 56  CITYICON  worker nomad
    // BACKGRND.LBX, 57  CITYICON  worker orc
    // BACKGRND.LBX, 58  CITYICON  worker troll

    for(itr = 0; itr < 14; itr++)
    {
        city_worker_icon_seg[itr] = LBX_Reload_Next(backgrnd_lbx_file, (45 + itr), GFX_Swap_Seg);
    }

    // BACKGRND.LBX, 59  CITYICON  farmer barbarian
    // BACKGRND.LBX, 60  CITYICON  farmer beastman
    // BACKGRND.LBX, 61  CITYICON  farmer darkelf
    // BACKGRND.LBX, 62  CITYICON  farmer draconian
    // BACKGRND.LBX, 63  CITYICON  farmer dwarf
    // BACKGRND.LBX, 64  CITYICON  farmer gnoll
    // BACKGRND.LBX, 65  CITYICON  farmer halfling
    // BACKGRND.LBX, 66  CITYICON  farmer highelf
    // BACKGRND.LBX, 67  CITYICON  farmer highmen
    // BACKGRND.LBX, 68  CITYICON  farmer klackon
    // BACKGRND.LBX, 69  CITYICON  farmer lizardman
    // BACKGRND.LBX, 70  CITYICON  farmer nomad
    // BACKGRND.LBX, 71  CITYICON  farmer orc
    // BACKGRND.LBX, 72  CITYICON  farmer troll

    for(itr = 0; itr < 14; itr++)
    {
        city_farmer_icon_seg[itr] = LBX_Reload_Next(backgrnd_lbx_file, (59 + itr), GFX_Swap_Seg);
    }

    // BACKGRND.LBX, 74  CITYICON  barbarian rebel
    // BACKGRND.LBX, 75  CITYICON  beastmanrebel
    // BACKGRND.LBX, 76  CITYICON  darkelf rebel
    // BACKGRND.LBX, 77  CITYICON  draconian rebel
    // BACKGRND.LBX, 78  CITYICON  dwarf rebel
    // BACKGRND.LBX, 79  CITYICON  gnoll rebel
    // BACKGRND.LBX, 80  CITYICON  halfling rebel
    // BACKGRND.LBX, 81  CITYICON  highelf rebel
    // BACKGRND.LBX, 82  CITYICON  highmen rebel
    // BACKGRND.LBX, 83  CITYICON  klackon rebel
    // BACKGRND.LBX, 84  CITYICON  lizardman rebel
    // BACKGRND.LBX, 85  CITYICON  nomad rebel
    // BACKGRND.LBX, 86  CITYICON  orc rebel
    // BACKGRND.LBX, 87  CITYICON  troll rebel

    for(itr = 0; itr < 14; itr++)
    {
        city_rebel_icon_seg[itr] = LBX_Reload_Next(backgrnd_lbx_file, (74 + itr), GFX_Swap_Seg);
    }

    // BACKGRND.LBX, 40  CITYICON  food icon
    // BACKGRND.LBX, 41  CITYICON  production icon
    // BACKGRND.LBX, 42  CITYICON  gold icon
    // BACKGRND.LBX, 43  CITYICON  power icon
    // BACKGRND.LBX, 44  CITYICON  research icon

    for(itr = 0; itr < 5; itr++)
    {
        city_lil_resource_icon_seg[itr] = LBX_Reload_Next(backgrnd_lbx_file, (40 + itr), GFX_Swap_Seg);
    }

    // BACKGRND.LBX, 94  CITYICON  grey lil bread
    city_grey_lil_bread_icon_seg = LBX_Reload_Next(backgrnd_lbx_file, 94, GFX_Swap_Seg);

    // BACKGRND.LBX, 88  CITYICO3  10 food
    // BACKGRND.LBX, 89  CITYICO3  10 prod
    // BACKGRND.LBX, 90  CITYICO3  10 gold
    // BACKGRND.LBX, 91  CITYICO3  10 power
    // BACKGRND.LBX, 92  CITYICO3  10 research
    // BACKGRND.LBX, 93  CITYICO3  10 black gold

    for(itr = 0; itr < 6; itr++)
    {
        city_big_resource_icon_seg[itr] = LBX_Reload_Next(backgrnd_lbx_file, (88 + itr), GFX_Swap_Seg);
    }

    // BACKGRND.LBX, 95  CITYICO3  grey big bread
    city_grey_big_bread_icon_seg = LBX_Reload_Next(backgrnd_lbx_file, 95, GFX_Swap_Seg);


    // BACKGRND.LBX, 32  OUTPOST  outpost background
    outpost_background_seg = LBX_Reload_Next(backgrnd_lbx_file, 32, GFX_Swap_Seg);

    // BACKGRND.LBX, 33  OUTNAME  outpost name backg
    outpost_name_background_seg = LBX_Reload_Next(backgrnd_lbx_file, 33, GFX_Swap_Seg);


    // BACKGRND.LBX, 34  OUTPOST  outpost brt house
    // BACKGRND.LBX, 35  OUTPOST  outpost brt tree
    // BACKGRND.LBX, 36  OUTPOST  outpost brt hut
    for(itr = 0; itr < 3; itr++)
    {
        outpost_bright_icon_seg[itr] = LBX_Reload_Next(backgrnd_lbx_file, (34 + itr), GFX_Swap_Seg);
    }

    // BACKGRND.LBX, 37  OUTPOST  outpost drk house
    // BACKGRND.LBX, 38  OUTPOST  outpost drk tree
    // BACKGRND.LBX, 39  OUTPOST  outpost drk hut
    for(itr = 0; itr < 3; itr++)
    {
        outpost_dark_icon_seg[itr] = LBX_Reload_Next(backgrnd_lbx_file, (37 + itr), GFX_Swap_Seg);
    }

    // BACKGRND.LBX, 25  LCONFIRM  lair confirm borde
    // BACKGRND.LBX, 26  LCONFIRM  lair bottom w/butt
    // BACKGRND.LBX, 27  LCONFIRM  lair bottom wo/but
    lair_confirm_border_seg = LBX_Reload_Next(backgrnd_lbx_file, 25, GFX_Swap_Seg);
    lair_bottom_with_button_seg = LBX_Reload_Next(backgrnd_lbx_file, 26, GFX_Swap_Seg);
    lair_bottom_without_button_seg = LBX_Reload_Next(backgrnd_lbx_file, 27, GFX_Swap_Seg);

    // BACKGRND.LBX,  0  CITYHALF  1/2 for city scrn
    city_half_background_seg = LBX_Reload_Next(backgrnd_lbx_file, 0, GFX_Swap_Seg);

}


// WZD o52p07
// fxn_o52p07()

// WZD o52p08
// fxn_o52p08()

// WZD o52p09
// fxn_o52p09()

// WZD o52p10
// fxn_o52p10()

// WZD o52p11
// fxn_o52p11()

// WZD o52p12
// fxn_o52p12()

// WZD o52p13
// drake178: ¿ ?
/*
; loads the standard and left click sounds
*/
void Load_Button_Sounds(void)
{

    // SOUNDFX.LBX, 000    SLX19 VO    Standard Button click
    snd_standard_button_click = LBX_Load(soundfx_lbx_file__ovr052, 0);

    // SOUNDFX.LBX, 002    OKBUTTN     left button click
    snd_left_button_click = LBX_Load(soundfx_lbx_file__ovr052, 2);

}

// WZD o52p14
// fxn_o52p14()

// WZD o52p15
// fxn_o52p15()


// WZD o52p16
void Load_SPELLDAT(void)
{
    spell_data_table = (struct s_SPELL_DATA *)LBX_Load_Data(spelldat_lbx_file, 0, 0, 215, 36);
}


// WZD o52p17
void Load_BUILDDAT(void)
{
    bldg_data_table = (struct s_BLDG_DATA *)LBX_Load_Data(builddat_lbx_file, 0, 0, 36, 52);
}


// WZD o52p18
// fxn_o52p18()

// WZD o52p19
// Load_Combat_Background_Bottom()


// WZD o52p20
void GFX_Swap_Reset(void)
{
    Allocate_First_Block(GFX_Swap_Seg, 1);
}


// WZD o52p21
// UU_WrldDatBuf_Reset()


// WZD o52p22
// AKA GFX_Swap_AppendUnits()
void Load_Unit_StatFigs(void)
{
    uint16_t itr;

    for(itr = 0; itr < 120; itr++)
    {
        _unit_type_table[itr].pict_seg = LBX_Reload_Next(units1_lbx_file, itr, GFX_Swap_Seg);
    }

    for(itr = 120; itr < 198; itr++)
    {
        _unit_type_table[itr].pict_seg = LBX_Reload_Next(units2_lbx_file, (itr - 120), GFX_Swap_Seg);
    }

}


// WZD o52p23
// GFX_Swap_AppendCmbt()

// WZD o52p24
// fxn_o52p24()

// WZD o52p25
void GFX_Swap_AppndCtScap__WIP(void)
{
    int16_t itr1;  // _SI_
    int16_t itr2;  // _DI_

    // CITYSCAP.LBX,   0  BACKS      ground background
    cityscape_background_arcanus_ground_seg = LBX_Reload_Next(cityscap_lbx_file, 0, GFX_Swap_Seg);


    // CITYSCAP.LBX,  87  CITYSPL1    flying fortress
    // CITYSCAP.LBX,  90  CITYSPL3    gaia
    cityscape_flyingfortress_mask_seg = LBX_Reload_Next(cityscap_lbx_file, 87, GFX_Swap_Seg);
    cityscape_gaiasblessing_mask_seg = LBX_Reload_Next(cityscap_lbx_file, 90, GFX_Swap_Seg);

    // CITYSCAP.LBX,  89  CITYSPL2    living lands
    // CITYSCAP.LBX,  86  CITYSPL1    cursed land mask
    cityscape_livinglands_mask_seg = LBX_Reload_Next(cityscap_lbx_file, 89, GFX_Swap_Seg);
    cityscape_cursedland_mask_seg = LBX_Reload_Next(cityscap_lbx_file, 86, GFX_Swap_Seg);


    // CITYSCAP.LBX,   3  CSWATER     river
    cityscape_background_arcanus_river_seg = LBX_Reload_Next(cityscap_lbx_file, 3, GFX_Swap_Seg);

    // CITYSCAP.LBX,   4  CSWATER     ocean
    cityscape_background_arcanus_ocean_seg = LBX_Reload_Next(cityscap_lbx_file, 4, GFX_Swap_Seg);

    // CITYSCAP.LBX,   1  FARBACKS    hills background
    cityscape_background_arcanus_hills_seg = LBX_Reload_Next(cityscap_lbx_file, 1, GFX_Swap_Seg);

    // CITYSCAP.LBX,   2  FARBACKS    mountain backgrnd
    cityscape_background_arcanus_mountain_seg = LBX_Reload_Next(cityscap_lbx_file, 2, GFX_Swap_Seg);

    // CITYSCAP.LBX,  91  FARBACKS    cloud of dark
    cityscape_background_arcanus_darkcloud_seg = LBX_Reload_Next(cityscap_lbx_file, 91, GFX_Swap_Seg);

    // CITYSCAP.LBX,  93  FARBACKS    alkar
    cityscape_background_arcanus_alkar_seg = LBX_Reload_Next(cityscap_lbx_file, 93, GFX_Swap_Seg);

    // CITYSCAP.LBX,  92  FARBACKS    chaos rift
    cityscape_background_arcanus_chaosrift_seg = LBX_Reload_Next(cityscap_lbx_file, 92, GFX_Swap_Seg);


    // CITYSCAP.LBX,  88  CITYSPL2    famine
    cityscape_famine_mask_seg = LBX_Reload_Next(cityscap_lbx_file, 88, GFX_Swap_Seg);



/*

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 7
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_Arc_GrassBG@], ax

*/



    // CITYSCAP.LBX,   8  KAOSCAPE    chaos ground
    cityscape_background_myrror_ground_seg = LBX_Reload_Next(cityscap_lbx_file, 8, GFX_Swap_Seg);

    // CITYSCAP.LBX,   9  KAOBACKS    choas hills
    cityscape_background_myrror_mountain_seg = LBX_Reload_Next(cityscap_lbx_file, 9, GFX_Swap_Seg);

    // CITYSCAP.LBX,  10  KAOBACKS    chaos mountain
    cityscape_background_myrror_hills_seg = LBX_Reload_Next(cityscap_lbx_file, 10, GFX_Swap_Seg);



/*

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 11
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_Myr_GrassBG@], ax
push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 113
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_Myr_HLight@], ax

*/

    // CITYSCAP.LBX, 111  KAOBACKS    choas dark cloud
    cityscape_background_myrror_darkcloud_seg = LBX_Reload_Next(cityscap_lbx_file, 111, GFX_Swap_Seg);

    // CITYSCAP.LBX, 112  KAOBACKS    choas chaos rift
    // cityscape_background_myrror_chaosrift_seg = LBX_Reload_Next(cityscap_lbx_file, 112, GFX_Swap_Seg);

/*

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 112
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_Myr_CRift@], ax

*/


    // CITYSCAP.LBX, 115  CHWATER     chaos river
    cityscape_background_myrror_river_seg = LBX_Reload_Next(cityscap_lbx_file, 115, GFX_Swap_Seg);

    // CITYSCAP.LBX, 116  CHWATER     chaos ocean
    cityscape_background_myrror_ocean_seg = LBX_Reload_Next(cityscap_lbx_file, 116, GFX_Swap_Seg);



/*

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 94
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_DeathWard@], ax
push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 95
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_ChaosWard@], ax
push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 98
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_NatWard@], ax
push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 96
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_LifeWard@], ax
push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 97
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_SorcWard@], ax
push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 99
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_NatsEye@], ax
push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 100
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_Inspiratn@], ax
push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 101
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6
mov     [IMG_CTY_Prosperity@], ax
push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 102
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6


mov     [IMG_CTY_Consecrate@], ax


push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 77
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next

add     sp, 6


mov     [IMG_CTY_WallofFire@], ax

    // CITYSCAP.LBX,  79  CSWALLS    wall of air
    IMG_CTY_WallofDark = LBX_Reload_Next(cityscap_lbx_file, 79, GFX_Swap_Seg);

*/

    /*
        loads building picts in of bldg idx
            bldg_picts_seg[{3, ..., 30}]
            bldg_picts_seg[31]
            bldg_picts_seg[{32,33}]
        the array skips elements {0,1,2} for None, Housing, and TradeGoods
    */
    // CITYSCAP.LBX,  45  ANIBUILD    barracks
    // ...
    // CITYSCAP.LBX,  71  BUILDS3     granary
    // CITYSCAP.LBX,  72  BUILDS3     farmer's market
    for(itr1 = 3; itr1 <= 30; itr1++)
    {
        bldg_picts_seg[itr1] = LBX_Reload_Next(cityscap_lbx_file, (42 + itr1), GFX_Swap_Seg);
    }

    // CITYSCAP.LBX,  78  BUILDS2     forester's guild
    bldg_picts_seg[31] = LBX_Reload_Next(cityscap_lbx_file, 78, GFX_Swap_Seg);
    // 34th element  bt_ForestersGuild     = 31

    // CITYSCAP.LBX,  73  BUILDS2     builder's hall
    // CITYSCAP.LBX,  74  BUILDS2     mechanician's guil
    // CITYSCAP.LBX,  75  BUILDS2     miner's guild
    for(itr1 = 32; itr1 <= 34; itr1++)
    {
        bldg_picts_seg[itr1] = LBX_Reload_Next(cityscap_lbx_file, (41 + itr1), GFX_Swap_Seg);
    }


    // CITYSCAP.LBX,  5  BACKS       roads
    cityscape_roads_seg = LBX_Reload_Next(cityscap_lbx_file, 5, GFX_Swap_Seg);


    // {{25,26,27,28,29},{30,31,32,33,34},{35,36,37,38,39}}
    // CITYSCAP.LBX,  25  HOUSES      house1
    // ...
    // CITYSCAP.LBX,  30  HOUSES      tree house1
    // ...
    // CITYSCAP.LBX,  35  HOUSES      mound1

    for(itr1 = 0; itr1 < 3; itr1++)
    {
        for(itr2 = 0; itr2 < 5; itr2++)
        {
            cityscape_houses_seg[((itr1 * 5) + itr2)] = LBX_Reload_Next(cityscap_lbx_file, (25 + (itr1 * 5) + itr2), GFX_Swap_Seg);
        }
    }


    // CITYSCAP.LBX, 19  HOUSES      tree1
    // CITYSCAP.LBX, 20  HOUSES      tree2
    // CITYSCAP.LBX, 21  HOUSES      tree3

    for(itr1 = 0; itr1 < 3; itr1++)
    {
        cityscape_trees_seg[itr1] = LBX_Reload_Next(cityscap_lbx_file, (19 + itr1), GFX_Swap_Seg);
    }

    // CITYSCAP.LBX, 22  HOUSES      rocks1
    // CITYSCAP.LBX, 23  HOUSES      rocks2
    // CITYSCAP.LBX, 24  HOUSES      rocks3

    for(itr1 = 0; itr1 < 3; itr1++)
    {
        cityscape_rocks_seg[itr1] = LBX_Reload_Next(cityscap_lbx_file, (22 + itr1), GFX_Swap_Seg);
    }


    // CITYSCAP.LBX,  6  BUILDS4     summon circle
    cityscape_summon_circle_seg = LBX_Reload_Next(cityscap_lbx_file, 6, GFX_Swap_Seg);


/*

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 83
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next
add     sp, 6
mov     [IMG_CTY_Earth_Gate@], ax

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 84
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next
add     sp, 6
mov     [IMG_CTY_StreamofLif@], ax

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 85
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next
add     sp, 6
mov     [IMG_CTY_Astral_Gate@], ax

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 12
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next
add     sp, 6
mov     [IMG_CTY_AltarofBtl@], ax

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 81
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next
add     sp, 6
mov     [IMG_CTY_Dark_Ritual@], ax

push    [GFX_Swap_Seg]                  ; base_seg
mov     ax, 82
push    ax                              ; entry_num
mov     ax, offset cityscap_lbx_file    ; "CITYSCAP"
push    ax                              ; file_name
call    LBX_Reload_Next
add     sp, 6
mov     [IMG_CTY_EvilPresnc@], ax
*/


    // CITYSCAP.LBX,  76  CSWALLS     city walls
    cityscape_city_walls_seg = LBX_Reload_Next(cityscap_lbx_file, 76, GFX_Swap_Seg);


    // CITYSCAP.LBX,  40  ANIBUILD    fortress
    cityscape_fortress_seg = LBX_Reload_Next(cityscap_lbx_file, 40, GFX_Swap_Seg);


    // CITYSCAP.LBX,  41  BUILDS1     trade goods
    building_trade_goods_seg = LBX_Reload_Next(cityscap_lbx_file, 41, GFX_Swap_Seg);


    // CITYSCAP.LBX,  42  BUILDS1     stnd housing
    bldg_housing_seg[0] = LBX_Reload_Next(cityscap_lbx_file, 42, GFX_Swap_Seg);

    // CITYSCAP.LBX,  43  BUILDS1     tree housing
    bldg_housing_seg[1] = LBX_Reload_Next(cityscap_lbx_file, 43, GFX_Swap_Seg);

    // CITYSCAP.LBX,  44  BUILDS1     mound housing
    bldg_housing_seg[2] = LBX_Reload_Next(cityscap_lbx_file, 44, GFX_Swap_Seg);


    // UNITVIEW.LBX,  
    unit_water_diamond_seg = LBX_Reload_Next(unitview_lbx_file, 34, GFX_Swap_Seg);

    // CITYSCAP.LBX,  103  BUILDS4     untit base
    unit_grass_diamond_seg = LBX_Reload_Next(cityscap_lbx_file, 103, GFX_Swap_Seg);


    // CITYSCAP.LBX, 104  CITYSPL4    city walls build
    IDK_CITYSPL4_city_walls_build_seg = LBX_Reload_Next(cityscap_lbx_file, 104, GFX_Swap_Seg);


    // IMG_CTY_Rubble@


    // CITYSCAP.LBX,  109  BUILDS1     horizontal mask
    IDK_BUILDS1_horizontal_mask_seg = LBX_Reload_Next(cityscap_lbx_file, 109, GFX_Swap_Seg);

    // CITYSCAP.LBX,  110  BUILDS1     vertical mask
    IDK_BUILDS1_vertical_mask_seg = LBX_Reload_Next(cityscap_lbx_file, 110, GFX_Swap_Seg);

    // CITYSCAP.LBX,  114  BUILDS4     big city wall
    cityscape_big_city_wall_seg = LBX_Reload_Next(cityscap_lbx_file, 114, GFX_Swap_Seg);

    // BACKGRND.LBX,  73  CITYICON    black gold
    black_gold1_icon_seg = LBX_Reload_Next(backgrnd_lbx_file, 73, GFX_Swap_Seg);

}


// WZD o52p26
// fxn_o52p6()


// WZD o52p27
void GFX_Swap_AppendUView(void)
{
    int16_t itr;

    // load [111of111] from SPECIAL.LBX
    // { UNICON1, UNICON2, UNICON3, NEWICON, UNICON3, UNICON4, UNICON5 }
    // includes unit/her level medal icons  {102, ..., 110}
    for(itr = 0; itr < 111; itr++)
    {
        // IMG_USW_Abilities.Teleporting@
        special_seg[itr] = LBX_Reload_Next(special_lbx_file, itr, GFX_Swap_Seg);
    }

    // load [(0to33)of44] from SPECIAL2.LBX
    // { UNICON6, UNICON7 }
    for(itr = 0; itr < 34; itr++)
    {
        // IMG_USW_Abilities.Confusion@
        special_seg[(111 + itr)] = LBX_Reload_Next(special2_lbx_file, itr, GFX_Swap_Seg);
    }



    // load {0,1,2,3,4} from UNITVIEW.LBX

    // UNITBACK small unit backgrn
    unitview_small_background_seg = LBX_Reload_Next(unitview_lbx_file, 0, GFX_Swap_Seg);
    // FULLUNIT large unit backgrn
    unitview_large_background_seg = LBX_Reload_Next(unitview_lbx_file, 1, GFX_Swap_Seg);
    // BLDBUTBK button background
    unitview_button_background_seg = LBX_Reload_Next(unitview_lbx_file, 2, GFX_Swap_Seg);
    // UNITBUTT up arrow
    unitview_up_arrow_seg = LBX_Reload_Next(unitview_lbx_file, 3, GFX_Swap_Seg);
    // UNITBUTT down arrow
    unitview_down_arrow_seg = LBX_Reload_Next(unitview_lbx_file, 4, GFX_Swap_Seg);


    // load [5,12] from UNITVIEW.LBX
    /*
        UNITDARK dark special borde
        UNITDARK dark special borde
        UNITDARK dark special borde
        UNITDARK dark special borde
        UNITDARK dark special borde
        UNITDARK dark special borde
        UNITDARK dark special borde
        UNITDARK dark special borde
    */
    for(itr = 0; itr < 8; itr++)
    {
        unitview_specials_borders_seg[itr] = LBX_Reload_Next(unitview_lbx_file, (5 + itr), GFX_Swap_Seg);
    }


    // load [13,27] from UNITVIEW.LBX
    /*
        SMALICON    sword
        SMALICON    fireball
        SMALICON    mithril sword
        SMALICON    magic sword
        SMALICON    adamantium sword
        SMALICON    bow
        SMALICON    rock
        SMALICON    flame breath
        SMALICON    thrown axe
        SMALICON    defense
        SMALICON    hits
        SMALICON    walking
        SMALICON    fly
        SMALICON    swim
        SMALICON    resistance
    */
    for(itr = 0; itr < 15; itr++)
    {
        unitview_stat_icons_seg[itr] = LBX_Reload_Next(unitview_lbx_file, (13 + itr), GFX_Swap_Seg);
    }


    // load [35,49] from UNITVIEW.LBX
    /*
        SMALICON    sword
        SMALICON    fireball
        SMALICON    mithril
        SMALICON    magic sword
        SMALICON    adam
        SMALICON    bow
        SMALICON    rock
        SMALICON    breath
        SMALICON    axe
        SMALICON    shield
        SMALICON    hits
        SMALICON    blank
        SMALICON    blank
        SMALICON    blank
        SMALICON    resist
    */
    for(itr = 0; itr < 15; itr++)
    {
        unitview_stat_gold_icons_seg[itr] = LBX_Reload_Next(unitview_lbx_file, (35 + itr), GFX_Swap_Seg);
    }


    _unitlist_background_seg = LBX_Reload_Next(unitview_lbx_file, 28, GFX_Swap_Seg);
    _unitlist_bottom_seg = LBX_Reload_Next(unitview_lbx_file, 29, GFX_Swap_Seg);

    // UNITVIEW.LBX,  30  BLDBUTB2    bld button backgrn
    IMG_OVL_BuildBtn_BG = LBX_Reload_Next(unitview_lbx_file, 30, GFX_Swap_Seg);

    red_button_seg = LBX_Reload_Next(backgrnd_lbx_file, 24, GFX_Swap_Seg);

    black_gold1_icon_seg = LBX_Reload_Next(backgrnd_lbx_file, 73, GFX_Swap_Seg);
    gold1_icon_seg = LBX_Reload_Next(backgrnd_lbx_file, 42, GFX_Swap_Seg);

    // UNITVIEW.LBX,  31  BUILDTAB    left city bld tab
    IMG_CTY_LeftBldTab = LBX_Reload_Next(unitview_lbx_file, 31, GFX_Swap_Seg);

    // UNITVIEW.LBX,  32  BUILDTAB    right city bld tab
    IMG_CTY_RightBldTab = LBX_Reload_Next(unitview_lbx_file, 32, GFX_Swap_Seg);

    // UNITVIEW.LBX,  33  PORTBORD    portrait border
    IMG_USW_Portrt_Brdr = LBX_Reload_Next(unitview_lbx_file, 33, GFX_Swap_Seg);

    // UNITVIEW.LBX,  34  WATERDIA    unit water diamond
    unit_water_diamond_seg = LBX_Reload_Next(unitview_lbx_file, 34, GFX_Swap_Seg);

    // CITYSCAP.LBX, 103  BUILDS4     untit base
    unit_grass_diamond_seg = LBX_Reload_Next(cityscap_lbx_file, 103, GFX_Swap_Seg);

    /*
        40  CITYICON    food icon
        41  CITYICON    production icon
        42  CITYICON    gold icon
        43  CITYICON    power icon
        44  CITYICON    research icon
    */
    for(itr = 0; itr < 5; itr++)
    {
        city_lil_resource_icon_seg[itr] = LBX_Reload_Next(backgrnd_lbx_file, (40 + itr), GFX_Swap_Seg);
    }

    /*
        94  CITYICON    grey lil bread
    */
    city_grey_lil_bread_icon_seg = LBX_Reload_Next(backgrnd_lbx_file, 94, GFX_Swap_Seg);

    /*
        88  CITYICO3    10 food
        89  CITYICO3    10 prod
        90  CITYICO3    10 gold
        91  CITYICO3    10 power
        92  CITYICO3    10 research
        93  CITYICO3    10 black gold
    */
    for(itr = 0; itr < 6; itr++)
    {
        city_big_resource_icon_seg[itr] = LBX_Reload_Next(backgrnd_lbx_file, (88 + itr), GFX_Swap_Seg);
    }

    /*
        95  CITYICO3    grey big bread
    */
    city_grey_big_bread_icon_seg = LBX_Reload_Next(backgrnd_lbx_file, 95, GFX_Swap_Seg);



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GFX_Swap_AppendUView()\n", __FILE__, __LINE__);
#endif
}

// WZD o52p28
// drake178: GFX_Swap_AppendItems()
// MoO2
// 1oom: 
/*
    Loads
        ITEMS.LBX
        ITEMISC.LBX
        not
            ITEMDATA.LBX
            ITEMPOW.LBX

Usage:
    Item Screen / Item View
    Unit View / Unit Statistics Window

*/
void Reload_Item_Pictures(void)
{
    int16_t itr;  // _SI_

    // load all of ITEMS.LBX
    for(itr = 0; itr < 116; itr++)
    {
        item_icons_seg[itr] = LBX_Reload_Next(items_lbx_file, itr, GFX_Swap_Seg);
    }

    /*
        ITEMISC.LBX, {0, ..., 36}
        FLICs: ITEMPOWR, ITEMSLOT, ITEMVIEW, ITEMTILE, 
    */

    // ITEMISC.LBX, 000  ITEMPOWR  item pro magic
    // ITEMISC.LBX, 001  ITEMPOWR  item regenerate
    // ITEMISC.LBX, 002  ITEMPOWR  item haste
    // ITEMISC.LBX, 003  ITEMPOWR  item true sight
    // ITEMISC.LBX, 004  ITEMPOWR  item path finding
    // ITEMISC.LBX, 005  ITEMPOWR  item water walking
    // ITEMISC.LBX, 006  ITEMPOWR  item pro evil
    // ITEMISC.LBX, 007  ITEMPOWR  item lion heart
    // ITEMISC.LBX, 008  ITEMPOWR  item invis
    // ITEMISC.LBX, 009  ITEMPOWR  item astral proj
    // ITEMISC.LBX, 010  ITEMPOWR  item endurance
    // ITEMISC.LBX, 011  ITEMPOWR  item rightousness
    // ITEMISC.LBX, 012  ITEMPOWR  item invulnerabil
    // ITEMISC.LBX, 013  ITEMPOWR  item resist elem
    // ITEMISC.LBX, 014  ITEMPOWR  item pro elements
    // ITEMISC.LBX, 015  ITEMPOWR  item cloak of fear
    // ITEMISC.LBX, 016  ITEMPOWR  item flight
    // ITEMISC.LBX, 017  ITEMPOWR  item resist magic
    // ITEMISC.LBX, 018  ITEMPOWR  item merging
    for(itr = 0; itr < 19; itr++)
    {
        item_power_icons_seg[itr] = LBX_Reload_Next(itemisc_lbx_file, itr, GFX_Swap_Seg);
    }

    // ITEMISC.LBX, 033  ITEMPOWR  item dispel evil
    // ITEMISC.LBX, 034  ITEMPOWR  item giant str
    // ITEMISC.LBX, 035  ITEMPOWR  item guardian wind
    item_power_icons_seg[19] = LBX_Reload_Next(itemisc_lbx_file, 33, GFX_Swap_Seg);
    item_power_icons_seg[20] = LBX_Reload_Next(itemisc_lbx_file, 34, GFX_Swap_Seg);
    item_power_icons_seg[21] = LBX_Reload_Next(itemisc_lbx_file, 35, GFX_Swap_Seg);


    // ITEMISC.LBX, 019  ITEMSLOT  sword
    // ITEMISC.LBX, 020  ITEMSLOT  bow
    // ITEMISC.LBX, 021  ITEMSLOT  weapon/staff
    // ITEMISC.LBX, 022  ITEMSLOT  wand
    // ITEMISC.LBX, 023  ITEMSLOT  misc
    // ITEMISC.LBX, 024  ITEMSLOT  armor
    item_slot_icons_seg[0] = LBX_Reload_Next(itemisc_lbx_file, 19, GFX_Swap_Seg);
    item_slot_icons_seg[1] = LBX_Reload_Next(itemisc_lbx_file, 20, GFX_Swap_Seg);
    item_slot_icons_seg[2] = LBX_Reload_Next(itemisc_lbx_file, 21, GFX_Swap_Seg);
    item_slot_icons_seg[3] = LBX_Reload_Next(itemisc_lbx_file, 22, GFX_Swap_Seg);
    item_slot_icons_seg[4] = LBX_Reload_Next(itemisc_lbx_file, 24, GFX_Swap_Seg);
    item_slot_icons_seg[5] = LBX_Reload_Next(itemisc_lbx_file, 23, GFX_Swap_Seg);


    // ITEMISC.LBX, 025  ITEMVIEW  item view backgrnd
    // ITEMISC.LBX, 026  ITEMVIEW  item view bullet
    item_view_background_seg = LBX_Reload_Next(itemisc_lbx_file, 25, GFX_Swap_Seg);
    item_view_bullet_seg = LBX_Reload_Next(itemisc_lbx_file, 26, GFX_Swap_Seg);


    // ITEMISC.LBX, 027  ITEMTILE  sword (unit view)
    // ITEMISC.LBX, 028  ITEMTILE  bow   (unit view)
    // ITEMISC.LBX, 029  ITEMTILE  weapon/staff (uv)
    // ITEMISC.LBX, 030  ITEMTILE  wand  (unit view)
    // ITEMISC.LBX, 031  ITEMTILE  misc  (unit view)
    // ITEMISC.LBX, 032  ITEMTILE  armor (unit view)
    itemtile_icons_seg[0] = LBX_Reload_Next(itemisc_lbx_file, 27, GFX_Swap_Seg);
    itemtile_icons_seg[1] = LBX_Reload_Next(itemisc_lbx_file, 28, GFX_Swap_Seg);
    itemtile_icons_seg[2] = LBX_Reload_Next(itemisc_lbx_file, 29, GFX_Swap_Seg);
    itemtile_icons_seg[3] = LBX_Reload_Next(itemisc_lbx_file, 30, GFX_Swap_Seg);
    itemtile_icons_seg[4] = LBX_Reload_Next(itemisc_lbx_file, 32, GFX_Swap_Seg);
    itemtile_icons_seg[5] = LBX_Reload_Next(itemisc_lbx_file, 31, GFX_Swap_Seg);

}

// WZD o52p29
void Spellbook_Load_Small_Pictures__WIP(void)
{
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Spellbook_Load_Small_Pictures__WIP()\n", __FILE__, __LINE__);
#endif

    // SPECIAL2.LBX, 034  SMLBOOK
    // SPECIAL2.LBX, 035  BCORNERS
    // SPECIAL2.LBX, 036  BCORNERS

    _spellbook_small_seg              = LBX_Reload_Next(special2_lbx_file, 34, GFX_Swap_Seg);
    _spellbook_small_left_corner_seg  = LBX_Reload_Next(special2_lbx_file, 35, GFX_Swap_Seg);
    _spellbook_small_right_corner_seg = LBX_Reload_Next(special2_lbx_file, 36, GFX_Swap_Seg);

    // SPECIAL2.LBX, 037  BOOKSYMB
    // SPECIAL2.LBX, 038  BOOKSYMB
    // SPECIAL2.LBX, 039  BOOKSYMB
    // SPECIAL2.LBX, 040  BOOKSYMB
    // SPECIAL2.LBX, 041  BOOKSYMB
    // SPECIAL2.LBX, 042  BOOKSYMB

    for(itr = 0; itr <= 5; itr++)
    {
        _spellbook_small_symbols[itr] = LBX_Reload_Next(special2_lbx_file, (37 + itr), GFX_Swap_Seg);
    }

    // SPECIAL2.LBX, 043  SMALTEXT

    _spellbook_small_text = LBX_Reload_Next(special2_lbx_file, 43, GFX_Swap_Seg);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Spellbook_Load_Small_Pictures__WIP()\n", __FILE__, __LINE__);
#endif

}


// WZD o52p30
void GFX_Swap_Cities(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GFX_Swap_Cities()\n", __FILE__, __LINE__);
#endif

    GFX_Swap_Reset();

    Load_Unit_StatFigs();  // LBX_Reload_Next(); UNITS_.LBX, _unit_type_table, GFX_Swap_Seg

    Reload_Item_Pictures();

    GFX_Swap_AppndCtScap__WIP();

    City_Screen_Load_Pictures();

    Spellbook_Load_Small_Pictures__WIP();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GFX_Swap_Cities()\n", __FILE__, __LINE__);
#endif
}

// WZD o52p31
void GFX_Swap_Overland(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GFX_Swap_Cities()\n", __FILE__, __LINE__);
#endif

    GFX_Swap_Reset();

    GFX_Swap_AppendUView();

    Reload_Item_Pictures();

    GFX_Swap_AppndCtScap__WIP();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GFX_Swap_Cities()\n", __FILE__, __LINE__);
#endif

}

// WZD o52p32
/*
Load graphics for 'Unit List Window'
    ...resets GFX_Swap_Seg

XREF:
    j_U_GFX_Swap_Units()
        Unit_List_Window()
*/
void Cache_Graphics_Unit_List_Window(void)
{
    GFX_Swap_Reset();
    Load_Unit_StatFigs();
    GFX_Swap_AppendUView();
}

// WZD o52p33

// WZD o52p34
// GFX_Swap_Combat()
