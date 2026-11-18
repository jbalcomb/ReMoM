/**
 * DeMoM.c — Windowless ReMoM
 *
 * An entry point for windowless/automated operation:
 *   --newgame [ReMoM.ini]   Create a new game from config (skip menus)
 *   --load FILE.GAM         Load a save file (skip menus): SAVE1..SAVE9.GAM keep their slot; any other name or path loads that file
 *   --continue              Same as 'Main Menu ' Continue (loads SAVE9.GAM)
 *   --menu                  Start at the Main Menu with no game (a fresh process, as ReMoM starts)
 *   --replay game.RMR       Replay recorded input
 *   --record game.RMR       Record input to file
 *   --scenario gameplay.HMS play a scripted scenario (uses Automation Driver)
 *
 * No logos, no credits, no main menu music. Initializes the engine,
 * creates or loads a game, and enters Screen_Control().
 *
 * DeMoM is hardwired to the windowless platform library — no display, no
 * audio, no external dependencies — regardless of which windowed backend
 * the build's preset chose for ReMoM.
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "../ext/stu_compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../STU/src/AI_METRICS.h"
#include "../STU/src/STU_DBG.h"
#include "../STU/src/STU_BRAK.h"
#include "../STU/src/STU_LOG.h"
#include "../STU/src/STU_GRAF.h"

#include "../platform/include/Platform.h"
#include "../platform/include/Platform_Perf.h"  /* Perf_Init() -- Mode A windowless logic timing */
#include "../platform/include/Platform_Replay.h"
#include "../platform/include/Platform_Keys.h"

#include "../MoX/src/MOM_DAT.h"
#include "../MoX/src/MOX_DAT.h"
#include "../MoX/src/MOX_DEF.h"
#include "../MoX/src/DOS.h"
#include "../MoX/src/MOX_TYPE.h"
#include "../MoX/src/MOX_T4.h"
#include "../MoX/src/Fields.h"
#include "../MoX/src/LBX_Load.h"
#include "../MoX/src/Allocate_Pool.h"  /* CLAUDE: Pool_Bytes_Peak() - static-pool high-water mark logged at shutdown */
#include "../MoX/src/LOADSAVE.h"
#include "../MoX/src/random.h"
#include "../MoX/src/MOX2.h"  /* CLAUDE: Check_Command_Line_Parameters_() */
#include "../MoX/src/MOX_SET.h"   /* magic_set.Save_Names[] */
#include "../MoX/src/Fonts.h"     /* Load_Palette(), Apply_Palette() -- --menu */

#include "../MoM/src/SPLMASTR.h"  /* CLAUDE: m_magic_winner_idx -- reset on the --load path */
#include "../MoM/src/NewGame.h"
#include "../MoM/src/MAPGEN.h"
#include "../MoM/src/INITGAME.h"
#include "../MoM/src/LoadScr.h"
#include "../MoM/src/LOADER.h"   /* Load_WZD_Resources */
#include "../MoM/src/CREDITS.h"  /* Init_Credits() */
#include "../MoM/src/MOM_SCR.h"
#include "../MoM/src/Settings.h"
#include "../STU/src/STU_DBG_MoM.h"  /* DBG_Print_MAGIC_SET(), DBG_Compare_MAGIC_SET() */
#include "../MoM/src/UNITTYPE.h"
#include "../MoM/src/Combat.h"   /* CLAUDE: Combat() + combat_results_gold for --combat test mode */
#include "../MoM/src/DIPLODEF.h" /* CLAUDE: DIPL_War for --combat test mode */
#include "../MoM/src/CITYCALC.h"  /* CLAUDE: Kill_Unit() for --cast-summon test mode */
#include "../MoM/src/NEXTTURN.h"  /* CLAUDE: Delete_Dead_Units() for --cast-summon test mode */
#include "../MoM/src/OverSpel.h"  /* CLAUDE: Cast_Spell_Overland() for --cast-summon test mode */
#include "../MoM/src/Spellbook.h" /* CLAUDE: spl_ enum for --cast-summon test mode */
#include "../MoM/src/SBookScr.h"  /* CLAUDE: Full_Draw_Main_Screen() for --cast-summon test mode */
#include "../MoM/src/Integrity_Check.h"  /* CLAUDE: Integrity_Checkpoint() at shutdown */
#include "../MoM/src/WZD_o143.h"  /* Random_City_Name_By_Race */
#include "../MoM/src/AIDATA.h"   /* CLAUDE: Allocate_AI_Data() for --ai-dead-units test mode */
#include "../MoM/src/AIDUDES.h"  /* CLAUDE: AI_Next_Turn(), AI_Player_Calculate_Target_Values() for --ai-dead-units test mode */
#include "../STU/src/STU_GARB.h"  /* CLAUDE: Get/Set_Stack_Garbage_Seed() for --ai-dead-units test mode */
#include "../MoX/src/EMS/EMS.h"  /* CLAUDE: EMMDATAH_Map() for --ai-dead-units test mode */

#include "ReMoM_Init.h"
#include "Automation_Driver.h"
#include "Game_Save_Dump.h"

/* _wizard_presets_table is defined in NewGame.c but has no extern in a header */
extern struct s_WIZARD_PRESET _wizard_presets_table[];

/* NewGame.c */
// MGC  dseg:309C
extern char str_MAGIC_SET__ovr050[];
extern char str_rb__ovr050[];
extern char str_wb__ovr050[];



/* DeMoM is always windowless — no SDL_main / SDL header pull-in needed. */



/* ========================================================================= */
/*  Forward Declarations                                                     */
/* ========================================================================= */

void Startup_Platform(void);
void Shutdown_Platform(void);

// extern char MOM_FONT_FILE[];
char MOM_FONT_FILE[] = "FONTS.LBX";
extern int16_t fields_count;
extern struct s_Field *p_fields;
// NewGame.c
// MGC  dseg:3090
extern char newgame_lbx_file__ovr050[];
// MGC  dseg:8DD6
extern SAMB_ptr newgame_BUILDWOR_map_build_bar_seg;
// MainScr.c
// WZD dseg:2E14
// MoO2  _given_chance_to_rename_home_star
extern int16_t _given_chance_to_rename_home_city;



/* ========================================================================= */
/*  Utility Functions                                                        */
/* ========================================================================= */

static char *Trim_Whitespace(char *str)
{
    char *end;
    while(*str && isspace((unsigned char)*str)) { str++; }
    if(*str == '\0') { return str; }
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) { *end-- = '\0'; }
    return str;
}

static int Str_Equal_CI(const char *a, const char *b)
{
    return (stu_stricmp(a, b) == 0);
}

/* --load FILE: the save slot (0..8) when FILE is exactly SAVE1.GAM .. SAVE9.GAM (any case, no
   directory part) -- the name Load_SAVE_GAM() builds from that slot.  Any other name or path
   returns ST_UNDEFINED, and the caller loads that exact file through
   g_load_save_gam_name_override.  (It used to read the slot from the characters after the first
   four, so "dir/SAVE1.GAM" or "SAVE_FIXTURE.GAM" became slot -1 and silently loaded SAVETEST.GAM.) */
static int16_t Save_Slot_From_File_Name(const char *name)
{
    if(
        (strlen(name) == 9) &&
        (stu_strnicmp(name, "SAVE", 4) == 0) &&
        (name[4] >= '1') &&
        (name[4] <= '9') &&
        (stu_stricmp(name + 5, ".GAM") == 0)
    )
    {
        return (int16_t)(name[4] - '1');
    }
    return ST_UNDEFINED;
}

static int Lookup_Wizard_By_Name(const char *name)
{
    int itr;
    for (itr = 0; itr < 14; itr++)
    {
        if(Str_Equal_CI(name, _wizard_presets_table[itr].name)) { return itr; }
    }
    return -1;
}

static int Lookup_Race_By_Name(const char *name)
{
    static const char *race_names[] = { "Barbarian", "Beastmen", "Dark Elf", "Draconian", "Dwarf", "Gnoll", "Halfling", "High Elf", "High Men", "Klackon", "Lizardman", "Nomad", "Orc", "Troll" };
    int itr;
    for (itr = 0; itr < 14; itr++)
    {
        if(Str_Equal_CI(name, race_names[itr])) { return itr; }
    }
    return -1;
}

static int Lookup_Banner_By_Name(const char *name)
{
    if(Str_Equal_CI(name, "Blue"))   { return BNR_Blue; }
    if(Str_Equal_CI(name, "Green"))  { return BNR_Green; }
    if(Str_Equal_CI(name, "Purple")) { return BNR_Purple; }
    if(Str_Equal_CI(name, "Red"))    { return BNR_Red; }
    if(Str_Equal_CI(name, "Yellow")) { return BNR_Yellow; }
    if(Str_Equal_CI(name, "Brown"))  { return BNR_Brown; }
    return -1;
}

static int Parse_Bool_TF(const char *value)
{
    if(Str_Equal_CI(value, "T") || Str_Equal_CI(value, "True") || Str_Equal_CI(value, "1")) { return 1; }
    if(Str_Equal_CI(value, "F") || Str_Equal_CI(value, "False") || Str_Equal_CI(value, "0")) { return 0; }
    return -1;
}



/* ========================================================================= */
/*  ReMoM.ini Configuration                                                 */
/* ========================================================================= */

struct s_DeMoM_Config
{
    int has_difficulty;
    int has_magic;
    int has_landsize;
    int has_opponents;
    int has_wizard;
    int has_race;
    int has_banner;
    int has_name;
    int has_seed;

    int16_t difficulty;
    int16_t magic;
    int16_t landsize;
    int16_t opponents;
    int16_t wizard_id;
    int16_t race;
    int16_t banner;
    char    wizard_name[LEN_WIZARD_NAME];
    uint32_t seed;

    int has_books;
    int16_t books_nature;
    int16_t books_sorcery;
    int16_t books_chaos;
    int16_t books_life;
    int16_t books_death;

    int has_retorts;
    int8_t retorts[18];

    int has_home_city_name;
    int has_save_slot;
    int has_save_name;

    char    home_city_name[LEN_CITY_NAME];
    int16_t save_slot;
    char    save_name[LEN_SAVE_DESCRIPTION];
};

static void Config_Init(struct s_DeMoM_Config *cfg)
{
    int itr;
    memset(cfg, 0, sizeof(*cfg));
    cfg->difficulty = god_Easy;
    cfg->magic = gom_Normal;
    cfg->landsize = gol_Medium;
    cfg->opponents = goo_One;
    cfg->wizard_id = 0;
    cfg->race = R_High_Man;
    cfg->banner = BNR_Blue;
    stu_strcpy(cfg->wizard_name, "Merlin");
    cfg->books_nature = -1;
    cfg->books_sorcery = -1;
    cfg->books_chaos = -1;
    cfg->books_life = -1;
    cfg->books_death = -1;
    for (itr = 0; itr < 18; itr++) { cfg->retorts[itr] = -1; }
    cfg->save_slot = -1;
}

