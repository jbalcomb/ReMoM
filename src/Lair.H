/*
    Lair

    WIZARDS.EXE
        ovr083

BACKGRND.LBX
    LCONFIRM    lair confirm borde
    LCONFIRM    lair bottom w/butt
    LCONFIRM    lair bottom wo/but

NOTE: IDGI, but the FanDom MoM Wiki uses the term "Encounter Zone"
*/

#ifndef LAIR_H
#define LAIR_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



// WZD o83p01
// drake178: TILE_HasEncounter()
int16_t Square_Has_Lair(int16_t world_x, int16_t world_y, int16_t map_plane);

// WZD o83p02
// EZ_Clear            ();
// WZD o83p03
// sub_6D490           ();
// WZD o83p04
// EZ_Resolve          ();
// WZD o83p05
// OVL_EZ_Confirm_Entry()
// WZD o83p06
// EZ_TreasureDialog   ();
// WZD o83p07
// EZ_GoldManaReward   ();

// WZD o83p08
// drake178: STR_ListSeparator()
void STR_ListSeparator(int16_t * size, int16_t count, char * list);

// WZD o83p09
char * STR_GetIndefinite(char * word);

// WZD o83p10
// EZ_GenerateRewards  ();
// WZD o83p11
// OVL_EZ_Confirm_Dlg  ();
// WZD o83p12
// OVL_DrawEZConfirm   ();
// WZD o83p13
// EZ_SpecialTreasure  ();
// WZD o83p14
// EZ_GetBookRealm     ();



#ifdef __cplusplus
}
#endif

#endif  /* LAIR_H */
