/*
    Advisor Screen

    WIZARDS.EXE
        ovr076
    
*/

#include "AdvsrScr.h"

#include "../../MoX/src/Fields.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Help.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */

#include "Explore.h"
#include "MainScr_Maps.h"
#include "MOM_SCR.h"
#include "REPORT.h"
#include "Spellbook.h"
#include "Surveyor.h"
#include "UnitMove.h"   // WTFMATE
#include "WIZVIEW.h"

#include <assert.h>
#include <stdlib.h>

#include "SDL_stdinc.h"



// WZD dseg:37C0                                                 BEGIN: ovr076 - Initialized Data  (Advisor Screen)

// WZD dseg:37C0
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

// WZD dseg:3897
char str_TaxRatePerPopulation__ovr076[] = " Tax Rate Per Population ";
// WZD dseg:38AA
char str_SPACE__ovr076[] = " ";  // in the midst of str_TaxRatePerPopulation__ovr076, per the 'String Optimizer'

// WZD dseg:38AC
char reload_lbx_file__ovr076[] = "RELOAD";

// WZD dseg:38AC
char str_hotkey_ESC__ovr076[] = "\x1B";

// WZD dseg:38B5
char str_January__ovr076[] = "January";
// WZD dseg:38BD
char str_February__ovr076[] = "February";
// WZD dseg:38C6
char str_March__ovr076[] = "March";
// WZD dseg:38CC
char str_April__ovr076[] = "April";
// WZD dseg:38D2
char str_May__ovr076[] = "May";
// WZD dseg:38D6
char str_June__ovr076[] = "June";
// WZD dseg:38DB
char str_July__ovr076[] = "July";
// WZD dseg:38E0
char str_August__ovr076[] = "August";
// WZD dseg:38E7
char str_September__ovr076[] = "September";
// WZD dseg:38F1
char str_October__ovr076[] = "October";
// WZD dseg:38F9
char str_November__ovr076[] = "November";
// WZD dseg:3902
char str_December__ovr076[] = "December";

// WZD dseg:390B
char str_HistoryOfWizardsPower__ovr076[] = "History Of Wizards Power";

// WZD dseg:391E
char str_Power[] = "Power";  // in the midst of str_HistoryOfWizardsPower__ovr076, per the 'String Optimizer'

// WZD dseg:3924
char str_CurrentStatusOfWiza[] = "Current Status Of Wizards";

// WZD dseg:393E
char str_Army__ovr076[] = "Army";

// WZD dseg:3943
char str_Strength__ovr076[] = "Strength";

// WZD dseg:394C
char str_Magic__ovr076[] = "Magic";

// WZD dseg:3952
char str_Spell__ovr076[] = "Spell";

// WZD dseg:3958
char str_Research[] = "Research";

// WZD dseg:3961
char str_hotkey_N__ovr076[] = "N";

// WZD dseg:3963
char str_hotkey_C__ovr076[] = "C";

// WZD dseg:3965
char str_Neutral__ovr076[] = "Neutral";

// WZD dseg:396D
char str_Arcanus__ovr076[] = "Arcanus";

// WZD dseg:3975
char str_Myrror__ovr076[] = "Myrror";

// WZD dseg:397C
char str_Plane__ovr076[] = " Plane";

// WZD dseg:3983
char str_DoYouWishToAllowTheGrandVizierToSelectWhatBuildingsYourCitiesCreate[] = "Do you wish to allow the Grand Vizier to select what buildings your cities create?";

// WZD dseg:3983                                                 END: ovr076 - Initialized Data  (Advisor Screen)




// WZD dseg:3983
// Confirmation Box - Message Text
char aDoYouWishToAllowThe[] = "Do you wish to allow the Grand Vizier to select what buildings your cities create?";



// WZD dseg:C296                                                 BEGIN:  ovr076 - Uninitialized Data  (Advisors Screen)

// WZD dseg:C296
int16_t m_cartograph_plane;

// WZD dseg:C298
SAMB_ptr cartograph_seg;

// WZD dseg:C29A
int16_t m_cartograph_y;
// WZD dseg:C29C
int16_t m_cartograph_x;

// WZD dseg:C29E
int16_t status_screen_y;

// WZD dseg:C2A0
int16_t status_screen_x;

// WZD dseg:C2A2
SAMB_ptr status_screen_seg;

// WZD dseg:C2A4
SAMB_ptr powergraph_screen_seg;

// WZD dseg:C2A4                                                 END:  ovr076 - Uninitialized Data  (Advisors Screen)





/*
    WIZARDS.EXE  ovr076
*/