static int Config_Parse(const char *filepath, struct s_DeMoM_Config *cfg)
{
    FILE *fp;
    char line[512];
    char *key;
    char *value;
    char *eq;
    int line_num = 0;
    int val_int;

    fp = stu_fopen_ci(filepath, "r");
    if(fp == NULL)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Could not open config: %s", filepath);
        return 0;
    }

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Loading config: %s", filepath);

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        line_num++;
        key = Trim_Whitespace(line);

        if(*key == '\0' || *key == '#' || *key == ';') { continue; }
        if(*key == '[') { continue; }

        eq = stu_strchr(key, '=');
        if(eq == NULL)
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] %s:%d: missing '=' in: %s", filepath, line_num, key);
            continue;
        }

        *eq = '\0';
        value = Trim_Whitespace(eq + 1);
        key = Trim_Whitespace(key);

        if(Str_Equal_CI(key, "difficulty"))
        {
            val_int = stu_atoi(value);
            if(Str_Equal_CI(value, "Intro"))           { val_int = god_Intro; }
            else if(Str_Equal_CI(value, "Easy"))       { val_int = god_Easy; }
            else if(Str_Equal_CI(value, "Normal"))     { val_int = god_Normal; }
            else if(Str_Equal_CI(value, "Hard"))       { val_int = god_Hard; }
            else if(Str_Equal_CI(value, "Impossible")) { val_int = god_Impossible; }
            cfg->difficulty = (int16_t)val_int;
            cfg->has_difficulty = 1;
        }
        else if(Str_Equal_CI(key, "magic"))
        {
            val_int = stu_atoi(value);
            if(Str_Equal_CI(value, "Weak"))          { val_int = gom_Weak; }
            else if(Str_Equal_CI(value, "Normal"))   { val_int = gom_Normal; }
            else if(Str_Equal_CI(value, "Powerful")) { val_int = gom_Powerful; }
            cfg->magic = (int16_t)val_int;
            cfg->has_magic = 1;
        }
        else if(Str_Equal_CI(key, "landsize"))
        {
            val_int = stu_atoi(value);
            if(Str_Equal_CI(value, "Small"))       { val_int = gol_Small; }
            else if(Str_Equal_CI(value, "Medium"))  { val_int = gol_Medium; }
            else if(Str_Equal_CI(value, "Large"))   { val_int = gol_Large; }
            cfg->landsize = (int16_t)val_int;
            cfg->has_landsize = 1;
        }
        else if(Str_Equal_CI(key, "opponents"))
        {
            cfg->opponents = (int16_t)stu_atoi(value);
            cfg->has_opponents = 1;
        }
        else if(Str_Equal_CI(key, "seed"))
        {
            cfg->seed = (uint32_t)strtoul(value, NULL, 0);
            cfg->has_seed = 1;
        }
        else if(Str_Equal_CI(key, "wizard"))
        {
            val_int = Lookup_Wizard_By_Name(value);
            if(val_int >= 0)
            {
                cfg->wizard_id = (int16_t)val_int;
                stu_strcpy(cfg->wizard_name, _wizard_presets_table[val_int].name);
            }
            else
            {
                LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] %s:%d: unknown wizard '%s'", filepath, line_num, value);
            }
            cfg->has_wizard = 1;
        }
        else if(Str_Equal_CI(key, "name"))
        {
            stu_strncpy(cfg->wizard_name, value, 19);
            cfg->wizard_name[19] = '\0';
            cfg->has_name = 1;
        }
        else if(Str_Equal_CI(key, "race"))
        {
            val_int = Lookup_Race_By_Name(value);
            if(val_int >= 0)
            {
                cfg->race = (int16_t)val_int;
            }
            else
            {
                LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] %s:%d: unknown race '%s'", filepath, line_num, value);
            }
            cfg->has_race = 1;
        }
        else if(Str_Equal_CI(key, "banner"))
        {
            val_int = Lookup_Banner_By_Name(value);
            if(val_int >= 0)
            {
                cfg->banner = (int16_t)val_int;
            }
            else
            {
                LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] %s:%d: unknown banner '%s'", filepath, line_num, value);
            }
            cfg->has_banner = 1;
        }
        else if(Str_Equal_CI(key, "books_nature"))  { cfg->books_nature = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if(Str_Equal_CI(key, "books_sorcery")) { cfg->books_sorcery = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if(Str_Equal_CI(key, "books_chaos"))   { cfg->books_chaos = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if(Str_Equal_CI(key, "books_life"))    { cfg->books_life = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if(Str_Equal_CI(key, "books_death"))   { cfg->books_death = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if(Str_Equal_CI(key, "home_city_name"))
        {
            stu_strncpy(cfg->home_city_name, value, LEN_CITY_NAME - 1);
            cfg->home_city_name[LEN_CITY_NAME - 1] = '\0';
            cfg->has_home_city_name = 1;
        }
        else if(Str_Equal_CI(key, "save_slot"))
        {
            cfg->save_slot = (int16_t)stu_atoi(value);
            cfg->has_save_slot = 1;
        }
        else if(Str_Equal_CI(key, "save_name"))
        {
            stu_strncpy(cfg->save_name, value, LEN_SAVE_DESCRIPTION - 1);
            cfg->save_name[LEN_SAVE_DESCRIPTION - 1] = '\0';
            cfg->has_save_name = 1;
        }
        else if(Str_Equal_CI(key, "Alchemy"))          { cfg->retorts[wsa_Alchemy] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Warlord"))          { cfg->retorts[wsa_Warlord] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Chaos Mastery"))    { cfg->retorts[wsa_Chaos_Mastery] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Nature Mastery"))   { cfg->retorts[wsa_Nature_Mastery] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Sorcery Mastery"))  { cfg->retorts[wsa_Sorcery_Mastery] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Infernal Power"))   { cfg->retorts[wsa_Infernal_Power] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Divine Power"))     { cfg->retorts[wsa_Divine_Power] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Sage Master"))      { cfg->retorts[wsa_Sage_Master] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Channeler"))        { cfg->retorts[wsa_Channeller] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Myrran"))           { cfg->retorts[wsa_Myrran] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Archmage"))         { cfg->retorts[wsa_Archmage] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Mana Focusing"))    { cfg->retorts[wsa_Mana_Focusing] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Node Mastery"))     { cfg->retorts[wsa_Node_Mastery] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Famous"))           { cfg->retorts[wsa_Famous] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Runemaster"))       { cfg->retorts[wsa_Runemaster] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Conjurer"))         { cfg->retorts[wsa_Conjurer] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Charismatic"))      { cfg->retorts[wsa_Charismatic] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if(Str_Equal_CI(key, "Artificer"))        { cfg->retorts[wsa_Artificer] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] %s:%d: unknown key '%s'", filepath, line_num, key);
        }
    }

    stu_fclose(fp);
    return 1;
}



/* ========================================================================= */
/*  New Game Creation                                                        */
/* ========================================================================= */

/* These globals live in NewGame.c and are not declared in NewGame.h.  We
   reference them here so DeMoM can re-fill _player_start_spells[0] after
   overriding the human player's book counts.  Kept as local externs to
   avoid touching the game-code headers. */
extern int16_t m_select_count_common[10];
extern int16_t m_select_count_uncommon[10];
extern struct s_DEFAULT_SPELLS _default_spells[5];
extern struct s_Init_Base_Realms _player_start_spells[PLAYER_COUNT_MAX];

/* Verbatim local copy of the per-realm start-spell fill block from
   Human_Player_Wizard_Profile (NewGame.c).  Read the human player's
   current spellranks[] and rewrite _player_start_spells[0] from
   _default_spells[] accordingly.

   Reason for the local copy: DeMoM overrides spellranks[] AFTER
   Human_Player_Wizard_Profile has already run, so the original inline
   fill becomes stale.  Re-running this against the post-override
   spellranks brings _player_start_spells[0] back in sync with the
   configured books — without that, Init_Computer_Players_Spell_Library
   sees a stale start-spell pool and produces a different spells_list[]
   than ReMoM's menu path, which surfaces as a Layer-A divergence inside
   Player_Research_Spells (Spells128.c:121).

   See doc/Devel-DeMoM-Newgame-Path-Alignment.md gap #2.

   Behavior is verbatim — including the pre-existing off-by-one indexing
   in the m_select_count_* lookups (doc gap #5).  Do not "fix" the
   indexing here: bug-for-bug equivalence with the game code is what
   keeps the RNG trace aligned. */
static void DeMoM_Refill_Human_Player_Start_Spells(void)
{
    int16_t itr = 0;
    int16_t spellranks = 0;

    for(itr = 0; itr < 13; itr++)
    {
        _player_start_spells[0].realms[sbr_Nature].spells[itr]  = 0;
        _player_start_spells[0].realms[sbr_Sorcery].spells[itr] = 0;
        _player_start_spells[0].realms[sbr_Chaos].spells[itr]   = 0;
        _player_start_spells[0].realms[sbr_Life].spells[itr]    = 0;
        _player_start_spells[0].realms[sbr_Death].spells[itr]   = 0;
    }

    spellranks = _players[0].spellranks[sbr_Nature];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Nature].spells[( 0 + itr)] = _default_spells[sbr_Nature].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Nature].spells[(10 + itr)] = _default_spells[sbr_Nature].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Nature].spells[(12 + itr)] = _default_spells[sbr_Nature].spells[(12 + itr)];
        }
    }

    spellranks = _players[0].spellranks[sbr_Sorcery];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Sorcery].spells[( 0 + itr)] = _default_spells[sbr_Sorcery].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Sorcery].spells[(10 + itr)] = _default_spells[sbr_Sorcery].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Sorcery].spells[(12 + itr)] = _default_spells[sbr_Sorcery].spells[(12 + itr)];
        }
    }

    spellranks = _players[0].spellranks[sbr_Chaos];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Chaos].spells[( 0 + itr)] = _default_spells[sbr_Chaos].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Chaos].spells[(10 + itr)] = _default_spells[sbr_Chaos].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Chaos].spells[(12 + itr)] = _default_spells[sbr_Chaos].spells[(12 + itr)];
        }
    }

    spellranks = _players[0].spellranks[sbr_Life];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Life].spells[( 0 + itr)] = _default_spells[sbr_Life].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Life].spells[(10 + itr)] = _default_spells[sbr_Life].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Life].spells[(12 + itr)] = _default_spells[sbr_Life].spells[(12 + itr)];
        }
    }

    spellranks = _players[0].spellranks[sbr_Death];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Death].spells[( 0 + itr)] = _default_spells[sbr_Death].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Death].spells[(10 + itr)] = _default_spells[sbr_Death].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Death].spells[(12 + itr)] = _default_spells[sbr_Death].spells[(12 + itr)];
        }
    }
}

