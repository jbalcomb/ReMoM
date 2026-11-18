/*
    STU_CI -- Capture/Inject: supply OG-captured values that ReMoM cannot reproduce on its own.

    Reads og-game-data-capture.fwv (produced by extract-ci-stage0.py from the OG-side [CI] probe) and hands OG's exact values to
    the call sites that read OG's uninitialized stack autos (e.g. the Choose_Landmass_For_Settler centroid, Generate_Towers `tries`).
    Distinct from STU_GD (which compares; this injects).  Reverse-engineering evaluation code: no counterpart in the original game.

    File lookup, first hit wins:  $OG_CI_FWV, then og-game-data-capture.fwv in the working directory.
    A missing file means "no injection" -- the call sites keep their Stack_Garbage() stand-ins.

    Compiled in only with STU_GD_CAPTURE (see STU_GD.h; Debug builds).  Without it both functions return -1 and never touch the
    file system, so a Release build's game behaviour cannot be changed by a stray capture file or environment variable.
*/

#ifndef STU_CI_H
#define STU_CI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Load the capture file.  Lazy and idempotent.  Returns the record count, or -1 when nothing was loaded. */
int STU_CI_Load(void);

/* Copy up to max values of record (key, site) into out.  Loads on first use.  Returns the number copied, or -1 if absent. */
int STU_CI_Get(const char * key, const char * site, long * out, int max);

#ifdef __cplusplus
}
#endif

#endif /* STU_CI_H */
