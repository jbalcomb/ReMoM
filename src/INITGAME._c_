/*
    MAGIC.EXE
    ovr056
*/

#include "INITGAME.H"

#include "NEWGAME.H"

#include "MOX/MOM_DATA.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOX_DEF.H"
#include "MOX/random.H"

#include "Spellbook.H"
#include "UNITTYPE.H"



// MGC dseg:354C
extern int16_t TBL_AI_Realm_PRS[6][6];
// MGC dseg:3594
extern int16_t TBL_AI_Realm_OBJ[6][5];
// MGC dseg:35D0
extern struct S_HERO_TEMPLATE _hero_premade_table[NUM_HERO_TYPES];

// o51p36
void Draw_Building_The_Worlds(int16_t percent);



// MGC o56p1
// drake178: WIZ_SetProfiles()
/*
*/
/*

*/
// WIZ_SetProfiles__WIP()

// MGC o56p2
// drake178: NEWG_FinalizeTables()
/*
finalizes the item, hero, and wizard record tables
RE-CHECK: some fields are not yet known
*/
/*

*/
void NEWG_FinalizeTables__WIP(void)
{
    int16_t itr_players = 0;  // _SI_

    Draw_Building_The_Worlds(100);

    Initialize_Items();

    Init_Heroes();

    Init_Players();

    AI_WIZ_StrategyReset__WIP();

    Init_Magic_Personalities_Objectives();

    Init_Summoning_Circle_And_Spell_Of_Mastery();

    Initialize_Messages();

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].capital_race = _CITIES[itr_players].race;
    }

    Init_Diplomatic_Relations();

    _players[HUMAN_PLAYER_IDX].gold_reserve = ((5 - _difficulty) * 25);

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {

        _players[itr_players].gold_reserve = 150;

// ; 80 + rnd(40) is stored here for each AI wizard (word
// ; array pointer, human excluded) when setting initial
// ; gold
// mov     ax, 40
// push    ax
// call    Random
// add     ax, 80
// mov     dx, _SI_itr_players
// shl     dx, 1
// mov     bx, [Wiz5_Spell_E0h@]             
// add     bx, dx
// mov     [bx], ax
        // Wiz5_Spell_E0h[itr_players] = (80 + Random(40));
        // mov     [AI_SCircle_Reevals@],  (offset _players.spells_list+18C8h) ; 16 bytes
        AI_SCircle_Reevals = ( (int16_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18C8) );  // 16 bytes        
    }

}


// MGC o56p3
// drake178: AI_WIZ_SetRelations()
/*
sets the starting relations between all wizards based
on their spellbooks
RE-CHECK: sets fields $182 to the visible relations
 and $4B0 to 0 - what are these?
 */
