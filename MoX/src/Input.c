/*
    WIZARDS.EXE
        seg036
    MAGIC.EXE
        seg034
*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_GARB.h"  /* Stack_Garbage_Range() */
#include "../../STU/src/STU_LOG.h"

#include "../../ext/stu_compat.h"

#include "Allocate.h"
#include "Fields.h"
#include "Fonts.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "MOX_DEF.h"
#include "MOX_KEYS.h"
#include "MOX_TYPE.h"
#include "TEXTBOX.h"
#include "Timer.h"
#include "capture.h"

#include <string.h>

#include "../../platform/include/Platform.h"

#include "Input.h"



#ifdef STU_DEBUG

int16_t DBG_In_MouseButtonLeft = ST_FALSE;
int16_t DBG_In_MouseButtonRight = ST_FALSE;
int16_t DBG_movement_map_grid_field_idx = 0;
int16_t DBG_reduced_map_grid_field_idx = 0;

// char field_names[127][30];
char * field_names[FIELD_COUNT_MAX] = {
    /* 000 */   "NOT IN USE",
    /* 001 */   "NOT IN USE",
    /* 002 */   "NOT IN USE",
    /* 003 */   "NOT IN USE",
    /* 004 */   "NOT IN USE",
    /* 005 */   "NOT IN USE",
    /* 006 */   "NOT IN USE",
    /* 007 */   "NOT IN USE",
    /* 008 */   "NOT IN USE",
    /* 009 */   "NOT IN USE",
    /* 010 */   "NOT IN USE",
    /* 011 */   "NOT IN USE",
    /* 012 */   "NOT IN USE",
    /* 013 */   "NOT IN USE",
    /* 014 */   "NOT IN USE",
    /* 015 */   "NOT IN USE",
    /* 016 */   "NOT IN USE",
    /* 017 */   "NOT IN USE",
    /* 018 */   "NOT IN USE",
    /* 019 */   "NOT IN USE",
    /* 020 */   "NOT IN USE",
    /* 021 */   "NOT IN USE",
    /* 022 */   "NOT IN USE",
    /* 023 */   "NOT IN USE",
    /* 024 */   "NOT IN USE",
    /* 025 */   "NOT IN USE",
    /* 026 */   "NOT IN USE",
    /* 027 */   "NOT IN USE",
    /* 028 */   "NOT IN USE",
    /* 029 */   "NOT IN USE",
    /* 030 */   "NOT IN USE",
    /* 031 */   "NOT IN USE",
    /* 032 */   "NOT IN USE",
    /* 033 */   "NOT IN USE",
    /* 034 */   "NOT IN USE",
    /* 035 */   "NOT IN USE",
    /* 036 */   "NOT IN USE",
    /* 037 */   "NOT IN USE",
    /* 038 */   "NOT IN USE",
    /* 039 */   "NOT IN USE",
    /* 040 */   "NOT IN USE",
    /* 041 */   "NOT IN USE",
    /* 042 */   "NOT IN USE",
    /* 043 */   "NOT IN USE",
    /* 044 */   "NOT IN USE",
    /* 045 */   "NOT IN USE",
    /* 046 */   "NOT IN USE",
    /* 047 */   "NOT IN USE",
    /* 048 */   "NOT IN USE",
    /* 049 */   "NOT IN USE",
    /* 050 */   "NOT IN USE",
    /* 051 */   "NOT IN USE",
    /* 052 */   "NOT IN USE",
    /* 053 */   "NOT IN USE",
    /* 054 */   "NOT IN USE",
    /* 055 */   "NOT IN USE",
    /* 056 */   "NOT IN USE",
    /* 057 */   "NOT IN USE",
    /* 058 */   "NOT IN USE",
    /* 059 */   "NOT IN USE",
    /* 060 */   "NOT IN USE",
    /* 061 */   "NOT IN USE",
    /* 062 */   "NOT IN USE",
    /* 063 */   "NOT IN USE",
    /* 064 */   "NOT IN USE",
    /* 065 */   "NOT IN USE",
    /* 066 */   "NOT IN USE",
    /* 067 */   "NOT IN USE",
    /* 068 */   "NOT IN USE",
    /* 069 */   "NOT IN USE",
    /* 070 */   "NOT IN USE",
    /* 071 */   "NOT IN USE",
    /* 072 */   "NOT IN USE",
    /* 073 */   "NOT IN USE",
    /* 074 */   "NOT IN USE",
    /* 075 */   "NOT IN USE",
    /* 076 */   "NOT IN USE",
    /* 077 */   "NOT IN USE",
    /* 078 */   "NOT IN USE",
    /* 079 */   "NOT IN USE",
    /* 080 */   "NOT IN USE",
    /* 081 */   "NOT IN USE",
    /* 082 */   "NOT IN USE",
    /* 083 */   "NOT IN USE",
    /* 084 */   "NOT IN USE",
    /* 085 */   "NOT IN USE",
    /* 086 */   "NOT IN USE",
    /* 087 */   "NOT IN USE",
    /* 088 */   "NOT IN USE",
    /* 089 */   "NOT IN USE",
    /* 090 */   "NOT IN USE",
    /* 091 */   "NOT IN USE",
    /* 092 */   "NOT IN USE",
    /* 093 */   "NOT IN USE",
    /* 094 */   "NOT IN USE",
    /* 095 */   "NOT IN USE",
    /* 096 */   "NOT IN USE",
    /* 097 */   "NOT IN USE",
    /* 098 */   "NOT IN USE",
    /* 099 */   "NOT IN USE",
    /* 100 */   "NOT IN USE",
    /* 101 */   "NOT IN USE",
    /* 102 */   "NOT IN USE",
    /* 103 */   "NOT IN USE",
    /* 104 */   "NOT IN USE",
    /* 105 */   "NOT IN USE",
    /* 106 */   "NOT IN USE",
    /* 107 */   "NOT IN USE",
    /* 108 */   "NOT IN USE",
    /* 109 */   "NOT IN USE",
    /* 110 */   "NOT IN USE",
    /* 111 */   "NOT IN USE",
    /* 112 */   "NOT IN USE",
    /* 113 */   "NOT IN USE",
    /* 114 */   "NOT IN USE",
    /* 115 */   "NOT IN USE",
    /* 116 */   "NOT IN USE",
    /* 117 */   "NOT IN USE",
    /* 118 */   "NOT IN USE",
    /* 119 */   "NOT IN USE",
    /* 120 */   "NOT IN USE",
    /* 121 */   "NOT IN USE",
    /* 122 */   "NOT IN USE",
    /* 123 */   "NOT IN USE",
    /* 124 */   "NOT IN USE",
    /* 125 */   "NOT IN USE",
    /* 126 */   "NOT IN USE",
    /* 127 */   "NOT IN USE"
};

#endif





// WZD dseg:8296                                                 ¿ BEGIN: Help - Initialized Data ?
// WZD dseg:8296                                                 BEGIN: seg036 (fields) - Initialized Data

// WZD dseg:8296
// AKA have_help
int16_t help_list_active = ST_FALSE;

// WZD dseg:8298
// AKA _help_count
int16_t help_list_count = 0;

// WZD dseg:8298                                                 ¿ END: Help - Initialized Data ?
// WZD dseg:8298                                                 END: seg036 (fields) - Initialized Data



// WZD dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard
// WZD dseg:82A6



// WZD dseg:E872                                                 ¿ BEGIN: fields, intput, ... ?
// WZD dseg:E872                                                 ¿ BEGIN: Help - Uninitialized Data ?

// WZD dseg:E872
// AKA _help_list
/*
    ¿ pointer to an array of pointers of data type 'struct s_HELP_FIELD' ?
*/
// // // struct s_HELP_FIELD * help_struct_pointer;
// // struct s_HELP_FIELD ** help_struct_pointer;
// struct s_HELP_FIELD * help_struct_pointer;
struct s_HELP_FIELD * help_struct;
int16_t * help_struct_pointer;
// help_struct_pointer[j + 0]
// help_struct_pointer[j + 1]
// help_struct_pointer[j + 3]
// help_struct_pointer[j + 2]
// help_struct_pointer[j + 4]

// WZD dseg:E872                                                 ¿ END: Help - Uninitialized Data ?
// ...
// ...
// ...
// WZD dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard





/*
    WIZARDS.EXE
    seg035
    seg036

    ? seg0035 vs. seg036 ?
    Hi/Lo?
    C/Asm?
    
    Input vs. Keyboard vs. Mouse vs. Fields vs. ?
    ?!? redraw functions and Toggle_Pages() ?!?

*/



/*
    WIZARDS.EXE
        eg036
    MAGIC.EXE
        seg034
*/

/* CLAUDE */  /* HACK */
/*
    Stand-in for p_fields[active_input_field_number].max_characters when active_input_field_number is ST_UNDEFINED.
    OGBUG  a commit clears active_input_field_number but not input_field_active (see the commit branches of Interpret_Mouse_Input()), so until the next commit the edit loops read
    p_fields[-1].max_characters: les bx, [p_fields] / add bx, -26h wraps the 16-bit offset to 0FFDAh, so the word comes from p_fields segment : 0FFFAh, 64 KB past the 5712-byte
    Allocate_Space(357) fields block, in whatever heap block follows. That value is unknowable, and it is a loop limit, so ReMoM uses garbage drawn once (the OG word does not change
    between reads) and bounded to 0 .. 30 = sizeof(continuous_string) (Fields.c), so the typing branch's continuous_string[pos] / [pos + 1] writes stay inside the array.
*/
static int16_t Active_Input_Field_Max_Characters(void)
{
    static int16_t p_fields_minus_1_max_characters = ST_UNDEFINED;
    if(active_input_field_number != ST_UNDEFINED)
    {
        return p_fields[active_input_field_number].max_characters;
    }
    if(p_fields_minus_1_max_characters == ST_UNDEFINED)
    {
        p_fields_minus_1_max_characters = Stack_Garbage_Range(0, 30);
    }
    return p_fields_minus_1_max_characters;
}

