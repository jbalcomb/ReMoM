# Dependency Vetting

**This document is the source of truth.** `tools/vet_dependency.py` implements the checkable parts;
the per-agent adapters (`.claude/skills/vet-dependency/SKILL.md`, the pointer in `AGENTS.md`, the
pointer in `.github/copilot-instructions.md`) all reference this file and must not duplicate it.

## Why this exists

An AI agent recommended installing a third-party FFmpeg build, was challenged on the dependency's
pedigree, and produced a **trust score of "90/100"** from five criteria invented on the spot. The
score was the failure — not the criteria. A number implies rigour that was not applied, and invites
the reader to stop thinking.

Two things came out of that exchange, and they are the backbone of this document:

1. **The criteria used would have green-lit xz-utils.** "Jia Tan" was an established co-maintainer
   with years of legitimate commit history, endorsed upstream, publishing hash-pinned releases
   through official channels. Every box ticked. CVE-2024-3094 ticks them all too.
2. **"Listed in MAINTAINERS" without a date is nearly content-free.** What matters is *tenure*, and
   tenure **relative to how long the artifact has existed** and to **when the person acquired
   publishing authority**. Absolute membership is not a signal; trajectory is.

## Rule 0 — no trust scores

Never produce a 0-100 rating, a letter grade, or "low/medium/high risk" for a dependency. Produce:

- the **facts** you actually verified, with their source,
- the **flags** that warrant attention,
- the **questions you could not answer**, recorded explicitly as gaps.

An unchecked item is a gap, never a pass. If a lookup fails, say so; do not quietly drop it.

## Tier by blast radius

Scrutiny should be proportional to what a compromise reaches. Decide the tier first — it determines
how much of the rest matters.

| Tier | Definition | Examples in this repo | Standard |
|---|---|---|---|
| **A** | Ships to users | `SDL2.dll`, `SDL2_mixer.dll`, `SDL3.dll` (committed under `assets/`, `bin/`, `IDE/`), FluidSynth pulled transitively by SDL_mixer | Pin hashes, record provenance, prefer building from source. A compromise reaches everyone who installs ReMoM. |
| **B** | Builds the product | MSVC, CMake, clang, googletest (`FetchContent`) | Pin versions, no floating fetches. A compromise can inject into shipped artifacts. |
| **C** | Dev-only, offline, scratch data | ffmpeg for demo-video encoding | Reduce blast radius instead of agonising over trust. |

**The most common mistake is misallocated attention** — scrutinising a Tier C tool while Tier A
binaries sit unexamined. Check the tier before checking anything else.

### Tier C is a blast-radius problem, not a trust problem
For a dev-only tool, do not try to establish trust; make trust unnecessary:
- no system-wide install, not on `PATH` — vendor it under `tools/` and invoke by absolute path,
- record its SHA256 in a manifest (same habit as the LBX/floppy byte manifests),
- run it against a scratch directory containing nothing of value, with no network,
- confirm it never ships and never touches the repo.

If it were malicious, it would be malicious at a folder of temporary files. That is a far stronger
position than any score.

## Checks

Run `python tools/vet_dependency.py --repo OWNER/NAME [--upstream OWNER/NAME --author "Name"]
[--manifest-url URL] [--tier A|B|C]`. It gathers the mechanical items below and prints the
unanswerable ones every run.

### Automated
| Check | Why |
|---|---|
| Upstream endorsement | Does the upstream project itself link this distributor? Weightiest single signal. |
| **Maintainer tenure** | Date of first upstream commit, and total commits — **exact-name matched**. |
| **Authority lag** | Time between first upstream contribution and creating the distribution channel. A long lag means standing was earned before authority was taken. A short lag is the xz shape. |
| Publisher channel age | How long the distribution repo has existed. |
| Activity | Abandoned projects do not receive security fixes. |
| Hash pinning | Does the package manifest pin a content hash, so a swapped binary is detected? |
| Artifact URL ownership | Does the manifest point at the maintainer's own infrastructure, consistent with the package ID? Detects namespace squatting. |

> **Exact-name matching matters.** GitHub's commit search silently degrades an unquoted
> `author-name:First Last` into `author-name:First` plus a loose term. Vetting this repo's own
> ffmpeg case, unquoted returned 378 commits and quoted returned 377 — a small difference that
> happened not to change the conclusion, and a method error that easily could have.

### Manual — automation cannot reach these
- **Build transparency / reproducibility.** *This is the check that xz would have failed*: the
  backdoor was in the release tarball but not the git tree. Prefer a distributor whose builds run in
  public CI with inspectable logs over one building on private infrastructure. For ffmpeg on Windows
  this favours **BtbN** (public GitHub Actions) over **Gyan** (own infrastructure), though both are
  linked from ffmpeg.org.
- **Account / key compromise.** Tenure defends against infiltration, never against a stolen key
  today. No amount of history closes this.
- **Sanctions and entity screening.** NIST SP 800-161 (C-SCRM), Section 889, OFAC SDN, BIS Entity
  List. A genuine and sometimes legally mandatory criterion for **organisations**, and the reason the
  Linux kernel removed maintainers in October 2024. For an **individual** open-source contributor it
  is usually not honestly applicable: nationality inferred from a name is both unreliable and
  prejudicial. State that it was not screened rather than fabricating a data point.
- **Runtime behaviour.** What the thing actually does when it runs.

## Recording the decision

Vetting is point-in-time. Record, next to the dependency:
- what was checked and when, the flags raised, and the gaps left open,
- the decision and who made it,
- the pinned version/hash — **re-vet when the version changes**, since a manifest read for v7.1 says
  nothing about what installs today.

## Who is accountable

An AI agent cannot hold responsibility for this decision: it carries no consequences, has no SLA,
and will not be present for the remediation. That asymmetry is precisely why it must not hand over a
confident-looking number. Agents supply verified facts and named gaps; a human decides and records.
