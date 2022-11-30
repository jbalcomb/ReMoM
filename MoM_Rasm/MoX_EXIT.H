#ifndef MOX_EXIT_H
#define MOX_EXIT_H

#include "MoX_TYPE.H"


extern int16_t MoX_MemFreeWorst_KB;                    // dseg:3CC2  0xFA00  ? Bytes vs. Paragrphs ? 62.5 KB vs 4000 Pr ? sizeof VGA 320x200 ?



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    MGC  seg005
*/

// MGC s05p02
void MoX_Exit_With_Message(char * string);

// MGC s05p05
void MoX_Update_MemFreeWorst_KB(void);


/*
    MGC  seg006
*/

// TBD  // MGC s06p01
// TBD  void VGA_SetTextMode(void);
// TBD  // MGC s06p02
// TBD  void QuitToDOS(char *quitmsg);

// MGC s06p04
uint16_t MoX_dos_memfree_kb(void);


#ifdef __cplusplus
}
#endif


#endif /* MOX_EXIT_H */
