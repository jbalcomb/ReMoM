#ifndef ST_DBG_H
#define ST_DBG_H
/*
    seg024
*/

/*

*/
extern int g_Debug_Disabled;               // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled
extern char *SCRDMP00;                     // dseg:4350    XREF: DBG_ScreenDump
extern char *Scrdmp_Ext;                   // dseg:4358    XREF: DBG_ScreenDump
extern char *Scrdmp_0;                     // dseg:435D    XREF: DBG_ScreenDump
extern char *Scrdmp_Base;                  // dseg:435F    XREF: DBG_ScreenDump
extern char *cnst_WB6;                     // dseg:4366    XREF: DBG_ScreenDump


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg024
*/
void DBG_Disable(void);                     // _s24p05c.c
int DBG_IsDisabled(void);                   // _s24p06c.c

#ifdef __cplusplus
}
#endif

#endif /* ST_DBG_H */
