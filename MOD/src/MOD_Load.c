/*
    MOD_Load -- loads a mod folder.  See MOD_Load.h.
*/

#include "MOD_Load.h"
#include "MOD_Hook.h"
#include "MOD_Lua.h"
#include "CAS.h"
#include "CAS_Game.h"

#include "../../MoX/src/SAVEFILE.h"
#include "../../STU/src/STU_LOG.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MOD_PATH_MAX 1024
#define MOD_INI_VALUE_MAX 256
#define MOD_CAS_MAX_SCRIPTS 64

/* CAS scripts bound to hooks (and the master script), freed by MOD_Unload(). */
static CAS_Script * mod_cas_scripts[MOD_CAS_MAX_SCRIPTS];
static int mod_cas_script_count;

/* Caster of Magic's MODDING.INI [Scripts] names for the hooks ReMoM has, accepted in [CAS] next to ReMoM's own names. */
static const struct
{
    const char * com2_name;
    const char * hook_name;
} mod_com2_hook_names[] =
{
    { "EconomyFood",     "city_food"       },
    { "EconomyProd",     "city_production" },
    { "EconomyGold",     "city_gold"       },
    { "EconomyResearch", "city_research"   },
    { "EconomyPower",    "city_power"      },
};

typedef void (*MOD_Ini_Fn)(const char * key, const char * value, void * user_data);

/* Calls fn(key, value) for every key=value line in [section] of the ini file.  Returns -1 if the file cannot be opened. */
static int MOD_Ini_Each(const char * ini_path, const char * section, MOD_Ini_Fn fn, void * user_data)
{
    FILE * fp;
    char line[512];
    int in_section = 0;
    char * text;
    char * end;
    char * equals;
    char * key_end;

    fp = fopen(ini_path, "r");
    if(fp == NULL)
    {
        return -1;
    }

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        text = line;
        while((*text == ' ') || (*text == '\t'))
        {
            text++;
        }
        end = text + strlen(text);
        while((end > text) && ((end[-1] == '\n') || (end[-1] == '\r') || (end[-1] == ' ') || (end[-1] == '\t')))
        {
            *--end = '\0';
        }

        if((text[0] == '\0') || (text[0] == ';') || (text[0] == '#'))
        {
            continue;
        }
        if(text[0] == '[')
        {
            in_section = (end > text + 1) && (end[-1] == ']') && ((size_t)(end - text - 2) == strlen(section)) && (strncmp(text + 1, section, strlen(section)) == 0);
            continue;
        }
        equals = strchr(text, '=');
        if(!in_section || (equals == NULL))
        {
            continue;
        }
        key_end = equals;
        while((key_end > text) && ((key_end[-1] == ' ') || (key_end[-1] == '\t')))
        {
            key_end--;
        }
        *key_end = '\0';
        equals++;
        while((*equals == ' ') || (*equals == '\t'))
        {
            equals++;
        }
        fn(text, equals, user_data);
    }

    fclose(fp);
    return 0;
}

struct s_MOD_INI_LOOKUP
{
    const char * key;
    char * value;
    size_t value_size;
};

static void MOD_Ini_Lookup(const char * key, const char * value, void * user_data)
{
    struct s_MOD_INI_LOOKUP * lookup = (struct s_MOD_INI_LOOKUP *)user_data;
    if((lookup->value[0] == '\0') && (strcmp(key, lookup->key) == 0))
    {
        strncpy(lookup->value, value, lookup->value_size - 1);
        lookup->value[lookup->value_size - 1] = '\0';
    }
}

/* Reads key=value from [section] of the ini file.  value is left empty if the file or key is missing. */
static void MOD_Ini_Read(const char * ini_path, const char * section, const char * key, char * value, size_t value_size)
{
    struct s_MOD_INI_LOOKUP lookup;
    value[0] = '\0';
    lookup.key = key;
    lookup.value = value;
    lookup.value_size = value_size;
    MOD_Ini_Each(ini_path, section, MOD_Ini_Lookup, &lookup);
}

static int MOD_Name_Equal(const char * a, const char * b)
{
    while((*a != '\0') && (*b != '\0'))
    {
        if(tolower((unsigned char)*a) != tolower((unsigned char)*b))
        {
            return 0;
        }
        a++;
        b++;
    }
    return (*a == *b);
}

