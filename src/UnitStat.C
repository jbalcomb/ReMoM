/*
    Unit Statistics Window
    
    WIZARDS.EXE
        ovr072
*/

#include "MoM.H"
#include "UnitStat.H"
#include "MainScr.H"    /* Draw_Unit_Enchantment_Outline(); Cycle_Unit_Enchantment_Animation(); */





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





// dseg:C176 00 00                                           
int16_t UV_InCombat;

// dseg:C178 00 00                                           
int16_t UV_IsHeroUnit;

// dseg:C17A
int16_t USW_TransparentBase;

// dseg:C17C
SAMB_ptr IMG_USW_UnitFigure;

// dseg:C17E
int16_t unitview_down_arrow_button;

// dseg:C180
int16_t unitview_up_arrow_button;

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
int16_t UV_Effect_Count;

// dseg:C192
SAMB_ptr UV_Effect_List;

// dseg:C192                                                                                         ; 1520 LBX_NearAlloc_Next bytes
// dseg:C194 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+USW_ShownFX_Flags dd 8 dup(0)           ; DATA XREF: UnitStatPup_Draw+4E7r ...
// dseg:C1B4 00 00 00 00 00 00                               USW_ItemSlot_ClkLbls dw 3 dup(0)        ; DATA XREF: UnitStatPup_Draw+32Cr ...
// dseg:C1BA 00 00 00 00 00 00                               USW_ItemSlot_IMGLbls dw 3 dup(0)        ; DATA XREF: UnitStatPup_Draw+31Dr ...
// dseg:C1C0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 USW_FX_ClickLabels dw 8 dup(0)          ; DATA XREF: UnitStatPup_Draw+4CEr ...
// dseg:C1D0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 USW_FX_ImageLabels dw 8 dup(0)          ; DATA XREF: UnitStatPup_Draw+4C1r ...

// dseg:C1E0
// ~== ITEM_ComposeBuffer@ for Item Screen
SAMB_ptr UV_item_icon_pict_seg;

// dseg:C1E2
int16_t UV_unit_idx;

// dseg:C1E4
int16_t UV_view_type;  // drake178: 2 if combat

// dseg:C1E6
int16_t UV_y_start;

// dseg:C1E8
int16_t UV_x_start;

// dseg:C1EA
// dseg:C1EA                                                 ¿ BEGIN: DIPLOMAC || Magic Screen - Uninitialized Data ?




/*
    WIZARDS.EXE  ovr072
*/

// WZD o072p01

