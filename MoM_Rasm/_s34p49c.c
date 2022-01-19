// _s34p49c.c GUI_CreateHotkey()
// ST_GUI.H

#include "ST_GUI.H"

#include "STU_DBG.H"

// 1oom :: uiobj.c
// int16_t uiobj_add_inputkey(uint32_t key)
// {
//     uiobj_t *p = &uiobj_tbl[uiobj_table_num];
//     uiobj_add_set_xys_offscreen(p);
//     p->type = 7;
//     p->vptr = 0;
//     p->key = key;
//     return UIOBJI_ALLOC();
// }


void GUI_CreateHotkey(unsigned char KeyCode)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: GUI_CreateHotkey(KeyCode=%u)\n", __FILE__, __LINE__, KeyCode);
#endif

    gfp_GUI_Control_Table[g_GUI_Control_Count].Left = 500;
    gfp_GUI_Control_Table[g_GUI_Control_Count].Top = 500;
    gfp_GUI_Control_Table[g_GUI_Control_Count].Right = 500;
    gfp_GUI_Control_Table[g_GUI_Control_Count].Bottom = 500;
    gfp_GUI_Control_Table[g_GUI_Control_Count].Ctrl_Type = Ctrl_Label;
    gfp_GUI_Control_Table[g_GUI_Control_Count].UU_Help = ST_UNDEFINED;
    
    gfp_GUI_Control_Table[g_GUI_Control_Count].Hotkey = KeyCode;

    if ( (gfp_GUI_Control_Table[g_GUI_Control_Count].Hotkey > 96) & (gfp_GUI_Control_Table[g_GUI_Control_Count].Hotkey < 123) )
    {
        gfp_GUI_Control_Table[g_GUI_Control_Count].Hotkey += 224;// add al, 'A'-'a'  // ; 224 (0xE0)
    }

    g_GUI_Control_Count += 1;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] gfp_GUI_Control_Table[%d].Hotkey=%u\n", __FILE__, __LINE__, (g_GUI_Control_Count - 1), gfp_GUI_Control_Table[(g_GUI_Control_Count - 1)].Hotkey);
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: GUI_CreateHotkey()\n", __FILE__, __LINE__);
#endif
    return (g_GUI_Control_Count - 1);
}
