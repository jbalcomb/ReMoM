/*
    WIZARDS.EXE
        seg002

MoO2  Module: timer
    Release_Time_
        Address: 01:0012C2C6
    Mark_Time_
        Address: 01:0012C2A0
    Time_Passed_
        Address: 01:0012C2AE
    Timer_Value_
        Address: 01:0012C2BD
*/
#ifndef TIMER_H
#define TIMER_H

#include "MOX_TYPE.h"

/* The ticks used by MOO1 are from 0040:005C, which is incremented by INT 8
   at 18.2 Hz. (FIXME ... unless the sound driver messes with it.)
   Hence the ratio 10000000 / 182 == 54945.054945054945054945054945055 */
#define MOO_TICKS_TO_US(_t_) (((_t_) * 5000000) / 91)
// NOTE(JimBalcomb,20250313): just found this in my old MGC Dasm
/*
@@Timeout:                                ;
cmp     ax, 4                             ; 4 * 54.925 milliseconds
                                          ;   ~= 219.7 milliseconds
                                          ;   ~= 0.2197 seconds
                                          ; elsewhere, there's a ref to this being about 250ms
                                          ;   so, may even be just ~= 1 sec
*/
// SEEALSO: win_CLK.C



#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t tick_count;

void Release_Time(int ticks);
void Mark_Time(void);
uint32_t Time_Passed(void);
uint32_t Timer_Value(void);


#ifdef __cplusplus
}
#endif

#endif  /* TIMER_H */
