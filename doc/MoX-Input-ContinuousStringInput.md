


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
    GUI_TextEditDialog+77           mov     [GUI_EditCursorOn], 0         
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
    GUI_TextEditDialog+DD  call    near ptr GUI_EditBoxRedraw      ; calls any active redraw function, draws the selected
    GUI_TextEditDialog+13D call    near ptr GUI_EditBoxRedraw      ; calls any active redraw function, draws the selected
    GUI_TextEditDialog+359 call    near ptr GUI_EditBoxRedraw      ; calls any active redraw function, draws the selected


## GUI_TextEditDialog()

XREF:
    NameStartingCity_Dialog_Popup+12E call    GUI_TextEditDialog
    NameOutpost_Dialog_Popup+147      call    GUI_TextEditDialog
    Hire_Hero_Popup+504               call    GUI_TextEditDialog



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
