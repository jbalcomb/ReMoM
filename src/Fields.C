

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Fields.H"


/*
    Initialized Data
*/
// TODO  int16_t _global_esc = ST_FALSE;
// TODO  int16_t help_list_active = ST_FALSE;
// TODO  int16_t help_list_count = 0;
// TODO  int16_t focused_field = ST_UNDEFINED;
// TODO  int16_t mouse_field = ST_FALSE;
// TODO  int16_t input_field_active = ST_FALSE;
// TODO  int16_t active_input_field_number = ST_UNDEFINED;
/*
    Uninitialized Data
*/
// TODO  int help_list;
// TODO  int16_t input_delay;
// TODO  int16_t cursor_offset;
int16_t fields_count;
struct s_Field * p_fields;



/*
    MGC  seg034
*/

// MGC s34p47
int16_t Add_Hidden_Field(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, int16_t hotkey, int16_t help)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Add_Hidden_Field(xmin = %d, ymin = %d, xmax = %d, ymax = %d, hotkey = %s, button_sound = %d)\n", __FILE__, __LINE__, xmin, ymin, xmax, ymax, hotkey, help);
#endif

    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;
    p_fields[fields_count].x2 = xmax;
    p_fields[fields_count].y2 = ymax;
    p_fields[fields_count].type = ft_HotKey;
    p_fields[fields_count].help = help;
    p_fields[fields_count].hotkey = hotkey;

    if ((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123) )
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count += 1;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Add_Hidden_Field(xmin = %d, ymin = %d, xmax = %d, ymax = %d, hotkey = %s, button_sound = %d) { (fields_count - 1) = %d }\n", __FILE__, __LINE__, xmin, ymin, xmax, ymax, hotkey, help, (fields_count - 1));
#endif

    return (fields_count - 1);
}

// MGC s34p49
int16_t Add_Hot_Key(int16_t select_char)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Add_Hot_Key(select_char = %d)\n", __FILE__, __LINE__, select_char);
#endif

    p_fields[fields_count].x1 = 500;
    p_fields[fields_count].y1 = 500;
    p_fields[fields_count].x2 = 500;
    p_fields[fields_count].y2 = 500;
    p_fields[fields_count].type = ft_HotKey;
    p_fields[fields_count].help = ST_UNDEFINED;
    p_fields[fields_count].hotkey = select_char;

    if((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123))
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count += 1;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Add_Hot_Key(select_char = %d) { (fields_count - 1) = %d }\n", __FILE__, __LINE__, select_char, (fields_count - 1));
#endif

    return (fields_count - 1);
}

// MGC s34p57
void Clear_Fields(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Clear_Fields()\n", __FILE__, __LINE__);
#endif

    /* ? down_mouse_button = ST_UNDEFINED; ? */
    fields_count = 1;
    // TODO  focused_field = ST_UNDEFINED;
    // TODO  mouse_field = ST_FALSE;
    // TODO  input_field_active = ST_FALSE;
    // TODO  active_input_field_number = ST_UNDEFINED;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Clear_Fields()\n", __FILE__, __LINE__);
#endif

}
