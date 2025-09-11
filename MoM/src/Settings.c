/*
    WIZARDS.EXE
        ovr125
    MAGIC.EXE

WZD
MGC

MoO2
    Module: FILEDEF
    Module: INITGAME
    Module: HAROLD
    Module: LOADSAVE
    Module: RUSS

*/

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/DOS.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/Timer.h"

#include "MOM_SCR.h"
#include "Spellbook.h"

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), frite(), fseek(); */
#include <string.h>     /* memcpy(), memset(), strcat(), strcpy(); */

#include "Settings.h"



/*
    LoadScr.C
*/
// WZD dseg:C9C4
extern SAMB_ptr ok_active;
// WZD dseg:C9CA
extern SAMB_ptr loadsave_background;



// WZD dseg:60D4 BEGIN:  ovr125 - Initialized Data  (Settings Screen)

// WZD dseg:60D4
struct s_mouse_list mouse_list_loadsave[1] = {
    {crsr_Finger, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX}
};

// WZD dseg:60E0
// ; 3 indicates returning from the settings screen
int16_t loadsave_settings_flag = ST_UNDEFINED;

// WZD dseg:60E2
int16_t TBL_Settings_Tops[13] = {
     53,    /*  0:  Sound Effects               Strategic Combat Only           */
     64,    /*  1:  Background Music            Auto Unit Information           */
     75,    /*  2:  Event Music                 Movement Animations             */
    102,    /*  3:  City Spell Events           Enemy Moves                     */
    113,    /*  4:  Overland Spell Events       Enemy Spells                    */
    124,    /*  5:  Summon Events               Spellbook Ordering              */
    135,    /*  6:  End of Turn Summary         Spell Animations                */
    153,    /*  7:  ¿ nothing ?                                                 */
    146,    /*  8:  Raze City                   Show Node Owners                */
    157,    /*  9:  N/A                         Expanding Help                  */
    167,    /* 10:                                      ¿ OK button border ?    */
    163,    /* 11:  Random Events                                               */
    174     /* 12:  End of Turn Wait                                            */
};

// WZD dseg:60FC
uint8_t COL_Settings[4] = {98, 98, 98, 98};

// WZD dseg:6100
char load_lbx_file__ovr125[] = "LOAD.LBX";

// WZD dseg:6108
char str_empty_string__ovr125[] = "";

// WZD dseg:6109
char settings_file__ovr125[] = "MAGIC.SET";

// WZD dseg:6113
char str_RB__ovr125[] = "rb";

// WZD dseg:6116
char str_hotkey_ESC__ovr125[] = "\x1B";

// WZD dseg:6118
char str_hotkey_O__ovr125[] = "O";

// WZD dseg:611A
char str_WB__ovr125[] = "wb";

// WZD dseg:611D
char cnst_Settings_0[] = "Sound Effects";
// WZD dseg:612B
char cnst_Settings_1[] = "Background Music";
// WZD dseg:613C
char cnst_Settings_2[] = "Event Music";
// WZD dseg:6148
char cnst_Settings_3[] = "City Spell Events";
// WZD dseg:615A
char cnst_Settings_4[] = "Overland Spell Events";
// WZD dseg:6170
char cnst_Settings_5[] = "Summoning Events";
// WZD dseg:6181
char cnst_Settings_6[] = "End of Turn Summary";
// WZD dseg:6195
char cnst_Settings_7[] = "Raze City";
// WZD dseg:619F
char cnst_Settings_8[] = "Random Events";
// WZD dseg:61AD
char cnst_Settings_9[] = "End of Turn Wait";
// WZD dseg:61BE
char cnst_Settings_A[] = "Strategic Combat Only";
// WZD dseg:61D4
char cnst_Settings_B[] = "Auto Unit Information";
// WZD dseg:61EA
char cnst_Settings_C[] = "Movement Animations";
// WZD dseg:61FE
char cnst_Settings_D[] = "Enemy Moves";
// WZD dseg:620A
char cnst_Settings_E[] = "Enemy Spells";
// WZD dseg:6217
char cnst_Settings_F[] = "Spell Book Ordering";
// WZD dseg:622B
char cnst_Settings_10[] = "Spell Animations";
// WZD dseg:623C
char cnst_Settings_11[] = "Show Node Owners";
// WZD dseg:624D
char cnst_Settings_12[] = "Expanding Help";

