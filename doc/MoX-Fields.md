

SEE ALSO: MoX_Input.md, MoM-Input.md
...
MoM-Input.md
    DEPRECATED
    SEEALSO: MoX_Input.md
...

SimTex used "field_num", but it's Zero-Based, so STU uses "field_idx".
¿ "style_num" as in Font Style Index ?



What uses these?
0000000E ColorSet1 dw ?
00000010 Rect_FX dw ?
00000012 Selectable dw ?
00000014 ColorSet2 dw ?

Add_Button_Info()
    p_fields[fields_count].Font_Index = Get_Current_Font_Index();
    p_fields[fields_count].ColorSet1 = Get_Current_Font_Color();
...
Draw_Field()
    Set_Font(p_fields[field_num].Font_Index, p_fields[field_num].ColorSet1, 0, 0);

So, should be normal_colors, ..., highlight_colors.

Push_Field_Down()
    if( (p_fields[down_mouse_button].type == ft_StringList) && (p_fields[field_num].Selectable != ST_FALSE) )
        Draw_Field(down_mouse_button, 0);





        Button Input Scroll MultiHotKey ClickLink StringList ContinuousStringInput Grid
       ? ft 0 ?     ft 4     ft 6     ft 8   ft 9   ft 10  ft 11     ft 12
Param0 Text@                 Min_Val  Str*   Parent
Param1 Trig_State            Max_Val  StrIdx Index  Index  IMG       box_width
Param2 State@       BG_Color StSeg    StrLen               BG_Color  box_height
Param3 FullFrames   Align    MinSlide               CBlock           xpos
Param4 MaxSlide     CStyle                                           ypos
Param5 pict_seg     MaxLen   Orient                        MaxLen
Param6 CursorColors          StOff                         Outline

ft_Input
Param6 AKA CorsorColors@ AKA CCol@
in GUI_CreateEditBox()
ITEM_CreateControls() passes COL_IC_EditBox@
WZD dseg:C8CE COL_IC_EditBox@ ; ~ [4][4] {177,178,179,180}
COL_IC_EditBox@ = Near_Allocate_Next(16)
a pointer to a 16 byte near allocation
which is 4 sets of 4 colors, each the same colors








Input Field Type #6: Slider
Parameters: 1,2,3,4,5,6

x1 = xmin
y1 = ymin
x2 = x1 + width
y2 = y1 + height
type = ft_Scroll
help = help
...
Font_Index  N/A
ColorSet1   N/A
Rect_FX     N/A
Selectable  N/A
ColorSet2   N/A
...
hotkey = hotkey


Param0 = min_valid
Param1 = max_valid

Param3 = min_value
Param4 = max_value  ()

Param2 = sgmt_variable
Param6 = ofst_variable

e.g., Alchemy_Popup_Add_Fields()
Add_Scroll_Field((m_alchemy_popup_start_x + 50), (m_alchemy_popup_start_y + 25), 0, 55, 3, 53, 55, 7, &GAME_AlchSliderState, cnst_HOTKEY_A_2, ST_UNDEFINED);

Add_Scroll_Field()
xmin = m_alchemy_popup_start_x + 50
ymin = m_alchemy_popup_start_y + 25
min_value =  0
max_value = 55
min_valid =  3
max_valid = 53
width  = 55
height =  7
variable = &GAME_AlchSliderState
hotkey   = "A"
help     = -1


GUI_SliderUpdate()

...Slide_Value...
    return_value = Slide_Value
    return_value = Param3
    return_value = Param3 + Slide_Value
    return_value = Param4

sgmt_variable = p_fields[field_num].Param2
ofst_variable = p_fields[field_num].Param6
*((int16_t)MK_FP(sgmt_variable,ofst_variable)) = return_value


ft_Grid
Draw_Field()
    grid_x = field_x / p_fields[field_num].Param1;  // always 12  box_width
    grid_y = field_y / p_fields[field_num].Param2;  // always 10  box_height
    *((int64_t *)p_fields[field_num].Param3) = grid_x;
    *((int64_t *)p_fields[field_num].Param4) = grid_y;
grid_x, grid_y are int16_t
Param3, Param4 are the addresses of the variables
...cast the address to a pointer, dereference the pointer to set the value



MoO2
Draw_Field()
...
signed integer (2 bytes) var_segment
signed integer (2 bytes) var_offset





## 1oom

C:\STU\developp\1oom\src\ui\classic\uiobj.c