// WZD o76p01
void Advisor_Screen(int16_t advisor_idx)
{
    char str0_Surveyor[26]      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str1_Cartographer[26]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str2_Apprentice[26]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str3_Historian[26]     = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str4_Astrologer[26]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str5_Chancellor[26]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str6_TaxCollector[26]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str7_GrandVizier[26]   = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str8_Mirror[26]        = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str9_EoT[26]           = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char box_list_stings[10][26] = {
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
    // char box_list_stings[260];
    // int16_t str_list[10];
    char* str_list[10] = { 0,0,0,0,0,0,0,0,0,0 };
    char fkey_string[4] = { 0, 0, 0, 0 };
    int16_t input_advisor_idx = 0;

    int16_t itr = 0;

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
        // input_advisor_idx = Selection_Box(9, box_list_stings, ST_FALSE, "Select An Advisor");
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
            Surveyor_Screen();
        } break;
        case 1:  /* Cartographer  (F2) */
        {
            assert(_map_x >= WORLD_XMIN && _map_x <= WORLD_XMAX);  /*  0 & 59 */
            assert(_map_y >= WORLD_YMIN && _map_y <= WORLD_YMAX);  /*  0 & 39 */
            Cartographer_Screen__WIP();
        } break;
        case 2:  /* Apprentice    (F3) */
        {
            Apprentice_Screen__WIP();
        } break;
        case 3:  /* Historian     (F4) */
        {
            Powergraph_Screen();
        } break;
        case 4:  /* Astrologer    (F5) */
        {
            Status_Screen();
        } break;
        case 5:  /* Chancellor    (F6) */
        {
            Chancellor_Screen__WIP(1);
        } break;
        case 6:  /* Tax Collector (F7) */
        {
            TaxCollector_Window();
        } break;
        case 7:  /* Grand Vizier  (F8) */
        {
            Grand_Vizier_Window();
        } break;
        case 8:  /* Mirror        (F9 )*/
        {
            Mirror_Screen(HUMAN_PLAYER_IDX, 150, 60, 180, 90);
        } break;        
    }

    _page_flip_effect = 3;

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
    char box_list_stings[10][26] = {
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    };
// NX_line9= byte ptr -0C6h
// NX_line10= byte ptr -0A8h
// NX_line11= byte ptr -8Ah
// NX_line12= byte ptr -6Ch
// NX_line13= byte ptr -4Eh
    char * str_list[8]   = { 0, 0, 0, 0, 0, 0, 0, 0 };
// UU_IDK_var_20= byte ptr -20h
    char str_unrest[10]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char str_gold[10]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t selection = 0;

    int16_t tax_rate = 0;
    int16_t itr_list = 0;

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



    // DELETEME  if(tax_rate > 6)
    // DELETEME  {
    // DELETEME      tax_rate = 6;
    // DELETEME  }
    SETMAX(tax_rate, 6);

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

    // TODO  Set_TaxCollector_Window_Help_List();

    // selection = Selection_Box(7, &str_list[0], ST_FALSE, aTaxPerPopulation);
    selection = Selection_Box(7, &str_list[0], ST_FALSE, " Tax Per Population ");

    Deactivate_Help_List();

    if(selection != ST_UNDEFINED)
    {
        _players[_human_player_idx].tax_rate = selection;
    }

    // TODO  CTY_RecalculateAll();

}

// WZD o76p03
/*
    "Historian"
    "History of Wizards Power"

*/
void Powergraph_Screen(void)
{

    int16_t input_field_idx;
    int16_t full_screen_ESC_field;
    int16_t leave_screen;  // _SI_

    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);

    // RELOAD.LBX, 000  STATBACK
    powergraph_screen_seg = LBX_Reload(reload_lbx_file__ovr076, 0, _screen_seg);

    Clear_Fields();
    Set_Page_Off();
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();
    PageFlip_FX();
    Set_Page_Off();

    Load_Palette_From_Animation(powergraph_screen_seg);

    Set_Palette_Changes(0, 255);

    Update_Remap_Gray_Palette();

    // DNE Copy_Back_To_Off();

    Assign_Auto_Function(Powergraph_Screen_Draw, 1);

    Deactivate_Help_List();

    Set_Powergraph_Screen_Help_List();

    status_screen_x = 20;
    status_screen_y = 11;

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_XMAX, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr076[0], ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == full_screen_ESC_field)
        {
            Play_Left_Click();
            leave_screen = ST_TRUE;
        }

        if(leave_screen == ST_FALSE)
        {
            Powergraph_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
    }

    Deactivate_Help_List();
    Deactivate_Auto_Function();
    Reset_Window();
    Clear_Fields();

}


