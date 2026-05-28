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