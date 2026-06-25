/*
    MAGIC.EXE
    ovr056

MoO2
Module: INITGAME
*/

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"  /* WORLD_SIZE, WORLD_OVERFLOW, NUM_PLANES (CI overrun inject) */
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/random.h"

#include "../../STU/src/STU_LOG.h"  /* Init_Heroes Spells write trace */

#include "NewGame.h"
#include "Spellbook.h"
#include "UNITTYPE.h"

#include "INITGAME.h"

#include "../../ext/stu_compat.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>  /* getenv, strtol (CI loader) */

/* ==================================================================== *
 *  Capture/Inject (CI), ReMoM side -- see INITGAME.h.
 * ==================================================================== *
 *  Loads og-game-data-capture.fwv (extract-ci-stage0.py output) and hands
 *  OG's captured values to the call sites that inject them.  The file lives in
 *  the ReMoM build's working dir; ReMoMber may run from assets/ or the build
 *  dir, so we try a few candidate paths (plus an OG_CI_FWV override) and log
 *  loudly rather than fail silently -- a missing file just means "no inject". */

#define GD_CI_MAX_VALS  512   /* longest record is the 240-int16 world overrun */
#define GD_CI_MAX_RECS   16

typedef struct {
    char key[32];
    char site[24];
    int  count;
    long vals[GD_CI_MAX_VALS];
} gd_ci_rec_t;

static gd_ci_rec_t gd_ci_recs[GD_CI_MAX_RECS];
static int gd_ci_nrecs  = 0;
static int gd_ci_loaded = 0;   /* 0 = not tried, 1 = loaded, -1 = tried + failed */

int gd_ci_load(void)
{
    const char* cands[3];
    const char* env;
    const char* used = 0;
    FILE* f = 0;
    char  line[8192];
    int   nc = 0, i;

    if (gd_ci_loaded != 0) {
        return (gd_ci_loaded == 1) ? gd_ci_nrecs : -1;
    }

    env = getenv("OG_CI_FWV");
    if (env && *env) { cands[nc++] = env; }
    cands[nc++] = "og-game-data-capture.fwv";   /* cwd (build dir, if run there) */
    cands[nc++] = "/home/jbalcomb/STU/devel/ReMoM/out/build/clang-debug/bin/Debug/og-game-data-capture.fwv";

    for (i = 0; i < nc; i++) {
        f = fopen(cands[i], "r");
        if (f) { used = cands[i]; break; }
    }
    if (!f) {
        LOG_INFO(LOG_CAT_GENERAL,
            "[CI] load FAILED: og-game-data-capture.fwv not found (cwd or build dir). No injection.");
        gd_ci_loaded = -1;
        return -1;
    }

    gd_ci_nrecs = 0;
    while (fgets(line, sizeof(line), f)) {
        char  key[64], site[64];
        int   count = 0, consumed = 0, n = 0;
        char* p = line;
        char* endp;
        gd_ci_rec_t* r;

        while (*p == ' ' || *p == '\t') { p++; }
        if (*p == '#' || *p == '\n' || *p == '\r' || *p == 0) { continue; }
        if (gd_ci_nrecs >= GD_CI_MAX_RECS) {
            LOG_INFO(LOG_CAT_GENERAL, "[CI] WARNING: >%d records; rest ignored.", GD_CI_MAX_RECS);
            break;
        }
        if (sscanf(p, "%63s %63s %d%n", key, site, &count, &consumed) != 3) {
            LOG_INFO(LOG_CAT_GENERAL, "[CI] WARNING: malformed record skipped.");
            continue;
        }
        r = &gd_ci_recs[gd_ci_nrecs];
        /* CLAUDE 2026-06-24: snprintf in lieu of strncpy + manual NUL —
           always NUL-terminates, sidesteps GCC -Wstringop-truncation. */
        snprintf(r->key,  sizeof(r->key),  "%s", key);
        snprintf(r->site, sizeof(r->site), "%s", site);

        p += consumed;
        for (;;) {
            long v = strtol(p, &endp, 10);
            if (endp == p) { break; }       /* no more numbers on the line */
            if (n < GD_CI_MAX_VALS) { r->vals[n] = v; }
            n++;
            p = endp;
        }
        if (n != count) {
            LOG_INFO(LOG_CAT_GENERAL,
                "[CI] WARNING: %s %s declared %d values but found %d.", r->key, r->site, count, n);
        }
        r->count = (n < GD_CI_MAX_VALS) ? n : GD_CI_MAX_VALS;
        gd_ci_nrecs++;
    }
    fclose(f);

    LOG_INFO(LOG_CAT_GENERAL, "[CI] loaded %d record(s) from %s", gd_ci_nrecs, used);
    gd_ci_loaded = 1;
    return gd_ci_nrecs;
}

int gd_ci_get(const char* key, const char* site, long* out, int max)
{
    int i, k, n;
    if (gd_ci_loaded == 0) { gd_ci_load(); }
    for (i = 0; i < gd_ci_nrecs; i++) {
        if (strcmp(gd_ci_recs[i].key, key) == 0 &&
            strcmp(gd_ci_recs[i].site, site) == 0) {
            n = gd_ci_recs[i].count;
            if (n > max) { n = max; }
            for (k = 0; k < n; k++) { out[k] = gd_ci_recs[i].vals[k]; }
            return n;
        }
    }
    return -1;
}

void gd_ci_inject_world_overrun(const char* site)
{
    long     vals[WORLD_OVERFLOW];
    int16_t* dst;
    int      n, i;

    if (!_world_maps) {
        LOG_INFO(LOG_CAT_GENERAL, "[CI] inject overrun (%s): _world_maps NULL -- skipped.", site);
        return;
    }
    n = gd_ci_get("world_maps_overrun", site, vals, WORLD_OVERFLOW);
    if (n < 0) {
        LOG_INFO(LOG_CAT_GENERAL, "[CI] inject overrun (%s): no record -- skipped.", site);
        return;
    }
    /* OG-faithful OOB target: one past the last valid cell, p_world_map[1][40][0]. */
    dst = (int16_t*)_world_maps + (NUM_PLANES * WORLD_SIZE);
    for (i = 0; i < n; i++) { dst[i] = (int16_t)vals[i]; }
    LOG_INFO(LOG_CAT_GENERAL,
        "[CI] inject overrun (%s): wrote %d int16 at _world_maps+%d.",
        site, n, (int)(NUM_PLANES * WORLD_SIZE));
}

/* ---- Game-data capture (CaptureGameData PRD/PLAN, Phase 4, ReMoM side) ----
 * Dump _players in the SAME [GD] format as the OG-side STU-DOSBox probe,
 * driven by the SAME generated field map (gd_wizard_fields.h from
 * gen-fieldmap.py), so the two logs diff field-for-field. Decode mirrors the
 * OG gd_decode_record exactly. */
enum { GD_U8 = 0, GD_I8, GD_U16, GD_I16, GD_U32, GD_I32, GD_STR, GD_BYTES };
typedef struct { uint16_t off; uint8_t kind; uint16_t count; const char* name; } gd_field_t;
#include "gd_wizard_fields.h"
#include "gd_node_fields.h"
#include "gd_tower_fields.h"
#include "gd_lair_fields.h"
#include "gd_city_fields.h"
#include "gd_unit_fields.h"
#include "gd_heroes_fields.h"
#include "gd_item_fields.h"

