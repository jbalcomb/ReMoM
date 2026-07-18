Spells128-Conquest_Spells.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr128\WIZ_ConquestSpells.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr128\WIZ_ConquestSpells.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

Change_City_Ownership()
    |-> j_Resolve_Wizard_Conquest()
        |-> Resolve_Wizard_Conquest()
            |-> Banish_Wizard()
                |-> Conquest_Spells()
            |-> Conquest_Spells()

---

# `Conquest_Spells` — Review / Walkthrough

| Function | Location | asm | Role |
|---|---|---|---|
| `Conquest_Spells` | [Spells128.c:1131-1205](../../MoM/src/Spells128.c#L1131-L1205) | `WIZ_ConquestSpells.asm` (IDA `ovr128:ACB00`, WZD o128p11) | After a conquest/banishment, awards the conqueror up to **2** random spells drawn from the *differential* spell list (spells the loser knew that the winner did not), then — only when the conqueror is player 0 (the human) — redraws the map and shows the "You found a `<spell>` spell in the ruins." popup. |

> **Status: DONE-DONE (2026-07-18) — faithful to `WIZ_ConquestSpells.asm` 1:1 except one documented, intentional divergence (D1); builds clean.** Structure, the neutral guard, the differential-list award loop (cap 2), the human-only (`winner==0`) message block, the 0/1/2/3-spell `switch`, the `SPELLSCR 60` image load, and the 13-arg `Notify2` call all match the disassembly (cross-checked against the Gemini `.c`). The lone departure — the spell-name copy uses a real buffer instead of reproducing the OG's used-as-pointer defect — is a deliberate, accepted divergence (D1).

## Signature & frame

`void Conquest_Spells(int16_t winner_idx, int16_t loser_idx)` ↔ asm `Conqueror` (bp+6, held in `di`) / `Target` (bp+8). Callers pass `(conqueror, loser)`: `Resolve_Wizard_Conquest` and `Banish_Wizard` both invoke it as `Conquest_Spells(winner, loser)`.

| Production local | asm slot | Notes |
|---|---|---|
| `spell_list[60]` | `Spells@` (-4Eh) | differential-list buffer filled by `Get_Differential_Spell_List` |
| `spell_list_idx` | `List_Index` (-12h) | `Random`-picked index |
| `new_spell_img` | `NewSpell_IMG@` (-10h) | SPELLSCR 60 image seg |
| `p_spell_name[LEN_SPELLDAT_NAME]` | `var_D` (**-0Dh, a single byte** in the asm) | **D1** — OG uses the byte's value as a pointer; production uses a real buffer. See below. |
| `awarded_spells[3]` | `Awarded_Spells` (-0Ch) | the spells granted (max 2 actually reached) |
| `spell_idx` | `Spell_Index` (-4) | current candidate |
| `spell_count` | `List_Count` (-2) | differential-list length |
| `awarded_count` | `si` | count granted so far |

## Verified faithful (phase notes)

- **Neutral guard** ([1143](../../MoM/src/Spells128.c#L1143)) — `winner==5 || loser==5 → return`, matching asm `cmp di,5 / jz` + `cmp Target,5 / jnz`.
- **Award loop** ([1148-1159](../../MoM/src/Spells128.c#L1148-L1159)) — `for(awarded_count=0; awarded_count < spell_count && awarded_count < 2; )`, incrementing `awarded_count` **only inside** the awarded branch (asm `inc si` inside the `Spell_Index != 0` path; loop exit `cmp si,List_Count / jge`, `cmp si,2 / jl`). Picks `Random(spell_count)-1`, awards via `Player_Gets_Spell(winner, spell_idx, 0)`, blanks the slot, records it.
- **Human-only message block** ([1160](../../MoM/src/Spells128.c#L1160)) — `if(winner_idx == 0)` matches asm `or di,di / jz`. Spells are granted to **any** winner; only player 0 (human) gets the redraw + popup. `Allocate_Reduced_Map / Full_Draw_Main_Screen / Copy_On_To_Off_Page / Copy_Off_To_Back` ([1162-1165](../../MoM/src/Spells128.c#L1162-L1165)).
- **Message `switch`** ([1166-1200](../../MoM/src/Spells128.c#L1166-L1200)) — the asm dispatches via `jmp [cs:off_ACD50+bx]` (`bx=awarded_count`, guard `cmp bx,3 / jbe`). The jump-table data isn't in the listing, but the four target labels build strictly increasing messages (`loc_ACBAB`=Msg1 "no spells"; `loc_ACBBD`=Msg2+name+Msg3; `loc_ACBD2`=+Msg4+name; `loc_ACC29`=+Msg5+name+Msg4+name), so the case→content mapping is unambiguous and matches production 0/1/2/3. (Case 3 is dead in both — the loop caps `awarded_count` at 2 — a faithful reproduction.)
- **Popup** ([1202-1203](../../MoM/src/Spells128.c#L1202-L1203)) — `new_spell_img = LBX_Reload_Next(spellscr, 60, _screen_seg)` (asm `3Ch`); `Notify2(0, 40, 2, GUI_NearMsgString, 0, new_spell_img, 0, 9, 0, 0, 0, 1, 0)` — all 13 args match the asm push order (`28h`=40).

## Divergence from the OG (D1 — intentional, accepted)

In the asm, the spell-name scratch `var_D` is a **single byte** (bp-0Dh) whose **value** is used as the destination pointer for the name copy — `_fstrcpy` is handed `0000:var_D` (`xor ax,ax` segment, `mov al,[bp+var_D] / cbw` offset). So the original writes each awarded spell's name to a garbage low-memory address; the "You found a `<spell>`…" popup would show corrupt text (or worse) when the human is awarded spells. The programmer clearly intended a name buffer (`char var_D[…]` / `&var_D`) but shipped the value-as-address.

Reproducing that verbatim (`char p_spell_name;` used as a pointer) is a `char`→`char*` type error that fails the project's warnings-as-errors (C4047 + C4024 → **C2220**, verified 2026-07-18 across all 12 use sites). Rather than hack around a defect this remote and unlikely to affect gameplay, production uses a real buffer:

```c
char p_spell_name[LEN_SPELLDAT_NAME] = { 0, … };   // 1138 — was the OG's single byte
…
stu_strcpy(p_spell_name, spell_data_table[awarded_spells[0]].name);   // ; BUG: value passed as pointer!  (now a real copy)
stu_strcat(GUI_NearMsgString, p_spell_name);
```

So the spell name renders **correctly** here — a behavioral **divergence** from the disassembly (the OG would show garbage). The divergence is deliberate and accepted as low-risk; if it ever matters it will surface in the OG-MoM matchup. The original defect is still marked at each site with `// ; BUG: value passed as pointer!` ([1173-1195](../../MoM/src/Spells128.c#L1173-L1195)); the two rejected alternatives (`char p_spell_name;` and `char * p_spell_name;`) are retained commented at [1136-1137](../../MoM/src/Spells128.c#L1136-L1137).

## Related references

- `…\ovr128\WIZ_ConquestSpells.asm` — IDA Pro 5.5 disassembly (**the authority**); the Gemini `.c` is a second opinion (agrees, and independently renders the `var_D`-as-pointer bug).
- `Get_Differential_Spell_List` — builds `spell_list` (spells the loser has that the winner lacks); `Player_Gets_Spell` — grants one spell to the winner.
- [`Resolve_Wizard_Conquest`](CONQUEST-Resolve_Wizard_Conquest.md) and `Banish_Wizard` — the two callers, both passing `(winner, loser)`.