// WZD o76p04
// drake178: sub_655C4()
void Powergraph_Screen_Draw(void)
{
    char months[12][12] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
    char dest[7] = { 0, 0, 0, 0, 0, 0, 0 };
    int16_t month = 0;  // ¿ used as 'first month' ... 'first turn' (¿ because turns are months ?) ?
    int16_t y1 = 0;
    int16_t x2 = 0;
    int16_t turns_count = 0;
    int16_t itr_players = 0;  // itr_players__turns
    int16_t itr_turns = 0;  // itr_players__turns
    int16_t WTF__turns_years = 0;
    int16_t WTF__turns_months = 0;
    uint8_t banner_colors[6] = { 0, 0, 0, 0, 0, 0 };
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t year = 0;  // _SI_  ¿ used as 'first year' ?
    int16_t months_count = 0;  // _DI_

    strcpy(dest, &str_TaxRatePerPopulation__ovr076[19]);  // ¿ "tion" ? ¿ but, also, unsed/overwritten ?

    strcpy(months[0],  str_January__ovr076);
    strcpy(months[1],  str_February__ovr076);
    strcpy(months[2],  str_March__ovr076);
    strcpy(months[3],  str_April__ovr076);
    strcpy(months[4],  str_May__ovr076);
    strcpy(months[5],  str_June__ovr076);
    strcpy(months[6],  str_July__ovr076);
    strcpy(months[7],  str_August__ovr076);
    strcpy(months[8],  str_September__ovr076);
    strcpy(months[9],  str_October__ovr076);
    strcpy(months[10], str_November__ovr076);
    strcpy(months[11], str_December__ovr076);

    banner_colors[0] = BANNER_COLOR_BLUE;
    banner_colors[1] = BANNER_COLOR_GREEN;
    banner_colors[2] = BANNER_COLOR_PURPLE;
    banner_colors[3] = BANNER_COLOR_RED;
    banner_colors[4] = BANNER_COLOR_YELLOW;

    Set_Page_Off();

    FLIC_Draw(0, 0, powergraph_screen_seg);

    strcpy(dest, str_SPACE__ovr076);

    for(itr_players = 1; itr_players < NUM_PLAYERS; itr_players++)
    {
        colors[itr_players] = (177 + itr_players);
    }

    Set_Font_Colors_15(4, &colors[0]);

    Set_Font_Style_Shadow_Down(4, 15, 0, 0);

    Set_Alias_Color(246);

    Set_Outline_Color(254);

    Print_Centered(160, 10, str_HistoryOfWizardsPower__ovr076);  // "History Of Wizards Power"

    WTF__turns_years = (_turn / 12);

    WTF__turns_months = (_turn % 12);

    // strcpy(GUI_String_1, (char *)&months[(WTF__turns_months * 12)][0]);
    // strcpy(GUI_String_1, &months[0]);
    strcpy(GUI_String_1, (char *)&months[8]);

    SDL_itoa((1400 + WTF__turns_years), GUI_String_2, 10);

    strcat(GUI_String_1, dest);

    strcat(GUI_String_1, GUI_String_2);

    Set_Font_Style_Shadow_Down(2, 15, 0, 0);

    Set_Outline_Color(254);

    Print_Right(309, 11, GUI_String_1);

    Line(16, 183, 303, 183, 180);

    Gradient_Fill(16, 184, 303, 184, 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed

    year = 0;

    month = 0;

    if(WTF__turns_years > 23)  // 23 tick marks, on the time-line
    {
        year = (WTF__turns_years - 23);  // 23 tick marks, on the time-line
        month = (11 - WTF__turns_months);
    }

    if(((year + 1) / 5) == 0)
    {
        year++;
        month = (WTF__turns_months + 23);  // 23 tick marks, on the time-line
    }

    Set_Font_Style_Shadow_Down(1, 0, 0, 0);

    Set_Outline_Color(254);

    Print_Integer(10, 186, (1400 + year));  // the actual/first year on the time-line

    months_count = 1;

    if(_turn < 288)
    {
        turns_count = _turn;
    }
    else
    {
        turns_count = 287;
    }

    for(itr_turns = 0; itr_turns < turns_count; itr_turns++)
    {

        if((months_count % 12) == 0)
        {

            x2 = (16 + ((months_count / 12) * 12));

            y1 = 182;

            Line(x2, y1, x2, (2 + y1), 180);

            if(((year + (months_count / 12)) % 5) == 0)
            {
                // TODO DEDU IDGI  the Dasm shows `10 +` but that simply does not work ... somehow it must get to adding another 12?
                // Print_Integer((10 + ((months_count / 12) * 12)), 186, (1400 + year + (months_count / 12)));
                Print_Integer((22 + ((months_count / 12) * 12)), 186, (1400 + year + (months_count / 12)));
            }

        }

        months_count++;
    }

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        if(_FORTRESSES[itr_players].active == ST_TRUE)
        {
            if(_players[_human_player_idx].Dipl.Contacted[itr_players] == ST_TRUE)  // ¿ current player ?
            {
                IDK_Powergraph_Draw_Turn_Data(itr_players, turns_count, month);
            }
        }
    }

    IDK_Powergraph_Draw_Turn_Data(_human_player_idx, turns_count, month);

    colors[0] = banner_colors[(_players[HUMAN_PLAYER_IDX].banner_id)];
    colors[1] = banner_colors[(_players[HUMAN_PLAYER_IDX].banner_id)];
    colors[2] = banner_colors[(_players[HUMAN_PLAYER_IDX].banner_id)];
    colors[3] = banner_colors[(_players[HUMAN_PLAYER_IDX].banner_id)];
    colors[4] = banner_colors[(_players[HUMAN_PLAYER_IDX].banner_id)];
    colors[5] = banner_colors[(_players[HUMAN_PLAYER_IDX].banner_id)];

    Set_Font_Colors_15(3, &colors[0]);

    Set_Font_Style_Shadow_Down(3, 15, 0, 0);

    Set_Outline_Color(254);

    Print(10, 23, _players[HUMAN_PLAYER_IDX].name);

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        if(_players[_human_player_idx].Dipl.Contacted[itr_players] == ST_TRUE)
        {
            colors[0] = banner_colors[(_players[itr_players].banner_id)];
            colors[1] = banner_colors[(_players[itr_players].banner_id)];
            colors[2] = banner_colors[(_players[itr_players].banner_id)];
            colors[3] = banner_colors[(_players[itr_players].banner_id)];
            colors[4] = banner_colors[(_players[itr_players].banner_id)];
            colors[5] = banner_colors[(_players[itr_players].banner_id)];

            Set_Font_Colors_15(3, &colors[0]);

            Set_Font_Style_Shadow_Down(3, 15, 0, 0);

            Set_Outline_Color(254);

            Print(10, (23 + (itr_players * 9)), _players[itr_players].name);
        }
    }

}


