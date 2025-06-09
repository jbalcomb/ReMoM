/*
    WIZARDS.EXE
        ...
        seg001
        ...
        seg009
        ...
*/

#ifndef MOX_DAT_H
#define MOX_DAT_H

#include "MOX_TYPE.H"

#include "MOM_Data.H"  /* struct s_WIZARD*/

#include "../MOM_DEF.H"  /* NUM_PLAYERS */



// WZD dseg:998E
extern SAMB_ptr _screen_seg;
// allocated in MoM_Init_Tables()



// WZD dseg:9ECA
extern struct s_WIZARD _players[NUM_PLAYERS];



#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif  /* MOX_DAT_H */
