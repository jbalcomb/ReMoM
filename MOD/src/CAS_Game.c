/*
    CAS_Game -- the CAS built-ins that read and change the game.  See CAS_Game.h.

    The CoM2 ID tables below follow the constant names in Warlord's MASTER.CAS and match them to 1.31 by name.
    -1 marks a CoM2 ID with no 1.31 counterpart.
*/

#include "CAS_Game.h"

#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/SAVEFILE.h"
#include "../../MoM/src/EVENTS.h"
#include "../../MoM/src/Terrain.h"
#include "../../MoM/src/TerrType.h"

#include <stdio.h>
#include <string.h>

#define CAS_SLOTS 100

/* ---------------------------------------------------------------------------------------------------------------- */
/*  CoM2 -> ReMoM IDs                                                                                               */
/* ---------------------------------------------------------------------------------------------------------------- */

/* Buildings (MASTER.CAS "Buildings").  CoM2 keeps most of 1.31's numbers but moved Armory, Temple, City Walls and the
   Ship Yard to make room for its own buildings. */
static const int16_t com2_building[] =
{
    /*  0 */ -1,
    /*  1 TradeGoods */ bt_TradeGoods, /*  2 Housing */ bt_Housing, /*  3 Barracks */ bt_Barracks, /*  4 Colosseum */ -1,
    /*  5 FighterGuild */ bt_FightersGuild, /*  6 ArmorerGuild */ bt_ArmorersGuild, /*  7 WarCollege */ bt_WarCollege,
    /*  8 Smithy */ bt_Smithy, /*  9 Stables */ bt_Stable, /* 10 AnimistGuild */ bt_AnimistsGuild,
    /* 11 FantasticStables */ bt_FantasticStable, /* 12 Dock */ bt_ShipWrightsGuild, /* 13 MagicMarket */ -1,
    /* 14 MaritimeGuild */ bt_MaritimeGuild, /* 15 Sawmill */ bt_Sawmill, /* 16 Library */ bt_Library,
    /* 17 SagesGuild */ bt_SagesGuild, /* 18 Oracle */ bt_Oracle, /* 19 Alchemistguild */ bt_AlchemistsGuild,
    /* 20 University */ bt_University, /* 21 WizardsGuild */ bt_WizardsGuild, /* 22 Shrine */ bt_Shrine,
    /* 23 AmplifyingTower */ -1, /* 24 Parthenon */ bt_Parthenon, /* 25 Cathedral */ bt_Cathedral,
    /* 26 Marketplace */ bt_Marketplace, /* 27 Bank */ bt_Bank, /* 28 MerchantGuild */ bt_MerchantsGuild,
    /* 29 Granary */ bt_Granary, /* 30 FarmerMarket */ bt_FarmersMarket, /* 31 ForesterGuild */ bt_ForestersGuild,
    /* 32 BuilderHall */ bt_BuildersHall, /* 33 MechaGuild */ bt_MechaniciansGuild, /* 34 MinerGuild */ bt_MinersGuild,
    /* 35 Monument */ -1, /* 36 LinkingTower */ -1, /* 37 CityWalls */ bt_CityWalls, /* 38 MercenaryCamp */ -1,
    /* 39 Armory */ bt_Armory, /* 40 Temple */ bt_Temple, /* 41 Moats */ -1, /* 42 Shipyard */ bt_ShipYard,
    /* 43 .. 74: Hangar, Brewery, ... Warlord's own buildings */
};

