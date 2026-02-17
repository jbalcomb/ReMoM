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

#include "MOX_TYPE.h"

#include "MOM_Data.h"  /* struct s_WIZARD*/

#include "../../MoX/src/MOM_DEF.h"  /* NUM_PLAYERS */



// WZD dseg:998E
extern SAMB_ptr _screen_seg;
// allocated in MoM_Init_Tables()



// WZD dseg:9ECA
extern struct s_WIZARD _players[NUM_PLAYERS];



/*

*/
enum e_SCOUT_BITS
{
    SCT_BottomLeft   = 1,
    SCT_TopLeft      = 2,
    SCT_TopRight     = 4,
    SCT_BottomRight  = 8
};



#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif  /* MOX_DAT_H */
