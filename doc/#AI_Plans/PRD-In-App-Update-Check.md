# PRD — In-App Update Check

**Status:** Draft
**Owner:** TBD
**Date:** 2026-07-24
**Related:** `RELEASES.md` (release/version mechanics), `PRD-Installer-Game-Data-Setup.md` (the `%APPDATA%\ReMoM\ReMoM.ini` config this reuses)

---

## Summary

Let ReMoM tell the player when a newer release exists. On request (and, optionally, once per launch), the game queries the GitHub Releases API for the latest published version, compares it against the version baked into the build, and — if newer — shows an unobtrusive "Update available: vX.Y.Z" line on the main menu with where to get it. It does **not** download or install anything; the player still grabs the new installer/ZIP themselves (see `RELEASES.md`). Manual re-download is the whole update mechanism today; this just makes players *aware* an update exists.

## Problem

There is no auto-update and no update *signal*. A player on v0.0.4 has no way to learn that v0.0.5 shipped short of manually checking the GitHub releases page. The in-app window title already shows their current version (`REMOM_VERSION_STRING`), but nothing compares it to what's available. Result: players silently run stale builds and miss fixes.

## Goals

- A **player-initiated** "Check for updates" action that queries the latest release and reports newer / up-to-date / check-failed.
- An **optional** auto-check at startup (default **off**), for players who want passive notification.
- A newer version surfaces as a **non-blocking** main-menu line, never a modal or a nag.
- **Explicit about the network call.** This is the first outbound network request the app has ever made; it happens only when the player asks or has opted in, never silently by default.
- **Dependency-light and OS-native** for the HTTP call — no new bundled third-party library on Windows.

## Non-Goals

- **Auto-download or auto-install.** Reporting only; the player runs the installer/ZIP as today. (A future "download the installer for me" is a separate, larger feature.)
- **Telemetry / analytics.** The request sends nothing but a plain GitHub API GET; we collect nothing back beyond the version string.
- **macOS.** macOS is already deferred project-wide (`RELEASES.md`); this ships Windows + Linux, macOS when the platform is un-deferred.
- **Pre-release / channel selection.** Only the single "latest published release" is considered. Draft/pre-release tags are ignored.
- **Bundling a JSON library.** The one field we need is extracted with a bounded string scan (see Implementation Notes), not a general parser.

## Functional Requirements

1. **Manual check (MVP core).** A "Check for updates" entry point (main-menu action or a key on the title screen — see Open Questions) performs the check synchronously-but-briefly (bounded timeout, see FR7) and reports one of: *"Up to date (vX.Y.Z)"*, *"Update available: vX.Y.Z"*, or *"Couldn't check for updates"*. A player-initiated action is itself the consent for that one request.
2. **Optional auto-check at startup.** Governed by `[Updates] check_at_startup` in `%APPDATA%\ReMoM\ReMoM.ini` (Windows) / the platform config dir. **Default `0` (off).** When `1`, the app performs one check per launch, after the main menu is up, on a path that never blocks input (see FR7). When off, no network call is made unless the player uses the manual action.
3. **Latest-version source.** `GET https://api.github.com/repos/jbalcomb/ReMoM/releases/latest`, `Accept: application/vnd.github+json`, a `User-Agent: ReMoM/<version>` header (GitHub requires a UA). HTTPS only.
4. **Version extraction.** From the response, read the `tag_name` field (e.g. `"v0.0.5"`). Strip a leading `v`. No full JSON parse — locate the `"tag_name"` key and read the quoted value (the response is a trusted, fixed-shape GitHub document).
5. **Version comparison.** Parse both the installed `REMOM_VERSION_STRING` and the fetched tag into numeric `MAJOR.MINOR.PATCH` and compare numerically (same triple the build already derives — see `CMakeLists.txt` `REMOM_VERSION_NUMERIC`). "Newer" = fetched triple > installed triple. Ties and older = "up to date."
6. **Dev-build suppression.** If `REMOM_VERSION_STRING` is `0.0.0-dev` (untagged local build), the manual check may still run but **never reports "update available"** (every real release is numerically newer than `0.0.0`, which would be noise). Auto-check is a no-op for dev builds.
7. **Non-blocking & bounded.** The request runs with a short connect/read timeout (≈3–5 s) and must not freeze the menu. Either a worker thread that posts its result back, or a one-shot async poll — the input loop keeps running throughout. On timeout/error the result is simply "couldn't check."
8. **Silent failure.** No network, DNS failure, non-200, rate-limit (HTTP 403), or malformed body → the auto-check shows **nothing**; the manual check shows "Couldn't check for updates." Never a repeated pop-up.
9. **Surface.** A newer version shows as one static line on the main menu — `Update available: v0.0.5  —  github.com/jbalcomb/ReMoM/releases` — styled like existing menu text, dismissable by normal navigation. No modal, no forced click.

## Acceptance Criteria

