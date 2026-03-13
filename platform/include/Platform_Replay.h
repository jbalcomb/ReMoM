/**
 * Platform_Replay.h — Record & Replay API for deterministic input capture.
 *
 * Records input state at the Platform_Event_Handler() boundary and replays it
 * in place of live input. Used for demo mode and debugging.
 *
 * File format (.RMR):
 *   Bytes  0-3:   Magic "RMOM" (0x524D4F4D)
 *   Bytes  4-7:   Version (uint32_t)
 *   Bytes  8-11:  Frame count (uint32_t)
 *   Bytes 12-15:  RNG seed at start of recording (uint32_t)
 *   Bytes 16+:    Array of Input_Frame structs
 */

#ifndef PLATFORM_REPLAY_H
#define PLATFORM_REPLAY_H

#include <stdint.h>



/* ========================================================================= */
/*  Constants                                                                */
/* ========================================================================= */

#define REPLAY_FILE_MAGIC    0x524D4F4D  /* "RMOM" */
#define REPLAY_FILE_VERSION  1
#define REPLAY_MAX_KEYS_PER_FRAME  8



/* ========================================================================= */
/*  Types                                                                    */
/* ========================================================================= */

/**
 * File header for .RMR replay files.
 */
typedef struct Replay_Header
{
    uint32_t magic;
    uint32_t version;
    uint32_t frame_count;
    uint32_t rng_seed;
} Replay_Header;

/**
 * One frame of recorded input state.
 * Captured after Platform_Event_Handler() processes all OS events.
 */
typedef struct Input_Frame
{
    uint64_t frame_number;
    uint64_t timestamp_ms;
    int16_t  mouse_x;
    int16_t  mouse_y;
    int16_t  mouse_buttons;
    int8_t   key_pressed;
    uint8_t  key_count;
    uint32_t keys[REPLAY_MAX_KEYS_PER_FRAME];
} Input_Frame;



#ifdef __cplusplus
extern "C" {
#endif



/* ========================================================================= */
/*  Recording API                                                            */
/* ========================================================================= */

/**
 * Begin recording input to a file.
 * Captures the current RNG seed in the file header.
 * @param filepath  Path to the output .RMR file.
 * @return  0 on success, non-zero on failure.
 */
int Platform_Record_Start(const char *filepath);

/**
 * Stop recording and finalize the file (writes frame count to header).
 */
void Platform_Record_Stop(void);

/**
 * Return non-zero if recording is currently active.
 */
int Platform_Record_Active(void);



/* ========================================================================= */
/*  Replay API                                                               */
/* ========================================================================= */

/**
 * Begin replaying input from a file.
 * Restores the RNG seed from the file header.
 * @param filepath  Path to the input .RMR file.
 * @return  0 on success, non-zero on failure.
 */
int Platform_Replay_Start(const char *filepath);

/**
 * Stop replaying and close the file. Returns to live input.
 */
void Platform_Replay_Stop(void);

/**
 * Return non-zero if replay is currently active (frames remaining).
 */
int Platform_Replay_Active(void);



/* ========================================================================= */
/*  Internal (called by Platform_Event_Handler)                              */
/* ========================================================================= */

/**
 * If recording, capture the current input state as a new frame.
 * Called at the end of Platform_Event_Handler() after processing OS events.
 */
void Replay_Capture_Frame(void);

/**
 * If replaying, inject the next recorded frame into the engine's input globals.
 * Called at the start of Platform_Event_Handler() instead of polling OS events.
 * @return  Non-zero if a frame was injected, 0 if replay has ended.
 */
int Replay_Inject_Frame(void);



#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_REPLAY_H */
