

WIZARDS.EXE
seg035
seg036

Fields
~Input
Mouse
Keyboard
Help
-Mouse Buffer, Click Records

? Data Segment ?








WZD dseg:8296 have_help
XREF
Up w Clear_Help_Fields+3       mov     [have_help], 0         
Up r Interpret_Mouse_Input+752 cmp     [have_help], 0         
Up r Interpret_Mouse_Input+B09 cmp     [have_help], e_ST_FALSE
Up w Set_Help_List+9           mov     [have_help], e_ST_TRUE 

WZD dseg:8298 _help_count
XREF
Up w Clear_Help_Fields+9            mov     [_help_count], 0 
Up r Draw_Fields:loc_2E684          cmp     [_help_count], 0 
Up r Draw_Fields:loc_2E7C7          cmp     si, [_help_count]
Up r GUI_ContextBasedHelp+1D        cmp     [_help_count], 0 
Up r GUI_ContextBasedHelp+A7        mov     di, [_help_count]
Up r GUI_ContextBasedHelp:loc_2780F cmp     di, [_help_count]
Up w Set_Help_List+12               mov     [_help_count], ax

Set_Help_List()
help_list is passed, presumably a pointer to an array of help structures
help_count is passed, presumably a count of elements in the array
have_help is set to ST_TRUE

help_list_active = ST_FALSE
help_list_count = 0
help_struct_pointer

