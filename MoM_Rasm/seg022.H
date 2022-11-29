#ifndef SEG022_H
#define SEG022_H

#include "ST_DEF.H"  /* _FAR */
#include "ST_SA.H"  /* SAMB_addr */


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    seg022
*/

// s22p22
int ST_MoveData_FP(unsigned char _FAR * fp_Dst, unsigned char _FAR * fp_Src, unsigned int nbytes);
int ST_MoveData(unsigned int dst_ofst, unsigned int dst_sgmt, unsigned int src_ofst, unsigned int src_sgmt, unsigned int nbytes);


#ifdef __cplusplus
}
#endif


#endif /* SEG022_H */
