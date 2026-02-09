/*
    MAGIC.EXE
    ovr056

MoO2
Module: INITGAME
*/

#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/random.h"

#include "NewGame.h"
#include "Spellbook.h"
#include "UNITTYPE.h"

#include "INITGAME.h"

#include <string.h>



// MGC  dseg:2AD0
struct s_WIZARD_PRESET _wizard_presets_table[15];

// MGC  dseg:2C6A
extern struct s_DEFAULT_SPELLS _default_spells[5];

// MGC  dseg:354C
extern int16_t TBL_AI_Realm_PRS[6][6];
// MGC  dseg:3594
extern int16_t TBL_AI_Realm_OBJ[6][5];
// MGC  dseg:35D0
extern struct S_HERO_TEMPLATE _hero_premade_table[NUM_HERO_TYPES];

// ...oops... MGC  dseg:8A42 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+_start_spells s_Init_Spells_Table <0>   ; DATA XREF: GAME_New_Screen_5+171w ...
// MGC  dseg:8A42 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+_player_start_spells s_Init_Base_Realms 6 dup(<0>)
// ...needed by INITGAME.c, so defn in NewGame.h
struct s_Init_Base_Realms _player_start_spells[PLAYER_COUNT_MAX];

// o51p36
void Draw_Building_The_Worlds(int16_t percent);



