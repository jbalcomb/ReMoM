/*
    Hall of Fame Screen

    MAGIC.EXE
        ovr061
    
    MoO2:
        Module: SCORE
*/

#ifndef SCORE_H
#define SCORE_H

#include "MOX/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif

// MGC  dseg:3C80 54 72 6F 6C 6C 73 00                            cnst_Race0D db 'Trolls',0
/*
                                            BEGIN: ovr061  (Hall Of Fame)
*/
// MGC  dseg:3C87 48 41 4C 4F 46 41 4D 00                         halofam_lbx_file db 'HALOFAM',0
// MGC  dseg:3C8F 1B 00                                           hof_hotkey_ESC db 1Bh,0                 ; should use dseg:28d0
// MGC  dseg:3C91 20 00                                           cnst_HoF_Space db ' ',0
// MGC  dseg:3C93 48 61 6C 6C 00                                  cnst_HoF_String_B db 'Hall',0
// MGC  dseg:3C98 4F 66 00                                        cnst_Of db 'Of',0
// MGC  dseg:3C9B 46 61 6D 65 00                                  cnst_Fame db 'Fame',0
// MGC  dseg:3CA0 4D 61 73 74 65 72 00                            cnst_HoF_String_C db 'Master',0
// MGC  dseg:3CA7 29 00                                           str_PAREN_CLOSE db ')',0
// MGC  dseg:3CA9 20 6F 66 20 74 68 65 20 00                      cnst_HoF_String_D db ' of the ',0
// MGC  dseg:3CB2 28 00                                           str_PAREN_OPEN db '(',0
// MGC  dseg:3CB4 25 29 00                                        cnst_HoF_String_E db '%)',0
// MGC  dseg:3CB7 00                                              align 2
/*
                                            END: ovr061  (Hall Of Fame)
*/
// MGC  dseg:3CB8 00 00                                           g_TIMER_Count_Lo dw 0



// MGC o61p01
void Hall_Of_Fame_Screen(void);

// MGC o61p02
void Hall_Of_Fame_Screen_Draw(void);


#ifdef __cplusplus
}
#endif

#endif  /* SCORE_H */
