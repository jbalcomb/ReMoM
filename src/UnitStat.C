/*
    Unit Statistics Window
    
    WIZARDS.EXE
        ovr072
*/

#include "MoM.H"
#include "UnitStat.H"
#include "CityScr.H"    /* Draw_Building_Picture_To_Bitmap(); */
#include "Lair.H"       /* STR_ListSeparator(); */
#include "MainScr.H"    /* Draw_Unit_Enchantment_Outline(); Cycle_Unit_Enchantment_Animation(); */
#include "UnitView.H"
#include "City_ovr55.H" /* Draw_Resource_Icons(); */
#include "ItemView.H"  /* Cycle_Item_Enchantment_Animation(); */

/*
    Product Screen
    used for Thing_View(); CalledFromFlag == 0, thing_idx < 100, view_type == 0
*/
extern char * building_description;
void Building_Allows_List__WIP(int16_t bldg_idx, int16_t * allows_list_count, int16_t allows_list[]);



// WZD dseg:33B8 01 00 20 00 40 00 04 00 02 00 08 00             MoveFlag_Array dw M_Cavalry, M_Forester, M_Mntnr, M_Swimming, M_Sailing, M_Flying
// WZD dseg:33B8                                                                                         ; DATA XREF: Stack_Movement_Modes+10o
// WZD dseg:33B8                                                                                         ; drake178: MoveFlag_Array
// WZD dseg:33C4 50 4F 52 54 52 41 49 54                         cnst_PORTRAIT_File2 db 'PORTRAIT'       ; DATA XREF: USW_CombatDisplay:loc_5E24Fo ...
// WZD dseg:33C4                                                                                         ; should use dseg:326e

// WZD dseg:33CC
char cnst_ZeroString_10[] = "0";

// WZD dseg:33CD
char cnst_Ok_BtnMsg_2[] = "Ok";

// WZD dseg:33D0
char cnst_HOTKEY_O_6[] = "O";

// WZD dseg:33D2
char cnst_Dismiss_BtnMsg[] = "Dismiss";

// WZD dseg:33DA
char cnst_HOTKEY_D_4[] = "D";

// WZD dseg:33DC
char cnst_HOTKEY_Esc6[] = "\1B";

// WZD dseg:33DE
char cnst_HOTKEY_X_4[] = "X";

// WZD dseg:33E0 44 6F 20 79 6F 75 20 77 69 73 68 20 74 6F 20 64+cnst_Disband_Msg1 db 'Do you wish to disband the unit of ',2,0
// WZD dseg:33E0 69 73 62 61 6E 64 20 74 68 65 20 75 6E 69 74 20+                                        ; DATA XREF: Unit_Statistics_Popup+695o
// WZD dseg:3405 01                                              cnst_Disband_Msg2 db 1                  ; DATA XREF: Unit_Statistics_Popup+705o
// WZD dseg:3405                                                                                         ; format string (color toggle)
// WZD dseg:3406 3F 00                                           cnst_Disband_Msg3 db '?',0              ; part of the above
// WZD dseg:3408 44 6F 20 79 6F 75 20 77 69 73 68 20 74 6F 20 64+cnst_Dismiss_Msg db 'Do you wish to dismiss ',2,0
// WZD dseg:3408 69 73 6D 69 73 73 20 02 00                                                              ; DATA XREF: Unit_Statistics_Popup:loc_5EA13o
// WZD dseg:3421 43 6F 73 74 00                                  cnst_Cost db 'Cost',0                   ; DATA XREF: Unit_Statistics_Popup_Draw+190o
// WZD dseg:3426 28 00                                           cnst_OpeningBrace db '(',0              ; DATA XREF: Unit_Statistics_Popup_Draw+1C6o
// WZD dseg:3428 29 00                                           cnst_ClosingBrace db ')',0              ; DATA XREF: Unit_Statistics_Popup_Draw+202o
// WZD dseg:342A 54 68 65 00                                     cnst_The db 'The',0                     ; DATA XREF: Unit_Statistics_Popup_Draw+32Co
// WZD dseg:342E 4D 6F 76 65 73 00                               cnst_Moves_2 db 'Moves',0               ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F1CCo
// WZD dseg:342E                                                                                         ; could use dseg:6204
// WZD dseg:3434 48 65 61 64 73 00                               cnst_Heads db 'Heads',0                 ; DATA XREF: Unit_Statistics_Popup_Draw+5FEo
// WZD dseg:343A 44 61 6D 61 67 65 00                            cnst_Damage db 'Damage',0               ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F2BAo
// WZD dseg:343A                                                                                         ; could use dseg:4259
// WZD dseg:3441 55 70 6B 65 65 70 00                            cnst_Upkeep_2 db 'Upkeep',0             ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F304o
// WZD dseg:3448 4D 65 6C 65 65 00                               cnst_Melee db 'Melee',0                 ; DATA XREF: Unit_Statistics_Popup_Draw+72Bo
// WZD dseg:344E 52 61 6E 67 65 00                               cnst_Range db 'Range',0                 ; DATA XREF: Unit_Statistics_Popup_Draw+7A7o
// WZD dseg:344E                                                                                         ; could use dseg:40c0
// WZD dseg:3454 41 72 6D 6F 72 00                               cnst_Armor db 'Armor',0                 ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F4C9o
// WZD dseg:3454                                                                                         ; could use dseg:4363 (or dseg:4425)
// WZD dseg:345A 52 65 73 69 73 74 00                            cnst_Resist db 'Resist',0               ; DATA XREF: Unit_Statistics_Popup_Draw+8BBo
// WZD dseg:3461 48 69 74 73 00                                  cnst_Hits db 'Hits',0                   ; DATA XREF: Unit_Statistics_Popup_Draw+916o
// WZD dseg:3466 4D 61 69 6E 74 65 6E 61 6E 63 65 00             cnst_Maintenance db 'Maintenance',0     ; DATA XREF: Unit_Statistics_Popup_Draw:loc_5F764o
// WZD dseg:3466                                                                                         ; could use dseg:2d3c
// WZD dseg:3472 41 6C 6C 6F 77 73 00                            cnst_Allows db 'Allows',0               ; DATA XREF: Unit_Statistics_Popup_Draw+C3Bo
// WZD dseg:3479 2E 00                                           cnst_Dot4 db '.',0                      ; DATA XREF: Unit_Statistics_Popup_Draw+D4Fo
// WZD dseg:3479                                                                                         ; should use dseg:2b31
// WZD dseg:347B 55 00                                           cnst_HOTKEY_U_4 db 'U',0                ; DATA XREF: USW_CreateScrollBtns+22o
// WZD dseg:347B                                                                                         ; should use dseg:2e28
// WZD dseg:347D 00                                              db    0

// WZD dseg:347E                                                 ?
// WZD dseg:347E                                                     BEGIN: DIPLOMAC || Magic Screen - Initialized Data
// WZD dseg:347E                                                 ?
// WZD dseg:347E
// WZD dseg:347E 01 00                                           GAME_AlchemyDir dw 1                    ; DATA XREF: GAME_AlchemyWindow+19Dr ...



// WZD dseg:C176                                                 ¿ BEGIN:  ovr072  Unit View ?

// dseg:C176 00 00                                           
int16_t UV_InCombat;

// dseg:C178 00 00                                           
int16_t UV_IsHeroUnit;

// dseg:C17A
int16_t USW_TransparentBase;

// dseg:C17C
// XREFs: Load_Unit_Figure()  Draw_Unit_Figure()
SAMB_ptr unit_figure_seg;  // loaded into screen_seg

// dseg:C17E
int16_t unitview_dn_button;

// dseg:C180
int16_t unitview_up_button;

// dseg:C182
int16_t unitview_dismiss_button;

// dseg:C184
int16_t unitview_ok_button;

// dseg:C186
int16_t unitview_full_screen;

// dseg:C188
int16_t unitview_button_box;

// dseg:C18A
int16_t unitview_window;

// dseg:C18C
int16_t UV_ListItem;

// dseg:C18E
int16_t UV_IsMultiPage;

// dseg:C190
int16_t UV_specials_count;

// dseg:C192
struct s_UV_List * UV_specials_list;

// dseg:C194
// drake178: USW_ShownFX_Flags
int32_t USW_ShownFX_Flags[8];

// dseg:C1B4
// drake178: USW_ItemSlot_ClkLbls
int16_t USW_ItemSlot_ClkLbls[3];

// dseg:C1BA
// drake178: USW_ItemSlot_IMGLbls
int16_t USW_ItemSlot_IMGLbls[3];

