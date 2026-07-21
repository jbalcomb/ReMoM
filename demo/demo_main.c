/*
    demo_main.c — entry point and effect dispatcher for the stand-alone VGA / video-effect demos.

    Usage:  demo_vga <effect>
    With no (or an unknown) effect name it prints the list of available effects.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../platform/include/Platform.h"  /* Startup_Platform (full-boot effects) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Files the full-boot effects (score/mastery/somlose) write in the working dir: the high-score/
   settings file and the combat cache.  So the demo can be run straight from bin/Debug without
   clobbering the staged copies, we snapshot MAGIC.SET before the run and restore it after (and
   remove the throwaway COMBAT.TMP).  Self-isolation -- no temp dir, no wrapper needed. */
static unsigned char * g_set_backup = NULL;
static long            g_set_backup_len = 0;

static void Demo_Snapshot_Writable_Files(void)
{
    FILE * f = fopen("MAGIC.SET", "rb");
    if(f != NULL)
    {
        fseek(f, 0, SEEK_END);
        g_set_backup_len = ftell(f);
        fseek(f, 0, SEEK_SET);
        if(g_set_backup_len > 0)
        {
            g_set_backup = (unsigned char *)malloc((size_t)g_set_backup_len);
            if(g_set_backup != NULL)
            {
                if(fread(g_set_backup, 1, (size_t)g_set_backup_len, f) != (size_t)g_set_backup_len)
                {
                    free(g_set_backup);
                    g_set_backup = NULL;
                }
            }
        }
        fclose(f);
    }
}

static void Demo_Restore_Writable_Files(void)
{
    if(g_set_backup != NULL)
    {
        FILE * f = fopen("MAGIC.SET", "wb");
        if(f != NULL)
        {
            fwrite(g_set_backup, 1, (size_t)g_set_backup_len, f);
            fclose(f);
        }
        free(g_set_backup);
        g_set_backup = NULL;
    }
    remove("COMBAT.TMP");
}

static const Demo_Effect * const g_effects[] =
{
    &effect_dissolve,
    &effect_earthquake,
    &effect_screen,
    &effect_summon,
    &effect_lose,
    &effect_win,
    &effect_collage,
    &effect_score,
    &effect_mastery,
    &effect_vanish,
    &effect_somlose,
    &effect_somwin
};
#define DEMO_EFFECT_COUNT  ((int)(sizeof(g_effects) / sizeof(g_effects[0])))



static void Demo_List_Effects(void)
{
    int i;

    printf("Available effects:\n");
    for(i = 0; i < DEMO_EFFECT_COUNT; i++)
    {
        printf("  %-12s %s\n", g_effects[i]->name, g_effects[i]->description);
    }
}


int main(int argc, char * argv[])
{
    const Demo_Effect * selected;
    int i;

    /* Unbuffered stdout so progress prints survive if the process is killed (headless smoke). */
    setvbuf(stdout, NULL, _IONBF, 0);

    /* Report faults with a symbolized stack rather than a bare "Segmentation fault". */
    Demo_Install_Crash_Handler();

    selected = NULL;
    if(argc >= 2)
    {
        for(i = 0; i < DEMO_EFFECT_COUNT; i++)
        {
            if(strcmp(argv[1], g_effects[i]->name) == 0)
            {
                selected = g_effects[i];
                break;
            }
        }
    }

    if(selected == NULL)
    {
        printf("Usage: %s <effect> [screen args...]\n", (argc >= 1) ? argv[0] : "demo_vga");
        printf("  screen     <LBX> <entry> <palette>\n");
        printf("  earthquake <LBX> <entry> <palette>\n");
        printf("  dissolve   <lbxA> <entryA> <lbxB> <entryB> <palette>   (one shared palette)\n");
        printf("  palette: 0=EMPERATO(main game)  1=LOADSAVE  2=ARCANUS(castle/magic)  4=spell-mastery\n");
        printf("  e.g. %s screen NEWGAME 0 0     %s dissolve MAIN 0 NEWGAME 0 0\n",
               (argc >= 1) ? argv[0] : "demo_vga", (argc >= 1) ? argv[0] : "demo_vga");
        Demo_List_Effects();
        return (argc >= 2) ? 1 : 0;   /* unknown effect = error; no argument = just listing */
    }

    /* Pass any arguments after the effect name through to the effect (screen overrides). */
    Demo_Set_Args(argc - 2, argv + 2);

    if(selected->full_boot)
    {
        /* Effect does its own full ReMoM_Init_Engine boot; just bring up the platform.
           These effects write MAGIC.SET / COMBAT.TMP, so protect the working dir's copies. */
        Demo_Snapshot_Writable_Files();
        Startup_Platform();
    }
    else
    {
        Demo_Boot();
    }
    printf("Running effect: %s -- close the window to exit\n", selected->name);
    selected->run();

    if(selected->full_boot)
    {
        Demo_Restore_Writable_Files();
    }
    Demo_Shutdown();

    return 0;
}
