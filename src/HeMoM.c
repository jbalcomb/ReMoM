/**
 * HeMoM.c — Headless Master of Magic
 *
 * A lean entry point for headless/automated operation:
 *   --newgame [ReMoM.ini]   Create a new game from config (skip menus)
 *   --load SAVE3.GAM        Load a save file (skip menus)
 *   --replay game.RMR       Replay recorded input
 *   --record game.RMR       Record input to file
 *
 * No logos, no credits, no main menu music. Initializes the engine,
 * creates or loads a game, and enters Screen_Control().
 *
 * Build with USE_HEADLESS=ON for zero external dependencies,
 * or with SDL for offscreen rendering (SDL_VIDEODRIVER=offscreen).
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
#include "../STU/src/STU_DBG.h"
#endif
#include "../platform/include/Platform.h"
#include "../platform/include/Platform_Replay.h"
#include "../platform/include/Platform_Keys.h"

#include "../MoX/src/MOM_DAT.h"
#include "../MoX/src/MOX_DAT.h"
#include "../MoX/src/MOX_DEF.h"
#include "../MoX/src/MOX_TYPE.h"
#include "../MoX/src/MOX_T4.h"
#include "../MoX/src/Fields.h"
#include "../MoX/src/LBX_Load.h"
#include "../MoX/src/LOADSAVE.h"
#include "../MoX/src/random.h"

#include "../MoM/src/NewGame.h"
#include "../MoM/src/MAPGEN.h"
#include "../MoM/src/INITGAME.h"
#include "../MoM/src/LoadScr.h"
#include "../MoM/src/MOM_SCR.h"

#include "ReMoM_Init.h"
#include "HeMoM_Player.h"
#include "HeMoM_SaveDump.h"

/* _wizard_presets_table is defined in NewGame.c but has no extern in a header */
extern struct s_WIZARD_PRESET _wizard_presets_table[];


#if defined(_WIN32) && !defined(_STU_WIN) && !defined(USE_HEADLESS)
#define SDL_MAIN_HANDLED
#ifdef USE_SDL3
#if __has_include(<SDL3/SDL_main.h>)
#include <SDL3/SDL_main.h>
#endif
#else
#if __has_include(<SDL.h>)
#include <SDL.h>
#endif
#endif
#endif



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
    while (*str && isspace((unsigned char)*str)) { str++; }
    if (*str == '\0') { return str; }
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) { *end-- = '\0'; }
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
        if (Str_Equal_CI(name, _wizard_presets_table[itr].name)) { return itr; }
    }
    return -1;
}

static int Lookup_Race_By_Name(const char *name)
{
    static const char *race_names[] = { "Barbarian", "Beastmen", "Dark Elf", "Draconian", "Dwarf", "Gnoll", "Halfling", "High Elf", "High Men", "Klackon", "Lizardman", "Nomad", "Orc", "Troll" };
    int itr;
    for (itr = 0; itr < 14; itr++)
    {
        if (Str_Equal_CI(name, race_names[itr])) { return itr; }
    }
    return -1;
}

static int Lookup_Banner_By_Name(const char *name)
{
    if (Str_Equal_CI(name, "Blue"))   { return BNR_Blue; }
    if (Str_Equal_CI(name, "Green"))  { return BNR_Green; }
    if (Str_Equal_CI(name, "Purple")) { return BNR_Purple; }
    if (Str_Equal_CI(name, "Red"))    { return BNR_Red; }
    if (Str_Equal_CI(name, "Yellow")) { return BNR_Yellow; }
    if (Str_Equal_CI(name, "Brown"))  { return BNR_Brown; }
    return -1;
}