// MGC o56p1
// drake178: WIZ_SetProfiles()
/*
; creates the AI wizards and sets the known and
; researchable spells for all players
*/
/*
MoO2
Init_New_Game_()
|-> Init_Players_()
So, ...
~== Init_Computer_Players()

OON XREF j_Init_Computer_Players() <-| NEWG_CreateWorld__WIP()

*/
void Init_Computer_Players(void)
{
    int16_t itr_num_players = 0;  // _DI_
    int16_t itr2 = 0;  // _SI_

    Init_Computer_Players_Wizard_Profile();

    for(itr_num_players = 1; itr_num_players < _num_players; itr_num_players++)
    {

        for(itr2 = 0; itr2 < 13; itr2++)
        {

            _player_start_spells[itr_num_players].realms[sbr_Nature].spells[itr2]   = _default_spells[sbr_Nature].spells[itr2];
            _player_start_spells[itr_num_players].realms[sbr_Sorcery].spells[itr2]  = _default_spells[sbr_Sorcery].spells[itr2];
            _player_start_spells[itr_num_players].realms[sbr_Chaos].spells[itr2]    = _default_spells[sbr_Chaos].spells[itr2];
            _player_start_spells[itr_num_players].realms[sbr_Life].spells[itr2]     = _default_spells[sbr_Life].spells[itr2];
            _player_start_spells[itr_num_players].realms[sbr_Death].spells[itr2]    = _default_spells[sbr_Death].spells[itr2];

        }

    }

    Init_Computer_Players_Spell_Library();

    _players[NEUTRAL_PLAYER_IDX].Defeated_Wizards = 0;

    _players[NEUTRAL_PLAYER_IDX].average_unit_cost = 0;

}


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

            _players[itr_players1].Dipl.Visible_Rel[itr_players2] = (int8_t)IDK;

            _players[itr_players2].Dipl.Visible_Rel[itr_players1] = (int8_t)IDK;

            _players[itr_players1].Dipl.Default_Rel[itr_players2] = (int8_t)IDK;

            _players[itr_players2].Dipl.Default_Rel[itr_players1] = (int8_t)IDK;

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
/*
; generates the main traits of the AI wizards by
; randomly selecting and then modifying default
; profiles to an extent specified by the difficulty
; setting
*/
/*
OON XREF WIZ_SetProfiles()

*/
void Init_Computer_Players_Wizard_Profile(void)
{
    int16_t Realm_Retort_Available = 0;
    int16_t Realm_Index = 0;
    int16_t Bookshelf[5] = { 0, 0, 0, 0, 0 };
    int16_t Myrran_AI_Count = 0;
    int16_t Picks_Left = 0;
    int16_t Banners_Taken[5] = { 0, 0, 0, 0, 0 };
    int8_t * wsa_ptr = 0;  // Pointer to 'Wizard Special Abilities'
    int16_t Book_Count = 0;
    int16_t Random_Banner = 0;
    int16_t Trait_Value = 0;  // NOTE: used to index wsa_ptr[]
    int16_t Picks_Used = 0;
    int16_t Trait_Type = 0;
    int16_t Erasure_Dice = 0;
    int16_t Random_Result = 0;
    int16_t Portrait_Taken = 0;
    int16_t itr3 = 0;
    int16_t itr1 = 0;  // _DI_
    int16_t itr2 = 0;  // _SI_

    // ; clear the taken banner indicators
    for(itr1 = 0; itr1 < NUM_BANNER_SELECTIONS; itr1++)
    {
        Banners_Taken[itr1] = ST_FALSE;
    }

    Banners_Taken[_players[0].banner_id] = ST_TRUE;

    // ; choose a different random portrait for each AI wizard
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {

        Portrait_Taken = ST_FALSE;

        Random_Result = Random(14);

        // ; check if the rolled portrait is already taken
        for(itr3 = 0; itr3 < itr2; itr3++)
        {

            if(_players[itr3].wizard_id == Random_Result)
            {

                Portrait_Taken = ST_TRUE;

            }

        }

        if(Portrait_Taken != ST_TRUE)
        {

            _players[itr2].wizard_id = (uint8_t)Random_Result;

        }
    }

    // ; copy the names corresponding to the chosen portraits
    // ; into the wizard records of each AI player
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {

        strcpy(_players[itr2].name, _wizard_presets_table[_players[itr2].wizard_id].name);

    }

    // ; copy the default profile traits into each AI wizard's
    // ; record, and if the difficulty is above normal, roll
    // ; a 20% chance to combine the books of any 2-realm
    // ; wizard into a single realm
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        
        _players[itr2].spellranks[sbr_Life] = _wizard_presets_table[_players[itr2].wizard_id].life;
        _players[itr2].spellranks[sbr_Death] = _wizard_presets_table[_players[itr2].wizard_id].death;
        _players[itr2].spellranks[sbr_Chaos] = _wizard_presets_table[_players[itr2].wizard_id].chaos;
        _players[itr2].spellranks[sbr_Nature] = _wizard_presets_table[_players[itr2].wizard_id].nature;
        _players[itr2].spellranks[sbr_Sorcery] = _wizard_presets_table[_players[itr2].wizard_id].sorcery;

        if(
            (Random(5) == 1)  // 1:5, 20%
            &&
            (_difficulty > god_Normal)
        )
        {

            Consolidate_Spell_Book_Realms(itr2);

        }

        wsa_ptr = &_players[itr2].alchemy;

        // ; clear the wizard's retorts
        for(itr1 = 0; itr1 < NUM_WIZARD_SPECIAL_ABILITIES; itr1++)
        {

            wsa_ptr[itr1] = 0;

        }

        if(_wizard_presets_table[_players[itr2].wizard_id].special != ST_UNDEFINED)
        {

            wsa_ptr[_wizard_presets_table[_players[itr2].wizard_id].special] = ST_TRUE;

        }

    }

    // ; if the difficulty is normal or above, roll to try and
    // ; remove some of the default profile traits
    for(itr2 = 0; itr2 < _num_players; itr2++)
    {

        switch(_difficulty)
        {
            case 0:
            case 1:
            {
                Erasure_Dice = 0;
            } break;
            case 2:
            {
                Erasure_Dice = Random(3);
            } break;
            case 3:
            {
                Erasure_Dice = (Random(3) + Random(3));
            } break;
            case 4:
            {
                Erasure_Dice = (Random(5) + Random(5));
            } break;
            case 5:
            {
                Erasure_Dice = (Random(8) + Random(8));
            } break;
            
        }

        wsa_ptr = &_players[itr2].alchemy;

        // ; for each erasure die, roll a type (N-C-S-L-D book or
        // ; retort), and if the wizard has such an asset, remove
        // ; one of them
        for(itr1 = 0; itr1 < Erasure_Dice; itr1++)
        {

            Trait_Type = Random(6);

            switch(Trait_Type)  // - 1; <= 5;
            {
                case 1: { if(_players[itr2].spellranks[sbr_Life]    > 1) { _players[itr2].spellranks[sbr_Life]    -= 1; } } break;
                case 2: { if(_players[itr2].spellranks[sbr_Death]   > 1) { _players[itr2].spellranks[sbr_Death]   -= 1; } } break;
                case 3: { if(_players[itr2].spellranks[sbr_Chaos]   > 1) { _players[itr2].spellranks[sbr_Chaos]   -= 1; } } break;
                case 4: { if(_players[itr2].spellranks[sbr_Nature]  > 1) { _players[itr2].spellranks[sbr_Nature]  -= 1; } } break;
                case 5: { if(_players[itr2].spellranks[sbr_Sorcery] > 1) { _players[itr2].spellranks[sbr_Sorcery] -= 1; } } break;
                case 6: {
                    wsa_ptr = &_players[itr2].alchemy;
                    for(itr3 = 0; itr3 < NUM_WIZARD_SPECIAL_ABILITIES; itr3++)
                    {
                        wsa_ptr[itr1] = ST_FALSE;
                    }
                } break;
            }

        }

    }

    // ; if the AI wizards have any picks left, spend them on
    // ; random profile traits, up to the predefined maximum
    // ; of 11 on Normal and below, 13 on Hard, or 15 on the
    // ; Impossible difficulty
    for(itr2 = 0; itr2 < _num_players; itr2++)
    {

        wsa_ptr = &_players[itr2].alchemy;

        Picks_Used = (
            _players[itr2].spellranks[sbr_Life]
            +
            _players[itr2].spellranks[sbr_Death]
            +
            _players[itr2].spellranks[sbr_Nature]
            +
            _players[itr2].spellranks[sbr_Chaos]
            +
            _players[itr2].spellranks[sbr_Sorcery]
        );

        Book_Count = Picks_Used;

        // ; calculate the total cost of the retorts in the
        // ; wizard's profile
        for(itr1 = 0; itr1 < NUM_WIZARD_SPECIAL_ABILITIES; itr1++)
        {

            if(wsa_ptr[itr1] != 0)
            {

                if(itr1 == wsa_Myrran)
                {

                    Picks_Used += 3;

                }
                else if(
                    (itr1 == wsa_Warlord)
                    ||
                    (itr1 == wsa_Infernal_Power)
                    ||
                    (itr1 == wsa_Divine_Power)
                    ||
                    (itr1 == wsa_Famous)
                    ||
                    (itr1 == wsa_Channeller)
                )
                {

                    Picks_Used += 2;

                }
                else
                {

                    Picks_Used += 1;

                }

            }

        }

        Picks_Left = 11;

        if(_difficulty == god_Hard)
        {
            Picks_Left = 13;
        }

        if(_difficulty == god_Impossible)
        {
            Picks_Left = 15;
        }

        // ; spend any remaining picks on random profile traits
        while(Picks_Used < Picks_Left)
        {

            Trait_Type = Random(8);

            Trait_Value = Random(4);  // NOTE: used to index wsa_ptr[]

            if((Trait_Value + Picks_Used) > Picks_Left)
            {

                Trait_Value = (Picks_Left - Picks_Used);

            }

            if((Trait_Value + Book_Count) > 12)
            {

                Trait_Value = (12 - Book_Count);

            }

            if(Trait_Value <= 0)
            {

                Trait_Type = 6;

            }

            if(Trait_Type == 6)
            {

                Trait_Value = 1;

            }

            switch(Trait_Type)  // - 1; <= 7
            {

                // DNE  case 0:

                case 1:  // add books to Life
                {

                    if(
                        (_players[itr2].spellranks[sbr_Life] < 11)
                        &&
                        (_players[itr2].spellranks[sbr_Death] == 0)
                    )
                    {

                        _players[itr2].spellranks[sbr_Life] += Trait_Value;

                    }

                    if(_players[itr2].spellranks[sbr_Life] > 11)
                    {

                        _players[itr2].spellranks[sbr_Life] = 11;

                    }

                } break;

                case 2:  // add books to Death
                {

                    if(
                        (_players[itr2].spellranks[sbr_Death] < 11)
                        &&
                        (_players[itr2].spellranks[sbr_Life] == 0)
                    )
                    {

                        _players[itr2].spellranks[sbr_Death] += Trait_Value;

                    }

                    if(_players[itr2].spellranks[sbr_Death] > 11)
                    {

                        _players[itr2].spellranks[sbr_Death] = 11;

                    }

                } break;

                case 3:  // add books to Nature
                {

                    if(
                        (_players[itr2].spellranks[sbr_Nature] < 11)
                    )
                    {

                        _players[itr2].spellranks[sbr_Nature] += Trait_Value;

                    }

                    if(_players[itr2].spellranks[sbr_Nature] > 11)
                    {

                        _players[itr2].spellranks[sbr_Nature] = 11;

                    }

                } break;

                case 4:  // add books to Sorcery
                {

                    if(
                        (_players[itr2].spellranks[sbr_Sorcery] < 11)
                    )
                    {

                        _players[itr2].spellranks[sbr_Sorcery] += Trait_Value;

                    }

                    if(_players[itr2].spellranks[sbr_Sorcery] > 11)
                    {

                        _players[itr2].spellranks[sbr_Sorcery] = 11;

                    }

                } break;

                case 5:  // add books to Chaos
                {

                    if(
                        (_players[itr2].spellranks[sbr_Chaos] < 11)
                    )
                    {

                        _players[itr2].spellranks[sbr_Chaos] += Trait_Value;

                    }

                    if(_players[itr2].spellranks[sbr_Chaos] > 11)
                    {

                        _players[itr2].spellranks[sbr_Chaos] = 11;

                    }

                } break;

                case 6:
                case 7:
                case 8:
                {

                    Trait_Value = (Random(NUM_WIZARD_SPECIAL_ABILITIES) - 1);

                    if(
                        (Trait_Value >= wsa_Chaos_Mastery)
                        &&
                        (Trait_Value <= wsa_Divine_Power)
                    )
                    {

                        // ; copy the wizard's bookshelf into the local array
                        for(Realm_Index = 0; Realm_Index < 4; Realm_Index++)
                        {

                            Bookshelf[Realm_Index] = _players[itr2].spellranks[Realm_Index];

                        }

                        if(_players[itr2].chaos_mastery > 0)
                        {

                            Bookshelf[sbr_Chaos] = 0;

                        }

                        if(_players[itr2].nature_mastery > 0)
                        {

                            Bookshelf[sbr_Nature] = 0;

                        }

                        if(_players[itr2].sorcery_mastery > 0)
                        {

                            Bookshelf[sbr_Sorcery] = 0;

                        }

                        if(_players[itr2].divine_power <= 0)
                        {

                            if((Picks_Left - 2) < Picks_Used)
                            {

                                Bookshelf[sbr_Life] = 0;

                            }

                        }

                        if(_players[itr2].infernal_power <= 0)
                        {

                            if((Picks_Left - 2) < Picks_Used)
                            {

                                Bookshelf[sbr_Death] = 0;

                            }

                        }

                        Realm_Retort_Available = 0;

                        for(Realm_Index = 0; Realm_Index < 4; Realm_Index++)
                        {

                            Realm_Retort_Available += Bookshelf[Realm_Index];

                        }

                        if(Realm_Retort_Available != 0)
                        {

                            Trait_Value = Get_Weighted_Choice(Bookshelf, 5);

                            if(Trait_Value == 2)
                            {
                                
                                Trait_Value = wsa_Nature_Mastery;

                            }
                            else if(Trait_Value == 3)
                            {

                                Trait_Value = wsa_Sorcery_Mastery;

                            }
                            else if(Trait_Value == 4)
                            {

                                Trait_Value = wsa_Chaos_Mastery;

                            }
                            else if(Trait_Value == 5)
                            {

                                Trait_Value = wsa_Divine_Power;

                            }
                            else if(Trait_Value == 6)
                            {

                                Trait_Value = wsa_Infernal_Power;

                            }

                            wsa_ptr[Trait_Value] = ST_TRUE;

                        }

                    }
                    else
                    {

                        if(
                            (Trait_Value == wsa_Warlord)
                            ||
                            (Trait_Value == wsa_Channeller)
                            ||
                            (Trait_Value == wsa_Famous)
                        )
                        {

                            if((Picks_Left - 1) > Picks_Used)
                            {

                                wsa_ptr[Trait_Value] = ST_TRUE;

                            }

                        }
                        else if(Trait_Value == wsa_Myrran)
                        {

                            if((Picks_Left - 2) > Picks_Used)
                            {

                                wsa_ptr[Trait_Value] = ST_TRUE;

                            }

                        }

                    }


                } break;

            }

            // after ... switch(Trait_Type)  // - 1; <= 7 ```dasm @@AfterSwitchTrait_Type:                  ; default ```

            Picks_Used = (
                _players[itr2].spellranks[sbr_Life]
                +
                _players[itr2].spellranks[sbr_Death]
                +
                _players[itr2].spellranks[sbr_Nature]
                +
                _players[itr2].spellranks[sbr_Chaos]
                +
                _players[itr2].spellranks[sbr_Sorcery]
            );

            Book_Count = Picks_Used;

            for(itr1 = 0; itr1 < NUM_WIZARD_SPECIAL_ABILITIES; itr1++)
            {

                if(wsa_ptr[itr1] != ST_FALSE)
                {

                    if(itr1 == wsa_Myrran)
                    {

                        Picks_Used += 3;

                    }
                    else if(
                            (Trait_Value == wsa_Warlord)
                            ||
                            (Trait_Value == wsa_Infernal_Power)
                            ||
                            (Trait_Value == wsa_Divine_Power)
                            ||
                            (Trait_Value == wsa_Famous)
                            ||
                            (Trait_Value == wsa_Channeller)
                    )
                    {

                        Picks_Used += 3;

                    }
                    else
                    {

                        Picks_Used += 1;

                    }

                }

            }

        }


// WTF?!?
        if(
            !(
                (
                    (Myrran_AI_Count < 1)
                    ||
                    (Myrran_AI_Count > 2)
                )
                &&
                (_difficulty > god_Normal)
                &&
                (_num_players > 3)
            )
        )
        {
            
        }
        else
        {
            // @@BeginTopLevelPlayerLoop
        }

    }
