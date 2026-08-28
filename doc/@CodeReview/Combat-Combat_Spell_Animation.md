Combat-Combat_Spell_Animation.md

'Combat Spell Animation' AoC

CMB_PlaySpellAnim()                    ==>  Combat_Spell_Animation()
Combat_Spell_Animation__WIP()          ==>  Combat_Spell_Animation()
TILE_BoltFromAbove()                   ==>  Combat_Spell_Animation_Bolt()
TILE_LightningBolt()                   ==>  Animate_Lightning_Bolt
TILE_CombatSpellAnim()                 ==>  Combat_Spell_Animation_Default()
Combat_Spell_Animation_Generic__WIP()  ==>  Combat_Spell_Animation_Default()
Cast_Call_Chaos__WIP()                 ==>  Cast_Call_Chaos()
Apply_Call_Chaos__WIP()                ==>  Apply_Call_Chaos()

SND_SpellCast

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr131\Combat_Spell_Animation__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\TILE_BoltFromAbove.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\Animate_Lightning_Bolt.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Combat_Load_Spell_Sound_Effect.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr131\Combat_Battlefield_Instant.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\Cast_Call_Chaos__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\Apply_Call_Chaos__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\Combat_Spell_Animation_Generic__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\Animate_Cracks_Call.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\Wall_Rise.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\Wall_Rise_Load.asm

OON XREF:  Combat_Cast_Apply_Spell_Effect() |-> j_Combat_Spell_Animation__WIP() |-> Combat_Spell_Animation()

OON XREF:  Combat_Cast_Apply_Spell_Effect() |-> j_Combat_Spell_Animation__WIP() |-> Combat_Spell_Animation() |-> Combat_Battlefield_Instant()

OON XREF:  Combat_Cast_Apply_Spell_Effect() |-> j_Combat_Spell_Animation__WIP() |-> Combat_Spell_Animation() |-> Combat_Spell_Animation_Bolt()

OON XREF:  Combat_Cast_Apply_Spell_Effect() |-> j_Combat_Spell_Animation__WIP() |-> Combat_Spell_Animation() |-> Combat_Spell_Animation_Default()

Combat_Spell_Animation()
    |-> Combat_Battlefield_Instant()
    |-> Combat_Screen_Draw()
    |-> Combat_Cast_Spell_Message()
    |-> Combat_Load_Spell_Sound_Effect()
    |-> Combat_Spell_Animation_Bolt()
    |-> Spell_Animation_Load_Graphics()
    |-> Animate_Lightning_Bolt()
    |-> Animate_Cracks_Call()
    |-> Combat_Spell_Animation_Default()

Combat_Spell_Animation()
    |-> Combat_Battlefield_Instant()
        |-> Cast_Call_Chaos()
            |-> Apply_Call_Chaos()

