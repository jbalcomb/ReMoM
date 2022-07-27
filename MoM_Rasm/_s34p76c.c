// _s34p76c.c GUI_Set_Redraw_Function
// ST_GUI.H

#include "ST_HEAD.H"

#include "ST_GUI.H"
#include "ST_SCRN.H"


//void GUI_Set_Redraw_Function(void _FAR *Refresh_Fn, int Refresh_Timer)
//void GUI_Set_Redraw_Function(void (*Refresh_Fn)(void *), int Refresh_Timer)  // per 1oom
void SCRN_Set_Redraw_Function(void (*Refresh_Fn)(void), int Refresh_Timer)  // ~= per 1oom
{

    gfp_SCRN_Redraw_Function = Refresh_Fn;  // in MoG, this works, as `void (*fxnptr)(void);` ... `fxnptr = &fxn;`

    g_SCRN_RedrawFn_Present = 1;

    if ( (Refresh_Timer <= 0) || (Refresh_Timer >= 10) )
    {
        g_SCRN_Redraw_Timer = 2;
    }
    else
    {
        g_SCRN_Redraw_Timer = Refresh_Timer;
    }

}
