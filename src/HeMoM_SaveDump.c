/**
 * HeMoM_SaveDump.c — Dump a .GAM save file to structured human-readable text.
 *
 * Reads the raw 123300-byte binary save and prints every field as a named
 * key=value line. The field layout matches Save_SAVE_GAM() / Load_SAVE_GAM()
 * in MoX/src/LOADSAVE.c exactly.
 *
 * Output format (one field per line):
 *   section.index.field = value
 *
 * Example:
 *   game.num_players = 5
 *   player[0].name = Rjak
 *   player[0].spellranks[0] = 2
 *   city[3].name = Haven
 *   world_map[0][1440] = 3
 */

#include "HeMoM_SaveDump.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../ext/stu_compat.h"
#include "../MoX/src/MOX_BASE.h"



/* ========================================================================= */
/*  Save File Layout Constants                                               */
/*  (mirrors the fread/fwrite sequence in LOADSAVE.c)                        */
/* ========================================================================= */

/* Counts */
#define SD_NUM_PLAYERS       6
#define SD_NUM_HERO_TYPES   35
#define SD_NUM_PLANES        2
#define SD_NUM_NODES        30
#define SD_NUM_FORTRESSES    6
#define SD_NUM_TOWERS        6
#define SD_NUM_LAIRS       102
#define SD_NUM_ITEMS       138
#define SD_NUM_CITIES      100
#define SD_NUM_UNITS      1009
#define SD_MAP_SIZE       2400    /* 60 x 40 */
#define SD_NUM_BUILDINGS    36
#define SD_NUM_CITY_ENCHANTMENTS 26
#define SD_NUM_RETORTS      18

/* Record sizes */
#define SD_HERO_SIZE        12
#define SD_WIZARD_SIZE    1224
#define SD_UNIT_SIZE        32
#define SD_CITY_SIZE       114
#define SD_LAIR_SIZE        24
#define SD_NODE_SIZE        48
#define SD_FORTRESS_SIZE     4
#define SD_TOWER_SIZE        4
#define SD_ITEM_SIZE        50
#define SD_INACTV_HERO_SIZE 16

/* Offsets (from the assert()s in LOADSAVE.c) */
#define SD_OFF_HEROES       0
#define SD_OFF_GAME      2520
#define SD_OFF_PLAYERS   2536
#define SD_OFF_WORLDMAPS 9880
#define SD_OFF_UU_TBL   19480
#define SD_OFF_LANDMASS 19864
#define SD_OFF_NODES    24664
#define SD_OFF_FORTRESS 26104
#define SD_OFF_TOWERS   26128
#define SD_OFF_LAIRS    26152
#define SD_OFF_ITEMS    28600
#define SD_OFF_CITIES   35500
#define SD_OFF_UNITS    46900
#define SD_OFF_TERRAIN  79188
#define SD_OFF_EXPLORED 83988
#define SD_OFF_MOVEMENT 88788
#define SD_OFF_EVENTS  117588
#define SD_OFF_FLAGS   117688
#define SD_OFF_VIZIER  122488
#define SD_OFF_PREMADE 122490
#define SD_OFF_HERONAMES 122740
#define SD_TOTAL_SIZE  123300



/* ========================================================================= */
/*  Read Helpers                                                             */
/* ========================================================================= */

static int8_t   rd_i8(const uint8_t *buf, int off) { return (int8_t)buf[off]; }
static uint8_t  rd_u8(const uint8_t *buf, int off) { return buf[off]; }
static int16_t  rd_i16(const uint8_t *buf, int off) { return (int16_t)(buf[off] | (buf[off + 1] << 8)); }
static uint16_t rd_u16(const uint8_t *buf, int off) { return (uint16_t)(buf[off] | (buf[off + 1] << 8)); }
static int32_t  rd_i32(const uint8_t *buf, int off) { return (int32_t)(buf[off] | (buf[off + 1] << 8) | (buf[off + 2] << 16) | (buf[off + 3] << 24)); }
static uint32_t rd_u32(const uint8_t *buf, int off) { return (uint32_t)(buf[off] | (buf[off + 1] << 8) | (buf[off + 2] << 16) | (buf[off + 3] << 24)); }

static void rd_str(const uint8_t *buf, int off, int maxlen, char *out)
{
    int itr;
    for (itr = 0; itr < maxlen; itr++)
    {
        out[itr] = (char)buf[off + itr];
        if (out[itr] == '\0') { break; }
    }
    out[maxlen - 1] = '\0';
}



/* ========================================================================= */
/*  Dump Sections                                                            */
/* ========================================================================= */

static void Dump_Heroes(FILE *fp, const uint8_t *sav)
{
    int player_idx, hero_idx, base;
    for (player_idx = 0; player_idx < SD_NUM_PLAYERS; player_idx++)
    {
        for (hero_idx = 0; hero_idx < SD_NUM_HERO_TYPES; hero_idx++)
        {
            base = SD_OFF_HEROES + (player_idx * SD_NUM_HERO_TYPES * SD_HERO_SIZE) + (hero_idx * SD_HERO_SIZE);
            fprintf(fp, "hero[%d][%d].level = %d\n", player_idx, hero_idx, rd_i16(sav, base + 0));
            fprintf(fp, "hero[%d][%d].abilities = 0x%08X\n", player_idx, hero_idx, rd_u32(sav, base + 2));
            fprintf(fp, "hero[%d][%d].casting_skill = %d\n", player_idx, hero_idx, rd_i8(sav, base + 6));
            fprintf(fp, "hero[%d][%d].spell_1 = %d\n", player_idx, hero_idx, rd_u8(sav, base + 7));
            fprintf(fp, "hero[%d][%d].spell_2 = %d\n", player_idx, hero_idx, rd_u8(sav, base + 8));
            fprintf(fp, "hero[%d][%d].spell_3 = %d\n", player_idx, hero_idx, rd_u8(sav, base + 9));
            fprintf(fp, "hero[%d][%d].spell_4 = %d\n", player_idx, hero_idx, rd_u8(sav, base + 10));
        }
    }
}