/* City enchantments (MASTER.CAS "City Buff IDs") -> s_CITY.enchantments[] index. */
static const int16_t com2_city_enchantment[] =
{
    /*  0 */ -1,
    /*  1 WallofFire */ WALL_OF_FIRE, /*  2 ChaosRift */ CHAOS_RIFT, /*  3 DarkRituals */ DARK_RITUALS,
    /*  4 EvilPresence */ EVIL_PRESENCE, /*  5 UranusBless */ -1, /*  6 Pestilence */ PESTILENCE,
    /*  7 CloudofShadow */ CLOUD_OF_SHADOW, /*  8 Drought */ FAMINE, /*  9 FlyingFortress */ FLYING_FORTRESS,
    /* 10 NatureWard */ NATURE_WARD, /* 11 SorceryWard */ SORCERY_WARD, /* 12 ChaosWard */ CHAOS_WARD,
    /* 13 LifeWard */ LIFE_WARD, /* 14 DeathWard */ DEATH_WARD, /* 15 NatureEye */ NATURES_EYE,
    /* 16 EarthGate */ EARTH_GATE, /* 17 StreamofLife */ STREAM_OF_LIFE, /* 18 GaiasBlessing */ GAIAS_BLESSING,
    /* 19 Inspirations */ INSPIRATIONS, /* 20 Prosperity */ PROSPERITY, /* 21 AltarofPeace */ -1,
    /* 22 HeavenlyLight */ HEAVENLY_LIGHT, /* 23 Consecration */ CONSECRATION, /* 24 WallofDarkness */ WALL_OF_DARKNESS,
    /* 25 AltarofBattle */ ALTAR_OF_BATTLE, /* 26 Abundance */ -1, /* 27 PhilStone */ -1, /* 28 AetherSink */ -1,
    /* 29 ChaosBless */ -1, /* 30 Inquisition */ -1, /* 31 PillarOfFaith */ -1, /* 32 Necropolis */ -1,
    /* 33 SorcererStone */ -1, /* 34 WallOfStone */ -1, /* 35 MarkOfConqueror */ -1, /* 36 AstralGate */ ASTRAL_GATE,
    /* 37 .. 42: CoM2's own */
};

/* Global enchantments (MASTER.CAS "Global Enchantment IDs") -> s_WIZARD.Globals[] index. */
static const int16_t com2_global[] =
{
    /*  0 */ -1,
    /*  1 EternalNight */ ETERNAL_NIGHT, /*  2 EvilOmens */ EVIL_OMENS, /*  3 ZombieMastery */ ZOMBIE_MASTERY,
    /*  4 AuraofMajesty */ AURA_OF_MAJESTY, /*  5 AEtherBinding */ -1, /*  6 RuneArray */ -1, /*  7 TimeStop */ TIME_STOP,
    /*  8 FairyRing */ -1, /*  9 SeismicMastery */ -1, /* 10 HerbMastery */ HERB_MASTERY, /* 11 ChaosSurge */ CHAOS_SURGE,
    /* 12 DoomMastery */ DOOM_MASTERY, /* 13 Doomsday */ -1, /* 14 MeteorStorm */ METEOR_STORMS,
    /* 15 Armageddon */ ARMAGEDDON, /* 16 Enlightenment */ -1, /* 17 LifeForce */ LIFE_FORCE, /* 18 Crusade */ CRUSADE,
    /* 19 JustCause */ JUST_CAUSE, /* 20 HolyArms */ HOLY_ARMS, /* 21 DivineOrder */ -1, /* 22 CharmofLife */ CHARM_OF_LIFE,
    /* 23 DetectMagic */ DETECT_MAGIC, /* 24 SurvivalInstinct */ -1, /* 25 DarkestHour */ -1, /* 26 Clairvoyance */ -1,
    /* 27 WindMastery */ WIND_MASTERY,
    /* 28 .. 60: CoM2's own */
};

/* Ores (MASTER.CAS "Ore types") -> terrain special.  Wild game and nightshade are flag bits; the minerals are the low
   nibble. */
static const int16_t com2_ore[] =
{
    /*  0 */ -1,
    /*  1 Wildgame */ TS_WILDGAME, /*  2 Nightshade */ 0x80, /*  3 Adamantium */ TS_ADAMANTIUM, /*  4 Mithril */ TS_MITHRIL,
    /*  5 Orihalcon */ -1, /*  6 Iron */ TS_IRON, /*  7 Coal */ TS_COAL, /*  8 Silver */ TS_SILVER, /*  9 Gold */ TS_GOLD,
    /* 10 Gems */ TS_GEMS, /* 11 Crysx */ TS_CRYSX, /* 12 Quork */ TS_QUORK,
};

