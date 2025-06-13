/*
    WIZARDS.EXE
        ovr129
*/

#include "MOM_DEF.H"
#include "MOX/Allocate.H"
#include "MOX/MOM_Data.H"
#include "MOX/MOX_DAT.H"

#include "NEXTTURN.H"
#include "OverSpel.H"
#include "SBookScr.H"
#include "Spells129.H"

int16_t * CMB_HolyBonusArray;



// WZD dseg:6686                                                 BEGIN:  ovr129 - Initialized Data
// WZD dseg:6686
// WZD dseg:6686 43 61 6C 6C 20 74 68 65 20 56 6F 69 64 00       aCallTheVoid db 'Call the Void',0       ; DATA XREF: Call_Forth_The_Force_Of_Nature+15Co

// WZD dseg:6694
char str_Consecration__ovr129[] = "Consecration";
// dseg:66A1
char str_DeathWard__ovr129[] = "Death Ward";
// dseg:66AC
char str_LifeWard__ovr129[] = "Life Ward";
// dseg:66B6
char str_NatureWard__ovr129[] = "Nature Ward";
// dseg:66C2
char str_ChaosWard__ovr129[] = "Chaos Ward";
// dseg:66CD
char str_SorceryWard__ovr129[] = "Sorcery Ward";
// dseg:66DA
char str_Nightshade__ovr129[] = "Nightshade";

// dseg:66E5 6D 65 73 73 61 67 65 00                         message_lbx_file__ovr129 db 'message',0 ; should use dseg:5aea
// dseg:66ED 43 68 61 6F 73 20 43 68 61 6E 6E 65 6C 73 00    aChaosChannels db 'Chaos Channels',0
// dseg:66FC 57 61 72 70 20 4E 6F 64 65 00                   aWarpNode db 'Warp Node',0
// dseg:6706 53 74 61 73 69 73 00                            cnst_Stasis_2 db 'Stasis',0             ; DATA XREF: Cast_Stasis:loc_AE038o
// dseg:670D 4E 61 74 75 72 65 27 73 20 43 75 72 65 73 00    aNatureSCures db 'Nature',27h,'s Cures',0
// dseg:671C 4D 6F 76 65 20 46 6F 72 74 72 65 73 73 00       aMoveFortress db 'Move Fortress',0      ; DATA XREF: Cast_Move_Fortress:loc_AE53Do
// dseg:672A 45 61 72 74 68 71 75 61 6B 65 00                aEarthquake db 'Earthquake',0           ; DATA XREF: Cast_Earthquake+Fo
// dseg:6735 6D 75 73 69 63 00                               music_lbx_file__ovr129 db 'music',0     ; DATA XREF: Call_Forth_The_Force_Of_Nature+1Co
// dseg:673B 4E 61 74 75 72 65 27 73 20 57 72 61 74 68 20 73+aNatureSWrathSt db 'Nature',27h,'s Wrath strikes ',0
// dseg:6753 79 6F 75 00                                     aYou db 'you',0                         ; DATA XREF: Call_Forth_The_Force_Of_Nature+51o
// dseg:6757 53 70 65 63 66 78 00                            specfx_lbx_file__ovr129 db 'Specfx',0   ; DATA XREF: Call_Forth_The_Force_Of_Nature+97o
// dseg:675E 42 41 43 4B 47 52 4E 44 00                      backgrnd__ovr129 db 'BACKGRND',0        ; DATA XREF: Call_Forth_The_Force_Of_Nature+AEo

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

    damage_total += battle_units[0].TopFig_Dmg;

    battle_units[0].Cur_Figures -= (damage_total / battle_units[0].hits);

    battle_units[0].TopFig_Dmg = (damage_total % battle_units[0].hits);

    if(battle_units[0].Cur_Figures > 0)
    {

        if(battle_units[0].TopFig_Dmg < 0)
        {

            battle_units[0].TopFig_Dmg = 0;

        }

        _UNITS[unit_idx].Damage = (((battle_units[0].Max_Figures - battle_units[0].Cur_Figures) * battle_units[0].hits) + battle_units[0].TopFig_Dmg);

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
// sub_AD46B()

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
// UNIT_ChaosChannel()

// WZD o129p09
// sub_ADC3E()

// WZD o129p10
// sub_ADDA2()

// WZD o129p11
// sub_AE017()

// WZD o129p12
// sub_AE352()

// WZD o129p13
// sub_AE51E()

// WZD o129p14
// sub_AE8B0()

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

