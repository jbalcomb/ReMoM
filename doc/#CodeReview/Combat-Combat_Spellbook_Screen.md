Combat-Combat_Spellbook_Screen.md

Combat_Cast_Spell__WIP()       ==>  Combat_Cast_Spell()
CmbBook_Draw__WIP()            ==>  Combat_Spellbook_Draw()
CmbBook_Compose__WIP()         ==>  Combat_Spellbook_Compose()
Combat_Spellbook_Build__WIP()  ==>  Combat_Spellbook_Build()
CMB_ComposeBackgrnd__WIP()     ==>  Combat_Compose_Background()
CMB_ComposeBookBG__WIP()       ==>  Combat_Compose_Spellbook_Background()
CMB_SpellSlider()              ==>  Combat_Spellbook_Mana_Adder_Screen()

CMB_SpellBookPage    ==>  g_combat_spellbook_left_page
CMB_RoadAnimStage    ==>  
CMB_WaterAnimStage   ==>  
CMB_CNodeAnimStage   ==>  
G_CMB_MWallAnimSkip  ==>  
CMB_CurseAnimStage   ==>  

CMB_SliderLimit         ==>  combat_max_extra_casting_cost
GAME_MP_SpellVar_1      ==>  g_spell_scratch_int
IMG_SBK_SliderBG        ==>  g_spell_screen_backdrop_seg
IMG_SBK_SliderDot       ==>  mana_slider_thumb_seg
IMG_SBK_SliderBar       ==>  mana_slider_arrow_bar_seg
xtramana_ok_button_seg
spl_anim_compose_seg

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Cast_Spell__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Spellbook_Screen.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Spellbook_Screen_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\CmbBook_Draw__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\CmbBook_Compose__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\Combat_Spellbook_Build__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\CMB_ComposeBackgrnd__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\CMB_ComposeBookBG__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr136\Combat_Spellbook_Mana_Adder_Screen.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr136\Combat_Spellbook_Mana_Adder_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr136\Combat_Spellbook_Mana_Adder_Load.asm

Combat_Screen()
    |-> j_Combat_Cast_Spell()
        |-> Combat_Cast_Spell()
    |-> Combat_Spellbook_Screen_Draw()
        |-> CmbBook_Draw__WIP()
    |-> Combat_Compose_Background()

Combat_Cast_Spell()
    |-> Combat_Spellbook_Screen()

Combat_Spellbook_Mana_Adder_Screen()
    |-> Combat_Spellbook_Mana_Adder_Load()
    |-> Combat_Spellbook_Mana_Adder_Draw()

---


# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-16).** No findings open; one deviation. Two passes: the spellbook screen chain (2026-07, nine functions, nine reconstruction errors) and the mana-adder cluster (2026-08-16, four functions against 709 asm lines).

The first pass covered `Combat_Spellbook_Screen` and everything it reaches, but not the mana-adder the cast path opens or the spellbook background compose. The second pass walked all four: `Combat_Compose_Spellbook_Background`, `Combat_Spellbook_Mana_Adder_Load`, `..._Draw` and `..._Screen`. Three reconstruction errors were raised in that pass and all three are fixed, along with four structural deviations. `cmake --build --preset MSVC-debug` is clean — `STU.lib`, `MOX.lib`, `momlib.lib`, `Platform_Headless.lib`, `ReMoM.exe` and `HeMoM.exe` all build and link.

