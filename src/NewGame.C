/*

    MAGIC.EXE
        ovr050
        ovr051
        ovr056
*/

#include "NEWGAME.H"

#include "MOX/Fonts.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_SET.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/MOM_DATA.H"  /* _difficulty, _magic, _landsize, _num_players */
#include "MOX/Timer.H"

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), frite(), fseek(); */



/*

    MoO2
        Module: NEWGAME
            Newgame_Screen_
                Address: 01:000CD435
            Reload_Newgame_Screen_
                Address: 01:000CCA1C
            Draw_Newgame_Screen_
                Address: 01:000CCD3C
            Draw_Pictures_
                Address: 01:000CCC3D
            Add_Fields_
                Address: 01:000CCE2E
            Update_Fields_
                Address: 01:000CCE24
            data (0 bytes) _fields
                Address: 02:0019933C
            data (0 bytes) _background_seg
                Address: 02:00199354

        Module: MAPGEN
    
Reload_Newgame_Screen_
is where the loading happens
i.e., (Re-)Load

*/
/*

terrain_stats_table
770 records
6 bytes
half movement cost per movement type
0 unknwon
1 walking
2 Forster
3 Mountaineer
4 Swimming
5 Sailing
But, also, why 770? world_map terrain_type offset is 762 ?

*/

/*
Load_TERRSTAT()
terrain_stats_table = SA_MK_FP0( LBXR_LoadSingle("TERRSTAT", 0, 0, 770, 6) )

    XREF NEWG_SetMoveMaps+61  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+94  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+C4  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+F7  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+12A les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+15D les     si, [terrain_stats_table]
*/

/*
Load_SPELLDAT()
spell_data_table    = SA_MK_FP0( LBXR_LoadSingle("SPELLDAT", 0, 0, 215, 36) )

*/



struct S_HERO_TEMPLATE
{
    /* 00 */  int16_t pick_count;
    /* 02 */  int16_t pick_type;  // {warrior, mage, any/both/either} ; enum e_RANDOM_PICK_TYPES
    /* 04 */  int32_t abilities;
    /* 08 */  int16_t casting_skill;
    /* 0A */  int16_t spell_1;  // ; enum Spells
    /* 0C */  int16_t spell_2;  // ; enum Spells
    /* 0E */  int16_t spell_3;  // ; enum Spells
    /* 10 */  int16_t spell_4;  // ; enum Spells
    /* 12 */
};
enum e_RANDOM_PICK_TYPES
{
    Warrior_Picks  = 0,
    mage_picks     = 1,
    Any_Picks      = 2
};
struct S_HERO_TEMPLATE _hero_premade_table[NUM_HERO_TYPES] = {
    /* picks        special                  casting  spells     */
    /* count, type, abilities,               skill    1,  2,  3,  4 */
    /*  */  {      0,   0, HSA_CONSTITUTION,              0,  0,  0,  0,  0 },
    /*  */  {      0,   0, HSA_MIGHT,                     0,  0,  0, 0, 0 },
    /*  */  {      0,   1, HSA_SAGE,                      2,  42, 48, 0, 0 },
    /*  */  {      0,   0, HSA_NOBLE,                     0,  0, 0, 0, 0 },
    /*  */  {      0,   0, 0,                             1,  spl_Resist_Elements, 0, 0, 0 },
    /*  */  {      0,   0, (HSA_LEADERSHIP | HSA_FEMALE), 1, 46, 53, 0, 0 },
    /*  */  {      1,   0, 0,                             0, 0, 0, 0, 0 },
    /*  */  {      1,   1, 0x40000000, 2, 125, 0, 0, 0 },
    /*  */  {      1,   0, 0x40000040, 0, 0, 0, 0, 0 },
    /*  */  {      0,   0, 0xD0000000, 0, 0, 0, 0, 0 },
    /*  */  {      0,   0, 0, 2, 13, 22, 5, 0 },
    /*  */  {      1,   0, 0x4000000, 0, 0, 0, 0, 0 },
    /*  */  {      1,   2, 0, 1, 86, 88, 94, 0 },
    /*  */  {      1,   1, 0x40000, 3, 83, 96, 90, 109 },
    /*  */  {      1,   0, 0x40, 0, 0, 0, 0, 0 },
    /*  */  {      1,   1, 0, 2, 47, 44, 0, 0 },
    /*  */  {      0,   0, 0x8000, 1, 2, 7, 0, 0 },
    /*  */  {      2,   0, 0x8000, 0, 0, 0, 0, 0 },
    /*  */  {      2,   1, 0x50000000, 3, 169, 170, 174, 172 },
    /*  */  {      2,   2, 0, 1, 0, 0, 0, 0 },
    /*  */  {      2,   0, 0x40, 0, 0, 0, 0, 0 },
    /*  */  {      1,   0, 0x11, 0, 0, 0, 0, 0 },
    /*  */  {      1,   0, 0x50008040, 0, 0, 0, 0, 0 },
    /*  */  {      1,   1, 0, 5, 91, 101, 104, 0 },
    /*  */  {      5,   2, 0, 1, 0, 0, 0, 0 },
    /*  */  {      2,   1, 0x40000000, 3, 50, 53, 67, 0 },
    /* Deathstryke Swordsman */  {      1,   0, 0x9209, 0, 0, 0, 0, 0 },
    /* Elana Priestess */        {      0,   0, 0x72040000, 4, 134, 125, 140, 150 },
    /* Roland Paladin */         {      1,   0, 0x1010008, 0, 0, 0, 0, 0 },
    /* Mortu Black Night */      {      1,   0, 0x9048, 0, 0, 0, 0, 0 },
    /* Alorra Elven Archer */    {      3,   2, 0x40000040, 1, 41, 56, 0, 0 },
    /* Sir Harold Knight */      {      1,   0, 0x20001012, 0, 0, 0, 0, 0 },
    /* Ravashack Necromancer */  {      2,   1, 0x40000, 4, 162, 165, 195, 182 },
    /* Warrax Chaos Warrior */   {      3,   2, 0x41000, 3, 0, 0, 0, 0 },
    /* Torin The Chosen */       {      2,   2, 0x1011002, 5, 128, 125, 126, 141 }
};

// dseg:35D0 00 00 00 00 00 10 00 00 00 00 00 00 00 00 00 00+_hero_premade_table s_HERO_TEMPLATE <0, 0, 1000h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 80 00 00 00 00 00 00 00 00+                                        ; DATA XREF: NEWG_CreateHeroes+66r ...
// dseg:35D0 00 00 00 00 00 00 01 00 00 00 20 00 02 00 2A 00+s_HERO_TEMPLATE <0, 0, 8000h, 0, 0, 0, 0, 0>
// dseg:35D0 30 00 00 00 00 00 00 00 00 00 00 00 00 20 00 00+s_HERO_TEMPLATE <0, 1, 200000h, 2, 42, 48, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+s_HERO_TEMPLATE <0, 0, 20000000h, 0, 0, 0, 0, 0>
// dseg:35D0 01 00 02 00 00 00 00 00 00 00 00 00 00 00 01 00+s_HERO_TEMPLATE <0, 0, 0, 1, 2, 0, 0, 0>
// dseg:35D0 00 40 01 00 2E 00 35 00 00 00 00 00 01 00 00 00+s_HERO_TEMPLATE <0, 0, 40000001h, 1, 46, 53, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00+s_HERO_TEMPLATE <1, 0, 0, 0, 0, 0, 0, 0>
// dseg:35D0 01 00 00 00 00 40 02 00 7D 00 00 00 00 00 00 00+s_HERO_TEMPLATE <1, 1, 40000000h, 2, 125, 0, 0, 0>
// dseg:35D0 01 00 00 00 40 00 00 40 00 00 00 00 00 00 00 00+s_HERO_TEMPLATE <1, 0, 40000040h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 D0 00 00 00 00 00 00+s_HERO_TEMPLATE <0, 0, 0D0000000h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 02 00 0D 00+s_HERO_TEMPLATE <0, 0, 0, 2, 13, 22, 5, 0>
// dseg:35D0 16 00 05 00 00 00 01 00 00 00 00 00 00 04 00 00+s_HERO_TEMPLATE <1, 0, 4000000h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 01 00 02 00 00 00 00 00+s_HERO_TEMPLATE <1, 2, 0, 1, 86, 88, 94, 0>
// dseg:35D0 01 00 56 00 58 00 5E 00 00 00 01 00 01 00 00 00+s_HERO_TEMPLATE <1, 1, 40000h, 3, 83, 96, 90, 109>
// dseg:35D0 04 00 03 00 53 00 60 00 5A 00 6D 00 01 00 00 00+s_HERO_TEMPLATE <1, 0, 40h, 0, 0, 0, 0, 0>
// dseg:35D0 40 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00+s_HERO_TEMPLATE <1, 1, 0, 2, 47, 44, 0, 0>
// dseg:35D0 01 00 00 00 00 00 02 00 2F 00 2C 00 00 00 00 00+s_HERO_TEMPLATE <0, 0, 8000h, 1, 2, 7, 0, 0>
// dseg:35D0 00 00 00 00 00 80 00 00 01 00 02 00 07 00 00 00+s_HERO_TEMPLATE <2, 0, 8000h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 02 00 00 00 00 80 00 00 00 00 00 00 00 00+s_HERO_TEMPLATE <2, 1, 50000000h, 3, 169, 170, 174, 172>
// dseg:35D0 00 00 00 00 02 00 01 00 00 00 00 50 03 00 A9 00+s_HERO_TEMPLATE <2, 2, 0, 1, 0, 0, 0, 0>
// dseg:35D0 AA 00 AE 00 AC 00 02 00 02 00 00 00 00 00 01 00+s_HERO_TEMPLATE <2, 0, 40h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 02 00 00 00 40 00 00 00+s_HERO_TEMPLATE <1, 0, 11h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 01 00 00 00 11 00+s_HERO_TEMPLATE <1, 0, 50008040h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00+s_HERO_TEMPLATE <1, 1, 0, 5, 91, 101, 104, 0>
// dseg:35D0 40 80 00 50 00 00 00 00 00 00 00 00 00 00 01 00+s_HERO_TEMPLATE <5, 2, 0, 1, 0, 0, 0, 0>
// dseg:35D0 01 00 00 00 00 00 05 00 5B 00 65 00 68 00 00 00+s_HERO_TEMPLATE <2, 1, 40000000h, 3, 50, 53, 67, 0>
// dseg:35D0 05 00 02 00 00 00 00 00 01 00 00 00 00 00 00 00+s_HERO_TEMPLATE <1, 0, 9209h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 02 00 01 00 00 00 00 40 03 00 32 00 35 00+s_HERO_TEMPLATE <0, 0, 72040000h, 4, 134, 125, 140, 150>
// dseg:35D0 43 00 00 00 01 00 00 00 09 92 00 00 00 00 00 00+s_HERO_TEMPLATE <1, 0, 1010008h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 04 72 04 00+s_HERO_TEMPLATE <1, 0, 9048h, 0, 0, 0, 0, 0>
// dseg:35D0 86 00 7D 00 8C 00 96 00 01 00 00 00 08 00 01 01+s_HERO_TEMPLATE <3, 2, 40000040h, 1, 41, 56, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 01 00 00 00 48 90+s_HERO_TEMPLATE <1, 0, 20001012h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 03 00 02 00+s_HERO_TEMPLATE <2, 1, 40000h, 4, 162, 165, 195, 182>
// dseg:35D0 40 00 00 40 01 00 29 00 38 00 00 00 00 00 01 00+s_HERO_TEMPLATE <3, 2, 41000h, 3, 0, 0, 0, 0>
// dseg:35D0 00 00 12 10 00 20 00 00 00 00 00 00 00 00 00 00+s_HERO_TEMPLATE <2, 2, 1011002h, 5, 128, 125, 126, 141>


// MGC dseg:52C0
int16_t _quit_button = 0;

// MGC dseg:5CB0
SAMB_ptr newgame_ok_button_seg = 0;

// MGC dseg:64AE
int16_t screen_changed = 0;

// MGC dseg:8E8E
SAMB_ptr _quit_active_seg = 0;

// MGC dseg:8E92
int16_t _ok_button = 0;




// MGC dseg:52B2                                                 ? BEGIN: Main Menu Screen ?
// MGC dseg:52B2
// MGC dseg:52B2                                                 ; char *GUI_String_2
// MGC dseg:52B2 00 00                                           GUI_String_2 dw 0                       ; DATA XREF: start+C7o ...
// MGC dseg:52B2                                                                                         ; _bdata_start
// MGC dseg:52B2                                                                                         ; 100 LBX_NearAlloc_Next bytes
// MGC dseg:52B4                                                 ; char *GUI_String_1
// MGC dseg:52B4 00 00                                           GUI_String_1 dw 0                       ; DATA XREF: Hall_Of_Fame_Screen+8Dw ...
// MGC dseg:52B4                                                                                         ; 100 LBX_NearAlloc_First bytes
// MGC dseg:52B6 00 00                                           _load_button dw 0                       ; DATA XREF: Main_Menu_Screen+19Fw ...
// MGC dseg:52B8 00 00                                           _background_seg dw 0                    ; DATA XREF: Load_Screen+1Bw ...
SAMB_ptr newgame_background_seg = 0;
// MGC dseg:52BA 00 00                                           menu_quit_button dw 0                   ; DATA XREF: Load_MGC_Resources+76w ...
// MGC dseg:52BA                                                                                         ; single-loaded 2-frame image
// MGC dseg:52BC 00 00                                           _halloffame_button dw 0                 ; DATA XREF: Main_Menu_Screen+29Ew ...
// MGC dseg:52BE 00 00                                           menu_hof_button dw 0                    ; DATA XREF: Load_MGC_Resources+40w ...
// MGC dseg:52BE                                                                                         ; single-loaded 2-frame image
// MGC dseg:52C0 00 00                                           _quit_button dw 0                       ; DATA XREF: Main_Menu_Screen+2DFw ...
// MGC dseg:52C2 00 00                                           menu_load_button dw 0                   ; DATA XREF: Load_MGC_Resources+52w ...

// MGC dseg:52C4 00 00                                           current_menu_screen dw 0                ; DATA XREF: Main_Menu_Screen+32w ...
int16_t auto_input_field_idx = 0;

