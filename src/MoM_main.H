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
    scr_Main_Menu_Screen = 0,

    scr_Continue = 10,
    scr_Load_Screen = 11,
    scr_New_Game_Screen = 12,
    scr_Quit_To_DOS = 13,
    scr_Hall_Of_Fame_Screen = 14,
    scr_Settings_Screen = 15,

    scr_City_Screen = 100,
    /* scr_Load_Screen = 101, */
    scr_Armies_Screen = 102,
    scr_Cities_Screen = 103,
    scr_Quit = 104,
    scr_Main_Screen = 105,
    scr_Magic_Screen = 106,
    scr_RoadBuilding = 107,  /* ?!? */
    scr_Production = 108,
    scr_Item_Screen = 109,
    scr_NextTurn = 110,  /* ? drake187: NextTurnProc ? MoO2: Do_Next_Turn_() ? */
    /* ?default? */
    scr_Spellbook = 112,
    /* ?default? */
    scr_Advisor_Screen = 114,
    scr_Diplomac = 115  /* ?!? */
    // ?!? Surveyor, Cartographer, etc.
};


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


extern int16_t PageFlipEffect;

extern uint8_t g_State_Run;

extern int16_t g_Current_Screen;

// extern uint8_t g_Video_Back_Buffer[];
extern uint8_t g_Palette[];
extern uint8_t g_Palette_XBGR[];



/*
    ~== MAGIC/WIZARDS.EXE |-> main()
*/
void MoM_main(void);


/*
    WIZARDS.EXE seg001
*/

// WZD s01p02
void Screen_Control(void);

// WZD s01p03
void PageFlip_FX(void);


void MoM_Tables_Init(int16_t gfx_buff_nparas);

// WZD s01p06
void WZD_Startup_MainGame(void);


/*
    MAGIC.EXE seg001
*/

// MGC s01p07
// Load_TERRSTAT

// MGC s01p08
// Load_SPELLDAT


/*
    WIZARDS.EXE  ovr052
*/

// WZD o51p01
void GAME_Overland_Init(void);

// WZD o51p02
void G_WLD_StaticAssetRfrsh(void);


/*
    WIZARDS.EXE  ovr052
*/

// WZD o52p01
void Load_Init_MainGame(void);

// WZD o52p02
void UNIT_Upkeep_Reset(void);

// WZD o52p03
void Terrain_Init(void);

// WZD o52p20
void GFX_Swap_Reset(void);

// WZD o52p22
// AKA GFX_Swap_AppendUnits()
void Load_Unit_StatFigs(void);
// WZD o52p30
void GFX_Swap_Cities(void);


/*
    WIZARDS.EXE  ovr063
*/

// WZD o63p15
int16_t Check_Planar_Seal(void);

/*
    WIZARDS.EXE  ovr067
*/

// WZD o67p15
int16_t IsPassableTower(int16_t world_x, int16_t world_y);


#ifdef __cplusplus
}
#endif


#endif  /* MOM_MAIN_H */
