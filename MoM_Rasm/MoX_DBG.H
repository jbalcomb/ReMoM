#ifndef MOX_DBG_H
#define MOX_DBG_H


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    MGC  seg024
*/

// s24p01
// DBG_Quit()

// // s24p02
// void DBG_ScreenDump(void);
// 
// // s24p03
// void VGA_GetDACBlock(unsigned int DAC_BLK, unsigned char * Buffer);
// 
// // s24p04
// void VGA_ReadScreenLine(unsigned int ScreenLine, unsigned char * Buffer);

// MGC s24p05
void Set_Release_Version(void);

// MGC s24p06
int Check_Release_Version(void);

// // s24p07
// // UU_VGA_SaveDrawSection()
// 
// // s24p08
// // UU_VGA_CopyToLBX()

#ifdef __cplusplus
}
#endif

#endif /* MOX_DBG_H */