static int Parse_Bool_TF(const char *value)
{
    if (Str_Equal_CI(value, "T") || Str_Equal_CI(value, "True") || Str_Equal_CI(value, "1")) { return 1; }
    if (Str_Equal_CI(value, "F") || Str_Equal_CI(value, "False") || Str_Equal_CI(value, "0")) { return 0; }
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
    if (fp == NULL)
    {
        fprintf(stderr, "[HeMoM] Could not open config: %s\n", filepath);
        return 0;
    }

    fprintf(stderr, "[HeMoM] Loading config: %s\n", filepath);

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line_num++;
        key = Trim_Whitespace(line);

        if (*key == '\0' || *key == '#' || *key == ';') { continue; }
        if (*key == '[') { continue; }

        eq = stu_strchr(key, '=');
        if (eq == NULL)
        {
            fprintf(stderr, "[HeMoM] %s:%d: missing '=' in: %s\n", filepath, line_num, key);
            continue;
        }

        *eq = '\0';
        value = Trim_Whitespace(eq + 1);
        key = Trim_Whitespace(key);

        if (Str_Equal_CI(key, "difficulty"))
        {
            val_int = stu_atoi(value);
            if (Str_Equal_CI(value, "Intro"))           { val_int = god_Intro; }
            else if (Str_Equal_CI(value, "Easy"))       { val_int = god_Easy; }
            else if (Str_Equal_CI(value, "Normal"))     { val_int = god_Normal; }
            else if (Str_Equal_CI(value, "Hard"))       { val_int = god_Hard; }
            else if (Str_Equal_CI(value, "Impossible")) { val_int = god_Impossible; }
            cfg->difficulty = (int16_t)val_int;
            cfg->has_difficulty = 1;
        }
        else if (Str_Equal_CI(key, "magic"))
        {
            val_int = stu_atoi(value);
            if (Str_Equal_CI(value, "Weak"))          { val_int = gom_Weak; }
            else if (Str_Equal_CI(value, "Normal"))   { val_int = gom_Normal; }
            else if (Str_Equal_CI(value, "Powerful")) { val_int = gom_Powerful; }
            cfg->magic = (int16_t)val_int;
            cfg->has_magic = 1;
        }
        else if (Str_Equal_CI(key, "landsize"))
        {
            val_int = stu_atoi(value);
            if (Str_Equal_CI(value, "Small"))       { val_int = gol_Small; }
            else if (Str_Equal_CI(value, "Medium"))  { val_int = gol_Medium; }
            else if (Str_Equal_CI(value, "Large"))   { val_int = gol_Large; }
            cfg->landsize = (int16_t)val_int;
            cfg->has_landsize = 1;
        }
        else if (Str_Equal_CI(key, "opponents"))
        {
            cfg->opponents = (int16_t)stu_atoi(value);
            cfg->has_opponents = 1;
        }
        else if (Str_Equal_CI(key, "seed"))
        {
            cfg->seed = (uint32_t)strtoul(value, NULL, 0);
            cfg->has_seed = 1;
        }
        else if (Str_Equal_CI(key, "wizard"))
        {
            val_int = Lookup_Wizard_By_Name(value);
            if (val_int >= 0)
            {
                cfg->wizard_id = (int16_t)val_int;
                stu_strcpy(cfg->wizard_name, _wizard_presets_table[val_int].name);
            }
            else
            {
                fprintf(stderr, "[HeMoM] %s:%d: unknown wizard '%s'\n", filepath, line_num, value);
            }
            cfg->has_wizard = 1;
        }
        else if (Str_Equal_CI(key, "name"))
        {
            stu_strncpy(cfg->wizard_name, value, 19);
            cfg->wizard_name[19] = '\0';
            cfg->has_name = 1;
        }
        else if (Str_Equal_CI(key, "race"))
        {
            val_int = Lookup_Race_By_Name(value);
            if (val_int >= 0)
            {
                cfg->race = (int16_t)val_int;
            }
            else
            {
                fprintf(stderr, "[HeMoM] %s:%d: unknown race '%s'\n", filepath, line_num, value);
            }
            cfg->has_race = 1;
        }
        else if (Str_Equal_CI(key, "banner"))
        {
            val_int = Lookup_Banner_By_Name(value);
            if (val_int >= 0)
            {
                cfg->banner = (int16_t)val_int;
            }
            else
            {
                fprintf(stderr, "[HeMoM] %s:%d: unknown banner '%s'\n", filepath, line_num, value);
            }
            cfg->has_banner = 1;
        }
        else if (Str_Equal_CI(key, "books_nature"))  { cfg->books_nature = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if (Str_Equal_CI(key, "books_sorcery")) { cfg->books_sorcery = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if (Str_Equal_CI(key, "books_chaos"))   { cfg->books_chaos = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if (Str_Equal_CI(key, "books_life"))    { cfg->books_life = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if (Str_Equal_CI(key, "books_death"))   { cfg->books_death = (int16_t)stu_atoi(value); cfg->has_books = 1; }
        else if (Str_Equal_CI(key, "Alchemy"))          { cfg->retorts[wsa_Alchemy] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Warlord"))          { cfg->retorts[wsa_Warlord] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Chaos Mastery"))    { cfg->retorts[wsa_Chaos_Mastery] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Nature Mastery"))   { cfg->retorts[wsa_Nature_Mastery] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Sorcery Mastery"))  { cfg->retorts[wsa_Sorcery_Mastery] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Infernal Power"))   { cfg->retorts[wsa_Infernal_Power] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Divine Power"))     { cfg->retorts[wsa_Divine_Power] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Sage Master"))      { cfg->retorts[wsa_Sage_Master] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Channeler"))        { cfg->retorts[wsa_Channeller] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Myrran"))           { cfg->retorts[wsa_Myrran] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Archmage"))         { cfg->retorts[wsa_Archmage] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Mana Focusing"))    { cfg->retorts[wsa_Mana_Focusing] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Node Mastery"))     { cfg->retorts[wsa_Node_Mastery] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Famous"))           { cfg->retorts[wsa_Famous] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Runemaster"))       { cfg->retorts[wsa_Runemaster] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Conjurer"))         { cfg->retorts[wsa_Conjurer] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Charismatic"))      { cfg->retorts[wsa_Charismatic] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else if (Str_Equal_CI(key, "Artificer"))        { cfg->retorts[wsa_Artificer] = (int8_t)Parse_Bool_TF(value); cfg->has_retorts = 1; }
        else
        {
            fprintf(stderr, "[HeMoM] %s:%d: unknown key '%s'\n", filepath, line_num, key);
        }
    }

    stu_fclose(fp);
    return 1;
}