- [ ] With `check_at_startup=0` and no manual action, the app makes **zero** network connections (verifiable with a socket monitor / firewall prompt).
- [ ] The manual "Check for updates" action, on a build older than the latest release, reports "Update available: v<latest>".
- [ ] The same action on a build equal to the latest reports "Up to date".
- [ ] With the network unreachable, the manual action reports "Couldn't check for updates" within the timeout and the menu stays responsive throughout.
- [ ] A `0.0.0-dev` build never displays "update available".
- [ ] `check_at_startup=1` shows the update line on the main menu (when one exists) without ever blocking input.
- [ ] The Windows build makes the request via an OS-native HTTP path with **no newly bundled DLL**.

## Implementation Notes

### Platform HTTP seam (the real work)

The app has **no HTTP code today** (verified: nothing in `platform/`, `src/`, `MoX/` references WinHTTP/WinINet/libcurl/sockets). Add one narrow platform entry point, e.g.:

```c
/* Returns 1 and fills out[] (NUL-terminated, truncated to cap) on HTTP 200;
   0 on any failure. Blocks up to timeout_ms. No allocation beyond out[]. */
int Platform_Http_Get(const char * url, char * out, int cap, int timeout_ms);
```

- **HTTP is per-OS, not per-render-backend.** Windows ships the SDL2 backend but the request should use **WinHTTP** (in every Windows since XP, no bundled dependency) — the same implementation serves the `MSVC-win32` and `MSVC-*` (SDL2) Windows builds. So the split is `platform/http/win_http.c` (WinHTTP) and `platform/http/posix_http.c` (libcurl), selected by OS, wired into both build systems.
- **Linux:** libcurl is near-universal and the AppImage can bundle it if needed; the plain ZIP already assumes system SDL2, so a system libcurl is a consistent assumption. (Confirm before committing — see Risks.)
- Keep the seam tiny: one GET, no cookies, no redirects beyond what WinHTTP/libcurl follow by default, HTTPS only.

### Response handling

- Extract `tag_name` with a bounded scan: find `"tag_name"`, skip to the next `"`, copy until the closing `"`, cap the length. Reject anything not matching `v?\d+\.\d+\.\d+`.
- Reuse the build's numeric-triple logic conceptually (`CMakeLists.txt` already splits `REMOM_VERSION` into `REMOM_VERSION_NUMERIC`); the runtime does the same split on both strings.

### Config

- One key in the existing `ReMoM.ini` (`%APPDATA%\ReMoM` on Windows), read via the same INI path the game-data setting uses:
  ```ini
  [Updates]
  check_at_startup=0
  ```
- Optional throttle key later (`last_check_epoch`) if once-per-launch proves chatty; not needed for MVP.

### Placement

- Manual action + result string near the main-menu build at [`src/ReMoM.c:624`](../../src/ReMoM.c#L624) (`// BEGIN: ~== Main Menu Screen`). The check itself lives in a small new module (e.g. `src/ReMoM_Update.c` or under `MoX`), calling `Platform_Http_Get`.
- This is **reconstruction-neutral**: an additive ReMoM feature with no OG-asm counterpart, so all of it is `/* CLAUDE */`/new-code, not fidelity-governed.

## Risks / Open Questions

- **Consent model.** MVP proposal: manual action = implicit per-request consent; auto-check default off. Alternative: a one-time first-run "Check for updates automatically? [Yes/No]" that sets `check_at_startup`. First-run prompt is friendlier for passive users but adds a startup dialog. **Decide before building.**
- **Linux HTTP dependency.** libcurl assumed present/bundleable. If we'd rather not depend on it, the fallback is shelling out to `curl`/`wget` (ugly) or skipping auto-check on Linux for MVP (Windows-only notification first). **Confirm the libcurl stance.**
- **GitHub rate limiting.** Unauthenticated API is 60 requests/hour/IP — irrelevant for a human clicking "check," fine for once-per-launch. No token needed (and we would not ship one). A 403 is treated as "couldn't check."
- **Surface location.** Main-menu line vs. a title-screen key vs. both. The main-menu line covers passive auto-check; a menu/hotkey covers the manual action. Likely both; confirm the exact UI hook.
- **Where "Check for updates" lives in the menu.** MoM's main menu is a fixed reconstructed layout; adding a visible menu item may be intrusive. A hotkey (e.g. `F1`-style) or a small corner line may fit better than a new button. **UI decision needed.**
- **Threading.** The engine loop is single-threaded; a worker thread posting a result flag back is simplest, but confirm no engine-state is touched off-thread (the HTTP thread only writes a small result struct the main loop polls).

## Dependencies

- `REMOM_VERSION_STRING` / `remom_version.h` (exists) — the installed-version side of the compare.
- The `%APPDATA%\ReMoM\ReMoM.ini` config path (exists; used by the game-data setup work) — for the `check_at_startup` toggle.
- WinHTTP (OS-provided on Windows). libcurl (Linux) — see Risks.
- Follows the project **dependency-vetting** policy for anything on Linux (`doc/#Devel/Dependency-Vetting.md`); WinHTTP is an OS component, no vetting artifact needed.
