SPLMASTR-Undead_Animation.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\CMB_CreateUndeadAnim.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\CMB_CreateUndeadAnim.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\CMB_DrawUndeadAnim.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\CMB_DrawUndeadAnim.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\CMB_LoadUndeadAnim.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\CMB_LoadUndeadAnim.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\CMB_GetUndeadCreator.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\CMB_GetUndeadCreator.c

CMB_CreateUndeadAnim()  ==>  Undead_Animation()
CMB_DrawUndeadAnim()    ==>  Undead_Animation_Draw()
CMB_LoadUndeadAnim()    ==>  Undead_Animation_Load()
CMB_GetUndeadCreator()  ==>  Find_Undead_Creator_Type()

End_Of_Combat__WIP()
    |-> CMB_CreateUndeadAnim()
        |-> CMB_LoadUndeadAnim()
        |-> CMB_DrawUndeadAnim()

---

# `Undead_Animation` + `_Draw` + `_Load` — Review / Walkthrough

The post-combat **raise-undead** cut-scene: a crypt backdrop with three sprite layers rising out of it over 40 frames — rear, middle, and the newly-raised figure in front — behind a masking overlay, with a wrapped caption underneath. Plays when Zombies or a Death-realm creator raise the slain. (Renamed from the IDA labels `CMB_{Create,Draw,Load}UndeadAnim`; the asm files on disk keep the old names.)

