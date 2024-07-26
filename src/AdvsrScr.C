/*
    Advisor Screen

    WIZARDS.EXE
        ovr076
    
*/

#include "MoM.H"



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

/*
    BEGIN: Tax Collector
*/
// WZD dseg:3859
char cnst_Info_Msg_7_3[] = " gold,";
// WZD dseg:3860
char cnst_Info_Msg_7_4[] = "% unrest";
// WZD dseg:3869
char str_0[] = "0";
// WZD dseg:386B
char tab_59[] = "\x1D\x3B";
// WZD dseg:386E
char cnst_Half[] = ".5";
// WZD dseg:3871
char tab_54[] = "\x1D\x36";
// WZD dseg:3874
char a10[] = "10";
// WZD dseg:3877
char a1_0[] = "1";
// WZD dseg:3879
char tab_52[] = "\x1D\x34";
// WZD dseg:387C
char a20[] = "20";
// WZD dseg:387F
char a1_5[] = "1.5";
// WZD dseg:3883
char a30[] = "30";
// WZD dseg:3886
char a2_0[] = "2";
// WZD dseg:3888
char a45[] = "45";
// WZD dseg:388B
char a2_5[] = "2.5";
// WZD dseg:388F
char a60[] = "60";
// WZD dseg:3892
char a3_0[] = "3";
// WZD dseg:3894
char a75[] = "75";
// WZD dseg:3897
// Selection Box - Title Text
char aTaxPerPopulation[] = " Tax Per Population ";
/*
    END: Tax Collector
*/