/* Retorts (MASTER.CAS "Retort names") -> offset of the int8_t in s_WIZARD, or -1.  CoM2 dropped the realm masteries,
   Infernal/Divine Power, Mana Focusing, Node Mastery and Famous, and added retorts of its own. */
#define RETORT_FIELD(_field_) ((int16_t)offsetof(struct s_WIZARD, _field_))
static const int16_t com2_retort[] =
{
    /*  0 */ -1,
    /*  1 Alchemist */ RETORT_FIELD(alchemy), /*  2 SageMaster */ RETORT_FIELD(sage_master), /*  3 Specialist */ -1,
    /*  4 Warlord */ RETORT_FIELD(warlord), /*  5 Myrran */ RETORT_FIELD(myrran), /*  6 Tactician */ -1,
    /*  7 Channeler */ RETORT_FIELD(channeler), /*  8 Guardian */ -1, /*  9 Procurator */ -1,
    /* 10 Archmage */ RETORT_FIELD(archmage), /* 11 Benefactor */ -1, /* 12 CultLeader */ -1,
    /* 13 Artificer */ RETORT_FIELD(artificer), /* 14 Runemaster */ RETORT_FIELD(runemaster), /* 15 Astrologer */ -1,
    /* 16 Conjurer */ RETORT_FIELD(conjurer), /* 17 Charismatic */ RETORT_FIELD(charismatic), /* 18 Enchanter */ -1,
};
#undef RETORT_FIELD

/* Difficulty: 1.31's five levels placed on CoM2's nine (Beginner, Easy, Normal, Fair, Advanced, Expert, Master,
   Lunatic, Phantasm).  Intro/Easy/Normal match by name; Hard and Impossible are a judgement call. */
static const int16_t com2_difficulty[] = { /* god_Intro */ 1, /* god_Easy */ 2, /* god_Normal */ 3, /* god_Hard */ 5, /* god_Impossible */ 7 };

/* Overland tile kinds (MASTER.CAS "Overland Tiles"). */
enum e_COM2_TILE
{
    com2_tile_Ocean = 0, com2_tile_Shore = 1, com2_tile_Lake = 2, com2_tile_RiverMouth = 3, com2_tile_Grassland = 4,
    com2_tile_Hill = 5, com2_tile_Mountain = 6, com2_tile_Volcano = 7, com2_tile_Desert = 8, com2_tile_Swamp = 9,
    com2_tile_Tundra = 10, com2_tile_River = 11, com2_tile_Forest = 12, com2_tile_ChaosNode = 13,
    com2_tile_NatureNode = 14, com2_tile_SorceryNode = 15, com2_tile_RiverStart = 16
};

#define LOOKUP(_table_, _id_) ((((_id_) >= 0) && ((size_t)(_id_) < sizeof(_table_) / sizeof((_table_)[0]))) ? (_table_)[(_id_)] : -1)

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Script slots                                                                                                    */
/* ---------------------------------------------------------------------------------------------------------------- */

static int32_t cas_city_slots[NUM_CITIES][CAS_SLOTS];
static int32_t cas_wizard_slots[NUM_PLAYERS][CAS_SLOTS];

