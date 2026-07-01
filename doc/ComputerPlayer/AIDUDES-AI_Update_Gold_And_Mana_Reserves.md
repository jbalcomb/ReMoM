AIDUDES-AI_Update_Gold_And_Mana_Reserves.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr145\AI_Update_Gold_And_Mana_Reserves.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr145\AI_Update_Gold_And_Mana_Reserves.c

AI_Next_Turn()
    |-> AI_Update_Gold_And_Mana_Reserves()

---

# `AI_Update_Gold_And_Mana_Reserves` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Update_Gold_And_Mana_Reserves` | [AIDUDES.c:1709-1812](../../MoM/src/AIDUDES.c#L1709-L1812) | Per-AI-player: rebalance `gold_reserve` and `mana_reserve` between three dispatched cases — (a) casting Spell of Return: convert gold to mana, zero gold; (b) `alchemy != 1`: one-in-ten Random roll to redistribute toward gold or mana based on the current ratio; (c) `alchemy == 1`: turn-driven fixed split (9:1 gold-heavy before turn 150, 7:3 mana-heavy after). All values clamped to `MAX_GOLD_RESERVE` / `MAX_MANA_RESERVE` (= 30000). |

Verified faithful to the disassembly `AI_Update_Gold_And_Mana_Reserves.asm` throughout (structure 1:1, the production code carries the OG asm labels as `/* loc_DXXXX */` comments to make the mapping visible at the source level).

## Purpose

The AI's wallet rebalancer. The human player has a slider for gold-vs-mana split (handled elsewhere); the AI doesn't, so this function imposes one based on the AI's situation:

- **Casting Spell of Return** — the AI is fleeing back to its home plane. Dump gold-as-mana to fuel the recall (Alchemy retort: 1:1 conversion; otherwise: 1:2). Has a documented OG bug that overwrites existing mana instead of adding.
- **No Alchemy retort + 1-in-10 roll** — three rebalance paths gated on the current gold:mana ratio. The function tries to keep the ratio in a corridor around gold/5 and mana/4.
- **Alchemy retort = 1** — the wizard's Alchemy ability lets gold and mana convert freely. Apply a fixed split: 90% gold / 10% mana before turn 150 (early-game build), then 70% mana / 30% gold after (mid-late-game spell-pressure phase).

