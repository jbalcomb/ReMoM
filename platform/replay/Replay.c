/**
 * Replay.c — Record & Replay implementation.
 *
 * Records engine-visible input state at the Platform_Event_Handler() boundary.
 * On replay, injects recorded frames instead of polling OS events.
 *
 * File format (.RMR) — human-readable text:
 *   Line 1:   # ReMoM Replay v2
 *   Line 2:   # random_seed=<seed>
 *   Line 3:   # frame_count=<count>  (patched on stop; 0 while recording)
 *   Line 4:   # idx,timestamp_ms,delta_ms,mouse_x,mouse_y,mouse_buttons,key_pressed,key_count,keys...
 *   Line 5+:  CSV data rows
 */

#include "../../platform/include/Platform.h"
#include "../../platform/include/Platform_Keys.h"
#include "../../platform/include/Platform_Replay.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/random.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



/* ========================================================================= */
/*  State                                                                    */
/* ========================================================================= */

static FILE *replay_file = NULL;
static FILE *record_log  = NULL;
static FILE *replay_log  = NULL;
static int   recording   = 0;
static int   replaying   = 0;

static uint32_t record_frame_count = 0;
static uint32_t replay_frame_count = 0;
static uint32_t replay_frame_index = 0;

static uint64_t replay_frame_number = 0;
static uint64_t replay_start_ticks = 0;
static int      replay_first_inject = 0;  /* 1 after first inject resets start_ticks */
static uint64_t record_start_ticks = 0;
static uint64_t record_prev_timestamp = 0;
static int      replay_recorded_screen_scale = 1;
static char     replay_rmr_filepath[512] = {0};  /* Saved for log comparison at stop. */

/* Peek buffer for timestamp-based injection. */
static Input_Frame peek_frame;
static int         peek_valid = 0;

/* Collapsed log state — tracks runs of identical input for compact logging.
   Used for both recording (rec_log) and playback (play_log). */
typedef struct Collapsed_Log_State
{
    int16_t  prev_mouse_x;
    int16_t  prev_mouse_y;
    int16_t  prev_mouse_buttons;
    int8_t   prev_key_pressed;
    uint32_t prev_key0;
    uint32_t run_start_idx;
    uint32_t run_count;
} Collapsed_Log_State;

static Collapsed_Log_State rec_log  = { -1, -1, -1, -1, 0, 0, 0 };
static Collapsed_Log_State play_log = { -1, -1, -1, -1, 0, 0, 0 };

/* Write detailed debug output to the appropriate log file, or stderr if not open. */
#define REC_LOG  (record_log != NULL ? record_log : stderr)
#define PLAY_LOG (replay_log != NULL ? replay_log : stderr)

/* Forward declarations — defined in the Internal section below. */
static void Log_Flush(Collapsed_Log_State *state, const char *prefix, FILE *logfile);
static void Log_Reset(Collapsed_Log_State *state);



/* Open a log file alongside the replay file.
   "FOO.RMR" -> "FOO.REC.log" for recording, "FOO.PLAY.log" for playback.
   Both use the same format so they can be diffed. */
static FILE* Replay_Open_Log(const char *filepath, const char *mode)
{
    char log_path[512];
    const char *dot = strrchr(filepath, '.');
    FILE *logfile;
    if(dot != NULL)
    {
        int prefix_len = (int)(dot - filepath);
        snprintf(log_path, sizeof(log_path), "%.*s-%s.log", prefix_len, filepath, mode);
    }
    else
    {
        snprintf(log_path, sizeof(log_path), "%s-%s.log", filepath, mode);
    }
    logfile = fopen(log_path, "w");
    if(logfile != NULL)
    {
        time_t now = time(NULL);
        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(logfile, "# ReMoM Replay Log\n");
        fprintf(logfile, "# mode=%s  file=%s\n", mode, filepath);
        fprintf(logfile, "# timestamp=%s\n", timebuf);
        fprintf(logfile, "#\n");
        fprintf(stderr, "REPLAY: log -> %s\n", log_path);
    }
    return logfile;
}