Apply_Call_Lightning()
Combat_Spell_Animation()
    |-> Animate_Lightning_Bolt()

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-20).** Eleven functions, 2,019 asm lines, all walked end to end against the listings. Ten findings were raised and all ten are fixed and verified in the current sources, so the findings section is empty; D1, D2 and D3 are recorded deviations, not defects. Builds clean. Most of the cohort had no owner before this doc.

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Cast_Call_Chaos` | [Spells133.c:1452](../../MoM/src/Spells133.c#L1452) | `ovr133/Cast_Call_Chaos__WIP.asm` | 472 | **full body walk — faithful** |
| `Combat_Battlefield_Instant` | [Spells131.c:702](../../MoM/src/Spells131.c#L702) | `ovr131/Combat_Battlefield_Instant.asm` | 396 | **full body walk — faithful** |
| `Combat_Spell_Animation` | [Spells131.c:994](../../MoM/src/Spells131.c#L994) | `ovr131/Combat_Spell_Animation__WIP.asm` | 248 | **full body walk — faithful** |
| `Combat_Spell_Animation_Bolt` | [Spells133.c:543](../../MoM/src/Spells133.c#L543) | `ovr133/TILE_BoltFromAbove.asm` | 206 | **full body walk — faithful** |
| `Apply_Call_Chaos` | [Spells133.c:1607](../../MoM/src/Spells133.c#L1607) | `ovr133/Apply_Call_Chaos__WIP.asm` | 180 | **full body walk — faithful** |
| `Animate_Lightning_Bolt` | [Spells133.c:972](../../MoM/src/Spells133.c#L972) | `ovr133/Animate_Lightning_Bolt.asm` | 118 | **full body walk — faithful** |
| `Wall_Rise_Load` | [Combat.c:22227](../../MoM/src/Combat.c#L22227) | `ovr163/Wall_Rise_Load.asm` | 109 | **full body walk — faithful** |
| `Wall_Rise` | [Spells133.c:319](../../MoM/src/Spells133.c#L319) | `ovr133/Wall_Rise.asm` | 101 | **full body walk — faithful** |
| `Combat_Spell_Animation_Default` | [Spells133.c:671](../../MoM/src/Spells133.c#L671) | `ovr133/Combat_Spell_Animation_Generic__WIP.asm` | 78 | **full body walk — faithful** |
| `Combat_Load_Spell_Sound_Effect` | [Combat.c:15852](../../MoM/src/Combat.c#L15852) | `ovr124/Combat_Load_Spell_Sound_Effect.asm` | 64 | **full body walk — faithful** |
| `Animate_Cracks_Call` | [Spells133.c:514](../../MoM/src/Spells133.c#L514) | `ovr133/Animate_Cracks_Call.asm` | 47 | **full body walk — faithful** |

## What belongs here, and why

This is the **combat spell-animation layer**: one dispatcher plus the effect animations it is the only caller of. The boundary is what happens on screen while a combat spell resolves — not selecting the spell, not applying its effect.

`Combat_Spell_Animation` is called from exactly one place, [Combat_Cast_Apply_Spell_Effect](Combat-Combat_Cast_Apply_Spell_Effect.md), through a `j_` thunk. Every arm of that dispatcher calls it first and then applies the effect, which is what makes animation a clean layer to cut.

Most of them had no owner before this doc:

- **`Combat_Load_Spell_Sound_Effect`** was in [Combat-Miscellaneous.md](Combat-Miscellaneous.md), and it belongs here without ambiguity: all four of its callers are in this cohort — `Combat_Battlefield_Instant` ([Spells131.c:764](../../MoM/src/Spells131.c#L764)), `Combat_Spell_Animation` ([Spells131.c:1107](../../MoM/src/Spells131.c#L1107)), `Apply_Call_Lightning` ([Spells133.c:251](../../MoM/src/Spells133.c#L251)) and `Cast_Call_Chaos` ([Spells133.c:1535](../../MoM/src/Spells133.c#L1535)).
- **`Wall_Rise`** and **`Wall_Rise_Load`** are a pair — `Wall_Rise` is the only caller of `Wall_Rise_Load` ([Spells133.c:342](../../MoM/src/Spells133.c#L342)) — and `Wall_Rise` is called only from the `scc_City_Enchantment_Positive` and `scc_Special_Spell` arms of the applier. It was listed as excluded from that review as "animation layer"; this is that layer.
- **`Combat_Spell_Animation_Bolt`**, **`Combat_Spell_Animation_Default`** and **`Cast_Call_Chaos`** each have exactly one caller inside this cohort — [Spells131.c:1118](../../MoM/src/Spells131.c#L1118), [Spells131.c:1147](../../MoM/src/Spells131.c#L1147) and [Spells131.c:713](../../MoM/src/Spells131.c#L713).

`Animate_Lightning_Bolt` has a second caller in `Apply_Call_Lightning` ([Spells133.c:271](../../MoM/src/Spells133.c#L271)), but its primary caller is the dispatcher at [Spells131.c:1123](../../MoM/src/Spells131.c#L1123), so it comes here.

### What does *not* belong here

- **`Spell_Animation_Load_Graphics`** ([SBookScr.c:961](../../MoM/src/SBookScr.c#L961)), even though the dispatcher calls it. It has 26 call sites across eight files — `OverSpel.c`, `SPLMASTR.c`, `SBookScr.c` and `Spells129.c` / `130` / `131` / `132` / `133` / `137` — so this cohort is a small minority of its consumers and the function belongs with the spell-graphics layer, not here.
- **`Battle_Unit_Summon_Animation`**, covered by [Combat-Combat_Summon.md](Combat-Combat_Summon.md).
- **`Combat_Cast_Spell_Message`**, covered by [Combat-Spell_Cast.md](Combat-Spell_Cast.md), which records that it sits there by decision rather than by call graph.
- **`Apply_Call_Lightning`** and **`Apply_Wrack`** ([Spells133.c:238](../../MoM/src/Spells133.c#L238), [Spells133.c:148](../../MoM/src/Spells133.c#L148)). Both call into this layer, but both are driven from the combat-turn path at [Combat.c:2298-2310](../../MoM/src/Combat.c#L2298-L2310) rather than from the animation dispatcher. They are per-turn enchantment effects that happen to animate, not animations.
- **`Combat_Screen_Draw`**, owned by [Combat-Combat_Screen.md](Combat-Combat_Screen.md).

## Findings

None outstanding. Every finding raised during this review has been fixed in the sources and verified there; the history is in git.

## Deviations

### D1 - two of the four cases are in a different order from the listing

The asm lays the case bodies out as `jt_spl_Fireball` (asm:48), `jt_spl_Fire_Bolt` (asm:61), `jt_spl_Ice_Bolt` (asm:73), `jt_spl_Doom_Bolt` (asm:87). Production runs Fire Bolt, Fireball, Ice Bolt, Doom Bolt - the last two now agree and the first two are swapped.

Left as it stands. The claim rests on Borland emitting scan-table case bodies in source order, which is an inference about codegen rather than something the listing states, and this region is demonstrably reordered already: Fire Bolt and Ice Bolt share a tail-merged `frame_count` store. Nothing behaves differently - a `switch` with no fall-through is order-independent.

### D2 - the `default` arm has no asm counterpart

asm:37-45 is Borland's scan-table dispatch: `mov cx, 4`, a four-entry value table, `loop`, and on no match `jmp loc_B72A2` - straight past the switch to `Set_Page_Off`. There is no default body.

[Spells133.c:596](../../MoM/src/Spells133.c#L596) adds `default: { STU_DEBUG_BREAK(); } break;`. It expands to `( (void)0 )` in release builds ([STU_DBG.h:15](../../STU/src/STU_DBG.h#L15)) so it costs nothing shipped, and it is a useful guard against a fifth bolt spell reaching this function silently. Recorded rather than proposed for removal.

### D3 - the frame loop is hoisted and scaled by the cell-effect hold

asm:37-42 re-evaluates the bound on **every** iteration:

```
loc_B71A9:
push    [spell_animation_seg]
call    FLIC_Get_FrameCount
pop     cx
cmp     ax, _SI_frame
jg      short loc_B7187
```

so the source condition is `FLIC_Get_FrameCount(spell_animation_seg) > frame`, with the call inside the `for`. [Spells133.c:526](../../MoM/src/Spells133.c#L526) hoists it into a `frame_count` local - correctly marked `DNE in Dasm` - and [Spells133.c:527](../../MoM/src/Spells133.c#L527) then scales the bound by `CELL_EFFECT_ANIM_HOLD`.

**This is a deliberate project-wide timing adaptation, not a local hack.** `CELL_EFFECT_ANIM_HOLD` is 7 ([CMBTDEF.h:23](../../MoM/src/CMBTDEF.h#L23)), and the consumer divides by the same constant when it picks the frame to display - [Combat.c:19071](../../MoM/src/Combat.c#L19071) is `Set_Animation_Frame(cmbt_cell_effect_seg, (cmbt_cell_effect_frame / CELL_EFFECT_ANIM_HOLD))`, with the matching wrap at [Combat.c:5427](../../MoM/src/Combat.c#L5427). The loop has to count seven times as far to show the same number of frames. Change one of the three and the animation breaks; they only make sense together.

Recorded so the hoist is not "corrected" back into the loop condition without also unwinding the other two sites.

## Verified faithful

### `Wall_Rise` ([Spells133.c:319](../../MoM/src/Spells133.c#L319), asm 101) and `Wall_Rise_Load` ([Combat.c:22227](../../MoM/src/Combat.c#L22227), asm 109)

Faithful.

**`Wall_Rise` frame - exact match.** `frame_count` at `bp-4`, `itr_cgx` at `bp-2`, `sub sp, 4`, with `wall_type` in SI and `itr_cgy` in DI. [Spells133.c:321](../../MoM/src/Spells133.c#L321)-[Spells133.c:324](../../MoM/src/Spells133.c#L324) declares the two stack locals first, then the two register locals.

**The spell-to-type mapping.** Wall of Stone gives 0, Wall of Fire gives 1, anything else gives 2 - asm:15-25, [Spells133.c:326](../../MoM/src/Spells133.c#L326). `Wall_Rise` is called from the applier's `scc_City_Enchantment_Positive` and `scc_Special_Spell` arms, so "anything else" is Wall of Darkness in practice.

**The type-0 body marks the city walled and fills a 4x4 block.** `battlefield->walled = 1` then `walls[itr_cgy][itr_cgx] = 1` over both indices bounded at 4 - asm:45-68 with the `shl 3` row stride and `shl 1` element size, [Spells133.c:355](../../MoM/src/Spells133.c#L355). The preserved note about the centre sections being marked despite having no wall is an OG observation and stays.

**The frame loop counts in a global.** asm:79-94 uses `_wall_rise_frame` itself as the loop variable rather than a local, and `_wall_rise_on` brackets the whole animation - `ST_TRUE` at asm:34 before the dispatch, `ST_FALSE` at asm:95 after the loop. [Spells133.c:368](../../MoM/src/Spells133.c#L368) reproduces that, which is worth stating because a global as a loop counter reads like a mistake and is not one: the combat screen redraw inside the loop is what consumes it.

**`Wall_Rise_Load` has no stack frame at all** - `push bp` / `mov bp, sp` / two register pushes, with `wall_type` in DI and `itr` in SI.

**It reallocates all eighteen battle-figure blocks first.** asm:17-30 is `Allocate_Next_Block(_screen_seg, 55)` into `battle_unit_picts_seg[itr]` for `itr < 18`, before any wall graphics load - [Combat.c:22236](../../MoM/src/Combat.c#L22236). The preserved `WARNING: redoing the pointers may cause trouble` sits on the right line.

**The three load cases differ in both count and base offset.** Type 0 loads **12** entries based at `_combat_wall_sprite_bank * 12` - IDA's `CMB_StoneWallType` - while types 1 and 2 load **14** each, based at `36` and `50`. asm:67 is `add ax, 36` and asm:87 is `add ax, 50`, matching [Combat.c:22254](../../MoM/src/Combat.c#L22254) and [Combat.c:22262](../../MoM/src/Combat.c#L22262) where production spells them `(3 * 12)` and `((3 * 12) + 14)`. The asymmetry is real: only the stone-wall set is bank-selected.

**`Release_Block(_screen_seg)`** closes the `Mark_Block` at the top - [Combat.c:22267](../../MoM/src/Combat.c#L22267).

### `Cast_Call_Chaos` ([Spells133.c:1452](../../MoM/src/Spells133.c#L1452), asm 472) and `Apply_Call_Chaos` ([Spells133.c:1452](../../MoM/src/Spells133.c#L1452), asm 180)

Both faithful.

**`Cast_Call_Chaos` frame - exact match, and it is the largest in this cohort.** `unit_cgy` at `bp-13Ch`, `unit_cgx` at `bp-0F4h` and `unit_anim_delay` at `bp-0ACh` are **72 bytes each** - thirty-six `int16_t`, one per battle-unit slot - then eight two-byte animation handles running from `bp-64h` to `bp-56h`, `effects` at `bp-54h` for another 72, and `screen_y`, `screen_x`, `player_idx`, `effect_frame_count`, `highest_frame_count`, `frame` filling `bp-0Ch` to `bp-2`. `sub sp, 13Ch`. [Spells133.c:1454](../../MoM/src/Spells133.c#L1454)-[Spells133.c:1465](../../MoM/src/Spells133.c#L1465) declares them in that order, with `battle_unit_idx` in SI. IDA names the eight handles individually; production groups them into one `effect_anim[cce_MAX]` array, which is a naming choice over the same eight slots and not a frame difference.

**The caster chain.** asm:29-32 is `cmp [bp+caster_idx], e_CASTER_IDX_BASE_sub1` / `jle`, then `add ax, -20` on the wizard arm - [Spells133.c:1483](../../MoM/src/Spells133.c#L1483).

**Six loads and two aliases.** The `LOAD_ANIMS_LOCAL` macro at [Spells133.c:1472](../../MoM/src/Spells133.c#L1472) reproduces asm:132-186 in order: MONSTER 47 through `LBX_Reload`, then SPECFX 3, SPECFX 2, CMBTFX 33, CMBTFX 3 and CMBTFX 4 through `LBX_Reload_Next`. Only the first uses `LBX_Reload`; the rest chain. Two of the eight are aliases rather than loads: asm:153-159 stores the SPECFX 2 result into both `cce_Chaos_Channel` and `cce_Warp_Creature`, and asm:169-175 stores the CMBTFX 3 result into both `cce_Warp_Lightning` and `cce_Doom_Bolt`. Production has both pairs sourced correctly.

**`Apply_Call_Chaos` frame - exact match.** `resist_fails` at `bp-8`, `damage_types` at `bp-6` spanning six bytes, `sub sp, 8`, with `battle_unit_idx` in SI.

**Its unit filter is `!= player_idx`.** asm:19-21 is `cmp ax, [bp+player_idx]` / `jnz` to continue and a fall-through jump to the loop increment, so only units **not** controlled by the caster are processed - [Spells133.c:1614](../../MoM/src/Spells133.c#L1614).

**The effect dispatch is a bounded `switch`.** asm:40 is `cmp bx, 7` guarding a jump table, matching the eight-case `switch(effects[battle_unit_idx])` at [Spells133.c:1620](../../MoM/src/Spells133.c#L1620) - 0 for no effect, then healing, chaos channels, warp creature, and three damage variants, with case 7 keyed on `Combat_Effective_Resistance`.


**`Apply_Call_Chaos` case bodies, all seven checked against the listing.** Case 1 guards on `race != rt_Death` then `Battle_Unit_Heal(battle_unit_idx, 5, 0)` - asm:54-62, [Spells133.c:1628](../../MoM/src/Spells133.c#L1628). Case 2 is `Unit_Is_Normal(unit_idx)` non-zero then `Apply_Chaos_Channels(unit_idx)`, both taking `.unit_idx` rather than the battle-unit index - asm:71-83, [Spells133.c:1635](../../MoM/src/Spells133.c#L1635). Case 3 is `Combat_Resistance_Check(battle_units[i], -1, sbr_Chaos)` through `SPUSH@` with `add sp, 72h`, then `> 0` gating `Apply_Warp_Creature(battle_unit_idx)` - note the **battle-unit** index there against the **unit** index in case 2 - asm:87-108, [Spells133.c:1642](../../MoM/src/Spells133.c#L1642). Cases 4, 5 and 6 pass `15`, `0` and `0` as the strength with `spl_Fire_Bolt`, `spl_Warp_Lightning` and `spl_Doom_Bolt` - asm:113, 137, 145. Case 7 tests `Combat_Effective_Resistance(...) < 10` and writes `{0, 0, 200}` into the three damage slots at asm:164-166, which production spells `dt_Normal` / `dt_Drain` / `dt_Doom` at [Spells133.c:1667](../../MoM/src/Spells133.c#L1667).

**The unit filter takes both halves.** asm:19-33 is `controller_idx != player_idx` **and** `status == bus_Active`, each falling through to the loop increment - [Spells133.c:1614](../../MoM/src/Spells133.c#L1614).

**The dispatch is a bounded jump table.** asm:40-45 is `cmp bx, 7` / `jbe` then `jmp [cs:off_B8AB5+bx]`, so out-of-range values skip to the increment with no default body. The `default: STU_DEBUG_BREAK();` at [Spells133.c:1673](../../MoM/src/Spells133.c#L1673) is an addition of the same kind recorded as D2 for the bolt animation - `(void)0` in release, and a useful guard now that the effect codes are documented.

**The rest of `Cast_Call_Chaos`, now walked.** The effect-assignment loop at asm:60-123 guards on `controller_idx != player_idx` **and** `status == bus_Active`, then stores `Random(5) - 1` into the delay array and snapshots the unit's `cgx` / `cgy`; the else arm sets only the delay to `ST_UNDEFINED` and leaves the coordinate arrays alone - [Spells133.c:1507](../../MoM/src/Spells133.c#L1507). asm:125-131 is `Combat_Compose_Background`, `Set_Page_Off`, `Combat_Screen_Draw`, `PageFlip_FX`, `Copy_On_To_Off_Page`, `Copy_Off_To_Back`, `Save_ScreenSeg` in that exact order.

**The effect roll and frame-count scan.** asm:187-219 is `Random(8) - 1` into the effect array, then `FLIC_Get_FrameCount` on the **indexed handle** - asm:198-206 loads `effect_anim[effects[i]]` by value - tracking the maximum into `highest_frame_count`. [Spells133.c:1526](../../MoM/src/Spells133.c#L1526)-[Spells133.c:1532](../../MoM/src/Spells133.c#L1532). The sound block at asm:220-230 nests `SND_SpellCast != ST_UNDEFINED` inside `magic_set.sound_effects == ST_TRUE` - [Spells133.c:1533](../../MoM/src/Spells133.c#L1533).

**The per-unit draw loop.** The sprite-size test picks effect codes **2, 3 and 6** for the large offsets `-28` / `-30` and everything else for `-14` / `-27` - asm:354-374, [Spells133.c:1564](../../MoM/src/Spells133.c#L1564). The visibility guard is a two-part `&&`: `(frame - delay) < highest_frame_count` and `(frame - delay) >= 0` - asm:380-392, [Spells133.c:1580](../../MoM/src/Spells133.c#L1580). Then a third test, `FLIC_Get_FrameCount(...) > (frame - delay)`, gates `Set_Animation_Frame` and `Clipped_Draw` - [Spells133.c:1586](../../MoM/src/Spells133.c#L1586).

**Epilogue.** `Restore_ScreenSeg` then `CMB_ComposeBackgrnd__WIP` after the loop, matching [Spells133.c:1597](../../MoM/src/Spells133.c#L1597).


### `Combat_Battlefield_Instant` ([Spells131.c:702](../../MoM/src/Spells131.c#L702), asm 396)

Faithful. This is the `Combat_Spell_Animation` arm for the five battlefield-wide instants - Flame Strike, Holy Word, Death Spell, Call Chaos and Mass Healing.

**Frame - exact match.** `unit_anim_delay` at `bp-52h` spanning **72 bytes**, thirty-six `int16_t`, then `anim_size` at `bp-0Ah`, `screen_y` at `bp-8`, `screen_x` at `bp-6`, `frame_count` at `bp-4`, `itr2` at `bp-2`, `sub sp, 52h`, with `spell_idx` promoted into DI and `itr1` in SI. [Spells131.c:704](../../MoM/src/Spells131.c#L704)-[Spells131.c:710](../../MoM/src/Spells131.c#L710) declares them in that order.

**Call Chaos leaves immediately.** asm:21-28 tests it first and jumps straight to the epilogue after the call, so none of the rest of the function runs for that spell.

**The delay array is filled by a two-way ownership test.** asm:158-183 is an `||` of two `&&` pairs, then an `&&` on status. Tracing the four paths: owner-differs-and-not-Mass-Healing reaches the body, owner-matches-and-Mass-Healing reaches it too, and the two mixed cases fall to the `ST_UNDEFINED` store at `loc_B327A`. That is exactly the shape at [Spells131.c:773](../../MoM/src/Spells131.c#L773)-[Spells131.c:789](../../MoM/src/Spells131.c#L789) - a spell hits the *other* side's units unless it is Mass Healing, which hits your own. The value is `Random(5) - 1`, so 0..4 because `Random` is 1-based, at [Spells131.c:791](../../MoM/src/Spells131.c#L791).

**`ST_UNDEFINED` is the "not affected" marker**, stored at [Spells131.c:795](../../MoM/src/Spells131.c#L795) and tested later as `unit_anim_delay[itr1] > ST_UNDEFINED` to decide which units get drawn.

**The animation loop runs four frames past the end.** asm:335-340 is `mov ax, [bp+frame_count]` / `add ax, 4` / `cmp ax, [bp+itr2]` / `jle` to exit, matching [Spells131.c:798](../../MoM/src/Spells131.c#L798) - the extra four cover the per-unit stagger the delay array introduces.

**The effect fires at the halfway point.** Each of `Apply_Death_Spell`, `Apply_Holy_Word`, `Apply_Flame_Strike` and the Mass Healing block is guarded by `((frame_count + 4) / 2) == itr2` - the `cwd` / `sub ax, dx` / `sar ax, 1` signed-halve idiom at asm:36-41 - so the spell resolves mid-animation. [Spells131.c:801](../../MoM/src/Spells131.c#L801).

**The same four-effect group appears twice in the listing**, once in each arm of the `anims_on` split - asm:34-119 and asm:224-276 - and production reproduces both. In the no-animation arm the guard is dead: `frame_count` and `itr2` are both still zero there, so `(0 + 4) / 2` is 2 and never equals 0. That is the original's own copy-paste and is preserved, not repaired.

**The epilogue.** `Release_Block(_screen_seg)`, `Set_Page_Off`, `Combat_Screen_Draw`, `PageFlip_FX` - all inside the animation arm, matching [Spells131.c:861](../../MoM/src/Spells131.c#L861) onward. The `Mark_Block` at the top of that arm is the pair.

### `Combat_Load_Spell_Sound_Effect` ([Combat.c:15852](../../MoM/src/Combat.c#L15852), asm 64)

Faithful. No findings.

**No stack frame.** `push bp` / `mov bp, sp` / `push si` with no `sub sp` - `spell_idx` is promoted into SI and there are no locals. Production uses the parameter directly.

**The whole body is one guard.** asm:9-10 is `cmp [magic_set.sound_effects], e_ST_TRUE` / `jnz`, so the else arm is the single store `SND_SpellCast = ST_UNDEFINED` at asm:59 - [Combat.c:16456](../../MoM/src/Combat.c#L16456) and [Combat.c:16474](../../MoM/src/Combat.c#L16474). That store is what every caller in this cohort then tests before playing, which is why the sound guards elsewhere read `SND_SpellCast != ST_UNDEFINED`.

**A silence sample is played first.** asm:11-13 pushes `SND_CMB_Silence@` into `Play_Sound` before anything is loaded - [Combat.c:16458](../../MoM/src/Combat.c#L16458), production's `sound_silent_seg`, itself loaded once at [Combat.c:1490](../../MoM/src/Combat.c#L1490). It stops whatever is currently playing before the buffer is reused.

**The `Mark_Block` / `Release_Block` pair is on `World_Data`, not `_screen_seg`** - asm:14-16 and asm:54-56, [Combat.c:16459](../../MoM/src/Combat.c#L16459) and [Combat.c:16470](../../MoM/src/Combat.c#L16470). Worth stating because every other block pairing in this cohort uses the screen segment.

**The sign of `.Sound` selects the archive.** asm:22-23 is `cmp [es:bx+s_SPELL_DATA.Sound], -1` / `jle`, so a positive value takes `soundfx_lbx_file__ovr124__2of2` with the raw index and a negative one takes `newsound_lbx_file__ovr124__2of2` with `abs()` - [Combat.c:16460](../../MoM/src/Combat.c#L16460), [Combat.c:16462](../../MoM/src/Combat.c#L16462) and [Combat.c:16467](../../MoM/src/Combat.c#L16467). That matches the field's documented meaning: positive is SOUNDFX.LBX, `abs(negative)` is NEWSOUND.LBX. The `abs` call appears only on the negative arm in the listing, and production matches.

**The two `LBX_Reload_Next` calls share a tail in the listing.** asm:34 jumps from the positive arm into `loc_A7F59`, so one `call LBX_Reload_Next` / `add sp, 6` serves both - Borland's cross-jump. Writing the call out in each arm is the correct reconstruction, not a duplication to collapse.

**The `SND_SpellCast_size` stores are the host-audio addition.** [Combat.c:16463](../../MoM/src/Combat.c#L16463) and its sibling have no asm counterpart; `lbxload_entry_length` is marked `/* HACK */ ... because SDL Mixed needs the sound buffer size` at [LBX_Load.h:61](../../MoX/src/LBX_Load.h#L61). Same family as the two-argument `Play_Sound` seen throughout this cohort - a platform adaptation, not a reconstruction error.

### `Combat_Spell_Animation_Default` ([Spells133.c:671](../../MoM/src/Spells133.c#L671), asm 78)

Faithful. This is the dispatcher's fallback - every combat spell that is not a bolt, Lightning Bolt or Cracks Call lands here.

**Frame - exact match.** `screen_y` at `bp-6`, `screen_x` at `bp-4`, `frame_count` at `bp-2`, `sub sp, 6`, with `frame` in SI. [Spells133.c:673](../../MoM/src/Spells133.c#L673)-[Spells133.c:676](../../MoM/src/Spells133.c#L676) declares the three stack locals in that order.

**`Combat_Grid_Screen_Coordinates(cgx, cgy, 4, 4, &screen_x, &screen_y)`** takes six words with `add sp, 0Ch` - [Spells133.c:677](../../MoM/src/Spells133.c#L677).

**The size split.** asm:28-29 is `cmp [bp+anim_size], 0` / `jnz`, so the `== 0` case is the fall-through and takes the smaller offsets - [Spells133.c:678](../../MoM/src/Spells133.c#L678) has the arms the right way round, and both arms subtract rather than assign.

**The sound guard** is `cmp [SND_SpellCast@], e_ST_UNDEFINED` / `jz` to skip, i.e. `!=` - [Spells133.c:688](../../MoM/src/Spells133.c#L688). `Play_Sound` takes two words here against one in the listing, the project-wide host-audio form.

**`FLIC_Get_FrameCount` is hoisted in the listing here.** asm:43-46 calls it once before the loop and stores the result in the `frame_count` slot, and asm:72 tests `cmp _SI_frame, [bp+frame_count]`. [Spells133.c:692](../../MoM/src/Spells133.c#L692) and [Spells133.c:694](../../MoM/src/Spells133.c#L694) match. **This is the opposite of `Animate_Cracks_Call`**, where the same call sits inside the loop condition and production hoists it deliberately as D3 - so the two functions genuinely differ and should not be harmonised. This one also takes the raw frame count with no `CELL_EFFECT_ANIM_HOLD` scaling, because it draws its own frames rather than driving the cell-effect globals.

**`Reset_Animation_Frame(spell_animation_seg)`** before the loop - [Spells133.c:693](../../MoM/src/Spells133.c#L693).

**The loop body.** `Mark_Time`, `Combat_Screen_Draw`, `Combat_Cast_Spell_Message(caster_idx, spell_idx)` with the spell pushed first so the source order is as production has it, `Clipped_Draw(screen_x, screen_y, spell_animation_seg)`, `PageFlip_FX`, `Release_Time(2)` - [Spells133.c:696](../../MoM/src/Spells133.c#L696) onward, call for call.

**No `Mark_Block` and no epilogue calls.** The function ends at the loop's closing brace in both the listing and production - unlike `Combat_Spell_Animation_Bolt`, which pairs a `Mark_Block` with `Release_Block` and `Combat_Compose_Background`.

### `Animate_Cracks_Call` ([Spells133.c:514](../../MoM/src/Spells133.c#L514), asm 47)

Faithful apart from D3.

**No stack frame.** `push bp` / `mov bp, sp` / `push si` with no `sub sp` - `frame` lives in SI and there are no stack locals. `frame_count` at [Spells133.c:517](../../MoM/src/Spells133.c#L517) is production's own addition for D3 and says so.

**The sound guard.** asm:10-11 is `cmp [SND_SpellCast@], e_ST_UNDEFINED` / `jz` to skip, so the source tests `!=` - [Spells133.c:518](../../MoM/src/Spells133.c#L518). `Play_Sound` takes two words here against one in the listing, the same project-wide host-audio form seen throughout this cohort.

**Four globals set before the loop, in the listing's order** - `CMB_Chasm_Anim_X`, `CMB_Chasm_Anim_Y`, `IMG_GUI_Chasm@`, then `CMB_Chasm_Anim` to `ST_TRUE` (asm:16-22). Production's names are `cmbt_cell_effect_cgx`, `_cgy`, `_seg` and `_active` at [Spells133.c:522](../../MoM/src/Spells133.c#L522)-[Spells133.c:525](../../MoM/src/Spells133.c#L525), the renames recorded in [Combat-Combat_Summon.md](Combat-Combat_Summon.md).

**The loop body.** `CMB_ChasmAnimStage = frame`, `Set_Page_Off`, `Combat_Screen_Draw`, `Combat_Cast_Spell_Message(caster_idx, spl_Cracks_Call)` - the spell pushed before the caster, so the source order is as [Spells133.c:532](../../MoM/src/Spells133.c#L532) has it - then `PageFlip_FX`. [Spells133.c:529](../../MoM/src/Spells133.c#L529) onward matches call for call.

**The animation is switched off at the end**, `CMB_Chasm_Anim = ST_FALSE` at asm:43, matching [Spells133.c:535](../../MoM/src/Spells133.c#L535). This function drives the effect entirely through those globals - it never draws the chasm itself, which is why the combat screen redraw inside the loop is what actually advances the picture.

### `Animate_Lightning_Bolt` ([Spells133.c:972](../../MoM/src/Spells133.c#L972), asm 118)

Faithful. No findings.

**Frame - exact match.** `screen_y` at `bp-4`, `screen_x` at `bp-2`, `sub sp, 4`, with `caster_idx` promoted into DI and `itr` in SI. [Spells133.c:974](../../MoM/src/Spells133.c#L974)-[Spells133.c:976](../../MoM/src/Spells133.c#L976) declares the two stack locals in that order.

**Three loops, and the first and third are the same shape.** asm:16-33 and asm:92-109 are both `for(itr = 0; itr < 5; itr++)` around `Combat_Screen_Draw`, a guarded `Combat_Cast_Spell_Message`, and `PageFlip_FX` - [Spells133.c:977](../../MoM/src/Spells133.c#L977) and [Spells133.c:1007](../../MoM/src/Spells133.c#L1007). **Neither calls `Set_Page_Off`**, while the middle loop does; that asymmetry is in the listing, so it is not an omission to tidy up.

**The caster guard is `> ST_UNDEFINED`.** All three loops test `cmp _DI_caster_idx, e_ST_UNDEFINED` / `jle` to skip - asm:20, asm:39 and asm:96, the last of which IDA prints as a literal `-1` rather than the enum. Same value, same test. `Combat_Cast_Spell_Message(caster_idx, spl_Lightning_Bolt)` pushes the spell then the caster, so the source order is as [Spells133.c:982](../../MoM/src/Spells133.c#L982) has it.

**The middle loop.** asm:34-90 is `for(itr = 0; itr < 2; itr++)` over `Set_Page_Off`, `Combat_Screen_Draw`, the caster message, and `Combat_Grid_Screen_Coordinates(cgx, cgy, 4, 4, &screen_x, &screen_y)` - six words, `add sp, 0Ch` - at [Spells133.c:995](../../MoM/src/Spells133.c#L995).

**The sound plays once, on the first pass.** asm:60-63 is `or si, si` / `jnz` then `cmp [magic_set.sound_effects], e_ST_TRUE` / `jnz`, both jumping to the same label. [Spells133.c:998](../../MoM/src/Spells133.c#L998) expresses it as nested `if`s rather than a single `&&`; the two forms are indistinguishable in the listing because Borland short-circuits both the same way, so this is a style choice and not a divergence.

**The animation frame is random per pass.** asm:68-72 is `Random(4)` then `dec ax`, giving 0..3 because `Random` is 1-based - [Spells133.c:1003](../../MoM/src/Spells133.c#L1003). `Clipped_Draw` then takes `screen_x` and `screen_y - 199` with the animation segment, three words with `add sp, 6`, at [Spells133.c:1004](../../MoM/src/Spells133.c#L1004).

**The tail.** asm:110-112 is `Set_Page_Off` / `Combat_Screen_Draw` / `PageFlip_FX` outside all three loops - [Spells133.c:1016](../../MoM/src/Spells133.c#L1016). Unlike its `Combat_Spell_Animation_Bolt` sibling this function never calls `Mark_Block`, so there is no `Release_Block` to pair and no `Combat_Compose_Background` at the end. That is the listing's shape, not a missing epilogue.

**`Play_Sound` takes two arguments here and one in the listing** - the same project-wide host-audio form noted for the bolt animation.

### `Combat_Spell_Animation_Bolt` ([Spells133.c:543](../../MoM/src/Spells133.c#L543), asm 206)

**Frame - exact match.** `sw_spell_idx` at `bp-0Eh`, `cgy_add` at `bp-0Ch`, `cgx_add` at `bp-0Ah`, `frame_num` at `bp-8`, `screen_y` at `bp-6`, `screen_x` at `bp-4`, `frame_count` at `bp-2`, `sub sp, 0Eh`, with `cgx` and `cgy` promoted into SI and DI. [Spells133.c:545](../../MoM/src/Spells133.c#L545)-[Spells133.c:551](../../MoM/src/Spells133.c#L551) declares the seven stack locals in that order.

**`Combat_Grid_Screen_Coordinates`** takes six words with `add sp, 0Ch`, pushed right to left as `&screen_y`, `&screen_x`, `4`, `4`, `cgy`, `cgx` - so the source order is `(cgx, cgy, 4, 4, &screen_x, &screen_y)`, matching [Spells133.c:552](../../MoM/src/Spells133.c#L552).

**The dispatch really is a `switch`.** asm:35-47 copies `spell_idx` into `sw_spell_idx` and then scans a four-entry table with `mov cx, 4` / `loop`, jumping through `[cs:bx+8]` on a hit - Borland's sparse-`switch` form for a small case set, where the addresses sit eight bytes past the values. The separate `sw_spell_idx` local exists because the switch expression is materialised before the scan; [Spells133.c:553](../../MoM/src/Spells133.c#L553) reproduces both the copy and the `switch`.

**All four cases match constant for constant.** Fireball is `-14`, `-21`, `+110`, `-66`, `-10`, `6`, 16; Fire Bolt is `-16`, `-20`, `+110`, `-66`, `-10`, `6`, 11; Doom Bolt is `-14`, `-25`, `screen_x` unshifted, `-72`, `0`, `6`, 13. Ice Bolt is `-17`, `-20`, `+110`, `-110`, `-10`, `10`, 11.

**Fire Bolt and Ice Bolt share their `frame_count` store.** asm:72 jumps from the Fire Bolt body to `loc_B7277`, which the Ice Bolt body falls into - one `mov [bp+frame_count], 11` serving both. That is Borland's cross-jump, so writing `frame_count = 11` in each case is the correct reconstruction rather than a duplication.

**The frame loop.** `Mark_Time`, then the three-way animation-frame choice: `frame_num` for Fireball, `frame_num / 4` for Doom Bolt (`idiv 4`), else 3 on the last frame or `frame_num % 3` (`idiv 3`, `push dx` taking the remainder). Then `Set_Page_Off` / `Copy_Back_To_Off`, and the `frame_num > 10` split - `cmp [bp+frame_num], 10` / `jle` - drawing at the fixed `screen_x` / `screen_y` with `Release_Time(2)` above the threshold, or at the travelling `cgx` / `cgy` with the `cgx_add` / `cgy_add` step and `Release_Time(1)` below it. [Spells133.c:611](../../MoM/src/Spells133.c#L611)-[Spells133.c:648](../../MoM/src/Spells133.c#L648) matches statement for statement.

**`Play_Sound` takes two arguments here and one in the listing.** asm:115-117 pushes only `[SND_SpellCast@]` and cleans one word; [Spells133.c:609](../../MoM/src/Spells133.c#L609) passes `SND_SpellCast_size` as well. That is the project-wide host-audio adaptation, not a reconstruction error - `Play_Sound(X, X_size)` is the form at every call site in the tree.

### `Combat_Spell_Animation` ([Spells131.c:994](../../MoM/src/Spells131.c#L994), asm 248)

**Frame — exact match.** `blue` at `bp-8`, `green` at `bp-6`, `red` at `bp-4`, `anim_size` at `bp-2`, `sub sp, 8`, with `spell_idx` promoted into SI and `itr` in DI. [Spells131.c:996-1000](../../MoM/src/Spells131.c#L996-L1000) declares the four stack locals in that order — note the colour triple runs blue, green, red rather than the reading order.

**The top-level split is a five-way `||` whose body sits far below.** asm:26-54 is five `cmp` / `jnz short <next>` / `jmp loc_B3927` pairs — `scc_Battlefield_Spell`, `scc_Counter_Spell`, `scc_Disenchant_Spell`, `spl_Raise_Dead`, `spl_Animate_Dead`. Each match short-circuits straight to `loc_B3927`, so that block is the `then` and the fall-through is the `else`. [Spells131.c:1001-1011](../../MoM/src/Spells131.c#L1001-L1011) has it that way round.

The two enum names differ from the listing and are the same values: IDA's `scc_Counter_Spell` is production's `scc_Combat_Counter_Magic` = 21 and `scc_Disenchant_Spell` is `scc_Disenchants` = 19 ([MOM_DAT.h:773](../../MoX/src/MOM_DAT.h#L773), [MOM_DAT.h:775](../../MoX/src/MOM_DAT.h#L775)).

**The inner split at `loc_B3927`.** Another five-way `||` — Flame Strike, Holy Word, Death Spell, Call Chaos, Mass Healing — with `jz` to the shared body at `loc_B3943`, which calls `Combat_Battlefield_Instant`. Four words pushed right to left with `add sp, 8` gives `Combat_Battlefield_Instant(player_idx, spell_idx, anims_on, caster_idx)`, matching [Spells131.c:1025](../../MoM/src/Spells131.c#L1025). The call goes through the `push cs` / `call near ptr` same-segment idiom because both functions live in `ovr131`.

**The `anims_on` early-out.** asm:56-60 is `cmp [bp+anims_on], e_ST_FALSE` / `jnz`, falling into `xor ax, ax` and a jump to the epilogue — so the source tests `anims_on != ST_FALSE` and the function returns without animating when animations are off. `loc_B3957` is the same test again on the other branch, sharing the same `@@JmpDone__Return_FALSE` target.

**The realm-to-colour mapping is a six-entry jump table.** asm:159-162 is `cmp bx, 5` / `ja loc_B39C3` / `shl bx, 1` / `jmp [cs:off_B3A41+bx]`, and the table itself is in `ovr131/_misc.asm`: `loc_B3980`, `loc_B3998`, `loc_B3991`, `loc_B39A4`, `loc_B39A6`, `loc_B39B2` for realms 0 to 5. Following each entry through the cross-jumps at `loc_B398A`, `loc_B39AB` and `loc_B39BC` gives Nature `(0, 63, 0)`, Sorcery `(0, 0, 63)`, Chaos `(63, 0, 0)`, Life `(63, 63, 63)`, Death `(0, 0, 0)` and Arcane `(63, 63, 63)`. All six match the `switch` on `spell_data_table[spell_idx].magic_realm` at [Spells131.c:1032](../../MoM/src/Spells131.c#L1032)-[Spells131.c:1075](../../MoM/src/Spells131.c#L1075) constant for constant, including the enum order (`sbr_Nature` = 0 through `sbr_Arcane` = 5, [MOM_DAT.h:732-737](../../MoX/src/MOM_DAT.h#L732-L737)), which is what the `cmp bx, 5` bound confirms.

The shared colour stores are Borland's tail-merge, so writing the triple out in each case is the correct reconstruction rather than a duplication — and `loc_B39A4` is a bare `jmp short loc_B39B2`, the Life body cross-jumped onto the identical Arcane body. The `jmp short $+2` at asm:188 is the degenerate trailing `break` that confirms this is a `switch` and not an `if` chain.

**The `default` arm has no asm counterpart.** An out-of-range realm takes the `ja` straight to `loc_B39C3` with `red` / `green` / `blue` left at whatever they held. The `default: { STU_DEBUG_BREAK(); } break;` at [Spells131.c:1070](../../MoM/src/Spells131.c#L1070) is the same class of addition recorded as D2.

**The tail.** asm:190-197 is `Set_Page_Off`, `Combat_Screen_Draw`, `Combat_Cast_Spell_Message(caster_idx, spell_idx)` — the spell pushed first, so the source order is as production has it — then `PageFlip_FX`. [Spells131.c:1076](../../MoM/src/Spells131.c#L1076)-[Spells131.c:1079](../../MoM/src/Spells131.c#L1079).

**The shade-and-restore loops.** `xor _DI_itr, _DI_itr` at asm:198 with the test `cmp _DI_itr, 20` / `jl` gives `for(itr = 0; itr < 20; itr++)`; asm:220 then re-initialises with `mov _DI_itr, 20` before the second test `cmp _DI_itr, -1` / `jg`, giving `for(itr = 20; itr > -1; itr--)`. Both bodies are `Set_Palette_Changes(0, 255)` — `255` pushed first — and `Tint_Palette((itr * 2), red, green, blue)`, four words with `add sp, 8`. [Spells131.c:1080](../../MoM/src/Spells131.c#L1080)-[Spells131.c:1089](../../MoM/src/Spells131.c#L1089) reproduces both, including the explicit re-initialisation to 20 that the down-loop's own `for` init already implies. Neither loop calls `Mark_Time` or `Release_Time`, so the fade runs as fast as the palette writes allow; that is the listing's behaviour, not a missing delay.

**The non-battlefield animator dispatch.** asm:61-125 matches [Spells131.c:1105](../../MoM/src/Spells131.c#L1105)-[Spells131.c:1150](../../MoM/src/Spells131.c#L1150) statement for statement: `Combat_Load_Spell_Sound_Effect(spell_idx)`, then the four-way bolt test into `Combat_Spell_Animation_Bolt(cgx, cgy, spell_idx, caster_idx)` — four words, `add sp, 8` — then Lightning Bolt into `Spell_Animation_Load_Graphics(spl_Call_Lightning)` followed by `Animate_Lightning_Bolt(cgx, cgy, caster_idx)` — three words, `add sp, 6` — and otherwise `Mark_Block(_screen_seg)` / `anim_size = Spell_Animation_Load_Graphics(spell_idx)` / `Release_Block(_screen_seg)` before the Cracks Call test and the `Combat_Spell_Animation_Default(cgx, cgy, anim_size, caster_idx, spell_idx)` fallback — five words, `add sp, 0Ah`. `anim_size` is captured only on that third path, which is why only the fallback gets a meaningful size.

**One OG quirk in that arm.** The bolt and Lightning Bolt paths both fall through to `loc_B391A`'s `Release_Block(_screen_seg)` without having called `Mark_Block` — only the third path marks the block it releases. Production reproduces it at [Spells131.c:1150](../../MoM/src/Spells131.c#L1150). It is harmless in practice because `Combat_Spell_Animation_Bolt` runs its own `Mark_Block` / `Release_Block` pair, but it is the listing's shape and stays.
