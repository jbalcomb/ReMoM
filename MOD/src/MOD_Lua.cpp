/*
    MOD_Lua -- the Lua scripting runtime for mods.  See MOD_Lua.h.

    The only C++ translation unit in the game; sol2 does the Lua binding.  Everything it exposes is reachable from C
    through MOD_Lua.h and MOD_Hook.h.
*/

#include "MOD_Lua.h"
#include "MOD_Hook.h"
#include "MOD_Rules.h"

extern "C" {
#include "../../STU/src/STU_LOG.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/random.h"
#include "../../MoM/src/TerrType.h"
#include "../../STU/src/STU_GRAF.h"  /* STU_GRAF_Executable_Dir() */
}

#include <sol/sol.hpp>

#include <cmath>
#include <deque>
#include <fstream>
#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include <string>

static std::unique_ptr<sol::state> mod_lua_state;
static std::deque<sol::protected_function> mod_lua_handlers;  /* deque: element addresses stay valid as it grows */
static std::string mod_lua_last_error;
static std::string mod_rules_profile;   /* "" when no rules profile is loaded */
static std::string mod_rules_dir;

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
    mod_rules_profile.clear();                                         /* the rules live in the state being closed */
    mod_rules_dir.clear();
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


/* ---------------------------------------------------------------------------------------------------------------- */
/*  Rules profiles (MOD_Rules.h)                                                                                    */
/* ---------------------------------------------------------------------------------------------------------------- */

static bool MOD_Rules_File_Exists(const std::string & path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    return static_cast<bool>(file);
}

/* Folder of a profile: <dir>/rules/<name> for the current directory, then the executable's directory. */
static std::string MOD_Rules_Find_Dir(const std::string & profile)
{
    char exe_dir[1024];
    std::string candidate;

    if((profile.find('/') != std::string::npos) || (profile.find('\\') != std::string::npos))
    {
        return MOD_Rules_File_Exists(profile + "/profile.lua") ? profile : std::string();
    }
    candidate = "rules/" + profile;
    if(MOD_Rules_File_Exists(candidate + "/profile.lua"))
    {
        return candidate;
    }
    if(STU_GRAF_Executable_Dir(exe_dir, sizeof(exe_dir)))
    {
        candidate = std::string(exe_dir) + "/rules/" + profile;
        if(MOD_Rules_File_Exists(candidate + "/profile.lua"))
        {
            return candidate;
        }
    }
    return std::string();
}

static bool MOD_Rules_Valid_Name(const std::string & name, bool is_file)
{
    if(name.empty())
    {
        return false;
    }
    for(char c : name)
    {
        bool ok = ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0') && (c <= '9')) || (c == '_') || (is_file && (c == '.'));
        if(!ok)
        {
            return false;
        }
    }
    return name.find("..") == std::string::npos;
}

static void MOD_Rules_Run_File(sol::state & lua, const std::string & path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    std::stringstream contents;
    if(!file)
    {
        throw std::runtime_error("rules: cannot open \"" + path + "\"");
    }
    contents << file.rdbuf();
    sol::protected_function_result result = lua.safe_script(contents.str(), sol::script_pass_on_error, "@" + path);
    if(!result.valid())
    {
        sol::error err = result;
        throw std::runtime_error(err.what());
    }
}

