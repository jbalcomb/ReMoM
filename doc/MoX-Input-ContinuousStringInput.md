


¿ MoO2 ?
¿ 1oom ?





¿ Auto_Input() ?

Draw_Fields()
    active_input_field_number





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
