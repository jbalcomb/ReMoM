
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "MoX_Data.H"






// WZD dseg:00AA                                                 BEGIN: Data Segment - Initialized Data


// ¿ MoO2: ~ remap_picture_colors ?
// WZD dseg:00AA 69 6A 6B 6C 6D                                  COL_Banners_P0 db 105, 106, 107, 108, 109
// WZD dseg:00AF 49 4A 4B 4C 4D                                  COL_Banners_P1 db 73, 74, 75, 76, 77
// WZD dseg:00B4 79 7A 7B 7C 7D                                  COL_Banners_P2 db 121, 122, 123, 124, 125
// WZD dseg:00B9 C9 A5 CB A6 2D                                  COL_Banners_P3 db 201, 165, 203, 166, 45
// WZD dseg:00BE A0 A1 A2 B2 B4                                  COL_Banners_P4 db 160, 161, 162, 178, 180
// WZD dseg:00C3 30 31 32 33 34                                  COL_Banners_P5 db 48, 49, 50, 51, 52
uint8_t COL_Banners[] = {
    0x69, 0x6A, 0x6B, 0x6C, 0x6D,
    0x49, 0x4A, 0x4B, 0x4C, 0x4D,
    0x79, 0x7A, 0x7B, 0x7C, 0x7D,
    0xC9, 0xA5, 0xCB, 0xA6, 0x2D,
    0xA0, 0xA1, 0xA2, 0xB2, 0xB4,
    0x30, 0x31, 0x32, 0x33, 0x34
};





// MGC dseg:52C6
// AKA gsa_WIZARDS_0to13
SAMB_ptr wizard_portrait_segs[14];  // ¿ here, because used by MGC Newgame_Screen(), but, also used by WZD Magic_Screen() ?





// WZD dseg:599C
// TODO  char hlpentry_lbx_file[] = "hlpentry";



// WZD dseg:6FFE
// AKA Map_LastDraw_X
int16_t prev_map_x = ST_UNDEFINED;
// WZD dseg:7000
// AKA Map_LastDraw_Y
int16_t prev_map_y = ST_UNDEFINED;
// WZD dseg:7002
// AKA OVL_NewMapSustain
int16_t map_sustain = 2;



// dseg:76DA
int16_t screen_window_x1 = SCREEN_XMIN;
// dseg:76DC
int16_t screen_window_y1 = SCREEN_YMIN;
// dseg:76DE
int16_t screen_window_x2 = SCREEN_XMAX;
// dseg:76E0
int16_t screen_window_y2 = SCREEN_YMAX;





// WZD dseg:7846 68 35 68 35                                     random_seed dd 35683568h                ; DATA XREF: Set_Random_Seed+6w ...

// MoX_Util  // WZD dseg:784A 01 00 02 00 04 00 08 00 10 00 20 00 40 00 80 00 bit_field_test_bits dw 1, 10b, 100b, 1000b, 10000b, 100000b, 1000000b, 10000000b
// MoX_Util  // WZD dseg:784A                                                                                         ; DATA XREF: Test_Bit_Field_+1Cr ...
// MoX_Util  uint16_t bit_field_test_bits[8] = {
// MoX_Util      0x0001,
// MoX_Util      0x0002,
// MoX_Util      0x0004,
// MoX_Util      0x0008,
// MoX_Util      0x0010,
// MoX_Util      0x0020,
// MoX_Util      0x0040,
// MoX_Util      0x0080
// MoX_Util  };