static void MOD_Rules_Bind(sol::state & lua, const std::string & profile)
{
    sol::table rules = lua.create_named_table("rules");
    sol::table terrain = lua.create_named_table("terrain");

    rules["profile"] = profile;
    rules["about_entries"] = lua.create_table();
    /* rules.about(rule, { status = ..., source = ..., note = ... }): what the profile says about one of its rules, for
       the rules catalog (tools/rules_catalog). */
    rules.set_function("about", [](const std::string & rule, sol::table info, sol::this_state ts)
    {
        sol::state_view view(ts);
        sol::table entries = view["rules"]["about_entries"];
        entries[rule] = info;
    });
    rules.set_function("load", [](sol::variadic_args args, sol::this_state ts)
    {
        sol::state_view view(ts);
        std::string dir = mod_rules_dir;
        std::string file;
        if(args.size() == 1)
        {
            file = args[0].get<std::string>();
        }
        else if(args.size() == 2)
        {
            std::string other = args[0].get<std::string>();
            if(!MOD_Rules_Valid_Name(other, false)) { throw std::runtime_error("rules.load: bad profile name \"" + other + "\""); }
            dir = MOD_Rules_Find_Dir(other);
            if(dir.empty()) { throw std::runtime_error("rules.load: no rules profile \"" + other + "\""); }
            file = args[1].get<std::string>();
        }
        else
        {
            throw std::runtime_error("rules.load(file) or rules.load(profile, file)");
        }
        if(!MOD_Rules_Valid_Name(file, true)) { throw std::runtime_error("rules.load: bad file name \"" + file + "\""); }
        MOD_Rules_Run_File(*mod_lua_state, dir + "/" + file);
    });

    /* 1.31's terrain types (MoM/src/TerrType.h, enum e_TERRAIN_TYPES), without the "tt_" prefix. */
    terrain["Ocean"] = (int)tt_Ocean;
    terrain["BugGrass"] = (int)tt_BugGrass;
    terrain["Shore1_Fst"] = (int)tt_Shore1_Fst;
    terrain["Lake"] = (int)tt_Lake;
    terrain["Shore1_Lst"] = (int)tt_Shore1_Lst;
    terrain["Grasslands1"] = (int)tt_Grasslands1;
    terrain["Forest1"] = (int)tt_Forest1;
    terrain["Mountain1"] = (int)tt_Mountain1;
    terrain["Desert1"] = (int)tt_Desert1;
    terrain["Swamp1"] = (int)tt_Swamp1;
    terrain["Tundra1"] = (int)tt_Tundra1;
    terrain["SorceryNode"] = (int)tt_SorceryNode;
    terrain["NatureNode"] = (int)tt_NatureNode;
    terrain["ChaosNode"] = (int)tt_ChaosNode;
    terrain["Hills1"] = (int)tt_Hills1;
    terrain["Grasslands2"] = (int)tt_Grasslands2;
    terrain["Grasslands3"] = (int)tt_Grasslands3;
    terrain["Desert2"] = (int)tt_Desert2;
    terrain["Desert3"] = (int)tt_Desert3;
    terrain["Desert4"] = (int)tt_Desert4;
    terrain["Swamp2"] = (int)tt_Swamp2;
    terrain["Swamp3"] = (int)tt_Swamp3;
    terrain["Volcano"] = (int)tt_Volcano;
    terrain["Grasslands4"] = (int)tt_Grasslands4;
    terrain["Tundra2"] = (int)tt_Tundra2;
    terrain["Tundra3"] = (int)tt_Tundra3;
    terrain["Forest2"] = (int)tt_Forest2;
    terrain["Forest3"] = (int)tt_Forest3;
    terrain["RiverM_1st"] = (int)tt_RiverM_1st;
    terrain["RiverM_end"] = (int)tt_RiverM_end;
    terrain["Lake1"] = (int)tt_Lake1;
    terrain["Lake2"] = (int)tt_Lake2;
    terrain["Lake3"] = (int)tt_Lake3;
    terrain["Lake4"] = (int)tt_Lake4;
    terrain["Shore2F_1st"] = (int)tt_Shore2F_1st;
    terrain["Shore2F_end"] = (int)tt_Shore2F_end;
    terrain["Rivers_1st"] = (int)tt_Rivers_1st;
    terrain["Rivers_end"] = (int)tt_Rivers_end;
    terrain["Mountains_Fst"] = (int)tt_Mountains_Fst;
    terrain["Mountains_Lst"] = (int)tt_Mountains_Lst;
    terrain["Hills_Fst"] = (int)tt_Hills_Fst;
    terrain["Hills_Lst"] = (int)tt_Hills_Lst;
    terrain["Desert_Fst"] = (int)tt_Desert_Fst;
    terrain["Desert_Lst"] = (int)tt_Desert_Lst;
    terrain["Shore2_1st"] = (int)tt_Shore2_1st;
    terrain["Shore2_end"] = (int)tt_Shore2_end;
    terrain["4WRiver1"] = (int)tt_4WRiver1;
    terrain["4WRiver2"] = (int)tt_4WRiver2;
    terrain["4WRiver3"] = (int)tt_4WRiver3;
    terrain["4WRiver4"] = (int)tt_4WRiver4;
    terrain["4WRiver5"] = (int)tt_4WRiver5;
    terrain["Shore3_1st"] = (int)tt_Shore3_1st;
    terrain["Shore3_end"] = (int)tt_Shore3_end;
    terrain["OceanAnim"] = (int)tt_OceanAnim;
    terrain["Tundra_1st"] = (int)tt_Tundra_1st;
    terrain["Tundra_Last"] = (int)tt_Tundra_Last;
}