/*
END:  ¿ jmp     @@BeginTopLevelPlayerLoop ?
*/

    // ; count the AI wizards with the Myrran retort
    Myrran_AI_Count = 0;
    for(itr1 = 1; itr1 < _num_players; itr1++)
    {

        if(_players[itr1].myrran != ST_FALSE)
        {
        
            Myrran_AI_Count++;

        }

    }

// ovr056:15E7 83 3E D2 87 03                                  cmp     [_num_players], 3               ; NewGame: magic.opponents + 1
// ovr056:15EC 7E 0F                                           jle     short loc_538DD
// ovr056:15EC
// ovr056:15EE E9 7D F7                                        jmp     loc_5304E
// ovr056:15EE
// ovr056:15F1                                                 ; ---------------------------------------------------------------------------
// ovr056:15F1 C7 06 7E 2B FF FF                               mov     [_wizard_presets_table.special+9Ah], -1
// ovr056:15F7 C7 06 7C 2B 07 00                               mov     [_wizard_presets_table.chaos+9Ah], 7
// ovr056:15F7
// ovr056:15FD
// ovr056:15FD                                                 loc_538DD:                              ; CODE XREF: Init_Computer_Players_Wizard_Profile+8A0j ...
// ovr056:15FD BE 01 00                                        mov     si, 1
// ovr056:1600 E9 EC 00                                        jmp     loc_539CF

    // ; assign a banner to each wizard, trying first the
    // ; color corresponding to the first realm they have at
    // ; least 4 books in, and choosing randomly if that one
    // ; is already taken
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {

        if(_players[itr2].spellranks[sbr_Nature] > 3)
        {

            if(Banners_Taken[1] == ST_FALSE)
            {

                _players[itr2].banner_id = BNR_Green;

                Banners_Taken[1] = ST_TRUE;

            }

        }
        else if(_players[itr2].spellranks[sbr_Chaos] > 3)
        {

            if(Banners_Taken[3] == ST_FALSE)
            {

                _players[itr2].banner_id = BNR_Red;

                Banners_Taken[3] = ST_TRUE;

            }

        }
        else if(_players[itr2].spellranks[sbr_Sorcery] > 3)
        {

            if(Banners_Taken[0] == ST_FALSE)
            {

                _players[itr2].banner_id = BNR_Blue;

                Banners_Taken[0] = ST_TRUE;

            }

        }
        else if(_players[itr2].spellranks[sbr_Death] > 3)
        {

            if(Banners_Taken[2] == ST_FALSE)
            {

                _players[itr2].banner_id = BNR_Purple;

                Banners_Taken[2] = ST_TRUE;

            }

        }
        else
        {

            Random_Banner = (Random(5) - 1);

            Banners_Taken[Random_Banner] = ST_TRUE;

        }

    }

    // @@Done

}


