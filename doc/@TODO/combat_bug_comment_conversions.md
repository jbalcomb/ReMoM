combat_bug_comment_conversions.md

Remaining raw `; BUG:` / `TODO` markers in the Combat AoC. Regenerated 2026-08-26 against the
current tree; the scanner lives in the session scratchpad.

## Done so far

- **The `/* OGBUG: */` style-match is complete.** Every site carries the colon now — 176 `/* OGBUG:`
  and 6 `// OGBUG:`, against the 101/63/3/3 split this list started from. No bare `/* OGBUG ` remain.
- **`DEDU` is in use for the unverified questions** — 14 sites, up from 9.
- **The `TODO`s were reformatted** to `/* TODO: ... */` rather than converted. Correct: they are port
  work items, not claims about the 1994 code.

## Still to convert: 15

| category | n | action |
| --- | --- | --- |
| A — asserted OG bug | 11 | `/* OGBUG: <text> */` |
| B — port work item | 4 | leave as `TODO`; nothing to do |
| C — unverified question | 0 | `/* DEDU: <text>? */` |

Placement drives how much work each one is:

- **standalone** — 13
- **trailing on code** — 2
- **in a block comment** — 0

`standalone` is a one-line swap. `trailing on code` keeps the comment on the line — code lines are
not wrapped in this project — so only the wrapper changes. `in a block comment` means the marker is
one line inside a `/* ... */` block where an inner `/*` cannot nest; those need the block
restructured, or the marker left as prose and the claim hoisted to a `/* OGBUG: */` above the block.

---

## A — convert to `/* OGBUG: */` (11)

| file | function | line | placement | proposed |
| --- | --- | --- | --- | --- |
| CMBMAGIC.c | `Combat_Cast_Spell` | [393](../../MoM/src/CMBMAGIC.c#L393) | standalone | `/* OGBUG: causes counters to use the effective cost */` |
| CMBMAGIC.c | `Combat_Cast_Spell` | [443](../../MoM/src/CMBMAGIC.c#L443) | standalone | `/* OGBUG: ignores the base casting cost */` |
| CMBMAGIC.c | `Combat_Cast_Spell` | [445](../../MoM/src/CMBMAGIC.c#L445) | standalone | `/* OGBUG: ignores casting cost modifiers */` |
| CMBMAGIC.c | `Combat_Cast_Spell` | [451](../../MoM/src/CMBMAGIC.c#L451) | standalone | `/* OGBUG: ignores casting cost modifiers */` |
| CMBMAGIC.c | `Combat_Cast_Spell` | [454](../../MoM/src/CMBMAGIC.c#L454) | standalone | `/* OGBUG: ignores casting cost modifiers */` |
| CMBMAGIC.c | `Combat_Cast_Spell` | [470](../../MoM/src/CMBMAGIC.c#L470) | standalone | `/* OGBUG: Banish has an effective gain of 1/15 mana, not 5 */` |
| CMBMAGIC.c | `Combat_Cast_Spell` | [534](../../MoM/src/CMBMAGIC.c#L534) | standalone | `/* OGBUG: this variable is still in use and needed below! */` |
| CMBMAGIC.c | `Combat_Cast_Spell` | [806](../../MoM/src/CMBMAGIC.c#L806) | standalone | `/* OGBUG: also removes short range attacks */` |
| CMBMAGIC.c | `Combat_Spellbook_Screen` | [1014](../../MoM/src/CMBMAGIC.c#L1014) | standalone | `/* OGBUG: this may not be the unit's original owner */` |
| CMBMAGIC.c | `Do_Legal_Spell_Check` | [1175](../../MoM/src/CMBMAGIC.c#L1175) | standalone | `/* OGBUG: the former does not recognize confused units as not belonging to the player, while the latter co */` |
| CMBMAGIC.c | `Do_Legal_Spell_Check` | [1222](../../MoM/src/CMBMAGIC.c#L1222) | standalone | `/* OGBUG: the former does not recognize confused units as not belonging to the player, while the latter co */` |

## C — convert to `/* DEDU: */` (0)


## B — no action (4)

- [Combat.c:7690](../../MoM/src/Combat.c#L7690) in `Strategic_Combat_Allocate` — port work item, already `/* TODO: */`
- [Combat.c:19728](../../MoM/src/Combat.c#L19728) in `Claim_EMS_Page_For_Figure_Set` — port work item, already `/* TODO: */`
- [Combat.c:19739](../../MoM/src/Combat.c#L19739) in `Map_Tile_EMS_Page_As_Sandbox` — port work item, already `/* TODO: */`
- [CMBTAI.c:1706](../../MoM/src/CMBTAI.c#L1706) in `Auto_Move_Unit` — port work item, already `/* TODO: */`

---

## Notes

- **`Combat_Cast_Spell` still holds 8 of them** — the densest single target left.
- **Attribution is fixed in this pass.** A marker above a function definition now attributes to the
  function below it. The previous version filed `Battle_Unit_Ranged_Attack_Icon`'s OGBUG under
  `Battle_Unit_Melee_Attack_Icon`; that entry is gone from the list because the conversion is done.
