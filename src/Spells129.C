/*
    WIZARDS.EXE
        ovr129
*/

#include "MOM_DEF.H"
#include "MOX/Allocate.H"
#include "MOX/MOM_Data.H"
#include "MOX/MOX_DAT.H"

#include "MOX/MOX_DEF.H"
#include "NEXTTURN.H"
#include "OverSpel.H"
#include "SBookScr.H"
#include "SPELLDEF.H"
#include "Spellbook.H"
#include "Spells129.H"
#include "Spells137.H"

int16_t * CMB_HolyBonusArray;



// WZD dseg:6686                                                 BEGIN:  ovr129 - Initialized Data

// WZD dseg:6686
char aCallTheVoid[] = "Call the Void";
// WZD dseg:6694
char str_Consecration__ovr129[] = "Consecration";
// WZD dseg:66A1
char str_DeathWard__ovr129[] = "Death Ward";
// WZD dseg:66AC
char str_LifeWard__ovr129[] = "Life Ward";
// WZD dseg:66B6
char str_NatureWard__ovr129[] = "Nature Ward";
// WZD dseg:66C2
char str_ChaosWard__ovr129[] = "Chaos Ward";
// WZD dseg:66CD
char str_SorceryWard__ovr129[] = "Sorcery Ward";
// WZD dseg:66DA
char str_Nightshade__ovr129[] = "Nightshade";
// WZD dseg:66E5
char message_lbx_file__ovr129[] = "message";
// WZD dseg:66ED
char aChaosChannels[] = "Chaos Channels";
// WZD dseg:66FC
char aWarpNode[] = "Warp Node";
// WZD dseg:6706
char cnst_Stasis_2[] = "Stasis";
// WZD dseg:670D
char aNatureSCures[] = "Nature's Cures";
// WZD dseg:671C
char aMoveFortress[] = "Move Fortress";
// WZD dseg:672A
char aEarthquake[] = "Earthquake";
// WZD dseg:6735
char music_lbx_file__ovr129[] = "music";
// WZD dseg:673B
char aNatureSWrathSt[] = "Nature's Wrath strikes ";
// WZD dseg:6753
char aYou[] = "you";
// WZD dseg:6757
char specfx_lbx_file__ovr129[] = "Specfx";
// WZD dseg:675E
char backgrnd__ovr129[] = "BACKGRND";

// WZD dseg:6767 00                                              align 2




/*
    WIZARDS.EXE  ovr129
*/

// WZD o129p01
/*
191  spl_Cruel_Unminding
Cruel Unminding:
Death. Instant. Casting Cost: 150 mana. Very Rare.
Permanently destroys one to 10% of a target wizard’s total spell casting skill points!
*/
int16_t Apply_Cruel_Unminding(int16_t player_idx)
{
    int16_t current_skill = 0;
    int16_t twos = 0;
    int16_t num = 0;
    int16_t ctr = 0;
    int16_t count = 0;
    int16_t random_skill = 0;  // _SI_

    current_skill = Player_Base_Casting_Skill(player_idx);

    random_skill = Random(10);

    random_skill = ((current_skill * random_skill) / 100);

    if(random_skill == 0)
    {
        random_skill = 1;
    }

    count = (current_skill - random_skill);

    if(_players[player_idx].archmage > 0)
    {
        count -= 10;
    }

    twos = 0;

    ctr = 0;

    while(ctr > count)
    {

        num += twos;    // { 2, 6, 12, 20, 30, 42, 56, 72, 90, 110, ... }
        // count of twos   { 1, 3,  6, 10, 15, 21, 28, 36, 45,  55, ... }

        twos += 2;      // { 2, 4,  6,  8, 10, 12, 14, 16, 18,  20 ... }

        ctr += 1;

    }

    _players[player_idx].spell_casting_skill = (num + 1);

    return random_skill;
    
}


// WZD o129p02
// drake178: OVL_ConvSpellAttack()
/*
; performs a conventional spell damage attack against
; all units stacked with the target, returning any
; items from slain heroes to their owner
; inherits all the BUGs of UNIT_ConvSpellATK
*/
/*

*/
void OVL_ConvSpellAttack__WIP(int16_t unit_idx, int16_t spell_idx)
{
    int16_t item_list[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
    int16_t item_count = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t player_idx = 0;
    int16_t itr_units = 0;  // _SI_

    wx = _UNITS[unit_idx].wx;
    wy = _UNITS[unit_idx].wy;
    wp = _UNITS[unit_idx].wp;

    item_count = 0;

    item_pool_in_process = ST_TRUE;

    m_item_wx = wx;
    m_item_wy = wy;
    m_item_wp = wp;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(
            (_UNITS[itr_units].wx == wx)
            &&
            (_UNITS[itr_units].wy == wy)
            &&
            (_UNITS[itr_units].wp == wp)
        )
        {

            player_idx = _UNITS[itr_units].owner_idx;

            UNIT_ConvSpellATK__WIP(itr_units, spell_idx, &item_count, &item_list[0], 0);

        }

    }

    Player_Process_Item_Pool(player_idx, item_count, &item_list[0]);

}


