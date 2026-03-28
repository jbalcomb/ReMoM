
NEWG_CreateCapitals()  ==> NEWG_CreateCapitals__WIP()
NEWG_CreateCapitals__WIP()  ==>  Generate_Home_City()


MAPGEN.c
// MGC o51p05
// drake178: NEWG_CreateCapitals()
/*
; PATCHED / rewritten in the worldgen customizer
;
; creates the fortress and corresponding capital city
; records for each player, along with their basic
; starting units
;
; BUG: the condition to allow AI high elves only on
;  Forest map squares can restart the whole fortress
;  generation process, yielding capitals closer to
;  each other or other objects than intended
*/
/*
SEEALSO: MoX-NewGame-Capitols.md

MoO2
MoDule: HOMEGEN
Generate_Home_Worlds_()
Generate_Home_Worlds_1_()

Randomize_Home_Worlds_()
Modify_Home_Worlds_()

Assign_Home_Star_Names_()

Init_Homeworld_Colony_()
    |-> Init_Homeworld_Colony2_()
        |-> Create_Ship_()

Module: ERICNET
    code (0 bytes) Init_Homeworld_Colony2_
        Address: 01:00013A3D
    code (0 bytes) Init_Homeworld_Colony_
        Address: 01:00013FA7


Init_New_Game_()

movsx   eax, si
push    eax
push    offset aMapD                    ; "Map:  %d"
push    17h
push    0
call    __wcpp_1_unwind_leave_
add     esp, 10h


push    dword ptr _settings.IDK_random_seed
push    offset aSeedLd                  ; "Seed: %ld"
push    24
push    0
mov     [ebp+IDK_Debug_Exit_Done], 1
call    __wcpp_1_unwind_leave_
add     esp, 10h


xor     edi, edi
inc     esi
call    Screen_Flic_Capture

Change_Home_City_Name_Popup(HUMAN_PLAYER_IDX);

The largest section of the main movement screen is occupied by the
main movement view. Here you can see a small town with a flag on top
that represents your starting city (aka “enchanted fortress”).

*/
/**
 * @brief Generates starting home cities (fortresses) for all players.
 *
 * Selects candidate fortress locations on the appropriate plane for each
 * wizard, applies minimum-distance constraints from other key world sites,
 * retries placement under progressively relaxed distance limits, and then
 * initializes the resulting starting city/fortress state.
 */
void Generate_Home_City__WIP(void)





20260205: Retry the unraveling...

Loops, in code structure order
    #1  Loop Distance               1 path  if(Tries_Per_Distance >= 1000) goto Loop_Distance;
    #2  Loop MaxPop                 1 path  if(max_pop_failures > 500)        goto Loop_Capital;
    #3  Players                     1 path  for(player_idx = 0; player_idx < 6; player_idx++)



player loop for Fortress looks like a legit For-Loop

max_pop_failures gets set to 0 as part of Loop_Distance, along with Tries_Per_Distance



max_pop_failures = 0;
Do {

} while(max_pop_failures++ <= 500)



Definitely after all of the looping...
    cities are set
    city index is player index
    add #1 unit
    add #2 unit





// MGC o51p05
// drake178: NEWG_CreateCapitals()
/*
; PATCHED / rewritten in the worldgen customizer
;
; creates the fortress and corresponding capital city
; records for each player, along with their basic
; starting units
;
; BUG: the condition to allow AI high elves only on
;  Forest tiles can restart the whole fortress
;  generation process, yielding capitals closer to
;  each other or other objects than intended
*/
/*

*/
void NEWG_CreateCapitals__WIP(void)




## NEWG_CreateCapitals__WIP()

looping issues

1st Loop:
    Tries_Per_Distance < 1000
2nd loop:
    player_idx < 6


@@Loop_Distances:

@@Loop_max_pop_failures:

@@Loop_Players:



Minimum_Fortress_Distance = 16;
Min_Node_Distance_Double = 8;
Minimum_Fortress_Distance--;
Min_Node_Distance_Double--;
SETMIN(Min_Node_Distance_Double, 1);
SETMIN(Minimum_Fortress_Distance, 10);

Tries_Per_Distance = 0;

max_pop_failures = 0;



The Fortress code has to be entirely before the Cities code, because the Cities code uses the wx,wy,wp of the Fortress.

From here, ...
    `for(_cities = 0, itr = 0; itr < _num_players; _cities++, itr++)`
does it loop back up?
If not, this is after all of Minimum_Fortress_Distance, Min_Node_Distance_Double, Tries_Per_Distance, and max_pop_failures.
Yeah, ...
...this is the part where it fucks off because of the High Elf and Forest square.
And, it's right after this that it does all the invalidation checks.



            /*
            HERE:
            fucks off because it requires a Forest square for High Elves
            definitely is not just falling through the loop
            otherwise, it does through the balance of the loop in a perfectly normal looking manner
            */
            if(
                (_CITIES[_cities].race == rt_High_Elf)
                &&
                (TILE_IsForest(wx, wy, wp) == ST_FALSE)
            )
            {

                goto IDK_TryAgain;

            }
Where did wx,wy,wp get set?



@@AddFortress