// WZD dseg:785A 64 00                                           UU_DBG_OptionBoxColor dw 64h            ; DATA XREF: UU_DBG_SetSelectSetting+Cw ...
// WZD dseg:785C 32 00                                           UU_DBG_UnknownOValue dw 32h             ; DATA XREF: UU_DBG_SetSelectSetting+12w
// WZD dseg:785E 00 00                                           UU_DBG_OptionsFontColor dw 0            ; DATA XREF: UU_DBG_SetSelectSetting+18w ...
// WZD dseg:7860 00 00                                           UU_DBG_OptionsFont dw 0                 ; DATA XREF: UU_DBG_SetSelectSetting+6w ...
// WZD dseg:7862 52 4E 44 20 6E 6F 20 30 27 73                   cnst_RND_Error db 'RND no 0',27h,'s'    ; DATA XREF: Random+Eo
// WZD dseg:786C 00                                              cnst_ZeroString_2 db 0                  ; DATA XREF: UU_DBG_SelectDialog+138o ...
// WZD dseg:786D 4E 4F 5F 48 45 4C 50 00                         UU_cnst_NoHelp db 'NO_HELP',0           ; DATA XREF: UU_DBG_SelectDialog+134o ...
// WZD dseg:7875 00                                              db    0
// WZD dseg:7876 00 00                                           release_version dw 0                    ; DATA XREF: DBG_Quit:loc_1E469r ...
// WZD dseg:7878 53 43 52 44 4D 50 30 30                         cnst_Scrdmp00_Full db 'SCRDMP00'        ; DATA XREF: DBG_ScreenDump+24o
// WZD dseg:7880 2E 46 4C 49 00                                  cnst_Scrdmp_Ext db '.FLI',0             ; DATA XREF: DBG_ScreenDump+AAo
// WZD dseg:7885 30 00                                           cnst_Scrdmp_0 db '0',0                  ; DATA XREF: DBG_ScreenDump+3Eo
// WZD dseg:7887 53 43 52 44 4D 50 00                            cnst_Scrdmp_Base db 'SCRDMP',0          ; DATA XREF: DBG_ScreenDump:loc_1E504o
// WZD dseg:788E 77 62 00                                        cnst_WB7 db 'wb',0                      ; DATA XREF: DBG_ScreenDump:loc_1E54Do






// WZD dseg:888A 01 01 93 01 43 32                               ExitStart _start_exit_proc <1, 1, __OVREXIT>

// WZD dseg:8890                                                 ¿ BEGIN: ?

