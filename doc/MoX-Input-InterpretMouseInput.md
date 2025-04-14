


20250403:
if(Keyboard_Status())
else if(Mouse_Button())
else if(Mouse_Buffer())
else
    return 0;



[ ] Yay KD
    [x] Keyboard Hot-Key
    [ ] ST_ENTER
        [ ] ST_ENTER && Nay input_field_active
        [ ] ST_ENTER && Yay input_field_active





Return Types

Program Paths - From & To


How to document this is a way that is useful and maintainable?
spreadsheet for paths and/or elements and/or outcomes
diagrams
tie code back to Dasm?
jump-to's for IDA?
colors in IDA?
    black for non-branching, jumps to a return?
sub-blocks for where remaining jumps to a return?


Can I break it up, without losing the OG-MoM feeling?
Macros?
The 1oom guy did some sub-functions, and some odd refectors.
Macros for Return Types, named for what they accomplish?



if(Keyboard_Status() == ST_TRUE)
    do the keyboard
else
    do the mouse

Handle Keyboard Input:
    get character and field_num
    if character & field_num
    or just character
    or just field_num
    ...
    hard-coded hot-keys




## _SI_alt_field_num

Assignments:
    _SI_alt_field_num = active_input_field_number
    _SI_alt_field_num = Scan_Field()
    _SI_alt_field_num = 1
    _SI_alt_field_num = Scan_Field()
    _SI_alt_field_num = 1
    _SI_alt_field_num += 1







## Program Paths
Top-Most
Keyboard Path
Mouse Path

