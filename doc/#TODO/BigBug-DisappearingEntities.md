Sometimes, on the main movement map, stacks are not drawn. might be specific to enemy stacks. might be just after the human player moves. probably something with #entities_on_movement_map. review the code in that area and see if your can find a cause
Grep "entities_on_movement_map"
19 lines of output
Grep "entities_on_movement_map" (in c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c)
19 lines of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 701-900)
Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 601-700)
Now let me look at the draw code that reads from entities_on_movement_map to render stacks.

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 3041-3140)
Now let me look at the Draw_Priority logic more carefully. The key question is what happens when the active stack moves — does Set_Entities_On_Map_Window get called to refresh after movement?

Grep "Draw_Priority" (in c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c)
11 lines of output
Grep "Set_Entities_On_Map_Window"
83 lines of output
Now let me look at the Draw_Priority logic more closely — specifically Set_Unit_Draw_Priority and Reset_Stack_Draw_Priority.

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 959-1038)
Now I see the issue. Let me look at the draw-map function that calls these and look at the draw_active_stack_flag interaction.

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 431-470)
Now I see the critical sequence: Set_Unit_Draw_Priority() → Reset_Stack_Draw_Priority() → Set_Entities_On_Map_Window(). Let me look more closely at the filtering logic inside Set_Entities_On_Map_Window, specifically lines 777-803.

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 778-807)
Now let me check ST_UNDEFINED value and what happens with Draw_Priority comparisons.

Grep "ST_UNDEFINED" (glob: **/MOX_DEF.h)
4 lines of output
Grep "#define ST_UNDEFINED"
1 line of output
Grep "UNITS_DRAW_PRIORITY"
6 lines of output
Read c:\STU\devel\ReMoM\MoX\src\MOX_UPD.h (lines 166-175)
Now let me check the Draw_Priority field type.

Grep "Draw_Priority" (glob: **/*.h)
6 lines of output
Draw_Priority is int8_t. Now I see the bug. Let me verify by looking at Set_Unit_Draw_Priority again carefully:

Here's the problem. In Set_Unit_Draw_Priority (line 958), when a unit's owner is ST_UNDEFINED (-1) or it has invisibility and isn't the human player, draw_priority gets set to ST_UNDEFINED (-1). Then at line 992 it's cast to int8_t and stored. So dead/invisible units get Draw_Priority = -1.

Now look at the filtering in Set_Entities_On_Map_Window:

Line 779: if(_UNITS[itr_units].Draw_Priority > _UNITS[prior_entity_idx].Draw_Priority) — When a slot already has an entity (prior_entity_idx != ST_UNDEFINED), a new unit only gets placed if its priority is strictly greater. But prior_entity_idx here is the unit index from entities_on_movement_map, not ST_UNDEFINED the constant. So if the prior entity at that map cell is a unit whose Draw_Priority happens to be -1 (dead/ownerless), a living enemy unit with priority 1 would correctly replace it. That path seems fine.

Line 788: When the cell is empty (prior_entity_idx == ST_UNDEFINED), units are only added if Draw_Priority > 0. The active stack units had their priority reset to 0 by Reset_Stack_Draw_Priority. They only get drawn if draw_active_stack_flag == -1 (line 796).

Here's the actual bug — enemy stacks sharing a square with the active player stack get suppressed. Here's how:

Reset_Stack_Draw_Priority() sets all units in _unit_stack to Draw_Priority = 0
Set_Entities_On_Map_Window() iterates all units. The active stack's units have priority 0, so they only go in if draw_active_stack_flag == -1
An enemy unit on the same square has Draw_Priority > 0, so it enters at line 792
But if the active stack unit was placed first (because draw_active_stack_flag == -1), then at line 779, the enemy unit's priority is compared to the active stack unit's priority of 0. Since the enemy has priority > 0, it replaces the active stack unit — that's fine
However, if iterating in unit index order, the enemy unit might be placed first (priority > 0, line 788), and then the active stack unit (priority 0) hits line 779: 0 > enemy_priority is false, so the active stack unit does NOT replace the enemy. This is correct behavior — enemy is shown.
Let me re-examine more carefully. The symptom is enemy stacks disappearing, specifically after the human player moves.

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 1121-1190)
Now let me check the Add_Nodes function — there was something suspicious at line 898.

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 881-945)
Found a bug at line 898, but that's for nodes, not units. Let me focus back on the actual unit drawing issue. Let me look at where the map is drawn after the player moves — particularly the main draw loop.

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 341-460)
Now let me check draw_active_stack_flag — what resets it and who manages it.

Grep "draw_active_stack_flag" (in c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c)
14 lines of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 531-570)
Grep "Reset_Draw_Active_Stack" (glob: **/*.c)
22 lines of output
Now let me look at the movement path code in MainScr.c to see what happens after the human player moves a stack.

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 1221-1265)
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 1381-1440)
Now let me look at what happens after normal movement — when the player clicks on a square to move to.

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 1701-1760)
Now let me look at the left-click movement — when the player clicks to move their stack to a destination.

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 1461-1580)
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 1581-1700)
Now let me find Move_Stack and Main_Screen_Reset:

Grep "void Main_Screen_Reset" (glob: **/*.c)
1 line of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 2178-2202)
OK, the Main_Screen_Reset sequence is: Reset_Draw_Active_Stack() (sets flag to 0 if stack exists) → Set_Unit_Draw_Priority() → Reset_Stack_Draw_Priority() (sets active stack units to 0) → Set_Entities_On_Map_Window().

Now the critical question: in Set_Entities_On_Map_Window, when draw_active_stack_flag is 0 (blinking mode), what happens to units with Draw_Priority == 0 (the active stack)?

Line 796: if( (_UNITS[itr_units].Draw_Priority == 0) && (draw_active_stack_flag == -1) ) — this requires draw_active_stack_flag == -1 (always-draw mode). But after Main_Screen_Reset, draw_active_stack_flag is 0 (blinking mode). So active stack units with Draw_Priority == 0 are NOT added to entities_on_movement_map.

This is intentional — the active stack blinks via the separate code at lines 597-660. But here's the issue:

When the active stack is NOT added to entities_on_movement_map, that cell remains ST_UNDEFINED. If an enemy unit occupies the same world square, the enemy goes through the normal path and gets added. That's fine.

But wait — let me re-read the logic. The problem might be different. Let me check: after the human player moves, does _unit_stack get updated to the next idle stack? If so, those new active stack units get Draw_Priority = 0, and any enemy units on that same square would still be handled by lines 777-784.

Actually, let me reconsider. The issue might be simpler. Let me look at line 1231 vs 1247 — the case where all_units_moved == ST_TRUE:

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 1241-1252)
Interesting — at line 1246, when all_units_moved == ST_TRUE, it calls Set_Unit_Draw_Priority() then Set_Entities_On_Map_Window() without calling Reset_Stack_Draw_Priority() in between. But Main_Screen_Reset() was just called on line 1243, which did the full sequence. Let me trace through...

Actually wait. At line 1246, Set_Unit_Draw_Priority() resets all priorities to their combat values. Then Set_Entities_On_Map_Window() is called. But Reset_Stack_Draw_Priority() is NOT called — so the active stack units still have their normal priority from Set_Unit_Draw_Priority(), not 0. And draw_active_stack_flag was set by Reset_Draw_Active_Stack() — but if all_units_moved == ST_TRUE, line 557 all_units_moved == ST_FALSE fails, so draw_active_stack_flag stays unchanged (whatever it was before).

Hmm, let me check what it was before. In Main_Screen_Reset at line 2182, Reset_Draw_Active_Stack() checks all_units_moved == ST_FALSE and _unit_stack_count > 0. If all_units_moved is already true at that point, draw_active_stack_flag doesn't get set to 0 and remains whatever it was.

Let me check what draw_active_stack_flag starts as:

Grep "draw_active_stack_flag" (glob: **/*.c)
16 lines of output
OK, draw_active_stack_flag is a global int16_t initialized to 0 by default. Let me now think about the overall flow more carefully.