// WZD dseg:8890 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Arcanus CONT_TBL <0>                ; DATA XREF: start+C7o ...
// WZD dseg:8890 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; _bdata_start
// WZD dseg:8C14 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Myrror CONT_TBL <0>
// WZD dseg:8F98 00 00                                           AI_SCircle_Reevals@ dw 0                ; DATA XREF: AI_Overland_Turn+136r ...
// WZD dseg:8F98                                                                                         ; 16 bytes
// WZD dseg:8F98                                                                                         ; 80 + rnd(40) is stored here for each AI wizard (word
// WZD dseg:8F98                                                                                         ; array pointer, human excluded) when setting initial
// WZD dseg:8F98                                                                                         ; gold during game creation in MAGIC.EXE
// WZD dseg:8F98                                                                                         ; decreased during the AI's turn, likely some relation
// WZD dseg:8F98                                                                                         ; or interest value
// WZD dseg:8F9A 00 00                                           AI_Arc_NewColTgtYs@ dw 0                ; DATA XREF: MoM_Init_Tables+3F6w ...
// WZD dseg:8F9A                                                                                         ; 12 bytes
// WZD dseg:8F9C 00 00                                           AI_Myr_NewColTgtYs@ dw 0                ; DATA XREF: MoM_Init_Tables+3FCw
// WZD dseg:8F9C                                                                                         ; 12 bytes
// WZD dseg:8F9E 00 00                                           AI_Arc_NewColTgtXs@ dw 0                ; DATA XREF: MoM_Init_Tables+3EAw ...
// WZD dseg:8F9E                                                                                         ; 20 bytes
// WZD dseg:8FA0 00 00                                           AI_Myr_NewColTgtXs@ dw 0                ; DATA XREF: MoM_Init_Tables+3F0w
// WZD dseg:8FA0                                                                                         ; 20 bytes
// WZD dseg:8FA2 00 00                                           Wiz5_Spell_50h@ dw 0                    ; DATA XREF: MoM_Init_Tables+3D2w
// WZD dseg:8FA2                                                                                         ; 20 bytes
// WZD dseg:8FA4 00 00                                           Wiz5_Spell_64h@ dw 0                    ; DATA XREF: MoM_Init_Tables+3D8w
// WZD dseg:8FA4                                                                                         ; 20 bytes
// WZD dseg:8FA6 00 00                                           Wiz5_Spell_28h@ dw 0                    ; DATA XREF: MoM_Init_Tables+3C6w
// WZD dseg:8FA6                                                                                         ; 20 bytes
// WZD dseg:8FA8 00 00                                           Wiz5_Spell_3Ch@ dw 0                    ; DATA XREF: MoM_Init_Tables+3CCw
// WZD dseg:8FA8                                                                                         ; 20 bytes
// WZD dseg:8FAA 00 00                                           AI_CONTX_Reevals@ dw 0                  ; DATA XREF: Loaded_Game_Update_WZD+B4r ...
// WZD dseg:8FAA                                                                                         ; 16 bytes
// WZD dseg:8FAC 00 00                                           AI_Arc_NewColConts@ dw 0                ; DATA XREF: MoM_Init_Tables+3DEw ...
// WZD dseg:8FAC                                                                                         ; 20 bytes
// WZD dseg:8FAE 00 00                                           AI_Myr_NewColConts@ dw 0                ; DATA XREF: MoM_Init_Tables+3E4w
// WZD dseg:8FAE                                                                                         ; 20 bytes
// WZD dseg:8FB0 00 00                                           AI_Arc_MainWarConts@ dw 0               ; DATA XREF: MoM_Init_Tables+3B4w ...
// WZD dseg:8FB0                                                                                         ; 12 bytes, Arcanus array
// WZD dseg:8FB2 00 00                                           AI_Myr_MainWarConts@ dw 0               ; DATA XREF: MoM_Init_Tables+3BAw
// WZD dseg:8FB2                                                                                         ; 12 bytes, Myrror array
// WZD dseg:8FB4 00                                              unk_3FA54 db    0                       ; DATA XREF: sub_F6CAB+103o
// WZD dseg:8FB5 00                                              db    0
// WZD dseg:8FB6 00                                              db    0
// WZD dseg:8FB7 00                                              db    0
// WZD dseg:8FB8 00                                              db    0
// WZD dseg:8FB9 00                                              db    0
// WZD dseg:8FBA 00                                              db    0
// WZD dseg:8FBB 00                                              db    0
// WZD dseg:8FBC 00                                              db    0
// WZD dseg:8FBD 00                                              db    0
// WZD dseg:8FBE 00                                              db    0
// WZD dseg:8FBF 00                                              db    0
// WZD dseg:8FC0 00                                              db    0
// WZD dseg:8FC1 00                                              db    0
// WZD dseg:8FC2 00                                              db    0
// WZD dseg:8FC3 00                                              db    0
// WZD dseg:8FC4 00 00                                           Some_AI_Turn_Var_2 dw 0                 ; DATA XREF: AI_Overland_Turn+212w
// WZD dseg:8FC4                                                                                         ; now 0
// WZD dseg:8FC6 00 00                                           Some_AI_Turn_Var_3 dw 0                 ; DATA XREF: AI_Overland_Turn+218w
// WZD dseg:8FC6                                                                                         ; now 0
// WZD dseg:8FC8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_Target_Values dw 19h dup(     0)     ; DATA XREF: AI_AddTarget+3Aw ...
// WZD dseg:8FFA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_Target_Strengths dw 19h dup(     0)  ; DATA XREF: AI_AddTarget+2Dw ...
// WZD dseg:902C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_Target_Ys dw 19h dup(     0)         ; DATA XREF: AI_AddTarget+20w ...
// WZD dseg:905E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_Target_Xs dw 19h dup(     0)         ; DATA XREF: AI_AddTarget+13w ...
// WZD dseg:9090 00                                              db    0
// WZD dseg:9091 00                                              db    0
// WZD dseg:9092 00 00                                           AI_Target_Count dw 0                    ; DATA XREF: AI_CreateTargetList+8w ...
// WZD dseg:9094 00 00                                           AI_Transport_Count dw 0                 ; DATA XREF: AI_OVL_Eval_Assets+1262w ...
// WZD dseg:9096 00 00                                           AI_SeekTransport_Cnt dw 0               ; DATA XREF: AI_SetUnitOrders:loc_EBDECw ...
// WZD dseg:9098 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_SeekTransport_Ps dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+B4r ...
// WZD dseg:90B6 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_SeekTransport_Ys dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+AAr ...
// WZD dseg:90D4 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_SeekTransport_Xs dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+8Er ...
// WZD dseg:90F2 00 00                                           AI_Arc_ContBalances@ dw 0               ; DATA XREF: AI_Continent_Eval+4C5r ...
// WZD dseg:90F2                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90F2                                                                                         ; own str - enemy str - own value
// WZD dseg:90F4 00 00                                           AI_Myr_ContBalances@ dw 0               ; DATA XREF: AI_Turn_NearAllocs+DCw
// WZD dseg:90F4                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90F4                                                                                         ; own str - enemy str - own value
// WZD dseg:90F6 00 00                                           AI_Arc_Cont_Nme_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+64r ...
// WZD dseg:90F6                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90F8 00 00                                           AI_Myr_Cont_Nme_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+72r ...
// WZD dseg:90F8                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90FA 00 00                                           AI_Arc_Cont_Own_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+48r ...
// WZD dseg:90FA                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90FC 00 00                                           AI_Myr_Cont_Own_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+56r ...
// WZD dseg:90FC                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90FE 00 00                                           CRP_AI_Arc_Cont_Nme_Str@ dw 0           ; DATA XREF: AI_Continent_Eval+2Cr ...
// WZD dseg:90FE                                                                                         ; redundant, should not be the primary pointer
// WZD dseg:9100 00 00                                           CRP_AI_Myr_Cont_Nme_Str@ dw 0           ; DATA XREF: AI_Continent_Eval+3Ar ...
// WZD dseg:9100                                                                                         ; redundant, should not be the primary pointer
// WZD dseg:9102 00 00                                           AI_Arc_Cont_Own_Str@ dw 0               ; DATA XREF: AI_Continent_Eval+10r ...
// WZD dseg:9102                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:9104 00 00                                           AI_Myr_Cont_Own_Str@ dw 0               ; DATA XREF: AI_Continent_Eval+1Er ...
// WZD dseg:9104                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:9106 00 00                                           AI_Enemy_Stacks@ dw 0                   ; DATA XREF: AI_Continent_Eval+121r ...
// WZD dseg:9106                                                                                         ; 1440 LBX_NearAlloc_Next bytes
// WZD dseg:9108 00 00                                           AI_Own_Stacks@ dw 0                     ; DATA XREF: AI_Continent_Eval+8Er ...
// WZD dseg:9108                                                                                         ; 800 LBX_NearAlloc_Next bytes
// WZD dseg:910A 00 00                                           AI_Enemy_Stack_Count dw 0               ; DATA XREF: AI_Continent_Eval:loc_D2593r ...
// WZD dseg:910C 00 00                                           AI_Own_Stack_Count dw 0                 ; DATA XREF: AI_Continent_Eval:loc_D24FFr ...
// WZD dseg:910E 00 00 00 00 00 00 00 00 00 00 00 00             AI_AtWarWith_Players dw 6 dup(0)        ; DATA XREF: AI_Count_Active_Wars+43w ...
// WZD dseg:911A 00 00                                           AI_Active_War_Count dw 0                ; DATA XREF: AI_Count_Active_Wars+7w ...
// WZD dseg:911C 00 00                                           CRP_AI_OVL_SpellList dw 0               ; DATA XREF: AI_OVL_GetSpellList+DBr ...
// WZD dseg:911C                                                                                         ; 50 LBX_NearAlloc_Next bytes
// WZD dseg:911C                                                                                         ; redundant - filled out but never used
// WZD dseg:911E 00 00                                           CRP_AI_OVL_SpellCount dw 0              ; DATA XREF: AI_OVL_GetSpellList+8w ...
// WZD dseg:911E                                                                                         ; redundant - calculated but never used
// WZD dseg:9120 00 00                                           AI_OVL_Spell_Cats dw 0                  ; DATA XREF: AI_OVL_SplCat_Picker:loc_E4471r ...
// WZD dseg:9120                                                                                         ; 92 LBX_NearAlloc_Next bytes
// WZD dseg:9122 00 00                                           AI_NME_Garrison_Strs dw 0               ; DATA XREF: AI_OVL_Eval_Assets+15r ...
// WZD dseg:9122                                                                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:9124 00 00                                           AI_Own_Garr_Strs dw 0                   ; DATA XREF: AI_OVL_Eval_Assets+24r ...
// WZD dseg:9124                                                                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:9126 00 00                                           AI_Enemy_City_Values dw 0               ; DATA XREF: AI_Continent_Eval+36Fr ...
// WZD dseg:9126                                                                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:9128 00 00                                           AI_Own_City_Values dw 0                 ; DATA XREF: AI_Continent_Eval+351r ...
// WZD dseg:9128                                                                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:912A
// WZD dseg:912A                                                 ¿ BEGIN: ?
// WZD dseg:912A
// WZD dseg:912A 00 00                                           g_TimeStop_PlayerNum dw 0               ; DATA XREF: Loaded_Game_Update_WZD+C5w ...
// WZD dseg:912A                                                                                         ; index of the Time Stop wizard plus 1 (0 if none)

