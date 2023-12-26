/*
    Unit View

    WIZARDS.EXE
        ovr072
        ovr078
        ovr089
*/
/*

USW_HeroAbilities is XREF'd by ovr102 IDK_Hero_Populate_USW_List_Structure

*/
/*
    HELP

in order, except 0x273  627
USW_HeroAbilities
0x0203  515
...
0x020E  526

USW_Abilities
0x018C  396
...
0x019A  410


USW_Attributes_1

*/

#include "MoM.H"
#include "UnitView.H"
#include <intrin.h>


// drake178: USW_HeroAbl
// sizeof:  08h  8d
struct USW_HeroAbl
{
    /* 00 */  int32_t Flags;  // HAbl_Flags
    /* 04 */  uint8_t special_icon_idx;
    /* 05 */  // 2-byte alignment padding
    /* 06 */  int16_t HLP_Index;  // enum HELP_TXT
    /* 08 */
};


// drake178: USW_Ability
// sizeof:  08h  8d
struct USW_Ability
{
    /* 00 */  char ** Name;
    /* 02 */  int16_t Abl_Flag;  // enum ABL_FLAGS
    /* 04 */  uint8_t special_icon_idx;
    /* 05 */  // 2-byte alignment padding
    /* 06 */  int16_t HLP_Index;  // enum HELP_TXT
    /* 08 */
};


// drake178: USW_Attribute_1
// sizeof:  08h  8d
struct USW_Attribute_1
{
    /* 00 */ char ** Name;
    /* 02 */ int16_t Attrib1_Flag;  // enum ATTRIB_1
    /* 04 */ uint8_t special_icon_idx;
    /* 05 */  // 2-byte alignment padding
    /* 06 */ int16_t HLP_Index;  // enum HELP_TXT
    /* 08 */
};


// drake178: USW_Attribute_2
// sizeof:  08h  8d
struct USW_Attribute_2
{
    /* 00 */ char ** Name;
    /* 02 */ int16_t Attrib2_Flag;  // enum ATTRIB_2
    /* 04 */ uint8_t special_icon_idx;
    /* 05 */  // 2-byte alignment padding
    /* 06 */ int16_t HLP_Index;  // enum HELP_TXT
    /* 08 */
};


// drake178: USW_AttackFlag
// sizeof:  08h  8d
struct USW_AttackFlag
{
    /* 00 */ char ** Name;
    /* 02 */ int16_t Attack_Flag;  // enum ATK_FLAGS
    /* 04 */ uint8_t special_icon_idx;
    /* 05 */  // 2-byte alignment padding
    /* 06 */ int16_t HLP_Index;  // enum HELP_TXT
    /* 08 */
};


// drake178: USW_Mutation
// sizeof:  06h  6d
struct USW_Mutation
{
    /* 00 */ char ** Name;
    /* 02 */ int8_t Mutation_Flag;  // enum MUT_FLAGS
    /* 03 */ uint8_t special_icon_idx;
    /* 04 */ int16_t HLP_Index;  // enum HELP_TXT
    /* 06 */
};


// drake178: USW_SR_Attack
// sizeof:  06h  6d
struct USW_SR_Attack
{
    /* 00 */ char ** Name;
    /* 02 */ int8_t Ranged_Type;  // enum Ranged_Type
    /* 03 */ uint8_t special_icon_idx;
    /* 04 */ int16_t HLP_Index;  // enum HELP_TXT
    /* 06 */
};



// WZD dseg:3A50
// drake178: struct USW_HeroAbl USW_HeroAbilities
// extern struct USW_HeroAbl USW_HeroAbilities[23];

// WZD dseg:3B08
// drake178: struct USW_Ability USW_Abilities
// extern struct USW_Ability USW_Abilities[15];





// WZD dseg:3A50                                                 ¿ BEGIN:  ovr078  UV / USW ?

