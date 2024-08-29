
Keyboard
Mouse





IDA Colors
Keyboard
Keyboard Debug Keys     light grey #46
Mouse           
Mouse Button Loop       bright green #11
Mouse Buffer    dark purple #40
...
    as of 20240725, in Input.C Interpret_Mouse_Input()
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
hunter green #35  Keyboard Hot-Keys
hunter green #36  ENTER as *click*








¿ UI *Behaviors* ?
Left Button
Right Button
Field Type
Click
Click & Hold
    e.g., Pwer Distribution Staves
        you can left click & hold to slide them up/down,
        but also, while held, you can move the cursor off then back on and it keeps the focus on the staves
        Mayhap, this is the *raison d'etre* for the `down_mouse_button`



## ¿ Hot-Spots ?
Push_Field_Down()
down_mouse_button
Invoke_Auto_Function()
Quick_Call_Auto_Function()

### Push_Field_Down()
made a big effort towards this not too long ago
can't seem to recall the particulars at this moment
IIRC, it was all about getting the Rigkt-Click on the Movement Map working for moving/scrolling the map - a mile-stone for the goal of Navigable/Interactive
I had Left-Click working way back when I was all up in MAGIC.EXE, but, somehow, in the move from MGZ to WZD and/or the move to .\src / away from translating via Casm
    I neglected/misplaced/forgot about that code and the manner in which I had been trying to build up the tangled web of logic that is Interpret_Mouse_Input()
Presently, I have just bumped back into the code that I had gotten working properly for the Movement Map, which is all about being a Grid Field
    I had to go all the way back down to using DOSBox-Debug to trace through the logic, the program-state,
        and the calls to what actually handled a right-click on a Grid Field
    Also, IIRC, this means of updating the values for the Grid Field seems odd, given that Push_Field_Down() otherwise is just meant to set the Up-/Down-State of Button Fields
        Actually, what the Scoll Bars, like the Magic Screen Power Distribution Staves? maybe even the Alchemy Pup - Alchemy Ratio Slider? 
ProgramPath:
    Nay KD, Yay MD, Not Help, Not Cancel
    Yay MD, Yay Field




### down_mouse_button


Up   p Interpret_Mouse_Input+B9C   call    near ptr Invoke_Auto_Function
     p Interpret_Mouse_Input+CB6   call    near ptr Invoke_Auto_Function
Down p Interpret_Mouse_Input+DF9   call    near ptr Invoke_Auto_Function
Down p GUI_EditBoxControl+18       call    near ptr Invoke_Auto_Function
Down p GUI_EditBoxRedraw+46        call    near ptr Invoke_Auto_Function
Down p UU_GUI_ScrollableTxtDlg+A35 call    near ptr Invoke_Auto_Function
Down p GUI_CreateTextDialog+1E6    call    near ptr Invoke_Auto_Function
Down p Quick_Call_Auto_Function+15 call    near ptr Invoke_Auto_Function
Down p GUI_NormalRedraw+15         call    near ptr Invoke_Auto_Function





### //MoO2  ¿ _global_esc vs. mouse_cancel_disabled ?
Module: MOX
    data (0 bytes) _global_esc
    Address: 02:00191A0E
Module: fields
    signed integer (2 bytes) mouse_cancel_disabled
    Address: 02:0017C4B0

mouse_cancel_disabled
XREF
    Init_Mouse_Keyboard()
    Enable_Cancel()
    Disable_Cancel()
    Interpret_Mouse_Input()

Init_Mouse_Keyboard()
    Set_Mouse_List(&default_mouse, 1)
    input_delay = 0
    down_mouse_button = ST_UNDEFINED
    mouse_cancel_disabled = ST_FALSE
    Clear_Fields()
Enable_Cancel()
    mouse_cancel_disabled = ST_TRUE
Disable_Cancel()
    mouse_cancel_disabled = ST_FALSE
Interpret_Mouse_Input()
    (4) if( mouse_cancel_disabled == ST_FALSE ) { ... }

_global_esc
XREF
    Screen_Control_+1FA                   mov _global_esc, e_ST_FALSE
    Init_New_Game_+256                    mov _global_esc, 0         
    Set_Defaults_+38                      mov _global_esc, 0         
    Colonization_+410                     mov _global_esc, 1         
    Empty_Colony_Queue_+71                mov _global_esc, 1         
    Has_Msg_+151                          mov _global_esc, 1         

    Main_Screen_+2FA                      cmp _global_esc, 0         
    Main_Screen_+1379                     cmp _global_esc, 0         
    Begin_Of_Turn_+F                      cmp _global_esc, 1         
    Main_Screen_Report_Handler_:loc_8B250 cmp _global_esc, 1         
    Choose_Network_Plyrs_Screen_+219      cmp _global_esc, 0         











