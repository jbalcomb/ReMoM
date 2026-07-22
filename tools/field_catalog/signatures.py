"""
signatures.py — positional argument schema for each Add_*Field function.

Signatures are transcribed from MoX/src/Fields.h. Each schema names which
positional argument holds which role, and how the clickable extent is derived:

  geom_kind:
    'rect'   explicit xmin,ymin,xmax,ymax               (Add_Hidden_Field)
    'wh'     xmin,ymin + explicit width,height          (Add_Scroll_Field)
    'w_only' xmin,ymin + width; height not in the call   (input fields)
    'grid'   xmin,ymin + box_w,box_h,cols,rows           (Add_Grid_Field)
    'sprite' xmin,ymin only; extent comes from the sprite (Add_Button/Picture)
    'none'   no geometry — a bare hotkey registration     (Add_Hot_Key, Multi)

  hotkey: positional index of an int16 hotkey arg, or None when the function
          takes no simple integer hotkey (char* select_char, multi-key string,
          or no hotkey at all).
"""

# add_func -> schema dict. Indices are 0-based positions in the argument list.
SIGNATURES = {
    # int16_t Add_Hidden_Field(xmin, ymin, xmax, ymax, hotkey, help)
    "Add_Hidden_Field": {"geom_kind": "rect", "xmin": 0, "ymin": 1, "xmax": 2, "ymax": 3, "hotkey": 4},
    # int16_t Add_Button_Field(xmin, ymin, string, pict_seg, hotkey, help)
    "Add_Button_Field": {"geom_kind": "sprite", "xmin": 0, "ymin": 1, "hotkey": 4},
    # int16_t Add_Picture_Field(xmin, ymin, pict_seg, hotkey, help)
    "Add_Picture_Field": {"geom_kind": "sprite", "xmin": 0, "ymin": 1, "hotkey": 3},
    # int16_t Add_Hot_Key(select_char)
    "Add_Hot_Key": {"geom_kind": "none", "hotkey": 0},
    # int16_t Add_Multi_Hot_Key_Field(string)   -- hotkeys packed in a string
    "Add_Multi_Hot_Key_Field": {"geom_kind": "none", "hotkey": None},
    # int16_t Add_Scroll_Field(xmin, ymin, min_value, max_value, min_valid, max_valid, width, height, *var, hotkey, help)
    "Add_Scroll_Field": {"geom_kind": "wh", "xmin": 0, "ymin": 1, "width": 6, "height": 7, "hotkey": 9},
    # int16_t Add_Grid_Field(xmin, ymin, box_width, box_height, horizontal_count, vertical_count, *xpos, *ypos, help)
    "Add_Grid_Field": {"geom_kind": "grid", "xmin": 0, "ymin": 1, "box_w": 2, "box_h": 3, "cols": 4, "rows": 5, "hotkey": None},
    # int16_t Add_Input_Field(xmin, ymin, width, string, max_characters, fill_color, justification, cursor_type, color_array[], select_char, help)
    "Add_Input_Field": {"geom_kind": "w_only", "xmin": 0, "ymin": 1, "width": 2, "hotkey": None},
    # int16_t Add_Continuous_String_Input_Field(xmin, ymin, width, string, max_characters, fill_color, marker_picture, help, shadow)
    "Add_Continuous_String_Input_Field": {"geom_kind": "w_only", "xmin": 0, "ymin": 1, "width": 2, "hotkey": None},
    # int16_t Add_String_List_Field(x1, y1, width, string, active_flag, variable, set_value, fill_type, fp1, fp2, fp3, fp4, hotkey(char*), help)
    "Add_String_List_Field": {"geom_kind": "w_only", "xmin": 0, "ymin": 1, "width": 2, "hotkey": None},
}

# The set the scanner looks for. Kept in one place so scan and build agree.
KNOWN_ADD_FUNCS = tuple(SIGNATURES.keys())