/* ========================================================================= */
/*  New Game Creation                                                        */
/* ========================================================================= */

static void Config_Apply_And_Create_New_Game(struct s_HeMoM_Config *cfg)
{
    int8_t *wsa_ptr;
    int itr;

    _difficulty = cfg->difficulty;
    _magic = cfg->magic;
    _landsize = cfg->landsize;
    _num_players = cfg->opponents + 1;

    Human_Player_Wizard_Profile(cfg->wizard_id);

    if (cfg->has_name)
    {
        memset(_players[0].name, 0, LEN_WIZARD_NAME);
        stu_strncpy(_players[0].name, cfg->wizard_name, LEN_WIZARD_NAME);
    }

    if (cfg->has_books)
    {
        if (cfg->books_nature >= 0)  { _players[0].spellranks[sbr_Nature]  = cfg->books_nature; }
        if (cfg->books_sorcery >= 0) { _players[0].spellranks[sbr_Sorcery] = cfg->books_sorcery; }
        if (cfg->books_chaos >= 0)   { _players[0].spellranks[sbr_Chaos]   = cfg->books_chaos; }
        if (cfg->books_life >= 0)    { _players[0].spellranks[sbr_Life]    = cfg->books_life; }
        if (cfg->books_death >= 0)   { _players[0].spellranks[sbr_Death]   = cfg->books_death; }
    }

    if (cfg->has_retorts)
    {
        wsa_ptr = &_players[0].alchemy;
        for (itr = 0; itr < NUM_RETORTS; itr++)
        {
            if (cfg->retorts[itr] >= 0)
            {
                wsa_ptr[itr] = cfg->retorts[itr];
            }
        }
    }

    NEWG_Clicked_Race = cfg->race;
    _players[0].banner_id = (uint8_t)cfg->banner;

    /* Set deterministic RNG seed if specified in config. */
    if (cfg->has_seed)
    {
        Set_Random_Seed(cfg->seed);
        fprintf(stderr, "[HeMoM] RNG seed set to %u (0x%08X)\n", cfg->seed, cfg->seed);
#ifdef STU_DEBUG
        dbg_prn("[HeMoM] RNG seed set to %u (0x%08X)\n", cfg->seed, cfg->seed);
        trc_prn("[HeMoM] RNG seed set to %u (0x%08X)\n", cfg->seed, cfg->seed);
#endif
    }

    fprintf(stderr, "[HeMoM] Creating new game: difficulty=%d magic=%d landsize=%d opponents=%d wizard=%s race=%d banner=%d seed=%u\n", cfg->difficulty, cfg->magic, cfg->landsize, cfg->opponents, cfg->wizard_name, cfg->race, cfg->banner, Get_Random_Seed());
#ifdef STU_DEBUG
    dbg_prn("[HeMoM] Creating new game: difficulty=%d magic=%d landsize=%d opponents=%d wizard=%s race=%d banner=%d seed=%u\n", cfg->difficulty, cfg->magic, cfg->landsize, cfg->opponents, cfg->wizard_name, cfg->race, cfg->banner, Get_Random_Seed());
    trc_prn("[HeMoM] Creating new game: difficulty=%d magic=%d landsize=%d opponents=%d wizard=%s race=%d banner=%d seed=%u\n", cfg->difficulty, cfg->magic, cfg->landsize, cfg->opponents, cfg->wizard_name, cfg->race, cfg->banner, Get_Random_Seed());
#endif

    // New_Game_Screen_7__WIP()
    // NEWGAME.LBX, 053  BUILDWOR   map build bar
    // newgame_BUILDWOR_map_build_bar_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 53, _screen_seg);
    newgame_BUILDWOR_map_build_bar_seg = LBX_Load(newgame_lbx_file__ovr050, 53);

    Init_New_Game();
    Initialize_Events();
    NEWG_FinalizeTables__WIP();
    Save_SAVE_GAM(8);

    /* Dump structured text representation of the save file for testing */
    HeMoM_Save_Dump("SAVE9.GAM", "SAVE9.txt");

    // MainScr.c  Main_Screen()  1910:
    //     if((_turn == 0) && (_given_chance_to_rename_home_city == ST_FALSE))
    //         Change_Home_City_Name_Popup(HUMAN_PLAYER_IDX);
    //         Assign_Auto_Function(Main_Screen_Draw, 1);
    //         _given_chance_to_rename_home_city = ST_TRUE;
    _given_chance_to_rename_home_city = ST_TRUE;
    
    fprintf(stderr, "[HeMoM] New game created successfully\n");
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
        if (mouse_x >= p_fields[fi].x1 && mouse_x <= p_fields[fi].x2 && mouse_y >= p_fields[fi].y1 && mouse_y <= p_fields[fi].y2)
        {
            fprintf(fp, "  field[%d]=(%d,%d)-(%d,%d)", fi, p_fields[fi].x1, p_fields[fi].y1, p_fields[fi].x2, p_fields[fi].y2);
            break;
        }
    }
}