static void Replay_Close_Logs(void)
{
    if(record_log != NULL)
    {
        fclose(record_log);
        record_log = NULL;
    }
    if(replay_log != NULL)
    {
        fclose(replay_log);
        replay_log = NULL;
    }
}



/* ========================================================================= */
/*  Recording                                                                */
/* ========================================================================= */

int Platform_Record_Start(const char *filepath)
{
    if(recording || replaying)
    {
        return -1;
    }

    /* "w+b": read+write, binary mode.  Binary avoids Windows \r\n translation
       issues when seeking back to patch frame_count in Platform_Record_Stop(). */
    replay_file = fopen(filepath, "w+b");
    if(replay_file == NULL)
    {
        return -1;
    }

    /* Write header lines. frame_count=0 is a placeholder; patched on stop. */
    fprintf(replay_file, "# ReMoM Replay v2\n");
    fprintf(replay_file, "# random_seed=%u\n", (unsigned)Get_Random_Seed());
    fprintf(replay_file, "# screen_scale=%d\n", Platform_Get_Scale());
    fprintf(replay_file, "# frame_count=%-10u\n", 0u);
    fprintf(replay_file, "# idx,timestamp_ms,delta_ms,mouse_x,mouse_y,mouse_buttons,key_pressed,key_count,keys...\n");

    record_frame_count = 0;
    replay_frame_number = 0;
    record_start_ticks = Platform_Get_Millies();
    record_prev_timestamp = 0;
    recording = 1;

    /* Reset collapsed log state for fresh recording. */
    Log_Reset(&rec_log);

    record_log = Replay_Open_Log(filepath, "RECORD");

    fprintf(stderr, "REPLAY: recording started -> %s  (random_seed=%u)\n", filepath, (unsigned)Get_Random_Seed());

    return 0;
}

void Platform_Record_Stop(void)
{
    long count_pos;

    if(!recording || replay_file == NULL)
    {
        return;
    }

    fprintf(stderr, "REPLAY: recording stopped. %u frames captured.\n", record_frame_count);

    /* Flush the final accumulated log run before closing the log. */
    Log_Flush(&rec_log, "REC", REC_LOG);

    /* Patch the frame_count line.  The format "# frame_count=%-10u\n"
       left exactly 10 digits of space so we can overwrite in place. */
    /* Windows \r\n bug: ftell() counts raw bytes but fgets() translates \r\n to \n,
       so ftell()-strlen(line) lands at the wrong position.  Save ftell() BEFORE fgets(). */
    {
        char line[256];
        long line_start;
        fseek(replay_file, 0, SEEK_SET);
        while(1)
        {
            line_start = ftell(replay_file);
            if(fgets(line, sizeof(line), replay_file) == NULL)
            {
                break;
            }
            if(strncmp(line, "# frame_count=", 14) == 0)
            {
                fseek(replay_file, line_start, SEEK_SET);
                fprintf(replay_file, "# frame_count=%-10u\n", record_frame_count);
                break;
            }
        }
    }

    fclose(replay_file);
    replay_file = NULL;
    recording = 0;
    Replay_Close_Logs();
}

int Platform_Record_Active(void)
{
    return recording;
}



/* ========================================================================= */
/*  Replay                                                                   */
/* ========================================================================= */

