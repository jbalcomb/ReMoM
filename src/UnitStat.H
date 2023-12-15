/*
    Unit Statistics Window
    ~& Unit Figure

    WIZARDS.EXE
        ovr072
        ovr089
*/

#ifndef UNITSTAT_H
#define UNITSTAT_H



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr072
*/

// WZD o072p01

// WZD o072p02
void USW_LoadAndShow(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t flag, int16_t unit_idx);

// WZD o072p03
void Unit_Statistics_Popup(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ViewTypeFlag, int16_t unit_idx);

// WZD o072p04
void Unit_Statistics_Popup_Do_Draw(void);

// WZD o072p05
void Unit_Statistics_Popup_Draw__WIP(int16_t x_start, int16_t y_start, int16_t ViewTypeFlag, int16_t unit_idx, SAMB_ptr BuildAllowList, int16_t EffectCount, SAMB_ptr ItemDrawSeg);

// WZD o072p06
// WZD o072p07
// WZD o072p08
// WZD o072p09
// WZD o072p10
// WZD o072p11
// WZD o072p12
// WZD o072p13
// WZD o072p14
// WZD o072p15
// WZD o072p16
// WZD o072p17
// WZD o072p18
// WZD o072p19



/*
    WIZARDS.EXE  ovr089
*/

// WZD o89p01

// WZD o89p02
void USW_MemAlloc(void);

// WZD o89p03
// drake178: ¿ USW_LoadFigureImage() ?
void Load_Unit_Figure(int16_t unit_idx, int16_t TypePass);

// WZD o89p04

// WZD o89p05
// drake178: UNIT_Draw()
void Draw_Unit_Figure(int16_t x_start, int16_t y_start, int16_t unit_idx, int16_t VType);

// WZD o89p06
void USW_GetFigPosition(int16_t figure_count, int16_t current_figure, int16_t * Fig_Left, int16_t * Fig_Top);

// WZD o89p07
// WZD o89p08
// WZD o89p09


#ifdef __cplusplus
}
#endif


#endif  /* UNITSTAT_H */
