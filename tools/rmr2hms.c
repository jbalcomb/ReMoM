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
 *       click X Y           (mouse button press, edge-triggered)
 *       backspace / enter / escape   (special key codes)
 *       key K               (single non-printable key)
 *       type "STRING"       (consecutive printable-character keys grouped)
 *   - Inserts `wait Nms` between actions based on inter-event timestamp gaps
 *
 * Limitations:
 *   - Right-click and middle-click not yet emitted (only mouse_buttons & 1)
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
    if (pending_type_len > 0)
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
    if (last_event_ms == 0) { last_event_ms = now_ms; return; }
    gap = now_ms - last_event_ms;
    /* Suppress trivial gaps (<= 1 frame at 18 Hz) — they're just frame jitter. */
    if (gap >= 60)
    {
        fprintf(out, "wait %llums\n", (unsigned long long)gap);
    }
    last_event_ms = now_ms;
}



/* ========================================================================= */
/*  Action emission                                                          */
/* ========================================================================= */

static void Emit_Click(FILE *out, uint64_t now_ms, int x, int y)
{
    Flush_Pending_Type(out);
    Emit_Wait(out, now_ms);
    fprintf(out, "click %d %d\n", x, y);
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
    if (key_char >= 0x20 && key_char <= 0x7e)
    {
        if (pending_type_len == 0)
        {
            /* Insert the wait before the first character of a new run. */
            Emit_Wait(out, now_ms);
            pending_type_start_ms = now_ms;
        }
        if (pending_type_len < (int)sizeof(pending_type) - 1)
        {
            pending_type[pending_type_len++] = (char)key_char;
        }
        return;
    }

    /* Anything else: emit as a single `key` action.  We use the character byte
       if printable, else the raw key code as a hex literal in a comment. */
    Flush_Pending_Type(out);
    Emit_Wait(out, now_ms);
    if (key_char >= 0x20 && key_char <= 0x7e)
    {
        fprintf(out, "key %c\n", (char)key_char);
    }
    else
    {
        fprintf(out, "# unmapped key: code=%d char=0x%02x packed=0x%08x\n", key_code, key_char, packed);
    }
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
    fprintf(stderr, "Usage: %s <input.RMR> [output.hms]\n", program_name);
    fprintf(stderr, "\n");
    fprintf(stderr, "Translates a .RMR replay recording into a .hms scenario script.\n");
    fprintf(stderr, "If output.hms is omitted, the output file is named after the input with\n");
    fprintf(stderr, "'.RMR' replaced by '.hms' (or '.hms' appended if no '.RMR' suffix).\n");
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

    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        Print_Usage(argv[0]);
        return 1;
    }

    input_path = argv[1];

    if (argc == 3)
    {
        output_path = argv[2];
    }
    else
    {
        size_t len = strlen(input_path);
        if (len >= sizeof(derived_path) - 5)
        {
            fprintf(stderr, "rmr2hms: input path too long\n");
            return 1;
        }
        memcpy(derived_path, input_path, len + 1);
        if (len >= 4
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
    if (fin == NULL)
    {
        fprintf(stderr, "rmr2hms: cannot open input '%s'\n", input_path);
        return 2;
    }

    fout = fopen(output_path, "w");
    if (fout == NULL)
    {
        fprintf(stderr, "rmr2hms: cannot open output '%s'\n", output_path);
        fclose(fin);
        return 2;
    }

    fprintf(fout, "# Auto-generated by rmr2hms from %s\n", input_path);
    fprintf(fout, "# Review for correctness before using as a canonical scenario.\n");
    fprintf(fout, "\n");

    while (fgets(line, sizeof(line), fin) != NULL)
    {
        uint32_t idx;
        uint64_t timestamp_ms, delta_ms;
        int mouse_x, mouse_y, mouse_buttons, key_pressed, key_count;
        uint32_t key0;

        /* Skip header / comment lines. */
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || line[0] == '\0')
        {
            continue;
        }

        if (!Parse_Frame(line, &idx, &timestamp_ms, &delta_ms,
                         &mouse_x, &mouse_y, &mouse_buttons, &key_pressed, &key_count, &key0))
        {
            continue;
        }

        frame_count_total++;

        /* Mouse-button edge: 0 -> nonzero is a click. */
        if (mouse_buttons != 0 && prev_mouse_buttons == 0)
        {
            Emit_Click(fout, timestamp_ms, mouse_x, mouse_y);
            actions_emitted++;
        }

        /* Key event: emitted when key_pressed transitions 0->1, or when held
           and the key0 value changes (next key in the buffer). */
        if (key_pressed == 1 && key_count > 0)
        {
            int is_new_event = (prev_key_pressed == 0) || (key0 != prev_key0);
            if (is_new_event)
            {
                Emit_Key(fout, timestamp_ms, key0);
                actions_emitted++;
            }
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
