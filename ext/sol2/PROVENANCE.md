# sol2 3.5.0 — vendored headers

| | |
|---|---|
| Upstream | https://github.com/ThePhD/sol2 |
| Version | tag `v3.5.0`, commit `9190880c593dfb018ccf5cc9729ab87739709862` (2025-02-25, ThePhD, "3.5.0 Release") |
| License | MIT (`LICENSE.txt`) |
| Tier | **A — ships to users** (compiled into ReMoM and HeMoM through `MOD/src/MOD_Lua.cpp`) |
| Files | the tag's `include/` tree, unmodified, plus `LICENSE.txt` and `README.md`. Header-only: nothing here is built on its own. |
| Hashes | `SHA256SUMS.txt` — re-run `sha256sum -c SHA256SUMS.txt` in this folder to detect drift |
| Vetted | 2026-10-27, by an AI agent; **the decision to ship it is the maintainer's, not the agent's** |

## Facts checked (from the git history of the upstream repo)

- The repository's history starts 2013-10-17. The current maintainer, ThePhD, has committed since 2013-12-01 (1,600 commits, the top committer by a wide margin).
- The release tag is an annotated tag on a commit by ThePhD.

## Flags

- **Activity:** the latest commit on the default branch is 2025-03-07, about 19 months before vetting.
  That is past the policy's 365-day inactivity threshold: security or compiler-compatibility fixes may not arrive upstream.

## Gaps (could not be checked — recorded, not passed)

- `tools/vet_dependency.py` could not reach the GitHub API from the vetting environment (HTTP 403), so none of its automated checks ran.
- The annotated tag's signature was not verified (no maintainer key available in the vetting environment).
- Account / key compromise, sanctions screening, runtime behaviour: not checked.
- MSVC build of this header set inside ReMoM (with the project's `/W4 /WX`) was not tested; only Clang on Linux was.

## How ReMoM uses it

Only `MOD/src/MOD_Lua.cpp` includes sol2. The rest of the game stays C and talks to Lua through the C header `MOD/src/MOD_Lua.h`.
Built with `SOL_ALL_SAFETIES_ON=1`, `SOL_USING_CXX_LUA=0` and `SOL_USE_LUA_HPP=0` (Lua is compiled as C).