/*
This is meant to match the OG-MoM code that finishes main() in MAGIC.EXE
and launches WIZARDS.EXE.
Effectively, after a 'New Game', you fall into the 'Continue' program-flow.
*/
static void New_Game_Continue(void)
{
    /* HACK */  Load_WZD_Resources();
    Load_SAVE_GAM(8);
    Loaded_Game_Update();
    current_screen = scr_Main_Screen;
}
static void New_Game_Settings(struct s_DeMoM_Config *cfg)
{
    char file_found[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FILE * file_pointer = 0;
#ifdef STU_DEBUG
    struct s_MAGIC_SET magic_set_snapshot;
#endif

/*
    BEGIN: Game Settings Load
*/
    if((STU_GRAF_User_DIR(str_MAGIC_SET__ovr050, file_found) == 0) || (STU_GRAF_User_LOF(str_MAGIC_SET__ovr050) == 0))  /* CLAUDE: -> user-data (WINDOWLESS: CWD) */
    {
        Set_Default_Game_Settings();
    }
    else
    {
        file_pointer = STU_GRAF_Open_User(str_MAGIC_SET__ovr050, str_rb__ovr050);  /* CLAUDE: -> user-data */
        fread(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_pointer);
        fclose(file_pointer);
    }
    if((magic_set.Difficulty < 0) || (magic_set.Difficulty > 4))
    {
        magic_set.Difficulty = 0;
    }
    if((magic_set.Opponents < 0) || (magic_set.Opponents > 4))
    {
        magic_set.Opponents = 1;
    }
    if((magic_set.LandSize < 0) || (magic_set.LandSize > 2))
    {
        magic_set.LandSize = 0;
    }
    if((magic_set.MagicPower < 0) || (magic_set.MagicPower > 2))
    {
        magic_set.MagicPower = 0;
    }
#ifdef STU_DEBUG
    DBG_Print_MAGIC_SET("Newgame_Screen_0 after load");
    memcpy(&magic_set_snapshot, &magic_set, sizeof(struct s_MAGIC_SET));
#endif
/*
    END: Game Settings Load
*/

    /* ~== Screen Loop */
    magic_set.Difficulty = cfg->difficulty;
    magic_set.Opponents  = cfg->opponents;
    magic_set.LandSize   = cfg->landsize;
    magic_set.MagicPower = cfg->magic;
    if(magic_set.Difficulty > god_Impossible) { magic_set.Difficulty = god_Intro; }
    if(magic_set.Opponents  > goo_Four      ) { magic_set.Opponents  = goo_One;   }
    if(magic_set.LandSize   > gol_Large     ) { magic_set.LandSize   = gol_Small; }
    if(magic_set.MagicPower > gom_Powerful  ) { magic_set.MagicPower = gom_Weak;  }
    /* if(input_field_idx == _ok_button) */
#ifdef STU_DEBUG
    DBG_Compare_MAGIC_SET(&magic_set_snapshot, &magic_set, "Newgame_Screen_0 before save");
#endif
    file_pointer = STU_GRAF_Open_User(str_MAGIC_SET__ovr050, str_wb__ovr050);  /* CLAUDE: -> user-data */
    stu_fwrite(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_pointer);
    stu_fclose(file_pointer);
    _landsize = magic_set.LandSize;  /* ~== NewGame.c  Line 1820 */
    _magic = magic_set.MagicPower;
    _num_players = (magic_set.Opponents + 1);
    _difficulty = magic_set.Difficulty;

}
static void Config_Apply_And_Create_New_Game(struct s_DeMoM_Config *cfg)
{
    int8_t * wsa_ptr = NULL;
    int itr = 0;

    wsa_ptr = &_players[0].alchemy;

    Human_Player_Wizard_Profile(cfg->wizard_id);  /* ~== Newgame_Screen_1() */

    /*
        "Wizard Creation"  (Custom Wizard)
        Newgame_Screen_4()
    */
    for(itr = 0; itr < NUM_WIZARD_SPECIAL_ABILITIES; itr++)
    {
        wsa_ptr[itr] = 0;
    }

    if(cfg->has_name)
    {
        memset(_players[0].name, 0, LEN_WIZARD_NAME);
        stu_strncpy(_players[0].name, cfg->wizard_name, LEN_WIZARD_NAME);
    }

    if(cfg->has_books)
    {
        if(cfg->books_nature >= 0)  { _players[0].spellranks[sbr_Nature]  = cfg->books_nature; }
        if(cfg->books_sorcery >= 0) { _players[0].spellranks[sbr_Sorcery] = cfg->books_sorcery; }
        if(cfg->books_chaos >= 0)   { _players[0].spellranks[sbr_Chaos]   = cfg->books_chaos; }
        if(cfg->books_life >= 0)    { _players[0].spellranks[sbr_Life]    = cfg->books_life; }
        if(cfg->books_death >= 0)   { _players[0].spellranks[sbr_Death]   = cfg->books_death; }

        /* Re-fill _player_start_spells[0] from the updated spellranks.
           Human_Player_Wizard_Profile() above already filled it once for
           Rjak's prefab; if we override the books, the start-spells must
           be refreshed to match — otherwise Init_Computer_Players_Spell_Library
           (called from Init_New_Game) ends up with a stale start-spell pool
           and produces a different spells_list[] than ReMoM's menu path.
           See doc/Devel-DeMoM-Newgame-Path-Alignment.md gap #2. */
        DeMoM_Refill_Human_Player_Start_Spells();
    }

    if(cfg->has_retorts)
    {
        for (itr = 0; itr < NUM_RETORTS; itr++)
        {
            if(cfg->retorts[itr] >= 0)
            {
                wsa_ptr[itr] = cfg->retorts[itr];
            }
        }
    }

    NEWG_Clicked_Race = cfg->race;
// NEWG_Clicked_Race = Arcanus_Races[itr];
// LOG_DEBUG(LOG_CAT_GENERAL, "NEWG_Clicked_Race: %d, Arcanus_Races[%d]: %d", NEWG_Clicked_Race, itr, Arcanus_Races[itr]);
// NEWG_Clicked_Race = Myrran_Races[(itr - 9)];
// LOG_DEBUG(LOG_CAT_GENERAL, "NEWG_Clicked_Race: %d, Myrran_Races[%d]: %d", NEWG_Clicked_Race, (itr - 9), Myrran_Races[(itr - 9)]);

    _players[0].banner_id = (uint8_t)cfg->banner;

    /* Set deterministic RNG seed if specified in config. */
    if(cfg->has_seed)
    {
        Set_Random_Seed(cfg->seed);
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] RNG seed set to %u (0x%08X)", cfg->seed, cfg->seed);
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] RNG seed set to %u (0x%08X)", cfg->seed, cfg->seed);
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] RNG seed set to %u (0x%08X)", cfg->seed, cfg->seed);
    }

    /*
    Mirror the ReMoM Screen 0 OK path: Randomize_Book_Heights consumes 66
       Random() draws.  Without this, DeMoM enters Init_New_Game with the RNG
       cursor 66 ticks ahead of ReMoM and every downstream world-gen draw
       diverges.  See doc/Devel-DeMoM-Newgame-Path-Alignment.md.
        This includes the first call to Random().
        This is the entirety of the code from the 'New Game Screen Functions'
    */
    /* Newgame_Screen_0 */
    New_Game_Settings(cfg);
    Randomize_Book_Heights();

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Creating new game: difficulty=%d magic=%d landsize=%d opponents=%d wizard=%s race=%d banner=%d seed=%u", cfg->difficulty, cfg->magic, cfg->landsize, cfg->opponents, cfg->wizard_name, cfg->race, cfg->banner, Get_Random_Seed());
    LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Creating new game: difficulty=%d magic=%d landsize=%d opponents=%d wizard=%s race=%d banner=%d seed=%u", cfg->difficulty, cfg->magic, cfg->landsize, cfg->opponents, cfg->wizard_name, cfg->race, cfg->banner, Get_Random_Seed());
    LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Creating new game: difficulty=%d magic=%d landsize=%d opponents=%d wizard=%s race=%d banner=%d seed=%u", cfg->difficulty, cfg->magic, cfg->landsize, cfg->opponents, cfg->wizard_name, cfg->race, cfg->banner, Get_Random_Seed());

    // New_Game_Screen_7__WIP()
    // NEWGAME.LBX, 053  BUILDWOR   map build bar
    // newgame_BUILDWOR_map_build_bar_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 53, _screen_seg);
    newgame_BUILDWOR_map_build_bar_seg = LBX_Load(newgame_lbx_file__ovr050, 53);

    /* Newgame_Control() |-> Init_New_Game() */
    Init_New_Game();
    Initialize_Events();
    Init_Runtime();

    Save_SAVE_GAM(8);

/*
    END: MAGIC.EXE
*/

/*
    BEGIN: WIZARDS.EXE
*/

    /* Loaded_Game_Update(), etc. */
    New_Game_Continue();

    /*
        MainScr.c
        Change_Home_City_Name_Popup
        (CityScr.c:1742) calls Random_City_Name_By_Race first
        — RNG consumer that must run for trace parity.
        Then override with cfg->home_city_name if set,
        equivalent to the user typing a name into the popup.
    */
    Random_City_Name_By_Race(_CITIES[HUMAN_PLAYER_IDX].race, _CITIES[HUMAN_PLAYER_IDX].name);
    if(cfg->has_home_city_name)
    {
        /* Mirror the keyboard popup: typed chars overwrite the random pre-fill
           byte-by-byte, Enter places one '\0' at the cursor, bytes past the
           cursor keep whatever Random_City_Name_By_Race wrote.  stu_strncpy
           would zero-pad the tail and diverge from ReMoM's SAVE bytes. */
        int i;
        for(i = 0; (i < LEN_CITY_NAME - 1) && (cfg->home_city_name[i] != '\0'); i++)
        {
            _CITIES[HUMAN_PLAYER_IDX].name[i] = cfg->home_city_name[i];
        }
        _CITIES[HUMAN_PLAYER_IDX].name[i] = '\0';
    }
    _given_chance_to_rename_home_city = ST_TRUE;

    /* Optional named-slot save.  Save_SAVE_GAM (LOADSAVE.c:197-201) writes
       MAGIC.SET internally for slots 0..7, so we only need to set the slot
       name first. */
    if(cfg->has_save_slot && cfg->save_slot >= 0 && cfg->save_slot < NUM_SAVES)
    {
        if(cfg->has_save_name)
        {
            /* Same keyboard-popup semantics as the home city name above:
               typed chars overwrite the pre-existing slot name byte-by-byte,
               Enter writes one '\0' at the cursor, bytes past the cursor
               keep whatever was there (e.g. residual "----" padding from a
               prior slot name).  stu_strncpy would zero-pad the tail. */
            int i;
            for(i = 0; (i < LEN_SAVE_DESCRIPTION - 1) && (cfg->save_name[i] != '\0'); i++)
            {
                magic_set.Save_Names[cfg->save_slot][i] = cfg->save_name[i];
            }
            magic_set.Save_Names[cfg->save_slot][i] = '\0';
        }
        Save_SAVE_GAM(cfg->save_slot);
    }

    
    /* Dump structured text representation of the final save file for testing. */
    Game_Save_Dump("SAVE9.GAM", "SAVE9.txt");
    
    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] New game created successfully");
}



/* ========================================================================= */
/*  Replay Callback                                                         */
/* ========================================================================= */

static void DeMoM_Replay_Log_Field_Hit(void *log, int mouse_x, int mouse_y)
{
    FILE *fp = (FILE *)log;
    int fi;
    for (fi = 0; fi < fields_count; fi++)
    {
        if(mouse_x >= p_fields[fi].x1 && mouse_x <= p_fields[fi].x2 && mouse_y >= p_fields[fi].y1 && mouse_y <= p_fields[fi].y2)
        {
            fprintf(fp, "  field[%d]=(%d,%d)-(%d,%d)", fi, p_fields[fi].x1, p_fields[fi].y1, p_fields[fi].x2, p_fields[fi].y2);
#if STU_CLOG
            /* CLAUDE 2026-07-22: stamp the field's Add_* origin (basename:line) — the join
               key into the field catalog / HMS alias table for rmr2hms naming. */
            {
                const char *origin_file = STU_CLOG_Field_Origin_File((int16_t)fi);
                if(origin_file[0] != '\0') { fprintf(fp, "@%s:%ld", origin_file, (long)STU_CLOG_Field_Origin_Line((int16_t)fi)); }
            }
#endif
            break;
        }
    }
#if STU_CLOG
    /* CLAUDE: drawing catalog -- the most recently drawn sprite under the click (PLAN-Draw-Catalog.md section 4) */
    {
        char draw_hit[64];
        if(STU_CLOG_Draw_Hit((int16_t)mouse_x, (int16_t)mouse_y, draw_hit, sizeof(draw_hit)) != 0) { fprintf(fp, "  draw=%s", draw_hit); }
    }
#endif
}



/* ========================================================================= */
/*  Usage                                                                    */
/* ========================================================================= */

/* ========================================================================= */
/*  Combat test mode (--combat)                                              */
/*                                                                           */
/*  Loads a named fixture save (SAVECMBT.GAM), patches the minimal overland  */
/*  state the combat dispatcher expects for a stack-vs-stack attack, seeds   */
/*  the RNG immediately before combat entry (so overland load history cannot */
/*  shift the combat rolls), invokes Combat() directly, and dumps the   */
/*  results as `key = value` text for the CTest assertion step.              */
/*                                                                           */
/*  Both combatants must be computer players: the dispatcher then routes to  */
/*  Strategic_Combat() (auto-resolve), which runs windowless.  A human  */
/*  combatant would route to the tactical battle screen, which this tracer   */
/*  does not support yet.                                                    */
/* ========================================================================= */

#define DEMOM_COMBAT_MAX_TROOPS 9
#define DEMOM_COMBAT_DUMP_FILE "DEMOM_COMBAT.txt"

static void DeMoM_Combat_Dump_Unit(FILE * fout, const char * prefix, int itr, int16_t unit_idx)
{
    fprintf(fout, "%s[%d].unit_idx = %d\n", prefix, itr, unit_idx);
    fprintf(fout, "%s[%d].owner_idx = %d\n", prefix, itr, _UNITS[unit_idx].owner_idx);
    fprintf(fout, "%s[%d].type = %d\n", prefix, itr, _UNITS[unit_idx].type);
    fprintf(fout, "%s[%d].status = %d\n", prefix, itr, _UNITS[unit_idx].Status);
    fprintf(fout, "%s[%d].level = %d\n", prefix, itr, _UNITS[unit_idx].Level);
    fprintf(fout, "%s[%d].xp = %d\n", prefix, itr, _UNITS[unit_idx].XP);
    fprintf(fout, "%s[%d].damage = %d\n", prefix, itr, _UNITS[unit_idx].Damage);
    fprintf(fout, "%s[%d].wx = %d\n", prefix, itr, _UNITS[unit_idx].wx);
    fprintf(fout, "%s[%d].wy = %d\n", prefix, itr, _UNITS[unit_idx].wy);
    fprintf(fout, "%s[%d].wp = %d\n", prefix, itr, _UNITS[unit_idx].wp);
}

