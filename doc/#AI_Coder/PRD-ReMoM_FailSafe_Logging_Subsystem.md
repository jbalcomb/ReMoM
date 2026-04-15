
# Product Requirements Document: ReMoM Logging Subsystem

## 1. Objective
Implement a high-performance, single-threaded, fail-safe logging subsystem for the ReMoM game engine. The system must never block the main game loop, must avoid dynamic memory allocation during gameplay, and must preserve crash logs even if the application terminates unexpectedly.

## 2. Technical Constraints
* **Language:** Strict C89 (ANSI C).
* **Target Platforms:** Windows, macOS, Linux.
* **Threading:** Strictly single-threaded. No mutexes or atomic operations required.
* **Memory:** 2 MB fixed-size static ring buffer (matching Windows Large Page size).
* **Dependencies:** Standard C Library only (`stdio.h`, `stdlib.h`, `string.h`, `stdarg.h`). Platform-specific safe string formatting (`_vsnprintf` on MSVC, `vsnprintf` on POSIX) is permitted via preprocessor directives.

## 3. Architecture

### 3.1. The Ring Buffer (Message Queue)
* **Storage:** A `static char` array of 2,097,152 bytes (2 MB).
* **Pointers:** Managed by `size_t head` (write index) and `size_t tail` (read index).
* **Write Logic:** 1. Format the incoming log string into a 1 KB temporary stack buffer to ensure memory safety.
    2. Copy the temporary buffer into the 2 MB ring buffer.
    3. If the copy exceeds the buffer boundary, wrap around to index 0.
    4. If writing the new message would overtake the `tail` (buffer full), silently drop the message to prioritize engine execution over logging.

### 3.2. Output Strategy (Trickle Writing)
* **Execution:** A `Pump()` function called once per frame/tick.
* **Throughput Limit:** Maximum of 4 KB written to disk per frame to prevent I/O stalling.
* **Failure Handling:** If the target file handle cannot be acquired, the `tail` pointer advances to match the `head` pointer, discarding the data but keeping the queue healthy.

### 3.3. File Rotation & Crash Recovery
* **Commit File:** Logs are streamed to `remom_log_new.txt` during runtime.
* **Startup Rotation Logic:** 1. If `remom_log_previous.txt` exists, delete it.
    2. If `remom_log_current.txt` exists, rename it to `remom_log_previous.txt`.
    3. If `remom_log_new.txt` exists (indicating a successful run or a crash), rename it to `remom_log_current.txt`.
    4. Open a fresh `remom_log_new.txt` for the current session.



