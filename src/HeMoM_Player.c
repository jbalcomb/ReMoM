/**
 * HeMoM_Player.c — Synthetic human player for headless testing.
 *
 * Reads a test scenario file and injects input actions one per frame
 * through the platform input layer (same functions the replay system uses).
 *
 * Actions with a wait count idle for that many frames before the next action.
 * Actions without an explicit wait execute on the next frame.
 *
 * The synthetic player sits at the same level as the replay system:
 * it writes into the engine's input globals, and the engine's Get_Input() /
 * Interpret_Mouse_Input() processes them normally through the Fields system.
 */

#include "HeMoM_Player.h"

#include "../platform/include/Platform.h"
#include "../platform/include/Platform_Keys.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../ext/stu_compat.h"

#ifdef STU_DEBUG
#include "../STU/src/STU_DBG.h"
#endif



/* ========================================================================= */
/*  Action Types                                                             */
/* ========================================================================= */

enum e_HeMoM_Action_Type
{
    act_NONE = 0,
    act_WAIT,           /* wait N frames */
    act_KEY,            /* press a key (character) */
    act_ESCAPE,         /* press Escape */
    act_ENTER,          /* press Enter */
    act_BACKSPACE,      /* press Backspace */
    act_CLICK,          /* left-click at (x, y) */
    act_RCLICK,         /* right-click at (x, y) */
    act_QUIT,           /* press Escape to quit */
    act_END             /* stop the synthetic player */
};

struct s_HeMoM_Action
{
    int type;
    int wait_frames;    /* frames to idle BEFORE this action fires */
    int16_t x;          /* for click/rclick */
    int16_t y;
    char character;     /* for act_KEY */
    uint32_t packed_key;/* pre-packed key value for keyboard buffer */
};

#define HEMOM_MAX_ACTIONS 4096



/* ========================================================================= */
/*  State                                                                    */
/* ========================================================================= */

static struct s_HeMoM_Action hemom_actions[HEMOM_MAX_ACTIONS];
static int hemom_action_count = 0;
static int hemom_action_index = 0;
static int hemom_wait_remaining = 0;
static int hemom_active = 0;
static int hemom_quit_phase = 0;  /* for act_QUIT: 0=first ESC, 1=wait, 2=second ESC */



/* ========================================================================= */
/*  Key Packing Helpers                                                      */
/* ========================================================================= */

static uint32_t Pack_Key_Char(char ch)
{
    int mox_key;
    uint32_t mox_mod = 0;
    char mox_character = ch;

    if (ch >= 'a' && ch <= 'z')
    {
        mox_key = MOX_KEY_a + (ch - 'a');
    }
    else if (ch >= 'A' && ch <= 'Z')
    {
        mox_key = MOX_KEY_a + (ch - 'A');
        mox_mod = MOX_MOD_SHIFT;
        mox_character = ch;
    }
    else if (ch == ' ')
    {
        mox_key = MOX_KEY_SPACE;
    }
    else if (ch >= '0' && ch <= '9')
    {
        mox_key = MOX_KEY_SPACE;  /* digits use mox_character, not mox_key */
    }
    else if (ch == '+')
    {
        mox_key = MOX_KEY_NUMPLUS;
    }
    else if (ch == '-')
    {
        mox_key = MOX_KEY_NUMMINUS;
    }
    else if (ch == '\t')
    {
        mox_key = MOX_KEY_TAB;
    }
    else
    {
        mox_key = MOX_KEY_SPACE;  /* fallback */
    }

    return ((uint32_t)mox_key) | mox_mod | (((uint32_t)(unsigned char)mox_character) << 8);
}

static uint32_t Pack_Key_Escape(void)
{
    /* Escape: character byte left as 0 so Read_Key() returns the key code (ST_KEY_ESCAPE = 0x1B). */
    return (uint32_t)MOX_KEY_ESCAPE;
}

static uint32_t Pack_Key_Enter(void)
{
    /* Enter: character byte left as 0 so Read_Key() returns the key code (ST_KEY_ENTER = 0x0C),
       NOT the ASCII '\r' (0x0D). The popup input loop checks against the MoX key code. */
    return (uint32_t)MOX_KEY_ENTER;
}

static uint32_t Pack_Key_Backspace(void)
{
    /* Backspace: character byte left as 0 so Read_Key() returns the key code (ST_KEY_BACKSPACE = 0x0B),
       NOT the ASCII '\b' (0x08). */
    return (uint32_t)MOX_KEY_BACKSPACE;
}



/* ========================================================================= */
/*  Scenario Parser                                                          */
/* ========================================================================= */

static char *Trim(char *str)
{
    char *end;
    while (*str && isspace((unsigned char)*str)) { str++; }
    if (*str == '\0') { return str; }
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) { *end-- = '\0'; }
    return str;
}

