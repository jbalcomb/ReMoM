Combat-Combat_Spell_Dispel.md

SEEALSO:  Combat-Spell_Cast.md
SEEALSO:  Combat-Cast_Spell_On_Battle_Unit.md

WIZ_DispelAttempt__STUB()   ==>  Combat_Spell_Dispel_Attempt()
CMB_CounterMessage__STUB()  ==>  Combat_Spell_Counter_Message()
GUI_DrawNearMessage()       ==>  Combat_Spell_Counter_Message_Box_Draw()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\WIZ_DispelAttempt__STUB.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\CMB_CounterMessage__STUB.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\GUI_DrawNearMessage.asm

Combat_Cast_Spell()
    |-> Combat_Spell_Dispel_Attempt()
        |-> Calculate_Dispel_Difficulty()
    |-> Combat_Spell_Counter_Message()
        |-> Combat_Spell_Counter_Message_Box_Draw()

Calculate_Dispel_Difficulty() is shared, not combat specific, so it belongs to the overland code.

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-17).** No findings open; one deviation. Three functions, 575 asm lines, every one walked against its listing.

Two reconstruction errors were raised and both are fixed, along with one structural deviation. D1 stays on the record — the original calls `Get_Paragraph_Max_Height` with three arguments against a two-parameter callee, which C99 cannot express. `cmake --build --preset MSVC-debug` is clean: `STU.lib`, `MOX.lib`, `momlib.lib`, `Platform_Headless.lib`, `ReMoM.exe` and `HeMoM.exe` all build and link.

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Combat_Spell_Counter_Message` | [Spells133.c:532](../../MoM/src/Spells133.c#L532) | `ovr133/Combat_Spell_Dispel_Attempt.asm` | 375 | **full body walk — faithful** |
| `Combat_Spell_Counter_Message_Box_Draw` | [Combat.c:12364](../../MoM/src/Combat.c#L12364) | `ovr113/Combat_Spell_Dispel_Attempt.asm` | 142 | **full body walk — faithful; D1** |
| `Combat_Spell_Dispel_Attempt` | [Spells133.c:651](../../MoM/src/Spells133.c#L651) | `ovr133/Combat_Spell_Dispel_Attempt.asm` | 58 | **full body walk — faithful** |

**Why these three are one bundle.** `Combat_Spell_Dispel_Attempt` and `Combat_Spell_Counter_Message` are each reached only from `Combat_Cast_Spell` — the dispel roll at [CMBMAGIC.c:523](../../MoM/src/CMBMAGIC.c#L523) and [623](../../MoM/src/CMBMAGIC.c#L623), the message at [CMBMAGIC.c:529](../../MoM/src/CMBMAGIC.c#L529) and [630](../../MoM/src/CMBMAGIC.c#L630) — so a caller-based attribution hands both to the cast path even though they are counter-magic code by subject. `Combat_Spell_Counter_Message_Box_Draw` has exactly one caller in the tree, `Combat_Spell_Counter_Message`, so it comes with them rather than staying loose in `ovr113`.

Both `ovr133` procs carry `__STUB` suffixes in the IDA database. That is inherited from the disassembly and says nothing about whether production is complete — verify against the listings, not the name.

## Renames

| dasm | production |
| --- | --- |
| `Combat_Spell_Dispel_Attempt` | `Combat_Spell_Dispel_Attempt` |
| `Combat_Spell_Counter_Message` | `Combat_Spell_Counter_Message` |

## Deviations

**D1 — `Get_Paragraph_Max_Height` is called with three arguments.** `seg019/Get_Paragraph_Max_Height.asm` declares two parameters, `max_width` at `bp+6` and `string` at `bp+8`, and never reads `bp+0Ah`. `Combat_Spell_Counter_Message_Box_Draw` pushes three words anyway and cleans six bytes:

```
mov     ax, 2
push    ax
mov     ax, offset GUI_NearMsgString
push    ax
mov     ax, 150
push    ax
call    Get_Paragraph_Max_Height
add     sp, 6
```

`ovr113/Combat_Spell_Dispel_Attempt.asm:51-58` — byte-for-byte the same shape as the `Get_Paragraph_Max_Width` call eight lines above it at asm:42-49, which pushes the same `2`.

The value is the paragraph print type: 0 left, 1 right, 2 centre, 3 full width, per the `switch` in `Get_Paragraph_Max_Width` ([paragrph.c:515-538](../../MoX/src/paragrph.c#L515-L538)). The height function cannot use it — `ymax` is `paragraph_line_y_start[max_lines - 1] + font_height` and the line breaking is done by `Mark_Paragraph`, which takes no alignment argument. Passing it is nonetheless deliberate rather than accidental: across all five call sites in the game the third argument tracks the print type of the adjacent `Print_Paragraph`, including a `0` where that call passes `0`.

**Not reproducible in C99** — a long-argument call is a constraint violation, the mirror of the short-argument `Gradient_Fill` calls recorded in [Combat-Combat_Screen.md](Combat-Combat_Screen.md). Production passes two arguments, which is behaviourally identical because the callee ignores the third. The comment at [Combat.c:12380](../../MoM/src/Combat.c#L12380) records the original form; it is a deviation note, not pending work.

The other four call sites are in `Combat_Results_Scroll_Text` and are recorded as D6 in [Combat-End_Of_Combat.md](Combat-End_Of_Combat.md), which owns that function.

## Findings

None open.

## Verified faithful

### `Combat_Spell_Counter_Message` ([Spells133.c:532](../../MoM/src/Spells133.c#L532), asm 375)

**Frame.** `Temp_String` at `bp-18h`, `Hero_Slot` at `bp-4`, `Display_Counter` at `bp-2`, `sub sp, 18h`. The buffer spans `bp-18h` to `bp-5`, i.e. **20 bytes** = `LEN_TEMP_STRING` ([MOX_BASE.h:31](../../MoX/src/MOX_BASE.h#L31)) — worth stating because the sibling `Combat_Spellbook_Mana_Adder_Draw` had over-sized buffers and this one does not. `string_length` and `caster_type_name` are register locals, DI and a folded BX, so they carry no ordering constraint.

**The `type` chain.** asm:17-20 tests `5` then `0` and falls through to the wizard-name case. The node branch is laid down far away at `loc_B6E47` with the `type == 0` body immediately after the test — layout, not source order.

**Three trailing-`s` tests, three different addressing forms, all correct.**

- asm:53 reads `[_players.wizard_id+bx]` where `bx = type * sizeof(s_WIZARD) + strlen`. That looks wrong and is not: `name` sits at offset 1 in `s_WIZARD`, so IDA named the byte using the preceding field. The effective address is `_players[type].name[strlen - 1]`, matching [566](../../MoM/src/Spells133.c#L566).
- asm:181 is `[bp+di+near_buffer-1]` — the stack buffer indexed by `strlen - 1`, matching [589](../../MoM/src/Spells133.c#L589).
- asm:300 folds the caster rebase into the base constant: `offset _players.name-(14h*4c8h)-1`, with `14h` = 20 = `CASTER_IDX_BASE` and `4c8h` = 1224 = `sizeof(s_WIZARD)`. Adding `caster_idx * sizeof(s_WIZARD)` and `strlen` yields `_players[caster_idx - CASTER_IDX_BASE].name[strlen - 1]`, matching [618](../../MoM/src/Spells133.c#L618).

**The caster chain.** asm:117-130 is `== CASTER_IDX_BASE`, then `< CASTER_IDX_BASE`, then the wizard case — three arms in production's order. Inside the second, asm:145 is `cmp [bp+Hero_Slot], -1` / `jle`, so the source reads `> -1`.

**The spell-name copy is a far call.** asm:317-329 pushes the `spell_data_table` segment and offset as a far pointer and `0` as the destination segment, then calls `_fstrcpy` with `add sp, 8`. Production uses `stu_strcpy`. The `0`-segment-for-a-near-destination pattern is already recorded in [Combat-End_Of_Combat.md](Combat-End_Of_Combat.md) for that overlay's `_fstrcpy` calls; this is the same substitution.

**The display branch and hold loop.** asm:344-350 is `cmp [_auto_combat_flag], e_ST_FALSE` / `jnz` to the else, so `Warn1(GUI_NearMsgString)` runs when the flag is clear and `Play_Standard_Click()` plus the loop when it is set. The loop is `mov Display_Counter, 0`, a bottom test `cmp 30` / `jl`, and a body of `Mark_Time`, `Set_Page_Off`, `Combat_Screen_Draw`, `Combat_Spell_Counter_Message_Box_Draw`, `PageFlip_FX`, `Release_Time(2)`, `inc Display_Counter` — thirty paced redraws, matching [637-645](../../MoM/src/Spells133.c#L637-L645) statement for statement.

### `Combat_Spell_Dispel_Attempt` ([Spells133.c:651](../../MoM/src/Spells133.c#L651), asm 58)

`Calculate_Dispel_Difficulty(spell_cast, player_idx, magic_realm)` matches the push order at asm:15-19, and its result plus `dispel_strength` becomes the difficulty total. The chance is a genuine 32-bit computation — asm:22-37 sign-extends the total as the divisor, sign-extends `dispel_strength`, loads `250` as a long, then goes through `LXMUL@` and `LDIV@`, taking the low word of the quotient. [Spells133.c:656](../../MoM/src/Spells133.c#L656) carries the `(int32_t)250` that reproduces it. The roll is `Random(250)` compared with `jg` to the false exit, so the source reads `if(Random(250) > chance) return ST_FALSE;` then `return ST_TRUE`.

Two notes. The quotient lands in an `int16_t` by implicit narrowing rather than an explicit cast — behaviourally the same low word the asm keeps, but it is an `int32_t` to `int16_t` assignment and Release builds with `/WX`, so it is the same C4244 exposure the mana-adder sites were given explicit casts for. And asm:48-49 are raw bytes, `db 0EBh` / `db 4` — an unreferenced `jmp short +4` stranded by the return-value pattern, with no production counterpart and none wanted.

### `Combat_Spell_Counter_Message_Box_Draw` ([Combat.c:12364](../../MoM/src/Combat.c#L12364), asm 142)

**The frame confirms the declaration order exactly** — `height` at `bp-0Ah`, `width` at `bp-8`, `colors[2]` at `bp-6`, `y2` at `bp-4`, `x2` at `bp-2`, with `x1` in SI and `y1` in DI, matching [Combat.c:12366-12372](../../MoM/src/Combat.c#L12366-L12372) line for line.

`colors[0] = 182`, `colors[1] = 177`, then `Set_Font_Colors_15(0, &colors[0])`, `Set_Font_Style_Shadow_Down(1, 15, 0, 0)`, `Set_Alias_Color(182)` and `Set_Outline_Color(2)` in that order. Both measurements wrap at 150 and both use the signed-halve idiom `cwd` / `sub ax, dx` / `sar ax, 1` for the centring arithmetic. `y1 = 5` is a literal and `y2 = height + 8`.

The four `Line` calls draw the bevel in the asm's order with the right corners — `(x1, y1, x1, y2, 230)`, `(x1, y1, x2 - 1, y1, 230)`, `(x2, y1, x2, y2, 237)`, `(x1 + 1, y2, x2, y2, 237)` — light on top and left, dark on right and bottom. `Print_Paragraph(x1 + 6, 8, 150, GUI_NearMsgString, 2)` closes it, five words with `add sp, 0Ah`, print type 2 matching the width measurement.

The three-argument `Get_Paragraph_Max_Height` call at asm:51-58 is D1.
