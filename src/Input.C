/*
    WIZARDS.EXE
        seg036
*/
#include "MoX.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
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



// DELETE  uint8_t g_Key_Pressed = ST_FALSE;   // Platform - Decl in MoM.hpp
// DELETE  uint16_t g_Last_Key_Pressed = 0;    // Platform - Decl in MoM.hpp
// DELETE  uint16_t scan_code_char_code = 0;   // Platform - Decl in MoM.hpp



/*
    MGC  seg033
*/

// TODO  // MGC s33p01
// TODO  Set_Mouse_List()

// TODO  // MGC s33p02
// TODO  Check_Mouse_Shape()

// TODO  // MGC s33p03

// TODO  // MGC s33p04
// TODO  // Get_Pointer_Offset()

// TODO  // MGC s33p07
// TODO  // MD_Init()

// TODO  // MGC s33p09
// TODO  // MD_Reset()

// TODO  // MGC s33p09
// TODO  // MD_Button_Hanlder()

// TODO  // MGC s33p11
// TODO  // MD_Draw_Disable()
// TODO  // MGC s33p12
// TODO  // MD_Draw_Restore()



/*
    WZD  seg036
    MGC  seg034
*/





void Handle_Key_Press(void)
{

}

void Handle_Right_Click(void)
{

}

void Handle_Left_Click(void)
{
    
}


//             // HACK: 
//             if(MD_ButtonStatus == MOUSE_BUTTON_LEFT)
//             {
// #ifdef STU_DEBUG
//                 DLOG("if(MD_ButtonStatus == MOUSE_BUTTON_LEFT)");
//                 DBG_In_MouseButtonLeft = ST_TRUE;
// #endif
//                 mouse_x = Pointer_X();
//                 mouse_y = Pointer_Y();
//                 pointer_offset = Get_Pointer_Offset();
//                 field_num = 0;
//                 character = 0;
//                 field_num = Scan_Field();
//                 if(field_num != 0)
//                 {
//                     field_num = field_num;
//                 }
// #ifdef STU_DEBUG
//                 dbg_prn("DEBUG: [%s, %d]: Left-Click field_num: %d\n", __FILE__, __LINE__, field_num);
//                 DBG_In_MouseButtonLeft = ST_FALSE;
// #endif
//             }


