# Crash reports and symbol resolution

When ReMoMber or HeMoM crashes, [STU_BRAK](../../STU/src/STU_BRAK.c) catches the
signal (`SIGFPE`, `SIGSEGV`, `SIGILL`, `SIGABRT`, `SIGBUS`) and prints a stack
trace to stderr and the STU_LOG file *before* the process dies. Whether that
trace shows readable function names or just hex addresses depends on how the
executable was linked and whether debug information is available. This doc
explains how the pieces fit together and how to turn a hex address into a
`file.c:123` location on each supported OS.

## The problem

A raw stack trace after a crash looks like a list of return addresses:

```
ReMoMber(+0x2a1f4)[0x55f0b8c1a1f4]
ReMoMber(+0x8b7c1)[0x55f0b8c807c1]
/lib/x86_64-linux-gnu/libc.so.6(+0x2a1ca)[0x7f8e6a02a1ca]
```

For the trace to be *useful*, the runtime needs to be able to look up which
function each address falls inside. That lookup happens in two very different
places:

1. **At crash time**, inside the process, via `backtrace_symbols` on POSIX or
   `SymFromAddr` on Windows. This is what STU_BRAK does and what makes names
   appear in the STU_LOG.
2. **After the fact**, on the developer's machine, by feeding an address to a
   tool like `addr2line`, `atos`, or a debugger. This is where you get
   file:line — the in-process lookups don't do that.

Both paths need the same input: the executable has to *carry* the symbol
information somewhere the tool can find it.

## What `-rdynamic` actually does

On ELF platforms (Linux, most BSDs), only symbols placed in the *dynamic*
symbol table are visible to `dlsym` and `backtrace_symbols`. By default the
linker only exports symbols that are referenced by shared libraries the
executable links against — a handful of names, none of them yours.

`-rdynamic` tells the linker to put **every non-hidden symbol** in the dynamic
symbol table, not just the ones that libraries need. After that,
`backtrace_symbols` inside STU_BRAK's POSIX handler can turn most return
addresses into names.

The knob is set portably in [src/CMakeLists.txt](../../src/CMakeLists.txt) via
`set_target_properties(<tgt> PROPERTIES ENABLE_EXPORTS ON)` on both `ReMoMber`
and `HeMoM`. CMake's `ENABLE_EXPORTS`:

- On **GNU/Clang** it passes `-rdynamic` (which the linker turns into
  `--export-dynamic`).
- On **Apple Clang** it passes `-Wl,-export_dynamic`.
- On **MSVC** it does nothing — Windows resolves symbols through the `.pdb`
  file, not the executable's export table.

You can verify it worked on Linux after a rebuild:

```
$ nm --dynamic out/build/clang-debug/bin/Debug/ReMoMber | grep -c ' T '
2049
```

A handful (≤10) means the flag didn't stick. Thousands means it did.

### What `-rdynamic` does *not* help with

- **`static` functions** are still hidden regardless. The compiler gives them
  internal linkage, so the linker has nothing to export. They show up as
  `ReMoMber+0xNNNN` in the trace even with `-rdynamic`. Resolve them with
  `addr2line`.
- **File:line information.** `-rdynamic` gives you the function name, not the
  source line. That still requires `addr2line`/`atos`/gdb reading the debug
  sections.
