
SEEALSO: MoM-AI-Continents.md
SEEALSO: MoM-NextTurn-AI.md

probably just 'AI Turn', not realy 'Next Turn'

AI_Pick_Action_Conts__WIP()  ==>  AI_Choose_War_Landmass()
_ai_landmass_war_targets  ==>  _ai_landmass_war_targets

I renamed AI_Pick_Action_Conts__WIP to AI_Choose_War_Landmass and _ai_landmass_war_targets to _ai_landmass_war_targets.
For BUG-B, I double-checked the disassembly. Looks like the comment and Gemini were wrong. It's definitely if this player's fortress is not active or if this landmass is this player's home continent.




_ai_landmass_war_targets[]   populated in AI_Choose_War_Landmass 
AI_NewColConts[]    populated in AI_Evaluate_Continents(); // ; choose the next colony continent from among those with no presence, or clear it if no such ones exist



_ai_reevaluate_continents_countdown
used by AI_Evaluate_Continents[]
...includes override utilizing g_ai_evaluation_map[]

AI_Evaluate_Continents()
AI_Reevaluate_Continent()
...last proc in ovr158 ...later addition?

OON XREF: AI_Set_Unit_Orders() |-> AI_Stacks_Stage_Expedition_Forces() |-> AI_Reevaluate_Continent()

AI_Set_Unit_Orders()
    |-> AI_Stacks_Stage_Expedition_Forces()
        |-> AI_Reevaluate_Continent()



AI_Choose_War_Landmass
    _ai_landmass_war_targets[wp][player_idx] = Get_Weighted_Choice(&final_landmass_weights[0], NUM_LANDMASSES);
weighted choice
    
    vs.
    count of map squares, minimum 10


_ai_landmass_strength_ratios[][]



        AI_Player_Calculate_Target_Values(player_idx);

        AI_Landmass_Values_And_Strengths(player_idx);  // uses the arrays just populated in AI_Player_Calculate_Target_Values()

        AI_Choose_War_Landmassayer_idx);  // uses the arrays just populated in AI_Landmass_Values_And_Strengths()


AIMOVE.c
AI_Choose_War_Landmass
AIMOVE-AI_Choose_War_Landmass
AI_Next_Turn()
AI_Choose_War_Landmassayer_idx);  /* populates _ai_landmass_war_targets[]; uses the arrays just populated in AI_Landmass_Values_And_Strengths() */


AI_Reevaluate_Continent()

## Landmass Type/Status:
    0: lmt_Unevaluated
        only used as the initialization value
    1: lmt_Own
        Decisively held — own unit-cost > 10 × enemy unit-cost. Not "I'm the only one here," just "I'm overwhelmingly stronger." */
    2: lmt_Contested
        Own vs. Contested;  Own city + non-trivial enemy unit-cost
    3: lmt_NoPresence
        just means the player does not have any cities on the landmass
    4: lmt_NoLanding
        Allied territory OR No stage square
    5: lmt_Leaveable
        Embarkation square was found — set when a dock square is reachable. Logically "we can depart from here," not "we have decided to abandon."
    6: lmt_NoTargets
        No attackable enemies, lairs, or nodes worth targeting.

_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Leaveable;

Here's the VS Code regex. Enable the .* (regex) toggle in the search bar.
Strict — only the fully-qualified path:
_ai_continents\.plane\[[^\]]+\]\.player\[[^\]]+\]\.type_array\[[^\]]+\]\s*[+\-*/%&|^]?=(?!=)
What it does:
_ai_continents\.plane\[[^\]]+\]\.player\[[^\]]+\]\.type_array\[[^\]]+\] — matches the full path with any single index expression inside each [ ]
\s* — optional whitespace before the operator
[+\-*/%&|^]? — optional compound-assignment prefix (catches +=, -=, *=, /=, |=, etc.)
=(?!=) — an = that is NOT followed by another = (excludes == comparisons)
This catches both plain = lmt_Own and counter-style += 1. It deliberately does NOT catch ++ or --, because I didn't see those used on type_array in AIMOVE.c — let me know if you want them added.
Broader — any pointer or pointer-aliased path ending in type_array (useful if anywhere uses s_LANDMASSES *lms; lms->type_array[i] = ...):
\btype_array\[[^\]]+\]\s*[+\-*/%&|^]?=(?!=)
The \b word boundary prevents false matches on identifiers that happen to end in type_array. This would catch any future code that goes through a struct pointer alias.
Quick sanity-check against what I'd expect it to find — the writes from my earlier grep:
AIMOVE.c lines 1806, 2757, 6027, 6031, 6036, 6056, 6061, 6063, 6134, 7013, 7161, 7167, 7175, 7206, 7215, 7217, 7387, 7470 — all should hit
And += 1 counter sites at lines 6011 and 7072 should also hit (the strict form will catch them via the optional + prefix).
Reads (the ==, !=, >=, <, etc. comparisons) won't match because of the (?!=) lookahead — and for !=/>=/< the path is followed by those operators, not by = alone.
Want a read-only version too (find all places type_array is consulted but not assigned)?