| Function | Location | asm | Role |
|---|---|---|---|
| `Undead_Animation_Load` | [SPLMASTR.c:2115](../../MoM/src/SPLMASTR.c#L2115) | `CMB_LoadUndeadAnim.asm` (o138p09) | Loads the backdrop/mask plates, the two rising layers, and the raised-figure sprite. |
| `Undead_Animation_Draw` | [SPLMASTR.c:2144](../../MoM/src/SPLMASTR.c#L2144) | `CMB_DrawUndeadAnim.asm` (o138p10) | Per-frame: the three-layer rise, then the settled pose, plus the caption. |
| `Undead_Animation` | [SPLMASTR.c:2177](../../MoM/src/SPLMASTR.c#L2177) | `CMB_CreateUndeadAnim.asm` (o138p11) | Setup, the 100-frame ESC-abortable loop, `Release_Block`. |
| `Find_Undead_Creator_Type` | [Combat.c:22225](../../MoM/src/Combat.c#L22225) | `CMB_GetUndeadCreator.asm` (o123p02) | Picks which unit gets credit for the raising; supplies the `unit_type` argument. |

The scene's segment layers (draw order), with the IDA labels they replaced:

| Segment | was | Source | Role |
|---|---|---|---|
| `undead_bk_seg` | `IMG_CrUndead_BG` | CMBTFX #27 "UNDEADBK" | crypt backdrop, drawn first every frame |
| `undead_rm_seg` | `IMG_CrUndead_Back3` | CMBTFX #30 "UNDEADRM" | rear rising layer (`+17` travel) |
| `undead_fm_seg` | `IMG_CrUndead_Mid2` | CMBTFX #31 "UNDEADFM" | middle rising layer (`+40` travel) |
| `undead_ms_seg` | `IMG_CrUndead_FG` | CMBTFX #29 "UNDEADMS" | mask/foreground plate, drawn **between** the middle layer and the figure |
| `undead_figure_seg` | `IMG_CrUndead_Front1` | MONSTER.LBX (per `unit_type`) | the raised figure (`+80` travel) |

> **Status: DONE-DONE — faithful 1:1.** All four functions match the disassembly, and `momlib` builds with zero warnings (MSVC-debug, 2026-07-23). The only preserved-behaviour note is **B1** (the cross-player hero-slot index, which the asm has too). Two callee names are reconstruction renames of asm symbols, not deviations.

## `Find_Undead_Creator_Type` — what it does

Picks **which of the player's units gets credit for raising the undead**, returning a `unit_type` that `Undead_Animation_Load` turns into the MONSTER.LBX sprite. Scans all battle units, scoring each qualifying one by upkeep, and keeps the highest.

Faithful shape of the asm (all 178 lines):

```c
Highest_Value = -1;
Result        = 10;                                  /* ut_Druid → MONSTER.LBX 46 "Sword" */
for (si = 0; si < _combat_total_unit_count; si++)
{
    if (battle_units[si].controller_idx == Player_Index
        && ((battle_units[si].attack_attributes & Att_LifeSteal)
            || (battle_units[si].Abilities & UA_CREATEUNDEAD)))
    {
        Unit_Value = battle_units[si].upkeep + 100;
        if (Unit_Value > Highest_Value
            && _UNITS[battle_units[si].unit_idx].type > ut_Chosen)
        {
            Highest_Value = Unit_Value;
            Result        = _UNITS[battle_units[si].unit_idx].type;
        }
    }
    else if (Highest_Value < 10 && Player_Index < 6)   /* only until an ability-creator is found */
    {
        if (_UNITS[battle_units[si].unit_idx].Hero_Slot > -1)
        {
            for (di = 0; di < 3; di++)
            {
                Item_Index = _players[Player_Index].Heroes[ _UNITS[battle_units[si].unit_idx].Hero_Slot ].Items[di];
                if (Item_Index > -1 && (_ITEMS[Item_Index].Powers & IP_Vampiric))
                {
                    Highest_Value = 10;
                    Result        = 10;
                }
            }
        }
    }
}
return Result;
```

Notes on the mapping:

- **Two qualifying paths** — an *ability* creator (Life Steal or Create Undead, asm 38/45) scored `upkeep + 100`, or a hero carrying a **Vampiric item** (asm 150-157). Because any ability creator scores ≥ 100 and the item path only sets 10, an ability creator always wins; the `Highest_Value < 10` test (asm 91) latches the item path off once one is found.
- **The `ut_Chosen` gate** (asm 70, `jbe`) restricts the credited type to `> ut_Chosen` (34) — i.e. fantastic creatures, not heroes/normal units.
- **The default `10`** is `ut_Druid` ([UNITTYPE.h:23](../../MoM/src/UNITTYPE.h#L23)), which `Undead_Animation_Load` maps to MONSTER.LBX **46 "Sword"** ([2133](../../MoM/src/SPLMASTR.c#L2133)) — the item-did-it sprite. Both the vampiric-item case and the nothing-found case land there.
- **32-bit mask idiom** — asm 150-154 loads both halves, `and dx, IP_Vampiric` / `and ax, 0` / `or dx, ax`, which is Borland's codegen for a 32-bit `&` with a constant whose high word is 0. Net effect is the single test above.

### Constant mapping

| asm symbol | production |
|---|---|
| `ut_Chosen` = 34 | [UNITTYPE.h:47](../../MoM/src/UNITTYPE.h#L47) |
| `ut_Druid` = 10 | [UNITTYPE.h:23](../../MoM/src/UNITTYPE.h#L23) |
| `Att_LifeSteal` | [Combat.h:583](../../MoM/src/Combat.h#L583) |
| `UA_CREATEUNDEAD` = 0x0080 | [MOM_DEF.h:708](../../MoX/src/MOM_DEF.h#L708) |
| literal `6` (asm 96) | `MOO_MONSTER_PLAYER_IDX` = 6 ([MOX_DEF.h:613](../../MoX/src/MOX_DEF.h#L613)) |
| `IP_Vampiric` | `IP_VAMPIRIC` (also used by [ItemView.c:329](../../MoM/src/ItemView.c#L329), [UnitView.c:1184](../../MoM/src/UnitView.c#L1184)) |
| `s_ITEM.enchantments` | `s_ITEM.Powers` (`uint32_t`, `0x2E`) — the `IP_` bit family |

## B1 — cross-player hero-slot index (preserved OG behaviour — do not "fix")

The fallback item scan runs over **every** battle unit, including the opponent's. The asm's `loc_A68BF` is entered from two places:

- `cmp ax,[bp+Player_Index]` / `jz loc_A6826` / **`jmp loc_A68BF`** (asm 28-30) — any unit whose controller does *not* match; and
- `test […Abilities], UA_CREATEUNDEAD` / **`jz loc_A68BF`** (asm 45-46) — a matching unit with neither ability.

Having got there, it indexes `_players[Player_Index].Heroes[…]` with **that unit's** `Hero_Slot` (asm 117-141: the slot comes from `battle_units[si]`, the player from the parameter). So an *enemy* hero's slot number reads the **calling player's** hero in the same slot, and a Vampiric item on that unrelated hero can set the result.

Production reproduces this exactly — the fallback branch carries no controller test, so the mismatch path reaches it just as the asm does. Recorded here so the missing check is not later mistaken for an omission and "corrected"; the observable effect is confined to which sprite the cut-scene shows.

## Verified faithful (structure)

All three animation bodies map to the asm call-for-call:

- **`Undead_Animation_Load`** ([2115-2140](../../MoM/src/SPLMASTR.c#L2115-L2140)) — 1:1 with all 80 asm lines: `Mark_Block(_screen_seg)`, then `LBX_Reload_Next(cmbtfx, 27/29/30/31, _screen_seg)` into `undead_bk_seg`/`undead_ms_seg`/`undead_rm_seg`/`undead_fm_seg`, then the three-way `unit_type` branch (`> ut_Druid`, `== ut_Druid` → 46, else → 47). The `> ut_Druid` subtrahend `spell_data_table[spl_Magic_Spirit].unit_type` ([2128](../../MoM/src/SPLMASTR.c#L2128)) matches asm 48-50 exactly (`sub ax, es:[bx + (spl_Magic_Spirit*24h) + s_SPELL_DATA.Param0]` — a fixed-element read, `spl_Magic_Spirit = 201`, [Spellbook.h:227](../../MoM/src/Spellbook.h#L227); `.unit_type` is the semantic arm of the same byte as `.Param0`).
- **`Undead_Animation_Draw`** ([2144-2173](../../MoM/src/SPLMASTR.c#L2144-L2173)) — 1:1 with all 141 asm lines: `start_x=70` / `y_start=14` / `last_rising_frame=40`, `Copy_Back_To_Off`, the backdrop, `Set_Window(0,0,319,y_start+109)`, then the rising branch — `undead_rm_seg` at `y+17-(ctr*17)/40`, `undead_fm_seg` at `y+40-(ctr*40)/40`, `undead_ms_seg` **between** them and the figure, `undead_figure_seg` at `x+78, y+110-(ctr*80)/40` — versus the settled branch (`FLIC_Draw` all three, figure at `+78,+30`), then `Reset_Window`, the font setup (outline 16, shadow 4/4/0/0, alias 190), and `Print_Paragraph(x+7, y+114, 165, GUI_NearMsgString, 2)`. Every multiply-then-divide matches the asm's `imul`/`cwd`/`idiv` order.
- **`Undead_Animation`** ([2177-2208](../../MoM/src/SPLMASTR.c#L2177-L2208)) — 1:1 with all 79 asm lines: `_Load`, `_osc_anim_ctr = 0`, `Assign_Auto_Function(Undead_Animation_Draw, 2)`, `Clear_Fields`, `Add_Hidden_Field` ([2187](../../MoM/src/SPLMASTR.c#L2187)), the `(_osc_anim_ctr < 100) && !leave_screen` loop with `Mark_Time` / `Get_Input` / ESC test, the **inner** `leave_screen == ST_FALSE` guard around `Set_Page_Off`/`_Draw`/`PageFlip_FX` (asm `or si,si; jnz loc_C1620`), `Release_Time(2)` and the increment **outside** that guard, and the closing `Release_Block(_screen_seg)`.

And the creator query:

- **`Find_Undead_Creator_Type`** ([Combat.c:22225](../../MoM/src/Combat.c#L22225)) — matches the asm's scoring and ordering: `highest_value = -1` / `unit_type = 10` seed, the `controller_idx == player_idx` + (`Att_LifeSteal` | `UA_CREATEUNDEAD`) qualifier, `upkeep + 100` scoring with the `> highest_value` then `> ut_Chosen` nesting (asm 57-71, both tests in that order), the `continue` that skips the fallback for a qualifying unit (asm `loc_A68BC` → `loc_A696D`), the `highest_value < 10 && player_idx < MOO_MONSTER_PLAYER_IDX` latch (asm 91/96), the `Hero_Slot > -1` test, the 3-slot item loop, and `_ITEMS[item_idx].Powers & IP_VAMPIRIC` setting both `highest_value` and `unit_type` to 10. Return value is `unit_type` (asm 171).

## Cross-reference — the `sdt_201` idiom

This function resolves an open question from [`Summon_Animation`](Spells137-Summon_Animation.md): its asm subtrahend `s_SPELL_DATA_TABLE.sdt_201.Param0` is the **same field** read here, because `spl_Magic_Spirit = 201`. Where that asm rendered the access as an opaque `sdt_201` member, this one names it (`spl_Magic_Spirit*24h`), confirming both are `spell_data_table[spl_Magic_Spirit].Param0`. Worth using the named constant in `Spells137.c:302` instead of the bare `201`.

## Related references

- `…\ovr138\CMB_{Create,Draw,Load}UndeadAnim.asm` — IDA Pro 5.5 disassembly (**the authority**), retaining the pre-rename filenames; Gemini `.c` second opinions (which carried the same undefined `uy_BeastSwordsmen`, since resolved to MONSTER.LBX `47`).
- Callers — **both live and building**: [Combat.c:22165](../../MoM/src/Combat.c#L22165) `Undead_Animation(ut_Zombies)` and [:22207](../../MoM/src/Combat.c#L22207) `Undead_Animation(Find_Undead_Creator_Type(player_idx))`. Context: the `End_Of_Combat__WIP` note at [Combat.c:21260](../../MoM/src/Combat.c#L21260).
- Declarations already present: [SPLMASTR.h:116](../../MoM/src/SPLMASTR.h#L116) / [:119](../../MoM/src/SPLMASTR.h#L119) / [:122](../../MoM/src/SPLMASTR.h#L122).
