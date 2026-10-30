/*
    MOD_Rules -- game rules written in Lua, one folder per rules profile.

    A rule that differs between games (1.31, the Community Patch, Caster of Magic, a mod) lives in a Lua script, not
    in C.  The C code keeps the engine work (reading the map, iterating a city's area) and asks the loaded profile for
    the rule's result.  The faithful C version of each rule stays as the fallback and as the reference the 1.31
    script is tested against.

    Profiles:  <search dir>/rules/<profile>/profile.lua, searched in the current directory, then beside the executable.
               A profile name containing '/' or '\' is a folder path instead.
        original          1.31, translated from the reconstructed C
        community_patch   1.31 plus the Community Patch's changes
        com2              Caster of Magic for Windows

    Script API, in addition to MOD_Lua's:
        rules                       the table profiles put their rule functions in (rules.square_production_bonus, ...)
        rules.profile               the profile's name
        rules.load(file)            runs another file of this profile
        rules.load(profile, file)   runs a file of another profile, e.g. to start from "original" and change one rule
        rules.label, rules.description, rules.base
                                    the profile's name, summary and the profile it starts from, for the rules catalog
        rules.about(rule, { status = ..., source = ..., note = ... })
                                    what the profile says about its version of a rule, for the rules catalog
        terrain                     1.31's terrain type constants (terrain.Mountain1, terrain.Desert_Lst, ...)
*/

#ifndef MOD_RULES_H
#define MOD_RULES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Loads a rules profile, replacing any loaded one.  Returns 0 on success, -1 if it cannot be found or fails (logged);
   on failure no profile is loaded and every rule uses its C version. */
int MOD_Rules_Load(const char * profile);

/* Name of the loaded profile, or "" if none. */
const char * MOD_Rules_Profile(void);

/* Calls rules.<name>(args...) and reads its integer result.  Returns 0 on success, 1 if the profile has no such rule
   (nothing logged; the caller uses its C version), -1 on a script error (logged; the caller uses its C version). */
int MOD_Rules_Call_Int(const char * name, const int32_t * args, int arg_count, int32_t * result);

/* Writes the rules catalog: runs catalog_path (assets/rules/catalog.lua) for the calculations and their data points,
   then every data point through each profile, and writes the results as RDF triples (Turtle) to out_path.
   Leaves the last profile loaded.  Returns 0 on success, -1 on error (logged). */
int MOD_Rules_Write_Catalog(const char * catalog_path, const char * const * profiles, int profile_count, const char * out_path);

#ifdef __cplusplus
}
#endif

#endif  /* MOD_RULES_H */
