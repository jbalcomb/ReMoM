MAPGEN-Set_Upper_Lair_Guardian_Count.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Set_Upper_Lair_Guardian_Count.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Set_Upper_Lair_Guardian_Count.c

Init_New_Game()
    |-> Set_Upper_Lair_Guardian_Count();

---

# `Set_Upper_Lair_Guardian_Count` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Set_Upper_Lair_Guardian_Count` | [MAPGEN.c:675-685](../../MoM/src/MAPGEN.c#L675-L685) | Stamps each lair's *initial* guardian count into the **high nibble** of its two guard-count bytes, preserving the current (remaining) count in the low nibble. |

## Purpose

Each lair tracks two guardian stacks (`guard1_count`, `guard2_count`, both `uint8_t`, [MOM_DAT.h:1774](../../MoX/src/MOM_DAT.h#L1774) / [1776](../../MoX/src/MOM_DAT.h#L1776)). At generation time the count lives in the low nibble. This pass packs a copy of that value into the high nibble so the game retains a "had vs. left" record: the **low nibble = guardians remaining**, the **high nibble = guardians originally placed**. As the player whittles a lair down, only the low nibble decrements; the high nibble stays as the original total. It's the **last** lair-touching step in `Init_New_Game`.

**RNG-neutral: zero `Random()` calls** ([675-685](../../MoM/src/MAPGEN.c#L675-L685)) — does not affect new-game `Random()` alignment.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:649](../../MoM/src/MAPGEN.c#L649) | Followed by `gd_dump_lairs("30_Set_Upper_Lair_Guardian_Count_LR")` ([650](../../MoM/src/MAPGEN.c#L650)) — the final dump checkpoint of the pipeline. |

## How it works

Single loop over the lair records. For each record ([MAPGEN.c:681-682](../../MoM/src/MAPGEN.c#L681-L682)):

```
guard1_count = guard1_count | (guard1_count << 4);
guard2_count = guard2_count | (guard2_count << 4);
```

For a 4-bit count `g` (0..15): `g << 4` moves it to the high nibble (low nibble = 0), and the combine leaves the low nibble holding the original `g`. Result byte = `0xGG` where both nibbles equal `g`. Downstream "is this lair still guarded?" checks read only the low nibble (see the `s_LAIR` field comments at [MOM_DAT.h:1774](../../MoX/src/MOM_DAT.h#L1774)).

## Findings

### R1 — loop bound stops 3 records short of the asm ([MAPGEN.c:679](../../MoM/src/MAPGEN.c#L679))

The asm iterates the **full 102-record** lair array:

```
loc_438BC:
    cmp     itr_lairs, 102
    jl      short loc_43841
```

Production loops `itr_lairs < NUM_LAIRS`, but `NUM_LAIRS` is **HACK-redefined to 99** ([MOM_DEF.h:114](../../MoX/src/MOM_DEF.h#L114), carrying the author's own open question *"Why/When did I hack this?"* at [115](../../MoX/src/MOM_DEF.h#L115)). The original `102` survives as `NUM_LAIRS_102` ([MOM_DEF.h:113](../../MoX/src/MOM_DEF.h#L113)), and the `_LAIRS` heap array is physically sized for 102 — other loops that walk the whole array correctly use `NUM_LAIRS_102` ([MAPGEN.c:3198](../../MoM/src/MAPGEN.c#L3198), [5678](../../MoM/src/MAPGEN.c#L5678)).

Consequence: records **99, 100, 101 are skipped** — their high nibble is never stamped, so those lairs carry no "initial count" record. To be 1:1 with the disassembly, line 679 should use `NUM_LAIRS_102`. RNG-neutral, so there is no `Random()`-alignment impact — but it is a genuine state divergence from OG-MoM.

This is not isolated: the `NUM_LAIRS = 99` hack also reaches [MAPGEN.c:1186](../../MoM/src/MAPGEN.c#L1186), [1593](../../MoM/src/MAPGEN.c#L1593), and [6371](../../MoM/src/MAPGEN.c#L6371). Each of those should be audited against its own asm before the hack is removed.

### R2 — pack operator is `+` in the asm, `|` in production ([MAPGEN.c:681-682](../../MoM/src/MAPGEN.c#L681-L682))

The asm combines with **addition**, not bitwise-or:

```
mov     al, [es:bx+s_LAIR.guard1_count]   ; al = g
push    ax
mov     al, [es:bx+s_LAIR.guard1_count]   ; al = g
mov     cl, 4
shl     al, cl                            ; al = g << 4
pop     dx                                ; dl = g
add     dl, al                            ; dl = g + (g << 4)
... store dl
```

Production uses `g | (g << 4)`. For nibble values 0..15 the low nibble of `g` and the high nibble of `g << 4` do not overlap, so `+` and `|` give the identical byte; they diverge only if a guard count exceeds 15. Guard stacks are small, so this is behaviorally equivalent in practice — but structurally it is a deviation from the asm's `ADD`.

## Verification against the asm

- **Loop structure** — `xor si,si` init, body, `inc si`, `cmp si,102 / jl` tail-test: matches production except the bound (see R1).
- **guard1 / guard2** — two independent read → `shl ...,4` → combine → store sequences, in that order: matches (combine operator differs, see R2).
- **No `Random()`**, no other external calls — pure in-place mutation of `_LAIRS`.
- **`__GEMINI` corroboration** — the `#if 0` reference block ([MAPGEN.c:686-699](../../MoM/src/MAPGEN.c#L686-L699)) independently agrees with the asm against production on *both* findings: it loops `itr_lairs < 102` ([693](../../MoM/src/MAPGEN.c#L693)) and combines with `+` ([695-696](../../MoM/src/MAPGEN.c#L695-L696)). (GEMINI is a second opinion, not authority — but here it lines up with the disassembly bytes.)

## Sub-functions / external calls

- None. Reads and writes only `_LAIRS[itr_lairs].guard1_count` / `.guard2_count` ([MOM_DAT.h:1774](../../MoX/src/MOM_DAT.h#L1774) / [1776](../../MoX/src/MOM_DAT.h#L1776)).

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Set_Upper_Lair_Guardian_Count.asm` — IDA Pro 5.5 disassembly (authority); loop bound `102`, combine via `add dl, al`.
- [MAPGEN.c:649-650](../../MoM/src/MAPGEN.c#L649-L650) — call site + final lair dump.
- [MOM_DEF.h:112-115](../../MoX/src/MOM_DEF.h#L112-L115) — the `NUM_LAIRS` (99) vs `NUM_LAIRS_102` hack at the root of R1.
- [MAPGEN-Generate_Lairs.md](MAPGEN-Generate_Lairs.md) / [MAPGEN-Create_Lair.md](MAPGEN-Create_Lair.md) — where the low-nibble guard counts are first placed.
