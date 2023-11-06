
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



#include "MoX.H"

#include "MainMenu.H"
#include "MainScr.H"
#include "MainScr_Maps.H"  // terrain_lbx_001, terrain_lbx_002



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

// dseg:29DE terrstat_lbx_file db 'TERRSTAT',0

// WZD dseg:29E7
char mapback_lbx_file[] = "MAPBACK";

// WZD dseg:29EF                            Menu_Screen_Load_Pictures()

// WZD dseg:29EF main_lbx_file db 'MAIN',0
// WZD dseg:29F4 backgrnd_lbx_file db 'BACKGRND',0
// WZD dseg:29FD cnst_SOUNDFX_File db 'SOUNDFX',0
// WZD dseg:2A05 spelldat_lbx_file db 'SPELLDAT.LBX',0
// WZD dseg:2A12 cnst_BUILDDAT_File db 'BUILDDAT.LBX',0
// WZD dseg:2A1F cnst_Units1_File db 'UNITS1',0
// WZD dseg:2A26 cnst_Units2_File db 'UNITS2',0
// WZD dseg:2A2D cnst_CMBMAGIC_File db 'CMBMAGIC',0
// WZD dseg:2A36 cnst_CMBTCITY_File db 'CMBTCITY',0
// WZD dseg:2A3F cnst_CHRIVER_File db 'CHRIVER',0
// WZD dseg:2A47 cnst_CITYWALL_File db 'CITYWALL',0
// WZD dseg:2A50 cnst_CITYSCAP_File db 'CITYSCAP',0
// WZD dseg:2A59 cnst_UNITVIEW_File db 'UNITVIEW',0
// WZD dseg:2A62 cnst_SPECIAL1_File db 'SPECIAL',0
// WZD dseg:2A6A cnst_SPECIAL2_File db 'SPECIAL2',0
// WZD dseg:2A73 cnst_ITEMS_File db 'ITEMS',0
// WZD dseg:2A79 cnst_ITEMISC_File db 'ITEMISC',0

// WZD dseg:2A81
char special2_lbx_file[] = "special2";

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
// Load_SPELLDAT();  // ; loads all records from SPELLDAT.LBX, overwriting the pointer to a previous allocation (8k wasted)

    Load_BUILDDAT();  // loads all records from BUILDDAT.LBX

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
    WIZARDS.EXE  ovr052
*/