// WZD o129p03
// drake178: UNIT_ConvSpellATK()
/*
; performs a conventional spell attack against the
; selected overland unit, converting it into a battle
; unit and then back to do so
; BUG: scuttles the sandbox instead of using the
;  Active_Unit@ allocation
; BUG: allocates and clears a holy bonus array but then
;  fails to set and use it
; BUG: if killing a hero, their item slots are set to
;  0 instead of the empty marker -1
*/
/*

#XREF:
    OVL_ConvSpellAttack__WIP()
    j_UNIT_ConvSpellATK__WIP()
        CTY_ChaosRift()
        WIZ_MeteorStorm()

*/
void UNIT_ConvSpellATK__WIP(int16_t unit_idx, int16_t spell_idx, int16_t * item_count, int16_t item_list[], int16_t attack_override_flag)
{
    uint32_t enchantments = 0;
    int16_t damage_total = 0;
    int16_t damage_types[3] = { 0, 0, 0 };
    int16_t itr2 = 0;
    int16_t itr1 = 0;

    AI_Eval_After_Spell = ST_TRUE;


    for(itr1 = 0; itr1 < 3; itr1++)
    {

        damage_types[itr1] = 0;

    }

    // ; BUG: why not use Active_Unit@ instead of scuttling
    // DOMSDOS  battle_units = SA_MK_FP0(Allocate_First_Block(_screen_seg, 8));
    battle_units = (struct s_BATTLE_UNIT *)Allocate_First_Block(_screen_seg, 8);

    // ; BUG: it would take a lot more than this for a bonus
    // ; to actually apply too
    // DOMSDOS  CMB_HolyBonusArray = SA_MK_FP0(Allocate_Next_Block(_screen_seg, 3));
    CMB_HolyBonusArray = (int16_t *)Allocate_Next_Block(_screen_seg, 3);

    for(itr1 = 0; itr1 < _num_players; itr1++)
    {

        CMB_HolyBonusArray[itr1] = 0;

    }

    Load_Battle_Unit(unit_idx, &battle_units[0]);

    CMB_ConvSpellAttack__WIP(spell_idx, 0, &damage_types[0], attack_override_flag);

    battle_units[0].enchantments |= _UNITS[unit_idx].enchantments;

    if((battle_units[0].enchantments & UE_REGENERATION) != 0)
    {

        damage_types[0] = 0;

    }

    damage_total = 0;

    for(itr2 = 0; itr2 < 3; itr2++)
    {

        damage_total += damage_types[itr2];

    }

    damage_total += battle_units[0].front_figure_damage;

    battle_units[0].Cur_Figures -= (damage_total / battle_units[0].hits);

    battle_units[0].front_figure_damage = (damage_total % battle_units[0].hits);

    if(battle_units[0].Cur_Figures > 0)
    {

        if(battle_units[0].front_figure_damage < 0)
        {

            battle_units[0].front_figure_damage = 0;

        }

        _UNITS[unit_idx].Damage = (((battle_units[0].Max_Figures - battle_units[0].Cur_Figures) * battle_units[0].hits) + battle_units[0].front_figure_damage);

    }
    else  /* DEAD / DIED */
    {

        if(_UNITS[unit_idx].Hero_Slot >  -1)
        {

            for(itr2 = 0; itr2 < 3; itr2++)
            {

                if(_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr2] > -1)
                {

                    item_list[*item_count] = _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr2];

                    *item_count += 1;

                }

                // ; BUG: empty slots are -1, not 0
                _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr2] = 0;

            }

        }

        Kill_Unit(unit_idx, 0);

    }

}


// WZD o129p04
// sub_AD1F0()

// WZD o129p05
int16_t Cast_CallOfTheVoid(int16_t player_idx)
{

    return ST_FALSE;

}


// WZD o129p06
// drake178: CTY_CallTheVoid()
/*
            // ; processes the statistical effects of the Call the
            // ; Void spell cast on the target city
            // ; returns the number of buildings destroyed
            // ;
            // ; BUG: can corrupt the corner tiles outside the
            // ;  catchment area
            // ; inherits BUGs from UNIT_ConvSpellATK
*/
int16_t CTY_CallTheVoid__STUB(int16_t city_idx)
{

    return 0;

}

