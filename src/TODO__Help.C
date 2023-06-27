
#include "MoX_TYPE.H"

#include "Help.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif





// WZD dseg:8296                                                 ¿ BEGIN: Help - Initialized Data ?

// WZD dseg:8296
// AKA have_help
int16_t help_list_active = ST_FALSE;

// WZD dseg:8298
// AKA _help_count
int16_t help_list_count = 0;

// WZD dseg:8298                                                 ¿ END: Help - Initialized Data ?


// WZD dseg:E872                                                 ¿ BEGIN: fields, intput, ... ?
// WZD dseg:E872                                                 ¿ BEGIN: Help - Uninitialized Data ?

// WZD dseg:E872
// AKA _help_list
// struct s_HLP_ENTRY * help_struct_pointer[50];
/*
    ¿ pointer to an array of pointers of data type 'struct s_HLP_ENTRY' ?
*/
struct s_HLP_ENTRY ** help_struct_pointer;


// WZD dseg:E872                                                 ¿ END: Help - Uninitialized Data ?
// ...
// ...
// ...
// WZD dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard



/*
    WZD  seg036
    MGC  seg034
*/

// WZD s36p12
/*
function (0 bytes) Set_Help_List
Address: 01:001196B8
Return type: void (1 bytes) 
pointer (4 bytes) help_pointer
signed integer (2 bytes) count
*/
// void Set_Help_List(struct s_HLP_ENTRY * help_pointer, int16_t count)
// void Set_Help_List(struct s_HLP_ENTRY * help_pointer[], int16_t count)
void Set_Help_List(struct s_HLP_ENTRY ** help_pointer, int16_t count)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Set_Help_List()\n", __FILE__, __LINE__);
#endif

    help_struct_pointer = help_pointer;
    help_list_count = count;
    help_list_active = ST_TRUE;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Set_Help_List()\n", __FILE__, __LINE__);
#endif
}

// WZD s36p13
void Deactivate_Help_List(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Deactivate_Help_List()\n", __FILE__, __LINE__);
#endif

    help_list_active = ST_FALSE;
    help_list_count = 0;
    help_struct_pointer = ST_NULL;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Deactivate_Help_List()\n", __FILE__, __LINE__);
#endif
}

// WZD s36p14
// CAUTION: returns ZERO on SUCCESS
int16_t Check_Help_List(void)
{
    int16_t help_entry_found;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Check_Help_List()\n", __FILE__, __LINE__);
#endif

    help_entry_found = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Check_Help_List()\n", __FILE__, __LINE__);
#endif
    return help_entry_found;
}

// WZD o159p01
void Draw_Help_Entry_(int16_t help_entry_idx)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Draw_Help_Entry_()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Draw_Help_Entry_()\n", __FILE__, __LINE__);
#endif
}
