
HELP.LBX
HLPENTRY.LBX

Help.C
Help.H
TEXTBOX.C
TEXTBOX.H

20250406:
...does not belong here...
but, ...
EMM_Map_VGAFILEH1_5__WIP()
meaningfulness?
***hard-coded for _VGAFILEH_seg; EMM_Map4Pages(emsFirst = 1, emsHandle = _VGAFILEH_seg); returns EMM_PageFrame;***
WTF is the intent of _VGAFILEH_seg?
Why {1,2,3,4} and not 0?



MoO2
help_field_outline_color
help_list_active
help_entry_found
help_pointer
help_list_count
help_box_mode
default_help_entry
Module: TEXTBOX
    Draw_Help_Entry_()

Check_Help_list()
Draw_Help_Entry()

1oom
uihelp.c
void ui_help(int help_index)
    p = &(helplbx[4 + HELP_ITEM_SIZE * help_index]);
    ui_help_draw_str0(p, ctbl, c3, c4, c5, c8, c0, 1);



## Draw_Help_Entry__WIP()

drake178: GUI_Help_RedrawFn()
; displays the indicated context-based help entry
;
; (dumps the first 64k of the sandbox into VGAFILEH
; pages 1-5, and the first 4000 bytes of the LBX near
; buffer into a sandbox allocation)
```c
    SAMB_ptr Loaded_IMG_Segments[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SAMB_ptr help_pict_seg = 0;
    int16_t Image_Start_Top = 0;
    int16_t old_fields_count = 0;
    int16_t Raw_Image_Width = 0;
    int16_t Raw_Image_Height = 0;
    int16_t Current_Entry = 0;
    int16_t LBX_NearBuf_DumpSeg = 0;
    int16_t Title_Font_Height = 0;
    int16_t Image_Top = 0;
    int16_t Image_Start_Left = 0;
    int16_t Max_IMG_Height = 0;
    int16_t Max_IMG_Width = 0;
    int16_t Include_Entry = 0;
    int16_t Title_Height = 0;
    int16_t Title_Top = 0;
    int16_t Title_Indent = 0;
    int16_t Entry_Load_Count = 0;
    int16_t Max_Y = 0;
    int16_t Scroll_Display_Top = 0;
    int16_t Scroll_Display_Left = 0;
    int16_t Bottom_Scroll_Height = 0;
    int16_t Top_Scroll_Height = 0;
    int16_t Entry_Loop_Var = 0;
    SAMB_ptr EMM_PageFrame = 0;
    int16_t Total_Text_Height = 0;
    int16_t Scroll_Bottom_IMG = 0;
    int16_t Big_Scroll_IMG = 0;
    int16_t itr = 0;  // _DI_
    int16_t IDK = 0;  // _SI_
```

Loaded_IMG_Segments[]




Down P Save_ScreenSeg+4    call    j_EMM_Map_VGAFILEH1_5           ; maps in pages 1 - 5 of the VGAFILEH EMM handle
Down P Restore_ScreenSeg+4 call    j_EMM_Map_VGAFILEH1_5           ; maps in pages 1 - 5 of the VGAFILEH EMM handle



; drake178: EMM_Sandbox2VGAFILEH()
; saves the first 64k of the LBX_Sandbox_Segment into pages 1-5 of the VGAFILEH EMS handle
Save_ScreenSeg()
    _SI_EMM_PageFrame = si
    _SI_EMM_PageFrame = EMM_Map_VGAFILEH1_5()
    _fmemcpy(_SI_EMM_PageFrame, _screen_seg, 32768);
    _fmemcpy(_SI_EMM_PageFrame+2048, _screen_seg+2048, 32768);

; drake178: EMM_VGAFILEH2Sandbox()
; loads pages 1-5 of the VGAFILEH EMS handle into the first 64k of the LBX_Sandbox_Segment
Restore_ScreenSeg()
    _SI_EMM_PageFrame = si
    _SI_EMM_PageFrame = EMM_Map_VGAFILEH1_5()
    _fmemcpy(_screen_seg, _SI_EMM_PageFrame, 32768);
    _fmemcpy(_screen_seg+2048, _SI_EMM_PageFrame+2048, 32768);

_fmemcpy(0, (_screen_seg + (2048 * 16)), 0, (_SI_EMM_PageFrame + (2048 * 16)), 32768);
// ; _fmemcpy(0, _screen_seg+32768, 0, EMM_PageFrame+32768, 32768, 0)
BCPP4 src shows memcpy() is an alias for _fmemcpy() for far memory ops
so, same sig, just FAR
size/nbytes is size_t / unsigned long



