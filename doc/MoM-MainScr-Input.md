


Left Arrow
←
U+02190
Up Arrow
↑
U+02191
Right Arrow
→
U+02192
Down Arrow
↓
U+02193

North West Arrow
↖
U+02196
North East Arrow
↗
U+02197
South East Arrow
↘
U+02198
South West Arrow
↙
U+02199


Mouse Buttom
Left/Right
Help
Movement Map
Reduced Map
Right-Click Movement Map
Left-Click Movement Map
Scroll Map
Move Stack  (1 square: Move; 2+ squares: Goto)
Right-Click Stack on Movement Map
Left-Click Stack on Movement Map
Right-Click City on Movement Map
Left-Click City on Movement Map
Yay/Nay Active Stack
Owned / Unowned
Right-Click Unit Picture on Unit Window
Left-Click Unit Picture on Unit Window
Unit Action Button
Next Turn Button
Game Buttons





Game Buttons
G   Game
S   Spells
A   Armies
C   Cities
M   Magic
I   Info
P   Plane

Done
Patrol
Wait
Build
Purify
Meld
Settle

N   Next Turn

C   Center Map
U   Unselect
F10 Quick-Save

Simtex Debug
Alt-A
? K, T ?
Shift-1
Shift-2
Shift-3
X?

Hot-Keys to Advisors
F1  Surveyor
F2  Cartographer
F3  Apprentice
F4  Historian
F5  Astrologer
F6  Chancellor
F7  Tax Collector
F8  Grand Vizier
F9  Mirror


Matching Input to Fields

hotkey_idx_F1

// Advisor - Surveyor
if(input_field_idx == hotkey_idx_F1)
{
    Advisor_Screen(0);
    Set_Redraw_Function(j_Main_Screen_Draw, 1);
    Main_Screen_Reset();
}

### Section 1
Hot-Keys to Advisors    {F1, F2, F3, F4, F5, F6, F7, F8, F9}
### Section 2
Simtex Debug/Non-Release    {Alt-T, Alt-P, Alt-K, Shift-3, Alt-T, Alt-N}
### Section 3
Miscellaneous           {F10, X, C, Q or U}
### Section 4.1
Game Buttons            {_info_button, _game_button, _cities_button, _magic_button, _armies_button, _spells_button}
### Section 5
Unit Action Buttons     {_patrol_button, _special_button, _done_button, _wait_button} special:{none, build, settle, purify, meld}
### Section 4.2
Game Buttons            {_plane_button}
### Section 6.1
Left-Click Unit Window Grid Field
### Section 6.2
Right-Click Unit Window Grid Field
### Section 7
Next Turn               _next_turn_button OR Time-Stop OR !EoT_Wait
### Section 8
Direction Keys          {hotkey_idx_Up, hotkey_idx_Home, hotkey_idx_PgUp, hotkey_idx_Left, hotkey_idx_Right, hotkey_idx_End, hotkey_idx_Down, hotkey_idx_PgDn}
### Section 9.1
Left-Click Movement Map
### Section 9.2
Right-Click Movement Map



_info_button
_game_button
_cities_button
_magic_button
_armies_button
_spells_button
_patrol_button
_special_button
_done_button
_wait_button
_plane_button
// Left-Click Unit Window Grid Field
// Right-Click Unit Window Grid Field
_next_turn_button



Settle < Special < Unit Action
input_idx = Get_Input()
input_idx == _special_button

j_IDK_Settle_s82061


## Source Code Navigation

### "BEGIN: Mouse Click"
### "BEGIN: Reduced Map Grid Field"
Left-Click Movement Map Grid Field
Right-Click Movement Map Grid Field



END: Mouse Click
END: Check Input against Fields


### Section 4.1
Game Buttons            {_info_button, _game_button, _cities_button, _magic_button, _armies_button, _spells_button}
### Section 4.2
Game Buttons            {_plane_button}

### BEGIN: Game Buttons - Info Button


### BEGIN: Game Buttons - Game Button

### BEGIN: Game Buttons - Cities Button

### BEGIN: Game Buttons - Magic Button

### BEGIN: Game Buttons - Armies Button


### BEGIN: Game Buttons - Plane Button




