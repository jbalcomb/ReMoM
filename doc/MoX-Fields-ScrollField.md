
In MoM,
Magic Screen Staves (3)
Alchemy Popup
Small Spellbook Extra Mana
Combat Spellbook Extra Mana



Fields.C
int16_t Add_Scroll_Field(int16_t xmin, int16_t ymin, int16_t min_value, int16_t max_value, int16_t min_valid, int16_t max_valid, int16_t width, int16_t height, int16_t * variable, int16_t hotkey, int16_t help)

    p_fields[fields_count].min_valid = min_valid;
    p_fields[fields_count].max_valid = max_valid;
    p_fields[fields_count].min_value = min_value;
    p_fields[fields_count].max_value = max_value;
    p_fields[fields_count].variable = variable;

`variable` is the current value

¿ min/max value vs. min/max valid ?



m_alchemy_arrowbar_pos = 3;

Add_Scroll_Field((m_alchemy_popup_start_x + 50), (m_alchemy_popup_start_y + 25), 0, 55, 3, 53, 55, 7, &m_alchemy_arrowbar_pos, cnst_HOTKEY_A_2[0], ST_UNDEFINED);

min_valid:   0
max_valid:  55
min_value:   3
max_value:  53
variable:   &m_alchemy_arrowbar_pos

m_alchemy_amount = ((_players[HUMAN_PLAYER_IDX].gold_reserve * (m_alchemy_arrowbar_pos - 3)) / conversion_rate);

¿ ` - 3` in calc is just min_value ?