int MOD_Rules_Load(const char * profile)
{
    std::string name = (profile != NULL) ? profile : "";
    std::string dir;

    if(MOD_Lua_Init() != 0)
    {
        return -1;
    }
    mod_rules_profile.clear();
    (*mod_lua_state)["rules"] = sol::lua_nil;

    dir = MOD_Rules_Find_Dir(name);
    if(dir.empty())
    {
        MOD_Lua_Set_Error("rules profile \"" + name + "\" not found (looked for rules/" + name + "/profile.lua here and beside the executable)");
        return -1;
    }

    try
    {
        mod_rules_dir = dir;
        MOD_Rules_Bind(*mod_lua_state, name);
        MOD_Rules_Run_File(*mod_lua_state, dir + "/profile.lua");
    }
    catch(const std::exception & ex)
    {
        (*mod_lua_state)["rules"] = sol::lua_nil;
        mod_rules_dir.clear();
        MOD_Lua_Set_Error(std::string("rules profile \"") + name + "\": " + ex.what());
        return -1;
    }

    mod_rules_profile = name;
    LOG_INFO(LOG_CAT_MOD, "[MOD] rules profile \"%s\" loaded from \"%s\"", name.c_str(), dir.c_str());
    return 0;
}

const char * MOD_Rules_Profile(void)
{
    return mod_rules_profile.c_str();
}

int MOD_Rules_Call_Int(const char * name, const int32_t * args, int arg_count, int32_t * result)
{
    static std::map<std::string, int> error_counts;
    const int max_logged_errors = 10;

    if(mod_rules_profile.empty() || !mod_lua_state)
    {
        return 1;
    }

    sol::table rules = (*mod_lua_state)["rules"];
    sol::object fn_object = rules[name];
    if(fn_object.get_type() != sol::type::function)
    {
        return 1;
    }

    sol::protected_function fn = fn_object.as<sol::protected_function>();
    std::vector<sol::object> lua_args;
    lua_args.reserve(arg_count);
    for(int itr = 0; itr < arg_count; itr++)
    {
        lua_args.push_back(sol::make_object(*mod_lua_state, (lua_Integer)args[itr]));
    }

    sol::protected_function_result call = fn(sol::as_args(lua_args));
    std::string problem;
    if(!call.valid())
    {
        sol::error err = call;
        problem = err.what();
    }
    else
    {
        sol::object returned = call;
        double value = (returned.get_type() == sol::type::number) ? returned.as<double>() : NAN;
        if(!std::isfinite(value) || (value != std::floor(value)) || (value > 2147483647.0) || (value < -2147483648.0))
        {
            problem = "must return an integer";
        }
        else
        {
            *result = (int32_t)value;
            return 0;
        }
    }

    int & count = error_counts[name];
    count++;
    if(count <= max_logged_errors)
    {
        MOD_Lua_Set_Error(std::string("rules.") + name + " (profile \"" + mod_rules_profile + "\"): " + problem + "; using the C version" + ((count == max_logged_errors) ? " (further errors from this rule are not logged)" : ""));
    }
    return -1;
}


/* ---------------------------------------------------------------------------------------------------------------- */
/*  Rules catalog (MOD_Rules_Write_Catalog)                                                                         */
/* ---------------------------------------------------------------------------------------------------------------- */

