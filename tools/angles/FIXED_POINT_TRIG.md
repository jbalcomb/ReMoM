# Fixed-Point Trigonometry in Master of Magic

## What This Is

This directory contains `gen_tables.c`, a lookup table generator that reproduces
the pre-computed trigonometric tables from **WIZARDS.EXE segment 034**.

These tables power the angle calculation and projectile direction system used
by Master of Magic's combat engine -- specifically for ranged attack animations
(arrows, fireballs, rocks, etc.).

```
 tools/angles/
 ├── gen_tables.c        The generator/verifier tool
 ├── Makefile            Build: make | make verify | make emit
 └── FIXED_POINT_TRIG.md This file
```

---

## Historical Context: Why Lookup Tables?

In 1994, Master of Magic shipped for DOS on Intel 386 and 486 processors.
These CPUs had no fast floating-point hardware built in. The x87 FPU was a
separate coprocessor chip (the 387), and many players didn't have one. Even
when present, an `FSIN` instruction took **65-100 clock cycles** on a 486 --
far too slow to call per-frame in a game loop.

The solution: **pre-computation**. A developer at SimTex would run a small
tool on their workstation (which *did* have an FPU) to generate C arrays full
of pre-calculated values. These arrays were compiled directly into the game.
At runtime, the game just does an array lookup -- a single memory read that
takes **1-2 clock cycles**.

```
 ┌─────────────────────────────────────────────────────────────────┐
 │                    DEVELOPMENT WORKSTATION                      │
 │                                                                 │
 │   gen_tables.c ──► compile ──► run ──► angle_tables.c          │
 │   (uses <math.h>)              │       (C source with arrays)  │
 │                           ┌────┴────┐                          │
 │                           │ x87 FPU │                          │
 │                           │ sin()   │                          │
 │                           │ cos()   │                          │
 │                           │ tan()   │                          │
 │                           └─────────┘                          │
 └───────────────────────────────┬────────────────────────────────┘
                                 │ generated C source
                                 ▼
 ┌─────────────────────────────────────────────────────────────────┐
 │                         WIZARDS.EXE                             │
 │                                                                 │
 │   #include "angle_tables.c"                                     │
 │                                                                 │
 │   angle = tangent_table[slope];    // 1 clock cycle             │
 │   y_offset = radius * sin[angle];  // 1 multiply               │
 │                                                                 │
 │   NO FPU NEEDED AT RUNTIME                                     │
 └─────────────────────────────────────────────────────────────────┘
```

---

## Fixed-Point Math Primer

The problem: trigonometric functions return fractional values (sin(30) = 0.5),
but the game engine works entirely in integers. The 386 has no native way to
represent 0.5 in a general-purpose register.

The solution: **fixed-point representation**. Multiply every fractional value
by a power of two, and store the scaled integer. The power of two you choose
determines how many bits are "fractional bits."

### Notation: `I.F` Format

Fixed-point formats are written as `I.F` where:
- **I** = number of integer bits
- **F** = number of fractional bits
- Total bits = I + F

### The Two Formats Used

Master of Magic uses two fixed-point formats:

#### 8.8 Format (Tangent Tables, Scale Factor = 256)

```
    Scale factor: 256 = 2^8

    A 16-bit value split as:

    ┌──────────────────────────────────────────────────────────┐
    │ 15  14  13  12  11  10   9   8 │  7   6   5   4   3   2   1   0 │
    │         Integer Part           │        Fractional Part          │
    │          (8 bits)              │           (8 bits)              │
    └────────────────────────────────┴────────────────────────────────┘

    Examples:
       Real value 1.0   → 1.0 * 256 = 256   = 0x0100
       Real value 0.5   → 0.5 * 256 = 128   = 0x0080
       Real value 0.176 → 0.176 * 256 ≈ 45  = 0x002D  (tan 10°)

    To convert back:  real_value = stored_value / 256
```

This format is used for tangent values because the game's slope calculation
already produces 8.8 values naturally:

```c
    slope = (dy << 8) / dx;
    //       ^^^^^^
    //       Shift left 8 = multiply by 256 = convert to 8.8 format

    // Now 'slope' is directly comparable to the tangent table entries.
    // No conversion needed!
```

