#ifndef ST_CTRL_H
#define ST_CTRL_H



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

// s34p47.asm
int CTRL_CreateClickLabel(int xl, int yt, int xr, int yb, int key, int hlp);
// s34p49c.c
int CTRL_CreateHotkey(unsigned char KeyCode, int hlp);

// _s34p71
void CTRL_DrawControls(void);
// _s34p72
void CTRL_DrawControl(int Ctrl_Index, int Input_State);
// _s34p73
void CTRL_SetFocus(int Ctrl_Index, int X_Pos, int Y_Pos);

#ifdef __cplusplus
}
#endif



#endif /* ST_CTRL_H */
