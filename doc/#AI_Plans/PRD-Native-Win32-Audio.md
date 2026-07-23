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

**XMIDI `TIMB` chunk** — the sequence's declaration of which timbres it needs. Layout, decoded from `assets/MUSIC.LBX` entry 100 and cross-checked against `sdl2_Audio.c:1207-1219`:

| Offset (wrapper stripped) | Field |
|---|---|
| `0x2E` | `"TIMB"` tag |
| `0x32` | `uint32` **big-endian** payload length |
| `0x36` | `uint16` **little-endian** entry count |
| `0x38` | count × 2 bytes: `{ uint8 patch; uint8 bank; }` |

Payload length is `2 × count + 2` and was verified self-consistent. Note the mixed endianness (BE chunk length, LE count) and that `(patch, bank)` is the same field order as the GTL index record at `SOUND.c:242-248`.

Measured: entry 100 declares 4 timbres, **all bank 55** (patches 24, 0, 72, 49). A survey of all 116 `MUSIC.LBX` tracks found a TIMB chunk in **116 of 116**.

AIL consumes TIMB by iterating `AIL_timbre_request()` and installing each result via `load_global_timbre()` → `AIL_install_timbre()` (`SOUND.c:807-817`). This is the mechanism that binds a sequence to the GTL, and it is the half of the design that a bare GTL loader would still be missing.

---

## 6. Requirements

### Phase 1 — Shared converter extraction (no behaviour change)

- **R1.1** Move `Convert_VOC_To_WAV()`, `fmt_mus_convert_xmid()`, `xmid_convert_evnt()`, and the LBX sound-wrapper constants out of `platform/sdl2/` and `platform/sdl3/` into one shared module, named for what it does (not for a consumer).
- **R1.2** SDL2 and SDL3 backends consume the shared module. No duplicated copies remain.
- **R1.3** No audible or byte-level change to SDL2/SDL3 output. Verified by re-rendering a known track and comparing hashes before and after.

*Rationale: doing this first means the Win32 backend consumes shared code rather than creating a third copy.*

### Phase 2A — WildMIDI: generalize the patch storage model

Work happens in `jbalcomb/wildmidi` (`J:/STU/developp/wildmidi`). **This phase must come first.** See BRA §6.1: WildMIDI's only patch store is a flat GENMIDI image (`synth.c:86`), with the GENMIDI base and key range hardcoded into lookup (`synth.c:602`, `:653`, `:806`) and a strictly 2-operator patch struct (`synth.c:56-67`). A GTL loader written before this phase would write its results into `op2_bank[]` and re-inherit every GENMIDI limitation — it would be `gtl2op2` with the intermediate file deleted.

- **R2A.1** Patch storage is **bank-addressed**, not a flat program-indexed array. MoM uses banks 55 and 127; the model must not assume a single implicit bank.
- **R2A.2** Percussion is addressed by MIDI key with **no hardcoded range**. The GENMIDI 35-81 clamp at `synth.c:602` and `:806` must not survive. Keys 82-87 are played by 21 MoM tracks (PRD §9.1).
- **R2A.3** The patch representation holds **two or four operators**. `fm_patch` currently has `mod_*`/`car_*`/`fb` only; operators 3-4 need somewhere to live even though Phase 5 supplies the synthesis.
- **R2A.4** Existing input formats — GENMIDI/`.op2`, SF2, GUS patches, and the built-in `@opl3` bank (`synth.c:793-795`) — keep working, via a compatibility shim onto the new model if needed.
- **R2A.5** No audible change to any existing format's output. Verified by re-rendering known tracks and comparing hashes before and after.

**Exit criteria:** `wildmidi -O`, a `.op2` bank, and an `.sf2` all render byte-identically to their pre-change output, while the model is capable of expressing a bank-55 patch, a percussion key of 87, and a 4-operator voice.

### Phase 2B — WildMIDI: native GTL loader and TIMB resolution

Depends on Phase 2A.