// MGC dseg:52C4                                                                                         ; 0 - continue, 1 -load, 2 - new, 3 - quit, 4 - HoF
// MGC dseg:52C4                                                                                         ; holds the mouse focus control instead during the
// MGC dseg:52C4                                                                                         ;  redraw functions of other screens
// MGC dseg:52C6 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+wizard_portrait_segs dw 14 dup(    0)   ; DATA XREF: Load_MGC_Resources+A9w ...
// MGC dseg:52C6 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 single-loaded images
// MGC dseg:52E2 00 00                                           IMG_NewG_BtnBorder@ dw 0                ; used by the map customizer and profile reloader
// MGC dseg:52E4 00 00                                           load_flag dw 0                          ; DATA XREF: Main_Menu_Screen+38w ...
// MGC dseg:52E4                                                                                         ; OXRB:Menu,MenuDraw.:.static?module-scoped?private?
// MGC dseg:52E4                                                                                         ; Da-Funk? "OXRB"? "only x-ref'd by"? if so, nowadays, use "OON XREF"
// MGC dseg:52E6 00 00                                           NEWG_HaveLastProfile dw 0               ; used by the profile reloader
// MGC dseg:52E8 00 00                                           mainmenu_top dw 0                       ; DATA XREF: Load_MGC_Resources+1Cw ...
// MGC dseg:52E8                                                                                         ; single-loaded 20 frame animation
// MGC dseg:52E8                                                                                         ; includes the background for the top of the screen too
// MGC dseg:52EA 00 00                                           _newgame_button dw 0                    ; DATA XREF: Main_Menu_Screen+25Dw ...
// MGC dseg:52EC 00 00                                           menu_new_button dw 0                    ; DATA XREF: Load_MGC_Resources+64w ...
// MGC dseg:52EC                                                                                         ; single-loaded 2-frame image
// MGC dseg:52EE 00 00                                           _continue_button dw 0                   ; DATA XREF: Main_Menu_Screen+204w ...
// MGC dseg:52F0 00 00                                           menu_continue_button dw 0               ; DATA XREF: Load_MGC_Resources+2Ew ...
// MGC dseg:52F0                                                                                         ; single-loaded 2-frame image
// MGC dseg:52F2 00 00                                           cont_flag dw 0                          ; DATA XREF: Main_Menu_Screen+2Cw ...
// MGC dseg:52F2                                                                                         ; OXRB:Menu,MenuDraw.:.static?module-scoped?private?
// MGC dseg:52F4 00 00                                           mainmenu_bottom dw 0                    ; DATA XREF: Load_MGC_Resources+88w ...
// MGC dseg:52F4                                                                                         ; single-loaded image
// MGC dseg:52F6 00 00                                           mainmenu_bot dw 0                       ; used by the map customizer to indicate which screen
// MGC dseg:52F6                                                                                         ; is currently active (basic/patch/world/world2)
// MGC dseg:52F8 00 00                                           _load_inactive_seg dw 0                 ; DATA XREF: Load_Screen+60w ...
// MGC dseg:52FA 00 00                                           NEWG_LastRetorts@ dw 0                  ; used by the profile reloader
// MGC dseg:52FC 00 00                                           g_GUI_MainScreenJump dw 0               ; DATA XREF: Menu_Screen_Control+5Aw ...
// MGC dseg:52FE 00 00                                           save_game_count dw 0                    ; DATA XREF: Main_Menu_Screen+C6w ...
// MGC dseg:5300 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 save_game_slots dw 8 dup(     0)        ; DATA XREF: Main_Menu_Screen+137w ...
// MGC dseg:5300
// MGC dseg:5300                                                 ? END: Main Menu Screen ?
// MGC dseg:5300
// MGC dseg:5310
// MGC dseg:5310                                                 ? Begin: New Game Screen ?
// MGC dseg:5310
// MGC dseg:5310 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_WorldgenSet dw 40h dup(0)          ; extended worldgen settings are loaded here by the
// MGC dseg:5310 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; worldgen customizer
// MGC dseg:5390 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_LastProfile db 0B6h dup(0)         ; the last saved wizard profile is loaded here by
// MGC dseg:5390 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; the profile reloader
// MGC dseg:5446 00 00                                           IMG_NewG_ValueBox@ dw 0                 ; NEWGAME.LBX entry 51, used by the world generator
// MGC dseg:5446                                                                                         ; customizer to display the setting values
// MGC dseg:5446                                                                                         ; (number, On/Off, Low/Norm/High, or Rand)
// MGC dseg:5448 00 00                                           IMG_NewG_CheckMark@ dw 0
// MGC dseg:544A 00 00                                           IMG_NewG_ScrollUp@ dw 0
// MGC dseg:544C 00 00                                           IMG_NewG_ScrollDown@ dw 0
// MGC dseg:544E 00 00                                           NEWG_WGP_OK_Label dw 0
// MGC dseg:5450 00 00                                           NEWG_WGP_Reset_Lbl dw 0
// MGC dseg:5452 00 00                                           NEWG_WGP_ScrollUp dw 0
// MGC dseg:5454 00 00                                           NEWG_WGP_ScrollDn dw 0
// MGC dseg:5456 00 00                                           NEWG_FirstPatchSet dw 0
// MGC dseg:5458 00 00                                           NEWG_FirstWorldSet dw 0
// MGC dseg:545A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_WGP_SetCtrls dw 24h dup(0)
// MGC dseg:54A2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_WGPSliderStates dw 12h dup(0)
// MGC dseg:54C6 00 00                                           NEWG_ConnMap_Seg dw 0                   ; a 9600 byte buffer in GFX_Swap_Seg
// MGC dseg:54C8 00                                              NEWG_CustomWorld db 0
// MGC dseg:54C9 00                                              NEWG_CustomPlanes db 0
// MGC dseg:54CA 00                                              NEWG_SliderDragged db 0
// MGC dseg:54CB 00                                              align 2
// MGC dseg:54CC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+UU_Some_AI_Array db 54Eh dup(0)
// MGC dseg:5A1A 00 00                                           Wiz5_Spell_E0h@ dw 0                    ; DATA XREF: NEWG_FinalizeTables+95r ...
// MGC dseg:5A1A                                                                                         ; 80 + rnd(40) is stored here for each AI wizard (word
// MGC dseg:5A1A                                                                                         ; array pointer, human excluded) when setting initial
// MGC dseg:5A1A                                                                                         ; gold
// MGC dseg:5A1C 00 00                                           Wiz5_Spell_C8h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3F6w
// MGC dseg:5A1E 00 00                                           Wiz5_Spell_D4h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3FCw
// MGC dseg:5A20 00 00                                           Wiz5_Spell_A0h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3EAw
// MGC dseg:5A22 00 00                                           Wiz5_Spell_B4h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3F0w
// MGC dseg:5A24 00 00                                           Wiz5_Spell_50h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D2w
// MGC dseg:5A26 00 00                                           Wiz5_Spell_64h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D8w
// MGC dseg:5A28 00 00                                           Wiz5_Spell_28h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3C6w
// MGC dseg:5A2A 00 00                                           Wiz5_Spell_3Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3CCw
// MGC dseg:5A2C 00 00                                           Wiz5_Spell_18h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3C0w
// MGC dseg:5A2E 00 00                                           Wiz5_Spell_78h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3DEw
// MGC dseg:5A30 00 00                                           Wiz5_Spell_8Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3E4w
// MGC dseg:5A32 00 00                                           Wiz5_Spell_00h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3B4w
// MGC dseg:5A34 00 00                                           Wiz5_Spell_0Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3BAw
// MGC dseg:5A36 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+G_UU_Some_AI_Array_2 db 176h dup(0)
// MGC dseg:5BAC 00 00 00 00                                     terrain_stats_table dd 0                ; DATA XREF: Load_TERRSTAT+28w ...





// MGC dseg:2ABC                                                 BEGIN:  ovr050 - Initialized Data

// MGC dseg:2ABC
struct s_mouse_list mouse_list_newgame0[1] = {
    { crsr_None, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX }
};

