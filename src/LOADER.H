#ifndef LOADER_H
#define LOADER_H

#include "MOM.H"

#ifdef __cplusplus
extern "C" {
#endif




// WZD dseg:97A4 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 unitview_specials_borders_seg@ dw 8 dup(0)          ; DATA XREF: GFX_Swap_AppendUView+DAw ...
// WZD dseg:97A4                                                                                         ; array of 8 appended reserved EMM headers in
// WZD dseg:97A4                                                                                         ; GFX_Swap_Seg, one for around each list slot
// WZD dseg:97B4 00 00                                           unitview_down_arrow_seg@ dw 0                 ; DATA XREF: GFX_Swap_AppendUView+B9w ...
// WZD dseg:97B4                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97B4                                                                                         ; 2 images (normal - clicked)
// WZD dseg:97B6 00 00                                           unitview_up_arrow_seg@ dw 0                   ; DATA XREF: GFX_Swap_AppendUView+A2w ...
// WZD dseg:97B6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97B6                                                                                         ; 2 images (normal - clicked)
// WZD dseg:97B8 00 00                                           unitview_large_background_seg@ dw 0                ; DATA XREF: GFX_Swap_AppendUView+74w ...
// WZD dseg:97B8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97BA 00 00                                           unitview_small_background_seg@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+5Dw ...
// WZD dseg:97BA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97BC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+unitview_stat_gold_icons_seg STAT_ICONS <0>       ; DATA XREF: GFX_Swap_AppendUView+12Aw ...
// WZD dseg:97BC 00 00 00 00 00 00 00 00 00 00 00 00 00 00                                               ; array of 15 appended reserved EMM header in
// WZD dseg:97BC                                                                                         ; GFX_Swap_Seg, each with one gold attribute image,
// WZD dseg:97BC                                                                                         ; although the movement ones are empty
// WZD dseg:97DA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+unitview_stat_icons_seg STAT_ICONS <0>       ; DATA XREF: GFX_Swap_AppendUView+102w ...
// WZD dseg:97DA 00 00 00 00 00 00 00 00 00 00 00 00 00 00                                               ; array of 15 appended reserved EMM header in
// WZD dseg:97DA                                                                                         ; GFX_Swap_Seg, each with one regular attribute image
// WZD dseg:97F8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_USW_Abilities USWICONS <0>          ; DATA XREF: GFX_Swap_AppendUView+1Dw ...
// WZD dseg:97F8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 145 appended reserved EMM headers in
// WZD dseg:97F8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; GFX_Swap_Seg, each with one image
// WZD dseg:97F8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; UU_Guises@ is also used to hold the diplomacy mirror
// WZD dseg:97F8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; image (BACKGRND.LBX entry 18, 5740 bytes in sandbox)


// WZD dseg:97F8 00                                              special_seg db    0                     ; DATA XREF: GFX_Swap_AppendUView+1Dw ...
// ...
// WZD dseg:98D6 00                                              special2_seg db    0                    ; DATA XREF: GFX_Swap_AppendUView+40w


// 00000000 struc USWICONS ; (sizeof=0x122)
// 00000000 Teleporting@ dw ?
// 00000002 Forester@ dw ?
// 00000004 Mountaineer@ dw ?
// 00000006 Item_BG@ dw ?
// 00000008 Plane_Shift@ dw ?
// 0000000A Poison_Immunity@ dw ?
// 0000000C Fire_Immunity@ dw ?
// 0000000E Stoning_Immunity@ dw ?
// 00000010 Weapon_Immunity@ dw ?
// 00000012 Missile_Immunity@ dw ?
// 00000014 Illusion_Immunity@ dw ?
// 00000016 Cold_Immunity@ dw ?
// 00000018 Magic_Immunity@ dw ?
// 0000001A UU_Shapeshifting@ dw ?
// 0000001C Large_Shield@ dw ?
// 0000001E Wall_Crusher@ dw ?
// 00000020 Healer@ dw ?
// 00000022 Init_Outpost@ dw ?
// 00000024 Invisibility@ dw ?
// 00000026 Create_Undead@ dw ?
// 00000028 Pathfinding@ dw ?
// 0000002A UU_Land_Corruption@ dw ?
// 0000002C Non_Corporeal@ dw ?
// 0000002E Wind_Walking@ dw ?
// 00000030 Regeneration@ dw ?
// 00000032 Purify@ dw ?
// 00000034 Stoning_Gaze@ dw ?
// 00000036 Stoning_Touch@ dw ?
// 00000038 Armor_Piercing@ dw ?
// 0000003A First_Strike@ dw ?
// 0000003C Poison_Touch@ dw ?
// 0000003E Life_Steal@ dw ?
// 00000040 Immolation@ dw ?
// 00000042 Resist_All@ dw ?
// 00000044 Holy_Bonus@ dw ?
// 00000046 Illusion@ dw ?
// 00000048 Construction@ dw ?
// 0000004A Scouting@ dw ?
// 0000004C Healing_Spell@ dw ?
// 0000004E Fireball_Spell@ dw ?
// 00000050 Meld@ dw ?
// 00000052 Doombolt_Spell@ dw ?
// 00000054 U_Empty_42@ dw ?
// 00000056 Leadership@ dw ?
// 00000058 U_Empty_44@ dw ?
// 0000005A Legendary@ dw ?
// 0000005C Armsmaster@ dw ?
// 0000005E Blademaster@ dw ?
// 00000060 Negate_First_Strike@ dw ?
// 00000062 Death_Immunity@ dw ?
// 00000064 Constitution@ dw ?
// 00000066 U_Empty_51@ dw ?
// 00000068 Might@ dw ?
// 0000006A U_Empty_53@ dw ?
// 0000006C Arcane_Power@ dw ?
// 0000006E Caster@ dw ?
// 00000070 UU_InnateSpell@ dw ?
// 00000072 Prayermaster@ dw ?
// 00000074 Lucky_Hero@ dw ?
// 00000076 Charmed@ dw ?
// 00000078 Noble@ dw ?
// 0000007A Sage@ dw ?
// 0000007C Demon_Skin@ dw ?
// 0000007E Demon_Wings@ dw ?
// 00000080 Chaos_Breath@ dw ?
// 00000082 Giant_Strength@ dw ?
// 00000084 UU_Fear@ dw ?
// 00000086 Black_Channels@ dw ?
// 00000088 Wraith_Form@ dw ?
// 0000008A Regeneration_UE@ dw ?
// 0000008C Path_Finding@ dw ?
// 0000008E Water_Walking@ dw ?
// 00000090 Resist_Elements@ dw ?
// 00000092 Elemental_Armor@ dw ?
// 00000094 Stone_Skin@ dw ?
// 00000096 Iron_Skin@ dw ?
// 00000098 Endurance@ dw ?
// 0000009A Haste@ dw ?
// 0000009C Invisibility_UE@ dw ?
// 0000009E Wind_Walking_UE@ dw ?
// 000000A0 Flight@ dw ?
// 000000A2 Resist_Magic@ dw ?
// 000000A4 Magic_Immunity_UE@ dw ?
// 000000A6 Flame_Blade@ dw ?
// 000000A8 Eldritch_Weapon@ dw ?
// 000000AA True_Sight@ dw ?
// 000000AC Holy_Weapon@ dw ?
// 000000AE Heroism@ dw ?
// 000000B0 Bless@ dw ?
// 000000B2 Lionheart@ dw ?
// 000000B4 UU_Aura_Blaze@ dw ?
// 000000B6 Planar_Travel@ dw ?
// 000000B8 Holy_Armor@ dw ?
// 000000BA Righteousness@ dw ?
// 000000BC Invulnerability@ dw ?
// 000000BE Shatter@ dw ?
// 000000C0 Weakness@ dw ?
// 000000C2 Cloak_of_Fear@ dw ?
// 000000C4 Possession@ dw ?
// 000000C6 Web@ dw ?
// 000000C8 Creature_Binding@ dw ?
// 000000CA Vertigo@ dw ?
// 000000CC ExpLevel_1_@ dw ?
// 000000CE ExpLevel_2_@ dw ?
// 000000D0 ExpLevel_3_@ dw ?
// 000000D2 ExpLevel_4_@ dw ?
// 000000D4 ExpLevel_5_@ dw ?
// 000000D6 ExpLevel_6_@ dw ?
// 000000D8 ExpLevel_7_@ dw ?
// 000000DA ExpLevel_8_@ dw ?
// 000000DC ExpLevel_9_@ dw ?
// 000000DE Confusion@ dw ?
// 000000E0 UU_Whirlwind@ dw ?
// 000000E2 Mind_Storm@ dw ?
// 000000E4 Black_Sleep@ dw ?
// 000000E6 Stasis@ dw ?
// 000000E8 Destruction@ dw ?
// 000000EA Immolation_UE@ dw ?
// 000000EC Guardian_Wind@ dw ?
// 000000EE Spell_Lock@ dw ?
// 000000F0 Undead@ dw ?
// 000000F2 UU_Mind_Twist@ dw ?
// 000000F4 Warped_Attack@ dw ?
// 000000F6 Warped_Defense@ dw ?
// 000000F8 Warped_Resist@ dw ?
// 000000FA To_Hit@ dw ?
// 000000FC Negative_To_Hit@ dw ?
// 000000FE Lucky_Unit@ dw ?
// 00000100 Berserk@ dw ?
// 00000102 Long_Range@ dw ?
// 00000104 Thrown@ dw ?
// 00000106 Web_Spell@ dw ?
// 00000108 Cause_Fear@ dw ?
// 0000010A Dispel_Evil@ dw ?
// 0000010C Doom_Gaze@ dw ?
// 0000010E Death_Gaze@ dw ?
// 00000110 Ammunition@ dw ?
// 00000112 Lightning_Breath@ dw ?
// 00000114 Fire_Breath@ dw ?
// 00000116 Summon_Demons@ dw ?
// 00000118 Chaos_Weapon@ dw ?
// 0000011A Death_Touch@ dw ?
// 0000011C UU_Blue_Touch@ dw ?
// 0000011E Agility@ dw ?
// 00000120 UU_Guises@ dw ?                         ; used to load the diplomacy mirror
// 00000122 ends USWICONS


// 00000000 struc STAT_ICONS ; (sizeof=0x1E, standard type)
// 00000000 Melee@ dw ?
// 00000002 Ranged_Magic@ dw ?
// 00000004 Mithril@ dw ?
// 00000006 Magic_Weapon@ dw ?
// 00000008 Adamantium@ dw ?
// 0000000A Ranged_Bow@ dw ?
// 0000000C Ranged_Rock@ dw ?
// 0000000E Fire_Breath@ dw ?
// 00000010 Thrown_Attack@ dw ?
// 00000012 Defense@ dw ?
// 00000014 Hits@ dw ?
// 00000016 Ground_Movement@ dw ?
// 00000018 Flying_Movement@ dw ?
// 0000001A Water_Movement@ dw ?
// 0000001C Resist@ dw ?
// 0000001E ends STAT_ICONS





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
extern char rsc0C_MAPBACK_LBX[];

// WZD dseg:29AB rsc0D_CMBMAGIC_LBX db 'CMBMAGIC.LBX',0
// WZD dseg:29B8 rsc0E_CMBTCITY_LBX db 'CMBTCITY.LBX',0
// WZD dseg:29C5 rsc0F_CITYWALL_LBX db 'CITYWALL.LBX',0

// WZD dseg:29D2                            Init_Terrain()

// WZD dseg:29D2
extern char terrain_lbx_file[];

// dseg:29DE terrstat_lbx_file db 'TERRSTAT',0

// WZD dseg:29E7
extern char mapback_lbx_file[];

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
// WZD dseg:2A81 special2_lbx_file db 'special2',0

// WZD dseg:2A81                            END: ovr052



/*
    MAGIC.EXE   seg001
*/

// MGC s01p04
void Load_MGC_Resources(void);


/*
    WIZARDS.EXE  ovr052
*/

// WZD o52p01
void Load_WZD_Resources(void);

// WZD o52p02
// drake178: UNIT_Upkeep_Reset()
void Units_Upkeeps(void);

// WZD o52p03
void Terrain_Init(void);

// WZD o52p04
// fxn_o52p04()

// WZD o52p05
void Main_Screen_Load_Pictures(void);

// WZD o52p06
void City_Screen_Load_Pictures(void);

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
void Load_Button_Sounds(void);

// WZD o52p14
// fxn_o52p14()

// WZD o52p15
// fxn_o52p15()
void Load_TERRSTAT(void);

// WZD o52p16
void Load_SPELLDAT(void);

// WZD o52p17
// drake178: ?
void Load_BUILDDAT(void);

// WZD o52p18
// fxn_o52p18()

// WZD o52p19
void Load_Combat_Background_Bottom(void);

// WZD o52p20
void GFX_Swap_Reset(void);

// WZD o52p21
// UU_WrldDatBuf_Reset()

// WZD o52p22
// AKA GFX_Swap_AppendUnits()
void Load_Unit_StatFigs(void);

// WZD o52p23
void Reload_Combat_Graphics_Cache(void);

// WZD o52p24
// fxn_o52p24()

// WZD o52p25
void GFX_Swap_AppndCtScap__WIP(void);

// WZD o52p26
// fxn_o52p6()

// WZD o52p27
void GFX_Swap_AppendUView(void);

// WZD o52p28
void Reload_Item_Pictures(void);

// WZD o52p29
void Spellbook_Load_Small_Pictures__WIP(void);

// WZD o52p30
void GFX_Swap_Cities(void);

// WZD o52p31
void GFX_Swap_Overland(void);

// WZD o52p32
void Cache_Graphics_Unit_List_Window(void);

// WZD o52p33
// U_GFX_Swap_Empty()

// WZD o52p34
void Cache_Graphics_Combat(void);



#ifdef __cplusplus
}
#endif

#endif  /* LOADER_H */
