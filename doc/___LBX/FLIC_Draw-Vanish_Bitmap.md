FLIC_Draw-Vanish_Bitmap.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg033\Vanish_Bitmap__WIP.asm    (ground truth — the vanish blit)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg033\RNG_GFX_Random__WIP.asm   (ground truth — the 32-bit seed LFSR)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg016\rnd_bitfiddle__1oom.asm   (ground truth — the 16-bit scramble LFSR)



RNG_GFX_Random__WIP()  ==>  Vanish_Bitmap_Dither_Seed()
rnd_bitfiddle__1oom()  ==>  Dither_Scramble_Seed()

_CS_GFX_RNG_TempSeed   ==>  m_vanish_bitmap_dither_seed
_CS_random_seed        ==>  m_dither_random_seed
_CS_GFX_RND100_Lookup  ==>  m_dither_threshold_matrix



In real-mode 16-bit Assembly, using the Code Segment override (mov [cs:_CS_GFX_RNG_TempSeed], ax) allowed developers to store data directly inside the executable code space instead of the Data Segment (ds). This was an optimization hack to save bytes and register swaps. In modern C, a static file-scoped variable is the absolute closest match to this concept. It restricts the variable's visibility exclusively to that translation unit (.c file), providing clean module encapsulation.



Regarding the LFSR functions (Dither_Scramble_Seed and Vanish_Bitmap_Dither_Seed), there are very clean mathematical definitions for these exact bit-manipulation loops. These functions are software implementations of Galois and Fibonacci Linear Feedback Shift Registers. In mathematical and digital signal processing codebases, the standard, meaningful nomenclature drops the generic x86 register names (ax, bx, cx, dx, si, di) and replaces them with standard LFSR terminology: State, Feedback Bit (or Tap), and Carry/Overflow flags.

1. The Maximum Bitmask Bounds ($0$ to $1023$):
The expression seed_lo & 0x3FFU clamps the lower word of the LFSR down to its lowest 10 bits. This guarantees a value between $0$ and $1023$.
2. The Sieve Range Limit ($0$ to $999$):
The subsequent while (bounded_seed >= range_limit) reduction loops using a range_limit argument of 1000 passed down from Vanish_Bitmap. This guarantees that the final returned value fits within a strict mathematical window of $0$ to $999$.
Why $1000$?
The engine forces the seed value to fall cleanly between $0$ and $999$ to create a fixed-point percentage baseline with three decimal places of resolution (steps of 0.1%).
Because the localized lookup table (m_dither_threshold_matrix) contains 256 pre-normalized values scaled exactly between 1 and 100 percent, this 0-999 seed is generated once per column to aggressively offset the starting row position. This acts as an initial pseudo-random phase shift, ensuring that every column samples the 1-100 threshold matrix at a highly dispersed phase offset relative to its neighbors. This mathematical decoupling is what eliminates vertical lines and ensures the dissolve effect remains completely uniform across the canvas, regardless of the target asset's actual dimensional boundaries.



MoO2
    Vanish_Bitmap_() |-> Vanish_Bitmap_Pixels_()
    Jumble_Bitmap_() |-> Jumble_Bitmap_Pixels_()
    ...shared
        _CS_percent
        m_dither_random_seed[]
        GFX_RND100_Lookup[]
    ...non-shared
        IDK_VanishBitmap_WidthHalf_1
        IDK_VanishBitmap_WidthHalf_2
        IDK_JumbleBitmap_source_bitmap
        IDK_JumbleBitmap_NegFlag



The purpose of _CS_GFX_RND100_Lookup is to act as an ordered dither matrix or a noise threshold map.
Instead of generating a raw random number for every single pixel on the fly (which would cause value clustering and require expensive calculations), the engine precomputes a uniform distribution of values normalized strictly between 1 and 100. As the engine steps line-by-line down a column, it evaluates this array like a repeating mathematical sieve: if the value in the table is less than or equal to the requested execution percentage, the pixel is marked for destruction (transparency). This technique guarantees a perfectly distributed grain dispersion without any clumps, patterns, or vertical stripes.
Given its structural purpose and your file-scoped module naming conventions, here are the most descriptive names:
m_dither_threshold_matrix (Recommended)
This is the most technically accurate name. In computer graphics, a "dither matrix" or "threshold matrix" is the standard term for a spatial lookup table used to convert continuous gradients or percentages into digital noise patterns.



