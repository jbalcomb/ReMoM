# Borland C++ 3.1 Codegen Test

How to settle "what C source produces this asm?" by compiling candidate C with the compiler OG-MoM was built with (Borland C++ 3.1) and comparing the generated asm against the OG asm. Use this whenever a reconstruction question depends on codegen ("does `xor ax, ax` mean `return 0`?", "does `return;` touch AX?", "if/else or ternary?", "which local lands in SI/DI?") instead of reasoning from memory.

The authority is always the OG asm (the IDA Pro 5.5 disassembly). The compiler is a tool for testing a candidate source against it: if a candidate compiles to the OG instructions, that candidate is consistent with the asm; if two candidates compile identically, the asm cannot tell them apart.

Everything runs from Git Bash on the dev box. No install needed: the compiler and DOSBox are already on disk.

## Tools on disk

| What | Path |
|---|---|
| **Borland C++ 3.1 (1992)** | `/c/STU/devel/mom131-source-map/tools/BORLANDC/` (`BIN/BCC.EXE`, `INCLUDE/`, `LIB/`) |
| Borland C++ 3.00 (1991) - **do not use** | `/c/STU/DBWD/BORLANDC/` |
| DOSBox 0.74-3 | `/c/STU/WD/DOSBox/DOSBox.exe` (a debugger build, `dosbox-74-3-debug.exe`, is in the same folder) |
| OG asm (ground truth to compare against) | `/c/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/segNNN/<Function>.asm` (resident) and `.../ovrNNN/<Function>.asm` (overlays) |

`BCC.EXE` is a 16-bit DOS program, so it runs inside DOSBox. On its first run DOSBox writes a default config to `C:\Users\<user>\AppData\Local\DOSBox\dosbox-0.74-3.conf`; that's expected.

### 3.1, not 3.00

The two installs produce different code, and only 3.1 matches the OG. The quickest tell is signed division by 2:

| Source | BCC 3.00 | BCC 3.1 | OG asm |
|---|---|---|---|
| `w / 2` | `mov bx, 2` / `cwd` / `idiv bx` | `cwd` / `sub ax, dx` / `sar ax, 1` | `cwd` / `sub ax, dx` / `sar ax, 1` (447 times in WIZARDS) |

3.00 also allocates register variables differently: in `Draw_Bitmap_Rotated` it needed a different declaration order to put the same variable in DI. A test run on 3.00 can send you after differences that are not in the source.

**Check the banner on every run.** The first line of `OUT.TXT` must read `Borland C++  Version 3.1 Copyright (c) 1992 Borland International`.

## Flags

```
bcc -mm -a -d -S -ID:\INCLUDE NAME.C          resident module (WIZARDS.EXE segNNN)
bcc -Y -mm -a -d -S -ID:\INCLUDE NAME.C       overlay module (WIZARDS.EXE ovrNNN)
```