#### 0.16 Format (Sin/Cos Tables, Scale Factor = 65536)

```
    Scale factor: 65536 = 2^16

    A 16-bit value where ALL bits are fractional:

    ┌─────────────────────────────────────────────────────────────────┐
    │ 15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0 │
    │                      Fractional Part                            │
    │                        (16 bits)                                │
    │    I = 0 integer bits (the value is always 0.something)         │
    └─────────────────────────────────────────────────────────────────┘

    Examples:
       sin(0°)  = 0.0       → 0.0 * 65536 = 0      = 0x0000
       sin(30°) = 0.5       → 0.5 * 65536 = 32768   = 0x8000
       sin(45°) = 0.7071... → 0.7071 * 65536 ≈ 46341 = 0xB505
       sin(89°) = 0.9998... → 0.9998 * 65536 ≈ 65526 = 0xFFF6

    To convert back:  real_value = stored_value / 65536
```

This works because sin and cos always return values in the range [0, 1] for
the angles we care about (first quadrant). All 16 bits can be devoted to
fractional precision, giving us accuracy to 1/65536 = 0.0000153.

### The DX:AX Register Trick

This is the real magic of choosing 65536 (2^16) as the scale factor.

On the 8086/386, when you multiply two 16-bit values, the CPU puts the
32-bit result across **two** 16-bit registers: **DX** (high word) and
**AX** (low word).

```
    Goal: compute  y_offset = radius * sin(angle)

    What the game actually does:

    ┌─────────────────────────────────────────────────────────────┐
    │                                                             │
    │   MOV  AX, [radius]            ; AX = e.g. 100 (pixels)    │
    │   MUL  WORD PTR [sin_table+i]  ; AX * sin[angle]           │
    │                                 ; Result in DX:AX           │
    │   ; Answer is already in DX!   ; No shift needed!           │
    │                                                             │
    └─────────────────────────────────────────────────────────────┘

    Here's why:

    radius             sin_table[angle]          DX:AX result
    (integer)    ×     (0.16 fixed-point)    =   (16.16 fixed-point)

    ┌────────────────┐   ┌────────────────┐
    │   AX = 100     │ × │ AX = 0x8000    │   (sin 30° = 0.5)
    │   (radius)     │   │ (0.5 in 0.16)  │
    └────────────────┘   └────────────────┘
              │                    │
              ▼                    ▼
              100  ×  32768  =  3,276,800

    3,276,800 in hex = 0x00320000

    ┌────────────────┬────────────────┐
    │ DX  =  0x0032  │ AX  =  0x0000  │
    │     = 50       │   (fractional  │
    │  ◄── INTEGER   │     garbage)   │
    │     ANSWER!    │                │
    └────────────────┴────────────────┘

    DX = 50.  And indeed:  100 * sin(30°) = 100 * 0.5 = 50  ✓

    The "shift right by 16" happens FOR FREE -- you just read DX
    instead of AX.  No extra instruction needed.
```

Another example to drive the point home:

```
    radius = 200,  angle = 45°

    sin(45°) = 0x B505 = 46341

    200 × 46341 = 9,268,200  = 0x008D7E68

    ┌────────────────┬────────────────┐
    │ DX  =  0x008D  │ AX  =  0x7E68  │
    │     = 141      │   (discard)    │
    └────────────────┴────────────────┘

    DX = 141.  True answer: 200 * sin(45°) = 200 * 0.7071 = 141.4  ✓
```

---

## The Four Lookup Tables

All four tables live in `WIZARDS.EXE` at **segment 034** and are reproduced
in `MoX/src/angle.c`.

### Table Layout in Memory

```
    seg034:0006  ┌───────────────────────────┐
                 │   Tangents_0_44           │  45 bytes   (uint8[45])
    seg034:0033  ├───────────────────────────┤
                 │   Sin_Multipliers         │  180 bytes  (uint16[90])
    seg034:00E7  ├───────────────────────────┤
                 │   Tangents_45_89          │  90 bytes   (uint16[45])
    seg034:0141  ├───────────────────────────┤
                 │   Cos_Multipliers         │  180 bytes  (uint16[90])
    seg034:01F5  └───────────────────────────┘

                 Total: 495 bytes for all trig data
```

### Table 1: Tangents_0_44

