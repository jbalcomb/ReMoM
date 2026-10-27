/*
    MOD_Lua -- the Lua scripting runtime for mods.  See MOD_Lua.h.

    The only C++ translation unit in the game; sol2 does the Lua binding.  Everything it exposes is reachable from C
    through MOD_Lua.h and MOD_Hook.h.
*/

#include "MOD_Lua.h"
#include "MOD_Hook.h"

extern "C" {
#include "../../STU/src/STU_LOG.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/random.h"
}

#include <sol/sol.hpp>

#include <cmath>
#include <deque>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

static std::unique_ptr<sol::state> mod_lua_state;
static std::deque<sol::protected_function> mod_lua_handlers;  /* deque: element addresses stay valid as it grows */
static std::string mod_lua_last_error;

static void MOD_Lua_Set_Error(const std::string & message)
{
    mod_lua_last_error = message;
    LOG_ERROR(LOG_CAT_MOD, "[MOD] Lua: %s", message.c_str());
}

static bool MOD_Lua_Valid_City(int city_idx)
{
    return (_CITIES != NULL) && (city_idx >= 0) && (city_idx < _cities);
}

static int32_t MOD_Lua_Hook_Trampoline(int16_t hook_id, int16_t subject_idx, int32_t value, void * user_data)
{
    sol::protected_function * fn = static_cast<sol::protected_function *>(user_data);
    sol::protected_function_result result = (*fn)(subject_idx, value);
    double new_value;

    if(!result.valid())
    {
        sol::error err = result;
        MOD_Lua_Set_Error(std::string("hook \"") + MOD_Hook_Name(hook_id) + "\" subject " + std::to_string(subject_idx) + ": " + err.what());
        return value;
    }

    sol::object returned = result;
    if(returned.get_type() != sol::type::number)
    {
        MOD_Lua_Set_Error(std::string("hook \"") + MOD_Hook_Name(hook_id) + "\" subject " + std::to_string(subject_idx) + ": handler must return a number; keeping " + std::to_string(value));
        return value;
    }

    new_value = returned.as<double>();
    if(!std::isfinite(new_value) || (new_value > 2147483647.0) || (new_value < -2147483648.0))
    {
        MOD_Lua_Set_Error(std::string("hook \"") + MOD_Hook_Name(hook_id) + "\" subject " + std::to_string(subject_idx) + ": result out of range; keeping " + std::to_string(value));
        return value;
    }

    return (int32_t)new_value;
}

static void MOD_Lua_Bind_Game_API(sol::state & lua)
{
    sol::table game = lua.create_named_table("game");

    game.set_function("on", [](const std::string & hook_name, sol::protected_function fn)
    {
        int16_t hook_id = MOD_Hook_Find(hook_name.c_str());
        if(hook_id < 0)
        {
            throw std::runtime_error("game.on: unknown hook \"" + hook_name + "\"");
        }
        mod_lua_handlers.push_back(fn);
        if(MOD_Hook_Register(hook_id, MOD_Lua_Hook_Trampoline, &mod_lua_handlers.back()) != 0)
        {
            mod_lua_handlers.pop_back();
            throw std::runtime_error("game.on: could not register on hook \"" + hook_name + "\"");
        }
    });

    game.set_function("rnd", [](int x) -> int
    {
        if((x < 1) || (x > 32767))
        {
            throw std::runtime_error("game.rnd: argument must be 1..32767");
        }
        return Random((int16_t)x) - 1;
    });

    game.set_function("maxcity", []() -> int
    {
        return (_CITIES != NULL) ? (_cities - 1) : -1;
    });

    game.set_function("cityowner", [](int c) -> int
    {
        if(!MOD_Lua_Valid_City(c)) { throw std::runtime_error("game.cityowner: no city " + std::to_string(c)); }
        return _CITIES[c].owner_idx;
    });

    game.set_function("cityrace", [](int c) -> int
    {
        if(!MOD_Lua_Valid_City(c)) { throw std::runtime_error("game.cityrace: no city " + std::to_string(c)); }
        return _CITIES[c].race;
    });

    game.set_function("citysize", [](int c) -> int
    {
        if(!MOD_Lua_Valid_City(c)) { throw std::runtime_error("game.citysize: no city " + std::to_string(c)); }
        return _CITIES[c].size;
    });

    game.set_function("isbuilt", [](int c, int b) -> int
    {
        if(!MOD_Lua_Valid_City(c)) { throw std::runtime_error("game.isbuilt: no city " + std::to_string(c)); }
        if((b < 0) || (b >= NUM_BUILDINGS)) { throw std::runtime_error("game.isbuilt: no building " + std::to_string(b)); }
        return ((_CITIES[c].bldg_status[b] == bs_Built) || (_CITIES[c].bldg_status[b] == bs_Replaced)) ? 1 : 0;
    });

    game.set_function("log", [](const std::string & text)
    {
        LOG_INFO(LOG_CAT_MOD, "[MOD] script: %s", text.c_str());
    });
}

