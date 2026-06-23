/*
    demo_effects.h — registry of VGA / video-effect demos.

    To add an effect: create effect_<name>.c defining a `const Demo_Effect effect_<name>` whose run()
    self-drives its animation loop (using the demo_harness helpers and looping until Demo_Quit()),
    declare it extern here, and add it to g_effects[] in demo_main.c.
*/

#ifndef DEMO_EFFECTS_H
#define DEMO_EFFECTS_H

typedef struct Demo_Effect
{
    const char * name;          /* command-line selector */
    const char * description;   /* one-line description shown in the listing */
    void (*run)(void);          /* self-driving loop; returns when Demo_Quit() is true */
} Demo_Effect;

extern const Demo_Effect effect_dissolve;
extern const Demo_Effect effect_earthquake;
extern const Demo_Effect effect_screen;
extern const Demo_Effect effect_summon;

#endif /* DEMO_EFFECTS_H */
