/*
    Hall of Fame Screen

    MAGIC.EXE
        ovr061
    WIZARDS.EXE
        ovr106
            Hall_Of_Fame_Screen()
            Hall_Of_Fame_Screen_Draw()
    
    WIZARDS.EXE
        ovr107
            End_Of_Game_Score()
            End_Of_Game_Score_Draw()

    MoO2:
        Module: SCORE
*/

#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/Help.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Util.h"
#include "../../MoX/src/Video.h"

#include "CITYCALC.h"
#include "Spellbook.h"

#include <stdlib.h>
#include <string.h>

#include "../../ext/stu_compat.h"

#include "SCORE.h"


// SPLMASTR.c
// WZD dseg:CA56                                                 BEGIN: ovr138 - Uninitialized Data
// WZD dseg:CA56
extern int16_t GAME_SoM_Cast_By;





// MGC dseg:3BEE 0A 3C 15 3C 1E 3C 29 3C 34 3C 3C 3C 43 3C 4D 3C+TBL_HoF_RaceStrings@ dw offset cnst_Race00,offset cnst_Race01,offset cnst_Race02,offset cnst_Race03,offset cnst_Race04,offset cnst_Race05,offset cnst_Race06,offset cnst_Race07,offset cnst_Race08,offset cnst_Race09,offset cnst_Race0A,offset cnst_Race0B,offset cnst_Race0C,offset cnst_Race0D
// MGC dseg:3BEE 58 3C 61 3C 6A 3C 74 3C 7B 3C 80 3C                                                     ; DATA XREF: Draw_Hall_Of_Fame_Screen+1CBr
// MGC dseg:3BEE                                                                                         ; "Barbarians"
// MGC dseg:3C0A 42 61 72 62 61 72 69 61 6E 73 00                cnst_Race00 db 'Barbarians',0           ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C15 42 65 61 73 74 6D 65 6E 00                      cnst_Race01 db 'Beastmen',0             ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C1E 44 61 72 6B 20 45 6C 76 65 73 00                cnst_Race02 db 'Dark Elves',0           ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C29 44 72 61 63 6F 6E 69 61 6E 73 00                cnst_Race03 db 'Draconians',0           ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C34 44 77 61 72 76 65 73 00                         cnst_Race04 db 'Dwarves',0              ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C3C 47 6E 6F 6C 6C 73 00                            cnst_Race05 db 'Gnolls',0               ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C43 48 61 6C 66 6C 69 6E 67 73 00                   cnst_Race06 db 'Halflings',0            ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C4D 48 69 67 68 20 45 6C 76 65 73 00                cnst_Race07 db 'High Elves',0           ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C58 48 69 67 68 20 4D 65 6E 00                      cnst_Race08 db 'High Men',0             ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C61 4B 6C 61 63 6B 6F 6E 73 00                      cnst_Race09 db 'Klackons',0             ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C6A 4C 69 7A 61 72 64 6D 65 6E 00                   cnst_Race0A db 'Lizardmen',0            ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C74 4E 6F 6D 61 64 73 00                            cnst_Race0B db 'Nomads',0               ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C7B 4F 72 63 73 00                                  cnst_Race0C db 'Orcs',0                 ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// MGC dseg:3C80 54 72 6F 6C 6C 73 00                            cnst_Race0D db 'Trolls',0               ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
char TBL_HoF_RaceStrings[NUM_RACES][LEN_RACE_NAME] =
{
    "Barbarians",
    "Beastmen",
    "Dark Elves",
    "Draconians",
    "Dwarves",
    "Gnolls",
    "Halflings",
    "High Elves",
    "High Men",
    "Klackons",
    "Lizardmen",
    "Nomads",
    "Orcs",
    "Trolls"
};

// MGC  dseg:3C80 54 72 6F 6C 6C 73 00                            cnst_Race0D db 'Trolls',0

/*
                                            BEGIN: ovr061  (Hall Of Fame)
*/

// MGC  dseg:3C87
char halofam_lbx_file__ovr106[] = "HALOFAM";

// MGC  dseg:3C8F
char str_hotkey_ESC__ovr106 = '\x1B';

// MGC  dseg:3C91
char str_SPACE__ovr106[] = " ";

// MGC  dseg:3C93
char cnst_HoF_String_B[] = "Hall";

// MGC  dseg:3C98
char cnst_Of[] = "Of";

