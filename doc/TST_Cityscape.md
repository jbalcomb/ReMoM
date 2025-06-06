


Cityscape

Testing / Learning-By-Doing / Debugging

void TST_Cityscape(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t bldg_idx_1, int16_t bldg_idx_2)

city_idx

xstart

ystart

bldg_idx_1

bldg_idx_2
    building index into bldg_data

City_Screen_Draw__WIP()
    4,101
Enemy_City_Screen_Draw()
    4,101
Cast_Spell_City_Enchantment_Animation_Draw()
    x_start+5,y_start+27  ... x_start = 53; y_start = 38;



Later, ...
    Cityscape_Draw_Buildings_And_Features__WIP()
        if((cr_bldg_idx == bt_Summoning_Circle) || (bldg_idx == bt_Summoning_Circle))
        e.g.,
            /*
                BEGIN:  Summon Circle  (100)
            */
            if((cr_bldg_idx == bt_Summoning_Circle) || (bldg_idx == bt_Summoning_Circle))
            {
                FLIC_Set_CurrentFrame(cityscape_summon_circle_seg, cityscape_bldg_anim_ctr);
                Draw_Picture_To_Bitmap(cityscape_summon_circle_seg, GfxBuf_2400B);
                if(cr_bldg_idx == new_bldg_idx)
                {
                    // TODO  Vanish_Bitmap(GfxBuf_2400B, IDK_cityscape_vanish_percent);
                }
                FLIC_Set_LoopFrame_1(GfxBuf_2400B);
                Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);
                Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, bt_Summoning_Circle, 1);
            }
            /*
                END:  Summon Circle  (100)
            */




Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, cr_bldg_idx, bldg_data_table[cr_bldg_idx].G_Animation);

Draw_Picture(col_sx, (row_sy - 32), GfxBuf_2400B);
Why the -32?
