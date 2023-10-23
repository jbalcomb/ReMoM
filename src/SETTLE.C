/*
    WIZARDS.EXE
        ovr100

    Moo2
        Module: COLONIZE

*/
/*
o100p04
IDK_Settle_s82061()
o100p05
STK_SettleTile()
o77p01
TILE_Settle()


Module: COLONIZE
    Make_New_Colony_Or_Outpost_
        Address: 01:000E5EB3
    Make_New_Colony_
        Address: 01:000E6071
    Make_New_Outpost_
        Address: 01:000E607F
    Allocate_New_Colony_Slots_
        Address: 01:000E64F4

Module: MAINPUPS
    New_Colony_Confirmation_Box_
        Address: 01:000C9FC4

*/
/*
Program-Flow: Special Unit Action - Settle 

    IDK_Settle_s82061()
        |-> STK_GetMovableUnits()
        |-> STK_SettleTile()
            |-> TILE_Settle(unit_x, unit_y, unit_p, unit_race, unit_owner_idx, unit_idx)
                |-> TILE_CanBeSettled()
                |-> CTY_CreateEmpty()

GUI_WarningType0(cnst_MaxCitiesError);  / * "No more outposts may be built." * /


Has Unit Type Ability 'Create Outpost' and NOT Unit Mutation - Undead

Manual:
**Build** outposts: settlers can build outposts (click on *build button*)
on map squares that are at least three map squares from any
other city.
...select the build option...
...build an outpost...

Game-Data:
"No more outposts may be built."
"New Outpost Founded"
"Outpost Of "
"New Outpost"
*/

#include "MoX.H"
#include "SETTLE.H"