Here's what I think the bug is. Let me look at the city override at lines 864-869:


if(city_in_view == ST_TRUE)
{
    SET_MAP_ENTITY(entity_mx, entity_my, (itr_cities + MAX_UNIT_COUNT));
}
Cities are placed unconditionally — they overwrite whatever unit was in that cell. So if an enemy unit is on a city tile, the city overwrites it. But the draw code at line 3072 only draws entities with unit_idx < MAX_UNIT_COUNT. Cities (idx >= MAX_UNIT_COUNT) are drawn separately. That's fine.

But wait — the city loop runs AFTER the unit loop. So if an enemy stack is on a square that also has a city, the city index overwrites the unit index, and then the draw code skips it because unit_idx >= MAX_UNIT_COUNT. The city draws separately, but the enemy unit is lost. That seems like it could be the original game's behavior though.

Let me reconsider. The user says "enemy stacks are not drawn" and "probably something with entities_on_movement_map." Let me focus on what's most likely wrong — the Draw_Priority comparison at line 779.

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 738-807)
Wait — I think I found the bug. Look at line 779:


if(_UNITS[itr_units].Draw_Priority > _UNITS[prior_entity_idx].Draw_Priority)
prior_entity_idx comes from entities_on_movement_map, which stores a unit index. But what if prior_entity_idx is not a valid unit index? The nodes code at line 937 stores (1100 + itr) into entities_on_movement_map. But Add_Nodes_To_Entities_On_Map_Window is called after Set_Entities_On_Map_Window, so that shouldn't be an issue within a single refresh.

Hmm, but the map array is initialized to ST_UNDEFINED (-1) at the start of each Set_Entities_On_Map_Window call, so stale values aren't the issue.

Let me think about this differently. The symptom is "stacks not drawn" — sometimes, probably enemy stacks, probably after the player moves.

Let me re-read the draw code at lines 3062-3083 more carefully:


entities_index = (((itr_map_y + map_start_y) * MAP_WIDTH) + (itr_map_x + map_start_x));
unit_idx = entities_on_movement_map[entities_index];
Wait — map_start_x and map_start_y are offsets. If map_grid_width == 3 (zoomed in), map_start_x = world_grid_x - _map_x. What if this results in an index that goes out of bounds of the entities_on_movement_map array?

Grep "entities_on_movement_map\[" (glob: **/MOX_DEF.h)
13 lines of output
Grep "entities_on_movement_map" (glob: **/MOM_DAT.*)
4 lines of output
The array is MAP_WIDTH * MAP_HEIGHT = 12 * 10 = 120 elements. Now let me check MAP_WIDTH/MAP_HEIGHT:

Grep "#define MAP_WIDTH|#define MAP_HEIGHT" (glob: **/*.h)
6 lines of output
Now let me look at the Check_Square_Scouted function — the scouting check at line 743 uses the original entity_wx (before world-wrap adjustment), but the scouting function needs the actual world coordinates. Let me check:

Grep "Check_Square_Scouted" (glob: **/*.c)
6 lines of output
Wait — I notice something important. At line 743:


square_is_scouted = Check_Square_Scouted(entity_wx, entity_wy, wp);
The third parameter is wp — the map window's plane. But the unit's actual plane is entity_wp (line 742). The scouting check is checking whether the unit's world position is scouted on the map window's plane. That should be correct since we're already filtering by plane on line 749.

Actually, wait. Let me re-read lines 739-743 more carefully:


entity_owner_idx = _UNITS[itr_units].owner_idx;
entity_wx = _UNITS[itr_units].wx;
entity_wy = _UNITS[itr_units].wy;
entity_wp = _UNITS[itr_units].wp;
square_is_scouted = Check_Square_Scouted(entity_wx, entity_wy, wp);
The scouting check happens before the plane check at line 749. For units on a different plane, Check_Square_Scouted(entity_wx, entity_wy, wp) checks scouting at the unit's XY but on the map window's plane, not the unit's plane. This is a bit odd but maybe intentional since we're about to filter by plane anyway.

Now, the key insight — there's no square_is_scouted check for enemy vs own units. Both go through the same scouting filter. If the enemy's square hasn't been scouted by the human player, the unit won't be added. That's correct behavior.

Let me look at this from a completely different angle. What if the issue is in the draw function, not in the entity-setting function?

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 2981-3050)
Now I see something suspicious at line 3048: if(map_start_x >= WORLD_WIDTH) — this should probably be >= MAP_WIDTH. But wait, this is for the 3x3 city map case. Let me look at the normal case:

For the normal (non-3x3) case, map_start_x = 0 and map_start_y = 0 (lines 3056-3057). The loop goes itr_map_x from 0 to map_grid_width and itr_map_y from 0 to map_grid_height. The index is (itr_map_y + 0) * MAP_WIDTH + (itr_map_x + 0) which is fine.

Let me now look at where Draw_Map_Units is called and what calls Set_Entities_On_Map_Window relative to those calls. The issue might be a timing problem — Set_Entities_On_Map_Window gets called, then the map scrolls, but the entities aren't refreshed.

