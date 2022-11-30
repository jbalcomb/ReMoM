
#include "ST_DBG.H"

/*
    seg024
*/

// seg024.C     
// int Debug_Disabled = 1;                  // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled
int Debug_Disabled = 0;
// in-code      char * SCRDMP00 = "SCRDMP00";               // dseg:4350    XREF: DBG_ScreenDump
// in-code      char * Scrdmp_Ext = ".FLI";                 // dseg:4358    XREF: DBG_ScreenDump
// in-code      char * Scrdmp_0 = "0";                      // dseg:435D    XREF: DBG_ScreenDump
// in-code      char * Scrdmp_Base = "SCRDMP";              // dseg:435F    XREF: DBG_ScreenDump
// in-code      char * cnst_WB6 = "wb";                     // dseg:4366    XREF: DBG_ScreenDump