// WZD dseg:3A50
// drake178: struct USW_HeroAbl USW_HeroAbilities
struct USW_HeroAbl USW_HeroAbilities[23] =
{
    { 0x00000001,  0x002B,  0x0203 },
    { 0x00000002,  0x002B,  0x0203 },
    { 0x00000008,  0x002D,  0x0204 },
    { 0x00000010,  0x002D,  0x0204 },
    { 0x00000040,  0x002F,  0x0205 },
    { 0x00000080,  0x002F,  0x0205 },
    { 0x00000200,  0x002E,  0x0206 },
    { 0x00000400,  0x002E,  0x0206 },
    { 0x00001000,  0x0032,  0x0207 },
    { 0x00002000,  0x0032,  0x0207 },
    { 0x00008000,  0x0034,  0x0208 },
    { 0x00010000,  0x0034,  0x0208 },
    { 0x00040000,  0x0036,  0x0209 },
    { 0x00080000,  0x0036,  0x0209 },
    { 0x00200000,  0x003D,  0x020A },
    { 0x00400000,  0x003D,  0x020A },
    { 0x01000000,  0x0039,  0x020B },
    { 0x02000000,  0x0039,  0x020B },
    { 0x80000000,  0x008F,  0x0273 },  /* out of order */
    { 0x04000000,  0x008F,  0x0273 },  /* out of order */
    { 0x08000000,  0x003A,  0x020C },
    { 0x10000000,  0x003B,  0x020D },
    { 0x20000000,  0x003C,  0x020E }
};

// USW_HeroAbl <<Ab_Leadership, 0>, 2Bh, 0, HLP_LEADERSHIP>
// USW_HeroAbl <<Ab_Leadership2, 0>, 2Bh, 0, HLP_LEADERSHIP>
// USW_HeroAbl <<Ab_Legendary, 0>, 2Dh, 0, HLP_LEGENDARY>
// USW_HeroAbl <<Ab_Legendary2, 0>, 2Dh, 0, HLP_LEGENDARY>
// USW_HeroAbl <<Ab_Blademaster, 0>, 2Fh, 0, HLP_BLADEMASTER>
// USW_HeroAbl <<Ab_Blademaster2, 0>, 2Fh, 0, HLP_BLADEMASTER>
// USW_HeroAbl <<Ab_Armsmaster, 0>, 2Eh, 0, HLP_ARMSMASTER>
// USW_HeroAbl <<Ab_Armsmaster2, 0>, 2Eh, 0, HLP_ARMSMASTER>
// USW_HeroAbl <<Ab_Constitution, 0>, 32h, 0, HLP_CONSTITUTION>
// USW_HeroAbl <<Ab_Constitution2, 0>, 32h, 0, HLP_CONSTITUTION>
// USW_HeroAbl <<Ab_Might, 0>, 34h, 0, HLP_MIGHT>
// USW_HeroAbl <<0, Ab_Might2>, 34h, 0, HLP_MIGHT>
// USW_HeroAbl <<0, Ab_Arcane_Power>, 36h, 0, HLP_ARCANE_POWER>
// USW_HeroAbl <<0, Ab_Arcane_Power2>, 36h, 0, HLP_ARCANE_POWER>
// USW_HeroAbl <<0, Ab_Sage>, 3Dh, 0, HLP_SAGE>
// USW_HeroAbl <<0, Ab_Sage2>, 3Dh, 0, HLP_SAGE>
// USW_HeroAbl <<0, Ab_Prayermaster>, 39h, 0, HLP_PRAYERMASTER>
// USW_HeroAbl <<0, Ab_Prayermaster2>, 39h, 0, HLP_PRAYERMASTER>
// USW_HeroAbl <<0, Ab_Agility>, 8Fh, 0, HLP_AGILE>
// USW_HeroAbl <<0, Ab_Agility2>, 8Fh, 0, HLP_AGILE>
// USW_HeroAbl <<0, Ab_Lucky>, 3Ah, 0, HLP_LUCKY_Hero>
// USW_HeroAbl <<0, Ab_Charmed>, 3Bh, 0, HLP_CHARMED>


