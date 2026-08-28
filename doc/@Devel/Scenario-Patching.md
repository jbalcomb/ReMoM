# Scenario Patching — setting up game state with `TST_Patch_Game_Data`

How to put a loaded game into a specific state so a scripted scene (or a manual test) can start from
it, without hand-playing hours to get there.

Used by the demo reel ([BRA-Demo-Reel.md](../%23AI_Plans/BRA-Demo-Reel.md) §9) and available for any
testing that needs a hard-to-reach starting state.

## How it works

`TST_Patch_Game_Data()` (`STU/src/STU_TST.c`) is called from `Loaded_Game_Update()` at
[LoadScr.c:874](../../MoM/src/LoadScr.c#L874) — **immediately after every save load**, which is the
right seam: the save is fully in memory, and nothing has started running on it yet.

Select a scenario with ReMoM's `--patch` flag:

```sh
ReMoM.exe --load SAVE1.GAM --patch som_ready
```

## It ships in Release, and that is deliberate

The demo reel is filmed from the **Release** binary an end user downloads — so gating this behind
`STU_DEBUG` would mean the setup never runs in the build being filmed.

Safety comes from being **inert by default** instead. With no `--patch` on the command line,
`g_tst_patch_scenario` is `NULL`, the function returns immediately, and nothing is touched. This is
the same pattern as `--capture`, `--load`, `--record`, `--replay` and `--scenario`: all present in the
Release binary, all doing nothing unless explicitly invoked.

*Verified:* a run without `--patch` produces zero `[TST]` log lines; a run with
`--patch som_ready` produces the scenario's log lines. The default path changes no game state.

**Consequence to accept:** a curious player can pass `--patch` and cheat. That is a cheat facility,
not a correctness bug, and it is the price of filming the same binary players get. If that becomes
unacceptable, the fix is to remove scenarios from release builds — not to debug-gate the mechanism,
which would break the filming requirement.

## Writing a scenario

Add a `TST_Scenario_*` function and a branch in `TST_Patch_Game_Data()`:

```c
static void TST_Scenario_Som_Ready(int16_t player_idx)
{
    TST_Learn_Spell(player_idx, spl_Spell_Of_Mastery);
    TST_Cheat_Power(player_idx);
}
```

Existing helpers in `STU_TST.c`:

| Helper | Effect |
|---|---|
| `TST_Learn_Spell(player_idx, spell_idx)` | Marks a spell `sls_Known` in `spells_list` (index is `spell_idx - 1`) |
| `TST_Cheat_Power(player_idx)` | gold 32767, mana 32767, `spell_casting_skill` 32767, recomputes `Nominal_Skill` / `Skill_Left` |

Direct field writes are fine too — see the commented examples in `TST_Patch_Game_Data()` for city
enchantments and global enchantments.

## RULES — what a scenario may and may not do

These come from [BRA-Demo-Reel.md](../%23AI_Plans/BRA-Demo-Reel.md) §9 and are binding for anything
that will be filmed. They are good discipline for tests too.

1. **Set preconditions only.** A scenario arranges state. It must never drive the sequence being
   demonstrated.
2. **Reachable state only.** Values a real game could legitimately hold. Giving a wizard a spell and
   mana is fine; impossible states are not.
3. **Patch at load only, never mid-sequence.** This function runs once, before anything starts.
4. **Never call a presentation or resolution function.** No `Spell_Of_Mastery()`, no
   `Win_Animation()`, no `Cast_Spell_Of_Mastery()`. If a scenario is calling those, it has stopped
   staging and started faking — that is what the `demo_vga` harness does, and why its output cannot
   be published.

The test to apply: **does everything from the patch point to the final frame execute as the game's
own logic?** If yes, it is staging. If the scenario itself produced the outcome, it is fabrication.

## Scenario: `som_ready`

> The human wizard knows the Spell of Mastery and can afford to cast it.

```sh
ReMoM.exe --load SAVE1.GAM --patch som_ready
```

Sets `spells_list[spl_Spell_Of_Mastery - 1] = sls_Known` and maxes gold/mana/casting skill. Nothing
else. From there the real path runs:

```
spellbook: select Spell of Mastery
  -> SBookScr.c:558   Cast_Spell_Of_Mastery()      "X has begun casting..."
  -> casting accumulates over turns
  -> OverSpel.c:811   Spell_Of_Mastery()           endgame branch
  -> Win_Animation() -> End_Of_Game_Score()
```

**Verified working.** With `--load SAVE1.GAM --patch som_ready`:
```
[TST] applying scenario patch 'som_ready'
[TST] scenario 'som_ready': player 0 knows Spell of Mastery; mana=32767 skill=32767 skill_left=181
```

### Known limitation: 181 casting points per turn

`TST_Cheat_Power` maxes `spell_casting_skill` to 32767, but per-turn casting is limited by
`Skill_Left`, which is `Player_Base_Casting_Skill()` — and that function accumulates `2 + 4 + 6 + …`
until it exceeds `spell_casting_skill`, so even a maxed skill yields only **181 points per turn**.

Spell of Mastery's casting cost (from `spell_data_table`, loaded from LBX) is far larger, so
`som_ready` alone means **many turns of pressing Next Turn** before the endgame fires. That is real
gameplay, but it is poor demo material and slow to render.

**Options, undecided — needs a human call:**

- **(a) Let it accumulate.** Wholly honest, no further patching, but the scene becomes minutes of
  Next Turn. Could be cut or shown as an explicit timelapse (BRA §2.1 allows honest timelapse).
- **(b) Patch mid-cast state.** Set `casting_spell_idx = spl_Spell_Of_Mastery` and
  `casting_cost_remaining` to roughly one turn's worth — representing a wizard who has been casting
  for many turns and is about to finish. This is a **legitimately reachable state**, satisfies rule 2,
  and everything after (the final turn completing, the endgame firing) is still the game's own logic.
  **This is the recommended variant for the reel** — a ~30 s scene instead of minutes.
- **(c) Raise `Nominal_Skill` / `Skill_Left` directly** so one turn completes the cast. Simplest, but
  a casting skill in the thousands is not a state a real game reaches, so it fails rule 2. Not
  recommended.

Option (b) is not yet implemented; `som_ready` is (a) today.

## Planned scenarios

For the endgame recordings in [BRA-Demo-Reel.md](../%23AI_Plans/BRA-Demo-Reel.md) §10:

| Scenario | Purpose |
|---|---|
| `som_ready` | Human casts Spell of Mastery → win. **Implemented.** |
| `som_ready_ai` | An AI casts it → human loses (`Spell_Of_Mastery_Lose`) |
| `military_win` | Human one step from eliminating the last rival wizard |
| `military_lose` | Human about to be eliminated |
| `banished` | Human banished, holding Spell of Return — the recovery path |

Unknown whether `banished` can be reached by patching alone or needs a genuine banishment first.