// WZD o072p02
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
// ; UV_Effect_List@, and finishes with a 480 byte
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
        IMG_USW_HeroPortrt = LBX_Reload_Next("PORTRAIT", _unit_type_table[_UNITS[unit_idx].type].Bldng1_or_Portrait, _screen_seg);
    }
    else
    {
        // ; appends a figure image of the specified unit type
        // ; into the LBX_Sandbox_Seg - expects a unit type index
        // ; if TypePass is 1, or a unit index otherwise
        // ; returns the segment address for the image in
        // ; addition to setting it into IMG_USW_UnitFigure@
        USW_LoadFigureImage(unit_idx, 0);
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

// WZD o072p03
void Unit_Statistics_Popup(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t view_type, int16_t unit_idx)
{
    int16_t hotkey_idx_X;
    int16_t hotkey_idx_ESC;
// Item_Index= word ptr -10h
    int16_t screen_changed;
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

    UV_Effect_Count = 0;

    if( (view_type == 1) || (view_type == 2) )
    {
        // ; creates a unit statistics window effect list structure into the passed buffer, setting the element count into the return pointer
        // ; contains multiple BUGs that will either cause some effects to not show up, or the same thing to be noted two different ways
        // AKA USW_CreateFXList(UV_unit_idx, UV_Effect_List, &UV_Effect_Count);
        // TODO  UV_Build_Effect_List(UV_Unit_Index, &UV_Effect_List[0], &UV_Effect_Count)
    }


    UV_ListItem = 0;
    UV_IsMultiPage = ST_FALSE;
    UV_IsHeroUnit = ST_FALSE;


    if(_UNITS[UV_unit_idx].Hero_Slot != -1)
    {
        UV_IsHeroUnit = ST_TRUE;
    }
    
    if( (UV_IsHeroUnit == ST_TRUE) && (UV_Effect_Count > 4) )
    {
        UV_IsMultiPage = ST_TRUE;
    }

    if(UV_Effect_Count > 8)
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
    // TODO  USW_SetFXHelp(UV_Effect_List, UV_Effect_Count, 0);

    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();

// push    [UV_ListItem]            ; FirstRec
// push    [UV_Effect_Count]               ; Count
// push    [UV_Effect_List@]               ; FX@
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
        // TODO  USW_SetFXHelp([UV_Effect_List, UV_Effect_Count, 0)



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

// WZD o072p04
/*
; draws the unit statistics window into the current
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

    // ; draws the unit statistics window, recruitment, or production detail window into the current draw frame
    // ; BUG: draws the wrong movement type icons in certain situations (only uses ground, water, and air too)
    // ; BUG: the "Allows" list for buildings can contain incorrect information
    // ; BUG: fails to animate item slot 0
    // ; WARNING: not enough icons for top tier units
    Unit_Statistics_Popup_Draw__WIP(UV_x_start, UV_y_start, UV_view_type, UV_unit_idx, UV_Effect_List, UV_Effect_Count, UV_item_icon_pict_seg);

    if(UV_view_type != 2)
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

// WZD o072p05
/*
; draws the unit statistics window, recruitment, or
; production detail window into the current draw frame
;
; BUG: draws the wrong movement type icons in certain
;  situations (only uses ground, water, and air too)
; BUG: the "Allows" list for buildings can contain
;  incorrect information
; BUG: fails to animate item slot 0
; WARNING: not enough icons for top tier units
*/
void Unit_Statistics_Popup_Draw__WIP(int16_t x_start, int16_t y_start, int16_t ViewTypeFlag, int16_t unit_idx, SAMB_ptr BuildAllowList, int16_t EffectCount, SAMB_ptr ItemDrawSeg)
{
// Destn_Offset= byte ptr -5Ah
// Allows_List= word ptr -46h
// Building_Image@= word ptr -34h
// Object_IMG_Height= word ptr -32h
// Object_IMG_Width= word ptr -30h
// Object_IMG_Top= word ptr -2Eh
// Object_IMG_Left= word ptr -2Ch
    int16_t Unit_Type;
// Icon_Row_Length= word ptr -28h
    uint8_t colors[6];
    // Image_Top= word ptr -20h
// Image_Left= word ptr -1Eh
// Gold_Icons= word ptr -1Ch
// unit_race= word ptr -1Ah
    int16_t View_Type;
// Allows_Count= word ptr -16h
    int16_t Building_Type;
    int16_t Unit;
// Hero_Owner= word ptr -10h
// Prod_Cost= word ptr -0Eh
// Text_Top= word ptr -0Ch
// Text_Left= word ptr -0Ah
// Producing= word ptr -8
// Attack_Icon_Index= word ptr -6
// Allows_List_Size= word ptr -4
// Loop_Var= word ptr -2

    int16_t UV_x_start_offset;  // In the Dasm, _SI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Unit_Statistics_Popup_Draw__WIP()\n", __FILE__, __LINE__);
#endif

    UV_x_start = x_start;
    UV_y_start = y_start;

    Unit = unit_idx;
    Building_Type = unit_idx;

    Set_Page_Off();

    if(USW_TransparentBase == ST_TRUE)
    {
        Fill(UV_x_start, UV_y_start, (UV_x_start + 281), (UV_y_start + 183), 0);
    }

    if(ViewTypeFlag == 0)
    {
        FLIC_Draw(UV_x_start, UV_y_start, unitview_small_background_seg);
    }
    else
    {
        FLIC_Draw(UV_x_start, UV_y_start, unitview_large_background_seg);
        if(ViewTypeFlag != 2)
        {
            FLIC_Draw((UV_x_start + 213), (UV_y_start + 133), unitview_button_background_seg);
        }
    }



    View_Type = ViewTypeFlag;

    if(ViewTypeFlag == 0)
    {
        if(unit_idx >= 100)
        {
            unit_idx -= 100;
            Unit_Type = unit_idx;
            View_Type = 1;
        }
        else
        {
            View_Type = 0;
        }
    }
    else
    {
        Unit_Type = _UNITS[unit_idx].type;
    }
    // HERE: ViewTypeFlag is {0,1,2}; View_Type is {0,1,2}


    UV_IsHeroUnit = ST_FALSE;

    if(ViewTypeFlag != 0)
    {
        if(_UNITS[unit_idx].Hero_Slot != -1)
        {
            UV_IsHeroUnit = ST_TRUE;
        }
    }



    colors[0] = 197;
    colors[1] = 255;
    Set_Font_Colors_15(1, &colors[0]);
    Set_Font_Style_Outline(1, 15, 0, 0);
    Set_Outline_Color(251);


    UV_x_start_offset = 0;

    if(ViewTypeFlag == 0)
    {
        // Unit_Statistics_Popup_Draw+13C
        // TODO  On Production Screen - "Cost" Bldg || Unit
    }



    // Dasm show `if != 0,1,2 then fall-through`; probably was a switch statement
    if(View_Type == 0)
    {
        /*
            View Type 0: town building construction
            draw the building image, cost, maintenance cost,
            allowed construction list, and description
            contains minor BUGs related to the allowed list
        */
    }
    else
    {
        if( (View_Type == 1) || (View_Type == 2) )
        {
            colors[0] = 197;
            colors[1] =  14;
            colors[2] = 255;
            colors[3] =  80;
            colors[4] = 255;
            Set_Font_Colors_15(4, &colors[0]);
            Set_Font_Style_Outline(4, 15, 0, 0);
            Set_Outline_Color(251);


            /*
                if's
                    normal or hero
                        carry capacity && terrain is sailable ? water base : grass base


            */
            // HACK: hard-coded to assume unit view, nonhero unit
            FLIC_Set_CurrentFrame(IMG_USW_GrassBase, 0);
            FLIC_Draw(( UV_x_start_offset + (UV_x_start + 5)), (UV_y_start + 5), IMG_USW_GrassBase);
            Draw_Unit_Figure((UV_x_start_offset + (UV_x_start + 11)), (UV_y_start - 3), unit_idx, ViewTypeFlag);


        }
    }






#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Unit_Statistics_Popup_Draw__WIP()\n", __FILE__, __LINE__);
#endif
}

// WZD o072p06
// WZD o072p07
// WZD o072p08
// WZD o072p09
// WZD o072p10
// WZD o072p11
// WZD o072p12
// WZD o072p13
// WZD o072p14
// WZD o072p15
// WZD o072p16
// WZD o072p17
// WZD o072p18
// WZD o072p19




/*
    WIZARDS.EXE  ovr089
*/

// WZD s89p01

// WZD s89p02
void USW_MemAlloc(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: USW_MemAlloc()\n", __FILE__, __LINE__);
#endif

    GUI_String_1 = (char *)Near_Allocate_First(100);  // 100 bytes
    UV_Effect_List = Near_Allocate_Next(1520);  // 1,520 bytes
    UV_item_icon_pict_seg = Allocate_Next_Block(_screen_seg, 30);  // 30 paragraphs, 480 bytes  19 x 19 pict

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: USW_MemAlloc()\n", __FILE__, __LINE__);
#endif

}

// WZD s89p03
/*
    loads a unit figure picture

*/
void USW_LoadFigureImage(int16_t unit_idx, int16_t TypePass)
{
    char FileName[20];
    char Conversion_String[6];
    int16_t Entry_Index;

    int16_t FigureLbxNbr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: USW_LoadFigureImage()\n", __FILE__, __LINE__);
#endif

    if(TypePass != ST_TRUE)
    {
        FigureLbxNbr = _UNITS[unit_idx].type;
    }
    else
    {
        FigureLbxNbr = unit_idx;
    }

    itoa(((FigureLbxNbr % 15) + 1), Conversion_String, 10);
    strcpy(FileName, "FIGURE");
    if( ((FigureLbxNbr % 15) + 1) < 10)
    {
        strcat(FileName, "S");
    }
    strcat(FileName, Conversion_String);
    strcat(FileName, ".LBX");

    Entry_Index = (((FigureLbxNbr % 15) * 8) + 2);

    IMG_USW_UnitFigure = LBX_Reload_Next(FileName, Entry_Index, _screen_seg);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: USW_LoadFigureImage()\n", __FILE__, __LINE__);
#endif

}

// WZD s89p04

// WZD s89p05
// drake178: UNIT_Draw()
/*
    if VType == 0, assumes _human_player_idx
    if VType != 0, gets figure count from *combat data*, instead of _unit_type_table

*/
void Draw_Unit_Figure(int16_t x_start, int16_t y_start, int16_t unit_idx, int16_t VType)
{
    int16_t Fig_Top;
    int16_t Fig_Left;
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


    if(VType != 0)
    {
        unit_type = _UNITS[unit_idx].type;
        unit_figure_count = Active_Unit->Cur_Figures;
        unit_owner_idx = _UNITS[unit_idx].owner_idx;
        unit_owner_banner_idx = _players[unit_owner_idx].banner_id;

// TODO          // les     bx, [spell_data_table]          ; 215 records of 36 bytes
// TODO          // mov     ax, es:[bx+(Hydra*24h)+s_SPELL_DATA.Param0]
// TODO          //   AKA  mov     ax, [es:bx+0FBCh]
// TODO          //   AKA  mov     ax, [es:(_unit_type_table.pict_seg+0E10h)+bx]
// TODO          if(spell_data_table[SPL_HYDRA].Param0 == unit_type)
// TODO          {
// TODO              unit_figure_count = 1;
// TODO          }

    }
    else
    {
        unit_type = unit_idx;
        unit_figure_count = _unit_type_table[unit_type].Figures;
        unit_owner_banner_idx = _players[_human_player_idx].banner_id;
    }

    UU_Highest_Fig_Index = (unit_figure_count - 1);

    for(itr_figure_count = 0; itr_figure_count < unit_figure_count; itr_figure_count++)
    {
        FLIC_Set_CurrentFrame(IMG_USW_UnitFigure, 2);
        Draw_Picture_To_Bitmap(IMG_USW_UnitFigure, UnitDraw_WorkArea);
        for(itr_banner_colors = 0; itr_banner_colors < 5; itr_banner_colors++)
        {
            Replace_Color(UnitDraw_WorkArea, (214 + itr_banner_colors), (COL_Banners[unit_owner_banner_idx] + itr_banner_colors));
        }
        if(VType == 0)
        {
            if( (_unit_type_table[unit_type].Abilities & 0x40 /* Ab_Invisibility */) != 0)
            {
                Replace_Color_All(UnitDraw_WorkArea, 233);
            }
        }
        else
        {
            if(Unit_Has_Invisibility(unit_idx) == ST_TRUE)
            {
                Replace_Color_All(UnitDraw_WorkArea, 233);
            }
            Draw_Unit_Enchantment_Outline(unit_idx);
        }

        FLIC_Set_LoopFrame_1(UnitDraw_WorkArea);
        USW_GetFigPosition(unit_figure_count, itr_figure_count, &Fig_Left, &Fig_Top);
        Draw_Picture((x_start + Fig_Left), (y_start + Fig_Top), UnitDraw_WorkArea);

    }

    Cycle_Unit_Enchantment_Animation();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Draw_Unit_Figure()\n", __FILE__, __LINE__);
#endif

}

// WZD s89p06
void USW_GetFigPosition(int16_t figure_count, int16_t current_figure, int16_t * Fig_Left, int16_t * Fig_Top)
{
    int16_t figure_x;
    int16_t figure_y;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: USW_GetFigPosition()\n", __FILE__, __LINE__);
#endif
    assert(figure_count > 0);
    assert(figure_count <= 8);
    assert(current_figure >= 0);
    assert(current_figure < 8);

    switch(figure_count - 1)
    {
        case 0:
        {
            DLOG("switch(figure_count - 1)  case 0:");
            figure_x = 1;
            figure_y = 8;
        } break;
        case 1:
        {
            DLOG("switch(figure_count - 1)  case 1:");
            if(current_figure != 0)
            {
                figure_x = 8;
            }
            else
            {
                figure_x = -8;
            }
            figure_y = 9;
        } break;
        case 2:
        {
            DLOG("switch(figure_count - 1)  case 2:");
            if(current_figure != 0)
            {
                if(current_figure != 0)
                {
                    figure_x = 8;
                }
                else
                {
                    figure_x = -7;
                }
                figure_y = 10;
            }
            else
            {
                figure_x = 0;
                figure_y = 4;
            }
        } break;
        case 3:
        {
            DLOG("switch(figure_count - 1)  case 3:");
            figure_x = 1;
            figure_y = 8;

        } break;
        case 4:
        {
            DLOG("switch(figure_count - 1)  case 4:");
            figure_x = 1;
            figure_y = 8;

        } break;
        case 5:
        {
            DLOG("switch(figure_count - 1)  case 5:");
            figure_x = 1;
            figure_y = 8;

        } break;
        case 6:
        {
            DLOG("switch(figure_count - 1)  case 6:");
            figure_x = 1;
            figure_y = 8;

        } break;
        case 7:
        {
            DLOG("switch(figure_count - 1)  case 7:");
            figure_x = 1;
            figure_y = 8;

        } break;
    }

    *Fig_Left = figure_x;
    *Fig_Top = figure_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: USW_GetFigPosition()\n", __FILE__, __LINE__);
#endif

}


// WZD s89p07
// WZD s89p08
// WZD s89p09
