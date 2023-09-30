/*
    WIZARDS.EXE
        ovr159

    MoO2
        Module: TEXTBOX
        ¿ Module: GENDRAW ?
*/
/*
Module: TEXTBOX

    data (0 bytes) _textbox_field_save
    Address: 02:00195330

    data (0 bytes) _textbox_seg
    Address: 02:00195354

    data (0 bytes) _textbox_af
    Address: 02:00195358

    data (0 bytes) _bottom_y
    Address: 02:0019535C

    data (0 bytes) _box_y
    Address: 02:0019535E

    data (0 bytes) _box_x
    Address: 02:00195360

    data (0 bytes) _old_fields_count
    Address: 02:00195362

    data (0 bytes) _old_help_list_active
    Address: 02:00195364

    data (0 bytes) _old_draw_fields_flag
    Address: 02:00195366

    data (0 bytes) _box_size
    Address: 02:00195368

    Draw_Help_Entry_
    Address: 01:000A59A1

    Text_Box_
    Address: 01:000A5EB2

    Timed_Text_Box_
    Address: 01:000A5EBC

    Do_Text_Box_
    Address: 01:000A5EBF

    Box_Centered_On_XY_
    Address: 01:000A6166

    Text_Box_Startup_
    Address: 01:000A62E0

    Text_Box_Cleanup_
    Address: 01:000A63DD

    Text_Box_Get_Input_
    Address: 01:000A644D

    Get_Mox_Text_Box_Base_Colors_
    Address: 01:000A64AB

    Get_Help_Lbx_Name_
    Address: 01:000A6547

    data (0 bytes) _textbox_normal_colors
    Address: 02:0017A400

    data (0 bytes) _textbox_normal2_colors
    Address: 02:0017A408

    data (0 bytes) _default_textbox_y
    Address: 02:0017A410

    data (0 bytes) _textbox_heights
    Address: 02:0017A415

    data (0 bytes) _helpbox_text_heights
    Address: 02:0017A41F

    data (0 bytes) _textbox_text_dy
    Address: 02:0017A429

    data (0 bytes) _field_save_ix
    Address: 02:0017A433

    data (0 bytes) _reload_saved_block
    Address: 02:0019536A
*/
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr159
*/

// WZD o159p01
void Draw_Help_Entry(int16_t help_entry_idx);

// WZD o159p02
// drake178: GUI_Help_RedrawFn()
void Draw_Help_Entry_Draw(void);

// WZD o159p03
// drake178: EMM_Map_VGAFILEH1_5()
uint16_t EMM_Map_VGAFILEH1_5(void);


#ifdef __cplusplus
}
#endif

#endif  /* TEXTBOX_H */
