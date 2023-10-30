







## Left-Click Unit Window Grid Field

Update_Stack_Active()

IIF
    Status != DONE  and  HMoves >= 1
So, ...
    a Unit in the Stack can not be made Active if its Status is Done or it has no movement points

(G_OVL_MapVar4 == ST_TRUE) && (_unit_stack_count > 1) && (_UNITS[stack_unit_idx].Status != US_Casting)

still don't know G_OVL_MapVar4 is all about
can't even get here if _unit_stack_count isn't greater than 1
What's so special about casting?

Then, get a count of units in the stack that are set to active
And, if more then one is set to active, set them all to inactive

In-game, when the stack is selected, all the active units are as-is
    but, when you left click on one of them, the others get set to inactive / deselected
    after that, though, left clicking on another *adds* it as active
    what's controlling the behavior being different for the first left-click vs. subsequent left-clicks?
    G_OVL_MapVar4?

May be that G_OVL_MapVar4 exists soley for controlling the behavior of the 'Unit Window' Stack Unit Activation?
    the first time you left-click on any Unit in the Stack in the 'Unit Window'
        it deactivates all the other Units in the Stack
    after that, when you click on another unit,
        it adds it to the Selection of Active Stack Units

¿ defaulted to ST_TRUE in G_WLD_StaticAssetRfrsh() ?
¿ reset in Select_Unit_Stack() ?
¿ unset in Update_Stack_Active() ?
renamed to reset_active_stack





## Right-Click Unit Window Grid Field









IDK_ActiveUnitStack_MovesOrPath_s53150()
XREF:
    j_IDK_ActiveUnitStack_MovesOrPath_s53150()

j_IDK_ActiveUnitStack_MovesOrPath_s53150()
XREF:
    Main_Screen+995

IDK_ActiveUnitStack_MovesOrPath_s53150()
    ¿ getting passed stack_idx or unit_window_field_idx ?











// WZD o57p07
void Add_Unit_Window_Fields(void);

// WZD o58p01
void Main_Screen_Draw_Unit_Window(int16_t start_x, int16_t start_y);

// WZD o063p06
void Unit_Window_Draw_Unit_Attributes(int16_t x, int16_t y, int16_t unit_idx);

// WZD o64p09
void Unit_Window_Picture_Coords(int16_t unit_stack_unit_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);


// WZD dseg:C052
int16_t Unit_Window_Fields[9];

// WZD dseg:C082 _unit_window_start_y dw 0
// WZD dseg:C084 _unit_window_start_x dw 0



// WZD 057p01
void Main_Screen(void)

    // IDK   _unit_window_start_x = 247;  // AKA OVL_STKUnitCards_Lft
    // IDK   _unit_window_start_y = 79;  // AKA OVL_STKUnitCards_Top


after
    END: Game Button - Plane Button
before
    BEGIN: Next Turn

        // Left-Click Unit Window Grid Field
        // ovr057:097B
        // Main_Screen+097B

        // Left-Click Unit Window Grid Field
        for(Stack_Index = 0; Stack_Index < _unit_stack_count; Stack_Index++)
        {
            if(Unit_Window_Fields[Stack_Index] = input_field_idx)
            {
                // RP_SND_LeftClickSound2();  // byte-identical to SND_LeftClickSound() 
                // IDK_ActiveUnitStack_MovesOrPath_s53150(Stack_Index);
                // Set_Entities_On_Map_Window(_curr_world_x, _curr_world_y, _world_plane);
                // NIU_MainScreen_local_flag = 1;
            }
        }
        
        // Right-Click Unit Window Grid Field
        for(Stack_Index = 0; Stack_Index < _unit_stack_count; Stack_Index++)
        {
            if(Unit_Window_Fields[Stack_Index] = -input_field_idx)
            {
                // OVL_ShowActiveStack();
                // UNIT_DrawPriorities();
                // Set_Entities_On_Map_Window(_curr_world_x, _curr_world_y, _world_plane);
                // Main_Screen_Draw();
                // PageFlip_FX();
                // Unit_Window_Picture_Coords(Stack_Index, &OLft, &OTop, Right@, Bottom@);
                // NOTE(JimBalcomb,20230802): this here looks like what I just saw for clicking the Hero Picture on the Items Screen
                //                            so, YayNayMay Unit_Window_Picture_Coords() is just getting the coords for the grow-out pop-up effect
                // TODO  USW_FullDisplay(_unit_stack[unit_idx].unit_idx, OLft, OTop, OLft+18, OTop+18);
                // Assign_Auto_Function(Main_Screen_Draw, 1);
                // Allocate_Reduced_Map();
                // Set_Mouse_List_Normal();
                // Reset_Active_Stack_Draw();
                // UNIT_DrawPriorities();
                // STK_NoUnitDraw();
                // Set_Entities_On_Map_Window(_curr_world_x, _curr_world_y, _world_plane);
                // Reset_Map_Draw();
                // MainScr_Prepare_Reduced_Map();
                // screen_changed = ST_TRUE;
                // Clear_Help_Fields();
                // TODO  Main_Screen_Help();
            }
        }








Page 47  (PDF Page 52)
Unit Window
The *unit window* is extensively described earlier in **Unit Actions**.
This window appears on the right side of the *main movement screen* whenever it is time for and active unit (stack) to move or when you right-click on a unit or stack in the *main movement view* or a *city screen*.
All units that appear together in the *unit window* are currently stacked (no more than nine units may be stacked together).
Grayed out units in the *unit window* are either patrolling or out of movement points for the turn.
Click on a unit in the *unit window* activates it.
Right-clicking on a unit displays the unit's statistics (see **Basic Unit Information**).
The *unit window* provides some unit information at a glance:
DAMAGE
EXPERIENCE LEVEL REPRESENTATION
WEAPON TYPES
MOVEMENT MODES
