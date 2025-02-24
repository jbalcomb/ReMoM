#ifndef MOM_SCR_H
#define MOM_SCR_H

/*
    DNE
        Main Menu, Settings
    MGC
        {0,1,2,3,4} == {Continue,Load_Game,New_Game,Quit_To_DOS,Hall_Of_Fame}
    WZD
        {0,1,2,3,4,5,6,7,8,9,10,11/13,12,11/13,14,15} == {City,Load,Armies,Cities,Quit,MainGame,Magic,RoadBuilding,Production,Items,NextTurn,¿default?,Spellbook,¿default?,Advisor,Diplomacy}
*/
enum e_SCREENS
{
    scr_Main_Menu_Screen    = 0,

    scr_Continue            = 10,
    scr_Load_Screen         = 11,
    scr_New_Game_Screen     = 12,
    scr_Quit_To_DOS         = 13,
    scr_Hall_Of_Fame_Screen = 14,
    scr_Settings_Screen     = 15,

    /* WZD */
    scr_City_Screen         = 100,
    /* scr_Load_Screen = 101, */
    scr_Armies_Screen       = 102,
    scr_Cities_Screen       = 103,
    scr_Quit                = 104,
    scr_Main_Screen         = 105,
    scr_Magic_Screen        = 106,
    scr_Road_Build          = 107,
    scr_Production_Screen   = 108,  /* Production Pop-Up - ¿ from the City List Screen and City Screen ? */
    scr_Item_Screen         = 109,
    scr_NextTurn            = 110,  /* ¿ drake187: NextTurnProc ? ¿ MoO2: Do_Next_Turn_() ? */
    /* ¿default? */
    scr_Spellbook_Screen    = 112,
    /* ¿default? */
    scr_Advisor_Screen      = 114,   /* WZD 0x0E */
    scr_Diplomacy_Screen    = 115,
    // ¿!? Surveyor, Cartographer, etc.

    scr_Test_Screen         = 200,

    scr_PoC_Screen          = 240
};




#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE seg001
*/

// WZD s01p02
// OON XREF:  main()  WZD s01p02
void Screen_Control(void);



#ifdef __cplusplus
}
#endif

#endif /* MOM_SCR_H */
