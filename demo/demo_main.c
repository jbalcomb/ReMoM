/*
    demo_main.c — entry point and effect dispatcher for the stand-alone VGA / video-effect demos.

    Usage:  demo_vga <effect>
    With no (or an unknown) effect name it prints the list of available effects.
*/

#include "demo_harness.h"
#include "demo_effects.h"

#include "../platform/include/Platform.h"  /* Startup_Platform (full-boot effects) */

#include <stdio.h>
#include <string.h>

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
    &effect_somlose
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
        /* Effect does its own full ReMoM_Init_Engine boot; just bring up the platform. */
        Startup_Platform();
    }
    else
    {
        Demo_Boot();
    }
    printf("Running effect: %s -- close the window to exit\n", selected->name);
    selected->run();
    Demo_Shutdown();

    return 0;
}