// WZD dseg:3983
// Confirmation Box - Message Text
char aDoYouWishToAllowThe[] = "Do you wish to allow the Grand Vizier to select what buildings your cities create?";





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
    char box_list_stings[10][26];
    // char box_list_stings[260];
    // int16_t str_list[10];
    char * str_list[10];
    char fkey_string[4];
    int16_t input_advisor_idx;

    int16_t itr;

    strcpy(fkey_string, cnst_Info_Msg_0); // " (F"

    if(advisor_idx == ST_UNDEFINED)
    {
        strcpy(str0_Surveyor, cnst_Info_Msg_1);  // "Surveyor"
        // strcat(str0_Surveyor, text_0x1D);
        strcat(str0_Surveyor, "\t");
        strcat(str0_Surveyor, fkey_string);
        strcat(str0_Surveyor, cnst_Info_Msg_1_2);

        strcpy(str1_Cartographer, cnst_Info_Msg_2);  // "Cartographer"
        // strcat(str1_Cartographer, text_0x1D);
        strcat(str1_Cartographer, "\t");
        strcat(str1_Cartographer, fkey_string);
        strcat(str1_Cartographer, cnst_Info_Msg_2_2);

        strcpy(str2_Apprentice, cnst_Info_Msg_3);  // "Apprentice"
        // strcat(str2_Apprentice, text_0x1D);
        strcat(str2_Apprentice, "\t");
        strcat(str2_Apprentice, fkey_string);
        strcat(str2_Apprentice, cnst_Info_Msg_3_2);

        strcpy(str3_Historian, cnst_Info_Msg_4);  // "Historian"
        // strcat(str3_Historian, text_0x1D);
        strcat(str3_Historian, "\t");
        strcat(str3_Historian, fkey_string);
        strcat(str3_Historian, cnst_Info_Msg_4_2);

        strcpy(str4_Astrologer, cnst_Info_Msg_5);  // "Astrologer"
        // strcat(str4_Astrologer, text_0x1D);
        strcat(str4_Astrologer, "\t");
        strcat(str4_Astrologer, fkey_string);
        strcat(str4_Astrologer, cnst_Info_Msg_5_2);

        strcpy(str5_Chancellor, cnst_Info_Msg_6);  // "Chancellor"
        // strcat(str5_Chancellor, text_0x1D);
        strcat(str5_Chancellor, "\t");
        strcat(str5_Chancellor, fkey_string);
        strcat(str5_Chancellor, cnst_Info_Msg_6_2);

        strcpy(str6_TaxCollector, cnst_Info_Msg_7);  // "Tax Collector"
        // strcat(str6_TaxCollector, text_0x1D);
        strcat(str6_TaxCollector, "\t");
        strcat(str6_TaxCollector, fkey_string);
        strcat(str6_TaxCollector, cnst_Info_Msg_7_2);

        strcpy(str7_GrandVizier, cnst_Info_Msg_8);  // "Grand Vizier"
        if(grand_vizier == ST_TRUE)
        {
            strcat(str7_GrandVizier, cnst_Info_Msg_0_2);
        }
        // strcat(str7_GrandVizier, text_0x1D);
        strcat(str7_GrandVizier, "\t");
        strcat(str7_GrandVizier, fkey_string);
        strcat(str7_GrandVizier, cnst_Info_Msg_8_2);

        strcpy(str8_Mirror, cnst_Info_Msg_9);  // "Mirror"
        // strcat(str8_Mirror, text_0x1D);
        strcat(str8_Mirror, "\t");
        strcat(str8_Mirror, fkey_string);
        strcat(str8_Mirror, cnst_Info_Msg_9_2);

        // strcpy(str9_EoT, 0);  // (offset cnst_Info_Msg_0+3)
        strcpy(str9_EoT, "");

        strcpy(box_list_stings[0], cnst_Info_Msg_1);  // "Surveyor"
        // strcat(str0_Surveyor, text_0x1D);  // ASCII  1Dh  29d  GS  (group separator)
        strcat(box_list_stings[0], "\x1D\x51");
        strcat(box_list_stings[0], fkey_string);
        strcat(box_list_stings[0], cnst_Info_Msg_1_2);

        strcpy(box_list_stings[1], cnst_Info_Msg_2);  // "Cartographer"
        // strcat(str1_Cartographer, text_0x1D);
        strcat(box_list_stings[1], "\x1D\x51");
        strcat(box_list_stings[1], fkey_string);
        strcat(box_list_stings[1], cnst_Info_Msg_2_2);

        strcpy(box_list_stings[2], cnst_Info_Msg_3);  // "Apprentice"
        // strcat(str2_Apprentice, text_0x1D);
        strcat(box_list_stings[2], "\x1D\x51");
        strcat(box_list_stings[2], fkey_string);
        strcat(box_list_stings[2], cnst_Info_Msg_3_2);

        strcpy(box_list_stings[3], cnst_Info_Msg_4);  // "Historian"
        // strcat(str3_Historian, text_0x1D);
        strcat(box_list_stings[3], "\x1D\x51");
        strcat(box_list_stings[3], fkey_string);
        strcat(box_list_stings[3], cnst_Info_Msg_4_2);

        strcpy(box_list_stings[4], cnst_Info_Msg_5);  // "Astrologer"
        // strcat(str4_Astrologer, text_0x1D);
        strcat(box_list_stings[4], "\x1D\x51");
        strcat(box_list_stings[4], fkey_string);
        strcat(box_list_stings[4], cnst_Info_Msg_5_2);

        strcpy(box_list_stings[5], cnst_Info_Msg_6);  // "Chancellor"
        // strcat(str5_Chancellor, text_0x1D);
        strcat(box_list_stings[5], "\x1D\x51");
        strcat(box_list_stings[5], fkey_string);
        strcat(box_list_stings[5], cnst_Info_Msg_6_2);

        strcpy(box_list_stings[6], cnst_Info_Msg_7);  // "Tax Collector"
        // strcat(str6_TaxCollector, text_0x1D);
        strcat(box_list_stings[6], "\x1D\x51");
        strcat(box_list_stings[6], fkey_string);
        strcat(box_list_stings[6], cnst_Info_Msg_7_2);

        strcpy(box_list_stings[7], cnst_Info_Msg_8);  // "Grand Vizier"
        if(grand_vizier == ST_TRUE)
        {
            strcat(box_list_stings[7], cnst_Info_Msg_0_2);
        }
        // strcat(str7_GrandVizier, text_0x1D);
        strcat(box_list_stings[7], "\x1D\x51");
        strcat(box_list_stings[7], fkey_string);
        strcat(box_list_stings[7], cnst_Info_Msg_8_2);

        strcpy(box_list_stings[8], cnst_Info_Msg_9);  // "Mirror"
        // strcat(str8_Mirror, text_0x1D);
        // strcat(box_list_stings[8], "\t");
        strcat(box_list_stings[8], "\x1D\x51");
        strcat(box_list_stings[8], fkey_string);
        strcat(box_list_stings[8], cnst_Info_Msg_9_2);

        // strcpy(str9_EoT, 0);  // (offset cnst_Info_Msg_0+3)
        strcpy(box_list_stings[9], "");

        for(itr = 0; itr < 10; itr++)
        {
            str_list[itr] = (char *)(str0_Surveyor + (26 * itr));  // sizeof 26
            // ¿ array of char * ?
            // ¿ ~2D array || array of struct sizeof 26 ?
            // str_list[itr] = box_list_stings[itr];
            // str_list[itr] = (box_list_stings + 26 * itr);
        }


        for(itr = 0; itr < 10; itr++)
        {
            // str_list[itr] = (char *)(str0_Surveyor + (26 * itr));  // sizeof 26
            // ¿ array of char * ?
            // ¿ ~2D array || array of struct sizeof 26 ?
            str_list[itr] = box_list_stings[itr];
            // str_list[itr] = (box_list_stings + 26 * itr);
        }

        Set_Advisor_Screen_Help_List();


        // input_advisor_idx = GUI_ListSelect_Dlg(9, &List@[0], 0, cnst_Info_Msg_0_3);  // "Select An Advisor"
        input_advisor_idx = Selection_Box(9, &str_list[0], ST_FALSE, "Select An Advisor");
        // returns -1 for ESC

        Deactivate_Help_List();
    }
    else  /* if(advisor_idx == ST_UNDEFINED) */
    {
        input_advisor_idx = advisor_idx;
    }

    switch(input_advisor_idx)
    {
        case 0:  /* Surveyor      (F1) */
        {
            // TODO  j_IDK_Surveyor_s7A3C0()

        } break;
        case 1:  /* Cartographer  (F2) */
        {
            // TODO  IDK_Cartographer_Screen()

        } break;
        case 2:  /* Apprentice    (F3) */
        {
            // TODO  IDK_AdvsrScr_Apprentice()

        } break;
        case 3:  /* Historian     (F4) */
        {
            // TODO  IDK_AdvsrScr_Historian()

        } break;
        case 4:  /* Astrologer    (F5) */
        {
            // TODO  IDK_AdvsrScr_Astrologer()

        } break;
        case 5:  /* Chancellor    (F6) */
        {
            // TODO  j_IDK_Chancellor_EoTSummary_EventsAnimScroll(1);

        } break;
        case 6:  /* Tax Collector (F7) */
        {
            TaxCollector_Window();

        } break;
        case 7:  /* Grand Vizier  (F8) */
        {
            GrandVizier_Window();

        } break;
        case 8:  /* Mirror        (F9 )*/
        {
            Mirror_Screen(HUMAN_PLAYER_IDX, 150, 60, 180, 90);
        } break;        
    }

    PageFlipEffect = 3;
    current_screen = scr_Main_Screen;

}