// MGC  dseg:3C9B
char cnst_Fame[] = "Fame";

// MGC  dseg:3CA0
char cnst_HoF_String_C[] = "Master";

// MGC  dseg:3CA7
char str_PAREN_CLOSE[] = ")";

// MGC  dseg:3CA9
char cnst_HoF_String_D[] = " of the ";

// MGC  dseg:3CB2
char str_PAREN_OPEN[] = "(";

// MGC  dseg:3CB4
char cnst_HoF_String_E[] = "%)";

// MGC  dseg:3CB7 00                                              align 2

/*
                                            END: ovr061  (Hall Of Fame)
*/



// WZD dseg:59B4                                                 BEGIN:  ovr106 - Initialized Data

// WZD dseg:59B4 D0 59 DB 59 E4 59 EF 59 FA 59 02 5A 09 5A 13 5A+TBL_HoF_RaceStrings@ dw offset cnst_Race00, offset cnst_Race01, offset cnst_Race02, offset cnst_Race03, offset cnst_Race04, offset cnst_Race05, offset cnst_Race06, offset cnst_Race07, offset cnst_Race08, offset cnst_Race09, offset cnst_Race0A, offset cnst_Race0B, offset cnst_Race0C
// WZD dseg:59B4 1E 5A 27 5A 30 5A 3A 5A 41 5A 46 5A                                                     ; DATA XREF: Hall_Of_Fame_Screen_Draw+1CBr
// WZD dseg:59B4                                                 dw offset cnst_Race0D                   ; "Barbarians"
// WZD dseg:59D0 42 61 72 62 61 72 69 61 6E 73 00                cnst_Race00 db 'Barbarians',0           ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:59DB 42 65 61 73 74 6D 65 6E 00                      cnst_Race01 db 'Beastmen',0             ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:59E4 44 61 72 6B 20 45 6C 76 65 73 00                cnst_Race02 db 'Dark Elves',0           ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:59EF 44 72 61 63 6F 6E 69 61 6E 73 00                cnst_Race03 db 'Draconians',0           ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:59FA 44 77 61 72 76 65 73 00                         cnst_Race04 db 'Dwarves',0              ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A02 47 6E 6F 6C 6C 73 00                            cnst_Race05 db 'Gnolls',0               ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A09 48 61 6C 66 6C 69 6E 67 73 00                   cnst_Race06 db 'Halflings',0            ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A13 48 69 67 68 20 45 6C 76 65 73 00                cnst_Race07 db 'High Elves',0           ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A1E 48 69 67 68 20 4D 65 6E 00                      cnst_Race08 db 'High Men',0             ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A27 4B 6C 61 63 6B 6F 6E 73 00                      cnst_Race09 db 'Klackons',0             ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A30 4C 69 7A 61 72 64 6D 65 6E 00                   cnst_Race0A db 'Lizardmen',0            ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A3A 4E 6F 6D 61 64 73 00                            cnst_Race0B db 'Nomads',0               ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A41 4F 72 63 73 00                                  cnst_Race0C db 'Orcs',0                 ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A46 54 72 6F 6C 6C 73 00                            cnst_Race0D db 'Trolls',0               ; DATA XREF: dseg:TBL_HoF_RaceStrings@o
// WZD dseg:5A4D 48 41 4C 4F 46 41 4D 00                         hof_lbx_file db 'HALOFAM',0             ; DATA XREF: Hall_Of_Fame_Screen+54o
// WZD dseg:5A55 1B 00                                           cnst_HOTKEY_Esc14 db 1Bh,0              ; DATA XREF: Hall_Of_Fame_Screen+C8o
// WZD dseg:5A55                                                                                         ; should use dseg:2c56
// WZD dseg:5A57 20 00                                           cnst_HoF_Space db ' ',0                 ; DATA XREF: Hall_Of_Fame_Screen_Draw:loc_86C0Ao
// WZD dseg:5A57                                                                                         ; (could use dseg:2ad9)
// WZD dseg:5A59 48 61 6C 6C 00                                  cnst_HoF_String_B db 'Hall',0           ; DATA XREF: Hall_Of_Fame_Screen_Draw+95o
// WZD dseg:5A5E 4F 66 00                                        cnst_Of db 'Of',0                       ; DATA XREF: Hall_Of_Fame_Screen_Draw+B3o
// WZD dseg:5A61 46 61 6D 65 00                                  cnst_Fame_2 db 'Fame',0                 ; DATA XREF: Hall_Of_Fame_Screen_Draw+D1o
// WZD dseg:5A61                                                                                         ; should use dseg:367b
// WZD dseg:5A66 4D 61 73 74 65 72 00                            cnst_HoF_String_C db 'Master',0         ; DATA XREF: Hall_Of_Fame_Screen_Draw+154o
// WZD dseg:5A6D 29 00                                           cnst_ClosingBrace_5 db ')',0            ; DATA XREF: Hall_Of_Fame_Screen_Draw+17Do
// WZD dseg:5A6D                                                                                         ; should use dseg:3428
// WZD dseg:5A6F 20 6F 66 20 74 68 65 20 00                      cnst_HoF_String_D db ' of the ',0       ; DATA XREF: Hall_Of_Fame_Screen_Draw+1B2o
// WZD dseg:5A78 28 00                                           cnst_OpeningBrace_3 db '(',0            ; DATA XREF: Hall_Of_Fame_Screen_Draw+241o
// WZD dseg:5A78                                                                                         ; should use dseg:3426
// WZD dseg:5A7A 25 29 00                                        cnst_HoF_String_E db '%)',0             ; DATA XREF: Hall_Of_Fame_Screen_Draw+25Fo