static int gd_es(int kind) {
    return (kind == GD_U16 || kind == GD_I16) ? 2
         : (kind == GD_U32 || kind == GD_I32) ? 4 : 1;
}
static long gd_rd(const uint8_t* r, int kind) {
    switch (kind) {
        case GD_I8:  return (long)(int8_t)r[0];
        case GD_U16: return (long)(uint16_t)(r[0] | (r[1] << 8));
        case GD_I16: return (long)(int16_t)(uint16_t)(r[0] | (r[1] << 8));
        case GD_U32: return (long)((uint32_t)r[0] | ((uint32_t)r[1] << 8)
                                 | ((uint32_t)r[2] << 16) | ((uint32_t)r[3] << 24));
        case GD_I32: return (long)(int32_t)((uint32_t)r[0] | ((uint32_t)r[1] << 8)
                                 | ((uint32_t)r[2] << 16) | ((uint32_t)r[3] << 24));
        default:     return (long)r[0];   /* U8 */
    }
}
void gd_dump_players(const char* point) {
    int p, i, k;
    char val[1100];
    for (p = 0; p < _num_players; p++) {
        const uint8_t* base = (const uint8_t*)&_players[p];
        for (i = 0; i < WIZARD_FIELD_COUNT; i++) {
            const gd_field_t* f = &wizard_fields[i];
            const uint8_t* fp = base + f->off;
            int q = 0;
            if(f->kind == GD_STR) {
                val[q++] = '"';
                for (k = 0; k < f->count && q < (int)sizeof(val) - 2; k++) {
                    uint8_t c = fp[k];
                    if(c == 0) break;
                    val[q++] = (c >= 32 && c < 127) ? (char)c : '.';
                }
                val[q++] = '"'; val[q] = 0;
            } else if(f->kind == GD_BYTES) {
                for (k = 0; k < f->count && q < (int)sizeof(val) - 3; k++)
                    q += snprintf(val + q, sizeof(val) - q, "%02X", fp[k]);
                val[q] = 0;
            } else {
                int es = gd_es(f->kind);
                for (k = 0; k < f->count; k++) {
                    long v = gd_rd(fp + k * es, f->kind);
                    q += snprintf(val + q, sizeof(val) - q, k ? ",%ld" : "%ld", v);
                    if(q > (int)sizeof(val) - 16) break;
                }
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _players[%d].%s = %s",
                      point, p, f->name, val);
        }
        /* gd_dump_players emits a large [GD] burst in a tight loop with no
         * main-loop pump in between, which overruns the async logger's ring
         * and drops messages.  Drain it per player so nothing is lost. */
        STU_Log_Flush_All();
    }
}

/* _NODES capture: mirror of gd_dump_players for the s_NODE array (node_fields).
 * Emits [GD] <point> _NODES[n].<field> = <value> for byte-compare vs OG. */
void gd_dump_nodes(const char* point) {
    int n, i, k;
    char val[1100];
    for (n = 0; n < NUM_NODES; n++) {
        const uint8_t* base = (const uint8_t*)&_NODES[n];
        for (i = 0; i < NODE_FIELD_COUNT; i++) {
            const gd_field_t* f = &node_fields[i];
            const uint8_t* fp = base + f->off;
            int q = 0;
            if(f->kind == GD_STR) {
                val[q++] = '"';
                for (k = 0; k < f->count && q < (int)sizeof(val) - 2; k++) {
                    uint8_t c = fp[k];
                    if(c == 0) break;
                    val[q++] = (c >= 32 && c < 127) ? (char)c : '.';
                }
                val[q++] = '"'; val[q] = 0;
            } else if(f->kind == GD_BYTES) {
                for (k = 0; k < f->count && q < (int)sizeof(val) - 3; k++)
                    q += snprintf(val + q, sizeof(val) - q, "%02X", fp[k]);
                val[q] = 0;
            } else {
                int es = gd_es(f->kind);
                for (k = 0; k < f->count; k++) {
                    long v = gd_rd(fp + k * es, f->kind);
                    q += snprintf(val + q, sizeof(val) - q, k ? ",%ld" : "%ld", v);
                    if(q > (int)sizeof(val) - 16) break;
                }
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _NODES[%d].%s = %s",
                      point, n, f->name, val);
        }
        STU_Log_Flush_All();
    }
}

/* _TOWERS capture: mirror of gd_dump_nodes for the s_TOWER array (tower_fields). */
void gd_dump_towers(const char* point) {
    int n, i, k;
    char val[1100];
    for (n = 0; n < NUM_TOWERS; n++) {
        const uint8_t* base = (const uint8_t*)&_TOWERS[n];
        for (i = 0; i < TOWER_FIELD_COUNT; i++) {
            const gd_field_t* f = &tower_fields[i];
            const uint8_t* fp = base + f->off;
            int q = 0;
            if(f->kind == GD_STR) {
                val[q++] = '"';
                for (k = 0; k < f->count && q < (int)sizeof(val) - 2; k++) {
                    uint8_t c = fp[k];
                    if(c == 0) break;
                    val[q++] = (c >= 32 && c < 127) ? (char)c : '.';
                }
                val[q++] = '"'; val[q] = 0;
            } else if(f->kind == GD_BYTES) {
                for (k = 0; k < f->count && q < (int)sizeof(val) - 3; k++)
                    q += snprintf(val + q, sizeof(val) - q, "%02X", fp[k]);
                val[q] = 0;
            } else {
                int es = gd_es(f->kind);
                for (k = 0; k < f->count; k++) {
                    long v = gd_rd(fp + k * es, f->kind);
                    q += snprintf(val + q, sizeof(val) - q, k ? ",%ld" : "%ld", v);
                    if(q > (int)sizeof(val) - 16) break;
                }
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _TOWERS[%d].%s = %s",
                      point, n, f->name, val);
        }
        STU_Log_Flush_All();
    }
}

void gd_dump_lairs(const char* point) {
    int n, i, k;
    char val[1100];
    for (n = 0; n < NUM_LAIRS; n++) {
        const uint8_t* base = (const uint8_t*)&_LAIRS[n];
        for (i = 0; i < LAIR_FIELD_COUNT; i++) {
            const gd_field_t* f = &lair_fields[i];
            const uint8_t* fp = base + f->off;
            int q = 0;
            if(f->kind == GD_STR) {
                val[q++] = '"';
                for (k = 0; k < f->count && q < (int)sizeof(val) - 2; k++) {
                    uint8_t c = fp[k];
                    if(c == 0) break;
                    val[q++] = (c >= 32 && c < 127) ? (char)c : '.';
                }
                val[q++] = '"'; val[q] = 0;
            } else if(f->kind == GD_BYTES) {
                for (k = 0; k < f->count && q < (int)sizeof(val) - 3; k++)
                    q += snprintf(val + q, sizeof(val) - q, "%02X", fp[k]);
                val[q] = 0;
            } else {
                int es = gd_es(f->kind);
                for (k = 0; k < f->count; k++) {
                    long v = gd_rd(fp + k * es, f->kind);
                    q += snprintf(val + q, sizeof(val) - q, k ? ",%ld" : "%ld", v);
                    if(q > (int)sizeof(val) - 16) break;
                }
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _LAIRS[%d].%s = %s",
                      point, n, f->name, val);
        }
        STU_Log_Flush_All();
    }
}

void gd_dump_cities(const char* point) {
    int n, i, k;
    char val[1100];
    /* placed-city count -> compare-gd masks _CITIES[n >= _cities] (unplaced tail). */
    LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _cities = %d", point, (int)_cities);
    for (n = 0; n < NUM_CITIES; n++) {
        const uint8_t* base = (const uint8_t*)&_CITIES[n];
        for (i = 0; i < CITY_FIELD_COUNT; i++) {
            const gd_field_t* f = &city_fields[i];
            const uint8_t* fp = base + f->off;
            int q = 0;
            if (f->kind == GD_STR) {
                val[q++] = '"';
                for (k = 0; k < f->count && q < (int)sizeof(val) - 2; k++) {
                    uint8_t c = fp[k];
                    if (c == 0) break;
                    val[q++] = (c >= 32 && c < 127) ? (char)c : '.';
                }
                val[q++] = '"'; val[q] = 0;
            } else if (f->kind == GD_BYTES) {
                for (k = 0; k < f->count && q < (int)sizeof(val) - 3; k++)
                    q += snprintf(val + q, sizeof(val) - q, "%02X", fp[k]);
                val[q] = 0;
            } else {
                int es = gd_es(f->kind);
                for (k = 0; k < f->count; k++) {
                    long v = gd_rd(fp + k * es, f->kind);
                    q += snprintf(val + q, sizeof(val) - q, k ? ",%ld" : "%ld", v);
                    if (q > (int)sizeof(val) - 16) break;
                }
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _CITIES[%d].%s = %s",
                      point, n, f->name, val);
        }
        STU_Log_Flush_All();
    }
}

/* _HEROES2 capture: per-player heroes.  _HEROES2 is an array of NUM_PLAYERS
 * pointers (one per player) to a s_HEROES { s_HERO heroes[NUM_HERO_TYPES]; }, so
 * iterate players, dereference each pointer, and dump the flattened heroes_fields
 * (heroes[0..34].<field>).  One [GD] line per (player, field):
 *   "[GD] <point> _HEROES2[<player>].<field> = <value>". */
