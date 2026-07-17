/**
 * HeMoM.c — Headless ReMoM
 *
 * An entry point for headless/automated operation:
 *   --newgame [ReMoM.ini]   Create a new game from config (skip menus)
 *   --load SAVE3.GAM        Load a save file (skip menus)
 *   --continue              Same as 'Main Menu ' Continue (loads SAVE9.GAM)
 *   --replay game.RMR       Replay recorded input
 *   --record game.RMR       Record input to file
 *   --scenario gameplay.HMS play a scripted scenario (uses Artificial Human Player)
 *
 * No logos, no credits, no main menu music. Initializes the engine,
 * creates or loads a game, and enters Screen_Control().
 *
 * HeMoM is hardwired to the headless platform library — no display, no
 * audio, no external dependencies — regardless of which windowed backend
 * the build's preset chose for ReMoMber.
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../ext/stu_compat.h"
#ifdef STU_DEBUG
#include "../STU/src/AI_METRICS.h"
#include "../STU/src/STU_DBG.h"
#endif
#include "../STU/src/STU_LOG.h"
#include "../platform/include/Platform.h"
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

#include "../MoM/src/NewGame.h"
#include "../MoM/src/MAPGEN.h"
#include "../MoM/src/INITGAME.h"
#include "../MoM/src/LoadScr.h"
#include "../MoM/src/LOADER.h"   /* Load_WZD_Resources */
#include "../MoM/src/MOM_SCR.h"
#include "../MoM/src/Settings.h"
#include "../MoM/src/UNITTYPE.h"
#include "../MoM/src/Combat.h"   /* CLAUDE: Combat__WIP() + CMB_Gold_Reward for --combat test mode */
#include "../MoM/src/DIPLODEF.h" /* CLAUDE: DIPL_War for --combat test mode */
#include "../MoM/src/WZD_o143.h"  /* Random_City_Name_By_Race */

#include "ReMoM_Init.h"
#include "Artificial_Human_Player.h"
#include "Game_Save_Dump.h"

/* _wizard_presets_table is defined in NewGame.c but has no extern in a header */
extern struct s_WIZARD_PRESET _wizard_presets_table[];

/* NewGame.c */
// MGC  dseg:309C
extern char str_MAGIC_SET__ovr050[];
extern char str_rb__ovr050[];
extern char str_wb__ovr050[];



/* HeMoM is always headless — no SDL_main / SDL header pull-in needed. */



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

struct s_HeMoM_Config
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

static void Config_Init(struct s_HeMoM_Config *cfg)
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

static int Config_Parse(const char *filepath, struct s_HeMoM_Config *cfg)
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
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Could not open config: %s", filepath);
        return 0;
    }

    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Loading config: %s", filepath);

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        line_num++;
        key = Trim_Whitespace(line);

        if(*key == '\0' || *key == '#' || *key == ';') { continue; }
        if(*key == '[') { continue; }

        eq = stu_strchr(key, '=');
        if(eq == NULL)
        {
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] %s:%d: missing '=' in: %s", filepath, line_num, key);
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
                LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] %s:%d: unknown wizard '%s'", filepath, line_num, value);
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
                LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] %s:%d: unknown race '%s'", filepath, line_num, value);
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
                LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] %s:%d: unknown banner '%s'", filepath, line_num, value);
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
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] %s:%d: unknown key '%s'", filepath, line_num, key);
        }
    }

    stu_fclose(fp);
    return 1;
}



/* ========================================================================= */
/*  New Game Creation                                                        */
/* ========================================================================= */

