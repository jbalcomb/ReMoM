/*
    MOX2 — module added by SimTex in MoO2 to host engine-wide cross-cutting
    code that didn't fit elsewhere.  In MoM/this reconstruction it doesn't
    exist yet; we're growing it incrementally as MoO2 functions get back-
    ported.

    First inhabitant: Check_Command_Line_Parameters_().  In MoO2 this is the
    single CLI-parsing entry point called early in _main().  Here, both HeMoM
    and ReMoM call it and then read the per-flag globals it sets.
*/
#ifndef MOX_MOX2_H
#define MOX_MOX2_H

#include "MOX_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
    Command-line seed override.  Default 0 = "use Randomize() / system clock"
    (the original behaviour).  When non-zero, sites that would otherwise call
    Randomize() (PreInit_Overland, etc.) honour this seed via Set_Random_Seed
    so subsequent Random() calls are deterministic.

    MoO2 has this declared in MOX2; we mirror that placement here.
*/
extern int32_t _cmd_line_seed;

/*
    Walks argv once and sets the _cmd_line_* globals based on recognised
    flags.  Currently handles:
        --seed N

    Unrecognised arguments are ignored at this layer; per-target main()
    parsers will keep handling target-specific flags during the incremental
    migration of CLI parsing from HeMoM/ReMoM into MOX2.
*/
void Check_Command_Line_Parameters_(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif /* MOX_MOX2_H */