int Platform_Replay_Start(const char *filepath)
{
    char line[256];
    unsigned random_seed = 0;
    unsigned recorded_screen_scale = 1;
    unsigned frame_count = 0;

    if(recording || replaying)
    {
        return -1;
    }

    /* Save filepath for log comparison at stop time. */
    snprintf(replay_rmr_filepath, sizeof(replay_rmr_filepath), "%s", filepath);

    /* Open in binary mode ("rb") so ftell/fseek work correctly on Windows.
       In text mode, \r\n translation causes ftell positions to be unreliable
       for fseek, which breaks the header-parsing rewind logic. */
    replay_file = fopen(filepath, "rb");
    if(replay_file == NULL)
    {
        fprintf(stderr, "REPLAY: failed to open %s\n", filepath);
        return -1;
    }

    /* Parse header lines.  Lines starting with '#' are header/comments.
       We read all '#' lines before the data, extracting known fields. */
    if(fgets(line, sizeof(line), replay_file) == NULL || strncmp(line, "# ReMoM Replay v2", 17) != 0)
    {
        fprintf(stderr, "REPLAY: bad magic in %s\n", filepath);
        fclose(replay_file);
        replay_file = NULL;
        return -1;
    }

    /* Read remaining header lines until we hit the first data line. */
    {
        long line_start;
        while(1)
        {
            line_start = ftell(replay_file);
            if(fgets(line, sizeof(line), replay_file) == NULL)
            {
                break;
            }
            if(line[0] != '#')
            {
                /* First data line — rewind to where it started. */
                fseek(replay_file, line_start, SEEK_SET);
                break;
            }
            sscanf(line, "# random_seed=%u", &random_seed);
            sscanf(line, "# screen_scale=%u", &recorded_screen_scale);
            sscanf(line, "# frame_count=%u", &frame_count);
        }
    }

    replay_recorded_screen_scale = (recorded_screen_scale > 0) ? (int)recorded_screen_scale : 1;

    replay_frame_count = frame_count;
    replay_frame_index = 0;
    replay_frame_number = 0;
    replay_start_ticks = Platform_Get_Millies();
    replay_first_inject = 0;

    /* Restore the RNG to the recorded state for deterministic replay. */
    Set_Random_Seed(random_seed);

    replaying = 1;

    replay_log = Replay_Open_Log(filepath, "REPLAY");
    Log_Reset(&play_log);

    fprintf(stderr, "REPLAY: playback started <- %s  (%u frames, random_seed=%u, recorded_screen_scale=%u, current_scale=%d)\n", filepath, frame_count, random_seed, recorded_screen_scale, Platform_Get_Scale());

    return 0;
}

void Platform_Replay_Stop(void)
{
    if(!replaying || replay_file == NULL)
    {
        return;
    }

    fprintf(stderr, "REPLAY: playback stopped at frame %u / %u\n", replay_frame_index, replay_frame_count);

    Log_Flush(&play_log, "PLAY", PLAY_LOG);

    fclose(replay_file);
    replay_file = NULL;
    replaying = 0;
    replay_frame_count = 0;
    replay_frame_index = 0;
    peek_valid = 0;
    Replay_Close_Logs();

    /* Auto-compare recording vs replay logs if a RECORD log exists. */
    if(replay_rmr_filepath[0] != '\0')
    {
        Platform_Replay_Compare_Logs(replay_rmr_filepath);
        replay_rmr_filepath[0] = '\0';
    }
}

int Platform_Replay_Active(void)
{
    return replaying;
}



/* ========================================================================= */
/*  Demo Mode                                                                */
/* ========================================================================= */

static uint64_t demo_idle_start = 0;

int Platform_Demo_Start(void)
{
    FILE *test = fopen("DEMO.RMR", "r");
    if(test == NULL)
    {
        return -1;
    }
    fclose(test);
    return Platform_Replay_Start("DEMO.RMR");
}

void Platform_Demo_Reset_Idle_Timer(void)
{
    demo_idle_start = Platform_Get_Millies();
}

int Platform_Demo_Idle_Expired(uint64_t timeout_ms)
{
    if(demo_idle_start == 0)
    {
        demo_idle_start = Platform_Get_Millies();
        return 0;
    }
    return (Platform_Get_Millies() - demo_idle_start) >= timeout_ms;
}



/* ========================================================================= */
/*  Internal — called by Platform_Event_Handler()                            */
/* ========================================================================= */

static void Log_Flush(Collapsed_Log_State *state, const char *prefix, FILE *logfile)
{
    if(state->run_count == 0)
    {
        return;
    }
    if(state->run_count == 1)
    {
        fprintf(logfile, "%s  idx=%-6u  mouse=(%3d,%3d) btn=%d  key=%d  (1 frame)\n", prefix, state->run_start_idx, (int)state->prev_mouse_x, (int)state->prev_mouse_y, (int)state->prev_mouse_buttons, (int)state->prev_key_pressed);
    }
    else
    {
        fprintf(logfile, "%s  idx=%-6u..%-6u  mouse=(%3d,%3d) btn=%d  key=%d  (%u frames, no change)\n", prefix, state->run_start_idx, state->run_start_idx + state->run_count - 1, (int)state->prev_mouse_x, (int)state->prev_mouse_y, (int)state->prev_mouse_buttons, (int)state->prev_key_pressed, state->run_count);
    }
    state->run_count = 0;
}