/* These globals live in NewGame.c and are not declared in NewGame.h.  We
   reference them here so HeMoM can re-fill _player_start_spells[0] after
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

   Reason for the local copy: HeMoM overrides spellranks[] AFTER
   Human_Player_Wizard_Profile has already run, so the original inline
   fill becomes stale.  Re-running this against the post-override
   spellranks brings _player_start_spells[0] back in sync with the
   configured books — without that, Init_Computer_Players_Spell_Library
   sees a stale start-spell pool and produces a different spells_list[]
   than ReMoM's menu path, which surfaces as a Layer-A divergence inside
   Player_Research_Spells (Spells128.c:121).

   See doc/Devel-HeMoM-Newgame-Path-Alignment.md gap #2.

   Behavior is verbatim — including the pre-existing off-by-one indexing
   in the m_select_count_* lookups (doc gap #5).  Do not "fix" the
   indexing here: bug-for-bug equivalence with the game code is what
   keeps the RNG trace aligned. */
static void HeMoM_Refill_Human_Player_Start_Spells(void)
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
static void New_Game_Settings(struct s_HeMoM_Config *cfg)
{
    char file_found[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FILE * file_pointer = 0;
#ifdef STU_DEBUG
    struct s_MAGIC_SET magic_set_snapshot;
#endif

/*
    BEGIN: Game Settings Load
*/
    if((DIR(str_MAGIC_SET__ovr050, file_found) == 0) || (LOF(str_MAGIC_SET__ovr050) == 0))
    {
        Set_Default_Game_Settings();
    }
    else
    {
        file_pointer = stu_fopen_ci(str_MAGIC_SET__ovr050, str_rb__ovr050);
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
    file_pointer = stu_fopen_ci(str_MAGIC_SET__ovr050, str_wb__ovr050);
    stu_fwrite(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_pointer);
    stu_fclose(file_pointer);
    _landsize = magic_set.LandSize;  /* ~== NewGame.c  Line 1820 */
    _magic = magic_set.MagicPower;
    _num_players = (magic_set.Opponents + 1);
    _difficulty = magic_set.Difficulty;

}
static void Config_Apply_And_Create_New_Game(struct s_HeMoM_Config *cfg)
{
    int8_t * wsa_ptr = NULL;
    int itr = 0;

    wsa_ptr = &_players[0].alchemy;

    Human_Player_Wizard_Profile(cfg->wizard_id);  /* ~== Newgame_Screen_1__WIP() Line 2294 */

    /*
        "Wizard Creation"  (Custom Wizard)
        Newgame_Screen_4__WIP()
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
           See doc/Devel-HeMoM-Newgame-Path-Alignment.md gap #2. */
        HeMoM_Refill_Human_Player_Start_Spells();
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
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] RNG seed set to %u (0x%08X)", cfg->seed, cfg->seed);
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] RNG seed set to %u (0x%08X)", cfg->seed, cfg->seed);
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] RNG seed set to %u (0x%08X)", cfg->seed, cfg->seed);
#endif
    }

    /*
    Mirror the ReMoM Screen 0 OK path: Randomize_Book_Heights consumes 66
       Random() draws.  Without this, HeMoM enters Init_New_Game with the RNG
       cursor 66 ticks ahead of ReMoM and every downstream world-gen draw
       diverges.  See doc/Devel-HeMoM-Newgame-Path-Alignment.md.
        This includes the first call to Random().
        This is the entirety of the code from the 'New Game Screen Functions'
    */
    /* Newgame_Screen_0 */
    New_Game_Settings(cfg);
    Randomize_Book_Heights();

    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Creating new game: difficulty=%d magic=%d landsize=%d opponents=%d wizard=%s race=%d banner=%d seed=%u", cfg->difficulty, cfg->magic, cfg->landsize, cfg->opponents, cfg->wizard_name, cfg->race, cfg->banner, Get_Random_Seed());
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Creating new game: difficulty=%d magic=%d landsize=%d opponents=%d wizard=%s race=%d banner=%d seed=%u", cfg->difficulty, cfg->magic, cfg->landsize, cfg->opponents, cfg->wizard_name, cfg->race, cfg->banner, Get_Random_Seed());
    LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Creating new game: difficulty=%d magic=%d landsize=%d opponents=%d wizard=%s race=%d banner=%d seed=%u", cfg->difficulty, cfg->magic, cfg->landsize, cfg->opponents, cfg->wizard_name, cfg->race, cfg->banner, Get_Random_Seed());
