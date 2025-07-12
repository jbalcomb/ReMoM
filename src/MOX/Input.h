/*
    WIZARDS.EXE
        seg036
*/
#ifndef INPUT_H
#define INPUT_H

#include "MOX_KEYS.h"
#include "MOX_TYPE.h"



#define FIELD_COUNT_MAX 128



// records from HLPENTRY.LBX
// max 50 * (5 * 2) loaded into _help_entries
// sizeof 0Ah 10d
/*
TODO rename to s_HELP_ENTRY  (not sure how I got to "help field", the "help field" is just a field which happens to be be for drawing a help entry)
MoO1, MoM, and MoO2 all look to not use a 'struct', but just access it as an array, where they just happen to know the size and offsets.

help_idx ~== help_struct_pointer[j + 0]
x1 ~== help_struct_pointer[j + 1]
y1 ~== help_struct_pointer[j + 3]
x2 ~== help_struct_pointer[j + 2]
y2 ~== help_struct_pointer[j + 4]
*/
struct s_HELP_FIELD
{
    int16_t help_idx;  // enum HELP_TXT
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
};

// sizeof:  500d  (50 * sizeof(struct s_HELP_FIELD))
struct s_HELP_LIST
{
    struct s_HELP_FIELD help_00;
    struct s_HELP_FIELD help_01;
    struct s_HELP_FIELD help_02;
    struct s_HELP_FIELD help_03;
    struct s_HELP_FIELD help_04;
    struct s_HELP_FIELD help_05;
    struct s_HELP_FIELD help_06;
    struct s_HELP_FIELD help_07;
    struct s_HELP_FIELD help_08;
    struct s_HELP_FIELD help_09;
    struct s_HELP_FIELD help_10;
    struct s_HELP_FIELD help_11;
    struct s_HELP_FIELD help_12;
    struct s_HELP_FIELD help_13;
    struct s_HELP_FIELD help_14;
    struct s_HELP_FIELD help_15;
    struct s_HELP_FIELD help_16;
    struct s_HELP_FIELD help_17;
    struct s_HELP_FIELD help_18;
    struct s_HELP_FIELD help_19;
    struct s_HELP_FIELD help_20;
    struct s_HELP_FIELD help_21;
    struct s_HELP_FIELD help_22;
    struct s_HELP_FIELD help_23;
    struct s_HELP_FIELD help_24;
    struct s_HELP_FIELD help_25;
    struct s_HELP_FIELD help_26;
    struct s_HELP_FIELD help_27;
    struct s_HELP_FIELD help_28;
    struct s_HELP_FIELD help_29;
    struct s_HELP_FIELD help_30;
    struct s_HELP_FIELD help_31;
    struct s_HELP_FIELD help_32;
    struct s_HELP_FIELD help_33;
    struct s_HELP_FIELD help_34;
    struct s_HELP_FIELD help_35;
    struct s_HELP_FIELD help_36;
    struct s_HELP_FIELD help_37;
    struct s_HELP_FIELD help_38;
    struct s_HELP_FIELD help_39;
    struct s_HELP_FIELD help_40;
    struct s_HELP_FIELD help_41;
    struct s_HELP_FIELD help_42;
    struct s_HELP_FIELD help_43;
    struct s_HELP_FIELD help_44;
    struct s_HELP_FIELD help_45;
    struct s_HELP_FIELD help_46;
    struct s_HELP_FIELD help_47;
    struct s_HELP_FIELD help_48;
    struct s_HELP_FIELD help_49;
};

// sizeof 0418h 1048d
struct s_HELP_DATA
{
    char title[30];
    char lbx_name[14];
    int16_t lbx_entry;
    int16_t Include_Entry;
    char help_text[1000];
};





// WZD dseg:8296                                                 ¿ BEGIN: Help - Initialized Data ?
// WZD dseg:8296                                                 BEGIN: seg036 (fields) - Initialized Data

// WZD dseg:8296
// AKA have_help
extern int16_t help_list_active;

// WZD dseg:8298
// AKA _help_count
extern int16_t help_list_count;

// WZD dseg:8298                                                 ¿ END: Help - Initialized Data ?
// WZD dseg:8298                                                 END: seg036 (fields) - Initialized Data



// WZD dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard
// WZD dseg:82A6



// WZD dseg:E872                                                 ¿ BEGIN: fields, intput, ... ?
// WZD dseg:E872                                                 ¿ BEGIN: Help - Uninitialized Data ?

// WZD dseg:E872
// AKA _help_list
// // // extern struct s_HELP_FIELD * help_struct_pointer;
// // extern struct s_HELP_FIELD ** help_struct_pointer;
// extern struct s_HELP_FIELD * help_struct_pointer;
extern struct s_HELP_FIELD * help_struct;
extern int16_t * help_struct_pointer;