## _ai_landmass_war_targets[]
(12 values; 2 planes * 6 players)

AI_Evaluate_Continents()
AI_Reevaluate_Continent()

AI_Choose_War_Landmass()

_ai_landmass_war_targets[wp][player_idx] holds the landmass index of the AI's main war continent on plane wp for this turn
(set fresh each turn by AI_Choose_War_Landmasshich runs earlier in AI_Turn)

XREF:
    AI_Evaluate_Continents+D94    mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Evaluate_Continents+DE2    mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Choose_War_Landmass    mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Choose_War_LandmassC   mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Choose_War_Landmass2   mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Wartime_Ocean_Movement_And_Cleanup+3B         mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Wartime_Ocean_Movement_And_Cleanup+6B         mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Wartime_Ocean_Movement_And_Cleanup+D          mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Roamers_Target_Or_Deploy+148      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Roamers_Target_Or_Deploy+15F      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Roamers_Target_Or_Deploy+187      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Order_To_War_Landmass+119      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Order_To_War_Landmass+145      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Order_To_War_Landmass+8C       mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Order_To_War_Landmass+F        mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Reevaluate_Continent+8DF   mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Ocean_Landmass_Orders+488 mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Ocean_Landmass_Orders+774 mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Stacks_Ocean_Landmass_Orders+798 mov     bx, [_ai_landmass_war_targets+bx]                                

Allocate_Data_Space()
    mov     [_ai_landmass_war_targets], (offset _players.spells_list+17E8h)
    mov     [_ai_landmass_war_targets+2], (offset _players.spells_list+17F4h)


AI_Reevaluate_Continent() :: Phase 7 :: _ai_landmass_war_targets[]




## _ai_landmass_strength_ratios[][]

indexed by plane and landmass

allocated in Allocate_AI_Data()

populated in AI_Landmass_Values_And_Strengths()
 * @brief Aggregates per-landmass military strength and strategic value totals for one AI player.
 * Using the city, garrison, and stack valuation data produced by @c AI_Player_Calculate_Target_Values(), ...


Usage:
AI_Set_Unit_Orders(itr_players)
    |-> AI_Balance_Upkeep(itr_players)
            if(_ai_landmass_strength_ratios[Unit_Plane][Unit_Landmass] == 0)
                Unit_Value *= 2;

XREF:
    AI_Continent_Eval+4C5      mov     bx, [_ai_landmass_strength_ratios+bx]        ; 120 LBX_NearAlloc_Next bytes
    AI_Balance_Upkeep__WIP+116 mov     bx, [_ai_landmass_strength_ratios+bx]        ; 120 LBX_NearAlloc_Next bytes
    AI_Balance_Upkeep__WIP+29A mov     bx, [_ai_landmass_strength_ratios+bx]        ; 120 LBX_NearAlloc_Next bytes
    sub_F6CAB+6D               mov     bx, [_ai_landmass_strength_ratios+bx]        ; 120 LBX_NearAlloc_Next bytes
    Allocate_AI_Data+D6        mov     [_ai_landmass_strength_ratios], ax           ; 120 LBX_NearAlloc_Next bytes
    Allocate_AI_Data+DC        mov     [_ai_landmass_strength_ratios+2], ax         ; 120 LBX_NearAlloc_Next bytes