static void Log_Reset(Collapsed_Log_State *state)
{
    state->prev_mouse_x = -1;
    state->prev_mouse_y = -1;
    state->prev_mouse_buttons = -1;
    state->prev_key_pressed = -1;
    state->prev_key0 = 0;
    state->run_start_idx = 0;
    state->run_count = 0;
}

void Replay_Capture_Frame(void)
{
    uint64_t timestamp;
    uint64_t delta;
    int key_count;
    uint32_t key0;
    int state_changed;

    if(!recording || replay_file == NULL)
    {
        return;
    }

    timestamp = Platform_Get_Millies() - record_start_ticks;
    delta = timestamp - record_prev_timestamp;
    record_prev_timestamp = timestamp;

    /* Read from the kilgore_key keyboard buffer, not scan_code_char_code
       (which is only set by the Win32 driver, never by SDL3). */
    key_count = Platform_Keyboard_Buffer_Pending_Count();
    key0 = (key_count > 0) ? Platform_Keyboard_Buffer_Peek_Latest() : 0;
    if(key_count > 1) { key_count = 1; }  /* Recording format supports 1 key per frame for now. */

    fprintf(replay_file, "%u,%llu,%llu,%d,%d,%d,%d,%d",
        record_frame_count,
        (unsigned long long)timestamp,
        (unsigned long long)delta,
        (int)pointer_x,
        (int)pointer_y,
        (int)platform_frame_mouse_buttons,
        (int)key_pressed,
        key_count);

    if(key_count > 0)
    {
        fprintf(replay_file, ",%u", (unsigned)key0);
    }
    fprintf(replay_file, "\n");

    /* Collapsed logging: flush previous run when state changes, then accumulate. */
    state_changed = (pointer_x != rec_log.prev_mouse_x) || (pointer_y != rec_log.prev_mouse_y) || (platform_frame_mouse_buttons != rec_log.prev_mouse_buttons) || (key_pressed != rec_log.prev_key_pressed) || (key0 != rec_log.prev_key0);

    if(state_changed)
    {
        Log_Flush(&rec_log, "REC", REC_LOG);

        /* Log the new state with full detail. */
        fprintf(REC_LOG, "idx=%-6u t=%-8llu  mouse=(%3d,%3d) btn=%d  key=%d sccc=0x%04X  dt=%llu",
            record_frame_count,
            (unsigned long long)timestamp,
            (int)pointer_x, (int)pointer_y,
            (int)platform_frame_mouse_buttons,
            (int)key_pressed,
            (unsigned)key0,
            (unsigned long long)delta);

        /* Show which field the mouse is over, if any. */
        {
            int fx = pointer_x;
            int fy = pointer_y;
            int fi;
            for(fi = 0; fi < fields_count; fi++)
            {
                if(fx >= p_fields[fi].x1 && fx <= p_fields[fi].x2 && fy >= p_fields[fi].y1 && fy <= p_fields[fi].y2)
                {
                    fprintf(REC_LOG, "  field[%d]=(%d,%d)-(%d,%d)", fi, p_fields[fi].x1, p_fields[fi].y1, p_fields[fi].x2, p_fields[fi].y2);
                    break;
                }
            }
        }

        fprintf(REC_LOG, "\n");

        rec_log.prev_mouse_x = pointer_x;
        rec_log.prev_mouse_y = pointer_y;
        rec_log.prev_mouse_buttons = platform_frame_mouse_buttons;
        rec_log.prev_key_pressed = key_pressed;
        rec_log.prev_key0 = key0;
        rec_log.run_start_idx = record_frame_count;
        rec_log.run_count = 1;
    }
    else
    {
        if(rec_log.run_count == 0)
        {
            rec_log.run_start_idx = record_frame_count;
        }
        rec_log.run_count++;
    }

    record_frame_count++;
    replay_frame_number++;
}