- **R2.1** WildMIDI accepts a Miles GTL passed to `WildMidi_Init()` / `-c`, detected **by content**, matching how `.op2` and `.sf2` are already detected (`wildmidi_lib.c:1739-1750`).
- **R2.2** Melodic bank and percussion bank are selectable, defaulting to 55 and 127, stored in the Phase 2A bank-addressed model. All 181 `FAT.OPL` timbres survive, including percussion keys 82-87.
- **R2.3** 2-operator timbres (`FAT.AD`, 12-byte payload) render at full fidelity.
- **R2.4** 4-operator timbres (`FAT.OPL`, 23-byte payload) load and **retain all four operators in storage**, rendering with the first two until Phase 5 supplies 4-op synthesis. Storage must not discard operators 3-4; that is the whole point of R2A.3.
- **R2.5** KSL/total-level is preserved bit-exactly. The Miles level byte *is* the OPL `0x40` register; do not repack lossily.
- **R2.6** **Parse the `TIMB` chunk instead of skipping it.** `f_xmidi.c:193-213` currently reads the length and jumps the payload, with the FIXME "May not be needed for playback as EVNT seems to hold patch events". That assumption is false for MoM: EVNT carries a program number but not the *bank*, and every MoM track relies on bank 55.
- **R2.7** **Resolve each TIMB `(patch, bank)` pair through the loaded GTL and install the timbre**, mirroring AIL's `AIL_timbre_request` → `load_global_timbre` → `AIL_install_timbre` loop (`SOUND.c:807-817`). Timbre counts are small — 4 for the overland track — so eager loading of a sequence's declared set is acceptable; on-demand is not required.
- **R2.8** **Define fallback behaviour** for a program change whose patch is absent from TIMB, and for a sequence with no TIMB chunk at all. Fallback must be explicit and logged, never a silent wrong-instrument substitution.
- **R2.9** Structured as a self-contained addition suitable for upstream submission to Mindwerks.

**Exit criteria:** `wildmidi -c assets/__FAT/FAT.AD out/xmi/MUSIC_100.XMI` renders using the four bank-55 timbres declared in that track's TIMB chunk, and the result is distinguishable from and preferred over the `gtl2op2`-produced GENMIDI equivalent.

**Explicitly out of scope for Phase 2B:** the `RBRN` chunk (`f_xmidi.c:215`, "Unknown what this is") is left skipped. No evidence it affects MoM playback; revisit only if a defect points at it.

### Phase 2C — WildMIDI: XMIDI event-stream fidelity

The playback path (`f_xmidi.c`, `_WM_ParseNewXmi`) diverges from AIL's interpreter in two ways that a survey of all 116 `MUSIC.LBX` tracks proved are exercised in the majority of them. These are independent of the timbre-storage work and can proceed in parallel with 2A, though R2C.2 shares the bank model with 2A/2B.

- **R2C.1** **Honour tempo meta-events (`FF 51 03`) instead of discarding them.** `f_xmidi.c:319-322` matches the event and comments `/* Ignore tempo events */`, skipping it. AIL implements tempo fully: `XMIDI_meta` (`ext/ail214/XMIDI.ASM:929`) dispatches `51h` to `__set_tempo` (`:1014`), driving `tempo_percent`/`tempo_target`/`tempo_accum`/`tempo_period` state (`:249-255`). **Measured: 90 of 116 tracks (78%) carry at least one tempo event.** This is the single highest-impact XMIDI divergence — every affected track plays at a wrong, flat tempo regardless of instrument fidelity.
- **R2C.2** **Interpret controller 114 (`PATCH_BANK_SEL`) as an XMIDI bank select**, routing it to the Phase 2A bank-addressed model rather than passing it through as a generic MIDI CC. AIL defines it as a core XMIDI controller (`ext/ail214/AIL.INC:65`) and logs it per channel (`ctrl_log`, `ext/ail214/XMIDI.ASM:201`). **Measured: 594 events across 87 of 116 tracks.** This is the play-time companion to the TIMB declaration (R2.6-R2.7): TIMB declares the timbre set, CC114 switches to bank 55 during playback. Both are required for correct bank binding.
- **R2C.3** Controllers 110/111/112 (`CHAN_LOCK`/`CHAN_PROTECT`/`VOICE_PROTECT`) and finite/nested FOR loops are **out of scope** — measured usage across the 116 tracks is zero for 110/111/112 and two events total for CC116, none for CC117. Documented as deliberately unhandled; revisit only if a track is found to need them.