Grep "Draw_Map_Units" (glob: **/*.c)
5 lines of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 2021-2060)
Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 1976-2025)
Now let me look at Main_Screen_Draw to understand the full draw path:

Grep "void Main_Screen_Draw"" (glob: **/*.c)
Grep "void Main_Screen_Draw\b" (glob: **/*.c)
1 line of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 2164-2183)
Grep "void Main_Screen_Draw_Do_Draw"
3 lines of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 3463-3542)
OK, so Draw_Maps at line 3476 is where the map window scroll/pan logic and entity setup happens. Let me now look at Draw_Maps fully — specifically the entity refresh timing:

The key flow for the Draw_Maps call path is:

Draw_Maps → checks if map moved → if so, calls Set_Unit_Draw_Priority, Reset_Stack_Draw_Priority, Set_Entities_On_Map_Window
Then Draw_Map_Window → Draw_Map_Units reads from entities_on_movement_map
The thing is, Draw_Maps scrolls the map one square at a time toward the target. So if the player moved 3 squares, it takes multiple calls to Draw_Maps before the map is fully centered. Each call updates entities. That should be fine.

Let me now re-examine the entity-placement logic one more time, focusing on a specific scenario: what happens when multiple enemy units are on the same square?

At line 777-803:

If a cell already has a unit (prior_entity_idx != ST_UNDEFINED): new unit replaces it only if new.Draw_Priority > old.Draw_Priority
If cell is empty: unit is added if Draw_Priority > 0, or if Draw_Priority == 0 and draw_active_stack_flag == -1
The problem is at line 777: prior_entity_idx != ST_UNDEFINED. Remember ST_UNDEFINED is -1. The entity map was initialized to ST_UNDEFINED (-1). But what if we're comparing against a node index? Nodes are added after Set_Entities_On_Map_Window, so that's not the issue here.

Wait — let me look more carefully. There's actually a subtle bug in the entity map initialization and the prior entity comparison. When a unit with Draw_Priority == 0 and draw_active_stack_flag != -1 is processed, the cell stays ST_UNDEFINED. That's correct — the active stack gets handled separately.

But now consider: what if the active stack units share a cell with enemy units? After Reset_Stack_Draw_Priority, the active stack units have Draw_Priority = 0. Since draw_active_stack_flag is typically 0 (blinking), these units DON'T get added to the entity map (line 796 fails).

If an enemy unit on that same square is iterated before any active stack unit, it sees prior_entity_idx == ST_UNDEFINED and gets added normally (line 788, Draw_Priority > 0 passes). Good.

If an enemy unit is iterated after an active stack unit was attempted but not placed, the cell is still ST_UNDEFINED, so the enemy also gets added normally. Good.

So sharing a cell between active stack and enemy shouldn't be the issue. Let me think about the specific scenario "after the human player moves"...

After movement, Main_Screen_Reset() calls:

Reset_Draw_Active_Stack() — sets draw_active_stack_flag = 0 (if stack exists)
Set_Unit_Draw_Priority() — recomputes all priorities
Reset_Stack_Draw_Priority() — sets active stack to priority 0
Set_Entities_On_Map_Window() — fills the entity map
Wait — step 2 is where Set_Unit_Draw_Priority is called. At line 978-981:


if(_UNITS[unit_idx].owner_idx == ST_UNDEFINED)
{
    draw_priority = ST_UNDEFINED;
}
And at 992: UNITS_DRAW_PRIORITY(unit_idx, (int8_t)draw_priority);

ST_UNDEFINED is -1, and (int8_t)(-1) is -1 as int8_t. That's fine.

But look at line 966:


draw_priority = _unit_type_table[_UNITS[unit_idx].type].Melee + _unit_type_table[_UNITS[unit_idx].type].Ranged;
What's the type of draw_priority? It's int16_t (line 960). The sum of Melee + Ranged could potentially be large for strong units. Then it's cast to int8_t at line 992. If the sum exceeds 127, the int8_t cast wraps to negative! A unit with Melee 70 + Ranged 70 = 140 would become -116 as int8_t.

That's the bug. If a powerful enemy unit has a combined Melee + Ranged attack > 127, its Draw_Priority wraps to a negative value. Then at line 788:


if(_UNITS[itr_units].Draw_Priority > 0)
This fails because Draw_Priority is negative. The unit falls through to line 796:


if( (_UNITS[itr_units].Draw_Priority == 0) && (draw_active_stack_flag == -1) )
This also fails (priority isn't 0, it's negative). The powerful enemy unit is never added to the entity map and never drawn.

But wait — would units typically have Melee + Ranged > 127? In MoM, base stats are usually small, but with hero buffs and enchantments... actually _unit_type_table gives BASE stats, not modified. Base Melee + Ranged of 128+ would be unusual.

Hmm, let me reconsider. Transport units get priority 50. If not transport, minimum is 1. Dead/invisible get -1. The int8_t range of [-128, 127] should be fine for base stats.

Let me look at this from a completely different angle. What if the issue is that entity_wx is being modified by the world-wrap logic at line 762, and this modified value leaks to the next iteration?

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 738-749)
Lines 740-742 re-read from _UNITS each iteration. The modified entity_wx at line 762 doesn't leak. Good.

OK, let me look at the scouting angle more carefully. Line 743:


square_is_scouted = Check_Square_Scouted(entity_wx, entity_wy, wp);
The third parameter is wp — the function parameter (the map window's plane). Let me check what Check_Square_Scouted does:

Grep "Check_Square_Scouted" (glob: **/*.c)
4 lines of output
Read c:\STU\devel\ReMoM\MoM\src\Explore.c (lines 74-103)
OK so Check_Square_Scouted requires both scouted AND explored. This is important — a square the player has previously explored but no longer has scouting on (no units/towers with scouting range nearby) would fail this check, and enemy units there wouldn't be drawn.

But that's likely the original game's intended behavior. Enemies disappear from the map when you can't see them.

Let me reconsider the user's symptom: "stacks are not drawn... might be specific to enemy stacks... might be just after the human player moves."

After the human player moves their stack to a new position, the scouting radius changes. Squares that were previously scouted by the old position might no longer be scouted from the new position. This is correct behavior.

But the user says it's happening in a way that seems wrong. Let me look at whether there's a timing issue with scouting updates vs entity map refresh.

Actually, let me look at a simpler theory. Let me re-read Reset_Stack_Draw_Priority:


void Reset_Stack_Draw_Priority(void)
{
    for(itr = 0; itr < _unit_stack_count; itr++)
    {
        unit_idx = _unit_stack[itr].unit_idx;
        UNITS_DRAW_PRIORITY(unit_idx, 0);
    }
}
This sets Draw_Priority = 0 for ALL units in _unit_stack — not just the player's active units. After WIZ_NextIdleStack is called, the stack is rebuilt. But what if _unit_stack includes enemy units that were in the same place?

Grep "void Select_Unit_Stack|_unit_stack_count|_unit_stack\[" (in c:\STU\devel\ReMoM\MoM\src\UNITSTK.c)
16 lines of output
Grep "void Select_Unit_Stack" (glob: **/*.c)
1 line of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 2646-2705)
Build_Unit_Stack takes player_idx as the first parameter. I bet it only includes units belonging to that player:

Grep "void Build_Unit_Stack" (glob: **/*.c)
1 line of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 2826-2885)
Yes, confirmed — Build_Unit_Stack only adds units owned by player_idx. So _unit_stack only contains the human player's units. Reset_Stack_Draw_Priority only zeroes out the human player's units. Enemy units keep their normal priorities. This is correct.

Let me look at this from yet another angle. Let me examine the CityScr.c call:

Read c:\STU\devel\ReMoM\MoM\src\CityScr.c (lines 249-263)
Notice — CityScr.c line 253 calls Set_Entities_On_Map_Window without calling Set_Unit_Draw_Priority() first! The city screen does Select_Unit_Stack → Set_Draw_Active_Stack_Always → Set_Entities_On_Map_Window. There's no Set_Unit_Draw_Priority call, so the priorities from before entering the city screen are still in effect. If Reset_Stack_Draw_Priority was called before entering, some units have priority 0.

But wait — Set_Draw_Active_Stack_Always sets draw_active_stack_flag = -1, so the check at line 796 would pass for priority-0 units. So they'd still be drawn on the city map. That's fine for the city screen.

Now when exiting the city screen:

Read c:\STU\devel\ReMoM\MoM\src\CityScr.c (lines 1626-1640)
Line 1630: Set_Entities_On_Map_Window is called without Set_Unit_Draw_Priority() first. After the city screen, draw_active_stack_flag is still -1 (always draw) from line 251. And the active stack's priorities are still 0 from Reset_Stack_Draw_Priority (called before entering city screen).

For enemy units: their Draw_Priority was set by the last Set_Unit_Draw_Priority call (before entering city screen). Those should be fine unless something changed during the city screen.

