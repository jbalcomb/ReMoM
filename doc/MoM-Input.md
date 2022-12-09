


Init Input
Init Keyboard
Init Mouse



Check Key Press
Get Key Press

Check Mouse Button Click
Get Mouse Button Click



Create Fields
Match Key or Button to Field

Loop to match string to multiple key hot-keys



Get_Input()
Process_Input()
Interpret_Keyboard_Input()
Interpret_Mouse_Input()


int16_t Interpret_Keyboard_Input(int16_t field_num)



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

