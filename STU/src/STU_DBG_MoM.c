/*
    STU_DBG_MoM -- see STU_DBG_MoM.h.
*/

#ifdef STU_DEBUG

#include "../../ext/stu_compat.h"   /* stu_stricmp() */

#include <assert.h>
#include <stddef.h>   /* size_t */
#include <string.h>   /* memcpy() */

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_TYPE.h"

#include "STU_LOG.h"

#include "../../MoM/src/Combat.h"  /* _combat_attacker_player, _combat_defender_player, _combat_total_unit_count, bus_* */

#include "STU_DBG_MoM.h"



/* moved from MoM/src/Settings.c */
void DBG_Print_MAGIC_SET(const char * label)
{
    int16_t itr;
    LOG_DEBUG(LOG_CAT_GENERAL, "===== MAGIC_SET [%s] =====", label);
    LOG_DEBUG(LOG_CAT_GENERAL, "  sound_effects:         %d", magic_set.sound_effects);
    LOG_DEBUG(LOG_CAT_GENERAL, "  background_music:      %d", magic_set.background_music);
    LOG_DEBUG(LOG_CAT_GENERAL, "  event_music:           %d", magic_set.event_music);
    LOG_DEBUG(LOG_CAT_GENERAL, "  city_spell_events:     %d", magic_set.city_spell_events);
    LOG_DEBUG(LOG_CAT_GENERAL, "  overland_spell_events: %d", magic_set.overland_spell_events);
    LOG_DEBUG(LOG_CAT_GENERAL, "  summoning_events:      %d", magic_set.summoning_events);
    LOG_DEBUG(LOG_CAT_GENERAL, "  end_of_turn_summary:   %d", magic_set.end_of_turn_summary);
    LOG_DEBUG(LOG_CAT_GENERAL, "  raze_city:             %d", magic_set.raze_city);
    LOG_DEBUG(LOG_CAT_GENERAL, "  random_events:         %d", magic_set.random_events);
    LOG_DEBUG(LOG_CAT_GENERAL, "  end_of_turn_wait:      %d", magic_set.end_of_turn_wait);
    LOG_DEBUG(LOG_CAT_GENERAL, "  strategic_combat_only: %d", magic_set.strategic_combat_only);
    LOG_DEBUG(LOG_CAT_GENERAL, "  auto_unit_information: %d", magic_set.auto_unit_information);
    LOG_DEBUG(LOG_CAT_GENERAL, "  enemy_moves:           %d", magic_set.enemy_moves);
    LOG_DEBUG(LOG_CAT_GENERAL, "  enemy_spells:          %d", magic_set.enemy_spells);
    LOG_DEBUG(LOG_CAT_GENERAL, "  spell_book_ordering:   %d", magic_set.spell_book_ordering);
    LOG_DEBUG(LOG_CAT_GENERAL, "  spell_animations:      %d", magic_set.spell_animations);
    LOG_DEBUG(LOG_CAT_GENERAL, "  show_node_owners:      %d", magic_set.show_node_owners);
    LOG_DEBUG(LOG_CAT_GENERAL, "  expanding_help:        %d", magic_set.expanding_help);
    LOG_DEBUG(LOG_CAT_GENERAL, "  sound_channels:        %d", magic_set.sound_channels);
    LOG_DEBUG(LOG_CAT_GENERAL, "  input_type:            %d", magic_set.input_type);
    LOG_DEBUG(LOG_CAT_GENERAL, "  Difficulty:            %d", magic_set.Difficulty);
    LOG_DEBUG(LOG_CAT_GENERAL, "  Opponents:             %d", magic_set.Opponents);
    LOG_DEBUG(LOG_CAT_GENERAL, "  LandSize:              %d", magic_set.LandSize);
    LOG_DEBUG(LOG_CAT_GENERAL, "  MagicPower:            %d", magic_set.MagicPower);
    LOG_DEBUG(LOG_CAT_GENERAL, "  movement_animations:   %d", magic_set.movement_animations);
    for(itr = 0; itr < NUM_SAVES; itr++)
    {
        LOG_DEBUG(LOG_CAT_GENERAL, "  Have_Save[%d]: %d  Save_Names[%d]: \"%s\"", itr, magic_set.Have_Save[itr], itr, magic_set.Save_Names[itr]);
    }
    LOG_DEBUG(LOG_CAT_GENERAL, "===== END MAGIC_SET [%s] =====", label);
}