// WZD o52p20
void GFX_Swap_Reset(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GFX_Swap_Reset()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Check_Allocation(GFX_Swap_Seg): %d\n", __FILE__, __LINE__, Check_Allocation(GFX_Swap_Seg));
    dbg_prn("DEBUG: [%s, %d]: Get_Free_Blocks(SAMB_head): %d\n", __FILE__, __LINE__, Get_Free_Blocks(GFX_Swap_Seg));
    dbg_prn("DEBUG: [%s, %d]: SA_GET_SIZE(GFX_Swap_Seg): %d\n", __FILE__, __LINE__, SA_GET_SIZE(GFX_Swap_Seg));
    dbg_prn("DEBUG: [%s, %d]: SA_GET_USED(GFX_Swap_Seg): %d\n", __FILE__, __LINE__, SA_GET_USED(GFX_Swap_Seg));
#endif

    Allocate_First_Block(GFX_Swap_Seg, 1);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Check_Allocation(GFX_Swap_Seg): %d\n", __FILE__, __LINE__, Check_Allocation(GFX_Swap_Seg));
    dbg_prn("DEBUG: [%s, %d]: Get_Free_Blocks(SAMB_head): %d\n", __FILE__, __LINE__, Get_Free_Blocks(GFX_Swap_Seg));
    dbg_prn("DEBUG: [%s, %d]: SA_GET_SIZE(GFX_Swap_Seg): %d\n", __FILE__, __LINE__, SA_GET_SIZE(GFX_Swap_Seg));
    dbg_prn("DEBUG: [%s, %d]: SA_GET_USED(GFX_Swap_Seg): %d\n", __FILE__, __LINE__, SA_GET_USED(GFX_Swap_Seg));
#endif

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

// WZD o52p27
void GFX_Swap_AppendUView(void)
{
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GFX_Swap_AppendUView()\n", __FILE__, __LINE__);
#endif

    // load [111of111] from SPECIAL.LBX
    for(itr = 0; itr < 111; itr++)
    {
        // IMG_USW_Abilities.Teleporting@
        // IMG_USW_Abilities[itr] = LBX_Reload_Next("SPECIAL", itr, GFX_Swap_Seg);
        special_seg[itr] = LBX_Reload_Next("SPECIAL", itr, GFX_Swap_Seg);
    }

    // load [(0to33)of44] from SPECIAL2.LBX
    for(itr = 0; itr < 34; itr++)
    {
        // IMG_USW_Abilities.Confusion@
        // IMG_USW_Abilities[itr] = LBX_Reload_Next("SPECIAL2", itr, GFX_Swap_Seg);
        special2_seg[itr] = LBX_Reload_Next("SPECIAL2", itr, GFX_Swap_Seg);
    }



    // load {0,1,2,3,4} from UNITVIEW.LBX

    // UNITBACK small unit backgrn
    unitview_small_background_seg = LBX_Reload_Next("UNITVIEW", 0, GFX_Swap_Seg);
    // FULLUNIT large unit backgrn
    unitview_large_background_seg = LBX_Reload_Next("UNITVIEW", 1, GFX_Swap_Seg);
    // BLDBUTBK button background
    unitview_button_background_seg = LBX_Reload_Next("UNITVIEW", 2, GFX_Swap_Seg);
    // UNITBUTT up arrow
    unitview_up_arrow_seg = LBX_Reload_Next("UNITVIEW", 3, GFX_Swap_Seg);
    // UNITBUTT down arrow
    unitview_down_arrow_seg = LBX_Reload_Next("UNITVIEW", 4, GFX_Swap_Seg);


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
        // IMG_USW_AbBorders@
        IMG_USW_AbBorders[itr] = LBX_Reload_Next("UNITVIEW", (5 + itr), GFX_Swap_Seg);
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
        // IMG_USW_Stat_Icons.Melee@
        IMG_USW_Stat_Icons[itr] = LBX_Reload_Next("UNITVIEW", (13 + itr), GFX_Swap_Seg);
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
        // IMG_USW_Stats_Gold.Melee@
        IMG_USW_Stats_Gold[itr] = LBX_Reload_Next("UNITVIEW", (35 + itr), GFX_Swap_Seg);
    }


    IMG_OVL_UnitList_BG = LBX_Reload_Next("UNITVIEW", 28, GFX_Swap_Seg);
    IMG_OVL_UnitListBtm = LBX_Reload_Next("UNITVIEW", 29, GFX_Swap_Seg);
    IMG_OVL_BuildBtn_BG = LBX_Reload_Next("UNITVIEW", 30, GFX_Swap_Seg);


    red_button_seg = LBX_Reload_Next("BACKGRND", 24, GFX_Swap_Seg);
    IMG_CTY_Neg_1_Gold = LBX_Reload_Next("BACKGRND", 73, GFX_Swap_Seg);
    IMG_USW_1_Gold = LBX_Reload_Next("BACKGRND", 42, GFX_Swap_Seg);


    IMG_CTY_LeftBldTab = LBX_Reload_Next("UNITVIEW", 31, GFX_Swap_Seg);
    IMG_CTY_RightBldTab = LBX_Reload_Next("UNITVIEW", 32, GFX_Swap_Seg);
    IMG_USW_Portrt_Brdr = LBX_Reload_Next("UNITVIEW", 33, GFX_Swap_Seg);
    IMG_USW_WaterBase = LBX_Reload_Next("UNITVIEW", 34, GFX_Swap_Seg);

    // ¿ BUILDS4    untit base ?
    IMG_USW_GrassBase = LBX_Reload_Next("CITYSCAP", 103, GFX_Swap_Seg);



    /*
        40  CITYICON    food icon
        41  CITYICON    production icon
        42  CITYICON    gold icon
        43  CITYICON    power icon
        44  CITYICON    research icon
    */
    for(itr = 0; itr < 5; itr++)
    {
        // IMG_CTY_1_Food@
        IMG_CTY_1_Food[itr] = LBX_Reload_Next("BACKGRND", (40 + itr), GFX_Swap_Seg);
    }

    /*
        94  CITYICON    grey lil bread
    */
    IMG_CTY_Neg_1_Food = LBX_Reload_Next("BACKGRND", 94, GFX_Swap_Seg);

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
        // IMG_CTY_10_Food@
        IMG_CTY_10_Food[itr] = LBX_Reload_Next("BACKGRND", (88 + itr), GFX_Swap_Seg);
    }

    /*
        95  CITYICO3    grey big bread
    */
    IMG_CTY_Neg_10_Food = LBX_Reload_Next("BACKGRND", 95, GFX_Swap_Seg);



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GFX_Swap_AppendUView()\n", __FILE__, __LINE__);
#endif
}

