Spells137-Summon_Animation.md

summons creature/monster or hero/champion or item/artifact

Renamed:
IDK_SummonAnim()       ==> Summon_Animation()
IDK_SummonAnim_Draw()  ==> Summon_Animation_Draw()
IDK_SummonAnim_Load()  ==> Summon_Animation_Load()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr137\IDK_SummonAnim.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr137\IDK_SummonAnim.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr137\IDK_SummonAnim_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr137\IDK_SummonAnim_Draw.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr137\IDK_SummonAnim_Load.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr137\IDK_SummonAnim_Load.c

Cast_Spell_Overland()
    switch(spell_data_table[spell_idx].type)
        case scc_Summoning:
            if(spell_idx == spl_Floating_Island)
            ...
            else
                Create_Unit(spell_data_table[spell_idx].unit_type, player_idx, _players[player_idx].summon_wx, _players[player_idx].summon_wy, _players[player_idx].summon_wp, ST_UNDEFINED);
                Evict_Weakest_Unit((_units - 1));
            MultiPurpose_Local_Var = ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM);
            MultiPurpose_Local_Var = (MultiPurpose_Local_Var / 10);
            ...
            Summon_Animation(spell_data_table[spell_idx].unit_type, MultiPurpose_Local_Var, player_idx);
            g_ai_recompute_needed = ST_TRUE;

MUSIC.LBX
    // MUSIC.LBX, 013 "MOM56 XM"    "Weak Creature Summons"
    // MUSIC.LBX, 014 "MOM57 XM"    "Avg Creature Summons"
    // MUSIC.LBX, 015 "MOM58 XM"    "Powerful Creature Summ"
    // MUSIC.LBX, 016 "MOM59 XM"    "Mighty Creature Summon"

SPELLSCR.LBX
    // SPELLSCR.LBX, 046  "SUMMON00"    ""
    // SPELLSCR.LBX, 047  "SUMMON01"    ""
    // SPELLSCR.LBX, 048  "SUMMON02"    ""
    // SPELLSCR.LBX, 049  "SUMMON03"    ""
    // SPELLSCR.LBX, 050  "SUMMON04"    ""
    // SPELLSCR.LBX, 051  "SUMMON05"    ""
    // SPELLSCR.LBX, 052  "SUMMON06"    ""
    // SPELLSCR.LBX, 053  "SUMMON07"    ""
    // SPELLSCR.LBX, 054  "SUMMON08"    ""
    // SPELLSCR.LBX, 055  "SUMMON09"    ""
    // SPELLSCR.LBX, 056  "SUMMON10"    "horus"
    // SPELLSCR.LBX, 057  "SUMMON11"    "ariel"
    // SPELLSCR.LBX, 058  "SUMMON12"    "tlaloc"
    // SPELLSCR.LBX, 059  "SUMMON13"    "kali"
    // SPELLSCR.LBX, 060  "SMLRESCH"    ""
    // SPELLSCR.LBX, 061  "SUMMGREY"    ""
    // SPELLSCR.LBX, 062  "SUMGREEN"    ""
    // SPELLSCR.LBX, 063  "SUMMBLUE"    ""
    // SPELLSCR.LBX, 064  "SUMMRED"     ""
    // SPELLSCR.LBX, 065  "SUMWHITE"    ""
    // SPELLSCR.LBX, 066  "SUMMPURP"    ""

MONSTER.LBX
    // MONSTER.LBX, 000  "MONSTER1" "Magic Spirit"
    ...
    // MONSTER.LBX, 012  "MONSTER2" "Ghouls"
    ...
    // MONSTER.LBX, 023  "MONSTER3" "Angel"
    ...
    // MONSTER.LBX, 035  "MONSTER4" "Great Wyrm"
    ...
    // MONSTER.LBX, 046  "MONSTER2" "Sword"
    // MONSTER.LBX, 047  "BLANK"    ""

##### Naming Things Is Hard
"Creature"
"Monster"

---

# `Summon_Animation` + `_Draw` + `_Load` — Review / Walkthrough

The overland **summon / enchant** cut-scene: the casting wizard's figure in a flame frame, the conjured **subject** (creature, hero, or enchanted item) rising through two magic-effect layers, and a "*X* Summoned" / "Hero Summoned" / "Item Enchanted" caption, over `_osc_anim_ctr` while the summon music plays. Drives summon-unit, summon-hero, and enchant-item overland casts. (Renamed from the IDA placeholder `IDK_SummonAnim*`; the asm files on disk keep the old name.)

