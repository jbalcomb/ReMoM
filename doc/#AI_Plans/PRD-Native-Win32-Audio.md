# PRD — Native Win32 Audio

**Status:** Draft
**Date:** 2026-07-21
**Related:** `BRA-Native-Win32-Audio.md` (decision record — read first), `doc/#Devel/Dependency-Vetting.md`

This document specifies *what* to build. The rationale for every architectural choice below — why WildMIDI, why `waveOut`, why native GTL instead of GENMIDI transcoding — is in the BRA and is not repeated here.

---

## 1. Problem statement

ReMoM's `USE_WIN32` backend exists to build and run on Windows with no SDL dependency. It is silent: `CMakeLists.txt:84-85` forces `NO_SOUND_LIBRARY`, and `platform/win32/win_PFL.cpp:487` is an explicit no-op. The default MSVC presets (`MSVC-debug`, `MSVC-release`) select this backend, so the primary Windows development build never exercises audio; only `MSVC-sdl2-debug` does.

## 2. Goals

**G1.** Music and sound effects play on the `USE_WIN32` backend, with no SDL dependency.
**G2.** Music is voiced by MoM's own Miles/AIL timbres, not a General MIDI substitute.
**G3.** The XMI and VOC converters exist in exactly one place, shared by all three backends.

## 3. Non-goals

- Low-latency audio. MoM is turn-based.
- Positional / surround audio.
- Changing SDL2 or SDL3 backend behaviour. They keep working as-is.
- MT-32 / Roland output (`FAT.MT`). Out of scope, but must not be foreclosed.
- Replacing FluidSynth on the SDL path.
- User-facing soundfont selection.

## 4. Constraints

| Constraint | Source |
|---|---|
| No SDL headers or libraries on the `USE_WIN32` path | Reason `USE_WIN32` exists |
| Public API is `Platform_Audio_Play_Sound(void *, uint32_t)` — one function | `platform/include/Platform.h:412` |
| Game-side call site is unchanged | `MoX/src/SOUND.c:738` `Play_Sound()` |
| Original game code is never deleted; changes marked `/* CLAUDE */` | `AGENTS.md` |
| WildMIDI modifications are LGPLv3 and must be published | `BRA` §7 |
| New `.c` files update both `CMakeLists.txt` and `Makefile.am` | `AGENTS.md` (standalone `tools/` are CMake-only) |

## 5. Background — verified input formats

All confirmed against real game data this cycle; see BRA §2 and §5.

**LBX sound entry:** 16-byte wrapper (`LEN_LBX_SND_HDR`), `uint16 0xDEAF` at +0, `uint16` subtype at +2 (1 = XMI, 2 = VOC). `sdl2_Audio.c:132-137`, `PFL_Audio_Internal.h:42`.

**Global Timbre Library** (`FAT.<data_suffix>`, filename built at `SOUND.c:678-682`): 6-byte index records `{char patch; char bank; unsigned long offset;}` terminated by `bank == 0xFF` (`SOUND.c:242-248`); payload at `offset` begins with `uint16` total length *including* those 2 bytes. Payload = mode byte + one or two 11-byte OPL voices, each `chr, lvl, ad, sr, ws` per operator plus feedback.

Measured contents of `assets/__FAT/FAT.OPL`: 181 timbres — bank 55 patches 0-127 (contiguous), bank 127 patches 35-87. `FAT.AD` has the same index shape with 12-byte single-voice payloads.

**Miles two-voice timbres are 4-operator** (`ext/ail214/YAMAHA.INC:342-355`), which is why GENMIDI cannot represent them.

---

## 6. Requirements

### Phase 1 — Shared converter extraction (no behaviour change)

- **R1.1** Move `Convert_VOC_To_WAV()`, `fmt_mus_convert_xmid()`, `xmid_convert_evnt()`, and the LBX sound-wrapper constants out of `platform/sdl2/` and `platform/sdl3/` into one shared module, named for what it does (not for a consumer).
- **R1.2** SDL2 and SDL3 backends consume the shared module. No duplicated copies remain.
- **R1.3** No audible or byte-level change to SDL2/SDL3 output. Verified by re-rendering a known track and comparing hashes before and after.

*Rationale: doing this first means the Win32 backend consumes shared code rather than creating a third copy.*

### Phase 2 — WildMIDI Tier 1: native GTL loader

Work happens in `jbalcomb/wildmidi` (`J:/STU/developp/wildmidi`).

- **R2.1** WildMIDI accepts a Miles GTL passed to `WildMidi_Init()` / `-c`, detected **by content**, matching how `.op2` and `.sf2` are already detected (`wildmidi_lib.c:1739-1750`).
- **R2.2** Melodic bank and percussion bank are selectable, defaulting to 55 and 127. Percussion is not restricted to keys 35-81 — the GENMIDI range limit must not be inherited.
- **R2.3** 2-operator timbres (`FAT.AD`, 12-byte payload) render at full fidelity using the existing synth path, with no `synth.c` changes.
- **R2.4** 4-operator timbres (`FAT.OPL`, 23-byte payload) load without error and degrade to their first two operators, clearly documented as a Tier 1 limitation.
- **R2.5** KSL/total-level is preserved bit-exactly. The Miles level byte *is* the OPL `0x40` register; do not repack lossily.
- **R2.6** Structured as a self-contained addition suitable for upstream submission to Mindwerks.