void gd_dump_heroes(const char* point) {
    int p, i, k;
    char val[1100];
    for (p = 0; p < NUM_PLAYERS; p++) {
        const uint8_t* base = (const uint8_t*)_HEROES2[p];
        if (!base) continue;   /* unallocated player slot */
        for (i = 0; i < HEROES_FIELD_COUNT; i++) {
            const gd_field_t* f = &heroes_fields[i];
            const uint8_t* fp = base + f->off;
            int q = 0;
            if (f->kind == GD_STR) {
                val[q++] = '"';
                for (k = 0; k < f->count && q < (int)sizeof(val) - 2; k++) {
                    uint8_t c = fp[k];
                    if (c == 0) break;
                    val[q++] = (c >= 32 && c < 127) ? (char)c : '.';
                }
                val[q++] = '"'; val[q] = 0;
            } else if (f->kind == GD_BYTES) {
                for (k = 0; k < f->count && q < (int)sizeof(val) - 3; k++)
                    q += snprintf(val + q, sizeof(val) - q, "%02X", fp[k]);
                val[q] = 0;
            } else {
                int es = gd_es(f->kind);
                for (k = 0; k < f->count; k++) {
                    long v = gd_rd(fp + k * es, f->kind);
                    q += snprintf(val + q, sizeof(val) - q, k ? ",%ld" : "%ld", v);
                    if (q > (int)sizeof(val) - 16) break;
                }
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _HEROES2[%d].%s = %s",
                      point, p, f->name, val);
        }
        STU_Log_Flush_All();
    }
}

/* _UNITS capture: mirror of gd_dump_cities for the s_UNIT array (unit_fields).
 * Dumps the full NUM_UNITS array; slots past the active-unit count (_units) are
 * uninitialised, so the active count is emitted first for compare-gd to mask
 * the inactive tail (_UNITS[n >= _units]). */
void gd_dump_units(const char* point) {
    int n, i, k;
    char val[1100];
    /* active-unit count -> compare-gd masks _UNITS[n >= _units] (inactive tail). */
    LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _units = %d", point, (int)_units);
    for (n = 0; n < NUM_UNITS; n++) {
        const uint8_t* base = (const uint8_t*)&_UNITS[n];
        for (i = 0; i < UNIT_FIELD_COUNT; i++) {
            const gd_field_t* f = &unit_fields[i];
            const uint8_t* fp = base + f->off;
            int q = 0;
            if (f->kind == GD_STR) {
                val[q++] = '"';
                for (k = 0; k < f->count && q < (int)sizeof(val) - 2; k++) {
                    uint8_t c = fp[k];
                    if (c == 0) break;
                    val[q++] = (c >= 32 && c < 127) ? (char)c : '.';
                }
                val[q++] = '"'; val[q] = 0;
            } else if (f->kind == GD_BYTES) {
                for (k = 0; k < f->count && q < (int)sizeof(val) - 3; k++)
                    q += snprintf(val + q, sizeof(val) - q, "%02X", fp[k]);
                val[q] = 0;
            } else {
                int es = gd_es(f->kind);
                for (k = 0; k < f->count; k++) {
                    long v = gd_rd(fp + k * es, f->kind);
                    q += snprintf(val + q, sizeof(val) - q, k ? ",%ld" : "%ld", v);
                    if (q > (int)sizeof(val) - 16) break;
                }
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _UNITS[%d].%s = %s",
                      point, n, f->name, val);
        }
        STU_Log_Flush_All();
    }
}

/* _ITEMS capture: mirror of gd_dump_cities for the s_ITEM array (item_fields).
 * _ITEMS is a single pointer to NUM_ITEMS contiguous s_ITEM records, all loaded
 * from ITEMDATA.LBX by Initialize_Items, so the whole array is compared -- no
 * placement-count tail to mask. */
void gd_dump_items(const char* point) {
    int n, i, k;
    char val[1100];
    for (n = 0; n < NUM_ITEMS; n++) {
        const uint8_t* base = (const uint8_t*)&_ITEMS[n];
        for (i = 0; i < ITEM_FIELD_COUNT; i++) {
            const gd_field_t* f = &item_fields[i];
            const uint8_t* fp = base + f->off;
            int q = 0;
            if (f->kind == GD_STR) {
                val[q++] = '"';
                for (k = 0; k < f->count && q < (int)sizeof(val) - 2; k++) {
                    uint8_t c = fp[k];
                    if (c == 0) break;
                    val[q++] = (c >= 32 && c < 127) ? (char)c : '.';
                }
                val[q++] = '"'; val[q] = 0;
            } else if (f->kind == GD_BYTES) {
                for (k = 0; k < f->count && q < (int)sizeof(val) - 3; k++)
                    q += snprintf(val + q, sizeof(val) - q, "%02X", fp[k]);
                val[q] = 0;
            } else {
                int es = gd_es(f->kind);
                for (k = 0; k < f->count; k++) {
                    long v = gd_rd(fp + k * es, f->kind);
                    q += snprintf(val + q, sizeof(val) - q, k ? ",%ld" : "%ld", v);
                    if (q > (int)sizeof(val) - 16) break;
                }
            }
            LOG_DEBUG(LOG_CAT_GENERAL, "[GD] %s _ITEMS[%d].%s = %s",
                      point, n, f->name, val);
        }
        STU_Log_Flush_All();
    }
}
/* ---- end game-data capture ---- */



// MGC  dseg:2AD0
extern struct s_WIZARD_PRESET _wizard_presets_table[15];

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
extern struct s_Init_Base_Realms _player_start_spells[PLAYER_COUNT_MAX];

// o51p36
void Draw_Building_The_Worlds(int16_t percent);



// MGC o56p1
/**
 * @brief Initializes AI player profiles and their starting spell libraries.
 *
 * @details
 * Builds non-human wizard setup data for a new game by first generating AI
 * wizard profiles, then copying default realm spell selections into
 * `_player_start_spells` for each computer player, and finally creating each
 * AI wizard's known/knowable spell library.
 *
 * The neutral player record is also initialized with baseline values used by
 * neutral units/cities in world simulation.
 *
 * Side effects include mutation of global game-state tables such as
 * `_players[]` and `_player_start_spells[]`.
 *
 * @note Player index 0 (human) is skipped in the AI spell-copy loop; AI
 *       initialization begins at player index 1 and runs to `_num_players - 1`.
 *
 * @return void
 *
 * @see Init_Computer_Players_Wizard_Profile
 * @see Init_Computer_Players_Spell_Library
 */
void Init_Computer_Players(void)
{
    int16_t itr_players = 0;
    int16_t itr2 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    Init_Computer_Players_Wizard_Profile();
    gd_dump_players("100_Wizard_Profile_P");

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: _players[%d].spellranks[]: N%d, S%d, C%d, L%d, D%d",
                itr_players,
                _players[itr_players].spellranks[sbr_Nature],
                _players[itr_players].spellranks[sbr_Sorcery],
                _players[itr_players].spellranks[sbr_Chaos],
                _players[itr_players].spellranks[sbr_Life],
                _players[itr_players].spellranks[sbr_Death]);
    }
    // HERE:  We have logged and verified the spellranks values betwen OG-MoM and ReMoM, 100% match.

    for(itr2 = 0; itr2 < 13; itr2++)
    {
        LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: _default_spells[%d]: N%d, S%d, C%d, L%d, D%d",
                itr2,
                _default_spells[sbr_Nature].spells[itr2],
                _default_spells[sbr_Sorcery].spells[itr2],
                _default_spells[sbr_Chaos].spells[itr2],
                _default_spells[sbr_Life].spells[itr2],
                _default_spells[sbr_Death].spells[itr2]);
    }

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {

        for(itr2 = 0; itr2 < 13; itr2++)
        {

            _player_start_spells[itr_players].realms[sbr_Nature].spells[itr2]   = _default_spells[sbr_Nature].spells[itr2];
            _player_start_spells[itr_players].realms[sbr_Sorcery].spells[itr2]  = _default_spells[sbr_Sorcery].spells[itr2];
            _player_start_spells[itr_players].realms[sbr_Chaos].spells[itr2]    = _default_spells[sbr_Chaos].spells[itr2];
            _player_start_spells[itr_players].realms[sbr_Life].spells[itr2]     = _default_spells[sbr_Life].spells[itr2];
            _player_start_spells[itr_players].realms[sbr_Death].spells[itr2]    = _default_spells[sbr_Death].spells[itr2];

        }

    }

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        for(itr2 = 0; itr2 < 13; itr2++)
        {
            LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: _player_start_spells[%d]: N%d, S%d, C%d, L%d, D%d",
                    itr_players,
                    _player_start_spells[itr_players].realms[sbr_Nature].spells[itr2],
                    _player_start_spells[itr_players].realms[sbr_Sorcery].spells[itr2],
                    _player_start_spells[itr_players].realms[sbr_Chaos].spells[itr2],
                    _player_start_spells[itr_players].realms[sbr_Life].spells[itr2],
                    _player_start_spells[itr_players].realms[sbr_Death].spells[itr2]);
        }
    }

    Init_Computer_Players_Spell_Library();
    gd_dump_players("101_Spell_Library_P");

    _players[NEUTRAL_PLAYER_IDX].Defeated_Wizards = 0;

    _players[NEUTRAL_PLAYER_IDX].average_unit_cost = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_Computer_Players rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p2