// MGC dseg:2AC8 1F 1E 1D 1C 1B 1A 19 18                         COL_NEWG_NameEdit db 31, 30, 29, 28, 27, 26, 25, 24
// MGC dseg:2AC8                                                                                         ; DATA XREF: GAME_New_Screen_3+91o
// MGC dseg:2AD0 4D 65 72 6C 69 6E 00 00 00 00 05 00 00 00 05 00+TBL_Default_Wizards  = WIZARD_Preset ptr $
// MGC dseg:2AD0 00 00 00 00 07 00 52 61 76 65 6E 00 00 00 00 00+                                        ; DATA XREF: Draw_NewGame_Screen1+2B7t ...
// MGC dseg:2AD0 00 00 06 00 05 00 00 00 00 00 FF FF 53 68 61 72+db 4Dh, 65h, 72h, 6Ch, 69h, 6Eh, 4 dup(0); Name
// MGC dseg:2AD0 65 65 00 00 00 00 00 00 00 00 00 00 05 00 05 00+dw 5                                    ; Life
// MGC dseg:2AD0 0F 00 4C 6F 20 50 61 6E 00 00 00 00 00 00 05 00+dw 0                                    ; Sorcery
// MGC dseg:2AD0 00 00 00 00 05 00 08 00 4A 61 66 61 72 00 00 00+dw 5                                    ; Nature
// MGC dseg:2AD0 00 00 00 00 0A 00 00 00 00 00 00 00 00 00 4F 62+dw 0                                    ; Death
// MGC dseg:2AD0 65 72 69 63 00 00 00 00 00 00 00 00 05 00 00 00+dw 0                                    ; Chaos
// MGC dseg:2AD0 05 00 0B 00 52 6A 61 6B 00 00 00 00 00 00 00 00+dw _Sage_Master                         ; Retort
// MGC dseg:2AD0 00 00 00 00 09 00 00 00 05 00 53 73 73 27 72 61+db 52h, 61h, 76h, 65h, 6Eh, 5 dup(0)    ; Name
// MGC dseg:2AD0 00 00 00 00 04 00 00 00 00 00 00 00 04 00 09 00+dw 0                                    ; Life
// MGC dseg:2AD0 54 61 75 72 6F 6E 00 00 00 00 00 00 00 00 00 00+dw 6                                    ; Sorcery
// MGC dseg:2AD0 00 00 0A 00 02 00 46 72 65 79 61 00 00 00 00 00+dw 5                                    ; Nature
// MGC dseg:2AD0 00 00 00 00 0A 00 00 00 00 00 03 00 48 6F 72 75+dw 0                                    ; Death
// MGC dseg:2AD0 73 00 00 00 00 00 05 00 05 00 00 00 00 00 00 00+dw 0                                    ; Chaos
// MGC dseg:2AD0 0A 00 41 72 69 65 6C 00 00 00 00 00 0A 00 00 00+dw _No_Retort                           ; Retort
// MGC dseg:2AD0 00 00 00 00 00 00 10 00 54 6C 61 6C 6F 63 00 00+db 53h, 68h, 61h, 72h, 2 dup(65h), 4 dup(0); Name
// MGC dseg:2AD0 00 00 00 00 00 00 04 00 05 00 00 00 01 00 4B 61+dw 0                                    ; Life
// MGC dseg:2AD0 6C 69 00 00 00 00 00 00 00 00 05 00 00 00 05 00+dw 0                                    ; Sorcery
// MGC dseg:2AD0 00 00 11 00 43 75 73 74 6F 6D 00 00 00 00 00 00+dw 0                                    ; Nature
// MGC dseg:2AD0 00 00 00 00 00 00 00 00 FF FF                   dw 5                                    ; Death
// MGC dseg:2AD0                                                 dw 5                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Conjurer                            ; Retort
// MGC dseg:2AD0                                                 db 4Ch, 6Fh, 20h, 50h, 61h, 6Eh, 4 dup(0); Name
// MGC dseg:2AD0                                                 dw 0                                    ; Life
// MGC dseg:2AD0                                                 dw 5                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 0                                    ; Nature
// MGC dseg:2AD0                                                 dw 0                                    ; Death
// MGC dseg:2AD0                                                 dw 5                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Channeller                          ; Retort
// MGC dseg:2AD0                                                 db 4Ah, 61h, 66h, 61h, 72h, 5 dup(0)    ; Name
// MGC dseg:2AD0                                                 dw 0                                    ; Life
// MGC dseg:2AD0                                                 dw 0Ah                                  ; Sorcery
// MGC dseg:2AD0                                                 dw 0                                    ; Nature
// MGC dseg:2AD0                                                 dw 0                                    ; Death
// MGC dseg:2AD0                                                 dw 0                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Alchemy                             ; Retort
// MGC dseg:2AD0                                                 db 4Fh, 62h, 65h, 72h, 69h, 63h, 4 dup(0); Name
// MGC dseg:2AD0                                                 dw 0                                    ; Life
// MGC dseg:2AD0                                                 dw 0                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 5                                    ; Nature
// MGC dseg:2AD0                                                 dw 0                                    ; Death
// MGC dseg:2AD0                                                 dw 5                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Mana_Focusing                       ; Retort
// MGC dseg:2AD0                                                 db 52h, 6Ah, 61h, 6Bh, 6 dup(0)         ; Name
// MGC dseg:2AD0                                                 dw 0                                    ; Life
// MGC dseg:2AD0                                                 dw 0                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 0                                    ; Nature
// MGC dseg:2AD0                                                 dw 9                                    ; Death
// MGC dseg:2AD0                                                 dw 0                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Infernal_Power                      ; Retort
// MGC dseg:2AD0                                                 db 53h, 2 dup(73h), 27h, 72h, 61h, 4 dup(0); Name
// MGC dseg:2AD0                                                 dw 4                                    ; Life
// MGC dseg:2AD0                                                 dw 0                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 0                                    ; Nature
// MGC dseg:2AD0                                                 dw 0                                    ; Death
// MGC dseg:2AD0                                                 dw 4                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Myrran                              ; Retort
// MGC dseg:2AD0                                                 db 54h, 61h, 75h, 72h, 6Fh, 6Eh, 4 dup(0); Name
// MGC dseg:2AD0                                                 dw 0                                    ; Life
// MGC dseg:2AD0                                                 dw 0                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 0                                    ; Nature
// MGC dseg:2AD0                                                 dw 0                                    ; Death
// MGC dseg:2AD0                                                 dw 0Ah                                  ; Chaos
// MGC dseg:2AD0                                                 dw _Chaos_Mastery                       ; Retort
// MGC dseg:2AD0                                                 db 46h, 72h, 65h, 79h, 61h, 5 dup(0)    ; Name
// MGC dseg:2AD0                                                 dw 0                                    ; Life
// MGC dseg:2AD0                                                 dw 0                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 0Ah                                  ; Nature
// MGC dseg:2AD0                                                 dw 0                                    ; Death
// MGC dseg:2AD0                                                 dw 0                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Nature_Mastery                      ; Retort
// MGC dseg:2AD0                                                 db 48h, 6Fh, 72h, 75h, 73h, 5 dup(0)    ; Name
// MGC dseg:2AD0                                                 dw 5                                    ; Life
// MGC dseg:2AD0                                                 dw 5                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 0                                    ; Nature
// MGC dseg:2AD0                                                 dw 0                                    ; Death
// MGC dseg:2AD0                                                 dw 0                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Archmage                            ; Retort
// MGC dseg:2AD0                                                 db 41h, 72h, 69h, 65h, 6Ch, 5 dup(0)    ; Name
// MGC dseg:2AD0                                                 dw 0Ah                                  ; Life
// MGC dseg:2AD0                                                 dw 0                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 0                                    ; Nature
// MGC dseg:2AD0                                                 dw 0                                    ; Death
// MGC dseg:2AD0                                                 dw 0                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Charismatic                         ; Retort
// MGC dseg:2AD0                                                 db 54h, 6Ch, 61h, 6Ch, 6Fh, 63h, 4 dup(0); Name
// MGC dseg:2AD0                                                 dw 0                                    ; Life
// MGC dseg:2AD0                                                 dw 0                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 4                                    ; Nature
// MGC dseg:2AD0                                                 dw 5                                    ; Death
// MGC dseg:2AD0                                                 dw 0                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Warlord                             ; Retort
// MGC dseg:2AD0                                                 db 4Bh, 61h, 6Ch, 69h, 6 dup(0)         ; Name
// MGC dseg:2AD0                                                 dw 0                                    ; Life
// MGC dseg:2AD0                                                 dw 5                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 0                                    ; Nature
// MGC dseg:2AD0                                                 dw 5                                    ; Death
// MGC dseg:2AD0                                                 dw 0                                    ; Chaos
// MGC dseg:2AD0                                                 dw _Artificer                           ; Retort
// MGC dseg:2AD0                                                 db 43h, 75h, 73h, 74h, 6Fh, 6Dh, 4 dup(0); Name
// MGC dseg:2AD0                                                 dw 0                                    ; Life
// MGC dseg:2AD0                                                 dw 0                                    ; Sorcery
// MGC dseg:2AD0                                                 dw 0                                    ; Nature
// MGC dseg:2AD0                                                 dw 0                                    ; Death
// MGC dseg:2AD0                                                 dw 0                                    ; Chaos
// MGC dseg:2AD0                                                 dw _No_Retort                           ; Retort
// MGC dseg:2C1A 02 00 03 00 04 00 05 00 06 00 07 00 08 00 09 00+RP_Book_Table dw 2, 3, 4, 5, 6, 7, 8, 9, 0Ah, 0Bh ; repurposed in the worldgen customizer
// MGC dseg:2C2E 01 00 02 00 03 00 04 00 05 00 06 00 07 00 08 00+TBL_SpellsPerBook_C dw 1, 2, 3, 4, 5, 6, 7, 8, 9, 0Ah
// MGC dseg:2C2E 09 00 0A 00                                                                             ; DATA XREF: GAME_New_Screen_5+2E7r ...
// MGC dseg:2C42 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_SpellsPerBook_U dw 9 dup(0), 2      ; DATA XREF: GAME_New_Screen_5+313r ...
// MGC dseg:2C56 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_SpellsPerBook_R dw 9 dup(0), 1      ; DATA XREF: GAME_New_Screen_5+33Fr ...
// MGC dseg:2C6A 06 00 07 00 09 00 08 00 04 00 05 00 01 00 03 00+TBL_Def_Spl_Nature  = Default_Spells ptr $
// MGC dseg:2C6A 02 00 0A 00 11 00 0F 00 1B 00                                                           ; DATA XREF: GAME_New_Screen_5+27Co ...
// MGC dseg:2C6A                                                 dw War_Bears, Stone_Skin, Sprites, Water_Walking, Giant_Strength, Web, Earth_to_Mud, Wall_of_Stone, Resist_Elements, Earth_Lore; Common
// MGC dseg:2C6A                                                 dw Cockatrice, Change_Terrain           ; Uncommon
// MGC dseg:2C6A                                                 dw Gorgons                              ; Rare
// MGC dseg:2C84 31 00 32 00 2D 00 2B 00 2E 00 30 00 2F 00 2A 00+TBL_Def_Spl_Sorcery  = Default_Spells ptr $
// MGC dseg:2C84 29 00 2C 00 38 00 3C 00 41 00                                                           ; DATA XREF: GAME_New_Screen_5+266o ...
// MGC dseg:2C84                                                 dw Nagas, Psionic_Blast, Phantom_Warriors, Floating_Island, Confusion, Counter_Magic, Word_of_Recall, Dispel_Magic_True, Resist_Magic, Guardian_Wind; Common
// MGC dseg:2C84                                                 dw Flight, Phantom_Beast                ; Uncommon
// MGC dseg:2C84                                                 dw Storm_Giant                          ; Rare
// MGC dseg:2C9E 53 00 5A 00 56 00 54 00 55 00 59 00 58 00 57 00+TBL_Def_Spl_Chaos  = Default_Spells ptr $
// MGC dseg:2C9E 52 00 51 00 5B 00 61 00 6A 00                                                           ; DATA XREF: GAME_New_Screen_5+250o ...
// MGC dseg:2C9E                                                 dw Fire_Bolt, Fire_Elemental, Eldritch_Weapon, Hell_Hounds, Corruption, Warp_Creature, Shatter, Wall_of_Fire, Disrupt, Warp_Wood; Common
// MGC dseg:2C9E                                                 dw Lightning_Bolt, Doombat              ; Uncommon
// MGC dseg:2C9E                                                 dw Efreet                               ; Rare
// MGC dseg:2CB8 82 00 81 00 7E 00 7F 00 7D 00 7C 00 7A 00 7B 00+TBL_Def_Spl_Life  = Default_Spells ptr $
// MGC dseg:2CB8 80 00 79 00 85 00 88 00 93 00                                                           ; DATA XREF: GAME_New_Screen_5+292o ...
// MGC dseg:2CB8                                                 dw Heroism, Guardian_Spirit, Holy_Armor, Just_Cause, Healing, Holy_Weapon, Star_Fires, Endurance, True_Light, Bless; Common
// MGC dseg:2CB8                                                 dw Resurrection, Unicorns               ; Uncommon
// MGC dseg:2CB8                                                 dw Angel                                ; Rare
// MGC dseg:2CD2 A7 00 A6 00 A2 00 A3 00 A5 00 A9 00 A8 00 A1 00+TBL_Def_Spl_Death  = Default_Spells ptr $
// MGC dseg:2CD2 AA 00 A4 00 AE 00 AF 00 B8 00                                                           ; DATA XREF: GAME_New_Screen_5:loc_41D07o ...
// MGC dseg:2CD2                                                 dw Life_Drain, Ghouls, Weakness, Dark_Rituals, Black_Sleep, Darkness, Terror, Skeletons, Mana_Leak, Cloak_of_Fear; Common
// MGC dseg:2CD2                                                 dw Black_Prayer, Black_Channels         ; Uncommon
// MGC dseg:2CD2                                                 dw Wraiths                              ; Rare
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Retort_Reqs Retort_Req 2 dup(<0, 0, 0, 0, 0, 0, 0>)
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00+                                        ; DATA XREF: GAME_New_Screen_4+167o ...
// MGC dseg:2CEC 00 00 00 00 04 00 00 00 00 00 00 00 01 00 00 00+Retort_Req <0, 1, 0, 0, 4, 0, 0>
// MGC dseg:2CEC 00 00 00 00 04 00 00 00 00 00 01 00 00 00 00 00+Retort_Req <0, 1, 0, 0, 0, 4, 0>
// MGC dseg:2CEC 00 00 00 00 04 00 00 00 01 00 00 00 04 00 00 00+Retort_Req <0, 1, 0, 0, 0, 0, 4>
// MGC dseg:2CEC 00 00 00 00 00 00 01 00 04 00 00 00 00 00 00 00+Retort_Req <0, 1, 0, 4, 0, 0, 0>
// MGC dseg:2CEC 00 00 01 00 02 00 00 00 00 00 00 00 00 00 00 00+Retort_Req <0, 1, 4, 0, 0, 0, 0>
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Retort_Req <1, 2, 0, 0, 0, 0, 0>
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 04 00 01 00+Retort_Req 2 dup(<0, 0, 0, 0, 0, 0, 0>)
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 04 00 01 00 00 00+Retort_Req 2 dup(<4, 1, 0, 0, 0, 0, 0>)
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 03 00 00 00 00 00+Retort_Req <0, 3, 0, 0, 1, 1, 1>
// MGC dseg:2CEC 01 00 01 00 01 00 00 00 00 00 00 00 00 00 00 00+Retort_Req <0, 0, 0, 0, 0, 0, 0>
// MGC dseg:2CEC 00 00 00 00 02 00 03 00 00 00 00 00 00 00 00 00+Retort_Req <2, 3, 0, 0, 0, 0, 0>
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Retort_Req  3 dup(<0>)
// MGC dseg:2DE8 00 00 01 00 08 00 0A 00 11 00 0F 00 07 00 09 00+TBL_Retort_Offsets dw RETORTS, RETORTS.Warlord, RETORTS.Channeler, RETORTS.Archmage, RETORTS.Artificer, RETORTS.Conjurer, RETORTS.Sage_Master, RETORTS.Myrran, RETORTS.Divine_Power, RETORTS.Famous, RETORTS.Runemaster, RETORTS.Charismatic, RETORTS.Chaos_Mastery, RETORTS.Nature_Mastery, RETORTS.Sorcery_Mastery, RETORTS.Infernal_Power, RETORTS.Mana_Focusing, RETORTS.Node_Mastery
// MGC dseg:2DE8 06 00 0D 00 0E 00 10 00 02 00 03 00 04 00 05 00+                                        ; DATA XREF: GAME_New_Screen_4+71Fr ...
// MGC dseg:2E0C 66 66 66 66                                     COL_SaveLoad1 db 4 dup( 66h)            ; DATA XREF: Load_Screen_Draw+Br ...
// MGC dseg:2E10 60 61 60 61                                     COL_SaveLoad2 db  60h, 61h, 60h, 61h    ; DATA XREF: Load_Screen_Draw+18r ...

// MGC dseg:2E14
uint8_t RP_COL_NEWG_4Shadow[4] = { 31, 31, 31, 31 };
// MGC dseg:2E18
uint8_t RP_COL_NEWG_4Font[4] = { 187, 187, 187, 187 };

// MGC dseg:2E1C
/*
¿ struct ?
55 bytes
5 * 11
*/
// cnst_Diff0_Text_11b db 'Intro',0,0,0,0,0,0
// cnst_Diff1_Text_11b db 'Easy',0,0,0,0,0,0,0
// cnst_Diff2_Text_11b db 'Normal',0,0,0,0,0
// cnst_Diff3_Text_11b db 'Hard',0,0,0,0,0,0,0
// cnst_Diff4_Text_11b db 'Impossible',0
struct s_DIFFICULTY_NAMES
{
    char name[11];
};
struct s_DIFFICULTY_NAMES _difficulty_names[5] =
{
    { 'I', 'n', 't', 'r', 'o',  '\0', '\0', '\0', '\0', '\0', '\0' },
    { 'E', 'a', 's', 'y', '\0', '\0', '\0', '\0', '\0', '\0', '\0' },
    { 'N', 'o', 'r', 'm', 'a',  'l',  '\0', '\0', '\0', '\0', '\0' },
    { 'H', 'a', 'r', 'd', '\0', '\0', '\0', '\0', '\0', '\0', '\0' },
    { 'I', 'm', 'p', 'o', 's',  's',  'i',  'b',  'l',  'e',  '\0' }
};

// MGC dseg:2E53
// cnst_LandS0_Text_7b db 'Small',0,0
// cnst_LandS1_Text_7b db 'Medium',0
// cnst_LandS2_Text_7b db 'Large',0,0
struct s_LAND_SIZE_NAMES
{
    char name[7];
};
struct s_LAND_SIZE_NAMES _land_size_names[3] =
{
    { 'S', 'm', 'a', 'l', 'l', '\0', '\0' },
    { 'M', 'e', 'd', 'i', 'u', 'm',  '\0' },
    { 'L', 'a', 'r', 'g', 'e', '\0', '\0' }
};

// MGC dseg:2E68
// cnst_Magic0_Text_9b db 'Weak',0,0,0,0,0
// cnst_Magic1_Text_9b db 'Normal',0,0,0
// cnst_Magic2_Text_9b db 'Powerful',0
struct s_MAGIC_STRENGTH_NAMES
{
    char name[9];
};
struct s_MAGIC_STRENGTH_NAMES _magic_strength_names[3] =
{
    { 'W', 'e', 'a', 'k', '\0', '\0', '\0', '\0', '\0' },
    { 'N', 'o', 'r', 'm', 'a',  'l',  '\0', '\0', '\0' },
    { 'P', 'o', 'w', 'e', 'r',  'f',  'u',  'l',  '\0' }
};

// MGC dseg:2E83
// cnst_Opnts0_Text_8b db 'One',0,0,0,0,0
// cnst_Opnts1_Text_8b db 'Two',0,0,0,0,0
// cnst_Opnts2_Text_8b db 'Three',0,0,0
// cnst_Opnts3_Text_8b db 'Four',0,0,0,0
struct s_OPPONENT_COUNT_NAMES
{
    char name[8];
};
struct s_OPPONENT_COUNT_NAMES _opponent_count_names[4] =
{
    { 'O', 'n', 'e', '\0', '\0', '\0', '\0', '\0' },
    { 'T', 'w', 'o', '\0', '\0', '\0', '\0', '\0' },
    { 'T', 'h', 'r', 'e',  'e',  '\0', '\0', '\0' },
    { 'F', 'o', 'u', 'r',  '\0', '\0', '\0', '\0' }
};

