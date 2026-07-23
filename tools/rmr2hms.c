/**
 * rmr2hms.c — Translate a .RMR replay recording into a .hms scenario script.
 *
 * Usage:
 *   rmr2hms <input.RMR> [output.hms]
 *
 * If output.hms is omitted, the output file is named after the input with
 * `.RMR` replaced by `.hms` (or `.hms` appended if no `.RMR` suffix).
 *
 * What it does:
 *   - Reads the per-frame CSV rows from the .RMR
 *   - Drops idle frames (no input state change)
 *   - Emits one HMS action per state-change event:
 *       click X Y / rclick X Y  (left/right mouse button press, edge-triggered)
 *       backspace / enter / escape   (special key codes)
 *       key K               (single non-printable key)
 *       type "STRING"       (consecutive printable-character keys grouped)
 *   - Inserts `wait Nms` between actions based on inter-event timestamp gaps
 *
 * Limitations:
 *   - Middle-click not yet emitted (only left mouse_buttons & 1 and right & 2)
 *   - Modifier keys not preserved on individual `key` actions
 *   - Wait granularity is exact ms; user may want to round/coarsen by hand
 *   - Mouse coordinates are emitted as-recorded (already in 320x200 game units)
 *
 * The output is meant as a starting point; review and tighten before using as
 * a canonical scenario.
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>



/* ========================================================================= */
/*  Key code helpers (mirror values from platform/include/Platform_Keys.h)   */
/* ========================================================================= */

#define MOX_KEY_LEFT             1
#define MOX_KEY_RIGHT            2
#define MOX_KEY_UP               3
#define MOX_KEY_DOWN             4
#define MOX_KEY_RIGHTUP          5
#define MOX_KEY_RIGHTDOWN        6
#define MOX_KEY_LEFTUP           7
#define MOX_KEY_LEFTDOWN         8
#define MOX_KEY_BACKSPACE       11
#define MOX_KEY_ENTER           12
#define MOX_KEY_TAB             13
#define MOX_KEY_ESCAPE          27
#define MOX_KEY_SPACE           32

#define KEY_CODE(packed)    ((int)((packed) & 0xff))
#define KEY_CHAR(packed)    ((int)(((packed) >> 8) & 0xff))



/* ========================================================================= */
/*  Pending Action State                                                     */
/* ========================================================================= */

/* The translator buffers a "type" string in progress so it can group runs of
   printable-character key events into a single `type "STR"` action. */

static char   pending_type[256];
static int    pending_type_len = 0;
static uint64_t pending_type_start_ms = 0;

static void Flush_Pending_Type(FILE *out)
{
    if(pending_type_len > 0)
    {
        pending_type[pending_type_len] = '\0';
        fprintf(out, "type %s\n", pending_type);
        pending_type_len = 0;
    }
}



/* ========================================================================= */
/*  Wait emission                                                            */
/* ========================================================================= */

static uint64_t last_event_ms = 0;

static void Emit_Wait(FILE *out, uint64_t now_ms)
{
    uint64_t gap;
    if(last_event_ms == 0) { last_event_ms = now_ms; return; }
    gap = now_ms - last_event_ms;
    /* Suppress trivial gaps (<= 1 frame at 18 Hz) — they're just frame jitter. */
    if(gap >= 60)
    {
        fprintf(out, "wait %llums\n", (unsigned long long)gap);
    }
    last_event_ms = now_ms;
}



/* ========================================================================= */
/*  Action emission                                                          */
/* ========================================================================= */

