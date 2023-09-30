
HELP.LBX
HLPENTRY.LBX

Help.C
Help.H


¿ further evidence of Input being a library ?
Help.C/.H is MoM, calling to MoX
~ MoX_HLP / MoX_Help is ...
// WZD s36p12, s36p13, s36p14
Set_Help_List(); Deactivate_Help_List(); Check_Help_List();

_help_entries just gets loaded with the bytes of the records of the entry in HLPENTRY.LBX
each record is 10 bytes
    ¿ uint16_t help_idx; int16_t x1; int16_t y1; int16_t x2; int16_t y2; ?
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
