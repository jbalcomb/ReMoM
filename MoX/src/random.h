/*
    WIZARDS.EXE
    seg001
    seg022

MoO2
    Module: random

*/
#ifndef MOX_RNG_H
#define MOX_RNG_H



#include "MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



/*
    WZD  seg001
*/

// WZD s01p07
int16_t Get_Weighted_Choice(int16_t * weight_array, int16_t weight_count);

// WZD s01p08
int16_t Get_Weighted_Choice_Long(int32_t * weight_array, int16_t weight_count);

// WZD s01p09
// Get_Weighted_Best_Choice()

// WZD s01p10
// Get_Weighted_Best_Choice_Long()



/*
    WZD  seg022
*/

// WZD s22p05
void Set_Random_Seed(uint32_t n);

// WZD s22p06
uint32_t Get_Random_Seed(void);

// WZD s22p07
void Randomize(void);

// WZD s22p08
int16_t Random(int16_t max);

/* CLAUDE: diagnostic — running count of calls to Random() since process start. */
extern uint64_t g_random_call_count;

/* CLAUDE: _cmd_line_seed declaration moved to MoX/src/MOX2.h.  Files that
   reference it (e.g. LoadScr.c::PreInit_Overland) now include MOX2.h. */

// win_CLK.C  /* Get_System_Clock_Counter() */
// uint32_t Get_System_Clock_Counter(void);
// uint32_t Read_System_Clock_Timer(void);
uint64_t Read_System_Clock_Timer(void);



#ifdef __cplusplus
}
#endif

#endif /* MOX_RNG_H */