### Section 8
Direction Keys          {hotkey_idx_Up, hotkey_idx_Home, hotkey_idx_PgUp, hotkey_idx_Left, hotkey_idx_Right, hotkey_idx_End, hotkey_idx_Down, hotkey_idx_PgDn}
field order: {UpLeft, Up, UpRight, Left, Right, DownLeft, Down, DownRight}
enum: {KP_Left, KP_Right, KP_Up, KP_Down, KP_RightUp, KP_RightDown, KP_LeftUp, KP_LeftDown}

the manual says "by hitting the keys on the numeric keypad"

WZD  seg035
Read_Key()
    mov     ah, 10h
    int     16h
    ; KEYBOARD - GET ENHANCED KEYSTROKE (AT model 339,XT2,XT286,PS)
    ; Return: AH = scan code, AL = character

seg035:040C 3D E0 4B                                        cmp     ax, EKey_Left
seg035:0411 B8 01 00                                        mov     ax, KP_Left
seg035:0417 3D E0 4D                                        cmp     ax, EKey_Right
seg035:041C B8 02 00                                        mov     ax, KP_Right
seg035:0422 3D E0 48                                        cmp     ax, EKey_Up
seg035:0427 B8 03 00                                        mov     ax, KP_Up
seg035:042D 3D E0 50                                        cmp     ax, EKey_Down
seg035:0432 B8 04 00                                        mov     ax, KP_Down
seg035:0438 3D E0 49                                        cmp     ax, EKey_PgUp
seg035:043D B8 05 00                                        mov     ax, KP_RightUp
seg035:0443 3D E0 51                                        cmp     ax, EKey_PgDn
seg035:0448 B8 06 00                                        mov     ax, KP_RightDown
seg035:044E 3D E0 47                                        cmp     ax, EKey_Home
seg035:0453 B8 07 00                                        mov     ax, KP_LeftUp
seg035:0459 3D E0 4F                                        cmp     ax, EKey_End
seg035:045E B8 08 00                                        mov     ax, KP_LeftDown

seg035:0509 80 FC 47                                        cmp     ah, EKey_Num7_Home
seg035:050E B8 1A 00                                        mov     ax, KP_Home
seg035:0514 80 FC 49                                        cmp     ah, EKey_Num9_PgUp
seg035:0519 B8 19 00                                        mov     ax, KP_PgUp
seg035:051F 80 FC 4F                                        cmp     ah, EKey_Num1_End
seg035:0524 B8 1C 00                                        mov     ax, KP_End
seg035:052A 80 FC 51                                        cmp     ah, EKey_Num3_PgDn
seg035:052F B8 1D 00                                        mov     ax, KP_PgDn
seg035:0535 80 FC 48                                        cmp     ah, EKey_Num8_Up
seg035:053A B8 03 00                                        mov     ax, KP_Up
seg035:0540 80 FC 50                                        cmp     ah, EKey_Num2_Down
seg035:0545 B8 04 00                                        mov     ax, KP_Down
seg035:054B 80 FC 4B                                        cmp     ah, EKey_Num4_Left
seg035:0550 B8 01 00                                        mov     ax, KP_Left
seg035:0556 80 FC 4D                                        cmp     ah, EKey_Num6_Right
seg035:055B B8 02 00                                        mov     ax, KP_Right



// enum Key_Press  KP_Home  = 1Ah
// WZD dseg:2E2C
// cnst_HOTKEY_Up dw KP_Up
// enum Key_Press  KP_Up  = 3
// WZD dseg:2E2E
// cnst_HOTKEY_PgUp dw KP_PgUp
// enum Key_Press  KP_PgUp  = 19h
// WZD dseg:2E30
// cnst_HOTKEY_Left dw KP_Left
// enum Key_Press  KP_Left  = 1
// WZD dseg:2E32
// cnst_HOTKEY_Right dw KP_Right
// enum Key_Press  KP_Right  = 2
// WZD dseg:2E34
// cnst_HOTKEY_End dw KP_End
// enum Key_Press  KP_End  = 1Ch
// WZD dseg:2E36
// cnst_HOTKEY_Down dw KP_Down
// enum Key_Press  KP_Down  = 4
// WZD dseg:2E38
// cnst_HOTKEY_PgDn dw KP_PgDn
// enum Key_Press  KP_PgDn  = 1Dh




### Section 9.1
Left-Click Movement Map
IIF *Stack Selected*