static void Dump_Game(FILE *fp, const uint8_t *sav)
{
    fprintf(fp, "game.num_players = %d\n", rd_i16(sav, SD_OFF_GAME + 0));
    fprintf(fp, "game.landsize = %d\n", rd_i16(sav, SD_OFF_GAME + 2));
    fprintf(fp, "game.magic = %d\n", rd_i16(sav, SD_OFF_GAME + 4));
    fprintf(fp, "game.difficulty = %d\n", rd_i16(sav, SD_OFF_GAME + 6));
    fprintf(fp, "game.cities = %d\n", rd_i16(sav, SD_OFF_GAME + 8));
    fprintf(fp, "game.units = %d\n", rd_i16(sav, SD_OFF_GAME + 10));
    fprintf(fp, "game.turn = %d\n", rd_i16(sav, SD_OFF_GAME + 12));
    fprintf(fp, "game.unit = %d\n", rd_i16(sav, SD_OFF_GAME + 14));
}

/*
 * Dump every field in struct s_WIZARD (1224 bytes).  Layout mirrored from
 * MoX/src/MOM_DAT.h:1382.  Nested sub-structs s_OWNED_HERO (28 B, x6),
 * s_WIZ_DIPL (306 B), s_Astr_Data (6 B) are expanded inline field-by-field.
 * Variable-length arrays (spells_list[240], Historian[288], Globals[24], etc.)
 * are emitted one element per line so diffs pinpoint which index changed.
 */