YayNayMay use Go-To's
e.g.,
    ProgramPath:
        `Mouse Button` == Right && _global_esc == ST_FALSE
    ProgramPath:
        `Mouse Buffer` == Right && _global_esc == ST_FALSE
    Then,
        Mouse_Buffer();
        Mouse_Buffer2();
        return ST_UNDEFINED;
    ¿ this/these path(s) DO NOT set back the Mouse_Button_Handler() ?
    ¿ ~== goto Clear_Mouse_Buffer__Return_UNDEFINED: ?



Get_Input()
Interpret_Mouse_Input()

¿ Outer-Most ?
```c

    if(Keyboard_Status() != 0)
    {
        Interpret_Keyboard_Input();
    }
    else
    {
        Mouse_Movement_Handler()
        if(Mouse_Button() != 0)
        {

        }
        else if (Mouse_Buffer() != 0)
        {

        }
        else
        {
            return_value = 0;
        }
        Mouse_Button_Handler();
    }
```

## Nay KD, Yay MD
    mouse_button = Mouse_Button()
    if mouse_button == 2
        if(help_list_active != ST_FALSE && Check_Help_List() != ST_FALSE)
            Mouse_Buffer(); Mouse_Buffer2()
            return 0;
        if(mouse_cancel_disabled != ST_FALSE)
            { while(Mouse_Button() == 2) { Quick_Call_Auto_Function(); } }
            Mouse_Buffer(); Mouse_Buffer2();
            return -1;
    
    ¿ Now, Left Button or just some click / button agnostic ?
    ¿ For sure, !(right-click & got help) !(right-click & got mouse_cancel_disabled) ?
    ...starts two different loops back to here...
    while/if (MD_GetButtonStatus() != 0)
        mouse_x = Pointer_X();
        mouse_y = Pointer_Y();
        cursor_offset = Get_Pointer_Offset();
        field_num = 0;
        // Unused_Local == ST_UNDEFINED
        character = 0;
        field_num = Scan_Field();  // returns the field number of the field for which the mouse cursor is in-bounds
        // ~== if the Mouse Cursor is over/on/in an Input Field
        so, got a Mouse Button, but YayNayMay have a Field?
        yeah, so, point is, we still have something to handle, even if the mouse cursor is not currently over an input field

        if( field_num = Scan_Field() == 0 )
            check down_mouse_button
        


***WE GOT A CLICK AND WE'RE ON A FIELD***

// `Begin Block: field_num = Scan_Field() != 0`

        if( field_num = Scan_Field() != 0 )

***HERE: field_num != down_mouse_button  ~==  Y/N call Push_Field_Down()***
MoO2: "dynamic refresh"

// `Begin Block: Do Push_Field_Down()`
    ~== do Push_Field_Down()
        if ...
            field_num != down_mouse_button
            && p_fields[field_num].type != ft_Input
            && p_fields[field_num].type != ft_ContinuousStringInput
            && !(down_mouse_button != ST_UNDEFINED && p_fields[field_num].type == ft_Grid && p_fields[down_mouse_button].type == ft_Grid )
// `End Block: Do Push_Field_Down()`
        else
            field_num == down_mouse_button
            || p_fields[field_num].type == ft_Input
            || p_fields[field_num].type == ft_ContinuousStringInput
            || (down_mouse_button != ST_UNDEFINED && p_fields[field_num].type == ft_Grid && p_fields[down_mouse_button].type == ft_Grid )

        HERE:  field_num != 0, but did or didn't match down_mouse_button


¿ there is no code for (field_num type != ft_ContinuousStringInput) && (input_field_active == ST_FALSE) ?
so, the block bounds/entry condition is ... if( !( () && () ) )
meaning, we do not need to handle exiting or entering or exiting & entering an edit-state

// `Begin Block: Continuous String Intput`
    e.g., Save Game Name
        clicking on to start editing
        clicking off
    ¿ Enter / Exit Edit-State ?

    input_field_active
    active_input_field_number
    continuous_string

    paths to enter edit field
    paths to exit edit field

    click on a field that is not an edit field, but there was an edit field active
    ~== if( (p_fields[field_num].type != ft_ContinuousStringInput) && (input_field_active != ST_FALSE) )

    loc_27425:
        convergence of clicked on a field that is not an edit field and there was or was not an edit field active
    