struct s_CATALOG_POINT
{
    std::string id;
    std::string label;
    std::vector<int32_t> args;
};

struct s_CATALOG_INPUT
{
    std::string name;
    std::string description;
};

struct s_CATALOG_CALCULATION
{
    std::map<std::string, std::string> fields;     /* rule, label, description, unit, c_function, ... */
    std::vector<s_CATALOG_INPUT> inputs;
    std::vector<s_CATALOG_POINT> points;
};

static std::string Catalog_Literal(const std::string & text)
{
    std::string out = "\"";
    for(char c : text)
    {
        if(c == '\\' || c == '"') { out += '\\'; out += c; }
        else if(c == '\n') { out += "\\n"; }
        else if(c == '\r') { }
        else { out += c; }
    }
    return out + "\"";
}

static std::string Catalog_Profile_Id(const std::string & profile)
{
    size_t slash = profile.find_last_of("/\\");
    return (slash == std::string::npos) ? profile : profile.substr(slash + 1);
}

/* Runs catalog.lua in the current state and reads its declarations. */
static bool Catalog_Read(const std::string & path, std::vector<s_CATALOG_CALCULATION> & calculations)
{
    sol::state & lua = *mod_lua_state;
    sol::table catalog = lua.create_named_table("catalog");
    sol::table declared = lua.create_table();
    catalog["declared"] = declared;
    catalog.set_function("calculation", [](sol::table entry, sol::this_state ts)
    {
        sol::state_view view(ts);
        sol::table list = view["catalog"]["declared"];
        list.add(entry);
    });
    try
    {
        MOD_Rules_Run_File(lua, path);
    }
    catch(const std::exception & ex)
    {
        MOD_Lua_Set_Error(std::string("rules catalog \"") + path + "\": " + ex.what());
        return false;
    }

    for(size_t itr = 1; itr <= declared.size(); itr++)
    {
        sol::table entry = declared[itr];
        s_CATALOG_CALCULATION calc;
        const char * keys[] = { "rule", "label", "description", "unit", "c_function", "c_original", "c_file", "wzd", "drake178" };
        for(const char * key : keys)
        {
            sol::optional<std::string> value = entry[key];
            if(value) { calc.fields[key] = *value; }
        }
        if(calc.fields["rule"].empty())
        {
            MOD_Lua_Set_Error("rules catalog: a calculation has no rule name");
            return false;
        }
        sol::optional<sol::table> inputs = entry["inputs"];
        if(inputs)
        {
            for(size_t i = 1; i <= inputs->size(); i++)
            {
                sol::table in = (*inputs)[i];
                calc.inputs.push_back({ in.get_or<std::string>("name", ""), in.get_or<std::string>("description", "") });
            }
        }
        sol::optional<sol::table> points = entry["data_points"];
        if(points)
        {
            for(size_t i = 1; i <= points->size(); i++)
            {
                sol::table pt = (*points)[i];
                s_CATALOG_POINT point;
                point.id = pt.get_or<std::string>("id", "");
                point.label = pt.get_or<std::string>("label", point.id);
                sol::table args = pt["args"];
                for(size_t a = 1; a <= args.size(); a++) { point.args.push_back(args.get<int32_t>(a)); }
                if(point.id.empty() || (point.args.size() != calc.inputs.size()))
                {
                    MOD_Lua_Set_Error("rules catalog: " + calc.fields["rule"] + " data point \"" + point.id + "\" needs an id and one argument per input");
                    return false;
                }
                calc.points.push_back(point);
            }
        }
        calculations.push_back(calc);
    }
    return true;
}