/*
    sets Grand Vizier to 'off'

*/
void Init_Diplomatic_Relations(void)
{
    int16_t Wiz_1_Books = 0;
    int16_t Wiz_2_Books = 0;
    int16_t Most_Chaos_Books = 0;
    int16_t Life_Book_Total = 0;
    int16_t Most_Death_Books = 0;
    int16_t itr_players1 = 0;  // _DI_
    int16_t itr_players2 = 0;  // _SI_
    int16_t IDK = 0;  // _CX_

    for(itr_players1 = 0; itr_players1 < _num_players; itr_players1++)
    {

        for(itr_players2 = (itr_players1 + 1); itr_players2 < _num_players; itr_players2++)
        {
            Most_Death_Books = _players[itr_players1].spellranks[sbr_Death];
            if(_players[itr_players2].spellranks[sbr_Death] > Most_Death_Books)
            {
                Most_Death_Books = _players[itr_players2].spellranks[sbr_Death];
            }

            Life_Book_Total = _players[itr_players1].spellranks[sbr_Life];

            Life_Book_Total += _players[itr_players2].spellranks[sbr_Life];

            Most_Chaos_Books = _players[itr_players1].spellranks[sbr_Chaos];
            if(_players[itr_players2].spellranks[sbr_Chaos] > Most_Chaos_Books)
            {
                Most_Chaos_Books = _players[itr_players2].spellranks[sbr_Chaos];
            }

            IDK = 0;
            if(
                (Life_Book_Total > 0)
                &&
                (Most_Death_Books > 0)
            )
            {
                IDK -= ((Life_Book_Total + Most_Death_Books) * 5);
            }
            else
            {
                IDK += (Life_Book_Total * 2);
                IDK -= (Most_Death_Books * 3);
            }

            IDK -= (Most_Chaos_Books * 2);

            Wiz_2_Books = _players[itr_players2].spellranks[sbr_Sorcery];
            Wiz_1_Books = _players[itr_players1].spellranks[sbr_Sorcery];
            if(Wiz_1_Books < Wiz_2_Books)
            {
                Wiz_2_Books = Wiz_1_Books;
            }
            IDK += Wiz_2_Books;

            Wiz_2_Books = _players[itr_players2].spellranks[sbr_Chaos];
            Wiz_1_Books = _players[itr_players1].spellranks[sbr_Chaos];
            if(Wiz_1_Books < Wiz_2_Books)
            {
                Wiz_2_Books = Wiz_1_Books;
            }
            IDK += Wiz_2_Books;

            Wiz_2_Books = _players[itr_players2].spellranks[sbr_Life];
            Wiz_1_Books = _players[itr_players1].spellranks[sbr_Life];
            if(Wiz_1_Books < Wiz_2_Books)
            {
                Wiz_2_Books = Wiz_1_Books;
            }
            IDK += Wiz_2_Books;

            if(IDK < -90)
            {
                IDK = -90;
            }

            _players[itr_players1].Dipl.Visible_Rel[itr_players2] = IDK;

            _players[itr_players2].Dipl.Visible_Rel[itr_players1] = IDK;

            _players[itr_players1].Dipl.Default_Rel[itr_players2] = IDK;

            _players[itr_players2].Dipl.Default_Rel[itr_players1] = IDK;

            _players[itr_players1].peace_duration[itr_players2] = 0;

            _players[itr_players2].peace_duration[itr_players1] = 0;

        }
    }

}


// MGC o56p4
// drake178: NEWG_ClearItems()
/*
clears the item table by setting the value of all
records to 0, and clears the predefined item
appearance table
*/
/*

*/
void Initialize_Items(void)
{
    int16_t itr = 0;  // _CX_
    for(itr = 0; itr < 138; itr++)
    {
        _ITEMS[itr].cost = 0;
    }
    for(itr = 0; itr < 250; itr++)
    {
        TBL_Premade_Items[itr] = 0;
    }
}


// MGC o56p5
// drake178: WIZ_Set_Sum_n_SoM()
/*
sets the summon location and spell of mastery
research cost into the record of every wizard
*/
/*
    sets summon wx,wy,wp from fortress wx,wy,wp, for all players
    sets research cost for Spell of Mastery, for all players    
*/
void Init_Summoning_Circle_And_Spell_Of_Mastery(void)
{
    int16_t itr_players = 0;  // _SI_

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].summon_wp = _FORTRESSES[itr_players].wp;
        _players[itr_players].summon_wx = _FORTRESSES[itr_players].wx;
        _players[itr_players].summon_wy = _FORTRESSES[itr_players].wy;
        
        _players[itr_players].som_research_cost = spell_data_table[spl_Spell_Of_Mastery].research_cost;

        if(
            (_players[itr_players].spellranks[0] == 11)
            &&
            (_players[itr_players].spellranks[1] == 11)
            &&
            (_players[itr_players].spellranks[2] == 11)
            &&
            (_players[itr_players].spellranks[3] == 11)
            &&
            (_players[itr_players].spellranks[4] == 11)
        )
        {
            _players[itr_players].som_research_cost -= 3000;
        }
        
    }

}