**One question left open.** The `; is this really necessary?` annotation at [SPLMASTR.c:764](../../MoM/src/SPLMASTR.c#L764) asks whether the battle-unit branch earns its keep. The branch is faithful to `loc_BD71A` and is reachable — [CMBMAGIC.c:412](../../MoM/src/CMBMAGIC.c#L412) splits on the same boundary immediately before the call — but whether the `casting_cost * 4` cap inside it ever binds depends on which unit types reach the combat spellbook with what mana, which is `Combat_Cast_Spell`'s territory.

**Not verified here.** The four overland sites in `Spellbook_Mana_Adder_Draw` and `Spellbook_Mana_Adder_Screen` — [SPLMASTR.c:510](../../MoM/src/SPLMASTR.c#L510), [532](../../MoM/src/SPLMASTR.c#L532), [605](../../MoM/src/SPLMASTR.c#L605), [608](../../MoM/src/SPLMASTR.c#L608) — carry the same 32-bit casts as their combat counterparts, but their `ovr115` listings were never read. Expected to match; unconfirmed.

## Review status

| Function | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| Combat_Spellbook_Screen | Combat.c:13770 | ovr112/Combat_Spellbook_Screen.asm | faithful; R1 fixed |
| Combat_Spellbook_Screen_Draw | Combat.c:13117 | ovr112/Combat_Spellbook_Screen_Draw.asm | faithful |
| CmbBook_Draw__WIP | Spellbook.c:1514 | ovr117/CmbBook_Draw__WIP.asm | faithful |
| CmbBook_Compose__WIP | Spellbook.c:1272 | ovr117/CmbBook_Compose__WIP.asm | faithful; R2, R3 fixed |
| Combat_Spellbook_Build | Spellbook.c:827 | ovr117/Combat_Spellbook_Build.asm | faithful; R4, R5, R6 fixed; R9 (type) fixed |
| Combat_Spellbook_Add_Spells_From_Magic_Realm | Spellbook.c:531 | ovr117/Combat_Spellbook_Add_Spells_From_Magic_Realm.asm | faithful; R7 fixed |
| Combat_Compose_Background | Combat.c:30454 | ovr163/Combat_Compose_Background.asm | faithful; R8 fixed; phantom debug cluster noted (deferred) |
| Combat_Cast_Spell_With_Caster | Combat.c:9061 | ovr103/zCombat_Cast_Spell_With_Caster.asm | faithful |
| Combat_Cast_Spell (spellbook slice) | Combat.c:13250-13288 | ovr112/Combat_Cast_Spell__WIP.asm loc_8D0A8-loc_8D135 | faithful |

Line numbers drift as renames/fixes land; the ASM label anchors are stable.

---

# Reopened scope

Four functions, 709 asm lines. None has a coverage-table row anywhere, and none has a tracker box.

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Combat_Spellbook_Mana_Adder_Screen` | [SPLMASTR.c:732](../../MoM/src/SPLMASTR.c#L732) | `ovr136/Combat_Spellbook_Mana_Adder_Screen.asm` | 341 | **full body walk — faithful** |
| `Combat_Spellbook_Mana_Adder_Draw` | [SPLMASTR.c:677](../../MoM/src/SPLMASTR.c#L677) | `ovr136/Combat_Spellbook_Mana_Adder_Draw.asm` | 259 | **full body walk — faithful; D11** |
| `Combat_Spellbook_Mana_Adder_Load` | [SPLMASTR.c:650](../../MoM/src/SPLMASTR.c#L650) | `ovr136/Combat_Spellbook_Mana_Adder_Load.asm` | 85 | **full body walk — faithful** |
| `Combat_Compose_Spellbook_Background` | [CMBMAGIC.c:157](../../MoM/src/CMBMAGIC.c#L157) | `ovr112/CMB_ComposeBookBG__WIP.asm` | 24 | **full body walk — faithful** |

**Why these two came here.** Both are reached only from `Combat_Cast_Spell` — `Combat_Compose_Spellbook_Background` at [CMBMAGIC.c:317](../../MoM/src/CMBMAGIC.c#L317) and [330](../../MoM/src/CMBMAGIC.c#L330) and `Combat_Spellbook_Mana_Adder_Screen` at [CMBMAGIC.c:421](../../MoM/src/CMBMAGIC.c#L421) — so a caller-based attribution hands them to the cast path. They are spellbook-screen code by subject matter, and [Combat-Spell_Cast.md](Combat-Spell_Cast.md) already lists them under its `'Combat Spellbook Screen'` exclusion. Until now that exclusion pointed at this doc, which did not contain them; this scope closes that gap.

**The mana-adder is a cluster, not one function.** `Combat_Spellbook_Mana_Adder_Screen` calls `Combat_Spellbook_Mana_Adder_Load` at [SPLMASTR.c:781](../../MoM/src/SPLMASTR.c#L781) and `Combat_Spellbook_Mana_Adder_Draw` at [SPLMASTR.c:801](../../MoM/src/SPLMASTR.c#L801), and nothing else in the tree calls either helper. Reviewing the screen without them would leave two-thirds of `ovr136` unadjudicated with no other review able to claim it, so all three are in scope together.

**`CMB_ComposeBookBG__WIP` is not `Combat_Compose_Background`.** The first pass already covers `Combat_Compose_Background` (`ovr163`, the combat background) in the table above. `CMB_ComposeBookBG__WIP` is a different 24-line proc in `ovr112` that composes the spellbook background. The names differ by four characters and the overlays differ; do not conflate them.

## Reopened — verified faithful

### `Combat_Compose_Spellbook_Background` ([CMBMAGIC.c:157](../../MoM/src/CMBMAGIC.c#L157), asm 24)

Faithful, nothing to change. Five calls, same order, same arguments:

| asm | production |
| --- | --- |
| `call j_CMB_ComposeBackgrnd__WIP` (6) | `Combat_Compose_Background();` |
| `call Set_Page_Off` (8) | `Set_Page_Off();` |
| `call j_Combat_Screen_Draw` (10) | `Combat_Screen_Draw();` |
| `push [_spellbook_small_seg]` / `push 10` / `push 16` / `call FLIC_Draw` / `add sp, 6` (11-19) | `FLIC_Draw(16, 10, _spellbook_small_seg);` |
| `call Copy_Off_To_Back` (21) | `Copy_Off_To_Back();` |

- **Argument order.** The three pushes are right-to-left, so `16` is x, `10` is y and the segment is third; `add sp, 6` confirms three words. `_spellbook_small_seg` is a `SAMB_ptr` ([Spellbook.h:324](../../MoM/src/Spellbook.h#L324)), a single word, which is why the push is a bare memory operand with no widening.
- **Frame.** `push bp` / `mov bp, sp` with no `sub sp` — no locals — no parameters, and nothing loaded into `ax` before `retf`, matching the `void (void)` signature.
- **The `j_` prefixes are overlay thunks**, not different functions. `Combat_Compose_Background` lives in `ovr163` ([Combat.c:23004](../../MoM/src/Combat.c#L23004)) and `Combat_Screen_Draw` in `ovr099`; both are outside `ovr112`, so the call goes through the overlay stub.
- **The rename is already in the ledger.** `Combat_Compose_Background` to `Combat_Compose_Background` is recorded in [Combat-Combat_Screen.md](Combat-Combat_Screen.md), where that function carries its own `faithful` row.

Worth restating because the names are four characters apart: the proc walked here is `CMB_ComposeBookBG__WIP` in `ovr112`, which composes the **spellbook** background. `Combat_Compose_Background` in `ovr163` is the **combat** background, and is its first call.

### `Combat_Spellbook_Mana_Adder_Load` ([SPLMASTR.c:650](../../MoM/src/SPLMASTR.c#L650), asm 85)

Faithful, nothing to change. Ten statements in the asm's order: `Mark_Block`, two `Allocate_Next_Block` calls bracketing four `LBX_Reload_Next` loads, the six-symbol loop, then `Reset_Cycle_Palette_Color`.

- **Allocation sizes and order.** 40 paragraphs first (asm:10-16), then the four LBX loads, then 1090 (asm:49-55). Both allocations push size then `_screen_seg` and clean two words.
- **The four LBX records are 5, 3, 4, 42 in that order** (asm:17-48), each `LBX_Reload_Next(spellscr_lbx_file__ovr136, N, _screen_seg)` with the filename pushed last, and each result stored to its own word global — `IMG_SBK_SliderBG`, `IMG_SBK_SliderDot`, `IMG_SBK_SliderBar`, `xtramana_ok_button_seg`. Note 3 and 4 are loaded out of numeric order; that is the original's order, not a transcription slip.
- **The symbol loop.** `xor si, si` / `cmp si, 6` / `jl` — a plain count to `NUM_MAGIC_REALMS` (6, [MOX_DEF.h:61](../../MoX/src/MOM_DEF.h#L61)). Each iteration allocates 4 paragraphs into `spellbook_symbols_bitm[itr]`, then calls `Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr])` — source pushed second, destination first, matching the push order at asm:70-74. Both arrays are `SAMB_ptr[NUM_MAGIC_REALMS]` ([Spellbook.h:306](../../MoM/src/Spellbook.h#L306), [Spellbook.h:341](../../MoM/src/Spellbook.h#L341)), which is why the index is `shl bx, 1`.
- **Frame.** `push bp` / `mov bp, sp` with no `sub sp`; `itr` lives in SI and is the only local, matching production.

**Two renames are not in any ledger.** The asm stores the 40-paragraph block to `IMG_SBK_PageText@` and the 1090-paragraph block to `IMG_SBK_Anims@`; production uses `g_gui_scratch_bitmap` and `spl_anim_compose_seg`. The mapping is confirmed by address rather than by name — `dseg/_misc.asm:5131-5136` lays out `IMG_OVL_TargetWizBG@`, `IMG_SBK_Anims@`, `IMG_SBK_PageText@` in consecutive words, and [SPLMASTR.h:93-97](../../MoM/src/SPLMASTR.h#L93-L97) carries the matching `dseg:CA32`, `CA34`, `CA36` comments. The `CA32` entry kept its IDA name in the comment; the two below it dropped theirs, which is what made the mapping non-obvious. Worth restoring those two names to the header comments.

## Reopened deviations

**D11 — `GAME_MP_SpellVar_2` is split in two.** The original reuses one word across overlays: `ovr136` stores the caster index into it (`Combat_Spellbook_Mana_Adder_Screen.asm:20`) while `ovr137` and the FLIC path store an LBX segment handle. Production keeps `GAME_MP_SpellVar_2` as `SAMB_ptr` for the graphics role and introduces `_mana_adder_caster_idx` — marked `DNE in Dasm` at [SPLMASTR.h:74](../../MoM/src/SPLMASTR.h#L74) — for the caster role, with the original assignment preserved as a comment at [SPLMASTR.c:682](../../MoM/src/SPLMASTR.c#L682). Writer and reader moved together, so behaviour is unchanged; the split exists because the two roles have incompatible types.

## Verified faithful

`x_start` is 5 when `_temp_sint_1 > 5` and 165 otherwise (asm:12-17, the `> 5` case in the fall-through). `y_start = 21 + ((_temp_sint_1 % 6) * 22)` uses a signed `idiv` with the remainder in DX. `_osc_anim_ctr = (_osc_anim_ctr + 1) % 8` likewise. `Combat_Spellbook_Draw(16, 10, ...)` and `FLIC_Draw((x_start - 4), (y_start + 1), IMG_SBK_SliderBG)` match their push orders.

The two text passes are byte-identical to each other apart from the outline colour and shadow direction — 246 with `Set_Font_Style_Shadow_Up(4, 4, 4, 4)`, then 251 with `Set_Font_Style_Shadow_Down(4, 4, 4, 4)`, each followed by `Set_Alias_Color(6)`, `Print((x_start + 4), (y_start + 8), ...)` and `Print_Right((x_start + 144), (y_start + 8), string)`. The original really does draw the same two strings twice.

`Casting_Cost_Reduction(HUMAN_PLAYER_IDX, g_active_spell_idx)` matches the push order at asm:184-187, and `_players.casting_cost_remaining` with no index is `_players[0]`. The tail — `Draw_Picture_To_Bitmap`, `Set_Window`, two `Draw_Picture_Windowed` calls at `+8` and `-40`, `FLIC_Draw` of the slider dot, `Reset_Window` — matches offset for offset.

### `Combat_Spellbook_Mana_Adder_Screen` ([SPLMASTR.c:732](../../MoM/src/SPLMASTR.c#L732), asm 341)

**Verified faithful — the wizard branch.** `cost_reduction = Casting_Cost_Reduction(HUMAN_PLAYER_IDX, g_active_spell_idx)` (asm:28-34, arguments in push order), then `CMB_SliderLimit = casting_cost * 4` emitted as `shl ax, 2` (asm:41-43). The clamp is a genuine compute-compare-recompute: asm:44-63 evaluates `(Cmbt_Skill_Left * (cost_reduction + 100)) / 100`, compares with `jge` to skip, and asm:64-82 evaluates the *same expression a second time* to store it — production reproduces the duplication at [700](../../MoM/src/SPLMASTR.c#L700) and [702](../../MoM/src/SPLMASTR.c#L702) rather than caching it, which is correct.

The decrement loop is a test-at-bottom `while` whose body is the single `dec [CMB_SliderLimit]` at asm:86, with the condition at asm:87-125 comparing the 32-bit `((CMB_SliderLimit + casting_cost) * cost_reduction) / 100` against `Cmbt_Skill_Left` and looping back on greater. The second clamp follows the same compute-compare-recompute shape around `Combat_Casting_Cost_Multiplier` (asm:128) and two `Casting_Cost` calls (asm:153, asm:181), storing through `mov [CMB_SliderLimit], dx` at asm:185.

**Entry.** `_temp_sint_1 = spellbook_field_idx`, `_mana_adder_caster_idx = caster_idx`, `g_active_spell_idx = spell_idx`, then `Copy_On_To_Off_Page()` and `Copy_Off_To_Back()` — asm:17-23, same order. The frame is three slotted locals (`y_start`, `ok_button_field`, `input_field_idx`) with `spell_idx` in DI and `cost_reduction`/`x_start` sharing SI, matching [SPLMASTR.c:734-738](../../MoM/src/SPLMASTR.c#L734-L738).

## Walk completed — the two remaining spans

**The battle-unit branch** (asm:188-223) matches [SPLMASTR.c:764-770](../../MoM/src/SPLMASTR.c#L764-L770). `CMB_SliderLimit = battle_units[caster_idx].mana - casting_cost`, then `if((casting_cost * 4) < CMB_SliderLimit)` emitted as `shl ax, 2` / `cmp` / `jge` to skip, then the same `casting_cost * 4` recomputed to store — the compute-compare-recompute shape again, not a cached temporary. Worth noting `mana` is read with `mov ah, 0`, a zero-extend, confirming the unsigned field this project already recorded elsewhere.

**The screen position** (asm:225-240) tests the **parameter** `spellbook_field_idx` where `Combat_Spellbook_Mana_Adder_Draw` tests the global `_temp_sint_1`; production matches each. `x_start` is 5 when `> 5` and 165 otherwise, `y_start = 21 + ((spellbook_field_idx % 6) * 22)`.

**Field setup** (asm:242-302) is `Combat_Spellbook_Mana_Adder_Load()`, `_osc_anim_ctr = 0`, `GAME_MP_SpellVar_1 = spell_data_table[spell_idx].casting_cost` — the parameter, not `g_active_spell_idx` — `_xtra_mana_pos = 3`, `Clear_Fields()`, then the scroll field with its 0/106/3/103/106/7 arguments and `&_xtra_mana_pos`, then the OK button storing into `ok_button_field`. `Assign_Auto_Function` receives the draw routine as a far pointer, pushed segment-then-offset, with 2 as the second argument.

**The loop and exit** (asm:303-334) otherwise match: `_osc_leave_screen = ST_FALSE`, a bottom-tested `while(_osc_leave_screen == ST_FALSE)`, `Mark_Time()` then `Get_Input()` into `input_field_idx`, the `== ok_button_field` test setting the exit flag, and the redraw guarded on the flag still being clear. The tail is `Clear_Fields()`, `Deactivate_Auto_Function()`, `Release_Block(_screen_seg)`, `Combat_Compose_Background()`, and `return GAME_MP_SpellVar_1` taken from AX at asm:334.

## Reconstruction errors fixed (R-numbered)

**R1 — Combat_Spellbook_Screen cost-vs-limit guard restructured wrong.**
ASM loc_8E252: `cost>limit && ( caster_idx > (CASTER_IDX_BASE-1) || item.embed != spell )` with early-exit so the item is read only when `caster_idx <= 19`. Production had flattened it to a flat AND with the comparison inverted (`caster_idx <= CASTER_IDX_BASE`), which let unaffordable spells through for wizard casters and did an OOB `battle_units[20]` read at `caster_idx == 20`. Fix: `(caster_idx > (CASTER_IDX_BASE - 1))` inside an OR, restoring the item-read short-circuit.

**R2 — CmbBook_Compose city-enchant guard: `_combat_environ` compare inverted.** Spellbook.c city-enchant/Wall-of-Stone block.
ASM loc_9B6F3 (jz-skip): zero the icon count iff `attacker == HUMAN || environ != 1`. Production used `_combat_environ == cnv_Enemy_City` and `cnv_Enemy_City == 1`, i.e. `== 1` — the opposite. The parallel guard in Combat_Spellbook_Screen (Combat.c:13958) correctly uses `!= 1`. Fix: `(_combat_environ != cnv_Enemy_City)`.

**R3 — CmbBook_Compose second icon-row branch was a copy of the first.**
ASM: `itr2 < 20` → x=`itr2*6+1`, y=`itr1*22+7`; `itr2 >= 20` (loc_9B877) → x=`(itr2 % 20)*6+1`, y=`itr1*22+13`. Production's `else` branch was byte-identical to the `if`, so a spell needing >20 mana icons painted the overflow row back on the top row at runaway x. Fix: `(1 + ((itr2 % 20) * 6))`, `(13 + (itr1 * 22))`.

**R4 — Combat_Spellbook_Build missing `m_spell_list_count++` after adding a hero spell.**
ASM loc_9AC6C stores AND `inc [GUI_Multipurpose_Int]`. Production stored into `m_spellbook_spell_list[m_spell_list_count]` but never incremented, so each add clobbered slot 0. Fix: add `m_spell_list_count++;` after the store.

**R5 — Combat_Spellbook_Build charged-item block placed inside the `for(itr1<4)` loop.**
ASM loc_9ACAB runs the `Item_Charges > 0` block once, after the loop exit at loc_9ACA3. Production nested it inside the loop (ran 4x). Fix: move it after the loop's closing brace. Block content (Hero_Slot via `unit_idx`, `controller_idx` for owner) was correct.

**R6 — Combat_Spellbook_Build charged-item block never stored the spell.**
ASM loc_9AD4E: after the dedup scan, store `Charged_Spell_Index` and `inc` the count when not already present. Production had the dedup loop but no store+increment. Fix: add the `if(Already_Added == ST_FALSE){ store; count++; }`.

**R7 — Combat_Spellbook_Add_Spells_From_Magic_Realm missing `m_spell_list_count++`.**
ASM loc_9A622: `inc [GUI_Multipurpose_Int]` on the eligible fall-through (Eligibility != 1). Production stored but never incremented → at most one entry. Fix: `m_spell_list_count++;` inside the `if(Eligibility != 1)` block.

**R8 — CMB_ComposeBackgrnd ChaosNode volcano FLIC_Draw commented out.**
ASM loc_F924C has a live `FLIC_Draw(screen_x, screen_y, IMG_CMB_Volcano[8])` (`push [IMG_CMB_Volcano@+10h]` = word index 8). Production had it commented. Fix: uncomment. Constants/macros verified faithful: COMBAT_GRID_WIDTH=21 (asm `imul 42` is the byte stride = 21 words x 2), MAX_SINT=30000, CTILE_LeftRightRiver1=56, CS_ChaosNode=10, all six CALC_* macros.

**R9 — s_HERO.Spells reconstructed as `uint8_t`, defeating the ">127 ignored" OG bug.**
ASM Combat_Spellbook_Build reads Spells with `cmp [..], 0 / jg` and `mov al, [..]; cbw` — signed byte. Under the original `uint8_t Spells[4]`, indices 128-255 stayed positive, so production silently *fixed* the OG bug (never dropped high-index hero spells). Fix: field changed to `int8_t Spells[4]` (MOX/src/MOM_DAT.h:1099), restoring signed reads. Blast radius small: only Spellbook.c (the reads) and INITGAME.c (byte-preserving premade-hero writes) touch it; confirm `(uint8_t)->int8_t` writes are clean under /WX.

## OGBUG ledger — inline `// ; BUG:` annotations verified against the ASM