// WZD dseg:912C
uint8_t * spell_data_table;

// WZD dseg:912C                                                                                         ; 215 records of 36 bytes
// WZD dseg:9130 00 00                                           IMG_GAME_SpellAnim@ dw 0                ; DATA XREF: GAME_LearnSpellAnim+374w ...
// WZD dseg:9130                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:9132 00 00                                           SND_SpellCast@ dw 0                     ; DATA XREF: GAME_LoadSpellSound+7Cw ...
// WZD dseg:9132                                                                                         ; appended into World_Data@ during combat, or the
// WZD dseg:9132                                                                                         ; Sandbox overland (-1 if none or SFX are disabled)
// WZD dseg:9134 00 00                                           SND_Spell_Music@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+39w ...

// WZD dseg:9136                                                 ? BEGIN:  - Uninitialized Data ?



// WZD dseg:9136
SAMB_ptr TBL_Items;
// WZD dseg:913A
SAMB_ptr TBL_Premade_Items;
// WZD dseg:913E
// IMG_USW_Items ITEM_ICONS
// WZD dseg:9226
SAMB_ptr Active_Unit;
// WZD dseg:922A
// TBL_BattleUnits
// WZD dseg:922E
// TBL_CombatEnchants


// WZD dseg:9232
SAMB_ptr p0_heroes;
// WZD dseg:9236
SAMB_ptr p1_heroes;
// WZD dseg:923A
SAMB_ptr p2_heroes;
// WZD dseg:923E
SAMB_ptr p3_heroes;
// WZD dseg:9242
SAMB_ptr p4_heroes;
// WZD dseg:9246
SAMB_ptr p5_heroes;


