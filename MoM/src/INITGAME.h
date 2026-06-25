/*
    MAGIC.EXE
    ovr056
*/

#ifndef INITGAME_H
#define INITGAME_H

#include "../../MoX/src/MOX_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif



/*
    MAGIC.EXE
        ovr056
*/

/* CaptureGameData Phase 4 (ReMoM side): dump _players in [GD] format,
 * matching the OG STU-DOSBox probe. Defined in INITGAME.c. */
void gd_dump_players(const char* point);
void gd_dump_nodes(const char* point);
void gd_dump_towers(const char* point);
void gd_dump_lairs(const char* point);
void gd_dump_cities(const char* point);
void gd_dump_units(const char* point);

/* Capture/Inject (CI), ReMoM side.  Reads og-game-data-capture.fwv (produced by
 * extract-ci-stage0.py from the OG [CI] probe) and injects OG's exact bytes for
 * values ReMoM cannot reproduce on its own -- uninitialized stack autos and
 * OOB-overrun reads.  Distinct from the gd_dump_* [GD] capture (which compares).
 * Defined in INITGAME.c.  Loads lazily on first use; idempotent. */
int  gd_ci_load(void);                                                  /* -> record count, or -1 on failure */
int  gd_ci_get(const char* key, const char* site, long* out, int max);  /* -> values copied, or -1 if absent */
void gd_ci_inject_world_overrun(const char* site);                      /* write OG's OOB int16 past _world_maps */

// MGC o56p1
void Init_Computer_Players(void);

// MGC o56p2
void Init_Runtime(void);

// MGC o56p3
void Init_Diplomatic_Relations(void);

// MGC o56p4
void Initialize_Items(void);

// MGC o56p5
void Init_Summoning_Circle_And_Spell_Of_Mastery(void);

// MGC o56p6
void Init_Magic_Personalities_Objectives(void);

// MGC o56p7
void Init_CP_Strategy(void);

// MGC o56p8
void Init_Players(void);

// MGC o56p9
void Init_Computer_Players_Wizard_Profile(void);

// MGC o56p10
void Init_Computer_Players_Spell_Library(void);

// MGC o56p11
void Initialize_Events(void);

// MGC o56p12
void Init_Heroes(void);

// MGC o56p13
void Initialize_Messages(void);

// MGC o56p14
// UU_ITEM_SetHeroSlots()

// MGC o56p15
// WIZ_ConsolidateBooks()
void Consolidate_Spell_Book_Realms(int16_t player_idx);

// MGC o56p16
int16_t Player_Base_Casting_Skill__MGC(int16_t player_idx);

// MGC o56p17
// RNG_WeightedPick16()



#ifdef __cplusplus
}
#endif

#endif  /* INITGAME_H */
