
#include "MoX.H"

/*
    WIZARDS.EXE  seg021
*/

// WZD s21p01
// MoO2: Refresh_Palette |-> Store_Palette_Block_
// 1oom :: uipal.c :: void ui_palette_set_n(void)
// AKA VGA_DAC_Write()
// IBM-PC, VGA, MS-DOS: write to the VGA-DAC
// MS-Windows: write to PFL-provded palette buffer
void Apply_Palette(void)
{

    int16_t itr;

// DELETE      for(itr = 0; itr < 256; itr++)  // TODO  ~ #define Color Count
// DELETE      {
// DELETE          if(*(p_Palette + 768 + itr) == 1)  // TODO  ~ #define Palette Flags Offset
// DELETE          {
// DELETE              *(g_Palette + itr) = *(p_Palette + itr);  // TODO  DELETE
// DELETE  
// DELETE              *(g_Palette_XBGR + (itr * 4) + 3) = 0x00;
// DELETE              *(g_Palette_XBGR + (itr * 4) + 2) = (*(p_Palette + (itr * 3) + 0) << 2);
// DELETE              *(g_Palette_XBGR + (itr * 4) + 1) = (*(p_Palette + (itr * 3) + 1) << 2);
// DELETE              *(g_Palette_XBGR + (itr * 4) + 0) = (*(p_Palette + (itr * 3) + 2) << 2);
// DELETE          }
// DELETE  
// DELETE      }

    for(itr = 0; itr < 256; itr++)  // TODO  ~ #define Color Count
    {
        if( *(p_Palette + 768 + itr) == 1 )  // TODO  ~ #define Palette Flags Offset
        {
// DELETE  #ifdef STU_DEBUG
// DELETE      if(itr == 0x72)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: *(p_Palette + (itr * 3) + 0): %02X\n", __FILE__, __LINE__, *(p_Palette + (itr * 3) + 0));
// DELETE          dbg_prn("DEBUG: [%s, %d]: *(p_Palette + (itr * 3) + 1): %02X\n", __FILE__, __LINE__, *(p_Palette + (itr * 3) + 1));
// DELETE          dbg_prn("DEBUG: [%s, %d]: *(p_Palette + (itr * 3) + 2): %02X\n", __FILE__, __LINE__, *(p_Palette + (itr * 3) + 2));
// DELETE      }
// DELETE  #endif
            *(PFL_Palette + (itr * 4) + 3) = 0x00;
            *(PFL_Palette + (itr * 4) + 2) = (*(p_Palette + (itr * 3) + 0) << 2);
            *(PFL_Palette + (itr * 4) + 1) = (*(p_Palette + (itr * 3) + 1) << 2);
            *(PFL_Palette + (itr * 4) + 0) = (*(p_Palette + (itr * 3) + 2) << 2);
        }
    }

}