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

#include "MoX_Lib.H"

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
