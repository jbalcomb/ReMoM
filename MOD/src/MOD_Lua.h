/*
    MOD_Lua -- the Lua scripting runtime for mods (Lua 5.4 through sol2).

    The game stays C; this header is the whole C-side view of Lua.  Scripts run in a sandbox: no io, os, package,
    debug, dofile, loadfile, load or require, and math.random draws from the game's own Random() so replays stay
    deterministic.

    Script API (table `game`; names follow Caster of Magic's CAS commands, lower-cased):
        game.on(hook_name, fn)      register fn(subject_idx, value) -> new value on a hook ("city_gold", ...)
        game.rnd(x)                 random integer 0 .. x-1                         ~ CAS RND(X)
        game.maxcity()              highest existing city index, -1 if none         ~ CAS MAXCITY()
        game.cityowner(c)           owner player index of city c                    ~ CAS CITYOWNER(C)
        game.cityrace(c)            race of city c                                  ~ CAS CITYRACE(C)
        game.citysize(c)            size of city c
        game.isbuilt(c, b)          1 if city c has building b (built or replaced)  ~ CAS ISBUILT(C,B)
        game.log(text)              write to the ReMoM log (print() does the same)
*/

#ifndef MOD_LUA_H
#define MOD_LUA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Creates the sandboxed Lua state.  Safe to call more than once.  Returns 0 on success, -1 on failure (logged). */
int MOD_Lua_Init(void);

/* Removes every hook handler and closes the Lua state. */
void MOD_Lua_Shutdown(void);

/* Runs a chunk of Lua source.  chunk_name appears in error messages.  Returns 0 on success, -1 on error (logged). */
int MOD_Lua_Run_String(const char * chunk_name, const char * source);

/* Reads and runs a Lua file.  Returns 0 on success, -1 on error (logged). */
int MOD_Lua_Run_File(const char * path);

/* Message of the most recent script error ("" if none). */
const char * MOD_Lua_Last_Error(void);

#ifdef __cplusplus
}
#endif

#endif  /* MOD_LUA_H */
