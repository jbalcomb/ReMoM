#ifndef ST_CRSR_H
#define ST_CRSR_H

#include "MoX_TYPE.H"    /* word */


struct s_SAVE_GAM
{


}

struct s_SAVE_GAM save_gam;


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


void Save_SAVE_GAM(short save_gam_idx);
void Load_SAVE_GAM(short save_gam_idx);


#ifdef __cplusplus
}
#endif

#endif /* ST_CRSR_H */
