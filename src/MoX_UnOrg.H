/*
    WIZARDS.EXE
        ...
        seg001
        ...
        seg009
        ...
*/

#ifndef MOX_UNORG_H
#define MOX_UNORG_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



extern int16_t PageFlipEffect;



/*
    WIZARDS.EXE seg001
*/

// WZD s01p03
void PageFlip_FX(void);

// WZD s01p04
void Fade_Out(void);

// WZD s01p05
void Fade_In(void);



/*
    WIZARDS.EXE  seg009
*/

// WZD s09p07
void String_Copy_Far__STUB(unsigned short int dst_ofst, unsigned short int dst_sgmt, unsigned short int src_ofst, unsigned short int src_sgmt);



#ifdef __cplusplus
}
#endif

#endif  /* MOX_UNORG_H */