// WZD o52p28
void GFX_Swap_AppendItems(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GFX_Swap_AppendItems()\n", __FILE__, __LINE__);
#endif





#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GFX_Swap_AppendItems()\n", __FILE__, __LINE__);
#endif
}

// WZD o52p29
void Spellbook_Load_Small_Pictures(void)
{
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Spellbook_Load_Small_Pictures()\n", __FILE__, __LINE__);
#endif

    _spellbook_small_seg = LBX_Reload_Next(special2_lbx_file, 34, GFX_Swap_Seg);  // "SMLBOOK", ""
    _spellbook_small_left_corner_seg = LBX_Reload_Next(special2_lbx_file, 35, GFX_Swap_Seg);  // "BCORNERS", ""
    _spellbook_small_right_corner_seg = LBX_Reload_Next(special2_lbx_file, 36, GFX_Swap_Seg);  // "BCORNERS", ""

    for(itr = 0; itr < 5; itr++)
    {
        _spellbook_small_symbols[itr] = LBX_Reload_Next(special2_lbx_file, (37 + itr), GFX_Swap_Seg);  // "BOOKSYMB", ""
    }

    _spellbook_small_text = LBX_Reload_Next(special2_lbx_file, 43, GFX_Swap_Seg);  // "SMALTEXT", ""

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Spellbook_Load_Small_Pictures()\n", __FILE__, __LINE__);
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
    // GFX_SwapAppendItems();
    // GFX_Swap_AppndCtScap();
    // City_Screen_Load_Pictures();
    // Spellbook_Load_Small_Pictures();

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

    // ; resets the suballocations in GFX_Swap_Seg,
    // ; effectively removing all of the data references
    GFX_Swap_Reset();

    // ; appends unit view and list graphics to GFX_Swap_Seg
    // ; these are all in reserved EMM handles, so only
    // ; headers will be created in the allocation
    GFX_Swap_AppendUView();

    // ; appends item and item power graphics to GFX_Swap_Seg
    // ; these are all in reserved EMM handles, so only
    // ; headers will be created in the allocation
    // TODO  GFX_Swap_AppendItems();

    // ; appends city scape graphics to GFX_Swap_Seg
    // ; these are all in reserved EMM handles, so only
    // ; headers will be created in the allocation
    // TODO  GFX_Swap_AppndCtScap();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GFX_Swap_Cities()\n", __FILE__, __LINE__);
#endif

}

// WZD o52p32
// WZD o52p33

// WZD o52p34
// GFX_Swap_Combat()