static void Emit_Click(FILE *out, uint64_t now_ms, int x, int y, int buttons, const char *name)
{
    /* RMR mouse_buttons bitmask: 1 = ST_LEFT_BUTTON, 2 = ST_RIGHT_BUTTON.
       Prefer right when the right bit is set so a right-click round-trips to
       the HMS `rclick` verb instead of collapsing to a left `click`. */
    const char *verb = (buttons & 2) ? "rclick" : "click";
    Flush_Pending_Type(out);
    Emit_Wait(out, now_ms);
    if(name != NULL)
    {
        /* Named action (Screen.Alias) resolved from the field the click landed on;
           the recorded coordinates are kept in a comment for reference. */
        fprintf(out, "%s %s   # was (%d, %d)\n", verb, name, x, y);
    }
    else
    {
        fprintf(out, "%s %d %d\n", verb, x, y);
    }
}

static void Emit_Key(FILE *out, uint64_t now_ms, uint32_t packed)
{
    int key_code = KEY_CODE(packed);
    int key_char = KEY_CHAR(packed);

    /* Special keys → keyword actions. */
    switch (key_code)
    {
        case MOX_KEY_BACKSPACE:
            Flush_Pending_Type(out);
            Emit_Wait(out, now_ms);
            fprintf(out, "backspace\n");
            return;
        case MOX_KEY_ENTER:
            Flush_Pending_Type(out);
            Emit_Wait(out, now_ms);
            fprintf(out, "enter\n");
            return;
        case MOX_KEY_ESCAPE:
            Flush_Pending_Type(out);
            Emit_Wait(out, now_ms);
            fprintf(out, "escape\n");
            return;
        /* Direction / numpad keys — move unit stack. */
        case MOX_KEY_LEFT:      Flush_Pending_Type(out); Emit_Wait(out, now_ms); fprintf(out, "left\n");      return;
        case MOX_KEY_RIGHT:     Flush_Pending_Type(out); Emit_Wait(out, now_ms); fprintf(out, "right\n");     return;
        case MOX_KEY_UP:        Flush_Pending_Type(out); Emit_Wait(out, now_ms); fprintf(out, "up\n");        return;
        case MOX_KEY_DOWN:      Flush_Pending_Type(out); Emit_Wait(out, now_ms); fprintf(out, "down\n");      return;
        case MOX_KEY_RIGHTUP:   Flush_Pending_Type(out); Emit_Wait(out, now_ms); fprintf(out, "upright\n");   return;
        case MOX_KEY_RIGHTDOWN: Flush_Pending_Type(out); Emit_Wait(out, now_ms); fprintf(out, "downright\n"); return;
        case MOX_KEY_LEFTUP:    Flush_Pending_Type(out); Emit_Wait(out, now_ms); fprintf(out, "upleft\n");    return;
        case MOX_KEY_LEFTDOWN:  Flush_Pending_Type(out); Emit_Wait(out, now_ms); fprintf(out, "downleft\n");  return;
        default:
            break;
    }

    /* Printable character → group into a `type` run. */
    if(key_char >= 0x20 && key_char <= 0x7e)
    {
        if(pending_type_len == 0)
        {
            /* Insert the wait before the first character of a new run. */
            Emit_Wait(out, now_ms);
            pending_type_start_ms = now_ms;
        }
        if(pending_type_len < (int)sizeof(pending_type) - 1)
        {
            pending_type[pending_type_len++] = (char)key_char;
        }
        return;
    }

    /* Anything else: emit as a single `key` action.  We use the character byte
       if printable, else the raw key code as a hex literal in a comment. */
    Flush_Pending_Type(out);
    Emit_Wait(out, now_ms);
    if(key_char >= 0x20 && key_char <= 0x7e)
    {
        fprintf(out, "key %c\n", (char)key_char);
    }
    else
    {
        fprintf(out, "# unmapped key: code=%d char=0x%02x packed=0x%08x\n", key_code, key_char, packed);
    }
}



/* ========================================================================= */
/*  Named-action resolution                                                  */
/*                                                                           */
/*  Two flat tables, both dependency-free:                                   */
/*    - the alias lookup (basename:line -> Screen.Alias), baked by           */
/*      `python -m tools.field_catalog.resolver export`                      */
/*    - the RECORD.log's per-frame field origin (idx -> basename:line),      */
/*      emitted by the engine's replay field-hit log (Phase 2)              */
/*  A click at frame `idx` -> origin -> alias -> `click Screen.Alias`.       */
/* ========================================================================= */

