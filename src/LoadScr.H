/*
    WIZARDS.EXE
        ovr160
    MAGIC.EXE
        ovr050

WZD
    Load_Screen
    Load_Screen_Draw
    Set_Load_Screen_Help_List
    Loaded_Game_Update
MGC
    Load_Screen
    Load_Screen_Draw

MoO2  Module: LOADSAVE

*/

#ifndef LOADSCR_H
#define LOADSCR_H

#ifdef __cplusplus
extern "C" {
#endif




/*
    WIZARDS.EXE  ovr051
*/

// WZD o51p01
// OON XREF: Loaded_Game_Update()
void GAME_Overland_Init(void);

// WZD o51p02
// OON XREF: Loaded_Game_Update() |-> GAME_Overland_Init()
void G_WLD_StaticAssetRfrsh(void);



/*
    WIZARDS.EXE ovr101
*/

// WZD o101p01  [1 of 1]
void GAME_NextHumanStack(void);



/*
    WIZARDS.EXE ovr160
*/

// WZD o160p01
void Load_Screen(void);

// WZD o160p02
void Load_Screen_Draw(void);

// WZD o160p03
void Set_Load_Screen_Help_List(void);

// WZD o160p04
void Loaded_Game_Update(void);




#ifdef __cplusplus
}
#endif

#endif  /* LOADSCR_H */
