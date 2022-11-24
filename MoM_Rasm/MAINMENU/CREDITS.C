
#include "CREDITS.H"



/*
    Initialized Data
*/

// MAINMENU CREDITS  int GAME_FirstShownCred = 0;            // dseg:38D8
// MAINMENU CREDITS  int GAME_CreditTextTop = 0;             // dseg:38DA
// MAINMENU CREDITS  int GAME_CredsRollTimer = 0;            // dseg:38DC

// char TBL_Credits_Texts[48][] = {
//     {"Game Designer"},  {"Steve Barcia"},
//     {"Programmers"},    {""},
//     {""},               {"Jim Cowlishaw',"},
//     {""},               {"Ken Burd',"},
//     {""},               {"Grissel Barcia"}
// };

// MAINMENU CREDITS  
// struct s_CreditsPairs TBL_Credits_Texts[48] = {
//     {"Game Designer",   ""},
//     {"",                "Steve Barcia"},        
//     {"Programmers",     ""},
//     {"",                "Jim Cowlishaw',"},
//     {"",                "Ken Burd',"},
//     {"",                "Grissel Barcia"},
//     {"Producer",        ""},
//     {"",                "Doug Caspian-Kaufman"},
//     {"Art Director",    ""},
//     {"",                "Jeff Dee"},
//     {"Artists",         ""},
//     {"",                "Shelly Hollen"},
//     {"",                "Amanda Dee"},
//     {"",                "Steve Austin"},
//     {"",                "George Purdy"},
//     {"",                "Patrick Owens"},
//     {"Music Producer",  ""},
//     {"",                "The Fat Man"},
//     {"Composer",        ""},
//     {"",                "Dave Govett"},
//     {"QA Lead",         ""},
//     {"",                "Destin Strader"},
//     {"Play Test",       ""},
//     {"",                "Mike Balogh"},
//     {"",                "Damon Harris"},
//     {"",                "Geoff Gessner"},
//     {"",                "Tammy Talbott"},
//     {"",                "Mick Uhl"},
//     {"",                "Jim Hendry"},
//     {"",                "Frank Brown"},
//     {"",                "Jim Tricario"},
//     {"",                "Jen MacLean"},
//     {"",                "Brian Wilson"},
//     {"",                "Brian Helleson"},
//     {"",                "Jeff Dinger"},
//     {"",                "Chris Bowling"},
//     {"",                "Charles Brubaker"},
//     {"",                "Tom Hughes"},
//     {"Sound Effects",   ""},
//     {""                 "Midian"},
//     {"Speech",          ""},
//     {"",                "Mark Reis"},
//     {"",                "Peter Woods"},
//     {"",                "David Ellis"},
//     {"Manual",          ""},
//     {"",                "Petra Schlunk"},
//     {"Special Thanks",  ""},
//     {"",                "Jenny Cowlishaw"}
// };

// dseg:38DE TBL_Credits_Texts CreditsPairs <offset cnst_Credits_00, offset cnst_Credits_01>; 0 ; could use dseg:2947 or the other way around
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 1
// dseg:38DE CreditsPairs <offset cnst_Credits_20, offset cnst_Credits_21>; 2
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_31>; 3
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_01>; 4
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_51>; 5
// dseg:38DE CreditsPairs 2 dup(<offset cnst_ZeroString_6, offset cnst_ZeroString_6>); 6
// dseg:38DE CreditsPairs <offset cnst_Credits_80, offset cnst_Credits_81>; 8
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 9
// dseg:38DE CreditsPairs <offset cnst_Credits_100, offset cnst_Credits_101>; 10
// dseg:38DE CreditsPairs <offset cnst_Credits_110, offset cnst_Credits_111>; 11
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_121>; 12
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_131>; 13
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_141>; 14
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_151>; 15
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_51>; 16
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 17
// dseg:38DE CreditsPairs <offset cnst_Credits_180, offset cnst_Credits_181>; 18
// dseg:38DE CreditsPairs <offset cnst_Credits_190, offset cnst_Credits_191>; 19
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 20
// dseg:38DE CreditsPairs <offset cnst_Credits_210, offset cnst_Credits_211>; 21
// dseg:38DE CreditsPairs <offset cnst_Credits_220, offset cnst_Credits_221>; 22
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_231>; 23
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_241>; 24
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_251>; 25
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_261>; 26
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_271>; 27
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_281>; 28
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_291>; 29
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_301>; 30
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_311>; 31
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_321>; 32
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_331>; 33
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_341>; 34
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_351>; 35
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_361>; 36
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 37
// dseg:38DE CreditsPairs <offset cnst_Credits_380, offset cnst_Credits_381>; 38
// dseg:38DE CreditsPairs <offset cnst_Credits_390, offset cnst_Credits_391>; 39
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_401>; 40
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_411>; 41
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 42
// dseg:38DE CreditsPairs <offset cnst_Credits_430, offset cnst_Credits_431>; 43
// dseg:38DE CreditsPairs 2 dup(<offset cnst_ZeroString_6, offset cnst_ZeroString_6>); 44
// dseg:38DE CreditsPairs <offset cnst_Credits_460, offset cnst_Credits_461>; 46
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 47

