


MagicScr.C
// WZD o73p06
void Alchemy_Popup(int16_t start_x, int16_t y_start);
// WZD o73p07
void Alchemy_Popup_Draw(void);
// WZD o73p08
void Alchemy_Popup_Add_Fields(void);




calculation
postion




Alchemy_Popup()
    m_alchemy_bar_bitm_seg = Allocate_First_Block(_screen_seg, 40);  // 40 PR, 640 B
    // MAGIC.LBX, 052  ALCHEMY    alchemy background
    // MAGIC.LBX, 053  ALCHBUTN   alchemy cancel
    // MAGIC.LBX, 054  ALCHBUTN   alchemy ok
    // MAGIC.LBX, 055  ALCHBUTN   alchemy right arro
    // MAGIC.LBX, 056  ALCHBUTN   alchemy left arrow
    // MAGIC.LBX, 057  ARROWBAR   alchemy lines >>>
    // MAGIC.LBX, 058  ALCHSTAR   alchemy star
    // MAGIC.LBX, 059  ALCHEMY    alchemy pow2gold
    m_alchemy_background_seg  = LBX_Reload_Next(magic_lbx_file, 52, _screen_seg);
    m_alchemy_cancel_button_seg = LBX_Reload_Next(magic_lbx_file, 53, _screen_seg);
    m_alchemy_ok_button_seg   = LBX_Reload_Next(magic_lbx_file, 54, _screen_seg);
    m_alchemy_left_arrow_button_seg = LBX_Reload_Next(magic_lbx_file, 56, _screen_seg);
    m_alchemy_right_arrow_button_seg = LBX_Reload_Next(magic_lbx_file, 55, _screen_seg);
    m_alchemy_bar_pict_seg  = LBX_Reload_Next(magic_lbx_file, 57, _screen_seg);
    m_alchemy_star_seg = LBX_Reload_Next(magic_lbx_file, 58, _screen_seg);
    m_alchemy_pow2gold_seg = LBX_Reload_Next(magic_lbx_file, 59, _screen_seg);


void Alchemy_Popup_Add_Fields(void)
    Add_Scroll_Field((m_alchemy_popup_start_x + 50), (m_alchemy_popup_start_y + 25), 0, 55, 3, 53, 55, 7, &m_alchemy_arrowbar_pos, cnst_HOTKEY_A_2[0], ST_UNDEFINED);