struct looks like
? `if(help_struct_pointer[itr] != 0xFFFF ...
x1
y1
x2
y2
looks like help_struct_pointer[itr] gets passed to Draw_Help_Entry()
...heh, wtf. it gets out of the loop by setting itr to help_list_count



    signed integer (4 bytes) default_help_entry
    Address: 02:0017C47A

    function (0 bytes) Set_Help_Field_Outline_Color
    Address: 01:00119497
        Num params: 1
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) color

    signed integer (2 bytes) help_field_outline_color
    Address: 02:0017C4B6

    signed integer (2 bytes) help_box_mode
    Address: 02:0017C4CE

ovr159

//drake178  GUI_DisplayHelp()
    code (0 bytes) Draw_Help_Entry_
    Address: 01:000A59A1
        Locals:














Init Input
Init Keyboard
Init Mouse



Check Key Press
Get Key Press

Check Mouse Button Status
Get Mouse Button Status
Get Mouse Position - X,Y
Get Mouse Button Click - L,R



Create Fields
Match Key or Button to Field

Loop to match string to multiple key hot-keys



int16_t Interpret_Keyboard_Input(int16_t field_num)



Get_Input()
Interpret_Mouse_Input()
Interpret_Keyboard_Input()
Keyboard_Status()
Read_Key()




Get_Input()
    |-> Interpret_Mouse_Input()
        |-> Keyboard_Status()
        |-> Interpret_Keyboard_Input()
            |-> Read_Key()

Program Path:
    return ZERO         Get_Input() |-> input_delay >= 1
    return field_index  Get_Input() |-> Interpret_Mouse_Input()

    ? Get_Input() |-> Interpret_Mouse_Input() |-> Keyboar_Status() != ST_FALSE
    character & field_num  Get_Input() |-> Interpret_Mouse_Input() |-> Interpret_Keyboard_Input()
    ❗❓❗ { {NAY character, NAY field_num}, {YAY character, NAY field_num}, {NAY character, YAY field_num}, {YAY character, YAY field_num}, } ❓❗❓
    BRANCH: character != 0 || character == 0
        ~== Global Hot-Key
        ... F11, F12, ESCAPCE, BACKTICK, TILDE ...
    

...
...
...
    Nay FieldType != ft_MultiHotKey
    ? field_num == 0 || character != field.hotkey ?
    character == KP_Enter
    character == '+' || character == '-'












### Interpret_Mouse_Input()
returns 0, -1, field_index, negative field_index, s_Field.Param0, ...

sets pointer_offset with call to Get_Pointer_Offset()
so, ...
    pointer_offset is exposed to the Input module, but current_mouse_list is not
    
if(Keyboard_Status == ST_FALSE) { character = Interpret_Keyboard_Input(&field_num); ... }

field_num from a hotkey
character for an input field
mouse click left button
mouse click right button
mouse over field, w/o key-press or mouse-click
-highlight buttons on main menu screen
? mouse click right button goes to help ?

So, ...
    go after mouse-over
    ? no key press, no button click, ???


if NOT Keyboard_Status(), NOT MD_GetButtonStatus(), NOT MD_GetClickRec1()
    Mouse_Button_Handler()
    Return 0;

code between Mouse_Movement_Handler() and Mouse_Button_Handler()
feels like if(Keyboard_Status()) { ... } else { ... }

if( Keyboard_Status() ) { ... } else { ... }
Mouse_Movement_Handler()
if( MD_GetButtonStatus() ) { ... } elseif( MD_GetClickRec1() ) { ... } else { ... }
    ... Mouse_Button_Handler(); return 0;
I don't think this plays.
Why have the test for MD_GetClickRec1()?

What about _global_esc?
    ... ... ... if(_global_esc == ST_FALSE) while(MD_GetButtonStatus() == 2) { GUI_1TickRedraw(); }
        MD_GetClickRec1(); MD_Get_ClickRec2(); return ST_UNDEFINED;
The code int the MD_GetClickRec1() branch has it following Check_Help_List() as well.
Looks to be an &&|| that gets lands in a loop waiting for the right-click to end.





## Initialization

// MGC _s33p01  ST_GUI.H
Set_Mouse_List(1, default_mouse);       AKA GUI_SetWindows(1, g_GUI_VirtualScreen);





## Per Screen
Add Fields
table of indexes of fields - rect, key, type, ...
Input -> Fields
Key Code & Field Index

Get_Input()
    returns ZERO on input_delay


assumption: have key_pressed
    Interpret_Keyboard_Input()
        out field_num
        ret character


Return type: signed integer (2 bytes) 
Parameters:
    pointer (4 bytes) field_num
Locals:
	signed integer (2 bytes) key
	signed integer (2 bytes) character
	signed integer (2 bytes) original_character







? uses field_num to check field_type, before checking if field_num is FALSE ?
branches on field_type Ctrl_AltString


Push_Field_Down()
Draw_Field(down_mouse_button)


MGC s34p71
Draw_Fields()
MGC s34p72
Draw_Field()
MGC s34p73
Push_Field_Down()



int16_t down_mouse_button = ST_UNDEFINED;
MGC
dseg:4D22 FF FF                       down_mouse_button dw -1
dseg:8250 FF FF                       down_mouse_button dw 0FFFFh





MoO2 ORION2.EXE

Get_Input()
    |-> Interpret_Mouse_Input()
        |-> Keyboard_Status()

...
mov     [ebp+field_num__scanned_field], 0
mov     [ebp+maybe__field_num__scanned_field], 0
mov     [ebp+var_4], 0
mov     [ebp+field_num__scanned_field], 0
mov     down_mouse_button, 0FFFFh
mov     [ebp+mouse_button], 0
mov     auto_input_variable, 0

call    Pointer_X                       ; returns mouse_x, from User_Mouse_Handler
mov     [ebp+x], eax
call    Pointer_Y                       ; returns mouse_y, from User_Mouse_Handler
mov     [ebp+y], eax
call    Get_Pointer_Offset
mov     pointer_offset, ax

if(Keyboard_Status())
    var_10 = Interpret_Keyboard_Input(&field_num__scanned_field);
    cmp word ptr [ebp+var_10]

else
    Mouse_Movement_Handler()
    if(Mouse_Button())


...


call Get_Input
                                        ; Address: 01:001171AB
                                        ; Num params: 0
                                        ; Return type: signed integer (2 bytes)
                                        ; Locals:
                                        ; signed integer (2 bytes) field
                                        ; signed integer (2 bytes) field_index
mov     ecx, eax
mov     ebx, eax

call    Scan_Input
mov     _scanned_field, ax

mov     eax, _mainmenu                  ; ? Table of Fields  ; for the Main Menu Screen ?
cmp     cx, [eax+0Ch]