// WZD s36p01
// MGC s34p01
/*
MoO2
    function (0 bytes) Interpret_Mouse_Input
    Address: 01:0011CEF5
        Num params: 0
        Return type: signed integer (2 bytes) 
        void (1 bytes) 
        Locals:
            signed integer (2 bytes) i
            signed integer (2 bytes) j
            signed integer (2 bytes) m
            signed integer (2 bytes) mouse_x
            signed integer (2 bytes) mouse_y
            signed integer (2 bytes) button_number
            signed integer (2 bytes) valid
            signed integer (2 bytes) xmin
            signed integer (2 bytes) ymin
            signed integer (2 bytes) xmax
            signed integer (2 bytes) ymax
            signed integer (2 bytes) field_type
            signed integer (2 bytes) field_num
            signed integer (2 bytes) character
            signed integer (2 bytes) return_value
            char (1 bytes) clear_multi_hotkey_fields
*/
/*

ProgramPath (Button-Up) - @@EndOfTheClickRoad
    IDA Color #14 ~ DarkSkyBlue
    5 incoming branches


ProgramPath - Nay KD, Yay MD, Nay Help, Nay Cancel, Yay Button, Yay Field
Color #45  64,128,128  ~ Light Forest Green
ProgramPath - Nay KD, Yay MD, Nay Help, Nay Cancel, Yay Button, Yay Field, YNM ft_ContinuousStringInput
Color #15  128,128,192  ~ Light Grey-Purple


ProgramPath - Mouse_Buffer()
Color #34 128,64,0 ~PooEmoji Brown
20240725: started changing to dark purple #40  SEEALSO: MoX-Input.md

20250406:
MoX-Input-InterpretMouseInput-Buffer.md
IDA light purple (color #22)

*/
int16_t Interpret_Mouse_Input(void)
{
/*
MoO2
    var_68= dword ptr -68h
    field_type= dword ptr -64h
    var_60= dword ptr -60h
    var_5C= dword ptr -5Ch
    var_58= dword ptr -58h
    var_54= dword ptr -54h
    var_50= dword ptr -50h
    var_4C= dword ptr -4Ch
    var_48__field_type= dword ptr -48h
    itr_fields_count= dword ptr -44h
    itr_continuous_string= dword ptr -40h
    IDK_itr_10= dword ptr -3Ch
    mouse_x= dword ptr -38h
    mouse_y= dword ptr -34h
    mouse_button= dword ptr -30h
    var_28= dword ptr -28h
    var_24= dword ptr -24h
    var_20= dword ptr -20h
    var_1C= dword ptr -1Ch
    var_18= dword ptr -18h
    field_num__scanned_field= dword ptr -14h
    character= dword ptr -10h
    maybe__field_num__scanned_field= dword ptr -0Ch
    return_value= dword ptr -8
    clear_multi_hotkey_fields= byte ptr -4
*/
/*

character= byte ptr -15h
field_num= word ptr -14h
Unused_Local= word ptr -12h
ymax= word ptr -10h
xmax= word ptr -0Eh
ymin= word ptr -0Ch
xmin= word ptr -0Ah
Accepted_Char= word ptr -8
mouse_button= word ptr -6
l_my= word ptr -4
l_mx= word ptr -2

_SI_alt_field_num = si
_DI_itr_continuous_string = di
*/
    int16_t field_num = 0;
    // int16_t character = 0;
    /* CLAUDE */  /* REBUG */  /* asm: character= byte ptr -15h, signed (cmp [bp+character], 64 / jle) */
    int8_t character = 0;
    int16_t Unused_Local = 0;
    int16_t ymax = 0;
    int16_t xmax = 0;
    int16_t ymin = 0;
    int16_t xmin = 0;
    int16_t l_mx = 0;
    int16_t l_my = 0;
    // int16_t pointer_offset = 0;
    // int16_t down_mouse_button = 0;
    // int16_t mouse_field = 0;
    /* CLAUDE */  /* REBUG */  /* asm 25, 34: mov [down_mouse_button], e_ST_UNDEFINED / mov [pointer_offset], ax - the globals (Fields.c), which these locals shadowed; the asm has no mouse_field */
    int16_t mouse_button = 0;
    uint16_t itr_continuous_string = 0;
    int16_t Accepted_Char = 0;
    /*
        _SI_YNM_itr_fields_count__scanned_field
            return by @@UnsetDownMouseButton_Return_ITR
            4 paths
            ¿ active/current field ?
            1) KD; character == KP_Enter && input_field_active != ST_FALSE; sets to active_input_field_number
            2) KD; character == +/-; sets to Scan_Field()
            3)
            4) 
    */
    int16_t alt_field_num = 0;  // _SI_
    int16_t scanned_field = 0;  // _SI_
    int16_t return_value = 0;  // _AX_  MoO2: return _EAX_ = return_value = maybe__field_num__scanned_field = itr_fields_count

/*
MoO1
    mov     [bp+oi], 0
    mov     uiobj_focus_oi, 0FFFFh
    mov     [bp+mbuttons], 0
    mov     uiobj_clicked_oi, 0
*/
/*
MoO2
    mov     [ebp+field_num__scanned_field], 0
    mov     [ebp+maybe__field_num__scanned_field], 0
    mov     [ebp+clear_multi_hotkey_fields], 0
    mov     [ebp+field_num__scanned_field], 0
    mov     down_mouse_button, e_ST_UNDEFINED_DW
    mov     [ebp+mouse_button], 0
    mov     auto_input_variable, 0
*/
    character = 0;
    field_num = 0;
    down_mouse_button = ST_UNDEFINED;
    mouse_button = 0;
    // mouse_field = 0;
    auto_input_variable = 0;  /* CLAUDE */  /* REBUG */  /* asm 27: mov [auto_input_variable], 0 */

    l_mx = Pointer_X();
    l_my = Pointer_Y();
    pointer_offset = Get_Pointer_Offset();



    // if(Keyboard_Status() == ST_TRUE)
    if(Keyboard_Status() != ST_FALSE)  /* CLAUDE */  /* REBUG */  /* asm 35-37: call Keyboard_Status / or ax, ax / jnz @@YayKeystroke */
    {
        character = Interpret_Keyboard_Input(&field_num);

        if(character == 0)
        {
            goto Return_Type_Z01;
        }


        // if(character == ST_KEY_F11)
        /* CLAUDE */  /* REBUG */  /* asm: cmp [bp+character], KP_F11 - a byte compare */
        if(character == (int8_t)ST_KEY_F11)
        {
            F11_Key();
        }
        
        // if(character == ST_KEY_F12)
        /* CLAUDE */  /* REBUG */  /* asm: cmp [bp+character], KP_F12 - a byte compare */
        if(character == (int8_t)ST_KEY_F12)
        {
            Save_Mouse_State();
            Restore_Mouse_On_Page();
            Screen_Flic_Capture();
            Save_Mouse_On_Page(Pointer_X(), Pointer_Y());
            Draw_Mouse_On_Page(Pointer_X(), Pointer_Y());
            Set_Pointer_Position(Pointer_X(), Pointer_Y());
            Restore_Mouse_State();
        }

        if((character == ST_KEY_ESCAPE) && (mouse_cancel_disabled == ST_FALSE))
        {
            goto Return_Type_ESC;
        }

        if((character == '`') && (Check_Release_Version() == ST_FALSE))
        {
            if(field_box_mode == ST_FALSE)
            {
                field_box_mode = ST_TRUE;
            }
            else
            {
                field_box_mode = ST_FALSE;
            }
        }

        if((character == '~') && (Check_Release_Version() == ST_FALSE))
        {
            if(help_box_mode == ST_FALSE)
            {
                help_box_mode = ST_TRUE;
            }
            else
            {
                help_box_mode = ST_FALSE;
            }
        }


        // ¿ BUGBUG: assumes field_num != 0 ?
        if(p_fields[field_num].type == ft_MultiHotKey)
        {
            goto Return_Type_F01;
        }


        /*
            BEGIN:  Keyboard Hot-Key
        */
        {
            // if((field_num != 0) && (p_fields[field_num].hotkey == character))
            /* CLAUDE */  /* REBUG */  /* asm: mov al, [byte ptr es:bx+s_Field.hotkey] / cmp al, [bp+character] - a byte compare */
            if((field_num != 0) && ((int8_t)p_fields[field_num].hotkey == character))
            {

                if(p_fields[field_num].type == ft_Scroll)
                {
                    goto Return_Type_Z01;
                }

                if(field_num != 0)
                {
                    l_mx = (p_fields[field_num].x1 + ((p_fields[field_num].x2 - p_fields[field_num].x1) / 2));
                    l_my = (p_fields[field_num].y1 + ((p_fields[field_num].y2 - p_fields[field_num].y1) / 2));
                    Push_Field_Down(field_num, l_mx, l_my);
                    Mouse_Button_Handler();

                    switch(p_fields[field_num].type)
                    {
                        case ft_RadioButton:
                        {
                            // toggle unset/set state
                            if(*p_fields[field_num].state == 0)
                            {
                                *p_fields[field_num].state = 1;
                            }
                            else
                            {
                                *p_fields[field_num].state = 0;
                            }
                        } break;
                        case ft_LockedButton:
                        {
                            // if not locked, then lock  (¿ no unlock ?)
                            if(*p_fields[field_num].state == 0)
                            {
                                *p_fields[field_num].state = 1;
                            }
                            else
                            {
                                Quick_Call_Auto_Function();
                                goto Return_Type_Z10;
                            }
                        } break;
                        case ft_ClickLink:
                        {
                            // @@Jmp_ClearDown_ReturnFieldNum:
                            // jmp     @@ClearDown_ReturnFieldNum
                            goto Return_Type_P10;
                        } break;
                    }

                    Quick_Call_Auto_Function();
                    goto Return_Type_F10;

                }
                /* CLAUDE */  /* REBUG */  /* asm 154-156: cmp [bp+field_num], 0 / jnz loc_267E2 / jmp @@QuickCall_ClearDown_Return_FieldNum - the else-branch (dead: field_num != 0 was tested above) */
                else
                {
                    Quick_Call_Auto_Function();
                    goto Return_Type_F10;
                }
            }
        }
        /*
            END:  Keyboard Hot-Key
        */


        /*
            BEGIN:  character == ST_KEY_ENTER
                IDA: 'poop brown' #34 - KD; ST_KEY_ENTER;
        */
        {
            if(character == ST_KEY_ENTER)
            {
                if(input_field_active != ST_FALSE)
                {
                    alt_field_num = active_input_field_number;
                    // MoO2:  Copy_Continuous_String()
                    /* HACK  with active_input_field_number -1 (left by a commit, see the OGBUG in the commit branches) the OG copies the text to the near pointer in the record before p_fields; ReMoM skips that wild write */
                    if(alt_field_num != ST_UNDEFINED)
                    {
                        stu_strcpy((char *)p_fields[alt_field_num].string, continuous_string);
                    }
                    if(mouse_auto_exit == ST_FALSE)
                    {
                        Quick_Call_Auto_Function();
                    }
                    goto Return_Type_5;
                }

                // IDA:  @@KD_Enter_NayTextInput:
                alt_field_num = Scan_Field();

                if(alt_field_num == 0)
                {
                    goto Return_Type_Z00;
                }

                if(alt_field_num > 0)
                {
                    if(p_fields[alt_field_num].type == ft_ContinuousStringInput)
                    {
                        if(input_field_active == ST_FALSE)
                        {
                            stu_strcpy(continuous_string, p_fields[alt_field_num].string);
                            GUI_EditAnimStage = 0;
                            GUI_EditCursorOn = ST_FALSE;
                            input_field_active = ST_TRUE;
                            active_input_field_number = alt_field_num;
                            /* CLAUDE */ Hw_Textinput_Start();
                            // jmp     @@IDK_KD_Enter_PostOp
                        }
                        else  /* input_field_active == ST_TRUE */
                        {
                            // ¿ UNREACHABLE ?
                            // if(active_input_field_number == alt_field_num)
                            // {
                            //     itr_continuous_string = 0;
                            //     while((continuous_string[itr_continuous_string] != '\0') && (p_fields[alt_field_num].max_characters > itr_continuous_string)) { itr_continuous_string++; }
                            //     if(continuous_string[(itr_continuous_string - 1)] == '_')
                            //     {
                            //         itr_continuous_string--;
                            //     }
                            //     continuous_string[itr_continuous_string] = 0;
                            //     GUI_EditAnimStage = 0;
                            //     active_input_field_number = ST_UNDEFINED;
                            //     stu_strcpy(p_fields[alt_field_num].string, continuous_string);
                            //     // @@IDK_KD_Enter_PostOp
                            // }
                            // else
                            // {
                            //     stu_strcpy(continuous_string, p_fields[alt_field_num].string);
                            //     GUI_EditAnimStage = 0;  // cursor_count
                            //     GUI_EditCursorOn = 0;   // cursor_on
                            //     active_input_field_number = alt_field_num;
                            //     // @@IDK_KD_Enter_PostOp:
                            // }
                            /* CLAUDE */  /* REBUG */  /* asm 368-369: cmp [active_input_field_number], si / jz loc_26A13 - the switch-field copy is the then-branch, the commit (loc_26A13) is the else */
                            if(active_input_field_number != alt_field_num)
                            {
                                stu_strcpy(continuous_string, p_fields[alt_field_num].string);
                                GUI_EditAnimStage = 0;  // cursor_count
                                GUI_EditCursorOn = 0;   // cursor_on
                                active_input_field_number = alt_field_num;
                                // @@IDK_KD_Enter_PostOp:
                            }
                            else
                            {
                                itr_continuous_string = 0;
                                while((continuous_string[itr_continuous_string] != '\0') && (p_fields[alt_field_num].max_characters > itr_continuous_string)) { itr_continuous_string++; }
                                /* OGBUG  with an empty string the test reads continuous_string[-1], the high byte of last_button_x (0 or 1, never '_') */
                                /* HACK */  if((itr_continuous_string > 0) && (continuous_string[(itr_continuous_string - 1)] == '_'))
                                {
                                    itr_continuous_string--;
                                }
                                continuous_string[itr_continuous_string] = 0;
                                GUI_EditAnimStage = 0;
                                active_input_field_number = ST_UNDEFINED;
                                stu_strcpy(p_fields[alt_field_num].string, continuous_string);
                                // @@IDK_KD_Enter_PostOp
                            }
                        }
                    }
                    else
                    {
                        if(input_field_active != 0)
                        {
                            // UNREACHABLE  if(p_fields[alt_field_num].type == ft_ContinuousStringInput)
                            // UNREACHABLE  {
                            // UNREACHABLE      itr_continuous_string = 0;
                            // UNREACHABLE      while((continuous_string[itr_continuous_string] != '\0') && (p_fields[alt_field_num].max_characters > itr_continuous_string)) { itr_continuous_string++; }
                            // UNREACHABLE      if(continuous_string[(itr_continuous_string - 1)] == '_')
                            // UNREACHABLE      {
                            // UNREACHABLE          itr_continuous_string--;
                            // UNREACHABLE      }
                            // UNREACHABLE      continuous_string[itr_continuous_string] = 0;
                            // UNREACHABLE      stu_strcpy(p_fields[alt_field_num].string, continuous_string);
                            // UNREACHABLE      GUI_EditAnimStage = 0;
                            // UNREACHABLE      input_field_active = ST_FALSE;
                            // UNREACHABLE      active_input_field_number = ST_UNDEFINED;
                            // UNREACHABLE  }
                            /* CLAUDE */  /* REBUG */  /* asm loc_26A69 (422-464): the block is in the asm (dead here: input_field_active is FALSE on this path); type != CSI (jz skips), max_characters and the copy use active_input_field_number */
                            if(p_fields[alt_field_num].type != ft_ContinuousStringInput)
                            {
                                itr_continuous_string = 0;
                                while((continuous_string[itr_continuous_string] != '\0') && (p_fields[active_input_field_number].max_characters > itr_continuous_string)) { itr_continuous_string++; }
                                /* HACK */  if((itr_continuous_string > 0) && (continuous_string[(itr_continuous_string - 1)] == '_'))
                                {
                                    itr_continuous_string--;
                                }
                                continuous_string[itr_continuous_string] = 0;
                                stu_strcpy(p_fields[active_input_field_number].string, continuous_string);
                                GUI_EditAnimStage = 0;
                                input_field_active = ST_FALSE;
                                active_input_field_number = ST_UNDEFINED;
                            }
                        }
                    }
                }

                // @@IDK_KD_Enter__New_Block:
                // IDA foggy hunter green #36
                if(alt_field_num == 0)
                {
                    if(list_field_on != 0)
                    {
                        for(alt_field_num = 1; alt_field_num < fields_count; alt_field_num++)
                        {
                            // if State@ == Trig_State
                            if(
                                (p_fields[alt_field_num].type == ft_StringList)
                                &&
                                (*p_fields[alt_field_num].variable1 == p_fields[alt_field_num].set_value)
                                &&
                                (p_fields[alt_field_num].active_flag != ST_FALSE)
                            )
                            {
                                // got Selectable StringList
                                // @@Jmp_ClearDown_Return_AltFieldNum:
                                // jmp     short @@ClearDown_Return_AltFieldNum
                                goto Return_Type_5;
                            }
                        }
                    }
                }
                else
                {
                    if((p_fields[alt_field_num].type != ft_Scroll) && (p_fields[alt_field_num].type != ft_ContinuousStringInput))
                    {
                        Push_Field_Down(alt_field_num, l_mx, l_my);
                    }

                    Mouse_Button_Handler();

                    switch(p_fields[alt_field_num].type)
                    {
                        case ft_RadioButton:
                        {
                            if(*p_fields[alt_field_num].state == 0)
                            {
                                *p_fields[alt_field_num].state = 1;
                            }
                            else
                            {
                                *p_fields[alt_field_num].state = 0;
                            }
                        } break;
                        case ft_LockedButton:
                        {
                            if(*p_fields[alt_field_num].state == 0)
                            {
                                *p_fields[alt_field_num].state = 1;
                            }
                            /* CLAUDE */  /* REBUG */  /* asm 514, 523-524: jnz @@Jmp_UnsetDownMouseButton / jmp @@ClearDown_JmpReturnZero (down_mouse_button = -1, return 0) - the else-branch was missing */
                            else
                            {
                                goto Return_Type_Z10;
                            }
                        } break;
                        case ft_ClickLink:
                        {
                            goto Return_Type_7;
                        } break;
                    }

                    if(mouse_auto_exit == ST_FALSE)
                    {
                        Quick_Call_Auto_Function();
                    }

                    // return the field_num for the scanned_field, because ENTER as *click*
                    goto Return_Type_5;

                }

            }
        }
        /*
            END:  character == KP_Enter  (assumes ft_ContinuousStringInput)
        */


        /*
            BEGIN:  +/- Scroll Bar Field
        */
        {
            // if((character == '+' || character == '-') && (alt_field_num = Scan_Field() != 0) && (p_fields[alt_field_num].type == ft_Scroll))
            /* CLAUDE */  /* REBUG */  /* asm 620-623: call Scan_Field / mov si, ax / or si, si / jz - the field number is stored, not (Scan_Field() != 0) */
            if((character == '+' || character == '-') && ((alt_field_num = Scan_Field()) != 0) && (p_fields[alt_field_num].type == ft_Scroll))
            {
                if(character == '+')
                {
                    Increment_Scroll_Bar(alt_field_num);
                }
                if(character == '-')
                {
                    Decrement_Scroll_Bar(alt_field_num);
                }
                goto Return_Type_4;
            }
        }
        /*
            END:  +/- Scroll Bar Field
        */

        /*
            BEGIN:  Input Field
        */
        if(input_field_active > 0)
        {
            if(character == ST_KEY_BACKSPACE)
            {
                alt_field_num = 0;
                // while( (continuous_string[alt_field_num] != '\0') && (continuous_string[alt_field_num] != '_') && ((p_fields[active_input_field_number].max_characters - 1) > alt_field_num))
                /* HACK */  while( (continuous_string[alt_field_num] != '\0') && (continuous_string[alt_field_num] != '_') && ((Active_Input_Field_Max_Characters() - 1) > alt_field_num))
                {
                    alt_field_num++;
                }

                if(alt_field_num > 0)
                {
                    continuous_string[(alt_field_num - 1)] = '\0';
                    GUI_EditAnimStage = 0;
                }
            }
            else
            {
                Accepted_Char = ST_FALSE;
                if(
                    (character > 64 && character < 93)
                    ||
                    (character > 45 && character < 59)
                    ||
                    (character == ' ')
                    ||
                    (character == '-')
                )
                {
                    Accepted_Char = ST_TRUE;
                }
                else if(character > 96 && character < 123)
                {
                    Accepted_Char = ST_TRUE;
                }

                if(Accepted_Char == ST_TRUE)
                {
                    alt_field_num = 0;
                    // while( (continuous_string[alt_field_num] != '_') && (continuous_string[alt_field_num] != '\0') && ((p_fields[active_input_field_number].max_characters - 1) > alt_field_num))
                    /* HACK */  while( (continuous_string[alt_field_num] != '_') && (continuous_string[alt_field_num] != '\0') && ((Active_Input_Field_Max_Characters() - 1) > alt_field_num))
                    {
                        alt_field_num++;
                    }

                    // if(((p_fields[active_input_field_number].max_characters - 1) > alt_field_num))
                    /* HACK */  if(((Active_Input_Field_Max_Characters() - 1) > alt_field_num))
                    {
                        continuous_string[alt_field_num] = (char)character;
                        continuous_string[(alt_field_num + 1)] = '\0';
                    }
                    else
                    {
                        continuous_string[alt_field_num] = '\0';
                    }

                    GUI_EditCursorOn = ST_FALSE;
                    GUI_EditAnimStage = 0;
                }

            }
        }
        /*
            END:  Input Field
        */

        // ¿ default end of 'Yay KD' or just end of if(input_field_active > 0) ?
        goto Return_Type_Z11;

    }
    else  /* BEGIN:  Keyboard_Status() != ST_TRUE */
    {

        Mouse_Movement_Handler();

        if(Mouse_Button() != ST_FALSE)
        {

            mouse_button = Mouse_Button();

            /*
                BEGIN:  Right-Click Help OR Right-Click Cancel
            */
            {

                // seg036:0758 83 7E FA 02                                     cmp     [bp+mouse_button], e_ST_RIGHT_BUTTON ; Right Button Pressed
                // 000197B8  00026DB8  Interpret_Mouse_Input+74C
                // HERE: right click on movement map
                if(mouse_button == ST_RIGHT_BUTTON)
                {

                    // if(
                    //     (help_list_active == ST_TRUE)
                    //     &&
                    //     Check_Help_List() == 0
                    // )
                    /* CLAUDE */  /* REBUG */  /* asm 1139-1145: cmp [help_list_active], ST_FALSE / jz / call Check_Help_List / or ax, ax / jnz */
                    if(
                        (help_list_active != ST_FALSE)
                        &&
                        (Check_Help_List() == 0)
                    )
                    {

                        Mouse_Buffer();

                        Mouse_Buffer2();

                        return 0;

                    }
                    else
                    {

                        if(mouse_cancel_disabled == ST_FALSE)
                        {

                            while(Mouse_Button() == ST_RIGHT_BUTTON)
                            {

                                Quick_Call_Auto_Function();

                            }

                            /* CLAUDE */  /* REBUG */  /* asm 1162, 787-790: jmp @@ClearClickRec_Return_UNDEF - call Mouse_Buffer / call Mouse_Buffer2 / jmp @@Return_UNDEF */
                            Mouse_Buffer();

                            Mouse_Buffer2();

                            return ST_UNDEFINED;

                        }

                    }

                }

            }
            /*
                END:  Right-Click Help OR Right-Click Cancel
            */
            
/*
    BEGIN: Loop Mouse_Button()
*/
            {
                // IDA:  @@Loop_MouseButton  bright green #11
                // TODO  make this be the loop that it looks like - need some Input Field that makes use of the feature  e.g., Magic Screen Power Distribution Staves
                
                while(Mouse_Button() != 0)
                {

                    l_mx = Pointer_X();
                    l_my = Pointer_Y();
                    pointer_offset = Get_Pointer_Offset();
                    field_num = 0;
                    // Unused_Local == ST_UNDEFINED
                    /* OGBUG  dead store: Unused_Local is written here and in the Mouse_Buffer() path and never read */
                    Unused_Local = ST_UNDEFINED;  /* CLAUDE */  /* REBUG */  /* asm 1173: mov [bp+Unused_Local], e_ST_UNDEFINED */
                    character = 0;
                    field_num = Scan_Field();

                    /*
                        BEGIN BLOCK: field_num = Scan_Field() != 0
                    */
                    if(field_num != 0)
                    {
                    
                        /*
                            BEGIN: Left-Click - Push_Field_Down()
                        */
                        {
                            if(
                                (field_num != down_mouse_button)
                                &&
                                (p_fields[field_num].type != ft_Input)
                                &&
                                (p_fields[field_num].type != ft_ContinuousStringInput)
                                &&
                                !(
                                    (down_mouse_button != ST_UNDEFINED)
                                    &&
                                    (p_fields[field_num].type == ft_Grid)
                                    &&
                                    (p_fields[down_mouse_button].type == ft_Grid)
                                )
                            )
                            {
                                
                                // BUGBUG  down_mouse_button can be -1 here  AVRL  how to avoid?  shouldn't happen? conditions should be ||?
                                // if(p_fields[down_mouse_button].type == ft_Scroll)
                                /* OGBUG  on the first pass of a press down_mouse_button is -1 (set at the top), so the asm (1291-1296: mov ax, [down_mouse_button] / imul / cmp [es:bx+s_Field.type], ft_Scroll) reads the type of the record before p_fields */
                                /* HACK */  if((down_mouse_button != ST_UNDEFINED) && (p_fields[down_mouse_button].type == ft_Scroll))
                                {
                                    Invoke_Auto_Function();
                                }

                                Push_Field_Down(field_num, l_mx, l_my);
                            }
                        }
                        /*
                            END: Left-Click - Push_Field_Down()
                        */

                        // HERE:    Nay KD, Yay MD, No Help, No Cancel, Yay Field, field_num !=/== down_mouse_button
                        // Unhandled Conditions:
                        //     field_num == down_mouse_button
                        //     (field_num != down_mouse_button) && (p_fields[field_num].type == ft_Input)
                        //     (field_num != down_mouse_button) && (p_fields[field_num].type == ft_ContinuousStringInput)
                        //     (field_num != down_mouse_button) && (down_mouse_button != ST_UNDEFINED) && ¿ ... ?

                        // Yay/Nay click on an edit field that is the active edit field
                        // NOTE: In MoM, if you click off of an active edit field, it restores the string in the previous edit field and activates the new one
                        //       the activation includes, e.g., for the save game name the arrow/marker icon and the blicking underscore cursor

                        /*
                            BEGIN BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State
                        */

                        // if((p_fields[field_num].type != ft_ContinuousStringInput) && (input_field_active == ST_FALSE)) { // DO NOTHING }
                        // if(
                            // (p_fields[field_num].type != ft_ContinuousStringInput)
                            // &&
                            // (input_field_active != ST_FALSE)
                        // )
                        // {
                            // // `Exit Edit-State`
                            // itr_continuous_string = 0;
                            // while((continuous_string[itr_continuous_string] != '\0') && (p_fields[active_input_field_number].max_characters > itr_continuous_string))
                            // {
                                // itr_continuous_string++;
                            // }
                            // if(continuous_string[(itr_continuous_string - 1)] == '_')
                            // {
                                // itr_continuous_string--;
                            // }
                            // continuous_string[itr_continuous_string] = '\0';
                            // /* HACK  with active_input_field_number -1 (left by a commit, see the OGBUG in the commit branches) the OG copies the text to the near pointer in the record before p_fields; ReMoM skips that wild write */
                            // if(active_input_field_number != ST_UNDEFINED)
                            // {
                                // stu_strcpy((char *)p_fields[active_input_field_number].string, continuous_string);
                            // }
                            // input_field_active = ST_FALSE;
                            // active_input_field_number = ST_UNDEFINED;
                            // /* CLAUDE */ Hw_Textinput_Stop();
                        // }
                        // else if(
                                // (p_fields[field_num].type == ft_ContinuousStringInput)
                                // &&
                                // (input_field_active == ST_FALSE)
                            // )
                        // {
                            // stu_strcpy(continuous_string, p_fields[field_num].string);
                            // GUI_EditAnimStage = 0;
                            // GUI_EditCursorOn = ST_FALSE;
                            // input_field_active = ST_TRUE;
                            // active_input_field_number = field_num;
                            // /* CLAUDE */ Hw_Textinput_Start();
                        // }
                        // else if(
                                // (p_fields[field_num].type == ft_ContinuousStringInput)
                                // &&
                                // (input_field_active != ST_FALSE)
                                // &&
                                // (field_num != active_input_field_number)
                            // )
                        // {
                            // // `Exit Edit-State`  ~== `Cancel Edit-State`
                            // // `Enter Edit-State`
                            // stu_strcpy(continuous_string, p_fields[field_num].string);
                            // GUI_EditAnimStage = 0;
                            // GUI_EditCursorOn = ST_FALSE;
                            // input_field_active = ST_TRUE;
                            // active_input_field_number = field_num;
                        // }
                        /* CLAUDE */  /* REBUG */  /* asm 1309-1393: cmp [es:bx+s_Field.type], ft_ContinuousStringInput / jnz loc_273C8 - the CSI test is outer: CSI { !active -> enter; else active_num != field -> switch } else { active -> commit } */
                        if(p_fields[field_num].type == ft_ContinuousStringInput)
                        {
                            if(input_field_active == ST_FALSE)
                            {
                                stu_strcpy(continuous_string, p_fields[field_num].string);
                                GUI_EditAnimStage = 0;
                                GUI_EditCursorOn = ST_FALSE;
                                input_field_active = ST_TRUE;
                                active_input_field_number = field_num;
                                /* CLAUDE */ Hw_Textinput_Start();
                            }
                            else if(active_input_field_number != field_num)
                            {
                                // `Exit Edit-State`  ~== `Cancel Edit-State`
                                // `Enter Edit-State`
                                stu_strcpy(continuous_string, p_fields[field_num].string);
                                GUI_EditAnimStage = 0;
                                GUI_EditCursorOn = ST_FALSE;
                                input_field_active = ST_TRUE;
                                active_input_field_number = field_num;
                            }
                        }
                        else
                        {
                            if(input_field_active != ST_FALSE)
                            {
                                // `Exit Edit-State`
                                itr_continuous_string = 0;
                                // while((continuous_string[itr_continuous_string] != '\0') && (p_fields[active_input_field_number].max_characters > itr_continuous_string))
                                /* HACK */  while((continuous_string[itr_continuous_string] != '\0') && (Active_Input_Field_Max_Characters() > itr_continuous_string))
                                {
                                    itr_continuous_string++;
                                }
                                /* OGBUG  with an empty string the test reads continuous_string[-1], the high byte of last_button_x (0 or 1, never '_') */
                                /* HACK */  if((itr_continuous_string > 0) && (continuous_string[(itr_continuous_string - 1)] == '_'))
                                {
                                    itr_continuous_string--;
                                }
                                continuous_string[itr_continuous_string] = '\0';
                                /* HACK  with active_input_field_number -1 (left by a commit, see the OGBUG in the commit branches) the OG copies the text to the near pointer in the record before p_fields; ReMoM skips that wild write */
                                if(active_input_field_number != ST_UNDEFINED)
                                {
                                    stu_strcpy((char *)p_fields[active_input_field_number].string, continuous_string);
                                }
                                input_field_active = ST_FALSE;
                                active_input_field_number = ST_UNDEFINED;
                                /* CLAUDE */ Hw_Textinput_Stop();
                            }
                        }
                        // @@After_MouseLeftButton_ContinuousStringInput
                        /*
                            END BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State
                        */
                        // @@After_MouseButtonLeft_EditState

                        auto_input_variable = field_num;

                        if(mouse_auto_exit != ST_FALSE)
                        {
                            break;
                        }

                        if(Mouse_Button() != 0)
                        {
                            Call_Auto_Function();
                        }

                    }
                    /*
                        END BLOCK: field_num = Scan_Field() != 0
                    */
                    else  /* (field_num == 0) */
                    {
                        if(down_mouse_button != ST_UNDEFINED)
                        {
                            if(p_fields[down_mouse_button].type == ft_Scroll)
                            {
                                Invoke_Auto_Function();
                            }
                            if(
                                (p_fields[down_mouse_button].type != ft_MultiButton)
                                &&
                                (p_fields[down_mouse_button].type != ft_StringList)
                                &&
                                (p_fields[down_mouse_button].type != ft_ContinuousStringInput)
                            )
                            {
                                Save_Mouse_State();
                                Restore_Mouse_On_Page();
                                Draw_Field(down_mouse_button, DRAW_FIELD_UP);
                                Save_Mouse_On_Page(l_mx, l_my);
                                Draw_Mouse_On_Page(l_mx, l_my);
                                Set_Pointer_Position(l_mx, l_my);
                                Restore_Mouse_State();
                            }
                            down_mouse_button = ST_UNDEFINED;
                        }
                        Set_Buffer_2(l_mx, l_my);
                        break;
                    }
                
                }  /* while(Mouse_Button() != 0) */

                // @@After_Mouse_Button_Loop
            }
/*
    END: Loop Mouse_Button()
*/


            if(p_fields[auto_input_variable].type == ft_Scroll)
            {
                Invoke_Auto_Function();
            }

            auto_input_variable = 0;


            if(field_num != 0)
            {

                Mouse_Buffer();  // HERE: the MBB path does not call Mouse_Buffer()

                Mouse_Buffer2();
                
                last_button_x = l_mx;
                
                last_button_y = l_my;

                last_button_number = mouse_button;

                // TODO says switch 9 cases, but only actually handles 4 - figure out which, including the odd (type - 1) part
                // the (type - 1) part is a compiler artifact; it subtracts whatever amount to make the first case be case 0
                // here, that means there was no case for type 0 ft_Button
                // Did it. It's in the MBB branch.
                switch(p_fields[field_num].type)
                {
                    case ft_Button:
                    {
                        // DNE in Dasm
                    } break;
                    case ft_RadioButton:
                    {
                        if(*p_fields[field_num].state == 0)
                        {
                            *p_fields[field_num].state = 1;
                        }
                        else
                        {
                            *p_fields[field_num].state = 0;
                        }
                    } break;
                    case ft_LockedButton:
                    {
                        if(*p_fields[field_num].state == 0)
                        {
                            *p_fields[field_num].state = 1;
                        }
                        else
                        {
                            Mouse_Button_Handler();
                            down_mouse_button = ST_UNDEFINED;
                            return 0;
                        }
                    } break;
                    case ft_MultiButton:
                    {

                    } break;
                    case ft_Input:
                    {
                        Push_Field_Down(field_num, l_mx, l_my);
                    } break;
                    case ft_Picture:
                    {

                    } break;
                    case ft_Scroll:
                    {

                    } break;
                    case ft_HotKey:
                    {

                    } break;
                    case ft_MultiHotKey:
                    {

                    } break;
                    case ft_ClickLink:
                    {
                        if(mouse_button == ST_RIGHT_BUTTON)
                        {
                            Mouse_Button_Handler();
                            down_mouse_button = ST_UNDEFINED;
                            if(mouse_cancel_disabled == ST_FALSE)
                            {
                                return ST_UNDEFINED;
                            }
                            else
                            {
                                return -(p_fields[field_num].parent);
                            }
                        }
                        else
                        {
                            Mouse_Button_Handler();
                            down_mouse_button = ST_UNDEFINED;
                            return p_fields[field_num].parent;
                        }
                    } break;
                }
            }
            
            // IDA: @@EndOfTheClickRoad

            Mouse_Button_Handler();
            down_mouse_button = ST_UNDEFINED;
            /*
                1oom
                    if(mb == MOUSE_BUTTON_MASK_RIGHT)
                    {
                        return -field_num;
                    }
                    else
                    {
                        return field_num;
                    }
            */
            
            switch(mouse_button)
            {
                case 0: { field_num =          0; goto Done; } break;
                case 1: { /* field_num =  field_num; */ goto Done; } break;  /* NOTE(JimBalcomb,20260310): Dasm shows this, but VS complains about self assignment */
                case 2: { field_num = -field_num; goto Done; } break;
                /* CLAUDE */  /* REBUG */  /* asm 1542-1549: mov ax, [bp+mouse_button] / ... / jmp short @@Done - any other button value is returned as is */
                default: { return mouse_button; } break;
            }        
        
        }  /* END:  if(Mouse_Button() != ST_FALSE) */
        else if(Mouse_Buffer() != ST_FALSE)
        {

            mouse_button = Mouse_Buffer_Button();
            
            /*
                BEGIN:  Right-Click Help OR Right-Click Cancel
            */
            if(mouse_button == ST_RIGHT_BUTTON)
            {

                if(
                    (help_list_active != ST_FALSE)
                    &&
                    (Check_Help_List() == 0)
                )
                {

                    Mouse_Buffer();

                    Mouse_Buffer2();

                    return 0;

                }

                if(mouse_cancel_disabled == ST_FALSE)
                {

                    Mouse_Buffer();

                    Mouse_Buffer2();

                    return ST_UNDEFINED;

                }

            }
            /*
                END:  Right-Click Help OR Right-Click Cancel
            */

            l_mx = Mouse_Buffer_X();
            l_my = Mouse_Buffer_Y();
            field_num = 0;
            Unused_Local = ST_UNDEFINED;
            character = 0;
            Check_Mouse_Shape(l_mx, l_my);
            pointer_offset = Get_Pointer_Offset();
            for(alt_field_num = 1; alt_field_num < fields_count; alt_field_num++)
            {
                xmin = p_fields[alt_field_num].x1;
                ymin = p_fields[alt_field_num].y1;
                xmax = p_fields[alt_field_num].x2;
                ymax = p_fields[alt_field_num].y2;
                if(
                    ((l_mx + pointer_offset) >= xmin)
                    &&
                    ((l_mx + pointer_offset) <= xmax)
                    &&
                    ((l_my + pointer_offset) >= ymin)
                    &&
                    ((l_my + pointer_offset) <= ymax)
                )
                {
                    field_num = alt_field_num;
                    break;  /* CLAUDE */  /* REBUG */  /* asm 853-854: mov [bp+field_num], si / jmp short loc_26EB0 - the first hit ends the scan */
                }
            }

            if(
                (field_num != 0)
                &&
                (p_fields[field_num].type != ft_ContinuousStringInput)
            )
            {
                auto_input_variable = field_num;
                Push_Field_Down(field_num, l_mx, l_my);
                Quick_Call_Auto_Function();
            }

            if(field_num != 0)
            {

                /*
                    BEGIN BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State
                */

                if(p_fields[field_num].type == ft_ContinuousStringInput)
                {
                    if(input_field_active == ST_FALSE)
                    {
                        stu_strcpy(continuous_string, p_fields[field_num].string);
                        GUI_EditAnimStage = 0;
                        GUI_EditCursorOn = ST_FALSE;
                        input_field_active = ST_TRUE;
                        active_input_field_number = field_num;
                        /* CLAUDE */ Hw_Textinput_Start();
                    }
                    else if(active_input_field_number != field_num)
                    {
                        stu_strcpy(continuous_string, p_fields[field_num].string);
                        GUI_EditAnimStage = 0;
                        GUI_EditCursorOn = ST_FALSE;
                        active_input_field_number = field_num;
                    }
                    else
                    {
                        itr_continuous_string = 0;
                        while((continuous_string[itr_continuous_string] != '\0') && (p_fields[field_num].max_characters > itr_continuous_string))
                        {
                            itr_continuous_string++;
                        }
                        // if(continuous_string[(itr_continuous_string - 1)] == '_')
                        /* OGBUG  with an empty string the test reads continuous_string[-1], the high byte of last_button_x (0 or 1, never '_') */
                        /* HACK */  if((itr_continuous_string > 0) && (continuous_string[(itr_continuous_string - 1)] == '_'))
                        {
                            itr_continuous_string--;
                        }
                        continuous_string[itr_continuous_string] = '\0';
                        GUI_EditAnimStage = 0;
                        /* OGBUG  commits the text but clears only active_input_field_number, not input_field_active: until the next commit the edit loop works on field -1, the record before p_fields */
                        active_input_field_number = ST_UNDEFINED;
                        stu_strcpy((char *)p_fields[field_num].string, continuous_string);
                    }
                }
                else
                {
                    if(input_field_active != ST_FALSE)
                    {
                        itr_continuous_string = 0;
                        // while((continuous_string[itr_continuous_string] != '\0') && (p_fields[active_input_field_number].max_characters > itr_continuous_string))
                        /* HACK */  while((continuous_string[itr_continuous_string] != '\0') && (Active_Input_Field_Max_Characters() > itr_continuous_string))
                        {
                            itr_continuous_string++;
                        }
                        // if(continuous_string[(itr_continuous_string - 1)] == '_')
                        /* OGBUG  with an empty string the test reads continuous_string[-1], the high byte of last_button_x (0 or 1, never '_') */
                        /* HACK */  if((itr_continuous_string > 0) && (continuous_string[(itr_continuous_string - 1)] == '_'))
                        {
                            itr_continuous_string--;
                        }
                        continuous_string[itr_continuous_string] = '\0';
                        /* HACK  with active_input_field_number -1 (left by a commit, see the OGBUG in the commit branches) the OG copies the text to the near pointer in the record before p_fields; ReMoM skips that wild write */
                        if(active_input_field_number != ST_UNDEFINED)
                        {
                            stu_strcpy((char *)p_fields[active_input_field_number].string, continuous_string);
                        }
                        input_field_active = ST_FALSE;
                        active_input_field_number = ST_UNDEFINED;
                        /* CLAUDE */ Hw_Textinput_Stop();
                    }
                }
                // @@After_MouseLeftButton_ContinuousStringInput
                /*
                    END BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State
                */

            }

            down_mouse_button = ST_UNDEFINED;
            
            if(field_num != 0)
            {
                // HERE: the MB path calls Mouse_Buffer()
                Mouse_Buffer2();
                last_button_x = l_mx;
                last_button_y = l_my;
                last_button_number = mouse_button;
            }

            if(field_num != 0)
            {

                // @@MB_Switch_FieldType:
                // 5 branches?
                // (p_fields[field_num].type - 1)
                // {0, 1, 3, 8}
                // {ft_01__ft_RadioButton__1, ft_02__ft_LockedButton__1, ft_04__ft_Input__1, ft_09__ft_ClickLink__1}
                // {ft_01__ft_RadioButton__2, ft_02__ft_LockedButton__2, ft_04__ft_Input__2, ft_09__ft_ClickLink__2}
                switch(p_fields[field_num].type)
                {

                    case ft_RadioButton:
                    {
                        if(*p_fields[field_num].state == 0)
                        {
                            *p_fields[field_num].state = 1;
                        }
                        else
                        {
                            *p_fields[field_num].state = 0;
                        }
                    } break;

                    case ft_LockedButton:
                    {
                        if(*p_fields[field_num].state == 0)
                        {
                            *p_fields[field_num].state = 1;
                        }
                        else
                        {
                            Mouse_Button_Handler();
                            down_mouse_button = ST_UNDEFINED;
                            return 0;
                        }
                    } break;

                    case ft_Input:
                    {
                        Push_Field_Down(field_num, l_mx, l_my);
                    } break;

                    case ft_ClickLink:
                    {
                        if(mouse_button == ST_RIGHT_BUTTON)
                        {
                            Mouse_Button_Handler();
                            down_mouse_button = ST_UNDEFINED;
                            if(mouse_cancel_disabled == ST_FALSE)
                            {
                                return ST_UNDEFINED;
                            }
                            else
                            {
                                return -(p_fields[field_num].parent);
                            }
                        }
                        else
                        {
                            Mouse_Button_Handler();
                            down_mouse_button = ST_UNDEFINED;
                            return p_fields[field_num].parent;
                        }
                    } break;

                }  /* switch(p_fields[field_num].type) */

            }  /* if(field_num != 0) */

            Mouse_Button_Handler();

            auto_input_variable = ST_FALSE;

            if(mouse_button == ST_RIGHT_BUTTON)
            {
                // return_value = field_num;
                // jump-to return -(return_value)
                return -(field_num);
            }
            else
            {
                return field_num;
            }

        }
        else
        {
            // return 0;
            goto Return_Type_Z01;  /* CLAUDE */  /* REBUG */  /* asm 765-766: @@JmpNoInput: jmp @@MouseButtonHandler_ReturnZero - Mouse_Button_Handler() / return 0 */
        }

    }  /* BEGIN:  Keyboard_Status() != ST_TRUE */


    // TODO  if(character != 0)
    // TODO  {
    // TODO      
    // TODO  }



    goto Done;
/*
    ¿ Types of Departues ?
        YNM  Mouse_Button_Handler()
        YNM  down_mouse_button = ST_UNDEFINED
        YNM  Quick_Call_Auto_Function()
        Return Values:
            ...
            field_num
            ...
    Default:  return field_num

*/


/*
return ST_UNDEFINED;
*/
Return_Type_ESC:
    return ST_UNDEFINED;

/*
return 0;
*/
Return_Type_Z00:
    return 0;

/*
down_mouse_button = ST_UNDEFINED; return 0;
*/
Return_Type_Z10:
    down_mouse_button = ST_UNDEFINED;
    return 0;

/*
Mouse_Button_Handler(); return 0;
*/
Return_Type_Z01:
    Mouse_Button_Handler();
    return 0;

/*
down_mouse_button = ST_UNDEFINED; Mouse_Button_Handler(); return 0;
*/
Return_Type_Z11:
    down_mouse_button = ST_UNDEFINED;
    Mouse_Button_Handler();
    return 0;

/*
return field_num;
*/
#pragma warning(suppress : 4102)  // TODO  unreferenced label
Return_Type_F00:
    LOG_DEBUG(LOG_CAT_INPUT, "[IKI] Return_Type_F00: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_DEBUG(LOG_CAT_GENERAL, "[IKI] Return_Type_F00: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_TRACE(LOG_CAT_GENERAL, "[IKI] Return_Type_F00: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    return field_num;

/*
down_mouse_button = ST_UNDEFINED; return field_num;
*/
Return_Type_F10:
    LOG_DEBUG(LOG_CAT_INPUT, "[IKI] Return_Type_F10: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_DEBUG(LOG_CAT_GENERAL, "[IKI] Return_Type_F10: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_TRACE(LOG_CAT_GENERAL, "[IKI] Return_Type_F10: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    down_mouse_button = ST_UNDEFINED;
    return field_num;

/*
Mouse_Button_Handler(); return field_num;
*/
Return_Type_F01:
    LOG_DEBUG(LOG_CAT_INPUT, "[IKI] Return_Type_F01: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_DEBUG(LOG_CAT_GENERAL, "[IKI] Return_Type_F01: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_TRACE(LOG_CAT_GENERAL, "[IKI] Return_Type_F01: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    Mouse_Button_Handler();
    return field_num;

/*
down_mouse_button = ST_UNDEFINED; Mouse_Button_Handler(); return field_num;
*/
#pragma warning(suppress : 4102)  // TODO  unreferenced label
Return_Type_F11:
    LOG_DEBUG(LOG_CAT_INPUT, "[IKI] Return_Type_F11: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_DEBUG(LOG_CAT_GENERAL, "[IKI] Return_Type_F11: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_TRACE(LOG_CAT_GENERAL, "[IKI] Return_Type_F11: field_num=%d character=%d(0x%02X '%c')", field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    down_mouse_button = ST_UNDEFINED;
    Mouse_Button_Handler();
    return field_num;

/*
Mouse_Button_Handler(); down_mouse_button == ST_UNDEFINED; return alt_field_num;
*/
Return_Type_4:
    LOG_DEBUG(LOG_CAT_INPUT, "[IKI] Return_Type_4: alt_field_num=%d character=%d(0x%02X '%c')", alt_field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_DEBUG(LOG_CAT_GENERAL, "[IKI] Return_Type_4: alt_field_num=%d character=%d(0x%02X '%c')", alt_field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_TRACE(LOG_CAT_GENERAL, "[IKI] Return_Type_4: alt_field_num=%d character=%d(0x%02X '%c')", alt_field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    Mouse_Button_Handler();
    down_mouse_button = ST_UNDEFINED;
    return alt_field_num;

/*
down_mouse_button == ST_UNDEFINED; return alt_field_num;
*/
Return_Type_5:
    LOG_DEBUG(LOG_CAT_INPUT, "[IKI] Return_Type_5: alt_field_num=%d character=%d(0x%02X '%c')", alt_field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_DEBUG(LOG_CAT_GENERAL, "[IKI] Return_Type_5: alt_field_num=%d character=%d(0x%02X '%c')", alt_field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    LOG_TRACE(LOG_CAT_GENERAL, "[IKI] Return_Type_5: alt_field_num=%d character=%d(0x%02X '%c')", alt_field_num, character, (unsigned)(uint8_t)character, (character >= 32 && character < 127) ? (char)character : '.');
    down_mouse_button = ST_UNDEFINED;
    return alt_field_num;

/*
down_mouse_button = ST_UNDEFINED; return p_fields[field_num].Param0;
*/
Return_Type_P10:
    down_mouse_button = ST_UNDEFINED;
    return p_fields[field_num].parent;

/*
down_mouse_button = ST_UNDEFINED; return p_fields[alt_field_num].Param0;
*/
Return_Type_7:
    down_mouse_button = ST_UNDEFINED;
    return p_fields[alt_field_num].parent;

Done:
    return field_num;

}



// WZD s36p12
/*
MoO2
Module: fields
    function (0 bytes) Set_Help_List
    Address: 01:001196B8
        Num params: 2
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        Locals:
            pointer (4 bytes) help_pointer
            signed integer (2 bytes) count
*/
/*
Set_Help_List(_help_entries, 20);
*/
void Set_Help_List(void * help_pointer, int16_t count)
{
    // help_struct_pointer = (struct s_HELP_FIELD *)help_pointer;
    help_struct_pointer = (int16_t *)help_pointer;
    help_struct = (struct s_HELP_FIELD *)help_pointer;
    help_list_count = count;
    help_list_active = ST_TRUE;
}

// WZD s36p13
void Deactivate_Help_List(void)
{
    help_list_active = ST_FALSE;
    help_list_count = 0;
    // help_struct_pointer = ST_NULL;
    help_struct = ST_NULL;
    help_struct_pointer = ST_NULL;
}

// WZD s36p14
/*
; checks if the current mouse coordinates correspond
; to a help entry area and if so, calls GUI_DisplayHelp
; to show the corresponding context-based help text
; returns 0 if successful, or 1 if help was not
; available for the clicked location
*/
/*
MoO2
Module: fields
    function (0 bytes) Check_Help_List
    Address: 01:00119732
        Num params: 0
        Return type: signed integer (4 bytes) 
        Locals:
            signed integer (2 bytes) help_entry_found
            signed integer (2 bytes) mouse_x
            signed integer (2 bytes) mouse_y
            signed integer (2 bytes) i
            signed integer (2 bytes) j
*/
/*
    CAUTION: returns ZERO on SUCCESS

*/
int16_t Check_Help_List(void)
{
    int16_t mouse_x = 0;
    int16_t mouse_y = 0;
    int16_t help_entry_found = 0;
    int16_t i = 0;  // _DI_
    int16_t j = 0;  // _SI_
    int16_t DBG_help_idx = 0;  // enum HELP_TXT
    int16_t DBG_x1 = 0;
    int16_t DBG_y1 = 0;
    int16_t DBG_x2 = 0;
    int16_t DBG_y2 = 0;

    help_entry_found = 0;

    mouse_x = Pointer_X();

    mouse_y = Pointer_Y();

    if(help_list_count == 0)
    {

        return 1;

    }
    else
    {

        for(i = 0; i < help_list_count; i++)
        {

            // j = (i * sizeof(struct s_HELP_FIELD));
            j = (i * 5);

            DBG_help_idx = help_struct_pointer[j + 0];
            DBG_x1 = help_struct_pointer[j + 1];
            DBG_x2 = help_struct_pointer[j + 3];
            DBG_y1 = help_struct_pointer[j + 2];
            DBG_y2 = help_struct_pointer[j + 4];

            if(help_struct_pointer[j + 0] != ST_UNDEFINED)
            {

                if(
                    (help_struct_pointer[j + 1] <= mouse_x)
                    &&
                    (help_struct_pointer[j + 3] >= mouse_x)
                    &&
                    (help_struct_pointer[j + 2] <= mouse_y)
                    &&
                    (help_struct_pointer[j + 4] >= mouse_y)
                )
                {

                    Draw_Help_Entry__WIP(help_struct_pointer[j + 0]);

                    i = help_list_count;  // BUGBUG  useless

                    return 0;
                    
                }

            }

        }

    }

    return 1;
}

// UU_GUI_GetControlWidth                         seg036     000011C1 0000004D R F . . B T .
// UU_GUI_GetControlHeight                        seg036     0000120E 0000004E R F . . B T .
// UU_GUI_GetLastClickX                           seg036     0000125C 0000000A R F . . B T .
// UU_GUI_GetLastClickY                           seg036     00001266 0000000A R F . . B T .
// UU_GUI_GetLastClickBtns                        seg036     00001270 0000000A R F . . B T .
// Get_Mouse_Field                                seg036     0000127A 0000000A R F . . B T .
/* CLAUDE */  /* the procedure at seg036:127A is Auto_Input (WIZARDS.lst 000278DA: mov ax, [auto_input_variable]), reconstructed in Fields.c */

// WZD s36p15
// MGC s34p15
// drake178: UU_GUI_GetControlWidth()
/*
    Returns the width of field field_idx (x2 - x1 + 1), or 0 when the index is past the last field. A negative index is taken as its absolute value.
    Called nowhere in WIZARDS.EXE or MAGIC.EXE.
*/
int16_t UU_Get_Field_Width(int16_t field_idx)
{
    int16_t field_width = 0;

    if(field_idx < 0)
    {
        field_idx = -field_idx;
    }

    if(field_idx >= fields_count)
    {
        return 0;
    }

    field_width = ((p_fields[field_idx].x2 - p_fields[field_idx].x1) + 1);

    return field_width;
}

// WZD s36p16
// MGC s34p16
// drake178: UU_GUI_GetControlHeight()
/*
    Returns the height of field field_idx (y2 - y1 + 1), or 0 when the index is past the last field. A negative index is taken as its absolute value.
    Called nowhere in WIZARDS.EXE or MAGIC.EXE.
*/
int16_t UU_Get_Field_Height(int16_t field_idx)
{
    int16_t field_height = 0;

    if(field_idx < 0)
    {
        field_idx = -field_idx;
    }

    if(field_idx >= fields_count)
    {
        return 0;
    }

    field_height = ((p_fields[field_idx].y2 - p_fields[field_idx].y1) + 1);

    return field_height;
}

// WZD s36p17
// MGC s34p17
// drake178: UU_GUI_GetLastClickX()
/*
    Called nowhere in WIZARDS.EXE or MAGIC.EXE.
*/
int16_t UU_Get_Last_Click_X(void)
{
    return last_button_x;
}

// WZD s36p18
// MGC s34p18
// drake178: UU_GUI_GetLastClickY()
/*
    Called nowhere in WIZARDS.EXE or MAGIC.EXE.
*/
int16_t UU_Get_Last_Click_Y(void)
{
    return last_button_y;
}

// WZD s36p19
// MGC s34p19
// drake178: UU_GUI_GetLastClickBtns()
/*
    Called nowhere in WIZARDS.EXE or MAGIC.EXE.
*/
int16_t UU_Get_Last_Click_Buttons(void)
{
    return last_button_number;
}

// Keyboard.c  s36p21  RP_GUI_KeyInputOnly                            seg036     00001284 00000675 R F . . B T .
// Keyboard.c  s36p22  Interpret_Keyboard_Input                       seg036     000018F9 000004AB R F . . B T .

// WZD s36p23
// MGC s34p23
// drake178: GUI_GetNextControl()
/*
    Arrow-key navigation: from the pointer at (pointer_x, pointer_y), over the field current_field_idx, finds the nearest field in direction (x_direction, y_direction) {-1, 0, 1}.
    Candidates are the fields below ft_Grid with no hotkey; an ft_StringList only when it is Selectable. Returns the field found, current_field_idx if none, 0 if there are no candidates.
    Straight moves: first the nearest field whose center is within 6 pixels of the pointer's line, in front of it; failing that, the nearest by squared distance with the off-axis distance
    under 44% of the on-axis one. Diagonal moves: the nearest by squared distance with the smaller-to-larger distance ratio from 34% to 105%.
    The only caller is Process_Direction_Key().
*/
int16_t Get_Next_Field_In_Direction(int16_t x_direction, int16_t y_direction, int16_t current_field_idx, int16_t pointer_x, int16_t pointer_y)
{
    int32_t distance_squared = 0;
    int32_t closest_distance = 0;
    int32_t axis_ratio = 0;
    int16_t is_candidate = 0;
    int16_t candidate_count = 0;
    int16_t closest_field_idx = 0;
    int16_t y_distance = 0;
    int16_t itr = 0;  // _SI_
    int16_t x_distance = 0;  // _DI_

    x_distance = 500;
    y_distance = 500;
    axis_ratio = 500;
    closest_distance = 200000;  /* 30D40h */
    closest_field_idx = current_field_idx;

    candidate_count = 0;
    for(itr = 1; itr < fields_count; itr++)
    {
        if(p_fields[itr].type < ft_Grid && p_fields[itr].hotkey == 0)
        {
            if(p_fields[itr].type != ft_StringList)
            {
                candidate_count++;
            }
            else if(p_fields[itr].active_flag != 0)
            {
                candidate_count++;
            }
        }
    }

    if(candidate_count == 0)
    {
        return 0;
    }

    /* Pure vertical movement (y_direction != 0 && x_direction == 0) */
    if(y_direction != 0 && x_direction == 0)
    {
        for(itr = 1; itr < fields_count; itr++)
        {
            if(itr == current_field_idx)
            {
                if(fields_count - 1 == itr)
                {
                    break;
                }
                else
                {
                    itr = itr + 1;
                }
            }

            is_candidate = ST_FALSE;
            if(p_fields[itr].type < ft_Grid && p_fields[itr].hotkey == 0)
            {
                if(p_fields[itr].type == ft_StringList)
                {
                    if(p_fields[itr].active_flag != 0)
                    {
                        is_candidate = ST_TRUE;
                    }
                }
                else
                {
                    is_candidate = ST_TRUE;
                }
            }

            if(is_candidate != ST_FALSE)
            {
                if(y_direction < 0)
                {
                    y_distance = (pointer_y - p_fields[itr].y1) - ((p_fields[itr].y2 - p_fields[itr].y1) / 2);
                }
                else
                {
                    y_distance = (((p_fields[itr].y2 - p_fields[itr].y1) / 2) + p_fields[itr].y1) - pointer_y;
                }

                x_distance = (((p_fields[itr].x2 - p_fields[itr].x1) / 2) + p_fields[itr].x1) - pointer_x;

                if(p_fields[itr].x1 <= pointer_x && p_fields[itr].x2 >= pointer_x)
                {
                    if(x_distance < -6 || x_distance > 6)
                    {
                        x_distance = 6;
                    }
                }

                if(x_distance > -6 && x_distance < 6 && y_distance > 0)
                {
                    if(y_distance < closest_distance)
                    {
                        closest_distance = y_distance;
                        closest_field_idx = itr;
                    }
                }
            }
        }

        if(closest_field_idx == current_field_idx)
        {
            for(itr = 1; itr < fields_count; itr++)
            {
                if(itr == current_field_idx)
                {
                    if(fields_count - 1 == itr)
                    {
                        break;
                    }
                    else
                    {
                        itr = itr + 1;
                    }
                }

                is_candidate = ST_FALSE;
                if(p_fields[itr].type < ft_Grid && p_fields[itr].hotkey == 0)
                {
                    if(p_fields[itr].type == ft_StringList)
                    {
                        if(p_fields[itr].active_flag != 0)
                        {
                            is_candidate = ST_TRUE;
                        }
                    }
                    else
                    {
                        is_candidate = ST_TRUE;
                    }
                }

                if(is_candidate != ST_FALSE)
                {
                    if(y_direction < 0)
                    {
                        y_distance = (pointer_y - p_fields[itr].y1) - ((p_fields[itr].y2 - p_fields[itr].y1) / 2);
                    }
                    else
                    {
                        y_distance = (((p_fields[itr].y2 - p_fields[itr].y1) / 2) + p_fields[itr].y1) - pointer_y;
                    }

                    x_distance = (((p_fields[itr].x2 - p_fields[itr].x1) / 2) + p_fields[itr].x1) - pointer_x;

                    if(p_fields[itr].x1 <= pointer_x && p_fields[itr].x2 >= pointer_x)
                    {
                        if(x_distance < -6 || x_distance > 6)
                        {
                            x_distance = 6;
                        }
                    }

                    if(x_distance < 0)
                    {
                        x_distance = -x_distance;
                    }

                    if(y_distance < 0)
                    {
                        x_distance = 500;
                    }

                    if(y_distance == 0)
                    {
                        y_distance = 1;
                    }

                    axis_ratio = ((int32_t)x_distance * 100) / (int32_t)y_distance;

                    if(axis_ratio >= 0 && axis_ratio < 44)
                    {
                        distance_squared = ((int32_t)x_distance * (int32_t)x_distance) + ((int32_t)y_distance * (int32_t)y_distance);
                        if(distance_squared < closest_distance)
                        {
                            closest_distance = distance_squared;
                            closest_field_idx = itr;
                        }
                    }
                }
            }
        }
    }

    /* Pure horizontal movement (x_direction != 0 && y_direction == 0) */
    if(x_direction != 0 && y_direction == 0)
    {
        for(itr = 1; itr < fields_count; itr++)
        {
            if(itr == current_field_idx)
            {
                if(fields_count - 1 == itr)
                {
                    break;
                }
                else
                {
                    itr = itr + 1;
                }
            }

            is_candidate = ST_FALSE;
            if(p_fields[itr].type < ft_Grid && p_fields[itr].hotkey == 0)
            {
                if(p_fields[itr].type == ft_StringList)
                {
                    if(p_fields[itr].active_flag != 0)
                    {
                        is_candidate = ST_TRUE;
                    }
                }
                else
                {
                    is_candidate = ST_TRUE;
                }
            }

            if(is_candidate != ST_FALSE)
            {
                if(x_direction < 0)
                {
                    x_distance = (pointer_x - p_fields[itr].x1) - ((p_fields[itr].x2 - p_fields[itr].x1) / 2);
                }
                else
                {
                    x_distance = (p_fields[itr].x1 + ((p_fields[itr].x2 - p_fields[itr].x1) / 2)) - pointer_x;
                }

                y_distance = (((p_fields[itr].y2 - p_fields[itr].y1) / 2) + p_fields[itr].y1) - pointer_y;

                if(p_fields[itr].y1 <= pointer_y && p_fields[itr].y2 >= pointer_y)
                {
                    if(y_distance < -6 || y_distance > 6)
                    {
                        y_distance = 6;
                    }
                }

                if(y_distance > -6 && y_distance < 6 && x_distance > 0)
                {
                    if(x_distance < closest_distance)
                    {
                        closest_distance = x_distance;
                        closest_field_idx = itr;
                    }
                }
            }
        }

        if(closest_field_idx == current_field_idx)
        {
            for(itr = 1; itr < fields_count; itr++)
            {
                if(itr == current_field_idx)
                {
                    if(fields_count - 1 == itr)
                    {
                        break;
                    }
                    else
                    {
                        itr = itr + 1;
                    }
                }

                is_candidate = ST_FALSE;
                if(p_fields[itr].type < ft_Grid && p_fields[itr].hotkey == 0)
                {
                    if(p_fields[itr].type == ft_StringList)
                    {
                        if(p_fields[itr].active_flag != 0)
                        {
                            is_candidate = ST_TRUE;
                        }
                    }
                    else
                    {
                        is_candidate = ST_TRUE;
                    }
                }

                if(is_candidate != ST_FALSE)
                {
                    if(x_direction < 0)
                    {
                        x_distance = (pointer_x - p_fields[itr].x1) - ((p_fields[itr].x2 - p_fields[itr].x1) / 2);
                    }
                    else
                    {
                        x_distance = (p_fields[itr].x1 + ((p_fields[itr].x2 - p_fields[itr].x1) / 2)) - pointer_x;
                    }

                    y_distance = (((p_fields[itr].y2 - p_fields[itr].y1) / 2) + p_fields[itr].y1) - pointer_y;

                    if(p_fields[itr].y1 <= pointer_y && p_fields[itr].y2 >= pointer_y)
                    {
                        if(y_distance < -6 || y_distance > 6)
                        {
                            y_distance = 6;
                        }
                    }

                    if(y_distance < 0)
                    {
                        y_distance = -y_distance;
                    }

                    if(x_distance < 0)
                    {
                        y_distance = 500;
                    }

                    if(x_distance == 0)
                    {
                        x_distance = 1;
                    }

                    axis_ratio = ((int32_t)y_distance * 100) / (int32_t)x_distance;

                    if(axis_ratio >= 0 && axis_ratio < 44)
                    {
                        distance_squared = ((int32_t)x_distance * (int32_t)x_distance) + ((int32_t)y_distance * (int32_t)y_distance);
                        if(distance_squared < closest_distance)
                        {
                            closest_distance = distance_squared;
                            closest_field_idx = itr;
                        }
                    }
                }
            }
        }
    }

    /* Diagonal upward movement (x_direction != 0 && y_direction < 0) */
    if(x_direction != 0 && y_direction < 0)
    {
        for(itr = 1; itr < fields_count; itr++)
        {
            if(itr == current_field_idx)
            {
                if(fields_count - 1 == itr)
                {
                    break;
                }
                else
                {
                    itr = itr + 1;
                }
            }

            is_candidate = ST_FALSE;
            if(p_fields[itr].type < ft_Grid && p_fields[itr].hotkey == 0)
            {
                if(p_fields[itr].type == ft_StringList)
                {
                    if(p_fields[itr].active_flag != 0)
                    {
                        is_candidate = ST_TRUE;
                    }
                }
                else
                {
                    is_candidate = ST_TRUE;
                }
            }

            if(is_candidate != ST_FALSE)
            {
                if(x_direction > 0)
                {
                    x_distance = (p_fields[itr].x1 + ((p_fields[itr].x2 - p_fields[itr].x1) / 2)) - pointer_x;
                }
                else
                {
                    x_distance = (pointer_x - p_fields[itr].x1) - ((p_fields[itr].x2 - p_fields[itr].x1) / 2);
                }

                y_distance = (pointer_y - ((p_fields[itr].y2 - p_fields[itr].y1) / 2)) - p_fields[itr].y1;

                if(x_distance < 0 || y_distance < 0)
                {
                    axis_ratio = 500;
                }
                else
                {
                    if(x_distance >= y_distance && y_distance != 0)
                    {
                        axis_ratio = ((int32_t)y_distance * 100) / (int32_t)x_distance;
                    }
                    if(y_distance > x_distance && x_distance != 0)
                    {
                        axis_ratio = ((int32_t)x_distance * 100) / (int32_t)y_distance;
                    }
                    if(x_distance == 0 || y_distance == 0)
                    {
                        axis_ratio = 500;
                    }

                    if(axis_ratio >= 34 && axis_ratio <= 105)
                    {
                        distance_squared = ((int32_t)x_distance * (int32_t)x_distance) + ((int32_t)y_distance * (int32_t)y_distance);
                        if(distance_squared < closest_distance)
                        {
                            closest_distance = distance_squared;
                            closest_field_idx = itr;
                        }
                    }
                }
            }
        }
    }

    /* Diagonal downward movement (x_direction != 0 && y_direction > 0) */
    if(x_direction != 0 && y_direction > 0)
    {
        for(itr = 1; itr < fields_count; itr++)
        {
            if(itr == current_field_idx)
            {
                if(fields_count - 1 == itr)
                {
                    break;
                }
                else
                {
                    itr = itr + 1;
                }
            }

            is_candidate = ST_FALSE;
            if(p_fields[itr].type < ft_Grid && p_fields[itr].hotkey == 0)
            {
                if(p_fields[itr].type == ft_StringList)
                {
                    if(p_fields[itr].active_flag != 0)
                    {
                        is_candidate = ST_TRUE;
                    }
                }
                else
                {
                    is_candidate = ST_TRUE;
                }
            }

            if(is_candidate != ST_FALSE)
            {
                if(x_direction > 0)
                {
                    x_distance = (p_fields[itr].x1 + ((p_fields[itr].x2 - p_fields[itr].x1) / 2)) - pointer_x;
                }
                else
                {
                    x_distance = (pointer_x - p_fields[itr].x1) - ((p_fields[itr].x2 - p_fields[itr].x1) / 2);
                }

                y_distance = (((p_fields[itr].y2 - p_fields[itr].y1) / 2) + p_fields[itr].y1) - pointer_y;

                if(x_distance < 0 || y_distance < 0)
                {
                    axis_ratio = 500;
                }
                else
                {
                    if(x_distance >= y_distance && y_distance != 0)
                    {
                        axis_ratio = ((int32_t)y_distance * 100) / (int32_t)x_distance;
                    }
                    if(y_distance > x_distance && x_distance != 0)
                    {
                        axis_ratio = ((int32_t)x_distance * 100) / (int32_t)y_distance;
                    }
                    if(x_distance == 0 || y_distance == 0)
                    {
                        axis_ratio = 500;
                    }

                    if(axis_ratio >= 34 && axis_ratio <= 105)
                    {
                        distance_squared = ((int32_t)x_distance * (int32_t)x_distance) + ((int32_t)y_distance * (int32_t)y_distance);
                        if(distance_squared < closest_distance)
                        {
                            closest_distance = distance_squared;
                            closest_field_idx = itr;
                        }
                    }
                }
            }
        }
    }

    return closest_field_idx;
}


// WZD s36p24
// MGC s34p24
// drake178: UU_GUI_MouseEMUMoveTo()
// 1oom  uiobj.c  uiobj_set_focus()
/*
; Unused in WIZARDS.EXE
;
; if there is no mouse, moves the cursor to the middle
; of the specified control
;
; MAGIC.EXE uses this to move the cursor when it
; displays the main menu screen after something else
*/
/*
    With no mouse installed, puts the emulated pointer on the center of field field_idx and redraws the pointer there; with a mouse it does nothing.
    MAGIC.EXE Main_Menu_Screen() calls it for the New Game button; WIZARDS.EXE never calls it.
*/
void Center_Emulated_Pointer_On_Field(int16_t field_idx)
{
    int16_t center_y = 0;
    int16_t center_x = 0;  // _DI_

    // if(mouse_installed)
    /* CLAUDE */  /* REBUG */  /* asm: cmp [mouse_installed], ST_FALSE / jz loc_291E6 (the body) / jmp @@Done - the body runs only when there is no mouse */
    if(mouse_installed == ST_FALSE)
    {
        center_x = (p_fields[field_idx].x1 + ((p_fields[field_idx].x2 - p_fields[field_idx].x1) / 2));
        center_y = (p_fields[field_idx].y1 + ((p_fields[field_idx].y2 - p_fields[field_idx].y1) / 2));

        if((center_y < 0) || (center_y >= SCREEN_HEIGHT) || (center_x < 0) || (center_x >= SCREEN_WIDTH))
        {
            return;
        }

        emulated_pointer_x = center_x;
        emulated_pointer_y = center_y;
        Check_Mouse_Shape(emulated_pointer_x, emulated_pointer_y);
        pointer_offset = Get_Pointer_Offset();
        emulated_pointer_x = (emulated_pointer_x - pointer_offset);
        emulated_pointer_y = (emulated_pointer_y - pointer_offset);
        Set_Pointer_Position(emulated_pointer_x, emulated_pointer_y);
        Restore_Mouse_On_Page();
        Save_Mouse_On_Page(emulated_pointer_x, emulated_pointer_y);
        Draw_Mouse_On_Page(emulated_pointer_x, emulated_pointer_y);
    }

}


// WZD s36p25
/*
    Returns field_idx of Field at Pointer Position
        0 if no field box contains the pointer
*/
int16_t Scan_Field(void)
{
    int16_t mx;
    int16_t my;
    int16_t itr;
    int16_t current_field;
    int16_t xmin;
    int16_t ymin;
    int16_t xmax;
    int16_t ymax;

    mx = Pointer_X();
    my = Pointer_Y();

    current_field = 0;

    Check_Mouse_Shape(mx, my);
    pointer_offset = Get_Pointer_Offset();

    for(itr = 1; itr < fields_count; itr++)
    {
        xmin = p_fields[itr].x1;
        ymin = p_fields[itr].y1;
        xmax = p_fields[itr].x2;
        ymax = p_fields[itr].y2;

        if(
            (mx + pointer_offset >= xmin) &&
            (mx + pointer_offset <= xmax) &&
            (my + pointer_offset >= ymin) &&
            (my + pointer_offset <= ymax)
        )
        {
            current_field = itr;
            break;
        }
    }

    return current_field;
}

// WZD s36p26
int16_t Scan_Input(void)
{
    int16_t mx = 0;
    int16_t my = 0;
    int16_t itr = 0;
    int16_t current_field = 0;

    mx = Pointer_X();
    my = Pointer_Y();

    current_field = 0;

    Check_Mouse_Shape(mx, my);
    pointer_offset = Get_Pointer_Offset();

    current_field = Scan_Field();

    if(p_fields[current_field].type == ft_ClickLink)
    {
        *p_fields[current_field].variable1 = p_fields[current_field].Param1;
        return p_fields[current_field].parent;
    }

    if(
        (p_fields[current_field].type == ft_StringList)
        &&
        (p_fields[current_field].active_flag == ST_FALSE)
    )
    {
        return 0;
    }

    if(
        (p_fields[current_field].type == ft_Grid)
        &&
        (
            (down_mouse_button == ST_UNDEFINED)
            ||
            (p_fields[down_mouse_button].type != ft_Grid)
        )
    )
    {
        // A sub-expression may overflow before being assigned to a wider type.
        *((int64_t *)p_fields[current_field].Param3) = ((Pointer_X() - p_fields[current_field].x1) / p_fields[current_field].Param1);
        *((int64_t *)p_fields[current_field].Param4) = ((Pointer_Y() - p_fields[current_field].y1) / p_fields[current_field].Param2);
    }

    return current_field;
}


// WZD s36p27
// drake178: UU_GUI_KeyWaitLoop()
// MoO2  Module: fields  Read_Character()  returns signed integer (2 bytes)
/*
Unused in MoM

loops until keyboard input is available, which it
returns
*/
/*
    Eh? NIU in MoO1, MoM, and MoO2. What about SL?
*/
uint8_t Read_Character(void)
{
    while(!Keyboard_Status()) {}
    return Read_Key();
}


// WZD s36p28
// MoO2  Module: fields  Reset_Wait_For_Input()
void Reset_Wait_For_Input(void)
{

    auto_input_variable = 0;

    while(Keyboard_Status() != 0)
    {
        Read_Key();
    }

    if(mouse_installed != ST_FALSE)
    {
        while(Mouse_Button() != 0)
        {
            Call_Auto_Function();
        }
    }

    Mouse_Buffer();
    Mouse_Buffer2();

}


// WZD s36p29
// MoO2  Module: fields  Wait_For_Input()
int16_t Wait_For_Input(void)
{
    // char charcode;  // clangd says comparrisons with ST_KEY_F11, etc. are always false, because they are out of range
    uint8_t charcode;
    int16_t keyboard_flag;
    int16_t mouse_button_flag;

    Reset_Wait_For_Input();

    keyboard_flag = ST_FALSE;
    mouse_button_flag = ST_FALSE;

    temp_field_count = fields_count;

    fields_count = 1;

    while(keyboard_flag == ST_FALSE)
    {
        if(mouse_installed != ST_FALSE)
        {
            Mouse_Movement_Handler();
            if((Mouse_Button() != 0) || (Mouse_Buffer() != 0))
            {
                mouse_button_flag = ST_TRUE;
                keyboard_flag = ST_TRUE;
            }
            if(Keyboard_Status() != 0)
            {
                /* HACK */  Platform_Event_Handler();  /* TODO  consolidate 'record key press' effort */
                charcode = Read_Key();
                if(charcode == ST_KEY_F11)
                {
                    F11_Key();
                }
                else if(charcode == ST_KEY_F12)
                {
                    Save_Mouse_State();
                    Restore_Mouse_On_Page();
                    Screen_Flic_Capture();
                    Save_Mouse_On_Page(Pointer_X(), Pointer_Y());
                    Draw_Mouse_On_Page(Pointer_X(), Pointer_Y());
                    Set_Pointer_Position(Pointer_X(), Pointer_Y());
                    Restore_Mouse_State();
                }
                else
                {
                    Mouse_Buffer();
                    keyboard_flag = ST_TRUE;
                }
            }
        }
        else
        {
            if(Keyboard_Status() != 0)
            {
                /* HACK */  Platform_Event_Handler();  /* TODO  consolidate 'record key press' effort */
                charcode = Read_Key();
                if(charcode == ST_KEY_F11)
                {
                    F11_Key();
                }
                else if(charcode == ST_KEY_F12)
                {
                    Save_Mouse_State();
                    Restore_Mouse_On_Page();
                    Screen_Flic_Capture();
                    Save_Mouse_On_Page(Pointer_X(), Pointer_Y());
                    Draw_Mouse_On_Page(Pointer_X(), Pointer_Y());
                    Set_Pointer_Position(Pointer_X(), Pointer_Y());
                    Restore_Mouse_State();
                }
                else
                {
                    Mouse_Buffer();
                    keyboard_flag = ST_TRUE;
                }
            }
        }
        Mouse_Button_Handler();
        Call_Auto_Function();
    }

    if(mouse_button_flag != ST_FALSE)
    {
        Mouse_Movement_Handler();
        while(Mouse_Button() != 0)
        {
            Call_Auto_Function();
        }
    }

    fields_count = temp_field_count;
    
    Mouse_Button_Handler();
    Mouse_Buffer();
    Mouse_Buffer2();

    return ST_SUCCESS;
}


// WZD s36p30
// MGC s34p30
// drake178: GUI_ProcessDirKey()
// MoO2 ?
// 1oom ?
/*
    dir_key: 'L', 'R', 'U', 'D' (straight), 'W', 'X', 'Y', 'Z' (diagonal: up-left, up-right, down-left, down-right).
    In a list field (list_field_on) an up / down key moves the list highlight instead.
    Otherwise finds the field under the pointer, asks Get_Next_Field_In_Direction() for the nearest field in the key's direction, and moves the emulated pointer to that field's
    center, redrawing the pointer there. Returns the field reached (the one under the pointer when there is none, or when the new center is off the screen).
*/
int16_t Process_Direction_Key(int16_t dir_key)
{
    int16_t y_direction = 0;
    int16_t x_direction = 0;
    int16_t field_under_pointer = 0;
    int16_t pointer_y = 0;
    int16_t pointer_x = 0;
    int16_t field_num = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    switch(dir_key)
    {
        case 'L': { x_direction = -1; y_direction =  0; } break;
        case 'R': { x_direction =  1; y_direction =  0; } break;
        case 'U': { x_direction =  0; y_direction = -1; } break;
        case 'D': { x_direction =  0; y_direction =  1; } break;
        case 'W': { x_direction = -1; y_direction = -1; } break;
        case 'X': { x_direction =  1; y_direction = -1; } break;
        case 'Y': { x_direction = -1; y_direction =  1; } break;
        case 'Z': { x_direction =  1; y_direction =  1; } break;
        /* CLAUDE */  /* REBUG */  /* asm: sub ax, 'D' / cmp bx, 16h / ja dirkey__01_DNE - the default case is in the asm (x 0, y 0) */
        default:  { x_direction =  0; y_direction =  0; } break;
    }

    if(
        (list_field_on != 0)
        &&
        (y_direction != 0)
    )
    {
        field_num = 0;
        field_num = Move_Dialog_Highlight(y_direction);
        return field_num;
    }

    // if(mouse_installed == ST_FALSE)
    // {
    //     Mouse_X = emulated_pointer_x;
    //     Mouse_Y = emulated_pointer_y;
    // }
    // else
    // {
    //     Mouse_X = Pointer_X();
    //     Mouse_Y = Pointer_Y();
    // }
    /* CLAUDE */  /* REBUG */  /* asm: cmp [mouse_installed], ST_FALSE / jz loc_297A9 - the then-arm is Pointer_X() / Pointer_Y(), the emulated pointer (loc_297A9) is the else */
    if(mouse_installed != ST_FALSE)
    {
        pointer_x = Pointer_X();
        pointer_y = Pointer_Y();
    }
    else
    {
        pointer_x = emulated_pointer_x;
        pointer_y = emulated_pointer_y;
    }

    field_under_pointer = 0;

    /* CLAUDE */  /* the loop body and everything after it were missing (the function was a stub): reconstructed from the asm, loc_297C3 .. @@Done */
    for(itr = (fields_count - 1); itr > 0; itr--)
    {
        if(
            (p_fields[itr].type < ft_Grid)
            &&
            (p_fields[itr].y1 <= (pointer_y + pointer_offset))
            &&
            (p_fields[itr].y2 >= (pointer_y + pointer_offset))
            &&
            (p_fields[itr].x1 <= (pointer_x + pointer_offset))
            &&
            (p_fields[itr].x2 >= (pointer_x + pointer_offset))
        )
        {
            if(p_fields[itr].type != ft_StringList)
            {
                field_under_pointer = itr;
            }
            else if(p_fields[itr].active_flag != 0)
            {
                field_under_pointer = itr;
            }
        }
    }

    field_num = field_under_pointer;

    if(
        (x_direction != 0)
        ||
        (y_direction != 0)
    )
    {
        field_num = Get_Next_Field_In_Direction(x_direction, y_direction, field_under_pointer, pointer_x, pointer_y);

        if(
            (field_num != field_under_pointer)
            &&
            (field_num != 0)
        )
        {
            emulated_pointer_x = (p_fields[field_num].x1 + ((p_fields[field_num].x2 - p_fields[field_num].x1) / 2));
            emulated_pointer_y = (p_fields[field_num].y1 + ((p_fields[field_num].y2 - p_fields[field_num].y1) / 2));

            Save_Mouse_State();

            if(
                (emulated_pointer_x > 0)
                &&
                (emulated_pointer_x < SCREEN_WIDTH)
                &&
                (emulated_pointer_y > 0)
                &&
                (emulated_pointer_y < SCREEN_HEIGHT)
            )
            {
                Check_Mouse_Shape(emulated_pointer_x, emulated_pointer_y);
                pointer_offset = Get_Pointer_Offset();
                emulated_pointer_x = (emulated_pointer_x - pointer_offset);
                emulated_pointer_y = (emulated_pointer_y - pointer_offset);
                Restore_Mouse_On_Page();
                Save_Mouse_On_Page(emulated_pointer_x, emulated_pointer_y);
                Draw_Mouse_On_Page(emulated_pointer_x, emulated_pointer_y);
                Set_Pointer_Position(emulated_pointer_x, emulated_pointer_y);
            }
            else
            {
                field_num = field_under_pointer;
                emulated_pointer_x = pointer_x;
                emulated_pointer_y = pointer_y;
            }

            Restore_Mouse_State();
        }
    }

    return field_num;
}


// WZD s36p31
// drake178: GUI_EditBoxControl()
/*
; creates and processes the event loop for an active
; edit box control, using any defined redraw function
; to refresh the screen, and setting the input text to
; the pointer defined in the control when finished
; WARNING: this function does not return until the edit box loses focus!
*/
/*
    only called from Draw_Feild()
    only processed if it is not selected
    doesn't/can't happen via Setup_Input_Box_Popup()
    any Add_Input_Field() outside of Setup_Input_Box_Popup()?
        only ITEM_CreateControls()
*/
void Input_Box_Popup(int16_t field_num)
{
    char input_string[63] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // char key;  // ¿ clangd ... out of range ?
    uint8_t key = 0;
    int16_t Timeout_Counter = 0;
    int16_t Control_Change = 0;
    int16_t Font_Height = 0;
    int16_t width = 0;
    int16_t String_Modified = 0;
    int16_t max_characters = 0;
    int16_t Allowed_Char = 0;
    int16_t string_pos = 0;
    int16_t itr = 0;

    Timeout_Counter = 4;

    // Clear_Fields();
    /* CLAUDE */  /* REBUG */  /* asm 20-21: mov [bp+Timeout_Counter], 4 / cmp [mouse_installed], ST_FALSE - no Clear_Fields() call */

    Hw_Textinput_Start();

    if(mouse_installed != ST_FALSE)
    {

        while(Mouse_Button() != 0) { Invoke_Auto_Function(); }

    }

    GUI_Edit_Position = 0;

    GUI_EditAnimStage = 0;

    max_characters = p_fields[field_num].max_characters;

    width = (p_fields[field_num].x2 - p_fields[field_num].x1);

    String_Modified = ST_FALSE;

    Save_Mouse_State();

    string_pos = 0;

    key = 0;

    Set_Font_Style(p_fields[field_num].font_style_num, p_fields[field_num].font_normal_color, p_fields[field_num].font_highlight_color, ST_NULL);

    stu_strcpy(input_string, p_fields[field_num].string);

    while(input_string[string_pos] != '\0') { string_pos++; }

    while((Get_String_Width(input_string) > width) && (string_pos > 0))
    {

        /* OGBUG  `string_pos -= string_pos` (asm `sub si, si`) sets string_pos to 0 instead of 1 less, so the first pass empties the whole default text: a default wider than the field is shown as an empty field */
        string_pos -= string_pos;

        input_string[string_pos] = 0;

    }

    GUI_Edit_Position = string_pos;

    // SETMAX(GUI_Edit_Position, max_characters);
    /* CLAUDE */  /* REBUG */  /* asm 113-117: cmp ax, [bp+max_characters] / jl loc_29B0D / mov [GUI_Edit_Position], ax */
    if(GUI_Edit_Position >= max_characters)
    {
        GUI_Edit_Position = max_characters;
    }

    // stu_strcpy(input_string, p_fields[field_num].string);
    stu_strcpy(p_fields[field_num].string, input_string);  /* CLAUDE */  /* REBUG */  /* asm 119-127: push input_string (src) / push [es:bx+s_Field.Param0__string] (dst) / call strcpy - ReMoM copied the other way, which undid the trim above */

    Font_Height = Get_Font_Height();

    Draw_Input_Box_Popup(field_num, input_string);


    /*
        BEGIN:  
    */

    Control_Change = ST_FALSE;

    // 1oom:  while((key != MOO_KEY_RETURN) && (!flag_mouse_button))
    // while((Control_Change == ST_FALSE) && (key != ST_KEY_ENTER))
    /* CLAUDE */  /* REBUG */  /* asm loc_29DC4: cmp [bp+key], ST_KEY_ENTER / jz / cmp [bp+Control_Change], ST_FALSE / jnz */
    while((key != ST_KEY_ENTER) && (Control_Change == ST_FALSE))
    {

        // if(Control_Change != ST_FALSE)
        // {
        //     break;
        // }
        /* CLAUDE */  /* REBUG */  /* asm loc_29B48: jmp short loc_29BA0 - straight to the inner loop test */

        while((Keyboard_Status() == 0) && (Control_Change == ST_FALSE))
        {

            if(GUI_EditTimeOutType == ST_UNDEFINED)
            {
                Timeout_Counter--;
            }

            if(Timeout_Counter == 0)
            {
                break;
            }

            if(
                (mouse_installed != ST_FALSE)
                &&
                (
                    (Mouse_Button() != 0)
                    ||
                    (Mouse_Buffer() != 0)
                )
            )
            {

                Control_Change = ST_TRUE;

                break;

            }

            GUI_EditAnimStage++;

            if(((Font_Height * 2) - 1) < GUI_EditAnimStage)
            {

                GUI_EditAnimStage = 0;
                
            }

            Draw_Input_Box_Popup(field_num, input_string);

        }

        if(
            (Timeout_Counter == 0)
            ||
            (Control_Change != ST_FALSE)
        )
        {
            break;
        }

        /* HACK */  Platform_Event_Handler();  /* TODO  consolidate 'record key press' effort */
        key = Read_Key();

/*
ST_KEY_LEFT             = 0x01
ST_KEY_RIGHT            = 0x02
ST_KEY_BACKSPACE        = 0x0B
ST_KEY_ENTER            = 0x0C
*/
        switch(key)
        {
            case ST_KEY_LEFT:
            {
                String_Modified = ST_TRUE;
                if(GUI_Edit_Position > 0)
                {
                    GUI_Edit_Position--;
                    GUI_EditAnimStage = 0;
                }
            } break;
            case ST_KEY_RIGHT:
            {
                if(
                    (GUI_Edit_Position < max_characters)
                    &&
                    (GUI_Edit_Position < string_pos)
                )
                {
                    GUI_Edit_Position++;
                    GUI_EditAnimStage = 0;
                    // if(GUI_Edit_Position == string_pos)
                    // {
                    //     input_string[string_pos] = ' ';
                    //     input_string[(string_pos + 1)] = '\0';
                    //     if(Get_String_Width(input_string) > width)
                    //     {
                    //         GUI_Edit_Position--;
                    //     }
                    //     input_string[string_pos] = '\0';
                    // }
                    /* CLAUDE */  /* REBUG */  /* asm 282-298: cmp [GUI_Edit_Position], si / jl loc_29CCE (so >=, not ==); then cmp ax, [bp+max_characters] / jge loc_29CC6 (dec) before the Get_String_Width() test - the max_characters test was missing (Setup_Input_Box_Popup() has == and no max test) */
                    if(GUI_Edit_Position >= string_pos)
                    {
                        input_string[string_pos] = ' ';
                        input_string[(string_pos + 1)] = '\0';
                        if(
                            (GUI_Edit_Position >= max_characters)
                            ||
                            (Get_String_Width(input_string) > width)
                        )
                        {
                            GUI_Edit_Position--;
                        }
                        input_string[string_pos] = '\0';
                    }
                }
            } break;
            case ST_KEY_DELETE:
            {
                // if((
                //     string_pos > 0)
                //     &&
                //     (GUI_Edit_Position > string_pos)
                // )
                // {
                //     for(itr = GUI_Edit_Position; itr < string_pos; itr++)
                //     {
                //         input_string[(itr)] = input_string[(itr + 1)];
                //     }
                //     input_string[string_pos] = '\0';
                // }
                /* CLAUDE */  /* REBUG */  /* asm (Input_Box_Popup loc_29C47, Setup_Input_Box_Popup key_09): or si, si / jle / cmp [GUI_Edit_Position], si / jge - the test is Edit_Position < string_pos; after the shift: dec si / mov [GUI_EditAnimStage], 0 / mov [bp+si+input_string], 0 */
                if(
                    (string_pos > 0)
                    &&
                    (GUI_Edit_Position < string_pos)
                )
                {
                    for(itr = GUI_Edit_Position; itr < string_pos; itr++)
                    {
                        input_string[(itr)] = input_string[(itr + 1)];
                    }
                    string_pos--;
                    GUI_EditAnimStage = 0;
                    input_string[string_pos] = '\0';
                }
            } break;
            case ST_KEY_BACKSPACE:
            {
                if(String_Modified == ST_FALSE)
                {
                    input_string[0] = '\0';
                    string_pos = 0;
                    GUI_Edit_Position = 0;
                    GUI_EditAnimStage = 0;
                    String_Modified = ST_TRUE;
                }
                else
                {
                    if(string_pos > 0)
                    {
                        if(GUI_Edit_Position >= string_pos)
                        {
                            string_pos--;
                            input_string[string_pos] = '\0';
                            GUI_Edit_Position--;
                            GUI_EditAnimStage = 0;
                        }
                        else
                        {
                            if(GUI_Edit_Position > 0)
                            {
                                for(itr = GUI_Edit_Position; itr < string_pos; itr++)
                                {
                                    input_string[(itr - 1)] = input_string[itr];
                                }
                                string_pos--;
                                GUI_Edit_Position--;
                                GUI_EditAnimStage = 0;
                                input_string[string_pos] = 0;
                            }
                        }
                    }
                }
            } break;
            default:  /* key_02_03_04_05_06_07_08 */
            {
                // Allowed_Char = ST_FALSE;
                // if(key == ST_KEY_ESCAPE)
                // {
                    // Control_Change = 2;
                    // Allowed_Char = ST_FALSE;
                // }
                // else
                // {
                    // if(
                        // ((key > 64) && (key < 93))
                        // ||
                        // ((key > 45) && (key < 59))
                        // ||
                        // (key == ' ')
                        // ||
                        // (key == '-')
                        // ||
                        // ((key > 96) && (key < 123))
                    // )
                    // {
                        // Allowed_Char = ST_TRUE;
                    // }
                    // if(Allowed_Char == ST_TRUE)
                    // {
                        // String_Modified = ST_TRUE;
                        // input_string[string_pos] = key;
                        // input_string[(string_pos + 1)] = '\0';
                        // if(
                            // (string_pos < max_characters)
                            // &&
                            // (Get_String_Width(input_string) <= width)
                        // )
                        // {
                            // input_string[string_pos] = '\0';
                            // if(GUI_Edit_Position < string_pos)
                            // {
                                // for(itr = string_pos; itr < GUI_Edit_Position; itr++)
                                // {
                                    // input_string[itr] = input_string[(itr - 1)];
                                // }
                                // string_pos++;
                                // input_string[GUI_Edit_Position] = key;
                                // GUI_Edit_Position++;
                            // }
                            // else
                            // {
                                // input_string[string_pos] = key;
                                // string_pos++;
                                // input_string[string_pos] = ' ';
                                // input_string[(string_pos + 1)] = '\0';
                                // if(
                                    // (string_pos < max_characters)
                                    // &&
                                    // (Get_String_Width(input_string) <= width)
                                // )
                                // {
                                    // GUI_Edit_Position++;
                                // }
                            // }
                        // }
                        // else
                        // {
                            // input_string[string_pos] = '\0';
                        // }

                        // input_string[string_pos] = '\0';

                        // GUI_EditAnimStage = 0;

                    // }
                // }

            // } break;
                /* CLAUDE */  /* REBUG */  /* asm (Input_Box_Popup 302-388, Setup_Input_Box_Popup 274-360): the ESC branch jumps to the Allowed_Char test (loc_29D1D / loc_2A0F8), so the test is outside the else; the shift loop is mov di, si / ... / dec di / cmp di, [GUI_Edit_Position] / jg; str[si] = 0 and AnimStage = 0 (loc_29DA4 / loc_2A17F) are in the then-branch only, the else-branch stores just str[si] = 0 (loc_29DB0 / loc_2A18B) */
                Allowed_Char = ST_FALSE;
                if(key == ST_KEY_ESCAPE)
                {
                    Control_Change = 2;
                    Allowed_Char = ST_FALSE;
                }
                else
                {
                    if(
                        ((key > 64) && (key < 93))
                        ||
                        ((key > 45) && (key < 59))
                        ||
                        (key == ' ')
                        ||
                        (key == '-')
                        ||
                        ((key > 96) && (key < 123))
                    )
                    {
                        Allowed_Char = ST_TRUE;
                    }
                }
                if(Allowed_Char == ST_TRUE)
                {
                    String_Modified = ST_TRUE;
                    input_string[string_pos] = key;
                    input_string[(string_pos + 1)] = '\0';
                    if(
                        (string_pos < max_characters)
                        &&
                        (Get_String_Width(input_string) <= width)
                    )
                    {
                        input_string[string_pos] = '\0';
                        if(GUI_Edit_Position < string_pos)
                        {
                            for(itr = string_pos; itr > GUI_Edit_Position; itr--)
                            {
                                input_string[itr] = input_string[(itr - 1)];
                            }
                            string_pos++;
                            input_string[GUI_Edit_Position] = key;
                            GUI_Edit_Position++;
                        }
                        else
                        {
                            input_string[string_pos] = key;
                            string_pos++;
                            input_string[string_pos] = ' ';
                            input_string[(string_pos + 1)] = '\0';
                            if(
                                (string_pos < max_characters)
                                &&
                                (Get_String_Width(input_string) <= width)
                            )
                            {
                                GUI_Edit_Position++;
                            }
                        }
                        input_string[string_pos] = '\0';
                        GUI_EditAnimStage = 0;
                    }
                    else
                    {
                        input_string[string_pos] = '\0';
                    }
                }

            } break;
        }

        Draw_Input_Box_Popup(field_num, input_string);

    }  /* while((key != ST_KEY_ENTER) && (Control_Change == ST_FALSE)) */
    /*
        END:  
    */

    /* CLAUDE */  Hw_Textinput_Stop();

    stu_strcpy(p_fields[field_num].string, input_string);

    if(Control_Change != ST_FALSE)
    {

        if(mouse_installed != ST_FALSE)
        {

            // jmp     short $+2

            while(Mouse_Button() != 0) { }

        }

    }

    Restore_Mouse_On_Page();

    Restore_Mouse_State();

    down_mouse_button = ST_UNDEFINED;

    GUI_Edit_Position = 0;

    GUI_EditAnimStage = 0;

}


// WZD s36p32
// drake178: GUI_TextEditDialog()
// MoO2  Module: NAMESTAR  Input_Box_Popup_()
// 1oom  uiobj.c  uiobj_textinput_do()
/*
; clears the GUI, creates an edit box with the passed
; parameters, and transfers control to it until it
; loses focus, at which point the resulting string is
; saved to the passed pointer and the function returns
; -1 if the Esc key was pressed, or 0 otherwise
*/
/*
    returns?
    
NameStartingCity_Dialog_Popup()
    start_x = 60;
    start_y = 30;
    Input_Box_Popup((start_x + 16), (start_y + 21), 75, Text, 12, 0, 0, 0, &color_array[0], ST_UNDEFINED)

*/
int16_t Setup_Input_Box_Popup(int16_t x_start, int16_t y_start, int16_t width, char * string, int16_t max_characters, int16_t fill_color, int16_t justification, int16_t cursor_type, uint8_t colors[], int16_t help)
{
    char input_string[63] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char key = 0;
    int16_t input_field_idx = 0;
    int16_t control_change = 0;
    int16_t timeout_counter = 0;
    int16_t font_height = 0;
    int16_t string_modified = 0;
    int16_t allowed_char = 0;
    int16_t string_pos = 0;
    int16_t itr = 0;

    timeout_counter = 4;

    Clear_Fields();

    /* CLAUDE */  Hw_Textinput_Start();

    if(mouse_installed != ST_FALSE)
    {
        // jmp     short $+2
        while(Mouse_Button() != 0) { }
        Mouse_Buffer();
        Mouse_Buffer2();
    }

    Set_Input_Delay(1);

    input_field_idx = Add_Input_Field(x_start, y_start, width, string, max_characters, fill_color, justification, cursor_type, &colors[0], str_empty_string__seg036, help);

    down_mouse_button = input_field_idx;  // ¿ DEDU:  manually set here so it won't trigger in Draw_Field() ?

    GUI_Edit_Position = 0;
    GUI_EditAnimStage = 0;
    GUI_EditCursorOn = ST_FALSE;

    string_modified = ST_FALSE;

    string_pos = 0;

    key = 0;

    font_height = Get_Font_Height();  // 10

    stu_strcpy(input_string, string);

    while(input_string[string_pos] != '\0') { string_pos++; }

    while((Get_String_Width(input_string) > width) && (string_pos > 0))
    {
        /* OGBUG  `string_pos -= string_pos` (asm `sub si, si`) empties the whole default text on the first pass instead of dropping one character */
        string_pos -= string_pos;
        input_string[string_pos] = 0;
    }

    GUI_Edit_Position = string_pos;

    if(GUI_Edit_Position >= max_characters)
    {
        GUI_Edit_Position = max_characters;
    }

    Draw_Input_Box_Popup(input_field_idx, input_string);


    /*
        BEGIN:  
    */

    control_change = ST_FALSE;

    // 1oom:  while((key != MOO_KEY_RETURN) && (!flag_mouse_button))
    while((key != ST_KEY_ENTER) && (control_change == ST_FALSE))
    {
        while((Keyboard_Status() == 0) && (control_change == ST_FALSE))
        {
            if(GUI_EditTimeOutType == ST_UNDEFINED)
            {
                timeout_counter--;
            }
            
            if(timeout_counter == 0)
            {
                break;
            }

            if(
                (mouse_installed != ST_FALSE)
                &&
                (
                    (Mouse_Button() != 0)
                    ||
                    (Mouse_Buffer() != 0)
                )
            )
            {
                control_change = ST_TRUE;
                break;
            }

            GUI_EditAnimStage++;

            if(((font_height * 2) - 1) < GUI_EditAnimStage)
            {
                GUI_EditAnimStage = 0;
            }

            Draw_Input_Box_Popup(input_field_idx, input_string);

        }

        // if(
        //     (timeout_counter == 0)
        //     ||
        //     (control_change != ST_FALSE)
        // )
        // {
        //     break;
        // }
        /* CLAUDE */  /* REBUG */  /* asm loc_29F92: cmp [bp+Timeout_Counter], 0 / jnz loc_29F9B / jmp loc_2A1AE; loc_29F9B: cmp [bp+Control_Change], ST_FALSE / jz loc_29FA4 / jmp loc_2A1AE - two separate tests, each with its own jump out */
        if(timeout_counter == 0)
        {
            break;
        }
        if(control_change != ST_FALSE)
        {
            break;
        }

        /* HACK */  Platform_Event_Handler();  /* TODO  consolidate 'record key press' effort */
        key = Read_Key();

/*
ST_KEY_LEFT             = 0x01
ST_KEY_RIGHT            = 0x02
ST_KEY_BACKSPACE        = 0x0B
ST_KEY_ENTER            = 0x0C
*/
        /* CLAUDE */  /* REBUG */  /* asm: case bodies are laid out in jump-target order key_10 (BACKSPACE), key_09 (DELETE), key_00 (LEFT), key_01 (RIGHT), then key_02_03_04_05_06_07_08 (default); the cases were reordered to match (bodies moved intact) */
        switch(key)
        {
            case ST_KEY_BACKSPACE:
            {
                if(string_modified == ST_FALSE)
                {
                    input_string[0] = '\0';
                    string_pos = 0;
                    GUI_Edit_Position = 0;
                    GUI_EditAnimStage = 0;
                    string_modified = ST_TRUE;
                }
                else if(string_pos > 0)
                {
                    if(GUI_Edit_Position >= string_pos)
                    {
                        string_pos--;
                        input_string[string_pos] = '\0';
                        GUI_Edit_Position--;
                        GUI_EditAnimStage = 0;
                    }
                    else if(GUI_Edit_Position > 0)
                    {
                        for(itr = GUI_Edit_Position; itr < string_pos; itr++)
                        {
                            input_string[(itr - 1)] = input_string[itr];
                        }
                        string_pos--;
                        GUI_Edit_Position--;
                        GUI_EditAnimStage = 0;
                        input_string[string_pos] = 0;
                    }
                }
            } break;
            case ST_KEY_DELETE:
            {
                if(
                    (string_pos > 0)
                    &&
                    (GUI_Edit_Position < string_pos)
                )
                {
                    for(itr = GUI_Edit_Position; itr < string_pos; itr++)
                    {
                        input_string[(itr)] = input_string[(itr + 1)];
                    }
                    string_pos--;
                    GUI_EditAnimStage = 0;
                    input_string[string_pos] = '\0';
                }
            } break;
            case ST_KEY_LEFT:
            {
                string_modified = ST_TRUE;
                if(GUI_Edit_Position > 0)
                {
                    GUI_Edit_Position--;
                    GUI_EditAnimStage = 0;
                }
            } break;
            case ST_KEY_RIGHT:
            {
                if(
                    (GUI_Edit_Position < max_characters)
                    &&
                    (GUI_Edit_Position < string_pos)
                )
                {
                    GUI_Edit_Position++;
                    GUI_EditAnimStage = 0;
                    if(GUI_Edit_Position == string_pos)
                    {
                        input_string[string_pos] = ' ';
                        input_string[(string_pos + 1)] = '\0';
                        if(Get_String_Width(input_string) > width)
                        {
                            GUI_Edit_Position--;
                        }
                        input_string[string_pos] = '\0';
                    }
                }
            } break;
            default:  /* key_02_03_04_05_06_07_08 */
            {
                allowed_char = ST_FALSE;
                if(key == ST_KEY_ESCAPE)
                {
                    control_change = 2;
                    allowed_char = ST_FALSE;
                }
                else
                {
                    if(
                        ((key > '@') && (key < ']'))
                        ||
                        ((key > '-') && (key < ';'))
                        ||
                        (key == ' ')
                        ||
                        (key == '-')
                        ||
                        ((key > '`') && (key < '{'))
                    )
                    {
                        allowed_char = ST_TRUE;
                    }
                }
                if(allowed_char == ST_TRUE)
                {
                    string_modified = ST_TRUE;
                    input_string[string_pos] = key;
                    input_string[(string_pos + 1)] = '\0';
                    if(
                        (string_pos < max_characters)
                        &&
                        (Get_String_Width(input_string) <= width)
                    )
                    {
                        input_string[string_pos] = '\0';
                        if(GUI_Edit_Position < string_pos)
                        {
                            for(itr = string_pos; itr > GUI_Edit_Position; itr--)
                            {
                                input_string[itr] = input_string[(itr - 1)];
                            }
                            string_pos++;
                            input_string[GUI_Edit_Position] = key;
                            GUI_Edit_Position++;
                        }
                        else
                        {
                            input_string[string_pos] = key;
                            string_pos++;
                            input_string[string_pos] = ' ';
                            input_string[(string_pos + 1)] = '\0';
                            if(
                                (string_pos < max_characters)
                                &&
                                (Get_String_Width(input_string) <= width)
                            )
                            {
                                GUI_Edit_Position++;
                            }
                        }
                        input_string[string_pos] = '\0';
                        GUI_EditAnimStage = 0;
                    }
                    else
                    {
                        input_string[string_pos] = '\0';
                    }
                }

            } break;
        }

        Draw_Input_Box_Popup(input_field_idx, input_string);
    }  /* while((key != ST_KEY_ENTER) && (control_change == ST_FALSE)) */
    /*
        END:  
    */


    /* CLAUDE */  Hw_Textinput_Stop();

    stu_strcpy(string, input_string);

    if(control_change != ST_FALSE)
    {
        if(mouse_installed != ST_FALSE)
        {
            // jmp     short $+2

            while(Mouse_Button() != 0) { }
        }
    }

    down_mouse_button = ST_UNDEFINED;

    Clear_Fields();

    Mouse_Buffer();
    Mouse_Buffer2();

    GUI_Edit_Position = 0;
    GUI_EditAnimStage = 0;

    if(control_change == 2)
    {
        return ST_SUCCESS;
    }
    else
    {
        return ST_FAILURE;
    }

}


// WZD s36p33
// drake178: GUI_EditBoxRedraw()
// MoO2  Module: NAMESTAR  Draw_Input_Box_Popup_()
// 1oom  uiobj.c  uiobj_handle_t4_sub2()
/*
; calls any active redraw function, draws the selected
; edit box with the passed text but original control
; parameters over it, then performs a simple page flip
; redrawing all other controls in the process
*/
/*
*/
void Draw_Input_Box_Popup(int16_t field_num, char * string)
{
    uint8_t ibeam_color_array[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char input_string[62] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Box_Width = 0;
    int16_t Cursor_Bottom = 0;
    int16_t Cursor_Height = 0;
    int16_t cursor_x1 = 0;
    int16_t Cursor_Width = 0;
    int16_t Lines_Drawn = 0;
    char Edit_Char[2] = { 0, 0 };
    int16_t anim_stage = 0;  // _DI_

    Mark_Time();

    Set_Page_Off();

    // ; nonstandard memcpy, copies an amount of bytes from one near memory location to another
    // ; fully replacable with other, already included code
    // TODO  Copy_Memory_Near(ibeam_color_array, p_fields[field_num].color_array, Get_Font_Height());  // ; ~ ibeam / edit cursor height
    memcpy(ibeam_color_array, p_fields[field_num].color_array, Get_Font_Height());  // ; ~ ibeam / edit cursor height

    stu_strcpy(input_string, string);

    Invoke_Auto_Function();

    /* OGBUG  (quirk) Box_Width is computed and never read */
    Box_Width = (p_fields[field_num].x2 - p_fields[field_num].x1);


    Set_Font_Style(p_fields[field_num].font_style_num, p_fields[field_num].font_normal_color, p_fields[field_num].font_highlight_color, ST_NULL);

    Cursor_Height = (Get_Font_Height() - 1);

    if(p_fields[field_num].fill_color != 0)
    {
        Fill(p_fields[field_num].x1, p_fields[field_num].y1, p_fields[field_num].x2, p_fields[field_num].y2, (uint8_t)p_fields[field_num].fill_color);
    }

    Edit_Char[1] = '\0';
    
    if(p_fields[field_num].cursor_type == crsr_None)
    {
        Edit_Char[0] = input_string[GUI_Edit_Position];

        input_string[GUI_Edit_Position] = '\0';

        cursor_x1 = (p_fields[field_num].x1 + Get_String_Width(input_string));

        input_string[GUI_Edit_Position] = Edit_Char[0];

        if(Edit_Char[0] == '\0')
        {
            Edit_Char[0] = ' ';
        }
        
        Cursor_Width = Get_String_Width(Edit_Char);

        if(
            (GUI_EditAnimStage > 0)
            &&
            (GUI_EditAnimStage <= Cursor_Height)
        )
        {
            // draw cursor lines up to down in ascending order
            anim_stage = GUI_EditAnimStage;

            Cursor_Bottom = (p_fields[field_num].y1 + Cursor_Height);

            Lines_Drawn = 0;

            while(anim_stage > 0)
            {
                Line(cursor_x1, (Cursor_Bottom - anim_stage + 1), (cursor_x1 + Cursor_Width + 1), (Cursor_Bottom - anim_stage + 1), ibeam_color_array[(1 + Lines_Drawn)]);
                Lines_Drawn++;
                anim_stage--;
            }
        }
        else if(GUI_EditAnimStage != 0)
        {
            // draw cursor lines down to up in descending order
            anim_stage = (Cursor_Height - (GUI_EditAnimStage - Cursor_Height));

            Lines_Drawn = 0;

            while(anim_stage > 0)
            {
                Line(cursor_x1, ((p_fields[field_num].y1 + anim_stage) - 1), ((cursor_x1 + Cursor_Width) + 1), ((p_fields[field_num].y1 + anim_stage) - 1), ibeam_color_array[(Cursor_Height - Lines_Drawn)]);
                Lines_Drawn++;
                anim_stage--;
            }
        }
    }
    else if(p_fields[field_num].cursor_type == crsr_Finger)
    {
        Edit_Char[0] = input_string[GUI_Edit_Position];
        input_string[GUI_Edit_Position] = '\0';
        cursor_x1 = (p_fields[field_num].x1 + Get_String_Width(input_string));
        input_string[GUI_Edit_Position] = Edit_Char[0];
        if(Edit_Char[0] == '\0')
        {
            Edit_Char[0] = ' ';
        }
        Cursor_Width = Get_String_Width(Edit_Char);
        anim_stage = (GUI_EditAnimStage % 3);

        if(
            (anim_stage == 0)
            &&
            (GUI_EditCursorOn == ST_FALSE)
        )
        {
            GUI_EditCursorOn = ST_TRUE;
        }
        else if(
            (anim_stage == 0)
            &&
            (GUI_EditCursorOn == ST_TRUE)
        )
        {
            GUI_EditCursorOn = ST_FALSE;
        }

        Cursor_Bottom = (p_fields[field_num].y1 + Cursor_Height);

        if(GUI_EditCursorOn != ST_FALSE)
        {
            Line(cursor_x1, (Cursor_Bottom + 1), (cursor_x1 + Cursor_Width + 1), (Cursor_Bottom + 1), ibeam_color_array[0]);
        }
    }

    // 4, 4, 3, 0
    Set_Font_Style(p_fields[field_num].font_style_num, p_fields[field_num].font_normal_color, p_fields[field_num].font_highlight_color, ST_NULL);

    Set_Highlight_Colors_On();
    
    Print(p_fields[field_num].x1, p_fields[field_num].y1, input_string);

    /* HACK */ Set_Normal_Colors_On();

    Apply_Palette();

    Toggle_Pages();

    Release_Time(auto_function_delay);

}


// WZD s36p34
// drake178: GUI_CreateEditBox()
// Input.c  Add_Input_Field()


// WZD s36p35
//...
// ...
// ...
// WZD s36p64


// WZD s36p65
void Init_Mouse_Keyboard(int16_t input_type)
{

    // p_fields = (struct s_Field *)Allocate_Space(357);  // 357 PR, 5712 B  (¿ 150 * 38 = 5700 ¿ + 12 ?)
    p_fields = (struct s_Field *)Allocate_Space((((150 * sizeof(struct s_Field)) / 16) + 1));  // 357 PR, 5712 B  (¿ 150 * 38 = 5700 ¿ + 12 ?)

    switch(input_type)
    {
        case 0:
        {
            Set_Mouse_Usable();
            mouse_installed = ST_FALSE;
            emulated_pointer_x = 158;
            emulated_pointer_y = 100;
            Set_Pointer_Position(158, 100);
        } break;
        case 1:
        {
            Set_Mouse_List(1, mouse_list_init);
            mouse_installed = Init_Mouse_Driver();
            if(mouse_installed == ST_FAILURE)
            {
                Set_Mouse_Usable();
                emulated_pointer_x = 158;
                emulated_pointer_y = 100;
                Set_Pointer_Position(158, 100);
            }
            else
            {
                mouse_installed = ST_TRUE;
            }
        } break;
        case 2:
        {
            Set_Mouse_Usable();
            mouse_installed = ST_FALSE;
            emulated_pointer_x = 158;
            emulated_pointer_y = 100;
            Set_Pointer_Position(158, 100);
        } break;
    }

    /* HACK */  /* CLAUDE */  /* no asm counterpart: ReMoM always runs with a pointer, whatever input_type (MAGIC.SET / CONFIG.MOM) says */
    Set_Mouse_List(1, mouse_list_init);
    Init_Mouse_Driver();
    mouse_installed = ST_TRUE;

    Save_Mouse_On_Page_(158, 100);
    input_delay = 0;
    down_mouse_button = ST_UNDEFINED;
    _global_esc = ST_FALSE;  // TODO  fixup the mixup of _global_esc with mouse_cancel_disabled
    mouse_cancel_disabled = ST_FALSE;
    Clear_Fields();

}



// WZD s36p66
/*
    returns field_idx/num
*/
int16_t Get_Input(void)
{
    int16_t field_index = 0;

    Platform_Event_Handler();

/* CLAUDE: disabled redundant `Pump_Events()` here.
 * Platform_Event_Handler() on the win32 backend already calls
 * Win_Pump_Messages() internally, so this second pump did nothing useful.
 * Worse, it created a window where keystrokes arriving between the two pumps
 * landed in the keyboard buffer AFTER Replay_Capture_Frame() had already
 * snapshotted, then were consumed by the popup's next Read_Key() — meaning
 * those keys were never recorded into the .RMR.  Symptom: typing
 * "HomeBoi" into a city-name popup recorded only the trailing 'i'. */
// #ifdef _STU_WIN
//     Pump_Events();
// #endif

    if(input_delay > 0)
    {
        input_delay--;
        return 0;
    }

    if(input_delay < 0)
    {
        input_delay = 0;
    }

    if(fields_count <= 1)
    {
        return 0;
    }

    if(mouse_installed != ST_FALSE)
    {
        field_index = Interpret_Mouse_Input();  // WZD s36p01
    }
    else
    {
        field_index = Interpret_Keyboard_Only_Input();  // WZD s36p21
    }

    Set_Page_Off();

#ifdef MOUSE_DEBUG
    if(field_index != 0)
    {
        int16_t fi = (field_index > 0) ? field_index : -field_index;
        if(fi > 0 && fi < fields_count)
        {
            MOUSE_LOG("SCR t=%llu GET_INPUT field=%d type=%d rect=(%d,%d)-(%d,%d)\n", (unsigned long long)Platform_Get_Millies(), field_index, p_fields[fi].type, p_fields[fi].x1, p_fields[fi].y1, p_fields[fi].x2, p_fields[fi].y2);
        }
        else
        {
            MOUSE_LOG("SCR t=%llu GET_INPUT field=%d\n", (unsigned long long)Platform_Get_Millies(), field_index);
        }
    }
#endif

    return field_index;

}



// WZD s36p67
void Set_Input_Delay(int16_t delay_count)
{
    input_delay = delay_count;
    Mouse_Buffer();
    Mouse_Buffer2();
}


// WZD s36p68
int16_t Get_Input_Delay(void)
{
    return input_delay;
}


// WZD s36p69
// UU_GUI_ClearSelectable();
// drake178: UU_GUI_ClearSelectable()
/*
    makes a string list field unselectable (the active_flag of Add_String_List_Field()); any other field type is left alone; no caller in WIZARDS or MAGIC
*/
void Disable_String_List_Field(int16_t field_num)
{
    if(p_fields[field_num].type == ft_StringList)
    {
        p_fields[field_num].active_flag = ST_FALSE;
    }
}


// WZD s36p70
// UU_GUI_SetSelectable();
// drake178: UU_GUI_SetSelectable()
/*
    makes a string list field selectable again (the active_flag of Add_String_List_Field()); any other field type is left alone; no caller in WIZARDS or MAGIC
*/
void Enable_String_List_Field(int16_t field_num)
{
    if(p_fields[field_num].type == ft_StringList)
    {
        p_fields[field_num].active_flag = ST_TRUE;
    }
}


// WZD s36p85
// MoO2  Module: video  Toggle_Pages()  &&  Module: MOX2  _TOGGLE_PAGES_()
/*
¿ how to explain ?
Elsewhere, `Toggle_Pages_No_Draw_Fields_Pre(); ... GUI_PageFlip();`
same procedure, except no Draw_Fields() call
*/
void Toggle_Pages(void)
{
    int16_t mouse_x;
    int16_t mouse_y;

    Save_Mouse_State();

    mouse_x = Pointer_X();
    mouse_y = Pointer_Y();

    Draw_Fields();  // MoO2  Draw_Visible_Fields() ...if(draw_fields_flag != ST_FALSE)

    Check_Mouse_Shape(mouse_x, mouse_y);

    Save_Mouse_Off_Page(mouse_x, mouse_y);  // video_page_buffer[1 - draw_page_num] -> mouse_off_page_buffer
    Draw_Mouse_Off_Page(mouse_x, mouse_y);  // mouse_palette -> video_page_buffer[1 - draw_page_num]

    Page_Flip();                            // draw_page_num = 1 - draw_page_num
    
    Restore_Mouse_Off_Page();               // mouse_background_buffer -> video_page_buffer[1 - draw_page_num]
    Copy_Mouse_Off_To_Mouse_Back();         // mouse_off_page_buffer -> mouse_background_buffer

    Restore_Mouse_State();

}