static void Dump_Players(FILE *fp, const uint8_t *sav)
{
    int itr, base, k, p;
    char name[21];
    char hero_name[15];

    for (itr = 0; itr < SD_NUM_PLAYERS; itr++)
    {
        base = SD_OFF_PLAYERS + (itr * SD_WIZARD_SIZE);

        /* --- 0x0000 header --- */
        fprintf(fp, "player[%d].wizard_id = %d\n",    itr, rd_u8(sav, base + 0x00));
        rd_str(sav, base + 0x01, 20, name);
        fprintf(fp, "player[%d].name = %s\n",         itr, name);
        fprintf(fp, "player[%d].capital_race = %d\n", itr, rd_u8(sav, base + 0x15));
        fprintf(fp, "player[%d].banner_id = %d\n",    itr, rd_u8(sav, base + 0x16));
        fprintf(fp, "player[%d].pad2B_17h = %d\n",    itr, rd_u8(sav, base + 0x17));
        fprintf(fp, "player[%d].personality = %d\n",  itr, rd_u16(sav, base + 0x18));
        fprintf(fp, "player[%d].objective = %d\n",    itr, rd_u16(sav, base + 0x1A));
        for (k = 0; k < 6; k++)
        {
            fprintf(fp, "player[%d].unknown_01Ch[%d] = %d\n", itr, k, rd_u8(sav, base + 0x1C + k));
        }
        fprintf(fp, "player[%d].som_research_cost = %d\n", itr, rd_u16(sav, base + 0x22));
        fprintf(fp, "player[%d].fame = %d\n",              itr, rd_u16(sav, base + 0x24));
        fprintf(fp, "player[%d].power_base = %d\n",        itr, rd_u16(sav, base + 0x26));
        fprintf(fp, "player[%d].volcanoes = %d\n",         itr, rd_u16(sav, base + 0x28));
        fprintf(fp, "player[%d].research_ratio = %d\n",    itr, rd_u8(sav,  base + 0x2A));
        fprintf(fp, "player[%d].mana_ratio = %d\n",        itr, rd_u8(sav,  base + 0x2B));
        fprintf(fp, "player[%d].skill_ratio = %d\n",       itr, rd_u8(sav,  base + 0x2C));
        fprintf(fp, "player[%d].volcano_power = %d\n",     itr, rd_u8(sav,  base + 0x2D));
        fprintf(fp, "player[%d].summon_wx = %d\n",         itr, rd_i16(sav, base + 0x2E));
        fprintf(fp, "player[%d].summon_wy = %d\n",         itr, rd_i16(sav, base + 0x30));
        fprintf(fp, "player[%d].summon_wp = %d\n",         itr, rd_i16(sav, base + 0x32));

        /* --- 0x0034 research_spells[8] --- */
        for (k = 0; k < 8; k++)
        {
            fprintf(fp, "player[%d].research_spells[%d] = %d\n", itr, k, rd_u16(sav, base + 0x34 + (k * 2)));
        }

        /* --- 0x0044 unknown + casting state --- */
        for (k = 0; k < 4; k++)
        {
            fprintf(fp, "player[%d].unknown_044h[%d] = %d\n", itr, k, rd_u8(sav, base + 0x44 + k));
        }
        fprintf(fp, "player[%d].average_unit_cost = %d\n",      itr, rd_u16(sav, base + 0x48));
        fprintf(fp, "player[%d].unknown_04Ah = %d\n",           itr, rd_u16(sav, base + 0x4A));
        fprintf(fp, "player[%d].cmbt_skill_left = %d\n",        itr, rd_u16(sav, base + 0x4C));
        fprintf(fp, "player[%d].casting_cost_remaining = %d\n", itr, rd_u16(sav, base + 0x4E));
        fprintf(fp, "player[%d].casting_cost_original = %d\n",  itr, rd_u16(sav, base + 0x50));
        fprintf(fp, "player[%d].casting_spell_idx = %d\n",      itr, rd_u16(sav, base + 0x52));
        fprintf(fp, "player[%d].skill_left = %d\n",             itr, rd_u16(sav, base + 0x54));
        fprintf(fp, "player[%d].nominal_skill = %d\n",          itr, rd_u16(sav, base + 0x56));
        fprintf(fp, "player[%d].tax_rate = %d\n",               itr, rd_u16(sav, base + 0x58));

        /* --- 0x005A Spellranks (Nature, Sorcery, Chaos, Life, Death) --- */
        fprintf(fp, "player[%d].spellranks[sbr_Nature] = %d\n",  itr, rd_i16(sav, base + 0x5A));
        fprintf(fp, "player[%d].spellranks[sbr_Sorcery] = %d\n", itr, rd_i16(sav, base + 0x5C));
        fprintf(fp, "player[%d].spellranks[sbr_Chaos] = %d\n",   itr, rd_i16(sav, base + 0x5E));
        fprintf(fp, "player[%d].spellranks[sbr_Life] = %d\n",    itr, rd_i16(sav, base + 0x60));
        fprintf(fp, "player[%d].spellranks[sbr_Death] = %d\n",   itr, rd_i16(sav, base + 0x62));

        /* --- 0x0064 Retorts (18 x int8) --- */
        fprintf(fp, "player[%d].retort.alchemy = %d\n",         itr, rd_i8(sav, base + 0x64));
        fprintf(fp, "player[%d].retort.warlord = %d\n",         itr, rd_i8(sav, base + 0x65));
        fprintf(fp, "player[%d].retort.chaos_mastery = %d\n",   itr, rd_i8(sav, base + 0x66));
        fprintf(fp, "player[%d].retort.nature_mastery = %d\n",  itr, rd_i8(sav, base + 0x67));
        fprintf(fp, "player[%d].retort.sorcery_mastery = %d\n", itr, rd_i8(sav, base + 0x68));
        fprintf(fp, "player[%d].retort.infernal_power = %d\n",  itr, rd_i8(sav, base + 0x69));
        fprintf(fp, "player[%d].retort.divine_power = %d\n",    itr, rd_i8(sav, base + 0x6A));
        fprintf(fp, "player[%d].retort.sage_master = %d\n",     itr, rd_i8(sav, base + 0x6B));
        fprintf(fp, "player[%d].retort.channeler = %d\n",       itr, rd_i8(sav, base + 0x6C));
        fprintf(fp, "player[%d].retort.myrran = %d\n",          itr, rd_i8(sav, base + 0x6D));
        fprintf(fp, "player[%d].retort.archmage = %d\n",        itr, rd_i8(sav, base + 0x6E));
        fprintf(fp, "player[%d].retort.mana_focusing = %d\n",   itr, rd_i8(sav, base + 0x6F));
        fprintf(fp, "player[%d].retort.node_mastery = %d\n",    itr, rd_i8(sav, base + 0x70));
        fprintf(fp, "player[%d].retort.famous = %d\n",          itr, rd_i8(sav, base + 0x71));
        fprintf(fp, "player[%d].retort.runemaster = %d\n",      itr, rd_i8(sav, base + 0x72));
        fprintf(fp, "player[%d].retort.conjurer = %d\n",        itr, rd_i8(sav, base + 0x73));
        fprintf(fp, "player[%d].retort.charismatic = %d\n",     itr, rd_i8(sav, base + 0x74));
        fprintf(fp, "player[%d].retort.artificer = %d\n",       itr, rd_i8(sav, base + 0x75));

        /* --- 0x0076 Heroes[6] (s_OWNED_HERO, 28 bytes each) --- */
        for (k = 0; k < 6; k++)
        {
            int h = base + 0x76 + (k * 28);
            fprintf(fp, "player[%d].hero[%d].unit_idx = %d\n", itr, k, rd_i16(sav, h + 0x00));
            rd_str(sav, h + 0x02, 14, hero_name);
            fprintf(fp, "player[%d].hero[%d].name = %s\n",       itr, k, hero_name);
            fprintf(fp, "player[%d].hero[%d].item[0] = %d\n",    itr, k, rd_i16(sav, h + 0x10));
            fprintf(fp, "player[%d].hero[%d].item[1] = %d\n",    itr, k, rd_i16(sav, h + 0x12));
            fprintf(fp, "player[%d].hero[%d].item[2] = %d\n",    itr, k, rd_i16(sav, h + 0x14));
            fprintf(fp, "player[%d].hero[%d].item_slot[0] = %d\n", itr, k, rd_i16(sav, h + 0x16));
            fprintf(fp, "player[%d].hero[%d].item_slot[1] = %d\n", itr, k, rd_i16(sav, h + 0x18));
            fprintf(fp, "player[%d].hero[%d].item_slot[2] = %d\n", itr, k, rd_i16(sav, h + 0x1A));
        }

        /* --- 0x011E unknown + 0x0120 Vault_Items[4] --- */
        fprintf(fp, "player[%d].unknown_11Eh = %d\n", itr, rd_u16(sav, base + 0x11E));
        for (k = 0; k < 4; k++)
        {
            fprintf(fp, "player[%d].vault_items[%d] = %d\n", itr, k, rd_i16(sav, base + 0x120 + (k * 2)));
        }

        /* --- 0x0128 Dipl (s_WIZ_DIPL, 306 bytes) --- */
        {
            int d = base + 0x128;
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.contacted[%d] = %d\n",           itr, p, rd_i8(sav,  d + 0x00 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.treaty_modifier[%d] = %d\n",     itr, p, rd_i16(sav, d + 0x06 + (p * 2))); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.peace_modifier[%d] = %d\n",      itr, p, rd_i16(sav, d + 0x12 + (p * 2))); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.exchange_modifier[%d] = %d\n",   itr, p, rd_i16(sav, d + 0x1E + (p * 2))); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.visible_rel[%d] = %d\n",         itr, p, rd_i8(sav,  d + 0x2A + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.dipl_status[%d] = %d\n",         itr, p, rd_i8(sav,  d + 0x30 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.da_strength[%d] = %d\n",         itr, p, rd_i16(sav, d + 0x36 + (p * 2))); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.dipl_action[%d] = %d\n",         itr, p, rd_i8(sav,  d + 0x42 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.da_spell[%d] = %d\n",            itr, p, rd_i16(sav, d + 0x48 + (p * 2))); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.da_city[%d] = %d\n",             itr, p, rd_i8(sav,  d + 0x54 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.default_rel[%d] = %d\n",         itr, p, rd_i8(sav,  d + 0x5A + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.contact_progress[%d] = %d\n",    itr, p, rd_i8(sav,  d + 0x60 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.broken_treaty[%d] = %d\n",       itr, p, rd_i8(sav,  d + 0x66 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_6C[%d] = %d\n",            itr, p, rd_i16(sav, d + 0x6C + (p * 2))); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.hidden_rel[%d] = %d\n",          itr, p, rd_i8(sav,  d + 0x78 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_7E[%d] = %d\n",            itr, p, rd_i8(sav,  d + 0x7E + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_84[%d] = %d\n",            itr, p, rd_i8(sav,  d + 0x84 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_8A[%d] = %d\n",            itr, p, rd_i8(sav,  d + 0x8A + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_90[%d] = %d\n",            itr, p, rd_i8(sav,  d + 0x90 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.tribute_spell[%d] = %d\n",       itr, p, rd_i8(sav,  d + 0x96 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_9C[%d] = %d\n",            itr, p, rd_u8(sav,  d + 0x9C + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_A2[%d] = %d\n",            itr, p, rd_u8(sav,  d + 0xA2 + p)); }
            for (k = 0; k < (SD_NUM_PLAYERS * 6); k++) { fprintf(fp, "player[%d].dipl.field_A8[%d] = %d\n",      itr, k, rd_i16(sav, d + 0xA8 + (k * 2))); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_F0[%d] = %d\n",            itr, p, rd_i8(sav,  d + 0xF0 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.tribute_gold[%d] = %d\n",        itr, p, rd_i16(sav, d + 0xF6 + (p * 2))); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_102[%d] = %d\n",           itr, p, rd_i8(sav,  d + 0x102 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_108[%d] = %d\n",           itr, p, rd_i8(sav,  d + 0x108 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_10E[%d] = %d\n",           itr, p, rd_i8(sav,  d + 0x10E + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_114[%d] = %d\n",           itr, p, rd_i8(sav,  d + 0x114 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_11A[%d] = %d\n",           itr, p, rd_i8(sav,  d + 0x11A + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_120[%d] = %d\n",           itr, p, rd_i8(sav,  d + 0x120 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.field_126[%d] = %d\n",           itr, p, rd_i8(sav,  d + 0x126 + p)); }
            for (p = 0; p < SD_NUM_PLAYERS; p++) { fprintf(fp, "player[%d].dipl.g_warning_progress[%d] = %d\n",  itr, p, rd_i8(sav,  d + 0x12C + p)); }
        }

        /* --- 0x025A research / mana / casting skill / researching spell --- */
        fprintf(fp, "player[%d].research_cost_remaining = %d\n", itr, rd_u16(sav, base + 0x25A));
        fprintf(fp, "player[%d].mana_reserve = %d\n",            itr, rd_u16(sav, base + 0x25C));
        fprintf(fp, "player[%d].spell_casting_skill = %d\n",     itr, rd_i32(sav, base + 0x25E));
        fprintf(fp, "player[%d].researching_spell_idx = %d\n",   itr, rd_i16(sav, base + 0x262));

        /* --- 0x0264 spells_list[240] --- */
        for (k = 0; k < 240; k++)
        {
            fprintf(fp, "player[%d].spells_list[%d] = %d\n", itr, k, rd_u8(sav, base + 0x264 + k));
        }

        /* --- 0x0354 tail fields --- */
        fprintf(fp, "player[%d].defeated_wizards = %d\n", itr, rd_u16(sav, base + 0x354));
        fprintf(fp, "player[%d].gold_reserve = %d\n",     itr, rd_u16(sav, base + 0x356));
        fprintf(fp, "player[%d].field_358 = %d\n",        itr, rd_u16(sav, base + 0x358));

        /* --- 0x035A Astr (s_Astr_Data, 6 bytes) --- */
        fprintf(fp, "player[%d].astr.magic_power = %d\n",    itr, rd_i16(sav, base + 0x35A));
        fprintf(fp, "player[%d].astr.spell_research = %d\n", itr, rd_i16(sav, base + 0x35C));
        fprintf(fp, "player[%d].astr.army_strength = %d\n",  itr, rd_i16(sav, base + 0x35E));

        fprintf(fp, "player[%d].pop_div_10k = %d\n", itr, rd_u16(sav, base + 0x360));

        /* --- 0x0362 Historian[288] --- */
        for (k = 0; k < 288; k++)
        {
            fprintf(fp, "player[%d].historian[%d] = %d\n", itr, k, rd_u8(sav, base + 0x362 + k));
        }

        /* --- 0x0482 Globals[NUM_OVERLAND_ENCHANTMENTS=24] --- */
        for (k = 0; k < 24; k++)
        {
            fprintf(fp, "player[%d].globals[%d] = %d\n", itr, k, rd_u8(sav, base + 0x482 + k));
        }

        fprintf(fp, "player[%d].magic_strategy = %d\n", itr, rd_u16(sav, base + 0x49A));
        fprintf(fp, "player[%d].unknown_49Ch = %d\n",   itr, rd_u16(sav, base + 0x49C));

        /* --- 0x049E Hostility[NUM_PLAYERS=6] --- */
        for (p = 0; p < SD_NUM_PLAYERS; p++)
        {
            fprintf(fp, "player[%d].hostility[%d] = %d\n", itr, p, rd_u16(sav, base + 0x49E + (p * 2)));
        }

        fprintf(fp, "player[%d].reevaluate_hostility_countdown = %d\n",       itr, rd_u16(sav, base + 0x4AA));
        fprintf(fp, "player[%d].reevaluate_magic_strategy_countdown = %d\n",  itr, rd_u16(sav, base + 0x4AC));
        fprintf(fp, "player[%d].reevaluate_magic_power_countdown = %d\n",     itr, rd_u16(sav, base + 0x4AE));

        /* --- 0x04B0 peace_duration[6] --- */
        for (p = 0; p < SD_NUM_PLAYERS; p++)
        {
            fprintf(fp, "player[%d].peace_duration[%d] = %d\n", itr, p, rd_u8(sav, base + 0x4B0 + p));
        }

        fprintf(fp, "player[%d].idk_ai_strategy_1 = %d\n", itr, rd_i16(sav, base + 0x4B6));
        fprintf(fp, "player[%d].idk_ai_strategy_2 = %d\n", itr, rd_i16(sav, base + 0x4B8));
        fprintf(fp, "player[%d].idk_ai_strategy_3 = %d\n", itr, rd_i16(sav, base + 0x4BA));
        fprintf(fp, "player[%d].idk_ai_strategy_4 = %d\n", itr, rd_i16(sav, base + 0x4BC));

        /* --- 0x04BE field_4BE[NUM_PLAYERS=6] --- */
        for (p = 0; p < SD_NUM_PLAYERS; p++)
        {
            fprintf(fp, "player[%d].field_4BE[%d] = %d\n", itr, p, rd_u8(sav, base + 0x4BE + p));
        }

        fprintf(fp, "player[%d].prim_realm = %d\n", itr, rd_u16(sav, base + 0x4C4));
        fprintf(fp, "player[%d].sec_realm = %d\n",  itr, rd_u16(sav, base + 0x4C6));
    }
}

static void Dump_World_Maps(FILE *fp, const uint8_t *sav)
{
    int plane, sq;
    for (plane = 0; plane < SD_NUM_PLANES; plane++)
    {
        for (sq = 0; sq < SD_MAP_SIZE; sq++)
        {
            fprintf(fp, "world_map[%d][%d] = %d\n", plane, sq, rd_u8(sav, SD_OFF_WORLDMAPS + (plane * SD_MAP_SIZE) + sq));
        }
    }
}

static void Dump_Nodes(FILE *fp, const uint8_t *sav)
{
    int itr, base;
    for (itr = 0; itr < SD_NUM_NODES; itr++)
    {
        base = SD_OFF_NODES + (itr * SD_NODE_SIZE);
        fprintf(fp, "node[%d].wx = %d\n", itr, rd_i8(sav, base + 0));
        fprintf(fp, "node[%d].wy = %d\n", itr, rd_i8(sav, base + 1));
        fprintf(fp, "node[%d].wp = %d\n", itr, rd_i8(sav, base + 2));
        fprintf(fp, "node[%d].owner_idx = %d\n", itr, rd_i8(sav, base + 3));
        fprintf(fp, "node[%d].power = %d\n", itr, rd_i8(sav, base + 4));
        fprintf(fp, "node[%d].type = %d\n", itr, rd_i8(sav, base + 0x2D));
        fprintf(fp, "node[%d].flags = %d\n", itr, rd_i8(sav, base + 0x2E));
    }
}

static void Dump_Fortresses(FILE *fp, const uint8_t *sav)
{
    int itr, base;
    for (itr = 0; itr < SD_NUM_FORTRESSES; itr++)
    {
        base = SD_OFF_FORTRESS + (itr * SD_FORTRESS_SIZE);
        fprintf(fp, "fortress[%d].wx = %d\n", itr, rd_i8(sav, base + 0));
        fprintf(fp, "fortress[%d].wy = %d\n", itr, rd_i8(sav, base + 1));
        fprintf(fp, "fortress[%d].wp = %d\n", itr, rd_i8(sav, base + 2));
        fprintf(fp, "fortress[%d].active = %d\n", itr, rd_i8(sav, base + 3));
    }
}

static void Dump_Towers(FILE *fp, const uint8_t *sav)
{
    int itr, base;
    for (itr = 0; itr < SD_NUM_TOWERS; itr++)
    {
        base = SD_OFF_TOWERS + (itr * SD_TOWER_SIZE);
        fprintf(fp, "tower[%d].wx = %d\n", itr, rd_i8(sav, base + 0));
        fprintf(fp, "tower[%d].wy = %d\n", itr, rd_i8(sav, base + 1));
        fprintf(fp, "tower[%d].owner_idx = %d\n", itr, rd_i8(sav, base + 2));
    }
}

static void Dump_Lairs(FILE *fp, const uint8_t *sav)
{
    int itr, base;
    for (itr = 0; itr < SD_NUM_LAIRS; itr++)
    {
        base = SD_OFF_LAIRS + (itr * SD_LAIR_SIZE);
        fprintf(fp, "lair[%d].wx = %d\n", itr, rd_i8(sav, base + 0));
        fprintf(fp, "lair[%d].wy = %d\n", itr, rd_i8(sav, base + 1));
        fprintf(fp, "lair[%d].wp = %d\n", itr, rd_i8(sav, base + 2));
        fprintf(fp, "lair[%d].intact = %d\n", itr, rd_i8(sav, base + 3));
        fprintf(fp, "lair[%d].type = %d\n", itr, rd_i8(sav, base + 4));
        fprintf(fp, "lair[%d].guard1_unit_type = %d\n", itr, rd_u8(sav, base + 5));
        fprintf(fp, "lair[%d].guard1_count = %d\n", itr, rd_u8(sav, base + 6));
        fprintf(fp, "lair[%d].guard2_unit_type = %d\n", itr, rd_u8(sav, base + 7));
        fprintf(fp, "lair[%d].guard2_count = %d\n", itr, rd_u8(sav, base + 8));
        fprintf(fp, "lair[%d].loot_gold = %d\n", itr, rd_i16(sav, base + 0x0A));
        fprintf(fp, "lair[%d].loot_mana = %d\n", itr, rd_i16(sav, base + 0x0C));
        fprintf(fp, "lair[%d].spell_n_special = %d\n", itr, rd_i8(sav, base + 0x0E));
        fprintf(fp, "lair[%d].item_count = %d\n", itr, rd_i8(sav, base + 0x10));
    }
}

static void Dump_Items(FILE *fp, const uint8_t *sav)
{
    int itr, base;
    char name[31];
    for (itr = 0; itr < SD_NUM_ITEMS; itr++)
    {
        base = SD_OFF_ITEMS + (itr * SD_ITEM_SIZE);
        rd_str(sav, base, 30, name);
        fprintf(fp, "item[%d].name = %s\n", itr, name);
        fprintf(fp, "item[%d].slot_type = %d\n", itr, rd_i8(sav, base + 0x20));
        fprintf(fp, "item[%d].type = %d\n", itr, rd_i8(sav, base + 0x21));
        fprintf(fp, "item[%d].cost = %d\n", itr, rd_i16(sav, base + 0x22));
        fprintf(fp, "item[%d].powers = 0x%08X\n", itr, rd_u32(sav, base + 0x2E));
    }
}

/*
 * Dump every field in struct s_CITY (114 bytes).  Layout from MOM_DAT.h:1673.
 */
static void Dump_Cities(FILE *fp, const uint8_t *sav)
{
    int itr, base, b;
    char name[15];
    for (itr = 0; itr < SD_NUM_CITIES; itr++)
    {
        base = SD_OFF_CITIES + (itr * SD_CITY_SIZE);

        /* 0x00 */ rd_str(sav, base, 14, name);
        fprintf(fp, "city[%d].name = %s\n", itr, name);
        /* 0x0E */ fprintf(fp, "city[%d].race = %d\n",              itr, rd_i8(sav, base + 0x0E));
        /* 0x0F */ fprintf(fp, "city[%d].wx = %d\n",                itr, rd_i8(sav, base + 0x0F));
        /* 0x10 */ fprintf(fp, "city[%d].wy = %d\n",                itr, rd_i8(sav, base + 0x10));
        /* 0x11 */ fprintf(fp, "city[%d].wp = %d\n",                itr, rd_i8(sav, base + 0x11));
        /* 0x12 */ fprintf(fp, "city[%d].owner_idx = %d\n",         itr, rd_i8(sav, base + 0x12));
        /* 0x13 */ fprintf(fp, "city[%d].size = %d\n",              itr, rd_i8(sav, base + 0x13));
        /* 0x14 */ fprintf(fp, "city[%d].population = %d\n",        itr, rd_i8(sav, base + 0x14));
        /* 0x15 */ fprintf(fp, "city[%d].farmer_count = %d\n",      itr, rd_i8(sav, base + 0x15));
        /* 0x16 */ fprintf(fp, "city[%d].sold_building = %d\n",     itr, rd_i8(sav, base + 0x16));
        /* 0x17 */ fprintf(fp, "city[%d].pad2B_17h = %d\n",         itr, rd_i8(sav, base + 0x17));
        /* 0x18 */ fprintf(fp, "city[%d].pop_10s = %d\n",           itr, rd_i16(sav, base + 0x18));
        /* 0x1A */ fprintf(fp, "city[%d].contacts = %d\n",          itr, rd_u8(sav, base + 0x1A));
        /* 0x1B */ fprintf(fp, "city[%d].pad2B_1Bh = %d\n",         itr, rd_i8(sav, base + 0x1B));
        /* 0x1C */ fprintf(fp, "city[%d].construction = %d\n",      itr, rd_i16(sav, base + 0x1C));
        /* 0x1E */ fprintf(fp, "city[%d].bldg_cnt = %d\n",          itr, rd_i8(sav, base + 0x1E));
        /* 0x1F */ for (b = 0; b < SD_NUM_BUILDINGS; b++)
        {
            fprintf(fp, "city[%d].bldg_status[%d] = %d\n", itr, b, rd_i8(sav, base + 0x1F + b));
        }
        /* 0x43 */ for (b = 0; b < SD_NUM_CITY_ENCHANTMENTS; b++)
        {
            fprintf(fp, "city[%d].enchantment[%d] = %d\n", itr, b, rd_u8(sav, base + 0x43 + b));
        }
        /* 0x5D */ fprintf(fp, "city[%d].production_units = %d\n",  itr, rd_i8(sav, base + 0x5D));
        /* 0x5E */ fprintf(fp, "city[%d].prod_accu = %d\n",         itr, rd_i16(sav, base + 0x5E));
        /* 0x60 */ fprintf(fp, "city[%d].gold_units = %d\n",        itr, rd_u8(sav, base + 0x60));
        /* 0x61 */ fprintf(fp, "city[%d].building_maintenance = %d\n", itr, rd_i8(sav, base + 0x61));
        /* 0x62 */ fprintf(fp, "city[%d].mana_units = %d\n",        itr, rd_i8(sav, base + 0x62));
        /* 0x63 */ fprintf(fp, "city[%d].research_units = %d\n",    itr, rd_i8(sav, base + 0x63));
        /* 0x64 */ fprintf(fp, "city[%d].food_units = %d\n",        itr, rd_i8(sav, base + 0x64));
        /* 0x65 */ for (b = 0; b < 13; b++)
        {
            fprintf(fp, "city[%d].road_connections[%d] = %d\n", itr, b, rd_u8(sav, base + 0x65 + b));
        }
    }
}

/*
 * Dump every field in struct s_UNIT (32 bytes).  Layout from MOM_DAT.h:1695.
 */
static void Dump_Units(FILE *fp, const uint8_t *sav)
{
    int itr, base;
    for (itr = 0; itr < SD_NUM_UNITS; itr++)
    {
        base = SD_OFF_UNITS + (itr * SD_UNIT_SIZE);
        /* 0x00 */ fprintf(fp, "unit[%d].wx = %d\n",              itr, rd_i8(sav,  base + 0x00));
        /* 0x01 */ fprintf(fp, "unit[%d].wy = %d\n",              itr, rd_i8(sav,  base + 0x01));
        /* 0x02 */ fprintf(fp, "unit[%d].wp = %d\n",              itr, rd_i8(sav,  base + 0x02));
        /* 0x03 */ fprintf(fp, "unit[%d].owner_idx = %d\n",       itr, rd_i8(sav,  base + 0x03));
        /* 0x04 */ fprintf(fp, "unit[%d].moves2_max = %d\n",      itr, rd_i8(sav,  base + 0x04));
        /* 0x05 */ fprintf(fp, "unit[%d].type = %d\n",            itr, rd_u8(sav,  base + 0x05));
        /* 0x06 */ fprintf(fp, "unit[%d].hero_slot = %d\n",       itr, rd_i8(sav,  base + 0x06));
        /* 0x07 */ fprintf(fp, "unit[%d].finished = %d\n",        itr, rd_i8(sav,  base + 0x07));
        /* 0x08 */ fprintf(fp, "unit[%d].moves2 = %d\n",          itr, rd_i8(sav,  base + 0x08));
        /* 0x09 */ fprintf(fp, "unit[%d].dst_wx = %d\n",          itr, rd_i8(sav,  base + 0x09));
        /* 0x0A */ fprintf(fp, "unit[%d].dst_wy = %d\n",          itr, rd_i8(sav,  base + 0x0A));
        /* 0x0B */ fprintf(fp, "unit[%d].status = %d\n",          itr, rd_i8(sav,  base + 0x0B));
        /* 0x0C */ fprintf(fp, "unit[%d].level = %d\n",           itr, rd_i8(sav,  base + 0x0C));
        /* 0x0D */ fprintf(fp, "unit[%d].pad2B_0Dh = %d\n",       itr, rd_u8(sav,  base + 0x0D));
        /* 0x0E */ fprintf(fp, "unit[%d].xp = %d\n",             itr, rd_i16(sav, base + 0x0E));
        /* 0x10 */ fprintf(fp, "unit[%d].move_failed = %d\n",     itr, rd_i8(sav,  base + 0x10));
        /* 0x11 */ fprintf(fp, "unit[%d].damage = %d\n",          itr, rd_i8(sav,  base + 0x11));
        /* 0x12 */ fprintf(fp, "unit[%d].draw_priority = %d\n",   itr, rd_i8(sav,  base + 0x12));
        /* 0x13 */ fprintf(fp, "unit[%d].pad2B_13h = %d\n",       itr, rd_u8(sav,  base + 0x13));
        /* 0x14 */ fprintf(fp, "unit[%d].in_tower = %d\n",        itr, rd_i16(sav, base + 0x14));
        /* 0x16 */ fprintf(fp, "unit[%d].sight_range = %d\n",     itr, rd_i8(sav,  base + 0x16));
        /* 0x17 */ fprintf(fp, "unit[%d].mutations = %d\n",       itr, rd_i8(sav,  base + 0x17));
        /* 0x18 */ fprintf(fp, "unit[%d].enchantments = 0x%08X\n", itr, rd_u32(sav, base + 0x18));
        /* 0x1C */ fprintf(fp, "unit[%d].rd_constr_left = %d\n",  itr, rd_i8(sav,  base + 0x1C));
        /* 0x1D */ fprintf(fp, "unit[%d].rd_from_x = %d\n",      itr, rd_i8(sav,  base + 0x1D));
        /* 0x1E */ fprintf(fp, "unit[%d].rd_from_y = %d\n",      itr, rd_i8(sav,  base + 0x1E));
        /* 0x1F */ fprintf(fp, "unit[%d].pad2B_1Fh = %d\n",       itr, rd_u8(sav,  base + 0x1F));
    }
}

static void Dump_Map_Array(FILE *fp, const char *name, const uint8_t *sav, int offset)
{
    int plane, sq;
    for (plane = 0; plane < SD_NUM_PLANES; plane++)
    {
        for (sq = 0; sq < SD_MAP_SIZE; sq++)
        {
            fprintf(fp, "%s[%d][%d] = %d\n", name, plane, sq, rd_u8(sav, offset + (plane * SD_MAP_SIZE) + sq));
        }
    }
}

static void Dump_Tail(FILE *fp, const uint8_t *sav)
{
    int itr;
    char name[15];

    /* Events table (100 bytes) */
    for (itr = 0; itr < 100; itr++)
    {
        fprintf(fp, "events[%d] = %d\n", itr, rd_u8(sav, SD_OFF_EVENTS + itr));
    }

    /* Grand vizier */
    fprintf(fp, "grand_vizier = %d\n", rd_u16(sav, SD_OFF_VIZIER));

    /* Premade items table (250 bytes) */
    for (itr = 0; itr < 250; itr++)
    {
        fprintf(fp, "premade_items[%d] = %d\n", itr, rd_u8(sav, SD_OFF_PREMADE + itr));
    }

    /* Hero names table */
    for (itr = 0; itr < SD_NUM_HERO_TYPES; itr++)
    {
        int base = SD_OFF_HERONAMES + (itr * SD_INACTV_HERO_SIZE);
        rd_str(sav, base, 14, name);
        fprintf(fp, "hero_name[%d].name = %s\n", itr, name);
        fprintf(fp, "hero_name[%d].xp = %d\n", itr, rd_i16(sav, base + 14));
    }
}



/* ========================================================================= */
/*  Public API                                                               */
/* ========================================================================= */

int HeMoM_Save_Dump(const char *save_filepath, const char *text_filepath)
{
    FILE *fin;
    FILE *fout;
    uint8_t sav[SD_TOTAL_SIZE];
    size_t bytes_read;

    fin = stu_fopen_ci(save_filepath, "rb");
    if (fin == NULL)
    {
        fprintf(stderr, "[HeMoM SaveDump] Could not open: %s\n", save_filepath);
        return 1;
    }

    bytes_read = fread(sav, 1, SD_TOTAL_SIZE, fin);
    fclose(fin);

    if (bytes_read != SD_TOTAL_SIZE)
    {
        fprintf(stderr, "[HeMoM SaveDump] Short read: %zu bytes (expected %d) from %s\n", bytes_read, SD_TOTAL_SIZE, save_filepath);
        return 1;
    }

    fout = fopen(text_filepath, "w");
    if (fout == NULL)
    {
        fprintf(stderr, "[HeMoM SaveDump] Could not create: %s\n", text_filepath);
        return 1;
    }

    fprintf(fout, "# HeMoM Save Dump\n");
    fprintf(fout, "# Source: %s\n\n", save_filepath);

    Dump_Heroes(fout, sav);
    fprintf(fout, "\n");

    Dump_Game(fout, sav);
    fprintf(fout, "\n");

    Dump_Players(fout, sav);
    fprintf(fout, "\n");

    Dump_World_Maps(fout, sav);
    fprintf(fout, "\n");

    Dump_Nodes(fout, sav);
    fprintf(fout, "\n");

    Dump_Fortresses(fout, sav);
    fprintf(fout, "\n");

    Dump_Towers(fout, sav);
    fprintf(fout, "\n");

    Dump_Lairs(fout, sav);
    fprintf(fout, "\n");

    Dump_Items(fout, sav);
    fprintf(fout, "\n");

    Dump_Cities(fout, sav);
    fprintf(fout, "\n");

    Dump_Units(fout, sav);
    fprintf(fout, "\n");

    Dump_Map_Array(fout, "terrain_special", sav, SD_OFF_TERRAIN);
    fprintf(fout, "\n");

    Dump_Map_Array(fout, "explored", sav, SD_OFF_EXPLORED);
    fprintf(fout, "\n");

    /* Movement cost maps: 2 planes * 14400 bytes = 28800 bytes.
       Too large to dump per-byte — dump as checksum per plane per mode. */
    {
        int plane, mode, sq;
        int mode_size = SD_MAP_SIZE;
        int modes_per_plane = 6;  /* UU_MvMd, walking, forester, mountaineer, swimming, sailing */
        static const char *mode_names[] = { "uu", "walking", "forester", "mountaineer", "swimming", "sailing" };
        for (plane = 0; plane < SD_NUM_PLANES; plane++)
        {
            for (mode = 0; mode < modes_per_plane; mode++)
            {
                uint32_t checksum = 0;
                int base = SD_OFF_MOVEMENT + (plane * modes_per_plane * mode_size) + (mode * mode_size);
                for (sq = 0; sq < mode_size; sq++)
                {
                    checksum += rd_u8(sav, base + sq);
                }
                fprintf(fout, "movement[%d].%s.checksum = %u\n", plane, mode_names[mode], checksum);
            }
        }
    }
    fprintf(fout, "\n");

    Dump_Map_Array(fout, "map_flags", sav, SD_OFF_FLAGS);
    fprintf(fout, "\n");

    Dump_Tail(fout, sav);

    fclose(fout);

    fprintf(stderr, "[HeMoM SaveDump] Wrote: %s\n", text_filepath);
    return 0;
}