## 4. Example
remom_log.h
```c
#ifndef REMOM_LOG_H
#define REMOM_LOG_H

#include <stddef.h>

/* Log priority levels */
typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} RemomLogLevel;

/* Initializes the logging system, rotates old logs, and opens the new log file.
   Call this at the very beginning of the engine startup. */
void remom_log_init(void);

/* Formats and pushes a message into the 2MB ring buffer. 
   Safe to call anywhere in the single-threaded loop. */
void remom_log_write(RemomLogLevel level, const char* fmt, ...);

/* Flushes up to 4KB of the ring buffer to disk.
   Call this once per frame in the main update loop. */
void remom_log_pump(void);

/* Flushes any remaining data in the buffer and closes the file handle.
   Call this during graceful engine shutdown. */
void remom_log_shutdown(void);

#endif /* REMOM_LOG_H */
```
remom_log.c
```
#include "remom_log.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Platform-specific safe snprintf for C89 */
#if defined(_MSC_VER)
    #define SAFE_VSNPRINTF _vsnprintf
#else
    /* C99 standard, but practically universally supported in POSIX C89 compilers */
    #define SAFE_VSNPRINTF vsnprintf
#endif

#define REMOM_LOG_BUFFER_SIZE (2 * 1024 * 1024) /* 2 MB */
#define REMOM_LOG_PUMP_LIMIT  (4 * 1024)        /* 4 KB per frame */
#define REMOM_LOG_TEMP_SIZE   1024              /* 1 KB max per log line */

static char s_RingBuffer[REMOM_LOG_BUFFER_SIZE];
static size_t s_Head = 0;
static size_t s_Tail = 0;

static FILE* s_LogFile = NULL;
static int s_FileDisabled = 0;

static const char* s_LevelNames[] = {
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

void remom_log_init(void) {
    /* Step 1: Rotate existing files safely.
       In standard C, rename() fails if the target exists, so we remove() first. */
    remove("remom_log_previous.txt");
    rename("remom_log_current.txt", "remom_log_previous.txt");
    rename("remom_log_new.txt", "remom_log_current.txt");

    /* Step 2: Open the new commit file for trickle writing */
    s_LogFile = fopen("remom_log_new.txt", "wb");
    if (!s_LogFile) {
        s_FileDisabled = 1;
    } else {
        s_FileDisabled = 0;
    }

    s_Head = 0;
    s_Tail = 0;
}

void remom_log_write(RemomLogLevel level, const char* fmt, ...) {
    char tempBuf[REMOM_LOG_TEMP_SIZE];
    int charsWritten;
    size_t length;
    size_t i;
    size_t capacity;
    va_list args;

    /* 1. Format the prefix */
    int prefixLen = sprintf(tempBuf, "[%s] ", s_LevelNames[level]);
    if (prefixLen < 0) return;

    /* 2. Format the user string safely into our temporary stack buffer */
    va_start(args, fmt);
    charsWritten = SAFE_VSNPRINTF(tempBuf + prefixLen, REMOM_LOG_TEMP_SIZE - prefixLen - 1, fmt, args);
    va_end(args);

    if (charsWritten < 0) return;

    length = prefixLen + charsWritten;
    
    /* Ensure null termination and add a newline if one isn't present */
    if (length >= REMOM_LOG_TEMP_SIZE - 1) {
        length = REMOM_LOG_TEMP_SIZE - 2;
    }
    if (tempBuf[length - 1] != '\n') {
        tempBuf[length++] = '\n';
    }
    tempBuf[length] = '\0';

    /* 3. Check for buffer overflow (drop log if full) */
    if (s_Head >= s_Tail) {
        capacity = REMOM_LOG_BUFFER_SIZE - (s_Head - s_Tail) - 1;
    } else {
        capacity = (s_Tail - s_Head) - 1;
    }

    if (length > capacity) {
        /* Buffer is full. In a game, it's better to drop the log than crash or block. */
        return;
    }

    /* 4. Copy into the ring buffer, handling wrap-around */
    for (i = 0; i < length; ++i) {
        s_RingBuffer[s_Head] = tempBuf[i];
        s_Head = (s_Head + 1) % REMOM_LOG_BUFFER_SIZE;
    }
}

void remom_log_pump(void) {
    size_t bytesToWrite = 0;
    size_t contiguousBytes;

    if (s_Head == s_Tail) {
        return; /* Nothing to write */
    }

    /* Determine how much we can write this frame */
    if (s_Head > s_Tail) {
        bytesToWrite = s_Head - s_Tail;
    } else {
        /* Wrapped around, write up to the end of the physical buffer first */
        bytesToWrite = REMOM_LOG_BUFFER_SIZE - s_Tail;
    }

    /* Throttle to the 4KB frame limit */
    if (bytesToWrite > REMOM_LOG_PUMP_LIMIT) {
        bytesToWrite = REMOM_LOG_PUMP_LIMIT;
    }

    /* If the file system failed, just discard the data to prevent memory overflow */
    if (s_FileDisabled) {
        s_Tail = (s_Tail + bytesToWrite) % REMOM_LOG_BUFFER_SIZE;
        return;
    }

    /* Write to disk */
    contiguousBytes = fwrite(s_RingBuffer + s_Tail, 1, bytesToWrite, s_LogFile);
    
    /* Advance the tail pointer based on actual bytes written */
    if (contiguousBytes > 0) {
        s_Tail = (s_Tail + contiguousBytes) % REMOM_LOG_BUFFER_SIZE;
    }
}

void remom_log_shutdown(void) {
    if (!s_FileDisabled && s_LogFile) {
        /* Bypass the 4KB limit and dump whatever is left */
        while (s_Head != s_Tail) {
            size_t bytesToWrite;
            size_t written;

            if (s_Head > s_Tail) {
                bytesToWrite = s_Head - s_Tail;
            } else {
                bytesToWrite = REMOM_LOG_BUFFER_SIZE - s_Tail;
            }

            written = fwrite(s_RingBuffer + s_Tail, 1, bytesToWrite, s_LogFile);
            if (written == 0) break; /* Disk error, abort to prevent infinite loop */
            s_Tail = (s_Tail + written) % REMOM_LOG_BUFFER_SIZE;
        }

        fflush(s_LogFile);
        fclose(s_LogFile);
        s_LogFile = NULL;
    }
}
```
