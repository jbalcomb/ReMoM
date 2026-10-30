/*
    rules_catalog -- writes the rules catalog: every calculation in assets/rules/catalog.lua, run for each of its data
    points under each rules profile, as RDF triples (Turtle).

    Usage:  rules_catalog CATALOG.lua OUT.ttl PROFILE [PROFILE ...]
            PROFILE is a profile name (looked up in rules/) or a profile folder.
    Exit:   0 on success, 1 on error (the message is printed).

    The committed catalog is doc/rules-catalog/rules_catalog.ttl; the RulesCatalog_UpToDate test regenerates it and
    fails if it differs.  tools/rules_catalog/ttl_to_table.py turns it into doc/rules-catalog/rules_catalog.md.
*/

#include "../../MOD/src/MOD_Lua.h"
#include "../../MOD/src/MOD_Rules.h"

#include <stdio.h>

int main(int argc, char * argv[])
{
    if(argc < 4)
    {
        fprintf(stderr, "usage: %s CATALOG.lua OUT.ttl PROFILE [PROFILE ...]\n", argv[0]);
        return 1;
    }
    if(MOD_Rules_Write_Catalog(argv[1], (const char * const *)&argv[3], argc - 3, argv[2]) != 0)
    {
        fprintf(stderr, "%s: %s\n", argv[0], MOD_Lua_Last_Error());
        return 1;
    }
    return 0;
}
