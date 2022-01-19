// _s34p85c.c GUI_SimplePageFlip
// ST_GUI.H

void GUI_SimplePageFlip(void)
{
    int mouse_x;
    int mouse_y;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: GUI_SimplePageFlip()\n", __FILE__, __LINE__);
#endif

    MOUSE_Disable_CDraw();

    mouse_x = MOUSE_GetX();
    mouse_y = MOUSE_GetY();

    GUI_DrawControls();  // _s34p71c

    GUI_FindWindow(mouse_x, mouse_y);
    GUI_SaveCursorArea_DSP(mouse_x, mouse_y);
    GUI_DrawCursor_DSP(mouse_x, mouse_y);

    VGA_PageFlip();  // _s26p04a.c
    
    GUI_RestoreCursorArea_DSP();
    GUI_CursorCopy_Bkup2Main();  // _s33p33.asm
    MOUSE_CDraw_Restore();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: GUI_SimplePageFlip()\n", __FILE__, __LINE__);
#endif
}
