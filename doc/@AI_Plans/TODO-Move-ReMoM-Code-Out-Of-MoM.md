# TODO: Move ReMoM-only code out of `MoM/`

`MoM/src/` is the reconstruction of the original game. Code that ReMoM added and that has no counterpart in the asm belongs in the ReMoM-side
sources, with only a call-site hook left in the reconstructed function. Each item below is ReMoM code that currently lives inside a reconstructed file.

Where it goes: both ReMoM and DeMoM run `Screen_Control()`, so the destination must be shared code that both targets link, named by what it does
(AGENTS.md, *Naming Conventions*), not a `ReMoM_` / `DeMoM_` file.

## 1. `Reset_Game_State` and its state, in MOM_SCR.c

- [ ] Move [Reset_Game_State](../../MoM/src/MOM_SCR.c#L83) out of `MoM/src/MOM_SCR.c`. It came in with commit `2dd7e27c` ("bug-fixes from amiga
      port", 2026-10-31) and has no asm. It resets the game state between games in one process, which the original never needed because it started
      a fresh WIZARDS.EXE / MAGIC.EXE each time. That includes its own `--seed1` / `Randomize()` reseed.
- It brings its file-scope state with it ([MOM_SCR.c:70-76](../../MoM/src/MOM_SCR.c#L70-L76)): `dirty_game_state`, `SAMB_HEADER_BYTES`,
  `units_samb_header` / `items_samb_header` / `lairs_samb_header`, `samb_headers_saved`.
- The uses inside `Screen_Control()` stay as hooks, and become calls into the moved code:
  - the reset-if-dirty and the first-time SAMB-header save, at [MOM_SCR.c:196-206](../../MoM/src/MOM_SCR.c#L196-L206);
  - `dirty_game_state = ST_TRUE`, at [MOM_SCR.c:429](../../MoM/src/MOM_SCR.c#L429).
- When it moves, update both build lists for the new file: its `CMakeLists.txt` and the root `Makefile.am`.