// MGC o56p6
// drake178: WIZ_SetPersonalities()
// MoO2  Module: KEN  Init_NPC_Personalities_Objectives_Themes_()
/*
sets the primary and secondary realms into each
player's wizard record, then rolls out the AI
personalities and objectives based on that and the
retorts already stored
BUG: the guaranteed AI researchable spells for Hard
 and Impossible will also remove those spells if
 the wizard already has them
BUG: 2+ realm wizards with Nature as the primary will
 also always set that as the secondary
*/
/*
    sets Primary_Realm and Secondary_Realm for all players
    sets starting spells for AI players
    sets Personality and Object for AI players
*/
void Init_Magic_Personalities_Objectives(void)
{
    int16_t AI_OBJ_Weights[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t AI_PRS_Weights[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t Max_Value = 0;
    int16_t Random_Result = 0;
    int16_t Secondary_Realm = 0;
    int16_t Primary_Realm = 0;
    int16_t Picked_Objective = 0;
    int16_t Picked_Personality = 0;
    int16_t spellranks = 0;
    int16_t itr = 0;  // _SI_
    int16_t itr_players = 0;  // _DI_
    
    // ; loop through the human player's bookshelf to find
    // ; the realm they have the most books of
    Primary_Realm = 0;
    spellranks = _players[HUMAN_PLAYER_IDX].spellranks[0];
    for(itr = 1; itr < NUM_MAGIC_TYPES; itr++)
    {
        if(_players[HUMAN_PLAYER_IDX].spellranks[itr] > spellranks)
        {
            Primary_Realm = itr;
            spellranks = _players[HUMAN_PLAYER_IDX].spellranks[itr];
        }
    }

    // ; loop through the human player's bookshelf to find
    // ; the realm they have the second most books of
    Secondary_Realm = 0;
    spellranks = _players[HUMAN_PLAYER_IDX].spellranks[0];
    for(itr = 1; itr < NUM_MAGIC_TYPES; itr++)
    {
        if(_players[HUMAN_PLAYER_IDX].spellranks[itr] > spellranks)
        {
            Secondary_Realm = itr;
            spellranks = _players[HUMAN_PLAYER_IDX].spellranks[itr];
        }
    }

    if(_players[HUMAN_PLAYER_IDX].spellranks[Secondary_Realm] == 0)
    {
        Secondary_Realm = Primary_Realm;
    }

    _players[HUMAN_PLAYER_IDX].Prim_Realm = Primary_Realm;

    _players[HUMAN_PLAYER_IDX].Sec_Realm = Secondary_Realm;

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        Primary_Realm = 0;
        spellranks = _players[itr_players].spellranks[0];
        
        for(itr = 1; itr < NUM_MAGIC_TYPES; itr++)
        {
            if(_players[itr_players].spellranks[itr] > spellranks)
            {
                Primary_Realm = itr;
                spellranks = _players[itr_players].spellranks[itr];
            }
        }

        switch(Primary_Realm)
        {
            case sbr_Nature:
            {
                _players[itr_players].spells_list[spl_Sprites] = 2;
            } break;
            case sbr_Sorcery:
            {
                _players[itr_players].spells_list[spl_Nagas] = 2;
            } break;
            case sbr_Chaos:
            {
                _players[itr_players].spells_list[spl_Hell_Hounds] = 2;
            } break;
            case sbr_Life:
            {
                _players[itr_players].spells_list[spl_Guardian_Spirit] = 2;
            } break;
            case sbr_Death:
            {
                _players[itr_players].spells_list[spl_Ghouls] = 2;
            } break;
        }

        if(_difficulty >= god_Hard)
        {
            switch(Primary_Realm)
            {
                case sbr_Nature:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == 0)
                    {
                        _players[itr_players].spells_list[spl_Basilisk] = 1;
                    }
                } break;
                case sbr_Sorcery:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == 0)  /* ; BUG: this is not the same spell! */
                    {
                        _players[itr_players].spells_list[spl_Phantom_Beast] = 1;
                    }
                } break;
                case sbr_Chaos:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == 0)  /* ; BUG: this is not the same spell! */
                    {
                        _players[itr_players].spells_list[spl_Chimeras] = 1;
                    }
                } break;
                case sbr_Life:
                {
                    if(_players[itr_players].spells_list[spl_Path_Finding] == 0)  /* ; BUG: this is not the same spell! */
                    {
                        _players[itr_players].spells_list[spl_Unicorns] = 1;
                    }
                } break;
                case sbr_Death:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == 0)  /* ; BUG: this is not the same spell! */
                    {
                        _players[itr_players].spells_list[spl_Shadow_Demons] = 1;
                    }
                } break;
            }
        }

        Secondary_Realm = 0;
        spellranks = _players[itr_players].spellranks[0];
        for(itr = 1; itr < NUM_MAGIC_TYPES; itr++)
        {
            if(_players[itr_players].spellranks[itr] > spellranks)
            {
                Secondary_Realm = itr;
                spellranks = _players[itr_players].spellranks[itr];
            }
        }
    
        if(_players[itr_players].spellranks[Secondary_Realm] == 0)
        {
            Secondary_Realm = Primary_Realm;
        }
    
        _players[itr_players].Prim_Realm = Primary_Realm;
    
        _players[itr_players].Sec_Realm = Secondary_Realm;
    
        for(itr = 1; itr < 6; itr++)
        {
            AI_PRS_Weights[itr] = TBL_AI_Realm_PRS[Primary_Realm][itr];
        }

        for(itr = 1; itr < 5; itr++)
        {
            AI_OBJ_Weights[itr] = TBL_AI_Realm_OBJ[Primary_Realm][itr];
        }

        if(_players[itr_players].warlord == 1)
        {
            AI_PRS_Weights[AI_PRS_RUTHLESS] += 2;
            AI_PRS_Weights[AI_PRS_AGGRESSIVE] += 3;
            AI_OBJ_Weights[AI_OBJ_MILITARIST] += 2;
            AI_OBJ_Weights[AI_OBJ_EXPANSIONIST] += 3;
        }

        if(_players[itr_players].chaos_mastery == 1)
        {
            AI_PRS_Weights[AI_PRS_CHAOTIC] += 3;
            AI_OBJ_Weights[AI_OBJ_THEURGIST] += 1;
        }

        if(_players[itr_players].nature_mastery == 1)
        {
            AI_PRS_Weights[AI_PRS_LAWFUL] += 3;
            AI_OBJ_Weights[AI_OBJ_THEURGIST] += 1;
        }

        if(_players[itr_players].infernal_power == 1)
        {
            AI_PRS_Weights[AI_PRS_MANIACAL] += 4;
        }

        if(_players[itr_players].divine_power == 1)
        {
            AI_PRS_Weights[AI_PRS_PEACEFUL] += 4;
        }

        if(_players[itr_players].channeler == 1)
        {
            AI_PRS_Weights[AI_PRS_AGGRESSIVE] += 2;
        }

        if(_players[itr_players].alchemy == 1)
        {
            AI_PRS_Weights[AI_OBJ_PERFECTIONIST] += 2;
        }

        if(_players[itr_players].archmage == 1)
        {
            AI_PRS_Weights[AI_OBJ_THEURGIST] += 4;
        }

        if(_players[itr_players].myrran == 1)
        {
            AI_PRS_Weights[AI_OBJ_EXPANSIONIST] += 4;
        }

        if(_players[itr_players].conjurer == 1)
        {
            AI_PRS_Weights[AI_OBJ_MILITARIST] += 3;
        }

        if(Primary_Realm == sbr_Life)
        {
            AI_PRS_Weights[AI_PRS_MANIACAL] = 0;
        }

        if(Primary_Realm == sbr_Death)
        {
            AI_PRS_Weights[AI_PRS_PEACEFUL] = 0;
        }

        Max_Value = 0;
        for(itr = 0; itr < 6; itr++)
        {
            Max_Value += AI_PRS_Weights[itr];
        }
        Random_Result = Random(Max_Value);
        Picked_Personality = 0;
        Random_Result -= AI_PRS_Weights[AI_PRS_MANIACAL];
        while((Random_Result > 0) && (Picked_Personality < 5))
        {
            Picked_Personality++;
            Random_Result -= AI_PRS_Weights[Picked_Personality];
        }

        Max_Value = 0;
        for(itr = 0; itr < 6; itr++)
        {
            Max_Value += AI_OBJ_Weights[itr];
        }
        Random_Result = Random(Max_Value);
        Picked_Objective = 0;
        Random_Result -= AI_OBJ_Weights[AI_PRS_MANIACAL];
        while((Random_Result > 0) && (Picked_Objective < 5))
        {
            Picked_Objective++;
            Random_Result -= AI_OBJ_Weights[Picked_Personality];
        }

        _players[itr_players].Personality = Picked_Personality;

        _players[itr_players].Objective = Picked_Objective;

    }

}