/*
    scouted / scouting - Unit Ability, Unit Special Ability, Building Effect, Spell Effect (City Enchantment)
    ~ scouted_table
    ~ TBL_scouted
    updated per turn and on move
    300 byte bit-field
    Set,Clear,Test Bit Field
    boolean {F,T} flag for Fog-of-War feature
    ~ IsScouted()
*/
// WZD dseg:9392
// drake178: Visibility_Myrror
uint8_t * square_scouted_p0;                // Bit_Field  alloc in Allocate_Data_Space()
// WZD dseg:9396
// drake178: Visibility_Arcanus
uint8_t * square_scouted_p1;                // Bit_Field  alloc in Allocate_Data_Space()


/* -2: NEVER, -1: ALWAYS, {0,1,2,3}: frame - draw off, {4,5,6,7}: frame - draw on */
// ? Set_Draw_Active_Stack_...() means this variable is private to ovr067 ?
// WZD dseg:9442
int16_t draw_active_stack_flag;  // AKA Active_Stack_Draw

// WZD dseg:9444
int16_t all_units_moved;


// dseg:9644 00 00                                           _city_idx dw 0                          ; DATA XREF: City_Screen+55r ...
// dseg:9646 00                                              db    0
// dseg:9647 00                                              db    0

// WZD dseg:9648
SAMB_ptr UnitDraw_WorkArea;  // alloc in MoM_Init_Tables(), 60 PR, 960 DB

