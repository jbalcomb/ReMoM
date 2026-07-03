NEXTTURN-Determine_Offer.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Determine_Offer.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Determine_Offer.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Determine_Offer()

---

# `Determine_Offer` — Walkthrough

**Location:** [NEXTTURN.c:3359-3536](../../MoM/src/NEXTTURN.c#L3359-L3536) (~178 lines, ends [line 3536](../../MoM/src/NEXTTURN.c#L3536)).
**WZD overlay:** ovr140, p19 (drake178 IDA label: `EVNT_RandomOffers`).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c) — normal (non-time-stop) end-of-turn (Wave 10B).

> Verified against the **`.asm`** (470 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** The header notes it "has multiple BUGs, both own and inherited"; those are OG behavior, catalogued below.

## Purpose

Once per turn, for each player, rolls the three "random offer" events: a **wandering merchant item** (human only), a **mercenary** company for hire, and a **hero** for hire — each with a fame-scaled percent chance (`offer_chance_pct`, tested against `Random(100)`). The human sees popups (`Merchant_Popup`, `Hire_Merc_Popup`, `Hire_Hero_Popup`); AI players auto-resolve (hire if affordable; `AI_Accept_Hero`). Players casting Spell of Return or without an active fortress are skipped.

## Signature

```c
void Determine_Offer(void)
```

`offer_chance_pct` (was `IDK`) is the reusable per-offer percent chance; the mercenary locals are `merc_amount / merc_cost / merc_level`, plus `hire_response`, `hero_slot`, `bookshelf`.

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `Determine_Offer.asm`.

1. **Init** ([3376-3379](../../MoM/src/NEXTTURN.c#L3376-L3379), asm:24-36) — `bookshelf[0..NUM_MAGIC_TYPES) = 12` (item-realm weights for `Make_Item`).
2. **Per-player guards** ([3382-3389](../../MoM/src/NEXTTURN.c#L3382-L3389), asm:45-55) — skip players casting `spl_Spell_Of_Return`, or whose `_FORTRESSES[p].active == ST_FALSE`.
3. **Item offer — HUMAN only** ([3390-3425](../../MoM/src/NEXTTURN.c#L3390-L3425), asm:58-143) — `player_fame = Player_Fame`; `offer_chance_pct = 2 + fame/25`, ×2 if famous, capped at 10 (`SETMAX`). If rolled: `Make_Item(0, bookshelf, 0)`, `item_price = cost × 3`; if the human can't afford it `Remove_Item`, else apply the Charismatic half-price and show `Merchant_Popup`. **See OGBUG B2.**
4. **Unit-cap gate** ([3426-3429](../../MoM/src/NEXTTURN.c#L3426-L3429), asm:145-147) — `_units >= 947` → skip the merc + hero offers for this player.
5. **Mercenary offer** ([3430-3491](../../MoM/src/NEXTTURN.c#L3430-L3491), asm:150-360) — `offer_chance_pct = 1 + fame/20`, ×2 if famous, cap 10, `+10` for AI (`itr_players > 0`). If rolled: `Generate_Mercenaries`, then force spawn coords to `_FORTRESSES[p]`. If `merc_amount > 0`, `_units + merc_amount < 1000`, and `gold >= merc_cost`: human gets `Hire_Merc_Popup`, AI auto-accepts; on accept, deduct gold and `Create_Unit` × `merc_amount` (set `Level`, `XP = TBL_Experience[merc_level]`, `Finished = FALSE`, `moves2 = moves2_max`, then `Evict_Weakest_Unit`).
6. **Hero offer** ([3492-3534](../../MoM/src/NEXTTURN.c#L3492-L3534), asm:362-456) — `offer_chance_pct = 3 + fame/25`, ×2 if famous, cap 10, `+10` for AI, then `/= ((Player_Hero_Count + 1)/2 + 1)`. If rolled and `_units + 1 < MAX_UNIT_COUNT`: `Hero_Slot_Open` + `Pick_Random_Hero`; if both `> ST_UNDEFINED`, human gets `Hire_Hero_Popup`, AI gets `AI_Accept_Hero`.

## OG quirks preserved (faithful — do not "fix")

- **B1 — `player_fame` is never computed per-player** ([3392](../../MoM/src/NEXTTURN.c#L3392) vs [3430](../../MoM/src/NEXTTURN.c#L3430)/[3492](../../MoM/src/NEXTTURN.c#L3492)) — `player_fame` is assigned only inside the HUMAN-only item block (asm:63-66, a `bp` local). The mercenary and hero probabilities read that same value for **every** player, so AI players' offers use the *human's* fame (or 0 if the human was skipped by the guards). Both asm and production. The header's "inherited BUG."
- **B2 — item affordability tested before the Charismatic discount** ([3409-3419](../../MoM/src/NEXTTURN.c#L3409-L3419)) — `gold < item_price` is checked against the full `cost × 3` (asm:112-114) *before* the Charismatic half-price (asm:115-121), so a Charismatic human who could afford the discounted price but not the full price loses the item. Preserved drake178 `BUG: wrong order of operations` note.
- **"conflicting condition - will always jump"** ([3459](../../MoM/src/NEXTTURN.c#L3459)) — preserved dev annotation on the `gold >= merc_cost` branch; production matches the asm's normal conditional (asm:242-245). (The `_units + merc_amount < 1000` test above is effectively always true given the `_units >= 947 → continue` gate and small `merc_amount`.)
- **Mercenary spawn coords overwrite `Generate_Mercenaries`' outputs** ([3450-3452](../../MoM/src/NEXTTURN.c#L3450-L3452), asm:202-225) — `wx/wy/wp` are set to `_FORTRESSES[p]` right after the call fills them, so the merc group always spawns at the fortress. Faithful.
- **AI `+10` offer bonus** ([3436](../../MoM/src/NEXTTURN.c#L3436)/[3498](../../MoM/src/NEXTTURN.c#L3498)) — `itr_players > 0` adds 10 to merc/hero `offer_chance_pct` (asm:171-173/383-385). Faithful.

## ReMoM additions (not OG)

- `DBG_trigger_offer_item` / `_merc` / `_hero` HACK blocks ([3399-3404](../../MoM/src/NEXTTURN.c#L3399-L3404), [3440-3445](../../MoM/src/NEXTTURN.c#L3440-L3445), [3503-3508](../../MoM/src/NEXTTURN.c#L3503-L3508)) — debug triggers that force the offer (`offer_chance_pct = 100`) and set the human's gold to 9999. Not in the asm.

## Related references

- `…\ovr140\Determine_Offer.asm` — IDA Pro 5.5 disassembly (ground truth, 470 lines).
- `…\ovr140\Determine_Offer.c` — Gemini translation of the asm (matches production).
- Callees: `Player_Fame`, `Make_Item` / `Remove_Item`, `Merchant_Popup`, `Generate_Mercenaries`, `Hire_Merc_Popup`, `Create_Unit`, [`Evict_Weakest_Unit`](../../MoM/src/CITYCALC.c#L1441) (renamed `UNIT_RemoveExcess`; the asm calls `j_UNIT_RemoveExcess`), `Player_Hero_Count`, `Hero_Slot_Open`, `Pick_Random_Hero`, `Hire_Hero_Popup`, `AI_Accept_Hero`, `o62p01_empty_function` (an OG empty function).
- Constants: `NUM_MAGIC_TYPES`, `HUMAN_PLAYER_IDX = 0`, `spl_Spell_Of_Return`, `MAX_UNIT_COUNT = 1000`, `ST_UNDEFINED = -1`, `TBL_Experience[]`.
- Tracked as **Wave 10B** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