But here's the issue at line 1630 — if the player built new units in the city, those units' Draw_Priority hasn't been set by Set_Unit_Draw_Priority(). New units would have whatever default Draw_Priority value they were initialized with (probably 0). At line 788, Draw_Priority > 0 fails. At line 796, Draw_Priority == 0 passes since draw_active_stack_flag == -1. So they'd be drawn. OK.

Hmm. Let me think about this more systematically. Let me look specifically at the Move_Stack flow and what happens afterwards:

Grep "void Move_Stack\b" (glob: **/*.c)
1 line of output
Grep "Move_Stack\(" (glob: **/*.h)
5 lines of output
Grep "int16_t Move_Stack\("" (glob: **/*.c)
Grep "^int16_t Move_Stack" (glob: **/*.c)
1 line of output
Read c:\STU\devel\ReMoM\MoM\src\UNITSTK.c (lines 211-300)
OK! Now I see something interesting. In Move_Stack at lines 284-288:


Set_Unit_Draw_Priority();
Reset_Stack_Draw_Priority();
Set_Entities_On_Map_Window(*map_x, *map_y, *map_p);
// TODO  fxnptr_o59p();
Reset_Draw_Active_Stack();
Reset_Draw_Active_Stack() is called AFTER Set_Entities_On_Map_Window(). But Reset_Draw_Active_Stack() sets draw_active_stack_flag = 0. At this point, entities_on_movement_map was built with whatever draw_active_stack_flag was at the time of Set_Entities_On_Map_Window — which was whatever it was before Move_Stack was called.

Before Move_Stack is called from line 1668, what is draw_active_stack_flag? It could be 0 (blinking), or it could have been incremented by the draw cycle.

But the real question is: after Move_Stack returns, Main_Screen_Reset() is called (line 1675), which does the full sequence again including Reset_Draw_Active_Stack + Set_Unit_Draw_Priority + Reset_Stack_Draw_Priority + Set_Entities_On_Map_Window. So the entities get rebuilt correctly.

Wait, let me re-read the movement flow:


Move_Stack(...)       // line 1668 — rebuilds entities inside
Main_Screen_Reset()   // line 1675 — rebuilds entities again
Both rebuild entities. The final state should be correct. Unless...

Let me look at WIZ_NextIdleStack:

Grep "void WIZ_NextIdleStack" (glob: **/*.c)
1 line of output
Read c:\STU\devel\ReMoM\MoM\src\UNITSTK.c (lines 473-582)
Line 569-570: When all_units_moved == ST_TRUE, it calls Set_Draw_Active_Stack_Always() (flag = -1) then Set_Entities_On_Map_Window without calling Set_Unit_Draw_Priority() first.

But this is after Move_Stack at line 277 already called WIZ_NextIdleStack, which then returns. Then Move_Stack at line 284 calls Set_Unit_Draw_Priority() → Reset_Stack_Draw_Priority() → Set_Entities_On_Map_Window(). So the entities are rebuilt with fresh priorities.

OK, I've been going deep. Let me step back and look at the most suspicious code path from a higher level. Let me check Move_Stack_DirKey since that's a common way to move:

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 2197-2256)
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 2257-2276)
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 2275-2304)
Move_Stack_DirKey calls Move_Stack but does NOT call Main_Screen_Reset() afterwards! And the caller at line 1559 just sets screen_changed = ST_TRUE. Let me check what screen_changed does:

Grep "screen_changed" (in c:\STU\devel\ReMoM\MoM\src\MainScr.c)
23 lines of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 1881-1930)
So when screen_changed == ST_FALSE, Main_Screen_Draw() gets called which redraws everything. When screen_changed == ST_TRUE (which is the case after direction key movement), Main_Screen_Draw() is skipped, and the loop goes back to the next input poll.

Wait — but screen_changed gets reset to ST_FALSE at line 1922, so on the NEXT iteration, Main_Screen_Draw() will be called... unless another input triggers screen_changed = ST_TRUE again.

Actually wait — the auto function Assign_Auto_Function(Main_Screen_Draw, 1) is what normally keeps the screen refreshing. After Move_Stack_DirKey, no Assign_Auto_Function is called. The Main_Screen_Draw call at line 1889 happens on the next loop iteration (when screen_changed has been reset to FALSE).

But here's the thing: Move_Stack internally calls Set_Entities_On_Map_Window already (line 286 of UNITSTK.c). So after a direction key move, the entity map is already refreshed. Then on the next draw loop, Main_Screen_Draw → Draw_Maps → if map needs scrolling, it calls Set_Entities_On_Map_Window again.

Let me now check if Move_Stack_DirKey or the left-click path are actually missing a Main_Screen_Reset call. The left-click path at line 1668-1675:


Move_Stack(...)          // rebuilds entities inside  
Main_Screen_Reset()      // rebuilds entities again
The direction key path at line 1559:


Move_Stack_DirKey(8)     // calls Move_Stack → rebuilds entities
screen_changed = TRUE    // skips this frame's draw
// next frame: Main_Screen_Draw → Draw_Maps → may rebuild entities
There's no Main_Screen_Reset() after Move_Stack_DirKey! The left-click move path calls Main_Screen_Reset (line 1675), but the direction key path doesn't.

But Move_Stack internally does Set_Unit_Draw_Priority + Reset_Stack_Draw_Priority + Set_Entities_On_Map_Window + Reset_Draw_Active_Stack. So the entity map should be correct after Move_Stack_DirKey returns.

Let me re-examine the Move_Stack internal flow one more time. After Move_Units runs, the stack might have moved. Then Select_Unit_Stack rebuilds _unit_stack at the new position. Then if movement points are exhausted, WIZ_NextIdleStack finds the next stack. Then:


Set_Unit_Draw_Priority();      // all units get priorities
Reset_Stack_Draw_Priority();   // active stack → 0
Set_Entities_On_Map_Window(*map_x, *map_y, *map_p);  // builds entity map
Reset_Draw_Active_Stack();     // draw_active_stack_flag = 0 (if stack exists)
The issue: Set_Entities_On_Map_Window is called with the current draw_active_stack_flag, which at this point could be anything (it was set by the previous Reset_Draw_Active_Stack or draw cycle). The new Reset_Draw_Active_Stack() happens AFTER the entity map is built.

But since the active stack has Draw_Priority == 0, the only way they get into the entity map is if draw_active_stack_flag == -1 (line 796). If draw_active_stack_flag happens to be -1 (always-draw mode), the active stack gets placed. Then Reset_Draw_Active_Stack changes it to 0 (blinking). Now the entity map has the active stack placed, but the draw code uses the blinking logic separately. This means the active stack unit would be drawn both by the entity-map draw code AND the separate blinking draw code. That's a double-draw, but not a missing-draw.