// WZD o76p05
// drake178: sub_65B47()
/*

*/
void IDK_Powergraph_Draw_Turn_Data(int16_t player_idx, int16_t turns, int16_t month)
{
    uint8_t banner_colors[5] = { 0,0,0,0,0 };
    int16_t var_B = 0;
    int16_t x2_itr = 0;
    int16_t turns_count = 0;
    int16_t y2 = 0;
    int16_t x2 = 00 ;
    int16_t x1 = 0 ;
    int16_t y1 = 0;  // _DI_
    int16_t itr_turn_data = 0;  // _SI_
    struct s_WIZARD * ptr_player = 0;
    uint8_t * ptr_Historian = 0;

    banner_colors[0] = BANNER_COLOR_BLUE;
    banner_colors[1] = BANNER_COLOR_GREEN;
    banner_colors[2] = BANNER_COLOR_PURPLE;
    banner_colors[3] = BANNER_COLOR_RED;
    banner_colors[4] = BANNER_COLOR_YELLOW;

    turns_count = turns;

    x2_itr = 0;

    for(itr_turn_data = month; itr_turn_data < turns_count; itr_turn_data++)
    {
        x2 = (16 + x2_itr);
        y2 = (181 - _players[player_idx].Historian[itr_turn_data]);  // 181 is 2 pixels up from the time-line
        ptr_player = &_players[player_idx];
        ptr_Historian = &_players[player_idx].Historian[0];

        SETMIN(y2, 22);
        if(itr_turn_data > month)
        {
            Line(x1, y1, x2, y2, banner_colors[_players[player_idx].banner_id]);
            x1 = x2;
            y1 = y2;
        }
        else
        {
            x1 = 16;
            y1 = (181 - _players[player_idx].Historian[0]);
            SETMIN(y1, 22);
        }
        x2_itr++;
    }

}

// WZD o76p06
// drake178: 
/*

*/
void Status_Screen(void)
{
    int16_t input_field_idx;
    int16_t full_screen_ESC_field;
    int16_t leave_screen;  // _SI_

    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);

    // RELOAD.LBX, 000  STATBACK
    // RELOAD.LBX, 001  STATUS
    status_screen_seg = LBX_Reload(reload_lbx_file__ovr076, 1, _screen_seg);

    Clear_Fields();
    Set_Page_Off();
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();
    PageFlip_FX();
    Set_Page_Off();

    Load_Palette_From_Animation(status_screen_seg);

    Set_Palette_Changes(0, 255);

    Update_Remap_Gray_Palette();

    Copy_Back_To_Off();

    Assign_Auto_Function(Status_Screen_Draw, 1);  // DEDU  looks weird

    Deactivate_Help_List();

    Set_Status_Screen_Help_List();

    status_screen_x = 20;
    status_screen_y = 11;

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_XMAX, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr076[0], ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == full_screen_ESC_field)
        {
            Play_Left_Click();
            leave_screen = ST_TRUE;
        }

        if(leave_screen == ST_FALSE)
        {
            Status_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
    }

    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Reset_Window();
    Clear_Fields();

}


