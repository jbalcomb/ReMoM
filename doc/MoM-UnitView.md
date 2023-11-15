
Unit View



x_start, y_start
x1, y1, x2, y2



Background

Unit Picture Base

Unit Picture
    Unit Figures
    Hero Portrait




Unit View - View Type
0:
1:
2:  ¿ Combat Only ?

Unit View Background
0:  unitview_small_background_seg
1:  unitview_large_background_seg & unitview_button_background_seg
2:  unitview_large_background_seg



Unit_Statistics_Popup_Draw__WIP()
    if(UV_view_type == 0)
        FLIC_Draw(UV_x_start, UV_y_start, unitview_small_background_seg);
    else
        FLIC_Draw(UV_x_start, UV_y_start, unitview_large_background_seg);
            if(UV_view_type != 2)
                FLIC_Draw((UV_x_start + 213), (UV_y_start + 133), unitview_button_background_seg);



// WZD dseg:97B8
// drake178: IMG_USW_Background@
// FULLUNIT large unit backgrn
unitview_large_background_seg = LBX_Reload_Next("UNITVIEW", 1, GFX_Swap_Seg);

// WZD dseg:97BA
// drake178: IMG_USW_UnitHire_BG@
// UNITBACK small unit backgrn
unitview_small_background_seg = LBX_Reload_Next("UNITVIEW", 0, GFX_Swap_Seg);

// WZD dseg:9540
// drake178: IMG_USW_SideBtns_BG
// BLDBUTBK button background
unitview_button_background_seg = LBX_Reload_Next("UNITVIEW", 2, GFX_Swap_Seg);