// WZD o76p02
/*
    Tax Collector - Advisor
    "Tax Per Population"
    
    ¿ Relationship ?
        Count of List Items := Count of Tax Rates

no idea why there are some many additional bytes allocated
probably meaningful that they fit the 30 chars def
same for UU_IDK_var_20
    ¿ some other string piece ? would have been after "unrest", if the 'compiler reverses order' theory holds
    ¿ someone designed it with hot-keys ?
*/
void TaxCollector_Window(void)
{
// box_list_stings__00= byte ptr -1B6h
// box_list_stings__01= byte ptr -198h
// box_list_stings__02= byte ptr -17Ah
// box_list_stings__03= byte ptr -15Ch
// box_list_stings__04= byte ptr -13Eh
// box_list_stings__05= byte ptr -120h
// box_list_stings__06= byte ptr -102h
// box_list_stings__07= byte ptr -0E4h
    char box_list_stings[10][26];
// NX_line9= byte ptr -0C6h
// NX_line10= byte ptr -0A8h
// NX_line11= byte ptr -8Ah
// NX_line12= byte ptr -6Ch
// NX_line13= byte ptr -4Eh
    char * str_list[8];
// UU_IDK_var_20= byte ptr -20h
    char str_unrest[10];
    char str_gold[10];
    int16_t selection;

    int16_t tax_rate;
    int16_t itr_list;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: TaxCollector_Window()\n", __FILE__, __LINE__);
#endif

    strcpy(str_gold, cnst_Info_Msg_7_3); // " gold,"

    strcpy(str_unrest, cnst_Info_Msg_7_4); // "% unrest"


    tax_rate = _players[_human_player_idx].tax_rate;



    // tax_rate: 0  "0 gold,    0% unrest "
    // tax_rate: 1  ".5 gold,  10% unrest "
    // tax_rate: 2  "1 gold,   20% unrest "
    // tax_rate: 3  "1.5 gold, 30% unrest*"
    // tax_rate: 4  "2 gold,   45% unrest "
    // tax_rate: 5  "2.5 gold, 60% unrest "
    // tax_rate: 6  "3 gold,   75% unrest "

    strcpy(box_list_stings[0], str_0);
    strcat(box_list_stings[0], str_gold);
    strcat(box_list_stings[0], tab_59);
    strcat(box_list_stings[0], str_0);
    strcat(box_list_stings[0], str_unrest);

    strcpy(box_list_stings[1], cnst_Half);
    strcat(box_list_stings[1], str_gold);
    strcat(box_list_stings[1], tab_54);
    strcat(box_list_stings[1], a10);
    strcat(box_list_stings[1], str_unrest);

    strcpy(box_list_stings[2], a1_0);
    strcat(box_list_stings[2], str_gold);
    strcat(box_list_stings[2], tab_52);
    strcat(box_list_stings[2], a20);
    strcat(box_list_stings[2], str_unrest);

    strcpy(box_list_stings[3], a1_5);
    strcat(box_list_stings[3], str_gold);
    strcat(box_list_stings[3], tab_52);
    strcat(box_list_stings[3], a30);
    strcat(box_list_stings[3], str_unrest);

    strcpy(box_list_stings[4], a2_0);
    strcat(box_list_stings[4], str_gold);
    strcat(box_list_stings[4], tab_52);
    strcat(box_list_stings[4], a45);
    strcat(box_list_stings[4], str_unrest);

    strcpy(box_list_stings[5], a2_5);
    strcat(box_list_stings[5], str_gold);
    strcat(box_list_stings[5], tab_52);
    strcat(box_list_stings[5], a60);
    strcat(box_list_stings[5], str_unrest);

    strcpy(box_list_stings[6], a3_0);
    strcat(box_list_stings[6], str_gold);
    strcat(box_list_stings[6], tab_52);
    strcat(box_list_stings[6], a75);
    strcat(box_list_stings[6], str_unrest);



    if(tax_rate > 6)
    {
        tax_rate = 6;
    }


    for(itr_list = 0; itr_list < 7; itr_list++)
    {
        if(itr_list == tax_rate)
        {
            strcat(box_list_stings[itr_list], cnst_Info_Msg_0_2);
        }
        str_list[itr_list] = box_list_stings[itr_list];
    }

    // ¿ why not set before and included in the loop like in 'Advisor Screen'
    // strcpy(box_list_stings[7] = *(asc_3A261 + 2));  // '\x00' BCPP3 string optimizer
    strcpy(box_list_stings[7], "");
    // str_list[7] = &box_list_stings[7];
    // str_list[7] = (char *)(box_list_stings + (30 * 7));  // sizeof(box_list_stings[0])
    str_list[7] = box_list_stings[7];

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: str_list[0]: %s\n", __FILE__, __LINE__, str_list[0]);
    dbg_prn("DEBUG: [%s, %d]: str_list[1]: %s\n", __FILE__, __LINE__, str_list[1]);
    dbg_prn("DEBUG: [%s, %d]: str_list[2]: %s\n", __FILE__, __LINE__, str_list[2]);
    dbg_prn("DEBUG: [%s, %d]: str_list[3]: %s\n", __FILE__, __LINE__, str_list[3]);
    dbg_prn("DEBUG: [%s, %d]: str_list[4]: %s\n", __FILE__, __LINE__, str_list[4]);
    dbg_prn("DEBUG: [%s, %d]: str_list[5]: %s\n", __FILE__, __LINE__, str_list[5]);
    dbg_prn("DEBUG: [%s, %d]: str_list[6]: %s\n", __FILE__, __LINE__, str_list[6]);
    dbg_prn("DEBUG: [%s, %d]: str_list[7]: %s\n", __FILE__, __LINE__, str_list[7]);
#endif


    // TODO  Set_TaxCollector_Window_Help_List();


    // selection = Selection_Box(7, &str_list[0], ST_FALSE, aTaxPerPopulation);
    selection = Selection_Box(7, &str_list[0], ST_FALSE, " Tax Per Population ");


    Deactivate_Help_List();


    if(selection != ST_UNDEFINED)
    {
        _players[_human_player_idx].tax_rate = selection;
    }


    // TODO  CTY_RecalculateAll();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: TaxCollector_Window()\n", __FILE__, __LINE__);
#endif

}

// WZD o76p03
// WZD o76p04
// WZD o76p05
// WZD o76p06
// WZD o76p07
// WZD o76p08
// WZD o76p09
// WZD o76p10

// WZD o76p11
void GrandVizier_Window(void)
{
    if(grand_vizier != ST_FALSE)
    {
        grand_vizier = ST_FALSE;
    }
    else
    {
        // ; loads and displays the provided message in a
        // ; confirmation dialog box, providing "Yes" and "No"
        // ; option buttons, and halting all other screen
        // ; animation until a result is selected
        // ; returns 1 if "Yes" is selected, or 0 otherwise
        if(Confirmation_Box(aDoYouWishToAllowThe) == ST_TRUE)
        {
            grand_vizier = ST_TRUE;
        }

    }
}