int MOD_Rules_Write_Catalog(const char * catalog_path, const char * const * profiles, int profile_count, const char * out_path)
{
    std::vector<s_CATALOG_CALCULATION> calculations;
    std::ostringstream ttl;
    const std::string ns = "https://github.com/jbalcomb/remom/ns/rules";

    if((profile_count < 1) || (MOD_Rules_Load(profiles[0]) != 0) || !Catalog_Read(catalog_path, calculations))
    {
        return -1;
    }

    ttl << "# ReMoM rules catalog: the game calculations rules profiles decide, and their results per profile.\n"
        << "# Generated by tools/rules_catalog from " << "assets/rules/catalog.lua" << " and the profiles; do not edit.\n"
        << "# One triple per line, so it also reads as a table (tools/rules_catalog/ttl_to_table.py).\n\n"
        << "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
        << "@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .\n"
        << "@prefix xsd: <http://www.w3.org/2001/XMLSchema#> .\n"
        << "@prefix remom: <" << ns << "#> .\n"
        << "@prefix calc: <" << ns << "/calculation/> .\n"
        << "@prefix input: <" << ns << "/input/> .\n"
        << "@prefix dp: <" << ns << "/datapoint/> .\n"
        << "@prefix profile: <" << ns << "/profile/> .\n"
        << "@prefix val: <" << ns << "/value/> .\n\n";

    /* The vocabulary, so the file describes itself in any triple store. */
    const char * vocabulary[][3] = {
        { "remom:Calculation", "rdfs:Class", "A game calculation a rules profile decides." },
        { "remom:Input", "rdfs:Class", "One argument of a calculation." },
        { "remom:DataPoint", "rdfs:Class", "A named set of arguments whose result is compared across profiles." },
        { "remom:Profile", "rdfs:Class", "A rules profile: a game version or mod (assets/rules/<id>/)." },
        { "remom:Value", "rdfs:Class", "The result of one data point under one profile." },
        { "remom:rule", "rdf:Property", "Name of the rule function in the profiles (rules.<rule>)." },
        { "remom:cFunction", "rdf:Property", "C function that asks the rules profile." },
        { "remom:cOriginal", "rdf:Property", "Faithful 1.31 C function the original profile is tested against." },
        { "remom:cFile", "rdf:Property", "Source file of the C functions." },
        { "remom:wzd", "rdf:Property", "Segment/procedure of the function in the 1.31 executable (WIZARDS.EXE)." },
        { "remom:drake178", "rdf:Property", "drake178's name for the function." },
        { "remom:unit", "rdf:Property", "Unit of the result." },
        { "remom:input", "rdf:Property", "An input of the calculation." },
        { "remom:position", "rdf:Property", "1-based position of an input among the arguments." },
        { "remom:calculation", "rdf:Property", "The calculation a data point or value belongs to." },
        { "remom:argument", "rdf:Property", "The data point's arguments, in input order, as text." },
        { "remom:dataPoint", "rdf:Property", "The data point a value is for." },
        { "remom:profile", "rdf:Property", "The profile a value is for." },
        { "remom:value", "rdf:Property", "The result." },
        { "remom:base", "rdf:Property", "The profile a profile starts from." },
        { "remom:status", "rdf:Property", "How far a profile's version of a calculation is confirmed." },
        { "remom:source", "rdf:Property", "Where a profile's version of a calculation comes from." },
        { "remom:note", "rdf:Property", "Anything else a profile says about its version of a calculation." },
        { "remom:about", "rdf:Property", "Links a profile's statement (status, source, note) to the calculation it is about." },
    };
    for(const auto & v : vocabulary)
    {
        ttl << v[0] << " rdf:type " << v[1] << " .\n";
        ttl << v[0] << " rdfs:comment " << Catalog_Literal(v[2]) << " .\n";
    }
    ttl << "\n";

    const std::map<std::string, std::string> predicates = {
        { "label", "rdfs:label" }, { "description", "rdfs:comment" }, { "unit", "remom:unit" }, { "c_function", "remom:cFunction" },
        { "c_original", "remom:cOriginal" }, { "c_file", "remom:cFile" }, { "wzd", "remom:wzd" }, { "drake178", "remom:drake178" } };
    for(const auto & calc : calculations)
    {
        const std::string rule = calc.fields.at("rule");
        const std::string c = "calc:" + rule;
        ttl << c << " rdf:type remom:Calculation .\n";
        ttl << c << " remom:rule " << Catalog_Literal(rule) << " .\n";
        for(const auto & p : predicates)
        {
            auto it = calc.fields.find(p.first);
            if(it != calc.fields.end()) { ttl << c << " " << p.second << " " << Catalog_Literal(it->second) << " .\n"; }
        }
        for(size_t i = 0; i < calc.inputs.size(); i++)
        {
            const std::string in = "input:" + rule + "-" + calc.inputs[i].name;
            ttl << c << " remom:input " << in << " .\n";
            ttl << in << " rdf:type remom:Input .\n";
            ttl << in << " rdfs:label " << Catalog_Literal(calc.inputs[i].name) << " .\n";
            ttl << in << " rdfs:comment " << Catalog_Literal(calc.inputs[i].description) << " .\n";
            ttl << in << " remom:position " << (i + 1) << " .\n";
        }
        for(const auto & pt : calc.points)
        {
            const std::string d = "dp:" + rule + "-" + pt.id;
            std::string args;
            for(size_t a = 0; a < pt.args.size(); a++) { args += (a ? ", " : "") + calc.inputs[a].name + "=" + std::to_string(pt.args[a]); }
            ttl << d << " rdf:type remom:DataPoint .\n";
            ttl << d << " remom:calculation " << c << " .\n";
            ttl << d << " rdfs:label " << Catalog_Literal(pt.label) << " .\n";
            ttl << d << " remom:argument " << Catalog_Literal(args) << " .\n";
        }
        ttl << "\n";
    }

    for(int p = 0; p < profile_count; p++)
    {
        if((p > 0) && (MOD_Rules_Load(profiles[p]) != 0))
        {
            return -1;
        }
        const std::string pid = Catalog_Profile_Id(profiles[p]);
        const std::string pr = "profile:" + pid;
        sol::table rules = (*mod_lua_state)["rules"];
        ttl << pr << " rdf:type remom:Profile .\n";
        ttl << pr << " rdfs:label " << Catalog_Literal(rules.get_or<std::string>("label", pid)) << " .\n";
        sol::optional<std::string> description = rules["description"];
        if(description) { ttl << pr << " rdfs:comment " << Catalog_Literal(*description) << " .\n"; }
        sol::optional<std::string> base = rules["base"];
        if(base) { ttl << pr << " remom:base profile:" << *base << " .\n"; }

        sol::table about = rules["about_entries"];
        for(const auto & calc : calculations)
        {
            const std::string rule = calc.fields.at("rule");
            sol::optional<sol::table> info = about[rule];
            if(info)
            {
                const std::string a = "val:" + rule + "-about-" + pid;
                ttl << a << " remom:profile " << pr << " .\n";
                ttl << a << " remom:about calc:" << rule << " .\n";
                const char * keys[] = { "status", "source", "note" };
                for(const char * key : keys)
                {
                    sol::optional<std::string> text = (*info)[key];
                    if(text) { ttl << a << " remom:" << key << " " << Catalog_Literal(*text) << " .\n"; }
                }
            }
            for(const auto & pt : calc.points)
            {
                int32_t result = 0;
                const std::string v = "val:" + rule + "-" + pt.id + "-" + pid;
                int rc = MOD_Rules_Call_Int(rule.c_str(), pt.args.data(), (int)pt.args.size(), &result);
                if(rc < 0)
                {
                    return -1;
                }
                ttl << v << " rdf:type remom:Value .\n";
                ttl << v << " remom:dataPoint dp:" << rule << "-" << pt.id << " .\n";
                ttl << v << " remom:profile " << pr << " .\n";
                if(rc == 0) { ttl << v << " remom:value " << result << " .\n"; }
                else        { ttl << v << " remom:status " << Catalog_Literal("not ported: the C version decides") << " .\n"; }
            }
        }
        ttl << "\n";
    }

    std::ofstream out(out_path, std::ios::out | std::ios::binary);
    if(!out)
    {
        MOD_Lua_Set_Error(std::string("rules catalog: cannot write \"") + out_path + "\"");
        return -1;
    }
    out << ttl.str();
    LOG_INFO(LOG_CAT_MOD, "[MOD] rules catalog written to \"%s\" (%d profiles, %d calculations)", out_path, profile_count, (int)calculations.size());
    return 0;
}
