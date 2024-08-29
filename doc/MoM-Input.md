



DEPRECATED
SEEALSO: MoX_Input.md







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





20230725:
#SRSLY Why are my notes on this so terribly bad?
Interpret_Mouse_Input()
    ¿¿¿
        if Keyboard_Status()
        else if MD_GetButtonStatus()
        else if MD_GetClickRec1()
        else
    ???
    ... else if MD_GetButtonStatus() ...





NOTE(JimBalcomb,20230628):
Just finished a lengthy session with DOSBox Debug to track down the ProgramPath for 'Right-Click, Scroll Map'.
Started a new ODG - MoM-ProgramFlow-Input.
Can't seem to figure out how to organize and track my notes or tie it back to IDA or the code.
Elsewhere, trying to see about breaking up or just breaking away from the Dasm for the Input Handler.
    'keyboard or mouse or nothing'
        if mouse...
            if mouse right and help or global_esc
            if mouse, still...



if !keyboard and !mouse and !clickrec then return 0


if(help_list_active == ST_TRUE && Check_Help_List() { MD_Get_ClickRec1(); MD_Get_ClickRec2(); return 0; }
branches off at help_list_active for _global_esc
so, maybe,
    if(MD_ButtonStatus = ST_RIGHTBUTTON)
    {
        if(help_list_active == ST_TRUE && Check_Help_List() { MD_Get_ClickRec1(); MD_Get_ClickRec2(); return 0; }
        else
        {
            if(_global_esc == ST_FALSE) { while(MD_GetButtonStatus() = ST_RIGHTBUTTON){ Quick_Call_Auto_Function()} return ST_UNDEFINED; }
        }
    }

Then, tests MD_GetButtonStatus() again...  which is odd, cause it already stored the result locally...
More odd, this is a loop of some sort... with multiple reentry points?
Anyway, ...
    @@IDK_Loop_GetButtonStatus
    field_num = Scan_Field()
    // Begin Block: Push_Field_Down()
    if (field_num != 0)
        AND
            (field_num != down_mouse_button) && (p_fields[field_num].type != ft_Input) && (p_fields[field_num].type != ft_ContinuousStringInput)
            OR
            (down_mouse_button != ST_UNDEFINED) && (p_fields[field_num].type != ft_Grid) && (p_fields[down_mouse_button].type != ft_Grid) )
        
            if(p_fields[down_mouse_button].type == ft_Slidebar)
            {
                GUI_CallRedrawFn()
            }

            Push_Field_Down()

    // End Block: Push_Field_Down()

    if(p_fields[field_num].type == ft_ContinuousStringInput)
        input_field_active != ST_FALSE
            active_input_field_number == field_num
                strcpy(GUI_EditString, Param0)
                GUI_EditAnimStage = 0
                GUI_EditCursorOn = 0
                input_field_active = e_ST_TRUE
                active_input_field_number = field_num
                Meh. Does a bunch more stuff...
    ...looks like 2-3 other block branches..

    @@IDK_After_Loop_GetButtonStatus





There's some block that iterate through all the fields - it checks the bounds and breaks on a match.
Looks like it sets field_num to the itr outside of the loop, but probably not, because the next line is checking if field_num is 0.
if field_num != and !ft_ContinuousStringInput
    Push_Field_Down(); GUI_1TickRedrw();

The block after that looks to be the only path forward, and starts with down_mouse_button = ST_UNDEFINED; if(field_num != 0)...


There appears to be another blok at loc_2744E, where it checks *GUI_MouseFocusCtrl* for ft_Slidebar and calls GUI_CallRedrawFn()
¿ mayhap, this is just the fall-through / end of the if block ?
But, also, does the use of *GUI_MouseFocusCtrl* here mean it expects to have one? Or, does it bail somewhere else?


For _global_esc, both Button and Buffer jump to the same block, which is odd. And, that block has MD_Get_ClickRec1/2(), which is nuts for the Button path.









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
    ... ... ... if(_global_esc == ST_FALSE) while(MD_GetButtonStatus() == 2) { Quick_Call_Auto_Function(); }
        MD_GetClickRec1(); MD_Get_ClickRec2(); return ST_UNDEFINED;
The code int the MD_GetClickRec1() branch has it following Check_Help_List() as well.
Looks to be an &&|| that gets lands in a loop waiting for the right-click to end.





### Interpret_Keyboard_Input()

int16_t Interpret_Keyboard_Input(int16_t * field_num)

returns character, inouts field_idx

whole buncha something with multi-hotkey/alt-strings

What's up with KD_prev_field_idx?
WZD dseg:824C  KD_prev_field_idx dw 0
1oom guy has it as "uiobj_kbd_alt_oi"

KD_prev_field_idx

multi_hotkey_active_field
...holds a field_idx

MoO2
    if multi_hotkey_active_field
    alpha_key = Get_Alt_Alpha_Key(character)
    multihotkey_field_idx = Check_Multi_Hot_Key_Fields(alpha_key)

¿ 1oom put the multihotkey Alt-Key check in uiobj_handle_kbd_find_alt() ?
    mookey_t k = KBD_GET_KEY(key);
    uint32_t kmod = KBD_GET_KEYMOD(key);
    char c = uiobj_get_keychar(key);
    ...
    if ((p->type == 8) && KBD_MOD_ONLY_ALT(key) && (k == p->key))
Yeah, because he used SDL keys, instead of hacking those back into ST keys.

1st block
    checks active multihotkey field and current hotkey character match
2nd block


#1
(
    p_fields[_SI_field_idx].hotkey != character
    ||
    (
        p_fields[_SI_field_idx].hotkey == character
        &&
        p_fields[_SI_field_idx].type == ft_MultiHotKey
    )
)
vs.
#2
(
    (p_fields[_SI_field_idx].hotkey != character)
    ||
    (p_fields[_SI_field_idx].type == ft_MultiHotKey)
)
vs.
#3
!(
    p_fields[_SI_field_idx].hotkey == character
    &&
    p_fields[_SI_field_idx].type != ft_MultiHotKey
)

#2
    if hotkey != character
        TRUE: jumps, doesn't even test the next condition
        FALSE: means (hotkey == character), tests next condition
    concern?
        (hotkey == character) && (type != ft_MultiHotKey)
        not possible, no concern

#2 and #3 are logically equivalent?
so, ... decision criteria?
communicating intent?
What are we looking for?
Why would we want to match type == ft_MultiHotKey if hotkey != character?
The immediate, subsequent re-check for (type == ft_MultiHotKey)
  makes it feel like this test must not be conclusive on that condition?
  So, it must not be #2?
    Starts from (hotkey == character)...
    Then only if also (type == ft_MultiHotKey)
    Hence, Bail: (hotkey == character && type != ft_MultiHotkey)
But, ...
    it converts character to alt key, so it wanted hotkey != character

Case 1: hotkey != character


Case 2: hotkey == character && type == ft_MultiHotKey

20240513
I think I just finally got it.
It should loop up the hotkey match...
but, if the field is a ft_MultiHotKey, it checks the Alt-Key and breaks
so, after the while loop, you have the field_idx of the field that matched the hotkey, whether it was a ft_MultiHotKey or not.
The flags for first checking the current multi hotkey are to prioritize matching its next hotkey character over any other field that might have that same character as its hotkey.
(feels terribly fragile)









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










## down_mouse_button
## GUI_MouseFocusCtrl



## down_mouse_button

Init_Mouse_Keyboard


Interpret_Mouse_Input+D                                      mov     [down_mouse_button], -1                            
Interpret_Mouse_Input+2C0                                    mov     [down_mouse_button], e_ST_UNDEFINED_DW             
Interpret_Mouse_Input:@@UnsetDownMouseButton_Return_ITR_     mov     [down_mouse_button], e_ST_UNDEFINED_DW             
Interpret_Mouse_Input:@@UnsetDownMouseButton_Return_ITR      mov     [down_mouse_button], e_ST_UNDEFINED_DW             
Interpret_Mouse_Input:@@UNDEF_DownMouseButton                mov     [down_mouse_button], -1                            
Interpret_Mouse_Input:loc_27035                              mov     [down_mouse_button], e_ST_UNDEFINED_DW             
Interpret_Mouse_Input+A57                                    mov     [down_mouse_button], -1                            
Interpret_Mouse_Input:@@Check_DownMouseButton                cmp     [down_mouse_button], e_ST_UNDEFINED_DW             
Interpret_Mouse_Input:loc_271F1                              mov     ax, [down_mouse_button]                            
Interpret_Mouse_Input:loc_2720B                              mov     ax, [down_mouse_button]                            
Interpret_Mouse_Input+BB4                                    mov     ax, [down_mouse_button]                            
Interpret_Mouse_Input+BC9                                    mov     ax, [down_mouse_button]                            
Interpret_Mouse_Input+BEB                                    push    [down_mouse_button]             ; field_idx        
Interpret_Mouse_Input:loc_2728E                              mov     [down_mouse_button], e_ST_UNDEFINED_DW             
Interpret_Mouse_Input+C3B                                    cmp     ax, [down_mouse_button]                            
Interpret_Mouse_Input+C6E                                    cmp     [down_mouse_button], e_ST_UNDEFINED_DW             
Interpret_Mouse_Input+C8A                                    mov     ax, [down_mouse_button]                            
Interpret_Mouse_Input:loc_2730B                              mov     ax, [down_mouse_button]                            
Interpret_Mouse_Input:@@UnsetDownMouseButton                 mov     [down_mouse_button], e_ST_UNDEFINED_DW             
Interpret_Mouse_Input+E94                                    mov     [down_mouse_button], -1                            
Interpret_Mouse_Input:@@UnsetDownMouseButton_Return_FieldNum mov     [down_mouse_button], e_ST_UNDEFINED_DW             
Interpret_Mouse_Input+F2F                                    mov     [down_mouse_button], e_ST_UNDEFINED_DW             

RP_GUI_KeyInputOnly:loc_27B0D                                mov     [down_mouse_button], -1                            
RP_GUI_KeyInputOnly:loc_27B67                                mov     [down_mouse_button], -1                            
RP_GUI_KeyInputOnly+509                                      mov     [down_mouse_button], -1                            

Scan_Input:loc_29481                                         cmp     [down_mouse_button], e_ST_UNDEFINED_DW             
Scan_Input+CF                                                mov     ax, [down_mouse_button]                            

GUI_EditBoxControl+413                                       mov     [down_mouse_button], -1                            
GUI_TextEditDialog+68                                        mov     [down_mouse_button], ax                            
GUI_TextEditDialog:loc_2A1D4                                 mov     [down_mouse_button], -1                            

Clear_Fields+9                                               mov     [down_mouse_button], -1                            

GUI_SetLastControl+A                                         mov     [down_mouse_button], -1                            

Init_Mouse_Keyboard+B7                                       mov     [down_mouse_button], e_ST_UNDEFINED_DW             

Draw_Fields:loc_2D32F                                        cmp     [down_mouse_button], _SI_itr_fields_count          
Draw_Fields:field_type_04                                    cmp     [down_mouse_button], _SI_itr_fields_count; case 0x4

Push_Field_Down+3D                                           cmp     _SI_field_num, [down_mouse_button]; _SI = field_num
Push_Field_Down:loc_2F90E                                    cmp     [down_mouse_button], -1                            
Push_Field_Down+52                                           mov     [down_mouse_button], _SI_field_num                 
Push_Field_Down:loc_2F970                                    cmp     [down_mouse_button], _SI_field_num                 
Push_Field_Down+B6                                           mov     ax, [down_mouse_button]                            
Push_Field_Down:loc_2F9A7                                    mov     ax, [down_mouse_button]                            
Push_Field_Down+11F                                          push    [down_mouse_button]             ; field_idx        
Push_Field_Down+12E                                          push    [down_mouse_button]             ; field_idx        
Push_Field_Down:loc_2FA00                                    mov     [down_mouse_button], _SI_field_num                 
Push_Field_Down+19C                                          mov     [down_mouse_button], _SI_field_num                 
Push_Field_Down+1AB                                          mov     [down_mouse_button], -1                            






## GUI_MouseFocusCtrl
WZD dseg:8268


In Interpret_Mouse_Input(), this gets set to field_num, after the call to Push_Field_Down()...
...on the program-path for a right-click, no help, !ft_Input, !ft_ContinuousStringInput, ??? ...



GUI_GetMouseFocus()
     return GUI_MouseFocusCtrl;
XREF:
     Settings_Screen_Draw()
     Load_Screen_Draw()

Interpret_Mouse_Input()
     On-Entry:
          down_mouse_button = -1
          GUI_MouseFocusCtrl = 0
     ~ Mouse Buffer
          ...
          ...
          ...
          field_num != 0
          p_fields[field_num].type != ft_ContinuousStringInput
               GUI_MouseFocusCtrl = field_num
               Push_Field_Down(field_num, mouse_x, mouse_y)
               Quick_Call_Auto_Function()





Up w Interpret_Mouse_Input+18        mov     [GUI_MouseFocusCtrl], 0         
Up w Interpret_Mouse_Input+862       mov     [GUI_MouseFocusCtrl], ax        
Up w Interpret_Mouse_Input+AE4       mov     [GUI_MouseFocusCtrl], e_ST_FALSE
Up w Interpret_Mouse_Input+DBC       mov     [GUI_MouseFocusCtrl], ax        
Up r Interpret_Mouse_Input:loc_2744E mov     ax, [GUI_MouseFocusCtrl]        
Up w Interpret_Mouse_Input:loc_27468 mov     [GUI_MouseFocusCtrl], 0         
Up r GUI_GetMouseFocus+3             mov     ax, [GUI_MouseFocusCtrl]        
Up w G_GUI_ClearInput+3              mov     [GUI_MouseFocusCtrl], 0         
   w Clear_Fields+F                  mov     [GUI_MouseFocusCtrl], 0         