| Flag | What | Where it comes from |
|---|---|---|
| `-mm` | medium model: far code, near data | The OG asm: far procedures (`retf`), and pointers to data passed as one word (a pointer argument is cleaned up with a single `pop cx`; `Retn_Struct@` is loaded with `mov bx, [Retn_Struct@]` and used through DS). `-ml` (large) pushes `ss` with every pointer and does not match. |
| `-a` | word alignment: pads data and struct members so a word starts on an even address | The build notes of `mom131-source-map`. **Consistent with the asm, not proven:** 1,411 of the 1,412 named words in `dseg` are at even addresses (the odd one, `directvideo`, is Borland's run-time library), and structures carry pad bytes before words (`s_LAIR` +0x11). Every data pad that could be placed falls at a module boundary, where the linker aligns anyway, or in AIL library code. |
| `-d` | merge duplicate string literals within a module | The build notes of `mom131-source-map`. **Not checkable from the asm:** a merged literal and a `char x[] = "..."` array look the same in the binary. |
| `-Y` | overlay code | The build notes of `mom131-source-map`, for modules linked as overlays. **Not checkable this way:** `-Y` changes nothing in `-S` output for a test function. |

`-a` and `-d` change only data layout, so they don't affect the code comparisons in this doc (the byte-identical result below was built with them).
| `-S` | write asm (`NAME.ASM`) instead of an `.obj` | For reading the output. See **`-S` artifacts** below. |

Do not add optimizer flags (`-O`, `-G`, `-Z`, ...) unless a question depends on them, and say which flags you used. (On BCC 3.00, `-O`, `-O2`, `-Ot`, `-Os`, `-G`, `-Z` and `-r-` each moved one test function further from the OG; this has not been repeated on 3.1.)

## Procedure

### 1. Make a work folder and write the test C

Use a throwaway folder (an agent's scratchpad, or anywhere outside the repo). Keep DOS 8.3 file names: `RETTEST.C`, not `return_test.c`.

```bash
T=/c/path/to/scratch/bcc
mkdir -p "$T"
cat > "$T/RETTEST.C" <<'EOF'
int g_flag;
int g_val;

void V_ret(int x)
{
    if(g_flag == 0)
    {
        return;
    }
    g_val = x;
}
EOF
```

Write each candidate as its own small function in the one file, with globals for inputs and outputs so the optimizer can't fold anything away. When testing a whole OG function, write it in Borland-era C: `int` / `unsigned` (16-bit), `long` for 32-bit, segments as `unsigned`, and prototypes for every callee.

### 2. Write the DOS batch file

```bash
printf '@echo off\r\nset PATH=D:\\BIN\r\nbcc -mm -a -d -S -ID:\\INCLUDE RETTEST.C > OUT.TXT\r\n' > "$T/RUN.BAT"
```

- Add `-Y` for a function from an `ovrNNN` overlay.
- `> OUT.TXT` captures the banner and BCC's warnings, which are useful evidence too (e.g. "Function should return a value", "Suspicious pointer conversion").

### 3. Run BCC in DOSBox

```bash
cd "$T" && rm -f RETTEST.ASM OUT.TXT && /c/STU/WD/DOSBox/DOSBox.exe -noconsole \
  -c "mount c \"$(cygpath -w "$T")\"" \
  -c "mount d \"C:\\STU\\devel\\mom131-source-map\\tools\\BORLANDC\"" \
  -c "c:" -c "call run.bat" -c "exit" >/dev/null 2>&1
cat "$T/OUT.TXT"
```

- C: is the work folder and D: is the Borland C++ 3.1 install.
- DOSBox exits on its own when the batch file finishes, in a second or two.
- **One DOSBox session per compile.** Start a new session for every `bcc` run; do not put several compiles in one batch file. 3.1 keeps state between runs in one session, and large modules can come out with different register choices.
- DOSBox also writes `stdout.txt` / `stderr.txt` into the current directory (`$T`). Look there if `OUT.TXT` is missing.

### 4. Read the generated asm

```bash
grep -v '^\s*;\|^\s*$\|?debug' "$T/RETTEST.ASM" | sed -n '/_V_ret/,/_TEXT\tends/p'
```

This strips Borland's source-line comments and `?debug` records, leaving the procedures. Compare them with the OG function's `.asm` under `Piethawn/out/WIZARDS/segNNN/` or `ovrNNN/`, instruction by instruction, including the `bp` offsets of the locals and which variables sit in SI and DI.

## `-S` artifacts (not source differences)

These show up when comparing `-S` output with the OG asm, and come from the assembly text or the link, not from the C:

| `-S` output | OG asm | Why |
|---|---|---|
| `ret` inside `proc far` | `retf` | the assembler turns it into `retf` |
| `call far ptr _Func` | `nop` / `push cs` / `call near ptr Func` | the linker's far-call optimization for a callee in the same segment |
| `mov ax, 2` | `mov ax, s_FLIC_HDR.height` | IDA prints a struct-offset name for the same constant |
| `call far ptr F_LDIV@` | `call LDIV@` | the same far runtime helper (`proc LDIV@ far` in `WIZARDS.asm`), named differently |
| `jge @@0` / `jmp label` | `jl short label` | `-S` sizes this backward jump conservatively; the `-c` object has the OG's `jl short` (confirmed by the byte comparison below, `Draw_Bitmap_Animation_Block`, 2026-11-10) |

Anything else that differs is a difference in the source: change the candidate and compile again.

**Limit of `-S`:** comparing `-S` output with the IDA listing checks instructions (mnemonics, registers, `bp` offsets, constants), not bytes. For the last word, compare bytes (next section).

## Byte comparison against `WIZARDS.EXE`

Compile the candidate with `-c` instead of `-S` (`bcc -c -mm -a -d -ID:\INCLUDE NAME.C`), then compare the `.obj`'s code bytes with the OG bytes in the EXE:

1. **Find the function in the EXE by its bytes,** not by the listing's addresses: search `WIZARDS.EXE` for the prologue plus the first few instructions (for example `55 8B EC 83 EC 4E 56 57 8B 76 18` is `push bp / mov bp, sp / sub sp, 4Eh / push si / push di / mov si, [bp+18h]`; it should occur exactly once).
2. **Take the `.obj`'s code bytes** from its `LEDATA` records (OMF record type `A0h`; segment index 1 is `NAME_TEXT`), and the locations its `FIXUPP` records (`9Ch`) patch: call targets and data offsets the linker fills in.
3. **Compare byte for byte, skipping the fixup locations.** A function's switch jump tables follow its code in the same segment and are part of the comparison.

`TDUMP.EXE` (in the 3.00 install's `BIN/`; the 3.1 copy has none) prints an `.obj`'s records and segment lengths, which is enough to check sizes by hand.

Result so far: `Draw_Bitmap_Animation_Block` compiled with 3.1 `-c -mm -a -d` is **byte-identical** to `WIZARDS.EXE` outside its 135 fixups: 4,283 bytes, 4,251 of code and 32 of jump tables, in the EXE at file offset `13E7D` (the MoM 1.31 EXE, MD5 `4123a5e17b7829dcb4457bd3c83cce3d`).

**Don't measure a function's size from the listing's addresses.** For that function the Piethawn `WIZARDS.lst` addresses run `214B0`..`22517` (0x1068 bytes), but the function occupies 0x109B bytes in the EXE.

## What this has settled so far

- **`xor ax, ax` on an early exit is `return 0;` in an `int` function** (worked example below).
- **If/else vs ternary:** `Draw_Bitmap_Animation_Block` (s30p21) computes a divisor as `mov ax, X_2` / `jmp` / `mov ax, X_3`, then a shared `sub ax, X_1 ...`. That looks like a ternary, but a ternary compiled with 3.1 does not reproduce the function; four if/else blocks, one assignment in each branch, do (BCC merges their identical tails).
- **Declaration order and SI/DI:** in `Draw_Bitmap_Rotated` (s30p20), `height` declared after `width` puts `width` in DI; the OG has `height` in DI, so the source declares `height` before `width`.
- **When candidates compile identically, the asm cannot choose between them.** For example, a `register` keyword on a variable 3.1 would put in a register anyway, or `int` vs `unsigned` for a value that is only stored and passed on. Don't call either form "the original".

## Worked example: does `xor ax, ax` on an early exit mean `return 0`?

**Question.** Several OG functions (`Generate_Ore`, `ovr143/TILE_GenerateOre.asm`; `Road_Build_Path`, `ovr069/Road_Build_Path.asm`) have an early exit of `xor ax, ax` / `jmp @@Done`, and a normal path that falls into `@@Done` without setting AX. Was that `return 0;` in a non-void function with no final return, or something else?

**Test file** (`RETTEST.C`): four candidates with the same body.

```c
int g_flag;
int g_val;

void V_ret(int x)            /* void, bare return */
{
    if(g_flag == 0)
    {
        return;
    }
    g_val = x;
}

int I_ret0(int x)            /* int, return 0, no final return */
{
    if(g_flag == 0)
    {
        return 0;
    }
    g_val = x;
}

int I_retbare(int x)         /* int, bare return */
{
    if(g_flag == 0)
    {
        return;
    }
    g_val = x;
}

NOTYPE(x)                    /* K&R, implicit int, bare return */
int x;
{
    if(g_flag == 0)
    {
        return;
    }
    g_val = x;
}
```

**Command:** steps 2–4 above, with `bcc -mm -a -d -S`.

**BCC output (`OUT.TXT`):**
```
Borland C++  Version 3.1 Copyright (c) 1992 Borland International
rettest.c:
Warning rettest.c 20: Function should return a value in function I_ret0
Warning rettest.c 29: Function should return a value in function I_retbare
Warning rettest.c 39: Function should return a value in function NOTYPE
```

**Generated asm (`RETTEST.ASM`, trimmed):**
```
_V_ret      proc far                  _I_ret0     proc far
    push bp                               push bp
    mov  bp,sp                            mov  bp,sp
    cmp  word ptr DGROUP:_g_flag,0        cmp  word ptr DGROUP:_g_flag,0
    jne  short @1@86                      jne  short @2@86
    jmp  short @1@114                     xor  ax,ax
@1@86:                                    jmp  short @2@114
    mov  ax,word ptr [bp+6]           @2@86:
    mov  word ptr DGROUP:_g_val,ax        mov  ax,word ptr [bp+6]
@1@114:                                   mov  word ptr DGROUP:_g_val,ax
    pop  bp                           @2@114:
    ret                                   pop  bp
                                          ret
```
`_I_retbare` and `_NOTYPE` are the same shape as `_V_ret`: a bare `jmp`, no `xor ax,ax`. (BCC 3.00 gives the same four shapes for this test.)

**Result.**

| Source | Early-out codegen |
|---|---|
| `void`, `return;` | `jmp` only |
| `int`, `return 0;`, no final return | **`xor ax,ax` + `jmp`** |
| `int`, `return;` | `jmp` only |
| K&R implicit int, `return;` | `jmp` only |

- Only a return with the value 0 emits `xor ax,ax`. A bare `return;` never touches AX, whatever the declared return type.
- The `int` + `return 0;` variant also reproduces the rest of the OG shape: the normal path leaves AX holding whatever was last loaded and falls into the epilogue.
- Borland only *warns* about the missing final return, so this shape compiles as-is and is plausible throughout the OG code.
- **Limit of the evidence:** `return 0;`, `return FALSE;`, `return ST_FALSE;` and `return(0);` all compile to the same `xor ax,ax`. The bytes prove "a return of the constant 0", not which spelling.

**What the ReMoM reconstruction does with this.**
- The function becomes `int16_t`, in both the `.c` and the `.h` prototype.
- The early exit is `return 0;` (or the matching `ST_*` / `ts_*` name).
- There is no return at the end in the OG. ReMoM builds with `/WX`, and MSVC's C4715 ("not all control paths return a value") would fail the build, so the end gets a marked stand-in: `/* HACK */  return 0;`. `Generate_Ore` in [WZD_o143.c](../../MoM/src/WZD_o143.c) uses this pattern.
- This was tracked as items #24 and #30 in [Bugs-And-Quirks-Triage.md](../@CodeReview/Bugs-And-Quirks-Triage.md).

## Gotchas (each one cost a failed run)

- **Use relative paths inside `RUN.BAT`, and `cd` to C: first.** An absolute path in the batch file (`c:\bcc_out.txt`) came out as `c:cc_out.txt`, because the `\b` was eaten somewhere between `printf` and DOSBox's redirect parser. `OUT.TXT` relative to `C:` works.
- **The batch file needs CRLF line endings**, which is why `printf ... \r\n` is used instead of a heredoc.
- **In a `printf` format string, each DOS backslash is written `\\`.**
- **`mount` needs a Windows path.** `cygpath -w "$T"` converts the Git Bash path.
- **Delete the old `.ASM` and `OUT.TXT` before each run.** If the compile fails, a stale file from the previous run still looks like a result.

## About `mom131-source-map`

`/c/STU/devel/mom131-source-map/` is someone else's project. It holds the Borland C++ 3.1 install used here, and a C + asm reconstruction of MoM 1.31 that its authors say rebuilds `WIZARDS.EXE` and `MAGIC.EXE` byte-identical.

- **It is not an authority.** The authority is the OG asm. Use its source only for ideas of what to try, then test the idea against the asm with the steps above.
- **Its names are not SimTex's.** The only known SimTex names are MoO2's debug symbols (`ORION2.EXE`); ReMoM's other names are from drake178 and Jim Balcomb.
- Its build notes are where `-a`, `-d` and `-Y` come from.
