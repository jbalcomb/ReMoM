/*
    MOD_Load -- loads a mod folder.  See MOD_Load.h.
*/

#include "MOD_Load.h"
#include "MOD_Lua.h"

#include "../../STU/src/STU_LOG.h"

#include <stdio.h>
#include <string.h>

#define MOD_PATH_MAX 1024
#define MOD_INI_VALUE_MAX 64

/* Reads key=value from the [Mod] section of mod.ini.  value is left empty if the file or key is missing. */
static void MOD_Ini_Read(const char * ini_path, const char * key, char * value, size_t value_size)
{
    FILE * fp;
    char line[256];
    int in_mod_section = 0;
    size_t key_len = strlen(key);
    char * text;
    char * end;

    value[0] = '\0';

    fp = fopen(ini_path, "r");
    if(fp == NULL)
    {
        return;
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
            in_mod_section = (strcmp(text, "[Mod]") == 0);
            continue;
        }
        if(in_mod_section && (strncmp(text, key, key_len) == 0) && (text[key_len] == '='))
        {
            strncpy(value, text + key_len + 1, value_size - 1);
            value[value_size - 1] = '\0';
            break;
        }
    }

    fclose(fp);
}

int MOD_Load_Directory(const char * mod_dir)
{
    char path[MOD_PATH_MAX];
    char name[MOD_INI_VALUE_MAX];
    char version[MOD_INI_VALUE_MAX];

    if((mod_dir == NULL) || (mod_dir[0] == '\0'))
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] MOD_Load_Directory: no mod folder given");
        return -1;
    }

    snprintf(path, sizeof(path), "%s/mod.ini", mod_dir);
    MOD_Ini_Read(path, "name", name, sizeof(name));
    MOD_Ini_Read(path, "version", version, sizeof(version));
    LOG_INFO(LOG_CAT_MOD, "[MOD] loading mod \"%s\" version \"%s\" from \"%s\"", (name[0] != '\0') ? name : "(unnamed)", (version[0] != '\0') ? version : "?", mod_dir);

    if(MOD_Lua_Init() != 0)
    {
        return -1;
    }

    snprintf(path, sizeof(path), "%s/scripts/main.lua", mod_dir);
    if(MOD_Lua_Run_File(path) != 0)
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] mod in \"%s\" failed to load: %s", mod_dir, MOD_Lua_Last_Error());
        return -1;
    }

    return 0;
}
