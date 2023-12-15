/*
    WIZARDS.EXE
        ...
        seg001
        ...
        seg009
        ...
        ovr063
        ...
        ovr067
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
void String_Copy_Far(unsigned short int dst_ofst, unsigned short int dst_sgmt, unsigned short int src_ofst, unsigned short int src_sgmt);



/*
    WIZARDS.EXE  ovr060
*/

// WZD o60p07
void TILE_VisibilityUpdt(void);


/*
    WIZARDS.EXE  ovr063
*/

// WZD o63p15
int16_t Check_Planar_Seal(void);


/*
    WIZARDS.EXE  ovr067
*/

// WZD o67p15
int16_t IsPassableTower(int16_t world_x, int16_t world_y);



#ifdef __cplusplus
}
#endif

#endif  /* MOX_UNORG_H */
