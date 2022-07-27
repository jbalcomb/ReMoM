#ifndef ST_SCRN_H
#define ST_SCRN_H



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

// ST_SCR.H
// _s34p76
//void GUI_Set_Redraw_Function(void _FAR *Refresh_Fn, int Refresh_Timer);
void SCRN_Set_Redraw_Function(void (*Refresh_Fn)(void), int Refresh_Timer);  // ~= per 1oom
// _s34p79
void SCRN_DisableRedraw(void);
// _s34p81c.c
void SCRN_CallRedrawFn(void);
// _s34p82
void SCRN_Redraw_WaitOne(void);
// _s34p83
void SCRN_Redraw_WaitTimer(void);
// _s34p85c.c
void SCRN_SimplePageFlip(void);


#ifdef __cplusplus
}
#endif



#endif /* ST_SCRN_H */