//             // HACK: 
//             if(MD_ButtonStatus == MOUSE_BUTTON_RIGHT)
//             {
// #ifdef STU_DEBUG
//                 DLOG("if(MD_ButtonStatus == MOUSE_BUTTON_RIGHT)");
//                 DBG_In_MouseButtonRight = ST_TRUE;
// #endif
//                 mouse_x = Pointer_X();
//                 mouse_y = Pointer_Y();
//                 pointer_offset = Get_Pointer_Offset();
//                 field_num = 0;
//                 character = 0;
//                 field_num = Scan_Field();
//                 if(field_num != 0)
//                 {
//                     field_num = (field_num * -1);
//                 }
// #ifdef STU_DEBUG
//                 dbg_prn("DEBUG: [%s, %d]: Right-Click field_num: %d\n", __FILE__, __LINE__, field_num);
//                 DBG_In_MouseButtonRight = ST_FALSE;
// #endif
//             }


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
    int16_t field_num;
    int16_t character;
    int16_t mx;
    int16_t my;
    int16_t pointer_offset;
    int16_t down_mouse_button;
    int16_t mouse_field;
    int16_t MD_ButtonStatus;
    uint16_t itr_continuous_string;
    int16_t Accepted_Char;
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
    int16_t alt_field_num;  // _SI_
    int16_t scanned_field;  // _SI_
    int16_t return_value;  // _AX_  MoO2: return _EAX_ = return_value = maybe__field_num__scanned_field = itr_fields_count

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
    MD_ButtonStatus = 0;
    mouse_field = 0;

    mx = Pointer_X();
    my = Pointer_Y();
    pointer_offset = Get_Pointer_Offset();



    if(Keyboard_Status() == ST_TRUE)
    {
        character = Interpret_Keyboard_Input(&field_num);

        if(character == 0)
        {
            goto Return_Type_Z01;
        }


        if(character == ST_KEY_F11)
        {
            F11_Key__WIP();
        }
        
        if(character == ST_KEY_F12)
        {
            Save_Mouse_State();
            Restore_Mouse_On_Page();
            Screen_Flic_Capture__STUB();
            Save_Mouse_On_Page(Pointer_X(), Pointer_Y());
            Draw_Mouse_On_Page(Pointer_X(), Pointer_Y());
            Set_Pointer_Position(Pointer_X(), Pointer_Y());
            Restore_Mouse_State();
        }

        if((character == ST_KEY_ESCAPE) && (mouse_cancel_disabled != ST_FALSE))
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
            if((field_num != 0) && (p_fields[field_num].hotkey == character))
            {

                if(p_fields[field_num].type == ft_Scroll)
                {
                    goto Return_Type_Z01;
                }

                if(field_num != 0)
                {
                    mx = (p_fields[field_num].x1 + ((p_fields[field_num].x2 - p_fields[field_num].x1) / 2));
                    my = (p_fields[field_num].y1 + ((p_fields[field_num].y2 - p_fields[field_num].y1) / 2));
                    Push_Field_Down(field_num, mx, my);
                    Mouse_Button_Handler();

                    switch(p_fields[field_num].type)
                    {
                        case ft_RadioButton:
                        {
                            // toggle unset/set state
                            if(p_fields[field_num].state == 0)
                            {
                                p_fields[field_num].state = 1;
                            }
                            else
                            {
                                p_fields[field_num].state = 0;
                            }
                        } break;
                        case ft_LockedButton:
                        {
                            // if not locked, then lock  (¿ no unlock ?)
                            if(p_fields[field_num].state == 0)
                            {
                                p_fields[field_num].state = 1;
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
                    strcpy((char *)p_fields[alt_field_num].string, continuous_string);
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
                            strcpy(continuous_string, p_fields[alt_field_num].string);
                            GUI_EditAnimStage = 0;
                            GUI_EditCursorOn = ST_FALSE;
                            input_field_active = ST_TRUE;
                            active_input_field_number = alt_field_num;
                            // jmp     @@IDK_KD_Enter_PostOp
                        }
                        else  /* input_field_active == ST_TRUE */
                        {
                            // ¿ UNREACHABLE ?
                            __debugbreak();
                            if(active_input_field_number == alt_field_num)
                            {
                                itr_continuous_string = 0;
                                while((continuous_string[itr_continuous_string] != '\0') && (p_fields[alt_field_num].max_characters > itr_continuous_string)) { itr_continuous_string++; }
                                if(continuous_string[(itr_continuous_string - 1)] == '_')
                                {
                                    itr_continuous_string--;
                                }
                                continuous_string[itr_continuous_string] = 0;
                                GUI_EditAnimStage = 0;
                                active_input_field_number = ST_UNDEFINED;
                                strcpy(p_fields[alt_field_num].string, continuous_string);
                                // @@IDK_KD_Enter_PostOp
                            }
                            else
                            {
                                strcpy(continuous_string, p_fields[alt_field_num].string);
                                GUI_EditAnimStage = 0;  // cursor_count
                                GUI_EditCursorOn = 0;   // cursor_on
                                active_input_field_number = alt_field_num;
                                // @@IDK_KD_Enter_PostOp:
                            }
                        }
                    }
                    else
                    {
                        if(input_field_active != 0)
                        {
                            __debugbreak();
                            // UNREACHABLE  if(p_fields[alt_field_num].type == ft_ContinuousStringInput)
                            // UNREACHABLE  {
                            // UNREACHABLE      itr_continuous_string = 0;
                            // UNREACHABLE      while((continuous_string[itr_continuous_string] != '\0') && (p_fields[alt_field_num].max_characters > itr_continuous_string)) { itr_continuous_string++; }
                            // UNREACHABLE      if(continuous_string[(itr_continuous_string - 1)] == '_')
                            // UNREACHABLE      {
                            // UNREACHABLE          itr_continuous_string--;
                            // UNREACHABLE      }
                            // UNREACHABLE      continuous_string[itr_continuous_string] = 0;
                            // UNREACHABLE      strcpy(p_fields[alt_field_num].string, continuous_string);
                            // UNREACHABLE      GUI_EditAnimStage = 0;
                            // UNREACHABLE      input_field_active = ST_FALSE;
                            // UNREACHABLE      active_input_field_number = ST_UNDEFINED;
                            // UNREACHABLE  }
                        }
                    }
                }

                // @@IDK_KD_Enter__New_Block:
                // IDA foggy hunter green #36
                if(alt_field_num == 0)
                {
                    if(GUI_DialogDirections != 0)
                    {
                        for(alt_field_num = 1; alt_field_num < fields_count; alt_field_num++)
                        {
                            // if State@ == Trig_State
                            if(
                                (p_fields[alt_field_num].type == ft_StringList)
                                &&
                                (p_fields[alt_field_num].Param2 == p_fields[alt_field_num].Param1)
                                &&
                                (p_fields[alt_field_num].Selectable != ST_FALSE)
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
                        Push_Field_Down(alt_field_num, mouse_x, mouse_y);
                    }

                    Mouse_Button_Handler();

                    switch(p_fields[alt_field_num].type)
                    {
                        case ft_RadioButton:
                        {
                            if(p_fields[alt_field_num].state == 0)
                            {
                                p_fields[alt_field_num].state = 1;
                            }
                            else
                            {
                                p_fields[alt_field_num].state = 0;
                            }
                        } break;
                        case ft_LockedButton:
                        {
                            if(p_fields[alt_field_num].state == 0)
                            {
                                p_fields[alt_field_num].state = 1;
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
            if((character == '+' || character == '-') && (alt_field_num = Scan_Field() != 0) && (p_fields[alt_field_num].type == ft_Scroll))
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
                while( (continuous_string[alt_field_num] != '\0') && (continuous_string[alt_field_num] != '_') && ((p_fields[active_input_field_number].max_characters - 1) > alt_field_num))
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
                    while( (continuous_string[alt_field_num] != '_') && (continuous_string[alt_field_num] != '\0') && ((p_fields[active_input_field_number].max_characters - 1) > alt_field_num))
                    {
                        alt_field_num++;
                    }

                    if(((p_fields[active_input_field_number].max_characters - 1) > alt_field_num))
                    {
                        continuous_string[alt_field_num] = character;
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
    else
    {
        Mouse_Movement_Handler();
        if(Mouse_Button() != ST_FALSE)
        {
            MD_ButtonStatus = Mouse_Button();

            // if(MD_ButtonStatus = ST_RIGHTBUTTON) { if(help_list_active == ST_TRUE && Check_Help_List() { MD_Get_ClickRec1(); MD_Get_ClickRec2(); return 0; } else { if(mouse_cancel_disabled == ST_FALSE) { while(Mouse_Button = ST_RIGHTBUTTON){ Quick_Call_Auto_Function()} return ST_UNDEFINED; } } }

            // IDA:  @@Loop_MouseButton  bright green #11
            // Begin Block: Loop Mouse_Button()
            // TODO  make this be the loop that it looks like - need some Input Field that makes use of the feature  e.g., Magic Screen Power Distribution Staves
            while(Mouse_Button() != 0)
            {

                mouse_x = Pointer_X();
                mouse_y = Pointer_Y();
                pointer_offset = Get_Pointer_Offset();
                field_num = 0;
                // Unused_Local == ST_UNDEFINED
                character = 0;
                field_num = Scan_Field();

                /*
                    BEGIN BLOCK: field_num = Scan_Field() != 0
                */
                if(field_num != 0)
                {
                    
                    // Begin Block: field_num = Scan_Field() != 0 && != down_mouse_button
                    // Begin Block: Push_Field_Down()
                    {
                        if(
                            (field_num != down_mouse_button) &&
                            (p_fields[field_num].type != ft_Input) &&
                            (p_fields[field_num].type != ft_ContinuousStringInput) &&
                            !( (down_mouse_button != ST_UNDEFINED) && (p_fields[field_num].type == ft_Grid) && (p_fields[down_mouse_button].type == ft_Grid) )
                        )
                        {
                            if(p_fields[down_mouse_button].type == ft_Scroll)
                            {
                                Invoke_Auto_Function();
                            }

                            Push_Field_Down(field_num, mouse_x, mouse_y);
                        }
                    }
                    // End Block: Push_Field_Down()
                    // End Block: field_num = Scan_Field() != 0 && != down_mouse_button

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
                    if(
                        (p_fields[field_num].type != ft_ContinuousStringInput)
                        &&
                        (input_field_active != ST_FALSE)
                    )
                    {
                        // `Exit Edit-State`
                        itr_continuous_string = 0;
                        while( (continuous_string[itr_continuous_string] != '\0') && (p_fields[active_input_field_number].max_characters < itr_continuous_string) )
                        {
                            itr_continuous_string++;
                        }
                        if(continuous_string[(itr_continuous_string - 1)] == '_')
                        {
                            itr_continuous_string--;
                        }
                        continuous_string[itr_continuous_string] = '\0';
                        strcpy((char *)p_fields[active_input_field_number].string, continuous_string);
                        input_field_active = ST_FALSE;
                        active_input_field_number = ST_UNDEFINED;
                    }
                    else if(
                        (p_fields[field_num].type == ft_ContinuousStringInput)
                        &&
                        (input_field_active == ST_FALSE)
                    )
                    {
                        strcpy(continuous_string, p_fields[field_num].string);
                        GUI_EditAnimStage = 0;
                        GUI_EditCursorOn = 0;
                        input_field_active = ST_TRUE;
                        active_input_field_number= field_num;
                    }
                    else if(
                        (p_fields[field_num].type == ft_ContinuousStringInput)
                        &&
                        (input_field_active != ST_FALSE)
                        &&
                        (field_num != active_input_field_number)
                    )
                    {
                        // `Exit Edit-State`
                        // `Enter Edit-State`
                        strcpy(continuous_string, p_fields[field_num].string);
                        GUI_EditAnimStage = 0;
                        GUI_EditCursorOn = 0;
                        input_field_active = ST_TRUE;
                        active_input_field_number = field_num;
                    }
                    /*
                        END BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State
                    */

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
                else
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
                            Save_Mouse_On_Page(mouse_x, mouse_y);
                            Draw_Mouse_On_Page(mouse_x, mouse_y);
                            Set_Pointer_Position(mouse_x, mouse_y);
                            Restore_Mouse_State();
                        }
                        down_mouse_button = ST_UNDEFINED;
                    }
                    Set_Buffer_2(mouse_x, mouse_y);
                }
            }
            /*
                END BLOCK: Loop Mouse_Button()
            */
            // IDA: @@IDK_After_Loop_GetButtonStatus


            if(p_fields[auto_input_variable].type == ft_Scroll)
            {
                Invoke_Auto_Function();
            }

            auto_input_variable = 0;


            if(field_num != 0)
            {
                // TODO  MD_Get_ClickRec1();
                // TODO  MD_Get_ClickRec2();
                // TODO  GUI_Processed_LastX = mouse_x;
                // TODO  GUI_Processed_LastY = mouse_y;
                // TODO  GUI_Processed_Btns = MD_ButtonStatus;

                // TODO says switch 9 cases, but only actually handles 4 - figure out which, including the odd (type - 1) part
                // the (type - 1) part is a compiler artifact; it subtracts whatever amount to make the first case be case 0
                // here, that means there was no case for type 0 ft_Button
                switch(p_fields[field_num].type)
                {
                    case ft_Button:
                    {
                        // DNE in Dasm
                    } break;

                    /*  1  0x01 */  // drake178: ToggleButton
                    case ft_RadioButton:
                    {

                    } break;

                    /*  2  0x02 */  // drake178: LockableButton
                    case ft_LockedButton:
                    {

                    } break;

                    /*  3  0x03 */  // drake178: MStateButton
                    case ft_MultiButton:
                    {

                    } break;

                    /*  4  0x04 */  // drake178: EditBox
                    case ft_Input:
                    {

                    } break;

                    /*  5  0x05 */  // drake178: ImageLabel      DNE/NIU in MoO2
                    case ft_Picture:
                    {

                    } break;

                    /*  6  0x06 */  // drake178: SlideBar
                    case ft_Scroll:
                    {

                    } break;

                    /*  7  0x07 */  // drake178: Label
                    case ft_HotKey:
                    {

                    } break;

                    /*  8  0x08 */  // drake178: Ctrl_AltString
                    case ft_MultiHotKey:
                    {

                    } break;
                }

            }


        }

        // IDA: @@EndOfTheClickRoad

        Mouse_Button_Handler();
        down_mouse_button = ST_UNDEFINED;
        switch(MD_ButtonStatus)
        {
            case 0: { field_num =          0; goto Done; } break;
            case 1: { field_num =  field_num; goto Done; } break;
            case 2: { field_num = -field_num; goto Done; } break;
        }        

    }
    // else if(MD_Get_ClickRec1()
    // ¿ ...else... Mouse_Button_Handler(); field_num = 0;


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
Return_Type_F00:
    return field_num;

/*
down_mouse_button = ST_UNDEFINED; return field_num;
*/
Return_Type_F10:
    down_mouse_button = ST_UNDEFINED;
    return field_num;

/*
Mouse_Button_Handler(); return field_num;
*/
Return_Type_F01:
    Mouse_Button_Handler();
    return field_num;

/*
down_mouse_button = ST_UNDEFINED; Mouse_Button_Handler(); return field_num;
*/
Return_Type_F11:
    down_mouse_button = ST_UNDEFINED;
    Mouse_Button_Handler();
    return field_num;

/*
Mouse_Button_Handler(); down_mouse_button == ST_UNDEFINED; return alt_field_num;
*/
Return_Type_4:
    Mouse_Button_Handler();
    down_mouse_button = ST_UNDEFINED;
    return alt_field_num;

/*
down_mouse_button == ST_UNDEFINED; return alt_field_num;
*/
Return_Type_5:
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


// WZD s36p08
void Set_Global_ESC(void)
{
    _global_esc = ST_TRUE;
}



// WZD s36p12
/*
function (0 bytes) Set_Help_List
Address: 01:001196B8
Return type: void (1 bytes) 
pointer (4 bytes) help_pointer
signed integer (2 bytes) count
*/
// // // void Set_Help_List(struct s_HELP_FIELD * help_pointer, int16_t count)
// // // void Set_Help_List(struct s_HELP_FIELD * help_pointer[], int16_t count)
// // void Set_Help_List(struct s_HELP_FIELD ** help_pointer, int16_t count)
// void Set_Help_List(int16_t * help_pointer, int16_t count)
void Set_Help_List(char * help_pointer, int16_t count)
{
    help_struct_pointer = (struct s_HELP_FIELD *)help_pointer;
    help_list_count = count;
    help_list_active = ST_TRUE;
}

// WZD s36p13
void Deactivate_Help_List(void)
{
    help_list_active = ST_FALSE;
    help_list_count = 0;
    help_struct_pointer = ST_NULL;
}

// WZD s36p14
// CAUTION: returns ZERO on SUCCESS
int16_t Check_Help_List__STUB(void)
{
    int16_t help_entry_found;

    help_entry_found = ST_FALSE;

    return help_entry_found;
}

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
void UU_GUI_MouseEMUMoveTo(int16_t field_idx)
{
    int16_t ymid; 
    int16_t xmid;  // _DI_

    if(mouse_installed)
    {
        xmid = (p_fields[field_idx].x1 + ((p_fields[field_idx].x2 - p_fields[field_idx].x1) / 2));
        ymid = (p_fields[field_idx].y1 + ((p_fields[field_idx].y2 - p_fields[field_idx].y1) / 2));

        if((ymid < 0) || (ymid >= SCREEN_HEIGHT) || (xmid < 0) || (xmid >= SCREEN_WIDTH))
        {
            return;
        }

        MOUSE_Emu_X = xmid;
        MOUSE_Emu_Y = ymid;
        Check_Mouse_Shape(MOUSE_Emu_X, MOUSE_Emu_Y);
        pointer_offset = Get_Pointer_Offset();
        MOUSE_Emu_X = (MOUSE_Emu_X - pointer_offset);
        MOUSE_Emu_Y = (MOUSE_Emu_Y - pointer_offset);
        Set_Pointer_Position(MOUSE_Emu_X, MOUSE_Emu_Y);
        Restore_Mouse_On_Page();
        Save_Mouse_On_Page(MOUSE_Emu_X, MOUSE_Emu_Y);
        Draw_Mouse_On_Page(MOUSE_Emu_X, MOUSE_Emu_Y);
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
    int16_t mx;
    int16_t my;
    int16_t itr;
    int16_t current_field;

    mx = Pointer_X();
    my = Pointer_Y();

    current_field = 0;

    Check_Mouse_Shape(mx, my);
    pointer_offset = Get_Pointer_Offset();

    current_field = Scan_Field();

    if(p_fields[current_field].type == ft_ClickLink)
    {
        // DONT  *p_fields[current_field].Param2 = p_fields[current_field].Param1;
        // DONT  return = p_fields[current_field].Param0;
    }

    if(
        (p_fields[current_field].type == ft_StringList)
        &&
        (p_fields[current_field].Selectable == ST_FALSE)
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
        *((int64_t *)p_fields[current_field].Param3) = ((Pointer_X() - p_fields[current_field].x1) / p_fields[current_field].Param1);
        *((int64_t *)p_fields[current_field].Param4) = ((Pointer_Y() - p_fields[current_field].y1) / p_fields[current_field].Param2);
    }

    return current_field;
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
    char charcode;
    int16_t keyboard_flag;
    int16_t mouse_button_flag;

    Reset_Wait_For_Input();

    keyboard_flag = ST_FALSE;
    mouse_button_flag = ST_FALSE;

    temp_field_count = fields_count;

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
                charcode = Read_Key();
                if(charcode == ST_KEY_F11)
                {
                    // TODO  DBG_Quit();
                }
                else if(charcode == ST_KEY_F12)
                {
                    Save_Mouse_State();
                    Restore_Mouse_On_Page();
                    // TODO  DBG_ScreenDump();
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
                charcode = Read_Key();
                if(charcode == ST_KEY_F11)
                {
                    // TODO  DBG_Quit();
                }
                else if(charcode == ST_KEY_F12)
                {
                    Save_Mouse_State();
                    Restore_Mouse_On_Page();
                    // TODO  DBG_ScreenDump();
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
// drake178: GUI_ProcessDirKey()
/*
    L,R,U,D,W,X,Y,Z
*/
int16_t Process_Direction_Key__STUB(int16_t dir_key)
{
    int16_t Y_Direction;
    int16_t X_Direction;
    int16_t Mouse_Cursor_At;
    int16_t Mouse_Y;
    int16_t Mouse_X;
    int16_t field_num;  // _SI_
    int16_t itr;  // _DI_

    switch(dir_key)
    {
        case 'L': { X_Direction = -1; Y_Direction =  0; } break;
        case 'R': { X_Direction =  1; Y_Direction =  0; } break;
        case 'U': { X_Direction =  0; Y_Direction = -1; } break;
        case 'D': { X_Direction =  0; Y_Direction =  1; } break;
        case 'W': { X_Direction = -1; Y_Direction = -1; } break;
        case 'X': { X_Direction =  1; Y_Direction = -1; } break;
        case 'Y': { X_Direction = -1; Y_Direction =  1; } break;
        case 'Z': { X_Direction =  1; Y_Direction =  1; } break;
    }

    if(
        (GUI_DialogDirections != 0)
        &&
        (Y_Direction != 0)
    )
    {
        field_num = 0;
        field_num = GUI_MoveDlgHighlight__STUB(Y_Direction);
        return field_num;
    }

    if(mouse_installed == ST_FALSE)
    {
        Mouse_X = MOUSE_Emu_X;
        Mouse_Y = MOUSE_Emu_Y;
    }
    else
    {
        Mouse_X = Pointer_X();
        Mouse_Y = Pointer_Y();
    }

    Mouse_Cursor_At = 0;

    for(itr = (fields_count - 1); itr > 0; itr--)
    {



    }

    field_num = Mouse_Cursor_At;




    return field_num;
}


// WZD s36p65
void Init_Mouse_Keyboard(int16_t input_type)
{

    p_fields = (struct s_Field *)Allocate_Space(357);  // 357 PR, 5712 B  (¿ 150 * 38 = 5700 ¿ + 12 ?)

    switch(input_type)
    {
        case 0:
        {
            goto Nay_Mouse;
        } break;
        case 1:
        {
            goto Yay_Mouse;
        } break;
        case 2:
        {
            goto Nay_Mouse;
        } break;
    }

    goto Done;


Nay_Mouse:
    RP_Mouse_SetUsable();
    mouse_installed = ST_FALSE;
    MOUSE_Emu_X = 158;
    MOUSE_Emu_Y = 100;
    Set_Pointer_Position(158, 100);
    goto Done;


Yay_Mouse:
    Set_Mouse_List(1, mouse_list_init);
    mouse_installed = Init_Mouse_Driver();
    if(mouse_installed != ST_FAILURE)
    {
        mouse_installed = ST_TRUE;
    }
    else
    {
        goto Nay_Mouse;
    }
    goto Done;


Done:

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
// PLATFORM  int16_t Get_Input(void)



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


// WZD s36p85
// MoO2  Module: video  Toggle_Pages()  &&  Module: MOX2  _TOGGLE_PAGES_
/*
¿ how to explain ?

Elsewhere, `GUI_PageFlipPrep(); ... GUI_PageFlip();`
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
