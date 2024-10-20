
MoM / Mox
Tactical Combat



Move_Units()
    Eval_Move_Path__WIP(..., &attack_flag, ...)
    if((attack_flag == ST_TRUE) || (UU_flag_FALSE == ST_TRUE))
        goto Combat_Handlers;
Combat_Handlers:
    if(OVL_Action_Type == 5)
    else
    combat_result = Combat__WIP(player_idx, defender_idx, troop_count, &troops[0]);
    if(combat_result == 1)
        Player_Army_At_Square(OVL_Action_XPos, OVL_Action_YPos, OVL_Action_Plane, player_idx, &troop_count, troops);



combat_result = Combat__WIP(player_idx, defender_idx, troop_count, &troops[0]);
    if(((combat_attacker_player_idx == _human_player_idx) || (defender_idx == _human_player_idx)) && (magic_set.strategic_combat_only == ST_FALSE))
        Battle_Outcome = Tactical_Combat__WIP(combat_attacker_player_idx, defender_idx, troops, troop_count, OVL_Action_XPos, OVL_Action_YPos, OVL_Action_Plane, &Item_Count, &Item_List[0]);



int16_t Tactical_Combat__WIP(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