// MGC dseg:2EA3 1F 1F 1F 1F 1F 1F 1F 00                         COL_NEWG_FontShadow db 7 dup(1Fh), 0    ; DATA XREF: Draw_NewGame_Screen1+Do
// MGC dseg:2EAB BB BB BB BB BB BB BB BB                         COL_NEWG_Font db 8 dup(0BBh)            ; DATA XREF: Draw_NewGame_Screen1+1Fo
// MGC dseg:2EB3 B7 B2                                           COL_NEWG_Retorts dw 0B2B7h              ; DATA XREF: Draw_NewGame_Screen1+2Cr
// MGC dseg:2EB5 1F 1F 1F 1F 1F 1F 1F 00                         RP_COL_NEWG_FontShadow2 db 7 dup(1Fh), 0
// MGC dseg:2EB5                                                                                         ; DATA XREF: GAME_Draw_NewScr3+Bo
// MGC dseg:2EB5                                                                                         ; redirected to dseg:2ea3 and repurposed in the
// MGC dseg:2EB5                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2EBD BB BB BB BB BB BB BB BB                         RP_COL_NEWG_Font2 db 8 dup(0BBh)        ; DATA XREF: GAME_Draw_NewScr3+1Do
// MGC dseg:2EBD                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2EBD                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2EC5 BB BB BB BB BB BB BB BB                         RP_COL_NEWG_Font3 db 8 dup(0BBh)        ; DATA XREF: GAME_New_Screen_7+Do
// MGC dseg:2EC5                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2EC5                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2ECD 1F 1F 1F 1F 1F                                  RP_COL_NEWG_5Shadow db 5 dup(1Fh)       ; DATA XREF: GAME_Draw_NewScr7+Do
// MGC dseg:2ECD                                                                                         ; redirected to dseg:2ea3 and repurposed in the
// MGC dseg:2ECD                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2ED2 BB BB BB BB BB                                  RP_COL_NEWG_5Font db 5 dup(0BBh)        ; DATA XREF: GAME_Draw_NewScr7+1Fo
// MGC dseg:2ED2                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2ED2                                                                                         ; worldgen customizer / last profile loader

// MGC dseg:2ED7 00                                              align 2                                 ; 2ed6 is still free

// MGC dseg:2ED8 00 00 05 00 06 00 07 00 08 00 09 00 0A 00 0B 00+TBL_Arcanian_Races dw R_Barbarian, R_Gnoll, R_Halfling, R_High_Elf, R_High_Man, R_Klackon, R_Lizardman, R_Nomad, R_Orc
// MGC dseg:2ED8 0C 00                                                                                   ; DATA XREF: GAME_New_Screen_6+Do
// MGC dseg:2EEA 01 00 02 00 03 00 04 00 0D 00                   TBL_Myrran_Races dw R_Beastman, R_Dark_Elf, R_Draconian, R_Dwarf, R_Troll
// MGC dseg:2EEA                                                                                         ; DATA XREF: GAME_New_Screen_6+1Fo
// MGC dseg:2EF4 BB BB BB BB BB BB BB BB                         RP_COL_NEWG_Font4 db 8 dup(0BBh)        ; DATA XREF: GAME_New_Screen_6+31o
// MGC dseg:2EF4                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2EF4                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2EFC 14 B0                                           COL_NEWG_Highlight dw 0B014h            ; DATA XREF: GAME_New_Screen_6+3Er
// MGC dseg:2EFE 19 32                                           COL_Available dw 3219h                  ; DATA XREF: GAME_New_Screen_6+44r
// MGC dseg:2F00 02 02                                           COL_NEWG_RacesShadow dw 202h            ; DATA XREF: GAME_New_Screen_6+4Ar
// MGC dseg:2F02 00 00 05 00 06 00 07 00 08 00 09 00 0A 00 0B 00+RP_TBL_Arcanian_Races_2 dw R_Barbarian, R_Gnoll, R_Halfling, R_High_Elf, R_High_Man, R_Klackon, R_Lizardman, R_Nomad, R_Orc
// MGC dseg:2F02 0C 00                                                                                   ; DATA XREF: GAME_Draw_NewScr6+Do
// MGC dseg:2F02                                                                                         ; redirected to dseg:2ed8 and repurposed in the
// MGC dseg:2F02                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2F14 01 00 02 00 03 00 04 00 0D 00                   RP_TBL_Myrran_Races_2 dw R_Beastman, R_Dark_Elf, R_Draconian, R_Dwarf, R_Troll
// MGC dseg:2F14                                                                                         ; DATA XREF: GAME_Draw_NewScr6+1Fo
// MGC dseg:2F14                                                                                         ; redirected to dseg:2eea and repurposed in the
// MGC dseg:2F14                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2F1E 1F 1F 1F 1F 1F                                  RP_COL_NEWG_5Shadow2 db 5 dup(1Fh)      ; DATA XREF: GAME_Draw_NewScr6+31o
// MGC dseg:2F1E                                                                                         ; redirected to dseg:2ea3 and repurposed in the
// MGC dseg:2F1E                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2F23 BB BB BB BB BB                                  RP_COL_NEWG_5Font2 db 5 dup(0BBh)       ; DATA XREF: GAME_Draw_NewScr6+43o
// MGC dseg:2F23                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2F23                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2F28 B4 B4 B4 B4 B4                                  COL_NEWG_RaceTitles db 5 dup(0B4h)      ; DATA XREF: GAME_Draw_NewScr6+55o
// MGC dseg:2F2D 14 B0                                           COL_NEWG_Highlight2 dw 0B014h           ; DATA XREF: GAME_Draw_NewScr6+62r
// MGC dseg:2F2D                                                                                         ; should use dseg:2efc
// MGC dseg:2F2F 19 32                                           COL_Available_2 dw 3219h                ; DATA XREF: GAME_Draw_NewScr6+68r
// MGC dseg:2F2F                                                                                         ; should use dseg:2efe
// MGC dseg:2F31 19 16                                           COL_NEWG_NA dw 1619h                    ; DATA XREF: GAME_Draw_NewScr6+6Er
// MGC dseg:2F33 02 02                                           COL_NEWG_RacesShdw2 dw 202h             ; DATA XREF: GAME_Draw_NewScr6+74r
// MGC dseg:2F33                                                                                         ; should use dseg:2f00
// MGC dseg:2F35 BB BB BB BB                                     COL_NEWG_4Font2 db 4 dup(0BBh)          ; DATA XREF: GAME_DrawRetortsStr+Cr ...
// MGC dseg:2F35                                                                                         ; should use dseg:2eab
// MGC dseg:2F39 B7 B2                                           COL_NEWG_Retorts2 dw 0B2B7h             ; DATA XREF: GAME_DrawRetortsStr+16r
// MGC dseg:2F39                                                                                         ; should use dseg:2eb3

// MGC dseg:2F3B 00                                              align 2

// MGC dseg:2F3C FF FF FF FF FF FF FF FF FF FF FF FF             CRP_Unused_Array db 0Ch dup(0FFh)       ; DATA XREF: GAME_DrawRetortsStr+22o
// MGC dseg:2F48 4C 69 66 65 00 00 00 00 00 00                   cnst_Realm0_Text_10b db 'Life',0,0,0,0,0,0
// MGC dseg:2F48                                                                                         ; DATA XREF: GAME_New_Screen_4+Fo
// MGC dseg:2F52 44 65 61 74 68 00 00 00 00 00                   cnst_Realm1_Text_10b db 'Death',0,0,0,0,0
// MGC dseg:2F5C 43 68 61 6F 73 00 00 00 00 00                   cnst_Realm2_Text_10b db 'Chaos',0,0,0,0,0
// MGC dseg:2F66 4E 61 74 75 72 65 00 00 00 00                   cnst_Realm3_Text_10b db 'Nature',0,0,0,0
// MGC dseg:2F70 53 6F 72 63 65 72 79 00 00 00                   cnst_Realm4_Text_10b db 'Sorcery',0,0,0
// MGC dseg:2F7A 01 00 02 00 08 00 09 00 0F 00                   TBL_2pickRetort_Lbls dw RETORT_Labels.Warlord, RETORT_Labels.Channeler, RETORT_Labels.Divine_Power, RETORT_Labels.Famous, RETORT_Labels.Infernal_Power
// MGC dseg:2F7A                                                                                         ; DATA XREF: GAME_New_Screen_4+21o
// MGC dseg:2F84 01 00 02 00 08 00 09 00 0F 00                   RP_TBL_2pickRet_Lbls dw RETORT_Labels.Warlord, RETORT_Labels.Channeler, RETORT_Labels.Divine_Power, RETORT_Labels.Famous, RETORT_Labels.Infernal_Power
// MGC dseg:2F84                                                                                         ; DATA XREF: GAME_Draw_NewScr4+Do
// MGC dseg:2F84                                                                                         ; redirected to dseg:2f7a and re-purposed in the realm
// MGC dseg:2F84                                                                                         ; ordering patch
// MGC dseg:2F8E 1F 1F 1F 1F 1F                                  COL_NEWG_5Shadow3 db 5 dup(1Fh)         ; DATA XREF: GAME_Draw_NewScr4+1Fo
// MGC dseg:2F8E                                                                                         ; should use dseg:2ea3
// MGC dseg:2F93 BB BB BB BB BB                                  COL_NEWG_5Font3 db 5 dup(0BBh)          ; DATA XREF: GAME_Draw_NewScr4+31o
// MGC dseg:2F93                                                                                         ; should use dseg:2eab
// MGC dseg:2F98 19 32                                           COL_Available_3 dw 3219h                ; DATA XREF: GAME_Draw_NewScr4+3Er
// MGC dseg:2F98                                                                                         ; should use dseg:2efe
// MGC dseg:2F9A 19 16                                           COL_NEWG_NA2 dw 1619h                   ; DATA XREF: GAME_Draw_NewScr4+44r
// MGC dseg:2F9A                                                                                         ; should use dseg:2f31
// MGC dseg:2F9C 16 B2                                           COL_NewG_Ret_Known dw 0B216h            ; DATA XREF: GAME_Draw_NewScr4+4Ar
// MGC dseg:2F9E 10 10                                           COL_NewG_Ret_Shdw dw 1010h              ; DATA XREF: GAME_Draw_NewScr4+50r
// MGC dseg:2FA0 01 01 01 00 01 01 00 01 00 01 00 01 00 00 00 00+TBL_Retort_Lbl_Init RETORT_Labels <1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0>
// MGC dseg:2FA0 00 00                                                                                   ; DATA XREF: GAME_Draw_NewScr4+5Bo
// MGC dseg:2FB2 03 00 04 00 02 00 00 00 01 00                   TBL_Realm_Order dw _Life, _Death, _Chaos, _Nature, _Sorcery
// MGC dseg:2FB2                                                                                         ; DATA XREF: GAME_New_Screen_5+Eo
// MGC dseg:2FBC 1A 00 4D 00 80 00                               UU_Label_Box_TitleTops dw 26, 77, 128   ; DATA XREF: GAME_SpellSel_GUI+Do
// MGC dseg:2FC2 25 00 58 00 8B 00                               UU_Label_Box_TextTops dw 37, 88, 139    ; DATA XREF: GAME_SpellSel_GUI+1Fo
// MGC dseg:2FC8 1F 1F 1F 1F 1F                                  COL_NEWG_5Shadow4 db 5 dup(1Fh)         ; DATA XREF: SCRN_Draw_NewScr5+Do
// MGC dseg:2FC8                                                                                         ; should use dseg:2ea3
// MGC dseg:2FCD BB BB BB BB BB                                  COL_NEWG_5Font4 db 5 dup(0BBh)          ; DATA XREF: SCRN_Draw_NewScr5+1Fo
// MGC dseg:2FCD                                                                                         ; should use dseg:2eab
// MGC dseg:2FD2 1A 00 4D 00 80 00                               Label_Box_TitleTops2 dw 26, 77, 128     ; DATA XREF: SCRN_Draw_NewScr5_2+Eo
// MGC dseg:2FD2                                                                                         ; should use dseg:2fbc
// MGC dseg:2FD8 25 00 58 00 8B 00                               Label_Box_TextTops2 dw 37, 88, 139      ; DATA XREF: SCRN_Draw_NewScr5_2+20o
// MGC dseg:2FD8                                                                                         ; should use dseg:2fc2
// MGC dseg:2FDE 19 32                                           COL_Available_4 dw 3219h                ; DATA XREF: SCRN_Draw_NewScr5_2+2Dr
// MGC dseg:2FDE                                                                                         ; should use dseg:2efe
// MGC dseg:2FE0 16 B2                                           COL_NewG_Spl_Known dw 0B216h            ; DATA XREF: SCRN_Draw_NewScr5_2+33r
// MGC dseg:2FE0                                                                                         ; should use dseg:2f9c
// MGC dseg:2FE2 10 10                                           COL_NewG_Spl_Shdw dw 1010h              ; DATA XREF: SCRN_Draw_NewScr5_2+39r
// MGC dseg:2FE2                                                                                         ; should use dseg:2f9e
// MGC dseg:2FE4 43 6F 6D 6D 6F 6E 00 00 00 00                   cnst_Rarity0_10b db 'Common',0,0,0,0    ; DATA XREF: SCRN_Draw_NewScr5_2+44o
// MGC dseg:2FEE 55 6E 63 6F 6D 6D 6F 6E 00 00                   cnst_Rarity1_10b db 'Uncommon',0,0
// MGC dseg:2FF8 52 61 72 65 00 00 00 00 00 00                   cnst_Rarity2_10b db 'Rare',0,0,0,0,0,0
// MGC dseg:3002 56 65 72 79 20 52 61 72 65 00                   cnst_Rarity3_10b db 'Very Rare',0
// MGC dseg:300C 4E 61 74 75 72 65 00 00 00 00                   cnst_Realm0_10b db 'Nature',0,0,0,0     ; DATA XREF: SCRN_Draw_NewScr5_2+57o
// MGC dseg:300C                                                                                         ; should use / convert dseg:2f48
// MGC dseg:3016 53 6F 72 63 65 72 79 00 00 00                   cnst_Realm1_10b db 'Sorcery',0,0,0
// MGC dseg:3020 43 68 61 6F 73 00 00 00 00 00                   cnst_Realm2_10b db 'Chaos',0,0,0,0,0
// MGC dseg:302A 4C 69 66 65 00 00 00 00 00 00                   cnst_Realm3_10b db 'Life',0,0,0,0,0,0
// MGC dseg:3034 44 65 61 74 68 00 00 00 00 00                   cnst_Realm4_10b db 'Death',0,0,0,0,0
// MGC dseg:303E 57 49 5A 41 52 44 53 2E 45 58 45                cnst_EXESwap_File db 'WIZARDS.EXE'      ; DATA XREF: GAME_WizardsLaunch+38o

