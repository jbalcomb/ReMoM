# BRA — Native Win32 Audio Architecture Decision

**Status:** Decided
**Date:** 2026-07-21
**Author role:** Business Requirements Analyst (decision record)
**Related:** `PRD-Native-Win32-Audio.md`

This document records *how* the native Win32 audio approach was decided — the goal, the criteria weighed, the options considered and rejected, and specifically the misstep of transcoding MoM's timbres into a foreign bank format and how listening exposed that as a poor fit. It is a decision record, not an implementation plan.

---

## 1. The ultimate goal

ReMoM's `USE_WIN32` backend exists so the game can build and run on Windows with **no SDL dependency**. Every other subsystem already satisfies that. Audio does not: the Win32 backend is silent by construction.

The goal is therefore twofold, and the two halves have different standards:

- **Audio exists at all on the native backend.** Music and sound effects play, driven by the same `Play_Sound()` call sites the DOS original used. This is a functional bar — it either makes sound or it does not.
- **Audio sounds like Master of Magic.** The music is voiced by the timbres George Alistair Sanger authored for the game, not by a generic General MIDI substitute. This is a *fidelity* bar, and it is continuous, not binary.

The second goal is the one that shaped every decision below. Treating it as a nice-to-have produced the wrong architecture twice.

---

## 2. Current-state finding (the fact that reshaped the effort)

Audio is **not** unimplemented in ReMoM. It is implemented twice and switched off for Win32.

| Fact | Evidence |
|---|---|
| SDL2 and SDL3 both have complete audio backends | `platform/sdl2/sdl2_Audio.c` (1243 lines), `platform/sdl3/sdl3_Audio.c` |
| `USE_WIN32` forcibly disables all of it | `CMakeLists.txt:84-85` — `USE_WIN32` → `add_compile_definitions(NO_SOUND_LIBRARY)` |
| The Win32 backend is an explicit no-op | `platform/win32/win_PFL.cpp:487` |
| The default MSVC presets set `USE_WIN32=TRUE` — `MSVC-debug`, `MSVC-release`, `MSVC-nosound-debug`. Only `MSVC-sdl2-debug` opts out. | `CMakePresets.json:31,48,69` |
| The entire public audio API is one function | `platform/include/Platform.h:412` — `Platform_Audio_Play_Sound(void *, uint32_t)` |

The one-function API is a significant finding in its own right: the surface the Win32 backend must satisfy is *tiny*. All complexity lives below it.

Already solved and reusable, in the SDL backends:

- **XMI → MIDI conversion** — `sdl2_Audio.c:1144` `fmt_mus_convert_xmid()`, ported from 1oom, including the AIL FOR/NEXT loop controllers `0x74`/`0x75`.
- **VOC → WAV conversion** — `sdl2_Audio.c:400` `Convert_VOC_To_WAV()`.
- **LBX sound wrapper parsing** — 16-byte header, `0xDEAF` magic, subtype 1=XMI / 2=VOC (`sdl2_Audio.c:132-137`, `PFL_Audio_Internal.h:42`).

Both converters are **duplicated verbatim between `platform/sdl2/` and `platform/sdl3/`**. A third copy for Win32 would make three.

---

## 3. Criteria considered

| Criterion | Why it matters |
|---|---|
| **No SDL dependency** | The entire reason `USE_WIN32` exists. Non-negotiable. |
| **Timbre fidelity** | Does the music use MoM's own instrument definitions, or a generic substitute? |
| **Mixing capability** | MoM plays background music and multiple SFX concurrently. Something must mix. |
| **Amount of new synthesis code** | Writing an OPL3 emulator or a MIDI sequencer from scratch is out of scope. |
| **Dependency blast radius** | Anything linked here is Tier A — it ships to users. |
| **Licence compatibility** | ReMoM has no root `LICENCE` file; copyleft obligations must be understood before adoption. |
| **Duplication pressure** | Does the choice create a third copy of the XMI/VOC converters? |

### Criteria explicitly excluded

- **Low latency.** MoM is turn-based. Sub-10 ms audio latency has no gameplay value here.
- **Surround / 3D positional audio.** The original is stereo at best; there is nothing to position.
- **Runtime-swappable soundfonts as a user feature.** May emerge later; not a driver of this decision.
- **Matching FluidSynth's output.** FluidSynth is the *SDL* path's accident of implementation, not a fidelity target. Sounding like FluidSynth is not a goal.

---

## 4. The synthesis question (the crux)

MoM's music is Miles/AIL XMI. Playing it requires a MIDI synthesiser, and ReMoM cannot use SDL_mixer's (FluidSynth) on this path. Three options were considered.

### Option A — Ship a General MIDI SoundFont

Bundle a `.sf2`, render with any SF2-capable synth. Simple, immediate, well-trodden.

**Rejected on fidelity.** A GM soundfont voices MoM's music with orchestral samples it was never written for. The game's music was authored for FM synthesis; the FM character *is* the sound. This satisfies goal 1 and fails goal 2 outright.

### Option B — WildMIDI + GENMIDI bank transcoded from MoM's timbres