// MGC o56p7
// drake178: AI_WIZ_StrategyReset()
/*
resets the war counter and magic strategy
reevaluation delay timers, and fills the unknown
wizard record fields $4B0-$4BD with -1s
*/
/*

*/
void AI_WIZ_StrategyReset__WIP(void)
{
    int16_t itr_players1 = 0;  // _CX_
    int16_t itr_players2 = 0;  // _SI_

    for(itr_players1 = 0; itr_players1 < _num_players; itr_players1++)
    {

        _players[itr_players1].reevaluate_hostility_countdown = 0;

        _players[itr_players1].reevaluate_magic_strategy_countdown = 0;

        _players[itr_players1].reevaluate_magic_power_countdown = 0;

        for(itr_players2 = 0; itr_players2 < _num_players; itr_players2++)
        {
            _players[itr_players1].peace_duration[itr_players2] = ST_UNDEFINED;
        }

    }

}


// MGC o56p8
// drake178: WIZ_SetStartingStats()
/*
calculates the starting fame and casting skill for
each wizard, clears their vault and hero list, sets
the SoM as researchable, sets the initial power
ratios, clears the research and casting fields, and
the global enchantment arrays
*/
/*

*/
void Init_Players(void)
{
    int16_t itr_heroes = 0;
    int16_t itr_players = 0;  // _SI_
    int16_t spell_skill = 0;  // _DI_

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(_players[itr_players].famous == ST_TRUE)
        {
            _players[itr_players].fame = 10;
        }
        else
        {
            _players[itr_players].fame = 0;
        }

        spell_skill = (_players[itr_players].spellranks[0] + _players[itr_players].spellranks[1] + _players[itr_players].spellranks[2] + _players[itr_players].spellranks[3] + _players[itr_players].spellranks[4]);

        if(_players[itr_players].archmage != ST_FALSE)
        {
            spell_skill += 5;
        }

        spell_skill *= 2;

        _players[itr_players].Nominal_Skill = 0;

        _players[itr_players].spell_casting_skill = 0;

        while(_players[itr_players].Nominal_Skill < spell_skill)
        {

            _players[itr_players].spell_casting_skill += 1;

            _players[itr_players].Nominal_Skill = Player_Nominal_Skill(itr_players);
            
        }

        // BUGBUG ; PATCHED here to allow casting in the first turn

        _players[itr_players].researching_spell_idx = spl_NONE;

        _players[itr_players].mana_reserve = 0;

        _players[itr_players].research_cost_remaining = 0;

        _players[itr_players].skill_ratio = 33;

        _players[itr_players].mana_ratio = 33;

        _players[itr_players].research_ratio = 33;

        _players[itr_players].spells_list[spl_Spell_Of_Mastery] = 3;  // TODO add enum for spell list statuses

        _players[itr_players].casting_spell_idx = spl_NONE;

        _players[itr_players].casting_cost_remaining = 0;

        _players[itr_players].Globals[ETERNAL_NIGHT] = 0;
        _players[itr_players].Globals[EVIL_OMENS] = 0;
        _players[itr_players].Globals[ZOMBIE_MASTERY] = 0;
        _players[itr_players].Globals[AURA_OF_MAJESTY] = 0;
        _players[itr_players].Globals[WIND_MASTERY] = 0;
        _players[itr_players].Globals[SUPPRESS_MAGIC] = 0;
        _players[itr_players].Globals[TIME_STOP] = 0;
        _players[itr_players].Globals[NATURES_AWARENESS] = 0;
        _players[itr_players].Globals[NATURES_WRATH] = 0;
        _players[itr_players].Globals[HERB_MASTERY] = 0;
        _players[itr_players].Globals[CHAOS_SURGE] = 0;
        _players[itr_players].Globals[DOOM_MASTERY] = 0;
        _players[itr_players].Globals[GREAT_WASTING] = 0;
        _players[itr_players].Globals[METEOR_STORM] = 0;
        _players[itr_players].Globals[ARMAGEDDON] = 0;
        _players[itr_players].Globals[TRANQUILITY] = 0;
        _players[itr_players].Globals[LIFE_FORCE] = 0;
        _players[itr_players].Globals[CRUSADE] = 0;
        _players[itr_players].Globals[JUST_CAUSE] = 0;
        _players[itr_players].Globals[HOLY_ARMS] = 0;

        _players[itr_players].volcanoes = 0;

        _players[itr_players].Globals[DETECT_MAGIC] = 0;

        _players[itr_players].tax_rate = 2;

        _players[itr_players].Vault_Items[0] = ST_UNDEFINED;
        _players[itr_players].Vault_Items[1] = ST_UNDEFINED;
        _players[itr_players].Vault_Items[2] = ST_UNDEFINED;
        _players[itr_players].Vault_Items[3] = ST_UNDEFINED;
        
        for(itr_heroes = 0; itr_heroes < NUM_HERO_SLOTS; itr_heroes++)
        {
            _players[itr_players].Heroes[itr_heroes].unit_idx = ST_UNDEFINED;
        }

    }
    
}