The 30000 ceiling (`MAX_GOLD_RESERVE = MAX_MANA_RESERVE = 30000`) caps each reserve. Anything that would exceed it is clamped at the SETMAX point.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Next_Turn` per-AI loop | Called per AI player per turn within the AI_Next_Turn dispatch. | No self-throttle — fires every turn for every AI player. |

## Globals / external state

| Symbol | Definition | Effect |
|---|---|---|
| `_players[player_idx]` (`s_WIZARD`) | per-player wizard record | Read: `casting_spell_idx`, `alchemy`, `gold_reserve`, `mana_reserve`. Mutated: `gold_reserve`, `mana_reserve`. |
| `_turn` | global turn counter | Read at two sites: `_turn >= 150` (alchemy=1 dispatch), `_turn < 100` (path-3 gate). |
| `Random(n)` | RNG | One `Random(10)` call per non-alchemy non-SoR invocation. Critical for PRNG parity. |

## Signature and locals

```c
void AI_Update_Gold_And_Mana_Reserves(int16_t player_idx)
```

OG stack locals (asm:4-6): `Desired_Mana`, `Desired_Gold`, `Resource_Total` — declared as **`dword ptr`** (32-bit). Production declares them as `int32_t` at [1711-1713](../../MoM/src/AIDUDES.c#L1711-L1713), matching the OG width so the `Resource_Total = gold + mana` and `Resource_Total * 9 / 10` chains don't overflow.

CamelCase preserved from the OG names; pending the lower_snake_case sweep (`Desired_Mana` → `desired_mana`, `Desired_Gold` → `desired_gold`, `Resource_Total` → `resource_total`).

## Structure

```mermaid
flowchart TD
    Entry(["AI_Update_Gold_And_Mana_Reserves(player_idx)"])
    SoR{"casting == spl_Spell_Of_Return?"}
    SoRPath["alchemy==1: mana = gold<br/>else: mana = gold/2<br/>gold = 0<br/>(OGBUG: overwrites prior mana)"]
    AlchemyGate{"alchemy != 1?"}
    NonAlchemy["mana==0 path:<br/>&nbsp;&nbsp;mana = gold/4<br/>&nbsp;&nbsp;gold = gold/2"]
    Roll{"Random(10) == 1?"}
    P1Cond{"mana &lt; gold/5?<br/>(loc_D437D)"}
    P1["loc_D43A2: if gold &gt; 100<br/>&nbsp;&nbsp;Desired_Mana += gold/4<br/>&nbsp;&nbsp;Desired_Gold -= gold/2"]
    P2Cond{"gold &lt; mana/4?<br/>(loc_D4462)"}
    P2["loc_D4487: if mana &gt; 100<br/>&nbsp;&nbsp;Desired_Gold += mana/4<br/>&nbsp;&nbsp;Desired_Mana -= mana/2"]
    P3["loc_D4547: if turn &lt; 100 AND mana &gt; 50<br/>&nbsp;&nbsp;Desired_Gold += mana/4<br/>&nbsp;&nbsp;Desired_Mana -= mana/2"]
    AlchemyPath{"_turn &gt;= 150?"}
    Mid["Desired_Mana = (g+m)*7/10<br/>Desired_Gold = (g+m) - Desired_Mana"]
    Early["Desired_Gold = (g+m)*9/10<br/>Desired_Mana = (g+m) - Desired_Gold"]
    Store["gold = Desired_Gold<br/>mana = Desired_Mana"]
    Done(["return"])

    Entry --> SoR
    SoR -->|yes| SoRPath --> Done
    SoR -->|no| AlchemyGate
    AlchemyGate -->|yes (alchemy!=1)| NonAlchemy --> Roll
    Roll -->|no| Done
    Roll -->|yes| P1Cond
    P1Cond -->|yes| P1 --> Done
    P1Cond -->|no| P2Cond
    P2Cond -->|yes| P2 --> Done
    P2Cond -->|no| P3 --> Done
    AlchemyGate -->|no (alchemy==1)| AlchemyPath
    AlchemyPath -->|yes| Mid --> Store
    AlchemyPath -->|no| Early --> Store --> Done