struct Alias_Entry { char origin[64]; char name[96]; };
static struct Alias_Entry g_alias[1024];
static int g_alias_count = 0;

struct Origin_Entry { int idx; char origin[64]; };
static struct Origin_Entry g_origin[8192];
static int g_origin_count = 0;

static const char *Lookup_Alias(const char *origin)
{
    int i;
    for(i = 0; i < g_alias_count; i++)
    {
        if(strcmp(g_alias[i].origin, origin) == 0) { return g_alias[i].name; }
    }
    return NULL;
}

static const char *Origin_For_Idx(int idx)
{
    int i;
    for(i = 0; i < g_origin_count; i++)
    {
        if(g_origin[i].idx == idx) { return g_origin[i].origin; }
    }
    return NULL;
}

/* Load the baked alias lookup. Silent no-op if the file is absent (naming off). */
static void Load_Alias_Lookup(const char *path)
{
    FILE *f = fopen(path, "r");
    char ln[256];
    if(f == NULL) { return; }
    while(fgets(ln, sizeof(ln), f) != NULL && g_alias_count < (int)(sizeof(g_alias) / sizeof(g_alias[0])))
    {
        char origin[64];
        char name[96];
        if(ln[0] == '#') { continue; }
        if(sscanf(ln, "%63s %95s", origin, name) != 2) { continue; }
        if(strchr(origin, ':') == NULL) { continue; }  /* skip the "origin name" header row */
        memcpy(g_alias[g_alias_count].origin, origin, strlen(origin) + 1);
        memcpy(g_alias[g_alias_count].name, name, strlen(name) + 1);
        g_alias_count++;
    }
    fclose(f);
}

/* Load the RECORD.log sibling of the .RMR (idx -> field origin). Silent no-op if absent. */
static void Load_Record_Origins(const char *rmr_path)
{
    char log_path[1024];
    size_t len = strlen(rmr_path);
    FILE *f;
    char ln[512];
    if(len + 12 >= sizeof(log_path)) { return; }
    memcpy(log_path, rmr_path, len + 1);
    if(len >= 4 && log_path[len - 4] == '.'
        && (log_path[len - 3] == 'R' || log_path[len - 3] == 'r')
        && (log_path[len - 2] == 'M' || log_path[len - 2] == 'm')
        && (log_path[len - 1] == 'R' || log_path[len - 1] == 'r'))
    {
        len -= 4;  /* drop ".RMR" */
    }
    memcpy(log_path + len, "-RECORD.log", 12);  /* 11 chars + NUL */
    f = fopen(log_path, "r");
    if(f == NULL) { return; }
    while(fgets(ln, sizeof(ln), f) != NULL && g_origin_count < (int)(sizeof(g_origin) / sizeof(g_origin[0])))
    {
        char *idx_p = strstr(ln, "idx=");
        char *at = strrchr(ln, '@');
        int idx;
        int n = 0;
        char *p;
        if(idx_p == NULL || at == NULL) { continue; }
        if(sscanf(idx_p + 4, "%d", &idx) != 1) { continue; }
        p = at + 1;
        while(*p != '\0' && *p != '\n' && *p != '\r' && *p != ' ' && n < 63)
        {
            g_origin[g_origin_count].origin[n++] = *p++;
        }
        if(n == 0) { continue; }
        g_origin[g_origin_count].origin[n] = '\0';
        g_origin[g_origin_count].idx = idx;
        g_origin_count++;
    }
    fclose(f);
}



/* ========================================================================= */
/*  Frame Parsing                                                            */
/* ========================================================================= */