/* Parse the next CSV line into the peek buffer. */
static int Replay_Peek_Frame(void)
{
    char line[512];
    unsigned idx, key_count, key0;
    unsigned long long timestamp, delta;
    int mx, my, btn, kp;

    if(peek_valid)
    {
        return 1;
    }

    if(replay_frame_index >= replay_frame_count)
    {
        fprintf(PLAY_LOG, "PEEK FAIL: frame_index=%u >= frame_count=%u\n", replay_frame_index, replay_frame_count);
        return 0;
    }

    if(fgets(line, sizeof(line), replay_file) == NULL)
    {
        fprintf(PLAY_LOG, "PEEK FAIL: fgets returned NULL (frame_index=%u, ftell=%ld, feof=%d, ferror=%d)\n", replay_frame_index, ftell(replay_file), feof(replay_file), ferror(replay_file));
        return 0;
    }

    /* Skip comment/blank lines. */
    if(line[0] == '#' || line[0] == '\n' || line[0] == '\r')
    {
        return Replay_Peek_Frame();
    }

    key0 = 0;
    key_count = 0;
    if(sscanf(line, "%u,%llu,%llu,%d,%d,%d,%d,%u,%u",
              &idx, &timestamp, &delta, &mx, &my, &btn, &kp, &key_count, &key0) < 7)
    {
        fprintf(PLAY_LOG, "PEEK FAIL: sscanf < 7 on line: %s", line);
        return 0;
    }

    memset(&peek_frame, 0, sizeof(peek_frame));
    peek_frame.frame_number  = idx;
    peek_frame.timestamp_ms  = timestamp;
    peek_frame.mouse_x       = (int16_t)mx;
    peek_frame.mouse_y       = (int16_t)my;
    peek_frame.mouse_buttons = (int16_t)btn;
    peek_frame.key_pressed   = (int8_t)kp;
    peek_frame.key_count     = (uint8_t)key_count;
    if(key_count > 0)
    {
        peek_frame.keys[0] = key0;
    }

    peek_valid = 1;
    return 1;
}

