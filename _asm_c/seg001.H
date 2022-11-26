#ifndef SEG001_H
#define SEG001_H


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg001
*/

// s01p01      MGC_MAIN.C
int MGC_Main(int argc, char *argv[]);

// s01p03
void GAME_MainMenu(void);

// s01p04
void GAME_LoadMainImages(void);

// s01p05
int SCREEN_Menu(void);

// s01p06
void SCREEN_Menu_Draw(void);

// s01p07
void GAME_Load_TERRSTAT_0(void);

// s01p08
void GAME_Load_SPELLDAT_0(void);


#ifdef __cplusplus
}
#endif


#endif /* SEG001_H */