/* Parse one CSV data row.  Format:
       idx,timestamp_ms,delta_ms,mouse_x,mouse_y,mouse_buttons,key_pressed,key_count[,key0]
   Returns 1 on success, 0 on parse error. */
static int Parse_Frame(const char *line,
                       uint32_t *idx,
                       uint64_t *timestamp_ms,
                       uint64_t *delta_ms,
                       int *mouse_x, int *mouse_y,
                       int *mouse_buttons,
                       int *key_pressed,
                       int *key_count,
                       uint32_t *key0)
{
    int n;
    *key0 = 0;
    n = sscanf(line, "%u,%llu,%llu,%d,%d,%d,%d,%d,%u",
               idx, (unsigned long long *)timestamp_ms, (unsigned long long *)delta_ms,
               mouse_x, mouse_y, mouse_buttons, key_pressed, key_count, key0);
    return (n >= 8);
}



/* ========================================================================= */
/*  Main                                                                     */
/* ========================================================================= */

static void Print_Usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s <input.RMR> [output.hms] [--lookup alias_lookup.fwv]\n", program_name);
    fprintf(stderr, "\n");
    fprintf(stderr, "Translates a .RMR replay recording into a .hms scenario script.\n");
    fprintf(stderr, "If output.hms is omitted, the output file is named after the input with\n");
    fprintf(stderr, "'.RMR' replaced by '.hms' (or '.hms' appended if no '.RMR' suffix).\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "If the input's sibling <name>-RECORD.log and an alias lookup are present,\n");
    fprintf(stderr, "clicks are emitted as named actions (click Screen.Alias) instead of raw\n");
    fprintf(stderr, "coordinates. --lookup defaults to tools/fields/alias_lookup.fwv.\n");
}