```

## Code walk

Line refs are production [AIDUDES.c](../../MoM/src/AIDUDES.c); cross-checked against `AI_Update_Gold_And_Mana_Reserves.asm` (the authority). Production embeds the OG asm labels as `/* loc_DXXXX */` comments to make the mapping verifiable in-source.

### Phase 1 — Spell of Return shortcut ([1714-1725](../../MoM/src/AIDUDES.c#L1714-L1725))

```c
if(_players[player_idx].casting_spell_idx == spl_Spell_Of_Return)
{
    if(_players[player_idx].alchemy == 1)
    {
        _players[player_idx].mana_reserve = _players[player_idx].gold_reserve;  /* OGBUG  should be +=, not = */
    }
    else
    {
        _players[player_idx].mana_reserve = (_players[player_idx].gold_reserve / 2);  /* OGBUG  should be +=, not = */
    }
    _players[player_idx].gold_reserve = 0;
}
```

Maps onto asm:14-55. SoR check (lines 14-19) ↔ production line 1714. Alchemy branch (lines 24-31) ↔ production lines 1716-1723. The `else` for alchemy != 1 uses the OG's signed-divide idiom `cwd; sub ax, dx; sar ax, 1` (asm:39-41) — for non-negative `gold_reserve` this is just `gold / 2`. The OG stores via `mov [_players.mana_reserve+bx], ax` (asm:49) then zeros gold (asm:54).

Both inline `/* OGBUG  should be +=, not = */` comments at lines 1718 and 1722 are the documented OG-faithful behavior — the assignment overwrites any prior mana_reserve value. Preserved.

### Phase 2 — Dispatch: alchemy != 1 (no retort) ([1727-1782](../../MoM/src/AIDUDES.c#L1727-L1782))

```c
else  // not Spell of Return
{
    if(_players[player_idx].alchemy != 1)
    {
        // mana==0 initialization
        if(_players[player_idx].mana_reserve == 0)
        {
            _players[player_idx].mana_reserve = (_players[player_idx].gold_reserve / 4);
            _players[player_idx].gold_reserve = (_players[player_idx].gold_reserve / 2);
        }

        if(Random(10) == 1)
        {
            /* loc_D437D */
            if(_players[player_idx].mana_reserve < (_players[player_idx].gold_reserve / 5)) { /* path 1 */ }
            /* loc_D4462 */
            else if(_players[player_idx].gold_reserve < (_players[player_idx].mana_reserve / 4)) { /* path 2 */ }
            /* loc_D4547 */
            else { /* path 3 */ }
        }
    }
    ...
```

Top-level dispatch maps onto asm `loc_D41B0`-`loc_D4318` (lines 57-64). The OG `cmp alchemy, 1; jz loc_D41C3; jmp loc_D4318` is the Borland compiler's natural emission for `if (alchemy != 1) { no-retort body } else { retort body }` when both bodies are hoisted to remote addresses — the `jz` jumps to the else when the `!=` test fails. Production's form at line 1728 is the faithful back-translation.

#### Sub-phase 2a — mana_reserve == 0 initialization ([1730-1734](../../MoM/src/AIDUDES.c#L1730-L1734))

```c
if(_players[player_idx].mana_reserve == 0)
{
    _players[player_idx].mana_reserve = (_players[player_idx].gold_reserve / 4);
    _players[player_idx].gold_reserve = (_players[player_idx].gold_reserve / 2);
}
```

Maps onto asm `loc_D4318`-`loc_D436B`. When the AI's mana_reserve is still 0 (typically early or after Spell of Return), seed it from gold:

- `mana_reserve = gold_reserve / 4` (asm:212-226: `mov ax, gold_reserve; mov bx, 4; cwd; idiv bx; mov mana_reserve, ax`)
- `gold_reserve = gold_reserve / 2` (asm:227-241: signed-divide-by-2 idiom)

Both reads of `gold_reserve` are pre-store values — the first assignment stores to `mana_reserve`, so the second read still sees the original `gold_reserve`. Faithful.

#### Sub-phase 2b — Random(10) gate + three rebalance paths ([1735-1781](../../MoM/src/AIDUDES.c#L1735-L1781))

```asm
loc_D436B:
mov ax, 10
push ax
call Random
pop cx
cmp ax, 1
jz short loc_D437D
jmp loc_D460E    ; not 1: exit
```

If the roll lands, the OG enters `loc_D437D` and dispatches across three rebalance paths via two nested compares. Production mirrors this with an `if/else if/else` chain whose conditions match the OG branch outcomes one-for-one:

##### Path 1 — `mana < gold/5` (loc_D43A2) ([1737-1750](../../MoM/src/AIDUDES.c#L1737-L1750))

```c
/* loc_D437D */
if(_players[player_idx].mana_reserve < (_players[player_idx].gold_reserve / 5))
{
    /* loc_D43A2 */
    if(_players[player_idx].gold_reserve > 100)
    {
        Desired_Gold = (int32_t)_players[player_idx].gold_reserve;
        Desired_Mana = (int32_t)_players[player_idx].mana_reserve;
        Desired_Mana += (Desired_Gold / 4);
        Desired_Gold = (Desired_Gold - (Desired_Gold / 2));
        SETMAX(Desired_Mana, MAX_MANA_RESERVE);
        SETMAX(Desired_Gold, MAX_GOLD_RESERVE);
        _players[player_idx].gold_reserve = Desired_Gold;
        _players[player_idx].mana_reserve = Desired_Mana;
    }
}
```

OG `loc_D437D` (asm:251-267): `cwd; idiv 5 → ax = gold/5; cmp mana, ax; jl loc_D43A2; jmp loc_D4462`. Production's `if (mana < gold/5)` is the direct back-translation.

OG `loc_D43A2` (asm:269-276): `cmp gold, 100; jg loc_D43B5; jmp loc_D445F`. Production's `if (gold > 100)` matches.

OG `loc_D43B5` body (asm:278-342): set up 32-bit Desired_Gold/Desired_Mana, `LDIV(Desired_Gold, 4)`, `add [Desired_Mana], ax`, `LDIV(Desired_Gold, 2)`, `sub [Desired_Gold], ax`, clamps to 30000, store. Production lines 1743-1750 produce identical semantics: `Desired_Mana += gold/4`, `Desired_Gold -= gold/2` (written as `Desired_Gold = Desired_Gold - Desired_Gold/2` — compiles to the same `sub`).

##### Path 2 — `gold < mana/4` (loc_D4487) ([1751-1764](../../MoM/src/AIDUDES.c#L1751-L1764))

```c
/* loc_D4462 */
else if(_players[player_idx].gold_reserve < (_players[player_idx].mana_reserve / 4))
{
    /* loc_D4487 */
    if(_players[player_idx].mana_reserve > 100)
    {
        Desired_Gold = (int32_t)_players[player_idx].gold_reserve;
        Desired_Mana = (int32_t)_players[player_idx].mana_reserve;
        Desired_Gold += (Desired_Mana / 4);
        Desired_Mana = (Desired_Mana - (Desired_Mana / 2));
        SETMAX(Desired_Mana, MAX_MANA_RESERVE);
        SETMAX(Desired_Gold, MAX_GOLD_RESERVE);
        _players[player_idx].gold_reserve = Desired_Gold;
        _players[player_idx].mana_reserve = Desired_Mana;
    }
}
```

OG `loc_D4462` (asm:346-362): `mov ax, mana; idiv 4 → ax = mana/4; cmp gold, ax; jl loc_D4487; jmp loc_D4547`. Production's `else if (gold < mana/4)` matches the `jl` direction.

OG `loc_D4487` (asm:364-372): `cmp mana, 100; jg loc_D449A; jmp loc_D4544`. Production's `if (mana > 100)` matches.

OG `loc_D449A` body (asm:374-441): mirror of `loc_D43B5` but rebalancing toward gold. `Desired_Gold += mana/4`, `Desired_Mana -= mana/2`. Production lines 1755-1762 match.

##### Path 3 — `gold >= mana/4` (loc_D4547) ([1765-1781](../../MoM/src/AIDUDES.c#L1765-L1781))

```c
/* loc_D4547 */
else
{
    if(_turn < 100)
    {
        if(_players[player_idx].mana_reserve > 50)
        {
            Desired_Gold = (int32_t)_players[player_idx].gold_reserve;
            Desired_Mana = (int32_t)_players[player_idx].mana_reserve;
            Desired_Gold += Desired_Mana / 4;
            Desired_Mana -= Desired_Mana / 2;
            SETMAX(Desired_Mana, MAX_MANA_RESERVE);
            SETMAX(Desired_Gold, MAX_GOLD_RESERVE);
            _players[player_idx].gold_reserve = (int)Desired_Gold;
            _players[player_idx].mana_reserve = (int)Desired_Mana;
        }
    }
}
```

The `else` is the fall-through `jmp loc_D4547` from `loc_D4462`'s false branch — i.e., `mana >= gold/5 AND gold >= mana/4`.

OG `loc_D4547` (asm:443-446): `cmp _turn, 100; jl loc_D4551; jmp loc_D460E`. Production's `if (_turn < 100)` matches.

OG `loc_D4551` (asm:448-455): `cmp mana, 50; jg loc_D4564; jmp loc_D460E`. Production's `if (mana > 50)` matches.

OG `loc_D4564` body (asm:457-521): same shape as `loc_D449A` — `Desired_Gold += mana/4`, `Desired_Mana -= mana/2`. Production lines 1771-1778 match. (Path 3's `-=` form is the most direct mapping to the OG `sub`/`sbb`; paths 1 and 2 use the verbose `X = X - X/2` form, which compiles to the same `sub` instruction.)

### Phase 3 — Dispatch: alchemy == 1 (with Alchemy retort) ([1784-1810](../../MoM/src/AIDUDES.c#L1784-L1810))

```c
else  /* "the retort path" */
{
    if(_turn >= 150)
    {
        Resource_Total = (_players[player_idx].gold_reserve + _players[player_idx].mana_reserve);
        Desired_Mana = (Resource_Total * 7 / 10);    /* mana = total * 0.7 */
        SETMAX(Desired_Mana, MAX_MANA_RESERVE);
        Desired_Gold = (Resource_Total - Desired_Mana);
        SETMAX(Desired_Gold, MAX_GOLD_RESERVE);
    }
    else
    {
        Resource_Total = (_players[player_idx].gold_reserve + _players[player_idx].mana_reserve);
        Desired_Gold = (Resource_Total * 9 / 10);    /* gold = total * 0.9 */
        SETMAX(Desired_Gold, MAX_GOLD_RESERVE);
        Desired_Mana = (Resource_Total - Desired_Gold);
        SETMAX(Desired_Mana, MAX_MANA_RESERVE);
    }
    _players[player_idx].gold_reserve = Desired_Gold;
    _players[player_idx].mana_reserve = Desired_Mana;
}
```

Maps onto asm `loc_D41C3`-`loc_D42F5`. Turn-150 split: the OG `cmp [_turn], 150; jl loc_D41CE; jmp loc_D4263` (asm:67-69) is the Borland compiler's emission for `if (_turn >= 150) { *7/10 } else { *9/10 }` when both bodies are hoisted — `jl` jumps to the else when the `>=` test fails. Production's form at line 1787 is the faithful back-translation.

Both paths use **32-bit math throughout** in the OG: `Resource_Total = gold + mana` is computed as a 32-bit `add`/`adc` pair (asm:88-91), and `Resource_Total * 9 / 10` (and `* 7 / 10`) is computed via Borland's long-arithmetic library calls `LXMUL@` and `LDIV@` (asm:100-103, 161-164). Production's `int32_t` locals preserve this width, so `gold + mana = 60000` (both clamped at 30000) and the multiplications stay within range.

Both paths converge at `loc_D42F5` for the final stores (asm:191-203). Each clamps both Desired_* to 30000 before storing. Production's clamps via `SETMAX` are equivalent.

## OG quirks preserved (faithful — do not "fix")

- **Spell of Return overwrites `mana_reserve` instead of adding** ([1718, 1722](../../MoM/src/AIDUDES.c#L1718)) — both `mana_reserve = gold_reserve` and `mana_reserve = gold_reserve / 2` blow away any prior mana value. OG asm at line 49 stores via `mov [_players.mana_reserve+bx], ax` (overwrite). Inline `/* OGBUG  should be +=, not = */` comments name it. Preserved.
- **`mana_reserve == 0` init scales mana to gold/4 and shrinks gold to gold/2** ([1730-1734](../../MoM/src/AIDUDES.c#L1730-L1734)) — a one-shot seeding that fires only once per game per wizard (when mana_reserve crosses zero). OG-faithful (asm:216 + asm:231 — both read original `gold_reserve` because the first store goes to `mana_reserve`).
- **Random(10) rolls only once per invocation** — the gate is one roll at the top of the rebalance block. Most invocations skip the rebalance entirely. Faithful (asm:243-249).
- **Three rebalance paths use identical body math but different gates** — paths 2 and 3 both rebalance toward gold (`Desired_Gold += mana/4`, `Desired_Mana -= mana/2`); they differ only in which outer condition leads to them and which inner gate they impose. Faithful to OG (`loc_D449A` and `loc_D4564` are structurally parallel asm blocks).

## Sub-functions / external calls

- **`Random(n)`** — one call (`Random(10)`) gates the rebalance for the alchemy != 1 path. No RNG in the SoR or alchemy=1 paths.
- **`SETMAX(var, max)`** — clamp macro: `if (var > max) var = max;`. Used at every `Desired_*` assignment in Phases 2-3.

No `CONTXXX_Map`. No I/O. No PHASE wrapper at the call site (typically).

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr145\AI_Update_Gold_And_Mana_Reserves.asm` — IDA Pro 5.5 disassembly (the authority).
- [AIDUDES-AI_Update_Magic_Power.md](AIDUDES-AI_Update_Magic_Power.md) — sibling Wave 3F function, balances skill/research/mana ratios. This function balances gold/mana reserves; the two are complementary.
- `s_WIZARD` fields read/written: `gold_reserve`, `mana_reserve`, `casting_spell_idx`, `alchemy`.
- `spl_Spell_Of_Return` — the recall-to-fortress spell that triggers the Phase 1 shortcut.
- `MAX_GOLD_RESERVE`, `MAX_MANA_RESERVE` — both `30000`, the per-reserve clamp ceiling.
- `LXMUL@`, `LDIV@` — Borland C 3.0 long-arithmetic library calls used by the OG for 32-bit multiply / divide. Production's `int32_t` locals + `(Resource_Total * 9 / 10)` produce equivalent 32-bit math on modern MSVC.