static int DeMoM_Combat_Run(int16_t defender_unit_idx, int16_t troop_count, int16_t troops[], int16_t tactical)
{
    FILE * fout = NULL;
    int16_t attacker_player_idx = 0;
    int16_t defender_player_idx = 0;
    int16_t defender_stack[DEMOM_COMBAT_MAX_TROOPS];
    int16_t defender_stack_count = 0;
    int16_t defender_wx = 0;
    int16_t defender_wy = 0;
    int16_t defender_wp = 0;
    int16_t attacker_won = 0;
    uint32_t combat_seed = 0;
    uint64_t rng_calls_before = 0;
    int16_t itr = 0;

    if(troop_count < 1)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] No attacker troops given");
        return 1;
    }
    if(defender_unit_idx < 0 || defender_unit_idx >= _units)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] Defender unit idx %d out of range (0..%d)", defender_unit_idx, _units - 1);
        return 1;
    }
    for(itr = 0; itr < troop_count; itr++)
    {
        if(troops[itr] < 0 || troops[itr] >= _units)
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] Attacker unit idx %d out of range (0..%d)", troops[itr], _units - 1);
            return 1;
        }
    }

    attacker_player_idx = _UNITS[troops[0]].owner_idx;
    defender_player_idx = _UNITS[defender_unit_idx].owner_idx;
    defender_wx = _UNITS[defender_unit_idx].wx;
    defender_wy = _UNITS[defender_unit_idx].wy;
    defender_wp = _UNITS[defender_unit_idx].wp;

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] attacker_player=%d defender_player=%d human_player=%d defender_at=(%d,%d,%d)", attacker_player_idx, defender_player_idx, _current_player_idx, defender_wx, defender_wy, defender_wp);

    if(attacker_player_idx == defender_player_idx)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] Attacker and defender have the same owner (%d)", attacker_player_idx);
        return 1;
    }
    if(tactical == 1)
    {
        /* The dispatcher only routes to the tactical battle screen when a human participates and the "Strategic Combat Only" option is off; g_cmbt_force_auto_combat then makes the AI drive both sides so no input is needed. */
        if((attacker_player_idx != _current_player_idx) && (defender_player_idx != _current_player_idx))
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] --combat-tactical requires one combatant to be the human player (%d); the dispatcher would auto-resolve an AI-vs-AI fight", _current_player_idx);
            return 1;
        }
        magic_set.strategic_combat_only = ST_FALSE;
        g_cmbt_force_auto_combat = ST_TRUE;
    }
    else
    {
        if((attacker_player_idx == _current_player_idx) || (defender_player_idx == _current_player_idx))
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] A combatant is the human player; the dispatcher would open the tactical battle screen -- use --combat-tactical for that");
            return 1;
        }
    }

    /* Capture defender stack membership before combat mutates the unit array. */
    for(itr = 0; itr < _units && defender_stack_count < DEMOM_COMBAT_MAX_TROOPS; itr++)
    {
        if(
            (_UNITS[itr].wx == defender_wx) &&
            (_UNITS[itr].wy == defender_wy) &&
            (_UNITS[itr].wp == defender_wp) &&
            (_UNITS[itr].owner_idx == defender_player_idx)
        )
        {
            defender_stack[defender_stack_count] = itr;
            defender_stack_count++;
        }
    }

    /* Patch diplomacy: Combat() silently aborts (No_Combat) when a non-human attacker strikes an Alliance/WizardPact partner, so force a state of war between the combatants in both directions. */
    _players[attacker_player_idx].Dipl.Dipl_Status[defender_player_idx] = DIPL_War;
    _players[defender_player_idx].Dipl.Dipl_Status[attacker_player_idx] = DIPL_War;

    /* Patch the overland state Combat() reads for a stack-vs-stack attack.  _combat_attacker_wx/Y is the square the attackers strike from; Combat() moves the troops there at entry, so it just needs to be a square adjacent to the defender. */
    _combat_environ = 0;      /* cnv_Enemy_Stack */
    _combat_environ_idx = 0;  /* unused for stack-vs-stack */
    _combat_attacker_wx = (defender_wx > 0) ? (int16_t)(defender_wx - 1) : (int16_t)(defender_wx + 1);
    _combat_attacker_wy = defender_wy;
    for(itr = 0; itr < troop_count; itr++)
    {
        _UNITS[troops[itr]].Status = us_Ready;
    }

    if(tactical == 1)
    {
        /* Cosmetic-only settings the tactical battle screen honors; disabling them skips per-frame draw/sleep loops (movement animations especially) that make a windowless AI-vs-AI battle enormously slow without affecting the combat outcome. */
        magic_set.movement_animations = ST_FALSE;
        magic_set.sound_effects = ST_FALSE;
        magic_set.background_music = ST_FALSE;
    }

    combat_seed = (_cmd_line_seed != 0) ? (uint32_t)_cmd_line_seed : 12345u;
    Set_Random_Seed(combat_seed);
    rng_calls_before = g_random_call_count;

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] Invoking Combat() seed=%u troop_count=%d defender_stack_count=%d", combat_seed, troop_count, defender_stack_count);

    attacker_won = Combat(attacker_player_idx, defender_unit_idx, troop_count, troops);

    g_cmbt_force_auto_combat = ST_FALSE;

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] Combat() returned %d", attacker_won);

    fout = fopen(DEMOM_COMBAT_DUMP_FILE, "w");
    if(fout == NULL)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] Could not create: %s", DEMOM_COMBAT_DUMP_FILE);
        return 1;
    }

    fprintf(fout, "# DeMoM Combat Dump\n");
    fprintf(fout, "# Mode: %s (direct Combat invoke)\n", (tactical == 1) ? "tactical AI-vs-AI (forced auto-combat)" : "strategic");
    fprintf(fout, "# NOTE: unit indices are pre-combat indices; combat may free dead units, so a dumped slot reflects whatever occupies that index afterwards (deterministic for a fixed fixture + seed).\n");
    fprintf(fout, "combat.seed = %u\n", combat_seed);
    fprintf(fout, "combat.attacker_player_idx = %d\n", attacker_player_idx);
    fprintf(fout, "combat.defender_player_idx = %d\n", defender_player_idx);
    fprintf(fout, "combat.defender_unit_idx = %d\n", defender_unit_idx);
    fprintf(fout, "combat.troop_count = %d\n", troop_count);
    fprintf(fout, "combat.defender_stack_count = %d\n", defender_stack_count);
    fprintf(fout, "combat.tactical = %d\n", tactical);
    fprintf(fout, "combat.attacker_won = %d\n", attacker_won);
    fprintf(fout, "combat.turns = %d\n", _combat_turn);
    fprintf(fout, "combat.winner_player_idx = %d\n", _combat_winner);
    fprintf(fout, "combat.gold_reward = %d\n", combat_results_gold);
    fprintf(fout, "combat.rng_calls = %llu\n", (unsigned long long)(g_random_call_count - rng_calls_before));
    fprintf(fout, "combat.random_seed_after = %u\n", Get_Random_Seed());
    fprintf(fout, "game.units_after = %d\n", _units);
    for(itr = 0; itr < troop_count; itr++)
    {
        DeMoM_Combat_Dump_Unit(fout, "troop", itr, troops[itr]);
    }
    for(itr = 0; itr < defender_stack_count; itr++)
    {
        DeMoM_Combat_Dump_Unit(fout, "defender", itr, defender_stack[itr]);
    }
    fclose(fout);

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Combat] Wrote %s", DEMOM_COMBAT_DUMP_FILE);
    return 0;
}



/* ========================================================================= */
/*  Summon test mode (--cast-summon)                                         */
/*                                                                           */
/*  Regression test for GitHub issue #21: after the human                    */
/*  player's overland creature summon, Cast_Spell_Overland() re-centers the  */
/*  map on the new unit.  The reconstruction read _UNITS[_units] (one past   */
/*  the new unit) instead of _UNITS[_units - 1].  That slot is whatever      */
/*  record was last left behind there.  Kill_Unit() sets wp = ST_UNDEFINED   */
/*  and Delete_Dead_Units() drops the count without clearing the record, so  */
/*  once the last two units have died, the new unit fills the first dead     */
/*  slot and the bad read hits the second: _map_plane became -1 and the      */
/*  next Draw_Map_Terrain() read before _world_maps and crashed.  With any   */
/*  other leftover record the map silently re-centers on the wrong square.   */
/*                                                                           */
/*  Loads SAVECMBT.GAM, reproduces that exact state (kills the last two      */
/*  units and compacts), casts SPELL_IDX for the human player through the    */
/*  real Cast_Spell_Overland(), and dumps the post-cast map focus for the    */
/*  CTest assertion step.                                                    */
/* ========================================================================= */

#define DEMOM_SUMMON_DUMP_FILE "DEMOM_SUMMON.txt"

static int DeMoM_Summon_Run(int16_t spell_idx)
{
    FILE * fout = NULL;
    int16_t killed_unit_idx = 0;
    int16_t units_before_cast = 0;
    int16_t stale_slot_owner_idx = 0;
    int16_t stale_slot_wp = 0;
    int16_t new_unit_idx = 0;

    if(spell_idx <= spl_NONE || spell_idx >= NUM_SPELLS)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] spell_idx %d out of range", spell_idx);
        return 1;
    }
    if(spell_data_table[spell_idx].type != scc_Summoning)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] spell_idx %d (%s) is not a creature summoning spell (type %d)", spell_idx, spell_data_table[spell_idx].name, spell_data_table[spell_idx].type);
        return 1;
    }
    if(_units < 3)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] fixture has too few units (%d)", _units);
        return 1;
    }

    /* Reproduce the precondition the way the game produces it: the last two units die, and the end-of-turn compaction drops the count without overwriting their records, leaving dead records (wp = ST_UNDEFINED) at _UNITS[_units] and _UNITS[_units + 1].  The summon fills the first; the bad read hits the second. */
    killed_unit_idx = (int16_t)(_units - 2);
    Kill_Unit((int16_t)(_units - 1), kt_Normal);
    Kill_Unit(killed_unit_idx, kt_Normal);
    Delete_Dead_Units();
    units_before_cast = _units;
    stale_slot_owner_idx = _UNITS[(_units + 1)].owner_idx;
    stale_slot_wp = _UNITS[(_units + 1)].wp;

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] killed units %d,%d; _units=%d; slot past the new unit _UNITS[%d] owner=%d wp=%d", killed_unit_idx, (killed_unit_idx + 1), _units, (_units + 1), stale_slot_owner_idx, stale_slot_wp);

    /* Cosmetic-only: shorten the summon screen's animation loop and skip music; neither affects the cast. */
    magic_set.spell_animations = ST_FALSE;
    magic_set.background_music = ST_FALSE;
    magic_set.sound_effects = ST_FALSE;

    _players[HUMAN_PLAYER_IDX].casting_spell_idx = spell_idx;
    _map_plane = 0;
    _active_world_x = 0;
    _active_world_y = 0;

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] Invoking Cast_Spell_Overland() spell=%d (%s) summon_at=(%d,%d,%d)", spell_idx, spell_data_table[spell_idx].name, _players[HUMAN_PLAYER_IDX].summon_wx, _players[HUMAN_PLAYER_IDX].summon_wy, _players[HUMAN_PLAYER_IDX].summon_wp);

    Cast_Spell_Overland(HUMAN_PLAYER_IDX);

    new_unit_idx = (int16_t)(_units - 1);

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] Cast_Spell_Overland() returned; _units=%d _map_plane=%d _active_world=(%d,%d)", _units, _map_plane, _active_world_x, _active_world_y);

    fout = fopen(DEMOM_SUMMON_DUMP_FILE, "w");
    if(fout == NULL)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] Could not create: %s", DEMOM_SUMMON_DUMP_FILE);
        return 1;
    }

    fprintf(fout, "# DeMoM Summon Dump\n");
    fprintf(fout, "summon.spell_idx = %d\n", spell_idx);
    fprintf(fout, "summon.killed_unit_idx = %d\n", killed_unit_idx);
    fprintf(fout, "summon.units_before_cast = %d\n", units_before_cast);
    fprintf(fout, "summon.units_after_cast = %d\n", _units);
    fprintf(fout, "summon.unit_created = %d\n", (_units == (units_before_cast + 1)) ? 1 : 0);
    fprintf(fout, "summon.stale_slot_owner_idx = %d\n", stale_slot_owner_idx);
    fprintf(fout, "summon.stale_slot_wp = %d\n", stale_slot_wp);
    fprintf(fout, "summon.location_wx = %d\n", _players[HUMAN_PLAYER_IDX].summon_wx);
    fprintf(fout, "summon.location_wy = %d\n", _players[HUMAN_PLAYER_IDX].summon_wy);
    fprintf(fout, "summon.location_wp = %d\n", _players[HUMAN_PLAYER_IDX].summon_wp);
    fprintf(fout, "new_unit.idx = %d\n", new_unit_idx);
    fprintf(fout, "new_unit.type = %d\n", _UNITS[new_unit_idx].type);
    fprintf(fout, "new_unit.owner_idx = %d\n", _UNITS[new_unit_idx].owner_idx);
    fprintf(fout, "new_unit.wx = %d\n", _UNITS[new_unit_idx].wx);
    fprintf(fout, "new_unit.wy = %d\n", _UNITS[new_unit_idx].wy);
    fprintf(fout, "new_unit.wp = %d\n", _UNITS[new_unit_idx].wp);
    fprintf(fout, "map.active_world_x = %d\n", _active_world_x);
    fprintf(fout, "map.active_world_y = %d\n", _active_world_y);
    fprintf(fout, "map.map_plane = %d\n", _map_plane);
    fprintf(fout, "check.new_unit_type_matches_spell = %d\n", (_UNITS[new_unit_idx].type == spell_data_table[spell_idx].unit_type) ? 1 : 0);
    fprintf(fout, "check.new_unit_owned_by_human = %d\n", (_UNITS[new_unit_idx].owner_idx == HUMAN_PLAYER_IDX) ? 1 : 0);
    fprintf(fout, "check.map_plane_in_range = %d\n", ((_map_plane >= 0) && (_map_plane < NUM_PLANES)) ? 1 : 0);
    fprintf(fout, "check.map_plane_matches_new_unit = %d\n", (_map_plane == _UNITS[new_unit_idx].wp) ? 1 : 0);
    fprintf(fout, "check.active_world_x_matches_new_unit = %d\n", (_active_world_x == _UNITS[new_unit_idx].wx) ? 1 : 0);
    fprintf(fout, "check.active_world_y_matches_new_unit = %d\n", (_active_world_y == _UNITS[new_unit_idx].wy) ? 1 : 0);
    fclose(fout);

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] Wrote %s; drawing the main screen with the post-cast map focus", DEMOM_SUMMON_DUMP_FILE);

    /* The reported crash is the first main-screen draw after the summon screen closes.  Do that draw here; if it faults, the dump above is already on disk and check.main_screen_drawn never gets written. */
    Full_Draw_Main_Screen();

    fout = fopen(DEMOM_SUMMON_DUMP_FILE, "a");
    if(fout == NULL)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] Could not append to: %s", DEMOM_SUMMON_DUMP_FILE);
        return 1;
    }
    fprintf(fout, "check.main_screen_drawn = 1\n");
    fclose(fout);

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM Summon] Main screen drawn");
    return 0;
}