int HeMoM_Player_Load_Scenario(const char *filepath)
{
    FILE *fp;
    char line[256];
    char *p;
    int line_num = 0;
    struct s_HeMoM_Action *act;

    hemom_action_count = 0;
    hemom_action_index = 0;
    hemom_wait_remaining = 0;
    hemom_active = 0;
    hemom_quit_phase = 0;

    fp = stu_fopen_ci(filepath, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "[HeMoM Player] Could not open scenario: %s\n", filepath);
        return 1;
    }

    fprintf(stderr, "[HeMoM Player] Loading scenario: %s\n", filepath);
#ifdef STU_DEBUG
    dbg_prn("[HeMoM Player] Loading scenario: %s\n", filepath);
    trc_prn("[HeMoM Player] Loading scenario: %s\n", filepath);
#endif

    while (fgets(line, sizeof(line), fp) != NULL && hemom_action_count < HEMOM_MAX_ACTIONS)
    {
        char *comment;
        line_num++;
        p = Trim(line);

        if (*p == '\0' || *p == '#') { continue; }

        /* Strip inline `#` comments and re-trim trailing whitespace. */
        comment = strchr(p, '#');
        if (comment != NULL)
        {
            *comment = '\0';
            while (comment > p && isspace((unsigned char)*(comment - 1)))
            {
                comment--;
                *comment = '\0';
            }
            if (*p == '\0') { continue; }
        }

        act = &hemom_actions[hemom_action_count];
        memset(act, 0, sizeof(*act));

        if (stu_strnicmp(p, "wait ", 5) == 0)
        {
            act->type = act_WAIT;
            act->wait_frames = atoi(p + 5);
            if (act->wait_frames < 1) { act->wait_frames = 1; }
        }
        else if (stu_strnicmp(p, "key ", 4) == 0)
        {
            char *val = Trim(p + 4);
            act->type = act_KEY;
            act->character = val[0];
            act->packed_key = Pack_Key_Char(val[0]);
        }
        else if (stu_stricmp(p, "escape") == 0)
        {
            act->type = act_ESCAPE;
            act->packed_key = Pack_Key_Escape();
        }
        else if (stu_stricmp(p, "enter") == 0)
        {
            act->type = act_ENTER;
            act->packed_key = Pack_Key_Enter();
        }
        else if (stu_stricmp(p, "backspace") == 0)
        {
            act->type = act_BACKSPACE;
            act->packed_key = Pack_Key_Backspace();
        }
        else if (stu_strnicmp(p, "click ", 6) == 0)
        {
            act->type = act_CLICK;
            if (sscanf(p + 6, "%hd %hd", &act->x, &act->y) != 2)
            {
                fprintf(stderr, "[HeMoM Player] %s:%d: bad click coords: %s\n", filepath, line_num, p);
                continue;
            }
        }
        else if (stu_strnicmp(p, "rclick ", 7) == 0)
        {
            act->type = act_RCLICK;
            if (sscanf(p + 7, "%hd %hd", &act->x, &act->y) != 2)
            {
                fprintf(stderr, "[HeMoM Player] %s:%d: bad rclick coords: %s\n", filepath, line_num, p);
                continue;
            }
        }
        else if (stu_stricmp(p, "next_turn") == 0)
        {
            act->type = act_KEY;
            act->character = 'n';
            act->packed_key = Pack_Key_Char('n');
        }
        else if (stu_stricmp(p, "quit") == 0)
        {
            act->type = act_QUIT;
            act->packed_key = Pack_Key_Escape();
        }
        else if (stu_stricmp(p, "end") == 0)
        {
            act->type = act_END;
        }
        else
        {
            fprintf(stderr, "[HeMoM Player] %s:%d: unknown action: %s\n", filepath, line_num, p);
            continue;
        }

        hemom_action_count++;
    }

    fclose(fp);

    if (hemom_action_count == 0)
    {
        fprintf(stderr, "[HeMoM Player] Scenario is empty\n");
        return 1;
    }

    hemom_active = 1;
    fprintf(stderr, "[HeMoM Player] Loaded %d actions\n", hemom_action_count);
#ifdef STU_DEBUG
    dbg_prn("[HeMoM Player] Loaded %d actions\n", hemom_action_count);
    trc_prn("[HeMoM Player] Loaded %d actions\n", hemom_action_count);
#endif
    return 0;
}



/* ========================================================================= */
/*  Per-Frame Execution                                                      */
/* ========================================================================= */