#endif

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
           would zero-pad the tail and diverge from ReMoMber's SAVE bytes. */
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
    
    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] New game created successfully");
}



/* ========================================================================= */
/*  Replay Callback                                                         */
/* ========================================================================= */

static void HeMoM_Replay_Log_Field_Hit(void *log, int mouse_x, int mouse_y)
{
    FILE *fp = (FILE *)log;
    int fi;
    for (fi = 0; fi < fields_count; fi++)
    {
        if(mouse_x >= p_fields[fi].x1 && mouse_x <= p_fields[fi].x2 && mouse_y >= p_fields[fi].y1 && mouse_y <= p_fields[fi].y2)
        {
            fprintf(fp, "  field[%d]=(%d,%d)-(%d,%d)", fi, p_fields[fi].x1, p_fields[fi].y1, p_fields[fi].x2, p_fields[fi].y2);
            break;
        }
    }
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
/*  shift the combat rolls), invokes Combat__WIP() directly, and dumps the   */
/*  results as `key = value` text for the CTest assertion step.              */
/*                                                                           */
/*  Both combatants must be computer players: the dispatcher then routes to  */
/*  Strategic_Combat__WIP() (auto-resolve), which runs headlessly.  A human  */
/*  combatant would route to the tactical battle screen, which this tracer   */
/*  does not support yet.                                                    */
/* ========================================================================= */

#define HEMOM_COMBAT_MAX_TROOPS 9
#define HEMOM_COMBAT_DUMP_FILE "HEMOM_COMBAT.txt"

static void HeMoM_Combat_Dump_Unit(FILE * fout, const char * prefix, int itr, int16_t unit_idx)
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

static int HeMoM_Combat_Run(int16_t defender_unit_idx, int16_t troop_count, int16_t troops[], int16_t tactical)
{
    FILE * fout = NULL;
    int16_t attacker_player_idx = 0;
    int16_t defender_player_idx = 0;
    int16_t defender_stack[HEMOM_COMBAT_MAX_TROOPS];
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
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] No attacker troops given");
        return 1;
    }
    if(defender_unit_idx < 0 || defender_unit_idx >= _units)
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] Defender unit idx %d out of range (0..%d)", defender_unit_idx, _units - 1);
        return 1;
    }
    for(itr = 0; itr < troop_count; itr++)
    {
        if(troops[itr] < 0 || troops[itr] >= _units)
        {
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] Attacker unit idx %d out of range (0..%d)", troops[itr], _units - 1);
            return 1;
        }
    }

    attacker_player_idx = _UNITS[troops[0]].owner_idx;
    defender_player_idx = _UNITS[defender_unit_idx].owner_idx;
    defender_wx = _UNITS[defender_unit_idx].wx;
    defender_wy = _UNITS[defender_unit_idx].wy;
    defender_wp = _UNITS[defender_unit_idx].wp;

    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] attacker_player=%d defender_player=%d human_player=%d defender_at=(%d,%d,%d)", attacker_player_idx, defender_player_idx, _human_player_idx, defender_wx, defender_wy, defender_wp);

    if(attacker_player_idx == defender_player_idx)
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] Attacker and defender have the same owner (%d)", attacker_player_idx);
        return 1;
    }
    if(tactical == 1)
    {
        /* The dispatcher only routes to the tactical battle screen when a human participates and the "Strategic Combat Only" option is off; g_cmbt_force_auto_combat then makes the AI drive both sides so no input is needed. */
        if((attacker_player_idx != _human_player_idx) && (defender_player_idx != _human_player_idx))
        {
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] --combat-tactical requires one combatant to be the human player (%d); the dispatcher would auto-resolve an AI-vs-AI fight", _human_player_idx);
            return 1;
        }
        magic_set.strategic_combat_only = ST_FALSE;
        g_cmbt_force_auto_combat = ST_TRUE;
    }
    else
    {
        if((attacker_player_idx == _human_player_idx) || (defender_player_idx == _human_player_idx))
        {
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] A combatant is the human player; the dispatcher would open the tactical battle screen -- use --combat-tactical for that");
            return 1;
        }
    }

    /* Capture defender stack membership before combat mutates the unit array. */
    for(itr = 0; itr < _units && defender_stack_count < HEMOM_COMBAT_MAX_TROOPS; itr++)
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

    /* Patch diplomacy: Combat__WIP() silently aborts (No_Combat) when a non-human attacker strikes an Alliance/WizardPact partner, so force a state of war between the combatants in both directions. */
    _players[attacker_player_idx].Dipl.Dipl_Status[defender_player_idx] = DIPL_War;
    _players[defender_player_idx].Dipl.Dipl_Status[attacker_player_idx] = DIPL_War;

    /* Patch the overland state Combat__WIP() reads for a stack-vs-stack attack.  OVL_Action_OriginX/Y is the square the attackers strike from; Combat__WIP() moves the troops there at entry, so it just needs to be a square adjacent to the defender. */
    _combat_environ = 0;      /* cnv_Enemy_Stack */
    _combat_environ_idx = 0;  /* unused for stack-vs-stack */
    OVL_Action_OriginX = (defender_wx > 0) ? (int16_t)(defender_wx - 1) : (int16_t)(defender_wx + 1);
    OVL_Action_OriginY = defender_wy;
    for(itr = 0; itr < troop_count; itr++)
    {
        _UNITS[troops[itr]].Status = us_Ready;
    }

    if(tactical == 1)
    {
        /* Cosmetic-only settings the tactical battle screen honors; disabling them skips per-frame draw/sleep loops (movement animations especially) that make a headless AI-vs-AI battle enormously slow without affecting the combat outcome. */
        magic_set.movement_animations = ST_FALSE;
        magic_set.sound_effects = ST_FALSE;
        magic_set.background_music = ST_FALSE;
    }

    combat_seed = (_cmd_line_seed != 0) ? (uint32_t)_cmd_line_seed : 12345u;
    Set_Random_Seed(combat_seed);
    rng_calls_before = g_random_call_count;

    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] Invoking Combat__WIP() seed=%u troop_count=%d defender_stack_count=%d", combat_seed, troop_count, defender_stack_count);

    attacker_won = Combat__WIP(attacker_player_idx, defender_unit_idx, troop_count, troops);

    g_cmbt_force_auto_combat = ST_FALSE;

    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] Combat__WIP() returned %d", attacker_won);

    fout = fopen(HEMOM_COMBAT_DUMP_FILE, "w");
    if(fout == NULL)
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] Could not create: %s", HEMOM_COMBAT_DUMP_FILE);
        return 1;
    }

    fprintf(fout, "# HeMoM Combat Dump\n");
    fprintf(fout, "# Mode: %s (direct Combat__WIP invoke)\n", (tactical == 1) ? "tactical AI-vs-AI (forced auto-combat)" : "strategic");
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
    fprintf(fout, "combat.gold_reward = %d\n", CMB_Gold_Reward);
    fprintf(fout, "combat.rng_calls = %llu\n", (unsigned long long)(g_random_call_count - rng_calls_before));
    fprintf(fout, "combat.random_seed_after = %u\n", Get_Random_Seed());
    fprintf(fout, "game.units_after = %d\n", _units);
    for(itr = 0; itr < troop_count; itr++)
    {
        HeMoM_Combat_Dump_Unit(fout, "troop", itr, troops[itr]);
    }
    for(itr = 0; itr < defender_stack_count; itr++)
    {
        HeMoM_Combat_Dump_Unit(fout, "defender", itr, defender_stack[itr]);
    }
    fclose(fout);

    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM Combat] Wrote %s", HEMOM_COMBAT_DUMP_FILE);
    return 0;
}