This is like TEXTBOX in MoO2?
...Push_Block(), Pop_Block()...
...Save_Block_To_Drive(), _reload_saved_block__flag, Restore_Block_From_Drive()...
¿ swap_block_address, swap_saved_block_size ?

drake178: EMM_Sandbox2VGAFILEH()
Save_ScreenSeg()
    Confirmation_Box()
    Warn()
    Selection_Box()
    Notify_Load()
    j_Save_ScreenSeg()
        Do_Lair_Confirm()
        Raze_City_Prompt()
        CMB_CallChaos()
        CMB_CallChaos()

drake178: EMM_VGAFILEH2Sandbox()
Restore_ScreenSeg()
    Confirmation_Box()
    Warn()
    Selection_Box()
    Notify1()
    Notify2()
    j_Restore_ScreenSeg()
        Do_Lair_Confirm()
        Raze_City_Prompt()
        CMB_CallChaos()
        CMB_CallChaos()

...
...
...
    Save_Block_To_Drive(); Allocate_First_Block();
    ||
    Push_Block(); Allocate_Next_Block();
    _textbox_seg = ...
    Buffer_Reload(_global_cache_seg, "textbox.lbx")














¿ further evidence of Input being a library ?
Help.C/.H is MoM, calling to MoX
~ MoX_HLP / MoX_Help is ...
// WZD s36p12, s36p13, s36p14
Set_Help_List(); Deactivate_Help_List(); Check_Help_List();

_help_entries just gets loaded with the bytes of the records of the entry in HLPENTRY.LBX
each record is 10 bytes, 5 2-byte values
    help_idx, x1, y1, x2, y2
but, that can't be quite right, can it?
cause, why would it be directly settings values in _help_entries, if they just get overwritten by LBX_Load_Data_Static()?
or, they are basically doing the same thing, but manually? even manually overwritting the loaded help on purpose?


_help_entries is 500 bytes of 50 * 10 help entry records




HLPENTRY_LBX_001.BIN
"main scrn Help"
Set_Main_Screen_Help_List()
    LBX_Load_Data_Static(hlpentry_lbx_file, 1, (SAMB_ptr)_help_entries, 0, 25, 10);
    Set_Help_List(_help_entries, 25);
@000007BA
25  count
10  size
idx   x1    y1    x2    y2    
216     7     4    45    15     'Game Button' enum HELP_TXT { ..., HLP_GAME                = 216,


_help_entries+bx], HLP_GRAY_GEM
mov     ax, _SI_itr
mov     dx, 10
imul    dx
mov     bx, ax
mov     [word ptr (_help_entries+64h)+bx], 0FFFFh




HELP.LBX
Entries:   3
Type:      5
    HELP_LBX_000.BIN
        num_recs     217
        record_size  200
    HELP_LBX_001.BIN
        num_recs     217
        record_size   23
    HELP_LBX_002.BIN
        num_recs     1614
        record_size  1048

HLPENTRY.LBX
Entries:  41
Type:      5





WZD dseg:BB7A
_help_entries db 500 dup(0);
uint8_t _help_entries[500];

¿ MUST BE int16_t to support/align with being set to ST_UNDEFINED ?
Dasm shows explicit uses of `word ptr`
¿ also noteable ? ¿ NEVER uses `OFFSET` or `LEA` ?

enum {214, ..., 806}
    HLP_PAGE_BACK               = 214,
    ...
    HLP_RAZE_CITY               = 806



//  ; (sizeof=0xA)
struct s_HLP_ENTRY
{
    int16_t help_idx;  // enum HELP_TXT
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
};

//  ; (sizeof=0x418)
struct s_HLP_DATA
{
    char title[30];
    char lbx_name[14];
    int16_t lbx_entry;
    int16_t Include_Entry;
    char help_text[1000];
};

MoX_Data.C/.H
// WZD dseg:E872
// AKA _help_list
// struct s_HLP_ENTRY * help_struct_pointer[50];
/*
    ¿ pointer to an array of pointers of data type 'struct s_HLP_ENTRY' ?
*/
// struct s_HLP_ENTRY ** help_struct_pointer;
struct s_HLP_ENTRY * help_struct_pointer;




As seen in Magic_Screen(), ...

mov     ax, _SI_itr
mov     dx, 10
imul    dx
mov     bx, ax
mov     [word ptr _help_entries+bx], HLP_GRAY_GEM
mov     ax, _SI_itr
mov     dx, 10
imul    dx
mov     bx, ax
mov     [word ptr (_help_entries+64h)+bx], 0FFFFh

¿ Why the multiply by 10 ?
¿ Would be 10 sizeof(int16_) - 20 bytes ?
¿ because, record_size is 10 ?