/* ========================================================================= */
/*  Usage                                                                    */
/* ========================================================================= */

static void Print_Usage(const char *program_name)
{
    fprintf(stderr, "HeMoM — Headless Master of Magic\n\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s --newgame [ReMoM.ini] [--scenario test.hms] [--record out.RMR]\n", program_name);
    fprintf(stderr, "  %s --continue [--scenario test.hms] [--record out.RMR]\n", program_name);
    fprintf(stderr, "  %s --load SAVE3.GAM [--scenario test.hms] [--record out.RMR]\n", program_name);
    fprintf(stderr, "  %s --newgame [ReMoM.ini] [--replay game.RMR]\n", program_name);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "  --newgame [FILE]   Create new game from config (default: ReMoM.ini)\n");
    fprintf(stderr, "  --continue         Load SAVE9.GAM (the WIZARDS.EXE / Continue path)\n");
    fprintf(stderr, "  --load FILE        Load a save file (SAVE1.GAM .. SAVE9.GAM, SAVETEST.GAM)\n");
    fprintf(stderr, "  --scenario FILE    Run synthetic player from scenario script (.hms)\n");
    fprintf(stderr, "  --replay FILE      Replay recorded input from .RMR file\n");
    fprintf(stderr, "  --record FILE      Record input to .RMR file\n");
    fprintf(stderr, "  --dump-save FILE   After Screen_Control returns, dump FILE.GAM to FILE.txt\n");
    fprintf(stderr, "  --help             Show this help\n");
}



/* ========================================================================= */
/*  Main                                                                     */
/* ========================================================================= */