// WZD dseg:5A7D 00                                              align 2

// WZD dseg:5A7D                                                 END:  ovr106 - Initialized Data



// WZD dseg:5A7E                                                 BEGIN:  ovr107 - Initialized Data

// WZD dseg:5A7E
char halofam_lbx_file__ovr107[] = "HALOFAM";
// WZD dseg:5A86
char str_hotkey_ESC__ovr107[] = "\x1B";
// WZD dseg:5A88
char cnst_Set_File3[] = "MAGIC.SET";
// WZD dseg:5A92
char cnst_WB3[] = "wb";
// WZD dseg:5A95
char cnst_HoF_String_0[] = ": ";
// WZD dseg:5A97
char cnst_HoF_String_1[] = "+";
// WZD dseg:5A99
char str_SPACE__ovr107[] = " ";
// WZD dseg:5A9B
char cnst_HoF_String_2[] = "Spells";
// WZD dseg:5AA2
char cnst_HoF_String_3[] = "Townsfolk";
// WZD dseg:5AAC
char cnst_HoF_String_4[] = "No";
// WZD dseg:5AAF
char cnst_HoF_String_5[] = "Banished";
// WZD dseg:5AB8
char cnst_HoF_String_6[] = "Wizards";
// WZD dseg:5AC0
char cnst_Fame_3[] = "Fame";
// WZD dseg:5AC5
char cnst_HoF_String_7[] = "Year";
// WZD dseg:5ACA
char cnst_HoF_String_8[] = ":";
// WZD dseg:5ACC
char cnst_Spell_3[] = "Spell";
// WZD dseg:5AD2
char cnst_Of_2[] = "Of";
// WZD dseg:5AD5
char cnst_HoF_String_9[] = "Mastery";
// WZD dseg:5ADD
char cnst_HoF_String_A[] = "Final Score:";

// WZD dseg:5ADD                                                 END:  ovr107 - Initialized Data



// WZD dseg:C7E6                                                 BEGIN:  ovr106 - Uninitialized Data
// DNE
// WZD dseg:C7E8                                                 END:  ovr106 - Uninitialized Data



// WZD dseg:C7E8                                                 BEGIN:  ovr107 - Uninitialized Data

// WZD dseg:C7E8
int16_t m_new_score;
// WZD dseg:C7EA
SAMB_ptr IMG_GAME_HoF_SoM;
// WZD dseg:C7EC
SAMB_ptr IMG_GAME_HoF_Fame;
// WZD dseg:C7EE
SAMB_ptr hof_banished_seg[4];
// WZD dseg:C7F6
SAMB_ptr IMG_GAME_HoF_Time;
// WZD dseg:C7F8
SAMB_ptr IMG_GAME_HoF_Book;
// WZD dseg:C7FA
SAMB_ptr IMG_GAME_HoF_Pop;

// WZD dseg:C7FA                                                 END:  ovr107 - Uninitialized Data