void HeMoM_Player_Frame(void)
{
    struct s_HeMoM_Action *act;
    int mox_key;
    char mox_character;
    uint32_t mox_mod;
    int16_t wx, wy;
    int scale;

    if (!hemom_active) { return; }
    if (hemom_action_index >= hemom_action_count)
    {
        hemom_active = 0;
        fprintf(stderr, "[HeMoM Player] Scenario complete (%d actions)\n", hemom_action_count);
#ifdef STU_DEBUG
        dbg_prn("[HeMoM Player] Scenario complete (%d actions)\n", hemom_action_count);
        trc_prn("[HeMoM Player] Scenario complete (%d actions)\n", hemom_action_count);
#endif
        quit_game_flag = ST_TRUE;
        return;
    }

    /* Handle multi-frame wait */
    if (hemom_wait_remaining > 0)
    {
        hemom_wait_remaining--;
        return;
    }

    act = &hemom_actions[hemom_action_index];

    switch (act->type)
    {
        case act_WAIT:
        {
            hemom_wait_remaining = act->wait_frames - 1;  /* -1 because this frame counts */
            hemom_action_index++;
            fprintf(stderr, "[HeMoM Player] wait %d frames\n", act->wait_frames);
#ifdef STU_DEBUG
            dbg_prn("[HeMoM Player] wait %d frames\n", act->wait_frames);
            trc_prn("[HeMoM Player] wait %d frames\n", act->wait_frames);
#endif
        } break;

        case act_KEY:
        {
            mox_key = (int)(act->packed_key & 0xFF);
            mox_character = (char)((act->packed_key >> 8) & 0xFF);
            mox_mod = act->packed_key & 0xFFFF0000u;
            Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
            hemom_action_index++;
            fprintf(stderr, "[HeMoM Player] key '%c' (packed=0x%08X)\n", act->character, act->packed_key);
#ifdef STU_DEBUG
            dbg_prn("[HeMoM Player] key '%c' (packed=0x%08X)\n", act->character, act->packed_key);
            trc_prn("[HeMoM Player] key '%c' (packed=0x%08X)\n", act->character, act->packed_key);
#endif
        } break;

        case act_ESCAPE:
        {
            mox_key = (int)(act->packed_key & 0xFF);
            mox_character = (char)((act->packed_key >> 8) & 0xFF);
            mox_mod = act->packed_key & 0xFFFF0000u;
            Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
            hemom_action_index++;
            fprintf(stderr, "[HeMoM Player] escape\n");
#ifdef STU_DEBUG
            dbg_prn("[HeMoM Player] escape\n");
            trc_prn("[HeMoM Player] escape\n");
#endif
        } break;

        case act_ENTER:
        {
            mox_key = (int)(act->packed_key & 0xFF);
            mox_character = (char)((act->packed_key >> 8) & 0xFF);
            mox_mod = act->packed_key & 0xFFFF0000u;
            Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
            hemom_action_index++;
            fprintf(stderr, "[HeMoM Player] enter\n");
#ifdef STU_DEBUG
            dbg_prn("[HeMoM Player] enter\n");
            trc_prn("[HeMoM Player] enter\n");
#endif
        } break;

        case act_BACKSPACE:
        {
            mox_key = (int)(act->packed_key & 0xFF);
            mox_character = (char)((act->packed_key >> 8) & 0xFF);
            mox_mod = act->packed_key & 0xFFFF0000u;
            Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
            hemom_action_index++;
            fprintf(stderr, "[HeMoM Player] backspace\n");
#ifdef STU_DEBUG
            dbg_prn("[HeMoM Player] backspace\n");
            trc_prn("[HeMoM Player] backspace\n");
#endif
        } break;

        case act_CLICK:
        case act_RCLICK:
        {
            scale = Platform_Get_Scale();
            wx = act->x * (int16_t)scale;
            wy = act->y * (int16_t)scale;
            pointer_x = act->x;
            pointer_y = act->y;
            if (act->type == act_CLICK)
            {
                platform_frame_mouse_buttons |= ST_LEFT_BUTTON;
                User_Mouse_Handler(ST_LEFT_BUTTON, wx, wy);
                fprintf(stderr, "[HeMoM Player] click (%d, %d)\n", act->x, act->y);
#ifdef STU_DEBUG
                dbg_prn("[HeMoM Player] click (%d, %d)\n", act->x, act->y);
                trc_prn("[HeMoM Player] click (%d, %d)\n", act->x, act->y);
#endif
            }
            else
            {
                platform_frame_mouse_buttons |= ST_RIGHT_BUTTON;
                User_Mouse_Handler(ST_RIGHT_BUTTON, wx, wy);
                fprintf(stderr, "[HeMoM Player] rclick (%d, %d)\n", act->x, act->y);
#ifdef STU_DEBUG
                dbg_prn("[HeMoM Player] rclick (%d, %d)\n", act->x, act->y);
                trc_prn("[HeMoM Player] rclick (%d, %d)\n", act->x, act->y);
#endif
            }
            hemom_action_index++;
        } break;

        case act_QUIT:
        {
            quit_game_flag = ST_TRUE;
            hemom_action_index++;
            fprintf(stderr, "[HeMoM Player] quit\n");
#ifdef STU_DEBUG
            dbg_prn("[HeMoM Player] quit\n");
            trc_prn("[HeMoM Player] quit\n");
#endif
        } break;

        case act_END:
        {
            hemom_active = 0;
            fprintf(stderr, "[HeMoM Player] end — synthetic player stopped\n");
#ifdef STU_DEBUG
            dbg_prn("[HeMoM Player] end — synthetic player stopped\n");
            trc_prn("[HeMoM Player] end — synthetic player stopped\n");
#endif
        } break;
    }
}

int HeMoM_Player_Active(void)
{
    return hemom_active;
}

void HeMoM_Player_Shutdown(void)
{
    hemom_active = 0;
    hemom_action_count = 0;
    hemom_action_index = 0;
    hemom_wait_remaining = 0;
}