// WZD o129p07
// drake178: CTY_CounterSpell()
/*
; attempts to use any relevant asset in the city to
; counter the selected spell if possible
; returns 1 if the spell can be cast, or 0 if countered
*/
/*

XREF:
    Cast_CallOfTheVoid()
    Cast_Stasis()
    j_CTY_CounterSpell__STUB()
        Cast_DeathWish()
        IDK_Spell_GreatUnsummoning()
        Cast_BlackWind()
        Cast_RaiseVolcano()
        Cast_Corruption()
        WIZ_MeteorStorm()
        Cast_Spell_Overland__WIP()

"Having nightshade as a resource in your city (see Terrain Specials) and a building that can make use of this resource (religious institutions, see Buildings), significantly increases a city’s ability to resist enemy city enchantments."

TERRAIN SPECIALS (MINERALS, BIOTA, NODES AND STRUCTURES)
TERRAIN SPECIAL
    Nightshade
SPECIAL EFFECTS AND RESTRICTIONS
    Cities with shrines, sage’s guilds or animist’s guilds (or their replacement buildings) use nightshade to automatically cast dispel magic against all city enchantments and global spells directed against the city every turn; each nightshade has a separate chance to dispel negative enchantments every turn

*/
int16_t Apply_Automatic_Spell_Counters(int16_t spell_idx, int16_t city_idx, int16_t player_idx, int16_t show_message)
{
    int16_t chance = 0;
    int16_t success = 0;

    success = ST_TRUE;

    if(
        (_CITIES[city_idx].enchantments[CONSECRATION] > 0)
        &&
        (
            (spell_data_table[spell_idx].magic_realm == sbr_Death)
            ||
            (spell_data_table[spell_idx].magic_realm == sbr_Chaos)
        )
    )
    {
        if(show_message != ST_FALSE)
        {
            Fizzle_Notification(player_idx, _CITIES[city_idx].owner_idx, spell_idx, str_Consecration__ovr129);
        }
        success = ST_FALSE;
    }

    if(
        (_CITIES[city_idx].enchantments[DEATH_WARD] > 0)
        &&
        (spell_data_table[spell_idx].magic_realm == sbr_Death)
    )
    {
        if(show_message != ST_FALSE)
        {
            Fizzle_Notification(player_idx, _CITIES[city_idx].owner_idx, spell_idx, str_DeathWard__ovr129);
        }
        success = ST_FALSE;
    }

    if(
        (_CITIES[city_idx].enchantments[LIFE_WARD] > 0)
        &&
        (spell_data_table[spell_idx].magic_realm == sbr_Life)
    )
    {
        if(show_message != ST_FALSE)
        {
            Fizzle_Notification(player_idx, _CITIES[city_idx].owner_idx, spell_idx, str_LifeWard__ovr129);
        }
        success = ST_FALSE;
    }

    if(
        (_CITIES[city_idx].enchantments[NATURE_WARD] > 0)
        &&
        (spell_data_table[spell_idx].magic_realm == sbr_Nature)
    )
    {
        if(show_message != ST_FALSE)
        {
            Fizzle_Notification(player_idx, _CITIES[city_idx].owner_idx, spell_idx, str_NatureWard__ovr129);
        }
        success = ST_FALSE;
    }

    if(
        (_CITIES[city_idx].enchantments[CHAOS_WARD] > 0)
        &&
        (spell_data_table[spell_idx].magic_realm == sbr_Chaos)
    )
    {
        if(show_message != ST_FALSE)
        {
            Fizzle_Notification(player_idx, _CITIES[city_idx].owner_idx, spell_idx, str_ChaosWard__ovr129);
        }
        success = ST_FALSE;
    }

    if(
        (_CITIES[city_idx].enchantments[SORCERY_WARD] > 0)
        &&
        (spell_data_table[spell_idx].magic_realm == sbr_Sorcery)
    )
    {
        if(show_message != ST_FALSE)
        {
            Fizzle_Notification(player_idx, _CITIES[city_idx].owner_idx, spell_idx, str_SorceryWard__ovr129);
        }
        success = ST_FALSE;
    }

    if(_CITIES[city_idx].enchantments[NIGHTSHADE] > 0)
    {
        chance = (Calculate_Dispel_Difficulty(_players[player_idx].casting_cost_original, player_idx, spell_data_table[spell_idx].magic_realm) + (_CITIES[city_idx].enchantments[NIGHTSHADE] * 100));
        chance = ((_CITIES[city_idx].enchantments[NIGHTSHADE] * 25000) / chance);
        if(Random(250) <= chance)
        {
            if(show_message != ST_FALSE)
            {
                Fizzle_Notification(player_idx, _CITIES[city_idx].owner_idx, spell_idx, str_Nightshade__ovr129);
            }
            success = ST_FALSE;
        }
    }

    return success;
}