/* COPILOT */
/**
 * @brief Finalizes per-player and global runtime state for a new game.
 *
 * @details
 * Runs the late initialization pass after the world, cities, and fortress
 * records already exist. The routine advances the world-building progress
 * display, clears item state, initializes hero data, resets player runtime
 * fields, seeds AI strategy timers, derives AI personalities/objectives,
 * copies fortress coordinates into each player's summoning-circle state,
 * clears message counters, and computes initial wizard-to-wizard diplomatic
 * relations.
 *
 * It then derives each active player's `capital_race` from the matching entry
 * in `_CITIES[]`, assigns the human player's starting gold from `_difficulty`,
 * and gives each AI player a fixed opening treasury plus an initial
 * `AI_SCircle_Reevals[]` delay randomized in the range `[80, 119]`.
 *
 * @return void
 *
 * @note Expects `_num_players`, `_players[]`, `_CITIES[]`, and fortress data
 *       to have already been populated by earlier new-game setup.
 * @note Mutates several global runtime tables as part of the final startup
 *       pass.
 *
 * @see Draw_Building_The_Worlds
 * @see Initialize_Items
 * @see Init_Heroes
 * @see Init_Players
 * @see Init_CP_Strategy
 * @see Init_Magic_Personalities_Objectives
 * @see Init_Summoning_Circle_And_Spell_Of_Mastery
 * @see Initialize_Messages
 * @see Init_Diplomatic_Relations
 */
void Init_Runtime(void)
{
    int16_t itr_players = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    Draw_Building_The_Worlds(100);

    Initialize_Items();
    gd_dump_items("200_Initialize_Items_I");

    Init_Heroes();
    gd_dump_players("201_Init_Heroes_P");
    gd_dump_heroes("202_Init_Heroes_HE");

    Init_Players();
    gd_dump_players("203_Init_Players_P");

    Init_CP_Strategy();
    gd_dump_players("204_Init_CP_Strategy_P");

    Init_Magic_Personalities_Objectives();
    gd_dump_players("205_Init_Magic_Pers_Obj_P");

    Init_Summoning_Circle_And_Spell_Of_Mastery();
    gd_dump_players("206_Init_Summoning_Circle_P");

    Initialize_Messages();

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].capital_race = _CITIES[itr_players].race;
    }
    gd_dump_players("207_capital_race_P");  /* at-entry Init_Diplomatic_Relations() */

    Init_Diplomatic_Relations();
    gd_dump_players("208_Init_Diplomatic_Relations_P");

    _players[HUMAN_PLAYER_IDX].gold_reserve = ((5 - _difficulty) * 25);
    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].gold_reserve = 150;
        AI_SCircle_Reevals[itr_players] = (80 + Random(40));
    }
    gd_dump_players("209_gold_reserve_P");  /* on-return Init_Runtime() */

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

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
    int16_t p1_ranks = 0;
    int16_t p2_ranks = 0;
    int16_t max_chaos = 0;
    int16_t sum_life = 0;
    int16_t max_death = 0;
    int16_t itr_players1 = 0;
    int16_t itr_players2 = 0;
    int16_t score = 0;  /* player pair's initial diplomatic relation score */
    int16_t shared_ranks = 0;  // DNE in Dasm

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    grand_vizier = ST_FALSE;

    for(itr_players1 = 0; itr_players1 < _num_players; itr_players1++)
    {

        for(itr_players2 = (itr_players1 + 1); itr_players2 < _num_players; itr_players2++)
        {

            max_death = _players[itr_players1].spellranks[sbr_Death];

            if(_players[itr_players2].spellranks[sbr_Death] > max_death)
            {
                max_death = _players[itr_players2].spellranks[sbr_Death];
            }

            sum_life =  _players[itr_players1].spellranks[sbr_Life];
            sum_life += _players[itr_players2].spellranks[sbr_Life];

            max_chaos = _players[itr_players1].spellranks[sbr_Chaos];
            if(_players[itr_players2].spellranks[sbr_Chaos] > max_chaos)
            {
                max_chaos = _players[itr_players2].spellranks[sbr_Chaos];
            }

            score = 0;
            if(
                (sum_life > 0)
                &&
                (max_death > 0)
            )
            {
                score -= ((sum_life + max_death) * 5);
            }
            else
            {
                score += (sum_life * 2);
                score -= (max_death * 3);
            }

            score -= (max_chaos * 2);

            p2_ranks = _players[itr_players2].spellranks[sbr_Sorcery];
            p1_ranks = _players[itr_players1].spellranks[sbr_Sorcery];
            shared_ranks = MIN(p1_ranks,p2_ranks);
            score += (shared_ranks * 2);

            p2_ranks = _players[itr_players2].spellranks[sbr_Chaos];
            p1_ranks = _players[itr_players1].spellranks[sbr_Chaos];
            shared_ranks = MIN(p1_ranks,p2_ranks);
            score += (shared_ranks * 2);

            p2_ranks = _players[itr_players2].spellranks[sbr_Nature];
            p1_ranks = _players[itr_players1].spellranks[sbr_Nature];
            shared_ranks = MIN(p1_ranks,p2_ranks);
            score += (shared_ranks * 2);

            p2_ranks = _players[itr_players2].spellranks[sbr_Life];
            p1_ranks = _players[itr_players1].spellranks[sbr_Life];
            shared_ranks = MIN(p1_ranks,p2_ranks);
            score += (shared_ranks * 2);

            SETMIN(score,-90);

            _players[itr_players1].Dipl.Visible_Rel[itr_players2] = (int8_t)score;
            _players[itr_players2].Dipl.Visible_Rel[itr_players1] = (int8_t)score;

            _players[itr_players1].Dipl.Default_Rel[itr_players2] = (int8_t)score;
            _players[itr_players2].Dipl.Default_Rel[itr_players1] = (int8_t)score;

            _players[itr_players1].peace_duration[itr_players2] = 0;
            _players[itr_players2].peace_duration[itr_players1] = 0;

        }

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o56p4
void Initialize_Items(void)
{
    int16_t itr = 0;
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    for(itr = 0; itr < NUM_ITEMS; itr++)
    {
        _ITEMS[itr].cost = 0;
    }
    for(itr = 0; itr < NUM_PREFAB_ITEMS; itr++)
    {
        _prefab_item_table[itr] = 0;
    }
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
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
    int16_t itr_players = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

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

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    
}


// MGC o56p6
// MoO2  Module: KEN  Init_NPC_Personalities_Objectives_Themes_()
/*
sets the primary and secondary realms into each player's wizard record, then rolls out the AI personalities and objectives based on that and the retorts already stored
BUG: the guaranteed AI researchable spells for Hard and Impossible will also remove those spells if the wizard already has them
BUG: 2+ realm wizards with Nature as the primary will also always set that as the secondary
*/
/*
    sets Primary_Realm and Secondary_Realm for all players
    sets starting spells for AI players
    sets Personality and Object for AI players
*/
/**
 * @brief Derives primary/secondary realms, seeds AI starting spells, and assigns AI personality/objective.
 *
 * @details
 * Performs new-game magic-profile initialization in two stages:
 * - Human player: determines `Prim_Realm` and `Sec_Realm` from spellbook ranks,
 *   with secondary collapsing to primary when no second realm rank exists.
 * - AI players (`1 .. _num_players-1`):
 *   - determines primary/secondary realms from spellbook ranks,
 *   - grants realm-based starting summon spells,
 *   - on hard+ difficulty applies additional realm-based spell seeding,
 *   - builds weighted personality/objective pools from realm tables,
 *   - applies retort-based weight modifiers,
 *   - draws weighted random `Personality` and `Objective` assignments.
 *
 * The function also enforces realm-ethic constraints in weighting
 * (`Life` cannot be maniacal, `Death` cannot be peaceful) before weighted picks.
 *
 * @return void
 *
 * @note Mutates `_players[]` fields including `Prim_Realm`, `Sec_Realm`,
 *       `spells_list`, `Personality`, and `Objective`.
 * @note Uses global weighting tables `TBL_AI_Realm_PRS` and
 *       `TBL_AI_Realm_OBJ`.
 * @warning Preserves legacy behaviors and known quirks documented in nearby
 *          comments (including realm/spell mismatch checks in hard+ seeding).
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
    int16_t itr = 0;
    int16_t itr_players = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* Phase 1 */
    /* Determine human player's primary and secondary realms */
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
    Secondary_Realm = 0;
    spellranks = _players[HUMAN_PLAYER_IDX].spellranks[0];
    for(itr = 1; itr < NUM_MAGIC_TYPES; itr++)
    {
        if(_players[HUMAN_PLAYER_IDX].spellranks[itr] > spellranks)
        {
            if(itr != Primary_Realm)
            {
                Secondary_Realm = itr;
                spellranks = _players[HUMAN_PLAYER_IDX].spellranks[itr];
            }
        }
    }
    if(_players[HUMAN_PLAYER_IDX].spellranks[Secondary_Realm] == 0)
    {
        Secondary_Realm = Primary_Realm;
    }
    _players[HUMAN_PLAYER_IDX].Prim_Realm = Primary_Realm;
    _players[HUMAN_PLAYER_IDX].Sec_Realm = Secondary_Realm;


    /* Phase 2 */
    /* Loop through AI players */
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
                _players[itr_players].spells_list[spl_Water_Walking] = sls_Known;  /* OGBUG  spl_Sprites ? */
            } break;
            case sbr_Sorcery:
            {
                _players[itr_players].spells_list[spl_Counter_Magic] = sls_Known;  /* OGBUG  spl_Nagas ? */
            } break;
            case sbr_Chaos:
            {
                _players[itr_players].spells_list[spl_Fire_Bolt] = sls_Known;  /* OGBUG  should be spl_Hell_Hounds ? */
            } break;
            case sbr_Life:
            {
                _players[itr_players].spells_list[spl_True_Light] = sls_Known;  /* OGBUG  spl_Guardian_Spirit ? */
            } break;
            case sbr_Death:
            {
                _players[itr_players].spells_list[spl_Black_Sleep] = sls_Known;  /* OGBUG  spl_Ghouls ? */
            } break;
        }

        if(_difficulty >= god_Hard)
        {
            switch(Primary_Realm)
            {
                case sbr_Nature:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == sls_Unknown)
                    {
                        _players[itr_players].spells_list[spl_Basilisk] = sls_Knowable;
                    }
                } break;
                case sbr_Sorcery:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == sls_Unknown)  /* OGBUG  should be spl_Phantom_Beast, not spl_Basilisk */
                    {
                        _players[itr_players].spells_list[spl_Phantom_Beast] = sls_Knowable;
                    }
                } break;
                case sbr_Chaos:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == sls_Unknown)  /* OGBUG  should be spl_Chimeras, not spl_Basilisk */
                    {
                        _players[itr_players].spells_list[spl_Chimeras] = sls_Knowable;
                    }
                } break;
                case sbr_Life:
                {
                    if(_players[itr_players].spells_list[spl_Path_Finding] == sls_Unknown)  /* OGBUG  should be spl_Unicorns, not spl_Path_Finding */
                    {
                        _players[itr_players].spells_list[spl_Unicorns] = sls_Knowable;
                    }
                } break;
                case sbr_Death:
                {
                    if(_players[itr_players].spells_list[spl_Basilisk] == sls_Unknown)  /* OGBUG  should be spl_Shadow_Demons, not spl_Basilisk */
                    {
                        _players[itr_players].spells_list[spl_Shadow_Demons] = sls_Knowable;
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
                if(itr != Primary_Realm)
                {
                    Secondary_Realm = itr;
                    spellranks = _players[itr_players].spellranks[itr];
                }
            }
        }
    
        if(_players[itr_players].spellranks[Secondary_Realm] == 0)
        {
            Secondary_Realm = Primary_Realm;
        }
    
        _players[itr_players].Prim_Realm = Primary_Realm;
    
        _players[itr_players].Sec_Realm = Secondary_Realm;
    
        for(itr = 0; itr < 6; itr++)
        {
            AI_PRS_Weights[itr] = TBL_AI_Realm_PRS[Primary_Realm][itr];
        }

        for(itr = 0; itr < 5; itr++)
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

