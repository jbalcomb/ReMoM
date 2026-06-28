/*
    MOX2 — see MOX2.h for module rationale.

    First migrated function: Check_Command_Line_Parameters_().  In MoO2 this
    handled all CLI flags for the whole game.  We're migrating MoM's flags
    into this incrementally; for now it parses --seed1/--seed2 and the remaining
    target-specific flags stay in HeMoM.c / ReMoM.c.
*/

#include <stdio.h>

#include "../../ext/stu_compat.h"

#include "MOX_TYPE.h"
#include "MOX2.h"
#include "../../STU/src/STU_LOG.h"



/*
    Default 0 means "use Randomize() / system clock", i.e. the original
    pre-MoO2 behaviour.  When set non-zero by --seed1 N on the command line,
    Randomize() callsites that have been ported to the MoO2 pattern (e.g.
    PreInit_Overland) substitute Set_Random_Seed(_cmd_line_seed) instead.
*/
int32_t _cmd_line_seed = 0;

/*
    Second seed: the PreInit_Overland (Load-Game) reseed.  See MOX2.h.
    Default 0 = fall back to _cmd_line_seed / Randomize().
*/
int32_t _cmd_line_seed2 = 0;



void Check_Command_Line_Parameters_(int argc, char *argv[])
{
    int argi;

    for (argi = 1; argi < argc; argi++)
    {
        /* --seed1 / --seed2: stu_strcmp is exact and neither is a prefix of the
         * other, so order is arbitrary; keep --seed2 first by habit. */
        if(stu_strcmp(argv[argi], "--seed2") == 0 && (argi + 1) < argc)
        {
            argi++;
            _cmd_line_seed2 = (int32_t)stu_strtol(argv[argi], NULL, 0);
            LOG_INFO(LOG_CAT_MOX2, "[MOX2] CLI: --seed2 %d (0x%08X)",
                (int)_cmd_line_seed2, (unsigned)_cmd_line_seed2);
        }
        else if(stu_strcmp(argv[argi], "--seed1") == 0 && (argi + 1) < argc)
        {
            argi++;
            /* CLAUDE 2026-05-28: was stu_atoi (decimal only, int-sized
             * truncates on 16-bit DOS).  stu_strtol with base 0 auto-
             * detects 0x-hex and preserves the full 32-bit range. */
            _cmd_line_seed = (int32_t)stu_strtol(argv[argi], NULL, 0);
            LOG_INFO(LOG_CAT_MOX2, "[MOX2] CLI: --seed1 %d (0x%08X)",
                (int)_cmd_line_seed, (unsigned)_cmd_line_seed);
        }
        /* Other flags are still handled by per-target main() during the
           incremental migration.  Add them here as they're moved over. */
    }
}
