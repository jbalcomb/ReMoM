# Bridge: ReMoM id comment fixes, pending on `main`

For agents working on `remom-mod` (the AI Workstation) before the user merges `main` into it. Written 2026-11-03.

## The situation

ReMoM's id comments (`// WZD s161p04` above a function) were checked against the 1.31 IDA databases on the Coder
Workstation (Velociraptor). The tables `link_remom.py` wrote there found the ids below wrong.

- **The fixes:** the user made them in the `main` checkout on Velociraptor (`C:\STU\devel\ReMoM`), mixed with other
  uncommitted work. With them, every WIZARDS and MAGIC id names a real procedure: no `only in ReMoM` rows, no wrong
  letters.
- **How they reach `remom-mod`:** the user commits them on `main`, then merges `main` into `remom-mod`.
- **Until then:** `remom-mod` still has the old ids. **Do not make these edits on `remom-mod` yourself**: only the
  user's exact edits keep the merge clean. Read old ids through the table below.

## The corrections

| Old id (as on `remom-mod` now) | Correct id | Where | Why |
|---|---|---|---|
| `WZD s80pNN`, `s81pNN`, `s82pNN`, `s90pNN`, `s91pNN`, `s96pNN`, `s103pNN`, `s127pNN`, `s161pNN` | the same with `o` (`s161p04` → `o161p04`) | EVENTS, ItemView, Combat, HIRE, Terrain, MAPGEN, MainScr_Maps (`.c`/`.h`) | overlays (`ovrNNN` in IDA) written with `s` |
| `WZD s34p49`–`s34p56` | `s36p49`–`s36p56` | [Fields.c](../../MoX/src/Fields.c), [Fields.h](../../MoX/src/Fields.h) | segment 36 (`Add_Hot_Key` and the field and scroll-bar functions) |
| `WZD s34p22` | `s36p22` | [Keyboard.c](../../MoX/src/Keyboard.c) (`Interpret_Keyboard_Input`) | segment 36 |
| `WZD s82p0` (×7, truncated) | `o82p01`–`o82p07`, in order | [ItemView.c](../../MoM/src/ItemView.c); [ItemView.h](../../MoM/src/ItemView.h) lists `p02` twice and no `p03` | truncated |
| `WZD o115p11`–`o115p17` | `o115p10`–`o115p16` (each one less) | [ItemMake.c](../../MoM/src/ItemMake.c), [ItemMake.h](../../MoM/src/ItemMake.h) | there is no `p10`: `p11` is the 10th procedure of `ovr115`, and the database has 16. So `Make_Item` is `o115p16` and `Create_Random_Item` `o115p15` |
| `WZD s24p09` | (comment line removed) | [FLIC_Draw.c](../../MoX/src/FLIC_Draw.c) | stale comment (the function moved to capture.c) |
| `WZD o145p0` | (comment line removed) | [AIDATA.c](../../MoM/src/AIDATA.c) | stale comment |

ReMoM's convention, confirmed by the databases: `s` for `segNNN` (load module), `o` for `ovrNNN` (overlays). IDA
numbers every segment of an `.EXE` once, so the number alone identifies the segment. `stubNNN` is the overlay stub of
`ovrNNN`.

## The merge (the user, on Velociraptor)

Tested on copies of both branches: `main` merges into `remom-mod` cleanly except **one conflict, in
`MoM/src/Terrain.c`**. `remom-mod` put the rules dispatcher (`Square_Production_Bonus`, which calls
`MOD_Rules_Call_Int`) directly above the `// WZD s161p04` line, and the fix changes that line. Resolve by keeping
`remom-mod`'s dispatcher and the faithful `Square_Production_Bonus_Original` below it, with the id line reading
`// WZD o161p04`. Then build and run the rules tests (`Square_Production_Bonus_Rules`, `RulesCatalog`), since that is
the code around the conflict.

## Checking after the merge

With the exports copied to the AI Workstation (`~/STU_DASM/mom131_export/`, see
[Handoff-remom-mod.md](Handoff-remom-mod.md)):

```bash
cd ~/STU/devel/ReMoM-Mod && M=~/STU_DASM/mom131_export && \
python3 -m tools.mom_patch_diff.remom_index --remom . --out $M/remom_index.tsv && \
python3 -m tools.mom_patch_diff.link_remom --exe WZD --joined $M/wizards_joined.tsv --addresses $M/wizards_addresses.tsv --index $M/remom_index.tsv --out $M/wizards_linked.tsv && \
python3 -m tools.mom_patch_diff.link_remom --exe MGC --joined $M/magic_joined.tsv --addresses $M/magic_addresses.tsv --index $M/remom_index.tsv --out $M/magic_linked.tsv
```

Expected, as on Velociraptor with the fixes: WZD `linked 1830 ... id letter differs 0, only in ReMoM 0`; MGC
`linked 134 ... only in ReMoM 0`.