If draw_active_stack_flag is NOT -1 (e.g., it's 0 from a previous reset), then active stack units don't get placed in the entity map. This is correct — they'll be drawn by the blinking code.

Hmm. Let me look at an entirely different theory. What if the problem is in Set_Entities_On_Map_Window being called with the wrong *map_x, *map_y? After WIZ_NextIdleStack, the map might have centered on the new stack's position, but what if *map_x still represents the old position?

Actually, let me look at Move_Stack lines 261 vs 277 again:


Select_Unit_Stack(player_idx, map_x, map_y, *map_p, unit_x, unit_y);  // line 261
...
WIZ_NextIdleStack(player_idx, map_x, map_y, map_p);  // line 277
Both of these call Select_Unit_Stack which may update *map_x, *map_y to center on the new stack. Then line 286: Set_Entities_On_Map_Window(*map_x, *map_y, *map_p) uses the updated values. That should be correct.

OK let me look at the problem from a completely different angle. Let me check Check_Square_Scouted — specifically whether square_scouted_p0/p1 is being updated correctly after movement. If scouting data is stale...

Grep "square_scouted_p0|square_scouted_p1" (glob: **/*.c)
16 lines of output
Read c:\STU\devel\ReMoM\MoM\src\Explore.c (lines 171-230)
Read c:\STU\devel\ReMoM\MoM\src\Explore.c (lines 227-306)
This is critical. Update_Scouted_And_Contacted__WIP is called to recalculate scouting. It clears ALL scouted flags and rebuilds them from unit positions and city positions. Let me find when this is called:

Grep "Update_Scouted_And_Contacted"" (glob: **/*.c)
1 line of output
Grep "Update_Scouted_And_Contacted" (glob: **/*.c)
11 lines of output
Now let me check: is Update_Scouted_And_Contacted__WIP called during Move_Units (the actual unit position update)?

Grep "Update_Scouted|void Move_Units" (in c:\STU\devel\ReMoM\MoM\src\WZD_o059.c)
1 line of output
Read c:\STU\devel\ReMoM\MoM\src\WZD_o059.c (lines 541-555)
That's just the Nature's Awareness spell. Update_Scouted_And_Contacted__WIP is NOT called during Move_Units. Let me check:

Grep "void Move_Units"" (glob: **/*.c)
1 line of output
Grep "^void Move_Units|^int16_t Move_Units" (glob: **/*.c)
2 lines of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 5005-5104)
Notice line 5047: Set_Draw_Active_Stack_Always() is called at the start of Move_Units. This sets draw_active_stack_flag = -1. Now let me look at Move_Units_Draw:

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 5528-5657)
Found it! Line 5637-5639:


Set_Unit_Draw_Priority();

Set_Entities_On_Map_Window(*map_x, *map_y, map_p);
Set_Unit_Draw_Priority() is called, but Reset_Stack_Draw_Priority() is NOT called before Set_Entities_On_Map_Window().

At this point, draw_active_stack_flag is -1 (set at line 5047 by Set_Draw_Active_Stack_Always()). And without Reset_Stack_Draw_Priority(), the active stack units still have their normal combat-based priorities from Set_Unit_Draw_Priority() (> 0). So they'll be placed in the entity map at line 788 (priority > 0, cell empty).

Now here's the key: the active stack is moving. Their world position has already been updated by the path step. So they're placed on their NEW square. Any enemy unit on that same square would only replace them if the enemy's priority is higher (line 779).

But wait — the real issue might be different. Let me look at the HACK at line 5626:


/* HACK */  display_moves = ST_TRUE;
This forces display_moves = ST_TRUE for ALL players, including AI. This means AI unit movement is always displayed. Let me check if this is intentional or a debug leftover...

Actually, let me refocus. Let me look at line 5626 carefully. The display_moves logic (lines 5568-5621) computes whether to show the movement animation. For human player, always true. For AI player, only if the path crosses a scouted square AND the units aren't all invisible.

But line 5626 overrides all of that with display_moves = ST_TRUE. This is labeled as /* HACK */. This might be causing display issues — AI movement animations playing when they shouldn't, but that's not the missing-stacks bug.

Let me look more carefully at Move_Units_Draw, specifically what happens after the movement animation:

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 5657-5776)
Lines 5691 and 5701 set ALL moving units' Draw_Priority to 0. Then at line 5735-5736:


Set_Draw_Active_Stack_Never();
Set_Entities_On_Map_Window(*map_x, *map_y, map_p);
Set_Draw_Active_Stack_Never() sets draw_active_stack_flag = -2. Then Set_Entities_On_Map_Window is called. The moving units have Draw_Priority = 0. With draw_active_stack_flag = -2:

Line 788: Draw_Priority > 0 → FALSE (priority is 0)
Line 796: Draw_Priority == 0 && draw_active_stack_flag == -1 → FALSE (flag is -2)
So the moving units are NOT placed in the entity map. This is intentional — during animation, the units are drawn separately with animation. But the entity map persists after this.

Now after the animation loop, what happens? Let me read ahead:

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 5851-5910)
I see that at line 5864, Set_Entities_On_Map_Window is called again when the unit scrolls off-screen edge, but still with draw_active_stack_flag = -2, so the moving units still won't be in the entity map.

Let me see what happens at the END of Move_Units_Draw:

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 5911-5970)
Move_Units_Draw ends at line 5959 with just Reset_Window() and Reset_Map_Draw(). It does NOT call Set_Entities_On_Map_Window at the end. And draw_active_stack_flag is still -2 (never draw).

Now, after Move_Units_Draw returns, control goes back to Move_Units. Let me find where Move_Units_Draw is called from:

Grep "Move_Units_Draw\("" (in c:\STU\devel\ReMoM\MoM\src\MainScr.c)
1 line of output
Grep "Move_Units_Draw\("" (glob: **/*.c)
1 line of output
Grep "Move_Units_Draw" (in c:\STU\devel\ReMoM\MoM\src\MainScr.c)
3 lines of output
Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 5481-5530)
Line 5488: Reset_Draw_Active_Stack() is called after the path traversal. Then at line 5492, Update_Scouted_And_Contacted__WIP() updates scouting for the human player.

Then Move_Units returns to Move_Stack. At Move_Stack line 284-288:


Set_Unit_Draw_Priority();
Reset_Stack_Draw_Priority();
Set_Entities_On_Map_Window(*map_x, *map_y, *map_p);
Reset_Draw_Active_Stack();
At this point, draw_active_stack_flag was set by Reset_Draw_Active_Stack() at line 5488. If all_units_moved == ST_FALSE && _unit_stack_count > 0, it becomes 0. If all_units_moved == ST_TRUE or _unit_stack_count == 0, it stays unchanged (still -2 from Set_Draw_Active_Stack_Never at line 5735).

Wait — Reset_Draw_Active_Stack at line 5488 is called BEFORE WIZ_NextIdleStack (which is at Move_Stack line 277). At line 5488, _unit_stack might still have the old stack. And all_units_moved might not be TRUE yet. So draw_active_stack_flag could become 0 at line 5488.

But then at Move_Stack line 277, WIZ_NextIdleStack is called which rebuilds _unit_stack. And at line 288, another Reset_Draw_Active_Stack() is called.

OK, this is getting very complex. Let me take a step back and think about the most likely cause of the reported bug: "enemy stacks not drawn after human player moves."

I think the most likely candidate is this sequence in Move_Units_Draw:

