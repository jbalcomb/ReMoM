/*
    Hall of Fame Screen

    WIZARDS.EXE
        ovr106
    
    MAGIC.EXE
        ovr061
    
    MoO2:
        Module: SCORE
*/

#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_SET.H"

#include "MOM.H"



// WZD dseg:59B4                                                 BEGIN:  ovr106 - Strings

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

// WZD dseg:5A7D                                                 END:  ovr106 - Strings







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





// MGC o61p01
void Hall_Of_Fame_Screen(void)
{
    int16_t full_screen_ESC_field;
    int16_t input_field_idx;
    SAMB_ptr hof_background_seg;
    int16_t leave_screen_flag;

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

        full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr106, ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == full_screen_ESC_field)
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

    Load_Palette(0, -1, 0);
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

// MGC o61p02
void Hall_Of_Fame_Screen_Draw(void)
{
    uint8_t color_array[16];
    int16_t score_percent;
    char str_SPACE[2];
    int16_t print_x;
    int16_t print_y;
    int16_t itr_color_array;
    int16_t itr_scores;

    strcpy(str_SPACE, str_SPACE__ovr106);

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

    Set_Font_Colors_15(2, &color_array[0]);     // set font style num 0; set font colors block 15
    Set_Font_Style_Shadow_Down(5, 15, 0, 0);    // set font style num 0; set normal,highlight,special colors - blocks 15,0,0; set shadow bottom right

    Set_Outline_Color(250);

    strcpy(GUI_String_1, cnst_HoF_String_B);  // "Hall"
    strcat(GUI_String_1, str_SPACE);          // " "
    strcat(GUI_String_1, cnst_Of);            // "Of"
    strcat(GUI_String_1, str_SPACE);          // " "
    strcat(GUI_String_1, cnst_Fame);          // "Fame"

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
                itoa((itr_scores+1), GUI_String_2, 10);
                strcpy(GUI_String_1, GUI_String_2);
                strcat(GUI_String_1, str_PAREN_CLOSE);  // ")"
                Print_Right(print_x, print_y, GUI_String_1);
            }

            strcpy(GUI_String_1, magic_set.hof_names[itr_scores]);
            strcat(GUI_String_1, cnst_HoF_String_D);  // " of the "
            strcat(GUI_String_1, TBL_HoF_RaceStrings[magic_set.hof_races[itr_scores]]);
            Print((print_x + 9), print_y, GUI_String_1);  // ¿ 9 is ~ x offset for title column ?
            Print_Integer_Right((print_x + 170), print_y, magic_set.hof_scores[itr_scores]);  // ¿ 9 is ~ x offset for score column ?

            // score_percent = (8000 / (magic_set.hof_scores[itr_scores] * 100));  // ... 8000 ... 100 ... LXMUL ... LDIV ...
            score_percent = ((magic_set.hof_scores[itr_scores] * 100) / 8000);

            itoa(score_percent, GUI_String_2, 10);
            strcpy(GUI_String_1, str_PAREN_OPEN);  // "("
            strcat(GUI_String_1, GUI_String_2);
            strcat(GUI_String_1, cnst_HoF_String_E);  // "%)"
            Print_Right((print_x + 198), print_y, GUI_String_1);  // ¿ 198 is ~ x offset for  column ?

            print_y += 13;
        }
    }

}
