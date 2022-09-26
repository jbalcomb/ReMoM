// _s34p22c.c UI_ProcessInput_KD()
// ST_GUI.H
// ? UI_ProcessInput_KD ?

#include "ST_CTRL.H"
#include "ST_GUI.H"

// #include "STU_DBG.H"

// 1oom :: uiobj.c :: static uint32_t uiobj_handle_kbd(int16_t *oiptr)

int UI_ProcessInput_KD(int *oiptr)
{
    char KeyCode;
    char InputCode;
    //int oi;
    int ioi;

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: UI_ProcessInput_KD()\n", __FILE__, __LINE__);
// #endif


    // HERE("InputCode = KD_GetKeyCode();");
    InputCode = KD_GetKeyCode();  // _s33p17.asm int KBD_GetKey(void)

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] InputCode: %d\n", __FILE__, __LINE__, InputCode);
// #endif

    KeyCode = InputCode;

    if ( (InputCode > 96) & ( InputCode < 123) )
    {
        InputCode = InputCode + 0xE0;  // 224 .. but, this just a byte? so, 122 + 224 = ?
    }

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] InputCode: %d\n", __FILE__, __LINE__, InputCode);
// #endif


// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] ioi: %d\n", __FILE__, __LINE__, ioi);
// #endif

    ioi = 1;
    // HERE("while ( ioi != g_CTRL_Control_Count )");

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] ioi=%d, g_CTRL_Control_Count=%d\n", __FILE__, __LINE__, ioi, g_CTRL_Control_Count);
// #endif

    while ( ioi != g_CTRL_Control_Count )
    {

// #ifdef DEBUG
//         dlvfprintf("DEBUG: [%s, %d] CtrlTbl[%d].Hotkey=%d, InputCode=%d\n", __FILE__, __LINE__, ioi, gfp_CTRL_Control_Table[ioi].Hotkey, InputCode);
// #endif

        if ( gfp_CTRL_Control_Table[ioi].Hotkey == InputCode )
        {
            // HERE("( gfp_CTRL_Control_Table[ioi].Hotkey == InputCode )");
            *oiptr = ioi;
        }

    ioi++;
    }


    if ( InputCode != gfp_CTRL_Control_Table[*oiptr].Hotkey )
    {
        // HERE("( InputCode != gfp_CTRL_Control_Table[oi].Hotkey )");
        InputCode = KeyCode;
    }

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: UI_ProcessInput_KD() {InputCode=%d, *oiptr=%d}\n", __FILE__, __LINE__, InputCode, *oiptr);
// #endif

    return InputCode;
}