Line 5691/5701: All moving units get Draw_Priority = 0
Line 5735: Set_Draw_Active_Stack_Never() (flag = -2)
Line 5736: Set_Entities_On_Map_Window() — entities built with flag -2, moving units have priority 0 → they're excluded
Animation plays
Move_Units_Draw returns without resetting draw_active_stack_flag
Back in Move_Units, line 5488: Reset_Draw_Active_Stack() — may or may not change flag depending on state
Move_Units returns to Move_Stack
Move_Stack calls Set_Unit_Draw_Priority() which sets ALL units to proper priorities
BUT — if between steps 2 and 8, draw_active_stack_flag remained at -2...
Actually no, step 8 recomputes all priorities. At step 9 (line 286), Set_Entities_On_Map_Window is called. At this point, draw_active_stack_flag was set at line 5488 by Reset_Draw_Active_Stack(). If all_units_moved == FALSE && _unit_stack_count > 0, flag = 0. If not, flag stays -2.

With flag = 0 and all non-active units having proper priorities from Set_Unit_Draw_Priority, enemy units should be added normally at line 788 (Draw_Priority > 0). This should work.

With flag = -2 (if all_units_moved was already TRUE before WIZ_NextIdleStack), then... at Move_Stack line 277 WIZ_NextIdleStack is called. If the result is all_units_moved = TRUE, then at line 569 Set_Draw_Active_Stack_Always() sets flag = -1, and line 570 Set_Entities_On_Map_Window is called. But then Move_Stack line 284 calls Set_Unit_Draw_Priority() (resetting priorities to combat values) and line 286 calls Set_Entities_On_Map_Window again. At this point flag = -1. With flag = -1, line 796 allows priority-0 units. And Set_Unit_Draw_Priority gave all live units priority >= 1 (or -1 for dead). So at line 788, everyone with priority > 0 gets in. This should work.

Hmm. Let me look at a completely different possibility. What if the entities_on_movement_map array is simply too small, or the index calculation goes out of bounds and corrupts adjacent memory?

The array is MAP_WIDTH * MAP_HEIGHT = 120 elements. The init loop (lines 729-735) iterates correctly. The set calls use GET_MAP_ENTITY(entity_mx, entity_my) which expands to entities_on_movement_map[(entity_my * MAP_WIDTH) + entity_mx].

What if entity_mx or entity_my end up out of range? Let me check:

Lines 765-766:


entity_mx = (entity_wx - wx);
entity_my = (entity_wy - wy);
The bounds checks at lines 752-758 ensure:

entity_wy >= wy && entity_wy < wy + MAP_HEIGHT → entity_my in [0, MAP_HEIGHT-1] ✓
entity_wx >= wx && entity_wx < wx + MAP_WIDTH (or with world-wrap) → entity_mx in [0, MAP_WIDTH-1] ✓
But wait — at line 760-763, if the world-wrap condition is met:


if((entity_wx + WORLD_WIDTH >= wx) && (entity_wx + WORLD_WIDTH < wx + MAP_WIDTH))
{
    entity_wx = (entity_wx + WORLD_WIDTH);
}
Then entity_mx = (entity_wx + WORLD_WIDTH) - wx. Since the condition ensures entity_wx + WORLD_WIDTH < wx + MAP_WIDTH, we get entity_mx < MAP_WIDTH. And since entity_wx + WORLD_WIDTH >= wx, entity_mx >= 0. Fine.

But there's a subtle issue. The first condition at line 756 is:


((entity_wx >= wx) && (entity_wx < wx + MAP_WIDTH))
and the second at line 757 is:


((entity_wx + WORLD_WIDTH >= wx) && (entity_wx + WORLD_WIDTH < wx + MAP_WIDTH))
These are OR'd together. If the first is true, we enter the block. Then at line 760, we check the world-wrap condition again. If the first condition was true but the second was also true, entity_wx gets adjusted by WORLD_WIDTH, making it potentially huge and entity_mx way out of bounds!

Wait, can both conditions be true simultaneously? For the first: entity_wx >= wx && entity_wx < wx + MAP_WIDTH. For the second: entity_wx + WORLD_WIDTH >= wx && entity_wx + WORLD_WIDTH < wx + MAP_WIDTH. Since WORLD_WIDTH = 60 and MAP_WIDTH = 12, the second condition requires entity_wx + 60 < wx + 12, i.e., entity_wx < wx - 48. But the first condition requires entity_wx >= wx. These can't both be true (since wx >= 0). So no, they're mutually exclusive. The OR is safe.

OK. I've done a very thorough review. Let me now present my findings. I think the most likely issue is actually fairly simple and I may have been overthinking it. Let me re-read the HACK line:

Line 5626: /* HACK */  display_moves = ST_TRUE;

This forces ALL unit movements to be displayed, including AI movements. During AI movement display:

Set_Draw_Active_Stack_Never() hides the human player's active stack
Entity map is rebuilt without the active stack
AI movement animation plays
After animation, Move_Units_Draw returns with draw_active_stack_flag = -2 and all moving units at Draw_Priority = 0
But wait — this is during the AI's turn (Next Turn processing), not during the human's turn. The user's symptom is "after the human player moves."

Let me reconsider. Let me look at line 3372 in MainScr.c where Update_Scouted_And_Contacted__WIP is called:

Read c:\STU\devel\ReMoM\MoM\src\MainScr.c (lines 3361-3380)
That's the earthgate code. Let me look at the Move_Units call more carefully, specifically the human player path. After Move_Units at line 5492, Update_Scouted_And_Contacted__WIP() is called for the human player. This rebuilds scouting. Then back in Move_Stack, Set_Entities_On_Map_Window is called, and at that point scouting is up to date.

OK, I think I've been going about this the wrong way. Let me look at the actual final state after the complete Move_Stack_DirKey flow:

Move_Stack_DirKey calls Move_Stack
Inside Move_Stack:
Move_Units runs (moves units, updates scouting at line 5492)
Select_Unit_Stack rebuilds _unit_stack for the new/next stack
If out of moves: WIZ_NextIdleStack finds next idle stack
Set_Unit_Draw_Priority() — all priorities set
Reset_Stack_Draw_Priority() — active stack → 0
Set_Entities_On_Map_Window() — entity map rebuilt with correct scouting
Reset_Draw_Active_Stack() — flag → 0 (if stack exists)
Back in Move_Stack_DirKey — nothing else
Back in main loop — screen_changed = TRUE, so this frame skips Main_Screen_Draw
Next frame — Main_Screen_Draw → Main_Screen_Draw_Do_Draw → Draw_Maps → if map needs to scroll, entities rebuilt
This sequence seems correct. All enemy units that are scouted and in the visible window should be in entities_on_movement_map and drawn.

Unless... the map hasn't scrolled to center on the new stack yet, and the old window position's entity map doesn't include newly-visible enemies.

Wait — that's it! After Move_Stack at line 286, Set_Entities_On_Map_Window(*map_x, *map_y, *map_p) is called with map_x/map_y that may have been updated by WIZ_NextIdleStack to center on the NEXT idle stack. But Draw_Maps at line 3476 is called with _prev_world_x, _prev_world_y as the target position, and the map scrolls one square per frame toward it. If the next idle stack is several squares away, the map is still showing the OLD area, but entities_on_movement_map was built for the NEW area!

