/*
    Hall of Fame Screen

    MAGIC.EXE
        ovr061
    
    MoO2:
        Module: SCORE
*/

#include "MoX.H"
#include "SCORE.H"



struct s_mouse_list mouse_list_hof[1] = {
    {0, 0, 0, 0, 319, 199}
};





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
char TBL_HoF_RaceStrings[14][20] =
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

// MGC  dseg:3C87 48 41 4C 4F 46 41 4D 00                         halofam_lbx_file db 'HALOFAM',0
char halofam_lbx_file[] = "HALOFAM";

// MGC  dseg:3C8F
char hof_hotkey_ESC = '\x1B';

// MGC  dseg:3C91
char cnst_HoF_Space[] = " ";

// MGC  dseg:3C93
char cnst_HoF_String_B[] = "Hall";

// MGC  dseg:3C98
char cnst_Of[] = "Of";

// MGC  dseg:3C9B
char cnst_Fame[] = "Fame";

// MGC  dseg:3CA0
char cnst_HoF_String_C[] = "Master";

// MGC  dseg:3CA7
char cnst_ClosingBrace[] = ")";

// MGC  dseg:3CA9
char cnst_HoF_String_D[] = " of the ";

// MGC  dseg:3CB2
char cnst_OpeningBrace[] = "(";

// MGC  dseg:3CB4
char cnst_HoF_String_E[] = "%)";

// MGC  dseg:3CB7 00                                              align 2

/*
                                            END: ovr061  (Hall Of Fame)
*/
// MGC  dseg:3CB8 00 00                                           g_TIMER_Count_Lo dw 0





// MGC o61p01
void Hall_Of_Fame_Screen(void)
{
    int16_t hotkey_ESC;
    int16_t input_field_idx;
    SAMB_ptr _halloffame_background_seg;
    int16_t leave_screen_flag;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Hall_Of_Fame_Screen()\n", __FILE__, __LINE__);
#endif

    // TODO  Deactivate_Auto_Function();
    Set_Mouse_List(1, mouse_list_hof);
    Clear_Fields();
    Fade_Out();
    Set_Page_Off();
    Fill(0, 0, 319, 199, 0);
    Toggle_Pages();
    Set_Page_Off();
    _halloffame_background_seg = LBX_Reload(halofam_lbx_file, 0, _screen_seg);
    FLIC_Draw(0, 0, _halloffame_background_seg);
    Apply_Palette();
    Copy_Off_To_Back();
    // TODO  VGA_SaveDraw_Frame4();

    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);
    
    Deactivate_Help_List();

    // TODO  Assign_Auto_Function(Hall_Of_Fame_Screen_Draw(), 1);

    leave_screen_flag = ST_FALSE;
    while(leave_screen_flag == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();
        // TODO  hotkey_idx_ESC = Add_Hidden_Field(0, 0, 319, 199, hof_hotkey_ESC, -1);
        hotkey_ESC = Add_Hidden_Field(0, 0, 319, 199, 27, -1);
        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC)
        {
            leave_screen_flag = ST_UNDEFINED;
        }

        if(leave_screen_flag == ST_FALSE)
        {
            Hall_Of_Fame_Screen_Draw();
            // TODO  MGC ovr050  Do_Toggle_Pages()                   ; used in place of pageflip_fx
            Toggle_Pages();
            Release_Time(2);
        }

    }

    Clear_Fields();
    Reset_Window();

    Clear_Fields();
    // TODO  Deactivate_Auto_Function();
    Deactivate_Help_List();
    Fade_Out();
    Load_Palette(0, -1, 0);
    // TODO  UU_Reset_Cycle_Palette_Color();
    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    // TODO  VGA_SetShades_Grey0();  
    Set_Page_Off();
    Fill(0, 0, 319, 199, 0);
    Toggle_Pages();
    Apply_Palette();
    Set_Mouse_List(1, mouse_list_default);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Hall_Of_Fame_Screen()\n", __FILE__, __LINE__);
#endif
}

// MGC o61p02
void Hall_Of_Fame_Screen_Draw(void)
{
    uint8_t color_array[16];
    int16_t score;
    char str_SPACE[2];
    int16_t print_x;
    int16_t print_y;
    int16_t itr_color_array;
    int16_t itr_scores;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Hall_Of_Fame_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    // strcpy(str_SPACE, cnst_HoF_Space);
    strcpy(str_SPACE, " ");

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
    Set_Font_Colors_15(2, &color_array[0]);  // set font style num 0; set font colors block 15
    Set_Font_Style1(5, 15, 0, 0);  // set font style num 0; set normal,highlight,special colors - blocks 15,0,0; set shadow bottom right
    Set_Outline_Color(250);

    strcpy(GUI_String_1, cnst_HoF_String_B);  // "Hall"
    strcat(GUI_String_1, str_SPACE);
    strcat(GUI_String_1, cnst_Of);  // "Of"
    strcat(GUI_String_1, str_SPACE);
    strcat(GUI_String_1, cnst_Fame);  // "Fame"

    Print_Centered(160, 7, GUI_String_1);

    for(itr_color_array = 0; itr_color_array < 16; itr_color_array++)
    {
        color_array[itr_color_array] = 179;
    }
    color_array[0] = 246;

    Set_Font_Colors_15(2, &color_array[0]);
    Set_Font_Style1(2, 15, 0, 0);
    Set_Outline_Color(250);

    print_x = 72;
    print_y = 42;

    for(itr_scores = 0; itr_scores < 10; itr_scores++)
    {
        if(magic_set.HallofFame_Scores[itr_scores] != ST_FALSE)
        {

            if(itr_scores == 0)
            {
                Print_Right(print_x, print_y, cnst_HoF_String_C);  // "Master"
            }
            else
            {
                itoa((itr_scores+1), GUI_String_2, 10);
                strcpy(GUI_String_1, GUI_String_2);
                strcat(GUI_String_1, cnst_ClosingBrace);  // ")"
                Print_Right(print_x, print_y, GUI_String_1);
            }

            strcpy(GUI_String_1, magic_set.HallofFame_Names[itr_scores]);  // sizeof(magic_set.HallofFame_Names[0]) == 20
            strcat(GUI_String_1, cnst_HoF_String_D);  // " of the "
            strcat(GUI_String_1, TBL_HoF_RaceStrings[magic_set.HallofFame_Races[itr_scores]]);
            Print((print_x + 9), print_y, GUI_String_1);  // ¿ 9 is ~ x offset for title column ?
            Print_Integer_Right((print_x + 170), print_y, magic_set.HallofFame_Scores[itr_scores]);  // ¿ 9 is ~ x offset for score column ?

            score = 8000 / (magic_set.HallofFame_Scores[itr_scores] * 100);  // ... 8000 ... 100 ... LXMUL ... LDIV ...
            
            itoa(score, GUI_String_2, 10);

            strcpy(GUI_String_1, cnst_OpeningBrace);  // "("
            strcat(GUI_String_1, GUI_String_2);
            strcat(GUI_String_1, cnst_HoF_String_E);  // "%)"
            Print_Right((print_x + 198), print_y, GUI_String_1);  // ¿ 198 is ~ x offset for  column ?

            print_y += 13;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Hall_Of_Fame_Screen_Draw()\n", __FILE__, __LINE__);
#endif

}