/*
    BEGIN:  OBJ
*/
        if(_players[itr_players].alchemy == 1)
        {
            AI_OBJ_Weights[AI_OBJ_PERFECTIONIST] += 2;
        }

        if(_players[itr_players].archmage == 1)
        {
            AI_OBJ_Weights[AI_OBJ_THEURGIST] += 4;
        }

        if(_players[itr_players].myrran == 1)
        {
            AI_OBJ_Weights[AI_OBJ_EXPANSIONIST] += 4;
        }

        if(_players[itr_players].conjurer == 1)
        {
            AI_OBJ_Weights[AI_OBJ_MILITARIST] += 3;
        }
/*
    END:  OBJ
*/

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
        for(itr = 0; itr < 5; itr++)
        {
            Max_Value += AI_OBJ_Weights[itr];
        }
        Random_Result = Random(Max_Value);
        Picked_Objective = 0;
        Random_Result -= AI_OBJ_Weights[AI_OBJ_PRAGMATIST];
        while((Random_Result > 0) && (Picked_Objective < 4))
        {
            Picked_Objective++;
            Random_Result -= AI_OBJ_Weights[Picked_Objective];
        }

        _players[itr_players].Personality = Picked_Personality;

        _players[itr_players].Objective = Picked_Objective;

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o56p7
/*
MoO2  Module: DIPLOMAC  Init_Diplomatic_Relations_()
*/
/**
 * @brief Resets per-player AI strategy timers and relationship sentinel state.
 *
 * @details
 * Iterates across all active players in `_num_players` and clears the
 * reevaluation countdowns used by AI hostility, magic-strategy, and
 * magic-power logic. It also initializes every entry in each player's
 * `peace_duration[]` array to `ST_UNDEFINED`, then resets the four
 * undocumented `IDK_AI_Strategy_*` fields to the same sentinel value.
 *
 * Despite the function name, the reset is applied to every active player slot
 * rather than only computer-controlled wizards.
 *
 * @return void
 *
 * @note This routine mutates the global `_players[]` table in place.
 * @note `ST_UNDEFINED` is used here as an initialization sentinel, not as a
 *       countdown value.
 */