// dseg:C1C0
// drake178: USW_FX_ClickLabels
int16_t USW_FX_ClickLabels[8];

// dseg:C1D0
// drake178: USW_FX_ImageLabels
int16_t USW_FX_ImageLabels[8];

// dseg:C1E0
// ~== ITEM_ComposeBuffer@ for Item Screen
SAMB_ptr UV_item_icon_pict_seg;

// dseg:C1E2
int16_t UV_unit_idx;

// dseg:C1E4
int16_t UV_view_type;  // enum e_VIEW_TYPE

// dseg:C1E6
int16_t UV_y_start;

// dseg:C1E8
int16_t UV_x_start;

// WZD dseg:C1E8                                                 ¿ END:  ovr072  Unit View ?



/*
    WIZARDS.EXE  ovr072
*/

// WZD o72p01
// drake178: USW_CombatDisplay()

// WZD o72p02
// drake178: USW_LoadAndShow()
/*
    Main_Screen()
        BEGIN: Right-Click Unit Window Grid Field
            USW_FullDisplay(_unit_stack[Stack_Index].unit_idx, target_world_x, target_world_y, (target_world_x + 18), (target_world_y + 18));
                USW_LoadAndShow(31, 6, x1, y1, x2, y2, 1, unit_idx);
*/
/*

*/
void USW_LoadAndShow(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t flag, int16_t unit_idx)
{
    SAMB_ptr Sandbox_Paragraph;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: USW_LoadAndShow()\n", __FILE__, __LINE__);
#endif

    // ; resets the GFX_Swap_Seg allocation, then appends reserved EMM handle headers into it for unit view, item, and city scape graphics
    GFX_Swap_Overland();

    Sandbox_Paragraph = Allocate_First_Block(_screen_seg, 1);

    Mark_Block(_screen_seg);

    USW_MemAlloc();
// ; resets the near allocation buffer, creates a 100
// ; byte GUI_String_1@ in it followed by an 1520 byte
// ; UV_specials_list@, and finishes with a 480 byte
// ; LBX_Alloc_Next into the sandbox for an item icon
// ; work area

    // UV_InCombat = 0;
    
    if(_UNITS[unit_idx].type <= 0x22 /* _Chosen */)
    {
        // ; loads an LBX entry into an existing LBX_Alloc_Space
        // ; segment, appending to the existing content
        // ; returns a segment pointer to the data
        // ; quits on failure
        // IMG_USW_HeroPortrt = LBX_Reload_Next(cnst_PORTRAIT_File2, _unit_type_table[_UNITS[unit_idx].type].Bldng1_or_Portrait, _screen_seg);
        IMG_USW_HeroPortrt = LBX_Reload_Next("PORTRAIT", _unit_type_table[_UNITS[unit_idx].type].hero_portrait, _screen_seg);
    }
    else
    {
        Load_Unit_Figure(unit_idx, 0);
    }

    // ; displays the unit statistics window for the specified
    // ; unit at the selected coordinates, "growing out" of
    // ; the passed rectangle if expanding help is enabled and
    // ; the function is called out of combat
    // ; inherits multiple BUGs
    Unit_Statistics_Popup(x_start, y_start, x1, y1, x2, y2, flag, unit_idx);

    Release_Block(_screen_seg);

    GFX_Swap_Cities();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: USW_LoadAndShow()\n", __FILE__, __LINE__);
#endif

}

// WZD o72p03
void Unit_Statistics_Popup(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t view_type, int16_t unit_idx)
{
    int16_t hotkey_idx_X;
    int16_t hotkey_idx_ESC;
// Item_Index= word ptr -10h
    int16_t screen_changed = ST_FALSE;
// Shown_Effect_Count= word ptr -0Ch
    int16_t Enable_Remove_Unit_Enchantment;  /* T/F - if T, match left-clicks to unit ability/effect input fields */
    int16_t scanned_field;
    int16_t input_field_idx;
// Unit_Disbanded= word ptr -4
    int16_t leave_screen;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: UnitStatPup_Draw()\n", __FILE__, __LINE__);
#endif

    UV_x_start = x_start;
    UV_y_start = y_start;

    UV_view_type = view_type; // ; 2 if combat

    UV_unit_idx = unit_idx;

    Clear_Fields();

    // TODO  Deactivate_Auto_Function();
    // TODO  Assign_Auto_Function(Unit_Statistics_Popup_Draw, 1);

    Deactivate_Help_List();

    // ; loads and sets the GUI help entry area array for the unit statistics window
    // TODO  HLP_Load_USW(view_type);

    UV_specials_count = 0;

    if( (view_type == 1) || (view_type == 2) )
    {
        // ; creates a unit statistics window effect list structure into the passed buffer, setting the element count into the return pointer
        // ; contains multiple BUGs that will either cause some effects to not show up, or the same thing to be noted two different ways
        // AKA USW_CreateFXList(UV_unit_idx, UV_specials_list, &UV_specials_count);
        // TODO  UV_Build_Effect_List(UV_Unit_Index, &UV_specials_list[0], &UV_specials_count)
    }


    UV_ListItem = 0;
    UV_IsMultiPage = ST_FALSE;
    UV_IsHeroUnit = ST_FALSE;


    if(_UNITS[UV_unit_idx].Hero_Slot != -1)
    {
        UV_IsHeroUnit = ST_TRUE;
    }
    
    if( (UV_IsHeroUnit == ST_TRUE) && (UV_specials_count > 4) )
    {
        UV_IsMultiPage = ST_TRUE;
    }

    if(UV_specials_count > 8)
    {
        UV_IsMultiPage = ST_TRUE;
    }

    Enable_Remove_Unit_Enchantment = ST_TRUE;

    if(view_type == 2)
    {
        Enable_Remove_Unit_Enchantment = ST_FALSE;
    }

    Load_Palette_From_Animation(unitview_large_background_seg);
    Apply_Palette();
    Set_Palette_Changes(244, 255);
    Calculate_Remap_Colors();
    Set_Page_Off();
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();
    screen_changed = ST_FALSE;  // DNE in Dasm
    Set_Input_Delay(4);

    
    USW_TransparentBase = ST_TRUE;
    Set_Page_Off();
    // ; draws the unit statistics window into the current
    // ; draw frame based on global variables - includes the
    // ; side and scroll buttons if applicable, but background
    // ; loading has to be done by a parent function as this
    // ; one draws on top of whatever is already there
    Unit_Statistics_Popup_Do_Draw();

    if(UV_InCombat == ST_FALSE)
    {
        // ; if expanding help is enabled, "grows" an area of the
        // ; current draw frame out of the current display frame
        // ; using both EMM_VGAFILE_H and a sandbox buffer, and
        // ; sets up the global variables used for redrawing the
        // ; growth lines afterwards, if necessary
        // TODO  HLP_DrawExpanding(UV_x_start, UV_y_start, (UV_x_start + 217), (UV_y_start + 184), UV_x_start, UV_y_start, (UV_x_start + 282), (UV_y_start + 184),x1, y1, x2, y2, 1);
    }

    USW_TransparentBase = 0;

    // ; clears the help entries assigned to the scroll up and
    // ; down buttons of the active ability/effect list, then
    // ; assigns help descriptions to the elements actually
    // ; shown, while clearing the entries for the rest
    // TODO  USW_SetFXHelp(UV_specials_list, UV_specials_count, 0);

    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();

// push    [UV_ListItem]            ; FirstRec
// push    [UV_specials_count]               ; Count
// push    [UV_specials_list@]               ; FX@
// mov     ax, [UV_y_start]
// add     ax, 108
// push    ax                              ; Top
// mov     ax, [UV_x_start]
// add     ax, 8
// push    ax                              ; Left
// nop
// push    cs
// call    near ptr USW_CreateFXControls   ; clears and recreates the controls associated with the
//                                         ; ability/effects section of the unit statistics
//                                         ; window, including the up and down scroll arrows

        Set_Font(4, 4, 0, 0);
        Set_Alias_Color(203);


        unitview_ok_button = -1000;
        if(view_type != 2)
        {
            // unitview_ok_button = Add_Button_Field((UV_x_start + 222), (UV_y_start + 162), cnst_Ok_BtnMsg_2, red_button_seg, cnst_HOTKEY_O_6, -1);
            unitview_ok_button = Add_Button_Field((UV_x_start + 222), (UV_y_start + 162), cnst_Ok_BtnMsg_2, red_button_seg, 'O', -1);
        }

        unitview_dismiss_button = -1000;
        if(view_type == 1)
        {
            // unitview_dismiss_button = Add_Button_Field((UV_x_start + 222), (UV_y_start + 143), cnst_Dismiss_BtnMsg, red_button_seg, cnst_HOTKEY_D_4, -1);
            unitview_dismiss_button = Add_Button_Field((UV_x_start + 222), (UV_y_start + 143), cnst_Dismiss_BtnMsg, red_button_seg, 'D', -1);
        }

        // unitview_window = Add_Hidden_Field(UV_x_start, UV_y_start, (UV_x_start + 217), (UV_y_start + 184), cnst_ZeroString_10, -1);
        unitview_window = Add_Hidden_Field(UV_x_start, UV_y_start, (UV_x_start + 217), (UV_y_start + 184), 0, -1);

        unitview_button_box = -1000;
        if(view_type != 2)
        {
            // unitview_button_box = Add_Hidden_Field((UV_x_start + 213), (UV_y_start + 133), (UV_x_start + 280), (UV_y_start + 183), cnst_ZeroString_10, -1);
            unitview_button_box = Add_Hidden_Field((UV_x_start + 213), (UV_y_start + 133), (UV_x_start + 280), (UV_y_start + 183), 0, -1);
        }

        // unitview_full_screen = Add_Hidden_Field(0, 0, 319, 199, cnst_ZeroString_10, -1);
        unitview_full_screen = Add_Hidden_Field(0, 0, 319, 199, 0, -1);

        // hotkey_idx_ESC = Add_Hot_Key(cnst_HOTKEY_Esc6);
        hotkey_idx_ESC = Add_Hot_Key('\x1B');
        // hotkey_idx_X = Add_Hot_Key(cnst_HOTKEY_X_4);
        hotkey_idx_X = Add_Hot_Key('X');


        // ; clears the help entries assigned to the scroll up and
        // ; down buttons of the active ability/effect list, then
        // ; assigns help descriptions to the elements actually
        // ; shown, while clearing the entries for the rest
        // TODO  USW_SetFXHelp([UV_specials_list, UV_specials_count, 0)



        input_field_idx = Get_Input();

        scanned_field = Scan_Input();


        /*
            BEGIN: Right-Click Hero Unit Item Slot Field
        */
        /*
            END: Right-Click Hero Unit Item Slot Field
        */

        /*
            BEGIN: Left-Click Unit Ability/Effect Field - Remove Unit Enchantment
        */
        /*
            END: Left-Click Unit Ability/Effect Field - Remove Unit Enchantment
        */

        /*
            BEGIN: Left-Click Dismiss Button
        */
        if(input_field_idx == unitview_dismiss_button)
        {

        }
        /*
            END: Left-Click Dismiss Button
        */

        /*
            BEGIN: Left-Click Off-Window, Left-Click OK Button, hotkey ESCAPE
        */
        if( (input_field_idx == unitview_full_screen) || (input_field_idx == unitview_ok_button) || (input_field_idx == hotkey_idx_ESC) )
        {
            // TODO  SND_LeftClickSound();
            leave_screen = ST_UNDEFINED;
        }

        /*
            END: Left-Click Off-Window, Left-Click OK Button, hotkey ESCAPE
        */




        
        if( (leave_screen == ST_FALSE) && (screen_changed == ST_FALSE) )
        {
            Copy_Back_To_Off();
            Unit_Statistics_Popup_Do_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
        screen_changed = ST_FALSE;
    }



// @@Done:
    PageFlipEffect = 3;
    Reset_Window();
    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: UnitStatPup_Draw()\n", __FILE__, __LINE__);
#endif

}