int main(int argc, char *argv[])
{
    int hemom_mode = 0;  /* 0=none, 1=newgame, 2=load */
    char hemom_file[260] = { 0 };
    char hemom_scenario[260] = { 0 };
    char hemom_dump_save[260] = { 0 };
    int argi;

#ifdef STU_DEBUG
    Debug_Log_Startup();
    dbg_prn("DEBUG: [%s, %d]: BEGIN: HeMoM  main()\n", __FILE__, __LINE__);
#endif
#ifdef STU_DEBUG
    Trace_Log_Startup();
    trc_prn("TRACE: [%s, %d]: BEGIN: HeMoM  main()\n", __FILE__, __LINE__);
#endif

    /* Log and parse CLI arguments */
    fprintf(stderr, "[HeMoM] argc=%d\n", argc);
#ifdef STU_DEBUG
    dbg_prn("[HeMoM] argc=%d\n", argc);
    trc_prn("[HeMoM] argc=%d\n", argc);
#endif
    for (argi = 0; argi < argc; argi++)
    {
        fprintf(stderr, "[HeMoM] argv[%d]=\"%s\"\n", argi, argv[argi]);
#ifdef STU_DEBUG
        dbg_prn("[HeMoM] argv[%d]=\"%s\"\n", argi, argv[argi]);
        trc_prn("[HeMoM] argv[%d]=\"%s\"\n", argi, argv[argi]);
#endif
    }

    for (argi = 1; argi < argc; argi++)
    {
        if (strcmp(argv[argi], "--help") == 0 || strcmp(argv[argi], "-h") == 0)
        {
            Print_Usage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[argi], "--newgame") == 0)
        {
            hemom_mode = 1;
            if ((argi + 1) < argc && argv[argi + 1][0] != '-')
            {
                argi++;
                stu_strcpy(hemom_file, argv[argi]);
            }
            else
            {
                stu_strcpy(hemom_file, "ReMoM.ini");
            }
            fprintf(stderr, "[HeMoM] CLI: --newgame \"%s\"\n", hemom_file);
#ifdef STU_DEBUG
            dbg_prn("[HeMoM] CLI: --newgame \"%s\"\n", hemom_file);
            trc_prn("[HeMoM] CLI: --newgame \"%s\"\n", hemom_file);
#endif
        }
        else if (strcmp(argv[argi], "--load") == 0 && (argi + 1) < argc)
        {
            hemom_mode = 2;
            argi++;
            stu_strcpy(hemom_file, argv[argi]);
            fprintf(stderr, "[HeMoM] CLI: --load \"%s\"\n", hemom_file);
#ifdef STU_DEBUG
            dbg_prn("[HeMoM] CLI: --load \"%s\"\n", hemom_file);
            trc_prn("[HeMoM] CLI: --load \"%s\"\n", hemom_file);
#endif
        }
        else if (strcmp(argv[argi], "--continue") == 0)
        {
            /* --continue is sugar for --load SAVE9.GAM (the WIZARDS.EXE path). */
            hemom_mode = 2;
            stu_strcpy(hemom_file, "SAVE9.GAM");
            fprintf(stderr, "[HeMoM] CLI: --continue (SAVE9.GAM)\n");
#ifdef STU_DEBUG
            dbg_prn("[HeMoM] CLI: --continue (SAVE9.GAM)\n");
            trc_prn("[HeMoM] CLI: --continue (SAVE9.GAM)\n");
#endif
        }
        else if (strcmp(argv[argi], "--replay") == 0 || strcmp(argv[argi], "--record") == 0)
        {
            fprintf(stderr, "[HeMoM] CLI: %s (deferred until after platform init)\n", argv[argi]);
#ifdef STU_DEBUG
            dbg_prn("[HeMoM] CLI: %s (deferred until after platform init)\n", argv[argi]);
            trc_prn("[HeMoM] CLI: %s (deferred until after platform init)\n", argv[argi]);
#endif
            /* Handled after Startup_Platform() */
        }
        else if (strcmp(argv[argi], "--scenario") == 0 && (argi + 1) < argc)
        {
            argi++;
            stu_strcpy(hemom_scenario, argv[argi]);
            fprintf(stderr, "[HeMoM] CLI: --scenario \"%s\"\n", hemom_scenario);
#ifdef STU_DEBUG
            dbg_prn("[HeMoM] CLI: --scenario \"%s\"\n", hemom_scenario);
            trc_prn("[HeMoM] CLI: --scenario \"%s\"\n", hemom_scenario);
#endif
        }
        else if (strcmp(argv[argi], "--dump-save") == 0 && (argi + 1) < argc)
        {
            argi++;
            stu_strcpy(hemom_dump_save, argv[argi]);
            fprintf(stderr, "[HeMoM] CLI: --dump-save \"%s\"\n", hemom_dump_save);
#ifdef STU_DEBUG
            dbg_prn("[HeMoM] CLI: --dump-save \"%s\"\n", hemom_dump_save);
            trc_prn("[HeMoM] CLI: --dump-save \"%s\"\n", hemom_dump_save);
#endif
        }
        else
        {
            fprintf(stderr, "[HeMoM] CLI: unknown arg \"%s\"\n", argv[argi]);
#ifdef STU_DEBUG
            dbg_prn("[HeMoM] CLI: unknown arg \"%s\"\n", argv[argi]);
            trc_prn("[HeMoM] CLI: unknown arg \"%s\"\n", argv[argi]);
#endif
        }
    }

    fprintf(stderr, "[HeMoM] Parsed: mode=%d file=\"%s\" scenario=\"%s\"\n", hemom_mode, hemom_file, hemom_scenario);
