


~ STU_DBG


STU_DEBUG

// e.g.,  #ifdef STU_DEBUG
// e.g.,  #include "../../STU/src/STU_DBG.h"
// e.g.,  #endif

CMake vs. VSC vs. VS  ...(VSC w/wo CMAKE), (VSC w/wo clangd), (VSC w/wo CMAKE)
CMake:
    CMakeLists.txt
        add_compile_definitions(STU_DEBUG)
        target_compile_definitions(MOX PRIVATE STU_DEBUG)
VSC:
    c_cpp_properties.json

    VSCode: File->Preferences->Settings->C/C++->IntelliSense->C_Cpp > Default: Defines
        The value to use in a configuration if defines is not specified, or the values to insert if ${default} is present in defines.
    ...didn't do anything



¿ Process ?

add debug flag for what is being debugged / the debug target / goal / bug / whatever will communicate the intent
...did this a time or three before...




e.g.,
Why are Units disappearing from the min movement map?
Are they just not being draw or are they being killed, deleted, or corrupted?
How to know which data changes are valid?
¿ snap-shot before and after, then step through a whole 'Next Turn', deciding what should or should not change ?
output a diff?

AI_Disband_To_Balance_Budget() ?!?
    Player_Resource_Income_Total(player_idx, &Gold_Income, &Food_Income, &Mana_Income);
    Gold_Income -158
    Food_Income    0
    Mana_Income -276

mana_income: 54
        mana_expense = Player_Armies_And_Enchantments_Mana_Upkeep(player_idx);
        gold_expense = Player_Armies_Gold_Upkeep(player_idx);
        food_expense = Player_Armies_Food_Upkeep(player_idx);
mana_expense    440
gold_expense    409
food_expense    200

difficulty_modifier 75

mana_expense    330
gold_expense    306
food_expense    150

        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            if(_CITIES[itr_cities].owner_idx == player_idx)
                gold_income += (_CITIES[itr_cities].gold_units - _CITIES[itr_cities].building_maintenance);
                food_income += (_CITIES[itr_cities].food_units - _CITIES[itr_cities].population);

gold_income 148
food_income 150

        for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
            if(_players[player_idx].Heroes[itr_heroes].unit_idx > -1)
                if((_HEROES2[player_idx]->heroes[_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].type].abilities & HSA_NOBLE) != 0)

gold_income 148

        *food_total = (food_income - food_expense);

food_total    0

        gold_income += (*food_total > 0) ? (*food_total / 2) : 0;
    
gold_income 148

        *gold_total = (gold_income - gold_expense);

gold_total  -158

        *mana_total = (mana_income - mana_expense);

mana_total  -276


_num_players: 5
DBG_player_idx: 0, DBG_gold_total:  142, DBG_food_total:  5, DBG_mana_total:  -56
DBG_player_idx: 1, DBG_gold_total: -158, DBG_food_total:  0, DBG_mana_total: -276
DBG_player_idx: 2, DBG_gold_total:   10, DBG_food_total: 40, DBG_mana_total:  -14
DBG_player_idx: 3, DBG_gold_total:   77, DBG_food_total: 62, DBG_mana_total:   30
DBG_player_idx: 4, DBG_gold_total: -195, DBG_food_total:  1, DBG_mana_total:  -20
DBG_player_idx: 5, DBG_gold_total: -154, DBG_food_total: -8, DBG_mana_total:  -92




disappearing units?
werewolves
unit_idx 79
19,15
stag beetle
unit_idx 718
19,16
stag beetle, on my node
unit_idx 696
18,12



How does it decide which Units to draw, on the main movement map?
    gets unit_idx from entities_on_movement_map[]
    if not -1 and < 1000
    Draw_Unit_Picture()

EOD: one unit_idx per stack in entities_on_movement_map[] and that is always drawn by Draw_Map_Units() |-> Draw_Unit_Picture()
So, ...
get unit_idx of disappearing Unit
see if it exists after 'Next Turn'
catch it getting killed/deleted
catch it not being in entities_on_movement_map[], anytime/everytime

## Draw_Priority
Reset_Stack_Draw_Priority() sets all units in the active stack to draw priority 0
_unit_type_table[].Melee + _unit_type_table[].Ranged
~ base strength / unit type strength
if 0, then 1
if boat, then 50
if no owner, then -1
if invisible, then -1
...
valid values: -1, 50, or [melee + ranged]
Nope.  assert(_UNITS[].Draw_Priority != 0)
assert(
    (_UNITS[].Draw_Priority == ST_UNDEFINED)
    ||
    (_UNITS[].Draw_Priority == 50)
    ||
    (
        (_UNITS[].Draw_Priority >= 1)
        &&
        (_UNITS[].Draw_Priority <= 25)
    )
);
highest known valid value is 25