// WZD o72p04
/*
; draws the 'Unit View' into the current
; draw frame based on global variables - includes the
; side and scroll buttons if applicable, but background
; loading has to be done by a parent function as this
; one draws on top of whatever is already there
*/
void Unit_Statistics_Popup_Do_Draw(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Unit_Statistics_Popup_Do_Draw()\n", __FILE__, __LINE__);
#endif

    if(UV_InCombat != ST_FALSE)
    {
        // TODO  G_USW_DrawEHLines();
    }

    Thing_View_Draw__WIP(UV_x_start, UV_y_start, UV_view_type, UV_unit_idx, UV_specials_list, UV_specials_count, UV_item_icon_pict_seg);

    if(UV_view_type != uvt_Cmbt)
    {
        FLIC_Set_CurrentFrame(red_button_seg, 0);
        FLIC_Draw((UV_x_start + 222), (UV_y_start + 162), red_button_seg);
        FLIC_Set_CurrentFrame(red_button_seg, 0);
        FLIC_Draw((UV_x_start + 222), (UV_y_start + 143), red_button_seg);
    }

    if(UV_IsMultiPage == ST_TRUE)
    {
        FLIC_Set_CurrentFrame(unitview_up_arrow_seg, 0);
        FLIC_Draw((UV_x_start + 204), (UV_y_start + 105), unitview_up_arrow_seg);
        FLIC_Set_CurrentFrame(unitview_down_arrow_seg, 0);
        FLIC_Draw((UV_x_start + 204), (UV_y_start + 168), unitview_down_arrow_seg);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Unit_Statistics_Popup_Do_Draw()\n", __FILE__, __LINE__);
#endif
}

// WZD o72p05
/*
    ~ Draw 'View' of *Thing*

Production_Screen_Draw()
Unit_Statistics_Popup_Draw__WIP((production_screen_xstart + build_table_item_xadd), production_screen_ystart, 0, product_idx, prod_specials_list, UV_specials_count, PS_product_pict_seg);
product_idx is building or unit
'View Type' is 0 - ¿ ?

; draws the unit statistics window, recruitment, or production detail window into the current draw frame
; BUG: draws the wrong movement type icons in certain situations (only uses ground, water, and air too)
; BUG: the "Allows" list for buildings can contain incorrect information
; BUG: fails to animate item slot 0
; WARNING: not enough icons for top tier units
*/
void Thing_View_Draw__WIP(int16_t x_start, int16_t y_start, int16_t CalledFromFlag, int16_t thing_idx, struct s_UV_List * specials_list, int16_t specials_count, SAMB_ptr item_icon_pict_seg)
{
    char temp_string[20];
    int16_t Allows_List[9];
    SAMB_ptr bldg_pict_seg;
    int16_t Object_IMG_Height;
    int16_t Object_IMG_Width;
    int16_t Object_IMG_Top;
    int16_t Object_IMG_Left;  // In Dasm, reused as tmp unit_type_idx
    int16_t unit_type_idx;
    int16_t Icon_Row_Length;
    uint8_t colors[6];
    int16_t bldg_pict_draw_y;
    int16_t bldg_pict_draw_x;
    int16_t Gold_Icons;
    int16_t unit_race;
    int16_t View_Type;
    int16_t Allows_Count;
    int16_t bldg_idx;
    int16_t unit_idx;
    int16_t hero_owner_idx;
    int16_t product_cost;
    int16_t Text_Top;
    int16_t Text_Left;
    int16_t product_idx;
    int16_t attack_icon_index;
    int16_t Allows_List_Size;
    int16_t itr;
    int16_t UV_x_start_offset;  // _SI_
    int16_t bldg_pict_h;   // DNE in Dasm
    int16_t bldg_pict_w;   // DNE in Dasm
    int16_t bldg_pict_y1;  // DNE in Dasm
    int16_t bldg_pict_x1;  // DNE in Dasm
    int16_t moves_icon_idx;  // DNE in Dasm, uses product_idx

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Thing_View_Draw__WIP()\n", __FILE__, __LINE__);
#endif

    UV_x_start = x_start;
    UV_y_start = y_start;

    unit_idx = thing_idx;
    bldg_idx = thing_idx;

    Set_Page_Off();

    if(USW_TransparentBase == ST_TRUE)
    {
        Fill(UV_x_start, UV_y_start, (UV_x_start + 281), (UV_y_start + 183), ST_TRANSPARENT);
    }

    /*
        Prod:  small background
        Stat:  large background && buttons
        Cmbt:  large background
    */
    if(CalledFromFlag == uvt_Prod)
    {
        FLIC_Draw(UV_x_start, UV_y_start, unitview_small_background_seg);
    }
    else
    {
        FLIC_Draw(UV_x_start, UV_y_start, unitview_large_background_seg);
        if(CalledFromFlag != uvt_Cmbt)
        {
            FLIC_Draw((UV_x_start + 213), (UV_y_start + 133), unitview_button_background_seg);
        }
    }


    /*
        CalledFromFlag && thing_idk
        
        State 1:  Prod && Bldg
        State 2:  Prod && Norm
        State 3:  Stat
        State 4:  Cmbt
    */
    View_Type = CalledFromFlag;
    if(CalledFromFlag == uvt_Prod)
    {
        if(unit_idx < 100)
        {
            View_Type = 0;
        }
        else
        {
            unit_idx -= 100;
            unit_type_idx = unit_idx;
            View_Type = 1;
        }
    }
    else
    {
        unit_type_idx = _UNITS[thing_idx].type;
    }
    UV_IsHeroUnit = ST_FALSE;
    if(CalledFromFlag != uvt_Prod)
    {
        if(_UNITS[thing_idx].Hero_Slot != -1)
        {
            UV_IsHeroUnit = ST_TRUE;
        }
    }
    /*
        CalledFromFlag && View_Type && UV_IsHeroUnit
        {0,0,0}  Prod-Bldg
        {0,1,0}  Prod-Norm
        {1,1,0}  Stat-Norm
        {2,2,0}  Cmbt-Norm
        {1,1,1}  Stat-Hero
        {2,2,1}  Cmbt-Hero
    
    */

    colors[0] = 197;
    colors[1] = 255;

    Set_Font_Colors_15(1, &colors[0]);
    Set_Font_Style_Outline(1, 15, 0, 0);
    Set_Outline_Color(251);


    UV_x_start_offset = 0;


    if(CalledFromFlag == uvt_Prod)  /* on 'Production Screen' */
    {
        product_idx = _CITIES[_city_idx].construction;
        _CITIES[_city_idx].construction = thing_idx;
        product_cost = City_Current_Product_Cost(_city_idx);  // Discount Cost
        _CITIES[_city_idx].construction = product_idx;
        UV_x_start_offset = 4;
        Print((UV_x_start + UV_x_start_offset + 51), (UV_y_start + 34), "Cost");
        itoa(product_cost, GUI_NearMsgString, 10);
        if(View_Type == 1)  /* Product is Unit */
        {
            strcat(GUI_NearMsgString, "(");
            itoa(_unit_type_table[unit_type_idx].Cost, temp_string, 10);  // Full Cost
            strcat(GUI_NearMsgString, temp_string);
            strcat(GUI_NearMsgString, ")");
        }
        Print((UV_x_start + UV_x_start_offset + 73), (UV_y_start + 34), GUI_NearMsgString);
    }



    // Dasm show `if != 0,1,2 then fall-through`; probably was a switch statement
    // HERE: (View_Type == 0) means the CalledFromFlag was 0 and the thing_idx was less than 100  i.e., On 'Production Screen', producing 'Building'
    if(View_Type == 0)
    {
/*
    BEGIN:  Buidling View
*/

        bldg_pict_draw_x = (UV_x_start + 6);
        bldg_pict_draw_y = (UV_y_start + 4);

        Mark_Block(_screen_seg);
        bldg_pict_seg = Allocate_Next_Block(_screen_seg, 500);  // 500 PR  8000 B
        Release_Block(_screen_seg);

        Draw_Building_Picture_To_Bitmap(_city_idx, bldg_idx, &bldg_pict_x1, &bldg_pict_y1, &bldg_pict_w, &bldg_pict_h, bldg_pict_seg);

        bldg_pict_draw_x += (((44 - bldg_pict_w) / 2) - bldg_pict_x1);
        bldg_pict_draw_y += (((36 - bldg_pict_h) / 2) - bldg_pict_y1);

        FLIC_Set_LoopFrame_1(bldg_pict_seg);

        Draw_Picture(bldg_pict_draw_x, bldg_pict_draw_y, bldg_pict_seg);

        colors[0] = 197;
        colors[1] = 14;
        colors[2] = 255;
        colors[3] = 80;
        colors[4] = 255;

        Set_Font_Colors_15(4, colors);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);
        Set_Font_Spacing_Width(1);

        strcpy(GUI_NearMsgString, bldg_data_table[bldg_idx].name);

        if(Get_String_Width(GUI_NearMsgString) > 100)
        {
            Print_Paragraph((UV_x_start_offset + UV_x_start + 51), (UV_y_start + 7), 100, GUI_NearMsgString, 0);
        }
        else
        {
            Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + 11), GUI_NearMsgString);
        }

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 47), "Maintenance");

        colors[0] = 197;
        colors[1] = 255;

        Set_Font_Colors_15(2, colors);
        Set_Font_Style_Outline(2, 15, 0, 0);
        Set_Outline_Color(251);

        if(bldg_data_table[bldg_idx].maintenance_cost == 0)
        {
            Print_Integer_Right((UV_x_start_offset + UV_x_start + 92), (UV_y_start + 48), bldg_data_table[bldg_idx].maintenance_cost);
        }
        else
        {
            Gold_Icons = abs(bldg_data_table[bldg_idx].maintenance_cost);

            for(itr = 0; itr < Gold_Icons; itr++)
            {
                FLIC_Draw((UV_x_start_offset + UV_x_start + (itr * 9) + 89), (UV_y_start + 49), gold1_icon_seg);
            }
        }

        Building_Allows_List__WIP(bldg_idx, &Allows_Count, &Allows_List[0]);

        colors[0] = 197;
        colors[1] = 14;
        colors[2] = 255;
        colors[3] = 80;
        colors[4] = 255;

        Set_Font_Colors_15(4, colors);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 58), "Allows");

        Text_Left = (UV_x_start_offset + UV_x_start + 60);
        Text_Top = (UV_y_start + 59);

        colors[0] = 197;
        colors[1] = 255;

        Set_Font_Colors_15(2, colors);
        Set_Font_Style_Outline(2, 15, 0, 0);
        Set_Font_Spacing_Width(1);
        Set_Font_LF(1);

        strcpy(GUI_NearMsgString, "");

        Allows_List_Size = 0;

        for(itr = 0; itr < Allows_Count; itr++)
        {
            STR_ListSeparator(&Allows_List_Size, Allows_Count, GUI_NearMsgString);

            if(Allows_List[itr] < 100)
            {
                strcpy(temp_string, bldg_data_table[Allows_List[itr]].name);
                strcat(GUI_NearMsgString, temp_string);
            }
            else
            {
                strcat(GUI_NearMsgString, *_unit_type_table[(Allows_List[itr] - 100)].Name);
            }
        }

        if(Allows_Count > 0)
        {
            strcat(GUI_NearMsgString, ".");
        }

        Print_Paragraph(Text_Left, Text_Top, 100, GUI_NearMsgString, 0);

        colors[0] = 197;
        colors[1] = 14;
        colors[2] = 255;
        colors[3] = 80;
        colors[4] = 255;

        Set_Font_Colors_15(4, colors);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);
        Set_Font_Spacing_Width(2);

        Print_Paragraph((UV_x_start_offset + UV_x_start + 9), (UV_y_start + 107), 147, building_description, 0);

