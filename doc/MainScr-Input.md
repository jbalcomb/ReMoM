

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



Settle < Special < Unit Action
input_idx = Get_Input()
input_idx == _special_button

j_IDK_Settle_s82061