void Init_CP_Strategy(void)
{
    int16_t player_idx = 0;
    int16_t other_player_idx = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(player_idx = 0; player_idx < _num_players; player_idx++)
    {
        _players[player_idx].reevaluate_hostility_countdown = 0;
        _players[player_idx].reevaluate_magic_strategy_countdown = 0;
        _players[player_idx].reevaluate_magic_power_countdown = 0;
        for(other_player_idx = 0; other_player_idx < _num_players; other_player_idx++)
        {
            _players[player_idx].peace_duration[other_player_idx] = ST_UNDEFINED;
        }
        _players[player_idx].niu_cp_target_1 = ST_UNDEFINED;
        _players[player_idx].niu_cp_target_2 = ST_UNDEFINED;
        _players[player_idx].cp_target_3 = ST_UNDEFINED;
        _players[player_idx].niu_cp_target_4 = ST_UNDEFINED;
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_CP_Strategy rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p8
/**
 * @brief Initializes baseline runtime state for every active player.
 *
 * @details
 * Walks the active player range `[0, _num_players)` and derives each
 * wizard's opening fame, nominal casting skill, and invested casting skill
 * from stored spellbook counts and retorts. It then clears or seeds the core
 * strategic resource fields used at game start, including mana, research,
 * casting progress, power-distribution ratios, tax rate, global enchantment
 * flags, vault contents, and hero slot assignments.
 *
 * The routine computes a target `spell_skill` from the player's five realm
 * book counts, adds the Archmage bonus when present, doubles the result, and
 * increments `spell_casting_skill` until `Player_Nominal_Skill()` reaches that
 * derived target.
 *
 * @return void
 *
 * @note Mutates the global `_players[]` table in place.
 * @note Every player starts with `spl_Spell_Of_Mastery` marked with state `3`
 *       and all hero slots initialized to `ST_UNDEFINED`.
 *
 * @see Player_Nominal_Skill
 */
void Init_Players(void)
{
    int16_t itr_heroes = 0;
    int16_t itr_players = 0;
    int16_t spell_skill = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

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
        /* Base skill is doubled for starting calculation */
        spell_skill += spell_skill;
        _players[itr_players].Nominal_Skill = 0;
        _players[itr_players].spell_casting_skill = 0;
        // The grow-until-reached loop lands spell_casting_skill at the smallest value where Player_Base_Casting_Skill__MGC returns spell_skill.
        // The inverse formula is N*(N-1)+1.
        while(_players[itr_players].Nominal_Skill < spell_skill)
        {
            _players[itr_players].spell_casting_skill += 1;
            _players[itr_players].Nominal_Skill = Player_Base_Casting_Skill__MGC(itr_players);
        }
        _players[itr_players].researching_spell_idx = spl_NONE;
        _players[itr_players].mana_reserve = 0;
        _players[itr_players].research_cost_remaining = 0;
        _players[itr_players].skill_ratio = 33;
        _players[itr_players].mana_ratio = 33;
        _players[itr_players].research_ratio = 34;
        _players[itr_players].spells_list[spl_Summon_Champion] = sls_Researchable;  /* OGBUG  should be 213 spl_Spell_Of_Mastery */
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

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Init_Players rng_call=%llu", (unsigned long long)g_random_call_count);
}


// MGC o56p9
void Init_Computer_Players_Wizard_Profile(void)
{
    int16_t Realm_Retort_Available = 0;
    int16_t Realm_Index = 0;
    int16_t Bookshelf[5] = { 0, 0, 0, 0, 0 };
    int16_t myrran_count = 0;
    int16_t Picks_Left = 0;
    int16_t banners[5] = { 0, 0, 0, 0, 0 };
    int8_t * wsa_ptr = 0;  // Pointer to 'Wizard Special Abilities'
    int16_t Book_Count = 0;
    int16_t random_banner = 0;
    int16_t Trait_Value = 0;  // NOTE: used to index wsa_ptr[]
    int16_t Picks_Used = 0;
    int16_t Trait_Type = 0;
    int16_t Erasure_Dice = 0;
    int16_t Random_Result = 0;
    int16_t Portrait_Taken = 0;
    int16_t itr3 = 0;
    int16_t itr1 = 0;
    int16_t itr2 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* Phase 1: Initialize local banner tracking */
    for(itr1 = 0; itr1 < NUM_BANNER_SELECTIONS; itr1++)
    {
        banners[itr1] = ST_FALSE;
    }
    banners[_players[HUMAN_PLAYER_IDX].banner_id] = ST_TRUE;


BeginTopLevelPlayerLoop:


    /* Phase 2 */
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        do {
            Portrait_Taken = ST_FALSE;
            Random_Result = Random(14) - 1; /* {0,...,13} */
            for(itr3 = 0; itr3 < itr2; itr3++)
            {
                if(_players[itr3].wizard_id == Random_Result)
                {
                    Portrait_Taken = ST_TRUE;
                }
            }
        } while(Portrait_Taken != ST_FALSE);
        _players[itr2].wizard_id = (uint8_t)Random_Result;
    }
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        stu_strcpy(_players[itr2].name, _wizard_presets_table[_players[itr2].wizard_id].name);
    }


    /* Phase 3 */
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
        for(itr1 = 0; itr1 < NUM_WIZARD_SPECIAL_ABILITIES; itr1++)
        {
            wsa_ptr[itr1] = 0;
        }
        if(_wizard_presets_table[_players[itr2].wizard_id].special != ST_UNDEFINED)
        {
            wsa_ptr[_wizard_presets_table[_players[itr2].wizard_id].special] = ST_TRUE;
        }
    }


    /* Phase 4 */
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        /* Calculate "Erasure Dice" to randomize/reduce AI starting power */
        switch(_difficulty)
        {
            case 0: { Erasure_Dice = 0; } break;
            case 1: { Erasure_Dice = 0; } break;
            case 2: { Erasure_Dice = Random(3); } break;
            case 3: { Erasure_Dice = (Random(3) + Random(3)); } break;
            case 4: { Erasure_Dice = (Random(5) + Random(5)); } break;
            case 5: { Erasure_Dice = (Random(8) + Random(8)); } break;
        }
        wsa_ptr = &_players[itr2].alchemy;
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
                        /* OGBUG  should use itr3, not itr1 */
                        wsa_ptr[itr1] = ST_FALSE;
                    }
                } break;
            }
        }
    }


    /* Phase 5 */
    for(itr2 = 1; itr2 < _num_players; itr2++)
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
        for(itr1 = 0; itr1 < NUM_WIZARD_SPECIAL_ABILITIES; itr1++)
        {
            if(wsa_ptr[itr1] == 0)
            {
                continue;
            }
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
        Picks_Left = 11;
        if(_difficulty == god_Hard)
        {
            Picks_Left = 13;
        }
        if(_difficulty == god_Impossible)
        {
            Picks_Left = 15;
        }
        while(Picks_Used < Picks_Left)
        {
            Trait_Type = Random(8);
            Trait_Value = (1 + Random(4));  // NOTE: used to index wsa_ptr[]
            /* clamps: Trait_Value vs (Picks_Left-Picks_Used) and vs (12-Book_Count) */
            /* if Trait_Value <= 0 -> Trait_Type = 6 ; if Trait_Type == 6 -> Trait_Value = 1 */
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
            switch(Trait_Type)
            {
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
                    if( (Trait_Value >= wsa_Chaos_Mastery) && (Trait_Value <= wsa_Divine_Power))
                    {
                        for(Realm_Index = 0; Realm_Index <= 4; Realm_Index++)
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
                        /* Semantically reads as "drop this realm from the weighted retort pool if the wizard already has the ability, or if picks are too tight to afford it" */
                        if((_players[itr2].divine_power > 0) || ((Picks_Left - 2) < Picks_Used))
                        {
                            Bookshelf[sbr_Life] = 0;
                        }
                        if((_players[itr2].infernal_power > 0) || ((Picks_Left - 2) < Picks_Used))
                        {
                            Bookshelf[sbr_Death] = 0;
                        }
                        Realm_Retort_Available = 0;
                        for(Realm_Index = 0; Realm_Index <= 4; Realm_Index++)
                        {
                            Realm_Retort_Available += Bookshelf[Realm_Index];
                        }
                        if(Realm_Retort_Available != 0)
                        {
                            Trait_Value = (2 + Get_Weighted_Choice(&Bookshelf[0], 5));
                            switch(Trait_Value)
                            {
                                case 2: { Trait_Value = wsa_Nature_Mastery;  } break;
                                case 3: { Trait_Value = wsa_Sorcery_Mastery; } break;
                                case 4: { Trait_Value = wsa_Chaos_Mastery;   } break;
                                case 5: { Trait_Value = wsa_Divine_Power;    } break;
                                case 6: { Trait_Value = wsa_Infernal_Power;  } break;
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
                        else
                        {
                            wsa_ptr[Trait_Value] = ST_TRUE;
                        }
                    }
                } break;  /* END  case {6,7,8}: */

            }  /* END:  switch(Trait_Type) */

            /* after ... switch(Trait_Type) ... ```dasm @@AfterSwitchTrait_Type:  ; default ``` */

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
                if(wsa_ptr[itr1] == ST_FALSE)
                {
                    continue;
                }
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

    }


    /* Phase 6: Myrran count sanity check */
    myrran_count = 0;
    for(itr1 = 1; itr1 < _num_players; itr1++)
    {
        if(_players[itr1].myrran != ST_FALSE)
        {
            myrran_count++;
        }
    }
    /* Requirement: At least 1 (but no more than 2) Myrrans on high difficulty / player counts */
    if((myrran_count < 1 || myrran_count > 2) && (_difficulty > god_Normal && _num_players > 3))
    {
        goto BeginTopLevelPlayerLoop;
    }
    /* dead-code for changing Sss'ra */
    /* Patch for specific preset table index if logic fails? */
    /* 9Ah 154d / sizeof(s_WIZARD_PRESET) = 7 */
    /* {"Sss'ra",  4,  0,  0, 0,  4, wsa_Myrran}, */
    /* _wizard_presets_table[7].special = ST_UNDEFINED; */
    /* _wizard_presets_table[7].chaos = 7; */


/*
END:  ¿ jmp     @@BeginTopLevelPlayerLoop ?
*/


    /* Phase ? */
    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        if(
            (_players[itr2].spellranks[sbr_Nature] > 3)
            &&
            (banners[BNR_Green] == ST_FALSE)
        )
        {
            _players[itr2].banner_id = BNR_Green;
            banners[BNR_Green] = ST_TRUE;
        }
        else if(
            (_players[itr2].spellranks[sbr_Chaos] > 3)
            &&
            (banners[BNR_Red] == ST_FALSE)
        )
        {
            _players[itr2].banner_id = BNR_Red;
            banners[BNR_Red] = ST_TRUE;
        }
        else if(
            (_players[itr2].spellranks[sbr_Sorcery] > 3)
            &&
            (banners[BNR_Blue] == ST_FALSE)
        )
        {
            _players[itr2].banner_id = BNR_Blue;
            banners[BNR_Blue] = ST_TRUE;
        }
        else if(
            (_players[itr2].spellranks[sbr_Death] > 3)
            &&
            (banners[BNR_Purple] == ST_FALSE)
        )
        {
            _players[itr2].banner_id = BNR_Purple;
            banners[BNR_Purple] = ST_TRUE;
        }
        else
        {
            /* Pick random available banner */
            do {
                random_banner = (Random(5) - 1);
            } while(banners[random_banner] == ST_TRUE);
            banners[random_banner] = ST_TRUE;
            _players[itr2].banner_id = (uint8_t)random_banner;
        }
    }

    // @@Done

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// MGC o56p10
/**
 * @brief Builds the initial spell-library state for every active player.
 *
 * @details
 * Clears each player's spell availability table, then repopulates it from the
 * player's starting realm books and preselected starting spell lists. The
 * routine marks the Arcane realm's fixed researchable spells, grants starting
 * spells for each non-zero realm, and expands the library with knowable
 * common, uncommon, rare, and very-rare spells according to the wizard's book
 * count in that realm.
 *
 * For fully mastered realms, all spells in the realm are made knowable and
 * the appropriate starting spells are promoted to known status. The function
 * also ensures that Spell of Mastery is always known, and that Artificer
 * wizards begin with Detect Magic and Disjunction.
 *
 * Side effects are limited to mutation of the global `_players[]` spell
 * library tables.
 *
 * @return void
 *
 * @note Expects `_num_players`, `_players[]`, and `_player_start_spells[]` to
 *       have already been initialized by earlier new-game setup.
 * @note Uses `Random()` while selecting knowable spells, so the resulting
 *       library distribution is intentionally nondeterministic.
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
    int16_t itr2 = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr = 0; itr < _num_players; itr++)
    {

        for(sbr = 0; sbr < NUM_MAGIC_REALMS; sbr++)
        {
            for(sri = 0; sri < NUM_SPELLS_PER_MAGIC_REALM; sri++)
            {
                _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + sri)] = 0;
            }
        }

        for(sbr = 0; sbr < NUM_MAGIC_REALMS; sbr++)
        {

            if(sbr == sbr_Arcane)
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
                continue;
            }

            if(_players[itr].spellranks[sbr] == 0)
            {
                continue;
            }
        

            if(_players[itr].spellranks[sbr] != 11)
            {

                for(itr2 = 0; itr2 < 10; itr2++)
                {
                    Availability_Array[itr2] = 0;
                }

                switch(_players[itr].spellranks[sbr])
                {
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

                switch(sbr)
                {
                    case sbr_Nature:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[
                                (
                                    (
                                        (
                                            (sbr * NUM_SPELLS_PER_MAGIC_REALM)
                                            +
                                            (_player_start_spells[itr].realms[sbr_Nature].spells[itr2])
                                        )
                                    - 1
                                    ) % NUM_SPELLS_PER_MAGIC_REALM
                                )
                            ] = sls_Known;
                        }
                    } break;
                    case sbr_Sorcery:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + (_player_start_spells[itr].realms[sbr_Sorcery].spells[itr2] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        }
                    } break;
                    case sbr_Chaos:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + (_player_start_spells[itr].realms[sbr_Chaos].spells[itr2] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        }
                    } break;
                    case sbr_Life:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + (_player_start_spells[itr].realms[sbr_Life].spells[itr2] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        }
                    } break;
                    case sbr_Death:
                    {
                        for(itr2 = 0; (_players[itr].spellranks[sbr] - 1) > itr2; itr2++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + (_player_start_spells[itr].realms[sbr_Death].spells[itr2] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        }
                    } break;
                }

                // IDA purple #32
                /* DIVBUG  the original assumed (rank - 1) pre-fill spells
                 * always land in common-rarity slots (0..9), but the
                 * `case sbr_X:` pre-fill above uses `% NUM_SPELLS_PER_MAGIC_REALM`
                 * (40), so spells can land in any rarity.  When fewer than
                 * (rank - 1) pre-filled spells actually land in common,
                 * the while loop below over-iterates.  Count the actual
                 * common-rarity non-Unknown slots instead. */
                Available_Spells = 0;
                for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                {
                    if(_players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] != sls_Unknown)
                    {
                        Available_Spells++;
                    }
                }
                LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @before-common    p=%d sbr=%d rank=%d limit=%d  initial=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, Available_Spells, (unsigned long long)g_random_call_count);
                while(Available_Spells < Availability_Limit)
                {
                    itr2 = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);
                    if(_players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] == sls_Unknown)
                    {
                        _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] = sls_Knowable;
                    }
                    Available_Spells = 0;
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        if(_players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] != sls_Unknown)
                        {
                            Available_Spells++;
                        }
                    }
                }
                LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @after-common     p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);