/*
    END:  Buidling View
*/
    }
    else if( (View_Type == 1) || (View_Type == 2) )
    {
/*
    BEGIN:  Unit View
*/
        colors[0] = 197;
        colors[1] =  14;
        colors[2] = 255;
        colors[3] =  80;
        colors[4] = 255;
        Set_Font_Colors_15(4, &colors[0]);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);

        if((CalledFromFlag != 0) && (_UNITS[unit_idx].Hero_Slot != -1))
        {
            hero_owner_idx = _UNITS[unit_idx].owner_idx;
            FLIC_Draw((UV_x_start + 9), (UV_y_start + 7), IMG_USW_HeroPortrt);
            FLIC_Draw((UV_x_start + 8), (UV_y_start + 6), IMG_USW_Portrt_Brdr);
            Print((UV_x_start + 51), (UV_y_start + 6), _players[hero_owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Name);
            Print((UV_x_start + 51), (UV_y_start + 17), "The");
            Print((UV_x_start + 75), (UV_y_start + 17), *_unit_type_table[unit_type_idx].Name);
        }
        else
        {
            if(
                (global_strategic_unit->carry_capacity > 0) ||
                ((CalledFromFlag == uvt_Stat) && (Terrain_Is_Sailable(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp) == ST_TRUE))
            )
            {
                FLIC_Draw((UV_x_start_offset + UV_x_start + 6), (UV_y_start + 18), unit_water_diamond_seg);
                Draw_Unit_Figure((UV_x_start_offset + UV_x_start + 11), (UV_y_start - 3), unit_idx, CalledFromFlag);
            }
            else
            {
                /* Prod-Norm: on 'Production Screen', do 'Unit' */
                FLIC_Set_CurrentFrame(unit_grass_diamond_seg, 0);
                FLIC_Draw(( UV_x_start_offset + (UV_x_start + 5)), (UV_y_start + 5), unit_grass_diamond_seg);
                Draw_Unit_Figure((UV_x_start_offset + UV_x_start + 11), (UV_y_start - 3), unit_idx, CalledFromFlag);  // HERE: unit_idx = (thing_idx - 100)
            }
        }

        if(
            (CalledFromFlag == uvt_Prod) ||
            ((global_strategic_unit->Attribs_1 & 0x8000) == 0)  /* attr_RaceSpecificUnit */
        )
        {
            product_idx = unit_type_idx;
            // WTF? or      _DI_CalledFromFlag, _DI_CalledFromFlag
            // WTF? jz      short $+2
            bldg_pict_y1 = 0;
            if (CalledFromFlag == uvt_Prod)
            {
                bldg_pict_y1 = -4;
            }
            Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + bldg_pict_y1 + 11), *_unit_type_table[product_idx].Name);
        }
        else
        {
            unit_race = global_strategic_unit->race;
            bldg_pict_x1 = unit_type_idx;
            // if(unit_race < rt_Arcane) // TODO  figure out how the Dasm makes sense for race-specific, race 14 Standard - Trireme, Galley, Catapult, Warship
            if (unit_race < rt_Standard)
            {
                Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + 11), *_race_type_table[unit_race].name);
                product_idx = Get_String_Width(*_race_type_table[unit_race].name);
                Print((UV_x_start_offset + UV_x_start + product_idx + 56), (UV_y_start + 11), *_unit_type_table[bldg_pict_x1].Name);
            }
            else
            {
                Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + 11), _race_type_names[unit_race]);  // TODO  figures out the structure that would make the indexing in the Dasm make sense
                product_idx = Get_String_Width(_race_type_names[unit_race]);
                Print((UV_x_start_offset + UV_x_start + product_idx + 56), (UV_y_start + 11), *_unit_type_table[bldg_pict_x1].Name);
            }
        }
        colors[0] = 197;
        colors[1] = 255;
        Set_Font_Colors_15(1, &colors[0]);
        Set_Font_Style_Outline(1, 15, 0, 0);
        Set_Outline_Color(251);
        bldg_pict_y1 = 0;
        if(CalledFromFlag == uvt_Prod)
        {
            bldg_pict_y1 = -7;
        }
        Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + bldg_pict_y1 + 27), "Moves");
        Moves_Type_Icon(global_strategic_unit->Move_Flags, &moves_icon_idx);  // {11,12,13} {"walking", "fly", "swim"}
        bldg_pict_x1 = 0;
        for(itr = 0; (global_strategic_unit->movement_points / 2) > itr; itr++)
        {
            FLIC_Draw((UV_x_start_offset + UV_x_start + bldg_pict_x1 + 84), (UV_y_start + bldg_pict_y1 + 25), unitview_stat_icons_seg[moves_icon_idx]);
            bldg_pict_x1 += 8;
        }

        if(CalledFromFlag != uvt_Prod)
        {
            product_idx = unit_type_idx;
            // 'Hydra' manual override of figure count, because 'Cur_Figures' is actually 'Heads'
            if(spell_data_table[SPL_HYDRA].Param0 == product_idx)
            {
                Print((UV_x_start_offset + UV_x_start + 121), (UV_y_start + 27), "Heads");
                Print_Integer_Right((UV_x_start_offset + UV_x_start + 153), (UV_y_start + 27), global_strategic_unit->Cur_Figures);
            }
        }

        if(
            (CalledFromFlag == uvt_Prod) ||
            (CalledFromFlag == uvt_Stat)
        )
        {
            bldg_pict_y1 = 0;
            if(CalledFromFlag == uvt_Prod)
            {
                bldg_pict_y1 = -7;
            }
            Print((UV_x_start_offset + UV_x_start + 51), (UV_y_start + bldg_pict_y1 + 34), "Upkeep");
            if(CalledFromFlag == uvt_Prod)
            {
                UV_Draw_Upkeep_Icons__WIP((UV_x_start_offset + UV_x_start + 85), (UV_y_start + bldg_pict_y1 + 33), -(unit_type_idx));
            }
            else
            {
                UV_Draw_Upkeep_Icons__WIP((UV_x_start_offset + UV_x_start + 85), (UV_y_start + bldg_pict_y1 + 33), unit_idx);
            }
        }
        else
        {
            Print((UV_x_start + 51), (UV_y_start + 34), "Damage");
            Print_Integer_Right((UV_x_start + 90), (UV_y_start + 34), global_strategic_unit->TopFig_Dmg);
        }

        colors[0] = 197;
        colors[1] = 14;
        colors[2] = 255;
        colors[3] = 80;
        colors[4] = 255;

        Set_Font_Colors_15(4, &colors[0]);
        Set_Font_Style_Outline(4, 15, 0, 0);
        Set_Outline_Color(251);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 47), "Melee");
        Melee_Type_Icon(global_strategic_unit->Weapon_Plus1, &attack_icon_index);
        Icon_Row_Length = 15;
        if(CalledFromFlag == uvt_Prod)
        {
            Icon_Row_Length = 10;
        }
        UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 47), attack_icon_index, global_strategic_unit->melee, Icon_Row_Length, global_strategic_unit->Gold_Melee, global_strategic_unit->Grey_Melee);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 58), "Range");
        if(
            (global_strategic_unit->ranged_type != -1) &&
            (
                ((global_strategic_unit->ranged_type / 10) == 1) || /* RGrp_Boulder */
                ((global_strategic_unit->ranged_type / 10) == 2) || /* RGrp_Missile */
                ((global_strategic_unit->ranged_type / 10) == 3)    /* RGrp_Magic   */
            )
        )
        {
            Ranged_Type_Icon(global_strategic_unit->ranged_type, &attack_icon_index);
            UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 58), attack_icon_index, global_strategic_unit->ranged, Icon_Row_Length, global_strategic_unit->Gold_Ranged, global_strategic_unit->Grey_Ranged);
        }

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 68), "Armor");
        UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 68), 9, global_strategic_unit->defense, Icon_Row_Length, global_strategic_unit->Gold_Defense, global_strategic_unit->Grey_Defense);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 79), "Resist");
        UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 79), 14, global_strategic_unit->resist, Icon_Row_Length, global_strategic_unit->Gold_Resist, global_strategic_unit->Grey_Resist);

        Print((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 90), "Hits");
        UV_Draw_Hits_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 90), 10, global_strategic_unit->hits, global_strategic_unit->TopFig_Dmg, Icon_Row_Length, global_strategic_unit->Gold_Hits);

        USW_DrawListPage((UV_x_start_offset + UV_x_start + 8), (UV_y_start + 108), specials_list, specials_count, UV_ListItem, item_icon_pict_seg);

