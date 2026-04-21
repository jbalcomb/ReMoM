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
#include <time.h>

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
    act_WAIT,           /* wait wait_ms milliseconds (wall clock via Platform_Get_Millies) */
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
    uint64_t wait_ms;   /* for act_WAIT — wall-clock milliseconds to wait */
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
static int      hemom_action_count = 0;
static int      hemom_action_index = 0;
static uint64_t hemom_wait_until_ms = 0;  /* Platform_Get_Millies() target for the active wait */
static int      hemom_wait_active = 0;    /* nonzero while a wait is in progress */
static int      hemom_active = 0;
static int      hemom_quit_phase = 0;     /* for act_QUIT: 0=first ESC, 1=wait, 2=second ESC */



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

static uint32_t Pack_Key_Direction(int dir_code)
{
    /* Direction / numpad keys (MOX_KEY_LEFT..MOX_KEY_LEFTDOWN, codes 1..8).
       Used to move unit stacks one square at a time on the world map.
       Character byte left as 0 so Read_Key() returns the key code. */
    return (uint32_t)dir_code;
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



/* ========================================================================= */
/*  Built-in Variables                                                       */
/*                                                                           */
/*  Populated once at scenario-load time and substituted into action strings */
/*  (currently only `type`).  Syntax: `$VARNAME` in the scenario source.     */
/* ========================================================================= */

#define HEMOM_MAX_VARNAME   32
#define HEMOM_MAX_VARVALUE  64

struct s_HeMoM_Var
{
    char name[HEMOM_MAX_VARNAME];
    char value[HEMOM_MAX_VARVALUE];
};

#define HEMOM_MAX_VARS 4

static struct s_HeMoM_Var hemom_vars[HEMOM_MAX_VARS];
static int hemom_var_count = 0;

static void Set_Var(const char *name, const char *value)
{
    int itr;
    for (itr = 0; itr < hemom_var_count; itr++)
    {
        if (strcmp(hemom_vars[itr].name, name) == 0)
        {
            strncpy(hemom_vars[itr].value, value, HEMOM_MAX_VARVALUE - 1);
            hemom_vars[itr].value[HEMOM_MAX_VARVALUE - 1] = '\0';
            return;
        }
    }
    if (hemom_var_count < HEMOM_MAX_VARS)
    {
        strncpy(hemom_vars[hemom_var_count].name, name, HEMOM_MAX_VARNAME - 1);
        hemom_vars[hemom_var_count].name[HEMOM_MAX_VARNAME - 1] = '\0';
        strncpy(hemom_vars[hemom_var_count].value, value, HEMOM_MAX_VARVALUE - 1);
        hemom_vars[hemom_var_count].value[HEMOM_MAX_VARVALUE - 1] = '\0';
        hemom_var_count++;
    }
}

static const char *Get_Var(const char *name)
{
    int itr;
    for (itr = 0; itr < hemom_var_count; itr++)
    {
        if (strcmp(hemom_vars[itr].name, name) == 0)
        {
            return hemom_vars[itr].value;
        }
    }
    return NULL;
}

/**
 * Expand $VARNAME references in `src` into `dst`.
 * Only substitutes registered variables; leaves unknown $FOO references
 * as-is (and logs a warning).
 */
static void Expand_Vars(const char *src, char *dst, size_t dst_size)
{
    size_t di = 0;
    const char *sp = src;

    while (*sp != '\0' && di + 1 < dst_size)
    {
        if (*sp == '$')
        {
            char name[HEMOM_MAX_VARNAME];
            size_t ni = 0;
            const char *vp;
            const char *vv;

            /* Read variable name: $[A-Za-z_][A-Za-z0-9_]* */
            vp = sp + 1;
            while (ni + 1 < sizeof(name) && ((*vp >= 'A' && *vp <= 'Z') || (*vp >= 'a' && *vp <= 'z') || (*vp >= '0' && *vp <= '9') || *vp == '_'))
            {
                name[ni++] = *vp++;
            }
            name[ni] = '\0';

            if (ni > 0)
            {
                vv = Get_Var(name);
                if (vv != NULL)
                {
                    size_t vv_len = strlen(vv);
                    size_t copyable = (vv_len < dst_size - di - 1) ? vv_len : (dst_size - di - 1);
                    memcpy(dst + di, vv, copyable);
                    di += copyable;
                    sp = vp;
                    continue;
                }
                else
                {
                    fprintf(stderr, "[HeMoM Player] unknown variable: $%s\n", name);
                    /* Fall through — emit the literal `$` and keep going. */
                }
            }
        }
        dst[di++] = *sp++;
    }
    dst[di] = '\0';
}

static void Populate_Builtin_Vars(void)
{
    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    char save_name[HEMOM_MAX_VARVALUE];
    if (lt != NULL)
    {
        /* AHP-YYYYMMDDHHMM — 16 chars, fits in LEN_SAVE_DESCRIPTION (20) */
        snprintf(save_name, sizeof(save_name), "AHP-%04d%02d%02d%02d%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min);
    }
    else
    {
        stu_strcpy(save_name, "AHP-00000000");
    }
    Set_Var("SAVE_NAME", save_name);
#ifdef STU_DEBUG
    dbg_prn("[HeMoM Player] $SAVE_NAME = %s\n", save_name);
    trc_prn("[HeMoM Player] $SAVE_NAME = %s\n", save_name);
#endif
}



/* ========================================================================= */
/*  Scenario Parser                                                          */
/* ========================================================================= */

/* Forward decl for recursive include support. */
static int Parse_Scenario_File(const char *filepath, int depth);

#define HEMOM_MAX_INCLUDE_DEPTH 8

/**
 * Parse a wait duration with a unit suffix (no bare numbers allowed).
 * Recognized: NNNf (frames), NNNms (milliseconds), NNNs (seconds), NNNm (minutes).
 * Returns the duration in milliseconds, or 0 on parse error.
 *
 * 1 frame = PLATFORM_MILLISECONDS_PER_FRAME ms (55ms — the BIOS tick interval).
 */
static uint64_t Parse_Wait_Duration_Ms(const char *raw)
{
    char *endp = NULL;
    long long n;

    if (raw == NULL || *raw == '\0') { return 0; }

    n = strtoll(raw, &endp, 10);
    if (endp == raw || n < 0) { return 0; }

    while (*endp == ' ' || *endp == '\t') { endp++; }
    if (*endp == '\0') { return 0; }  /* bare number — not allowed */

    if (stu_stricmp(endp, "f") == 0)  { return (uint64_t)n * PLATFORM_MILLISECONDS_PER_FRAME; }
    if (stu_stricmp(endp, "ms") == 0) { return (uint64_t)n; }
    if (stu_stricmp(endp, "s") == 0)  { return (uint64_t)n * 1000; }
    if (stu_stricmp(endp, "m") == 0)  { return (uint64_t)n * 60 * 1000; }

    return 0;
}

int HeMoM_Player_Load_Scenario(const char *filepath)
{
    int rc;

    hemom_action_count = 0;
    hemom_action_index = 0;
    hemom_wait_until_ms = 0;
    hemom_wait_active = 0;
    hemom_active = 0;
    hemom_quit_phase = 0;
    hemom_var_count = 0;

    Populate_Builtin_Vars();

    fprintf(stderr, "[HeMoM Player] Loading scenario: %s\n", filepath);
#ifdef STU_DEBUG
    dbg_prn("[HeMoM Player] Loading scenario: %s\n", filepath);
    trc_prn("[HeMoM Player] Loading scenario: %s\n", filepath);
#endif

    rc = Parse_Scenario_File(filepath, 0);
    if (rc != 0) { return rc; }

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

/**
 * Resolve a path relative to the current scenario file's directory.
 * If `name` is already absolute, it's used as-is.
 */
static void Resolve_Include_Path(const char *parent_file, const char *name, char *out, size_t out_size)
{
    const char *slash_fwd;
    const char *slash_bwd;
    const char *last_slash;
    size_t dir_len;

    /* Treat as absolute if it starts with / or \ or has a drive letter. */
    if (name[0] == '/' || name[0] == '\\' || (name[0] != '\0' && name[1] == ':'))
    {
        strncpy(out, name, out_size - 1);
        out[out_size - 1] = '\0';
        return;
    }

    slash_fwd = strrchr(parent_file, '/');
    slash_bwd = strrchr(parent_file, '\\');
    last_slash = (slash_fwd > slash_bwd) ? slash_fwd : slash_bwd;

    if (last_slash == NULL)
    {
        strncpy(out, name, out_size - 1);
        out[out_size - 1] = '\0';
        return;
    }

    dir_len = (size_t)(last_slash - parent_file) + 1;
    if (dir_len + strlen(name) + 1 > out_size)
    {
        strncpy(out, name, out_size - 1);
        out[out_size - 1] = '\0';
        return;
    }

    memcpy(out, parent_file, dir_len);
    stu_strcpy(out + dir_len, name);
}

static int Parse_Scenario_File(const char *filepath, int depth)
{
    FILE *fp;
    char  line[512];
    char *p;
    int   line_num = 0;
    struct s_HeMoM_Action *act;

    if (depth >= HEMOM_MAX_INCLUDE_DEPTH)
    {
        fprintf(stderr, "[HeMoM Player] include depth exceeded (limit %d) at %s\n", HEMOM_MAX_INCLUDE_DEPTH, filepath);
        return 1;
    }

    fp = stu_fopen_ci(filepath, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "[HeMoM Player] Could not open scenario: %s\n", filepath);
        return 1;
    }

#ifdef STU_DEBUG
    dbg_prn("[HeMoM Player] Parsing: %s (depth=%d)\n", filepath, depth);
    trc_prn("[HeMoM Player] Parsing: %s (depth=%d)\n", filepath, depth);
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

        /* Include directive — inlines another scenario file. */
        if (stu_strnicmp(p, "include ", 8) == 0)
        {
            char *raw = Trim(p + 8);
            char  resolved[512];
            int   sub_rc;
            Resolve_Include_Path(filepath, raw, resolved, sizeof(resolved));
#ifdef STU_DEBUG
            dbg_prn("[HeMoM Player] %s:%d: include %s -> %s\n", filepath, line_num, raw, resolved);
            trc_prn("[HeMoM Player] %s:%d: include %s -> %s\n", filepath, line_num, raw, resolved);
#endif
            sub_rc = Parse_Scenario_File(resolved, depth + 1);
            if (sub_rc != 0)
            {
                fclose(fp);
                return sub_rc;
            }
            continue;
        }

        act = &hemom_actions[hemom_action_count];
        memset(act, 0, sizeof(*act));

        if (stu_strnicmp(p, "wait ", 5) == 0)
        {
            char *raw = Trim(p + 5);
            uint64_t duration_ms = Parse_Wait_Duration_Ms(raw);
            if (duration_ms == 0)
            {
                fprintf(stderr, "[HeMoM Player] %s:%d: bad wait duration (need NNNf|NNNms|NNNs|NNNm): %s\n", filepath, line_num, raw);
                continue;
            }
            act->type = act_WAIT;
            act->wait_ms = duration_ms;
            hemom_action_count++;
        }
        else if (stu_strnicmp(p, "key ", 4) == 0)
        {
            char *val = Trim(p + 4);
            act->type = act_KEY;
            act->character = val[0];
            act->packed_key = Pack_Key_Char(val[0]);
            hemom_action_count++;
        }
        else if (stu_stricmp(p, "escape") == 0)
        {
            act->type = act_ESCAPE;
            act->packed_key = Pack_Key_Escape();
            hemom_action_count++;
        }
        else if (stu_stricmp(p, "enter") == 0)
        {
            act->type = act_ENTER;
            act->packed_key = Pack_Key_Enter();
            hemom_action_count++;
        }
        else if (stu_stricmp(p, "backspace") == 0)
        {
            act->type = act_BACKSPACE;
            act->packed_key = Pack_Key_Backspace();
            hemom_action_count++;
        }
        /* Direction / numpad keys — move unit stack one square. */
        else if (stu_stricmp(p, "left") == 0)      { act->type = act_KEY; act->packed_key = Pack_Key_Direction(MOX_KEY_LEFT);      hemom_action_count++; }
        else if (stu_stricmp(p, "right") == 0)     { act->type = act_KEY; act->packed_key = Pack_Key_Direction(MOX_KEY_RIGHT);     hemom_action_count++; }
        else if (stu_stricmp(p, "up") == 0)        { act->type = act_KEY; act->packed_key = Pack_Key_Direction(MOX_KEY_UP);        hemom_action_count++; }
        else if (stu_stricmp(p, "down") == 0)      { act->type = act_KEY; act->packed_key = Pack_Key_Direction(MOX_KEY_DOWN);      hemom_action_count++; }
        else if (stu_stricmp(p, "upright") == 0)   { act->type = act_KEY; act->packed_key = Pack_Key_Direction(MOX_KEY_RIGHTUP);   hemom_action_count++; }
        else if (stu_stricmp(p, "downright") == 0) { act->type = act_KEY; act->packed_key = Pack_Key_Direction(MOX_KEY_RIGHTDOWN); hemom_action_count++; }
        else if (stu_stricmp(p, "upleft") == 0)    { act->type = act_KEY; act->packed_key = Pack_Key_Direction(MOX_KEY_LEFTUP);    hemom_action_count++; }
        else if (stu_stricmp(p, "downleft") == 0)  { act->type = act_KEY; act->packed_key = Pack_Key_Direction(MOX_KEY_LEFTDOWN);  hemom_action_count++; }
        else if (stu_strnicmp(p, "click ", 6) == 0)
        {
            act->type = act_CLICK;
            if (sscanf(p + 6, "%hd %hd", &act->x, &act->y) != 2)
            {
                fprintf(stderr, "[HeMoM Player] %s:%d: bad click coords: %s\n", filepath, line_num, p);
                continue;
            }
            hemom_action_count++;
        }
        else if (stu_strnicmp(p, "rclick ", 7) == 0)
        {
            act->type = act_RCLICK;
            if (sscanf(p + 7, "%hd %hd", &act->x, &act->y) != 2)
            {
                fprintf(stderr, "[HeMoM Player] %s:%d: bad rclick coords: %s\n", filepath, line_num, p);
                continue;
            }
            hemom_action_count++;
        }
        else if (stu_stricmp(p, "next_turn") == 0)
        {
            act->type = act_KEY;
            act->character = 'n';
            act->packed_key = Pack_Key_Char('n');
            hemom_action_count++;
        }
        else if (stu_stricmp(p, "quit") == 0)
        {
            act->type = act_QUIT;
            act->packed_key = Pack_Key_Escape();
            hemom_action_count++;
        }
        else if (stu_stricmp(p, "end") == 0)
        {
            act->type = act_END;
            hemom_action_count++;
        }
        else if (stu_strnicmp(p, "type ", 5) == 0)
        {
            /* `type <string>` expands each character into an act_KEY.
               $VARNAME references are substituted at parse time. */
            char *raw = Trim(p + 5);
            char  expanded[HEMOM_MAX_VARVALUE * 2];
            const char *c;

            Expand_Vars(raw, expanded, sizeof(expanded));

#ifdef STU_DEBUG
            dbg_prn("[HeMoM Player] %s:%d: type \"%s\" -> \"%s\"\n", filepath, line_num, raw, expanded);
            trc_prn("[HeMoM Player] %s:%d: type \"%s\" -> \"%s\"\n", filepath, line_num, raw, expanded);
#endif

            for (c = expanded; *c != '\0'; c++)
            {
                if (hemom_action_count >= HEMOM_MAX_ACTIONS) { break; }
                act = &hemom_actions[hemom_action_count];
                memset(act, 0, sizeof(*act));
                act->type = act_KEY;
                act->character = *c;
                act->packed_key = Pack_Key_Char(*c);
                hemom_action_count++;
            }
            /* Skip the default hemom_action_count++ below — we already did it. */
            continue;
        }
        else
        {
            fprintf(stderr, "[HeMoM Player] %s:%d: unknown action: %s\n", filepath, line_num, p);
            continue;
        }
    }

    fclose(fp);
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

    /* Wall-clock wait: idle until Platform_Get_Millies() reaches the target. */
    if (hemom_wait_active)
    {
        if (Platform_Get_Millies() < hemom_wait_until_ms)
        {
            return;
        }
        hemom_wait_active = 0;
    }

    act = &hemom_actions[hemom_action_index];

    {
        unsigned long long t_now = (unsigned long long)Platform_Get_Millies();

        switch (act->type)
        {
            case act_WAIT:
            {
                hemom_wait_until_ms = Platform_Get_Millies() + act->wait_ms;
                hemom_wait_active = 1;
                hemom_action_index++;
                fprintf(stderr, "[HeMoM Player] t=%llu ms wait %llu ms\n", t_now, (unsigned long long)act->wait_ms);
#ifdef STU_DEBUG
                dbg_prn("[HeMoM Player] t=%llu ms wait %llu ms\n", t_now, (unsigned long long)act->wait_ms);
                trc_prn("[HeMoM Player] t=%llu ms wait %llu ms\n", t_now, (unsigned long long)act->wait_ms);
#endif
            } break;

            case act_KEY:
            {
                mox_key = (int)(act->packed_key & 0xFF);
                mox_character = (char)((act->packed_key >> 8) & 0xFF);
                mox_mod = act->packed_key & 0xFFFF0000u;
                Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
                hemom_action_index++;
                fprintf(stderr, "[HeMoM Player] t=%llu ms key '%c' (packed=0x%08X)\n", t_now, act->character, act->packed_key);
#ifdef STU_DEBUG
                dbg_prn("[HeMoM Player] t=%llu ms key '%c' (packed=0x%08X)\n", t_now, act->character, act->packed_key);
                trc_prn("[HeMoM Player] t=%llu ms key '%c' (packed=0x%08X)\n", t_now, act->character, act->packed_key);
#endif
            } break;

            case act_ESCAPE:
            {
                mox_key = (int)(act->packed_key & 0xFF);
                mox_character = (char)((act->packed_key >> 8) & 0xFF);
                mox_mod = act->packed_key & 0xFFFF0000u;
                Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
                hemom_action_index++;
                fprintf(stderr, "[HeMoM Player] t=%llu ms escape\n", t_now);
#ifdef STU_DEBUG
                dbg_prn("[HeMoM Player] t=%llu ms escape\n", t_now);
                trc_prn("[HeMoM Player] t=%llu ms escape\n", t_now);
#endif
            } break;

            case act_ENTER:
            {
                mox_key = (int)(act->packed_key & 0xFF);
                mox_character = (char)((act->packed_key >> 8) & 0xFF);
                mox_mod = act->packed_key & 0xFFFF0000u;
                Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
                hemom_action_index++;
                fprintf(stderr, "[HeMoM Player] t=%llu ms enter\n", t_now);
#ifdef STU_DEBUG
                dbg_prn("[HeMoM Player] t=%llu ms enter\n", t_now);
                trc_prn("[HeMoM Player] t=%llu ms enter\n", t_now);
#endif
            } break;

            case act_BACKSPACE:
            {
                mox_key = (int)(act->packed_key & 0xFF);
                mox_character = (char)((act->packed_key >> 8) & 0xFF);
                mox_mod = act->packed_key & 0xFFFF0000u;
                Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
                hemom_action_index++;
                fprintf(stderr, "[HeMoM Player] t=%llu ms backspace\n", t_now);
#ifdef STU_DEBUG
                dbg_prn("[HeMoM Player] t=%llu ms backspace\n", t_now);
                trc_prn("[HeMoM Player] t=%llu ms backspace\n", t_now);
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
                    fprintf(stderr, "[HeMoM Player] t=%llu ms click (%d, %d)\n", t_now, act->x, act->y);
#ifdef STU_DEBUG
                    dbg_prn("[HeMoM Player] t=%llu ms click (%d, %d)\n", t_now, act->x, act->y);
                    trc_prn("[HeMoM Player] t=%llu ms click (%d, %d)\n", t_now, act->x, act->y);
#endif
                }
                else
                {
                    platform_frame_mouse_buttons |= ST_RIGHT_BUTTON;
                    User_Mouse_Handler(ST_RIGHT_BUTTON, wx, wy);
                    fprintf(stderr, "[HeMoM Player] t=%llu ms rclick (%d, %d)\n", t_now, act->x, act->y);
#ifdef STU_DEBUG
                    dbg_prn("[HeMoM Player] t=%llu ms rclick (%d, %d)\n", t_now, act->x, act->y);
                    trc_prn("[HeMoM Player] t=%llu ms rclick (%d, %d)\n", t_now, act->x, act->y);
#endif
                }
                hemom_action_index++;
            } break;

            case act_QUIT:
            {
                quit_game_flag = ST_TRUE;
                hemom_action_index++;
                fprintf(stderr, "[HeMoM Player] t=%llu ms quit\n", t_now);
#ifdef STU_DEBUG
                dbg_prn("[HeMoM Player] t=%llu ms quit\n", t_now);
                trc_prn("[HeMoM Player] t=%llu ms quit\n", t_now);
#endif
            } break;

            case act_END:
            {
                hemom_active = 0;
                fprintf(stderr, "[HeMoM Player] t=%llu ms end — synthetic player stopped\n", t_now);
#ifdef STU_DEBUG
                dbg_prn("[HeMoM Player] t=%llu ms end — synthetic player stopped\n", t_now);
                trc_prn("[HeMoM Player] t=%llu ms end — synthetic player stopped\n", t_now);
#endif
            } break;
        }
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
    hemom_wait_until_ms = 0;
    hemom_wait_active = 0;
}