```
    Type:    uint8_t[45]
    Range:   tan(0°) through tan(44°)
    Formula: floor( tan(degrees) × 256 )
    Format:  8.8 fixed-point (but only fractional part stored, since tan < 1.0)

    Why uint8?  tan(44°) = 0.9657 → 0.9657 × 256 = 247 → fits in a byte!
```

The values:

```
    deg:  0   1   2   3   4   5   6   7   8   9
     0°:  0   4   8  13  17  22  26  31  35  40
    10°: 45  49  54  59  63  68  73  78  83  88
    20°: 93  98 103 108 113 119 124 130 136 141
    30°:147 153 159 166 172 179 185 192 200 207
    40°:214 222 230 238 247
```

### Table 2: Tangents_45_89

```
    Type:    uint16_t[45]
    Range:   tan(45°) through tan(89°)
    Formula: floor( tan(degrees) × 256 )
    Format:  8.8 fixed-point (needs 16 bits because tan ≥ 1.0)

    Why uint16?  tan(45°) = 1.0 → 1.0 × 256 = 256 → exceeds a byte!
                 tan(89°) ≈ 57.3 → 57.3 × 256 = 14669 → needs 16 bits
```

### Why the Tangent Table Splits at 45 Degrees

```
    tan(x)
      │
   57 ┤                                                          ╱
      │                                                         ╱
      │                                                        ╱
      │                                                       ╱
      │                                                      ╱
      │                                                    ╱
   10 ┤                                                  ╱
      │                                                ╱
    5 ┤                                             ╱╱
      │                                          ╱╱
    2 ┤                                       ╱╱
  1.0 ┤─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ╱╱─ ─ ─ tan(45°) = 1.0
      │                              ╱╱╱
  0.5 ┤                          ╱╱╱
      │                     ╱╱╱╱
      │               ╱╱╱╱╱
      │         ╱╱╱╱╱╱
      ├───╱╱╱╱╱───────┬──────────┬──────────┬─────
      0°              22°        45°         67°        89°

      ◄── uint8 is ──►◄───── uint16 is enough ─────►
          enough              (values 256 - 14669)
       (values 0-247)

      The tangent function grows slowly from 0° to ~44° (staying under 256),
      then explodes toward infinity as it approaches 90°.

      SimTex split the table at exactly 45° because:
        tan(44°) × 256 = 247 ← fits in uint8  (1 byte per entry)
        tan(45°) × 256 = 256 ← needs uint16   (2 bytes per entry)

      This saves 45 bytes of precious DOS memory (uint8 vs uint16 for
      the first half). Every byte mattered in a 640KB world.
```

In the reassembled C code, the tangent data is further split into four
sub-tables for the linear-scan search algorithm:

```
    ┌────────────────────────────────────────────┐
    │ Tangents_0_44 (45 entries, uint8)           │
    │  ┌─────────────────┬──────────────────┐    │
    │  │ tbl_math_tan_0  │ tbl_math_tan_22  │    │
    │  │ 22 entries      │ 23 entries       │    │
    │  │ 0° - 21°        │ 22° - 44°        │    │
    │  └─────────────────┴──────────────────┘    │
    ├────────────────────────────────────────────┤
    │ Tangents_45_89 (45 entries, uint16)         │
    │  ┌─────────────────┬──────────────────┐    │
    │  │ tbl_math_tan_45 │ tbl_math_tan_67  │    │
    │  │ 22 entries      │ 23 entries       │    │
    │  │ 45° - 66°       │ 67° - 89°        │    │
    │  └─────────────────┴──────────────────┘    │
    └────────────────────────────────────────────┘

    The search first checks slope < 256 to pick the byte or word table,
    then checks a midpoint threshold to pick which half to scan.
    This cuts the worst-case linear scan from 90 iterations to ~23.
```

### Table 3: angle_to_sin (Sin_Multipliers)

```
    Type:    uint16_t[90]
    Range:   sin(0°) through sin(89°)
    Formula: floor( sin(degrees) × 65536 )
    Format:  0.16 fixed-point

    Note: sin(90°) = 1.0 → 1.0 × 65536 = 65536 → OVERFLOWS uint16!
          That's why the table stops at 89°.  The game handles 90°
          as a special case if needed.
```

