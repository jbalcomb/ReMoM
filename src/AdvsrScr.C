/*
    Advisor Screen

    WIZARDS.EXE
        ovr076
    
*/

#include "MoM.H"
#include "AdvsrScr.H"



// WZD dseg:37C0                                                 ¿ BEGIN: Advisor Screen - Initialized Data

// WZD dseg:37C0 20 28 46 00                                     
char cnst_Info_Msg_0[] = " (F";

// WZD dseg:37C4 53 75 72 76 65 79 6F 72 00                      
char cnst_Info_Msg_1[] = "Surveyor";

// WZD dseg:37CD
char text_0x1D[] = "\x1D";

// WZD dseg:37CE 51 00                                           aQ_0 db 'Q',0
// WZD dseg:37D0
char cnst_Info_Msg_1_2[] = "1)";
// WZD dseg:37D3
char cnst_Info_Msg_2[] = "Cartographer";
// WZD dseg:37E0
char cnst_Info_Msg_2_2[] = "2)";
// WZD dseg:37E3
char cnst_Info_Msg_3[] = "Apprentice";
// WZD dseg:37EE
char cnst_Info_Msg_3_2[] = "3)";
// WZD dseg:37F1
char cnst_Info_Msg_4[] = "Historian";
// WZD dseg:37FB
char cnst_Info_Msg_4_2[] = "4)";
// WZD dseg:37FE
char cnst_Info_Msg_5[] = "Astrologer";
// WZD dseg:3809
char cnst_Info_Msg_5_2[] = "5)";
// WZD dseg:380C
char cnst_Info_Msg_6[] = "Chancellor";
// WZD dseg:3817
char cnst_Info_Msg_6_2[] = "6)";
// WZD dseg:381A
char cnst_Info_Msg_7[] = "Tax Collector";
// WZD dseg:3828
char cnst_Info_Msg_7_2[] = "7)";
// WZD dseg:382B
char cnst_Info_Msg_8[] = "Grand Vizier";
// WZD dseg:3838
char cnst_Info_Msg_0_2[] = "*";
// WZD dseg:383A
char cnst_Info_Msg_8_2[] = "8)";
// WZD dseg:383D
char cnst_Info_Msg_9[] = "Mirror";
// WZD dseg:3844
char cnst_Info_Msg_9_2[] = "9)";
// WZD dseg:3847
char cnst_Info_Msg_0_3[] = "Select An Advisor";

// WZD dseg:3859 20 67 6F 6C 64 2C 00                            cnst_Info_Msg_7_3 db ' gold,',0
// WZD dseg:3860 25 20 75 6E 72 65 73 74 00                      cnst_Info_Msg_7_4 db '% unrest',0
// WZD dseg:3869 30 00                                           a0_0 db '0',0
// WZD dseg:386B 1D                                              db  1Dh
// WZD dseg:386C 3B 00                                           asc_3A30C db ';',0
// WZD dseg:386E 2E 35 00                                        cnst_Half db '.5',0
// WZD dseg:3871 1D                                              db  1Dh
// WZD dseg:3872 36 00                                           a6_0 db '6',0
// WZD dseg:3874 31 30 00                                        a10 db '10',0
// WZD dseg:3877 31 00                                           a1_0 db '1',0
// WZD dseg:3879 1D                                              db  1Dh
// WZD dseg:387A 34 00                                           a4_0 db '4',0
// WZD dseg:387C 32 30 00                                        a20 db '20',0
// WZD dseg:387F 31 2E 35 00                                     a1_5 db '1.5',0
// WZD dseg:3883 33 30 00                                        a30 db '30',0
// WZD dseg:3886 32 00                                           a2_0 db '2',0
// WZD dseg:3888 34 35 00                                        a45 db '45',0
// WZD dseg:388B 32 2E 35 00                                     a2_5 db '2.5',0
// WZD dseg:388F 36 30 00                                        a60 db '60',0
// WZD dseg:3892 33 00                                           a3_0 db '3',0
// WZD dseg:3894 37 35 00                                        a75 db '75',0
// WZD dseg:3897 20 54 61 78 20 50 65 72 20 50 6F 70 75 6C 61 74+aTaxPerPopulation db ' Tax Per Population ',0



/*
    WIZARDS.EXE  ovr076
*/