/* ========================================================================= */
/*  AI-turn dead-unit test mode (--ai-dead-units)                            */
/*                                                                           */
/*  BUGS_AND_QUIRKS #137: AI_Player_Calculate_Target_Values() processes      */
/*  every record in _UNITS[0.._units), dead ones (owner_idx -1) included,    */
/*  exactly like the OG; a dead unit counts as an enemy stack at its last    */
/*  square.  Its Effective_Unit_Strength() reads _players[-1] / _HEROES2[-1],*/
/*  bounded in ReMoM by the HACK Stack_Garbage stand-ins.                     */
/*                                                                           */
/*  Loads SAVECMBT.GAM and kills the listed units with the real Kill_Unit()  */
/*  WITHOUT compacting, so the dead records sit in the middle of _UNITS[].   */
/*  Then:                                                                    */
/*    1. probe (b): Kill_Unit() left owner -1, wp -1 -- the state of a unit  */
/*       killed by an earlier AI player inside the same AI_Next_Turn().      */
/*       Runs the real AI_Player_Calculate_Target_Values() for the first     */
/*       active AI player and records the enemy stacks on plane -1.          */
/*    2. the real AI_Next_Turn() (its sanity loop turns the dead records     */
/*       into state (a): wp 0, owner -1), RNG pinned to --seed1.             */
/*    3. probe (a): the real AI_Player_Calculate_Target_Values() again, now  */
/*       on the state-(a) records, and records the enemy stacks at the dead  */
/*       units' Arcanus squares.                                             */
/*  The probes save/restore random_seed and the stack-garbage seed, so the   */
/*  real AI turn is unaffected by them.  Dumps DEMOM_AI_DEAD_UNITS.txt.      */
/* ========================================================================= */

#define DEMOM_AI_DEAD_MAX_UNITS 16
#define DEMOM_AI_DEAD_DUMP_FILE "DEMOM_AI_DEAD_UNITS.txt"

struct s_DEMOM_AI_DEAD_PROBE
{
    int16_t player_idx;
    int16_t units_visited;
    int16_t dead_units_visited;
    int16_t dead_units_visited_wp_undefined;
    int16_t dead_units_visited_wp_zero;
    int16_t enemy_stack_count;
    int16_t own_stack_count;
    int16_t enemy_stacks_on_plane_undefined;
    int32_t enemy_stacks_on_plane_undefined_value;
    int16_t dead_square_has_enemy_stack[DEMOM_AI_DEAD_MAX_UNITS];
    int16_t dead_square_stack_value[DEMOM_AI_DEAD_MAX_UNITS];
    int16_t dead_square_live_others[DEMOM_AI_DEAD_MAX_UNITS];
    int16_t dead_square_city[DEMOM_AI_DEAD_MAX_UNITS];
};

static int16_t DeMoM_AI_Dead_First_Active_AI_Player(void)
{
    int16_t itr_players = 0;
    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        if(_FORTRESSES[itr_players].active == ST_TRUE)
        {
            return itr_players;
        }
    }
    return ST_UNDEFINED;
}

/* Runs the real AI_Player_Calculate_Target_Values() for player_idx on the current _UNITS[] and records what it built for the dead units' squares. */
static void DeMoM_AI_Dead_Probe(struct s_DEMOM_AI_DEAD_PROBE * probe, int16_t player_idx, int16_t dead_count, int16_t dead_units[])
{
    uint32_t saved_random_seed = 0;
    uint32_t saved_garbage_seed = 0;
    uint64_t saved_random_call_count = 0;
    int16_t itr = 0;
    int16_t itr_stacks = 0;
    int16_t itr_units = 0;
    int16_t itr_cities = 0;
    int16_t dead_wx = 0;
    int16_t dead_wy = 0;
    int16_t dead_wp = 0;

    memset(probe, 0, sizeof(*probe));
    probe->player_idx = player_idx;

    /* What the unit loop (for itr_units = 0; itr_units < _units) visits: it has no owner / plane test, so every record below _units, dead or alive. */
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        probe->units_visited++;
        if(_UNITS[itr_units].owner_idx == ST_UNDEFINED)
        {
            probe->dead_units_visited++;
            if(_UNITS[itr_units].wp == ST_UNDEFINED)
            {
                probe->dead_units_visited_wp_undefined++;
            }
            if(_UNITS[itr_units].wp == 0)
            {
                probe->dead_units_visited_wp_zero++;
            }
        }
    }

    saved_random_seed = Get_Random_Seed();
    saved_garbage_seed = Get_Stack_Garbage_Seed();
    saved_random_call_count = g_random_call_count;

    EMMDATAH_Map();
    Allocate_AI_Data();
    Player_Hostile_Opponents(player_idx);
    AI_Player_Calculate_Target_Values(player_idx);

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] probe player=%d: AI_Player_Calculate_Target_Values() visited %d records, %d dead (wp -1: %d, wp 0: %d); enemy_stacks=%d own_stacks=%d; Random() calls during probe=%llu", player_idx, probe->units_visited, probe->dead_units_visited, probe->dead_units_visited_wp_undefined, probe->dead_units_visited_wp_zero, _ai_all_enemy_stack_count, _ai_all_own_stack_count, (unsigned long long)(g_random_call_count - saved_random_call_count));

    Set_Random_Seed(saved_random_seed);
    Set_Stack_Garbage_Seed(saved_garbage_seed);
    g_random_call_count = saved_random_call_count;

    probe->enemy_stack_count = _ai_all_enemy_stack_count;
    probe->own_stack_count = _ai_all_own_stack_count;

    /* s_AI_TARGET.wp is uint8_t, so a stack started by a wp -1 record reads back as 255; no live unit is ever on plane -1. */
    for(itr_stacks = 0; itr_stacks < _ai_all_enemy_stack_count; itr_stacks++)
    {
        if(_ai_all_enemy_stacks[itr_stacks].wp == (uint8_t)ST_UNDEFINED)
        {
            probe->enemy_stacks_on_plane_undefined++;
            probe->enemy_stacks_on_plane_undefined_value += _ai_all_enemy_stacks[itr_stacks].value;
        }
    }

    for(itr = 0; itr < dead_count; itr++)
    {
        dead_wx = _UNITS[dead_units[itr]].wx;
        dead_wy = _UNITS[dead_units[itr]].wy;
        dead_wp = _UNITS[dead_units[itr]].wp;
        probe->dead_square_stack_value[itr] = 0;
        for(itr_stacks = 0; itr_stacks < _ai_all_enemy_stack_count; itr_stacks++)
        {
            if(
                (_ai_all_enemy_stacks[itr_stacks].wx == (uint8_t)dead_wx)
                &&
                (_ai_all_enemy_stacks[itr_stacks].wy == (uint8_t)dead_wy)
                &&
                (_ai_all_enemy_stacks[itr_stacks].wp == (uint8_t)dead_wp)
            )
            {
                probe->dead_square_has_enemy_stack[itr] = 1;
                probe->dead_square_stack_value[itr] = _ai_all_enemy_stacks[itr_stacks].value;
            }
        }
        /* Live records of other players on the same square; 0 means the stack can only have come from dead records. */
        for(itr_units = 0; itr_units < _units; itr_units++)
        {
            if(
                (_UNITS[itr_units].owner_idx != ST_UNDEFINED)
                &&
                (_UNITS[itr_units].owner_idx != player_idx)
                &&
                (_UNITS[itr_units].wx == dead_wx)
                &&
                (_UNITS[itr_units].wy == dead_wy)
                &&
                (_UNITS[itr_units].wp == dead_wp)
            )
            {
                probe->dead_square_live_others[itr]++;
            }
        }
        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {
            if(
                (_CITIES[itr_cities].wx == dead_wx)
                &&
                (_CITIES[itr_cities].wy == dead_wy)
                &&
                (_CITIES[itr_cities].wp == dead_wp)
            )
            {
                probe->dead_square_city[itr] = 1;
            }
        }
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] probe player=%d dead unit %d at (%d,%d,%d): enemy_stack=%d value=%d live_others=%d city=%d", player_idx, dead_units[itr], dead_wx, dead_wy, dead_wp, probe->dead_square_has_enemy_stack[itr], probe->dead_square_stack_value[itr], probe->dead_square_live_others[itr], probe->dead_square_city[itr]);
    }
}

static void DeMoM_AI_Dead_Dump_Probe(FILE * fout, const char * prefix, struct s_DEMOM_AI_DEAD_PROBE * probe, int16_t dead_count)
{
    int16_t itr = 0;
    fprintf(fout, "%s.player_idx = %d\n", prefix, probe->player_idx);
    fprintf(fout, "%s.units_visited = %d\n", prefix, probe->units_visited);
    fprintf(fout, "%s.dead_units_visited = %d\n", prefix, probe->dead_units_visited);
    fprintf(fout, "%s.dead_units_visited_wp_undefined = %d\n", prefix, probe->dead_units_visited_wp_undefined);
    fprintf(fout, "%s.dead_units_visited_wp_zero = %d\n", prefix, probe->dead_units_visited_wp_zero);
    fprintf(fout, "%s.enemy_stack_count = %d\n", prefix, probe->enemy_stack_count);
    fprintf(fout, "%s.own_stack_count = %d\n", prefix, probe->own_stack_count);
    fprintf(fout, "%s.enemy_stacks_on_plane_undefined = %d\n", prefix, probe->enemy_stacks_on_plane_undefined);
    fprintf(fout, "%s.enemy_stacks_on_plane_undefined_value = %d\n", prefix, (int)probe->enemy_stacks_on_plane_undefined_value);
    for(itr = 0; itr < dead_count; itr++)
    {
        fprintf(fout, "%s.dead[%d].has_enemy_stack = %d\n", prefix, itr, probe->dead_square_has_enemy_stack[itr]);
        fprintf(fout, "%s.dead[%d].stack_value = %d\n", prefix, itr, probe->dead_square_stack_value[itr]);
        fprintf(fout, "%s.dead[%d].live_others_on_square = %d\n", prefix, itr, probe->dead_square_live_others[itr]);
        fprintf(fout, "%s.dead[%d].city_on_square = %d\n", prefix, itr, probe->dead_square_city[itr]);
    }
}