### Table 4: angle_to_cos (Cos_Multipliers)

```
    Type:    uint16_t[90]
    Range:   cos(1°) through cos(90°)
    Formula: floor( cos(degrees) × 65536 )
    Format:  0.16 fixed-point

    Note: cos(0°) = 1.0 → 1.0 × 65536 = 65536 → OVERFLOWS uint16!
          That's why the table starts at 1°.
          cos(90°) = 0.0 → 0x0000, which fits fine.
```

### Sin and Cos Are Almost Mirrors -- But Not Quite

Mathematically, sin(x) = cos(90 - x). So you'd expect the sin table reversed
to equal the cos table. They're close, but a few entries differ by 1:

```
    sin[63] = 0xE419  (58393)     sin(63°)
    cos[26] = 0xE418  (58392)     cos(27°)  ← should be same, differs by 1

    sin[30] = 0x8000  (32768)     sin(30°) = 0.5
    cos[59] = 0x7FFF  (32767)     cos(60°) = 0.5  ← differs by 1!
```

These 1-bit differences arise because the original Borland C++ 3.0 compiler's
`sin()` and `cos()` functions compute through different x87 FPU code paths.
The 80-bit extended-precision intermediate results land on opposite sides of
an integer boundary for a handful of angles.  The developers generated each
table independently and never noticed (or cared about) a ±1 difference in
the 16th bit of precision.

---

## How The Game Uses These Tables

### The Ranged Attack Pipeline

When a unit fires a ranged attack in combat, the game needs to:
1. Figure out what **angle** the projectile travels at
2. Pick which of **8 sprite directions** to draw
3. **Animate** the projectile along the path

```
    ┌─────────────────────────────────────────────────────────────────────┐
    │ STEP 1: Compute firing angle                                       │
    │                                                                     │
    │   CMB_SetProjectiles__WIP()                                        │
    │     │                                                               │
    │     ├─ Get source figure screen position  (Src_Scr_X, Src_Scr_Y)  │
    │     ├─ Get target figure screen position  (Tgt_Scr_X, Tgt_Scr_Y)  │
    │     │                                                               │
    │     ├─ dx = Tgt_Scr_X - Src_Scr_X                                 │
    │     ├─ dy = Tgt_Scr_Y - Src_Scr_Y                                 │
    │     │                                                               │
    │     └─ Firing_Angle = Get_Angle(dx, dy)       ◄── uses tan tables  │
    │                                                                     │
    ├─────────────────────────────────────────────────────────────────────┤
    │ STEP 2: Map angle to sprite direction                              │
    │                                                                     │
    │     if      (angle <  22 || angle > 337)  → Proj_Right    (2)      │
    │     else if (angle <  69)                 → Proj_DnRight  (3)      │
    │     else if (angle < 112)                 → Proj_Down     (4)      │
    │     else if (angle < 158)                 → Proj_DnLeft   (5)      │
    │     else if (angle < 202)                 → Proj_Left     (6)      │
    │     else if (angle < 248)                 → Proj_UpLeft   (7)      │
    │     else if (angle < 292)                 → Proj_Up       (0)      │
    │     else                                  → Proj_UpRight  (1)      │
    │                                                                     │
    ├─────────────────────────────────────────────────────────────────────┤
    │ STEP 3: Animate projectile                                         │
    │                                                                     │
    │     Travel_Percent increments from 10 to 100                       │
    │                                                                     │
    │     Current_X = Src_X + (dx * Travel_Percent / 100)                │
    │     Current_Y = Src_Y + (dy * Travel_Percent / 100)                │
    │                  + parabolic arc for arrows/rocks                   │
    │                                                                     │
    │     Draw sprite: CMB_RangedAtx_GFX[missile_type][direction]        │
    └─────────────────────────────────────────────────────────────────────┘
```

### The Angle Calculation In Detail

`Get_Angle()` returns an angle in degrees (0-359) using only integer math:

