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



/* ========================================================================= */
/*  State                                                                    */
/* ========================================================================= */

static FILE *replay_file = NULL;
static FILE *replay_log  = NULL;
static int   recording   = 0;
static int   replaying   = 0;

static uint32_t record_frame_count = 0;
static uint32_t replay_frame_count = 0;
static uint32_t replay_frame_index = 0;

static uint64_t replay_frame_number = 0;
static uint64_t replay_start_ticks = 0;
static uint64_t record_start_ticks = 0;
static uint64_t record_prev_timestamp = 0;
static int      replay_recorded_screen_scale = 1;

/* Peek buffer for timestamp-based injection. */
static Input_Frame peek_frame;
static int         peek_valid = 0;

/* Write detailed debug output to the log file if open, otherwise stderr. */
#define REPLAY_LOG (replay_log != NULL ? replay_log : stderr)



/* Open a log file alongside the replay file.  "FOO.RMR" -> "FOO.log" */
static void Replay_Open_Log(const char *filepath)
{
    char log_path[512];
    const char *dot = strrchr(filepath, '.');
    if(dot != NULL)
    {
        int prefix_len = (int)(dot - filepath);
        snprintf(log_path, sizeof(log_path), "%.*s.log", prefix_len, filepath);
    }
    else
    {
        snprintf(log_path, sizeof(log_path), "%s.log", filepath);
    }
    replay_log = fopen(log_path, "w");
    if(replay_log != NULL)
    {
        fprintf(stderr, "REPLAY: log -> %s\n", log_path);
    }
}

static void Replay_Close_Log(void)
{
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

    replay_file = fopen(filepath, "w");
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

    Replay_Open_Log(filepath);

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

    /* Patch the frame_count line.  The format "# frame_count=%-10u\n"
       left exactly 10 digits of space so we can overwrite in place. */
    /* Rewind to start and skip header lines until we find "# frame_count=". */
    {
        char line[256];
        fseek(replay_file, 0, SEEK_SET);
        while(fgets(line, sizeof(line), replay_file) != NULL)
        {
            if(strncmp(line, "# frame_count=", 14) == 0)
            {
                count_pos = ftell(replay_file) - (long)strlen(line);
                fseek(replay_file, count_pos, SEEK_SET);
                fprintf(replay_file, "# frame_count=%-10u\n", record_frame_count);
                break;
            }
        }
    }

    fclose(replay_file);
    replay_file = NULL;
    recording = 0;
    Replay_Close_Log();
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

    replay_file = fopen(filepath, "r");
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

    /* Restore the RNG to the recorded state for deterministic replay. */
    Set_Random_Seed(random_seed);

    replaying = 1;

    Replay_Open_Log(filepath);

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

    fclose(replay_file);
    replay_file = NULL;
    replaying = 0;
    replay_frame_count = 0;
    replay_frame_index = 0;
    peek_valid = 0;
    Replay_Close_Log();
}

int Platform_Replay_Active(void)
{
    return replaying;
}



/* ========================================================================= */
/*  Internal — called by Platform_Event_Handler()                            */
/* ========================================================================= */