// MGC o56p10
// drake178: WIZ_SetSpells()
/*
; sets the known and researchable spells for all
; wizards based on their books and spell selection
; tables
*/
/*
OON XREF WIZ_SetProfiles()

iter over players
iter over realms
...
per rarity
    clear temp array
    switch on book count, set knowable limit
    randomly select spells into the temp array
    transfer the temp array to the player's spell library

*/
void Init_Computer_Players_Spell_Library(void)
{
    int16_t Availability_Array[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // 2-byte, signed, sizeof 10
    int16_t Available_Spells = 0;
    int16_t Common_Index = 0;
    int16_t InRarity_Index = 0;
    int16_t Availability_Limit = 0;
    int16_t sri = 0;
    int16_t sbr = 0;
    int16_t itr = 0;
    int16_t IDK_itr_10 = 0;  // _DI_

    for(itr = 0; itr < _num_players; itr++)
    {

        // clear the wizard's entire spell array
        for(sbr = 0; sbr < NUM_MAGIC_REALMS; sbr++)
        {
            for(sri = 0; sri < NUM_SPELLS_PER_MAGIC_REALM; sri++)
            {
                _players[itr].spells_list[((sbr * 40) + sri)] = 0;
            }
        }

        for(sbr = 0; sbr < NUM_MAGIC_REALMS; sbr++)
        {

            if(sbr != sbr_Arcane)
            {

                if(_players[itr].spellranks[sbr] != 0)
                {

                    if(_players[itr].spellranks[sbr] != 11)
                    {

                        // ; clear the availability array (not used for commons)

                        for(IDK_itr_10 = 0; IDK_itr_10 < 10; IDK_itr_10++)
                        {

                            Availability_Array[IDK_itr_10] = 0;

                        }

                        // ; set the availability limit for the common spells of
                        // ; the realm based on the book count
                        
                        switch(_players[itr].spellranks[sbr])  // - 1; <= 9
                        {
                            /* DNE  case 0: { } break; */
                            case  1: { Availability_Limit =  3; } break;
                            case  2: { Availability_Limit =  5; } break;
                            case  3: { Availability_Limit =  6; } break;
                            case  4: { Availability_Limit =  7; } break;
                            case  5: { Availability_Limit =  8; } break;
                            case  6: { Availability_Limit =  9; } break;
                            case  7: { Availability_Limit = 10; } break;
                            case  8: { Availability_Limit = 10; } break;
                            case  9: { Availability_Limit = 10; } break;
                            case 10: { Availability_Limit = 10; } break;
                        }

                        // ; copy up to one less common spell from their
                        // ; selection than the wizard has nature books
                        switch(sbr)
                        {
                            case sbr_Nature:
                            {
                                for(IDK_itr_10 = 0; (_players[itr].spellranks[sbr] - 1) > IDK_itr_10; IDK_itr_10++)
                                {
                                    _players[itr].spells_list[((_player_start_spells[itr].realms[sbr].spells[((sbr_Nature * 13) + IDK_itr_10)] - 1) / NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                                }
                            } break;
                            case sbr_Sorcery:
                            {
                                for(IDK_itr_10 = 0; (_players[itr].spellranks[sbr] - 1) > IDK_itr_10; IDK_itr_10++)
                                {
                                    _players[itr].spells_list[((_player_start_spells[itr].realms[sbr].spells[((sbr_Sorcery * 13) + IDK_itr_10)] - 1) / NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                                }
                            } break;
                            case sbr_Chaos:
                            {
                                for(IDK_itr_10 = 0; (_players[itr].spellranks[sbr] - 1) > IDK_itr_10; IDK_itr_10++)
                                {
                                    _players[itr].spells_list[((_player_start_spells[itr].realms[sbr].spells[((sbr_Chaos * 13) + IDK_itr_10)] - 1) / NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                                }
                            } break;
                            case sbr_Life:
                            {
                                for(IDK_itr_10 = 0; (_players[itr].spellranks[sbr] - 1) > IDK_itr_10; IDK_itr_10++)
                                {
                                    _players[itr].spells_list[((_player_start_spells[itr].realms[sbr].spells[((sbr_Life * 13) + IDK_itr_10)] - 1) / NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                                }
                            } break;
                            case sbr_Death:
                            {
                                for(IDK_itr_10 = 0; (_players[itr].spellranks[sbr] - 1) > IDK_itr_10; IDK_itr_10++)
                                {
                                    _players[itr].spells_list[((_player_start_spells[itr].realms[sbr].spells[((sbr_Death * 13) + IDK_itr_10)] - 1) / NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                                }
                            } break;
                        }

                        Available_Spells = (_players[itr].spellranks[sbr] - 1);

                        // ; mark random common spells from the realm as
                        // ; researchable until the availability limit is reached

                        while(Available_Spells < Availability_Limit)
                        {

                            IDK_itr_10 = (Random(10) - 1);

                            if(_players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + IDK_itr_10)] == sls_Unknown)
                            {

                                _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + IDK_itr_10)] = sls_Knowable;

                            }

                            Available_Spells = 0;

                            for(IDK_itr_10 = 0; IDK_itr_10 < 10; IDK_itr_10++)
                            {

                                if(_players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + IDK_itr_10)] == sls_Unknown)
                                {

                                    Available_Spells++;

                                }

                            }

                        }

/*
    BEGIN: Knowable - Uncommon
*/

                        if(_players[itr].spellranks[sbr] > 7)
                        {

                            // ; mark all uncommon spells of the realm as researchable

                            for(IDK_itr_10 = 0; IDK_itr_10 < 10; IDK_itr_10++)
                            {

                                _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 10 + IDK_itr_10)] = sls_Knowable;

                            }

                        }
                        else
                        {

                            // ; clear the availability array
                            for(IDK_itr_10 = 0; IDK_itr_10 < NUM_SPELLS_PER_MAGIC_RARITY; IDK_itr_10++)
                            {

                                Availability_Array[IDK_itr_10] = 0;

                            }

                            // ; set the availability limit for the uncommon spells of
                            // ; the realm based on the book count
                            switch(_players[itr].spellranks[sbr])  // - 1; <= 9
                            {
                                /* DNE  case 0: { } break; */
                                case  1: { Availability_Limit =  1; } break;
                                case  2: { Availability_Limit =  2; } break;
                                case  3: { Availability_Limit =  3; } break;
                                case  4: { Availability_Limit =  4; } break;
                                case  5: { Availability_Limit =  5; } break;
                                case  6: { Availability_Limit =  6; } break;
                                case  7: { Availability_Limit =  8; } break;
                                case  8: { Availability_Limit = 10; } break;
                                case  9: { Availability_Limit = 10; } break;
                                case 10: { Availability_Limit = 10; } break;
                            }

                            // ; fill the availability array with random uncommon
                            // ; spells up to the limit
                            for(IDK_itr_10 = 0; IDK_itr_10 < Availability_Limit; IDK_itr_10++)
                            {

                                InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);

                                if(Availability_Array[InRarity_Index] != 1)
                                {

                                    Availability_Array[InRarity_Index] = 1;

                                }

                            }

                            // ; copy the array into the wizard's profile as uncommon
                            // ; researchable spells

                            for(IDK_itr_10 = 0; IDK_itr_10 < NUM_SPELLS_PER_MAGIC_RARITY; IDK_itr_10++)
                            {

                                if(Availability_Array[IDK_itr_10] == 1)
                                {

                                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 10 + IDK_itr_10)] = sls_Knowable;

                                }

                            }

                        }

/*
    END: Knowable - Uncommon
*/

/*
    BEGIN: Knowable - Rare
*/
                        
                        if(_players[itr].spellranks[sbr] > 9)
                        {

                            // ; mark all rare spells of the realm as researchable

                            for(IDK_itr_10 = 0; IDK_itr_10 < NUM_SPELLS_PER_MAGIC_REALM; IDK_itr_10++)
                            {

                                _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 20 + IDK_itr_10)] = sls_Knowable;

                            }

                        }
                        else
                        {

                            // ; clear the availability array
                            for(IDK_itr_10 = 0; IDK_itr_10 < NUM_SPELLS_PER_MAGIC_RARITY; IDK_itr_10++)
                            {

                                Availability_Array[IDK_itr_10] = 0;

                            }

                            // ; set the availability limit for the rare spells of
                            // ; the realm based on the book count
                            switch(_players[itr].spellranks[sbr])  // - 1; <= 8
                            {
                                /* DNE  case 0: { } break; */
                                case  1: { Availability_Limit = 0; } break;
                                case  2: { Availability_Limit = 1; } break;
                                case  3: { Availability_Limit = 2; } break;
                                case  4: { Availability_Limit = 3; } break;
                                case  5: { Availability_Limit = 4; } break;
                                case  6: { Availability_Limit = 5; } break;
                                case  7: { Availability_Limit = 6; } break;
                                case  8: { Availability_Limit = 7; } break;
                                case  9: { Availability_Limit = 9; } break;
                                // WTF  DNE  case 10: { Availability_Limit = ; } break;
                            }

                            // ; fill the availability array with random rare
                            // ; spells up to the limit
                            for(IDK_itr_10 = 0; IDK_itr_10 < Availability_Limit; IDK_itr_10++)
                            {

                                InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);

                                if(Availability_Array[InRarity_Index] != 1)
                                {

                                    Availability_Array[InRarity_Index] = 1;

                                }

                            }

                            // ; copy the array into the wizard's profile as rare
                            // ; researchable spells

                            for(IDK_itr_10 = 0; IDK_itr_10 < NUM_SPELLS_PER_MAGIC_RARITY; IDK_itr_10++)
                            {

                                if(Availability_Array[IDK_itr_10] == 1)
                                {

                                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 20 + IDK_itr_10)] = sls_Knowable;

                                }

                            }

                        }

/*
    END: Knowable - Rare
*/

/*
    BEGIN: Knowable - Very Rare
*/
                        
                        if(_players[itr].spellranks[sbr] > 9)
                        {

                            // ; mark all very rare spells of the realm as researchable

                            for(IDK_itr_10 = 0; IDK_itr_10 < NUM_SPELLS_PER_MAGIC_REALM; IDK_itr_10++)
                            {

                                _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 30 + IDK_itr_10)] = sls_Knowable;

                            }

                        }
                        else
                        {

                            // ; clear the availability array
                            for(IDK_itr_10 = 0; IDK_itr_10 < NUM_SPELLS_PER_MAGIC_RARITY; IDK_itr_10++)
                            {

                                Availability_Array[IDK_itr_10] = 0;

                            }

                            // ; set the availability limit for the very rare spells of
                            // ; the realm based on the book count
                            switch(_players[itr].spellranks[sbr])  // - 1; <= 8
                            {
                                /* DNE  case 0: { } break; */
                                case  1: { Availability_Limit = 0; } break;
                                case  2: { Availability_Limit = 0; } break;
                                case  3: { Availability_Limit = 1; } break;
                                case  4: { Availability_Limit = 2; } break;
                                case  5: { Availability_Limit = 3; } break;
                                case  6: { Availability_Limit = 4; } break;
                                case  7: { Availability_Limit = 5; } break;
                                case  8: { Availability_Limit = 6; } break;
                                case  9: { Availability_Limit = 7; } break;
                                // WTF  DNE  case 10: { Availability_Limit = ; } break;
                            }

                            // ; fill the availability array with random very rare
                            // ; spells up to the limit
                            for(IDK_itr_10 = 0; IDK_itr_10 < Availability_Limit; IDK_itr_10++)
                            {

                                InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);

                                if(Availability_Array[InRarity_Index] != 1)
                                {

                                    Availability_Array[InRarity_Index] = 1;

                                }

                            }

                            // ; copy the array into the wizard's profile as very rare
                            // ; researchable spells

                            for(IDK_itr_10 = 0; IDK_itr_10 < NUM_SPELLS_PER_MAGIC_RARITY; IDK_itr_10++)
                            {

                                if(Availability_Array[IDK_itr_10] == 1)
                                {

                                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 30 + IDK_itr_10)] = sls_Knowable;

                                }

                            }

                        }

