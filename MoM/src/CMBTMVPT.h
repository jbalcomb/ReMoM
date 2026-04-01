/*
    Combat

    WIZARDS.EXE
        ovr155  ¿ MoO2  Module: CMBTMOV1 ?  (would have been CMBTMOVE?)

...a Grid-based Bellman-Ford Iterative Relaxation algorithm...  //Sidav
*/

#ifndef CMBTMVPT_H
#define CMBTMVPT_H

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr155
*/

// WZD ovr155p01
void Combat_Move_Path_Find(int16_t source_cgx, int16_t source_cgy, int16_t destination_cgx, int16_t destination_cgy);

// WZD ovr155p02
void Combat_Move_Path_Valid(int16_t source_cgx, int16_t source_cgy, int16_t moves2);



#ifdef __cplusplus
}
#endif

#endif  /* CMBTMVPT_H */