typedef struct uiobj_s {
    /*00*/ uint16_t x0;
    /*02*/ uint16_t y0;
    /*04*/ uint16_t x1;    /* inclusive */
    /*06*/ uint16_t y1;    /* inclusive */
    /* 13 types
        t0..3: buttons with gfx (and optional text) with different highlight conditions
        t1: toggle
        t2: set1
        t3: setval
        t4: textinput
        t6: slider
        t7: mousearea or inputkey
        t8: altstr
        ta: text line
        tb: scroll area
        tc: mouse wheel area
    */


static void uiobj_slider_plus(uiobj_t *p, int adj)
static void uiobj_slider_minus(uiobj_t *p, int adj)

int16_t uiobj_add_t0(uint16_t x, uint16_t y, const char *str, uint8_t *lbxdata, mookey_t key)
int16_t uiobj_add_t1(uint16_t x, uint16_t y, const char *str, uint8_t *lbxdata, int16_t *vptr, mookey_t key)
int16_t uiobj_add_t2(uint16_t x, uint16_t y, const char *str, uint8_t *lbxdata, int16_t *vptr, mookey_t key)
int16_t uiobj_add_t3(uint16_t x, uint16_t y, const char *str, uint8_t *lbxdata, int16_t *vptr, int16_t z18, mookey_t key)
int16_t uiobj_add_textinput(int x, int y, int w, char *buf, uint16_t buflen, uint8_t rcolor, bool alignr, bool allow_lcase, const uint8_t *colortbl, mookey_t key)
int16_t uiobj_add_slider_int(uint16_t x0, uint16_t y0, uint16_t vmin, uint16_t vmax, uint16_t w, uint16_t h, int16_t *vptr)
int16_t uiobj_add_slider_func(uint16_t x0, uint16_t y0, uint16_t vmin, uint16_t vmax, uint16_t w, uint16_t h, int16_t *vptr, void (*cb)(void *ctx, uint8_t slideri, int16_t value), void *ctx, uint8_t slideri)
int16_t uiobj_add_mousearea(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, mookey_t key)
int16_t uiobj_add_mousearea_limited(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t scale, mookey_t key)
int16_t uiobj_add_mousewheel(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, int16_t *vptr)
int16_t uiobj_add_mousearea_all(mookey_t key)
int16_t uiobj_add_inputkey(uint32_t key)
int16_t uiobj_add_alt_str(const char *str)
int16_t uiobj_add_ta(uint16_t x, uint16_t y, uint16_t w, const char *str, bool z12, int16_t *vptr, int16_t z18, uint8_t subtype, uint8_t sp0v, mookey_t key)
int16_t uiobj_add_tb(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xscale, uint16_t yscale, int16_t *xptr, int16_t *yptr, uint8_t *zptr, uint8_t zmax)


## MoO2

"variable"
Add_Scroll_Field()
cseg01:00115622 mov     eax, dword ptr y_pointer        ; fields_count: mov eax, dword ptr y_pointer; sar eax, 16
cseg01:00115627 sar     eax, 10h
cseg01:0011562A imul    eax, 37h
cseg01:0011562D mov     edx, p_fields
cseg01:00115633 add     edx, eax
cseg01:00115635 mov     eax, [ebp+variable]
cseg01:00115638 mov     dword ptr [edx+s_field.fill_color__variable], eax ; NOTE: `dword p

dseg02:001A8848
// ; 13750 / 55 = 250
// sizeof(struct s_field):  37h  50d
struct s_field sfields[250]
dseg02:0017C480
p_fields dd offset sfields
dseg02:0017C484
old_fields dd offset sfields





struct s_Field
enum e_Field_Type

¿ Parameters Per Field Type ?


enum e_Field_Type
{
    ft_Button,                  /*  0  0x00 */  // drake178: TODO
    ft_RadioButton,             /*  1  0x01 */  // drake178: ToggleButton
    ft_LockedButton,            /*  2  0x02 */  // drake178: LockableButton
    ft_MultiButton,             /*  3  0x03 */  // drake178: MStateButton
    ft_Input,                   /*  4  0x04 */  // drake178: EditBox
    ft_Picture,                 /*  5  0x05 */  // drake178: ImageLabel      DNE/NIU in MoO2
    ft_Scroll,                  /*  6  0x06 */  // drake178: SlideBar
    ft_HotKey,                  /*  7  0x07 */  // drake178: Label
    ft_MultiHotKey,             /*  8  0x08 */  // drake178: Ctrl_AltString
    ft_ClickLink,               /*  9  0x09 */  // drake178: ClickLink       DNE/NIU in MoO2
    ft_StringList,              /* 10  0x0A */  // drake178: DialogLine
    ft_ContinuousStringInput,   /* 11  0x0B */  // drake178: EditSelect
    ft_Grid                     /* 12  0x0C */  // drake178: TODO
    // MoO2 ft_IrregularButton  /* 13  0x0D */
    // MoO2 ft_Circle           /* 14  0x0E */
    // MoO2 ?!? Add_String_Field() ?!?
};