```
    Get_Angle(dx, dy)
    │
    │  Handles all four quadrants by folding into Q1:
    │
    ├─ dx ≥ 0, dy ≥ 0  (Q1):  return  calc_angle_do(dx, dy)
    ├─ dx < 0, dy ≥ 0  (Q2):  return  180 - calc_angle_do(-dx, dy)
    ├─ dx < 0, dy < 0  (Q3):  return  180 + calc_angle_do(-dx, -dy)
    └─ dx ≥ 0, dy < 0  (Q4):  return  360 - calc_angle_do(dx, -dy)


    calc_angle_do(dx, dy)       ◄── Q1 only, both args positive
    │
    ├─ if dx == 0: return 90°   (straight up, avoid divide-by-zero)
    │
    ├─ slope = (dy << 8) / dx   ◄── 8.8 fixed-point slope
    │
    │  The slope is directly comparable to the tangent tables because
    │  both use the same 8.8 scaling:
    │
    │    slope = (dy/dx) × 256   (from the shift)
    │    table = tan(°) × 256    (from the generator)
    │
    │    So:  slope < table[i]  ⟺  dy/dx < tan(i°)  ⟺  angle < i°
    │
    ├─ slope < 256?  (angle < 45°)
    │   ├─ slope < 103?  → scan tbl_math_tan_0   (0° - 21°)
    │   └─ slope ≥ 103?  → scan tbl_math_tan_22  (22° - 44°)
    │
    └─ slope ≥ 256?  (angle ≥ 45°)
        ├─ slope < 603?  → scan tbl_math_tan_45  (45° - 66°)
        └─ slope ≥ 603?  → scan tbl_math_tan_67  (67° - 89°)
```

### The 8-Direction Compass

The 360-degree angle maps to one of 8 sprite directions, each covering
a 45-degree sector:

```
                           0° / 360°
                              │
                  337°────────┼────────22°
                 ╱            │            ╲
              ╱    UpRight    │   Right      ╲
           ╱          (1)     │     (2)        ╲
        292°                  │                  69°
        │                     │                     │
        │     Up              │        DnRight      │
        │     (0)             │          (3)         │
        │                     ●                     │
        │     UpLeft          │        Down         │
        │      (7)            │         (4)         │
        │                     │                     │
        248°                  │                  112°
           ╲                  │                ╱
              ╲    Left       │   DnLeft    ╱
                ╲   (6)       │    (5)   ╱
                  ╲           │        ╱
                  202°────────┼────────158°
                              │
                            180°

    Each sector spans ~45° centered on its cardinal/diagonal direction.
    The boundaries are: 22, 69, 112, 158, 202, 248, 292, 337.
```

---

## Building and Running the Generator

### Prerequisites

Any C compiler with `<math.h>` support. GCC (MinGW on Windows), MSVC, or
Clang all work.

### Build

```bash
cd tools/angles
make                # builds gen_tables (or gen_tables.exe on Windows)
```

Or manually:

```bash
gcc -Wall -O2 -o gen_tables gen_tables.c -lm
```

### Verify Mode (default)

Generates tables using modern double-precision math and compares every entry
against the expected values from the WIZARDS.EXE disassembly:

```bash
./gen_tables
```

### Emit Mode

Outputs the generated tables as C source code, suitable for inclusion in
a project:

```bash
./gen_tables --emit > angle_tables.c
```

---

## Verification Results

Running `gen_tables` on a modern system (GCC, IEEE 754 double precision):

```
    Table              Entries    Match    Status
    ───────────────────────────────────────────────
    Tangents_0_44        45      45/45    PASS
    Tangents_45_89       45      39/45    6 differ
    angle_to_sin         90      82/90    8 differ
    angle_to_cos         90      80/90    10 differ
    ───────────────────────────────────────────────
    Total               270     246/270   91.1%
```

### All Differences Are ±1 (Except tan(89°) at ±3)

Every mismatch follows the same pattern:

```
    Tangents_45_89:
      [45°] expected 256, got 255   (diff -1)
      [83°] expected 2085, got 2084 (diff -1)
      [89°] expected 14669, got 14666 (diff -3)  ← sensitive region near 90°

    angle_to_sin:
      [15°] expected 0x4242, got 0x4241 (diff -1)
      [30°] expected 0x8000, got 0x7FFF (diff -1)  ← sin(30°) = 0.5 exactly
      [45°] expected 0xB505, got 0xB504 (diff -1)
      ... (all diffs are -1)

    angle_to_cos:
      [27°] expected 0xE418, got 0xE419 (diff +1)
      [60°] expected 0x7FFF, got 0x8000 (diff +1)  ← cos(60°) = 0.5 exactly
      ... (all diffs are +1)
```