static void Print_Usage(const char *program_name)
{
    LOG_INFO(LOG_CAT_HEMOM, "HeMoM — Headless Master of Magic\n");
    LOG_INFO(LOG_CAT_HEMOM, "Usage:");
    LOG_INFO(LOG_CAT_HEMOM, "  %s --newgame [ReMoM.ini] [--scenario test.hms] [--record out.RMR]", program_name);
    LOG_INFO(LOG_CAT_HEMOM, "  %s --continue [--scenario test.hms] [--record out.RMR]", program_name);
    LOG_INFO(LOG_CAT_HEMOM, "  %s --load SAVE3.GAM [--scenario test.hms] [--record out.RMR]", program_name);
    LOG_INFO(LOG_CAT_HEMOM, "  %s --newgame [ReMoM.ini] [--replay game.RMR]", program_name);
    LOG_INFO(LOG_CAT_HEMOM, "  %s --combat DEFENDER_UNIT ATTACKER_UNITS [--seed1 N]", program_name);
    LOG_INFO(LOG_CAT_HEMOM, "\nOptions:");
    LOG_INFO(LOG_CAT_HEMOM, "  --newgame [FILE]   Create new game from config (default: ReMoM.ini)");
    LOG_INFO(LOG_CAT_HEMOM, "  --continue         Load SAVE9.GAM (continue from previous --newgame)");
    LOG_INFO(LOG_CAT_HEMOM, "  --load FILE        Load a save file (SAVE1.GAM .. SAVE9.GAM, SAVETEST.GAM)");
    LOG_INFO(LOG_CAT_HEMOM, "  --scenario FILE    Run artificial human player from scenario script (.hms)");
    LOG_INFO(LOG_CAT_HEMOM, "  --replay FILE      Replay recorded input from .RMR file");
    LOG_INFO(LOG_CAT_HEMOM, "  --record FILE      Record input to .RMR file");
    LOG_INFO(LOG_CAT_HEMOM, "  --dump-save FILE   After Screen_Control returns, dump FILE.GAM to FILE.txt");
    LOG_INFO(LOG_CAT_HEMOM, "  --combat D A,B,..  Load SAVECMBT.GAM, run strategic combat: attacker units A,B,.. vs the stack of defender unit D; dump results to %s", HEMOM_COMBAT_DUMP_FILE);
    LOG_INFO(LOG_CAT_HEMOM, "  --combat-tactical D A,B,..  Same, but run the tactical battle screen with forced auto-combat (one side must be the human player)");
    LOG_INFO(LOG_CAT_HEMOM, "  --help             Show this help");
}