int Replay_Inject_Frame(void)
{
    uint64_t elapsed;
    Input_Frame inject;
    uint32_t skipped;

    if(!replaying || replay_file == NULL)
    {
        return 0;
    }

    /* On the very first inject, reset start_ticks to eliminate the gap between
       Platform_Replay_Start() (called during arg parsing) and the first actual
       Platform_Event_Handler() call (after game loading finishes). */
    if(!replay_first_inject)
    {
        replay_start_ticks = Platform_Get_Millies();
        replay_first_inject = 1;
    }

    elapsed = Platform_Get_Millies() - replay_start_ticks;

    /* Inject one frame per call to avoid overwriting clicks with subsequent btn=0.
       But skip runs of identical-state frames whose timestamps have all been reached —
       during recording, tight game loops can generate thousands of duplicate frames
       in the same millisecond.  Only the last frame before a state change matters. */
    if(!Replay_Peek_Frame())
    {
        Platform_Replay_Stop();
        return 0;
    }

    if(peek_frame.timestamp_ms > elapsed)
    {
        return 1;  /* Not time yet — still replaying, wait for next handler call. */
    }

    /* Consume this frame as the candidate to inject. */
    inject = peek_frame;
    peek_valid = 0;

    /* Fast-forward through duplicate frames that are all ready to inject.
       Tight game loops (world gen, dialogs) can produce thousands of identical
       frames at the same timestamp.  Skip them all, keeping the last one. */
    skipped = 0;
    while(Replay_Peek_Frame())
    {
        if(peek_frame.timestamp_ms > elapsed)
        {
            break;  /* Next frame is in the future — inject current. */
        }
        /* If state changed, inject the PREVIOUS frame (inject). */
        if(peek_frame.mouse_x != inject.mouse_x || peek_frame.mouse_y != inject.mouse_y || peek_frame.mouse_buttons != inject.mouse_buttons || peek_frame.key_pressed != inject.key_pressed || peek_frame.key_count != inject.key_count)
        {
            break;
        }
        /* Same state, same or past timestamp — skip inject, advance. */
        skipped++;
        replay_frame_index++;
        replay_frame_number++;
        inject = peek_frame;
        peek_valid = 0;
    }
    if(skipped > 0)
    {
        fprintf(PLAY_LOG, "PLAY SKIP   %u duplicate frames (idx %u..%u)\n", skipped, (unsigned)(inject.frame_number - skipped), (unsigned)inject.frame_number);
    }

    /* Inject this frame.  Replay coordinates are in game-space (320x200).
       User_Mouse_Handler() expects window-space and divides by current scale,
       so convert game coords to current window coords. */
    {
        int current_screen_scale = Platform_Get_Scale();
        int16_t wx = inject.mouse_x * (int16_t)current_screen_scale;
        int16_t wy = inject.mouse_y * (int16_t)current_screen_scale;
        uint32_t play_key0 = (inject.key_count > 0) ? inject.keys[0] : 0;
        int play_state_changed = (inject.mouse_x != play_log.prev_mouse_x) || (inject.mouse_y != play_log.prev_mouse_y) || (inject.mouse_buttons != play_log.prev_mouse_buttons) || (inject.key_pressed != play_log.prev_key_pressed) || (play_key0 != play_log.prev_key0);

        pointer_x = inject.mouse_x;
        pointer_y = inject.mouse_y;

        if(play_state_changed)
        {
            Log_Flush(&play_log, "PLAY", PLAY_LOG);

            fprintf(PLAY_LOG, "idx=%-6u t=%-8llu  mouse=(%3d,%3d) btn=%d  key=%d sccc=0x%04X  elapsed=%llu", (unsigned)inject.frame_number, (unsigned long long)inject.timestamp_ms, (int)inject.mouse_x, (int)inject.mouse_y, (int)inject.mouse_buttons, (int)inject.key_pressed, (unsigned)play_key0, (unsigned long long)elapsed);

            /* Show which field the mouse is over, if any. */
            {
                int fx = inject.mouse_x;
                int fy = inject.mouse_y;
                int fi;
                for(fi = 0; fi < fields_count; fi++)
                {
                    if(fx >= p_fields[fi].x1 && fx <= p_fields[fi].x2 && fy >= p_fields[fi].y1 && fy <= p_fields[fi].y2)
                    {
                        fprintf(PLAY_LOG, "  field[%d]=(%d,%d)-(%d,%d)", fi, p_fields[fi].x1, p_fields[fi].y1, p_fields[fi].x2, p_fields[fi].y2);
                        break;
                    }
                }
            }

            fprintf(PLAY_LOG, "\n");

            play_log.prev_mouse_x = inject.mouse_x;
            play_log.prev_mouse_y = inject.mouse_y;
            play_log.prev_mouse_buttons = inject.mouse_buttons;
            play_log.prev_key_pressed = inject.key_pressed;
            play_log.prev_key0 = play_key0;
            play_log.run_start_idx = (uint32_t)inject.frame_number;
            play_log.run_count = 1;
        }
        else
        {
            if(play_log.run_count == 0)
            {
                play_log.run_start_idx = (uint32_t)inject.frame_number;
            }
            play_log.run_count++;
        }

        /* Always call User_Mouse_Handler() — even with buttons=0 — so the
           software cursor gets drawn (Check_Mouse_Shape, Save/Draw_Mouse_On_Page). */
        if(inject.mouse_buttons != 0)
        {
            fprintf(PLAY_LOG, "PLAY CLICK  btn=%d at window=(%d,%d) -> game=(%d,%d)  mouse_enabled=%d  mouse_interrupt_active=%d\n", (int)inject.mouse_buttons, (int)wx, (int)wy, (int)(wx / current_screen_scale), (int)(wy / current_screen_scale), (int)mouse_enabled, (int)mouse_interrupt_active);
        }
        User_Mouse_Handler(inject.mouse_buttons, wx, wy);
        if(inject.mouse_buttons != 0)
        {
            fprintf(PLAY_LOG, "PLAY AFTER  pointer=(%d,%d)  platform_frame_mouse_buttons=%d  mouse_buffer_x=%d  mouse_buffer_y=%d\n", (int)pointer_x, (int)pointer_y, (int)platform_frame_mouse_buttons, (int)mouse_buffer_x, (int)mouse_buffer_y);
        }
    }

    /* Write keyboard input into the platform keyboard buffer so that
       Read_Key() can consume it — the game reads keys from the buffer,
       not from scan_code_char_code directly.
       The replay file stores kilgore_key values directly:
         bits 0-7: mox_key, bits 8-15: mox_character, bits 16+: mox_mod. */
    if(inject.key_count > 0)
    {
        uint32_t kilgore_key = inject.keys[0];
        int mox_key = (int)(kilgore_key & 0xFF);
        char mox_character = (char)((kilgore_key >> 8) & 0xFF);
        uint32_t mox_mod = kilgore_key & 0xFFFF0000u;
        fprintf(PLAY_LOG, "PLAY KBD_BUF kilgore=0x%08X  mox_key=%d  char=0x%02X  mod=0x%X  key_pressed_before=%d\n", kilgore_key, mox_key, (unsigned char)mox_character, mox_mod, (int)key_pressed);
        Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
        fprintf(PLAY_LOG, "PLAY KBD_BUF DONE  key_pressed_after=%d\n", (int)key_pressed);
    }

    replay_frame_index++;
    replay_frame_number++;

    return 1;  /* Still replaying. */
}