// dseg:964A 00                                              db    0
// dseg:964B 00                                              db    0

// WZD dseg:964C
// MoO2: ~== _ship_node
int16_t entities_on_movement_map[120];  //  12 * 10  MAP_WIDTH * MAP_HEIGHT

// dseg:973C                                                 ; unsigned int gsa_BACKGRND_3_IMG_CMB_Bottom_BG
// dseg:973C 00 00                                           gsa_BACKGRND_3_IMG_CMB_Bottom_BG dw 0   ; DATA XREF: Load_Combat_Background_Bottom+12w ...


// dseg:998A END:  Main Screen Pictures

// WZD dseg:998C
// GUI_SmallWork_IMG@ dw 0
    // 96h paragraphs used for building GUI notification images
    // (although the pointer variable is also used for building combat figure images, after which it is reassigned)

// WZD dseg:998E
SAMB_ptr _screen_seg;
// allocated in MoM_Init_Tables()

// WZD dseg:9990
// _current_screen

// WZD dseg:9992 RP_GUI_GrowOutFrames dw 0               ; DATA XREF: VGA_PageFlip_FX+41r ...
// WZD dseg:9994 RP_GUI_GrowOutTop dw 0                  ; DATA XREF: VGA_PageFlip_FX+45r ...
// WZD dseg:9996 RP_GUI_GrowOutLeft dw 0                 ; DATA XREF: VGA_PageFlip_FX+49r ...

// WZD dseg:9998
// AKA TBL_Events
uint8_t * _events_table;  // alloc in Allocate_Data_Space()  7 PR 112 B  type? 1-byte,2-byte signed,unsigned 112 events or 56 events? Hrrm... all the indexing is in evens, so 2-byte?

// WZD dseg:999C
int16_t _unit_stack_count;
// WZD dseg:999E
struct s_STACK _unit_stack[9];




// WZD dseg:9C90
SAMB_ptr TBL_OvlMovePaths_EMS;
// WZD dseg:9C94
// CONTX_Arc_NmeStrMap
// WZD dseg:9C98
// CONTX_Myr_NmeStrMap
// WZD dseg:9C9C
SAMB_ptr TBL_Catchments_EMS;
// WZD dseg:9CA0
SAMB_ptr TBL_SharedTiles_EMS;
// WZD dseg:9CA4
SAMB_ptr TBL_TempMoveMap_EMS;
// WZD dseg:9CA8
// COL_MinimapTiles
// WZD dseg:9CAC
SAMB_ptr TBL_MoveMaps_EMS;


// WZD dseg:9CB0
/*
    302 Paragraphs, 4832 Bytes
    4800 bytes used - 1-byte value, 2400 world squares, 2 planes
*/
uint8_t * TBL_Scouting;                     // load in Load_SAVE_GAM()
// WZD dseg:9CB4  
uint8_t * TBL_Terrain_Flags;                // load in Load_SAVE_GAM()
// WZD dseg:9CB8  
SAMB_ptr TBL_Terr_Specials;                 // load in Load_SAVE_GAM()

