/*
    MOX2 — see MOX2.h for module rationale.

    First migrated function: Check_Command_Line_Parameters_().  In MoO2 this
    handled all CLI flags for the whole game.  We're migrating MoM's flags
    into this incrementally; for now it parses --seed and the remaining
    target-specific flags stay in HeMoM.c / ReMoM.c.
*/

#include <stdio.h>

#include "../../ext/stu_compat.h"

#include "MOX_TYPE.h"
#include "MOX2.h"



/*
    Default 0 means "use Randomize() / system clock", i.e. the original
    pre-MoO2 behaviour.  When set non-zero by --seed N on the command line,
    Randomize() callsites that have been ported to the MoO2 pattern (e.g.
    PreInit_Overland) substitute Set_Random_Seed(_cmd_line_seed) instead.
*/
int32_t _cmd_line_seed = 0;



void Check_Command_Line_Parameters_(int argc, char *argv[])
{
    int argi;

    for (argi = 1; argi < argc; argi++)
    {
        if (stu_strcmp(argv[argi], "--seed") == 0 && (argi + 1) < argc)
        {
            argi++;
            _cmd_line_seed = (int32_t)stu_atoi(argv[argi]);
            fprintf(stderr, "[MOX2] CLI: --seed %d (0x%08X)\n",
                (int)_cmd_line_seed, (unsigned)_cmd_line_seed);
        }
        /* Other flags are still handled by per-target main() during the
           incremental migration.  Add them here as they're moved over. */
    }
}