Use WildMIDI's built-in OPL3 emulator, and transcode MoM's `FAT.OPL` Global Timbre Library into the DMX GENMIDI (`.op2`) format WildMIDI accepts.

**Attempted, implemented, and rejected — see §5.** This is the misstep this document exists to record.

### Option C — WildMIDI reading the Miles GTL natively (chosen)

Teach WildMIDI to read MoM's Global Timbre Library directly, honouring banks 55/127 and the full timbre definition, with no intermediate format.

**Chosen.** It is the only option where the fidelity ceiling is set by MoM's data rather than by a third format's limitations.

---

## 5. The misstep — GENMIDI as an intermediate format

Option B was built. `tools/gtl2op2.c` converts a Miles GTL to a GENMIDI bank; it reported a clean-looking result:

```
melodic    : 128 converted, 0 missing   (bank 55, GM programs 0-127)
percussion : 47 converted, 0 missing   (bank 127, keys 35-81)
DROPPED    : 6 percussion timbre(s) above key 81
```

128 of 128 melodic programs. It rendered, it was audibly MoM, and it was clearly better than generic GM.

**It was still wrong, and listening caught it before analysis did.** Three variants were rendered — both voices with GENMIDI's `DOUBLE` flag, voice 1 only (`--single`), and the separate `FAT.AD` OPL2 bank. The maintainer judged `--single` the most correct.

That result is diagnostic, not a preference. It says the second voice was being *mis-applied*, and dropping it sounded better than layering it wrongly. The disassembly confirms why:

| Evidence | Source |
|---|---|
| `OPL3BNK` = the 11-byte 2-op `BNK` struct **plus 11 more bytes** commented `;op_2` and `;op_3` | `ext/ail214/YAMAHA.INC:342-355` |
| AIL allocates from `NUM_4OP_VOICES equ 6` four-operator voice pairs | `ext/ail214/YAMAHA.INC:47`, `:1403-1420` |
| Pairing checks `V_channel[bx]` and `V_channel[bx+3]` — the OPL3 4-op channel offset | `ext/ail214/YAMAHA.INC:1403-1420` |
| AIL writes register `104h` (4-op connection select) | `ext/ail214/YAMAHA.INC:884` |

**A Miles two-voice OPL timbre is a 4-operator voice, not two layered 2-operator voices.** GENMIDI's `DOUBLE` flag means two detuned 2-op voices played together — a different thing. GENMIDI has no way to express a 4-op voice at all.

Every loss in the conversion traces to the intermediate format, not to MoM:

1. 4-op voices misrepresented as layered 2-op pairs (the audible defect)
2. 6 percussion timbres dropped — GENMIDI holds keys 35-81; `FAT.OPL` has 35-87
3. The GTL mode byte discarded — no GENMIDI field for it
4. Banks 55 and 127 collapsed into GENMIDI's flat 175-record table

**Lesson, stated generally:** *a format conversion whose losses are all attributable to the target format is a signal that the target format is wrong.* The instinct to reach for the format the tool already accepts is exactly how fidelity gets silently capped. It was audible here; on a less musical subsystem it would not have been.

`gtl2op2` is **retained as a development A/B tool**, not as a shipping path.

---

## 6. Decision

### 6.1 Synthesis — WildMIDI, forked, with native GTL support

WildMIDI provides the pieces that would otherwise have to be written: a MIDI sequencer, a Miles XMI parser (`src/f_xmidi.c`, `src/xmi2mid.c`), and the Nuked OPL3 emulator core. Verified working against MoM data before adoption: `MUSIC.LBX` entry 100 parsed and rendered to 97 seconds of audio.

The work is split into two tiers because they carry very different risk:

- **Tier 1 — GTL loader, no synth changes.** Read a Miles GTL natively; honour banks 55/127. `FAT.AD` (12-byte payload = the plain 2-operator `BNK` struct) is **fully expressible by WildMIDI's synth as it stands**. `FAT.OPL` degrades gracefully to its first two operators. Self-contained and plausibly upstreamable.
- **Tier 2 — 4-operator support in `synth.c`.** WildMIDI writes `0x105` (OPL3 NEW mode) at `src/synth.c:190` but **never writes `0x104`**, and has no 4-op channel-pair allocator. The Nuked core underneath does support 4-op (`ch_4op2` in `src/opl3.c`). `ext/ail214/YAMAHA.INC` is the reference implementation.

Tier 1 delivers MoM's real timbres with no synthesis risk. Tier 2 is where the fidelity goal is actually met.

**Rejected alternative — patch the game instead of the library.** Pre-converting timbres in ReMoM and feeding WildMIDI a supported format is Option B by another name, with the same ceiling.

### 6.2 Output — waveOut (winmm)

`waveOut` was chosen over XAudio2 and WASAPI.

The analysis initially favoured XAudio2 on the grounds that it supplies a mixer and thereby deletes a work item. **The maintainer chose `waveOut`, and the excluded-criteria list is why that is right:** low latency was explicitly excluded, and MoM's mixing needs are modest and known. XAudio2's advantage was priced against a requirement this project does not have.

