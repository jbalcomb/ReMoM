#ifndef STU_TST_H
#define STU_TST_H



#ifdef __cplusplus
extern "C" {
#endif


/*
    Scenario patching -- see doc/#Devel/Scenario-Patching.md

    TST_Patch_Game_Data() runs from Loaded_Game_Update(), immediately after every save load, in EVERY
    build including Release.  That is deliberate: the demo reel is filmed from the Release binary an
    end user downloads, so a STU_DEBUG gate would mean the setup never runs in the build being filmed.

    Safety comes from being INERT BY DEFAULT instead: with no scenario selected the function returns
    immediately and touches nothing, exactly as it behaves today.  This is the same pattern as
    --capture / --load / --record / --replay / --scenario, which all ship in the Release binary and do
    nothing unless explicitly asked for.

    Set by ReMoM's `--patch <name>` flag.  NULL (the default) means "do nothing".
*/
extern const char * g_tst_patch_scenario;

void TST_Patch_Game_Data(void);


#ifdef __cplusplus
}
#endif

#endif /* STU_TST_H */