// WZD o76p07
// drake178: sub_65D33()
/*

*/
void Status_Screen_Draw(void)
{
    char dest[4] = { 0, 0, 0, 0 };
    uint8_t banner_colors[6] = {0, 0, 0, 0, 0, 0};
    int16_t var_12 = 0;
    int16_t scores[3] = { 0, 0, 0 };
    int16_t itr_players = 0;
    int16_t var_8 = 0;
    uint8_t colors[6] = {0, 0, 0, 0, 0, 0};
    int16_t itr_scores = 0;  // _SI_
    int16_t IDK_y = 0;  // _DI_

    strcpy(dest, str_SPACE__ovr076);

    banner_colors[0] = 172;
    banner_colors[1] = 216;
    banner_colors[2] = 124;
    banner_colors[3] = 201;
    banner_colors[4] = 211;

    Set_Page_Off();

    FLIC_Draw(status_screen_x, status_screen_y, status_screen_seg);

    for(itr_players = 1; itr_players < NUM_PLAYERS; itr_players++)
    {
        colors[itr_players] = (177 + itr_players);
    }

    Set_Font_Colors_15(4, &colors[0]);

    Set_Font_Style_Shadow_Down(4, 15, 0, 0);

    Set_Alias_Color(246);

    Set_Outline_Color(254);

    Print_Centered((status_screen_x + 138), (status_screen_y + 10), str_CurrentStatusOfWiza);

    Set_Font_Style_Shadow_Down(2, 3, 0, 0);

    Set_Alias_Color(246);

    Set_Outline_Color(254);

    strcpy(GUI_String_1, str_Army__ovr076);

    strcat(GUI_String_1, dest);

    strcat(GUI_String_1, str_Strength__ovr076);

    Print_Centered((status_screen_x + 138), (status_screen_y + 23), GUI_String_1);

    strcpy(GUI_String_1, str_Magic__ovr076);

    strcat(GUI_String_1, dest);

    strcat(GUI_String_1, str_Power);

    Print_Centered((status_screen_x + 138), (status_screen_y + 72), GUI_String_1);

    strcpy(GUI_String_1, str_Spell__ovr076);

    strcat(GUI_String_1, dest);

    strcat(GUI_String_1, str_Research);

    Print_Centered((status_screen_x + 138), (status_screen_y + 121), GUI_String_1);

    Set_Font_Style_Shadow_Down(0, 15, 0, 0);

    Set_Alias_Color(246);

    Set_Outline_Color(254);

    var_12 = 0;
    
    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {

        if(_players[_human_player_idx].Dipl.Contacted[itr_players] == ST_TRUE)
        {
            var_8 = 35;

            scores[0] = _players[itr_players].Astr.Army_Strength;

            scores[1] = _players[itr_players].Astr.Magic_Power;

            scores[2] = _players[itr_players].Astr.Spell_Research;

            for(itr_scores = 0; itr_scores < 3; itr_scores++)
            {
                SETMAX(scores[itr_scores], 200);

                IDK_y = (var_12 * 7)+ (status_screen_y + var_8);

                Print((status_screen_x + 14), IDK_y, _players[itr_players].name);

                Gradient_Fill((status_screen_x + 64), (IDK_y + 2), (status_screen_x + 65 + scores[itr_scores]), (IDK_y + 3), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);

                Fill((status_screen_x + 64), (IDK_y + 1), (status_screen_x + 64 + scores[itr_scores]), (IDK_y + 2), banner_colors[_players[itr_players].banner_id]);

                var_8 += 49;
            }

            var_12++;
        }

    }

}