static int DeMoM_AI_Dead_Units_Run(int16_t dead_count, int16_t dead_units[])
{
    FILE * fout = NULL;
    struct s_DEMOM_AI_DEAD_PROBE probe_b;
    struct s_DEMOM_AI_DEAD_PROBE probe_a;
    int16_t dead_owner[DEMOM_AI_DEAD_MAX_UNITS] = { 0 };
    int16_t dead_type[DEMOM_AI_DEAD_MAX_UNITS] = { 0 };
    int16_t dead_hero_slot[DEMOM_AI_DEAD_MAX_UNITS] = { 0 };
    int16_t dead_wp_before[DEMOM_AI_DEAD_MAX_UNITS] = { 0 };
    int16_t dead_wp_after_kill[DEMOM_AI_DEAD_MAX_UNITS] = { 0 };
    int16_t units_before = 0;
    int16_t probe_player_idx = 0;
    int16_t killed_in_turn_wp_undefined = 0;
    int16_t dead_after_turn = 0;
    int16_t heroes_killed = 0;
    int16_t active_ai_players = 0;
    uint32_t turn_seed = 0;
    uint64_t rng_calls_before = 0;
    int16_t itr = 0;
    int16_t itr_units = 0;
    int16_t is_listed = 0;

    if(dead_count < 1 || dead_count > DEMOM_AI_DEAD_MAX_UNITS)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] dead unit count %d out of range (1..%d)", dead_count, DEMOM_AI_DEAD_MAX_UNITS);
        return 1;
    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] fixture unit %d owner=%d type=%d hero_slot=%d at (%d,%d,%d)", itr_units, _UNITS[itr_units].owner_idx, _UNITS[itr_units].type, _UNITS[itr_units].Hero_Slot, _UNITS[itr_units].wx, _UNITS[itr_units].wy, _UNITS[itr_units].wp);
    }

    for(itr = 0; itr < dead_count; itr++)
    {
        if(dead_units[itr] < 0 || dead_units[itr] >= (_units - 1))
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] unit idx %d out of range (0..%d; the last record is excluded so the dead records sit in the middle of _UNITS[])", dead_units[itr], (_units - 2));
            return 1;
        }
        if(_UNITS[dead_units[itr]].owner_idx == ST_UNDEFINED)
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] unit idx %d is already dead in the fixture", dead_units[itr]);
            return 1;
        }
    }

    probe_player_idx = DeMoM_AI_Dead_First_Active_AI_Player();
    if(probe_player_idx == ST_UNDEFINED)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] fixture has no active AI player");
        return 1;
    }

    /* Kill through the real Kill_Unit(), and do NOT compact: the records stay in _UNITS[0.._units). */
    units_before = _units;
    for(itr = 0; itr < dead_count; itr++)
    {
        dead_owner[itr] = _UNITS[dead_units[itr]].owner_idx;
        dead_type[itr] = _UNITS[dead_units[itr]].type;
        dead_hero_slot[itr] = _UNITS[dead_units[itr]].Hero_Slot;
        dead_wp_before[itr] = _UNITS[dead_units[itr]].wp;
        if(dead_hero_slot[itr] > -1)
        {
            heroes_killed++;
        }
        Kill_Unit(dead_units[itr], kt_Normal);
        dead_wp_after_kill[itr] = _UNITS[dead_units[itr]].wp;
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] Kill_Unit(%d) owner %d->%d type=%d hero_slot %d->%d wp %d->%d", dead_units[itr], dead_owner[itr], _UNITS[dead_units[itr]].owner_idx, dead_type[itr], dead_hero_slot[itr], _UNITS[dead_units[itr]].Hero_Slot, dead_wp_before[itr], dead_wp_after_kill[itr]);
    }

    /* Probe (b): owner -1, wp -1, as left by Kill_Unit() -- what a later AI player sees after an earlier one killed the unit inside the same AI_Next_Turn(). */
    DeMoM_AI_Dead_Probe(&probe_b, probe_player_idx, dead_count, dead_units);

    /* Cosmetic-only: AI overland casts can open spell-animation screens; skip music and sound. */
    magic_set.spell_animations = ST_FALSE;
    magic_set.background_music = ST_FALSE;
    magic_set.sound_effects = ST_FALSE;
    /* An AI stack attacking a human stack would otherwise open the tactical battle screen, which needs input. */
    magic_set.strategic_combat_only = ST_TRUE;

    /* scr_Continue clears m_magic_winner_idx after Loaded_Game_Update(); this mode skips Screen_Control(), so do the same.  Left at its zero-initialized value, Get_Winner() reports player 0 and AI_Next_Turn()'s EOG_HACK stops after the first AI overland cast. */
    m_magic_winner_idx = ST_UNDEFINED;
    magic_master_idx = ST_UNDEFINED;
    g_conquest_winner_idx = ST_UNDEFINED;
    for(itr = 1; itr < _num_players; itr++)
    {
        if(_FORTRESSES[itr].active == ST_TRUE)
        {
            active_ai_players++;
        }
    }

    turn_seed = (_cmd_line_seed != 0) ? (uint32_t)_cmd_line_seed : 12345u;
    Set_Random_Seed(turn_seed);
    rng_calls_before = g_random_call_count;

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] Invoking AI_Next_Turn() seed=%u _units=%d dead=%d", turn_seed, _units, dead_count);

    AI_Next_Turn();

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] AI_Next_Turn() returned; _units=%d rng_calls=%llu", _units, (unsigned long long)(g_random_call_count - rng_calls_before));

    /* After the turn: records killed by an AI player inside this AI_Next_Turn() still have wp -1 (the sanity loop runs only at entry). */
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == ST_UNDEFINED)
        {
            dead_after_turn++;
            is_listed = ST_FALSE;
            for(itr = 0; itr < dead_count; itr++)
            {
                if(dead_units[itr] == itr_units)
                {
                    is_listed = ST_TRUE;
                }
            }
            if((is_listed == ST_FALSE) && (_UNITS[itr_units].wp == ST_UNDEFINED))
            {
                killed_in_turn_wp_undefined++;
                LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] unit %d died inside AI_Next_Turn() (state b): type=%d at (%d,%d,%d)", itr_units, _UNITS[itr_units].type, _UNITS[itr_units].wx, _UNITS[itr_units].wy, _UNITS[itr_units].wp);
            }
            else if(is_listed == ST_FALSE)
            {
                LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] unit %d dead after AI_Next_Turn(), not a pre-turn kill: type=%d hero_slot=%d at (%d,%d,%d)", itr_units, _UNITS[itr_units].type, _UNITS[itr_units].Hero_Slot, _UNITS[itr_units].wx, _UNITS[itr_units].wy, _UNITS[itr_units].wp);
            }
        }
    }

    /* Probe (a): the pre-turn kills, now wp 0 / owner -1 after AI_Next_Turn()'s sanity loop. */
    DeMoM_AI_Dead_Probe(&probe_a, probe_player_idx, dead_count, dead_units);

    fout = fopen(DEMOM_AI_DEAD_DUMP_FILE, "w");
    if(fout == NULL)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] Could not create: %s", DEMOM_AI_DEAD_DUMP_FILE);
        return 1;
    }

    fprintf(fout, "# DeMoM AI Dead Units Dump (BUGS_AND_QUIRKS #137)\n");
    fprintf(fout, "ai_dead.seed = %u\n", turn_seed);
    fprintf(fout, "ai_dead.units_before = %d\n", units_before);
    fprintf(fout, "ai_dead.dead_count = %d\n", dead_count);
    fprintf(fout, "ai_dead.heroes_killed = %d\n", heroes_killed);
    for(itr = 0; itr < dead_count; itr++)
    {
        fprintf(fout, "dead[%d].unit_idx = %d\n", itr, dead_units[itr]);
        fprintf(fout, "dead[%d].owner_before = %d\n", itr, dead_owner[itr]);
        fprintf(fout, "dead[%d].type = %d\n", itr, dead_type[itr]);
        fprintf(fout, "dead[%d].hero_slot = %d\n", itr, dead_hero_slot[itr]);
        fprintf(fout, "dead[%d].wp_before = %d\n", itr, dead_wp_before[itr]);
        fprintf(fout, "dead[%d].wp_after_kill = %d\n", itr, dead_wp_after_kill[itr]);
        fprintf(fout, "dead[%d].owner_after_turn = %d\n", itr, _UNITS[dead_units[itr]].owner_idx);
        fprintf(fout, "dead[%d].wp_after_turn = %d\n", itr, _UNITS[dead_units[itr]].wp);
        fprintf(fout, "dead[%d].hero_slot_after_turn = %d\n", itr, _UNITS[dead_units[itr]].Hero_Slot);
    }
    DeMoM_AI_Dead_Dump_Probe(fout, "probe_b", &probe_b, dead_count);
    fprintf(fout, "turn.num_players = %d\n", _num_players);
    fprintf(fout, "turn.active_ai_players = %d\n", active_ai_players);
    fprintf(fout, "turn.magic_master_idx = %d\n", magic_master_idx);
    fprintf(fout, "turn.rng_calls = %llu\n", (unsigned long long)(g_random_call_count - rng_calls_before));
    fprintf(fout, "turn.units_after = %d\n", _units);
    fprintf(fout, "turn.dead_records_after = %d\n", dead_after_turn);
    fprintf(fout, "turn.killed_in_turn_wp_undefined = %d\n", killed_in_turn_wp_undefined);
    DeMoM_AI_Dead_Dump_Probe(fout, "probe_a", &probe_a, dead_count);
    fprintf(fout, "check.ai_turn_completed = 1\n");
    fclose(fout);

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM AI Dead] Wrote %s", DEMOM_AI_DEAD_DUMP_FILE);
    return 0;
}



static void Print_Usage(const char *program_name)
{
    LOG_INFO(LOG_CAT_DEMOM, "DeMoM — Driven Master of Magic\n");
    LOG_INFO(LOG_CAT_DEMOM, "Usage:");
    LOG_INFO(LOG_CAT_DEMOM, "  %s --newgame [ReMoM.ini] [--scenario test.hms] [--record out.RMR]", program_name);
    LOG_INFO(LOG_CAT_DEMOM, "  %s --continue [--scenario test.hms] [--record out.RMR]", program_name);
    LOG_INFO(LOG_CAT_DEMOM, "  %s --load SAVE3.GAM [--scenario test.hms] [--record out.RMR]", program_name);
    LOG_INFO(LOG_CAT_DEMOM, "  %s --menu --scenario test.hms [--record out.RMR]", program_name);
    LOG_INFO(LOG_CAT_DEMOM, "  %s --newgame [ReMoM.ini] [--replay game.RMR]", program_name);
    LOG_INFO(LOG_CAT_DEMOM, "  %s --combat DEFENDER_UNIT ATTACKER_UNITS [--seed1 N]", program_name);
    LOG_INFO(LOG_CAT_DEMOM, "\nOptions:");
    LOG_INFO(LOG_CAT_DEMOM, "  --newgame [FILE]   Create new game from config (default: ReMoM.ini)");
    LOG_INFO(LOG_CAT_DEMOM, "  --continue         Load SAVE9.GAM (continue from previous --newgame)");
    LOG_INFO(LOG_CAT_DEMOM, "  --load FILE        Load a save file: SAVE1.GAM .. SAVE9.GAM (that slot), or any other file name or path");
    LOG_INFO(LOG_CAT_DEMOM, "  --menu             Start at the Main Menu with no game loaded (requires --scenario)");
    LOG_INFO(LOG_CAT_DEMOM, "  --scenario FILE    Run Automation Driver from scenario script (.hms)");
    LOG_INFO(LOG_CAT_DEMOM, "  --alias-points F   Named-action points table for `click Screen.Alias` (alias_points.fwv)");
    LOG_INFO(LOG_CAT_DEMOM, "  --replay FILE      Replay recorded input from .RMR file");
    LOG_INFO(LOG_CAT_DEMOM, "  --record FILE      Record input to .RMR file");
    LOG_INFO(LOG_CAT_DEMOM, "  --dump-save FILE   After Screen_Control returns, dump FILE.GAM to FILE.txt");
    LOG_INFO(LOG_CAT_DEMOM, "  --combat D A,B,..  Load SAVECMBT.GAM, run strategic combat: attacker units A,B,.. vs the stack of defender unit D; dump results to %s", DEMOM_COMBAT_DUMP_FILE);
    LOG_INFO(LOG_CAT_DEMOM, "  --combat-tactical D A,B,..  Same, but run the tactical battle screen with forced auto-combat (one side must be the human player)");
    LOG_INFO(LOG_CAT_DEMOM, "  --cast-summon S    Load SAVECMBT.GAM, leave a dead record past the last unit, cast creature-summon spell S for the human player; dump map focus to %s", DEMOM_SUMMON_DUMP_FILE);
    LOG_INFO(LOG_CAT_DEMOM, "  --ai-dead-units A,B,..  Load SAVECMBT.GAM, Kill_Unit() units A,B,.. without compacting, run AI_Next_Turn(); dump AI target-value probes to %s", DEMOM_AI_DEAD_DUMP_FILE);
    LOG_INFO(LOG_CAT_DEMOM, "  --help             Show this help");
}



