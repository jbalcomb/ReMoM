/*
    AI Spell

    WIZARDS.EXE
    ovr156

MoO2
Module: AITECH
*/

#include "MOX/MOX_DAT.H"  /* _players[] */

#include "MOM.H"



// WZD dseg:70F8                                                 BEGIN:  ovr156 - Initialized Data

// WZD dseg:70F8
char CRP_AI_SpellTargetError[] = " could not be found for CP.";

// WZD dseg:7114 00                                              db    0
// WZD dseg:7115 00                                              db    0

// WZD dseg:7115                                                 END:  ovr156 - Initialized Data



/*
    WIZARDS.EXE ovr156
*/

// WZD o156p01
// drake178: AI_Research_Picker()
// ~ MoO2  Module: AITECH  AI_Tech_Select_()
/*
; selects the spell to research from the wizard's list
; of candidates using a weighted random roll,
; prioritizing combat spells in research groups from
; which the player does not yet have a known spell
;
; contains multipe BUGs that prevent research-related
; profile traits from properly affecting the outcome
*/
void AI_Research_Picker__STUB(int16_t player_idx)
{
    /* Dasm does itr 90, so this must be 90 bytes + 1 for 2-byte alignment */
    int16_t Possessed_Categories[91] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Unused_Var = 0;
    uint32_t Costs_32bit[NUM_RESEARCH_SPELLS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Candidate_Costs[NUM_RESEARCH_SPELLS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Highest_Cost = 0;
    int16_t Weight_Total = 0;
    int16_t Cnd_Category = 0;
    int16_t Skill = 0;
    int16_t Mana = 0;
    int16_t Research = 0;
    int16_t Loop_Var = 0;
    int16_t Research_Bonus = 0;
    int16_t Candidate_Count = 0;
    int16_t Original_Cost = 0;
    int16_t selection = 0;
    int16_t * ptr__players_spellranks = 0;
    uint16_t * ptr__players_research_spells = 0;
    int16_t itr_candidates = 0;  // _DI_

    ptr__players_research_spells = &_players[player_idx].research_spells[0];

    ptr__players_spellranks = &_players[player_idx].spellranks[0];

    Candidate_Count = 0;

    for(itr_candidates = 0; itr_candidates < NUM_RESEARCH_SPELLS; itr_candidates++)
    {

        if(ptr__players_research_spells[itr_candidates] != 0)
        {
            Candidate_Count++;
        }

    }

    if(Candidate_Count == 0)
    {

        _players[player_idx].researching_spell_idx = spl_NONE;

        _players[player_idx].research_ratio = 0;
        _players[player_idx].mana_ratio = 50;
        _players[player_idx].skill_ratio = 50;

        return;

    }


    // ...
    // ...
    // ...


    // ; chooses a random item from a list of 32bit weighted
    // ; chances; condensing the weights such that the total
    // ; fits into a single call of the 9-bit RNG (max 512),
    // ; using repeated divisions by 2 if necessary
    // TODO  Picked_Candidate = Get_Weighted_Choice_Long(&Costs_32bit[0], Candidate_Count);

    selection = Random(Candidate_Count);

    _players[player_idx].researching_spell_idx = ptr__players_research_spells[selection];

}


// WZD o156p02
// drake178: AI_OVL_SpellPicker()
/*
; selects the next overland spell to cast for the
; specified wizard
;
; multiple BUGs and unexplored sub-functions,
; RE-EXPLORE!
*/
/*



*/
void AI_Spell_Select__STUB(int16_t player_idx)
{
    int16_t Mana_Income = 0;
    int16_t Food_Income = 0;
    int16_t Gold_Income = 0;
    uint8_t * ptr_players_spells_known = 0;
    int16_t Mana_Per_Turn = 0;
    int16_t spellbook_page_spell_index = 0;
    int16_t Selected_Category = 0;
    int16_t spell_idx = 0;  // _DI_


    ptr_players_spells_known = &_players[player_idx].spells_list[0];


AI_Compute_Spells_Info(player_idx);

// TODO  Selected_Category = AI_OVL_SplCat_Picker(player_idx);
// ; selects a spell category for the AI to use next:
// ;   0 - no spell
// ;   1 - summon
// ;   2 - unit enchantment
// ;   3 - city enchantment
// ;   4 - disenchant / true
// ;   5 - disjunction / true
// ;   6 - summoning circle
// ;   7 - overland curse / damage
// ;   8 - suppression global
// ;   9 - global enchantment
// ;  10 - spell of mastery
// ;
// ; contains a number of BUGs - re-examine and fix later
Selected_Category = 0;

    switch(Selected_Category)
    {

        case 0:
        {

            spell_idx = 0;

        } break;

        case 1:
        {

        } break;

        case 2:
        {

        } break;

        case 3:
        {

        } break;

        case 4:
        {

        } break;

        case 5:
        {

        } break;

        case 6:
        {

        } break;

        case 7:
        {

        } break;

        case 8:
        {

        } break;

        case 9:
        {

        } break;

        case 10:
        {

        } break;

    }


    Player_Resource_Income_Total(player_idx, &Gold_Income, &Food_Income, &Mana_Income);

    Mana_Per_Turn = ((_players[player_idx].mana_reserve / 10) + Mana_Income);

    if((spell_data_table[spell_idx].casting_cost / 50) > Mana_Per_Turn)
    {

        spell_idx = 0;

    }


    /* TODO */ WIZ_SetOverlandSpell__WIP(player_idx, spell_idx, spellbook_page_spell_index);


}


// WZD o156p03
// AI_OVL_SplCat_Picker()

// WZD o156p04
// drake178: AI_OVL_GetSpellList()
/*
; recalculates the overland spells available to the
; selected player, setting AI_OVL_SpellCount,
; AI_OVL_Spell_Cats, and AI_OVL_SpellList accordingly
*/
/*



*/
void AI_Compute_Spells_Info(int16_t player_idx)
{
    int16_t spell_idx = 0;
    int16_t itr = 0;
    int16_t itr_spells = 0;  // _DI_
    int16_t itr_realms = 0;  // _SI_
    int16_t ai_spell_group = 0;  // _CX_

    CRP_AI_OVL_SpellCount = 0;  // ; redundant - calculated but never used


    for(itr = 0; itr < 90; itr++)
    {

        AI_OVL_Spell_Cats[itr] = 0;

    }

    for(itr_realms = 0; itr_realms < NUM_MAGIC_REALMS; itr_realms++)
    {

        for(itr_spells = 0; itr_spells < NUM_SPELLS_PER_MAGIC_REALM; itr_spells++)
        {

            if(_players[player_idx].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == 2 /* S_Known */)
            {

                spell_idx = ((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells);

                ai_spell_group = spell_data_table[spell_idx].AI_Group;

                if(ai_spell_group ==  5) { continue; }  // ; combat damage spells
                if(ai_spell_group == 24) { continue; }
                if(ai_spell_group == 47) { continue; }
                if(ai_spell_group == 70) { continue; }
                if(ai_spell_group == 12) { continue; }
                if(ai_spell_group == 31) { continue; }
                if(ai_spell_group == 55) { continue; }
                if(ai_spell_group == 77) { continue; }
                if(ai_spell_group == 16) { continue; }
                if(ai_spell_group == 35) { continue; }
                if(ai_spell_group == 59) { continue; }
                if(ai_spell_group == 81) { continue; }
                if(ai_spell_group == 17) { continue; }
                if(ai_spell_group == 36) { continue; }
                if(ai_spell_group == 60) { continue; }
                if(ai_spell_group == 82) { continue; }
                if(ai_spell_group ==  1) { continue; }  // ; Dispel Magic
                if(ai_spell_group == 20) { continue; }
                if(ai_spell_group == 43) { continue; }
                if(ai_spell_group == 19) { continue; }
                if(ai_spell_group ==  0) { continue; }  // ¿ NONE ?

                // ; redundant - filled out but never used
                // ; redundant - calculated but never used
                CRP_AI_OVL_SpellList[CRP_AI_OVL_SpellCount] = spell_idx;

                AI_OVL_Spell_Cats[ai_spell_group] = 1;

                CRP_AI_OVL_SpellCount++;

            }

        }

    }

}


// WZD o156p05
// drake178: AI_OVL_PickSummon()

// WZD o156p06
// drake178: AI_OVL_PickUnitBuff()

// WZD o156p07
// drake178: AI_OVL_PickRealmSupr()

// WZD o156p08
// drake178: AI_OVL_PickGlobal()

// WZD o156p09
// drake178: AI_OVL_PickCurse()

// WZD o156p10
// drake178: AI_OVL_PickCityBuff()

// WZD o156p11
// drake178: SPL_IsLifeSupressed()

// WZD o156p12
// drake178: SPL_IsChsSuppressed()

// WZD o156p13
// drake178: SPL_IsDthSuppressed()

// WZD o156p14
// drake178: CRP_SPL_IsNatSuppressed()

// WZD o156p15
// drake178: AI_OVL_PickDise()

// WZD o156p16
// drake178: AI_OVL_PickDisj()

// WZD o156p17
// drake178: AITP_WallofStone()

// WZD o156p18
// drake178: AITP_Transmute()

// WZD o156p19
// drake178: AITP_ChangeTerrain()
/*
; AI target picker for Change Terrain - selects the
; first desert or swamp in the catchment of the highest
; value city with such
; returns 1 if a valid target was found, or 0 otherwise
; BUG: treats catchment corners as affecting the city
; WARNING: only targets deserts and swamps
*/
/*

*/
int16_t AITP_ChangeTerrain__WIP(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp)
{
    int16_t Target_Plane = 0;
    int16_t Target_Y = 0;
    int16_t Target_X = 0;
    int16_t Tile_Y = 0;
    int16_t Plane = 0;
    int16_t Y_Modifier = 0;
    int16_t X_Modifier = 0;
    int16_t Highest_Value = 0;
    int16_t Target_City = 0;
    int16_t itr_cities = 0;  // _SI_

    __debugbreak();

    Target_City = ST_UNDEFINED;

    Highest_Value = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

    }



    if(Target_City == ST_UNDEFINED)
    {
        return ST_FALSE;
    }
    else
    {
        return ST_TRUE;
    }

}


// WZD o156p20
// AITP_MoveFortress()

// WZD o156p21
// AITP_FlyingFortress()

// WZD o156p22
// AITP_EarthGate()

// WZD o156p23
// AITP_GaiasBlessing()

// WZD o156p24
// AITP_WallofFire()

// WZD o156p25
// AITP_WallofDarkness()

// WZD o156p26
// AITP_HeavenlyLight()

// WZD o156p27
// AITP_AltarofBattle()

// WZD o156p28
// AITP_StreamofLife()

// WZD o156p29
// AITP_Inspirations()

// WZD o156p30
// drake178: AITP_Prosperity()

// WZD o156p31
// drake178: AITP_AstralGate()

// WZD o156p32
// drake178: AITP_DarkRituals()

// WZD o156p33
// drake178: AITP_CloudofShadow()

// WZD o156p34
// drake178: AITP_SpellWard()

// WZD o156p35
// drake178: AITP_Consecration()

// WZD o156p36
// drake178: UU_DBG_GetKnownSpells()

// WZD o156p37
// drake178: CRP_DBG_SpellTargetError()
void Cast_Spell_Target_Error(int16_t spell_idx)
{

    if(Check_Release_Version() == ST_TRUE)
    {
        return;
    }

    String_Copy_Far(near_buffer, spell_data_table[spell_idx].name);

    strcat(near_buffer, CRP_AI_SpellTargetError);  // " could not be found for CP."

    Exit_With_Message(near_buffer);

}


// WZD o156p38
// drake178: AI_ReleaseGlobals()
/*
releases unwanted global enchantments or, if at zero
mana, all global enchantments

WARNING: this may not be the best thing to do...
*/
/*

*/
void AI_Sanity_Check_Overland_Enchantments(int16_t player_idx)
{

    if(
        (_players[player_idx].Globals[ETERNAL_NIGHT] != 0)
        &&
        (_players[player_idx].Prim_Realm != sbr_Death)
        &&
        (_players[player_idx].Sec_Realm != sbr_Death)
    )
    {

        _players[player_idx].Globals[ETERNAL_NIGHT] = 0;

    }

    if(
        (_players[player_idx].Globals[EVIL_OMENS] != 0)
        &&
        (_players[player_idx].Prim_Realm == sbr_Life)
        &&
        (_players[player_idx].Sec_Realm == sbr_Life)
        &&
        (_players[player_idx].Prim_Realm == sbr_Nature)
        &&
        (_players[player_idx].Sec_Realm == sbr_Nature)
    )
    {

        _players[player_idx].Globals[EVIL_OMENS] = 0;

    }

    if(
        (_players[player_idx].Globals[CHAOS_SURGE] != 0)
        &&
        (_players[player_idx].Prim_Realm != sbr_Chaos)
        &&
        (_players[player_idx].Sec_Realm != sbr_Chaos)
    )
    {

        _players[player_idx].Globals[CHAOS_SURGE] = 0;

    }

    if(
        (_players[player_idx].Globals[TRANQUILITY] != 0)
        &&
        (_players[player_idx].Prim_Realm == sbr_Chaos)
        &&
        (_players[player_idx].Sec_Realm == sbr_Chaos)
    )
    {

        _players[player_idx].Globals[TRANQUILITY] = 0;

    }

    if(
        (_players[player_idx].Globals[LIFE_FORCE] != 0)
        &&
        (_players[player_idx].Prim_Realm == sbr_Chaos)
        &&
        (_players[player_idx].Sec_Realm == sbr_Chaos)
    )
    {

        _players[player_idx].Globals[LIFE_FORCE] = 0;

    }

    if(_players[player_idx].mana_reserve == 0)
    {

        _players[player_idx].Globals[ETERNAL_NIGHT] = 0;
        _players[player_idx].Globals[EVIL_OMENS] = 0;
        _players[player_idx].Globals[ZOMBIE_MASTERY] = 0;
        _players[player_idx].Globals[AURA_OF_MAJESTY] = 0;
        _players[player_idx].Globals[WIND_MASTERY] = 0;
        _players[player_idx].Globals[SUPPRESS_MAGIC] = 0;
        _players[player_idx].Globals[TIME_STOP] = 0;
        _players[player_idx].Globals[NATURES_AWARENESS] = 0;
        _players[player_idx].Globals[NATURES_WRATH] = 0;
        _players[player_idx].Globals[HERB_MASTERY] = 0;
        _players[player_idx].Globals[CHAOS_SURGE] = 0;
        _players[player_idx].Globals[DOOM_MASTERY] = 0;
        _players[player_idx].Globals[GREAT_WASTING] = 0;
        _players[player_idx].Globals[METEOR_STORM] = 0;
        _players[player_idx].Globals[ARMAGEDDON] = 0;
        _players[player_idx].Globals[TRANQUILITY] = 0;
        _players[player_idx].Globals[LIFE_FORCE] = 0;
        _players[player_idx].Globals[CRUSADE] = 0;
        _players[player_idx].Globals[JUST_CAUSE] = 0;
        _players[player_idx].Globals[HOLY_ARMS] = 0;
        _players[player_idx].Globals[PLANAR_SEAL] = 0;
        _players[player_idx].Globals[CHARM_OF_LIFE] = 0;
        _players[player_idx].Globals[DETECT_MAGIC] = 0;
        _players[player_idx].Globals[AWARENESS] = 0;
    }

}


// WZD o156p39
// drake178: sub_E7CC9()

// WZD o156p40
// drake178: AI_CanTargetCities()

// WZD o156p41
// drake178: sub_E8070()

// WZD o156p42
// drake178: sub_E8448()

// WZD o156p43
// drake178: sub_E882B()

// WZD o156p44
// drake178: sub_E88F3()

// WZD o156p45
// drake178: AITP_OVL_TileSpells()
/* 
AI target picker for overland tile spells
returns 1 if a successful target is found, 0 if not,
or the passed return value if the spell is undefined
contains BUGs in every targeting function, and some
select completely inappropriate targets
*/
/*
    calls out to get wx,wy,wp, for 14 spells

*/
int16_t Get_Map_Square_Target_For_Spell(int16_t something, int16_t * wx, int16_t * wy, int16_t * wp, int16_t spell_idx, int16_t player_idx)
{
    int16_t Passed_Return = 0;
    int16_t return_value = 0;  // DNE in Dasm

    switch(spell_idx)
    {
        case spl_Transmute:
        {
            __debugbreak();
            // SPELLY  return_value = AITP_Transmute(player_idx, wx, wy, wp);
        } break;
        case spl_Change_Terrain:
        {
            /* SPELLY */  return_value = AITP_ChangeTerrain__WIP(player_idx, wx, wy, wp);

        } break;
        case spl_Ice_Storm:
        case spl_Fire_Storm:
        case spl_Black_Wind:
        case spl_Stasis:
        {
            __debugbreak();
            // SPELLY  return_value AITP_OVL_HarmStack(player_idx, wx, wy, wp);
        } break;
        case spl_Floating_Island:
        {
            return_value = Pick_Target_For_Floating_Island(player_idx, wx, wy, wp);
        } break;
        case spl_Enchant_Road:
        {
            __debugbreak();
            // SPELLY  return_value = AITP_EnchantRoad(player_idx, wx, wy, wp);
        } break;
        case spl_Disenchant_True:
        case spl_Disenchant_Area:
        {
            __debugbreak();
            // SPELLY  return_value = AITP_Disenchant(player_idx, wx, wy, wp);
        } break;
        case spl_Corruption:
        case spl_Raise_Volcano:
        {
            __debugbreak();
            // SPELLY  return_value = AITP_HarmTerrain(player_idx, wx, wy, wp, spell_idx);
        } break;
        case spl_Plane_Shift:
        {
            __debugbreak();
            // SPELLY  return_value = AITP_PlaneShift(player_idx, wx, wy, wp);
        } break;
        case spl_Natures_Cures:
        {
            __debugbreak();
            // SPELLY  return_value = AITP_NaturesCures(player_idx, wx, wy, wp);
        } break;
        default:
        {
            Cast_Spell_Target_Error(spell_idx);  // "[spell name] could not be found for CP."
            return_value = something;
            Passed_Return = return_value;
        } break;
    }

    return return_value;

}


// WZD o156p46
// drake178: AITP_OVL_HarmStack()


// WZD o156p47
// drake178: AITP_FloatingIsland()
/*
; AI target picker for Floating Island - selects the
; closest ocean tile within 10 tiles of a random city
; in a cardinal direction
; returns 1 if a valid target was found, or 0 otherwise
; BUG: utterly useless targeting
*/
/*

*/
int16_t Pick_Target_For_Floating_Island(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp)
{
    int16_t city_idx = 0;
    int16_t Tries = 0;
    int16_t uu_player_idx = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t Vertical = 0;
    int16_t city_wp = 0;
    int16_t UpLeft = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t check_value = 0;  // _DI_
    int16_t itr_squares = 0;  // _SI_

    uu_player_idx = player_idx;

    check_value = ST_FALSE;

    // ; make 200 attempts to find a random city that belongs
    // ; to the player
    for(Tries = 0; ((Tries < 200) && (check_value == ST_FALSE)); Tries++)
    {

        city_idx = (Random(_cities) - 1);

        if(_CITIES[city_idx].owner_idx == player_idx)
        {

            check_value = ST_TRUE;

            city_wp = _CITIES[city_idx].wp;
            
            city_wx = _CITIES[city_idx].wx;
            
            city_wy = _CITIES[city_idx].wy;

        }

    }

    if(check_value == ST_FALSE)
    {
        return ST_FALSE;
    }

    // ; find the closest ocean tile within 10 tiles in any of
    // ; the cardinal directions (straight only)

    itr_squares = 0;

    UpLeft = ST_TRUE;

    Vertical = ST_FALSE;

    check_value = ST_FALSE;

    while((check_value == ST_FALSE) && (itr_squares < 10))
    {
        if(UpLeft == ST_FALSE)
        {
            if(Vertical == ST_FALSE)
            {
                if(Map_Square_Is_Legal_For_Floating_Island((city_wx + itr_squares), city_wy, city_wp) == ST_TRUE)
                {
                    check_value = ST_TRUE;
                    target_wx = (city_wx + itr_squares);
                    target_wy = city_wy;
                }
                Vertical = ST_TRUE;
            }
            else  /* (Vertical == ST_TRUE) */
            {
                if(Map_Square_Is_Legal_For_Floating_Island(city_wx, (city_wy + itr_squares), city_wp) == ST_TRUE)
                {
                    check_value = ST_TRUE;
                    target_wx = city_wx;
                    target_wy = (city_wy + itr_squares);
                }
                Vertical = ST_TRUE;
            }
        }
        else  /* (UpLeft == ST_TRUE) */
        {
            if(Vertical == ST_FALSE)
            {
                if(Map_Square_Is_Legal_For_Floating_Island((city_wx - itr_squares), city_wy, city_wp) == ST_TRUE)
                {
                    check_value = ST_TRUE;
                    target_wx = (city_wx - itr_squares);
                    target_wy = city_wy;
                }
                Vertical = ST_TRUE;
            }
            else  /* (Vertical == ST_TRUE) */
            {
                if(Map_Square_Is_Legal_For_Floating_Island(city_wx, (city_wy - itr_squares), city_wp) == ST_TRUE)
                {
                    check_value = ST_TRUE;
                    target_wx = city_wx;
                    target_wy = (city_wy - itr_squares);
                }
                Vertical = ST_FALSE;
                itr_squares++;
                UpLeft = ST_FALSE;
            }
        }
    }

    if(check_value == ST_FALSE)
    {
        return ST_FALSE;
    }
    else
    {
        *wx = target_wx;
        *wy = target_wy;
        *wp = city_wp;
        return ST_TRUE;
    }

}


// WZD o156p48
// drake178: AITP_EnchantRoad()

// WZD o156p49
// drake178: AITP_Disenchant()

// WZD o156p50
// drake178: AITP_HarmTerrain()

// WZD o156p51
// drake178: sub_E9FA9()

// WZD o156p52
// drake178: AITP_PlaneShift()

// WZD o156p53
// drake178: AITP_NaturesCures()

// WZD o156p54
// drake178: sub_EA43C()

// WZD o156p55
// drake178: sub_EA4FE()

// WZD o156p56
// drake178: AI_MyrrorAdvantage()

// WZD o156p57
// drake178: TILE_IsOcean()
/*
; returns 1 if the tile is a shore, ocean, or lake, or
; 0 otherwise
; differs from TILE_IsAISailable and TILE_IsAISailable2
; in that this also returns 0 for river-outflow single
; tile lakes
; differs from TILE_IsSailable in that this returns 0
; for all inland lakes
*/
/*

*/
int16_t Map_Square_Is_Legal_For_Floating_Island(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // DNE in Dasm

    // DEDU  `% _TerType_Count`? same seen in NEXTTURN.C
    terrain_type = (GET_TERRAIN_TYPE(wx, wy, wp) % _TerType_Count);

    if(
        ((terrain_type < _Tundra00001000) && (terrain_type > _River1111_5))
        ||
        ((terrain_type <= _Shore111R1110) && (terrain_type > _Desert10101111))
        ||
        ((terrain_type <= _Shore000R0000) && (terrain_type > _1LakeRiv_S))
        ||
        ((terrain_type <= _Shore10101111) && (terrain_type != TT_BugGrass) && (terrain_type != _1Lake))
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}


// WZD o156p58
// drake178: sub_EA61E()

// WZD o156p59
// drake178: WIZ_SummonInFortress()
