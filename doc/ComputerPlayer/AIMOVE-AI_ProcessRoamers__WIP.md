AIMOVE-AI_ProcessRoamers__WIP.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_ProcessRoamers__WIP.c



searches for and assigns targets and corresponding move orders to all roamer stack on the continent
or, if there are no targets but at least one roaming stack with at least 7 units,
 changes the landmass type and rally point of the continent to prepare for troops to move to the action continent instead


_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Abandon;
_ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)Best_LoadTile_X;
_ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)Best_LoadTile_Y;