// WZD dseg:4064
char cnst_LargeShield[] = "Large Shield";
char cnst_PlaneShift[] = "Plane Shift";
char cnst_WallCrusher[] = "Wall Crusher";
char cnst_Healer_2[] = "Healer";
char cnst_CreateOutpost[] = "Create Outpost";
char cnst_Invisibility[] = "Invisibility";
char cnst_CreateUndead[] = "Create Undead";
char cnst_LongRange[] = "Long Range";
char cnst_LandCorruption[] = "Land Corruption";
char cnst_MeldWithNode[] = "Meld With Node";
char cnst_NonCorporeal[] = "Non Corporeal";
char cnst_WindWalking[] = "Wind Walking";
char cnst_Regeneration[] = "Regeneration";
char cnst_Purify[] = "Purify";
char cnst_Negate1stStrike[] = "Negate First Strike";


char * unit_ability_names[15] =
{
    cnst_LargeShield,
    cnst_PlaneShift,
    cnst_WallCrusher,
    cnst_Healer_2,
    cnst_CreateOutpost,
    cnst_Invisibility,
    cnst_CreateUndead,
    cnst_LongRange,
    cnst_LandCorruption,
    cnst_MeldWithNode,
    cnst_NonCorporeal,
    cnst_WindWalking,
    cnst_Regeneration,
    cnst_Purify,
    cnst_Negate1stStrike
};


// WZD dseg:3B08
// drake178: struct USW_Ability USW_Abilities
struct USW_Ability USW_Abilities[15] =
{
/*   name                      flags    pict     help  */
    {&unit_ability_names[ 0],  0x0002,  0x000E,  0x018C},
    {&unit_ability_names[ 1],  0x0004,  0x0004,  0x018D},
    {&unit_ability_names[ 2],  0x0008,  0x000F,  0x018E},
    {&unit_ability_names[ 3],  0x0010,  0x0010,  0x018F},
    {&unit_ability_names[ 4],  0x0020,  0x0011,  0x0190},
    {&unit_ability_names[ 5],  0x0040,  0x0012,  0x0191},
    {&unit_ability_names[ 6],  0x0080,  0x0013,  0x0192},
    {&unit_ability_names[ 7],  0x0100,  0x0081,  0x0193},
    {&unit_ability_names[ 8],  0x0200,  0x0015,  0x0194},
    {&unit_ability_names[ 9],  0x0400,  0x0028,  0x0195},
    {&unit_ability_names[10],  0x0800,  0x0016,  0x0196},
    {&unit_ability_names[11],  0x1000,  0x0017,  0x0197},
    {&unit_ability_names[12],  0x2000,  0x0018,  0x0198},
    {&unit_ability_names[13],  0x4000,  0x0019,  0x0199},
    {&unit_ability_names[14],  0x8000,  0x0030,  0x019A}
};


char cnst_Lucky[] = "Lucky";
char cnst_PoisonImmunity[] = "Poison Immunity";
char cnst_FireImmunity[] = "Fire Immunity";
char cnst_StoningImmunity[] = "Stoning Immunity";
char cnst_WeaponImmunity[] = "Weapon Immunity";
char cnst_MissileImmunity[] = "Missiles Immunity";
char cnst_IllusionsImm[] = "Illusions Immunity";
char cnst_ColdImmunity[] = "Cold Immunity";
char cnst_MagicImmunity[] = "Magic Immunity";
char cnst_DeathImmunity[] = "Death Immunity";

char * unit_attribute_1_names[] =
{
    cnst_Lucky,
    cnst_PoisonImmunity,
    cnst_FireImmunity,
    cnst_StoningImmunity,
    cnst_WeaponImmunity,
    cnst_MissileImmunity,
    cnst_IllusionsImm,
    cnst_ColdImmunity,
    cnst_MagicImmunity,
    cnst_DeathImmunity
};

// WZD dseg:3B80
// drake178: struct USW_Attribute_1 USW_Attributes_1
struct USW_Attribute_1 USW_Attributes_1[10] =
{
    { &unit_attribute_1_names[0],  0x0400,  0x007F,  0x019B },
    { &unit_attribute_1_names[1],  0x0080,  0x0005,  0x019C },
    { &unit_attribute_1_names[2],  0x0001,  0x0006,  0x019D },
    { &unit_attribute_1_names[3],  0x0002,  0x0007,  0x019E },
    { &unit_attribute_1_names[4],  0x0100,  0x0008,  0x019F },
    { &unit_attribute_1_names[5],  0x0004,  0x0009,  0x01A0 },
    { &unit_attribute_1_names[6],  0x0008,  0x000A,  0x01A1 },
    { &unit_attribute_1_names[7],  0x0010,  0x000B,  0x01A2 },
    { &unit_attribute_1_names[8],  0x0020,  0x000C,  0x01A3 },
    { &unit_attribute_1_names[9],  0x0040,  0x0031,  0x01A4 }
};

