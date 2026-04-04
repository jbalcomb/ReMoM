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

static void Dump_Players(FILE *fp, const uint8_t *sav)
{
    int itr, base, r;
    char name[21];
    for (itr = 0; itr < SD_NUM_PLAYERS; itr++)
    {
        base = SD_OFF_PLAYERS + (itr * SD_WIZARD_SIZE);
        fprintf(fp, "player[%d].wizard_id = %d\n", itr, rd_u8(sav, base + 0));
        rd_str(sav, base + 1, 20, name);
        fprintf(fp, "player[%d].name = %s\n", itr, name);
        fprintf(fp, "player[%d].capital_race = %d\n", itr, rd_u8(sav, base + 0x15));
        fprintf(fp, "player[%d].banner_id = %d\n", itr, rd_u8(sav, base + 0x16));
        fprintf(fp, "player[%d].personality = %d\n", itr, rd_u16(sav, base + 0x18));
        fprintf(fp, "player[%d].objective = %d\n", itr, rd_u16(sav, base + 0x1A));
        fprintf(fp, "player[%d].fame = %d\n", itr, rd_u16(sav, base + 0x24));
        fprintf(fp, "player[%d].power_base = %d\n", itr, rd_u16(sav, base + 0x26));
        fprintf(fp, "player[%d].tax_rate = %d\n", itr, rd_u16(sav, base + 0x58));
        /* Spellranks: Nature, Sorcery, Chaos, Life, Death */
        fprintf(fp, "player[%d].spellranks[sbr_Nature] = %d\n", itr, rd_i16(sav, base + 0x5A));
        fprintf(fp, "player[%d].spellranks[sbr_Sorcery] = %d\n", itr, rd_i16(sav, base + 0x5C));
        fprintf(fp, "player[%d].spellranks[sbr_Chaos] = %d\n", itr, rd_i16(sav, base + 0x5E));
        fprintf(fp, "player[%d].spellranks[sbr_Life] = %d\n", itr, rd_i16(sav, base + 0x60));
        fprintf(fp, "player[%d].spellranks[sbr_Death] = %d\n", itr, rd_i16(sav, base + 0x62));
        /* Retorts */
        for (r = 0; r < SD_NUM_RETORTS; r++)
        {
            fprintf(fp, "player[%d].retort[%d] = %d\n", itr, r, rd_i8(sav, base + 0x64 + r));
        }
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

static void Dump_Cities(FILE *fp, const uint8_t *sav)
{
    int itr, base, b;
    char name[15];
    for (itr = 0; itr < SD_NUM_CITIES; itr++)
    {
        base = SD_OFF_CITIES + (itr * SD_CITY_SIZE);
        rd_str(sav, base, 14, name);
        fprintf(fp, "city[%d].name = %s\n", itr, name);
        fprintf(fp, "city[%d].race = %d\n", itr, rd_i8(sav, base + 0x0E));
        fprintf(fp, "city[%d].wx = %d\n", itr, rd_i8(sav, base + 0x0F));
        fprintf(fp, "city[%d].wy = %d\n", itr, rd_i8(sav, base + 0x10));
        fprintf(fp, "city[%d].wp = %d\n", itr, rd_i8(sav, base + 0x11));
        fprintf(fp, "city[%d].owner_idx = %d\n", itr, rd_i8(sav, base + 0x12));
        fprintf(fp, "city[%d].size = %d\n", itr, rd_i8(sav, base + 0x13));
        fprintf(fp, "city[%d].population = %d\n", itr, rd_i8(sav, base + 0x14));
        fprintf(fp, "city[%d].farmer_count = %d\n", itr, rd_i8(sav, base + 0x15));
        fprintf(fp, "city[%d].construction = %d\n", itr, rd_i16(sav, base + 0x1C));
        fprintf(fp, "city[%d].bldg_cnt = %d\n", itr, rd_i8(sav, base + 0x1E));
        for (b = 0; b < SD_NUM_BUILDINGS; b++)
        {
            fprintf(fp, "city[%d].bldg_status[%d] = %d\n", itr, b, rd_i8(sav, base + 0x1F + b));
        }
    }
}

static void Dump_Units(FILE *fp, const uint8_t *sav)
{
    int itr, base;
    for (itr = 0; itr < SD_NUM_UNITS; itr++)
    {
        base = SD_OFF_UNITS + (itr * SD_UNIT_SIZE);
        fprintf(fp, "unit[%d].wx = %d\n", itr, rd_i8(sav, base + 0));
        fprintf(fp, "unit[%d].wy = %d\n", itr, rd_i8(sav, base + 1));
        fprintf(fp, "unit[%d].wp = %d\n", itr, rd_i8(sav, base + 2));
        fprintf(fp, "unit[%d].owner_idx = %d\n", itr, rd_i8(sav, base + 3));
        fprintf(fp, "unit[%d].type = %d\n", itr, rd_u8(sav, base + 5));
        fprintf(fp, "unit[%d].hero_slot = %d\n", itr, rd_i8(sav, base + 6));
        fprintf(fp, "unit[%d].finished = %d\n", itr, rd_i8(sav, base + 7));
        fprintf(fp, "unit[%d].moves2 = %d\n", itr, rd_i8(sav, base + 8));
        fprintf(fp, "unit[%d].status = %d\n", itr, rd_i8(sav, base + 0x0B));
        fprintf(fp, "unit[%d].level = %d\n", itr, rd_i8(sav, base + 0x0C));
        fprintf(fp, "unit[%d].enchantments = 0x%08X\n", itr, rd_u32(sav, base + 0x18));
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
