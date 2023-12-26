
# Unit Figures



uses UnitDraw_WorkArea



Unit_Figure_Load()
Unit_Figure_Draw()
Unit_Figure_Position()



FIGURE LBX
    FIGURES {1,2,3,4,5,6,7,8,9}
    FIGURE  {10,11,12,1,14,15,16}

Unit Type / 15 + 1 is Figure LBX Num
Unit Type % 15 + 2 is Entry Num



### Unit_Figure_Load()


### Unit_Figure_Draw()
    gets passed unit_idx or unit_type, and flag for which
    unit_figure_count = _unit_type_table[unit_type].Figures;
    Unit_Figure_Position(unit_figure_count, itr_figure_count, &fig_y, &fig_x);
    Draw_Picture((x_start + fig_y), (y_start + fig_x), UnitDraw_WorkArea);

    Production_Screen()
    Production_Screen_Draw()
    Production_Screen_Draw_()
    Thing_View_Draw__WIP((popup_base_x + list_field_width), popup_base_y, 0, product_idx, prod_specials_list, UV_specials_count, PS_product_pict_seg);
    Draw_Unit_Figure((UV_x_start_offset + UV_x_start + 11), (UV_y_start - 3), unit_idx, CalledFromFlag);
    

### Unit_Figure_Position()


### UnitDraw_WorkArea
