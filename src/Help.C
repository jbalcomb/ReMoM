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
    Set_Help_List((char *)&_help_entries[0], 25);
}


// WZD o104p02
// HLPENTRY.LBX  ""
void Set_ArmyList_Screen_Help(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 2, (SAMB_ptr)_help_entries, 0, 16, 10);
    Set_Help_List((char *)&_help_entries[0], 16);
}


// WZD o104p03
// HLPENTRY.LBX  ""
void Set_Items_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 3, (SAMB_ptr)_help_entries, 0, 23, 10);
    Set_Help_List((char *)&_help_entries[0], 23);
}


// WZD o104p04
// HLPENTRY.LBX  ""
void Set_CityList_Screen_Help(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 10, (SAMB_ptr)_help_entries, 0, 10, 10);
    Set_Help_List((char*)&_help_entries[0], 10);
}


// WZD o104p05
// HLPENTRY.LBX  ""
void Set_Magic_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 5, (SAMB_ptr)_help_entries, 0, 47, 10);
    Set_Help_List((char *)&_help_entries[0], 47);
}


// WZD o104p06
// HLPENTRY.LBX, 6  "city scrn Help"
void Set_City_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 6, (SAMB_ptr)_help_entries, 0, 22, 10);
    Set_Help_List((char *)&_help_entries[0], 22);
}

// WZD o104p07
// HLPENTRY.LBX  "wizview scrn Help"
void Set_Mirror_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 6, (SAMB_ptr)_help_entries, 0, 18, 10);
    Set_Help_List((char *)&_help_entries[0], 18);
}

// WZD o104p08
// HLPENTRY.LBX  ""

// WZD o104p09
// HLPENTRY.LBX  ""

// WZD o104p10
// HLPENTRY.LBX  ""


// WZD o104p11
// HLPENTRY.LBX  "alchemy scrn Help"
void Set_Alchemy_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 11, (SAMB_ptr)_help_entries, 0, 6, 10);
    Set_Help_List((char *)&_help_entries[0], 6);
}


// WZD o104p12
// HLPENTRY.LBX  "build Help"
void Set_Production_Screen_Help(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 12, (SAMB_ptr)_help_entries, 0, 43, 10);
    Set_Help_List((char *)&_help_entries[0], 43);
}


// WZD o104p13
// HLPENTRY.LBX, 13  "view Help"
// HLPENTRY.LBX, 22  "combat view Help"
void Set_Unit_View_Help(int16_t uv_type)
{
    if(uv_type == uvt_Cmbt)
    {
        LBX_Load_Data_Static(hlpentry_lbx_file, 22, (SAMB_ptr)_help_entries, 0, 23, 10);
    }
    else
    {
        LBX_Load_Data_Static(hlpentry_lbx_file, 13, (SAMB_ptr)_help_entries, 0, 23, 10);
    }
    Set_Help_List((char *)&_help_entries[0], 23);
}


// WZD o104p14
// HLPENTRY.LBX, 14  "enemy city Help"
void Set_Enemy_City_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 14, (SAMB_ptr)_help_entries, 0, 11, 10);
    Set_Help_List((char *)&_help_entries[0], 11);
}

// WZD o104p15
// HLPENTRY.LBX  ""

// WZD o104p16
// HLPENTRY.LBX  ""

// WZD o104p17
// HLPENTRY.LBX  ""
void Set_Outpost_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 17, (SAMB_ptr)_help_entries, 0, 4, 10);
    Set_Help_List((char *)&_help_entries[0], 4);
}

// WZD o104p18
// HLPENTRY.LBX  ""

// WZD o104p19
// HLPENTRY.LBX  "road Help"
void Set_Road_Build_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 19, (SAMB_ptr)_help_entries, 0, 13, 10);
    Set_Help_List((char *)&_help_entries[0], 13);
}

// WZD o104p20
// HLPENTRY.LBX  ""

// WZD o104p21
// HLPENTRY.LBX  ""

// WZD o104p22
// HLPENTRY.LBX, 38  "Tax Help"
void Set_TaxCollector_Window_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 38, (SAMB_ptr)_help_entries, 0, 1, 10);
    Set_Help_List((char *)&_help_entries[0], 1);
}


// WZD o104p23
// HLPENTRY.LBX, 39  "advisor Help"
void Set_Advisor_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 39, (SAMB_ptr)_help_entries, 0, 1, 10);
    Set_Help_List((char *)&_help_entries[0], 1);
}


// WZD o104p24
// HLPENTRY.LBX, 40  "score Help"
void HLP_Load_HallofFame(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 27, (SAMB_ptr)_help_entries, 0, 6, 10);
    Set_Help_List((char *)&_help_entries[0], 27);  // TODO  ¿ bug - someone put entry_num instead of help_count ?
}