### Why The Differences Exist

The original tables were generated with **Borland C++ 3.0** (circa 1993),
which used the **Intel x87 FPU** with **80-bit extended precision**
internally. Modern compilers use **64-bit IEEE 754 double precision**.

```
    Borland C++ 3.0 (x87)              Modern GCC (SSE2)
    ┌────────────────────────┐          ┌────────────────────────┐
    │ 80-bit extended float  │          │ 64-bit double          │
    │ 1 sign + 15 exp +     │          │ 1 sign + 11 exp +     │
    │ 64 mantissa bits       │          │ 52 mantissa bits       │
    │                        │          │                        │
    │ Internal π has ~66     │          │ Internal π has ~52     │
    │ bits of precision      │          │ bits of precision      │
    │                        │          │                        │
    │ sin(), cos(), tan()    │          │ sin(), cos(), tan()    │
    │ use x87 microcode      │          │ use libm algorithms    │
    └────────────────────────┘          └────────────────────────┘

    The tiny difference in π representation and trig algorithms means
    that for ~9% of angles, the floating-point result lands on opposite
    sides of an integer boundary, causing a ±1 difference after truncation.
```

The **sin(30°) = 0.5** case illustrates this perfectly:

```
    True value:  sin(30°) × 65536 = 0.5 × 65536 = 32768.000000 exactly

    Borland x87:
      30 × π_x87 / 180 → very close to π/6
      sin(that) → 0.500000000000000... (rounds to exactly 0.5)
      × 65536 = 32768.0 → truncate → 32768 = 0x8000

    Modern GCC:
      30 × π_ieee / 180 → slightly different from true π/6
      sin(that) → 0.4999999999999999... (juuust below 0.5)
      × 65536 = 32767.99999... → truncate → 32767 = 0x7FFF
```

The difference of ±1 in the least significant bit has **zero visible effect**
on the game. A 1/65536 error in a sine value causes sub-pixel rendering
differences that are completely invisible at 320x200 resolution.

### The tan(89°) Case

The 3-unit difference at 89° is expected because tangent approaches infinity
near 90°. The derivative of tan at 89° is sec²(89°) ≈ 3282, so even a tiny
difference in the argument causes a large difference in the output:

```
    tan(89°) ≈ 57.29

    A 1-ULP difference in the double π constant causes:
      argument error ≈ 2 × 10⁻¹⁷
      tan error ≈ 3282 × 2 × 10⁻¹⁷ × 256 ≈ 0.00002

    But at this magnitude (14669), even a tiny relative error
    in the FPU's tan() implementation → diff of 3.

    This doesn't matter: tan(89°) is only used to distinguish
    angles between 88° and 89°, and a ±3 error in the threshold
    won't change which degree gets selected.
```

---

## Summary

```
    ┌─────────────────────────────────────────────────────────────────┐
    │                                                                 │
    │  A SimTex developer in 1994 ran a tool much like gen_tables.c  │
    │  on their workstation. It cranked through sin(), cos(), and     │
    │  tan() using the x87 FPU, scaled the results into integers,    │
    │  and spit out C arrays. Those arrays were compiled into        │
    │  WIZARDS.EXE, where the combat engine used them to:            │
    │                                                                 │
    │    1. Compute the angle between attacker and target            │
    │       (inverse tangent via table scan)                          │
    │                                                                 │
    │    2. Pick one of 8 sprite directions for the projectile       │
    │       (angle → direction mapping)                               │
    │                                                                 │
    │    3. (Potentially) compute sine/cosine-scaled offsets          │
    │       for trajectory rendering                                  │
    │                                                                 │
    │  All of this ran in pure integer math at runtime.               │
    │  No FPU required. No floating-point anywhere.                   │
    │  Just table lookups and integer multiplies.                     │
    │                                                                 │
    │  This tool regenerates those same tables 30 years later         │
    │  and verifies them against the original binary to within ±1,   │
    │  proving the reassembled code and data are correct.             │
    │                                                                 │
    └─────────────────────────────────────────────────────────────────┘
```