// dseg:399E cnst_Credits_00 db 'Game Designer'
// dseg:39AB cnst_ZeroString_6 db 0
// dseg:39AC cnst_Credits_01 db 'Steve Barcia',0
// dseg:39B9 cnst_Credits_20 db 'Programmers',0
// dseg:39C5 cnst_Credits_21 db 'Jim Cowlishaw',0
// dseg:39D3 cnst_Credits_31 db 'Ken Burd',0
// dseg:39DC cnst_Credits_51 db 'Grissel Barcia',0
// dseg:39EB cnst_Credits_80 db 'Producer',0
// dseg:39F4 cnst_Credits_81 db 'Doug Caspian-Kaufman',0
// dseg:3A09 cnst_Credits_100 db 'Art Director',0
// dseg:3A16 cnst_Credits_101 db 'Jeff Dee',0
// dseg:3A1F cnst_Credits_110 db 'Artists',0
// dseg:3A27 cnst_Credits_111 db 'Shelly Hollen',0
// dseg:3A35 cnst_Credits_121 db 'Amanda Dee',0
// dseg:3A40 cnst_Credits_131 db 'Steve Austin',0
// dseg:3A4D cnst_Credits_141 db 'George Purdy',0
// dseg:3A5A cnst_Credits_151 db 'Patrick Owens',0
// dseg:3A68 cnst_Credits_180 db 'Music Producer',0
// dseg:3A77 cnst_Credits_181 db 'The Fat Man',0
// dseg:3A83 cnst_Credits_190 db 'Composer',0
// dseg:3A8C cnst_Credits_191 db 'Dave Govett',0
// dseg:3A98 cnst_Credits_210 db 'QA Lead',0
// dseg:3AA0 cnst_Credits_211 db 'Destin Strader',0
// dseg:3AAF cnst_Credits_220 db 'Play Test',0
// dseg:3AB9 cnst_Credits_221 db 'Mike Balogh',0
// dseg:3AC5 cnst_Credits_231 db 'Damon Harris',0
// dseg:3AD2 cnst_Credits_241 db 'Geoff Gessner',0
// dseg:3AE0 cnst_Credits_251 db 'Tammy Talbott',0
// dseg:3AEE cnst_Credits_261 db 'Mick Uhl',0
// dseg:3AF7 cnst_Credits_271 db 'Jim Hendry',0
// dseg:3B02 cnst_Credits_281 db 'Frank Brown',0
// dseg:3B0E cnst_Credits_291 db 'Jim Tricario',0
// dseg:3B1B cnst_Credits_301 db 'Jen MacLean',0
// dseg:3B27 cnst_Credits_311 db 'Brian Wilson',0
// dseg:3B34 cnst_Credits_321 db 'Brian Helleson',0
// dseg:3B43 cnst_Credits_331 db 'Jeff Dinger',0
// dseg:3B4F cnst_Credits_341 db 'Chris Bowling',0
// dseg:3B5D cnst_Credits_351 db 'Charles Brubaker',0
// dseg:3B6E cnst_Credits_361 db 'Tom Hughes',0
// dseg:3B79 cnst_Credits_380 db 'Sound Effects',0   ; could use dseg:2947 or the other way around
// dseg:3B87 cnst_Credits_381 db 'Midian',0
// dseg:3B8E cnst_Credits_390 db 'Speech',0
// dseg:3B95 cnst_Credits_391 db 'Mark Reis',0
// dseg:3B9F cnst_Credits_401 db 'Peter Woods',0
// dseg:3BAB cnst_Credits_411 db 'David Ellis',0
// dseg:3BB7 cnst_Credits_430 db 'Manual',0
// dseg:3BBE cnst_Credits_431 db 'Petra Schlunk',0
// dseg:3BCC cnst_Credits_460 db 'Special Thanks',0
// dseg:3BDB cnst_Credits_461 db 'Jenny Cowlishaw',0

// MAINMENU CREDITS  char cnst_Credits_DOT = '.'; // dseg:3BEB
// dseg:3BED align 2


/*
    Uninitialized Data
*/

// MAINMENU CREDITS  unsigned int gsa_GUI_Credits_Text_IMG; // dseg:948C











