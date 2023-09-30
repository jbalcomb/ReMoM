


BEGIN: Game Button - Plane Button

(input_field_idx == _plane_button)

BEGIN: Do_Plane_Button
END: Do_Plane_Button





Planar Seal added later?
Towers of Wizardy added later?

need code for Active_Stack_On_Astral_Gate()
need code for Unit_Has_Planar_Travel_Item() and UNIT_BU_ApplyItems()
need code for Stack_Movement_Modes()
need code for RP_CTY_CheckSpellWard__STUB() and CTY_CheckSpellWard__STUB()
need code for TILE_GetEnemyStack()
need code for TILE_GetUnitStack()

UNIT_HasInvisibility() still needs j_UNIT_BU_ApplyItems(), for Set_Unit_Draw_Priority(), from Do_Plane_Button()




Main Screen

        /*
            BEGIN: Game Button - Plane Button
        */
        if(input_field_idx == _plane_button)
        {
            // TODO  SND_LeftClickSound();
            Do_Plane_Button(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Main_Screen_Reset();
            Reset_Map_Draw();
            MainScr_Prepare_Reduced_Map();
            // TODO  Deactivate_Auto_Function
            // TODO  Assign_Auto_Function(Main_Screen_Draw(), 1);
            Set_Mouse_List_Default();
        }
        /*
            END: Game Button - Plane Button
        */



Do_Plane_Button()
Stack_Has_Planar_Travel()
    Active_Stack_On_Astral_Gate()
    Unit_Has_Planar_Travel_Item()
        UNIT_BU_ApplyItems()
Check_Planar_Seal()
GUI_WarningType0()
Check_Stack_Plane_Shift()
    Active_Stack_Movement_Modes()
    Terrain_Is_Sailable()
Set_Unit_Draw_Priority()
Reset_Stack_Draw_Priority()
Set_Entities_On_Map_Window()



// WZD o59p18
Do_Plane_Button(_human_player_idx, &_map_x, &_map_y, &_map_plane);

has a variable for the state of the situation
    Nay Active Stack
    Nay Planar Travel
    Yay Planar Travel, Nay Planar Seal
    Yay Planar Travel, Yay Planar Seal

branches first on stack_planar_travel ==/!= ST_TRUE
    means Yay Planar Travel, Nay Planar Seal


if stack_planar_travel != ST_TRUE
    if blocked, jumps to to done
    if curr_map_plane == orig_map_plane
        sets curr_map_plane = ((curr_map_plane + 1) % PLANE_COUNT_MAX)
        sets _unit_stack_count = 0
    ¿ why would they be equal ?
        ¿ only happens if Check_Stack_Plane_Shift() returned -2 ?
        Nope, cause that doesn't get called on this program-path. But, same condition as stack_planar_travel == 99  Yay PlanarTravel Yay PlanarSeal
    if _unit_stack_count > 0
    else jumps to done
        ¿ may have just set _unit_stack_count to 0 ?
            ¿ only happens if Check_Stack_Plane_Shift() returned -2 ?
    if Check_Planar_Seal() == ST_FALSE jumps to done


Paths by Does or Does Not?
    falls through to done
        Yay PlanarTravel, Yay PlanarSeal
        !(_unit_stack_count > 0)

Where is the program-path for (all_units_moved == ST_TRUE)
    stack_planar_travel would still be set to ST_FALSE
        isn't ST_TRUE
        isn't 99
        ¿ would curr_map_plane still not be equal to orig_map_plane ?
        ¿ does it get swallowed/superced by the subsequent check for !(_unit_stack_count > 0) ?
            ¿ would have to, would it not, because you can't have an active stack if all units are moved ?
        ¿ there are no other tests on (all_units_moved == ST_TRUE) or (stack_planar_travel == ST_FALSE) ?





/*
    checks if Active Stack is on City with Astral Gate
    or if all Units have Planar Travel, by Enchantment, Ability, or Item
    returns ST_TRUE or ST_FALSE
*/
int16_t Stack_Has_Planar_Travel(void)


Stack_Has_Planar_Travel()
Check_Planar_Seal()
GUI_WarningType0()
Check_Stack_Plane_Shift()

    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(*map_x, *map_y, curr_map_plane);