// USW_Attribute_1 <offset cnst_Lucky, Abl_Lucky, 127, 0, HLP_LUCKY>
// USW_Attribute_1 <offset cnst_PoisonImmunity, Imm_Poison, 5, 0, HLP_POISON_IMMUNITY>
// USW_Attribute_1 <offset cnst_FireImmunity, Imm_Fire, 6, 0, HLP_FIRE_IMMUNITY>
// USW_Attribute_1 <offset cnst_StoningImmunity, Imm_Stoning, 7, 0, HLP_STONING_IMMUNITY>
// USW_Attribute_1 <offset cnst_WeaponImmunity, Imm_Weapon, 8, 0, HLP_WEAPON_IMMUNITY>
// USW_Attribute_1 <offset cnst_MissileImmunity, Imm_Missiles, 9, 0, HLP_MISSILES_IMMUNITY>
// USW_Attribute_1 <offset cnst_IllusionsImm, Imm_Illusion, 10, 0, HLP_ILLUSIONS_IMMUNITY>
// USW_Attribute_1 <offset cnst_ColdImmunity, Imm_Cold, 12, 0, HLP_COLD_IMMUNITY>
// USW_Attribute_1 <offset cnst_MagicImmunity, Imm_Magic, 12, 0, HLP_MAGIC_IMMUNITY>
// USW_Attribute_1 <offset cnst_DeathImmunity, Imm_Death, 49, 0, HLP_DEATH_IMMUNITY>


char cnst_HealingSpell[] = "Healing Spell";
char cnst_Spell_2[] = "Spell";
char cnst_FireBallSpell[] = "Fire Ball Spell";
char cnst_DoomboltSpell[] = "Doombolt Spell";
char cnst_Immolation[] = "Immolation";
char cnst_CauseFearSpell[] = "Cause Fear Spell";
char cnst_WebSpell[] = "Web Spell";
char cnst_Resistance2All[] = "Resistance To All";
char cnst_HolyBonus[] = "Holy Bonus";

char * unit_attribute_2_names[9] =
{
    cnst_HealingSpell,
    cnst_Spell_2,
    cnst_FireBallSpell,
    cnst_DoomboltSpell,
    cnst_Immolation,
    cnst_CauseFearSpell,
    cnst_WebSpell,
    cnst_Resistance2All,
    cnst_HolyBonus
};

// WZD dseg:3BD0
// drake178: struct USW_Attribute_2 USW_Attributes_2
struct USW_Attribute_2 USW_Attributes_2[] =
{
    { &unit_attribute_2_names[0],  0x0001,  0x0026,  0x01A5 },
    { &unit_attribute_2_names[1],  0x0002,  0x0027,  0x01A6 },
    { &unit_attribute_2_names[2],  0x0004,  0x0029,  0x01A7 },
    { &unit_attribute_2_names[3],  0x0008,  0x0020,  0x01A8 },
    { &unit_attribute_2_names[4],  0x0020,  0x0084,  0x01A9 },
    { &unit_attribute_2_names[5],  0x0010,  0x0083,  0x01AA },
    { &unit_attribute_2_names[6],  0x0040,  0x0021,  0x01AB },
    { &unit_attribute_2_names[7],  0x0080,  0x0022,  0x01AC },
};