/*
    END: Knowable - Very Rare
*/

                    }
                    else
                    {

                        // ; mark all spells of the realm as researchable
                        for(IDK_itr_10 = 0; IDK_itr_10 < NUM_SPELLS_PER_MAGIC_REALM; IDK_itr_10++)
                        {

                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + IDK_itr_10)] = 1;

                        }

                        switch(sbr)
                        {
                            case 0:
                            {
                                for(Common_Index = 0; Common_Index < 10; Common_Index++)
                                {
                                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                                }
                                // IDGI
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                            } break;
                            case 1:
                            {
                                for(Common_Index = 0; Common_Index < 10; Common_Index++)
                                {
                                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                                }
                                // IDGI
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                            } break;
                            case 2:
                            {
                                for(Common_Index = 0; Common_Index < 10; Common_Index++)
                                {
                                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                                }
                                // IDGI
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                            } break;
                            case 3:
                            {
                                for(Common_Index = 0; Common_Index < 10; Common_Index++)
                                {
                                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                                }
                                // IDGI
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                            } break;
                            case 4:
                            {
                                for(Common_Index = 0; Common_Index < 10; Common_Index++)
                                {
                                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                                }
                                // IDGI
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                                // TODO  _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + _player_start_spells[itr].realms[sbr].spells[])] = sls_Known;
                            } break;
                        }

                    }

                }

            }
            else
            {

                _players[itr].spells_list[((sbr * 40) +  0)] = sls_Knowable;  // spl_Demon_Lord
                _players[itr].spells_list[((sbr * 40) +  1)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  2)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  3)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  4)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  5)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  6)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  7)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  8)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) +  9)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) + 10)] = sls_Knowable;
                _players[itr].spells_list[((sbr * 40) + 11)] = sls_Knowable;
                // WTF. Why not 0x0C?  ¿ spl_Summon_Champion ?  _players[itr].spells_list[((sbr * 40) + 12)] = 1;
                _players[itr].spells_list[((sbr * 40) + 13)] = sls_Known;  // spl_Spell_Of_Mastery

            }

        }

        _players[itr].spells_list[spl_Demon_Lord] = sls_Known;

        if(_players[itr].artificer != 0)
        {

            _players[itr].spells_list[spl_Detect_Magic] = sls_Known;

            _players[itr].spells_list[spl_Disjunction] = sls_Known;
            
        }

    }

}


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
            _HEROES2[itr_players]->heroes[itr_hero_types].Casting_Skill = (int8_t)casting_skill;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[0] = (uint8_t)_hero_premade_table[itr_hero_types].spell_1;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[1] = (uint8_t)_hero_premade_table[itr_hero_types].spell_2;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[2] = (uint8_t)_hero_premade_table[itr_hero_types].spell_3;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[3] = (uint8_t)_hero_premade_table[itr_hero_types].spell_4;

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
/*
; PATCHED / rewritten
;
; if the wizard has two different realms of spellbooks,
; consolidates them into a single realm, chosen
; randomly from the two they have
;
; INCONSISTENT: ignores the Chaos + Nature combination
; WARNING: will not work properly if defaults are
;  altered to have 3 realms or Life + Death
*/
/*
  Death & Chaos
  Death & Nature
  Death & Sorcery
   Life & Chaos
   Life & Nature
   Life & Sorcery
Sorcery & Chaos
Sorcery & Nature
*/
void Consolidate_Spell_Book_Realms(int16_t player_idx)
{

    /*
        BEGIN:  Death & Chaos
    */
    if((_players[player_idx].spellranks[sbr_Death] != 0) && (_players[player_idx].spellranks[sbr_Chaos] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Death] += _players[player_idx].spellranks[sbr_Chaos];
            _players[player_idx].spellranks[sbr_Chaos] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Chaos] += _players[player_idx].spellranks[sbr_Death];
            _players[player_idx].spellranks[sbr_Death] = 0;
        }
    }
    /*
        END:  Death & Chaos
    */

    /*
        BEGIN:  Death & Nature
    */
    if((_players[player_idx].spellranks[sbr_Death] != 0) && (_players[player_idx].spellranks[sbr_Nature] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Death] += _players[player_idx].spellranks[sbr_Nature];
            _players[player_idx].spellranks[sbr_Nature] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Nature] += _players[player_idx].spellranks[sbr_Death];
            _players[player_idx].spellranks[sbr_Death] = 0;
        }
    }
    /*
        END:  Death & Nature
    */
   
    /*
        BEGIN:  Death & Sorcery
    */
    if((_players[player_idx].spellranks[sbr_Death] != 0) && (_players[player_idx].spellranks[sbr_Sorcery] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Death] += _players[player_idx].spellranks[sbr_Sorcery];
            _players[player_idx].spellranks[sbr_Sorcery] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Sorcery] += _players[player_idx].spellranks[sbr_Death];
            _players[player_idx].spellranks[sbr_Death] = 0;
        }
    }
    /*
        END:  Death & Sorcery
    */
   
    /*
        BEGIN:  Life & Chaos
    */
    if((_players[player_idx].spellranks[sbr_Life] != 0) && (_players[player_idx].spellranks[sbr_Chaos] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Life] += _players[player_idx].spellranks[sbr_Chaos];
            _players[player_idx].spellranks[sbr_Chaos] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Chaos] += _players[player_idx].spellranks[sbr_Life];
            _players[player_idx].spellranks[sbr_Life] = 0;
        }
    }
    /*
        END:  Life & Chaos
    */
   
    /*
        BEGIN:  Life & Nature
    */
    if((_players[player_idx].spellranks[sbr_Life] != 0) && (_players[player_idx].spellranks[sbr_Nature] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Life] += _players[player_idx].spellranks[sbr_Nature];
            _players[player_idx].spellranks[sbr_Nature] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Nature] += _players[player_idx].spellranks[sbr_Life];
            _players[player_idx].spellranks[sbr_Life] = 0;
        }
    }
    /*
        END:  Life & Nature
    */
   
    /*
        BEGIN:  Life & Sorcery
    */
    if((_players[player_idx].spellranks[sbr_Life] != 0) && (_players[player_idx].spellranks[sbr_Sorcery] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Life] += _players[player_idx].spellranks[sbr_Sorcery];
            _players[player_idx].spellranks[sbr_Sorcery] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Sorcery] += _players[player_idx].spellranks[sbr_Life];
            _players[player_idx].spellranks[sbr_Life] = 0;
        }
    }
    /*
        END:  Life & Sorcery
    */
   
    /*
        BEGIN:  Sorcery & Chaos
    */
    if((_players[player_idx].spellranks[sbr_Sorcery] != 0) && (_players[player_idx].spellranks[sbr_Chaos] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Sorcery] += _players[player_idx].spellranks[sbr_Chaos];
            _players[player_idx].spellranks[sbr_Chaos] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Chaos] += _players[player_idx].spellranks[sbr_Sorcery];
            _players[player_idx].spellranks[sbr_Sorcery] = 0;
        }
    }
    /*
        END:  Sorcery & Chaos
    */
   
    /*
        BEGIN:  Sorcery & Nature
    */
    if((_players[player_idx].spellranks[sbr_Sorcery] != 0) && (_players[player_idx].spellranks[sbr_Nature] != 0))
    {
        if(Random(2) == 1)
        {
            _players[player_idx].spellranks[sbr_Sorcery] += _players[player_idx].spellranks[sbr_Nature];
            _players[player_idx].spellranks[sbr_Nature] = 0;
        }
        else
        {
            _players[player_idx].spellranks[sbr_Nature] += _players[player_idx].spellranks[sbr_Sorcery];
            _players[player_idx].spellranks[sbr_Sorcery] = 0;
        }
    }
    /*
        END:  Sorcery & Nature
    */

}


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