/*
END: Knowable - Common
*/

/*
BEGIN: Knowable - Uncommon
*/

                if(_players[itr].spellranks[sbr] > 7)
                {
                    for(itr2 = 0; itr2 < 10; itr2++)
                    {
                        _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 10 + itr2)] = sls_Knowable;
                    }
                }
                else
                {
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        Availability_Array[itr2] = 0;
                    }
                    switch(_players[itr].spellranks[sbr])
                    {
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
                    itr2 = 0;
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @before-uncommon  p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    while(itr2 < Availability_Limit)
                    {
                        InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);
                        if(Availability_Array[InRarity_Index] != 1)
                        {
                            Availability_Array[InRarity_Index] = 1;
                            itr2++;
                        }
                    }
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @after-uncommon   p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        if(Availability_Array[itr2] == 1)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 10 + itr2)] = sls_Knowable;
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
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 20 + itr2)] = sls_Knowable;
                    }
                }
                else
                {
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        Availability_Array[itr2] = 0;
                    }
                    switch(_players[itr].spellranks[sbr])
                    {
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
                    itr2 = 0;
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @before-rare      p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    while(itr2 < Availability_Limit)
                    {
                        InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);
                        if(Availability_Array[InRarity_Index] != 1)
                        {
                            Availability_Array[InRarity_Index] = 1;
                            itr2++;
                        }
                    }
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @after-rare       p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        if(Availability_Array[itr2] == 1)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 20 + itr2)] = sls_Knowable;
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
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 30 + itr2)] = sls_Knowable;
                    }
                }
                else
                {
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        Availability_Array[itr2] = 0;
                    }
                    switch(_players[itr].spellranks[sbr])
                    {
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
                    itr2 = 0;
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @before-very_rare p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    while(itr2 < Availability_Limit)
                    {
                        InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);
                        if(Availability_Array[InRarity_Index] != 1)
                        {
                            Availability_Array[InRarity_Index] = 1;
                            itr2++;
                        }
                    }
                    LOG_TRACE(LOG_CAT_GENERAL, "[ICPSL] @after-very_rare  p=%d sbr=%d rank=%d limit=%d  rng_call=%llu", itr, sbr, _players[itr].spellranks[sbr], Availability_Limit, (unsigned long long)g_random_call_count);
                    for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_RARITY; itr2++)
                    {
                        if(Availability_Array[itr2] == 1)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + 30 + itr2)] = sls_Knowable;
                        }
                    }
                }

