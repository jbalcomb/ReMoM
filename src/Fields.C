
#include <string.h>

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Fields.H"
#include "FLIC_Draw.H"
#include "Fonts.H"


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
    WZD  seg036
    MGC  seg034
*/

// WZD s36p38
int16_t Add_Multi_Hot_Key_Field(char * argString)
{
    char varString[30];
    int16_t itr_string_length = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Add_Multi_Hot_Key_Field()\n", __FILE__, __LINE__);
#endif

    strcpy(varString, argString);

    while(varString[itr_string_length] != '\0' && itr_string_length < 30) itr_string_length++;

    p_fields[fields_count].x1 = 500;
    p_fields[fields_count].y1 = 500;
    p_fields[fields_count].x2 = 500;
    p_fields[fields_count].y2 = 500;
    p_fields[fields_count].type = ft_MultiHotKey;
    p_fields[fields_count].help = ST_UNDEFINED;
    p_fields[fields_count].Param0 = argString[0];  // TODO(JimBalcomb,20230612): fix Param0 - ? need char * ?
    p_fields[fields_count].Param1 = 0;
    p_fields[fields_count].Param2 = itr_string_length;
    p_fields[fields_count].hotkey = varString[0];  // TODO(JimBalcomb,20230612): fix hotkey - ? need char * ?

    if ((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123) )
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count +=1;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Add_Multi_Hot_Key_Field()\n", __FILE__, __LINE__);
#endif

    return (fields_count - 1);
}


// WZD s36p42
void Add_Button_Info(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Add_Button_Info()\n", __FILE__, __LINE__);
#endif

    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;

    p_fields[fields_count].Param0 = string;

    p_fields[fields_count].x2 = FLIC_Get_Width(pict_seg) + p_fields[fields_count].x1 - 1;
    p_fields[fields_count].y2 = FLIC_Get_Height(pict_seg) + p_fields[fields_count].y1 - 1;

    p_fields[fields_count].Font_Index = Get_Current_Font_Index();
    p_fields[fields_count].ColorSet1 = Get_Current_Font_Color();

    p_fields[fields_count].help = help;

    // p_fields[fields_count].Param5 = pict_seg;
    // p_fields[fields_count].Param5 = (uint16_t *)pict_seg;
    p_fields[fields_count].pict_seg = pict_seg;

    p_fields[fields_count].Param3 = Get_Full_Store_Flag(pict_seg);

    // TODO(JimBalcomb,20230612): make hotkey make sense - looks like (char *)
    // mov     si, [bp+select_char]
    // mov     al, [si]
    // mov     [byte ptr es:bx+s_Field.hotkey], al

    p_fields[fields_count].hotkey = hotkey;

    if ((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123) )
    {
        p_fields[fields_count].hotkey -= 32;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Add_Button_Info()\n", __FILE__, __LINE__);
#endif
}


// WZD s36p43
int16_t Add_Button_Field(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Add_Button_Field()\n", __FILE__, __LINE__);
#endif

    Add_Button_Info(xmin, ymin, string, pict_seg, hotkey, help);

    p_fields[fields_count].type = ft_Button;

    fields_count +=1;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Add_Button_Field()\n", __FILE__, __LINE__);
#endif

    return (fields_count - 1);
}

// WZD s36p47
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

// WZD s34p49
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

// WZD s34p51
int16_t Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int16_t *xpos, int16_t *ypos, int16_t help)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Add_Grid_Field()\n", __FILE__, __LINE__);
#endif

    p_fields[fields_count].x1 = 500;
    p_fields[fields_count].y1 = 500;

    p_fields[fields_count].x2 = (box_width * horizontal_count) + xmin - 1;
    p_fields[fields_count].y2 = (box_height * vertical_count) + ymin - 1;

    p_fields[fields_count].help = help;

    p_fields[fields_count].type = ft_Grid;

    p_fields[fields_count].Param1 = box_width;
    p_fields[fields_count].Param2 = box_height;

    p_fields[fields_count].hotkey = ST_NULL;

    p_fields[fields_count].Param3 = *xpos;
    p_fields[fields_count].Param4 = *ypos;

    if((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123))
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count += 1;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Add_Grid_Field()\n", __FILE__, __LINE__);
#endif

    return (fields_count - 1);
}

// WZD s36p57
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
