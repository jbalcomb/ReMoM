/*
    Unit Statistics Window
    ~& Unit Figure

    WIZARDS.EXE
        ovr072
*/

#ifndef UNITSTAT_H
#define UNITSTAT_H



// WZD dseg:C176                                                 ¿ BEGIN:  ovr072  Unit View ?

// int16_t uv_combat;
// int16_t uv_hero;
// int16_t uv_fill_base;
extern SAMB_ptr unit_figure_seg;
// int16_t unitview_dn_button;
// int16_t unitview_up_button;
// int16_t unitview_dismiss_button;
// int16_t unitview_ok_button;
// int16_t unitview_full_screen;
// int16_t unitview_button_box;
// int16_t unitview_window;
extern int16_t uv_specials_list_index;
extern int16_t uv_multipage;
extern int16_t uv_specials_list_count;
extern struct s_UV_List * uv_specials_list_array;
// int32_t uv_specials_fields_ability_flags[8];
// int16_t uv_item_desc_fields[3];
// int16_t uv_item_icon_fields[3];
// int16_t uv_special_desc_fields[8];
// int16_t uv_special_icon_fields[8];
extern SAMB_ptr UV_item_icon_pict_seg;
// int16_t uv_unit_idx;
// int16_t uv_view_type;  // enum e_VIEW_TYPE
// int16_t UV_y_start;
// int16_t UV_x_start;

// WZD dseg:C1E8                                                 ¿ END:  ovr072  Unit View ?





#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr072
*/

// WZD o72p01
// drake178: USW_CombatDisplay()

// WZD o72p02
void USW_LoadAndShow(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t flag, int16_t unit_idx);

// WZD o72p03
void Unit_Statistics_Popup(int16_t x_start, int16_t y_start, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t CalledFromFlag, int16_t unit_idx);

// WZD o72p04
void Unit_Statistics_Popup_Do_Draw(void);

// WZD o72p05
void Thing_View_Draw__WIP(int16_t x_start, int16_t y_start, int16_t CalledFromFlag, int16_t thing_idx, struct s_UV_List * specials_list, int16_t specials_count, SAMB_ptr item_icon_pict_seg);

// WZD o72p06
void UV_Draw_Upkeep_Icons(int16_t xstart, int16_t ystart, int16_t unit_or_neg_type);

// WZD o72p07
void UV_Draw_Stat_Icons(int16_t xstart, int16_t ystart, int16_t icon_idx, int16_t Attrib, int16_t row_length, int16_t Gold, int16_t Grey);

// WZD o72p08
void UV_Draw_Hits_Icons(int16_t xstart, int16_t ystart, int16_t icon_idx, int16_t hits_total, int16_t hits_damaged, int16_t row_length, int16_t hits_bonus);

// WZD o72p09
void Moves_Type_Icon(int16_t move_flags, int16_t * moves_icon_idx);

// WZD o72p10
void USW_DrawListPage(int16_t xstart, int16_t ystart, struct s_UV_List * specials_list, int16_t specials_count, int16_t specials_item, SAMB_ptr item_icon_pict_seg);

// WZD o72p11
void UV_Add_Specials_Fields(int16_t xstart, int16_t ystart, struct s_UV_List * effect_list, int16_t effect_count, int16_t effect_item);

// WZD o72p12
// USW_SetFXHelp()
void UV_Set_Specials_Help_Fields(struct s_UV_List * specials_list, int16_t specials_count, int16_t specials_index);

// WZD o72p13
void UV_Add_UpDn_Buttons(int16_t xstart, int16_t ystart);

// WZD o72p14
void UV_Clear_Specials_Fields(void);

// WZD o72p15
void UV_Handle_Arrow_Buttons(int16_t input_field_idx, int16_t * display_count);

// WZD o72p16
void Melee_Type_Icon(int16_t melee_type, int16_t * icon_idx);

// WZD o72p17
void Ranged_Type_Icon(int16_t ranged_type, int16_t * icon_idx);

// WZD o72p18
void UV_Setup_HireScr(int16_t unit_idx, int16_t specials_list_count);

// WZD o72p19
void UV_Setup_ProdScr(int16_t specials_count);



#ifdef __cplusplus
}
#endif


#endif  /* UNITSTAT_H */
