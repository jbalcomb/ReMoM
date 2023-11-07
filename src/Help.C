/*
    WIZARDS.EXE
        ovr104


HELP.LBX
HLPENTRY.LBX

MoO2
    Module: GENDRAW
    Module: EVANHELP
    ...
    ¿ Per-Screen Modules ?

*/

#include "MoX.H"



/*
    WIZARDS.EXE  ovr104
*/

// WZD o104p01
// HLPENTRY.LBX, 1  "main scrn Help"
void Set_Main_Screen_Help_List(void)
{
    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(hlpentry_lbx_file, 1, (SAMB_ptr)_help_entries, 0, 25, 10);
    Set_Help_List(_help_entries, 25);
}


// WZD o104p02
// HLPENTRY.LBX  ""
void Set_ArmyList_Screen_Help(void)
{
    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(hlpentry_lbx_file, 2, (SAMB_ptr)_help_entries, 0, 16, 10);
    Set_Help_List(_help_entries, 16);
}


// WZD o104p03
// HLPENTRY.LBX  ""
// drake178: HLP_Load_ItemsScreen()
void Set_Items_Screen_Help_List(void)
{
    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(hlpentry_lbx_file, 3, (SAMB_ptr)_help_entries, 0, 23, 10);
    Set_Help_List(_help_entries, 23);
}


// WZD o104p04
// HLPENTRY.LBX  ""
void Set_CityList_Screen_Help(void)
{
    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(hlpentry_lbx_file, 10, (SAMB_ptr)_help_entries, 0, 10, 10);
    Set_Help_List(_help_entries, 10);

}


// WZD o104p05
// HLPENTRY.LBX  ""
void Set_Magic_Screen_Help_List(void)
{
    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(hlpentry_lbx_file, 5, (SAMB_ptr)_help_entries, 0, 47, 10);
    Set_Help_List(_help_entries, 47);
}


// WZD o104p06
// HLPENTRY.LBX  ""

// WZD o104p07
// HLPENTRY.LBX  ""

// WZD o104p08
// HLPENTRY.LBX  ""

// WZD o104p09
// HLPENTRY.LBX  ""

// WZD o104p10
// HLPENTRY.LBX  ""


// WZD o104p11
// HLPENTRY.LBX  ""
void Set_Alchemy_Screen_Help_List(void)
{
    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(hlpentry_lbx_file, 11, (SAMB_ptr)_help_entries, 0, 6, 10);
    Set_Help_List(_help_entries, 6);
}


// WZD o104p12
// HLPENTRY.LBX  ""

// WZD o104p13
// HLPENTRY.LBX  ""

// WZD o104p14
// HLPENTRY.LBX  ""

// WZD o104p15
// HLPENTRY.LBX  ""

// WZD o104p16
// HLPENTRY.LBX  ""

// WZD o104p17
// HLPENTRY.LBX  ""

// WZD o104p18
// HLPENTRY.LBX  ""

// WZD o104p19
// HLPENTRY.LBX  ""

// WZD o104p20
// HLPENTRY.LBX  ""

// WZD o104p21
// HLPENTRY.LBX  ""

// WZD o104p22
// HLPENTRY.LBX, 38  "Tax Help"
void Set_TaxCollector_Window_Help_List(void)
{
    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(hlpentry_lbx_file, 38, (SAMB_ptr)_help_entries, 0, 1, 10);
    Set_Help_List(_help_entries, 1);
}


// WZD o104p23
// HLPENTRY.LBX, 39  "advisor Help"
void Set_Advisor_Screen_Help_List(void)
{
    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(hlpentry_lbx_file, 39, (SAMB_ptr)_help_entries, 0, 1, 10);
    Set_Help_List(_help_entries, 1);
}


// WZD o104p24
// HLPENTRY.LBX, 40  "score Help"
// AKA HLP_Load_HallofFame
void HLP_Load_HallofFame(void)
{
    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(hlpentry_lbx_file, 27, (SAMB_ptr)_help_entries, 0, 6, 10);
    Set_Help_List(_help_entries, 27);  // TODO  ¿ bug - someone put entry_num instead of help_count ?
}