/* ========================================================================= */
/*  Main                                                                     */
/* ========================================================================= */

int main(int argc, char *argv[])
{
    int demom_mode = 0;  /* 0=none, 1=newgame, 2=load, 3=combat, 4=cast-summon, 5=menu, 6=ai-dead-units */
    char demom_file[260] = { 0 };
    char demom_scenario[260] = { 0 };
    char demom_dump_save[260] = { 0 };
    int16_t combat_defender_unit_idx = ST_UNDEFINED;
    int16_t combat_troops[DEMOM_COMBAT_MAX_TROOPS] = { 0 };
    int16_t combat_troop_count = 0;
    int16_t combat_tactical = 0;
    int16_t summon_spell_idx = 0;
    int16_t ai_dead_units[DEMOM_AI_DEAD_MAX_UNITS] = { 0 };
    int16_t ai_dead_count = 0;
    int argi;

    STU_Log_Startup("ReMoM.ini");

    /* CLAUDE: install crash reporting right after logging exists.  The handler flushes STU_LOG
       before reporting -- the log is buffered, so an abort otherwise discards the whole run's log.
       Catches abort()/failed assertions as well as CPU faults; see STU/src/STU_BRAK.h. */
    STU_BRAK_Install();

    /* CLAUDE: performance capture (Mode A -- windowless logic timing).  Must come AFTER
       STU_Log_Startup() so the .fwv header can record the severity/CALL_TRACE state the capture ran
       under: numbers taken with tracing on measure the logger, not the game.  No-ops unless
       REMOM_PERF (or the REMOM_INPUT_METRICS alias) is set, and records nothing unless the build
       has PERF_INSTRUMENT.  Perf_Shutdown() runs via atexit(). */
    Perf_Init("windowless",
#ifdef STU_DEBUG
              "Debug"
#else
              "Release"
#endif
              );

    /* Windowless profile: data resolves from CWD (plus REMOM_DATA_DIR override),
       matching the existing test / matchup harness behavior. */
    STU_GRAF_Init(STU_GRAF_WINDOWLESS);

    /* Fail soft (to stderr, via the windowless Platform_Show_Error) if the game
       data isn't present. */
    if(ReMoM_Preflight_Game_Data() != 0)
    {
        return 1;
    }

    LOG_INFO(LOG_CAT_GENERAL, "BEGIN: DeMoM main() [STU_LOG tracer bullet]");
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: BEGIN: DeMoM  main()", __FILE__, __LINE__);
    LOG_TRACE(LOG_CAT_GENERAL, "TRACE: [%s, %d]: BEGIN: DeMoM  main()", __FILE__, __LINE__);

    /* Log and parse CLI arguments */
    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] argc=%d", argc);
    LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] argc=%d", argc);
    LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] argc=%d", argc);
    for (argi = 0; argi < argc; argi++)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] argv[%d]=\"%s\"", argi, argv[argi]);
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] argv[%d]=\"%s\"", argi, argv[argi]);
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] argv[%d]=\"%s\"", argi, argv[argi]);
    }

    /* CLAUDE: MoO2 pattern -- a single shared CLI-parser that sets _cmd_line_*
       globals.  Currently only handles --seed; remaining flags are still
       parsed by the per-target loop below during the incremental migration. */
    Check_Command_Line_Parameters_(argc, argv);

    for (argi = 1; argi < argc; argi++)
    {
        if(stu_strcmp(argv[argi], "--help") == 0 || stu_strcmp(argv[argi], "-h") == 0)
        {
            Print_Usage(argv[0]);
            return 0;
        }
        else if(stu_strcmp(argv[argi], "--newgame") == 0)
        {
            demom_mode = 1;
            if((argi + 1) < argc && argv[argi + 1][0] != '-')
            {
                argi++;
                stu_strcpy(demom_file, argv[argi]);
            }
            else
            {
                stu_strcpy(demom_file, "ReMoM.ini");
            }
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --newgame \"%s\"", demom_file);
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] CLI: --newgame \"%s\"", demom_file);
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] CLI: --newgame \"%s\"", demom_file);
        }
        else if(stu_strcmp(argv[argi], "--load") == 0 && (argi + 1) < argc)
        {
            demom_mode = 2;
            argi++;
            stu_strcpy(demom_file, argv[argi]);
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --load \"%s\"", demom_file);
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] CLI: --load \"%s\"", demom_file);
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] CLI: --load \"%s\"", demom_file);
        }
        else if(stu_strcmp(argv[argi], "--continue") == 0)
        {
            demom_mode = 2;
            stu_strcpy(demom_file, "SAVE9.GAM");
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --continue (SAVE9.GAM)");
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] CLI: --continue (SAVE9.GAM)");
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] CLI: --continue (SAVE9.GAM)");
        }
        else if(stu_strcmp(argv[argi], "--replay") == 0 || stu_strcmp(argv[argi], "--record") == 0)
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: %s (deferred until after platform init)", argv[argi]);
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] CLI: %s (deferred until after platform init)", argv[argi]);
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] CLI: %s (deferred until after platform init)", argv[argi]);
            /* Handled after Startup_Platform() */
        }
        else if(stu_strcmp(argv[argi], "--menu") == 0)
        {
            demom_mode = 5;
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --menu (start at the Main Menu, no game)");
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] CLI: --menu (start at the Main Menu, no game)");
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] CLI: --menu (start at the Main Menu, no game)");
        }
        else if(stu_strcmp(argv[argi], "--scenario") == 0 && (argi + 1) < argc)
        {
            argi++;
            stu_strcpy(demom_scenario, argv[argi]);
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --scenario \"%s\"", demom_scenario);
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] CLI: --scenario \"%s\"", demom_scenario);
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] CLI: --scenario \"%s\"", demom_scenario);
        }
        else if(stu_strcmp(argv[argi], "--alias-points") == 0 && (argi + 1) < argc)
        {
            argi++;
            Automation_Driver_Set_Alias_Points_Path(argv[argi]);
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --alias-points \"%s\"", argv[argi]);
        }
        else if(strcmp(argv[argi], "--dump-save") == 0 && (argi + 1) < argc)
        {
            argi++;
            stu_strcpy(demom_dump_save, argv[argi]);
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --dump-save \"%s\"", demom_dump_save);
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] CLI: --dump-save \"%s\"", demom_dump_save);
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] CLI: --dump-save \"%s\"", demom_dump_save);
        }
        else if((stu_strcmp(argv[argi], "--seed1") == 0 || stu_strcmp(argv[argi], "--seed2") == 0) && (argi + 1) < argc)
        {
            /* CLAUDE: --seed1/--seed2 parsing lives in MOX2::Check_Command_Line_Parameters_().
               Skip the value here so the rest of this loop doesn't see it as
               an "unknown arg". */
            argi++;
        }
        else if(stu_strcmp(argv[argi], "--seed") == 0 && (argi + 1) < argc)
        {
            /* CLAUDE: --seed was renamed --seed1 in MOX2 (2026-06-28, a8da405b); warn loudly instead of silently running unpinned. */
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --seed is no longer parsed -- use --seed1 N (RNG will NOT be pinned this run)");
            argi++;
        }
        else if((stu_strcmp(argv[argi], "--combat") == 0 || stu_strcmp(argv[argi], "--combat-tactical") == 0) && (argi + 2) < argc)
        {
            char * token;
            demom_mode = 3;
            combat_tactical = (stu_strcmp(argv[argi], "--combat-tactical") == 0) ? 1 : 0;
            stu_strcpy(demom_file, "SAVECMBT.GAM");
            argi++;
            combat_defender_unit_idx = (int16_t)stu_atoi(argv[argi]);
            argi++;
            token = strtok(argv[argi], ",");
            while(token != NULL && combat_troop_count < DEMOM_COMBAT_MAX_TROOPS)
            {
                combat_troops[combat_troop_count] = (int16_t)stu_atoi(token);
                combat_troop_count++;
                token = strtok(NULL, ",");
            }
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: %s defender_unit=%d attacker_troop_count=%d", combat_tactical ? "--combat-tactical" : "--combat", combat_defender_unit_idx, combat_troop_count);
        }
        else if(stu_strcmp(argv[argi], "--cast-summon") == 0 && (argi + 1) < argc)
        {
            demom_mode = 4;
            stu_strcpy(demom_file, "SAVECMBT.GAM");
            argi++;
            summon_spell_idx = (int16_t)stu_atoi(argv[argi]);
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --cast-summon spell_idx=%d", summon_spell_idx);
        }
        else if(stu_strcmp(argv[argi], "--ai-dead-units") == 0 && (argi + 1) < argc)
        {
            char * token;
            demom_mode = 6;
            stu_strcpy(demom_file, "SAVECMBT.GAM");
            argi++;
            token = strtok(argv[argi], ",");
            while(token != NULL && ai_dead_count < DEMOM_AI_DEAD_MAX_UNITS)
            {
                ai_dead_units[ai_dead_count] = (int16_t)stu_atoi(token);
                ai_dead_count++;
                token = strtok(NULL, ",");
            }
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --ai-dead-units count=%d", ai_dead_count);
        }
        else if(strcmp(argv[argi], "--ai-metrics") == 0)
        {
#ifdef STU_DEBUG
            AI_Metrics_Enabled = 1;
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --ai-metrics (enabled)");
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] CLI: --ai-metrics (enabled)");
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] CLI: --ai-metrics (enabled)");
#else
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: --ai-metrics ignored (not a debug build)");
#endif
        }
        else
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] CLI: unknown arg \"%s\"", argv[argi]);
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] CLI: unknown arg \"%s\"", argv[argi]);
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] CLI: unknown arg \"%s\"", argv[argi]);
        }
    }

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Parsed: mode=%d file=\"%s\" scenario=\"%s\"", demom_mode, demom_file, demom_scenario);
    LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Parsed: mode=%d file=\"%s\" scenario=\"%s\"", demom_mode, demom_file, demom_scenario);
    LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Parsed: mode=%d file=\"%s\" scenario=\"%s\"", demom_mode, demom_file, demom_scenario);

#ifdef STU_DEBUG
    AI_Metrics_Startup();