// USW_Attribute_2 <offset cnst_HealingSpell, Spl_Healing, 26h, HLP_HEALING_SPELL>
// USW_Attribute_2 <offset cnst_FireBallSpell, Spl_Fireball, 27h, HLP_FIRE_BALL_SPELL>
// USW_Attribute_2 <offset cnst_DoomboltSpell, Spl_DoomBolt, 29h, HLP_DOOMBOLT_SPELL>
// USW_Attribute_2 <offset cnst_Immolation, Ab_Immolation, 20h, HLP_IMMOLATION>
// USW_Attribute_2 <offset cnst_CauseFearSpell, Ab_CauseFear, 84h, HLP_CAUSE_FEAR_SPELL>
// USW_Attribute_2 <offset cnst_WebSpell, Spl_Web, 83h, HLP_WEB_SPELL>
// USW_Attribute_2 <offset cnst_Resistance2All, Ab_ResistAll, 21h, HLP_RESISTANCE_TO_ALL>
// USW_Attribute_2 <offset cnst_HolyBonus, Ab_HolyBonus, 22h, HLP_HOLY_BONUS>


char cnst_ArmorPiercing[] = "Armor Piercing";
char cnst_Poison[] = "Poison";
char cnst_LifeSteal[] = "Life Steal";
char cnst_AutomaticDmg[] = "Automatic Damage";
char cnst_Destruction[] = "Destruction";
char cnst_Illusion[] = "Illusion";
char cnst_StoningTouch[] = "Stoning Touch";
char cnst_DeathTouch[] = "Death Touch";
char cnst_PowerDrain[] = "Power Drain";
char cnst_DispelEvil[] = "Dispel Evil";

char * unit_attack_flag_names[] =
{
    cnst_ArmorPiercing,
    cnst_Poison,
    cnst_LifeSteal,
    cnst_AutomaticDmg,
    cnst_Destruction,
    cnst_Illusion,
    cnst_StoningTouch,
    cnst_DeathTouch,
    cnst_PowerDrain,
    cnst_DispelEvil,
};

// WZD dseg:3C10
// drake178: struct USW_AttackFlag USW_ATK_Flags
struct USW_AttackFlag USW_ATK_Flags[11] = 
{
    { &unit_attack_flag_names[ 0],  0x0001,  0x001C,  0x01AD },
    { &unit_attack_flag_names[ 1],  0x0002,  0x001D,  0x01AE },
    { &unit_attack_flag_names[ 2],  0x0004,  0x001E,  0x01AF },
    { &unit_attack_flag_names[ 3],  0x0008,  0x001F,  0x01B0 },
    { &unit_attack_flag_names[ 4],  0x0010,  0x008C,  0x01B1 },
    { &unit_attack_flag_names[ 5],  0x0020,  0x0074,  0x01B2 },
    { &unit_attack_flag_names[ 6],  0x0040,  0x0023,  0x01B3 },
    { &unit_attack_flag_names[ 7],  0x0080,  0x001B,  0x01B4 },
    { &unit_attack_flag_names[ 8],  0x0200,  0x008D,  0x01B5 },
    { &unit_attack_flag_names[ 9],  0x0400,  0x008E,  0x01B6 },
    { &unit_attack_flag_names[10],  0x0800,  0x0085,  0x01B7 }
};

// USW_AttackFlag <offset cnst_ArmorPiercing, Att_ArmorPrc, 1Ch, 0, HLP_ARMOR_PIERCING>
// USW_AttackFlag <offset cnst_FirstStrike, Att_1stStrike, 1Dh, 0, HLP_FIRST_STRIKE>
// USW_AttackFlag <offset cnst_Poison, Att_Poison, 1Eh, 0, HLP_POISON>
// USW_AttackFlag <offset cnst_LifeSteal, Att_LifeSteal, 1Fh, 0, HLP_LIFE_STEAL>
// USW_AttackFlag <offset cnst_AutomaticDmg, Att_DoomDmg, 8Ch, 0, HLP_CHAOS>
// USW_AttackFlag <offset cnst_Destruction, Att_Destruct, 74h, 0, HLP_DESTRUCTION>
// USW_AttackFlag <offset cnst_Illusion, Att_Illusion, 23h, 0, HLP_ILLUSION>
// USW_AttackFlag <offset cnst_StoningTouch, Att_StnTouch, 1Bh, 0, HLP_STONING_TOUCH>
// USW_AttackFlag <offset cnst_DeathTouch, Att_DthTouch, 8Dh, 0, HLP_CAUSE_DEATH>
// USW_AttackFlag <offset cnst_PowerDrain, Att_PwrDrain, 8Eh, 0, HLP_POWER_DRAIN>
// USW_AttackFlag <offset cnst_DispelEvil, Att_DsplEvil, 85h, 0, HLP_DISPEL_EVIL>