static void MOD_Lua_Sandbox(sol::state & lua)
{
    sol::table math = lua["math"];

    lua["dofile"] = sol::lua_nil;
    lua["loadfile"] = sol::lua_nil;
    lua["load"] = sol::lua_nil;
    lua["require"] = sol::lua_nil;

    lua["print"] = [](sol::variadic_args args, sol::this_state ts)
    {
        sol::state_view lua_view(ts);
        sol::protected_function tostring = lua_view["tostring"];
        std::string line;
        for(auto arg : args)
        {
            std::string part = tostring(arg.get<sol::object>());
            if(!line.empty()) { line += "\t"; }
            line += part;
        }
        LOG_INFO(LOG_CAT_MOD, "[MOD] script: %s", line.c_str());
    };

    /* The game's RNG, not Lua's: anything else breaks record/replay. */
    math.set_function("random", [](sol::variadic_args args) -> double
    {
        if(args.size() == 0)
        {
            return (double)(Random(32767) - 1) / 32767.0;
        }
        if(args.size() == 1)
        {
            int m = args[0].get<int>();
            if((m < 1) || (m > 32767)) { throw std::runtime_error("math.random: argument must be 1..32767"); }
            return (double)Random((int16_t)m);
        }
        int lo = args[0].get<int>();
        int hi = args[1].get<int>();
        if((hi < lo) || ((hi - lo) > 32766)) { throw std::runtime_error("math.random: interval must be non-empty and at most 32767 wide"); }
        return (double)(lo + Random((int16_t)(hi - lo + 1)) - 1);
    });
    math["randomseed"] = sol::lua_nil;
}

int MOD_Lua_Init(void)
{
    if(mod_lua_state)
    {
        return 0;
    }

    try
    {
        mod_lua_state = std::make_unique<sol::state>();
        /* Deliberately not io, os, package, debug. */
        mod_lua_state->open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::utf8);
        MOD_Lua_Sandbox(*mod_lua_state);
        MOD_Lua_Bind_Game_API(*mod_lua_state);
    }
    catch(const std::exception & ex)
    {
        mod_lua_state.reset();
        MOD_Lua_Set_Error(std::string("init failed: ") + ex.what());
        return -1;
    }

    mod_lua_last_error.clear();
    LOG_INFO(LOG_CAT_MOD, "[MOD] Lua runtime ready (%s)", LUA_RELEASE);
    return 0;
}

void MOD_Lua_Shutdown(void)
{
    MOD_Hook_Clear_All();
    mod_lua_handlers.clear();
    mod_lua_state.reset();
}

int MOD_Lua_Run_String(const char * chunk_name, const char * source)
{
    if(MOD_Lua_Init() != 0)
    {
        return -1;
    }

    sol::protected_function_result result = mod_lua_state->safe_script(source, sol::script_pass_on_error, chunk_name);
    if(!result.valid())
    {
        sol::error err = result;
        MOD_Lua_Set_Error(err.what());
        return -1;
    }

    return 0;
}

int MOD_Lua_Run_File(const char * path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    std::stringstream contents;

    if(!file)
    {
        MOD_Lua_Set_Error(std::string("cannot open script \"") + path + "\"");
        return -1;
    }

    contents << file.rdbuf();
    LOG_INFO(LOG_CAT_MOD, "[MOD] running script \"%s\"", path);
    return MOD_Lua_Run_String((std::string("@") + path).c_str(), contents.str().c_str());
}

const char * MOD_Lua_Last_Error(void)
{
    return mod_lua_last_error.c_str();
}
