# Handoff: `remom-mod` → a Claude Code session on the Ubuntu workstation

Written at the end of a cloud session (2026-11-02) that had no access to the IDA databases, the game installs or the
patch folder. Read this first, then the documents it links. Branch: **`remom-mod`** in `github.com/jbalcomb/remom`.

## Starting the session

```bash
cd ~/devel/ReMoM                         # wherever the repo is cloned
git fetch origin remom-mod && git checkout remom-mod
claude
```

On Ubuntu this session can also be offered to cloud sessions through Remote Control (`claude remote-control` in the
repo folder). The cloud session that wrote this note could not attach, because the other computer that offers
`ReMoM` (Velociraptor) runs Windows.

Paths below use `~/MoM` for game data and outputs; substitute the real locations. [AGENTS.md](../../AGENTS.md)'s
shell rules were written for the Windows boxes (Git Bash, `/c/STU/...`). On this machine, use plain Linux paths and
the `clang-debug` preset (`cmake --preset clang-debug && cmake --build --preset clang-debug`); MSVC is Windows only.
Scripts are `python3`.

## Decisions already made (don't relitigate)

- **Branch model:** `remom-mod` holds ReMoM's own support code for mods. Other people's mods are never committed.
  - The fidelity rule (100% faithful to the disassembly) governs the faithful reconstruction, not this branch. Here,
    original code is the 1.31 reference and may be restructured to make rules moddable.
  - The user maintains AGENTS.md; do not edit it.
- **Rules in Lua:** each game version (1.31 `original`, Community Patch, CoM2, other mods) is a *rules profile*.
  - A rule that differs between versions moves into Lua as a faithful translation of the 1.31 C.
  - A test proves the translation equals the C for every input.
  - The other profiles override only what they change.
- **City economy fields are `int32_t`,** with ReMoM's own save format. 1.31 saves import one way, and every value
  1.31 would have wrapped is logged as OGBUG.
- **Tables are TSV** (more readable than CSV). The calculation catalog is RDF triples (Turtle).
- **Heavy binary or data analysis runs as offline scripts** that write compact reports, not as an LLM reading binaries.
- **Function tags** use the format in [Plan-Function-And-File-Tags.md](Plan-Function-And-File-Tags.md):
  `// @MoM WZD s161p04 ida:seg161:0005 seg:1A00:0005 file:0x01A2B5 len:0x0084`, plus `@MoM-file`, `@MoM-seg` and
  `@ReMoM`.
- **Dependencies:** any new one goes through [doc/@Devel/Dependency-Vetting.md](../@Devel/Dependency-Vetting.md)
  (for example `capstone` for `--disasm`).

## What is on the branch

| Area | Where | State |
|---|---|---|
| Lua hooks (sol2 3.5.0, Lua 5.4.8 in `ext/`), `--mod DIR` | `MOD/src/MOD_Hook.*`, `MOD_Lua.cpp`, `MOD_Load.c`, [Modding.md](../@Devel/Modding.md) | done, tested |
| Stand-in LBX data with range-checked tables | `tools/dummy_lbx/` | done |
| 32-bit city economy fields, ReMoM save format, 1.31 import | `MoX/src/MOM_DAT.h`, `MoX/src/SAVEFILE.*`, `MoX/src/LOADSAVE.c` | done, tested |
| CAS interpreter (Caster of Magic scripts) + `cas_check` | `MOD/src/CAS.c`, `tools/cas_check.c` | all 44 Warlord/CoM2 scripts parse; keyword table matches Caster.exe |
| CAS game built-ins with CoM2 ID tables, economy hook inputs, script slots in saves | `MOD/src/CAS_Game.c`, `MoM/src/City_Hook_Inputs.c` | 46 of the 111 built-ins Warlord uses; its 5 economy scripts run in a HeMoM turn |
| Rules profiles (`--rules NAME`) | `MOD/src/MOD_Rules.h`, `assets/rules/` | first rule ported: `Square_Production_Bonus` (all 393,216 inputs equal the C) |
| Rules catalog as RDF triples + table | `assets/rules/catalog.lua`, `tools/rules_catalog/`, [doc/rules-catalog/](../rules-catalog/) | generated; CTest keeps it current |
| Warlord / CoM2 gap analysis | [Warlord-CoM2-Gap-Analysis.md](Warlord-CoM2-Gap-Analysis.md) | includes Caster.exe and Casapi.dll findings |
| Install-to-install diff, IDA map export, address table | `tools/mom_patch_diff/` ([README](../../tools/mom_patch_diff/README.md)) | tested on synthetic data only; **never run against real IDA** |
| Community Patch FILESET patch index → per-version to-do lists | `tools/mom_patches/` ([README](../../tools/mom_patches/README.md)) | tested on synthetic data and 40 of the 1,131 patch files |
| Function and file tag plan | [Plan-Function-And-File-Tags.md](Plan-Function-And-File-Tags.md) | plan only |

## Data (the user's Drive and disks)