// MGC o061p01
// WZD o106p01
void Hall_Of_Fame_Screen(void)
{
    int16_t full_screen_esc = 0;
    int16_t input_field_idx = 0;
    SAMB_ptr hof_background_seg = ST_NULL;
    int16_t leave_screen_flag = 0;
    Deactivate_Auto_Function();
    Set_Mouse_List(1, mouse_list_none);
    Clear_Fields();
    Fade_Out();
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0);
    Toggle_Pages();
    Set_Page_Off();
    // HALOFAM.LBX, 000  HALOFAM     hof background
    hof_background_seg = LBX_Reload(halofam_lbx_file__ovr106, 0, _screen_seg);
    FLIC_Draw(0, 0, hof_background_seg);
    Apply_Palette();
    Copy_Off_To_Back();
    Copy_Off_To_Page4();
    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);
    Deactivate_Help_List();
    Assign_Auto_Function(Hall_Of_Fame_Screen_Draw, 1);
    leave_screen_flag = ST_FALSE;
    while(leave_screen_flag == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();
        full_screen_esc = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr106, ST_UNDEFINED);
        input_field_idx = Get_Input();
        if(input_field_idx == full_screen_esc)
        {
            leave_screen_flag = ST_UNDEFINED;
        }
        if(leave_screen_flag == ST_FALSE)
        {
            Hall_Of_Fame_Screen_Draw();
            PageFlip_FX();
            Release_Time(2);
        }
    }
    Clear_Fields();
    Reset_Window();
    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Fade_Out();
    Load_Palette(0, ST_UNDEFINED, ST_NULL);
    Reset_Cycle_Palette_Color();
    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    Calculate_Remap_Colors();
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Toggle_Pages();
    Apply_Palette();
    Set_Mouse_List(1, mouse_list_default);
}