p_fields[field_num].type == ft_ContinuousStringInput          p_fields[field_num].type != ft_ContinuousStringInput
input_field_active != ST_FALSE                                input_field_active != ST_FALSE


***Do Nothing***
p_fields[field_num].type != ft_ContinuousStringInput  &&  input_field_active == ST_FALSE
p_fields[field_num].type == ft_ContinuousStringInput  &&  input_field_active != ST_FALSE  &&  field_num == active_input_field_number


***Enter Edit-State***
    p_fields[field_num].type == ft_ContinuousStringInput  &&  input_field_active == ST_FALSE
    p_fields[field_num].type == ft_ContinuousStringInput  &&  input_field_active != ST_FALSE  &&  field_num != active_input_field_number
    {
        strcpy(continuous_string, p_fields[field_num].Param0)
        GUI_EditAnimStage = 0
        GUI_EditCursorOn = 0
        input_field_active = ST_TRUE
        active_input_field_number = field_num
    }


***click NOT on an edit field - Yay/Nay previous field click was an edit field***
¿¿¿
    field_num type !=//== ft_ContinuousStringInput
    input_field_active !=/== ST_FALSE
    field_num !=/== active_input_field_number
    ¿ GUI_EditString ?
    ¿ GUI_MouseFocusCtrl ?

???


¿ any way to make these conditions make sense for the branching without having the whole block wrapped in the two NOT multi-conditions ?

if(p_fields[field_num].type != ft_ContinuousStringInput)
    if(input_field_active == ST_FALSE)
        `DO NOTHING`

if(p_fields[field_num].type != ft_ContinuousStringInput)
    if(input_field_active != ST_FALSE)
        `Exit Edit-State`

if(p_fields[field_num].type == ft_ContinuousStringInput)
    if(input_field_active == ST_FALSE)
        `Enter Edit-State`

if(p_fields[field_num].type == ft_ContinuousStringInput)
    if(input_field_active != ST_FALSE)
        if(field_num != active_input_field_number)
        `Exit Edit-State`
        `Enter Edit-State`

if(p_fields[field_num].type == ft_ContinuousStringInput)
    if(input_field_active != ST_FALSE)
        if(field_num == active_input_field_number)
            `DO NOTHING`


if(p_fields[field_num].type != ft_ContinuousStringInput)
    if(input_field_active == ST_FALSE)
        `DO NOTHING`
    else /* if(input_field_active != ST_FALSE) */
        `Exit Edit-State`
else /* if(p_fields[field_num].type == ft_ContinuousStringInput) */
    if(input_field_active == ST_FALSE)
        `Enter Edit-State`
    else /* if(input_field_active != ST_FALSE) */
        if(field_num == active_input_field_number)
            `DO NOTHING`
        else /* if(field_num != active_input_field_number) */
            `Exit Edit-State`
            `Enter Edit-State`


// `End Block: Continuous String Intput`
/*
    END BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State
*/




GUI_MouseFocusCtrl = field_num;  // MoO2: auto_input_variable

cmp     [GUI_ClickActivate], e_ST_FALSE ; MoO2: mouse_auto_exit


if(GUI_ClickActivate != ST_FALSE)
{
    if(p_fields[GUI_MouseFocusCtrl].type == ft_Scroll)
    {
        // Invoke_Auto_Function();
    }

    GUI_MouseFocusCtrl = 0;

    if(field_num != 0)

Mouse_Buffer()
Mouse_Buffer2()
GUI_Processed_LastX = mouse_x;
GUI_Processed_LastY = mouse_y;
GUI_Processed_Btns = mouse_button;

switch(p_fields[field_num].type)
{

}

}
¿ else ?







## Code-Blocks / Segments / Sections / Spaces
if Keyboard_Status() or Mouse_Button() or Mouse_Buffer() ...

if Mouse_Button()
    ...help...
    ...cancel...
    ...Yay/Nay mouse over field...
    ...Yay/Nay down_mouse_button...

Nay KD, Yay MD, Not Help, Not Cancel, Yay Field
    if field_num = Scan_Field() == down_mouse_button


Nay field_num, Yay down_mouse_button
Yay field_num, Nay down_mouse_button
Yay field_num, Yay down_mouse_button
    if field_num == down_mouse_button
    if field_num != down_mouse_button