// WZD o76p08
void Cartographer_Screen__WIP(void)
{
    int16_t hotkey_escape = 0;
    int16_t cartograph_dot_grid = 0;
    int64_t grid_row = 0;     // int16_t in Dasm
    int64_t grid_column = 0;  // int16_t in Dasm
    int16_t hotkey_next = 0;
    int16_t hotkey_close = 0;
    int16_t leave_screen = 0;  // _SI_
    int16_t input_field_idx = 0;  // _DI_

    GUI_String_1 = (char *)Near_Allocate_First(100);

    Assign_Auto_Function(Cartographer_Screen_Draw__WIP, 1);

    Deactivate_Help_List();

    m_cartograph_plane = _map_plane;

    m_cartograph_x = 16;
    m_cartograph_y = 27;

    Set_Cartographer_Screen_Help_List();

    Cartograph_Screen_Draw_Map__WIP(0);

    Cartograph_Screen_Draw_Map__WIP(1);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        hotkey_next = Add_Hidden_Field(285, 180, 314, 199, str_hotkey_N__ovr076[0], ST_UNDEFINED);

        hotkey_close = Add_Hidden_Field(265, 143, 299, 169, str_hotkey_C__ovr076[0], ST_UNDEFINED);

        hotkey_escape = Add_Hot_Key(str_hotkey_ESC__ovr076[0]);

        /* MainScr.C#L2147  Main_Screen_Add_Fields() */  // _main_map_grid_field = Add_Grid_Field(MAP_SCREEN_X, MAP_SCREEN_Y, SQUARE_WIDTH, SQUARE_HEIGHT, MAP_WIDTH, MAP_HEIGHT, &_main_map_grid_x, &_main_map_grid_y, ST_UNDEFINED);
        cartograph_dot_grid = Add_Grid_Field(m_cartograph_x, m_cartograph_y, 4, 4, WORLD_WIDTH, WORLD_HEIGHT, &grid_column, &grid_row, ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_next)
        {
            Play_Left_Click();
            m_cartograph_plane = (1 - m_cartograph_plane);
        }

        if(
            (input_field_idx == hotkey_close)
            ||
            (input_field_idx == hotkey_escape)
        )
        {
            Play_Left_Click();
            leave_screen = ST_TRUE;
        }

        if(input_field_idx == cartograph_dot_grid)
        {

            leave_screen = ST_TRUE;

            _map_plane = m_cartograph_plane;

            _map_x = grid_column;

            _map_y = grid_row;

            _unit_stack_count = 0;

            /* MainScr.C@L1333  Main_Screen() */  // Center_Map(&_map_x, &_map_y, _UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp);
            assert(_map_x >= WORLD_XMIN && _map_x <= WORLD_XMAX);  /*  0 & 59 */
            assert(_map_y >= WORLD_YMIN && _map_y <= WORLD_YMAX);  /*  0 & 39 */
            Center_Map(&_map_x, &_map_y, grid_column, grid_row, _map_plane);

            Set_Draw_Active_Stack_Always();
            Set_Unit_Draw_Priority();
            Reset_Stack_Draw_Priority();

            Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

            Reset_Map_Draw();

        }
        
        if(leave_screen == ST_FALSE)
        {
            Cartographer_Screen_Draw__WIP();
            PageFlip_FX();
            Release_Time(1);
        }

    }

    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Reset_Window();
    Clear_Fields();
    _page_flip_effect = 2;

}


// WZD o76p09
/*
; loads the contents of either VGA frame 3 (Arcanus) or
; 4 (Myrror) into the current draw frame, and if the
; mouse cursor is over a visible city, draws its name
; over its square
*/
void Cartographer_Screen_Draw__WIP(void)
{
    int16_t banner_colors[NUM_BANNER_COLORS] = { 0, 0, 0, 0, 0, 0 };
    int16_t city_dot_y = 0;
    int16_t city_dot_x = 0;
    uint8_t colors [4] = { 0, 0, 0, 0 };
    int16_t l_my = 0;
    int16_t l_mx = 0;
    int16_t itr_cities = 0;  // _DI_

    banner_colors[0] = BANNER_COLOR_BLUE;
    banner_colors[1] = COL_Banners2[1];
    banner_colors[2] = COL_Banners2[2];
    banner_colors[3] = BANNER_COLOR_RED;
    banner_colors[4] = COL_Banners2[4];
    banner_colors[5] = COL_Banners2[5];

    Set_Page_Off();

    switch(m_cartograph_plane)
    {
        case 0:
        {
            Copy_Back_To_Off();
        } break;
        case 1:
        {
            Copy_Page4_To_Off();
        } break;
    }

    l_mx = Pointer_X();
    l_my = Pointer_Y();

    if(
        (l_mx < m_cartograph_x)
        ||
        (l_my < m_cartograph_y)
        ||
        ((m_cartograph_x + CARTOGRAPHER_MAP_WIDTH) < l_mx)
        ||
        ((m_cartograph_y + CARTOGRAPHER_MAP_HEIGHT) < l_my)
    )
    {
        return;
    }

    l_mx = ((l_mx - m_cartograph_x) / 4);

    l_my = ((l_my - m_cartograph_y) / 4);

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        if
        (
            (_CITIES[itr_cities].wp == m_cartograph_plane)
            &&
            (_CITIES[itr_cities].wx == l_mx)
            &&
            (_CITIES[itr_cities].wy == l_my)
        )
        {

            if(SQUARE_EXPLORED(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, _CITIES[itr_cities].wp))
            {

                city_dot_x = (m_cartograph_x + (l_mx * 4));

                city_dot_y = (m_cartograph_y + (l_my * 4));

                colors[0] = ST_BLACK;

                colors[1] = banner_colors[_players[_CITIES[itr_cities].owner_idx].banner_id];

                Set_Font_Colors_15(0, &colors[0]);

                Set_Outline_Color(1);

                Set_Font_Style_Outline(0, 15, 0, 0);

                Print_Centered((city_dot_x + 2), (city_dot_y - 6), _CITIES[itr_cities].name);

            }

        }

    }

}