void CAS_Game_Reset_Slots(void)
{
    memset(cas_city_slots, 0, sizeof(cas_city_slots));
    memset(cas_wizard_slots, 0, sizeof(cas_wizard_slots));
    Save_File_Register_Section("CASCITYD", cas_city_slots, sizeof(cas_city_slots[0]), NUM_CITIES);
    Save_File_Register_Section("CASWIZD ", cas_wizard_slots, sizeof(cas_wizard_slots[0]), NUM_PLAYERS);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Argument checks                                                                                                 */
/* ---------------------------------------------------------------------------------------------------------------- */

#define FAIL(...) do { snprintf(err, err_size, __VA_ARGS__); return -1; } while(0)
#define CHECK_CITY(_c_) do { if((_CITIES == NULL) || ((_c_) < 1) || ((_c_) > _cities)) { FAIL("no city %ld", (long)(_c_)); } } while(0)
/* City 0 is "no city" (CITYONTILE of an empty square); scripts read from it before checking, so it reads as `_none_`. */
#define CITY_OR_NONE(_c_, _none_) do { if((_c_) == 0) { *result = (_none_); return 0; } CHECK_CITY(_c_); } while(0)
#define CITY(_c_) (_CITIES[(_c_) - 1])
#define CHECK_WIZARD(_w_) do { if(((_w_) < 0) || ((_w_) >= NUM_PLAYERS)) { FAIL("no wizard %ld", (long)(_w_)); } } while(0)
#define CHECK_SLOT(_v_) do { if(((_v_) < 1) || ((_v_) > CAS_SLOTS)) { FAIL("slot %ld is outside 1..%d", (long)(_v_), CAS_SLOTS); } } while(0)

/* Map square of plane P (1-based), x wrapped, as a ReMoM wp/wx/wy.  Returns 0 if P or y is off the map. */
static int com2_square(int32_t p, int32_t x, int32_t y, int * wp, int * wx, int * wy)
{
    if((p < 1) || (p > NUM_PLANES) || (y < 0) || (y >= WORLD_HEIGHT))
    {
        return 0;
    }
    x %= WORLD_WIDTH;
    if(x < 0)
    {
        x += WORLD_WIDTH;
    }
    *wp = p - 1;
    *wx = x;
    *wy = y;
    return 1;
}

/* CoM2 kind of a ReMoM terrain type. */
static int com2_tile_kind(int terrain_type)
{
    int t = terrain_type % NUM_TERRAIN_TYPES;
    if((t == tt_Ocean) || (t == tt_OceanAnim))                                      { return com2_tile_Ocean; }
    if(t == tt_BugGrass)                                                            { return com2_tile_Grassland; }
    if((t == tt_Lake) || ((t >= tt_Lake1) && (t <= tt_Lake4)))                       { return com2_tile_Lake; }
    if((t >= tt_Shore1_Fst) && (t <= tt_Shore1_Lst))                                 { return com2_tile_Shore; }
    if((t == tt_Grasslands1) || (t == tt_Grasslands2) || (t == tt_Grasslands3) || (t == tt_Grasslands4)) { return com2_tile_Grassland; }
    if((t == tt_Forest1) || (t == tt_Forest2) || (t == tt_Forest3))                  { return com2_tile_Forest; }
    if((t == tt_Mountain1) || ((t >= tt_Mountains_Fst) && (t <= tt_Mountains_Lst)))  { return com2_tile_Mountain; }
    if((t == tt_Hills1) || ((t >= tt_Hills_Fst) && (t <= tt_Hills_Lst)))             { return com2_tile_Hill; }
    if((t == tt_Desert1) || ((t >= tt_Desert2) && (t <= tt_Desert4)) || ((t >= tt_Desert_Fst) && (t <= tt_Desert_Lst))) { return com2_tile_Desert; }
    if((t == tt_Swamp1) || (t == tt_Swamp2) || (t == tt_Swamp3))                     { return com2_tile_Swamp; }
    if((t == tt_Tundra1) || (t == tt_Tundra2) || (t == tt_Tundra3) || ((t >= tt_Tundra_1st) && (t <= tt_Tundra_Last))) { return com2_tile_Tundra; }
    if(t == tt_SorceryNode)                                                         { return com2_tile_SorceryNode; }
    if(t == tt_NatureNode)                                                          { return com2_tile_NatureNode; }
    if(t == tt_ChaosNode)                                                           { return com2_tile_ChaosNode; }
    if(t == tt_Volcano)                                                             { return com2_tile_Volcano; }
    if((t >= tt_RiverM_1st) && (t <= tt_RiverM_end))                                 { return com2_tile_RiverMouth; }
    if(((t >= tt_Rivers_1st) && (t <= tt_Rivers_end)) || ((t >= tt_4WRiver1) && (t <= tt_4WRiver5))) { return com2_tile_River; }
    return com2_tile_Shore;                                                         /* the remaining shore ranges */
}

/* Calls fn for each square of city c's area (c is the ReMoM index) (the 21 squares 1.31 uses: 5x5 without the corners). */
static int city_area_count(int c, int (*fn)(int wp, int wx, int wy, int32_t what), int32_t what)
{
    int count = 0;
    int dy;
    int dx;
    int wp = _CITIES[c].wp;
    for(dy = -2; dy <= 2; dy++)
    {
        int wy = _CITIES[c].wy + dy;
        int reach = ((dy == -2) || (dy == 2)) ? 1 : 2;
        if((wy < 0) || (wy >= WORLD_HEIGHT))
        {
            continue;
        }
        for(dx = -reach; dx <= reach; dx++)
        {
            int wx = (_CITIES[c].wx + dx + WORLD_WIDTH) % WORLD_WIDTH;
            count += fn(wp, wx, wy, what);
        }
    }
    return count;
}

static int square_has_special(int wp, int wx, int wy, int32_t special)
{
    uint8_t value = _map_square_terrain_specials[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx];
    if((special == TS_WILDGAME) || (special == 0x80))
    {
        return (value & special) != 0;
    }
    return (value & 0x0F) == special;
}

static int square_is_kind(int wp, int wx, int wy, int32_t kind)
{
    return com2_tile_kind(p_world_map[wp][wy][wx]) == kind;
}

/* 1 if the wizard's fortress is gone but they are casting Spell of Return (CoM2: banished). */
static int wizard_banished(int w)
{
    return (_FORTRESSES != NULL) && !_FORTRESSES[w].active && (_players[w].casting_spell_idx == 214);   /* spl_Spell_Of_Return */
}

static int wizard_defeated(int w)
{
    return (_FORTRESSES != NULL) && !_FORTRESSES[w].active && !wizard_banished(w);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Built-ins                                                                                                       */
/* ---------------------------------------------------------------------------------------------------------------- */

int CAS_Game_Difficulty(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    *result = LOOKUP(com2_difficulty, _difficulty);
    if(*result < 0) { FAIL("difficulty %d has no CoM2 level", (int)_difficulty); }
    return 0;
}

int CAS_Game_Landsize(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { *result = _landsize; return 0; }
int CAS_Game_Nofwizards(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { *result = _num_players - 1; return 0; }
int CAS_Game_Maxcity(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { *result = (_CITIES != NULL) ? _cities : 0; return 0; }

int CAS_Game_Cityowner(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CITY_OR_NONE(a[0], -1); *result = CITY(a[0]).owner_idx; return 0; }
int CAS_Game_Cityrace(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CITY_OR_NONE(a[0], -1); *result = CITY(a[0]).race; return 0; }
int CAS_Game_Population(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CITY_OR_NONE(a[0], 0); *result = (CITY(a[0]).population * 1000) + (CITY(a[0]).Pop_10s * 10); return 0; }
int CAS_Game_Popunits(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CITY_OR_NONE(a[0], 0); *result = CITY(a[0]).population; return 0; }

int CAS_Game_Isbuilt(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int b;
    CITY_OR_NONE(a[0], 0);
    b = LOOKUP(com2_building, a[1]);
    *result = (b >= 0) && ((CITY(a[0]).bldg_status[b] == bs_Built) || (CITY(a[0]).bldg_status[b] == bs_Replaced));
    return 0;
}

int CAS_Game_Buildingsowned(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int b = LOOKUP(com2_building, a[1]);
    int ce = (a[2] != 0) ? LOOKUP(com2_city_enchantment, a[2]) : -1;
    int c;
    CHECK_WIZARD(a[0]);
    *result = 0;
    if((b < 0) || ((a[2] != 0) && (ce < 0)))
    {
        return 0;
    }
    for(c = 0; (_CITIES != NULL) && (c < _cities); c++)                 /* c: ReMoM index */
    {
        if((_CITIES[c].owner_idx == a[0]) && ((_CITIES[c].bldg_status[b] == bs_Built) || (_CITIES[c].bldg_status[b] == bs_Replaced)) && ((ce < 0) || (_CITIES[c].enchantments[ce] != 0)))
        {
            (*result)++;
        }
    }
    return 0;
}

int CAS_Game_Gold(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); *result = _players[a[0]].gold_reserve; return 0; }
int CAS_Game_Mana(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); *result = _players[a[0]].mana_reserve; return 0; }

int CAS_Game_Getcitydata(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_SLOT(a[1]); CITY_OR_NONE(a[0], 0); *result = cas_city_slots[a[0] - 1][a[1] - 1]; return 0; }
int CAS_Game_Setcitydata(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_SLOT(a[1]); CITY_OR_NONE(a[0], 0); cas_city_slots[a[0] - 1][a[1] - 1] = a[2]; return 0; }
int CAS_Game_Getwizarddata(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); CHECK_SLOT(a[1]); *result = cas_wizard_slots[a[0]][a[1] - 1]; return 0; }
int CAS_Game_Setwizarddata(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); CHECK_SLOT(a[1]); cas_wizard_slots[a[0]][a[1] - 1] = a[2]; return 0; }

int CAS_Game_Cityenchant(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int ce = LOOKUP(com2_city_enchantment, a[1]);
    CITY_OR_NONE(a[0], -1);
    *result = ((ce >= 0) && (CITY(a[0]).enchantments[ce] != 0)) ? (CITY(a[0]).enchantments[ce] - 1) : -1;   /* stored as player + 1 */
    return 0;
}

int CAS_Game_Books(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    CHECK_WIZARD(a[0]);
    *result = ((a[1] >= 1) && (a[1] <= 5)) ? _players[a[0]].spellranks[a[1] - 1] : 0;   /* CoM2 realms 1..5 = sbr_Nature..sbr_Death; 6 Arcane has no books */
    return 0;
}

int CAS_Game_Spellstate(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    CHECK_WIZARD(a[0]);
    *result = ((a[1] >= 1) && (a[1] <= 214)) ? _players[a[0]].spells_list[a[1] - 1] : 0;   /* 1.31's spells keep their IDs; 0-3 mean the same in both */
    return 0;
}

int CAS_Game_Hasglobal(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int g = LOOKUP(com2_global, a[1]);
    CHECK_WIZARD(a[0]);
    *result = (g >= 0) && (_players[a[0]].Globals[g] != 0);
    return 0;
}

int CAS_Game_Retort(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int offset = LOOKUP(com2_retort, a[1]);
    CHECK_WIZARD(a[0]);
    *result = (offset >= 0) && (((const int8_t *)&_players[a[0]])[offset] != 0);
    return 0;
}

int CAS_Game_Oreinrange(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int special = LOOKUP(com2_ore, a[1]);
    CITY_OR_NONE(a[0], 0);
    *result = (special > 0) ? city_area_count(a[0] - 1, square_has_special, special) : 0;
    return 0;
}

int CAS_Game_Terraininrange(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    CITY_OR_NONE(a[0], 0);
    *result = city_area_count(a[0] - 1, square_is_kind, a[1]);   /* LTRiverStart: 1.31 has no river-source tiles, so 0 */
    return 0;
}

int CAS_Game_Planesizex(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { *result = WORLD_WIDTH; return 0; }
int CAS_Game_Planesizey(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { *result = WORLD_HEIGHT; return 0; }

int CAS_Game_Cityontile(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int wp;
    int wx;
    int wy;
    int c;
    int ok;
    if(n == 1)
    {
        if((_UNITS == NULL) || (a[0] < 1) || (a[0] > _units)) { FAIL("no unit %ld", (long)a[0]); }
        ok = com2_square(_UNITS[a[0] - 1].wp + 1, _UNITS[a[0] - 1].wx, _UNITS[a[0] - 1].wy, &wp, &wx, &wy);
    }
    else if(n == 3)
    {
        ok = com2_square(a[0], a[1], a[2], &wp, &wx, &wy);
    }
    else
    {
        FAIL("CITYONTILE takes 1 or 3 arguments");
    }
    *result = 0;
    for(c = 0; ok && (_CITIES != NULL) && (c < _cities); c++)
    {
        if((_CITIES[c].wp == wp) && (_CITIES[c].wx == wx) && (_CITIES[c].wy == wy))
        {
            *result = c + 1;
            break;
        }
    }
    return 0;
}

int CAS_Game_Isfortress(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int w;
    CITY_OR_NONE(a[0], 0);
    *result = 0;
    for(w = 0; (_FORTRESSES != NULL) && (w < NUM_FORTRESSES); w++)
    {
        if(_FORTRESSES[w].active && (_FORTRESSES[w].wp == CITY(a[0]).wp) && (_FORTRESSES[w].wx == CITY(a[0]).wx) && (_FORTRESSES[w].wy == CITY(a[0]).wy))
        {
            *result = 1;
        }
    }
    return 0;
}

int CAS_Game_Banished(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); *result = wizard_banished(a[0]); return 0; }
int CAS_Game_Defeated(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); *result = wizard_defeated(a[0]); return 0; }

/* CoM2's scoring options are its own game-setup choices; ReMoM has none of them, so none is picked. */
int CAS_Game_Scoringoption(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { *result = 0; return 0; }

/* Event status and how long it has run (MASTER.CAS "Random events").  Returns 0 for an event ReMoM does not have. */
static int com2_event(int32_t e, int16_t * status, int16_t * duration, int16_t * value)
{
    struct s_EVENT_DATA * ev = events_table;
    *duration = 0;
    *value = 0;
    if(ev == NULL) { return 0; }
    switch(e)
    {
        case  1: *status = ev->Conjunction_Sorcery_Status; *duration = ev->Conjunction_Sorcery_Duration; return 1;
        case  2: *status = ev->Conjunction_Chaos_Status;   *duration = ev->Conjunction_Chaos_Duration;   return 1;
        case  3: *status = ev->Conjunction_Nature_Status;  *duration = ev->Conjunction_Nature_Duration;  return 1;
        case  4: *status = ev->Good_Moon_Status;           *duration = ev->Good_Moon_Duration;           return 1;
        case  5: *status = ev->Bad_Moon_Status;            *duration = ev->Bad_Moon_Duration;            return 1;
        case  6: *status = ev->Pirates_Status;     *value = ev->Pirates_Data;     return 1;
        case  8: *status = ev->Depletion_Status;   *value = ev->Depletion_Data;   return 1;
        case  9: *status = ev->marriage_status;    *value = ev->marriage_neutral_city_idx + 1; return 1;   /* cities are 1-based in CAS */
        case 12: *status = ev->Earthquake_Status;  *value = ev->Earthquake_Data + 1;  return 1;
        case 13: *status = ev->Meteor_Status;      *value = ev->Meteor_Data + 1;      return 1;
        case 14: *status = ev->minerals_status;    *value = ev->minerals_data;    return 1;
        case 15: *status = ev->Plague_Status;      *duration = ev->Plague_Duration; *value = ev->Plague_Data + 1; return 1;
        case 16: *status = ev->Population_Boom_Status; *duration = ev->Population_Boom_Duration; *value = ev->Population_Boom_Data + 1; return 1;
        case 17: *status = ev->Rebellion_Status;   *value = ev->Rebellion_Data + 1;   return 1;
        case 18: *status = ev->Gift_Status;        *value = ev->Gift_Data;        return 1;
        default: return 0;                         /* 7 Merchant, 10 Stroke, 11 Flux, 19 RaisingIsland, 20 MonsterOutbreak */
    }
}

int CAS_Game_Eventturn(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int16_t status = 0;
    int16_t duration;
    int16_t value;
    *result = 0;
    if(com2_event(a[0], &status, &duration, &value) && (status != es_Nothing))
    {
        *result = (duration > 0) ? duration : 1;   /* an active event has been active for at least this turn */
    }
    return 0;
}

int CAS_Game_Eventval(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int16_t status = 0;
    int16_t duration;
    int16_t value = 0;
    *result = 0;
    if(com2_event(a[0], &status, &duration, &value) && (status != es_Nothing))
    {
        *result = value;
    }
    return 0;
}

int CAS_Game_Treaty(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    CHECK_WIZARD(a[0]);
    CHECK_WIZARD(a[1]);
    *result = _players[a[0]].Dipl.Dipl_Status[a[1]];
    if(*result > WAR) { *result = WAR; }   /* 1.31's Final War is a war to CoM2 */
    return 0;
}

int CAS_Game_Contact(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); CHECK_WIZARD(a[1]); *result = (_players[a[0]].Dipl.Contacted[a[1]] != 0); return 0; }
int CAS_Game_Relation(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); CHECK_WIZARD(a[1]); *result = _players[a[0]].Dipl.Visible_Rel[a[1]]; return 0; }
int CAS_Game_Homerace(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); *result = _players[a[0]].capital_race; return 0; }
int CAS_Game_Researching(const int32_t * a, int n, int32_t * result, char * err, size_t err_size) { CHECK_WIZARD(a[0]); *result = _players[a[0]].researching_spell_idx; return 0; }