// MGC dseg:3049
char empty_string__ovr050[] = "";

// MGC dseg:304A 4A 45 4E 4E 59 00                               cnst_EXESwap_Arg db 'JENNY',0           ; DATA XREF: GAME_WizardsLaunch+30o ...
// MGC dseg:3050 4C 4F 41 44 2E 4C 42 58 00                      load_lbx_file db 'LOAD.LBX',0           ; DATA XREF: Load_Screen+Fo ...

// MGC dseg:3059
char str_ESC__ovr050[] = "\x1B";

// MGC dseg:305B 4C 00                                           cnst_HOTKEY_L_2 db 'L',0                ; DATA XREF: Load_Screen+196o
// MGC dseg:305D 53 41 56 45 00                                  cnst_SAVE3 db 'SAVE',0                  ; DATA XREF: Load_Screen:@@Loop_SaveSlotso
// MGC dseg:3062 2E 47 41 4D 00                                  cnst_SAVE_ext3 db '.GAM',0              ; DATA XREF: Load_Screen+1FDo
// MGC dseg:3067 43 6F 70 79 72 69 67 68 74 20 20 53 69 6D 74 65+load_screen_copyright db 'Copyright  Simtex Software, 1995   V1.31',0

// MGC dseg:3090
char newgame_lbx_file__ovr050[] = "NEWGAME.LBX";

// MGC dseg:309C
char str_MAGIC_SET__ovr050[] = "MAGIC.SET";

// MGC dseg:30A6
char str_rb__ovr050[] = "rb";

// MGC dseg:30A9
char str_wb__ovr050[] = "wb";

// MGC dseg:30AC 53 65 6C 65 63 74 20 57 69 7A 61 72 64 00       cnst_Wiz_Select db 'Select Wizard',0    ; DATA XREF: Draw_NewGame_Screen1+87o
// MGC dseg:30BA 53 65 6C 65 63 74 20 50 69 63 74 75 72 65 00    cnst_Pic_Select db 'Select Picture',0   ; DATA XREF: Draw_NewGame_Screen1:loc_3EE5Do
// MGC dseg:30C9 2E 00                                           cnst_DOT db '.',0                       ; DATA XREF: Draw_NewGame_Screen1+50Eo ...
// MGC dseg:30CB 57 69 7A 61 72 64 27 73 20 4E 61 6D 65 00       cnst_Name_Select db 'Wizard',27h,'s Name',0
// MGC dseg:30D9 53 65 6C 65 63 74 20 42 61 6E 6E 65 72 00       cnst_Banner_Select db 'Select Banner',0 ; DATA XREF: GAME_Draw_NewScr7+60o
// MGC dseg:30E7 59 6F 75 20 63 61 6E 20 6E 6F 74 20 73 65 6C 65+cnst_Race_Error db 'You can not select a Myrran race unless you have the Myrran special.',0
// MGC dseg:312C 53 65 6C 65 63 74 20 52 61 63 65 00             cnst_Race_Select db 'Select Race',0     ; DATA XREF: GAME_Draw_NewScr6+C3o
// MGC dseg:3138 41 72 63 61 6E 69 61 6E 20 52 61 63 65 73 3A 00 cnst_Arcanian_Races db 'Arcanian Races:',0
// MGC dseg:3148 4D 79 72 72 61 6E 20 52 61 63 65 73 3A 00       cnst_Myrran_Races db 'Myrran Races:',0  ; DATA XREF: GAME_Draw_NewScr6+337o
// MGC dseg:3156 59 6F 75 20 6E 65 65 64 20 74 6F 20 6D 61 6B 65+cnst_Pick_Error_0 db 'You need to make all your picks before you can continue',0
// MGC dseg:318E 59 6F 75 20 6D 61 79 20 6E 6F 74 20 73 65 6C 65+cnst_Pick_Error_1 db 'You may not select more than 6 special abilities',0
// MGC dseg:31BF 54 6F 20 73 65 6C 65 63 74 20 00                cnst_Pick_Error_20 db 'To select ',0    ; DATA XREF: GAME_New_Screen_4:loc_40AA3o
// MGC dseg:31CA 20 79 6F 75 20 6E 65 65 64 3A 20 20 20 00       cnst_Pick_Error_21 db ' you need:   ',0 ; DATA XREF: GAME_New_Screen_4+44Co
// MGC dseg:31D8 20 70 69 63 6B 73 20 69 6E 20 00                cnst_Pick_Error_22 db ' picks in ',0    ; DATA XREF: GAME_New_Screen_4+4D8o
// MGC dseg:31E3 20 4D 61 67 69 63 00                            cnst_Pick_Error_23 db ' Magic',0        ; DATA XREF: GAME_New_Screen_4+502o ...
// MGC dseg:31EA 20 70 69 63 6B 00                               cnst_Pick_Error_24 db ' pick',0         ; DATA XREF: GAME_New_Screen_4+561o
// MGC dseg:31F0 73                                              cnst_Pick_Error_25 db 's'               ; DATA XREF: GAME_New_Screen_4+583o
// MGC dseg:31F1 20 69 6E 20 61 6E 79 20 00                      cnst_Pick_Error_26 db ' in any ',0      ; DATA XREF: GAME_New_Screen_4:loc_40C05o
// MGC dseg:31FA 20 52 65 61 6C 6D 73 20 6F 66 20 4D 61 67 69 63+cnst_Pick_Error_27 db ' Realms of Magic',0
// MGC dseg:31FA 00                                                                                      ; DATA XREF: GAME_New_Screen_4+5DFo
// MGC dseg:320B 52 65 61 6C 6D 20 6F 66 20 4D 61 67 69 63 00    cnst_Pick_Error_28 db 'Realm of Magic',0
// MGC dseg:320B                                                                                         ; DATA XREF: GAME_New_Screen_4:loc_40C61o
// MGC dseg:321A 2C 20 00                                        cnst_Pick_Error_29 db ', ',0            ; DATA XREF: GAME_New_Screen_4+623o ...
// MGC dseg:321D 20 70 69 63 6B 20 69 6E 20 00                   cnst_Pick_Error_2A db ' pick in ',0     ; DATA XREF: GAME_New_Screen_4+669o
// MGC dseg:3227 59 6F 75 20 63 61 6E 20 6E 6F 74 20 73 65 6C 65+cnst_Pick_Error_3 db 'You can not select both Life and Death magic',0
// MGC dseg:3227 63 74 20 62 6F 74 68 20 4C 69 66 65 20 61 6E 64+                                        ; DATA XREF: GAME_New_Screen_4:loc_40D2Eo
// MGC dseg:3254 59 6F 75 20 68 61 76 65 20 61 6C 72 65 61 64 79+cnst_Pick_Error_4 db 'You have already made all your'
// MGC dseg:3254 20 6D 61 64 65 20 61 6C 6C 20 79 6F 75 72                                               ; DATA XREF: GAME_New_Screen_4:loc_40D34o
// MGC dseg:3272 20 70 69 63 6B 73 00                            cnst_Picks db ' picks',0                ; DATA XREF: GAME_Draw_NewScr4+AA9o ...
// MGC dseg:3279 59 6F 75 20 64 6F 6E 27 74 20 68 61 76 65 20 65+cnst_Pick_Error_5 db 'You don',27h,'t have enough picks left to make this selection. You need 2 picks',0
// MGC dseg:3279 6E 6F 75 67 68 20 70 69 63 6B 73 20 6C 65 66 74+                                        ; DATA XREF: GAME_New_Screen_4:loc_40D3Ao

// MGC dseg:32C3
char hlpentry_lbx_file__MGC_ovr050[] = "hlpentry.lbx";

// MGC dseg:32D0 59 6F 75 20 68 61 76 65 20 6E 6F 20 70 69 63 6B+cnst_Pick_Error_6 db 'You have no picks left in this area, to deselect click on a selected item',0
// MGC dseg:32D0 73 20 6C 65 66 74 20 69 6E 20 74 68 69 73 20 61+                                        ; DATA XREF: GAME_New_Screen_5:loc_42154o
// MGC dseg:331A 53 65 6C 65 63 74 20 00                         cnst_Spell_Select_0 db 'Select ',0      ; DATA XREF: SCRN_Draw_NewScr5_2+DDo
// MGC dseg:3322 20 53 70 65 6C 6C 73 00                         cnst_Spell_Select_1 db ' Spells',0      ; DATA XREF: SCRN_Draw_NewScr5_2+108o
// MGC dseg:332A 3A 20 00                                        cnst_Spell_Select_2 db ': ',0           ; DATA XREF: SCRN_Draw_NewScr5_2+22Bo ...
// MGC dseg:332D 20 61 6E 64 20 00                               cnst_And db ' and ',0                   ; DATA XREF: STR_ListSeparator+1Co

// MGC dseg:3333 00                                              align 2

// MGC dseg:3334 00 00 FF FF 00 00 01 00 00 00                   TILE_Cardinal_XMod dw 0, 0FFFFh, 0, 1, 0
// MGC dseg:3334                                                                                         ; DATA XREF: NEWG_CreateLands+240r ...
// MGC dseg:333E 01 00 00 00 FF FF 00 00 00 00                   TILE_Cardinal_YMod dw 1, 0, 0FFFFh, 0, 0
// MGC dseg:333E                                                                                         ; DATA XREF: NEWG_CreateLands+24Fr ...
// MGC dseg:3348 02 00 03 00 00 00 01 00                         TILE_OppositeDirs dw 2, 3, 0, 1         ; DATA XREF: NEWG_CreateRiver+1F8r
// MGC dseg:3350 00 00 FF FF 00 00 01 00 00 00                   TILE_Cardinal_XMod2 dw 0, 0FFFFh, 0, 1, 0
// MGC dseg:3350                                                                                         ; DATA XREF: NEWG_SetSpecLands+1A3r ...
// MGC dseg:3350                                                                                         ; should use dseg:3334
// MGC dseg:335A 01 00 00 00 FF FF 00 00 00 00                   TILE_Cardinal_YMod2 dw 1, 0, 0FFFFh, 0, 0
// MGC dseg:335A                                                                                         ; DATA XREF: NEWG_SetSpecLands+1B2r ...
// MGC dseg:335A                                                                                         ; should use dseg:333e
// MGC dseg:3364                                                 ; #110 NEWG_Hourglass_Wnd
// MGC dseg:3364 06 00 00 00 00 00 00 00 3F 01 C7 00             NEWG_Hourglass_Wnd s_MOUSE_LIST <Crsr_Hourglass, 0, 0, 0, 319, 199>
// MGC dseg:3364                                                                                         ; DATA XREF: NEWG_CreateWorld+2Do
// MGC dseg:3364                                                                                         ; should use dseg:1f6c
// MGC dseg:3370 26 34 2C 34 37 34 43 34 50 34 60 34 68 34 77 34+UU_EZ_Names@_Array dw offset UU_cnst_Tower, offset UU_cnst_ChaosNode, offset UU_cnst_NatureNode, offset UU_cnst_SorceryNode, offset UU_cnst_Cave, offset UU_cnst_Dungeon, offset UU_cnst_AncientTmpl, offset UU_cnst_Keep, offset UU_cnst_Lair, offset UU_cnst_Ruins, offset UU_cnst_FallenTmpl ; "tower"
// MGC dseg:3386 A2 00 A2 00 A2 00 A2 00 BB 00 BB 00 BB 00 BB 00+TILE_River_Types dw _Grasslands1, _Grasslands1, _Grasslands1, _Grasslands1
// MGC dseg:3386 BC 00 BC 00 BC 00 BC 00 BD 00 C1 00 E9 00 BD 00+                                        ; DATA XREF: NEWG_SetRiverTiles+127t
// MGC dseg:3386 B9 00 B9 00 B9 00 B9 00 ED 00 EE 00 EF 00 ED 00+dw _River1000, _River1000, _River1000, _River1000
// MGC dseg:3386 BF 00 C3 00 EB 00 BF 00 FB 00 FC 00 FD 00 FE 00+dw _River0100, _River0100, _River0100, _River0100
// MGC dseg:3386 BA 00 BA 00 BA 00 BA 00 C0 00 C4 00 EC 00 C0 00+dw _River1100_1, _River1100_2, _River1100_3, _River1100_1
// MGC dseg:3386 F0 00 F1 00 F2 00 F0 00 F3 00 F4 00 F5 00 F6 00+dw _River0010, _River0010, _River0010, _River0010
// MGC dseg:3386 BE 00 C2 00 EA 00 BE 00 FF 00 00 01 01 01 02 01+dw _River1010_1, _River1010_2, _River1010_3, _River1010_1
// MGC dseg:3386 F7 00 F8 00 F9 00 FA 00 D4 01 D5 01 D6 01 D7 01 dw _River0110_1, _River0110_2, _River0110_3, _River0110_1
// MGC dseg:3386                                                 dw _River1110_1, _River1110_2, _River1110_3, _River1110_4
// MGC dseg:3386                                                 dw _River0001, _River0001, _River0001, _River0001
// MGC dseg:3386                                                 dw _River1001_1, _River1001_2, _River1001_3, _River1001_1
// MGC dseg:3386                                                 dw _River0101_1, _River0101_2, _River0101_3, _River0101_1
// MGC dseg:3386                                                 dw _River1101_1, _River1101_2, _River1101_3, _River1101_4
// MGC dseg:3386                                                 dw _River0011_1, _River0011_2, _River0011_3, _River0011_1
// MGC dseg:3386                                                 dw _River1011_1, _River1011_2, _River1011_3, _River1011_4
// MGC dseg:3386                                                 dw _River0111_1, _River0111_2, _River0111_3, _River0111_4
// MGC dseg:3386                                                 dw _River1111_1, _River1111_2, _River1111_3, _River1111_4
// MGC dseg:3406 42 55 49 4C 44 44 41 54 00                      cnst_BUILDDAT_File db 'BUILDDAT',0      ; DATA XREF: NEWG_CreateWorld+13o
// MGC dseg:340F 42 75 69 6C 64 69 6E 67 20 54 68 65 20 57 6F 72+cnst_Creation_Msg db 'Building The Worlds...',0
// MGC dseg:340F 6C 64 73 2E 2E 2E 00                                                                    ; DATA XREF: NEWG_CreateWorld+69o
// MGC dseg:3426 74 6F 77 65 72 00                               UU_cnst_Tower db 'tower',0              ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:342C 63 68 61 6F 73 20 6E 6F 64 65 00                UU_cnst_ChaosNode db 'chaos node',0     ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3437 6E 61 74 75 72 65 20 6E 6F 64 65 00             UU_cnst_NatureNode db 'nature node',0   ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3443 73 6F 72 63 65 72 79 20 6E 6F 64 65 00          UU_cnst_SorceryNode db 'sorcery node',0 ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3450 6D 79 73 74 65 72 69 6F 75 73 20 63 61 76 65 00 UU_cnst_Cave db 'mysterious cave',0     ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3460 64 75 6E 67 65 6F 6E 00                         UU_cnst_Dungeon db 'dungeon',0          ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3468 61 6E 63 69 65 6E 74 20 74 65 6D 70 6C 65 00    UU_cnst_AncientTmpl db 'ancient temple',0
// MGC dseg:3468                                                                                         ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3477 61 62 61 6E 64 6F 6E 65 64 20 6B 65 65 70 00    UU_cnst_Keep db 'abandoned keep',0      ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3486 6D 6F 6E 73 74 65 72 20 6C 61 69 72 00          UU_cnst_Lair db 'monster lair',0        ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3493 61 6E 63 69 65 6E 74 20 72 75 69 6E 73 00       UU_cnst_Ruins db 'ancient ruins',0      ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:34A1 66 61 6C 6C 65 6E 20 74 65 6D 70 6C 65 00       UU_cnst_FallenTmpl db 'fallen temple',0 ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:34AF 54 45 52 52 54 59 50 45 00                      cnst_TERRTYPE_File db 'TERRTYPE',0      ; DATA XREF: NEWG_SetDeserts+26o ...
// MGC dseg:34B8 43 49 54 59 4E 41 4D 45 00                      cnst_CITYNAME_File db 'CITYNAME',0      ; DATA XREF: CTY_SetDefaultName+23o

