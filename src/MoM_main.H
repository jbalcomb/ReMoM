#ifndef MOM_MAIN_H
#define MOM_MAIN_H


/*
    DNE
        Main Menu, Settings
    MGC
        {0,1,2,3,4} == {Continue,Load_Game,New_Game,Quit_To_DOS,Hall_Of_Fame}
    WZD
        {0,1,2,3,4,5,6,7,8,9,10,11/13,12,11/13,14,15} == {City,Load,Armies,Cities,Quit,MainGame,Magic,RoadBuilding,Production,Items,NextTurn,?default?,Spellbook,?default?,Advisor,Diplomacy}
*/
enum e_SCREENS
{
    scr_Main_Menu = 0,

    scr_Continue = 10,
    scr_Load_Game = 11,
    scr_New_Game = 12,
    scr_Quit_To_DOS = 13,
    scr_Hall_Of_Fame = 14,
    scr_Settings = 15,

    scr_City = 100,
    scr_Load = 101,
    scr_Armies = 102,
    scr_Cities = 103,
    scr_Quit = 104,
    scr_MainGame = 105,
    scr_Magic = 106,
    scr_RoadBuilding = 107,
    scr_Production = 108,
    scr_Items = 109,
    scr_NextTurn = 110,
    /* ?default? */
    scr_Spellbook = 112,
    /* ?default? */
    scr_Advisor = 114,
    scr_Diplomac = 115

};


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


extern uint8_t g_State_Run;

extern int16_t g_Current_Screen;

extern uint8_t g_Video_Back_Buffer[];
extern uint8_t g_Palette_XBGR[];


void Screen_Control(void);

void MoM_main(void);


#ifdef __cplusplus
}
#endif


#endif /* MOM_MAIN_H */