**Exit criteria:** a track with tempo changes (any of the 90) renders at varying tempo matching AIL's timing, and a CC114 bank switch selects the intended bank in the Phase 2A model rather than being ignored.

**Method note:** the usage counts come from a validated EVNT-stream parser (correct XMIDI note-on VLQ-duration handling; 0 desyncs across all 116 tracks), not the earlier raw byte-scan whose controller counts were false positives. See §9.1.

### Phase 3 — Win32 audio backend (music only)

- **R3.1** New `platform/win32/win_Audio.c` implementing `Platform_Audio_Play_Sound` for subtype 1 (XMI).
- **R3.2** `waveOut` (winmm) output, using `src/player/out_win32mm.c` in the WildMIDI tree as reference.
- **R3.3** WildMIDI renders to PCM via `WildMidi_OpenBuffer` + `WildMidi_GetOutput`; a render thread feeds `waveOut` buffers.
- **R3.4** Timbres load from the `FAT.*` file the game already locates via `SOUND.c:678-682`. No new shipped asset, no build-time generation.
- **R3.5** `USE_WIN32` no longer unconditionally defines `NO_SOUND_LIBRARY`; `DISABLE_AUDIO` remains available to opt out.
- **R3.6** Missing or unreadable `FAT.*` degrades to silence with a logged error, never a crash. Errors carry context per `AGENTS.md`.

**Exit criteria:** launching ReMoM under an MSVC preset plays the overland background track (`MUSIC.LBX` entry 100, `MUSIC_Bkgrnd_Neutral`, `SOUND.h:191`) with MoM's own timbres.

### Phase 4 — Win32 SFX and mixing

- **R4.1** `Platform_Audio_Play_Sound` handles subtype 2 (VOC) via the Phase 1 shared converter.
- **R4.2** Music and multiple concurrent SFX mix. Voice count and overflow policy to be set from characterising `MoX/src/SOUND.c` — see BRA §10 open question 5.
- **R4.3** Button-click SFX (`SOUNDFX.LBX` entries 0 and 2) play, matching the preload described in `doc/STU-Init-Sequence.md:89-112`.
- **R4.4** No buffer leak per effect. *Note: the SDL2 path leaks — `Mix_FreeChunk` is never called on the VOC path. Do not replicate.*

**Exit criteria:** clicking a button during background music produces both sounds, with no drift or dropout over a 10-minute session.

### Phase 5 — WildMIDI: 4-operator synthesis

Depends on Phase 2A (storage for operators 3-4) and Phase 2B (timbres to put there).

- **R5.1** `synth.c` manages OPL3 connection-select register `0x104`. It currently writes only `0x105` (`synth.c:190`).
- **R5.2** A 4-operator voice-pair allocator, porting the approach in `ext/ail214/YAMAHA.INC:1403-1420` (`NUM_4OP_VOICES = 6`, pairing at channel offset +3).
- **R5.3** `FAT.OPL` timbres render as authored, using all four operators.
- **R5.4** 2-op timbres and existing GENMIDI/SF2/GUS paths are unaffected. Regression-checked by re-rendering known tracks.