/*
    END:  Unit View
*/
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Thing_View_Draw__WIP()\n", __FILE__, __LINE__);
#endif
}


// WZD o72p06
// drake178: UNIT_DrawUpkeep()
void UV_Draw_Upkeep_Icons__WIP(int16_t xstart, int16_t ystart, int16_t unit_or_neg_type)
{
    int16_t mana_upkeep;
    int16_t gold_upkeep;
    int16_t total_upkeep;
    int16_t icon_x;
    int16_t icon_y;
    int16_t unit_idx;  // _SI_
    int16_t food_upkeep;  // _DI_

    unit_idx = unit_or_neg_type;

    if(unit_or_neg_type > -1)
    {

    }
    else
    {
        unit_idx = abs(unit_or_neg_type);
        gold_upkeep = _unit_type_table[unit_idx].Upkeep;
        mana_upkeep = 0;
        food_upkeep = 1;
    }

    total_upkeep = gold_upkeep + mana_upkeep + food_upkeep;

    icon_y = ystart;
    icon_x = xstart;

    Draw_Resource_Icons(&icon_x, icon_y, total_upkeep, gold_upkeep, city_big_resource_icon_seg[2], city_lil_resource_icon_seg[2]);

    Draw_Resource_Icons(&icon_x, icon_y, total_upkeep, mana_upkeep, city_big_resource_icon_seg[3], city_lil_resource_icon_seg[3]);

    Draw_Resource_Icons(&icon_x, icon_y, total_upkeep, food_upkeep, city_big_resource_icon_seg[0], city_lil_resource_icon_seg[0]);

}

