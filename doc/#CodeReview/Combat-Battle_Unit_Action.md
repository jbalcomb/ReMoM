Combat-Battle_Unit_Action.md



g_combat_grid_action_map


Combat_Screen__WIP()
    |-> Battle_Unit_Action()
        |-> Battle_Unit_Attack
        |-> Move_Battle_Unit

Check_Attack_Melee
BU_MeleeWallCheck

Combat_Move_Path_Valid
Assign_Combat_Grids

Move_Battle_Unit()
    |-> Combat_Move_Path_Find

Assign_Mouse_Images()
    |-> BU_MeleeWallCheck()



Combat.c
Combat_Screen__WIP()
        /*
            BEGIN:  Left-Click Combat Grid
        */
        {
            // @@LeftClickCombatGrid
            if(input_field_idx == combat_grid_field)
            {
                _human_handle_immobile = ST_FALSE;
                frame_scanned_cgx = Get_Combat_Grid_Cell_X(((int16_t)grid_sx + 4), ((int16_t)grid_sy + 4));
                frame_scanned_cgy = Get_Combat_Grid_Cell_Y(((int16_t)grid_sx + 4), ((int16_t)grid_sy + 4));
                Battle_Unit_Action(_active_battle_unit, frame_scanned_cgx, frame_scanned_cgy);
                for(itr = 0; itr < _combat_total_unit_count; itr++)
                {
                    BU_SetVisibility(itr);
                }
                Assign_Combat_Grids();
                m_unknown_variable__ovr090 = ST_TRUE;
                winner = Check_For_Winner();
                if(winner != ST_UNDEFINED)  /* invalid / no winner / none / neither */
                {
                    leave_screen = ST_UNDEFINED;
                    input_field_idx = 0;
                }
            }
        }
        /*
            END:  Left-Click Combat Grid
        */
        
// WZD s91p06
/* GEMINI */
void Battle_Unit_Action(int16_t _battle_unit_idx, int16_t cgx, int16_t cgy)