/*
END: Knowable - Very Rare
*/

            }
            else  /* (_players[itr].spellranks[sbr] == 11) */
            {
                for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_REALM; itr2++)
                {
                    _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] = sls_Knowable;
                }

                switch(sbr)
                {
                    case sbr_Nature:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Nature].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Nature].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Nature].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                    case sbr_Sorcery:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Sorcery].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Sorcery].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Sorcery].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                    case sbr_Chaos:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Chaos].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Chaos].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Chaos].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                    case sbr_Life:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Life].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Life].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Life].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                    case sbr_Death:
                    {
                        for(Common_Index = 0; Common_Index < 10; Common_Index++)
                        {
                            _players[itr].spells_list[((sbr * NUM_SPELLS_PER_MAGIC_REALM) + Common_Index)] = sls_Known;
                        }
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Death].spells[10] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Death].spells[11] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                        _players[itr].spells_list[sbr * NUM_SPELLS_PER_MAGIC_REALM + ((_player_start_spells[itr].realms[sbr_Death].spells[12] - 1) % NUM_SPELLS_PER_MAGIC_REALM)] = sls_Known;
                    } break;
                }

            }  /* END  if(_players[itr].spellranks[sbr] != 11) else */

        }

        _players[itr].spells_list[spl_Demon_Lord] = sls_Known;
        if(_players[itr].artificer != 0)
        {
            _players[itr].spells_list[spl_Detect_Magic] = sls_Known;
            _players[itr].spells_list[spl_Disjunction] = sls_Known;
        }

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

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
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
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
    gd_dump_players("Initialize_Events");
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Initialize_Events rng_call=%llu", (unsigned long long)g_random_call_count);
}

// MGC o56p12
// MoO2  Module: INITGAME  Init_Leaders_()
/**
 * @brief Initializes hero records for every player at new-game start.
 *
 * @details
 * Walks every player slot and every premade hero template, then seeds the
 * player-local hero records from `_hero_premade_table`. The routine resets the
 * hero level, starts from the template's warrior/mage/any pick budget, and
 * rolls randomized hero abilities until the available picks are consumed.
 *
 * Ability assignment rules are based on the premade template's `pick_type`:
 * - Warrior picks unlock martial abilities and some mixed traits.
 * - Mage picks unlock magical abilities, casting skill, and some mixed traits.
 * - Any/Both picks apply the same pool to both warrior and mage counters.
 *
 * The function also copies the template spell loadout into the runtime hero
 * record and emits trace logging for the generated spell state.
 *
 * @return void
 *
 * @note Mutates `_HEROES2[]` in place for all player slots and hero types.
 * @note Uses `Random(14)` repeatedly while drawing hero perks, so the final
 *       hero composition is intentionally nondeterministic.
 * @warning Preserves legacy behavior in the selection loop, including the
 *          original pick consumption pattern and the existing hero-template
 *          constraints.
 */
void Init_Heroes(void)
{
    int32_t abilities = 0;
    int16_t all_picks = 0;
    int16_t mage_picks = 0;
    int16_t casting_skill = 0;
    int16_t itr_hero_types = 0;
    int16_t itr_players = 0;
    int16_t warrior_picks = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    for(itr_players = 0; itr_players < (NUM_PLAYERS - 1); itr_players++)
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
                            /* OGBUG  the Knight has no caster picks, this should be $1E (the Elven Archer) */
                            if(itr_hero_types != ut_Knight)  /* ¿ should ut_ElvenArcher  = 30 ? */
                            {
                                /* OGBUG  excludes the Warlock and the Chaos Warrior, this should be $1E (Ranged_Lightning) */
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
                                warrior_picks--;
                                mage_picks--;
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
                        if(((abilities & HSA_NOBLE) == 0) && (itr_hero_types != ut_Chosen)) { abilities |= HSA_NOBLE; warrior_picks--; mage_picks--; }
                    } break;
                    case 10:  /* Charm */
                    {
                        if(((abilities & HSA_CHARMED) == 0) && ((_unit_type_table[itr_hero_types].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)) { abilities |= HSA_CHARMED; warrior_picks--; mage_picks--; }
                    } break;
                    case 11:  /* Lucky */
                    {
                        if((abilities & HSA_LUCKY) == 0) { abilities |= HSA_LUCKY; warrior_picks--; mage_picks--; }
                    } /* OGBUG  missing `break;` */
                    case 12:  /* Agility */
                    {
                        if(((abilities & HSA_AGILITY2) == 0) && (warrior_picks > 0)) { if((abilities & HSA_AGILITY) != 0) { abilities ^= HSA_AGILITY; abilities |= HSA_AGILITY2; } else { abilities |= HSA_AGILITY; } warrior_picks--; mage_picks--; }
                    } break;
                    case 13:  /* Sage */
                    {
                        if(((abilities & HSA_SAGE2) == 0) && (mage_picks > 0)) { if((abilities & HSA_SAGE) != 0) { abilities ^= HSA_SAGE; abilities |= HSA_SAGE2; } else { abilities |= HSA_SAGE; } warrior_picks--; mage_picks--; }
                    } break;

                }

            }

            _HEROES2[itr_players]->heroes[itr_hero_types].abilities = abilities;
            _HEROES2[itr_players]->heroes[itr_hero_types].Casting_Skill = (int8_t)casting_skill;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[0] = (uint8_t)_hero_premade_table[itr_hero_types].spell_1;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[1] = (uint8_t)_hero_premade_table[itr_hero_types].spell_2;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[2] = (uint8_t)_hero_premade_table[itr_hero_types].spell_3;
            _HEROES2[itr_players]->heroes[itr_hero_types].Spells[3] = (uint8_t)_hero_premade_table[itr_hero_types].spell_4;

            LOG_TRACE(LOG_CAT_GENERAL,
                "[Init_Heroes] p=%d ht=%d table_spells=(%d,%d,%d,%d) struct_after=(0x%02X,0x%02X,0x%02X,0x%02X) rng_call=%llu",
                itr_players, itr_hero_types,
                _hero_premade_table[itr_hero_types].spell_1,
                _hero_premade_table[itr_hero_types].spell_2,
                _hero_premade_table[itr_hero_types].spell_3,
                _hero_premade_table[itr_hero_types].spell_4,
                _HEROES2[itr_players]->heroes[itr_hero_types].Spells[0],
                _HEROES2[itr_players]->heroes[itr_hero_types].Spells[1],
                _HEROES2[itr_players]->heroes[itr_hero_types].Spells[2],
                _HEROES2[itr_players]->heroes[itr_hero_types].Spells[3],
                (unsigned long long)g_random_call_count);

        }

    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

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
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
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
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=Initialize_Messages rng_call=%llu", (unsigned long long)g_random_call_count);
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
/*
¿ ~ Player_Base_Casting_Skill() ?
*/
/**
 * @brief Converts stored casting-skill investment into nominal casting skill.
 *
 * @details
 * Interprets `_players[player_idx].spell_casting_skill` as an accumulated pool
 * of invested skill points and walks the game's triangular progression one
 * step at a time until the running threshold meets or exceeds that stored
 * investment. The resulting step count becomes the player's nominal casting
 * skill, with an additional +10 applied when the wizard has the Archmage
 * retort.
 *
 * This routine is used during player initialization to translate invested
 * casting points back into the displayed base skill value.
 *
 * @param player_idx Index of the player record in `_players[]`.
 *
 * @return The player's nominal casting skill, including the Archmage bonus
 *         when applicable.
 *
 * @note Uses the same progression as `Player_Base_Casting_Skill()` in
 *       [MoM/src/NEXTTURN.c](MoM/src/NEXTTURN.c).
 */
int16_t Player_Base_Casting_Skill__MGC(int16_t player_idx)
{
    int32_t num = 0;
    int16_t twos = 0;
    int16_t casting_skill = 0;
    twos = 0;
    casting_skill = 0;
    num = 0;
    while(_players[player_idx].spell_casting_skill > num)
    {
        casting_skill += 1;
        twos += 2;
        num += twos;
    }
    if(_players[player_idx].archmage > 0)
    {
        casting_skill += 10;
    }
    return casting_skill;
}


// MGC o56p17
// drake178: RNG_WeightedPick16()
// RNG_WeightedPick16()
