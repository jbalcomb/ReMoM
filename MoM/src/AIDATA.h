/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
        ¿ ovr164 ?
*/

#ifndef AIDATA_H
#define AIDATA_H



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p0



/*
    WIZARDS.EXE  ovr164
*/

// WZD o164p01
void NPC_Excess_Garrison(void);

// WZD o164p02
// EVNT_RampageMonsters()

// WZD o164p03
// EVNT_GenerateRaiders()

// WZD o164p04
int16_t NPC_Destinations(void);

// WZD o164p05
void Allocate_AI_Data(void);

// WZD o164p06
void AI_Magic_Strategy__WIP(int16_t player_idx);

// WZD o164p07
void AI_Evaluate_Hostility(int16_t player_idx);

// WZD o164p08
void Build_NPC_Stacks(void);



#ifdef __cplusplus
}
#endif

#endif  /* AIDATA_H */
