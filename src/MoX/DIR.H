#ifndef MOX_DIR_H
#define MOX_DIR_H

#include "MoX_Lib.H"

/*
    MoX_DIR.H
    dos_DIR.C
    win_DIR.C
*/

#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    MGC  seg003
    WZD  seg003
*/

// MGC s03p05
// WZD s03p05
int32_t LOF(char * file_name);


/*
    MGC  seg004
    WZD  seg004
*/

// MGC s04p01
// WZD s04p01
int16_t DIR(char * match_string, char * found_file);



#ifdef __cplusplus
}
#endif

#endif /* MOX_DIR_H */
