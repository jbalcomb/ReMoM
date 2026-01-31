
MoO2
Universe_Generation_()
    _n_space_monsters = 0;
Generate_Star_Special_()
    _n_space_monsters++;
Make_System_Monsters_()
    _user_wants_n_space_monsters





// MGC o51p16
// drake178: NEWG_CreateEZs()
; populates the encounter zone table
*/
void NEWG_CreateEZs__WIP(void)

// MGC o51p17
// drake178: NEWG_CreateEncounter()
/*
; clears and then populates the specified encounter
; record with guardians and treasure based on zone
; type, altering the passed budget based on the plane
; and the difficulty setting
;
; BUG: has multiple smaller bugs and inconsistencies
*/


## NEWG_CreateEZs__WIP()

    for(itr = 0; itr < NUM_TOWERS; itr++)
        NEWG_CreateEncounter__WIP(itr, 0, _TOWERS[itr].wx, _TOWERS[itr].wy, 3, 0, (650 + (50 * Random(11))));
