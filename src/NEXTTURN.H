/*
    Next Turn Procedure

    WIZARDS.EXE
        ovr060
        ovr119
        ovr121
*/

#ifndef NEXTTURN_H
#define NEXTTURN_H

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr060
*/

// WZD o60p01
// WZD o60p02
// WZD o60p03

// WZD o60p04
void Next_Turn_Proc(void);

// WZD o60p05

// WZD o60p06
void Update_Units_MvsSts(void);

// WZD o60p07
// void TILE_VisibilityUpdt(void);



/*
    WIZARDS.EXE  ovr119
*/

// WZD o119p01
// AKA IDK_EoT_s9EC10()
void Next_Turn_Calc(void);

// WZD o119p02
// CTY_CheckMinFarmers()

// WZD o119p03
// sub_9EF06()



/*
    WIZARDS.EXE  ovr121
*/

// WZD o121p01
// WZD o121p02
// WZD o121p03
// WZD o121p04
// WZD o121p05
// WZD o121p06
// WZD o121p07
// WZD o121p08
int16_t UNIT_GetHalfMoves_WIP(int16_t unit_idx);

// WZD o121p09

// WZD o121p10
// WZD o121p11
// WZD o121p12
// WZD o121p13
// WZD o121p14



#ifdef __cplusplus
}
#endif

#endif  /* NEXTTURN_H */
