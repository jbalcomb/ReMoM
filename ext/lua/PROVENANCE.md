# Lua 5.4.8 — vendored source

| | |
|---|---|
| Upstream | https://github.com/lua/lua (the Lua team's development repository) |
| Version | tag `v5.4.8`, commit `6e22fedb74cf0c9b6656e9fce8b7331db847c605` (2025-05-21, Roberto Ierusalimschy, "New release number, updated copyright year") |
| License | MIT, copyright Lua.org, PUC-Rio (see `LICENSE.txt`, copied from the end of `lua.h`) |
| Tier | **A — ships to users** (linked into ReMoM and HeMoM) |
| Files | every `*.c` / `*.h` at the tag's root, except `lua.c` (standalone interpreter), `onelua.c` (amalgamation) and `ltests.c` / `ltests.h` (Lua's internal test harness). No file was modified. |
| Hashes | `SHA256SUMS.txt` — re-run `sha256sum -c SHA256SUMS.txt` in this folder to detect drift |
| Vetted | 2026-10-27, by an AI agent; **the decision to ship it is the maintainer's, not the agent's** |

## Facts checked (from the git history of the upstream repo)

- The repository's history starts 1993-07-28 ("The Lua team").
- Top committers: Roberto Ierusalimschy (5,720 commits), Roberto I (97), Waldemar Celes (90), Luiz Henrique de Figueiredo (28).
  The release tag was committed by the long-standing principal author.
- Active: latest commit on the default branch is 2026-10-16. A later patch release, `v5.4.9` (2026-08-07), exists.

## Flags

- The upstream README says the GitHub repository is "mirrored irregularly" and points to lua.org for official releases.
  This copy is taken from the **git tag**, not the lua.org release tarball.

## Gaps (could not be checked — recorded, not passed)

- `tools/vet_dependency.py` could not reach the GitHub API from the vetting environment (HTTP 403), so none of its automated checks ran.
- The lua.org release tarball for 5.4.8 could not be downloaded from the vetting environment, so it was **not** compared against the git tag.
  This is the comparison that catches an xz-style tarball-only change; do it before shipping.
- Tags in this repository are lightweight (unsigned); there is no signature to verify.
- Account / key compromise, sanctions screening, runtime behaviour: not checked.

## How ReMoM uses it

Built as the static library `lua_static` (see `ext/CMakeLists.txt`), compiled as C.
Mod scripts run in a sandbox: `io`, `os`, `package`, `debug`, `dofile`, `loadfile` and `require` are not available,
and `math.random` is replaced by the game's own `Random()` (see `MOD/src/MOD_Lua.cpp`).