// WZD o76p01
void Advisor_Screen(int16_t advisor_idx)
{
    char str0_Surveyor[26];
    char str1_Cartographer[26];
    char str2_Apprentice[26];
    char str3_Historian[26];
    char str4_Astrologer[26];
    char str5_Chancellor[26];
    char str6_TaxCollector[26];
    char str7_GrandVizier[26];
    char str8_Mirror[26];
    char str9_EoT[26];
    // int16_t str_list[10];
    char * str_list[10];
    char fkey_string[4];
    int16_t input_advisor_idx;

    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Advisor_Screen()\n", __FILE__, __LINE__);
#endif

    strcpy(fkey_string, cnst_Info_Msg_0); // " (F"

    if(advisor_idx == ST_UNDEFINED)
    {
        strcpy(str0_Surveyor, cnst_Info_Msg_1);  // "Surveyor"
        strcat(str0_Surveyor, text_0x1D);
        strcat(str0_Surveyor, fkey_string);
        strcat(str0_Surveyor, cnst_Info_Msg_1_2);

        strcpy(str1_Cartographer, cnst_Info_Msg_2);  // "Cartographer"
        strcat(str1_Cartographer, text_0x1D);
        strcat(str1_Cartographer, fkey_string);
        strcat(str1_Cartographer, cnst_Info_Msg_2_2);

        strcpy(str2_Apprentice, cnst_Info_Msg_3);  // "Apprentice"
        strcat(str2_Apprentice, text_0x1D);
        strcat(str2_Apprentice, fkey_string);
        strcat(str2_Apprentice, cnst_Info_Msg_3_2);

        strcpy(str3_Historian, cnst_Info_Msg_4);  // "Historian"
        strcat(str3_Historian, text_0x1D);
        strcat(str3_Historian, fkey_string);
        strcat(str3_Historian, cnst_Info_Msg_4_2);

        strcpy(str4_Astrologer, cnst_Info_Msg_5);  // "Astrologer"
        strcat(str4_Astrologer, text_0x1D);
        strcat(str4_Astrologer, fkey_string);
        strcat(str4_Astrologer, cnst_Info_Msg_5_2);

        strcpy(str5_Chancellor, cnst_Info_Msg_6);  // "Chancellor"
        strcat(str5_Chancellor, text_0x1D);
        strcat(str5_Chancellor, fkey_string);
        strcat(str5_Chancellor, cnst_Info_Msg_6_2);

        strcpy(str6_TaxCollector, cnst_Info_Msg_7);  // "Tax Collector"
        strcat(str6_TaxCollector, text_0x1D);
        strcat(str6_TaxCollector, fkey_string);
        strcat(str6_TaxCollector, cnst_Info_Msg_7_2);

        strcpy(str7_GrandVizier, cnst_Info_Msg_8);  // "Grand Vizier"
        if(grand_vizier == ST_TRUE)
        {
            strcat(str7_GrandVizier, cnst_Info_Msg_0_2);
        }
        strcat(str7_GrandVizier, text_0x1D);
        strcat(str7_GrandVizier, fkey_string);
        strcat(str7_GrandVizier, cnst_Info_Msg_8_2);

        strcpy(str8_Mirror, cnst_Info_Msg_9);  // "Mirror"
        strcat(str8_Mirror, text_0x1D);
        strcat(str8_Mirror, fkey_string);
        strcat(str8_Mirror, cnst_Info_Msg_9_2);

        strcpy(str9_EoT, 0);  // (offset cnst_Info_Msg_0+3)

        for(itr = 0; itr < 10; itr++)
        {
            // TODO  str_list[itr] = str0_Surveyor[itr];  // sizeof 26
            // ¿ array of char * ?
            // ¿ ~2D array || array of struct sizeof 26 ?
        }

        Set_Advisor_Screen_Help_List();

        // TODO  input_advisor_idx = GUI_ListSelect_Dlg(9, &List@[0], 0, cnst_Info_Msg_0_3);  // "Select An Advisor"
// ; displays a list selection dialog with the specified
// ; title and items, with a separate button corresponding
// ; to each one, that can be highlighted by mousing over
// ; it or selected by clicking
// ; returns the index of the selected item
// ;
// ; BUG: uses two graphical button controls for
// ; multi-page lists that are no longer in memory when
// ; returning - doing a GUI page flip before clearing
// ; them will cause a crash
// ;
// ; WARNING: the drawing function expects the list of
// ; items to be terminated by a zero string

        Deactivate_Help_List();
    }
    else
    {
        input_advisor_idx = advisor_idx;
    }

    switch(input_advisor_idx)
    {
        case 0:
        {
            DLOG("switch(input_advisor_idx))  case 0:");
            
        } break;
        case 1:
        {

        } break;
        case 2:
        {

        } break;
        case 3:
        {

        } break;
        case 4:
        {

        } break;
        case 5:
        {

        } break;
        case 6:
        {

        } break;
        case 7:
        {

        } break;
        case 8:
        {

        } break;        
    }

    PageFlipEffect = 3;
    current_screen = scr_Main_Screen;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Advisor_Screen()\n", __FILE__, __LINE__);
#endif

}

// WZD o76p02
// WZD o76p03
// WZD o76p04
// WZD o76p05
// WZD o76p06
// WZD o76p07
// WZD o76p08
// WZD o76p09
// WZD o76p10
// WZD o76p11
