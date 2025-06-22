/*
    WIZARDS.EXE
        ovr132
*/

#ifndef SPELLS132_H
#define SPELLS132_H

#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif




/*
    WIZARDS.EXE  ovr132
*/

// WZD o132p01
void Cast_Summon_Hero(int16_t player_idx, int16_t type);

// WZD o132p02
void Cast_Incarnation(int16_t player_idx);

// WZD o132p03
// CTY_ChaosRift()

// WZD o132p04
// WIZ_MeteorStorm()

// WZD o132p05
// CTY_StreamOfLife()

// WZD o132p06
int16_t WIZ_HireHero(int16_t player_idx, int16_t Type, int16_t Hero_Slot, int16_t Saved);

// WZD o132p07
int16_t Cast_Resurrection(int16_t player_idx);

// WZD o132p08
int16_t Cast_Plane_Shift_Check(int16_t wx, int16_t wy, int16_t wp);

// WZD o132p09
void Cast_Plane_Shift_Draw(int16_t wx, int16_t wy, int16_t wp);

// WZD o132p10
int16_t Cast_Plane_Shift(int16_t player_idx);

// WZD o132p11
// WIZ_GreatWasting()

// WZD o132p12
// CTY_GaiasBlessing()

// WZD o132p13
// WIZ_Armageddon()

// WZD o132p14
void Cast_Floating_Island(int16_t player_idx);

// WZD o132p15
// CTY_NightshadeDispel()

// WZD o132p16
int16_t Cast_Lycantrophy(int16_t player_idx);

// WZD o132p17
int16_t Cast_Wall_Of_Stone(int16_t player_idx);

// WZD o132p18
int16_t Cast_SpellOfReturn(int16_t player_idx);



#ifdef __cplusplus
}
#endif


#endif  /* SPELLS132_H */