void DBG_Compare_MAGIC_SET(const struct s_MAGIC_SET * before, const struct s_MAGIC_SET * after, const char * label)
{
    int16_t itr;
    int16_t diff_count = 0;
    LOG_DEBUG(LOG_CAT_GENERAL, "===== MAGIC_SET DIFF [%s] =====", label);
    if(before->sound_effects != after->sound_effects) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED sound_effects:         %d -> %d", before->sound_effects, after->sound_effects); diff_count++; }
    if(before->background_music != after->background_music) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED background_music:      %d -> %d", before->background_music, after->background_music); diff_count++; }
    if(before->event_music != after->event_music) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED event_music:           %d -> %d", before->event_music, after->event_music); diff_count++; }
    if(before->city_spell_events != after->city_spell_events) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED city_spell_events:     %d -> %d", before->city_spell_events, after->city_spell_events); diff_count++; }
    if(before->overland_spell_events != after->overland_spell_events) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED overland_spell_events: %d -> %d", before->overland_spell_events, after->overland_spell_events); diff_count++; }
    if(before->summoning_events != after->summoning_events) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED summoning_events:      %d -> %d", before->summoning_events, after->summoning_events); diff_count++; }
    if(before->end_of_turn_summary != after->end_of_turn_summary) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED end_of_turn_summary:   %d -> %d", before->end_of_turn_summary, after->end_of_turn_summary); diff_count++; }
    if(before->raze_city != after->raze_city) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED raze_city:             %d -> %d", before->raze_city, after->raze_city); diff_count++; }
    if(before->random_events != after->random_events) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED random_events:         %d -> %d", before->random_events, after->random_events); diff_count++; }
    if(before->end_of_turn_wait != after->end_of_turn_wait) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED end_of_turn_wait:      %d -> %d", before->end_of_turn_wait, after->end_of_turn_wait); diff_count++; }
    if(before->strategic_combat_only != after->strategic_combat_only) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED strategic_combat_only: %d -> %d", before->strategic_combat_only, after->strategic_combat_only); diff_count++; }
    if(before->auto_unit_information != after->auto_unit_information) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED auto_unit_information: %d -> %d", before->auto_unit_information, after->auto_unit_information); diff_count++; }
    if(before->enemy_moves != after->enemy_moves) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED enemy_moves:           %d -> %d", before->enemy_moves, after->enemy_moves); diff_count++; }
    if(before->enemy_spells != after->enemy_spells) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED enemy_spells:          %d -> %d", before->enemy_spells, after->enemy_spells); diff_count++; }
    if(before->spell_book_ordering != after->spell_book_ordering) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED spell_book_ordering:   %d -> %d", before->spell_book_ordering, after->spell_book_ordering); diff_count++; }
    if(before->spell_animations != after->spell_animations) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED spell_animations:      %d -> %d", before->spell_animations, after->spell_animations); diff_count++; }
    if(before->show_node_owners != after->show_node_owners) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED show_node_owners:      %d -> %d", before->show_node_owners, after->show_node_owners); diff_count++; }
    if(before->expanding_help != after->expanding_help) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED expanding_help:        %d -> %d", before->expanding_help, after->expanding_help); diff_count++; }
    if(before->sound_channels != after->sound_channels) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED sound_channels:        %d -> %d", before->sound_channels, after->sound_channels); diff_count++; }
    if(before->input_type != after->input_type) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED input_type:            %d -> %d", before->input_type, after->input_type); diff_count++; }
    if(before->Difficulty != after->Difficulty) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED Difficulty:            %d -> %d", before->Difficulty, after->Difficulty); diff_count++; }
    if(before->Opponents != after->Opponents) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED Opponents:             %d -> %d", before->Opponents, after->Opponents); diff_count++; }
    if(before->LandSize != after->LandSize) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED LandSize:              %d -> %d", before->LandSize, after->LandSize); diff_count++; }
    if(before->MagicPower != after->MagicPower) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED MagicPower:            %d -> %d", before->MagicPower, after->MagicPower); diff_count++; }
    if(before->movement_animations != after->movement_animations) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED movement_animations:   %d -> %d", before->movement_animations, after->movement_animations); diff_count++; }
    for(itr = 0; itr < NUM_SAVES; itr++)
    {
        if(before->Have_Save[itr] != after->Have_Save[itr]) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED Have_Save[%d]:          %d -> %d", itr, before->Have_Save[itr], after->Have_Save[itr]); diff_count++; }
        if(stu_stricmp(before->Save_Names[itr], after->Save_Names[itr]) != 0) { LOG_DEBUG(LOG_CAT_GENERAL, "  CHANGED Save_Names[%d]:         \"%s\" -> \"%s\"", itr, before->Save_Names[itr], after->Save_Names[itr]); diff_count++; }
    }
    if(diff_count == 0) { LOG_DEBUG(LOG_CAT_GENERAL, "  (no changes)"); }
    else { LOG_DEBUG(LOG_CAT_GENERAL, "  Total changes: %d", diff_count); }
    LOG_DEBUG(LOG_CAT_GENERAL, "===== END MAGIC_SET DIFF [%s] =====", label);
}



