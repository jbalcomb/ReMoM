/*
    WIZARDS.EXE
        ovr100

    Moo2
        Module: COLONIZE

*/

#ifndef SETTLE_H
#define SETTLE_H

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE ovr100
*/

// WZD o100p01
// drake178: AI_MoveUnits()
// AI_MoveUnits()

// WZD o100p02
// drake178: AI_UNIT_Meld()
// AI_UNIT_Meld()

// WZD o100p03
// drake178: AI_UNIT_Settle()
// AI_UNIT_Settle()

// WZD o100p04
// drake178: Do_Build_Outpost()
int16_t Do_Build_Outpost(void);

// WZD o100p05
// drake178: STK_SettleTile()
int16_t STK_SettleTile(int16_t troop_count, int16_t troops[]);

// WZD o100p06
// drake178: AI_UNIT_Move()
// AI_UNIT_Move()

// WZD o100p07
// drake178: sub_8227A()
// sub_8227A()

// WZD o100p08
// drake178: sub_82377()
// sub_82377()

// WZD o100p09
// drake178: UNIT_PushOffTile()
void UNIT_PushOffTile(int16_t unit_idx);

// WZD o100p10
// drake178: TILE_UNIT_CanMoveTo()
int16_t Unit_Space_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t unit_idx);

// WZD o100p11
// drake178: AI_UNIT_SeekTransprt()
// AI_UNIT_SeekTransprt()

// WZD o100p12
// drake178: AI_UNIT_BuildRoad()
// AI_UNIT_BuildRoad()

// WZD o100p13
// drake178: TILE_CanShiftPlanes()
// TILE_CanShiftPlanes()



#ifdef __cplusplus
}
#endif

#endif  /* SETTLE_H */
