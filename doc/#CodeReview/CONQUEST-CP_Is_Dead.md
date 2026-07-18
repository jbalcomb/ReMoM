CONQUEST-CP_Is_Dead.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_IsWon__STUB.asm   (ground truth — IDA Pro 5.5 disassembly; the `__STUB` suffix is the Piethawn extraction convention, not a stub body)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_IsWon__STUB.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

Resolve_Wizard_Conquest()
    |-> CP_Is_Dead()

---

# `CP_Is_Dead` — Review / Walkthrough

| Function | Location | Role |
|---|---|---|
| `CP_Is_Dead` | [CONQUEST.c:947-981](../../MoM/src/CONQUEST.c#L947-L981) | IDA `ovr093:0DAF`, WZD 093p09. Scans player slots `1 … _num_players-1` and counts how many are still "alive" — a slot counts if it has an active fortress **or** owns at least one city. Returns `ST_TRUE` when zero opponents remain (human victory / all computer players eliminated), else `ST_FALSE`. |

> **Status: done-done — faithful to `GAME_IsWon__STUB.asm` 1:1.** Structure, loop bounds, the `active_computer_players` accumulator, the two "alive" tests, and the inverted return all match the disassembly instruction-for-instruction. The Gemini `.c` agrees. No deviations.

## Signature & frame

```c
int16_t CP_Is_Dead(void)
```

Locals map onto the asm's frame / register slots exactly:

| Production local | asm slot | Notes |
|---|---|---|
| `active_computer_players` | `di` | the accumulator; `xor di, di` init. Gemini names it `active_other_players` — same slot |
| `itr_players` | `si` | outer loop index (starts at 1) |
| `city_count` | `City_Count` (bp-04h) | per-opponent city tally, capped at 2 |
| `itr_cities` | `itr_cities` (bp-02h) | inner loop index |

## Structure map (asm ↔ production)

| Step | asm | production | fidelity |
|---|---|---|---|
| `di = 0` | `xor di, di` (12) | [953](../../MoM/src/CONQUEST.c#L953) | faithful |
| Outer loop `itr_players = 1 … < _num_players` | `mov si,1` / `loc_799B4` | [954](../../MoM/src/CONQUEST.c#L954) | faithful |
| Alive-by-fortress: `_FORTRESSES[itr_players].active == ST_TRUE → di++` | `loc_7995E`-`loc_79974` | [956-959](../../MoM/src/CONQUEST.c#L956-L959) | faithful |
| Else: count cities, `city_count = 0`, inner loop `itr_cities` while `< _cities && city_count < 2` | `loc_79974`-`loc_799AC` | [962-969](../../MoM/src/CONQUEST.c#L962-L969) | faithful |
| Alive-by-city: `city_count > 0 → di++` | `cmp City_Count,0 / jle / inc di` (52-54) | [970-973](../../MoM/src/CONQUEST.c#L970-L973) | faithful |
| Return: `di == 0 → ST_TRUE`, else `ST_FALSE` | `or di,di / jnz / mov ax,ST_TRUE … xor ax,ax` (60-71) | [976-980](../../MoM/src/CONQUEST.c#L976-L980) | faithful |

## Notes

- **Return framing.** The function returns `ST_TRUE` when *no* opponents remain — the test reads equally as "all computer players dead" or "the human has won" (opposite sides of the same condition). The `ST_TRUE`-on-zero-survivors semantics are preserved verbatim.

- **Slot 0 skipped (`itr_players` starts at 1).** The scan covers `1 … _num_players-1`; index 0 is never examined. The code's own doc comment ([941](../../MoM/src/CONQUEST.c#L941)) calls it the "presumed human slot." The caller `Resolve_Wizard_Conquest` ([440](../../MoM/src/CONQUEST.c#L440)) only reaches `CP_Is_Dead` on the `city_owner_idx != _human_player_idx` branch, consistent with slot 0 being the human. This is the OG behaviour — the start-at-1 is in the asm (`mov itr_players, 1`), not a reconstruction choice.

## Inner-loop cap `city_count < 2` — matches

The inner loop bails when `city_count` reaches 2 ([963](../../MoM/src/CONQUEST.c#L963)), matching the asm `cmp [bp+City_Count], 2 / jl short loc_79980`. Only `city_count > 0` is tested afterward, so the cap at 2 is the OG's own early-out. Faithful — part of the structure map above, not a deviation.

## The two `= 0` on `active_computer_players` — both correct

`active_computer_players` is set to 0 in two places, and both belong:

- [952](../../MoM/src/CONQUEST.c#L952) — the declaration initializer is the mandatory zero-init modernization applied uniformly to all four locals (`city_count`, `itr_cities`, `itr_players` carry the same `= 0`). It is a codebase convention, **not** an asm correspondence.
- [953](../../MoM/src/CONQUEST.c#L953) — the standalone statement is what corresponds to the asm `xor di, di`.

Neither is redundant-to-remove: dropping 952 would break the zero-init convention, dropping 953 would delete the faithful OG statement. Both stay. 1:1.

## Related references

- `…\ovr093\GAME_IsWon__STUB.asm` — IDA Pro 5.5 disassembly (**the authority**).
- `…\ovr093\GAME_IsWon__STUB.c` — Gemini translation (second opinion; agrees).
- [`Resolve_Wizard_Conquest`](../../MoM/src/CONQUEST.c#L191) — sole caller; invokes `CP_Is_Dead` at [440](../../MoM/src/CONQUEST.c#L440) on the non-human-conquest branch to gate `GAME_PlayVictoryAnim__STUB(_human_player_idx)`.