// MGC dseg:34C1 00 00 00                                        align 4

// MGC dseg:34C1                                                 END:  ovr050 - Initialized Data



// MGC dseg:8A42                                                 BEGIN:  ovr050 - Uninitialized Data

// MGC dseg:8A42 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Spells_Nature Default_Spells <0>    ; DATA XREF: GAME_New_Screen_5+171w ...
// MGC dseg:8A5C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Spells_Sorcery Default_Spells <0>   ; DATA XREF: GAME_New_Screen_5+17Bw ...
// MGC dseg:8A76 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Spells_Chaos Default_Spells <0>     ; DATA XREF: GAME_New_Screen_5+167w ...
// MGC dseg:8A90 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Spells_Life Default_Spells <0>      ; DATA XREF: GAME_New_Screen_5+185w ...
// MGC dseg:8AAA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Spells_Death Default_Spells <0>     ; DATA XREF: GAME_New_Screen_5+18Fw ...
// MGC dseg:8AC4 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Spells_P1 Default_Spells 5 dup(<0>)
// MGC dseg:8B46 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Spells_P2 Default_Spells 5 dup(<0>)
// MGC dseg:8BC8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Spells_P3 Default_Spells 5 dup(<0>)
// MGC dseg:8C4A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Spells_P4 Default_Spells 5 dup(<0>)
// MGC dseg:8CCC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+UU_TBL_Spells_P5 Default_Spells 5 dup(<0>)
// MGC dseg:8D4E 00 00                                           NEWG_PickAttempt dw 0                   ; DATA XREF: GAME_New_Screen_4+436r ...
// MGC dseg:8D50 00 00                                           NEWG_PickError dw 0                     ; DATA XREF: GAME_New_Screen_4+15Bw ...

// MGC dseg:8D52
// ; initialized to random values of 0, 1, or 2
int16_t TBL_Bookshelf_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8D68
int16_t TBL_Realm0_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8D7E
int16_t TBL_Realm1_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8D94
int16_t TBL_Realm2_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8DAA
int16_t TBL_Realm3_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8DC0
int16_t TBL_Realm4_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// MGC dseg:8DD6 00 00                                           IMG_NEWG_MapBuildBG@ dw 0               ; DATA XREF: GAME_New_Screen_7+83w ...
// MGC dseg:8DD8 00 00                                           NEWG_SliderPos_Sorc dw 0                ; DATA XREF: GAME_New_Screen_4+155w ...
// MGC dseg:8DDA 00 00                                           NEWG_SliderPos_Nat dw 0                 ; DATA XREF: GAME_New_Screen_4+14Fw ...
// MGC dseg:8DDC 00 00                                           NEWG_SliderPos_Chaos dw 0               ; DATA XREF: GAME_New_Screen_4+149w ...
// MGC dseg:8DDE 00 00                                           NEWG_SliderPos_Death dw 0               ; DATA XREF: GAME_New_Screen_4+143w ...
// MGC dseg:8DE0 00 00                                           NEWG_SliderPos_Life dw 0                ; DATA XREF: GAME_New_Screen_4+13Dw ...
// MGC dseg:8DE2 00 00                                           NEWG_PortraitSelType dw 0               ; DATA XREF: GAME_New_Screen_1+C7w ...
// MGC dseg:8DE4 00                                              db    0
// MGC dseg:8DE5 00                                              db    0
// MGC dseg:8DE6 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_NewG_Books@ dw 0Fh dup(0)           ; DATA XREF: GAME_New_Screen_1+99w ...
// MGC dseg:8DE6 00 00 00 00 00 00 00 00 00 00 00 00 00 00                                               ; 5 groups of 3 images each (L - S - N - D - C)

// MGC dseg:8E04
int16_t newgame_landsize_button_field = 0;
// MGC dseg:8E06
int16_t newgame_magic_button_field = 0;
// MGC dseg:8E08
int16_t newgame_opponents_button_field = 0;
// MGC dseg:8E0A
int16_t newgame_difficulty_button_field = 0;

// MGC dseg:8E0C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_Select_Labels dw 1Eh dup(0)        ; DATA XREF: GAME_New_Screen_1+109w ...
// MGC dseg:8E48 00 00                                           NEWG_Moused_Wizard dw 0                 ; DATA XREF: GAME_New_Screen_1+C1w ...

// MGC dseg:8E4A
SAMB_ptr IMG_NewG_ButtonBGs[15];

// MGC dseg:8E68
SAMB_ptr IMG_NewG_RgtOverlay;

// MGC dseg:8E6A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_Retort_Labels dw 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
// MGC dseg:8E6A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: GAME_New_Screen_4+2BEw ...
// MGC dseg:8E8E 00 00                                           _quit_active_seg dw 0                   ; DATA XREF: Load_Screen+32w ...
// MGC dseg:8E90 00 00                                           _load_active_seg dw 0                   ; DATA XREF: Load_Screen+49w ...
// MGC dseg:8E92 00 00                                           _ok_button dw 0                         ; DATA XREF: Load_Screen+1BDw ...
// MGC dseg:8E94 00 00                                           NEWG_Picks_Remaining dw 0               ; DATA XREF: GAME_New_Screen_4+10Ew ...
// MGC dseg:8E96 00 00                                           _selection_marker_seg dw 0              ; DATA XREF: Load_Screen+D3w ...
// MGC dseg:8E98 00 00                                           selected_load_game_slot_idx dw 0        ; DATA XREF: Load_Screen+2D8w ...
// MGC dseg:8E9A 00 00                                           MAY__selected_save_game_slot_idx dw 0
// MGC dseg:8E9C 00 00                                           NEWG_SpellSel_Realm dw 0                ; DATA XREF: GAME_New_Screen_5+1D3w ...
// MGC dseg:8E9E 00 00                                           _settings_button dw 0                   ; DATA XREF: Load_Screen+2D5w ...
// MGC dseg:8EA0 00 00                                           _settings_button_seg dw 0               ; DATA XREF: Load_Screen+BCw ...
// MGC dseg:8EA2 00 00                                           _text_fill_seg dw 0                     ; DATA XREF: Load_Screen+A5w ...
// MGC dseg:8EA4 00 00                                           NEWG_Clicked_Race dw 0                  ; DATA XREF: GAME_New_Screen_6+1EDw ...
// MGC dseg:8EA6 00 00                                           dw 0
// MGC dseg:8EA8 00 00                                           dw 0
// MGC dseg:8EAA 00 00                                           NEWG_ProfileComplete dw 0               ; DATA XREF: GAME_New_Screen_4+114w ...
// MGC dseg:8EAC 00 00                                           _ok_inactive_seg dw 0                   ; DATA XREF: Load_Screen+8Ew ...
// MGC dseg:8EAE 00 00                                           _save_inactive_seg dw 0                 ; DATA XREF: Load_Screen+77w ...

// MGC dseg:8EAE                                                 END:  ovr050 - Uninitialized Data



// MGC dseg:8EB0                                                 BEGIN:  ovr051 - Uninitialized Data

// MGC dseg:8EB0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_Landmass_Races dw 200 dup(0)       ; DATA XREF: NEWG_CreateNeutrals+39w ...
// MGC dseg:9040 00 00                                           NEWG_LandmassCount dw 0                 ; DATA XREF: NEWG_ClearLandmasses+4w ...
// MGC dseg:9042 00 00                                           dw 0
// MGC dseg:9044 00 00                                           dw 0
// MGC dseg:9046 00 00                                           dw 0
// MGC dseg:9048 00 00                                           dw 0
// MGC dseg:904A 00 00                                           dw 0

// MGC dseg:904A                                                 END:  ovr051 - Uninitialized Data





/*
    MAGIC.EXE
    ovr050
*/

// o50p01
// drake178: GAME_New_Create()
/*
; displays and processes the series of screens that
; control the creation of a new game, from generic
; options to wizard customization
; returns 0 if the Esc key is pressed from the first
; screen, but if successful, this function does not
; return, instead saving the created game into the
; continue slot (index 8), and launching WIZARDS.EXE
*/
/*

*/
void Newgame_Control(void)
{
    int16_t Create_State = 0;
    int16_t Can_Create = 0;
    int16_t custom_game_flag = 0;  // _DI_
    int16_t newgame_state = 0;  // _SI_

    _landsize = 1;
    _magic = 1;
    _num_players = 1;  // ; NewGame: magic.opponents + 1
    _difficulty = 1;

    newgame_state = 0;

    Can_Create = ST_FALSE;

    while(Can_Create == ST_FALSE)
    {

        Deactivate_Help_List();
        
        Create_State = newgame_state;

        switch(newgame_state)
        {
            case -1:
            {
                return 0;  // ¿ cancel ?
            } break;
            case 0:
            {
                Newgame_Screen0();
            } break;
            case 1:
            {
                Newgame_Screen1__WIP();
            } break;
            case 2:
            {
                Newgame_Screen2__WIP();
            } break;
            case 3:
            {
                newgame_state = Newgame_Screen3__WIP();
                if(custom_game_flag != ST_FALSE)
                {
                    if(newgame_state == ST_UNDEFINED)
                    {
                        newgame_state = 2;
                    }
                    else
                    {
                        newgame_state = 4;
                    }
                }
                else
                {
                    if(newgame_state == ST_UNDEFINED)
                    {
                        newgame_state = 1;
                    }
                    else
                    {
                        newgame_state = 4;
                    }
                }
            } break;
            case 4:
            {
                Newgame_Screen4__WIP();
            } break;
            case 5:
            {
                Newgame_Screen5__WIP();
            } break;
            case 6:
            {
                newgame_state = Newgame_Screen6__WIP();
                if(newgame_state = ST_UNDEFINED)
                {
                    if(custom_game_flag != ST_FALSE)
                    {
                        newgame_state = 5;
                    }
                    else
                    {
                        newgame_state = 3;
                    }
                }
            } break;
            case 7:
            {
                Newgame_Screen7__WIP();
            } break;
            case 99:
            {

            } break;

        }
        
    }

    NEWG_CreateWorld();

    Initialize_Events();

    NEWG_FinalizeTables__WIP();

    Save_SAVE_GAM(8);

    GAME_WizardsLaunch(8);

}


// o50p02
// drake178: GAME_WizardsLaunch()
/*
; fades out the screen, if the passed save index is not
; that of the continue save (index 8), loads that game
; and saves it as the current continue, then launches
; WIZARDS.EXE (which always loads that save on startup)
*/
/*

*/
void GAME_WizardsLaunch(int16_t save_gam_idx)
{
    Fade_Out();
    Stop_Music__STUB();
    Audio_Uninit__STUB();
    if(save_gam_idx != 8)
    {
        Load_SAVE_GAM(save_gam_idx);
        Save_SAVE_GAM(8);
    }
    GAME_EXE_Swap(cnst_EXESwap_File, cnst_EXESwap_Arg, cnst_EXESwap_Arg, empty_string__ovr050);
}


// o50p03
// Load_Screen()

// o50p04
// Load_Screen_Draw()

