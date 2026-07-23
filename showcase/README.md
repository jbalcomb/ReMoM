# Showcase reel scenes

The marketing video.  **Not to be confused with `demo/`**, which is the `demo_vga` VFX test harness --
that code drives rendering functions directly and its output must never appear in published footage
(see the rule below).


The editable source of the ReMoM showcase video.  Shot list and editorial intent:
[doc/#AI_Plans/PLAN-Demo-Reel-Scenes.md](../doc/%23AI_Plans/PLAN-Demo-Reel-Scenes.md).
Capture/render machinery: [doc/#AI_Plans/PRD-Scripted-Demo-Capture.md](../doc/%23AI_Plans/PRD-Scripted-Demo-Capture.md).

Files are numbered so `--concat "out/demo/[0-9]*.mp4"` orders the cut correctly.

```
NN_name.RMR   the raw recording a script was derived from (kept, so it can be re-derived)
NN_name.hms   the scene script -- THIS is the artifact we maintain and edit
```

## Authoring one scene

```sh
GAME=out/build/MSVC-sdl2-debug/bin/Debug/ReMoM.exe    # SDL2 build -- the Win32 build is SILENT

# 1. record a rough pass by playing it
$GAME --record showcase/03_overland.RMR --seed1 12345

# 2. convert to editable text
out/build/MSVC-debug/bin/Debug/rmr2hms.exe showcase/03_overland.RMR showcase/03_overland.hms

# 3. HAND-EDIT for pacing  <-- this is the actual work, see below

# 4. render and watch
python tools/render_demo.py --scene 03_overland \
    --scenario showcase/03_overland.hms --game $GAME --seed 12345
# note: render_demo.py's flag is --seed; it passes --seed1 through to the game

# 5. back to 3 until it reads well
```

Final cut:
```sh
python tools/render_demo.py --concat "out/demo/[0-9]*.mp4" --output out/demo/reel.mp4
```

## Editing for pacing

A raw recording is a person hunting for buttons.  A demo wants confident moves and deliberate pauses.

- Coarsen the waits `rmr2hms` emits — it produces exact millisecond gaps (`wait 3187ms`); round them
  (`wait 3s`) so the intent is readable and editable.
- Delete fumbles, overshoots and double-clicks that were corrections rather than actions.
- **Hold ~3 s minimum on any screen with text.** The viewer has never seen it, and it is a 320x200
  screen scaled to 1080p.
- Add `#` comments marking each beat. Future-you is re-cutting this.

## Rules

- **End the scene by quitting the game.** A clean exit finalizes the capture properly; otherwise
  `render_demo.py --max-seconds` has to kill it and patch the WAV header and the truncated last frame.
- **Pin the seed** (`--seed1 N`) for anything touching RNG, and pass the same seed when rendering.
- **Keep scenes short.** Divergence risk grows with length; a short scene is cheap to re-record.

## Do NOT film the `demo_vga` harness

The `demo/` video test harness drives sequences like the summoning animation and the Spell of Mastery
victory directly, with no scripting — and it must **never** supply a frame of this reel.

It reaches those states by calling engine functions with setup fixups the game never performs
(clearing `magic_master_idx`, hand-allocating `GUI_String_1`, invoking `Spell_Of_Mastery()` on a save
where nobody cast it). The pixels match the game's; the claim they make to a viewer does not. This
reel is published evidence about the software, shown to people who cannot check it.

See [BRA-Demo-Reel.md](../doc/%23AI_Plans/BRA-Demo-Reel.md) §2 for the full rule and its
corollaries (no cheated saves, no silent speed-ups, bugs stay in).


## .hms syntax

Fuller than `rmr2hms` emits — see `assets/Stage_0.hms`, which documents the action set inline:

```
click X Y          left click at game coordinates (320x200 space)
rclick X Y         right click
type STRING        type text
key K              press a key
enter / escape / backspace / left
next_turn          press 'N' (Next Turn hotkey)
wait 500ms         wall-clock pause; also 1s / 1m
wait 30f           wait N frames (1 frame = 55ms) -- frame-accurate, prefer for anything
                   that must line up with an animation
quit               set the quit flag
end                stop the player (idle forever)
# comment          whole-line or trailing
```

Durations require an explicit unit suffix.  `rmr2hms` emits exact millisecond gaps
(`wait 3187ms`); round them by hand so the intent is readable.
