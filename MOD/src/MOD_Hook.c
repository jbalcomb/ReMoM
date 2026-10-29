/*
    MOD_Hook -- the hook dispatcher for mods.  See MOD_Hook.h.
*/

#include "MOD_Hook.h"

#include "../../STU/src/STU_LOG.h"

#include <stddef.h>
#include <string.h>

struct s_MOD_HOOK_HANDLER
{
    MOD_Hook_Fn fn;
    void * user_data;
};

static struct s_MOD_HOOK_HANDLER mod_hook_handlers[MOD_HOOK_COUNT][MOD_HOOK_MAX_HANDLERS];
static int16_t mod_hook_handler_count[MOD_HOOK_COUNT];

/* The inputs of the hook call in progress (see MOD_Hook_Inputs()). */
static const struct s_MOD_HOOK_INPUT * mod_hook_inputs;
static int mod_hook_input_count;

static const char * mod_hook_names[MOD_HOOK_COUNT] =
{
    "city_food",
    "city_production",
    "city_gold",
    "city_research",
    "city_power"
};

int MOD_Hook_Register(int16_t hook_id, MOD_Hook_Fn fn, void * user_data)
{
    int16_t slot;

    if((hook_id < 0) || (hook_id >= MOD_HOOK_COUNT) || (fn == NULL))
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] MOD_Hook_Register: invalid hook_id=%d or NULL handler", (int)hook_id);
        return -1;
    }

    slot = mod_hook_handler_count[hook_id];
    if(slot >= MOD_HOOK_MAX_HANDLERS)
    {
        LOG_ERROR(LOG_CAT_MOD, "[MOD] MOD_Hook_Register: hook \"%s\" already has %d handlers (max)", mod_hook_names[hook_id], (int)MOD_HOOK_MAX_HANDLERS);
        return -1;
    }

    mod_hook_handlers[hook_id][slot].fn = fn;
    mod_hook_handlers[hook_id][slot].user_data = user_data;
    mod_hook_handler_count[hook_id] = slot + 1;

    LOG_INFO(LOG_CAT_MOD, "[MOD] hook \"%s\": handler %d registered", mod_hook_names[hook_id], (int)slot);

    return 0;
}

void MOD_Hook_Clear_All(void)
{
    memset(mod_hook_handlers, 0, sizeof(mod_hook_handlers));
    memset(mod_hook_handler_count, 0, sizeof(mod_hook_handler_count));
}

int16_t MOD_Hook_Count(int16_t hook_id)
{
    if((hook_id < 0) || (hook_id >= MOD_HOOK_COUNT))
    {
        return 0;
    }
    return mod_hook_handler_count[hook_id];
}

int32_t MOD_Hook_City_Value(int16_t hook_id, int16_t city_idx, int32_t value)
{
    return MOD_Hook_City_Value_Inputs(hook_id, city_idx, value, NULL, 0);
}

int32_t MOD_Hook_City_Value_Inputs(int16_t hook_id, int16_t city_idx, int32_t value, const struct s_MOD_HOOK_INPUT * inputs, int input_count)
{
    int16_t itr;

    if((hook_id < 0) || (hook_id >= MOD_HOOK_COUNT))
    {
        return value;
    }

    mod_hook_inputs = inputs;
    mod_hook_input_count = (inputs != NULL) ? input_count : 0;
    for(itr = 0; itr < mod_hook_handler_count[hook_id]; itr++)
    {
        value = mod_hook_handlers[hook_id][itr].fn(hook_id, city_idx, value, mod_hook_handlers[hook_id][itr].user_data);
    }
    mod_hook_inputs = NULL;
    mod_hook_input_count = 0;

    return value;
}

const struct s_MOD_HOOK_INPUT * MOD_Hook_Inputs(int * count)
{
    *count = mod_hook_input_count;
    return mod_hook_inputs;
}

const char * MOD_Hook_Name(int16_t hook_id)
{
    if((hook_id < 0) || (hook_id >= MOD_HOOK_COUNT))
    {
        return NULL;
    }
    return mod_hook_names[hook_id];
}

int16_t MOD_Hook_Find(const char * name)
{
    int16_t itr;

    if(name == NULL)
    {
        return -1;
    }

    for(itr = 0; itr < MOD_HOOK_COUNT; itr++)
    {
        if(strcmp(mod_hook_names[itr], name) == 0)
        {
            return itr;
        }
    }

    return -1;
}