// o50p05
/*
PATCHED - rewritten completely in the last profile
loader/worldgen customizer/patch enabler
displays and processes the new game creation screen
for selecting the basic campaign options (difficulty,
# of opponents, land size, and magic)
returns 1 and saves the selection to both disk and
memory if the OK button is clicked, or returns -1
without saving if the Esc key is pressed
*/
/*

*/
int16_t Newgame_Screen0(void)
{
    char file_found[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t leave_screen = 0;
    int16_t First_Draw_Done = 0;
    int16_t input_field_idx = 0;  // _SI_
    FILE * file_pointer = 0;  // _DI_
    
    screen_changed = ST_TRUE;
    
    // ; conflicting condition - will never jump
    if(screen_changed == ST_FALSE)
    {
        Fade_Out();
    }

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);

    Set_Page_On();

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);

    Set_Page_Off();

    Load_Palette(0, -1, ST_NULL);

    // _background_seg = LBX_Reload_Next(newgame_lbx_file, 0, _screen_seg);
    newgame_background_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 0, _screen_seg);

    IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 1, _screen_seg);

    newgame_ok_button_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 2, _screen_seg);

    _quit_active_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 3, _screen_seg);

    IMG_NewG_ButtonBGs[0] = LBX_Reload_Next(newgame_lbx_file__ovr050, 4, _screen_seg);

    IMG_NewG_ButtonBGs[1] = LBX_Reload_Next(newgame_lbx_file__ovr050, 5, _screen_seg);

    IMG_NewG_ButtonBGs[2] = LBX_Reload_Next(newgame_lbx_file__ovr050, 6, _screen_seg);

    IMG_NewG_ButtonBGs[3] = LBX_Reload_Next(newgame_lbx_file__ovr050, 7, _screen_seg);

    if(
        (DIR(str_MAGIC_SET__ovr050, file_found) == 0)
        ||
        (LOF(str_MAGIC_SET__ovr050) == 0)
    )
    {
        Set_Default_Game_Settings();
    }
    else
    {

        file_pointer = fopen(str_MAGIC_SET__ovr050, str_rb__ovr050);

        fread(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_pointer);

        fclose(file_pointer);
        
    }

    if(
        (magic_set.Difficulty < 0)
        ||
        (magic_set.Difficulty > 4)
    )
    {
        magic_set.Difficulty = 0;
    }

    if(
        (magic_set.Opponents < 0)
        ||
        (magic_set.Opponents > 4)
    )
    {
        magic_set.Opponents = 1;
    }

    if(
        (magic_set.LandSize < 0)
        ||
        (magic_set.LandSize > 2)
    )
    {
        magic_set.LandSize = 0;
    }

    if(
        (magic_set.MagicPower < 0)
        ||
        (magic_set.MagicPower > 2)
    )
    {
        magic_set.MagicPower = 0;
    }

    Set_Mouse_List(1, mouse_list_newgame0);

    Clear_Fields();

    Set_Page_On();

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);

    Set_Page_Off();

    _ok_button = Add_Button_Field(252, 179, &empty_string__ovr050[0], newgame_ok_button_seg, &empty_string__ovr050[0], ST_UNDEFINED);

    _quit_button = Add_Button_Field(171, 179, &empty_string__ovr050[0], _quit_active_seg, &str_ESC__ovr050[0], ST_UNDEFINED);

    newgame_difficulty_button_field = Add_Hidden_Field(250, 38, 315, 54, &empty_string__ovr050[0], ST_UNDEFINED);

    newgame_opponents_button_field = Add_Hidden_Field(250, 65, 315, 81, &empty_string__ovr050[0], ST_UNDEFINED);

    newgame_landsize_button_field = Add_Hidden_Field(250, 92, 315, 108, &empty_string__ovr050[0], ST_UNDEFINED);

    newgame_magic_button_field = Add_Hidden_Field(250, 119, 315, 135, &empty_string__ovr050[0], ST_UNDEFINED);

    leave_screen = ST_FALSE;

    First_Draw_Done = ST_FALSE;

    Assign_Auto_Function(Newgame_Screen0_Draw, 1);

    
    Set_Newgame_Screen0_Help_List();
    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        Mark_Time();

        if(input_field_idx == _quit_button)
        {
            Deactivate_Auto_Function();
            Deactivate_Help_List();
            return ST_UNDEFINED;
        }

        if(input_field_idx == _ok_button)
        {

            file_pointer = fopen(str_MAGIC_SET__ovr050, str_wb__ovr050);
            
            fwrite(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_pointer);
            
            fclose(file_pointer);

            Deactivate_Auto_Function();

            Deactivate_Help_List();

            Randomize_Book_Heights();

            _landsize = magic_set.LandSize;
            
            _magic = magic_set.MagicPower;
            
            _num_players = (magic_set.Opponents + 1);
            
            _difficulty = magic_set.Difficulty;
            
            return 1;            
        }
        
        if(input_field_idx == newgame_difficulty_button_field)
        {

            magic_set.Difficulty++;

            if(magic_set.Difficulty > 4)
            {
                magic_set.Difficulty = 1;
            }

        }

        if(input_field_idx == newgame_opponents_button_field)
        {

            magic_set.Opponents++;

            if(magic_set.Opponents > 4)
            {
                magic_set.Opponents = 1;
            }

        }

        if(input_field_idx == newgame_landsize_button_field)
        {

            magic_set.LandSize++;

            if(magic_set.LandSize > 2)
            {
                magic_set.LandSize = 0;
            }

        }

        if(input_field_idx == newgame_magic_button_field)
        {

            magic_set.MagicPower++;

            if(magic_set.MagicPower > 2)
            {
                magic_set.MagicPower = 0;
            }

        }

        if(leave_screen == ST_FALSE)
        {

            Newgame_Screen0_Draw();

            Toggle_Pages();

            // ; conflicting condition - will never jump
            if(screen_changed == ST_FALSE)
            {

                Copy_On_To_Off_Page();

                Fade_In();
                
                First_Draw_Done = ST_TRUE;
                
            }

            Release_Time(2);

        }

    }

}

// o50p06
/*
PATCHED - rewritten completely in the last profile
loader/worldgen customizer/patch enabler

draws the first screen of the new game creation
options into the current draw segment
*/
/*

*/
void Newgame_Screen0_Draw(void)
{
    struct s_OPPONENT_COUNT_NAMES l_opponent_count_names[4] = { 0, 0, 0, 0 };
    struct s_MAGIC_STRENGTH_NAMES l_magic_strength_names[3] = { 0, 0, 0 };
    struct s_LAND_SIZE_NAMES l_land_size_names[3] = { 0, 0, 0 };
    struct s_DIFFICULTY_NAMES l_difficulty_names[5] = { 0, 0, 0, 0, 0 };
    // uint8_t colors2[4] = { 0, 0, 0, 0 };
    // uint8_t colors1[4] = { 0, 0, 0, 0 };
    uint8_t * colors2;
    uint8_t * colors1;

    colors1 = RP_COL_NEWG_4Shadow;
    
    colors2 = RP_COL_NEWG_4Font;
    
    memcpy(l_difficulty_names, _difficulty_names, sizeof(_difficulty_names));
    
    memcpy(l_land_size_names, _land_size_names, sizeof(_land_size_names));
    
    memcpy(l_magic_strength_names, _magic_strength_names, sizeof(_magic_strength_names));

    memcpy(l_opponent_count_names, _opponent_count_names, sizeof(_opponent_count_names));

    // FLIC_Draw(0, 0, _background_seg);
    FLIC_Draw(0, 0, newgame_background_seg);

    Fill(250, 38, 315, 54, ST_BLACK);
    
    Fill(250, 65, 315, 82, ST_BLACK);

    Fill(250, 92, 315, 107, ST_BLACK);
    
    Fill(250, 119, 315, 134, ST_BLACK);

    Fill(251, 178, 315, 193, ST_BLACK);

    Fill(170, 178, 234, 193, ST_BLACK);

    FLIC_Draw(165, 0, IMG_NewG_RgtOverlay);

    auto_input_field_idx = Auto_Input();

    Set_Font_Style(3, 15, ST_NULL, ST_NULL);

    if(auto_input_field_idx != newgame_difficulty_button_field)
    {
        FLIC_Draw(251, 39, IMG_NewG_ButtonBGs[0]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 43, l_difficulty_names[magic_set.Difficulty].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 42, l_difficulty_names[magic_set.Difficulty].name);
    }
    else
    {
        FLIC_Draw(252, 40, IMG_NewG_ButtonBGs[0]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 44, l_difficulty_names[magic_set.Difficulty].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 43, l_difficulty_names[magic_set.Difficulty].name);
    }

    if(auto_input_field_idx != newgame_opponents_button_field)
    {
        FLIC_Draw(251, 66, IMG_NewG_ButtonBGs[1]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 71, l_opponent_count_names[magic_set.Opponents].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 70, l_opponent_count_names[magic_set.Opponents].name);
    }
    else
    {
        FLIC_Draw(252, 67, IMG_NewG_ButtonBGs[1]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 72, l_opponent_count_names[magic_set.Opponents].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 71, l_opponent_count_names[magic_set.Opponents].name);
    }

    if(auto_input_field_idx != newgame_landsize_button_field)
    {
        FLIC_Draw(251, 93, IMG_NewG_ButtonBGs[2]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 97, l_land_size_names[magic_set.LandSize].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 96, l_land_size_names[magic_set.LandSize].name);
    }
    else
    {
        FLIC_Draw(252, 94, IMG_NewG_ButtonBGs[2]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 98, l_land_size_names[magic_set.LandSize].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 97, l_land_size_names[magic_set.LandSize].name);
    }

    if(auto_input_field_idx != newgame_magic_button_field)
    {
        FLIC_Draw(251, 120, IMG_NewG_ButtonBGs[3]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 124, l_magic_strength_names[magic_set.MagicPower].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 123,     l_magic_strength_names[magic_set.MagicPower].name);
    }
    else
    {
        FLIC_Draw(252, 121, IMG_NewG_ButtonBGs[3]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 125, l_magic_strength_names[magic_set.MagicPower].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 124, l_magic_strength_names[magic_set.MagicPower].name);
    }

}

// o50p07
// drake178: GAME_RandBookBinders()
/*
; randomizes the spellbook binder image arrays used
; for displaying bookshelves
*/
/*
Pretty sure, ...
    randomizes the bookshelf book heights
    for the bookshelf under the wizard portrait
    and while selecting books for a custom wizard
    only has 4 because you can't have Life *and* Death

¿ UU_Settings_Product was to be a random seed ?

*/
void Randomize_Book_Heights(void)
{
    int32_t itr = 0;
    int32_t UU_Settings_Product = 0;

    UU_Settings_Product = ((((magic_set.MagicPower * magic_set.Opponents) * (magic_set.Difficulty * magic_set.LandSize)) * 123) + 1234);

    for(itr = 0; itr < 11; itr++)
    {
        TBL_Bookshelf_Books[itr] = (Random(3) - 1);
        TBL_Realm4_Books[itr]    = (Random(3) - 1);
        TBL_Realm3_Books[itr]    = (Random(3) - 1);
        TBL_Realm2_Books[itr]    = (Random(3) - 1);
        TBL_Realm0_Books[itr]    = (Random(3) - 1);
        TBL_Realm1_Books[itr]    = (Random(3) - 1);
        TBL_Bookshelf_Books[itr] = (Random(3) - 1);
    }

}

// o50p08
// drake178: GAME_New_Screen_1()
/*

*/
void Newgame_Screen1__WIP(void)
{



}

// o50p09
// Draw_NewGame_Screen1()

// o50p10
/*

*/
void Newgame_Screen2__WIP(void)
{



}

// o50p11
/*

*/
int16_t Newgame_Screen3__WIP(void)
{



}


// o50p12
// GAME_Draw_NewScr3()

// o50p13
/*

*/
void Newgame_Screen7__WIP(void)
{



}


// o50p14
// GAME_Draw_NewScr7()

// o50p15
/*

*/
int16_t Newgame_Screen6__WIP(void)
{



}


// o50p16
// GAME_Draw_NewScr6()

// o50p17
// GAME_DrawRetortsStr()

// o50p18
// NEWG_DrawDefShelf()

// o50p19
/*

*/
void Newgame_Screen4__WIP(void)
{



}


// o50p20
// GAME_Draw_NewScr4()

// o50p21
/*

*/
void Newgame_Screen5__WIP(void)
{



}


// o50p22
// GAME_SpellSel_GUI()

// o50p23
// SCRN_Draw_NewScr5()

// o50p24
// SCRN_Draw_NewScr5_2()

// o50p25
// WIZ_CopyDefault()

// o50p26
// Fade_Out()

// o50p27
// VGA_Fade_In()

// o50p28
// Set_Load_Screen_Help_List_MGC()

// o50p29
// drake178: HLP_Load_NewGOptions()
/*
loads and sets the GUI help entry area array for the
new game options selection screen
*/
/*

¿ Set_Options_Screen_Help_List ?

*/
void Set_Newgame_Screen0_Help_List(void)
{

    // HLPENTRY.LBX, 29  Options Screen Help
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 29, (SAMB_ptr)_help_entries, 6);

    Set_Help_List(_help_entries, 6);

}


// o50p30
// HLP_Load_BannerSel()

// o50p31
// HLP_Load_PortraitSel()

// o50p32
// HLP_Load_WizardSel()

// o50p33
// HLP_Load_WizCreate()

// o50p34
// HLP_Load_RaceSel()

// o50p35
// STR_ListSeparator()

// o50p36
// CRP_Empty_Dialog_Fn2()

// o50p37
// CRP_Empty_Dialog_Fn1()

// o50p38
// Do_Toggle_Pages()




/*
    MAGIC.EXE
    ovr051
*/

// o51p01
// drake178: NEWG_CreateWorld()
/*
*/
/*

*/
void NEWG_CreateWorld(void)
{



}

// o51p02
// NEWG_EZ_MarkHadnLeft()

// o51p03
// NEWG_TileIsleExtend()

// o51p04
// NEWG_CreateTowers()

// o51p05
// NEWG_CreateCapitals()

// o51p06
// NEWG_ClearLandmasses()

// o51p07
// NEWG_EqualizeNodes()

// o51p08
// NEWG_SetSpecLands()

// o51p09
// NEWG_SetBaseLands()

// o51p10
// NEWG_CreateLands()

// o51p11
// NEWG_CreateNodes()

// o51p12
// NEWG_CreateNodeAura()

// o51p13
// NODE_IsAuraUnique()

// o51p14
// NEWG_SetNodeType()

// o51p15
// TILE_SetLandMass()

// o51p16
// NEWG_CreateEZs()

// o51p17
// NEWG_CreateEncounter()

// o51p18
// UU_UNIT_RandomRacial()

// o51p19
// NEWG_SetDeserts()

// o51p20
// NEWG_CreateShores()

// o51p21
// NEWG_RandomizeTiles()

// o51p22
// NEWG_CreateRiver()

// o51p23
// NEWG_SetRiverTiles()

// o51p24
// NEWG_CreateNeutrals()

// o51p25
// CTY_SetDefaultName()

// o51p26
// NEWG_CreateRoads()

// o51p27
// CRP_NEWG_CreatePathGrids()

// o51p28
// NEWG_CreateSpecials()

// o51p29
// NEWG_DesertSpecials()

