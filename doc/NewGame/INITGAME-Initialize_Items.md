INITGAME-Initialize_Items.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Initialize_Items.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Initialize_Items.c

Init_Runtime()
    |-> Initialize_Items();

---

# `Initialize_Items` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Initialize_Items` | [INITGAME.c:790-807](../../MoM/src/INITGAME.c#L790-L807) | New-game reset of the item subsystem: zeroes every `_ITEMS[]` record's `cost`, then clears the entire `_prefab_item_table` byte table. |

`MGC` overlay 56 (asm `proc Initialize_Items far`).

## Purpose

Blanks the two item tables before new-game item generation runs. Setting each
`_ITEMS[i].cost = 0` marks all item-instance slots as empty/unused (cost is the
"is this slot live?" discriminator), and zeroing `_prefab_item_table` clears the
premade-item availability/usage table.

**RNG-neutral: zero `Random()` calls** ([790-807](../../MoM/src/INITGAME.c#L790-L807)). Does not affect new-game `Random()` alignment.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_Runtime` / INITGAME | [INITGAME.c:637](../../MoM/src/INITGAME.c#L637) | `gd_dump_items("200_Initialize_Items_I")` is a `// TODO` ([638](../../MoM/src/INITGAME.c#L638)), not yet wired. |

## How it works

Two independent zeroing loops:

1. **Item costs** ([796-799](../../MoM/src/INITGAME.c#L796-L799)) — `for itr in 0..137`: `_ITEMS[itr].cost = 0`. `_ITEMS` is `struct s_ITEM *` ([MOM_DAT.h:3075](../../MoX/src/MOM_DAT.h#L3075)), so the write is `s_ITEM`-strided.
2. **Premade-item table** ([800-803](../../MoM/src/INITGAME.c#L800-L803)) — `for itr in 0..249`: `_prefab_item_table[itr] = 0`. `_prefab_item_table` is `SAMB_ptr` = `unsigned char *` ([MOX_TYPE.h:26](../../MoX/src/MOX_TYPE.h#L26)), so the write is byte-strided.

The loop bounds `138` and `250` are literal in both production and the asm (no
named constants exist for them).

## Verification against the asm

| Item | OG asm | Production |
| --- | --- | --- |
| Cost loop bound | `cmp cx, 138 / jl` → `0..137` | `itr < 138` ✓ |
| Cost write | `[es:bx + s_ITEM.cost] = 0`, `bx = _ITEMS + cx*size s_ITEM` | `_ITEMS[itr].cost = 0` (struct-strided) ✓ |
| Premade loop bound | `cmp cx, 250 / jl` → `0..249` | `itr < 250` ✓ |
| Premade write | `add bx, cx` (byte stride) → `[byte ptr es:bx] = 0` | `_prefab_item_table[itr] = 0` (`SAMB_ptr` = 1-byte element) ✓ |
| RNG | none | none ✓ |

Both loops are 1:1 with the disassembly (init `xor cx,cx`, `jl`-tested ascending
bound, post-increment); no findings.

## Sub-functions / external calls

- None. Writes only `_ITEMS[].cost` ([MOM_DAT.h:3075](../../MoX/src/MOM_DAT.h#L3075)) and `_prefab_item_table` ([MOM_DAT.h:3078](../../MoX/src/MOM_DAT.h#L3078)).

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Initialize_Items.asm` — IDA Pro 5.5 disassembly (authority); two zeroing loops (`loc_52675` costs, `loc_52693` premade).
- [INITGAME.c:637-638](../../MoM/src/INITGAME.c#L637-L638) — call site (item dump is a pending TODO).
- [INITGAME-Init_Players.md](INITGAME-Init_Players.md) / [INITGAME-Init_Heroes.md](INITGAME-Init_Heroes.md) — sibling `Init_Runtime` steps.