These are faithful OG behaviors (present in the disassembly). Do NOT fix in baseline.

- **Signed-cost overflow** (Combat_Spellbook_Build, CmbBook_Compose): wizard mana-icon budget = `(mana_reserve * 10) / cost_multiplier`, integer-truncated, stored into the signed-16-bit `g_spellbook_cast_mana_limit`. With the x0.5 multiplier this is `mana_reserve * 2`, wrapping negative past 16,383 mana. `mana_reserve` is uint16_t, `g_spellbook_cast_mana_limit` is int16_t; under MSVC int the 32-bit intermediate matches the asm LXMUL@/LDIV@ and truncates on store, reproducing it.
- **Charmed-caster item owner** (Combat_Spellbook_Build; Combat_Spellbook_Screen): embedded-item spell read from `_players[unit.controller_idx].Heroes[...]`, the current controller, not the original owner. A captured/charmed caster reads items from the wrong wizard. (CmbBook_Compose, by contrast, hardcodes player 0 — faithful, different quirk.)
- **Item bypasses legality** (Combat_Spellbook_Build Can_Cast scan): `Item_Charges > 0 -> Can_Cast = TRUE` outright, skipping the summon-on-water/terrain checks.
- **Base cost ignores Evil Omens** (Combat_Spellbook_Build battle-unit scan): compares base `casting_cost`, not the enchantment-adjusted cost. CONFIRMED: `Casting_Cost()` (NEXTTURN.c:1248-1268) applies a 3/2 multiplier to Nature/Life spells when any player holds `Globals[EVIL_OMENS] > 0`; the wizard path calls `Casting_Cost()`, the battle-unit path uses raw `spell_data_table[..].casting_cost`, so the battle-unit scan ignores Evil Omens.
- **Cracks Call raw-index compare** (Combat_Spellbook_Build, CmbBook_Compose): the Cracks-Call-on-ocean guard compares raw `spell_idx`, not `abs()`, unlike the adjacent summon check. Latent in combat unless a negated index reaches it.
- **`// conflicting condition - will always jump`** (Combat.c:13280): CONFIRMED accurate. Inside the `if(spell_idx == spl_NONE)` block, `is_spell_like_ability` is always ST_FALSE (the four ability cases at Combat.c:13212/17/22/27 each set spell_idx to a real spell before this block, so a spell-like ability never enters the spl_NONE branch). The `|| (is_spell_like_ability == ST_TRUE)` term is dead; the branch is governed solely by `type < scc_Infusable_Spell` and the jnz skip always fires.