// WZD o72p07
// drake178: USW_DrawStatIcons()
// TODO  figure what the Gold and Grey really and rename everything and properly document the process
// UV_Draw_Stat_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 47), attack_icon_index, global_strategic_unit->melee, Icon_Row_Length, global_strategic_unit->Gold_Melee, global_strategic_unit->Grey_Melee)
void UV_Draw_Stat_Icons(int16_t xstart, int16_t ystart, int16_t icon_idx, int16_t Attrib, int16_t row_length, int16_t Gold, int16_t Grey)
{
    int16_t total_icons;
    int16_t row2_length;
    int16_t icon_y;
    int16_t icon_x;  // _SI_
    int16_t itr;  // _DI_

    total_icons = Attrib + Grey;

    if(total_icons >= 1)
    {
        if((row_length * 2) < total_icons)
        {
            if((row_length * 2) < Attrib)
            {
                total_icons = (row_length * 2);
                Grey = 0;
            }
            else
            {
                Grey = ((row_length * 2) - Attrib);
                total_icons = (Attrib + Grey);
            }
        }

        Gold = (total_icons - Gold);
        Grey = (total_icons - Grey);

        icon_x = xstart;
        icon_y = ystart;

        if(total_icons <= row_length)
        {
            for(itr = 0; itr < total_icons; itr++)
            {
                if(itr < Gold)
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                }
                else
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                }

                if(itr >= Grey)
                {
                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                }

                icon_x += 10;

                if(((itr + 1) % 5) == 0)
                {
                    icon_x += 2;
                }
            }
        }
        else
        {
            row2_length = (total_icons - row_length);

            for(itr = 0; itr < row_length; itr++)
            {
                if(itr < Gold)
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                }
                else
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                }

                if(itr >= Grey)
                {
                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                }

                icon_x += 10;

                if(((itr + 1) % 5) != 0)
                {
                    icon_x += 2;
                }
            }

            icon_x = (xstart + 3);
            icon_y = (ystart + 2);

            for(itr = 0; itr < row2_length; itr++)
            {
                if(itr < Gold)
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                }
                else
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                }

                if(itr >= Grey)
                {
                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                }

                icon_x += 10;

                if(((itr + 1) % 5) != 0)
                {
                    icon_x += 2;
                }
            }
        }
    }

}


// WZD o72p08
// drake178: USW_DrawHitsIcons()
/*
UV_Draw_Hits_Icons((UV_x_start_offset + UV_x_start + 49), (UV_y_start + 90), 10, global_strategic_unit->hits, Icon_Row_Length, global_strategic_unit->TopFig_Dmg, global_strategic_unit->Gold_Hits);
global_strategic_unit->hits is copied from the unit_type_table
base hits?
includes level bonuses?
    improvements are added to the base attributes (before level raises) of the unit
haven't seen that yet, because it would apply to the 'Production Screen'?
the (bonus = total - bonus) must mean that the total already includes the bonus? so, it's definitely total, not base?

    ¿ vs. UV_Draw_Stat_Icons() ?
        ~ 3 icons  regular, bonus, damage
        total is base + bonus
        or base is total - bonus
        draw regular and/or bonus
        until undamaged count is reach
        then draws damaged regular and/or bonus

"damage hits against its total hits"
"darkened hearts"
"more of its hearts are darkened"

¿ where do bonus hits come from ?
*/
void UV_Draw_Hits_Icons(int16_t xstart, int16_t ystart, int16_t icon_idx, int16_t hits_total, int16_t hits_damaged, int16_t row_length, int16_t hits_bonus)
{
    int16_t row2_length;
    int16_t row1_length;
    int16_t hits_undamaged;
    int16_t icon_y;
    int16_t icon_x;  // _SI_
    int16_t itr;  // _DI_
    int16_t hits_base;  // DNE in Dasm

    hits_base = (hits_total - hits_bonus);

    if(hits_total >= 1)
    {
        if((row_length * 2) < hits_total)
        {
            hits_total = (row_length * 2);
        }

        icon_x = xstart;
        icon_y = ystart;

        if(hits_total <= row_length)
        {
            hits_undamaged = (hits_total - hits_damaged);

            for(itr = 0; itr < hits_total; itr++)
            {
                if(itr >= hits_base)
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                }
                else
                {
                    FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                }

                if(itr >= hits_undamaged)
                {
                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                }

                icon_x += 10;

                if(((itr + 1) % 5) == 0)
                {
                    icon_x += 2;
                }
            }
        }
        else
        {
            row1_length = row_length;
            row2_length = (hits_total - row_length);

            if(hits_damaged < row2_length)
            {
                for(itr = 0; itr < row_length; itr++)
                {
                    if(itr >= hits_base)
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                    }
                    else
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                    }

                    icon_x += 10;

                    if(((itr + 1) % 5) == 0)
                    {
                        icon_x += 2;
                    }
                }

                icon_x = (xstart + 3);
                icon_y = (ystart + 2);

                hits_undamaged = (row2_length - hits_damaged);

                for(itr = 0; itr < row2_length; itr++)
                {
                    if(itr >= (hits_base - row_length))
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                    }
                    else
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                    }

                    if(itr >= hits_undamaged)
                    {
                        Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                    }

                    icon_x += 10;

                    if(((itr + 1) % 5) == 0)
                    {
                        icon_x += 2;
                    }
                }
            }
            else  /* (hits_damaged >= row2_length) */
            {
                for(itr = 0; itr < row2_length; itr++)
                {
                    if(itr >= (hits_base - row_length))
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                    }
                    else
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                    }

                    Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed

                    icon_x += 10;

                    if(((itr + 1) % 5) == 0)
                    {
                        icon_x += 2;
                    }
                }

                icon_x = (xstart + 3);
                icon_y = (ystart + 2);

                hits_undamaged = (row1_length - (hits_damaged - row2_length));

                for(itr = 0; itr < row1_length; itr++)
                {
                    if(itr >= hits_base)
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_gold_icons_seg[icon_idx]);
                    }
                    else
                    {
                        FLIC_Draw(icon_x, icon_y, unitview_stat_icons_seg[icon_idx]);
                    }

                    if(itr >= hits_undamaged)
                    {
                        Gradient_Fill(icon_x, icon_y, (icon_x + 8), (icon_y + 7), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed
                    }

                    icon_x += 10;

                    if(((itr + 1) % 5) == 0)
                    {
                        icon_x += 2;
                    }
                }
            }
        }
    }

}

// WZD o72p09
// drake178: USW_GetBaseMoveIcon()
/*
    UnitView: Thing_View_Draw__WIP()
    UnitList: Draw_Unit_List_Window_Pup()

    Dasm looks like generated code
        tests 0x00, which is a no-op
        only tests one bit, in-order of types
*/
void Moves_Type_Icon(int16_t move_flags, int16_t * moves_icon_idx)
{
    int16_t icon_idx;  // _DI_

    icon_idx = 11;

    if((move_flags & MV_WALKING) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_CAVALRY) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_SAILING) != 0)
    {
        icon_idx = 13;
    }
    else if((move_flags & MV_FLYING) != 0)
    {
        icon_idx = 12;
    }
    else if((move_flags & MV_SWIMMING) != 0)
    {
        icon_idx = 13;
    }
    else if((move_flags & MV_TELEPORT) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_FORESTER) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_MOUNTAINEER) != 0)
    {
        icon_idx = 11;
    }
    else if((move_flags & MV_MERGING) != 0)
    {
        icon_idx = 11;
    }

    *moves_icon_idx = icon_idx;
}


