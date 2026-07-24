/**
 * Platform_Input_Metrics.h — Layer 1 of the Platform-Input responsiveness toolkit.
 *
 * See doc/#AI_Plans/{BRA,PRD,PLAN}-Platform-Input.md.
 *
 * An in-memory, per-present timeline of input/render cadence, written at exit as a fixed-width
 * values file (.fwv -- project convention, NOT csv).  It answers the three questions the "mouse
 * feels laggy" reports can't: is the cursor FREEZING (long gaps between input ticks), is the engine
 * STACKING vsync-blocked presents (several presents per displayed frame, each blocking a refresh),
 * or is the cursor TELEPORTING (large per-present position steps from integer scale quantization)?
 *
 * Runtime-gated, shipped in release, OFF by default (BRA decision 2026-07-23).  Enabled by setting
 * the environment variable REMOM_INPUT_METRICS to a non-zero value:
 *     REMOM_INPUT_METRICS=1            -> writes ./remom_input_metrics.fwv
 *     REMOM_INPUT_METRICS=/path/x.fwv  -> writes that path
 * When disabled, every hook is a single Input_Metrics_Active() branch with no I/O and no allocation.
 *
 * Backend-agnostic: it touches neither SDL nor engine globals.  Each windowed backend feeds it from
 * its event-handler tick, its cursor-position poll, and a bracket around its present call.  The SDL3
 * and win32 backends can adopt the same three hooks unchanged.
 *
 * Crash-safe: rows are written to a buffered stream and flushed every ~0.5 s, so a SIGSEGV, kill, or
 * force-quit (none of which reach atexit) loses at most the last half second -- essential for a tool
 * whose whole job is diagnosing sessions that misbehave.  The per-row write is a userspace buffer
 * copy (no syscall), so it does not perturb the present timing it measures.
 *
 * All times are milliseconds (the SDL_GetTicks() domain the backends already use).  1 ms resolution
 * is adequate for a 16.7 ms refresh; a later revision can move to a high-resolution counter.
 */

#ifndef PLATFORM_INPUT_METRICS_H
#define PLATFORM_INPUT_METRICS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Open the output stream and begin recording.  Call once, after the window/renderer exist, only when
 * the REMOM_INPUT_METRICS gate is set.  Registers an atexit() flush for clean exits; the periodic
 * mid-run flush covers the crash/kill paths that never reach atexit.
 * @param out_path      Destination .fwv path.
 * @param backend       "SDL2" / "SDL3" / "win32" -- recorded in the file header.
 * @param video_driver  e.g. SDL_GetCurrentVideoDriver() ("x11" / "wayland") -- header, and itself a
 *                      variance source the reports may hinge on.
 * @param scale         Integer window/screen scale.
 * @param refresh_hz    Display refresh rate, or 0 if unknown.
 * @return 0 on success (recording active), non-zero on failure (stays inactive).
 */
int Input_Metrics_Init(const char * out_path, const char * backend, const char * video_driver, int scale, int refresh_hz);

/**
 * Non-zero while recording.  The cheap gate every hook tests first.
 */
int Input_Metrics_Active(void);

/**
 * Note one input-handler tick (Platform_Event_Handler / Platform_Pump_Events).  Tracks the worst gap
 * between ticks since the last present -- the worst-case cursor freeze.
 * @param now_ms  SDL_GetTicks() at the tick.
 */
void Input_Metrics_Note_Tick(uint64_t now_ms);

/**
 * Note one cursor-position sample, in game coordinates.  The per-present row's dx/dy is the net
 * motion between presents; a large single step reads as a teleport/quantization.
 */
void Input_Metrics_Note_Poll(int game_x, int game_y);

/**
 * Tag the NEXT present with a reason string (e.g. "cursor" for a cursor-driven present).  Consumed
 * and reset to "frame" by the next Input_Metrics_Record_Present().  Truncated to fit the column.
 */
void Input_Metrics_Set_Reason(const char * reason);

/**
 * Record one present.  Bracket the backend's present call: capture the tick before and after so the
 * block time (vsync stall) and the interval since the previous present are both measured.
 * @param t_before_ms  SDL_GetTicks() immediately before the present call.
 * @param t_after_ms   SDL_GetTicks() immediately after it returns.
 */
void Input_Metrics_Record_Present(uint64_t t_before_ms, uint64_t t_after_ms);

/**
 * Flush the ring buffer to the .fwv file and free it.  Idempotent; also runs via atexit().  Safe to
 * call when inactive.
 */
void Input_Metrics_Shutdown(void);


#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_INPUT_METRICS_H */
