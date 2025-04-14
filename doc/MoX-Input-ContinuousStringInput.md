

GUI_EditBoxControl()  ==>  Input_Box_Popup()
GUI_TextEditDialog()  ==>  Setup_Input_Box_Popup()
GUI_CreateEditBox()   ==>  Add_Input_Field()
NameStartingCity_Dialog_Popup()  ==>  Change_Home_City_Name_Popup()


2025030:
Module: NAMESTAR
    code (0 bytes) Draw_Input_Box_Popup_
    Address: 01:00091B89
        Locals:

    code (0 bytes) Remapped_Input_Box_Popup_
    Address: 01:00091BB4
        Locals:

    code (0 bytes) Setup_Input_Box_Popup_
    Address: 01:00091BD4
        Locals:

    code (0 bytes) Input_Box_Popup_
    Address: 01:00091F14
        Locals:

// WZD s36p32
// drake178: GUI_TextEditDialog()
// MoO2  Module: Module: NAMESTAR  Input_Box_Popup_()
// 1oom  uiobj.c  uiobj_textinput_do()
/*
; clears the GUI, creates an edit box with the passed
; parameters, and transfers control to it until it
; loses focus, at which point the resulting string is
; saved to the passed pointer and the function returns
; -1 if the Esc key was pressed, or 0 otherwise
*/
/*

NameStartingCity_Dialog_Popup()
    start_x = 60;
    start_y = 30;
    Setup_Input_Box_Popup((start_x + 16), (start_y + 21), 75, Text, 12, 0, 0, 0, &color_array[0], ST_UNDEFINED)

*/
int16_t Setup_Input_Box_Popup(int16_t x_start, int16_t y_start, int16_t width, char * string, int16_t max_characters, int16_t fill_color, int16_t justification, int16_t cursor_type, uint8_t colors[], int16_t help)





¿ MoO2 ?
¿ 1oom ?



e.g., Load/Save Screen
Draw_Fields() prints the strings of the fields.
left-click on one of the fields
prints highlighted, draws marker picture, draws animated cursor  (and draws "Load" and "Save" buttons)
BUG: saves "_", but also adds another and then removes both
¿ how to exit input from keyboard ? arrow off and hitting enter does not... does nothing?



Where does it write the 'Game Name's back to the MAGIC.SET?
...on Quit?
...on Load?
...on Save?





MoO2
    Add_Input_Field()
    Input_String()
    Display_Input_Field()
    Get_Input_String()
    function (0 bytes) Draw_Visible_Fields_Back
    Address: 01:001174CB
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            signed integer (2 bytes) i
            signed integer (2 bytes) j
            signed integer (2 bytes) height
            signed integer (2 bytes) space
            signed integer (2 bytes) temp_value
            signed integer (2 bytes) var_segment
            signed integer (2 bytes) var_offset
            signed integer (2 bytes) cursor_change
            signed integer (2 bytes) old_window_x1
            signed integer (2 bytes) old_window_x2
            signed integer (2 bytes) old_window_y1
            signed integer (2 bytes) old_window_y2




¿ Auto_Input() ?

## Draw_Fields()
    switch(p_fields[itr_fields_count].type)
        case ft_ContinuousStringInput:  /* 11  0x0B */  // drake178: EditSelect
            active_input_field_number


## active_input_field_number


### GUI_EditAnimStage
{0,1,2,3}
incrememnted on every call to Draw_Fields(), reset to 0 after 3

### Text_Change
if((GUI_EditAnimStage / 2) != GUI_EditCursorOn)
    Text_Change = ST_TRUE;

### GUI_EditCursorOn

XREF:
    Draw_Fields+10EB                cmp     ax, [GUI_EditCursorOn]        
    Draw_Fields+10FE                mov     [GUI_EditCursorOn], ax        
    Draw_Fields+1118                cmp     [GUI_EditCursorOn], 0         
    GUI_EditBoxRedraw+2F9           cmp     [GUI_EditCursorOn], 0         
    GUI_EditBoxRedraw+300           mov     [GUI_EditCursorOn], 1         
    GUI_EditBoxRedraw+30C           cmp     [GUI_EditCursorOn], 1         
    GUI_EditBoxRedraw+313           mov     [GUI_EditCursorOn], 0         
    GUI_EditBoxRedraw+330           cmp     [GUI_EditCursorOn], 0         
    Setup_Input_Box_Popup+77           mov     [GUI_EditCursorOn], 0         
    Interpret_Mouse_Input+360       mov     [GUI_EditCursorOn], e_ST_FALSE
    Interpret_Mouse_Input+39B       mov     [GUI_EditCursorOn], e_ST_FALSE
    Interpret_Mouse_Input+8C6       mov     [GUI_EditCursorOn], 0         
    Interpret_Mouse_Input+906       mov     [GUI_EditCursorOn], 0         
    Interpret_Mouse_Input+D09       mov     [GUI_EditCursorOn], 0         
    Interpret_Mouse_Input+D48       mov     [GUI_EditCursorOn], 0         
    Interpret_Mouse_Input:loc_26D40 mov     [GUI_EditCursorOn], 0         
    RP_GUI_KeyInputOnly+2F2         mov     [GUI_EditCursorOn], 0         
    RP_GUI_KeyInputOnly+32C         mov     [GUI_EditCursorOn], 0         
    RP_GUI_KeyInputOnly:loc_27EF7   mov     [GUI_EditCursorOn], 0         

