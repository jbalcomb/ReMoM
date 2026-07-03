NEXTTURN-Diplomacy_Growth_For_Enchantments.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Diplomacy_Growth_For_Enchantments__WIP.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Diplomacy_Growth_For_Enchantments__WIP.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Diplomacy_Growth_For_Enchantments()       NEXTTURN
            |-> Change_Relations_For_Enchantments()
            |-> WIZ_GreatWasting() / WIZ_MeteorStorm() / WIZ_Armageddon()

---

# `Diplomacy_Growth_For_Enchantments` — Walkthrough

**Location:** [NEXTTURN.c:4030-4129](../../MoM/src/NEXTTURN.c#L4030-L4129) (~100 lines, ends [line 4129](../../MoM/src/NEXTTURN.c#L4129)).
**WZD overlay:** ovr140 (o140p24; drake178/MoO2: `DIPLOMAC` module `Diplomacy_Growth_()`).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L616) — normal end-of-turn, call site [NEXTTURN.c:734](../../MoM/src/NEXTTURN.c#L734).

> Verified against the **`.asm`** (338 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** Builds clean (Debug, MSVC).

## Purpose

Once per turn, for every player, apply the standing per-turn side-effects of that player's active global enchantments: reset the casting-cost cache when nothing is being cast, and for each global enchantment the player has up, nudge diplomatic relations via `Change_Relations_For_Enchantments` (and, for the three "world-wrecker" enchantments, first run the enchantment's per-turn map effect). This is the enchantment counterpart to the main `Diplomacy_Growth`.

## Signature

```c
void Diplomacy_Growth_For_Enchantments(void)
```

Locals: `itr_players` (`_SI_`).

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `Diplomacy_Growth_For_Enchantments__WIP.asm`. One `for(itr_players = 0; itr_players < _num_players; …)` loop ([4033](../../MoM/src/NEXTTURN.c#L4033), asm:8, 327-331); every block below is inside it.

1. **Casting-cost reset** ([4035-4038](../../MoM/src/NEXTTURN.c#L4035-L4038), asm:16-22) — `casting_spell_idx == spl_NONE` → `casting_cost_original = 0`.
2. **Spell of Mastery** ([4039-4041](../../MoM/src/NEXTTURN.c#L4039-L4041), asm:28-36) — `casting_spell_idx == spl_Spell_Of_Mastery` → `Change_Relations_For_Enchantments(itr_players, spl_Spell_Of_Mastery, 2)` (note the weight `2`, not `10`).

Blocks 3-22 all share the shape `if(Globals[X] > 0) Change_Relations_For_Enchantments(itr_players, spl_X, 10);` (the asm tests `cmp …, 0; jle skip` → fire on `> 0`). In asm order:

3. Eternal Night ([4043](../../MoM/src/NEXTTURN.c#L4043), asm:42-50) · 4. Evil Omens ([4047](../../MoM/src/NEXTTURN.c#L4047), asm:56-64) · 5. Zombie Mastery ([4051](../../MoM/src/NEXTTURN.c#L4051), asm:70-78) · 6. Aura of Majesty ([4055](../../MoM/src/NEXTTURN.c#L4055), asm:84-92) · 7. Wind Mastery ([4059](../../MoM/src/NEXTTURN.c#L4059), asm:98-106) · 8. Suppress Magic ([4063](../../MoM/src/NEXTTURN.c#L4063), asm:112-120) · 9. Time Stop ([4067](../../MoM/src/NEXTTURN.c#L4067), asm:126-134) · 10. Nature's Awareness ([4071](../../MoM/src/NEXTTURN.c#L4071), asm:140-148) · 11. Nature's Wrath ([4075](../../MoM/src/NEXTTURN.c#L4075), asm:154-162) · 12. Herb Mastery ([4079](../../MoM/src/NEXTTURN.c#L4079), asm:168-176) · 13. Chaos Surge ([4083](../../MoM/src/NEXTTURN.c#L4083), asm:182-190) · 14. Doom Mastery ([4087](../../MoM/src/NEXTTURN.c#L4087), asm:196-204).

15-17. **World-wrecker enchantments** — same shape but call the per-turn map effect first:
- **Great Wasting** ([4092-4096](../../MoM/src/NEXTTURN.c#L4092-L4096), asm:210-221) — `WIZ_GreatWasting(itr_players)` then `Change_Relations_For_Enchantments(itr_players, spl_Great_Wasting, 10)`.
- **Meteor Storms** ([4097-4101](../../MoM/src/NEXTTURN.c#L4097-L4101), asm:227-238) — `WIZ_MeteorStorm(itr_players)` then relations.
- **Armageddon** ([4102-4106](../../MoM/src/NEXTTURN.c#L4102-L4106), asm:244-255) — `WIZ_Armageddon(itr_players)` then relations.

18. Tranquility ([4108](../../MoM/src/NEXTTURN.c#L4108), asm:261-269) · 19. Life Force ([4112](../../MoM/src/NEXTTURN.c#L4112), asm:275-283) · 20. Crusade ([4116](../../MoM/src/NEXTTURN.c#L4116), asm:289-297) · 21. Just Cause ([4120](../../MoM/src/NEXTTURN.c#L4120), asm:303-311) · 22. Holy Arms ([4124](../../MoM/src/NEXTTURN.c#L4124), asm:317-325).

Block order, spell ids, and weights all match the asm exactly.

## Faithful (do not "fix")

- **`j_WIZ_*` thunks → direct `WIZ_*` calls.** The asm calls the IDA jump thunks `j_WIZ_GreatWasting` (asm:213), `j_WIZ_MeteorStorm` (asm:230), `j_WIZ_Armageddon` (asm:247); production calls the real functions directly (`WIZ_GreatWasting`/`WIZ_MeteorStorm`/`WIZ_Armageddon`, defined in [Spells132.c:1227/388/1287](../../MoM/src/Spells132.c#L388)). The `j_` prefix is an IDA far-call trampoline artifact, not a distinct symbol — dropping it is correct reconstruction, not a divergence.

## Related references

- `…\ovr140\Diplomacy_Growth_For_Enchantments__WIP.asm` — IDA Pro 5.5 disassembly (ground truth, 338 lines).
- `…\ovr140\Diplomacy_Growth_For_Enchantments__WIP.c` — Gemini translation of the asm.
- Callees: `Change_Relations_For_Enchantments`, `WIZ_GreatWasting`/`WIZ_MeteorStorm`/`WIZ_Armageddon` ([Spells132.c](../../MoM/src/Spells132.c), overlay o132).
- Sibling: [`Diplomacy_Growth`](../../MoM/src/DIPLOMAC.c) — the main per-turn relations pass (already reviewed).
- Constants/fields: `spl_NONE`, `spl_Spell_Of_Mastery`, `casting_spell_idx`, `casting_cost_original`, `_players[].Globals[…]` enchantment slots (`ETERNAL_NIGHT`, `EVIL_OMENS`, `ZOMBIE_MASTERY`, `AURA_OF_MAJESTY`, `WIND_MASTERY`, `SUPPRESS_MAGIC`, `TIME_STOP`, `NATURES_AWARENESS`, `NATURES_WRATH`, `HERB_MASTERY`, `CHAOS_SURGE`, `DOOM_MASTERY`, `GREAT_WASTING`, `METEOR_STORMS`, `ARMAGEDDON`, `TRANQUILITY`, `LIFE_FORCE`, `CRUSADE`, `JUST_CAUSE`, `HOLY_ARMS`).
- Tracked in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