/* The units on a square, in unit-index order (CoM2's own order is not documented).  *unit is the nth one's CAS ID, or 0. */
static int units_on_square(int32_t p, int32_t x, int32_t y, int32_t nth, int32_t * unit)
{
    int wp;
    int wx;
    int wy;
    int u;
    int count = 0;
    *unit = 0;
    if(!com2_square(p, x, y, &wp, &wx, &wy) || (_UNITS == NULL))
    {
        return 0;
    }
    for(u = 0; u < _units; u++)
    {
        if((_UNITS[u].owner_idx != ST_UNDEFINED) && (_UNITS[u].wp == wp) && (_UNITS[u].wx == wx) && (_UNITS[u].wy == wy))
        {
            count++;
            if(count == nth)
            {
                *unit = u + 1;
            }
        }
    }
    return count;
}

int CAS_Game_Nofunitsontile(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    int32_t unit;
    *result = units_on_square(a[0], a[1], a[2], 0, &unit);
    return 0;
}

int CAS_Game_Unitontile(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    units_on_square(a[0], a[1], a[2], a[3], result);
    return 0;
}

/* GETSTAT(U,S,B[,F]): only S = SOwner (80) so far; the unit stats come with the unit hooks. */
int CAS_Game_Getstat(const int32_t * a, int n, int32_t * result, char * err, size_t err_size)
{
    if(a[1] != 80)
    {
        FAIL("GETSTAT stat %ld is not implemented in ReMoM yet (only SOwner)", (long)a[1]);
    }
    if(a[0] == 0)
    {
        *result = -1;   /* UNITONTILE gives 0 for an empty square; Warlord reads the owner before checking NOFUNITSONTILE */
        return 0;
    }
    if((_UNITS == NULL) || (a[0] < 1) || (a[0] > _units)) { FAIL("no unit %ld", (long)a[0]); }
    *result = _UNITS[a[0] - 1].owner_idx;
    return 0;
}