Decisive practical factor: **`src/player/out_win32mm.c` in the WildMIDI tree is working `waveOut` reference code** (5.4 KB), already in the dependency being adopted. Neither XAudio2 nor WASAPI comes with reference code in-tree.

### 6.3 Shared converter code — de-duplicate, do not triplicate

The XMI and VOC converters exist twice already. The Win32 backend must **not** add a third copy. They move to a shared location per `AGENTS.md` ("name shared modules by what they do, not by which target first uses them").

---

## 7. Dependency record — WildMIDI (Tier A)

Per `doc/#Devel/Dependency-Vetting.md`. **No trust score is produced, by design.**

**Classification:** Tier A — ships to users.

**Verified facts (2026-07-21):**

| Item | Finding |
|---|---|
| Upstream | `Mindwerks/wildmidi`, created 2014-01-09 (12.5 yr), 236 stars, active (pushed within days) |
| Licence | Library **LGPLv3+**, player GPLv3 — read from the header comment in `include/wildmidi_lib.h`, not from a badge |
| Latest release | **0.4.6, 2024-04-11** |
| Version adopted | **Unreleased 0.5.0 from `master`**, commit `417c36aeac15ed6f815f9f702b0fc9cf3ce1aec8` (2026-07-14) |
| Fork | `jbalcomb/wildmidi`, on `master` at `417c36ae`, no local modifications as of this record |
| Automated flags | None triggered |

**Flags:**

1. **Pinned to an unreleased commit.** The OPL3 renderer and SF2 support are 0.5.0-only. This is a git commit, not a release — the vetting policy warns against exactly this for Tier A. The fork under `jbalcomb/wildmidi` mitigates it: the pin is now to infrastructure the project controls.
2. **LGPLv3 + modification.** Tiers 1 and 2 both modify the library, so the modified library source must be published. The fork satisfies this structurally. Dynamic linking (`BUILD_SHARED_LIBS=ON`) keeps the obligation to the simple case; static linking would add relinking obligations.
3. **ReMoM has no root `LICENCE` file.** Not a WildMIDI problem, but it must be resolved before shipping anything that links copyleft code.

**Gaps — explicitly unclosed, not passes:**

- Build reproducibility of any prebuilt binary. *This is the check xz-utils would have failed.* Mitigated in practice by building from source in-tree, not eliminated.
- Account / signing-key compromise. Tenure defends against infiltration, never against a stolen key today.
- Sanctions / entity screening. Not applicable to individual contributors in an honest reading; recorded as not screened rather than fabricated.
- Runtime behaviour of the library beyond what has been exercised.

**Accountability:** an AI agent cannot hold this decision. Facts and gaps above; the maintainer decides and records.

---

## 8. What did and did not fit the goal

**Fit:**

- WildMIDI as the synthesis engine — supplies sequencer, XMI parser, and OPL3 core; verified against real MoM data before adoption.
- Forking rather than vendoring patches — satisfies LGPL, controls the pin, keeps upstreaming open.
- `waveOut` — matched to requirements actually held rather than requirements assumed.
- The Tier 1 / Tier 2 split — lets real timbres land before the risky synth work.

**Did not fit:**

- GM SoundFont — fails the fidelity goal outright.
- GENMIDI transcoding — capped fidelity at the intermediate format's limits; audibly wrong.
- XAudio2 — solved a problem (mixing complexity, latency) larger than the one this project has.
- A third copy of the XMI/VOC converters — rejected on maintenance grounds.

---

## 9. Lesson carried from BRA-Combat-Testing

`BRA-Combat-Testing.md` records forcing tactical combat onto a headless substrate it was never designed for. **The same shape recurred here:** MoM's timbres were forced into GENMIDI, a format they were never designed for, because it was the format the tool already accepted.

Both mistakes share a signature — *the constraint came from the tooling, not from the goal, and was not noticed until something concrete was observed* (a battle that would not run; music that sounded wrong). The general rule this reinforces: **when the substrate is chosen before the goal is stated, verify the fit explicitly rather than assuming it.**

---

## 10. Open questions

1. **Does the GTL mode byte (`payload[0]`) carry meaning ReMoM needs?** Not yet determined. `ext/ail214/GLIB.C` is the tool that *builds* GTL files and is the authoritative source.
2. **Is `FAT.OPL` or `FAT.AD` the right default?** `FAT.AD` is the OPL2 bank an actual SoundBlaster would have played in 1994. `FAT.OPL` is richer but needs Tier 2. Resolvable only by listening once Tier 2 exists.
3. **Roland/MT-32 path.** `FAT.MT` (65026 bytes) and the timbre-init sequence at `SNDDRV.LBX` entry 20 (`SOUND.c:713-716`) are out of scope here, but the GTL loader should not foreclose them.
4. **Will Mindwerks accept a Miles GTL loader upstream?** Worth attempting for Tier 1; it would remove the fork-maintenance burden. Tier 2 (4-op) is a larger ask.
5. **SFX mixing policy.** How many concurrent VOC effects must mix, and what happens on overflow. The DOS original's behaviour is in `MoX/src/SOUND.c` and has not been characterised.