// WZD dseg:9CBC  
// SAMB_ptr _CITIES;
// struct s_CITY _CITIES[CITY_COUNT_MAX];  // 100 * sizeof(114)
struct s_CITY * _CITIES;

// WZD dseg:9CC0
// drake178: TBL_Encounters
/*
    alloc in Allocate_Data_Space()
    load in Load_SAVE_GAM() - read 102 of 24

*/
struct s_LAIR * TBL_Lairs;

// WZD dseg:9CC4  
// struct s_TOWER _TOWERS[NUM_TOWERS];  // 6 * sizeof(4)
// struct s_TOWER _TOWERS[];
struct s_TOWER * _TOWERS;

// WZD dseg:9CC8  
struct s_FORTRESS * _FORTRESSES;

// WZD dseg:9CCC  
struct s_NODE * TBL_Nodes;

// WZD dseg:9CD0
uint8_t * TBL_Landmasses;  // 12Eh 302d PR 302*16=4832, in World_Data
// WZD dseg:9CD4
SAMB_ptr UU_TBL_2;
// WZD dseg:9CD8
SAMB_ptr UU_TBL_1;
// WZD dseg:9CDC
// AKA TBL_Maps;
// SAMB_ptr _world_maps;
uint8_t * _world_maps;

// WZD dseg:9D1A
uint16_t tmp_World_Data_Paras;

// WZD dseg:9D1C
SAMB_ptr World_Data_Extra;

// WZD dseg:9D1E
// ? blocks, alloc in MoM_Init_Tables()
SAMB_ptr GFX_Swap_Seg;

// WZD dseg:9D20
SAMB_ptr World_Data;

// WZD dseg:9D22
uint16_t grand_vizier;


// WZD dseg:9EC2
// SAMB_ptr TBL_Units;
// struct s_UNIT _UNITS[UNIT_COUNT_MAX];  // 1009 * sizeof(32)
// struct s_UNIT _UNITS[];
struct s_UNIT * _UNITS;

// WZD dseg:9EC6
SAMB_ptr TBL_Hero_Names;  // 25h 37d PR 37*16=592

// WZD dseg:9ECA
struct s_WIZARD _players[6];
// struct s_WIZARD * _players;


// WZD dseg:BB7A
// struct s_HLP_ENTRY _help_entries[50];
uint8_t _help_entries[500];


// WZD dseg:BD6E 
// config_mom s_CONFIG_MOM
// WZD dseg:BD80 
// UU_IMG_OVL_WorkMark@

// WZD dseg:974A
// AKA G_OVL_MapDisplay_Y
int16_t _prev_world_y;
// WZD dseg:974C
// G_OVL_MapDisplay_X
int16_t _prev_world_x;

// WZD dseg:BD82 
int16_t OVL_Map_CenterY;  // AKA _active_world_y
// WZD dseg:BD84 
int16_t OVL_Map_CenterX;  // AKA _active_world_x
// WZD dseg:BD86 
int16_t _map_plane;  // AKA _world_plane
// WZD dseg:BD88 
int16_t _map_y;  // AKA _curr_world_y
// WZD dseg:BD8A 
int16_t _map_x;  // AKA _curr_world_x

// WZD dseg:BD8C
int16_t _human_player_idx = 0;

// WZD dseg:BD8E
int16_t _unit;
int16_t _turn;
int16_t _units;  // MoO2 _NUM_SHIPS
int16_t _cities;
int16_t _difficulty;
int16_t _magic;
int16_t _landsize;
int16_t _num_players;  // MoO2 _NUM_PLAYERS    New Game: magic_set.opponents + 1
struct s_GAME_DATA game_data;
// WZD dseg:BD9E
// magic_set s_MAGIC_SET

// ovr150 MainScr_Maps  // WZD dseg:CB5C
// ovr150 MainScr_Maps  // AKA OVL_NewMapDrawing
// ovr150 MainScr_Maps  int16_t draw_map_full;  //; determines whether non-animated terrain tiles will be redrawn or not