- **Stripped binaries.** If someone runs `strip` on the executable, dynamic
  symbols may survive (they're needed at runtime), but the `.debug_*` sections
  used by `addr2line` are gone.

## Debug info: the other half

You almost always want to combine `-rdynamic` with a build that actually
carries debug information. Both `clang-debug` and `MSVC-debug` presets already
do this — you just want to know how to read what's there:

- **Linux/glibc, macOS** — debug info lives in DWARF sections inside the
  executable (`.debug_info`, `.debug_line`, etc.). `addr2line -e ReMoMber ADDR`
  reads them.
- **macOS** — DWARF may live inside a `.dSYM` bundle next to the executable if
  you did a release build with `dsymutil`. `atos -o ReMoMber.dSYM/... ADDR`.
- **Windows/MSVC** — debug info is in a separate `.pdb` file. Address→symbol is
  handled by dbghelp *inside* STU_BRAK; the `.pdb` must be next to the `.exe`
  (Debug/Release outputs) or on `_NT_SYMBOL_PATH`.
- **Windows/MinGW** — DWARF inside the `.exe`, same as Linux. `addr2line` from
  the MinGW toolchain reads it directly.

## Using STU_BRAK

STU_BRAK is installed at process start by `ReMoM.c` and `HeMoM.c` via
`STU_BRAK_Install()`. Nothing else you have to do — a crash just prints:

```
[stu-brak] SIGFPE (arithmetic error)
[stu-brak]   #00 Sort_Research_List  (Spells128.c:963)
[stu-brak]   #01 Player_Research_Spells  (Spells128.c:202)
[stu-brak]   #02 Player_Gets_Spell  (NEXTTURN.c:3816)
...
```

On Windows the `.pdb` is loaded automatically and lines show up. On POSIX you
need `-rdynamic` for the function names — file:line is a follow-up step with
`addr2line`, described below.

You can also fire a trace *voluntarily* — no crash needed — via
`STU_BRAK_Report("some reason")`. Useful for assertion paths or for pinning
down the caller of a function you're investigating.

## Per-OS specifics

### Linux (glibc, GCC or Clang)

Nothing special beyond `-rdynamic` + a debug build. STU_BRAK uses `backtrace()`
+ `backtrace_symbols_fd()` from `<execinfo.h>`, which needs both a dynamic
symbol table (for the *name*) and DWARF sections (which `addr2line` reads for
file:line).

Verify:

```
$ file out/build/clang-debug/bin/Debug/ReMoMber
ELF 64-bit LSB pie executable ... with debug_info, not stripped
$ nm --dynamic .../ReMoMber | grep -c ' T '
2049
```

If names still don't resolve after `-rdynamic`, check that the executable
wasn't stripped and that the crash-time frames aren't in a `static` function
(look at [Static functions](#static-functions-and-addr2line) below).

### macOS (Apple Clang)

`ENABLE_EXPORTS` passes `-Wl,-export_dynamic` which does the equivalent job.
`backtrace()`/`backtrace_symbols()` are available from `<execinfo.h>` just like
Linux and STU_BRAK's POSIX path works unchanged.

For file:line, macOS uses `atos` instead of `addr2line`:

```
$ atos -o out/build/clang-debug/bin/Debug/ReMoMber 0x100003a1c
Sort_Research_List (in ReMoMber) (Spells128.c:963)
```

If you built a Release binary with a separate `.dSYM`:

```
$ atos -o ReMoMber.dSYM/Contents/Resources/DWARF/ReMoMber 0x100003a1c
```

### Windows (MSVC)

`-rdynamic` doesn't apply. Windows crash handling uses **dbghelp** on the
Windows side of `STU_BRAK.c`:

- STU_BRAK installs `SetUnhandledExceptionFilter` and calls `SymInitialize` +
  `StackWalk64` + `SymFromAddr` + `SymGetLineFromAddr64`.
- Symbols are resolved from the `.pdb` file next to the `.exe`. The
  `MSVC-debug` preset already produces one.
- Report format includes file:line directly:

```
[stu-brak] ACCESS_VIOLATION (0xC0000005) at 0x00007ff6a1b23a1c
[stu-brak] reading address 0x0000000000000000
[stu-brak]   #00 Sort_Research_List  (Spells128.c:963)
[stu-brak]   #01 Player_Research_Spells  (Spells128.c:202)
```

If names show up but file:line does not, the `.pdb` is missing from the
executable's directory or from `_NT_SYMBOL_PATH`.

### Windows (MinGW / clang-cl / other non-MSVC)

You can use STU_BRAK's Windows path unchanged (it does not require MSVC — just
the Win32 API), but symbol *files* differ:

- **MinGW-w64 + GCC** produces DWARF-in-PE by default. dbghelp does not read
  DWARF. Either compile with `-gcodeview` (Clang or recent GCC) to emit
  CodeView, or accept that dbghelp will only resolve exported names and use
  `addr2line` from the MinGW toolchain for file:line.
- **clang-cl** emits PDB by default; behavior matches MSVC.

## Static functions and `addr2line`

Even with `-rdynamic`, a frame inside a `static` function shows up as
`ReMoMber(+0xNNNN)` — the linker never exported the name. The address is still
useful; feed it to `addr2line` to get the source location.

### The Linux/MinGW workflow

Extract addresses from the STU_LOG or the terminal capture. STU_BRAK emits them
in `+0xNNNN` form for POSIX unresolved frames:

```
ReMoMber(+0x8b7c1) [0x55f0b8c807c1]
```

Feed the offset to `addr2line`:

```
$ addr2line -e out/build/clang-debug/bin/Debug/ReMoMber -f -C -i 0x8b7c1
Sort_Research_List
/home/jbalcomb/STU/devel/ReMoM/MoM/src/Spells128.c:963
```

Flags:

- `-e <exe>` — the executable that contains the addresses.
- `-f` — print the enclosing function name.
- `-C` — demangle C++ names (harmless for pure C).
- `-i` — walk inlined frames (a single address can correspond to several
  source lines if the compiler inlined).

You can feed multiple addresses at once, or pipe them:

```
$ echo -e "0x8b7c1\n0x8b0f2\n0x8a4d8" | addr2line -e ReMoMber -f -C -i
```

### Extracting the offset from a `[0x...]` absolute address

Sometimes only the absolute address is printed (no `+0xNNNN`), which is what
you get if the executable was built without PIE or if `backtrace_symbols_fd`
couldn't compute an offset. Subtract the executable's load base:

```
$ awk '/r-xp/ { print $1; exit }' /proc/<pid>/maps
55f0b8bef000-...
$ printf '0x%x\n' $((0x55f0b8c807c1 - 0x55f0b8bef000))
0x917c1
```

Feed that offset to `addr2line`. For dead crashes where you can't read
`/proc/<pid>/maps`, the STU_BRAK-emitted `+0xNNNN` form is easier — that
number is already the offset, no subtraction needed.

### macOS

`addr2line` from LLVM (`llvm-addr2line`) works, but the native tool is `atos`:

```
$ atos -o ReMoMber -l 0x100000000 0x100003a1c
Sort_Research_List (in ReMoMber) (Spells128.c:963)
```

`-l` is the load address (from `vmmap <pid>` or the crash report); omit for
non-ASLR binaries.

### Windows (MSVC .pdb)

Address→symbol is already done inside STU_BRAK via dbghelp — you shouldn't
need a separate tool. If you want it out-of-process:

- `cvdump.exe` from the DIA SDK dumps a `.pdb`.
- WinDbg's `!address` / `.reload` / `ln <addr>` works interactively.
- `llvm-symbolizer` reads both PDB and DWARF-in-PE:
  ```
  > llvm-symbolizer.exe --obj=ReMoMber.exe 0x8b7c1
  Sort_Research_List
  C:\path\Spells128.c:963
  ```

## Recipe: taking a crash from log to source

1. Grab the STU_LOG file from the most recent run.
2. Search for the last `[stu-brak]` block. It ends at the SIGFPE/SIGSEGV/... line and lists frames above.
3. Anything already reading `func (file.c:line)` — done.
4. For anything in `func+0xNNNN` (name known, file:line missing) or bare
   `+0xNNNN` (both missing): pass the offset to `addr2line -e <exe> -f -C -i`.
5. For crashes on Windows, dbghelp should have printed file:line already; if
   it printed only names, the `.pdb` isn't next to the `.exe`.

## When names still don't appear

Symptoms and fixes:

| Symptom | Likely cause | Fix |
|---|---|---|
| POSIX trace is all hex | `-rdynamic` not applied (no `ENABLE_EXPORTS`) | Rebuild after `set_target_properties(<tgt> PROPERTIES ENABLE_EXPORTS ON)` |
| POSIX trace shows a few names then hex | Frames are in `static` functions | Use `addr2line` for the hex ones |
| `addr2line` says `??` | Debug info missing/stripped, or wrong exe | Rebuild without `strip`; make sure `-g` is on (Debug preset already has it) |
| Windows trace shows names but no file:line | `.pdb` not next to `.exe` | Copy the `.pdb` alongside; or set `_NT_SYMBOL_PATH` |
| Windows trace is all hex | STU_BRAK's dbghelp init failed | Check `[stu-brak] SymInitialize failed` line above the frames — usually a missing `.pdb` or permission issue |
| Linux trace ends after 1–2 frames | Frame pointers omitted by the optimizer | Compile with `-fno-omit-frame-pointer` (Debug builds usually already do) |

## References

- [STU/src/STU_BRAK.c](../../STU/src/STU_BRAK.c) — the crash handler itself.
- [src/CMakeLists.txt](../../src/CMakeLists.txt) — where `ENABLE_EXPORTS` is set.
- glibc `backtrace(3)` man page: https://man7.org/linux/man-pages/man3/backtrace.3.html
- CMake `ENABLE_EXPORTS` property: https://cmake.org/cmake/help/latest/prop_tgt/ENABLE_EXPORTS.html
- `addr2line(1)` — `man addr2line`.
- Windows dbghelp: https://learn.microsoft.com/windows/win32/debug/debug-help-library