/* ========================================================================= */
/*  Main                                                                     */
/* ========================================================================= */

int main(int argc, char *argv[])
{
    int hemom_mode = 0;  /* 0=none, 1=newgame, 2=load, 3=combat */
    char hemom_file[260] = { 0 };
    char hemom_scenario[260] = { 0 };
    char hemom_dump_save[260] = { 0 };
    int16_t combat_defender_unit_idx = ST_UNDEFINED;
    int16_t combat_troops[HEMOM_COMBAT_MAX_TROOPS] = { 0 };
    int16_t combat_troop_count = 0;
    int16_t combat_tactical = 0;
    int argi;

    STU_Log_Startup("ReMoM.ini");
    LOG_INFO(LOG_CAT_GENERAL, "BEGIN: HeMoM main() [STU_LOG tracer bullet]");
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: BEGIN: HeMoM  main()", __FILE__, __LINE__);
    LOG_TRACE(LOG_CAT_GENERAL, "TRACE: [%s, %d]: BEGIN: HeMoM  main()", __FILE__, __LINE__);

    /* Log and parse CLI arguments */
    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] argc=%d", argc);
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] argc=%d", argc);
    LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] argc=%d", argc);
#endif
    for (argi = 0; argi < argc; argi++)
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] argv[%d]=\"%s\"", argi, argv[argi]);
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] argv[%d]=\"%s\"", argi, argv[argi]);
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] argv[%d]=\"%s\"", argi, argv[argi]);
#endif
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
            hemom_mode = 1;
            if((argi + 1) < argc && argv[argi + 1][0] != '-')
            {
                argi++;
                stu_strcpy(hemom_file, argv[argi]);
            }
            else
            {
                stu_strcpy(hemom_file, "ReMoM.ini");
            }
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: --newgame \"%s\"", hemom_file);
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] CLI: --newgame \"%s\"", hemom_file);
            LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] CLI: --newgame \"%s\"", hemom_file);