CS_width
XREF:
    Mask_Bitmap_Pixels+1D
    Mask_Bitmap_Pixels+3E
    Mask_Bitmap_Pixels_Inverse+1D
    Mask_Bitmap_Pixels_Inverse+3E
    Reduce_Bitmap+58
    Reduce_Bitmap+9B
    Enlarge_Bitmap+72
    Enlarge_Bitmap+B9
    Vanish_Bitmap+23
    Vanish_Bitmap+63





// WZD s33p15
Vanish_Bitmap
Vanish_Bitmap__WIP

; deletes an amount of random pixels from an LBX image
; roughly corresponding to the specified percent (they
; are replaced with transparent pixels - color #00)
; uses three local RNGs, not the global one
; Attributes: bp-based frame
; int __cdecl __far Vanish_Bitmap__WIP(unsigned int bitmap, int percent)
proc Vanish_Bitmap__WIP far

Vanish_Bitmap__WIP()
XREF:
    Cast_Plane_Shift+252
    Cast_Plane_Shift+44C
    Cast_Plane_Shift+4A0
    Cast_Plane_Shift+629
    BU_Teleport+153
    BU_Teleport+1A8
    BU_TunnelTo+13D
    BU_TunnelTo+1F4
    BU_CombatSummon__SEGRAX+1BA
    Spell_Target_Global_Enchantment_Disjunct__WIP+1A5
    Spell_Of_Mastery_Lose_Draw+43F
    Cityscape_Draw_Buildings+25F
    Cityscape_Draw_Buildings+2D3
    Cityscape_Draw_Buildings+332
    Cityscape_Draw_Buildings+3A3
    Cityscape_Draw_Buildings+414
    Cityscape_Draw_Buildings+484
    Cityscape_Draw_Buildings+4FE
    Cityscape_Draw_Buildings+56E
    Cityscape_Draw_Buildings+5DF
    Cityscape_Draw_Buildings+673
    Draw_Map_Nodes+2F5



// WZD s33p19
/*
drake178:
    a secondary linear feedback shift register
    using the same feedback polynomial as RNG_Random,
    also shifted 9 states per call,
    but actually returning the lowest order 10 bits of the resulting state (1-1023)
    rather than the shifted out return value
*/
// WZD seg033:09E3 93 28                                           _CS_Local_LFSR_LO dw 2893h                  ; DATA XREF: RNG_GFX_Random__NOP+1Ar ...
// WZD seg033:09E5 0A 00                                           _CS_Local_LFSR_HI dw 0Ah                    ; DATA XREF: RNG_GFX_Random__NOP+1Fr ...
static uint16_t _CS_Local_LFSR_LO = 0x2893;
static uint16_t _CS_Local_LFSR_HI = 0x0A;
static uint32_t _bitmap_random_seed = 0x0A2893;  /* 665747 */
int16_t Vanish_Bitmap_Dither_Seed(int16_t max)

; a secondary linear feedback shift register using the
; same feedback polynomial as RNG_Random, also shifted
; 9 states per call, but actually returning the lowest
; order 10 bits of the resulting state (1-1023) rather
; than the shifted out return value
; Attributes: bp-based frame
; int __cdecl __far Vanish_Bitmap_Dither_Seed(int n)
proc Vanish_Bitmap_Dither_Seed far



Graphics.c
// WZD s16p07
// drake178: RNG_Direct_LFSR()
// MoO2  
// 1oom  rnd.c  rnd_bitfiddle()
uint16_t Dither_Scramble_Seed(uint16_t ax)

; modifies ax treated as a 16bit linear feedback shift
; register, that is advanced by 8 states
; int __far Dither_Scramble_Seed(void)
proc Dither_Scramble_Seed far



---

# `Vanish_Bitmap` + `Vanish_Bitmap_Dither_Seed` + `Dither_Scramble_Seed` — Review / Walkthrough

> **Status: DONE-DONE (2026-07-20) — faithful to the disassembly 1:1; builds clean (MSVC-debug).** The whole family — the in-place `Vanish_Bitmap` erase and both LFSRs — matches the asm instruction-for-instruction, cross-checked against the 57,599-vector-verified `Random()` twin (same LFSR polynomial family). The concepts primer below is kept as reference for anyone reading these routines; each function then has its asm mapping.

## 0. Concepts primer (what this code is doing)

**The effect.** `Vanish_Bitmap(bitmap, percent)` makes an image dissolve into transparency a grain at a time, like Wolfenstein 3D's "fizzlefade." At `percent = 100` the image is whole; as `percent` drops toward 0, more and more pixels are turned transparent (color `0`) in a scattered, clump-free pattern, until nothing is left.

**Two pieces make the scatter look random but *even*:**

1. **A dither threshold matrix** (`_CS_GFX_RND100_Lookup`, 256 bytes of values 1..100). Walking down a column, each pixel reads the next table entry as its "threshold." A pixel vanishes when its threshold is compared against `percent`. Because the table is a pre-shuffled uniform 1..100 distribution, you get an even grain with no stripes or clumps — cheaper and better-looking than calling a RNG per pixel.

2. **An LFSR to pick where each column starts reading the table.** A *linear-feedback shift register* is a cheap pseudo-random generator: hold a value, shift it one bit, and feed a new top bit computed by XOR-ing a few chosen bits ("taps"). Iterated, it walks through a long non-repeating sequence. Here the LFSR's output high byte becomes the starting index into the 256-byte matrix for each column, so every column dithers with a different phase.

**The two x86 idioms these LFSRs hinge on** (and the classic mistranslations to avoid):

- **`rcr reg, 1` — rotate right *through the carry flag*.** Unlike C's `>>`, `rcr` shifts the CPU's carry bit *into* the top of the register and shifts the register's bottom bit *out into* carry. LFSRs use this to chain registers: the bit shifted out of one register (captured in carry by a preceding `shr`) becomes the incoming top bit of the next. The correct C is *not* a self-rotate `((x>>1)|((x&1)<<15))` — that recycles the register's own bit. It must be `x = (x >> 1) | (incoming_carry << 15)`, where `incoming_carry` is the bit shifted out of the *previous* operation. Getting this wrong severs the feedback and the "LFSR" degenerates into a plain rotate of the seed.
- **`xor al, bh` — XOR the *low* byte of one register with the *high* byte of another.** In C this is `ax ^= (bx >> 8) & 0x00FF;` (bring bx's high byte down to the low position, XOR only ax's low byte). It is *not* `ax ^= bx` (that XORs bx's low byte) and *not* `ax ^= (bx & 0xFF00)` (that XORs ax's *high* byte). Since only the LFSR's bit 0 survives as the feedback bit, taking the wrong byte taps the wrong source bit and produces a different sequence.

## 1. The three functions (final, faithful state)

| Function | Location | asm | What it does |
|---|---|---|---|
| `Vanish_Bitmap(bitmap, percent)` | [FLIC_Draw.c:3712](../../MoX/src/FLIC_Draw.c#L3712) | `Vanish_Bitmap__WIP.asm` (s33p15) | In-place dissolve: per column, advance the 16-bit LFSR, take its high byte as the dither-matrix start index, and walk the column erasing (`ST_TRANSPARENT`) each pixel whose `m_dither_threshold_matrix[idx] > percent`. |
| `Vanish_Bitmap_Dither_Seed(n)` | [FLIC_Draw.c:3861](../../MoX/src/FLIC_Draw.c#L3861) | `RNG_GFX_Random__WIP.asm` (s33p19) | 32-bit secondary LFSR (`m_dither_random_seed`), 9 states, returns `(seed_lo & 0x3FF) % n`. Seeds each `Vanish_Bitmap` run. |
| `Dither_Scramble_Seed(seed)` | [Graphics.c:1100](../../MoX/src/Graphics.c#L1100) | `rnd_bitfiddle__1oom.asm` (s16p07) | 16-bit LFSR, 8 states — advances the per-column seed. |

The earlier register-style reconstructions (`Vanish_Bitmap__WIP`, `RNG_GFX_Random__WIP`, `rnd_bitfiddle__1oom`) and the duplicate `Dither_Scramble_Seed__take2` have been removed; the three above are the single live path.

## 2. `Vanish_Bitmap` — in-place erase

Signature is the OG's `Vanish_Bitmap(SAMB_ptr bitmap, int16_t percent)` — **one buffer, edited in place**. The caller draws the image into `bitmap` first (e.g. `Draw_Picture_To_Bitmap(..., GfxBuf_2400B)`); `Vanish_Bitmap` only turns some of those pixels transparent. There is no separate frame buffer and no screen coordinates. Faithful to `Vanish_Bitmap__WIP.asm`:

- outer `do { … } while(--CS_width)` per column (asm `dec [cs:CS_width] / jnz`; `CS_width` is the code-segment width the asm keeps);
- inner `do { … } while(--height)` per row (asm `loop`); `offset` walks linearly from `SZ_FLIC_HDR` (asm `inc di`);
- each column: `Dither_Scramble_Seed` advances the seed, `dither_idx = seed >> 8` (**full** high byte), and `dither_idx` is a `uint8_t` so it wraps 0..255 (asm `inc bx / xor bh,bh`);
- **erase polarity:** `if(m_dither_threshold_matrix[dither_idx] > percent) bitmap[offset] = ST_TRANSPARENT;` — writes `0` only on the vanish branch; kept pixels are left untouched (asm `cmp al,dl / jz/jb skip / xor al,al / stosb`). So `percent` = how much survives.

## 3. The two LFSRs — same polynomial, verified against `Random()`

Both seed generators belong to the SimTex LFSR family whose 32-bit form is `Random()` ([random.c:296](../../MoX/src/random.c#L296)) — a reconstruction **tested against 57,599 OG seed-transition vectors**. That makes `Random()` the gold-standard cross-check (it writes the feedback tap out explicitly: `low_seed ^ (low_seed>>1) ^ (low_seed>>2) ^ (low_seed>>4) ^ (low_seed>>6) ^ (high_seed>>15)`).

- **`Dither_Scramble_Seed` (16-bit, 8 states).** Per state: tap XORs after `working_bits` shifts 1/2/4/6, then the byte tap `lfsr_state ^= (working_bits >> 8) & 0x00FF` (= `xor al, bh`), then the carry-linked feedback `cf = lfsr_state & 1; feedback_tap = (feedback_tap >> 1) | (cf << 15); lfsr_state = feedback_tap;` (= `shr ax,1 / rcr dx,1 / mov ax,dx`). Feedback bit = `seed[0]^[1]^[2]^[4]^[6]^[15]`.
- **`Vanish_Bitmap_Dither_Seed` (32-bit, 9 states).** The `seed_hi:seed_lo` pair, each state chaining `shr dx,1 / rcr bx,1` for the taps and ending `shr ax,1 / rcr di,1 / rcr si,1` for the 32-bit shift-with-feedback; reseeds to `0x30BE` on all-zero state; returns `(seed_lo & 0x3FF) % n`. Feedback bit = `seed[0]^[1]^[2]^[4]^[6]^[31]` — identical to `Random()`'s.

Both were verified two ways: instruction-level against their asm, and by matching `Random()`'s vector-verified feedback polynomial. (MoO2 dropped `Vanish_Bitmap_Dither_Seed` and called `Random()` directly — the in-code note; the two share this polynomial and could share one core here as well, deferred.)

## Related references

- `…\seg033\Vanish_Bitmap__WIP.asm`, `…\seg033\RNG_GFX_Random__WIP.asm`, `…\seg016\rnd_bitfiddle__1oom.asm` — IDA Pro 5.5 disassembly (**the authority**).
- [`Random()`](../../MoX/src/random.c#L296) — the vector-verified 32-bit sibling of the same LFSR polynomial; the gold-standard cross-check for `Vanish_Bitmap_Dither_Seed`.
- drake178 (in-code): "deletes an amount of random pixels … replaced with transparent pixels (color #00) … uses three local RNGs, not the global one"; the seed LFSR "return[s] the lowest order 10 bits of the resulting state (1-1023)."
- `m_dither_threshold_matrix` — the 256-byte dither threshold matrix ([FLIC_Draw.c:3706](../../MoX/src/FLIC_Draw.c#L3706)); values 1..100, a pre-shuffled uniform distribution (asm `_CS_GFX_RND100_Lookup`).
- Callers of `Vanish_Bitmap` (2-arg, in-place): SPLMASTR, Spells132/133, CITYSCAP — see the asm XREF list (`Cast_Plane_Shift`, `BU_Teleport`, `Cityscape_Draw_Buildings`, `Draw_Map_Nodes`, `Spell_Of_Mastery_Lose_Draw`, …).