/* moved from MoM/src/MOM_DBG.c */
/*
_HEROES2

_num_players
_landsize
_magic
_difficulty
_cities
_units
_turn
_unit

_players

_world_maps  // 4800 * 2 = 9600

connectivity_grid_land
connectivity_grid_sea
_landmasses

_NODES
_FORTRESSES
_TOWERS
_LAIRS
_ITEMS
_CITIES
_UNITS

_map_square_terrain_specials
_square_explored
movement_mode_cost_maps
events_table
_map_square_flags

grand_vizier

_prefab_item_table
hero_names_table

*/

void Test_Run(void)
{
    size_t test_size;

    test_size = sizeof(struct s_WIZARD);
    test_size = sizeof(struct s_NODE);
    test_size = sizeof(struct s_FORTRESS);
    test_size = sizeof(struct s_TOWER);
    test_size = sizeof(struct s_LAIR);
    test_size = sizeof(struct s_ITEM);
    test_size = sizeof(struct s_CITY);
    test_size = sizeof(struct s_UNIT);

    assert(sizeof(struct s_HERO) == HERO_RECORD_SIZE);  // _HEROES2

    assert(sizeof(_num_players) == 2);
    assert(sizeof(_landsize) == 2);
    assert(sizeof(_magic) == 2);
    assert(sizeof(_difficulty) == 2);
    assert(sizeof(_cities) == 2);
    assert(sizeof(_units) == 2);
    assert(sizeof(_turn) == 2);
    assert(sizeof(_unit) == 2);

    assert(sizeof(struct s_WIZARD) == WIZARD_RECORD_SIZE);  // 1224  _players
    // _world_maps  // 4800 * 2 = 9600
    // connectivity_grid_land
    // connectivity_grid_sea
    // _landmasses
    assert(sizeof(struct s_NODE) == NODE_RECORD_SIZE);          //   48  _NODES
    assert(sizeof(struct s_FORTRESS) == FORTRESS_RECORD_SIZE);  //    4  _FORTRESSES
    assert(sizeof(struct s_TOWER) == TOWER_RECORD_SIZE);        //    4  _TOWERS
    assert(sizeof(struct s_LAIR) == LAIR_RECORD_SIZE);          //   24  _LAIRS
    assert(sizeof(struct s_ITEM) == ITEM_RECORD_SIZE);          //   50  _ITEMS
    assert(sizeof(struct s_CITY) == CITY_RECORD_SIZE);          //  114  _CITIES
    assert(sizeof(struct s_UNIT) == UNIT_RECORD_SIZE);          //   32  _UNITS

    // _map_square_terrain_specials
    // _square_explored
    // movement_mode_cost_maps
    // events_table
    // _map_square_flags
    assert(sizeof(grand_vizier) == 2);
    // _prefab_item_table
    // hero_names_table

}



/* moved from MoM/src/Combat.c */
/* the combat move-path knobs, declared in STU_DBG_Combat_Move_Path.h with the macros that use them */
int16_t DBG_cgx = 0;  // 14
int16_t DBG_cgy = 0;  // 12
int16_t DBG_player_idx = HUMAN_PLAYER_IDX;
int16_t DBG_target_cgx = 0;  // 13 W
int16_t DBG_target_cgy = 0;  // 11 N
int16_t DBG_PFA_1311_set = ST_FALSE;
int16_t DBG_PFA_1411_set = ST_FALSE;
int16_t DBG_PFA_1511_set = ST_FALSE;
/* CLAUDE */ int16_t DBG_atk_active, DBG_atk_dead, DBG_atk_gone, DBG_def_active, DBG_def_dead, DBG_def_gone;