#endif
        }
        else if(stu_strcmp(argv[argi], "--load") == 0 && (argi + 1) < argc)
        {
            hemom_mode = 2;
            argi++;
            stu_strcpy(hemom_file, argv[argi]);
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: --load \"%s\"", hemom_file);
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] CLI: --load \"%s\"", hemom_file);
            LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] CLI: --load \"%s\"", hemom_file);
#endif
        }
        else if(stu_strcmp(argv[argi], "--continue") == 0)
        {
            hemom_mode = 2;
            stu_strcpy(hemom_file, "SAVE9.GAM");
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: --continue (SAVE9.GAM)");
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] CLI: --continue (SAVE9.GAM)");
            LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] CLI: --continue (SAVE9.GAM)");
#endif
        }
        else if(stu_strcmp(argv[argi], "--replay") == 0 || stu_strcmp(argv[argi], "--record") == 0)
        {
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: %s (deferred until after platform init)", argv[argi]);
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] CLI: %s (deferred until after platform init)", argv[argi]);
            LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] CLI: %s (deferred until after platform init)", argv[argi]);
#endif
            /* Handled after Startup_Platform() */
        }
        else if(stu_strcmp(argv[argi], "--scenario") == 0 && (argi + 1) < argc)
        {
            argi++;
            stu_strcpy(hemom_scenario, argv[argi]);
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: --scenario \"%s\"", hemom_scenario);
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] CLI: --scenario \"%s\"", hemom_scenario);
            LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] CLI: --scenario \"%s\"", hemom_scenario);
#endif
        }
        else if(strcmp(argv[argi], "--dump-save") == 0 && (argi + 1) < argc)
        {
            argi++;
            stu_strcpy(hemom_dump_save, argv[argi]);
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: --dump-save \"%s\"", hemom_dump_save);
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] CLI: --dump-save \"%s\"", hemom_dump_save);
            LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] CLI: --dump-save \"%s\"", hemom_dump_save);
#endif
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
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: --seed is no longer parsed -- use --seed1 N (RNG will NOT be pinned this run)");
            argi++;
        }
        else if((stu_strcmp(argv[argi], "--combat") == 0 || stu_strcmp(argv[argi], "--combat-tactical") == 0) && (argi + 2) < argc)
        {
            char * token;
            hemom_mode = 3;
            combat_tactical = (stu_strcmp(argv[argi], "--combat-tactical") == 0) ? 1 : 0;
            stu_strcpy(hemom_file, "SAVECMBT.GAM");
            argi++;
            combat_defender_unit_idx = (int16_t)stu_atoi(argv[argi]);
            argi++;
            token = strtok(argv[argi], ",");
            while(token != NULL && combat_troop_count < HEMOM_COMBAT_MAX_TROOPS)
            {
                combat_troops[combat_troop_count] = (int16_t)stu_atoi(token);
                combat_troop_count++;
                token = strtok(NULL, ",");
            }
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: %s defender_unit=%d attacker_troop_count=%d", combat_tactical ? "--combat-tactical" : "--combat", combat_defender_unit_idx, combat_troop_count);
        }
        else if(strcmp(argv[argi], "--ai-metrics") == 0)
        {
#ifdef STU_DEBUG
            AI_Metrics_Enabled = 1;
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: --ai-metrics (enabled)");
            LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] CLI: --ai-metrics (enabled)");
            LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] CLI: --ai-metrics (enabled)");