/* ========================================================================= */
/*  Log Comparison                                                           */
/* ========================================================================= */

/**
 * Compare recording and replay logs side-by-side.
 * Reads <basename>-RECORD.log and <basename>-REPLAY.log, displays them
 * in fixed-width columns, stops at the first divergence and shows what
 * the replay SHOULD have done.
 *
 * Only compares lines starting with "idx=" (the state-change lines).
 * Skips collapsed-run summaries, SKIP, CLICK, KBD_BUF, and comment lines.
 *
 * @param rmr_filepath  Path to the .RMR file (e.g., "NEW_GAME.RMR").
 */
void Platform_Replay_Compare_Logs(const char *rmr_filepath)
{
    char rec_path[512];
    char play_path[512];
    char rec_line[1024];
    char play_line[1024];
    FILE *rec_file;
    FILE *play_file;
    const char *dot;
    int prefix_len;
    int line_number = 0;
    int mismatches = 0;
    int max_context_after_mismatch = 5;

    dot = strrchr(rmr_filepath, '.');
    if(dot != NULL)
    {
        prefix_len = (int)(dot - rmr_filepath);
    }
    else
    {
        prefix_len = (int)strlen(rmr_filepath);
    }
    snprintf(rec_path,  sizeof(rec_path),  "%.*s-RECORD.log", prefix_len, rmr_filepath);
    snprintf(play_path, sizeof(play_path), "%.*s-REPLAY.log", prefix_len, rmr_filepath);

    rec_file  = fopen(rec_path, "r");
    play_file = fopen(play_path, "r");
    if(rec_file == NULL)
    {
        fprintf(stderr, "COMPARE: cannot open %s\n", rec_path);
        if(play_file) fclose(play_file);
        return;
    }
    if(play_file == NULL)
    {
        fprintf(stderr, "COMPARE: cannot open %s\n", play_path);
        fclose(rec_file);
        return;
    }

    fprintf(stderr, "\n========== REPLAY LOG COMPARISON ==========\n");
    fprintf(stderr, "%-60s | %-60s\n", "RECORD", "REPLAY");
    fprintf(stderr, "%-60s | %-60s\n", "------------------------------------------------------------", "------------------------------------------------------------");

    /* Read next "idx=" line from each file, skipping non-state-change lines. */
    while(1)
    {
        int got_rec = 0;
        int got_play = 0;

        /* Advance rec_file to next "idx=" line. */
        while(fgets(rec_line, sizeof(rec_line), rec_file) != NULL)
        {
            /* Strip trailing newline. */
            {
                size_t len = strlen(rec_line);
                while(len > 0 && (rec_line[len - 1] == '\n' || rec_line[len - 1] == '\r')) { rec_line[--len] = '\0'; }
            }
            if(strncmp(rec_line, "idx=", 4) == 0)
            {
                got_rec = 1;
                break;
            }
        }

        /* Advance play_file to next "idx=" line. */
        while(fgets(play_line, sizeof(play_line), play_file) != NULL)
        {
            {
                size_t len = strlen(play_line);
                while(len > 0 && (play_line[len - 1] == '\n' || play_line[len - 1] == '\r')) { play_line[--len] = '\0'; }
            }
            if(strncmp(play_line, "idx=", 4) == 0)
            {
                got_play = 1;
                break;
            }
        }

        if(!got_rec && !got_play)
        {
            break;  /* Both files exhausted. */
        }

        line_number++;

        /* Truncate lines for display (keep first 58 chars). */
        if(strlen(rec_line) > 58) { rec_line[58] = '\0'; }
        if(strlen(play_line) > 58) { play_line[58] = '\0'; }

        if(!got_rec)
        {
            fprintf(stderr, "%-60s | %-60s  <-- RECORD ended\n", "(end)", play_line);
            mismatches++;
            break;
        }
        if(!got_play)
        {
            fprintf(stderr, "%-60s | %-60s  <-- REPLAY ended\n", rec_line, "(end)");
            /* Show remaining RECORD lines as "should have been". */
            fprintf(stderr, "\n--- REPLAY should have continued with: ---\n");
            fprintf(stderr, "  %s\n", rec_line);
            {
                int remaining = max_context_after_mismatch;
                while(remaining > 0 && fgets(rec_line, sizeof(rec_line), rec_file) != NULL)
                {
                    size_t len = strlen(rec_line);
                    while(len > 0 && (rec_line[len - 1] == '\n' || rec_line[len - 1] == '\r')) { rec_line[--len] = '\0'; }
                    if(strncmp(rec_line, "idx=", 4) == 0)
                    {
                        fprintf(stderr, "  %s\n", rec_line);
                        remaining--;
                    }
                }
            }
            break;
        }

        /* Compare the common prefix: idx, t, mouse, btn, key, sccc (before the dt=/elapsed= suffix). */
        {
            /* Find the tail field (dt= or elapsed=) and compare only up to it. */
            const char *rec_tail  = strstr(rec_line, "  dt=");
            const char *play_tail = strstr(play_line, "  elapsed=");
            int rec_prefix_len  = rec_tail  ? (int)(rec_tail  - rec_line) : (int)strlen(rec_line);
            int play_prefix_len = play_tail ? (int)(play_tail - play_line) : (int)strlen(play_line);

            int match = (rec_prefix_len == play_prefix_len) && (strncmp(rec_line, play_line, rec_prefix_len) == 0);

            if(match)
            {
                fprintf(stderr, "%-60s | %-60s\n", rec_line, play_line);
            }
            else
            {
                fprintf(stderr, "%-60s | %-60s  <-- MISMATCH\n", rec_line, play_line);
                mismatches++;
                if(mismatches >= 3)
                {
                    fprintf(stderr, "\n--- Too many mismatches, stopping. ---\n");
                    fprintf(stderr, "--- REPLAY should have been: ---\n");
                    fprintf(stderr, "  %s\n", rec_line);
                    {
                        int remaining = max_context_after_mismatch;
                        while(remaining > 0 && fgets(rec_line, sizeof(rec_line), rec_file) != NULL)
                        {
                            size_t len = strlen(rec_line);
                            while(len > 0 && (rec_line[len - 1] == '\n' || rec_line[len - 1] == '\r')) { rec_line[--len] = '\0'; }
                            if(strncmp(rec_line, "idx=", 4) == 0)
                            {
                                fprintf(stderr, "  %s\n", rec_line);
                                remaining--;
                            }
                        }
                    }
                    break;
                }
            }
        }
    }

    if(mismatches == 0)
    {
        fprintf(stderr, "\n=== PERFECT MATCH: %d state changes compared, all identical. ===\n", line_number);
    }
    else
    {
        fprintf(stderr, "\n=== COMPARISON DONE: %d mismatches found in %d state changes. ===\n", mismatches, line_number);
    }

    fclose(rec_file);
    fclose(play_file);
}