#endif

    if(demom_mode == 0)
    {
        Print_Usage(argv[0]);
        LOG_INFO(LOG_CAT_DEMOM, "\nError: must specify --newgame or --load");
        return 1;
    }

    if((demom_mode == 5) && (demom_scenario[0] == '\0'))
    {
        Print_Usage(argv[0]);
        LOG_INFO(LOG_CAT_DEMOM, "\nError: --menu requires --scenario (windowless has no other input)");
        return 1;
    }

    Startup_Platform();

    /* Register replay callbacks */
    Platform_Replay_Register_Random_Seed_Callbacks(Get_Random_Seed, Set_Random_Seed);
    Platform_Replay_Register_Field_Log_Callback(DeMoM_Replay_Log_Field_Hit);

    /* Process --replay and --record flags (after platform init) */
    for (argi = 1; argi < argc; argi++)
    {
        if(stu_strcmp(argv[argi], "--record") == 0 && (argi + 1) < argc)
        {
            argi++;
            Platform_Record_Start(argv[argi]);
        }
        else if(stu_strcmp(argv[argi], "--replay") == 0 && (argi + 1) < argc)
        {
            argi++;
            Platform_Replay_Start(argv[argi]);
        }
    }

    /* Initialize engine (shared with ReMoM) */
    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Initializing engine...");
    ReMoM_Init_Engine();

    /* The Main Menu draws its credits scroll into _credits_pict_seg (Draw_Credits), which only Init_Credits() allocates;
       ReMoM.c calls it at startup, after ReMoM_Init_Engine().  DeMoM reaches the Main Menu in --menu mode and whenever a
       game ends or is quit to the menu, and without this Draw_Credits() wrote through the never-allocated picture
       (Create_Picture crash) once the menu had been up for 70 frames. */
    Init_Credits();

    /* Create or load game */
    if(demom_mode == 1)
    {
        struct s_DeMoM_Config cfg;
        Config_Init(&cfg);
        if(!Config_Parse(demom_file, &cfg))
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Failed to parse config: %s", demom_file);
            Shutdown_Platform();
            return 1;
        }
        Config_Apply_And_Create_New_Game(&cfg);
        current_screen = scr_Main_Screen;
    }
    else if(demom_mode == 2)
    {
        int16_t save_slot = 0;

        save_slot = Save_Slot_From_File_Name(demom_file);

        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Loading save: %s (slot %d)", demom_file, save_slot);
        /* WIZARDS.EXE-path startup: load WZD resources before Load_SAVE_GAM,
           mirroring WIZARDS.EXE _main's `Load_WZD_Resources` -> `Load_SAVE_GAM`
           -> `Loaded_Game_Update` sequence. */
        Load_WZD_Resources();
        if(save_slot == ST_UNDEFINED)
        {
            /* Not SAVEn.GAM (SAVETEST.GAM, a fixture name, a path): load exactly that file, the way the fixture modes below do. */
            g_load_save_gam_name_override = demom_file;
        }
        Load_SAVE_GAM(save_slot);
        Loaded_Game_Update();
        /* CLAUDE: mirror the reset that Screen_Control's scr_Continue / scr_Load_Screen cases do
           (MoM/src/MOM_SCR.c:176 and :207).  m_magic_winner_idx is defined without an initializer
           (MoM/src/SPLMASTR.c:286), so it starts at 0 -- which reads as "player 0 has already won
           by Spell of Mastery", not "no winner" (ST_UNDEFINED = -1).  DeMoM's --load path builds
           the loaded-game state itself rather than going through those screen cases, so without
           this line the reset never happens.

           It stayed invisible because Get_Winner() is only consulted when an overland spell
           COMPLETES (AIDUDES.c:268, NEXTTURN.c:462, EVENTS.c:996, MainScr.c:5410).  On the fresh
           turn-0 saves the existing tests use, no spell finishes in five turns.  On a late-game
           save an AI finishes one on the first turn, Get_Winner() returns the uninitialized 0, and
           the EOG_HACK early-return at NEXTTURN.c:774 then truncates every later turn -- skipping
           _turn++ and the whole post-Event_Twiddle half of the pipeline. */
        m_magic_winner_idx = ST_UNDEFINED;
        g_conquest_winner_idx = ST_UNDEFINED;
        current_screen = scr_Main_Screen;
    }
    else if(demom_mode == 5)
    {
        /* CLAUDE: --menu -- what ReMoM.c does when there is no --continue / --load: palette 0, then
           Screen_Control() at the Main Menu (minus the logos and menu music; the credits are initialized above, after ReMoM_Init_Engine()).  No game is
           created or loaded, and no WZD resources are loaded -- scr_Continue does that itself
           (Load_WZD_Resources_Once) when the scenario starts or continues a game. */
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Starting at the Main Menu (no game)");
        Load_Palette(0, ST_UNDEFINED, 0);
        Apply_Palette();
        current_screen = scr_Main_Menu_Screen;
    }
    else if(demom_mode == 4)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Loading summon fixture save: %s", demom_file);
        Load_WZD_Resources();
        g_load_save_gam_name_override = demom_file;
        Load_SAVE_GAM(ST_UNDEFINED);
        Loaded_Game_Update();
        /* CLAUDE: same reset as the --load path and scr_Continue (MoM/src/MOM_SCR.c:258); m_magic_winner_idx has no initializer, so 0 would read as "player 0 has won" and stop AI_Next_Turn() at the EOG_HACK. */
        m_magic_winner_idx = ST_UNDEFINED;
        g_conquest_winner_idx = ST_UNDEFINED;
        current_screen = scr_Main_Screen;
        if(DeMoM_Summon_Run(summon_spell_idx) != 0)
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Summon run failed");
            Shutdown_Platform();
            return 1;
        }
    }
    else if(demom_mode == 6)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Loading AI dead-units fixture save: %s", demom_file);
        Load_WZD_Resources();
        g_load_save_gam_name_override = demom_file;
        Load_SAVE_GAM(ST_UNDEFINED);
        Loaded_Game_Update();
        current_screen = scr_Main_Screen;
        if(DeMoM_AI_Dead_Units_Run(ai_dead_count, ai_dead_units) != 0)
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] AI dead-units run failed");
            Shutdown_Platform();
            return 1;
        }
    }
    else if(demom_mode == 3)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Loading combat fixture save: %s", demom_file);
        Load_WZD_Resources();
        g_load_save_gam_name_override = demom_file;
        /* CLAUDE: --load trace -- mirrors the ReMoM.c --load trace points so the two paths can be
           compared.  NOTE this path passes ST_UNDEFINED, not a slot index. */
        LOG_INFO(LOG_CAT_LOADSAVE, "[--load] ASSIGN g_load_save_gam_name_override=%p \"%s\" (DeMoM combat-fixture path) -> Load_SAVE_GAM(ST_UNDEFINED)", (void *)g_load_save_gam_name_override, (g_load_save_gam_name_override != NULL) ? g_load_save_gam_name_override : "(null)");
        Load_SAVE_GAM(ST_UNDEFINED);
        Loaded_Game_Update();
        /* CLAUDE: same reset as the --load path and scr_Continue (MoM/src/MOM_SCR.c:258); m_magic_winner_idx has no initializer, so 0 would read as "player 0 has won" and stop AI_Next_Turn() at the EOG_HACK. */
        m_magic_winner_idx = ST_UNDEFINED;
        g_conquest_winner_idx = ST_UNDEFINED;
        current_screen = scr_Main_Screen;
        if(DeMoM_Combat_Run(combat_defender_unit_idx, combat_troop_count, combat_troops, combat_tactical) != 0)
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Combat run failed");
            Shutdown_Platform();
            return 1;
        }
    }

    /* Load and register Automation Driver scenario */
    if(demom_scenario[0] != '\0')
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Loading scenario: %s", demom_scenario);
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Loading scenario: %s", demom_scenario);
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Loading scenario: %s", demom_scenario);
        if(Automation_Driver_Load_Scenario(demom_scenario) != 0)
        {
            LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Failed to load scenario: %s", demom_scenario);
            LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Failed to load scenario: %s", demom_scenario);
            LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Failed to load scenario: %s", demom_scenario);
            Shutdown_Platform();
            return 1;
        }
        Platform_Register_Frame_Callback(Automation_Driver_Frame);
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Registered Automation Driver callback");
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Registered Automation Driver callback");
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Registered Automation Driver callback");
    }
    else
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] No --scenario specified");
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] No --scenario specified");
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] No --scenario specified");
    }

    /* Enter game loop — but skip it for --newgame without --scenario.
       Config_Apply_And_Create_New_Game() already wrote SAVE9.GAM, and DeMoM
       has no interactive UI, so entering Screen_Control() here would just
       spin forever waiting for input that can never arrive. */
    if(demom_mode == 3)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Combat run complete — skipping Screen_Control()");
    }
    else if(demom_mode == 4)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Summon run complete — skipping Screen_Control()");
    }
    else if(demom_mode == 6)
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] AI dead-units run complete — skipping Screen_Control()");
    }
    else if(demom_mode == 1 && demom_scenario[0] == '\0')
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Newgame complete — SAVE9.GAM written, skipping Screen_Control()");
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Newgame complete — SAVE9.GAM written, skipping Screen_Control()");
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Newgame complete — SAVE9.GAM written, skipping Screen_Control()");
    }
    else
    {
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Entering Screen_Control()");
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Entering Screen_Control()");
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Entering Screen_Control()");
        /* Crossing from MAGIC.EXE-path (--newgame setup) or no-flag init into
           WIZARDS.EXE-path (gameplay).  Load WZD resources now if mode 2 didn't
           already (mode 1 + --scenario, or no-flag interactive). */
        if((demom_mode != 2) && (demom_mode != 5))
        {
            Load_WZD_Resources();
        }
        Screen_Control();

        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Screen_Control() returned");
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Screen_Control() returned");
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Screen_Control() returned");
    }

    /* Dump a save file to text after the game loop exits.  Used by tests
       that need to inspect a save produced by the scenario. */
    if(demom_dump_save[0] != '\0')
    {
        char dump_text[260];
        size_t len = strlen(demom_dump_save);
        size_t i;
        stu_strcpy(dump_text, demom_dump_save);
        /* Replace ".GAM" suffix with ".txt" if present, else append ".txt". */
        if(len >= 4 && stu_stricmp(dump_text + len - 4, ".GAM") == 0)
        {
            dump_text[len - 4] = '\0';
        }
        i = strlen(dump_text);
        if((i + 4) < sizeof(dump_text))
        {
            dump_text[i + 0] = '.';
            dump_text[i + 1] = 't';
            dump_text[i + 2] = 'x';
            dump_text[i + 3] = 't';
            dump_text[i + 4] = '\0';
        }
        LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Dumping %s -> %s", demom_dump_save, dump_text);
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Dumping %s -> %s", demom_dump_save, dump_text);
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Dumping %s -> %s", demom_dump_save, dump_text);
        Game_Save_Dump(demom_dump_save, dump_text);
        LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Game_Save_Dump returned");
        LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Game_Save_Dump returned");
    }

    /* Cleanup */
    LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Cleanup: Automation_Driver_Shutdown()");
    LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Cleanup: Automation_Driver_Shutdown()");
    Automation_Driver_Shutdown();
    if(Platform_Record_Active()) { Platform_Record_Stop(); }
    if(Platform_Replay_Active()) { Platform_Replay_Stop(); }

    LOG_DEBUG(LOG_CAT_GENERAL, "[DeMoM] Cleanup: Shutdown_Platform()");
    LOG_TRACE(LOG_CAT_GENERAL, "[DeMoM] Cleanup: Shutdown_Platform()");
    Shutdown_Platform();

#ifdef STU_DEBUG
    AI_Metrics_Shutdown();
#endif

    LOG_INFO(LOG_CAT_GENERAL, "[POOL] peak bytes used = %u  (vs POOL_ARENA_CAPACITY)", Pool_Bytes_Peak());  /* CLAUDE: static-pool high-water mark; informs POOL_ARENA_CAPACITY sizing (PLAN-Static-Pool-Allocator Phase 2b) */
    LOG_INFO(LOG_CAT_INTEGRITY, "[POOL] guard mode = %d, arenas carved = %u, tracked = %u", Pool_Guards_Enabled(), Pool_Arena_Count(), Pool_Tracked_Arena_Count());  /* CLAUDE: proves whether the guard check below had anything to check */
    /* CLAUDE: final integrity checkpoint -- windowless runs (next-turn, AI, world-gen) may never reach Main_Screen_Draw(), so check once here; with REMOM_POOL_GUARDS=1 this catches any overrun the run caused */
    LOG_INFO(LOG_CAT_INTEGRITY, "[INTEGRITY] DeMoM shutdown: %d checks failing", Integrity_Checkpoint("DeMoM shutdown"));

    LOG_TRACE(LOG_CAT_GENERAL, "TRACE: [%s, %d]: END: DeMoM  main()", __FILE__, __LINE__);

    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: END: DeMoM  main()", __FILE__, __LINE__);

    LOG_INFO(LOG_CAT_GENERAL, "END: DeMoM main() [STU_LOG tracer bullet]");

    STU_Log_Shutdown();

    LOG_INFO(LOG_CAT_DEMOM, "[DeMoM] Done");
    return 0;
}