// MGC o56p9
// drake178: AI_CreateWizards()
// AI_CreateWizards()

// MGC o56p10
// drake178: WIZ_SetSpells()
// WIZ_SetSpells()


// MGC o56p11
// drake178: NEWG_ClearEvents()
// ¿ ~== MoO2  Module: REPORT  Initialize_Reports_() ?
/*
; clears the event data structure by setting the status
; of all events to 0
*/
/*

*/
void Initialize_Events(void)
{
    events_table->last_event_turn = 50;
    events_table->Meteor_Status = 0;
    events_table->Gift_Status = 0;
    events_table->Disjunction_Status = 0;
    events_table->marriage_status = 0;
    events_table->Earthquake_Status = 0;
    events_table->Pirates_Status = 0;
    events_table->Plague_Status = 0;
    events_table->Rebellion_Status = 0;
    events_table->Donation_Status = 0;
    events_table->Depletion_Status = 0;
    events_table->minerals_status = 0;
    events_table->Population_Boom_Status = 0;
    events_table->Good_Moon_Status = 0;
    events_table->Bad_Moon_Status = 0;
    events_table->Conjunction_Chaos_Status = 0;
    events_table->Conjunction_Nature_Status = 0;
    events_table->Conjunction_Sorcery_Status = 0;
    events_table->Mana_Short_Status = 0;
}