#ifdef STU_DEBUG
    dbg_prn("[HeMoM] Parsed: mode=%d file=\"%s\" scenario=\"%s\"\n", hemom_mode, hemom_file, hemom_scenario);
    trc_prn("[HeMoM] Parsed: mode=%d file=\"%s\" scenario=\"%s\"\n", hemom_mode, hemom_file, hemom_scenario);
#endif

    if (hemom_mode == 0)
    {
        Print_Usage(argv[0]);
        fprintf(stderr, "\nError: must specify --newgame or --load\n");
        return 1;
    }

    /* Set SDL video driver to offscreen for non-headless SDL builds */
#if !defined(USE_HEADLESS) && !defined(_STU_WIN)
#ifdef USE_SDL3
    stu_putenv("SDL_VIDEODRIVER=offscreen");
#else
    stu_putenv("SDL_VIDEODRIVER=dummy");
#endif
    fprintf(stderr, "[HeMoM] SDL video driver set to offscreen\n");
#endif

    Startup_Platform();

    /* Register replay callbacks */
    Platform_Replay_Register_Random_Seed_Callbacks(Get_Random_Seed, Set_Random_Seed);
    Platform_Replay_Register_Field_Log_Callback(HeMoM_Replay_Log_Field_Hit);

    /* Process --replay and --record flags (after platform init) */
    for (argi = 1; argi < argc; argi++)
    {
        if (strcmp(argv[argi], "--record") == 0 && (argi + 1) < argc)
        {
            argi++;
            Platform_Record_Start(argv[argi]);
        }
        else if (strcmp(argv[argi], "--replay") == 0 && (argi + 1) < argc)
        {
            argi++;
            Platform_Replay_Start(argv[argi]);
        }
    }

    /* Initialize engine (shared with ReMoMber) */
    fprintf(stderr, "[HeMoM] Initializing engine...\n");
    ReMoM_Init_Engine();

    /* Create or load game */
    if (hemom_mode == 1)
    {
        struct s_HeMoM_Config cfg;
        Config_Init(&cfg);
        if (!Config_Parse(hemom_file, &cfg))
        {
            fprintf(stderr, "[HeMoM] Failed to parse config: %s\n", hemom_file);
            Shutdown_Platform();
            return 1;
        }
        Config_Apply_And_Create_New_Game(&cfg);
        current_screen = scr_Main_Screen;
    }
    else if (hemom_mode == 2)
    {
        int16_t save_slot;

        if (Str_Equal_CI(hemom_file, "SAVETEST.GAM"))
        {
            save_slot = ST_UNDEFINED;
        }
        else
        {
            const char *digit_ptr = hemom_file + 4;
            save_slot = (int16_t)(stu_atoi(digit_ptr) - 1);
        }

        fprintf(stderr, "[HeMoM] Loading save: %s (slot %d)\n", hemom_file, save_slot);
        Load_SAVE_GAM(save_slot);
        Loaded_Game_Update();
        current_screen = scr_Main_Screen;
    }

    /* Load and register synthetic player scenario */
    if (hemom_scenario[0] != '\0')
    {
        fprintf(stderr, "[HeMoM] Loading scenario: %s\n", hemom_scenario);
#ifdef STU_DEBUG
        dbg_prn("[HeMoM] Loading scenario: %s\n", hemom_scenario);
        trc_prn("[HeMoM] Loading scenario: %s\n", hemom_scenario);
#endif
        if (HeMoM_Player_Load_Scenario(hemom_scenario) != 0)
        {
            fprintf(stderr, "[HeMoM] Failed to load scenario: %s\n", hemom_scenario);
#ifdef STU_DEBUG
            dbg_prn("[HeMoM] Failed to load scenario: %s\n", hemom_scenario);
            trc_prn("[HeMoM] Failed to load scenario: %s\n", hemom_scenario);
#endif
            Shutdown_Platform();
            return 1;
        }
        Platform_Register_Frame_Callback(HeMoM_Player_Frame);
        fprintf(stderr, "[HeMoM] Registered synthetic player callback\n");
#ifdef STU_DEBUG
        dbg_prn("[HeMoM] Registered synthetic player callback\n");
        trc_prn("[HeMoM] Registered synthetic player callback\n");
#endif
    }
    else
    {
        fprintf(stderr, "[HeMoM] No --scenario specified\n");
#ifdef STU_DEBUG
        dbg_prn("[HeMoM] No --scenario specified\n");
        trc_prn("[HeMoM] No --scenario specified\n");
#endif
    }

    /* Enter game loop */
    fprintf(stderr, "[HeMoM] Entering Screen_Control()\n");