// MGC o061p02
// WZD o106p02
void Hall_Of_Fame_Screen_Draw(void)
{
    uint8_t color_array[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t score_percent = 0;
    char str_SPACE[2] = { 0, 0 };
    int16_t print_x = 0;
    int16_t print_y = 0;
    int16_t itr_color_array = 0;
    int16_t itr_scores = 0;
    stu_strcpy(str_SPACE, str_SPACE__ovr106);
    color_array[ 0] = 246;
    color_array[ 1] =  49;
    color_array[ 2] = 176;
    color_array[ 3] = 162;
    color_array[ 4] = 178;
    color_array[ 5] = 179;
    color_array[ 6] =  51;
    color_array[ 7] = 180;
    color_array[ 8] = 184;
    color_array[ 9] = 185;
    color_array[10] = 213;
    color_array[11] = 186;
    color_array[12] = 246;
    color_array[13] = 246;
    color_array[14] = 246;
    color_array[15] = 246;
    Set_Page_Off();
    Reset_Window();
    Copy_Back_To_Off();
    Set_Font_Colors_15(2, &color_array[0]);
    Set_Font_Style_Shadow_Down(5, 15, 0, 0);
    Set_Outline_Color(250);
    stu_strcpy(GUI_String_1, cnst_HoF_String_B);  // "Hall"
    stu_strcat(GUI_String_1, str_SPACE);          // " "
    stu_strcat(GUI_String_1, cnst_Of);            // "Of"
    stu_strcat(GUI_String_1, str_SPACE);          // " "
    stu_strcat(GUI_String_1, cnst_Fame);          // "Fame"
    Print_Centered(160, 7, GUI_String_1);
    for(itr_color_array = 0; itr_color_array < 16; itr_color_array++)
    {
        color_array[itr_color_array] = 179;
    }
    color_array[0] = 246;
    Set_Font_Colors_15(2, &color_array[0]);
    Set_Font_Style_Shadow_Down(2, 15, 0, 0);
    Set_Outline_Color(250);
    print_x = 72;
    print_y = 42;
    for(itr_scores = 0; itr_scores < NUM_SCORES; itr_scores++)
    {
        if(magic_set.hof_scores[itr_scores] != ST_FALSE)
        {
            if(itr_scores == 0)
            {
                Print_Right(print_x, print_y, cnst_HoF_String_C);  // "Master"
            }
            else
            {
                stu_itoa((itr_scores+1), GUI_String_2, 10);
                stu_strcpy(GUI_String_1, GUI_String_2);
                stu_strcat(GUI_String_1, str_PAREN_CLOSE);  // ")"
                Print_Right(print_x, print_y, GUI_String_1);
            }
            stu_strcpy(GUI_String_1, magic_set.hof_names[itr_scores]);
            stu_strcat(GUI_String_1, cnst_HoF_String_D);  // " of the "
            stu_strcat(GUI_String_1, TBL_HoF_RaceStrings[magic_set.hof_races[itr_scores]]);
            Print((print_x + 9), print_y, GUI_String_1);  // ¿ 9 is ~ x offset for title column ?
            Print_Integer_Right((print_x + 170), print_y, magic_set.hof_scores[itr_scores]);  // ¿ 9 is ~ x offset for score column ?
            score_percent = (((int32_t)magic_set.hof_scores[itr_scores] * 100) / 8000);
            stu_itoa(score_percent, GUI_String_2, 10);
            stu_strcpy(GUI_String_1, str_PAREN_OPEN);  // "("
            stu_strcat(GUI_String_1, GUI_String_2);
            stu_strcat(GUI_String_1, cnst_HoF_String_E);  // "%)"
            Print_Right((print_x + 198), print_y, GUI_String_1);  // ¿ 198 is ~ x offset for  column ?
            print_y += 13;
        }
    }
}



/*
    WIZARDS.EXE 107
*/

// WZD o107p01
// MoO2  Module: SCORE  End_Of_Game_Hi_Score_()
void End_Of_Game_Score(void)
{
    FILE * fp;
    int conquests_ctr;
    int full_screen_esc;
    int input_field_idx;
    int leave_screen;
    void * hof_flic;
    int itr;
    int itr2;
    m_new_score = 0;
    Stop_All_Sounds__STUB();
    Set_Mouse_List(1, mouse_list_none);
    Deactivate_Auto_Function();
    Clear_Fields();
    Fade_Out();
    Set_Page_Off();
    Fill(0, 0, 319, 199, 0);
    Toggle_Pages();
    Set_Page_Off();
    // HALOFAM, 000  "HALOFAM"  "hof background"
    hof_flic = LBX_Reload(halofam_lbx_file__ovr107, 0, _screen_seg);
    FLIC_Draw(0, 0, hof_flic);
    Apply_Palette();
    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 255);
    Calculate_Remap_Colors();
    Copy_Off_To_Back();
    Copy_Off_To_Page4();
    Set_Mouse_List(1, mouse_list_default);
    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);
    // HALOFAM, 029  "HOFPIX"   "hourglass"
    // HALOFAM, 030  "HOFPIX"   "spell book"
    // HALOFAM, 031  "HOFPIX"   "fame"
    // HALOFAM, 032  "HOFPIX"   "spell master"
    IMG_GAME_HoF_Time = LBX_Reload(halofam_lbx_file__ovr107, 29, _screen_seg);
    IMG_GAME_HoF_Book = LBX_Reload_Next(halofam_lbx_file__ovr107, 30, _screen_seg);
    IMG_GAME_HoF_Fame = LBX_Reload_Next(halofam_lbx_file__ovr107, 31, _screen_seg);
    IMG_GAME_HoF_SoM = LBX_Reload_Next(halofam_lbx_file__ovr107, 32, _screen_seg);
    // HALOFAM, 015  "HOFPIX"   ""
    // HALOFAM, 016  "HOFPIX"   ""
    // HALOFAM, 017  "HOFPIX"   ""
    // HALOFAM, 018  "HOFPIX"   ""
    // HALOFAM, 019  "HOFPIX"   ""
    // HALOFAM, 020  "HOFPIX"   ""
    // HALOFAM, 021  "HOFPIX"   ""
    // HALOFAM, 022  "HOFPIX"   ""
    // HALOFAM, 023  "HOFPIX"   ""
    // HALOFAM, 024  "HOFPIX"   ""
    // HALOFAM, 025  "HOFPIX"   ""
    // HALOFAM, 026  "HOFPIX"   ""
    // HALOFAM, 027  "HOFPIX"   ""
    // HALOFAM, 028  "HOFPIX"   ""
    IMG_GAME_HoF_Pop = LBX_Reload_Next(halofam_lbx_file__ovr107, (15 + _players[_human_player_idx].capital_race), _screen_seg);
    Assign_Auto_Function(End_Of_Game_Score_Draw, 2);
    /* If SoM Win, make all other Wizards as Banished */
    if(GAME_SoM_Cast_By == _human_player_idx)
    {
        for(itr = 1; itr < _num_players; itr++)
        {
            if(_FORTRESSES[itr].active == ST_TRUE)
            {
                Set_Bit_Field_Near(itr, (char *)&_players[_human_player_idx].Defeated_Wizards);
            }
        }
    }
    conquests_ctr = 0;
    for(itr = 0; itr < _num_players; itr++)
    {
        if(Test_Bit_Field_Near(itr, (char *)&_players[_human_player_idx].Defeated_Wizards) == ST_TRUE)
        {
            // HALOFAM, 001  "HOFPIX"   "merlin"
            // HALOFAM, 002  "HOFPIX"   "shaman"
            // HALOFAM, 003  "HOFPIX"   "sharee"
            // HALOFAM, 004  "HOFPIX"   "lopan"
            // HALOFAM, 005  "HOFPIX"   "arab"
            // HALOFAM, 006  "HOFPIX"   "oberic"
            // HALOFAM, 007  "HOFPIX"   "rjak"
            // HALOFAM, 008  "HOFPIX"   "ssra"
            // HALOFAM, 009  "HOFPIX"   "baron"
            // HALOFAM, 010  "HOFPIX"   "freya"
            // HALOFAM, 011  "HOFPIX"   "galen"
            // HALOFAM, 012  "HOFPIX"   "ariel"
            // HALOFAM, 013  "HOFPIX"   "aztec"
            // HALOFAM, 014  "HOFPIX"   "kali"
            hof_banished_seg[conquests_ctr] = LBX_Reload_Next(halofam_lbx_file__ovr107, (1 + _players[itr].wizard_id), _screen_seg);
            conquests_ctr++;
        }
    }
    Deactivate_Help_List();
    Set_Score_Screen_Help_List();
    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();
        full_screen_esc = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_hotkey_ESC__ovr107[0], ST_UNDEFINED);
        input_field_idx = Get_Input();
        if(input_field_idx == full_screen_esc)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(leave_screen == ST_FALSE)
        {
            End_Of_Game_Score_Draw();
            PageFlip_FX();
            Release_Time(2);
        }
    }
    Stop_All_Sounds__STUB();
    Clear_Fields();
    Reset_Window();
    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    /* Upsert the Hi Score */
    for(itr = 0; itr < 10; itr++)
    {
        if(magic_set.hof_scores[itr] < m_new_score)
        {
            for(itr2 = 8; itr2 >= itr; itr2--)
            {
                stu_strcpy(magic_set.hof_names[itr2 + 1], magic_set.hof_names[itr2]);
                magic_set.hof_scores[itr2 + 1] = magic_set.hof_scores[itr2];
                magic_set.hof_races[itr2 + 1] = magic_set.hof_races[itr2];
            }
            magic_set.hof_scores[itr] = m_new_score;
            magic_set.hof_races[itr] = _players[_human_player_idx].capital_race;
            stu_strcpy(magic_set.hof_names[itr], _players[_human_player_idx].name);
            fp = stu_fopen_ci(cnst_Set_File3, cnst_WB3);
            if(fp != NULL)
            {
                fwrite(&magic_set, 466, 1, fp);
                fclose(fp);
            }
            break;
        }
    }
    Hall_Of_Fame_Screen();  /* ¿ ~== MoO2  Module: SCORE  End_Of_Game_Hi_Score_() ? */
    Fade_Out();
    Load_Palette(0, ST_UNDEFINED, ST_NULL);
    Reset_Cycle_Palette_Color();
    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    Calculate_Remap_Colors();
    Set_Page_Off();
    Fill(0, 0, 319, 199, 0);
    Toggle_Pages();
    Apply_Palette();
    Set_Mouse_List(1, mouse_list_default);
}