// MGC o56p12
// drake178: NEWG_CreateHeroes()
// MoO2  Module: INITGAME  Init_Leaders_()
void Init_Heroes(void)
{
    int32_t abilities = 0;
    int16_t all_picks = 0;
    int16_t mage_picks = 0;
    int16_t casting_skill = 0;
    int16_t itr_hero_types = 0;
    int16_t itr_players = 0;
    int16_t warrior_picks = 0;  // _DI_

    for(itr_players = 0; itr_players < 5; itr_players++)
    {

        for(itr_hero_types = 0; itr_hero_types < NUM_HERO_TYPES; itr_hero_types++)
        {

            _HEROES2[itr_players]->heroes[itr_hero_types].Level = 0;

            warrior_picks = 0;

            mage_picks = 0;

            all_picks = 0;

            switch(_hero_premade_table[itr_hero_types].pick_type)
            {
                case 0:  /* Warrior */
                {
                    warrior_picks = _hero_premade_table[itr_hero_types].pick_count;
                } break;
                case 1:  /* Mage */
                {
                    mage_picks = _hero_premade_table[itr_hero_types].pick_count;
                } break;
                case 2:  /* All/Any/Both/Either/Neither */
                {
                    all_picks = _hero_premade_table[itr_hero_types].pick_count;
                } break;
            }

            abilities = _hero_premade_table[itr_hero_types].abilities;

            casting_skill = _hero_premade_table[itr_hero_types].casting_skill;

            if(all_picks > 0)
            {
                warrior_picks = all_picks;
                mage_picks = all_picks;
            }

            while((warrior_picks > 0) || (mage_picks > 0))
            {

                switch(Random(14) - 1)
                {
                    case 0:  /* Leadership */
                    {
                        if(((abilities & HSA_LEADERSHIP2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_LEADERSHIP) != 0) { abilities ^= HSA_LEADERSHIP; abilities |= HSA_LEADERSHIP2; } else { abilities |= HSA_LEADERSHIP; } warrior_picks--; mage_picks--; }
                    } break;
                    case 1:  /* Legendary */
                    {
                        if(((abilities & HSA_LEGENDARY2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_LEGENDARY) != 0) { abilities ^= HSA_LEGENDARY; abilities |= HSA_LEGENDARY2; } else { abilities |= HSA_LEGENDARY; } warrior_picks--; mage_picks--; }
                    } break;
                    case 2:  /* Armsmaster */
                    {
                        if(((abilities & HSA_ARMSMASTER2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_ARMSMASTER) != 0) { abilities ^= HSA_ARMSMASTER; abilities |= HSA_ARMSMASTER2; } else { abilities |= HSA_ARMSMASTER; } warrior_picks--; mage_picks--; }
                    } break;
                    case 3:  /* Blademaster */
                    {
                        if(((abilities & HSA_BLADEMASTER2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_BLADEMASTER) != 0) { abilities ^= HSA_BLADEMASTER; abilities |= HSA_BLADEMASTER2; } else { abilities |= HSA_BLADEMASTER; } warrior_picks--; mage_picks--; }
                    } break;
                    case 4:  /* Might */
                    {
                        if(((abilities & HSA_MIGHT2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_MIGHT) != 0) { abilities ^= HSA_MIGHT; abilities |= HSA_MIGHT2; } else { abilities |= HSA_MIGHT; } warrior_picks--; mage_picks--; }
                    } break;
                    case 5:  /* Constitution */
                    {
                        if(((abilities & HSA_CONSTITUTION2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_CONSTITUTION) != 0) { abilities ^= HSA_CONSTITUTION; abilities |= HSA_CONSTITUTION2; } else { abilities |= HSA_CONSTITUTION; } warrior_picks--; mage_picks--; }
                    } break;
                    case 6:  /* Arcane Power */
                    {
                        if(((abilities & HSA_ARCANE_POWER2) == 0) && (mage_picks > 0))
                        {
                            // ; BUG: the Knight has no caster picks, this should be $1E (the Elven Archer)
                            if(itr_hero_types != ut_Knight)  /* ¿ should ut_ElvenArcher  = 30 ? */
                            {
                                // ; BUG: excludes the Warlock and the Chaos Warrior, this should be $1E (Ranged_Lightning)
                                if(
                                    ((abilities & HSA_ARCANE_POWER) != 0)
                                    &&
                                    (_unit_type_table[itr_hero_types].Ranged_Type >= rat_Fireball)  /* ¿ should be rat_Lightning    =  30 ? */
                                    &&
                                    (_unit_type_table[itr_hero_types].Ranged_Type <= rat_Nat_Bolt)
                                )
                                {
                                    abilities ^= HSA_ARCANE_POWER;
                                    abilities |= HSA_ARCANE_POWER2;
                                }
                                else
                                {
                                    abilities |= HSA_ARCANE_POWER;
                                }
                            }
                        }
                    } break;
                    case 7:  /* Prayermaster */
                    {
                        if(((abilities & HSA_PRAYERMASTER2) == 0) && (mage_picks > 0)) { if((abilities & HSA_PRAYERMASTER) != 0) { abilities ^= HSA_PRAYERMASTER; abilities |= HSA_PRAYERMASTER2; } else { abilities |= HSA_PRAYERMASTER; } warrior_picks--; mage_picks--; }
                    } break;
                    case 8:  /* Casting Skill */
                    {
                        if(mage_picks > 0) { casting_skill++; warrior_picks--; mage_picks--; }
                    } break;
                    case 9:  /* Noble */
                    {
                        if(((abilities & HSA_NOBLE) == 0) && (itr_hero_types = ut_Chosen)) { abilities |= HSA_NOBLE; warrior_picks--; mage_picks--; }
                    } break;
                    case 10:  /* Charm */
                    {
                        if(((abilities & HSA_CHARMED) == 0) && ((_unit_type_table[itr_hero_types].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)) { abilities |= HSA_CHARMED; warrior_picks--; mage_picks--; }
                    } break;
                    case 11:  /* Lucky */
                    {
                        if((abilities & HSA_LUCKY) == 0) { abilities |= HSA_LUCKY; warrior_picks--; mage_picks--; }
                    } /* BUGBUG: case 11/Luck missing `break;`, falls through to case 12/Agility */
                    case 12:  /* Agility */
                    {
                        if(((abilities & HSA_AGILITY2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_AGILITY) != 0) { abilities |= HSA_AGILITY2; } else { abilities |= HSA_AGILITY; } warrior_picks--; mage_picks--; }
                    } break;
                    case 13:  /* Sage */
                    {
                        if(((abilities & HSA_SAGE2) == 0) && (mage_picks > 0)) { if((abilities & HSA_SAGE) != 0) { abilities |= HSA_SAGE2; } else { abilities |= HSA_SAGE; } warrior_picks--; mage_picks--; }
                    } break;

                }

            }

            _HEROES2[itr_players]->heroes[itr_hero_types].abilities = abilities;
            _HEROES2[itr_players]->heroes[itr_hero_types].Casting_Skill = casting_skill;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[0] = _hero_premade_table[itr_hero_types].spell_1;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[1] = _hero_premade_table[itr_hero_types].spell_2;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[2] = _hero_premade_table[itr_hero_types].spell_3;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[3] = _hero_premade_table[itr_hero_types].spell_4;

        }

    }

}


// MGC o56p13
// drake178: CRP_MSG_Clear()
/*
zeroes the counters of all chancellor (scroll)
message types, clearing all event messages
*/
/*

*/
void Initialize_Messages(void)
{
    MSG_UnitLost_Count = 0;
    MSG_UnitKilled_Count = 0;
    MSG_UEsLost_Count = 0;
    MSG_CEsLost_Count = 0;
    MSG_GEs_Lost = 0;
    MSG_CityGrowth_Count = 0;
    MSG_CityDeath_Count = 0;
    MSG_BldLost_Count = 0;
    MSG_CityLost_Count = 0;
    MSG_CityGained_Count = 0;
}


// MGC o56p14
// drake178: UU_ITEM_SetHeroSlots()
// UU_ITEM_SetHeroSlots()

// MGC o56p15
// drake178: WIZ_ConsolidateBooks()
// WIZ_ConsolidateBooks()

// MGC o56p16
// drake178: WIZ_GetNominalSkill()
/*
calculates and returns the wizard's nominal spell
casting skill from their stored invested skill
points, adding 10 for Archmage if applicable
*/
/*

¿ ~ Player_Base_Casting_Skill() ?
*/
int16_t Player_Nominal_Skill(int16_t player_idx)
{
    int32_t Skill_Points_Left = 0;
    int16_t IDK = 0;  // _SI_
    int16_t nominal_skill = 0;  // _CX_

    IDK = 0;
    
    nominal_skill = 0;
    
    Skill_Points_Left = 0;

    while(_players[player_idx].spell_casting_skill > Skill_Points_Left)
    {
        nominal_skill += 1;
        IDK += 2;
        Skill_Points_Left += IDK;
    }

    if(_players[player_idx].archmage > 0)
    {
        nominal_skill += 10;
    }

    return nominal_skill;
}


// MGC o56p17
// drake178: RNG_WeightedPick16()
// RNG_WeightedPick16()