// WZD dseg:E872                                                 ¿ END: Help - Uninitialized Data ?
// ...
// ...
// ...
// WZD dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard





#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



// Interpret_Mouse_Input                          seg036     0000000C 00000F5F R F . . B T .
// UU_GUI_SetTimeOutP                             seg036     00000F90 0000001C R F . . B T .
// UU_GUI_SetTimeOutN                             seg036     00000FAC 0000001C R F . . B T .
// UU_GUI_EditStart                               seg036     00000FC8 00000010 R F . . B T .
// UU_GUI_SetClickBuffer                          seg036     00000FD8 00000031 R F . . B T .
// UU_GUI_LoadClickFile                           seg036     00001009 00000054 R F . . B T .
// UU_GUI_SaveClickFile                           seg036     0000105D 00000047 R F . . B T .
// 
// G_GUI_ClearInput                               seg036     00002EE2 0000003C R F . . B T .
// G_GUI_PressAnyKey                              seg036     00002F1E 0000018B R F . . B T .
// GUI_ProcessDirKey                              seg036     000030A9 000002C8 R F . . B T .
// Input_Box_Popup                             seg036     0000339F 0000042B R F . . B T .
// Setup_Input_Box_Popup                             seg036     000037E1 000003C9 R F . . B T .
// Draw_Input_Box_Popup                              seg036     00003BC1 000003E7 R F . . B T .
// GUI_CreateEditBox                              seg036     00003FA8 000001CF R F . . B T .
// 
// GUI_CreateImageLabel                           seg036     00004667 00000147 R F . . B T .
// UU_GUI_CreateWndImgLbl                         seg036     000047AE 000001D0 R F . . B T .
// Set_Button_Down_Offsets                        seg036     0000497E 00000011 R F . . B T .



/*
    WIZARDS.EXE seg036
    MAGIC.EXE seg034
*/

// WZD s36p01
int16_t Interpret_Mouse_Input(void);


// WZD s36p08
void Set_Global_ESC(void);


// WZD s36p12
void Set_Help_List(void * help_pointer, int16_t count);

// WZD s36p13
void Deactivate_Help_List(void);

// WZD s36p14
int16_t Check_Help_List(void);

// UU_GUI_GetControlWidth                         seg036     000011C1 0000004D R F . . B T .
// UU_GUI_GetControlHeight                        seg036     0000120E 0000004E R F . . B T .
// UU_GUI_GetLastClickX                           seg036     0000125C 0000000A R F . . B T .
// UU_GUI_GetLastClickY                           seg036     00001266 0000000A R F . . B T .
// UU_GUI_GetLastClickBtns                        seg036     00001270 0000000A R F . . B T .
// Get_Mouse_Field                                seg036     0000127A 0000000A R F . . B T .
// RP_GUI_KeyInputOnly                            seg036     00001284 00000675 R F . . B T .
// Interpret_Keyboard_Input                       seg036     000018F9 000004AB R F . . B T .
// GUI_GetNextControl                             seg036     00001DC5 00000DAC R F . . B T .

// WZD s36p24
void UU_GUI_MouseEMUMoveTo(int16_t field_idx);


// WZD s36p25
int16_t Scan_Field(void);

// WZD s36p26
int16_t Scan_Input(void);

// WZD s36p27
uint8_t Read_Character(void);

// WZD s36p28
void Reset_Wait_For_Input(void);

// WZD s36p29
int16_t Wait_For_Input(void);

// WZD s36p30
int16_t Process_Direction_Key__STUB(int16_t dir_key);

// WZD s36p31
void Input_Box_Popup(int16_t field_num);

// WZD s36p32
int16_t Setup_Input_Box_Popup(int16_t x_start, int16_t y_start, int16_t width, char * string, int16_t max_characters, int16_t fill_color, int16_t justification, int16_t cursor_type, uint8_t colors[], int16_t help);

// WZD s36p33
void Draw_Input_Box_Popup(int16_t Editbox_Index, char * Local_String);

// WZD s36p34
// GUI_CreateEditBox()


// WZD s36p65  AKA GUI_Init()  AKA IN_Init()
void Init_Mouse_Keyboard(int16_t input_type);



// PLATFORM
// WZD s36p66
/*
    returns field_idx/num
*/
int16_t Get_Input(void);



// WZD s36p67
void Set_Input_Delay(int16_t delay);

// WZD s36p68
int16_t Get_Input_Delay(void);


// WZD s36p85
// drake178: GUI_SimplePageFlip()
// AKA SCRN_SimplePageFlip()
// MoO2  Module: video  Toggle_Pages()  &&  Module: MOX2  _TOGGLE_PAGES_
void Toggle_Pages(void);



#ifdef __cplusplus
}
#endif

#endif  /* INPUT_H */