// WZD o76p10
void Cartograph_Screen_Draw_Map__WIP(int16_t flag)
{
    int16_t banner_colors[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    SAMB_ptr mapflags_segs[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    uint8_t colors[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t var_8 = 0;
    int16_t y = 0;
    int16_t x = 0;
    SAMB_ptr worldmap_seg = 0;
    int16_t itr_legend = 0;  // _DI_
    int16_t itr_players = 0;  // _DI_
    int16_t itr_colors = 0;  // _DI_
    int16_t itr_nodes = 0;  // _DI_
    int16_t itr_lairs = 0;  // _DI_
    int16_t itr_towers = 0;  // _DI_
    int16_t itr_units = 0;  // _DI_
    int16_t itr_cities = 0;  // _DI_

    banner_colors[0] = BANNER_COLOR_BLUE;
    banner_colors[1] = COL_Banners2[1];
    banner_colors[2] = COL_Banners2[2];
    banner_colors[3] = BANNER_COLOR_RED;
    banner_colors[4] = COL_Banners2[4];
    banner_colors[5] = COL_Banners2[5];

    Set_Page_Off();

    // RELOAD.LBX, 002  WORLDMAP
    worldmap_seg = LBX_Reload(reload_lbx_file__ovr076, 2, _screen_seg);

    FLIC_Draw(0, 0, worldmap_seg);

    cartograph_seg = Allocate_First_Block(_screen_seg, 2500);  // 2500 PR, 40000 B  (16 + 16 + (240 * 160)) ?

    // RELOAD.LBX, 003  MAPFLAGS
    // RELOAD.LBX, 004  MAPFLAGS
    // RELOAD.LBX, 005  MAPFLAGS
    // RELOAD.LBX, 006  MAPFLAGS
    // RELOAD.LBX, 007  MAPFLAGS
    // RELOAD.LBX, 008  MAPFLAGS
    for(itr_legend = 0; itr_legend < NUM_PLAYERS; itr_legend++)
    {
        mapflags_segs[itr_legend] = LBX_Reload_Next(reload_lbx_file__ovr076, (3 + itr_legend), _screen_seg);
    }

    // TODO  Cartograph_Screen_Draw_Map_Terrain__NOWORKIE(flag, cartograph_seg);
    /* HACK */  Create_Picture(CARTOGRAPHER_MAP_WIDTH, CARTOGRAPHER_MAP_HEIGHT, (cartograph_seg + 16));  // (WORLD_WIDTH * 4) x (WORLD_HEIGHT * 4)
    Draw_Picture(m_cartograph_x, m_cartograph_y, (cartograph_seg + 16));

    Set_Font_Style(1, 1, 0, 0);

    Set_Alias_Color(238);

    Set_Outline_Color(254);

    Set_Font_Spacing_Width(1);

    var_8 = 0;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[_human_player_idx].Dipl.Contacted[itr_players] == ST_TRUE)
        {
            FLIC_Draw(260, (80 + (var_8 * 9)), mapflags_segs[_players[itr_players].banner_id]);
            Print(269, (82 + (var_8 * 9)), _players[itr_players].name);
            var_8++;
        }
    }

    FLIC_Draw(260, (80 + (var_8 * 9)), mapflags_segs[5]);

    Print(269, (82 + (var_8 * 9)), str_Neutral__ovr076);  // "Neutral"

    for(itr_colors = 1; itr_colors < 13; itr_colors++)
    {
        colors[itr_colors] = 1;
    }

    Set_Font_Colors_15(0, &colors[0]);

    Set_Alias_Color(25);

    Set_Font_Style_Outline(5, 15, 0, 0);

    Set_Alias_Color(25);

    Set_Outline_Color(239);

    if(flag == 0)
    {
        strcpy(GUI_String_1, str_Arcanus__ovr076);  // "Arcanus"
    }
    else
    {
        strcpy(GUI_String_1, str_Myrror__ovr076);  // "Myrror"
    }

    strcat(GUI_String_1, str_Plane__ovr076);  // " Plane"

    Print_Centered(160, 12, GUI_String_1);

    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {
        if(_NODES[itr_nodes].wp == flag)
        {
            if(GET_SQUARE_EXPLORED(_NODES[itr_nodes].wx, _NODES[itr_nodes].wy, _NODES[itr_nodes].wp) != UNEXPLORED)
            {
                x = (m_cartograph_x + (_NODES[itr_nodes].wx * 4) + 2);
                y = (m_cartograph_y + (_NODES[itr_nodes].wy * 4) + 1);
                Dot(x, y, ST_LIGHT_GRAY);
            }
        }
    }

    for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
    {
        if(_LAIRS[itr_lairs].wp == flag)
        {
            if(_LAIRS[itr_lairs].Intact == ST_TRUE)
            {
                if(GET_SQUARE_EXPLORED(_LAIRS[itr_lairs].wx, _LAIRS[itr_lairs].wy, _LAIRS[itr_lairs].wp) != UNEXPLORED)
                {
                    x = (m_cartograph_x + (_LAIRS[itr_lairs].wx * 4) + 2);
                    y = (m_cartograph_y + (_LAIRS[itr_lairs].wy * 4) + 1);
                    Dot(x, y, ST_LIGHT_GRAY);
                }
            }
        }
    }

    for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
    {
        if(GET_SQUARE_EXPLORED(_TOWERS[itr_towers].wx, _TOWERS[itr_towers].wy, flag) != UNEXPLORED)
        {
            x = (m_cartograph_x + (_TOWERS[itr_towers].wx * 4) + 2);
            y = (m_cartograph_y + (_TOWERS[itr_towers].wy * 4) + 1);
            Dot(x, y, ST_LIGHT_GRAY);
        }
    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].wp == flag)
            ||
            (_UNITS[itr_units].in_tower == ST_TRUE)
        )
        {
            if(Check_Square_Scouted(_UNITS[itr_units].wx, _UNITS[itr_units].wy, _UNITS[itr_units].wp) == ST_TRUE)
            {
                x = (m_cartograph_x + (_UNITS[itr_units].wx * 4) + 2);
                y = (m_cartograph_x + (_UNITS[itr_units].wy * 4) + 1);
                if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
                {
                    if(_UNITS[itr_units].owner_idx == HUMAN_PLAYER_IDX)
                    {
                        Dot(x, y, banner_colors[_players[_UNITS[itr_units].owner_idx].banner_id]);
                    }
                    else if(_UNITS[itr_units].owner_idx == NEUTRAL_PLAYER_IDX)
                    {
                        // nada, nunca, zero, ... jumps to end of loops
                    }
                    else
                    {
                        if(Unit_Has_Invisibility(itr_units) != ST_TRUE)
                        {
                            Dot(x, y, banner_colors[_players[_UNITS[itr_units].owner_idx].banner_id]);
                        }
                    }
                }
            }
        }
    }

    var_8 = 0;  // ¿ ST_FALSE ?
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(
            (_CITIES[itr_cities].wp == flag)
            ||
            (_CITIES[itr_cities].wp == 2)  /* IDGI, but *means* city is *accessible* from both planes/either plane */
        )
        {
            if(GET_SQUARE_EXPLORED(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, flag) != UNEXPLORED)
            {
                x = (m_cartograph_x + (_CITIES[itr_cities].wx * 4));
                y = (m_cartograph_x + (_CITIES[itr_cities].wy * 4));
                Fill((x + 1), (y + 1), (x + 3), (y + 3), ST_BLACK);
                if(_CITIES[itr_cities].owner_idx != NEUTRAL_PLAYER_IDX)
                {
                    Fill(x, y, (x + 2), (y + 2), banner_colors[_players[_CITIES[itr_cities].owner_idx].banner_id]);
                }
                else
                {
                    /*  ovr076:1CFF  B8 10 00    mov     ax, 10h */
                    Fill(x, y, (x + 2), (y + 2), ST_BROWN);
                    var_8 = 1;  // ¿ ST_TRUE ?
                    var_8 = LO_BYTE(var_8);
                }
            }
        }
    }

    switch(flag)
    {
        case ARCANUS_PLANE:
        {
            Copy_Off_To_Back();
        } break;
        case MYRROR_PLANE:
        {
            Copy_Off_To_Page4();
        } break;
    }

}


// WZD o76p11
void Grand_Vizier_Window(void)
{
    if(grand_vizier != ST_FALSE)
    {

        grand_vizier = ST_FALSE;

    }
    else
    {

        if(Confirmation_Box(aDoYouWishToAllowThe) == ST_TRUE)
        {

            grand_vizier = ST_TRUE;

        }

    }
}