// o51p30
// NEWG_HillSpecials()

// o51p31
// NEWG_MntnSpecials()

// o51p32
// NEWG_SetScoutingMaps()

// o51p33
// TILE_SetScouting()

// o51p34
// NEWG_SetMoveMaps()

// o51p35
// TILE_InvalidOutflow()

// o51p36
// drake178: NEWG_UpdateProgress()
// ~ MoO2  Module: MISC  Draw_Advancing_Fill_Message_()
/*
PATCHED in the worldgen customizer to use a created
background image and always redraw, allowing the
progress bar to be reset if necessary
updates the progress bar to the specified percentage
before doing a simple page flip
WARNING: can't go backwards with this setup
*/
/*

*/
void Draw_Building_The_Worlds(int16_t percent)
{
    int16_t width = 0;
    Set_Page_Off();
    if(percent = 0)
    {
        return;
    }
    if(percent >= 100)
    {
        Fill(90, 89, 225, 94, 172);
    }
    else
    {
        width = ((percent * 136) / 100);
        Fill(90, 89, (90 + width), 94, 172);
    }
    Toggle_Pages();
}

// o51p37
// UNIT_Create()

// o51p38
// NEWG_AnimateOceans()

// o51p39
// TILE_IsOcean()

// o51p40
// TILE_HasNode()

// o51p41
// TILE_HasTower()

// o51p42
// TILE_IsForest()

// o51p43
// TILE_IsRiver()

// o51p44
// UU_TILE_IsRiverOutlet()

// o51p45
// UU_TILE_IsShore()

// o51p46
// TILE_IsMountains()

// o51p47
// TILE_IsHills()

// o51p48
// TILE_IsSwamp()

// o51p49
// TILE_IsDesert()

// o51p50
// TILE_IsGrasslands()

// o51p51
// TILE_GetFood()

// o51p52
// TILE_GetMaxPop()

// o51p53
// TILE_GetCatchment()

// o51p54
// UU_Empty_Tile_Fn()



// MGC o56p1
// drake178: WIZ_SetProfiles()
/*
*/
/*

*/
// WIZ_SetProfiles__WIP()

// MGC o56p2
// drake178: NEWG_FinalizeTables()
/*
finalizes the item, hero, and wizard record tables
RE-CHECK: some fields are not yet known
*/
/*

*/
void NEWG_FinalizeTables__WIP(void)
{
    int16_t itr_players = 0;  // _SI_

    Draw_Building_The_Worlds(100);

    Initialize_Items();

    Init_Heroes();

    WIZ_SetStartingStats__WIP();

    AI_WIZ_StrategyReset__WIP();

    WIZ_SetPersonalities__WIP();

    WIZ_Set_Sum_n_SoM__WIP();

    Initialize_Messages();

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].capital_race = _CITIES[itr_players].race;
    }

    AI_WIZ_SetRelations__WIP();

    _players[HUMAN_PLAYER_IDX].gold_reserve = ((5 - _difficulty) * 25);

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {

        _players[itr_players].gold_reserve = 150;


// ; 80 + rnd(40) is stored here for each AI wizard (word
// ; array pointer, human excluded) when setting initial
// ; gold
// mov     ax, 40
// push    ax
// call    Random
// add     ax, 80
// mov     dx, _SI_itr_players
// shl     dx, 1
// mov     bx, [Wiz5_Spell_E0h@]             
// add     bx, dx
// mov     [bx], ax
        // Wiz5_Spell_E0h[itr_players] = (80 + Random(40));
        // mov     [AI_SCircle_Reevals@],  (offset _players.spells_list+18C8h) ; 16 bytes
        AI_SCircle_Reevals = ( (int16_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18C8) );  // 16 bytes        
    }

}


// MGC o56p3
// drake178: AI_WIZ_SetRelations()
// AI_WIZ_SetRelations()

// MGC o56p4
// drake178: NEWG_ClearItems()
/*
clears the item table by setting the value of all
records to 0, and clears the predefined item
appearance table
*/
/*

*/
void Initialize_Items(void)
{
    int16_t itr = 0;  // _CX_
    for(itr = 0; itr < 138; itr++)
    {
        _ITEMS[itr].cost = 0;
    }
    for(itr = 0; itr < 250; itr++)
    {
        TBL_Premade_Items[itr] = 0;
    }
}


// MGC o56p5
// drake178: WIZ_Set_Sum_n_SoM()
// WIZ_Set_Sum_n_SoM()

// MGC o56p6
// drake178: WIZ_SetPersonalities()
// WIZ_SetPersonalities()

// MGC o56p7
// drake178: AI_WIZ_StrategyReset()
// AI_WIZ_StrategyReset()

// MGC o56p8
// drake178: WIZ_SetStartingStats()
// WIZ_SetStartingStats()

// MGC o56p9
// drake178: AI_CreateWizards()
// AI_CreateWizards()

// MGC o56p10
// drake178: WIZ_SetSpells()
// WIZ_SetSpells()


// MGC o56p11
// drake178: NEWG_ClearEvents()
// ¿ ~== MoO2  Module: REPORT  Initialize_Reports_() ?
/*
; clears the event data structure by setting the status
; of all events to 0
*/
/*

*/
void Initialize_Events(void)
{
    events_table->last_event_turn = 50;
    events_table->Meteor_Status = 0;
    events_table->Gift_Status = 0;
    events_table->Disjunction_Status = 0;
    events_table->marriage_status = 0;
    events_table->Earthquake_Status = 0;
    events_table->Pirates_Status = 0;
    events_table->Plague_Status = 0;
    events_table->Rebellion_Status = 0;
    events_table->Donation_Status = 0;
    events_table->Depletion_Status = 0;
    events_table->minerals_status = 0;
    events_table->Population_Boom_Status = 0;
    events_table->Good_Moon_Status = 0;
    events_table->Bad_Moon_Status = 0;
    events_table->Conjunction_Chaos_Status = 0;
    events_table->Conjunction_Nature_Status = 0;
    events_table->Conjunction_Sorcery_Status = 0;
    events_table->Mana_Short_Status = 0;
}

// MGC o56p12
// drake178: NEWG_CreateHeroes()
// MoO2  Module: INITGAME  Init_Leaders_()
void Init_Heroes(void)
{
    int32_t abilities = 0;
    int16_t all_picks = 0;
    int16_t mage_picks = 0;
    int16_t casting_skill = 0;
    int16_t itr_hero_types = 0;
    int16_t itr_players = 0;
    int16_t warrior_picks = 0;  // _DI_

    for(itr_players = 0; itr_players < 5; itr_players++)
    {

        for(itr_hero_types = 0; itr_hero_types < NUM_HERO_TYPES; itr_hero_types++)
        {

            _HEROES2[itr_players]->heroes[itr_hero_types].Level = 0;

            warrior_picks = 0;

            mage_picks = 0;

            all_picks = 0;

            switch(_hero_premade_table[itr_hero_types].pick_type)
            {
                case 0:  /* Warrior */
                {
                    warrior_picks = _hero_premade_table[itr_hero_types].pick_count;
                } break;
                case 1:  /* Mage */
                {
                    mage_picks = _hero_premade_table[itr_hero_types].pick_count;
                } break;
                case 2:  /* All/Any/Both/Either/Neither */
                {
                    all_picks = _hero_premade_table[itr_hero_types].pick_count;
                } break;
            }

            abilities = _hero_premade_table[itr_hero_types].abilities;

            casting_skill = _hero_premade_table[itr_hero_types].casting_skill;

            if(all_picks > 0)
            {
                warrior_picks = all_picks;
                mage_picks = all_picks;
            }

            while((warrior_picks > 0) || (mage_picks > 0))
            {

                switch(Random(14) - 1)
                {
                    case 0:  /* Leadership */
                    {
                        if(((abilities & HSA_LEADERSHIP2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_LEADERSHIP) != 0) { abilities ^= HSA_LEADERSHIP; abilities |= HSA_LEADERSHIP2; } else { abilities |= HSA_LEADERSHIP; } warrior_picks--; mage_picks--; }
                    } break;
                    case 1:  /* Legendary */
                    {
                        if(((abilities & HSA_LEGENDARY2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_LEGENDARY) != 0) { abilities ^= HSA_LEGENDARY; abilities |= HSA_LEGENDARY2; } else { abilities |= HSA_LEGENDARY; } warrior_picks--; mage_picks--; }
                    } break;
                    case 2:  /* Armsmaster */
                    {
                        if(((abilities & HSA_ARMSMASTER2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_ARMSMASTER) != 0) { abilities ^= HSA_ARMSMASTER; abilities |= HSA_ARMSMASTER2; } else { abilities |= HSA_ARMSMASTER; } warrior_picks--; mage_picks--; }
                    } break;
                    case 3:  /* Blademaster */
                    {
                        if(((abilities & HSA_BLADEMASTER2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_BLADEMASTER) != 0) { abilities ^= HSA_BLADEMASTER; abilities |= HSA_BLADEMASTER2; } else { abilities |= HSA_BLADEMASTER; } warrior_picks--; mage_picks--; }
                    } break;
                    case 4:  /* Might */
                    {
                        if(((abilities & HSA_MIGHT2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_MIGHT) != 0) { abilities ^= HSA_MIGHT; abilities |= HSA_MIGHT2; } else { abilities |= HSA_MIGHT; } warrior_picks--; mage_picks--; }
                    } break;
                    case 5:  /* Constitution */
                    {
                        if(((abilities & HSA_CONSTITUTION2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_CONSTITUTION) != 0) { abilities ^= HSA_CONSTITUTION; abilities |= HSA_CONSTITUTION2; } else { abilities |= HSA_CONSTITUTION; } warrior_picks--; mage_picks--; }
                    } break;
                    case 6:  /* Arcane Power */
                    {
                        if(((abilities & HSA_ARCANE_POWER2) == 0) && (mage_picks > 0))
                        {
                            // ; BUG: the Knight has no caster picks, this should be $1E (the Elven Archer)
                            if(itr_hero_types != ut_Knight)  /* ¿ should ut_ElvenArcher  = 30 ? */
                            {
                                // ; BUG: excludes the Warlock and the Chaos Warrior, this should be $1E (Ranged_Lightning)
                                if(
                                    ((abilities & HSA_ARCANE_POWER) != 0)
                                    &&
                                    (_unit_type_table[itr_hero_types].Ranged_Type >= rat_Fireball)  /* ¿ should be rat_Lightning    =  30 ? */
                                    &&
                                    (_unit_type_table[itr_hero_types].Ranged_Type <= rat_Nat_Bolt)
                                )
                                {
                                    abilities ^= HSA_ARCANE_POWER;
                                    abilities |= HSA_ARCANE_POWER2;
                                }
                                else
                                {
                                    abilities |= HSA_ARCANE_POWER;
                                }
                            }
                        }
                    } break;
                    case 7:  /* Prayermaster */
                    {
                        if(((abilities & HSA_PRAYERMASTER2) == 0) && (mage_picks > 0)) { if((abilities & HSA_PRAYERMASTER) != 0) { abilities ^= HSA_PRAYERMASTER; abilities |= HSA_PRAYERMASTER2; } else { abilities |= HSA_PRAYERMASTER; } warrior_picks--; mage_picks--; }
                    } break;
                    case 8:  /* Casting Skill */
                    {
                        if(mage_picks > 0) { casting_skill++; warrior_picks--; mage_picks--; }
                    } break;
                    case 9:  /* Noble */
                    {
                        if(((abilities & HSA_NOBLE) == 0) && (itr_hero_types = ut_Chosen)) { abilities |= HSA_NOBLE; warrior_picks--; mage_picks--; }
                    } break;
                    case 10:  /* Charm */
                    {
                        if(((abilities & HSA_CHARMED) == 0) && ((_unit_type_table[itr_hero_types].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)) { abilities |= HSA_CHARMED; warrior_picks--; mage_picks--; }
                    } break;
                    case 11:  /* Lucky */
                    {
                        if((abilities & HSA_LUCKY) == 0) { abilities |= HSA_LUCKY; warrior_picks--; mage_picks--; }
                    } /* BUGBUG: case 11/Luck missing `break;`, falls through to case 12/Agility */
                    case 12:  /* Agility */
                    {
                        if(((abilities & HSA_AGILITY2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_AGILITY) != 0) { abilities |= HSA_AGILITY2; } else { abilities |= HSA_AGILITY; } warrior_picks--; mage_picks--; }
                    } break;
                    case 13:  /* Sage */
                    {
                        if(((abilities & HSA_SAGE2) == 0) && (mage_picks > 0)) { if((abilities & HSA_SAGE) != 0) { abilities |= HSA_SAGE2; } else { abilities |= HSA_SAGE; } warrior_picks--; mage_picks--; }
                    } break;

                }

            }

            _HEROES2[itr_players]->heroes[itr_hero_types].abilities = abilities;
            _HEROES2[itr_players]->heroes[itr_hero_types].Casting_Skill = casting_skill;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[0] = _hero_premade_table[itr_hero_types].spell_1;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[1] = _hero_premade_table[itr_hero_types].spell_2;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[2] = _hero_premade_table[itr_hero_types].spell_3;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[3] = _hero_premade_table[itr_hero_types].spell_4;

        }

    }

}


// MGC o56p13
// drake178: CRP_MSG_Clear()
/*
zeroes the counters of all chancellor (scroll)
message types, clearing all event messages
*/
/*

*/
void Initialize_Messages(void)
{
    MSG_UnitLost_Count = 0;
    MSG_UnitKilled_Count = 0;
    MSG_UEsLost_Count = 0;
    MSG_CEsLost_Count = 0;
    MSG_GEs_Lost = 0;
    MSG_CityGrowth_Count = 0;
    MSG_CityDeath_Count = 0;
    MSG_BldLost_Count = 0;
    MSG_CityLost_Count = 0;
    MSG_CityGained_Count = 0;
}


// MGC o56p14
// drake178: UU_ITEM_SetHeroSlots()
// UU_ITEM_SetHeroSlots()

// MGC o56p15
// drake178: WIZ_ConsolidateBooks()
// WIZ_ConsolidateBooks()

// MGC o56p16
// drake178: WIZ_GetNominalSkill()
// WIZ_GetNominalSkill()

// MGC o56p17
// drake178: RNG_WeightedPick16()
// RNG_WeightedPick16()