// WZD dseg:625C
char cnst_SettingsGrp_1[] = "Sound and Music";
// WZD dseg:626C
char cnst_SettingsGrp_2[] = "Messages";
// WZD dseg:6275
char cnst_SettingsGrp_3[] = "Combat";
// WZD dseg:627C
char cnst_SettingsGrp_4[] = "Display";

// WZD dseg:6284
char helpentry_lbx_file__ovr125[] = "hlpentry.lbx";

// WZD dseg:6291 align 2

// WZD dseg:6291 END:  ovr125 - Initialized Data  (Settings Screen)



// WZD dseg:C980 BEGIN:  ovr125 - Uninitialized Data  (Settings Screen)

// WZD dseg:C980 loadsave_settings_button dw 0
// WZD dseg:C982 dw 0
// WZD dseg:C984 dw 0

// WZD dseg:C986
SAMB_ptr IMG_GUI_ChkBoxDn[2];

// WZD dseg:C98A dw 0  // Eh? Maybe IMG_GUI_ChkBoxDn was a [3], like the two below?

// WZD dseg:C98C
SAMB_ptr IMG_GUI_ChkBoxNo[3];

// WZD dseg:C992
SAMB_ptr IMG_GUI_ChkBoxYes[3];

// WZD dseg:C998
int16_t * magic_set_ptr;

// WZD dseg:C99A save_inactive dw 0
// WZD dseg:C99C save_game_slots dw 8 dup(0)
// WZD dseg:C9AC loadsave_save_button dw 0
// WZD dseg:C9AE loadsave_quit_button dw 0

// WZD dseg:C9B0
int16_t loadsave_ok_button;

// WZD dseg:C9B2 selected_load_game_slot_idx dw 0
// WZD dseg:C9B4 selected_save_game_slot_idx dw 0
// WZD dseg:C9B6 loadsave_load_button dw 0

// WZD dseg:C9B8
int16_t load_screen_fade_in_flag;  // ; set to ST_FALSE on entering the settings screen

// WZD dseg:C9BA load_inactive dw 0
// WZD dseg:C9BC selection_marker dw 0
// WZD dseg:C9BE save_game_count dw 0
// WZD dseg:C9C0 settings_button dw 0
// WZD dseg:C9C2 loadsave_text_fill_seg dw 0
// WZD dseg:C9C4 ok_active dw 0
// WZD dseg:C9C6 load_active dw 0
// WZD dseg:C9C8 quit_active dw 0
// WZD dseg:C9CA loadsave_background dw 0

// WZD dseg:C9CA END:  ovr125 - Uninitialized Data  (Settings Screen)



void Read_MAGIC_SET(void)
{
    FILE * file_pointer;
    file_pointer = fopen("MAGIC.SET", "rb");
    fread(&_magic_set, 466, 1, file_pointer);
    fclose(file_pointer);
}

void Write_MAGIC_SET(void)
{
    FILE * file_pointer;
    file_pointer = fopen("MAGIC.SET", "wb");
    fwrite(&_magic_set, 466, 1, file_pointer);
    fclose(file_pointer);
}



/*
    WIZARDS.EXE  ovr125
*/