// WZD o107p02
void End_Of_Game_Score_Draw(void)
{
    char space[2] = { 0, 0 };
    char Bonus_Pts_String[6] = { 0, 0, 0, 0, 0, 0, };
    uint8_t colors[4];
    int16_t xstart = 0;
    int16_t conquests_count = 0;
    int16_t draw_y = 0;
    int16_t draw_x = 0;
    int16_t itr_realms = 0;
    int16_t itr = 0;
    int16_t score = 0;
    int16_t temp_val = 0;
    int16_t quotient = 0;
    stu_strcpy(Bonus_Pts_String, cnst_HoF_String_0);  // ": "
    stu_strcpy(space, str_SPACE__ovr107);  // " "
    for(itr = 0; itr < 4; itr++)
    {
        colors[itr] = (117 + itr);
    }
    colors[0] = 240;
    xstart = 47;
    Set_Page_Off();
    Reset_Window();
    Copy_Back_To_Off();
    Set_Font_Style_Shadow_Down(3, 4, 0, 0);
    Set_Outline_Color(250);
    Set_Alias_Color(246);
    Print_Centered(160, 3, _players[_human_player_idx].name);
    score = 0;
    temp_val = 0;
    /* +1 point per spell known */
    for(itr_realms = 0; itr_realms < NUM_MAGIC_REALMS; itr_realms++)
    {
        for(itr = 0; itr < NUM_SPELLS_PER_MAGIC_REALM; itr++)
        {
            if(_players[_human_player_idx].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr)] == sls_Known)
            {
                temp_val++;
            }
        }
    }
    score += temp_val;
    Set_Font_Style_Shadow_Down(2, 4, 0, 0);
    Set_Outline_Color(250);
    Set_Alias_Color(246);
    stu_itoa(temp_val, GUI_String_2, 10);
    stu_strcpy(GUI_String_1, GUI_String_2);
    stu_strcat(GUI_String_1, space);
    stu_strcat(GUI_String_1, cnst_HoF_String_2);
    stu_strcat(GUI_String_1, Bonus_Pts_String);
    stu_strcat(GUI_String_1, GUI_String_2);
    Print(xstart, 15, GUI_String_1);
    draw_x = xstart;
    draw_y = 25;
    for(itr = 0; itr < temp_val && itr < 50; itr++)
    {
        FLIC_Draw(draw_x, draw_y, IMG_GAME_HoF_Book);
        draw_x += 8;
        if(itr == 24)
        {
            draw_x = xstart + 4;
            draw_y += 3;
        }
    }
    temp_val = 0;
    for(itr = 0; itr < _cities; itr++)
    {
        if(_CITIES[itr].owner_idx == _human_player_idx)
        {
            temp_val += _CITIES[itr].population;
        }
    }
    stu_itoa(temp_val, GUI_String_2, 10);
    stu_strcpy(GUI_String_1, GUI_String_2);
    stu_strcat(GUI_String_1, space);
    stu_strcat(GUI_String_1, cnst_HoF_String_3);  // "Townsfolk"
    stu_strcat(GUI_String_1, Bonus_Pts_String);
    temp_val = temp_val / 2;
    stu_itoa(temp_val, GUI_String_2, 10);
    stu_strcat(GUI_String_1, GUI_String_2);
    Print(xstart, 37, GUI_String_1);
    score += temp_val;
    draw_x = xstart;
    draw_y = 46;
    for(itr = 0; itr < temp_val && itr < 100; itr++)
    {
        FLIC_Draw(draw_x, draw_y, IMG_GAME_HoF_Pop);
        draw_x += 9;
        if(itr > 0 && (itr + 1) % 25 == 0)
        {
            quotient = (itr + 1) / 25;
            if(quotient == 1 || quotient == 3)
            {
                draw_x = xstart + 4;
            }
            else
            {
                draw_x = xstart;
            }
            draw_y += 5;
        }
    }
    conquests_count = 0;
    for(itr = 0; itr < _num_players; itr++)
    {
        if(Test_Bit_Field_Near(itr, (char *)&_players[_human_player_idx].Defeated_Wizards) == ST_TRUE)
        {
            conquests_count++;
        }
    }
    temp_val = conquests_count;
    if(temp_val == 0)
    {
        stu_strcpy(GUI_String_1, cnst_HoF_String_4);  // "No"
    }
    else
    {
        stu_itoa(temp_val, GUI_String_2, 10);
        stu_strcpy(GUI_String_1, GUI_String_2);
    }
    stu_strcat(GUI_String_1, space);
    stu_strcat(GUI_String_1, cnst_HoF_String_5);  // "Banished"
    stu_strcat(GUI_String_1, space);
    stu_strcat(GUI_String_1, cnst_HoF_String_6);  // "Wizards"
    stu_strcat(GUI_String_1, Bonus_Pts_String);
    temp_val = temp_val * 50;
    stu_itoa(temp_val, GUI_String_2, 10);
    stu_strcat(GUI_String_1, GUI_String_2);
    Print(xstart, 78, GUI_String_1);
    score += temp_val;
    draw_x = xstart;
    draw_y = 87;
    for(itr = 0; itr < conquests_count; itr++)
    {
        FLIC_Draw(draw_x, draw_y, hof_banished_seg[itr]);
        draw_x += 35;
    }
    temp_val = Player_Fame(_human_player_idx);
    stu_itoa(temp_val, GUI_String_2, 10);
    stu_strcpy(GUI_String_1, GUI_String_2);
    stu_strcat(GUI_String_1, space);
    stu_strcat(GUI_String_1, cnst_Fame_3);
    stu_strcat(GUI_String_1, Bonus_Pts_String);
    temp_val = temp_val * 2;
    stu_itoa(temp_val, GUI_String_2, 10);
    stu_strcat(GUI_String_1, GUI_String_2);
    FLIC_Draw(xstart, 123, IMG_GAME_HoF_Fame);
    Print(xstart + 18, 128, GUI_String_1);
    score += temp_val;
    temp_val = _turn / 12 + 1400;
    stu_strcpy(GUI_String_1, cnst_HoF_String_7);
    stu_strcat(GUI_String_1, space);
    stu_itoa(temp_val, GUI_String_2, 10);
    stu_strcat(GUI_String_1, GUI_String_2);
    stu_strcat(GUI_String_1, cnst_HoF_String_8);
    stu_strcat(GUI_String_1, space);
    temp_val = 0;
    if(
        _FORTRESSES[_human_player_idx].active == ST_TRUE
        &&
        (
            GAME_SoM_Cast_By == ST_UNDEFINED
            ||
            GAME_SoM_Cast_By == _human_player_idx
        )
    )
    {
        temp_val = (2000 - (_turn * 2));
    }
    if(temp_val >= 0)
    {
        stu_strcat(GUI_String_1, cnst_HoF_String_1);
    }
    stu_itoa(temp_val, GUI_String_2, 10);
    stu_strcat(GUI_String_1, GUI_String_2);
    FLIC_Draw(xstart, 142, IMG_GAME_HoF_Time);
    Print(xstart + 18, 147, GUI_String_1);
    score += temp_val;
    stu_strcpy(GUI_String_1, cnst_Spell_3);
    stu_strcat(GUI_String_1, space);
    stu_strcat(GUI_String_1, cnst_Of_2);
    stu_strcat(GUI_String_1, space);
    stu_strcat(GUI_String_1, cnst_HoF_String_9);
    stu_strcat(GUI_String_1, Bonus_Pts_String);
    temp_val = 0;
    if(GAME_SoM_Cast_By == _human_player_idx)
    {
        temp_val = 250;
    }
    stu_itoa(temp_val, GUI_String_2, 10);
    stu_strcat(GUI_String_1, GUI_String_2);
    FLIC_Draw(xstart, 161, IMG_GAME_HoF_SoM);
    Print(xstart + 18, 166, GUI_String_1);
    score += temp_val;
    switch(_difficulty)
    {
        case god_Intro:
            score = score / 2;
            break;
        case god_Easy:
            score = (score * 3) / 4;
            break;
        case god_Normal:
            break;
        case god_Hard:
            score = score * 2;
            break;
        case god_Impossible:
            score = score * 3;
            break;
        default:
            break;
    }
    stu_strcpy(GUI_String_1, cnst_HoF_String_A);
    stu_strcat(GUI_String_1, space);
    stu_itoa(score, GUI_String_2, 10);
    stu_strcat(GUI_String_1, GUI_String_2);
    Print(xstart, 180, GUI_String_1);
    temp_val = score / 40;
    if(temp_val > 200)
    {
        temp_val = 200;
    }
    Gradient_Fill( 47, 189, 251, 189,  3, 0, ST_NULL, ST_NULL, ST_NULL);
    Gradient_Fill( 47, 190,  47, 197,  3, 0, ST_NULL, ST_NULL, ST_NULL);
    Gradient_Fill( 48, 197, 251, 197, 15, 1, ST_NULL, ST_NULL, ST_NULL);
    Gradient_Fill(251, 190, 251, 196, 15, 1, ST_NULL, ST_NULL, ST_NULL);
    Fill(48, 190, 250, 196, 0);
    Fill(49, 191, temp_val + 49, 195, 77);
    m_new_score = score;
}