## Naming ledger (this review)

Original -> recommended (variable = lower_snake_case; the Proper_Snake_Case originals violate the standard).

Combat_Spellbook_Build:
- Can_Cast -> has_castable_spell (return value: does the caster have any castable spell; gate to open the book)
- Already_Added -> already_in_list (dedup flag; candidate already in m_spellbook_spell_list)
- Charged_Spell_Index -> item_embed_spell_idx (embed_spell_idx of the caster's charged item)

Combat_Cast_Spell:
- Spell_Like_Ability -> is_spell_like_ability (cast came from a unit innate ability: Doom Bolt/Fireball/Web/Healing, not the spellbook)
- Selected_Spell -> selected_page_slot (page-relative slot 0-5 of the clicked spell; paired with g_combat_spellbook_left_page; NOT the spell id)
- Opponent_Index -> opposing_player_idx (caster's opponent; the counterer in the Counter Magic message)
- Overland_Cast_Save -> saved_overland_spell_idx (scratch backup of the human overland casting_spell_idx across the mana-adder screen)
- Effective_Cost -> effective_cost (post-Casting_Cost_Reduction cost charged to skill/mana; single-purpose)

CMB_SpellBookPage -> g_combat_spellbook_left_page (the saved combat page; parallel to overland g_spellbook_last_left_page)

## Combat_Cast_Spell — coalesced stack slots (boundaries CONFIRMED against asm)

The Borland compiler packed several disjoint-lifetime source locals into single stack slots. Splitting them into named locals is legitimate reconstruction ONLY where lifetimes are provably disjoint (a fresh definition before any read, no read pulling a prior value across the boundary). Every boundary below was traced against Combat_Cast_Spell__WIP.asm line numbers; a split is marked SAFE only when the region opens with a fresh write/address-take and the prior region's value is dead.

**`Target_X = word ptr -0Ch`** — five lifetimes:

| ASM lines | Role | Split |
| --- | --- | --- |
| 601-820 | counter-magic enchantment index + clobbering cost multiplier | **MERGED — do NOT split** |
| 827-870 | counter-magic realm | SAFE -> `counter_realm` |
| 913-920 | cost multiplier | SAFE -> `cost_multiplier` |
| 1110-1158 | real combat-grid X (address-taken 1110/1120) | SAFE -> `target_cgx` |
| 1174-1182 | cost multiplier (final payment) | SAFE -> `cost_multiplier` |

- The 601-820 region is the OG bug: the enchantment index (written 601/605, read 610/626) is overwritten with the cost multiplier at asm 656 and used at 664, yet the index is still read at 809-820 (`combat_enchantments[..] -= 5`). The `// ; BUG: this variable is still in use and needed below!` note at Combat.c:13474 documents it. Splitting silently fixes the bug — keep merged as `counter_magic_idx` (never a coordinate here; `target_cgx` is a misleading name in this block).
- Each SAFE region opens with a fresh definition (`mov [Target_X], ...` at 827/913/1174, or `lea` address-take at 1110) with no read of the prior region crossing in.

**`IDK_mana` is the `DI` register (`_DI_IDK_mana`), NOT a stack slot** (asm 355/568/590 `mov [Effective_Cost], _DI_IDK_mana`). No coalescing/fidelity concern — the `cast_mana` / `available_mana_pool` / `total_mana_cost` names are pure source-readability choices.

**`Can_Cast = word ptr -2`** — two disjoint lifetimes, SAFE to split:
- asm 145-146: Combat_Spellbook_Build gate -> `has_castable_spell` (dead after 146).
- asm 299-357: scratch save of `casting_cost_remaining` across the mana-adder -> `saved_casting_cost_remaining` (fresh write 299; partner to `saved_overland_spell_idx`).
- No `Can_Cast` reference between asm 146 and 299 -> disjoint, confirmed.

---