// WZD o125p01
void Settings_Screen(void)
{
    char found_file[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Local_Settings_Tops[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Option_Control_Indices[19] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FILE * file_pointer = 0;
    int16_t first_draw_done = 0;
    int16_t hotkey_ESC = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t itr = 0;  // _SI_
    int16_t row = 0;  // _DI_

// TODO  lea     ax, [bp+Local_Settings_Tops]
// TODO  push    ss
// TODO  push    ax                              ; Dest_Struct
// TODO  mov     ax, offset TBL_Settings_Tops
// TODO  push    ds
// TODO  push    ax                              ; Src_Struct
// TODO  mov     cx, 26
// TODO  call    SCOPY@
    memcpy(Local_Settings_Tops, TBL_Settings_Tops, 26);


    // LOAD.LBX, 011  SETTING2    Settings Backgrnd
    loadsave_background = LBX_Reload(load_lbx_file__ovr125, 11, _screen_seg);

    // LOAD.LBX, 004  LOADSAVE    Ok   active
    ok_active = LBX_Reload_Next(load_lbx_file__ovr125, 4, _screen_seg);

    // LOAD.LBX, 013  ICONS2      Check active 1
    // LOAD.LBX, 014  ICONS2      Check active 2
    // LOAD.LBX, 015  ICONS2      Check active 3
    for(itr = 0; itr < 3; itr++)
    {
        IMG_GUI_ChkBoxYes[itr] = LBX_Reload_Next(load_lbx_file__ovr125, (13 + itr), _screen_seg);
    }

    // LOAD.LBX, 016  ICONS2      Check inactive 1
    // LOAD.LBX, 017  ICONS2      Check inactive 2
    // LOAD.LBX, 018  ICONS2      Check inactive 3
    for(itr = 0; itr < 3; itr++)
    {
        IMG_GUI_ChkBoxNo[itr] = LBX_Reload_Next(load_lbx_file__ovr125, (16 + itr), _screen_seg);
    }

    // LOAD.LBX, 019  ICONS2      Check down 1
    // LOAD.LBX, 020  ICONS2      Check down 2
    for(itr = 0; itr < 2; itr++)
    {
        IMG_GUI_ChkBoxDn[itr] = LBX_Reload_Next(load_lbx_file__ovr125, (19 + itr), _screen_seg);
    }


    if(DIR(settings_file__ovr125, found_file) == ST_FAILURE)  // "MAGIC.SET"
    {
        Set_Default_Game_Settings();
    }
    else
    {
        file_pointer = fopen(settings_file__ovr125, str_RB__ovr125);  // "MAGIC.SET", "rb"
        // TODO  fwrite(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_handle);
        fwrite(&magic_set, 466, 1, file_pointer);
        fclose(file_pointer);
    }


    magic_set_ptr = (int16_t *)&magic_set;

    Set_Mouse_List(1, mouse_list_loadsave);

    Clear_Fields();

    hotkey_ESC = Add_Hot_Key(str_hotkey_ESC__ovr125[0]);

    loadsave_ok_button = Add_Hidden_Field(265, 175, 306, 189, str_hotkey_O__ovr125[0], ST_UNDEFINED);

    for(itr = 0; itr < 8; itr++)
    {
        // ¿ manually create gap between "Raze City" and "Random Events" ?
        row = itr;
        if(row == 7)
        {
            row = 8;  // nothing at 7, skip to 8 for Raze City
        }
        Option_Control_Indices[itr] = Add_Hidden_Field(31, Local_Settings_Tops[row], 175, (Local_Settings_Tops[row] + 9), str_empty_string__ovr125[0], ST_UNDEFINED);
    }

    // ¿ "Random Events" ?
    Option_Control_Indices[8] = Add_Hidden_Field(31, Local_Settings_Tops[11], 175, (Local_Settings_Tops[11] + 9), str_empty_string__ovr125[0], ST_UNDEFINED);

    // ¿ "End Of Turn Wait" ?
    Option_Control_Indices[9] = Add_Hidden_Field(31, Local_Settings_Tops[12], 175, (Local_Settings_Tops[12] + 9), str_empty_string__ovr125[0], ST_UNDEFINED);

    for(itr = 0; itr < 8; itr++)
    {
        row = itr;
        if(itr > 5)
        {
            row += 1;  // nothing at 7
        }
        if(itr > 1)
        {
            row += 1;  // nothing at 2 / skip Movement Animations
        }
        Option_Control_Indices[(10 + itr)] = Add_Hidden_Field(192, Local_Settings_Tops[row], 310, (Local_Settings_Tops[row] + 9), str_empty_string__ovr125[0], ST_UNDEFINED);
    }

    // Movement Animations
    Option_Control_Indices[18] = Add_Hidden_Field(192, 74, 310, (9 + 74), str_empty_string__ovr125[0], ST_UNDEFINED);


    leave_screen = ST_FALSE;

    first_draw_done = ST_FALSE;

    load_screen_fade_in_flag = ST_FALSE;

    Assign_Auto_Function(Settings_Screen_Draw, 1);

    Set_Settings_Screen_Help_List();


    while(leave_screen == ST_FALSE)
    {
        input_field_idx = Get_Input();

        Mark_Time();

        if(input_field_idx == hotkey_ESC)
        {
            leave_screen = ST_TRUE;
        }

        if(input_field_idx == loadsave_ok_button)
        {
            file_pointer = fopen(settings_file__ovr125, str_WB__ovr125);  // "MAGIC.SET", "wb"
            fwrite(&magic_set, 466, 1, file_pointer);
            fclose(file_pointer);
            leave_screen = ST_TRUE;
        }

        for(itr = 0; itr < 18; itr++)
        {
            if(Option_Control_Indices[itr] == input_field_idx)
            {
                if(magic_set_ptr[itr] == ST_FALSE)
                {
                    magic_set_ptr[itr] = ST_TRUE;
                }
                else
                {
                    magic_set_ptr[itr] = ST_FALSE;
                }
            }
            // Spell Book Ordering
            if(input_field_idx == Option_Control_Indices[14])
            {
                CMB_SpellBookPage = 0;
                SBK_SomePageSaveVar = 0;
                SBK_Candidate_Page = 0;
            }
        }

        if(input_field_idx == Option_Control_Indices[18])
        {
            magic_set.movement_animations = (1 - magic_set.movement_animations);
        }

        if(leave_screen == ST_FALSE)
        {
            Settings_Screen_Draw();
            Toggle_Pages();
            if(first_draw_done == ST_FALSE)
            {
                Copy_On_To_Off_Page();
                first_draw_done = ST_TRUE;
            }
            Release_Time(1);
        }
    }

    Deactivate_Auto_Function();

    Deactivate_Help_List();

    if(loadsave_settings_flag == 2)
    {
        current_screen = scr_Load_Screen;
    }

    loadsave_settings_flag = 3;

}


// WZD o125p02
void Settings_Screen_Draw(void)
{
    uint8_t * colors;
    int16_t Focused_Control;
    int16_t itr;  // _SI_
    int16_t IDK;  // _DI_

    colors = &COL_Settings[0];

    Focused_Control = Auto_Input();

    FLIC_Draw(0, 0, loadsave_background);

    Fill(265, 175, 306, 189, 0);

    Set_Font_Style_Shadow_Down(2, 2, 45, ST_NULL);

    IDK = 0;


    for(itr = 0; itr < 3; itr++)
    {
        if(magic_set_ptr[itr] == 0)
        {
            FLIC_Draw(31, (52 + (11 * itr)), IMG_GUI_ChkBoxNo[(IDK % 3)]);
        }
        else
        {
            FLIC_Draw(31, (52 + (11 * itr)), IMG_GUI_ChkBoxYes[(IDK % 3)]);
        }
        IDK++;
    }


    for(itr = 0; itr < 5; itr++)
    {
        if(magic_set_ptr[(3 + itr)] == 0)
        {
            FLIC_Draw(31, (101 + (11 * itr)), IMG_GUI_ChkBoxNo[(IDK % 3)]);
        }
        else
        {
            FLIC_Draw(31, (101 + (11 * itr)), IMG_GUI_ChkBoxYes[(IDK % 3)]);
        }
        IDK++;
    }


    if(magic_set_ptr[8] == 0)
    {
        FLIC_Draw(31, 163, IMG_GUI_ChkBoxNo[(IDK % 3)]);
    }
    else
    {
        FLIC_Draw(31, 163, IMG_GUI_ChkBoxYes[(IDK % 3)]);
    }

    IDK++;

    if(magic_set_ptr[9] == 0)
    {
        FLIC_Draw(31, 174, IMG_GUI_ChkBoxNo[(IDK % 3)]);
    }
    else
    {
        FLIC_Draw(31, 174, IMG_GUI_ChkBoxYes[(IDK % 3)]);
    }

    IDK++;


    // Startegic Combat Only, Auto Unit Information
    for(itr = 0; itr < 2; itr++)
    {
        if(magic_set_ptr[(10 + itr)] == 0)
        {
            FLIC_Draw(192, (52 + (11 * itr)), IMG_GUI_ChkBoxNo[(IDK % 3)]);
        }
        else
        {
            FLIC_Draw(192, (52 + (11 * itr)), IMG_GUI_ChkBoxYes[(IDK % 3)]);
        }
        IDK++;
    }

    // Enemy Moves, Enemy Spells, Spell Book Ordering, Spell Animations, Show Node Owners, Expanding Help
    for(itr = 0; itr < 6; itr++)
    {
        if(magic_set_ptr[(12 + itr)] == 0)
        {
            FLIC_Draw(192, (101 + (11 * itr)), IMG_GUI_ChkBoxNo[(IDK % 3)]);
        }
        else
        {
            FLIC_Draw(192, (101 + (11 * itr)), IMG_GUI_ChkBoxYes[(IDK % 3)]);
        }
        IDK++;
    }

    // Movement Animations
    if(magic_set.movement_animations == ST_FALSE)
    {
        FLIC_Draw(192, 74, IMG_GUI_ChkBoxNo[0]);
    }
    else
    {
        FLIC_Draw(192, 74, IMG_GUI_ChkBoxYes[0]);
    }


    if(Focused_Control == loadsave_ok_button)
    {
        FLIC_Draw(267, 177, ok_active);
    }
    else
    {
        FLIC_Draw(266, 176, ok_active);
    }


    Print( 47,  54, cnst_Settings_0);   // "Sound Effects"
    Print( 47,  65, cnst_Settings_1);   // "Background Music"
    Print( 47,  76, cnst_Settings_2);   // "Event Music"
    Print( 47, 103, cnst_Settings_3);   // "City Spell Events"
    Print( 47, 114, cnst_Settings_4);   // "Overland Spell Events"
    Print( 47, 125, cnst_Settings_5);   // "Summoning Events"
    Print( 47, 136, cnst_Settings_6);   // "End of Turn Summary"
    Print( 47, 147, cnst_Settings_7);   // "Raze City"
    Print( 47, 165, cnst_Settings_8);   // "Random Events"
    Print( 47, 176, cnst_Settings_9);   // "End of Turn Wait"

    Print(208,  54, cnst_Settings_A);   // "Strategic Combat Only"
    Print(208,  65, cnst_Settings_B);   // "Auto Unit Information"
    Print(208,  76, cnst_Settings_C);   // "Movement Animations"
    Print(208, 103, cnst_Settings_D);   // "Enemy Moves"
    Print(208, 114, cnst_Settings_E);   // "Enemy Spells"
    Print(208, 125, cnst_Settings_F);   // "Spell Book Ordering"
    Print(208, 136, cnst_Settings_10);  // "Spell Animations"
    Print(208, 147, cnst_Settings_11);  // "Show Node Owners"
    Print(208, 158, cnst_Settings_12);  // "Expanding Help"

    Set_Font_Colors_15(3, &colors[0]);
    Set_Font_Style_Shadow_Down(3, 15, 45, ST_NULL);

    Print(31, 42, cnst_SettingsGrp_1);  // "Sound and Music"
    Print(31, 92, cnst_SettingsGrp_2);  // "Messages"

    Print(192, 42, cnst_SettingsGrp_3);  // "Combat"
    Print(192, 92, cnst_SettingsGrp_4);  // "Display"

}


// WZD o125p03
void Load_MAGIC_SET(void)
{
    char found_file[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FILE * file_pointer = 0;
    int16_t itr = 0;

    if(
        (DIR("MAGIC.SET", found_file) == ST_FAILURE)
        ||
        (LOF("MAGIC.SET") != 466)
    )
    {
        STU_DEBUG_BREAK();
        Set_Default_Game_Settings();
        file_pointer = fopen("MAGIC.SET","wb");
        fwrite(&magic_set, 466, 1, file_pointer);
    }
    else
    {
        file_pointer = fopen("MAGIC.SET","rb");
        fread(&magic_set, 466, 1, file_pointer);
    }
    fclose(file_pointer);


    if(magic_set.movement_animations != ST_FALSE) { magic_set.movement_animations = ST_TRUE; }
    if(magic_set.raze_city != ST_FALSE) { magic_set.raze_city = ST_TRUE; }

    
    if(magic_set.hof_scores[0] == 0)
    {
        for(itr = 0; itr < NUM_HOF_ENTRIES; itr++)
        {
            magic_set.hof_scores[itr] = 0;
            magic_set.hof_races[itr] = 0;
            strcpy(magic_set.hof_names[itr], "");
        }
    }

    file_pointer = fopen("MAGIC.SET","wb");
    fwrite(&magic_set, 466, 1, file_pointer);
    fclose(file_pointer);

}


// WZD o125p04
void Set_Default_Game_Settings(void)
{
    int16_t itr;

    magic_set.sound_effects = ST_TRUE;
    magic_set.background_music = ST_TRUE;
    magic_set.event_music = ST_TRUE;
    magic_set.enemy_moves = ST_TRUE;
    magic_set.enemy_spells = ST_TRUE;
    magic_set.random_events = ST_TRUE;
    magic_set.end_of_turn_wait = ST_TRUE;
    magic_set.end_of_turn_summary = ST_TRUE;
    magic_set.raze_city = ST_TRUE;
    magic_set.spell_book_ordering = ST_TRUE;
    magic_set.strategic_combat_only = ST_FALSE;
    magic_set.spell_animations = ST_TRUE;
    magic_set.city_spell_events = ST_TRUE;
    magic_set.overland_spell_events = ST_TRUE;
    magic_set.summoning_events = ST_TRUE;
    magic_set.auto_unit_information = ST_TRUE;
    magic_set.show_node_owners = ST_TRUE;
    magic_set.expanding_help = ST_TRUE;
    magic_set.movement_animations = ST_TRUE;

    magic_set.Difficulty = god_Intro;
    magic_set.Opponents = goo_Four;
    magic_set.MagicPower = gom_Normal;
    magic_set.LandSize = gol_Medium;

    magic_set.input_type = 1;  // ¿ Keyboard & Mouse ?
    magic_set.sound_channels = 2;

    for(itr = 0; itr < NUM_SAVES; itr++)
    {
        magic_set.Have_Save[itr] = ST_FALSE;
    }

    for(itr = 0; itr < NUM_HOF_ENTRIES; itr++)
    {
        magic_set.hof_scores[itr] = 0;
        magic_set.hof_races[itr] = 0;
        strcpy(magic_set.hof_names[itr], "");
    }

}


// WZD o125p05
// HLPENTRY.LBX  "settings Help"
void Set_Settings_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 34, (SAMB_ptr)_help_entries, 0, 21, 10);
    Set_Help_List((char *)&_help_entries[0], 21);
}
