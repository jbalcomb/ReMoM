// _s34p72c.c GUI_DrawControl
// ST_GUI.H

#include "ST_CTRL.H"
#include "ST_GUI.H"


void CTRL_DrawControl(int Ctrl_Index, int Input_State)
{
    int Half_V_Spacing;
    int Half_Font_Height;

    int tmp_SI;

    dlvfprintf("DEBUG: [%s, %d] BEGIN: CTRL_DrawControl()\n", __FILE__, __LINE__);

    tmp_SI = Ctrl_Index;


    //  0: Ctrl_ClickButton
    //  1: Ctrl_ToggleButton
    //  2: Ctrl_LockableButton
    //  3: Ctrl_MStateButton
    //  4: Ctrl_EditBox
    //  5: Ctrl_ImageLabel
    //  6: Ctrl_Slidebar
    //  7: Ctrl_Label
    //  8: Ctrl_AltString
    //  9: Ctrl_ClickLink
    // 10: Ctrl_DialogLine
    // 11: Ctrl_EditSelect
    // 12: Ctrl_ClickGrid
    switch (gfp_CTRL_Control_Table[tmp_SI].Ctrl_Type)
        {
            case Ctrl_ClickButton:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_ClickButton\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_ToggleButton:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_ToggleButton\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_LockableButton:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_LockableButton\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_MStateButton:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_MStateButton\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_EditBox:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_EditBox\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_ImageLabel:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_ImageLabel\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_Slidebar:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_Slidebar\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_Label:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_Label\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_AltString:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_AltString\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_ClickLink:
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_ClickLink\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_DialogLine :
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_DialogLine\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_EditSelect :
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_EditSelect\n", __FILE__, __LINE__);
                break;
            }
            case Ctrl_ClickGrid :
            {
                dlvfprintf("DEBUG: [%s, %d] case Ctrl_ClickGrid\n", __FILE__, __LINE__);
                break;
            }
        }


    dlvfprintf("DEBUG: [%s, %d] END: CTRL_DrawControl()\n", __FILE__, __LINE__);
}