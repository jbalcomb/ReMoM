/*
    Spellbook Screen

    WIZARDS.EXE
        ovr134
    
    SPELLSCR.LBX
*/

#include "MoM.H"
#include "SBookScr.H"
#include "Spellbook.H"
#include "MainScr.H"
#include "MainScr_Maps.H"




// WZD dseg:6994                                                 ¿ BEGIN: SPELLSCR || Spellbook Screen - Initialized Data ?

// WZD dseg:6994
char cnst_HLPENTRY_File6[] = "hlpentry";

// WZD dseg:699C 00                                              cnst_ZeroString_14 db 0                 ; DATA XREF: Spellbook_Screen+13Ao ...
// WZD dseg:699D 46 00                                           hotkey_F__4 db 'F',0                    ; DATA XREF: Spellbook_Screen+205o ...
// WZD dseg:699F 42 00                                           hotkey_B__5 db 'B',0                    ; DATA XREF: Spellbook_Screen+232o ...








/*
    WIZARDS.EXE  ovr134
*/

// WZD o134p01
void IDK_Create_SpellScreen_Background(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: IDK_Create_SpellScreen_Background()\n", __FILE__, __LINE__);
#endif

    Allocate_Reduced_Map();
    Set_Page_Off();
    Reset_Map_Draw();
    Main_Screen_Draw();
    Gradient_Fill(0, 0, 319, 199, 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);
    FLIC_Draw(16, 10, _spellbook_small_seg);
    Copy_Off_To_Back();
    Set_Page_On();
    LBX_Load_Data_Static(cnst_HLPENTRY_File6, 0, (SAMB_ptr)_help_entries, 0, 15, 10);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: IDK_Create_SpellScreen_Background()\n", __FILE__, __LINE__);
#endif
}


// WZD o134p02
void Spellbook_Screen_Draw(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Spellbook_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    OVL_DrawSpellbook(16, 10);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Spellbook_Screen_Draw()\n", __FILE__, __LINE__);
#endif
}


// WZD o134p03
void Spellbook_Screen(void)
{
// dst_ofst= byte ptr -44h
    // int16_t IDK_six_fields_1= word ptr -30h
    // int16_t IDK_six_fields_2= word ptr -24h
    int16_t hotkey_B;
    int16_t hotkey_F;
// Ctrl_Index= word ptr -14h
// Spell_Index= word ptr -12h
// Abort_Spell__YN= word ptr -10h
    int16_t hotkey_ESC;
    int16_t y_start;
// var_A= word ptr -0Ah
// var_8= word ptr -8
// var_6= word ptr -6
    int16_t input_field_idx;
    int16_t leave_screen;

    int16_t itr_spellbook_page_count;
    int16_t x_start;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Spellbook_Screen()\n", __FILE__, __LINE__);
#endif

    // var_A = 0;

    // TODO  OVL_DisableIncmBlink();

    Load_Palette_From_Animation(_spellbook_small_seg);
    Set_Palette_Changes(0, 223);
    Clear_Palette_Changes(224, 255);
    // TODO  Update_Remap_Gray_Palette();
    // TODO  Reset_Cycle_Palette_Color();

    IDK_Create_SpellScreen_Background();

// TODO     SBK_BuildSpellbook(0, 6);  // Book_Type, Page_Size
// TODO     SBK_Dogears = 1;
// TODO     var_8 = 0;
// TODO 
// TODO     for(itr_spellbook_page_count = 0; itr_spellbook_page_count < SBK_BookPageCount; itr_spellbook_page_count++)
// TODO     {
// TODO 
// TODO     }

    // TODO  Assign_Auto_Function(Spellbook_Screen_Draw(), 2);
    Set_Outline_Color(0);
    Set_Font_Style1(0, 3, 0, 0);
    Clear_Fields();
    Set_Help_List(_help_entries, 15);


    x_start = 16;
    y_start = 12;


    /*
        itr 2*6 spell fields
    */



    hotkey_ESC = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183, ST_UNDEFINED, ST_UNDEFINED);
    hotkey_F = Add_Hidden_Field(x_start + 259, y_start + 2, x_start + 272, y_start + 15, ST_UNDEFINED, ST_UNDEFINED);
    hotkey_B = Add_Hidden_Field(x_start + 13, y_start + 2, x_start + 26, y_start + 14, ST_UNDEFINED, ST_UNDEFINED);

    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        input_field_idx = Get_Input();

        /*
            itr 2*6 spell fields
        */

        if(input_field_idx == hotkey_ESC)
        {
            // TODO  RP_SND_LeftClickSound2();
            // leave_screen = ST_TRUE;
        }



        hotkey_ESC = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183, ST_UNDEFINED, ST_UNDEFINED);
        hotkey_F = Add_Hidden_Field(x_start + 259, y_start + 2, x_start + 272, y_start + 15, ST_UNDEFINED, ST_UNDEFINED);
        hotkey_B = Add_Hidden_Field(x_start + 13, y_start + 2, x_start + 26, y_start + 14, ST_UNDEFINED, ST_UNDEFINED);

        Set_Page_Off();
        Spellbook_Screen_Draw();
        PageFlip_FX();
    }

// @@LeaveScreen
    Deactivate_Auto_Function();
    current_screen = scr_Main_Screen;
    // TODO  SBK_SomePageSaveVar = SBK_OpenPage;
    Clear_Fields();

    // TODO  OVL_EnableIncmBlink();

    Deactivate_Help_List();

// TODO      if(var_A == ST_TRUE)
// TODO      {
// TODO          WIZ_SetOverlandSpell(0, Spell_Index, Ctrl_Index);
// TODO      }
// TODO      else
// TODO      {
// TODO          OVL_MosaicFlip();
// TODO      }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Spellbook_Screen()\n", __FILE__, __LINE__);
#endif
}


// WZD o134p04
// WZD o134p05
// WZD o134p06

// WZD o134p07
// drake178: OVL_MosaicFlip()
void OVL_MosaicFlip(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: OVL_MosaicFlip()\n", __FILE__, __LINE__);
#endif

    Set_Page_Off();
    Reset_Map_Draw();
    Main_Screen_Draw();
    // TODO  VGA_MosaicFlip();  |-> GUI_PageFlip();
    // WTF?  mov     ax, seg dseg
    // WTF?  mov     ds, ax
    // WTF?  mov     ax, [GUI_DS_Save]
    // WTF?  mov     ds, ax
    Check_Mouse_Shape(Pointer_X(), Pointer_Y());
    Save_Mouse_Off_Page(Pointer_X(), Pointer_Y());
    Draw_Mouse_Off_Page(Pointer_X(), Pointer_Y());
    Page_Flip();
    Restore_Mouse_Off_Page();
    Copy_Mouse_Off_To_Mouse_Back();
    Restore_Mouse_State();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: OVL_MosaicFlip()\n", __FILE__, __LINE__);
#endif
}

// WZD o134p08
// WZD o134p09
// WZD o134p10
// WZD o134p11
// WZD o134p12