/* moved from MoX/src/MOM_DAT.c */
/* CLAUDE */ struct s_BATTLE_UNIT DBG_battle_units[MAX_BATTLE_UNIT_SLOT_COUNT];  // not in the asm; used for debugging purposes in Combat()  (was MAX_BATTLE_UNIT_COUNT=18, too small when summoned units push count past 18)



/* moved from MoM/src/Combat.c (the inline counter block in the combat loop) */
void DBG_Count_Battle_Units(void)
{
    int16_t dbg_i;
    DBG_atk_active=0; DBG_atk_dead=0; DBG_atk_gone=0; DBG_def_active=0; DBG_def_dead=0; DBG_def_gone=0; for(dbg_i=0; dbg_i<_combat_total_unit_count; dbg_i++) { if(battle_units[dbg_i].controller_idx==_combat_attacker_player) { if(battle_units[dbg_i].status==bus_Active) DBG_atk_active++; else if(battle_units[dbg_i].status==bus_Dead) DBG_atk_dead++; else if(battle_units[dbg_i].status==bus_Gone) DBG_atk_gone++; } else if(battle_units[dbg_i].controller_idx==_combat_defender_player) { if(battle_units[dbg_i].status==bus_Active) DBG_def_active++; else if(battle_units[dbg_i].status==bus_Dead) DBG_def_dead++; else if(battle_units[dbg_i].status==bus_Gone) DBG_def_gone++; } }
}



/* moved from MoM/src/Combat.c */
/* CLAUDE: copy battle_units[] into DBG_battle_units[] for DBG_Compare_Battle_Units(); the count is clamped to the array, so a bad _combat_total_unit_count cannot overrun it */
void DBG_Snapshot_Battle_Units(void)
{
    int16_t count = _combat_total_unit_count;
    LOG_DEBUG(LOG_CAT_COMBAT, "[Combat_Screen] snapshot: _units=%d  _combat_total_unit_count=%d", _units, _combat_total_unit_count);
    if(count < 0)
    {
        LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Snapshot_Battle_Units] BOGUS _combat_total_unit_count=%d, nothing copied", _combat_total_unit_count);
        return;
    }
    if(count > MAX_BATTLE_UNIT_SLOT_COUNT)
    {
        LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Snapshot_Battle_Units] _combat_total_unit_count=%d is past MAX_BATTLE_UNIT_SLOT_COUNT=%d, copying %d", _combat_total_unit_count, MAX_BATTLE_UNIT_SLOT_COUNT, MAX_BATTLE_UNIT_SLOT_COUNT);
        count = MAX_BATTLE_UNIT_SLOT_COUNT;
    }
    memcpy(DBG_battle_units, battle_units, (sizeof(struct s_BATTLE_UNIT) * count));
}

