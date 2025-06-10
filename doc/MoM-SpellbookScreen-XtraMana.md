
"XTRAMANA"
'Scroll Field'

// SPELLSCR.LBX, 003  "XTRAMANA"    "star"
// SPELLSCR.LBX, 004  "XTRAMANA"    "arrow bar"
// SPELLSCR.LBX, 005  "XTRAMANA"    "background"
// SPELLSCR.LBX, 042  "XTRABUTT"    "var ok button"

SPLMASTR.C
// WZD o136p05
void Combat_Spellbook_Mana_Adder_Load(void)
// WZD o136p06
void Combat_Spellbook_Mana_Adder_Draw(void)
// WZD o136p07
int16_t Combat_Spellbook_Mana_Adder_Screen(int16_t spell_idx, int16_t spellbook_field_idx, int16_t caster_idx)





GAME_MP_SpellVar_1
_xtra_mana_pos
CMB_SliderLimit



## CMB_SliderLimit
Combat_Spellbook_Mana_Adder_Screen()
    ...just calcs and sets
Combat_Spellbook_Mana_Adder_Draw()
    GAME_MP_SpellVar_1 = (((_xtra_mana_pos - 3) * CMB_SliderLimit) / 100);


    
## GAME_MP_SpellVar_1

Combat_Spellbook_Mana_Adder_Draw()

xor     ax, ax
mov     dx, 100                         ; ¿ ~ conversion_rate ?
push    ax
push    dx                              ; divisor
mov     ax, [_xtra_mana_pos]
cwd
add     ax, -3
adc     dx, 0FFFFh
push    ax
mov     ax, [CMB_SliderLimit]           ; is this really necessary?
push    dx
cwd
pop     cx
pop     bx
call    LXMUL@                          ; #2 * #3 / #1
push    dx
push    ax                              ; dividend
call    LDIV@                           ; #2 * #3 / #1
mov     [GAME_MP_SpellVar_1], ax

GAME_MP_SpellVar_1 = (((_xtra_mana_pos - 3) * CMB_SliderLimit) / 100);

...itoa()...Print_Right();
"+1"

NOTE: the 100 here is ~conversion_rate for 'Alchemy Popup'
    m_alchemy_amount = ((_players[HUMAN_PLAYER_IDX].gold_reserve * (m_alchemy_arrowbar_pos - 3)) / conversion_rate);



## _xtra_mana_pos






    GAME_MP_SpellVar_1 = (((_xtra_mana_pos - 3) * CMB_SliderLimit) / 100);


Fields.C
int16_t Add_Scroll_Field(int16_t xmin, int16_t ymin, int16_t min_value, int16_t max_value, int16_t min_valid, int16_t max_valid, int16_t width, int16_t height, int16_t * variable, int16_t hotkey, int16_t help)

    p_fields[fields_count].min_valid = min_valid;
    p_fields[fields_count].max_valid = max_valid;
    p_fields[fields_count].min_value = min_value;
    p_fields[fields_count].max_value = max_value;
    p_fields[fields_count].variable = variable;

_xtra_mana_pos = 3;

Add_Scroll_Field((x_start + 12), (y_start + 22), 0, 106, 3, 103, 106, 7, &_xtra_mana_pos, (int16_t)str_empty_string__ovr136[0], ST_UNDEFINED);

min_valid:    0
max_valid:  106
min_value:    3
max_value:  103
variable:   _xtra_mana_pos

