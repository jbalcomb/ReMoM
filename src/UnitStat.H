/*
    Unit Statistics Window
    ~& Unit Figure

    WIZARDS.EXE
        ovr072
        ovr089
*/

#ifndef UNITSTAT_H
#define UNITSTAT_H



// dseg:C18C
extern int16_t UV_ListItem;

// dseg:C18E
extern int16_t UV_IsMultiPage;

// dseg:C190
extern int16_t UV_specials_count;



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr072
*/

// WZD o72p01
// drake178: USW_CombatDisplay()

// WZD o72p02
// drake178: USW_LoadAndShow()
void USW_LoadAndShow(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t flag, int16_t unit_idx);

// WZD o72p03
void Unit_Statistics_Popup(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t CalledFromFlag, int16_t unit_idx);

// WZD o72p04
void Unit_Statistics_Popup_Do_Draw(void);

// WZD o72p05
void Thing_View_Draw__WIP(int16_t x_start, int16_t y_start, int16_t CalledFromFlag, int16_t thing_idx, struct s_UV_List * specials_list, int16_t specials_count, SAMB_ptr item_icon_pict_seg);

// WZD o72p06
// drake178: UNIT_DrawUpkeep()
void UV_Draw_Upkeep_Icons__WIP(int16_t xstart, int16_t ystart, int16_t unit_or_neg_type);

// WZD o72p07
// drake178: USW_DrawStatIcons()
// USW_DrawStatIcons()
void UV_Draw_Stat_Icons(int16_t xstart, int16_t ystart, int16_t icon_idx, int16_t Attrib, int16_t row_length, int16_t Gold, int16_t Grey);

// WZD o72p08
// drake178: USW_DrawHitsIcons()
void UV_Draw_Hits_Icons(int16_t xstart, int16_t ystart, int16_t icon_idx, int16_t hits_total, int16_t hits_damaged, int16_t row_length, int16_t hits_bonus);

// WZD o72p09
// drake178: USW_GetBaseMoveIcon()
void Moves_Type_Icon(int16_t move_flags, int16_t * moves_icon_idx);

// WZD o72p10
// drake178: USW_DrawListPage()
void USW_DrawListPage(int16_t xstart, int16_t ystart, struct s_UV_List * specials_list, int16_t specials_count, int16_t specials_item, SAMB_ptr item_icon_pict_seg);

// WZD o72p11
// drake178: USW_CreateFXControls()
void UV_Add_Effect_Fields(int16_t xstart, int16_t ystart, struct s_UV_List * effect_list, int16_t effect_count, int16_t effect_item);

// WZD o72p12
// drake178: USW_SetFXHelp()
// USW_SetFXHelp()

// WZD o72p13
// drake178: USW_CreateScrollBtns()
void UV_Add_UpDn_Buttons(int16_t xstart, int16_t ystart);

// WZD o72p14
// drake178: USW_ClearFXControls()
void UV_Clear_Effect_Fields(void);

// WZD o72p15
// drake178: USW_GetShownFXCount()
// USW_GetShownFXCount()

// WZD o72p16
// drake178: USW_GetWpnIconIndex()
void Melee_Type_Icon(int16_t melee_type, int16_t * icon_idx);

// WZD o72p17
// drake178: USW_GetRngIconIndex()
void Ranged_Type_Icon(int16_t ranged_type, int16_t * icon_idx);

// WZD o72p18
// drake178: USW_HireScreenPrep()
// USW_HireScreenPrep()

// WZD o72p19
void UV_Setup_ProdScr(int16_t specials_count);



/*
    WIZARDS.EXE  ovr089
*/

// WZD o89p01
void Prod_Init_Strategic_Unit(int16_t unit_type, struct s_STRATEGIC_UNIT * strategic_unit);

// WZD o89p02
void USW_MemAlloc(void);

// WZD o89p03
// drake178: ¿ USW_LoadFigureImage() ?
void Load_Unit_Figure(int16_t type_or_unit, int16_t use_type);

// WZD o89p04
// IDK_Get_Unit_Enchantment_Names

// WZD o89p05
// drake178: UNIT_Draw()
void Draw_Unit_Figure(int16_t x_start, int16_t y_start, int16_t unit_idx, int16_t VType);

// WZD o89p06
// drake178: USW_GetFigPosition()
void Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * fig_x, int16_t * fig_y);

// WZD o89p07
// UV_Remove_Unit_Enchantment

// WZD o89p08
// UNIT_RemoveEnchant

// WZD o89p09
// sub_759DE

// WZD o89p10
// drake178: UNIT_GetDependants()
void UNIT_GetDependants(int16_t unit_idx, int16_t * troop_count, int16_t troops[]);



#ifdef __cplusplus
}
#endif


#endif  /* UNITSTAT_H */
