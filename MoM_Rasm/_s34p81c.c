// _s34p81c.c GUI_CallRedrawFn
// ST_GUI.H

#include "ST_GUI.H"
#include "ST_SCRN.H"


void SCRN_CallRedrawFn(void)
{
    if ( g_SCRN_RedrawFn_Present != 0 )
    {
        //(_FAR *gfp_GUI_Redraw_Function)();  // Error Not an allowed type in function GUI_CallRedrawFn
        //gfp_GUI_Redraw_Function();  // Error Call of nonfunction in function GUI_CallRedrawFn
        //(*gfp_GUI_Redraw_Function)();  // Error Not an allowed type in function GUI_CallRedrawFn
        //gfp_GUI_Redraw_Function();  // using set as `void (*Refresh_Fn)(void *)`
        //gfp_GUI_Redraw_Function();  // using set as `void (*Refresh_Fn)(void)`  // Error Call of nonfunction in function GUI_CallRedrawFn
        //(*gfp_GUI_Redraw_Function)();  // using set as `void (*Refresh_Fn)(void)`  // Error Not an allowed type in function GUI_CallRedrawFn
        gfp_SCRN_Redraw_Function();  // using set as `void (*Refresh_Fn)(void)`
    }
}