## GUI_EditBoxRedraw()

XREF:
    GUI_EditBoxControl+13C call    near ptr GUI_EditBoxRedraw      ; calls any active redraw function, draws the selected
    GUI_EditBoxControl+19C call    near ptr GUI_EditBoxRedraw      ; calls any active redraw function, draws the selected
    GUI_EditBoxControl+3C0 call    near ptr GUI_EditBoxRedraw      ; calls any active redraw function, draws the selected
    Setup_Input_Box_Popup+DD  call    near ptr GUI_EditBoxRedraw      ; calls any active redraw function, draws the selected
    Setup_Input_Box_Popup+13D call    near ptr GUI_EditBoxRedraw      ; calls any active redraw function, draws the selected
    Setup_Input_Box_Popup+359 call    near ptr GUI_EditBoxRedraw      ; calls any active redraw function, draws the selected


## Setup_Input_Box_Popup()

XREF:
    NameStartingCity_Dialog_Popup+12E call    Setup_Input_Box_Popup
    NameOutpost_Dialog_Popup+147      call    Setup_Input_Box_Popup
    Hire_Hero_Popup+504               call    Setup_Input_Box_Popup



Where is the draw of the `marker_picture`?
    Draw_Fields()
        switch(p_fields[itr_fields_count].type)
            case ft_ContinuousStringInput:  /* 11  0x0B */  // drake178: EditSelect
                if(p_fields[itr_fields_count].marker_picture != ST_NULL)
                {
                    FLIC_Draw(p_fields[itr_fields_count].x1, p_fields[itr_fields_count].y1, p_fields[itr_fields_count].marker_picture);
                }

The underscore for the cursor position is in Draw_Fields().
    // WZD dseg:82A6
    char GUI_EditCursor[] = "_";

Text_Change
GUI_EditCursorOn
GUI_EditAnimStage




## Keyboard Keys

Enter
Backspace




Fields.H
enum e_Field_Type
    ft_ContinuousStringInput,   /* 11  0x0B */  // drake178: EditSelect

struct s_Field
        char * string;      //  4  ft_input  11  ft_ContinuousStringInput
        SAMB_ptr marker_picture;  // 11  ft_ContinuousStringInput
        int16_t fill_color;         //  4  ft_input  11  ft_ContinuousStringInput
        int16_t  max_characters;  //  4 ft_Input  11 ft_ContinuousStringInput
        int16_t shadow;           // 11  ft_ContinuousStringInput



## Where used?
Name Game  (Load/Save - Game Name)
Name City
Name Wizard  (New Game - Custom Wizard - Wizard Name)
Name Hero





## 1oom

C:\STU\developp\1oom\src\ui\classic\uiobj.h
C:\STU\developp\1oom\src\ui\classic\uiobj.c

int16_t uiobj_add_textinput(int x, int y, int w, char *buf, uint16_t buflen, uint8_t rcolor, bool alignr, bool allow_lcase, const uint8_t *colortbl, mookey_t key)

bool uiobj_read_str(int x, int y, int w, char *buf, int buflen, uint8_t rcolor, bool alignr, const uint8_t *ctbl)

static bool uiobj_textinput_do(uiobj_t *p, int w, char *buf, int buflen, bool allow_lcase, bool copy_truncated)

int16_t uiobj_add_textinput(int x, int y, int w, char *buf, uint16_t buflen, uint8_t rcolor, bool alignr, bool allow_lcase, const uint8_t *colortbl, mookey_t key)
{
    uiobj_t *p = &uiobj_tbl[fields_count];
    uiobj_add_set_xys(p, x, y, x + w, y + lbxfont_get_height(), ui_scale);
    p->t4.fontnum = lbxfont_get_current_fontnum();
    p->t4.fonta2 = lbxfont_get_current_fonta2();
    p->t4.fonta4 = lbxfont_get_current_fonta2b();
    p->t4.buflen = buflen;
    p->t4.buf = buf;
    p->t4.rectcolor = rcolor;
    p->t4.align_right = alignr;
    p->t4.allow_lcase = allow_lcase;
    p->t4.colortbl = colortbl;
    p->type = 4;
    p->vptr = 0;
    p->key = key;
    return UIOBJI_ALLOC();
}