Hard-Coded Hot-Keys
`
~
F11
F12
Enter
+
-
"+"||"-" just for Scroll (Bar) Fields
¿ Enter just for Text/Input Fields ?


Keyboard Path defaults to reset/clear/no-input?
    down_mouse_button = ST_UNDEFINED;
    Mouse_Button_Handler();
    return 0;



### Branching Canundrums

@@Nay_FieldType_CtrlAltStr

if((field_num != 0) && (p_fields[field_num].hotkey == character))
vs.
if((field_num == 0) || (p_fields[field_num].hotkey != character))

maybe if(field_num && hotkey && ft_Scroll)
but, then if(field_num ==/!= 0)

given compiler's penchant for reversing tests, prior branch would be if !field_num || !hotkey
which gets you to if ENTER and +/-
then input_field_active, with ranges of acceptable characters and updating the input string

Do both KP_ENTER and +-Scroll branches assume there's no hotkey and/or field_num?
if KP_ENTER
    if input_field_active
    if Scan_Field()
    else return 0
if '+'/'-' and Scan_Field() and ft_Scroll
...does not fall through to next block - if input_field_active and acceptable character
so, if input_field_active is the last block, handling any remaining logic for character?

@@Apply_Character_To_Input
seg036:0632
00019692
00026C92
Return_Type_1
    down_mouse_button = ST_UNDEFINED;
    Mouse_Button_Handler();
    return 0;
...default end of Main Keyboard Branch

Yeah. So, DEFINITELY ...
    1) if KP_ENTER
    2) if((character == '+' || character == '-') && (YNM_itr_fields_count__scanned_field = Scan_Field() != 0) && (p_fields[YNM_itr_fields_count__scanned_field].type == ft_Scroll))
    3) if input_field_active


#### ST_KEY_ENTER
...several impossible combinations of conditions
Seems fine on exiting an active input
Seems fine on check of Scan_Field() == 0 and bail
...
Gets to something like...
    if Scan_Field() and ft_ContinuousStringInput then active the field
        disconnected logic of another input field (still) being active
            deactivate the old input field
                reset anim, undef active field idx, save string
        else activate the new input field
        probably not else though since it needs to activate the new one in either case
        but, I don't see a fall through
        also, ...
        if enter was pressed while the pointer was on the currently activate input field

    ¿ ?
    ...
    @@IDK_KD_Enter_PostOp
    3 paths; from inside Scan_Field() && type == ft_ContinuousStringInput
    @@IDK_KD_Enter__New_Block
    5 paths

¿ states and transitions ?

1) if input_field_active, then save the string
2) if pointer on ft_ContinuousStringInput field and no active, new active input string field
if pointer on ft_ContinuousStringInput field and one is active
3)     same one, then deactivate and save string
4)     not the same one, load string and activate

1)  alt_field_num = active_input_field_number;
    strcpy((char *)p_fields[alt_field_num].string, continuous_string);

2)  

3)

4)





1) exit the active input string field
2) 

NOTE: it checks input_field_active just a bit later for the rest of the string input
Also, MoO2 seems to have all the same confounding logic.




### KD; ST_KEY_ENTER;
HERE:  ~ if((field_num == 0) || (p_fields[field_num].hotkey != character))

cmp     [input_field_active], e_ST_FALSE
jz      short loc_26977
...
    if(input_field_active == ST_FALSE)
    if(!input_field_active)
    if(input_field_active != ST_FALSE)
    if(input_field_active == ST_TRUE)
    if(input_field_active)
...
    if(input_field_active == ST_TRUE)
    assumes ft_ContinuousStringInput  11d  0bh





## Return Types

...group by return value: 0, field_num, YNM_itr_fields_count__scanned_field
...and where field_num or YNM_itr_fields_count__scanned_field came from

¿¿¿
    @@ down_mouse_button = ST_UNDEFINED;
        @@ Mouse_Button_Handler()
            @@ return 0;
???

~ 'No Input'
    Keyboard_State() == 1; character = Interpret_Keyboard_Input() == 0;
    Keyboard_State() == 0; Mouse_Button() == 0; Mouse_Buffer() == 0;


Return_Type_Z1:                                                           return 0;
Return_Type_Z2:                                   Mouse_Button_Handler(); return 0;
Return_Type_Z3: down_mouse_button = ST_UNDEFINED; Mouse_Button_Handler(); return 0;

Return_Type_2: Mouse_Button_Handler(); return field_num;
Return_Type_3: Quick_Call_Auto_Function(); down_mouse_button = ST_UNDEFINED; return field_num;

Return_Type_4: Mouse_Button_Handler(); down_mouse_button == ST_UNDEFINED; return YNM_itr_fields_count__scanned_field;
Return_Type_5:                         down_mouse_button = ST_UNDEFINED; return YNM_itr_fields_count__scanned_field;

Return_Type_6: down_mouse_button = ST_UNDEFINED; return p_fields[field_num].Param0;
Return_Type_7: down_mouse_button = ST_UNDEFINED; return p_fields[alt_field_num].Param0;

¿ ~ Global Escape ?
    // @@Return_UNDEF
    return ST_UNDEFINED;



~ KD; if(p_fields[field_num].type == ft_MultiHotKey)
    means we have a field_num from Interpret_Keyboard
    which means we matched a hotkey or ft_MultiHotKey
        and, if ft_MultiHotKey, it was completed


~ KD; field_num != 0 && character == hotkey; type == ft_Scroll
    // jmp     @@NoInput
    Mouse_Button_Handler();
    // @@Return_ZERO:
    return 0;


~ KD; Scanned Field is Scroll Field; character is '+'||'-';
¿ 
Return_Type_4:
    Mouse_Button_Handler();
    down_mouse_button == ST_UNDEFINED;
    return YNM_itr_fields_count__scanned_field;
¿ what is the subsequent impact of returning the scann field index ?


if((character == ST_KEY_ESCAPE) && (mouse_cancel_disabled != ST_FALSE))
            // @@Return_UNDEF
            return ST_UNDEFINED;



### YNM_itr_fields_count__scanned_field

set to Scan_Field() in keyboard +- scroll field
    used for return value
set to active_input_field_number in keyboard enter ative


From Which paths does it get returned?
...
KD; ST_KEY_ENTER
...



### active_input_field_number

Draw_Fields()
    field_type_11:                          ; case 0xB  ft_ContinuousStringInput
    cmp     [active_input_field_number], _SI_itr_fields_count


Param6 {-1,0,1}
-1: Set_Font_Style1(font, ColorSet1, ColorSet2)
 0: Set_Font(font, ColorSet1, ColorSet2)
 1: Set_Font_Style2(font, ColorSet1, ColorSet2)

Set_Font_Style_Shadow_Down
Set_Font_Style
Set_Font_Style_Shadow_Up