| What | Drive folder id | Notes |
|---|---|---|
| Warlord 1.5.12.6.2 for CoM2 1.05.11 (Suppanut4625) | `1wY_HjFxxSfDaMXbDfcyz2Gp-6wx4otfG` | permission given; never commit |
| Caster of Magic for Windows 1.05.11 (Seravy) | `1hJCYt78ESRb3NpIhUGoWYuLJ5izhYWm8` | `Data/`, manual, `CasApi/` headers |
| `Caster.exe` parsed (strings, tables, 2 KB hex chunks) | `14ZBAD0HggZn5Jmywvd8sIt8L5yHiTVVf` | carries TD32 debug symbols: every procedure name |
| `Casapi.dll` parsed | `1eE6mF7b4Vef7NGCkNIrJp7x-pvbnTGuk` | 912 exports; same gameplay units as the exe |
| FILESET patch files: `140` (1.40n), `151`, `152`, `Optional` | `1a3fgiWBulCqz4tN7thk4X6p-hZdgKXfq` | 1,131 text files; also shipped with the Community Patch |

drake178's differential patcher (`MOMDP_32.EXE`; `MOMDIFFP.EXE` for DOSBox) builds any of 1.00–1.60 from a 1.31
install. On Ubuntu, run the Windows one under Wine.

## Next steps, in order

1. **Export the 1.31 function maps.** IDA 5.5 under Wine:
   ```bash
   IDA=~/.wine/drive_c/IDA55/idag.exe     # wherever IDA 5.5 is installed under Wine
   python3 tools/mom_patch_diff/export_map.py --wine --ida "$IDA" --idb ~/MoM/ida/WIZARDS.idb --out ~/MoM/wizards_map.tsv
   python3 tools/mom_patch_diff/export_map.py --wine --ida "$IDA" --idb ~/MoM/ida/MAGIC.idb   --out ~/MoM/magic_map.tsv
   ```
   - `export_map.idc` has never run in a real IDA. If it stops, `--keep` keeps `ida.log`, which names the IDC line.
   - Alternative: run IDA natively on the Coder Workstation (Windows 10) as a REST service under NSSM, per
     [Plan-IDA-Service.md](Plan-IDA-Service.md) (plan only).
   - Check: `python3 -m tools.mom_patch_diff.address_table --remom . --out ~/MoM/address_table --map WIZARDS.EXE=... --exe 1.31=~/MoM/MoM131`.
     Nearly every `seg*`/`ovr*` function should show a 1.31 offset.
2. **Index every patch file** (whole folder, with base and maps):
   ```bash
   python3 -m tools.mom_patches.patch_index ~/MoM/patches --out ~/MoM/patch_index --remom . \
       --base WIZARDS.EXE=~/MoM/MoM131/WIZARDS.EXE --base MAGIC.EXE=~/MoM/MoM131/MAGIC.EXE \
       --map WIZARDS.EXE=~/MoM/wizards_map.tsv --map MAGIC.EXE=~/MoM/magic_map.tsv
   ```
   - Copy `todo_140.md`, `todo_151.md`, `todo_152.md` and `todo_Optional.md` into the repo, under
     `doc/@AI_Plans/patches/`.
   - `old_matches_base` shows which patches expect 1.31 and which expect an earlier patch level. Re-run with that
     version's `.EXE` as `--base` for those.
3. **Build each version and diff it:** `MOMDP_32.EXE` (under Wine) in copies of the 1.31 install, then the
   `mom_patch_diff` loop in its README. This gives function-level and data-table changes per release, and confirms
   the patch index.
4. **Function tags:** write `tools/mom_tags/` per the plan (report, apply with dry-run, `Tags_Valid` CTest, docs).
   - Resolve the 9 doubtful ids first. `gd_dump_units` picks up 5 ids; `Add_Hot_Key` and `Draw_Fields` each have two
     `WZD` ids.
5. **Port patches as rules profiles:** `unofficial_140n` → `151` → `152` → `community_patch_160`, each starting
   from the previous one. Follow *Rules profiles → To port another rule* in [Modding.md](../@Devel/Modding.md), and
   add each calculation to `assets/rules/catalog.lua` so the catalog gains a column per version.
6. **The other reconstruction** (one C file per segment, in a folder per executable, carrying only the segment's IDA
   start address): `scan_project.py` per the tag plan → `--project` index.
7. **Warlord:** continue the gap analysis order (profile mechanism done → dynamic counts → CoM2 INI loaders → asset
   overrides → remaining 34 hooks). `Casapi.dll` can serve as a test oracle on Windows or Wine, once vetted.

## Known loose ends

- The old remote branch `modding-phase1-lua-hooks` still exists; the cloud proxy refused the delete. Delete it on
  GitHub.
- The Community Patch rules profile is still identical to 1.31 for the one ported rule, because no change for it is
  confirmed. Candidate: 1.31 gives the base desert tile (`Desert1`) 0% production.
- CoM2 terrain details that are guesses: halving on shared squares, Gaia's Blessing on Nature Nodes, volcano owner
  production. Check them against `Casapi.dll`.
- `DIFFICULTY()` maps 1.31's five levels onto CoM2's nine as 1, 2, 3, 5, 7. That is a judgement call.
- The CAS script slots (`CASCITYD`, `CASWIZD `) are not cleared when a new game starts in the same session.
- After a played turn, `research_spells` differs between two identical HeMoM runs. This also happens on `main`, so it
  is not caused by this branch, but it blocks whole-save comparisons.
- HeMoM integration tests fail on the stand-in data in the same 7 places as on `main`; real data should be tried.
