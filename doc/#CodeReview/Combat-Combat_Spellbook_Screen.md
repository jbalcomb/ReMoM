Combat-Combat_Spellbook_Screen.md

Combat_Cast_Spell__WIP()       ==>  Combat_Cast_Spell()
CmbBook_Draw__WIP()            ==>  Combat_Spellbook_Draw()
CmbBook_Compose__WIP()         ==>  Combat_Spellbook_Compose()
Combat_Spellbook_Build__WIP()  ==>  Combat_Spellbook_Build()
CMB_ComposeBackgrnd__WIP()     ==>  Combat_Compose_Background()

AI_SetCombatRealms()  ==>  
AI_SelectCmbtSpell()  ==>  

CMB_SpellBookPage    ==>  g_combat_spellbook_left_page
CMB_RoadAnimStage    ==>  
CMB_WaterAnimStage   ==>  
CMB_CNodeAnimStage   ==>  
G_CMB_MWallAnimSkip  ==>  
CMB_CurseAnimStage   ==>  

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Cast_Spell__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Spellbook_Screen.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Spellbook_Screen_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\CmbBook_Draw__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\CmbBook_Compose__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\Combat_Spellbook_Build__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\CMB_ComposeBackgrnd__WIP.asm



ovr112  Combat Spellbook ~ CMBTSPBK?
CMBMAGIC?



g_spellbook_mode = 1;
...add enum/DEFINE



Combat_Screen__WIP()
    |-> j_Combat_Cast_Spell__WIP()
        |-> Combat_Cast_Spell__WIP()
    |-> Combat_Spellbook_Screen_Draw()
        |-> CmbBook_Draw__WIP()
    |-> CMB_ComposeBackgrnd__WIP()



Combat.c
Combat_Cast_Spell()
Line 13278
spell_idx = Combat_Spellbook_Screen(caster_idx, &Selected_Spell);

Combat_Cast_Spell()
    |-> Combat_Spellbook_Screen()



CMBMAGIC.LBX

COMPIX.LBX, 001  "CMBTBUTT"   "spell button"
COMPIX.LBX, 023  "LOCKBUTT"   "spell lock"

COMPIX.LBX, 005  "COMENCHI"   "light"
COMPIX.LBX, 006  "COMENCHI"   "darkness"
COMPIX.LBX, 007  "COMENCHI"   "warp reality"
COMPIX.LBX, 008  "COMENCHI"   "black curse"
COMPIX.LBX, 009  "COMENCHI"   "wrack"
COMPIX.LBX, 010  "COMENCHI"   "metal fires"
COMPIX.LBX, 011  "COMENCHI"   "prayer"
COMPIX.LBX, 012  "COMENCHI"   "high prayer"
COMPIX.LBX, 013  "COMENCHI"   "terror"
COMPIX.LBX, 014  "COMENCHI"   "lightning"
COMPIX.LBX, 015  "COMENCHI"   "counter magic"
COMPIX.LBX, 016  "CSPELL"     "right arrow"
COMPIX.LBX, 017  "CSPELL"     "left arrow"

---


# 1:1 Fidelity Review

**Status: DONE-DONE.** Nine reconstruction errors (R1-R9) found and fixed; one phantom debug cluster flagged for the deferred combat.c cleanup. Verified: `cmake --build --preset MSVC-debug` and `--preset MSVC-release` both pass clean (Release confirms `/WX` accepts the R9 `int8_t` field change and the `(uint8_t)->int8_t` premade-hero writes in INITGAME.c).

All reviewed functions have had their `__WIP` suffixes dropped (`CmbBook_Compose__WIP` -> `Combat_Spellbook_Compose`, `CmbBook_Draw__WIP` -> `Combat_Spellbook_Draw`, `Combat_Spellbook_Build__WIP` -> `Combat_Spellbook_Build`, `CMB_ComposeBackgrnd__WIP` -> `Combat_Compose_Background`). No residual.

Ground truth = IDA Pro 5.5 disassembly (`.asm` files listed above). Gemini `.c` files are second opinions, not OG truth. Every finding below was verified against the asm bytes.

## Review status

| Function | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| Combat_Spellbook_Screen | Combat.c:13770 | ovr112/Combat_Spellbook_Screen.asm | faithful; R1 fixed |
| Combat_Spellbook_Screen_Draw | Combat.c:13117 | ovr112/Combat_Spellbook_Screen_Draw.asm | faithful |
| CmbBook_Draw__WIP | Spellbook.c:1514 | ovr117/CmbBook_Draw__WIP.asm | faithful |
| CmbBook_Compose__WIP | Spellbook.c:1272 | ovr117/CmbBook_Compose__WIP.asm | faithful; R2, R3 fixed |
| Combat_Spellbook_Build__WIP | Spellbook.c:827 | ovr117/Combat_Spellbook_Build__WIP.asm | faithful; R4, R5, R6 fixed; R9 (type) fixed |
| Combat_Spellbook_Add_Spells_From_Magic_Realm | Spellbook.c:531 | ovr117/Combat_Spellbook_Add_Spells_From_Magic_Realm.asm | faithful; R7 fixed |
| CMB_ComposeBackgrnd__WIP | Combat.c:30454 | ovr163/CMB_ComposeBackgrnd__WIP.asm | faithful; R8 fixed; phantom debug cluster noted (deferred) |
| Combat_Cast_Spell_With_Caster | Combat.c:9061 | ovr103/zCombat_Cast_Spell_With_Caster.asm | faithful |
| Combat_Cast_Spell (spellbook slice) | Combat.c:13250-13288 | ovr112/Combat_Cast_Spell__WIP.asm loc_8D0A8-loc_8D135 | faithful |

Line numbers drift as renames/fixes land; the ASM label anchors are stable.

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

Combat_Spellbook_Build__WIP:
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