| Function | Location | asm | Role |
|---|---|---|---|
| `Summon_Animation_Load` | [Spells137.c:247](../../MoM/src/Spells137.c#L247) | `IDK_SummonAnim_Load.asm` (o137p01) | Loads the flame frame, palette flic, both fx layers, the subject sprite, and the caster figure. |
| `Summon_Animation_Draw` | [Spells137.c:376](../../MoM/src/Spells137.c#L376) | `IDK_SummonAnim_Draw.asm` (o137p02) | Per-frame: caster figure + rising subject + fx layers + caption. |
| `Summon_Animation` | [Spells137.c:433](../../MoM/src/Spells137.c#L433) | `IDK_SummonAnim.asm` (o137p03) | Setup, music, the `anim_stg_cnt` frame loop, teardown. |

The scene's segment layers (bottom → top), all keyed to the new names:

| Segment | Source | Role |
|---|---|---|
| `summon_flame_frame_seg` | SPELLSCR #9 "FLAMEFR1" | static backdrop, drawn once in `_Load` → cached, restored each frame |
| `spell_caster_figure_seg` | SPELLSCR `46 + wizard_id` | the casting wizard's figure (shared with `Spell_Of_Mastery`, ovr138) |
| `summon_fx_back_seg` | SPELLSCR #10 "GLOBALMK" | rear animated effect overlay |
| `spell_subject_figure_seg` | MONSTER.LBX (creature / hero / item) | the conjured subject, rising |
| `summon_fx_front_seg` | SPELLSCR #11 "ENCHANT" | front animated effect overlay |
| `summon_palette_seg` | SPELLSCR `46 + race_type` | palette donor only — never drawn |

> **Status: DONE-DONE — faithful 1:1.** All three functions match the disassembly call-for-call. The only preserved-behaviour note is **B1** (a `type_or_other == 0` gap the asm has too). The monster-index formula (former R1) is resolved and faithful — see Verified faithful. Two callees are reconstruction renames of asm symbols — `Dissolve_Main_Screen` = `OVL_MosaicFlip`, `Play_Background_Music` = the target of the asm's `j_…__STUB` thunk — not behavioural deviations.

## B1 — `type_or_other == 0` leaves the subject sprite unset (preserved OG behaviour)

In `_Load`, the subject sprite is assigned in the `type_or_other > 0` branch and the `-1` / `-2`/`-20` / `-3`/`-30` negative cases ([302-328](../../MoM/src/Spells137.c#L302-L328)). **`type_or_other == 0` matches none of them**, so `spell_subject_figure_seg` is never (re)assigned on that path — flagged by the `DEDU ¿ HERE … uninitialized ?` comment at [331](../../MoM/src/Spells137.c#L331).

This is **faithful to the asm**: `IDK_SummonAnim_Load.asm` does `or si,si; jle loc_BE953` (≤ 0 skips the `>0` load) then five `cmp`/`jz` against -1/-2/-20/-3/-30 — a zero falls through all of them and `IDK_MONSTER_seg` keeps its prior value. So this is a preserved OG quirk, not a reconstruction gap.

**DEDU (does not block fidelity):** determine whether `type_or_other == 0` is reachable. If a `unit_type`/spell of 0 can drive this path, the scene would draw whatever sprite the segment last held (a stale-pointer visual, not a crash — `spell_subject_figure_seg` is a valid segment from a prior summon). If game-state invariants guarantee `type_or_other != 0` here, it is a dead case. Preserve either way.

## Verified faithful

- **Monster-index formula (former R1 — resolved).** The `type_or_other > 0` load ([302](../../MoM/src/Spells137.c#L302)) is `spell_subject_figure_seg = LBX_Reload_Next(monster, type_or_other - spell_data_table[201].unit_type, …)`. This matches the asm exactly (`IDK_SummonAnim_Load.asm:103-105`): `sub ax, [es:bx + s_SPELL_DATA_TABLE.sdt_201.Param0]` — `sdt_201` = element **[201]**, `.Param0` = the union alias read here as `.unit_type` (the field holds a `rt_`/`e_RACE_TYPE` value). The earlier `DBG_entry_num` scratch ladder and the hardcoded-`154` form are gone.
- **`_Draw`** ([376-429](../../MoM/src/Spells137.c#L376-L429)) — 1:1 with all 184 lines of asm: `start_x=30`/`start_y=42`, `Copy_Back_To_Off` (restores `summon_flame_frame_seg`), the two `Set_Window` clips, `Clipped_Draw` of `spell_caster_figure_seg` at `(+8,+3)`, `FLIC_Draw` `summon_fx_back_seg` at `(+55,+54)`, the rising-subject gate `_osc_anim_ctr < 30 && spell_animations && _osc_player_idx == HUMAN_PLAYER_IDX` → `y = start_y + 110 - _osc_anim_ctr*3` else `start_y + 18`, `summon_fx_front_seg`, and the caption ladder (`_osc_summon_unit_type > 0` → "*name* Summoned"; `== -1` → "Item Enchanted"; `> -10` → "Hero Summoned"; else "Champion Summoned"). Alias confirmed: asm `_temp_sint_1__bldg_city_player_spell_idx` = `_osc_player_idx` ([Spells137.c:210](../../MoM/src/Spells137.c#L210) `#define _osc_player_idx _temp_sint_1`); asm `_temp_sint_4` = `_osc_summon_unit_type` ([MOM_DAT.h:4778](../../MoX/src/MOM_DAT.h#L4778)). The caption's `.unit_type` ([408](../../MoM/src/Spells137.c#L408)) is the reconstruction's alias of the asm's `.Param0` (`_Draw.asm:123`).
- **`Summon_Animation`** ([433-495](../../MoM/src/Spells137.c#L433-L495)) — 1:1: `Stop_All_Sounds`, music `LBX_Reload(music, MUSIC_Weak_Summon + magic_realm_spell_idx)` ([443](../../MoM/src/Spells137.c#L443)), the background-music gate, `_osc_anim_ctr=0` / `_osc_player_idx=player_idx` / `_osc_summon_unit_type=unit_type` ([449-451](../../MoM/src/Spells137.c#L449-L451)), `Allocate_Reduced_Map`, `Full_Draw_Main_Screen`, `Summon_Animation_Load`, `Assign_Auto_Function(Summon_Animation_Draw, 2)`, `pfe_Dissolve`, `Add_Hidden_Field`, `anim_stg_cnt = spell_animations ? 130 : 40` ([461/465](../../MoM/src/Spells137.c#L461)), the `(_osc_anim_ctr < anim_stg_cnt) && !_osc_leave_screen` loop ([468](../../MoM/src/Spells137.c#L468)) with the `Get_Input == full_screen_field` break and the `pfe_None` reset after the first frame, then `Deactivate_Auto_Function`/`Dissolve_Main_Screen`/`Stop_All_Sounds`/`Release_Block`/`Play_Background_Music` ([482-486](../../MoM/src/Spells137.c#L482-L486)), and the `player_idx == HUMAN ? mouse_list_default : mouse_list_hourglass` tail.
- **`_Load` (rest)** ([247-371](../../MoM/src/Spells137.c#L247-L371)) — the `type_or_other < 0 ? rt_Arcane : _unit_type_table[type_or_other].race_type` select, `unit_race += 46` ([259](../../MoM/src/Spells137.c#L259); asm 39-40) — the base offset mapping race → the SPELLSCR SUMMON-palette flic band (46-66), sourced only for its palette via `summon_palette_seg`; the `Mark_Block`/`LBX_Reload_Next` sequence for the flame frame / palette flic / both fx layers; the negative-`type_or_other` hero/item branch (`-1`→46 "Sword", `-2`/`-20`→45 "Male Hero", `-3`/`-30`→44 "Female Hero"); `spell_caster_figure_seg = spellscr[46 + _players[player_idx].wizard_id]`; and the closing palette reload + `Apply_Palette`. All arg-for-arg.

## Related references

- `…\ovr137\IDK_SummonAnim{,_Draw,_Load}.asm` — IDA Pro 5.5 disassembly (**the authority**), retaining the pre-rename filenames; Gemini `.c` second opinions.
- Caller chain: overland spell dispatch → `Summon_Animation` for summon-unit / summon-hero / enchant-item casts.
- `spell_caster_figure_seg` is **shared with** `Spell_Of_Mastery` (ovr138), which loads it from `splmastr[14 + wizard_id]` — the cross-module sharing consistent with an ovr137→ovr138 split.
- `_temp_sint_1` / `_temp_sint_4` are shared scratch globals; the `_osc_*` aliases are local `#define`s in [Spells137.c](../../MoM/src/Spells137.c#L210).
