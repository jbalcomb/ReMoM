AIBUILD-Player_Colony_Autobuild_NP.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr157\Player_Colony_Autobuild_NP.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr157\Player_Colony_Autobuild_NP.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()

Player_All_Colony_Autobuild()
    |-> Player_Colony_Autobuild_NP()

---

# `Player_Colony_Autobuild_NP` — Walkthrough

**Location:** [AIBUILD.c:442-530](../../MoM/src/AIBUILD.c#L442-L530) (~89 lines, ends [line 530](../../MoM/src/AIBUILD.c#L530)).
**WZD overlay:** ovr157, p03 (drake178 IDA label: `AI_CTY_NeutralProd`).
**Called from:** [`Player_All_Colony_Autobuild`](../../MoM/src/AIBUILD.c#L33) — per neutral-owned city.

> Verified against the **`.asm`** (336 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. Production is now 1:1 with the asm — the four reconstruction divergences found during review (inverted `need_units`, the unit-weight square, the bogus `_UNITS[].type` indirection on Transport/Construction, and storing the choice index instead of the product) have all been repaired.

## Purpose

Picks the next construction project for a **neutral** city. Much simpler than `_CP`/`_HP`: neutrals have no wizard Objective, so building weights are the flat per-category base only. It excludes every unit that is a settler / engineer / transport (neutrals build only plain combat units, and only when the square is under-garrisoned), zeroes the two naval guilds, forces Barracks first if unbuilt, and otherwise makes a weighted pick.

## Signature

```c
void Player_Colony_Autobuild_NP(int16_t city_idx)
```

## Code walk

Line refs are production [AIBUILD.c](../../MoM/src/AIBUILD.c); cross-checked against `Player_Colony_Autobuild_NP.asm`.

1. **Product array + `uu_city_landmass`** ([453-454](../../MoM/src/AIBUILD.c#L453-L454), asm:19-63) — `Calculate_Product_Array`; the landmass is computed and stored but never read (`uu_` dead store, faithful — OG does the same).
2. **`need_units`** ([455-462](../../MoM/src/AIBUILD.c#L455-L462), asm:64-104) — `TRUE` iff `Map_Square_Unit_Count(…) < (5 + Random(15))` (few units on the square → needs units). Written as `if((5+Random(15)) <= count) FALSE; else TRUE`, matching asm's `cmp count, threshold; jge → FALSE`.
3. **Base weights** ([463-477](../../MoM/src/AIBUILD.c#L463-L477), asm:108-166) — units `(cost/10)²`, cost = `_unit_type_table[product_array[itr] - 100].cost`; buildings `ai_build_base_weights_table[Category]` **only** (no objective term — neutrals have no Objective).
4. **Unit-exclusion filter** ([478-507](../../MoM/src/AIBUILD.c#L478-L507), asm:178-253):
   - Building & `need_units == ST_TRUE` → weight 0 ([482](../../MoM/src/AIBUILD.c#L482), asm:246-253).
   - Unit that is settler (`UA_CREATEOUTPOST`) / transport (`Transport != 0`) / engineer (`Construction != 0`) → weight 0 ([489-498](../../MoM/src/AIBUILD.c#L489-L498), asm:187-233), all three fields read directly off `_unit_type_table[product_array[itr] - 100]`.
   - Plain combat unit & `need_units == ST_FALSE` → weight 0 ([499-505](../../MoM/src/AIBUILD.c#L499-L505), asm:235-242).
5. **Naval-guild zeroing** ([508-518](../../MoM/src/AIBUILD.c#L508-L518), asm:265-293) — `bt_ShipWrightsGuild` and `bt_MaritimeGuild` weights forced to 0.
6. **Barracks-first, else weighted pick** ([519-527](../../MoM/src/AIBUILD.c#L519-L527), asm:294-329) — if `bt_Barracks` is `bs_NotBuilt`, build it; else `construction = product_array[Get_Weighted_Choice(…)]`.

## Faithful (do not "fix")

- **Building weights are base-only** ([475](../../MoM/src/AIBUILD.c#L475), asm:159-161) — no `ai_build_objective_weights_table` term. Neutrals have no wizard Objective. Faithful.
- **`uu_city_landmass` dead store** ([454](../../MoM/src/AIBUILD.c#L454), asm:30-63) — computed and stored, never read. OG does the same (`uu_` prefix). Preserved.

## ReMoM additions (not OG)

- Construction-range asserts ([528-529](../../MoM/src/AIBUILD.c#L528-L529), `bt_AUTOBUILD` / `bt_MAX_THINGS`) — debug only. (NP has no `AI_Metrics_Emit_Build` call.)

## Related references

- `…\ovr157\Player_Colony_Autobuild_NP.asm` — IDA Pro 5.5 disassembly (ground truth, 336 lines).
- `…\ovr157\Player_Colony_Autobuild_NP.c` — Gemini translation of the asm (agrees with the asm; production now matches both).
- Siblings [`AIBUILD-Player_Colony_Autobuild_CP.md`](AIBUILD-Player_Colony_Autobuild_CP.md), [`AIBUILD-Player_Colony_Autobuild_HP.md`](AIBUILD-Player_Colony_Autobuild_HP.md) (both repaired). Dispatcher [`Player_All_Colony_Autobuild`](../../MoM/src/AIBUILD.c#L33).
- Weight table `ai_build_base_weights_table[Category]`; local weight buffer `product_weights[]`.
- Constants: `WORLD_SIZE = 2400`, `WORLD_WIDTH = 60`, `bt_Barracks = 3`, `bt_ShipWrightsGuild`, `bt_MaritimeGuild`, `UA_CREATEOUTPOST = 0x20`, `bs_NotBuilt = 255`.