void Replay_Capture_Frame(void)
{
    uint64_t timestamp;
    uint64_t delta;
    int key_count;
    uint32_t key0;

    if(!recording || replay_file == NULL)
    {
        return;
    }

    timestamp = Platform_Get_Millies() - record_start_ticks;
    delta = timestamp - record_prev_timestamp;
    record_prev_timestamp = timestamp;

    key_count = (scan_code_char_code != 0) ? 1 : 0;
    key0 = key_count > 0 ? (uint32_t)scan_code_char_code : 0;

    fprintf(replay_file, "%u,%llu,%llu,%d,%d,%d,%d,%d",
        record_frame_count,
        (unsigned long long)timestamp,
        (unsigned long long)delta,
        (int)pointer_x,
        (int)pointer_y,
        (int)mouse_buffer_button,
        (int)key_pressed,
        key_count);

    if(key_count > 0)
    {
        fprintf(replay_file, ",%u", (unsigned)key0);
    }
    fprintf(replay_file, "\n");

    /* Debug: log interesting frames (clicks, keypresses, or every 60th frame). */
    if(mouse_buffer_button != 0 || key_pressed != 0 || (record_frame_count % 60) == 0)
    {
        fprintf(REPLAY_LOG, "REC  idx=%-6u t=%-8llu dt=%-5llu  mouse=(%3d,%3d) btn=%d  key=%d sccc=0x%04X",
            record_frame_count,
            (unsigned long long)timestamp,
            (unsigned long long)delta,
            (int)pointer_x, (int)pointer_y,
            (int)mouse_buffer_button,
            (int)key_pressed,
            (unsigned)key0);

        /* Show which field the mouse is over, if any. */
        {
            int fx = pointer_x;
            int fy = pointer_y;
            int fi;
            for(fi = 0; fi < fields_count; fi++)
            {
                if(fx >= p_fields[fi].x1 && fx <= p_fields[fi].x2 && fy >= p_fields[fi].y1 && fy <= p_fields[fi].y2)
                {
                    fprintf(REPLAY_LOG, "  field[%d]=(%d,%d)-(%d,%d)", fi, p_fields[fi].x1, p_fields[fi].y1, p_fields[fi].x2, p_fields[fi].y2);
                    break;
                }
            }
        }

        fprintf(REPLAY_LOG, "\n");
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
        return 0;
    }

    if(fgets(line, sizeof(line), replay_file) == NULL)
    {
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
    int injected = 0;

    if(!replaying || replay_file == NULL)
    {
        return 0;
    }

    elapsed = Platform_Get_Millies() - replay_start_ticks;

    /* Inject all frames whose timestamp has been reached. */
    while(Replay_Peek_Frame())
    {
        if(peek_frame.timestamp_ms > elapsed)
        {
            break;  /* Not time yet — wait for the next handler call. */
        }

        /* Inject this frame.  Replay coordinates are in game-space (320x200).
           User_Mouse_Handler() expects window-space and divides by current scale,
           so convert game coords to current window coords. */
        {
            int current_screen_scale = Platform_Get_Scale();
            int16_t wx = peek_frame.mouse_x * (int16_t)current_screen_scale;
            int16_t wy = peek_frame.mouse_y * (int16_t)current_screen_scale;

            pointer_x = peek_frame.mouse_x;
            pointer_y = peek_frame.mouse_y;

            fprintf(REPLAY_LOG, "PLAY INJECT idx=%-6u  game=(%3d,%3d) window=(%d,%d)  btn=%d  screen_scale=%d  pointer_before=(%d,%d)  fields_count=%d  mouse_buffer_button=%d\n",
                (unsigned)peek_frame.frame_number,
                (int)peek_frame.mouse_x, (int)peek_frame.mouse_y,
                (int)wx, (int)wy,
                (int)peek_frame.mouse_buttons,
                current_screen_scale,
                (int)pointer_x, (int)pointer_y,
                (int)fields_count,
                (int)mouse_buffer_button);

            if(peek_frame.mouse_buttons != 0)
            {
                fprintf(REPLAY_LOG, "PLAY CLICK  btn=%d at window=(%d,%d) -> game=(%d,%d)  mouse_enabled=%d  mouse_interrupt_active=%d\n",
                    (int)peek_frame.mouse_buttons, (int)wx, (int)wy, (int)(wx / current_screen_scale), (int)(wy / current_screen_scale),
                    (int)mouse_enabled, (int)mouse_interrupt_active);
                User_Mouse_Handler(peek_frame.mouse_buttons, wx, wy);
                fprintf(REPLAY_LOG, "PLAY AFTER  pointer=(%d,%d)  mouse_buffer_button=%d  mouse_buffer_x=%d  mouse_buffer_y=%d\n",
                    (int)pointer_x, (int)pointer_y, (int)mouse_buffer_button, (int)mouse_buffer_x, (int)mouse_buffer_y);
            }
        }

        /* Write keyboard input into the platform keyboard buffer so that
           Read_Key() can consume it — the game reads keys from the buffer,
           not from scan_code_char_code directly.
           The replay file stores SCCC values (scan_code << 8 | char_code).
           Convert to kilgore_key format: mox_character in bits 8-15. */
        if(peek_frame.key_count > 0)
        {
            uint32_t sccc = peek_frame.keys[0];
            char char_code = (char)(sccc & 0xFF);
            fprintf(stderr, "PLAY KBD_BUF char_code=0x%02X  key_pressed_before=%d\n", (unsigned char)char_code, (int)key_pressed);
            Platform_Keyboard_Buffer_Add_Key_Press(MOX_KEY_UNKNOWN, MOX_MOD_NONE, char_code);
            fprintf(stderr, "PLAY KBD_BUF DONE  key_pressed_after=%d\n", (int)key_pressed);
        }

        /* Debug: log injected frames that have actual input. */
        if(peek_frame.mouse_buttons != 0 || peek_frame.key_pressed != 0 || (replay_frame_index % 60) == 0)
        {
            fprintf(REPLAY_LOG, "PLAY idx=%-6u t=%-8llu  elapsed=%-8llu  mouse=(%3d,%3d) btn=%d  key=%d sccc=0x%04X",
                (unsigned)peek_frame.frame_number,
                (unsigned long long)peek_frame.timestamp_ms,
                (unsigned long long)elapsed,
                (int)peek_frame.mouse_x, (int)peek_frame.mouse_y,
                (int)peek_frame.mouse_buttons,
                (int)peek_frame.key_pressed,
                (unsigned)(peek_frame.key_count > 0 ? peek_frame.keys[0] : 0));

            /* Show which field the mouse is over, if any. */
            {
                int fx = peek_frame.mouse_x;
                int fy = peek_frame.mouse_y;
                int fi;
                for(fi = 0; fi < fields_count; fi++)
                {
                    if(fx >= p_fields[fi].x1 && fx <= p_fields[fi].x2 && fy >= p_fields[fi].y1 && fy <= p_fields[fi].y2)
                    {
                        fprintf(REPLAY_LOG, "  field[%d]=(%d,%d)-(%d,%d)", fi, p_fields[fi].x1, p_fields[fi].y1, p_fields[fi].x2, p_fields[fi].y2);
                        break;
                    }
                }
            }

            fprintf(REPLAY_LOG, "\n");
        }

        replay_frame_index++;
        replay_frame_number++;
        peek_valid = 0;
        injected = 1;
    }

    /* If no more frames remain, stop replay. */
    if(!Replay_Peek_Frame() && !peek_valid)
    {
        Platform_Replay_Stop();
        return injected;
    }

    return 1;  /* Still replaying (even if no frame injected this call — waiting for timestamp). */
}
