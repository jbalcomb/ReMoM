#ifndef MOX_MOM_H
#define MOX_MOM_H


#define EMM_PAGES_REQUIRED 158
#define EMM_MIN_KB 2700  // 168 pages ? 168 = 158 + (1 + 5 + 4) ?
#define RAM_MIN_KB 583

#define MOM_FONT_FILE "FONTS.LBX"
#define GAME_FONT_FILE MOM_FONT_FILE

extern int RAM_Min_KB;


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

// s10p17
void RAM_SetMinKB(int Min_KB);


#ifdef __cplusplus
}
#endif

#endif /* MOX_MOM_H */