/* Hook handler for a CAS script: inputs CITYID (the subject), VALUE (the value so far) and the hook's own inputs
   (MOD_Hook_Inputs()); RESULT starts as VALUE.
   A script that adds result lines (ADDRESLINE, CoM2's economy scripts) gives their total; otherwise RESULT.
   A script error keeps the value it was given. */
static int32_t MOD_CAS_Hook(int16_t hook_id, int16_t subject_idx, int32_t value, void * user_data)
{
    CAS_Script * script = (CAS_Script *)user_data;
    struct s_CAS_INPUT inputs[2 + MOD_HOOK_MAX_INPUTS];
    int input_count = 2;
    int hook_input_count = 0;
    const struct s_MOD_HOOK_INPUT * hook_inputs = MOD_Hook_Inputs(&hook_input_count);
    int32_t result = value;
    int32_t line_total = 0;
    int itr;

    inputs[0].name = "CITYID";
    inputs[0].value = subject_idx + 1;                                  /* CAS cities are 1-based (CAS_Game.h) */
    inputs[1].name = "VALUE";
    inputs[1].value = value;
    for(itr = 0; (itr < hook_input_count) && (input_count < 2 + MOD_HOOK_MAX_INPUTS); itr++)
    {
        inputs[input_count].name = hook_inputs[itr].name;
        inputs[input_count].value = hook_inputs[itr].value;
        input_count++;
    }

    if(CAS_Run(script, inputs, input_count, "RESULT", &result) != 0)
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] hook %s for city %d: script failed, keeping %d", MOD_Hook_Name(hook_id), (int)subject_idx, (int)value);
        return value;
    }
    if(CAS_Last_Result_Lines(&line_total) > 0)
    {
        return line_total;
    }
    return result;
}

static CAS_Script * MOD_CAS_Load(const char * mod_dir, const char * file)
{
    char path[MOD_PATH_MAX];
    CAS_Script * script;
    int warning_count = 0;
    const char * warnings;

    if(mod_cas_script_count >= MOD_CAS_MAX_SCRIPTS)
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] more than %d CAS scripts; \"%s\" not loaded", MOD_CAS_MAX_SCRIPTS, file);
        return NULL;
    }
    snprintf(path, sizeof(path), "%s/%s", mod_dir, file);
    script = CAS_Load_File(path);
    if((script == NULL) || (CAS_Check(script) != 0))
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] CAS script \"%s\" failed to load: %s", path, CAS_Last_Error());
        CAS_Free(script);
        return NULL;
    }
    warnings = CAS_Warnings(&warning_count);
    if(warning_count > 0)
    {
        LOG_WARN(LOG_CAT_MOD, "[MOD] CAS script \"%s\": %d warning(s):\n%s", path, warning_count, warnings);
    }
    mod_cas_scripts[mod_cas_script_count++] = script;
    return script;
}

struct s_MOD_CAS_BIND
{
    const char * mod_dir;
    int errors;
    int bound;
};

static void MOD_CAS_Bind(const char * key, const char * value, void * user_data)
{
    struct s_MOD_CAS_BIND * bind = (struct s_MOD_CAS_BIND *)user_data;
    const char * hook_name = key;
    int16_t hook_id;
    CAS_Script * script;
    size_t itr;

    if(MOD_Name_Equal(key, "master") || MOD_Name_Equal(key, "System"))
    {
        return;                                                         /* run first, by MOD_Load_Directory() */
    }
    for(itr = 0; itr < sizeof(mod_com2_hook_names) / sizeof(mod_com2_hook_names[0]); itr++)
    {
        if(MOD_Name_Equal(key, mod_com2_hook_names[itr].com2_name))
        {
            hook_name = mod_com2_hook_names[itr].hook_name;
        }
    }
    hook_id = MOD_Hook_Find(hook_name);
    if(hook_id < 0)
    {
        LOG_WARN(LOG_CAT_MOD, "[MOD] [CAS] %s=%s: ReMoM has no such hook yet; skipped", key, value);
        return;
    }
    script = MOD_CAS_Load(bind->mod_dir, value);
    if(script == NULL)
    {
        bind->errors++;
        return;
    }
    if(MOD_Hook_Register(hook_id, MOD_CAS_Hook, script) != 0)
    {
        bind->errors++;
        return;
    }
    bind->bound++;
    LOG_INFO(LOG_CAT_MOD, "[MOD] hook %s runs CAS script \"%s\"", hook_name, value);
}