#else
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: --ai-metrics ignored (not a debug build)");
#endif
        }
        else
        {
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] CLI: unknown arg \"%s\"", argv[argi]);
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] CLI: unknown arg \"%s\"", argv[argi]);
            LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] CLI: unknown arg \"%s\"", argv[argi]);
#endif
        }
    }

    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Parsed: mode=%d file=\"%s\" scenario=\"%s\"", hemom_mode, hemom_file, hemom_scenario);
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Parsed: mode=%d file=\"%s\" scenario=\"%s\"", hemom_mode, hemom_file, hemom_scenario);
    LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Parsed: mode=%d file=\"%s\" scenario=\"%s\"", hemom_mode, hemom_file, hemom_scenario);
#endif

#ifdef STU_DEBUG
    AI_Metrics_Startup();
#endif

    if(hemom_mode == 0)
    {
        Print_Usage(argv[0]);
        LOG_INFO(LOG_CAT_HEMOM, "\nError: must specify --newgame or --load");
        return 1;
    }

    Startup_Platform();

    /* Register replay callbacks */
    Platform_Replay_Register_Random_Seed_Callbacks(Get_Random_Seed, Set_Random_Seed);
    Platform_Replay_Register_Field_Log_Callback(HeMoM_Replay_Log_Field_Hit);

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

    /* Initialize engine (shared with ReMoMber) */
    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Initializing engine...");
    ReMoM_Init_Engine();

    /* Create or load game */
    if(hemom_mode == 1)
    {
        struct s_HeMoM_Config cfg;
        Config_Init(&cfg);
        if(!Config_Parse(hemom_file, &cfg))
        {
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Failed to parse config: %s", hemom_file);
            Shutdown_Platform();
            return 1;
        }
        Config_Apply_And_Create_New_Game(&cfg);
        current_screen = scr_Main_Screen;
    }
    else if(hemom_mode == 2)
    {
        int16_t save_slot;

        if(Str_Equal_CI(hemom_file, "SAVETEST.GAM"))
        {
            save_slot = ST_UNDEFINED;
        }
        else
        {
            const char *digit_ptr = hemom_file + 4;
            save_slot = (int16_t)(stu_atoi(digit_ptr) - 1);
        }

        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Loading save: %s (slot %d)", hemom_file, save_slot);
        /* WIZARDS.EXE-path startup: load WZD resources before Load_SAVE_GAM,
           mirroring WIZARDS.EXE _main's `Load_WZD_Resources` -> `Load_SAVE_GAM`
           -> `Loaded_Game_Update` sequence. */
        Load_WZD_Resources();
        Load_SAVE_GAM(save_slot);
        Loaded_Game_Update();
        current_screen = scr_Main_Screen;
    }
    else if(hemom_mode == 3)
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Loading combat fixture save: %s", hemom_file);
        Load_WZD_Resources();
        g_load_save_gam_name_override = hemom_file;
        Load_SAVE_GAM(ST_UNDEFINED);
        Loaded_Game_Update();
        current_screen = scr_Main_Screen;
        if(HeMoM_Combat_Run(combat_defender_unit_idx, combat_troop_count, combat_troops, combat_tactical) != 0)
        {
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Combat run failed");
            Shutdown_Platform();
            return 1;
        }
    }

    /* Load and register artificial human player scenario */
    if(hemom_scenario[0] != '\0')
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Loading scenario: %s", hemom_scenario);
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Loading scenario: %s", hemom_scenario);
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Loading scenario: %s", hemom_scenario);
#endif
        if(HeMoM_Player_Load_Scenario(hemom_scenario) != 0)
        {
            LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Failed to load scenario: %s", hemom_scenario);
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Failed to load scenario: %s", hemom_scenario);
            LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Failed to load scenario: %s", hemom_scenario);
#endif
            Shutdown_Platform();
            return 1;
        }
        Platform_Register_Frame_Callback(HeMoM_Player_Frame);
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Registered artificial human player callback");
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Registered artificial human player callback");
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Registered artificial human player callback");
#endif
    }
    else
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] No --scenario specified");
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] No --scenario specified");
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] No --scenario specified");
#endif
    }

    /* Enter game loop — but skip it for --newgame without --scenario.
       Config_Apply_And_Create_New_Game() already wrote SAVE9.GAM, and HeMoM
       has no interactive UI, so entering Screen_Control() here would just
       spin forever waiting for input that can never arrive. */
    if(hemom_mode == 3)
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Combat run complete — skipping Screen_Control()");
    }
    else if(hemom_mode == 1 && hemom_scenario[0] == '\0')
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Newgame complete — SAVE9.GAM written, skipping Screen_Control()");
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Newgame complete — SAVE9.GAM written, skipping Screen_Control()");
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Newgame complete — SAVE9.GAM written, skipping Screen_Control()");
#endif
    }
    else
    {
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Entering Screen_Control()");
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Entering Screen_Control()");
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Entering Screen_Control()");
#endif
        /* Crossing from MAGIC.EXE-path (--newgame setup) or no-flag init into
           WIZARDS.EXE-path (gameplay).  Load WZD resources now if mode 2 didn't
           already (mode 1 + --scenario, or no-flag interactive). */
        if(hemom_mode != 2)
        {
            Load_WZD_Resources();
        }
        Screen_Control();

        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Screen_Control() returned");
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Screen_Control() returned");
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Screen_Control() returned");
#endif
    }

    /* Dump a save file to text after the game loop exits.  Used by tests
       that need to inspect a save produced by the scenario. */
    if(hemom_dump_save[0] != '\0')
    {
        char dump_text[260];
        size_t len = strlen(hemom_dump_save);
        size_t i;
        stu_strcpy(dump_text, hemom_dump_save);
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
        LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Dumping %s -> %s", hemom_dump_save, dump_text);
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Dumping %s -> %s", hemom_dump_save, dump_text);
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Dumping %s -> %s", hemom_dump_save, dump_text);
#endif
        Game_Save_Dump(hemom_dump_save, dump_text);
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Game_Save_Dump returned");
        LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Game_Save_Dump returned");
#endif
    }

    /* Cleanup */
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Cleanup: HeMoM_Player_Shutdown()");
    LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Cleanup: HeMoM_Player_Shutdown()");