// WZD dseg:42A7 43 68 61 6F 73 20 43 68 61 6E 6E 65 6C 00       cnst_ChaosChannel db 'Chaos Channel',0  ; DATA XREF: dseg:USW_ChaosChannelso

char cnst_ChaosChannel[] = "Chaos Channel";

char * unit_chaos_channels_names[] =
{
    cnst_ChaosChannel
};

// WZD dseg:3C68
// drake178: struct USW_Mutation USW_ChaosChannels
struct USW_Mutation USW_ChaosChannels[] =
{
    { &unit_chaos_channels_names[0],  0x04,  0x3E,  0x01B8 },
    { &unit_chaos_channels_names[0],  0x08,  0x3F,  0x01B9 },
    { &unit_chaos_channels_names[0],  0x10,  0x40,  0x01BA }
};

// USW_Mutation <offset cnst_ChaosChannel, CC_Defense, 3Eh, HLP_CHAOS_CHANNEL_Def>
// USW_Mutation <offset cnst_ChaosChannel, CC_Flight, 3Fh, HLP_CHAOS_CHANNEL_Fly>
// USW_Mutation <offset cnst_ChaosChannel, CC_Breath, 40h, HLP_CHAOS_CHANNEL_Brth>


// WZD dseg:42B5
char cnst_Thrown[] = "Thrown";
char cnst_FireBreath[] = "Fire Breath";
char cnst_Lightning[] = "Lightning";
char cnst_StoningGaze[] = "Stoning Gaze";
char cnst_DoomGaze[] = "Doom Gaze";
char cnst_DeathGaze[] = "Death Gaze";

char * unit_sr_attack_names[] =
{
    cnst_Thrown,
    cnst_FireBreath,
    cnst_Lightning,
    cnst_StoningGaze,
    cnst_DoomGaze,
    cnst_DeathGaze
};

// WZD dseg:3C7A
// drake178: struct USW_SR_Attack USW_SR_Attacks
struct USW_SR_Attack USW_SR_Attacks[] =
{
    { &unit_sr_attack_names[0], 0x64,  0x82,  0x01BB },
    { &unit_sr_attack_names[1], 0x65,  0x8A,  0x01BC },
    { &unit_sr_attack_names[2], 0x66,  0x89,  0x01BD },
    { &unit_sr_attack_names[3], 0x67,  0x1A,  0x01BE },
    { &unit_sr_attack_names[4], 0x68,  0x86,  0x01BF },
    { &unit_sr_attack_names[5], 0x69,  0x87,  0x01C0 }
};

// USW_SR_Attack <offset cnst_Thrown, SR_Thrown, 82h, HLP_THROWN>
// USW_SR_Attack <offset cnst_FireBreath, SR_FireBreath, 8Ah, HLP_FIRE_BREATH>
// USW_SR_Attack <offset cnst_Lightning, SR_Lightning, 89h, HLP_LIGHTNING_BREATH>
// USW_SR_Attack <offset cnst_StoningGaze, SR_StoneGaze, 1Ah, HLP_STONING_GAZE>
// USW_SR_Attack <offset cnst_DoomGaze, SR_MultiGaze, 86h, HLP_DOOM_GAZE>
// USW_SR_Attack <offset cnst_DeathGaze, SR_DeathGaze, 87h, HLP_DEATH_GAZE>










/*
    WIZARDS.EXE  ovr072
*/



/*
    WIZARDS.EXE  ovr078
*/

// WZD o78p01
// USW_Build_Effect_List