// WZD o72p10
// drake178: USW_DrawListPage()
/*
    OON Thing_View_Draw__WIP()



*/
void USW_DrawListPage(int16_t xstart, int16_t ystart, struct s_UV_List * specials_list, int16_t specials_count, int16_t specials_item, SAMB_ptr item_icon_pict_seg)
{
    uint8_t colors[6];
    int16_t item_icon_y_start;
    int16_t item_icon_x_start;
    int16_t display_count;
    int16_t itr; // _SI_

    display_count = 8;

    if((specials_item + display_count) > specials_count)
    {
        display_count = (specials_count - specials_item);
    }

    if(
        (specials_item == 0) &&
        (UV_IsHeroUnit == ST_TRUE)
    )
    {
        display_count = 4;
    }

    colors[0] = 197;
    colors[1] = 255;
    colors[2] =  80;
    colors[3] = 255;

    Set_Font_Colors_15(2, &colors[0]);
    Set_Font_Style_Outline(2, 15, 0, 0);
    Set_Outline_Color(251);
    Set_Font_Spacing_Width(1);

    item_icon_y_start = ystart;

    for(itr = 0; itr < display_count; itr++)
    {
        item_icon_x_start = xstart;
        if(display_count > 4)
        {
            if(itr > 3)
            {
                item_icon_x_start += 107;
                if(itr == 4)
                {
                    item_icon_y_start = ystart;
                }
            }
        }

        if(specials_list->picts[(specials_item + itr)] != -1)
        {
            FLIC_Draw((item_icon_x_start - 1), (item_icon_y_start - 1), unitview_specials_borders_seg[itr]);

            if(specials_list->flags[(specials_item + itr)] == -99)
            {
                // SPECIAL.LBX, 003  UNICON1   item
                FLIC_Draw(item_icon_x_start, item_icon_y_start, special_seg[3]);
                // helps was set to the negative if the hero item slot was -1/invalid/empty so do 'slot' instead of 'item'
                if(specials_list->helps[(specials_item + itr)] > 0)
                {
                    ITEM_DrawIntoIMGSeg__WIP(specials_list->helps[(specials_item + itr)], item_icon_pict_seg);
                    Draw_Picture_Windowed((item_icon_x_start - 2), (item_icon_y_start - 2), item_icon_pict_seg);
                }
                else
                {
                    FLIC_Draw(item_icon_x_start, item_icon_y_start, (SAMB_ptr)specials_list->picts[(specials_item + itr)]);
                }
            }
            else
            {
                FLIC_Draw(item_icon_x_start, item_icon_y_start, (SAMB_ptr)specials_list->picts[(specials_item + itr)]);
            }
        }

        Print((item_icon_x_start + 19), (item_icon_y_start + 5), specials_list->names[(specials_item + itr)]);

        item_icon_y_start += 17;
    }

    Cycle_Item_Enchantment_Animation();

}


// WZD o72p11
// drake178: USW_CreateFXControls()
void UV_Add_Effect_Fields(int16_t xstart, int16_t ystart, struct s_UV_List * effect_list, int16_t effect_count, int16_t effect_item)
{
    int16_t ItemSlot_Index;
    int16_t xpos;
    int16_t Display_Count;
    int16_t itr;  // _SI_
    int16_t ypos;  // _DI_

    UV_Clear_Effect_Fields();

    Display_Count = 8;

    if((effect_item + Display_Count) > effect_count)
    {
        Display_Count = (effect_count - effect_item);
    }

    if((UV_ListItem == 0) && (UV_IsHeroUnit == 1))
    {
        Display_Count = 4;
    }

    UV_Add_UpDn_Buttons((xstart + 196), (ystart - 3));

    ItemSlot_Index = 0;

    ypos = ystart;

    for(itr = 0; itr < Display_Count; itr++)
    {
        xpos = xstart;
        
        if((Display_Count > 4) && (itr > 3))
        {
            xpos += 107;
            if(itr == 4)
            {
                ypos = ystart;
            }
        }

        if(effect_list->flags[(effect_item + itr)] == -99)  /* 'Special' is 'Item' */
        {
            USW_ItemSlot_IMGLbls[ItemSlot_Index] = Add_Picture_Field(xpos, ypos, (SAMB_ptr)effect_list->picts[(effect_item + itr)], 0, ST_UNDEFINED);
            USW_ItemSlot_ClkLbls[ItemSlot_Index] = Add_Hidden_Field((xpos + 19), (ypos + 5), (xpos + 93), (ypos + 13), 0, ST_UNDEFINED);
            ItemSlot_Index++;
        }
        else
        {
            USW_FX_ImageLabels[itr] = Add_Picture_Field(xpos, ypos, (SAMB_ptr)effect_list->picts[(effect_item + itr)], 0,  effect_list->helps[(effect_item + itr)]);
            USW_FX_ClickLabels[itr] = Add_Hidden_Field((xpos + 19), (ypos + 5), (xpos + 93), (ypos + 13), 0, effect_list->helps[(effect_item + itr)]);
            USW_ShownFX_Flags[itr] = effect_list->flags[(effect_item + itr)];
        }
        ypos += 17;
    }

}


// WZD o72p12
// drake178: USW_SetFXHelp()
// USW_SetFXHelp()

// WZD o72p13
// drake178: USW_CreateScrollBtns()
void UV_Add_UpDn_Buttons(int16_t xstart, int16_t ystart)
{
    unitview_up_button = INVALID_FIELD;
    unitview_dn_button = INVALID_FIELD;

    if(UV_IsMultiPage == ST_TRUE)
    {
        unitview_up_button = Add_Button_Field(xstart, ystart,        0, unitview_up_arrow_seg, 'U', ST_UNDEFINED);
        unitview_dn_button = Add_Button_Field(xstart, (ystart + 63), 0, unitview_up_arrow_seg, 'D', ST_UNDEFINED);
    }

}

// WZD o72p14
// drake178: USW_ClearFXControls()
void UV_Clear_Effect_Fields(void)
{
    int16_t itr;  // _AX_

    for(itr = 0; itr < 8; itr++)
    {
        USW_FX_ImageLabels[itr] = INVALID_FIELD;
        USW_FX_ClickLabels[itr] = INVALID_FIELD;
        USW_ShownFX_Flags[itr]  = INVALID_FIELD;
    }

    for(itr = 0; itr < 3; itr++)
    {
        USW_ItemSlot_IMGLbls[itr] = INVALID_FIELD;
        USW_ItemSlot_ClkLbls[itr] = INVALID_FIELD;
    }
    
}


// WZD o72p15
// drake178: USW_GetShownFXCount()
// USW_GetShownFXCount()

// WZD o72p16
// drake178: USW_GetWpnIconIndex()
void Melee_Type_Icon(int16_t melee_type, int16_t * icon_idx)
{
    if(melee_type > 0)
    {
        if(melee_type == 2)
        {
            *icon_idx = 3;
        }
        else if(melee_type == 3)
        {
            *icon_idx = 2;
        }
        else if(melee_type == 4)
        {
            *icon_idx = 4;
        }
        else
        {
            *icon_idx = 0;
        }
    }
    else
    {
        *icon_idx = 0;
    }
}


// WZD o72p17
// drake178: USW_GetRngIconIndex()
void Ranged_Type_Icon(int16_t ranged_type, int16_t * icon_idx)
{
    switch(ranged_type)
    {
        case rat_Rock:
        case rat_Cannon:
        case rat_Sling:
        case rat_Unknown:
        {
            *icon_idx = 6;
        } break;
        case rat_Lightning:
        case rat_Fireball:
        case rat_Sorcery:
        case rat_Deathbolt:
        case rat_Icebolt:
        case rat_Pr_Shaman:
        case rat_Drow:
        case rat_Sprite:
        case rat_Nat_Bolt:
        {
            *icon_idx = 1;
        } break;
        case srat_Thrown:
        {
            *icon_idx = 8;
        } break;
        case srat_FireBreath:
        case srat_Lightning:
        case srat_StoneGaze:
        {
            *icon_idx = 7;
        } break;
        default:
        {
            *icon_idx = 5;
        } break;
    }
}
// WZD o72p18
// drake178: USW_HireScreenPrep()
// USW_HireScreenPrep()

// WZD o72p19
void UV_Setup_ProdScr(int16_t specials_count)
{
    UV_ListItem = 0;

    UV_IsMultiPage = ST_FALSE;

    UV_specials_count = specials_count;

    UV_IsHeroUnit = ST_FALSE;

    USW_TransparentBase = ST_FALSE;

}



/*
    WIZARDS.EXE  ovr089
*/

// WZD o89p01
void Prod_Init_Strategic_Unit(int16_t unit_type, struct s_STRATEGIC_UNIT * strategic_unit)
{
    //MoO2  Module: struct  Copy_Structure_()
    memcpy(strategic_unit, &_unit_type_table[unit_type].Melee, sizeof(struct s_UNIT_TYPE));

    strategic_unit->TopFig_Dmg = 0;
    strategic_unit->Weapon_Plus1 = 0;
    strategic_unit->Gold_Melee = 0;
    strategic_unit->Gold_Ranged = 0;
    strategic_unit->Gold_Defense = 0;
    strategic_unit->Gold_Resist = 0;
    strategic_unit->Gold_Hits = 0;
    strategic_unit->Grey_Melee = 0;
    strategic_unit->Grey_Ranged = 0;
    strategic_unit->Grey_Defense = 0;
    strategic_unit->Grey_Resist = 0;

    strategic_unit->Poison_Strength = _unit_type_table[unit_type].Spec_Att_Attr;

}


// WZD o89p02
void USW_MemAlloc(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: USW_MemAlloc()\n", __FILE__, __LINE__);
#endif

    GUI_String_1 = (char *)Near_Allocate_First(100);  // 100 bytes
    UV_specials_list = (struct s_UV_List *)Near_Allocate_Next(1520);  // 1,520 bytes  40 * sizeof(s_UV_List)
    UV_item_icon_pict_seg = Allocate_Next_Block(_screen_seg, 30);  // 30 paragraphs, 480 bytes  19 x 19 pict

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: USW_MemAlloc()\n", __FILE__, __LINE__);
#endif

}

