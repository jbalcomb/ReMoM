#ifndef CMBTTST_H
#define CMBTTST_H

#include "MOX/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



// ...absolute bare minimum...
int16_t Combat_Screen_TST_001(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);

// ...complete base...
int16_t Combat_Screen_TST_002(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);

// ...terrain...
int16_t Combat_Screen_TST_003(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);

// ...units...
int16_t Combat_Screen_TST_004(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t* item_count, int16_t item_list[]);



#ifdef __cplusplus
}
#endif

#endif  /* CMBTTST_H */