**Exit criteria:** `FAT.OPL` output is preferred by ear over both `FAT.AD` and the Phase 2B two-operator rendering.

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
| Phase 2A touches a core WildMIDI data structure | Regresses GENMIDI/SF2/GUS paths for every WildMIDI user, not just ReMoM | R2A.4 compatibility shim; R2A.5 byte-identical re-render of all three existing formats |
| Phase 5 4-op work is larger than estimated | Fidelity goal G2 only partly met | Phase 2B ships MoM's real timbres (2-op rendering) independently; Phase 5 is additive |
| Pinned to unreleased WildMIDI `master` | Upstream churn breaks the fork | Fork controls the pin; re-vet on any move |
| Upstream declines the GTL loader | Permanent fork maintenance | Acceptable — the fork already exists for LGPL reasons |
| `waveOut` latency is worse than expected | Audible lag on SFX | Latency explicitly a non-goal; revisit only if it affects click feedback |
| Phase 1 refactor regresses SDL2/SDL3 | Working backends break | R1.3 hash comparison before and after |
| The GTL mode byte matters and is being ignored | Subtle wrongness | Resolve from `ext/ail214/GLIB.C` during Phase 2B |

## 9. Open questions

Carried from BRA §10 — items 1, 2, 3, and 5 are inputs to Phases 2, 4, and 5 and should be closed as those phases begin.

### 9.1 XMIDI event handling — surveyed against all 116 tracks

The question "does WildMIDI handle XMIDI the way AIL intended?" was answered by a validated EVNT-stream parser run over all 116 `MUSIC.LBX` tracks (correct XMIDI note-on VLQ-duration handling; 0 desyncs), cross-referenced against AIL's own interpreter.

| Feature | Measured usage | WildMIDI | Disposition |
|---|---|---|---|
| Tempo meta `FF 51 03` | **90 / 116 tracks** | discarded (`f_xmidi.c:319-322`) | **R2C.1** |
| Controller 114 `PATCH_BANK_SEL` | **594 events, 87 / 116 tracks** | passed through as generic CC | **R2C.2** |
| `TIMB` chunk | **116 / 116 tracks** | skipped (`f_xmidi.c:193-213`) | R2.6-R2.8 |
| Controller 116 `FOR_LOOP` | 2 events, 2 tracks | partial (`f_xmidi.c:333-335`) | R2C.3 (out of scope) |
| Controller 117 `NEXT_LOOP` | 0 | — | R2C.3 (out of scope) |
| Controllers 110/111/112 | 0 | passed through | R2C.3 (out of scope) |

**Conclusion: there is a real XMIDI work item, and it is bigger than the timbre binding alone.** The two highest-impact divergences — tempo (90 tracks) and CC114 bank-select (87 tracks) — are event-stream fidelity issues in `f_xmidi.c`, not timbre-storage issues. Tempo especially: 78% of the soundtrack plays at a wrong flat tempo regardless of instruments. These are Phase 2C.

**Correction of an earlier claim.** A first pass concluded "no general fix-XMIDI work item," based on a raw byte-scan for `0xBn` followed by 116/117 that reported 78 and 74 occurrences. Those counts were false positives — XMIDI's variable-length delta bytes were mistaken for status bytes. The validated parser puts the real counts at 2 and 0, and more importantly the first pass never scanned for tempo or CC114 at all, which are the divergences that actually matter. The lesson is the recurring one in BRA §9: an indicative-but-unvalidated measurement drove a wrong conclusion; the validated one reversed it.

## 10. Sequencing note

**Phase 2A is the gate.** It is the only phase that must precede another for correctness rather than convenience: a GTL loader written before the patch model is generalised produces `gtl2op2` with the intermediate file removed — same 175-record ceiling, same percussion clamp, same two operators. See BRA §6.1.

Phase 1 is independent of all WildMIDI work and can proceed in parallel. Phases 3-4 depend on 2B for anything to voice correctly, though a Win32 backend could be brought up against the built-in `@opl3` bank first if that de-risks the `waveOut` work.

Phases 1, 2A, 2B, 3 and 4 deliver G1 and G3 and most of G2. Phase 5 completes G2. Phase 6 is not gated on anything and should be started immediately — it is three unrecorded items deep and is the only phase carrying an external obligation.
