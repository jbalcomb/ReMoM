#ifndef ST_DBG_H
#define ST_DBG_H
/*
    seg024
*/

// seg024.C     extern int Debug_Disabled;               // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled
// in-code      extern char *SCRDMP00;                     // dseg:4350    XREF: DBG_ScreenDump
// in-code      extern char *Scrdmp_Ext;                   // dseg:4358    XREF: DBG_ScreenDump
// in-code      extern char *Scrdmp_0;                     // dseg:435D    XREF: DBG_ScreenDump
// in-code      extern char *Scrdmp_Base;                  // dseg:435F    XREF: DBG_ScreenDump
// in-code      extern char *cnst_WB6;                     // dseg:4366    XREF: DBG_ScreenDump


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg024
*/

// s24p01
// DBG_Quit()

// s24p02
void DBG_ScreenDump(void);

// s24p03
void VGA_GetDACBlock(unsigned int DAC_BLK, unsigned char * Buffer);

// s24p04
void VGA_ReadScreenLine(unsigned int ScreenLine, unsigned char * Buffer);

// s24p05
// MoX_DBG  void DBG_Disable(void);

// s24p06
// MoX_DBG  int DBG_IsDisabled(void);

// s24p07
// UU_VGA_SaveDrawSection()

// s24p08
// UU_VGA_CopyToLBX()

#ifdef __cplusplus
}
#endif

#endif /* ST_DBG_H */