Let me verify — at Move_Stack line 286, *map_x, *map_y point to _map_x, _map_y (from Move_Stack_DirKey's call at line 2279). After WIZ_NextIdleStack at line 277, _map_x, _map_y have been updated to center on the next idle stack.

Then Set_Entities_On_Map_Window(_map_x, _map_y, _map_p) builds the entity map for the NEW map window position. But the screen is still showing the OLD map window position (scrolling hasn't happened yet).

When Draw_Maps runs next frame, the scroll loop moves l_map_x, l_map_y by ONE square toward _prev_world_x, _prev_world_y. Inside the loop, if the map moved, Set_Entities_On_Map_Window(l_map_x, l_map_y, map_plane) is called with the INTERMEDIATE position. So the entity map gets rebuilt correctly for each frame during scrolling.

But Draw_Map_Units reads from entities_on_movement_map at the current map_draw_curr_x/y position. If the entity map was just rebuilt for l_map_x, l_map_y, that should match. Wait, Draw_Map_Units at line 3050 gets map_draw_curr_x, map_draw_curr_y which equals map_x from Draw_Map_Window line 2013 which equals the value of l_map_x, l_map_y from Draw_Maps. And Set_Entities_On_Map_Window was called with the same l_map_x, l_map_y. So they should match.

OK I'm going in circles. Let me just look at whether there's a simple, obvious bug in the code. Let me re-examine the entities_on_movement_map filling logic one more time, focused.

Lines 777-803:


if(prior_entity_idx != ST_UNDEFINED)      // Cell already has a unit
{
    if(new.priority > old.priority)        // Replace if higher priority
        SET_MAP_ENTITY
}
else                                       // Cell is empty
{
    if(new.priority > 0)                   // Normal units
        SET_MAP_ENTITY
    else if(new.priority == 0 && flag == -1)  // Active stack only if "always draw"
        SET_MAP_ENTITY
}
The bug is on line 779. When prior_entity_idx != ST_UNDEFINED, the code does _UNITS[prior_entity_idx].Draw_Priority. But prior_entity_idx comes from entities_on_movement_map, which could be a city index (itr_cities + MAX_UNIT_COUNT). Cities are placed unconditionally at line 868 and overwrite any unit in that cell.

But wait — the city loop runs AFTER the unit loop. So when iterating units, prior_entity_idx can only be a previous unit index or ST_UNDEFINED. Cities can't interfere with the unit loop.

Unless Set_Entities_On_Map_Window is called more than once without reinitializing... but it does initialize at lines 729-735 every time.

Hmm wait, let me look one more time at the scouting call at line 743:


square_is_scouted = Check_Square_Scouted(entity_wx, entity_wy, wp);
This uses wp, the map window's plane. But what about units in towers? Line 749:


if( (_UNITS[itr_units].wp == wp) || (_UNITS[itr_units].in_tower == ST_TRUE) )
A unit in a tower on a DIFFERENT plane passes this check (because in_tower == ST_TRUE). But the scouting check at line 743 used wp (the map window's plane), not the unit's actual plane. So the scouting check is for the correct plane — the one being displayed.

Actually, wait. If a unit is in a tower on plane 1, and we're viewing plane 0, the scouting check is Check_Square_Scouted(unit_wx, unit_wy, 0). If that square on plane 0 is scouted, the unit would be shown. This seems intentional — towers connect planes, so you'd see units at tower locations on both planes.

OK, I've done an extremely thorough review. Let me summarize the most likely candidates:

Here's my analysis of the likely cause. The core Set_Entities_On_Map_Window logic is correct, but there are a couple of suspicious patterns:

Most likely: display_moves = ST_TRUE HACK at line 5626 — This forces ALL AI movement to be displayed. During AI movement animation, Set_Draw_Active_Stack_Never() is called (flag = -2) and the entity map is rebuilt without the player's active stack. After animation, the priorities of the moving AI units are zeroed. While Move_Stack eventually rebuilds everything, the HACK causes unnecessary visual disruptions.

Second candidate: Missing Update_Scouted_And_Contacted__WIP on some paths — When the player moves via direction keys, scouting IS updated inside Move_Units (line 5492), but only for the human player. The entity map rebuild at Move_Stack line 286 uses the scouting state at that moment, which should be correct.

Third candidate: The Move_Stack entity refresh order — Set_Entities_On_Map_Window is called at line 286 with potentially the NEW map position (after WIZ_NextIdleStack recentered), while Reset_Draw_Active_Stack at line 288 happens AFTER. There's a brief inconsistency window but it's overwritten by the next draw cycle.

Actually, let me re-examine the simplest explanation. The user says "might be just after the human player moves." After Move_Stack via direction keys or left-click, the flow is:

Move_Stack → Move_Units → movement animation → scouting update → Select_Unit_Stack on new position → maybe WIZ_NextIdleStack → Set_Unit_Draw_Priority → Reset_Stack_Draw_Priority → Set_Entities_On_Map_Window → Reset_Draw_Active_Stack

The Set_Entities_On_Map_Window call at Move_Stack:286 uses *map_x, *map_y which may have been shifted by WIZ_NextIdleStack to center on a completely different stack. But Draw_Maps scrolls toward this new position one square at a time, calling Set_Entities_On_Map_Window each frame with the intermediate position. So the first frame after Move_Stack returns still has the entity map built for the FINAL position, not the current display position. The draw code reads entities at the current scroll position — which may not match the entity map's window.

Wait, that's the exact flow. After Move_Stack returns:

entities_on_movement_map is built for _map_x, _map_y (the new final position)
The screen currently shows some intermediate position
Next frame: Main_Screen_Draw → Draw_Maps — the map scrolls one step, then IF the map moved, Set_Entities_On_Map_Window is called with the intermediate position
So there's exactly one frame where the entity map doesn't match the display position. During that frame, Draw_Map_Units reads from an entity map that was built for a different window — meaning the indices are offset. A unit at window position (3,4) in the old window might be at (4,4) in the new window, so entities_on_movement_map[4*12+3] would reference the wrong cell.

But actually, in Move_Stack_DirKey, screen_changed = TRUE causes the first frame after the move to SKIP Main_Screen_Draw entirely (line 1885). So that mismatch frame is never drawn. On the next frame, screen_changed = FALSE, Main_Screen_Draw runs, Draw_Maps scrolls and rebuilds entities correctly.

Actually wait — Main_Screen_Reset is NOT called after Move_Stack_DirKey. But it IS called after left-click movement (line 1675). Main_Screen_Reset calls Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane) with the globals, plus Assign_Auto_Function(Main_Screen_Draw, 1). Without Main_Screen_Reset, the auto-function might not be set up.

Let me check what Assign_Auto_Function does and whether it matters:

Grep "Assign_Auto_Function"" (glob: **/*.c)
2 lines of output
Grep "void Assign_Auto_Function" (glob: **/*.c)
2 lines of output
OK, the auto function is probably for periodic screen refresh. The left-click path calls Main_Screen_Reset which sets this up. The direction key path doesn't. But the main loop at line 1889 calls Main_Screen_Draw() directly when screen_changed == FALSE, so the auto-function may not be critical.

Let me reconsider from the top. I think the actual cause is simpler than I've been looking for. Let me check the Draw_Active_Stack_Section code (around line 597) one more time:

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 571-665)
Look at line 614: if((draw_active_stack_flag == -1) || (draw_active_stack_flag > 3)). When draw_active_stack_flag > 3 (values 4-7), the active stack is drawn. When 0-3, it's hidden (blink off).

When drawn (flag > 3 or == -1), line 632: if(draw_active_stack_flag != -1) — this is the blinking case (not always-draw). The active stack is drawn via Draw_Unit_Picture.

When draw_active_stack_flag == -1 (always-draw), line 632 is FALSE, so Draw_Unit_Picture is NOT called here. Instead, the active stack was placed in entities_on_movement_map (because flag == -1 at line 796), so it gets drawn by Draw_Map_Units.

This is the documented BUG at line 583: "contains a BUG that may unintentionally draw the stack over other units/cities already on the tile when always draw (-1) is set."

But the user's issue is stacks NOT being drawn. So:

When draw_active_stack_flag == 0 (initial blinking state):

Draw_Active_Unit_Stack: flag becomes (0+1) % 8 = 1. Since 1 <= 3, the active stack is NOT drawn (blink off phase)
entities_on_movement_map: active stack has priority 0, flag != -1, so not placed
The active stack is correctly NOT drawn in the blink-off phase
For the first 4 draw calls after reset, the stack is invisible (flag = 0→1, 1→2, 2→3, 3→4). On the 5th call, flag = 4 > 3, so it's drawn. Then 5→5, 6→6, 7→7, 0→0... wait, (7+1) % 8 = 0. So: invisible for frames 1-4, visible for frames 5-8, then repeats. That's the blink pattern.

The question is: when the active stack is invisible (blink off), is the entity at that map cell shown? The active stack was NOT placed in entities_on_movement_map. If there's an enemy unit at the same square, was it placed?

Yes — enemy units have priority > 0 and get placed normally. They show up even when the active stack is blinking off.

But what about other friendly units at the same square that aren't in _unit_stack? All units at the same square belonging to the human player are in _unit_stack (from Build_Unit_Stack). So all of them get priority 0 from Reset_Stack_Draw_Priority. None of them are in the entity map when flag != -1. During blink-on, only the first active unit is drawn by Draw_Active_Unit_Stack.

So friendly stacks at the active location disappear during blink-off. Enemy units there remain visible. This is expected MoM behavior.

I think the actual bug might be more subtle. Let me check one more thing — what if _unit_stack_count is 0 but draw_active_stack_flag hasn't been updated?

Read c:\STU\devel\ReMoM\MoM\src\MainScr_Maps.c (lines 555-566)
When all_units_moved == TRUE or _unit_stack_count == 0, Reset_Draw_Active_Stack does NOT change draw_active_stack_flag. If it was -2 from Set_Draw_Active_Stack_Never, it stays -2. Then Draw_Active_Unit_Stack at line 597 checks flag != -2 and exits early. The active stack is not drawn. And in Set_Entities_On_Map_Window, the active stack (priority 0) at line 796 checks flag == -1 which is FALSE (-2 != -1), so not placed. If Set_Unit_Draw_Priority was called, the units have priority > 0 and get placed normally. But if Reset_Stack_Draw_Priority was also called, they're back to 0 and DON'T get placed.

Here's a scenario that could cause missing units:

Move_Units is called, which calls Set_Draw_Active_Stack_Never() (flag = -2)
Move_Units calls Move_Units_Draw which zeros moving units' priorities
Move_Units calls Reset_Draw_Active_Stack() at line 5488 — but if all_units_moved == TRUE or _unit_stack_count == 0, flag stays -2
Back in Move_Stack, Set_Unit_Draw_Priority() resets all priorities to combat values
Reset_Stack_Draw_Priority() sets active stack to 0
Set_Entities_On_Map_Window() — flag is still -2
Active stack units: priority 0, line 796 checks flag == -1, FALSE → NOT placed
BUT this is correct! The active stack shouldn't be in the entity map when it's blinking
Reset_Draw_Active_Stack() at line 288 — if all_units_moved == FALSE && _unit_stack_count > 0, flag → 0. Now blinking works.
But what if WIZ_NextIdleStack (line 277) set all_units_moved = TRUE? Then:

At line 569: Set_Draw_Active_Stack_Always() → flag = -1
At line 570: Set_Entities_On_Map_Window() — flag is -1, active stack (priority 0) placed
Back in Move_Stack line 284: Set_Unit_Draw_Priority() — all priorities reset to combat values
Line 285: Reset_Stack_Draw_Priority() — active stack → 0. But wait, _unit_stack_count was set to 0 at line 568! So Reset_Stack_Draw_Priority iterates 0 units and does nothing.
Line 286: Set_Entities_On_Map_Window() — flag is still -1 (not changed since line 569). With flag -1 and all units having combat priorities (> 0), everyone gets placed at line 788. Including formerly-active-stack units (which have their combat priorities since Reset_Stack_Draw_Priority did nothing). This is fine.
Line 288: Reset_Draw_Active_Stack() — all_units_moved == TRUE, so flag stays -1.
End state: all units placed, flag = -1, _unit_stack_count = 0. Draw_Active_Unit_Stack at line 599 checks _UNITS[_unit_stack[0].unit_idx].owner_idx. With _unit_stack_count = 0, _unit_stack[0] has stale data. This could be a bug (accessing stale unit index), but since _unit_stack_count = 0, the main loop's unit window code skips it.

I think at this point I need to present my findings to the user. Let me compile the most likely candidates.

Here's what I found after a thorough review:

Most likely cause: the /* HACK */ display_moves = ST_TRUE; at MainScr.c:5626

This forces ALL unit movements to be displayed (including AI), overriding the normal display_moves logic. During movement animation:

Line 5735: Set_Draw_Active_Stack_Never() sets draw_active_stack_flag = -2
Lines 5691/5701: All moving units get Draw_Priority = 0
Line 5736: Set_Entities_On_Map_Window() is called — but with flag = -2 and priorities = 0, the moving units are excluded from the entity map
After the animation, Move_Units_Draw returns without restoring draw_active_stack_flag or unit priorities. While Move_Stack eventually rebuilds everything (lines 284-288), there's a window where the entity map is stale.

Secondary suspect: ordering issue in Move_Stack at UNITSTK.c:284-288


Set_Unit_Draw_Priority();         // recompute priorities
Reset_Stack_Draw_Priority();      // active stack → 0
Set_Entities_On_Map_Window(...);  // build entity map
Reset_Draw_Active_Stack();        // flag → 0 (AFTER entity map built)
Reset_Draw_Active_Stack() runs AFTER Set_Entities_On_Map_Window(). The entity map is built with whatever draw_active_stack_flag was before — potentially -2 (never draw) from the animation phase. With flag = -2, the line 796 check (flag == -1) fails, so active-stack units (priority 0) aren't placed. But here's the key: Reset_Draw_Active_Stack at line 288 then sets the flag to 0 (blinking), and on the next draw cycle, Draw_Active_Unit_Stack handles the blink. But enemy units sharing that map cell ARE placed (they have priority > 0), so this doesn't explain missing enemy stacks.

Third suspect: Missing scouting update on some code paths

Update_Scouted_And_Contacted__WIP() is only called inside Move_Units for the human player (line 5492). If there's a path that rebuilds the entity map without updating scouting first, enemy units on newly-unscouted squares would disappear. But I couldn't find such a path after normal movement.

My best guess: The HACK at line 5626 is the root cause. It was likely added for debugging and causes AI movement animations to interfere with the entity map state. When an AI stack moves through a scouted area, the animation sequence clobbers draw_active_stack_flag and unit priorities. If the human player's turn-end triggers AI movement (Next Turn), the entity map can be left in a bad state when control returns to the human player's next turn.

Want me to investigate the HACK line further, or try removing it to see if that fixes the issue?