#ifdef STU_DEBUG
    dbg_prn("[HeMoM] Entering Screen_Control()\n");
    trc_prn("[HeMoM] Entering Screen_Control()\n");
#endif
    Screen_Control();

    fprintf(stderr, "[HeMoM] Screen_Control() returned\n");
#ifdef STU_DEBUG
    dbg_prn("[HeMoM] Screen_Control() returned\n");
    trc_prn("[HeMoM] Screen_Control() returned\n");
#endif

    /* Dump a save file to text after the game loop exits.  Used by tests
       that need to inspect a save produced by the scenario. */
    if (hemom_dump_save[0] != '\0')
    {
        char dump_text[260];
        size_t len = strlen(hemom_dump_save);
        size_t i;
        stu_strcpy(dump_text, hemom_dump_save);
        /* Replace ".GAM" suffix with ".txt" if present, else append ".txt". */
        if (len >= 4 && stu_stricmp(dump_text + len - 4, ".GAM") == 0)
        {
            dump_text[len - 4] = '\0';
        }
        i = strlen(dump_text);
        if ((i + 4) < sizeof(dump_text))
        {
            dump_text[i + 0] = '.';
            dump_text[i + 1] = 't';
            dump_text[i + 2] = 'x';
            dump_text[i + 3] = 't';
            dump_text[i + 4] = '\0';
        }
        fprintf(stderr, "[HeMoM] Dumping %s -> %s\n", hemom_dump_save, dump_text);
#ifdef STU_DEBUG
        dbg_prn("[HeMoM] Dumping %s -> %s\n", hemom_dump_save, dump_text);
        trc_prn("[HeMoM] Dumping %s -> %s\n", hemom_dump_save, dump_text);
#endif
        HeMoM_Save_Dump(hemom_dump_save, dump_text);
#ifdef STU_DEBUG
        dbg_prn("[HeMoM] HeMoM_Save_Dump returned\n");
        trc_prn("[HeMoM] HeMoM_Save_Dump returned\n");
#endif
    }

    /* Cleanup */
#ifdef STU_DEBUG
    dbg_prn("[HeMoM] Cleanup: HeMoM_Player_Shutdown()\n");
    trc_prn("[HeMoM] Cleanup: HeMoM_Player_Shutdown()\n");
#endif
    HeMoM_Player_Shutdown();
    if (Platform_Record_Active()) { Platform_Record_Stop(); }
    if (Platform_Replay_Active()) { Platform_Replay_Stop(); }

#ifdef STU_DEBUG
    dbg_prn("[HeMoM] Cleanup: Shutdown_Platform()\n");
    trc_prn("[HeMoM] Cleanup: Shutdown_Platform()\n");
#endif
    Shutdown_Platform();

#ifdef STU_DEBUG
    trc_prn("TRACE: [%s, %d]: END: HeMoM  main()\n", __FILE__, __LINE__);
    Trace_Log_Shutdown();
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: HeMoM  main()\n", __FILE__, __LINE__);
    Debug_Log_Shutdown();
#endif

    fprintf(stderr, "[HeMoM] Done\n");
    return 0;
}