// WZD o89p03
/*
    loads a unit figure picture

*/
// void USW_LoadFigureImage(int16_t unit_idx, int16_t TypePass)
/*
    Loads a 'Unit Figure' entry into the screen_seg
    takes unit_idx or unit_type
    type_flag:  0 == unit_idx  1 == unit_type

USW_LoadAndShow() calls this with the 'Type Flag' unset
USW_LoadHireScreen calls this with the 'Type Flag' set
*/
void Load_Unit_Figure(int16_t type_or_unit, int16_t use_type)
{
    char file_name[20];
    char buffer[6];
    int16_t entry_num;

    int16_t figure_num;  // _SI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Load_Unit_Figure()\n", __FILE__, __LINE__);
#endif

    if(use_type != ST_TRUE)
    {
        figure_num = _UNITS[type_or_unit].type;  // got passed unit_idx
    }
    else
    {
        figure_num = type_or_unit;  // got passed unit_type
    }

    itoa(((figure_num / 15) + 1), buffer, 10);
    strcpy(file_name, "FIGURE");
    if( ((figure_num / 15) + 1) < 10)
    {
        strcat(file_name, "S");
    }
    strcat(file_name, buffer);
    strcat(file_name, ".LBX");

    entry_num = (((figure_num % 15) * 8) + 2);  // TODO  document this algo to the unit figure picture  (knows how many figures are in each figure lbx)

    unit_figure_seg = LBX_Reload_Next(file_name, entry_num, _screen_seg);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Load_Unit_Figure()\n", __FILE__, __LINE__);
#endif

}

// WZD o89p04

// WZD o89p05
// drake178: UNIT_Draw()
/*
    CalledFromFlag is passed from Thing_View_Draw__WIP()
    if CalledFromFlag == 0, assumes _human_player_idx
    if CalledFromFlag != 0, gets figure count from *combat data*, instead of _unit_type_table

*/
void Draw_Unit_Figure(int16_t x_start, int16_t y_start, int16_t unit_or_type, int16_t CalledFromFlag)
{
    int16_t fig_x;
    int16_t fig_y;
    int16_t unit_owner_banner_idx;
    int16_t unit_owner_idx;
    int16_t unit_type;
    int16_t UU_Highest_Fig_Index;
    int16_t unit_figure_count;
    int16_t itr_figure_count;

    int16_t itr_banner_colors;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Draw_Unit_Figure()\n", __FILE__, __LINE__);
#endif


    if(CalledFromFlag != uvt_Prod)
    {
        unit_type = _UNITS[unit_or_type].type;
        unit_figure_count = global_strategic_unit->Cur_Figures;
        unit_owner_idx = _UNITS[unit_or_type].owner_idx;
        unit_owner_banner_idx = _players[unit_owner_idx].banner_id;
        // 'Hydra' manual override of figure count, because 'Cur_Figures' is actually 'Heads'
        if(spell_data_table[SPL_HYDRA].Param0 == unit_type)
        {
            unit_figure_count = 1;
        }
    }
    else
    {
        unit_type = unit_or_type;
        unit_figure_count = _unit_type_table[unit_type].Figures;
        unit_owner_banner_idx = _players[_human_player_idx].banner_id;
    }

    UU_Highest_Fig_Index = (unit_figure_count - 1);

    for(itr_figure_count = 0; itr_figure_count < unit_figure_count; itr_figure_count++)
    {
        FLIC_Set_CurrentFrame(unit_figure_seg, 2);
        Draw_Picture_To_Bitmap(unit_figure_seg, UnitDraw_WorkArea);

        for(itr_banner_colors = 0; itr_banner_colors < 5; itr_banner_colors++)
        {
            // Draw_Map_Towers()
            // Replace_Color(Map_Square_WorkArea, 224 + itr_color_remap, *(COL_Banners + (_players[tower_owner_idx].banner_id * 5)));
            Replace_Color(UnitDraw_WorkArea, (214 + itr_banner_colors), COL_Banners[((unit_owner_banner_idx * 5) + itr_banner_colors)]);
        }

        if(CalledFromFlag == uvt_Prod)
        {
            if( (_unit_type_table[unit_type].Abilities & 0x40 /* Ab_Invisibility */) != 0)
            {
                Replace_Color_All(UnitDraw_WorkArea, 233);
            }
        }
        else
        {
            if(Unit_Has_Invisibility(unit_or_type) == ST_TRUE)
            {
                Replace_Color_All(UnitDraw_WorkArea, 233);
            }
            Draw_Unit_Enchantment_Outline(unit_or_type);
        }

        FLIC_Set_LoopFrame_1(UnitDraw_WorkArea);
        Unit_Figure_Position(unit_figure_count, itr_figure_count, &fig_y, &fig_x);
        Draw_Picture((x_start + fig_y), (y_start + fig_x), UnitDraw_WorkArea);

    }

    Cycle_Unit_Enchantment_Animation();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Draw_Unit_Figure()\n", __FILE__, __LINE__);
#endif

}

// WZD o89p06
// drake178: USW_GetFigPosition()
void Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * fig_x, int16_t * fig_y)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: USW_GetFigPosition()\n", __FILE__, __LINE__);
#endif

    assert(figure_count   >  0);
    assert(figure_count   <= 8);
    assert(current_figure >= 0);
    assert(current_figure <  8);

    switch(figure_count - 1)
    {
        case 0:
        {
            *fig_x = 1;
            *fig_y = 8;
        } break;
        case 1:
        {
            switch(current_figure)
            {
                case 0: { *fig_x = -8; *fig_y =  9; } break;
                case 1: { *fig_x =  8; *fig_y =  9; } break;
            }
        } break;
        case 2:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =  0; *fig_y =  4; } break;
                case 1: { *fig_x = -7; *fig_y = 10; } break;
                case 2: { *fig_x =  8; *fig_y = 10; } break;
            }
        } break;
        case 3:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =  1; *fig_y =  3; } break;
                case 1: { *fig_x = -9; *fig_y =  8; } break;
                case 2: { *fig_x = 11; *fig_y =  8; } break;
                case 3: { *fig_x =  1; *fig_y = 13; } break;
            }
        } break;
        case 4:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =  1; *fig_y =  3; } break;
                case 1: { *fig_x = -9; *fig_y =  8; } break;
                case 2: { *fig_x =  1; *fig_y =  8; } break;
                case 3: { *fig_x =  9; *fig_y =  8; } break;
                case 4: { *fig_x =  1; *fig_y = 14; } break;
            }
        } break;
        case 5:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =   1; *fig_y =  3; } break;
                case 1: { *fig_x =   4; *fig_y =  7; } break;
                case 2: { *fig_x = -10; *fig_y =  8; } break;
                case 3: { *fig_x =  10; *fig_y = 10; } break;
                case 4: { *fig_x =  -3; *fig_y = 11; } break;
                case 5: { *fig_x =   1; *fig_y = 15; } break;
            }
        } break;
        case 6:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =  1; *fig_y =  3; } break;
                case 1: { *fig_x =  6; *fig_y =  6; } break;
                case 2: { *fig_x = -9; *fig_y =  8; } break;
                case 3: { *fig_x =  1; *fig_y =  8; } break;
                case 4: { *fig_x = 10; *fig_y =  8; } break;
                case 5: { *fig_x = -3; *fig_y = 11; } break;
                case 6: { *fig_x =  1; *fig_y = 14; } break;
            }
        } break;
        case 7:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =   1; *fig_y =  3; } break;
                case 1: { *fig_x =   6; *fig_y =  6; } break;
                case 2: { *fig_x =  -2; *fig_y =  7; } break;
                case 3: { *fig_x = -10; *fig_y =  8; } break;
                case 4: { *fig_x =  10; *fig_y =  8; } break;
                case 5: { *fig_x =   3; *fig_y =  9; } break;
                case 6: { *fig_x =  -5; *fig_y = 11; } break;
                case 7: { *fig_x =   1; *fig_y = 14; } break;
            }
        } break;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: USW_GetFigPosition()\n", __FILE__, __LINE__);
#endif

}


// WZD o89p07
// UV_Remove_Unit_Enchantment

// WZD o89p08
// UNIT_RemoveEnchant

// WZD o89p09
// sub_759DE

// WZD o89p10
// UNIT_GetDependants