#endif
    HeMoM_Player_Shutdown();
    if(Platform_Record_Active()) { Platform_Record_Stop(); }
    if(Platform_Replay_Active()) { Platform_Replay_Stop(); }

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "[HeMoM] Cleanup: Shutdown_Platform()");
    LOG_TRACE(LOG_CAT_GENERAL, "[HeMoM] Cleanup: Shutdown_Platform()");
#endif
    Shutdown_Platform();

#ifdef STU_DEBUG
    AI_Metrics_Shutdown();
#endif

    LOG_INFO(LOG_CAT_GENERAL, "[POOL] peak bytes used = %u  (vs POOL_ARENA_CAPACITY)", Pool_Bytes_Peak());  /* CLAUDE: static-pool high-water mark; informs POOL_ARENA_CAPACITY sizing (PLAN-Static-Pool-Allocator Phase 2b) */

    LOG_TRACE(LOG_CAT_GENERAL, "TRACE: [%s, %d]: END: HeMoM  main()", __FILE__, __LINE__);

    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: END: HeMoM  main()", __FILE__, __LINE__);

    LOG_INFO(LOG_CAT_GENERAL, "END: HeMoM main() [STU_LOG tracer bullet]");

    STU_Log_Shutdown();

    LOG_INFO(LOG_CAT_HEMOM, "[HeMoM] Done");
    return 0;
}