int main(int argc, char *argv[])
{
    const char *input_path;
    const char *output_path;
    char        derived_path[1024];
    FILE       *fin;
    FILE       *fout;
    char        line[512];
    int         frame_count_total = 0;
    int         actions_emitted = 0;

    /* Per-frame state we track to detect transitions. */
    int prev_mouse_buttons = 0;
    int prev_key_pressed = 0;
    uint32_t prev_key0 = 0;

    const char *lookup_path = NULL;
    const char *positional[2] = { NULL, NULL };
    int np = 0;
    int ai;

    for(ai = 1; ai < argc; ai++)
    {
        if(strcmp(argv[ai], "--help") == 0 || strcmp(argv[ai], "-h") == 0)
        {
            Print_Usage(argv[0]);
            return 1;
        }
        if(strcmp(argv[ai], "--lookup") == 0)
        {
            if(ai + 1 >= argc) { fprintf(stderr, "rmr2hms: --lookup needs a path\n"); return 1; }
            lookup_path = argv[++ai];
            continue;
        }
        if(np < 2) { positional[np++] = argv[ai]; }
        else { fprintf(stderr, "rmr2hms: too many arguments\n"); return 1; }
    }

    if(np < 1)
    {
        Print_Usage(argv[0]);
        return 1;
    }

    input_path = positional[0];

    /* Named-action tables (both optional; naming is skipped if either is absent).
       Default lookup path is relative to the repo root (the usual CWD). */
    Load_Alias_Lookup(lookup_path != NULL ? lookup_path : "tools/fields/alias_lookup.fwv");
    Load_Record_Origins(input_path);

    if(positional[1] != NULL)
    {
        output_path = positional[1];
    }
    else
    {
        size_t len = strlen(input_path);
        if(len >= sizeof(derived_path) - 5)
        {
            fprintf(stderr, "rmr2hms: input path too long\n");
            return 1;
        }
        memcpy(derived_path, input_path, len + 1);
        if(len >= 4
            && (derived_path[len - 4] == '.')
            && (derived_path[len - 3] == 'R' || derived_path[len - 3] == 'r')
            && (derived_path[len - 2] == 'M' || derived_path[len - 2] == 'm')
            && (derived_path[len - 1] == 'R' || derived_path[len - 1] == 'r'))
        {
            derived_path[len - 3] = 'h';
            derived_path[len - 2] = 'm';
            derived_path[len - 1] = 's';
        }
        else
        {
            memcpy(derived_path + len, ".hms", 5);
        }
        output_path = derived_path;
    }

    fin = fopen(input_path, "r");
    if(fin == NULL)
    {
        fprintf(stderr, "rmr2hms: cannot open input '%s'\n", input_path);
        return 2;
    }

    fout = fopen(output_path, "w");
    if(fout == NULL)
    {
        fprintf(stderr, "rmr2hms: cannot open output '%s'\n", output_path);
        fclose(fin);
        return 2;
    }

    fprintf(fout, "# Auto-generated by rmr2hms from %s\n", input_path);
    fprintf(fout, "# Review for correctness before using as a canonical scenario.\n");
    if(g_alias_count > 0 && g_origin_count > 0)
    {
        fprintf(fout, "# Named clicks (click Screen.Alias) resolved from the RECORD.log field origins.\n");
        fprintf(fout, "# Named clicks require HMS parser support for Screen.Alias to execute.\n");
    }
    fprintf(fout, "\n");

    while(fgets(line, sizeof(line), fin) != NULL)
    {
        uint32_t idx;
        uint64_t timestamp_ms, delta_ms;
        int mouse_x, mouse_y, mouse_buttons, key_pressed, key_count;
        uint32_t key0;

        /* Skip header / comment lines. */
        if(line[0] == '#' || line[0] == '\n' || line[0] == '\r' || line[0] == '\0')
        {
            continue;
        }

        if(!Parse_Frame(line, &idx, &timestamp_ms, &delta_ms,
                         &mouse_x, &mouse_y, &mouse_buttons, &key_pressed, &key_count, &key0))
        {
            continue;
        }

        frame_count_total++;

        /* Mouse-button edge: 0 -> nonzero is a click. */
        if(mouse_buttons != 0 && prev_mouse_buttons == 0)
        {
            /* Resolve the clicked field to a Screen.Alias name via the RECORD.log
               origin for this frame; NULL falls back to bare `click X Y`. */
            const char *origin = Origin_For_Idx((int)idx);
            const char *name = (origin != NULL) ? Lookup_Alias(origin) : NULL;
            Emit_Click(fout, timestamp_ms, mouse_x, mouse_y, mouse_buttons, name);
            actions_emitted++;
        }

        /* Key event: emitted when key_pressed transitions 0->1, or when held
           and the key0 value changes (next key in the buffer). */
        if(key_pressed == 1 && key_count > 0)
        {
            int is_new_event = (prev_key_pressed == 0) || (key0 != prev_key0);
            if(is_new_event)
            {
                Emit_Key(fout, timestamp_ms, key0);
                actions_emitted++;
            }
        }

        /* Key release (1 -> 0) ends a discrete keypress: flush any pending
           `type` run so distinct hotkey presses (e.g. 'n' then 'a') become
           separate `type n` / `type a` actions instead of merging into
           `type na`.  A genuinely typed string arrives buffered — key_pressed
           stays 1 while key0 cycles — so it accumulates without a release and
           is unaffected. */
        if(key_pressed == 0 && prev_key_pressed == 1)
        {
            Flush_Pending_Type(fout);
        }

        prev_mouse_buttons = mouse_buttons;
        prev_key_pressed = key_pressed;
        prev_key0 = key0;
    }

    /* Flush any trailing typed text. */
    Flush_Pending_Type(fout);

    fprintf(fout, "\n");
    fprintf(fout, "# end of recording — %d frames scanned, %d actions emitted\n", frame_count_total, actions_emitted);

    fclose(fout);
    fclose(fin);

    fprintf(stdout, "rmr2hms: %d frames scanned, %d actions emitted -> %s\n", frame_count_total, actions_emitted, output_path);
    return 0;
}