/* CLAUDE */
void DBG_Compare_Battle_Units(const char * label)
{
    int16_t itr;
    int16_t mismatch_count = 0;
    struct s_BATTLE_UNIT * live;
    struct s_BATTLE_UNIT * snap;
    LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] %s: _combat_total_unit_count=%d, battle_units=%p, DBG_battle_units=%p", label, _combat_total_unit_count, (void *)battle_units, (void *)DBG_battle_units);
    if(_combat_total_unit_count < 0 || _combat_total_unit_count > MAX_BATTLE_UNIT_SLOT_COUNT)
    {
        LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] %s: BOGUS _combat_total_unit_count=%d, aborting compare", label, _combat_total_unit_count);
        return;
    }
    for(itr = 0; itr < _combat_total_unit_count && itr < MAX_BATTLE_UNIT_SLOT_COUNT; itr++)
    {
        live = &battle_units[itr];
        snap = &DBG_battle_units[itr];
        if(snap->unit_idx < 0 || snap->unit_idx > _units || snap->status < bus_Active || snap->status > bus_Gone || snap->controller_idx < 0 || snap->controller_idx > NEUTRAL_PLAYER_IDX)
        {
            LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] SNAPSHOT ALREADY GARBAGE [%d]: unit_idx=%d  status=%d  controller_idx=%d  (_units=%d)", itr, snap->unit_idx, snap->status, snap->controller_idx, _units);
        }
        if(live->unit_idx < 0 || live->unit_idx > _units || live->status < bus_Active || live->status > bus_Gone || live->controller_idx < 0 || live->controller_idx > NEUTRAL_PLAYER_IDX)
        {
            LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] LIVE DATA GARBAGE [%d]: unit_idx=%d  status=%d  controller_idx=%d  (_units=%d)", itr, live->unit_idx, live->status, live->controller_idx, _units);
        }
        if(memcmp(live, snap, sizeof(struct s_BATTLE_UNIT)) != 0)
        {
            mismatch_count++;
            LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] MISMATCH battle_units[%d]:", itr);
            if(live->unit_idx != snap->unit_idx) { LOG_DEBUG(LOG_CAT_COMBAT, "  unit_idx:        live=%d  snap=%d", live->unit_idx, snap->unit_idx); }
            if(live->status != snap->status) { LOG_DEBUG(LOG_CAT_COMBAT, "  status:          live=%d  snap=%d", live->status, snap->status); }
            if(live->controller_idx != snap->controller_idx) { LOG_DEBUG(LOG_CAT_COMBAT, "  controller_idx:  live=%d  snap=%d", live->controller_idx, snap->controller_idx); }
            if(live->melee != snap->melee) { LOG_DEBUG(LOG_CAT_COMBAT, "  melee:           live=%d  snap=%d", live->melee, snap->melee); }
            if(live->ranged != snap->ranged) { LOG_DEBUG(LOG_CAT_COMBAT, "  ranged:          live=%d  snap=%d", live->ranged, snap->ranged); }
            if(live->defense != snap->defense) { LOG_DEBUG(LOG_CAT_COMBAT, "  defense:         live=%d  snap=%d", live->defense, snap->defense); }
            if(live->resist != snap->resist) { LOG_DEBUG(LOG_CAT_COMBAT, "  resist:          live=%d  snap=%d", live->resist, snap->resist); }
            if(live->hits != snap->hits) { LOG_DEBUG(LOG_CAT_COMBAT, "  hits:            live=%d  snap=%d", live->hits, snap->hits); }
            if(live->figure_cnt != snap->figure_cnt) { LOG_DEBUG(LOG_CAT_COMBAT, "  figure_cnt:     live=%d  snap=%d", live->figure_cnt, snap->figure_cnt); }
            if(live->figure_max != snap->figure_max) { LOG_DEBUG(LOG_CAT_COMBAT, "  figure_max:     live=%d  snap=%d", live->figure_max, snap->figure_max); }
            if(live->front_figure_damage != snap->front_figure_damage) { LOG_DEBUG(LOG_CAT_COMBAT, "  front_fig_dmg:   live=%d  snap=%d", live->front_figure_damage, snap->front_figure_damage); }
            if(live->cgx != snap->cgx) { LOG_DEBUG(LOG_CAT_COMBAT, "  cgx:             live=%d  snap=%d", live->cgx, snap->cgx); }
            if(live->cgy != snap->cgy) { LOG_DEBUG(LOG_CAT_COMBAT, "  cgy:             live=%d  snap=%d", live->cgy, snap->cgy); }
            if(live->enchantments != snap->enchantments) { LOG_DEBUG(LOG_CAT_COMBAT, "  enchantments:    live=0x%08X  snap=0x%08X", live->enchantments, snap->enchantments); }
            if(live->combat_effects != snap->combat_effects) { LOG_DEBUG(LOG_CAT_COMBAT, "  combat_effects:  live=0x%04X  snap=0x%04X", live->combat_effects, snap->combat_effects); }
            if(live->race != snap->race) { LOG_DEBUG(LOG_CAT_COMBAT, "  race:            live=%d  snap=%d", live->race, snap->race); }
            if(live->movement_points != snap->movement_points) { LOG_DEBUG(LOG_CAT_COMBAT, "  movement_points: live=%d  snap=%d", live->movement_points, snap->movement_points); }
            if(live->Abilities != snap->Abilities) { LOG_DEBUG(LOG_CAT_COMBAT, "  Abilities:       live=0x%04X  snap=0x%04X", live->Abilities, snap->Abilities); }
            if(live->item_enchantments != snap->item_enchantments) { LOG_DEBUG(LOG_CAT_COMBAT, "  item_enchants:   live=0x%08X  snap=0x%08X", live->item_enchantments, snap->item_enchantments); }
        }
    }
    if(mismatch_count == 0)
    {
        LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] %s: all %d battle units match snapshot", label, itr);
    }
    else
    {
        LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] %s: %d of %d battle units DIFFER from snapshot", label, mismatch_count, itr);
    }
}

#endif  /* STU_DEBUG */
