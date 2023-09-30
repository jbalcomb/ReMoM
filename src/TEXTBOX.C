
#include "MoX_TYPE.H"

#include "Help.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



/*
    WIZARDS.EXE  ovr159
*/

// WZD o159p01
// MoO2  Module: TEXTBOX  Draw_Help_Entry_()
void Draw_Help_Entry(int16_t help_entry_idx)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Draw_Help_Entry(help_entry_idx = %d)\n", __FILE__, __LINE__, help_entry_idx);
#endif



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Draw_Help_Entry(help_entry_idx = %d)\n", __FILE__, __LINE__, help_entry_idx);
#endif
}


// WZD o159p02
// drake178: GUI_Help_RedrawFn()
void Draw_Help_Entry_Draw(void)
{
    // NOTE: Empty, on purpose!! Also, IDGI.
}


// WZD o159p03
// drake178: EMM_Map_VGAFILEH1_5()
uint16_t EMM_Map_VGAFILEH1_5(void)
{
    // TODO  EMM_Map4Pages(1, g_EmmHndl_VGAFILEH);
    // TODO  return EMM_PageFrame;  // ; contains the segment address of the EMS page frame
    return ST_UNDEFINED;
}