// WZD o129p08
/*
; applies the Chaos Channels effect to the target unit,
; granting it a random one of the CC_Flight, CC_Breath,
; or CC_Defense mutations, although the first can't be
; applied to flying or sailing units, while the second
; won't work on units with a ranged attack other than a
; Thrown attack of strength 3 or lower
*/
/*

*/
void UNIT_ChaosChannel__STUB(int16_t unit_idx)
{



}


// WZD o129p09
int16_t Cast_ChaosChannels(int16_t player_idx)
{

    return ST_FALSE;

}

// WZD o129p10
int16_t Cast_WarpNode(int16_t player_idx)
{

    return ST_FALSE;

}


// WZD o129p11
int16_t Cast_Stasis(int16_t player_idx)
{

    return ST_FALSE;

}


// WZD o129p12
int16_t Cast_NaturesCures(int16_t player_idx)
{

    return ST_FALSE;

}

// WZD o129p13
int16_t Cast_Move_Fortress(int16_t player_idx)
{
    int16_t status = 0;
    int16_t return_value = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t city_idx = 0;

    // DNE  Allocate_Reduced_Map();

    // DNE  Mark_Block(_screen_seg);

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        return_value = Pick_Target_For_City_Enchantment__WIP(stt_Friendly_City, &city_idx, spl_Move_Fortress, player_idx);

    }
    else
    {

        status = ST_FALSE;

        return_value = ST_TRUE;

        while((status == ST_FALSE) && (return_value == ST_TRUE))
        {

            return_value = Spell_Casting_Screen__WIP(stt_Friendly_City, &city_idx, &wy, &wp, &target_wx, &target_wy, aMoveFortress);  // "Wall of Stone"

            if(return_value == ST_TRUE)
            {

                if(
                    (_CITIES[city_idx].wx == _FORTRESSES[HUMAN_PLAYER_IDX].wx)
                    &&
                    (_FORTRESSES[HUMAN_PLAYER_IDX].wy == _CITIES[city_idx].wy)
                    &&
                    (_FORTRESSES[HUMAN_PLAYER_IDX].wp == _CITIES[city_idx].wp)
                )
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr129, 0, (SAMB_ptr)&GUI_NearMsgString[0], 3, 1, 150);

                    Warn0(GUI_NearMsgString);

                }
                else
                {

                    status = ST_TRUE;

                }

            }

        }

    }

    if(return_value == ST_TRUE)
    {

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (magic_set.enemy_spells == ST_TRUE)
                &&
                (SQUARE_EXPLORED(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) != ST_FALSE)
                &&
                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > ST_FALSE)
            )
        )
        {

            AI_Eval_After_Spell = ST_TRUE;

            Cast_Spell_City_Enchantment_Animation_1__WIP(city_idx, spl_Move_Fortress, player_idx);

        }

                if(
                    (_FORTRESSES[HUMAN_PLAYER_IDX].wx == _players[HUMAN_PLAYER_IDX].summon_wx)
                    &&
                    (_FORTRESSES[HUMAN_PLAYER_IDX].wy == _players[HUMAN_PLAYER_IDX].summon_wy)
                    &&
                    (_FORTRESSES[HUMAN_PLAYER_IDX].wp == _players[HUMAN_PLAYER_IDX].summon_wp)
                )
                {

                    _players[HUMAN_PLAYER_IDX].summon_wx = _CITIES[city_idx].wx;

                    _players[HUMAN_PLAYER_IDX].summon_wy = _CITIES[city_idx].wy;

                    _players[HUMAN_PLAYER_IDX].summon_wp = _CITIES[city_idx].wp;

                }

                    _FORTRESSES[HUMAN_PLAYER_IDX].wx = _CITIES[city_idx].wx;

                    _FORTRESSES[HUMAN_PLAYER_IDX].wy = _CITIES[city_idx].wy;

                    _FORTRESSES[HUMAN_PLAYER_IDX].wp = _CITIES[city_idx].wp;

                    _players[HUMAN_PLAYER_IDX].capital_race = _CITIES[city_idx].race;

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (magic_set.enemy_spells == ST_TRUE)
                &&
                (SQUARE_EXPLORED(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) != ST_FALSE)
                &&
                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > ST_FALSE)
            )
        )
        {

            Cast_Spell_City_Enchantment_Animation_2__WIP(city_idx, spl_Move_Fortress, player_idx);

            Release_Block(_screen_seg);

        }

    }

    return return_value;

}


// WZD o129p14
int16_t Cast_Earthquake(int16_t player_idx)
{

    return ST_FALSE;

}

// WZD o129p15
// drake178: CTY_Earthquake()
/*
; processes the statistical effects of the Earthquake
; spell cast on the target city
; returns the number of buildings destroyed
*/
/*

*/
int16_t CTY_Earthquake__STUB(int16_t city_idx, int16_t * list_count, int16_t list_array[])
{

    return 0;

}


// WZD o129p16
// sub_AEDB1()