// WZD o78p02
/*
    only called from 'Production Screen'
    therefore, no Unit Enchantments, no Hero Items
        and, only Norm specials, no Fant specials, no Hero specials
    Construction
    Scouting
    Teleporting, Forester, Mountaineer
    USW_Abilities
    USW_Attributes_1
    USW_Attributes_2
    USW_ATK_Flags
    USW_SR_Attacks
*/
void Prod_Build_Specials_List(int16_t unit_type, struct s_UV_List * specials_list, int16_t * specials_count)
{
    char scout_rank[5][5];
    int16_t value;
    char temp_buffer[10];
    int16_t itr;
    int16_t list_index;  // SI

    strcpy(scout_rank[0], "");   // if > 1, so never used
    strcpy(scout_rank[1], "I");  // if > 1, so never used
    strcpy(scout_rank[2], "II");
    strcpy(scout_rank[3], "III");
    strcpy(scout_rank[4], "IV");

    list_index = 0;

    if(_unit_type_table[unit_type].Construction > 0)
    {
        strcpy(specials_list->names[list_index], "Construction");
        specials_list->flags[list_index] = -1;
        specials_list->picts[list_index] = (SAMB_INT)special_seg[36];
        specials_list->helps[list_index] = HLP_CONSTRUCTION;
        list_index++;
    }

    if(_unit_type_table[unit_type].Sight > 1)
    {
        strcpy(specials_list->names[list_index], "Scouting");
        value = _unit_type_table[unit_type].Sight;
        strcpy(temp_buffer, scout_rank[value]);  // BUGBUG: should use index, not value  Sight == 1 is not 'Special', so 2 should be "I"
        strcat(specials_list->names[list_index], " ");
        strcat(specials_list->names[list_index], temp_buffer);
        specials_list->flags[list_index] = -1;
        specials_list->picts[list_index] = (SAMB_INT)special_seg[37];
        specials_list->helps[list_index] = HLP_SCOUTING;
        list_index++;
    }

    if((_unit_type_table[unit_type].Move_Flags & TELEPORTING) != 0)
    {
        strcpy(specials_list->names[list_index], "Teleporting");
        specials_list->flags[list_index] = -1;
        specials_list->picts[list_index] = (SAMB_INT)special_seg[0];
        specials_list->helps[list_index] = HLP_TELEPORTING;
        list_index++;
    }

    if((_unit_type_table[unit_type].Move_Flags & FORESTER) != 0)
    {
        strcpy(specials_list->names[list_index], "Forester");
        specials_list->flags[list_index] = -1;
        specials_list->picts[list_index] = (SAMB_INT)special_seg[1];
        specials_list->helps[list_index] = HLP_FORESTER_Ab;
        list_index++;
    }

    if((_unit_type_table[unit_type].Move_Flags & MOUNTAINEER) != 0)
    {
        strcpy(specials_list->names[list_index], "Mountaineer");
        specials_list->flags[list_index] = -1;
        specials_list->picts[list_index] = (SAMB_INT)special_seg[2];
        specials_list->helps[list_index] = HLP_MOUNTAINEER_Ab;
        list_index++;
    }

//if(unit_type == 37)
//{
//    __debugbreak();
//}

    for(itr = 0; itr < 15; itr++)
    {
        if((_unit_type_table[unit_type].Abilities & USW_Abilities[itr].Abl_Flag) != 0)
        {
            strcpy(specials_list->names[list_index], *USW_Abilities[itr].Name);
            specials_list->flags[list_index] = -1;
            specials_list->picts[list_index] = (SAMB_INT)special_seg[USW_Abilities[itr].special_icon_idx];
            specials_list->helps[list_index] = USW_Abilities[itr].HLP_Index;
            list_index++;
        }
    }

    for(itr = 0; itr < 10; itr++)
    {
        if((_unit_type_table[unit_type].Attribs_1 & USW_Attributes_1[itr].Attrib1_Flag) != 0)
        {
            strcpy(specials_list->names[list_index], *USW_Attributes_1[itr].Name);
            specials_list->flags[list_index] = -1;
            specials_list->picts[list_index] = (SAMB_INT)special_seg[USW_Attributes_1[itr].special_icon_idx];
            specials_list->helps[list_index] = USW_Attributes_1[itr].HLP_Index;
            list_index++;
        }
    }

    for(itr = 0; itr < 8; itr++)
    {
        if((_unit_type_table[unit_type].Attribs_2 & USW_Attributes_2[itr].Attrib2_Flag) != 0)
        {
            strcpy(specials_list->names[list_index], *USW_Attributes_2[itr].Name);
            specials_list->flags[list_index] = -1;
            specials_list->picts[list_index] = (SAMB_INT)special_seg[USW_Attributes_2[itr].special_icon_idx];
            specials_list->helps[list_index] = USW_Attributes_2[itr].HLP_Index;
            list_index++;
        }
    }

    for(itr = 0; itr < 11; itr++)
    {
        if((_unit_type_table[unit_type].Attack_Flags & USW_ATK_Flags[itr].Attack_Flag) != 0)
        {
            strcpy(specials_list->names[list_index], *USW_ATK_Flags[itr].Name);

            if((USW_ATK_Flags[itr].Attack_Flag & 0x04) != 0)  /* Att_Poison */
            {
                value = global_strategic_unit->Poison_Strength;
                itoa(value, temp_buffer, 10);
                strcat(specials_list->names[list_index], " ");
                strcat(specials_list->names[list_index], temp_buffer);
            }

            if((USW_ATK_Flags[itr].Attack_Flag & 0x80) != 0)  /* Att_StnTouch */
            {
                value = global_strategic_unit->Spec_Att_Attrib;
                itoa(value, temp_buffer, 10);
                strcat(specials_list->names[list_index], " ");
                strcat(specials_list->names[list_index], temp_buffer);
            }

            specials_list->flags[list_index] = -1;
            specials_list->picts[list_index] = (SAMB_INT)special_seg[USW_ATK_Flags[itr].special_icon_idx];
            specials_list->helps[list_index] = USW_ATK_Flags[itr].HLP_Index;
            list_index++;
        }
    }

    for(itr = 0; ((itr < 6) && (list_index < 39)); itr++)
    {
        if(global_strategic_unit->ranged_type == USW_SR_Attacks[itr].Ranged_Type)
        {
            strcpy(specials_list->names[list_index], *USW_SR_Attacks[itr].Name);

            value = global_strategic_unit->ranged;

            if(
                (USW_SR_Attacks[itr].Ranged_Type == 103) ||
                (USW_SR_Attacks[itr].Ranged_Type == 105)
            )
            {
                value = global_strategic_unit->Spec_Att_Attrib;
            }

            itoa(value, temp_buffer, 10);
            strcat(specials_list->names[list_index], " ");
            strcat(specials_list->names[list_index], temp_buffer);

            specials_list->flags[list_index] = -1;
            specials_list->picts[list_index] = (SAMB_INT)special_seg[USW_SR_Attacks[itr].special_icon_idx];
            specials_list->helps[list_index] = USW_SR_Attacks[itr].HLP_Index;
            list_index++;

            if(USW_SR_Attacks[itr].Ranged_Type == 104)
            {
                strcpy(specials_list->names[list_index], *USW_SR_Attacks[(itr + 1)].Name);
                value = global_strategic_unit->Spec_Att_Attrib;
                itoa(value, temp_buffer, 10);
                strcat(specials_list->names[list_index], " ");
                strcat(specials_list->names[list_index], temp_buffer);
                specials_list->flags[list_index] = -1;
                specials_list->picts[list_index] = (SAMB_INT)special_seg[USW_SR_Attacks[(itr + 1)].special_icon_idx];
                specials_list->helps[list_index] = USW_SR_Attacks[(itr + 1)].HLP_Index;
                list_index++;

                strcpy(specials_list->names[list_index], *USW_SR_Attacks[(itr - 1)].Name);
                value = global_strategic_unit->Spec_Att_Attrib;
                itoa(value, temp_buffer, 10);
                strcat(specials_list->names[list_index], " ");
                strcat(specials_list->names[list_index], temp_buffer);
                specials_list->flags[list_index] = -1;
                specials_list->picts[list_index] = (SAMB_INT)special_seg[USW_SR_Attacks[(itr - 1)].special_icon_idx];
                specials_list->helps[list_index] = USW_SR_Attacks[(itr - 1)].HLP_Index;
                list_index++;
            }
        }
    }

    *specials_count = list_index;

}


// WZD o78p03
// drake178: ITEM_GetPowerNames()
// ITEM_GetPowerNames()



/*
    WIZARDS.EXE  ovr089
*/