// _o60p01c.c GAME_DrawCredits
// MGC_DEF.H

#include "MGC_DEF.H"

#include "seg028.H"     /* FLIC_Prepare() */


// ./MAINMENU/CREDITS.C/.H
// void Draw_Credits(void)  // Main Menu Screen
int GAME_DrawCredits(void)
{

    int Role_String_Width;
    int Loop_Var;
    unsigned char Credit_FadeIn_Colors[10];
    unsigned char Credit_FadeOut_Colors[10];
    unsigned char Color_Array[4];

    int tmp_SI;
    int tmp_DI;

    GAME_CredsRollTimer = GAME_CredsRollTimer + 1;

    if ( GAME_CredsRollTimer > 30000 )
    {
        GAME_CredsRollTimer = 70;
    }

    if ( GAME_CredsRollTimer < 70 )
    {
        return 0;
    }

    Color_Array[0] = 68;  // 0x44

    for ( Loop_Var = 0; Loop_Var < 10; Loop_Var++ )
    {
        Credit_FadeOut_Colors[Loop_Var] = 224 + Loop_Var;
    }

    for ( Loop_Var = 0; Loop_Var < 10; Loop_Var++ )
    {
        Credit_FadeOut_Colors[Loop_Var] = 233 - Loop_Var;
    }

    Credit_FadeIn_Colors[0] = 232;

    // NOTE(JimBalcomb,20221124): This is the only use of FLIC_Prepare() in MGC. It is used regularly in WZD.
    FLIC_Prepare(280, 122, gsa_GUI_Credits_Text_IMG);  // MGC s28p02  seg028.H

    VGA_SetDrawWindow(0, 40, 319, 137);

    tmp_SI = GAME_CreditTextTop;

    for ( Loop_Var = 0; Loop_Var < 10; Loop_Var++ )
    {
        
        if ( tmp_SI < 99 )
        {

            if ( GAME_FirstShownCred + Loop_Var < 48 )
            {

                Color_Array[1] = 224;

                if ( tmp_SI < 10 )
                {
                    Color_Array[1] = Credit_FadeOut_Colors[10 - tmp_SI];
                }

                if ( tmp_SI > 88 )
                {
                    Color_Array[1] = Credit_FadeIn_Colors[10 - (tmp_SI + -88)];
                }

                //VGA_SetFontnColor(2, &Color_Array);

                //LBX_DrawTextLine(0, tmp_SI, TBL_Credits_Texts[GAME_FirstShownCred + Loop_Var].Role, gsa_GUI_Credits_Text_IMG);

                Role_String_Width = VGA_GetStringWidth(TBL_Credits_Texts[GAME_FirstShownCred + Loop_Var].Role);

                if ( Role_String_Width > 1 )
                {
                    for ( tmp_DI = Role_String_Width + 1; tmp_DI < 102; tmp_DI += 4 )
                    {
                        //LBX_DrawTextLine(tmp_DI, tmp_SI, cnst_Credits_DOT, gsa_GUI_Credits_Text_IMG);
                    }
                }

                //LBX_DrawTextLine(105, tmp_SI, TBL_Credits_Texts[GAME_FirstShownCred + Loop_Var].Name, gsa_GUI_Credits_Text_IMG);
                tmp_SI = tmp_SI + 10;

            }
        }

    }

    GAME_CreditTextTop = GAME_CreditTextTop - 1;

    if ( GAME_CreditTextTop < 1 )
    {
        GAME_FirstShownCred = GAME_FirstShownCred + 1;
        GAME_CreditTextTop = GAME_CreditTextTop + 10;

        if ( GAME_FirstShownCred > 48 )
        {
            GAME_CredsRollTimer = 0;
            GAME_FirstShownCred = 0;
            GAME_CreditTextTop = 95;
        }
    }

    //VGA_WndOverlayLBX(85, 35, gsa_GUI_Credits_Text_IMG);
    VGA_ResetDrawWindow();

}




// _o60p02c.c GAME_PrepareCredits
// MGC_DEF.H

#include "MGC_DEF.H";
#include "MOM_DEF.H";
#include "ST_FLIC.H";
#include "ST_GUI.H";
#include "ST_SA.H"

// ./MAINMENU/CREDITS.C/.H
// void Load_Credits(void)  // Main Menu Screen
void GAME_PrepareCredits(void)
{
    GAME_FirstShownCred = 0;
    GAME_CreditTextTop = 95;
    gsa_GUI_Credits_Text_IMG = FP_SEG(SA_Alloc_First(MK_FP(gsa_Sandbox,0), 3125));
    FLIC_Prepare(280, 122, gsa_GUI_Credits_Text_IMG);
}
