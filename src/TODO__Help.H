#ifndef HELP_H
#define HELP_H

#include "MoX_TYPE.H"
#include "MoX_DEF.H"


//  ; (sizeof=0xA)
struct s_HLP_ENTRY
{
    int16_t help_idx;
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


// WZD dseg:8296                                                 ¿ BEGIN: Help - Initialized Data ?

// WZD dseg:8296
// AKA have_help
extern int16_t help_list_active;

// WZD dseg:8298
// AKA _help_count
extern int16_t help_list_count;

// WZD dseg:8298                                                 ¿ END: Help - Initialized Data ?

// WZD dseg:E872                                                 ¿ BEGIN: fields, intput, ... ?
// WZD dseg:E872                                                 ¿ BEGIN: Help - Uninitialized Data ?

// WZD dseg:E872
// AKA _help_list
// extern struct s_HLP_ENTRY * help_struct_pointer;
/*
    ¿ pointer to an array of pointers of data type 'struct s_HLP_ENTRY' ?
*/
extern struct s_HLP_ENTRY ** help_struct_pointer;


// WZD dseg:E872                                                 ¿ END: Help - Uninitialized Data ?
// ...
// ...
// ...
// WZD dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WZD  seg036
    MGC  seg034
*/

// WZD s36p12
// void Set_Help_List(struct s_HLP_ENTRY * help_pointer, int16_t count);
// void Set_Help_List(struct s_HLP_ENTRY * help_pointer[], int16_t count);
void Set_Help_List(struct s_HLP_ENTRY ** help_pointer, int16_t count);

// WZD s36p13
void Deactivate_Help_List(void);

// WZD s36p14
int16_t Check_Help_List(void);

// WZD o159p01
void Draw_Help_Entry_(int16_t help_entry_idx);


#ifdef __cplusplus
}
#endif



#endif /* HELP_H */