**Exit criteria:** `wildmidi -c assets/__FAT/FAT.AD out/xmi/MUSIC_100.XMI` renders, and the result is distinguishable from and preferred over the `gtl2op2`-produced GENMIDI equivalent.

### Phase 3 — Win32 audio backend (music only)

- **R3.1** New `platform/win32/win_Audio.c` implementing `Platform_Audio_Play_Sound` for subtype 1 (XMI).
- **R3.2** `waveOut` (winmm) output, using `src/player/out_win32mm.c` in the WildMIDI tree as reference.
- **R3.3** WildMIDI renders to PCM via `WildMidi_OpenBuffer` + `WildMidi_GetOutput`; a render thread feeds `waveOut` buffers.
- **R3.4** Timbres load from the `FAT.*` file the game already locates via `SOUND.c:678-682`. No new shipped asset, no build-time generation.
- **R3.5** `USE_WIN32` no longer unconditionally defines `NO_SOUND_LIBRARY`; `DISABLE_AUDIO` remains available to opt out.
- **R3.6** Missing or unreadable `FAT.*` degrades to silence with a logged error, never a crash. Errors carry context per `AGENTS.md`.

**Exit criteria:** launching ReMoMber under an MSVC preset plays the overland background track (`MUSIC.LBX` entry 100, `MUSIC_Bkgrnd_Neutral`, `SOUND.h:191`) with MoM's own timbres.

### Phase 4 — Win32 SFX and mixing

- **R4.1** `Platform_Audio_Play_Sound` handles subtype 2 (VOC) via the Phase 1 shared converter.
- **R4.2** Music and multiple concurrent SFX mix. Voice count and overflow policy to be set from characterising `MoX/src/SOUND.c` — see BRA §10 open question 5.
- **R4.3** Button-click SFX (`SOUNDFX.LBX` entries 0 and 2) play, matching the preload described in `doc/STU-Init-Sequence.md:89-112`.
- **R4.4** No buffer leak per effect. *Note: the SDL2 path leaks — `Mix_FreeChunk` is never called on the VOC path. Do not replicate.*

**Exit criteria:** clicking a button during background music produces both sounds, with no drift or dropout over a 10-minute session.

### Phase 5 — WildMIDI Tier 2: 4-operator support

- **R5.1** `synth.c` manages OPL3 connection-select register `0x104`. It currently writes only `0x105` (`synth.c:190`).
- **R5.2** A 4-operator voice-pair allocator, porting the approach in `ext/ail214/YAMAHA.INC:1403-1420` (`NUM_4OP_VOICES = 6`, pairing at channel offset +3).
- **R5.3** `FAT.OPL` timbres render as authored, using all four operators.
- **R5.4** 2-op timbres and existing GENMIDI/SF2/GUS paths are unaffected. Regression-checked by re-rendering known tracks.

**Exit criteria:** `FAT.OPL` output is preferred by ear over both `FAT.AD` and the Tier 1 two-operator degradation.

### Phase 6 — Dependency and licence record

- **R6.1** Record the WildMIDI Tier A decision per `doc/#Devel/Dependency-Vetting.md` — pinned commit, flags, unclosed gaps, who decided, when. BRA §7 holds the verified facts; the *decision* is the maintainer's to record.
- **R6.2** Resolve the absent root `LICENCE` file before shipping anything linking LGPLv3 code.
- **R6.3** Document that `jbalcomb/wildmidi` is a modified LGPLv3 library and where its source is published.
- **R6.4** Re-vet when the pinned commit moves.

---

## 7. Tooling already built

| Tool | Purpose | Status |
|---|---|---|
| `tools/lbx_extract.c` | Extract LBX entries to standalone `.XMI` / `.VOC` / `.BIN` | Built, verified against `MUSIC.LBX`, `SOUNDFX.LBX`, `SNDDRV.LBX` |
| `tools/gtl2op2.c` | Miles GTL → GENMIDI, for A/B comparison | Built; **development tool only, not a shipping path** (BRA §5) |

Both are standalone (no engine link) and CMake-only, matching `rmr2hms`.

---

## 8. Risks

| Risk | Impact | Mitigation |
|---|---|---|
| Tier 2 4-op work is larger than estimated | Fidelity goal G2 only partly met | Tier 1 ships real timbres independently; Tier 2 is additive |
| Pinned to unreleased WildMIDI `master` | Upstream churn breaks the fork | Fork controls the pin; re-vet on any move |
| Upstream declines the GTL loader | Permanent fork maintenance | Acceptable — the fork already exists for LGPL reasons |
| `waveOut` latency is worse than expected | Audible lag on SFX | Latency explicitly a non-goal; revisit only if it affects click feedback |
| Phase 1 refactor regresses SDL2/SDL3 | Working backends break | R1.3 hash comparison before and after |
| The GTL mode byte matters and is being ignored | Subtle wrongness | Resolve from `ext/ail214/GLIB.C` during Phase 2 |

## 9. Open questions

Carried from BRA §10 — items 1, 2, 3, and 5 are inputs to Phases 2, 4, and 5 and should be closed as those phases begin.

## 10. Sequencing note

Phases 1-4 deliver G1 and G3 and a large part of G2. Phase 5 completes G2. Phase 6 is not gated on the others and should be started immediately — it is currently three unrecorded items deep and is the only phase with an external obligation attached.