/* Loads the [CAS] section: master=FILE runs first, then each hook=FILE is bound to its hook.  Returns the number of
   scripts loaded, or -1 on an error. */
static int MOD_CAS_Load_Section(const char * mod_dir, const char * ini_path)
{
    char master[MOD_INI_VALUE_MAX];
    struct s_MOD_CAS_BIND bind;
    CAS_Script * script;

    MOD_Ini_Read(ini_path, "CAS", "master", master, sizeof(master));
    if(master[0] == '\0')
    {
        MOD_Ini_Read(ini_path, "CAS", "System", master, sizeof(master));
    }
    if(master[0] != '\0')
    {
        script = MOD_CAS_Load(mod_dir, master);
        if((script == NULL) || (CAS_Run_Master(script) != 0))
        {
            LOG_ERROR(LOG_CAT_MOD, "[MOD] CAS master script \"%s\" failed: %s", master, CAS_Last_Error());
            return -1;
        }
    }

    bind.mod_dir = mod_dir;
    bind.errors = 0;
    bind.bound = 0;
    MOD_Ini_Each(ini_path, "CAS", MOD_CAS_Bind, &bind);
    return (bind.errors == 0) ? mod_cas_script_count : -1;
}

int MOD_Load_Directory(const char * mod_dir)
{
    char path[MOD_PATH_MAX];
    char name[MOD_INI_VALUE_MAX];
    char version[MOD_INI_VALUE_MAX];
    FILE * fp;
    int cas_scripts;

    if((mod_dir == NULL) || (mod_dir[0] == '\0'))
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] MOD_Load_Directory: no mod folder given");
        return -1;
    }

    snprintf(path, sizeof(path), "%s/mod.ini", mod_dir);
    MOD_Ini_Read(path, "Mod", "name", name, sizeof(name));
    MOD_Ini_Read(path, "Mod", "version", version, sizeof(version));
    LOG_INFO(LOG_CAT_MOD, "[MOD] loading mod \"%s\" version \"%s\" from \"%s\"", (name[0] != '\0') ? name : "(unnamed)", (version[0] != '\0') ? version : "?", mod_dir);

    if(MOD_Lua_Init() != 0)
    {
        return -1;
    }

    CAS_Game_Reset_Slots();                                             /* the scripts' city / wizard slots, saved with the game */
    cas_scripts = MOD_CAS_Load_Section(mod_dir, path);
    if(cas_scripts == 0)
    {
        Save_File_Unregister_Sections();                                /* no CAS scripts: nothing to save */
    }
    if(cas_scripts < 0)
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] mod in \"%s\" failed to load its CAS scripts", mod_dir);
        return -1;
    }

    snprintf(path, sizeof(path), "%s/scripts/main.lua", mod_dir);
    fp = fopen(path, "r");
    if(fp == NULL)
    {
        if(cas_scripts > 0)
        {
            return 0;                                                   /* a CAS-only mod */
        }
        LOG_ERROR(LOG_CAT_MOD, "[MOD] \"%s\" has neither scripts/main.lua nor [CAS] scripts in mod.ini", mod_dir);
        return -1;
    }
    fclose(fp);
    if(MOD_Lua_Run_File(path) != 0)
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] mod in \"%s\" failed to load: %s", mod_dir, MOD_Lua_Last_Error());
        return -1;
    }

    return 0;
}

void MOD_Unload(void)
{
    int itr;
    MOD_Lua_Shutdown();                                                 /* also clears every hook */
    for(itr = 0; itr < mod_cas_script_count; itr++)
    {
        CAS_Free(mod_cas_scripts[itr]);
        mod_cas_scripts[itr] = NULL;
    }
    mod_cas_script_count = 0;
    CAS_Clear_Master();
    Save_File_Unregister_Sections();
}
