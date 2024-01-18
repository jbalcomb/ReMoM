
#include "MoX_DBG.H"



/*
    Initialized Data
*/
// uint16_t release_version = MoX_ST_FALSE;                  // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled

#ifdef STU_DEBUG
uint16_t release_version = ST_FALSE;
#else
uint16_t release_version = ST_TRUE;                  // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled
#endif




/*
    MAGIC.EXE    seg024
    WIZARDS.EXE  seg024
*/

// WZD s24p01
// AKA DBG_Quit()

// WZD s24p02
// AKA DBG_ScreenDump()

// WZD s24p03
// drake178: VGA_GetDACBlock()

// WZD s24p04
// drake178: VGA_ReadScreenLine()

// WZD s24p05
// MoO2  Module: capture  Release_Version()  Address: 01:00110B34
void Release_Version(void)
{
    release_version = ST_TRUE;
}

// WZD s24p06
// MoO2  Module: capture  Check_Release_Version()  Address: 01:00110B5C
int Check_Release_Version(void)
{
    return release_version;
}

// WZD s24p07
// drake178: UU_VGA_SaveDrawSection()
// Screen_Picture_Capture()
// MoO2  Module: capture  Screen_Flic_Capture()  Address: 01:001101F0

// WZD s24p08
// drake178: UU_VGA_CopyToLBX()
// Capture_Screen_Block()
// MoO2  Module: shear  Capture_Screen_Block_()  Address: 01:0014791B
