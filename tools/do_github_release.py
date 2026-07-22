#!/usr/bin/env python3
"""tools/do_github_release.py -- cut the next release, watch CI, publish on green.

Flow:
  1. Find the newest vMAJOR.MINOR.PATCH git tag and bump PATCH (default) to a new tag.
  2. Create + push the tag -- this fires .github/workflows/release.yml (build, test,
     package all platforms, then create a DRAFT GitHub release).
  3. Watch the Actions run for that tag until it finishes.
  4. success -> publish the draft release; flash the screen + HIGH beep.
     failure -> leave the draft as-is;   flash the screen + LOW  beep.

Usage:
  python3 tools/do_github_release.py             # no bump chosen: print current version + usage, do nothing
  python3 tools/do_github_release.py --patch     # bump PATCH (X.Y.Z -> X.Y.Z+1), release, publish
  python3 tools/do_github_release.py --minor     # bump MINOR (X.Y+1.0)
  python3 tools/do_github_release.py --major     # bump MAJOR (X+1.0.0)
  python3 tools/do_github_release.py --patch -n  # dry run: show the tag it WOULD cut, do nothing
  python3 tools/do_github_release.py --patch -y  # don't prompt before tag+push
  python3 tools/do_github_release.py --patch --no-publish   # release + watch, but leave it a draft

Requires: git, and the GitHub CLI `gh` (authenticated).  Tones use aplay/paplay if
present, else the terminal bell.
"""

import argparse
import json
import math
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import time
import wave

WORKFLOW = "release.yml"


# --- shell helpers ---------------------------------------------------------
def run(cmd, check=True, capture=True):
    r = subprocess.run(
        cmd, text=True,
        stdout=subprocess.PIPE if capture else None,
        stderr=subprocess.PIPE if capture else None,
    )
    if check and r.returncode != 0:
        sys.exit("error: " + " ".join(cmd) + "\n" + ((r.stderr or "").strip()))
    return (r.stdout or "").strip()


def gh_json(args):
    return json.loads(run(["gh"] + args))


# --- terminal flash + tones ------------------------------------------------
def flash(times=6):
    """Reverse the whole screen briefly, a few times (DECSCNM visual bell)."""
    for _ in range(times):
        sys.stdout.write("\033[?5h"); sys.stdout.flush(); time.sleep(0.08)
        sys.stdout.write("\033[?5l"); sys.stdout.flush(); time.sleep(0.08)


def _tone_wav(path, freq, secs=0.28, vol=0.5, rate=44100):
    n = int(rate * secs)
    edge = int(rate * 0.01)  # 10 ms fade in/out so it doesn't click
    with wave.open(path, "w") as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(rate)
        buf = bytearray()
        for i in range(n):
            env = min(1.0, i / edge, (n - i) / edge)
            s = int(vol * env * 32767 * math.sin(2 * math.pi * freq * i / rate))
            buf += struct.pack("<h", s)
        w.writeframes(bytes(buf))


def beep(success):
    # rising high pair for success; descending low pair for failure
    freqs = [880, 1320] if success else [220, 165]
    player = next((p for p in ("paplay", "aplay") if shutil.which(p)), None)
    if not player:
        for _ in range(2 if success else 3):
            sys.stdout.write("\a"); sys.stdout.flush(); time.sleep(0.18)
        return
    for f in freqs:
        tf = tempfile.NamedTemporaryFile(suffix=".wav", delete=False)
        tf.close()
        try:
            _tone_wav(tf.name, f)
            quiet = ["-q"] if player == "aplay" else []
            subprocess.run([player] + quiet + [tf.name],
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        finally:
            os.unlink(tf.name)


def finish(success, msg):
    color = "\033[1;32m" if success else "\033[1;31m"
    print(color + msg + "\033[0m")
    flash()
    beep(success)
    sys.exit(0 if success else 1)


# --- version ---------------------------------------------------------------
def latest_version():
    vers = []
    for t in run(["git", "tag", "--list", "v[0-9]*"]).splitlines():
        m = re.fullmatch(r"v(\d+)\.(\d+)\.(\d+)", t.strip())
        if m:
            vers.append(tuple(int(x) for x in m.groups()))
    return max(vers) if vers else None


# --- main ------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(
        description="Cut the next patch release, watch CI, publish on green.")
    ap.add_argument("-n", "--dry-run", action="store_true",
                    help="show the tag it WOULD cut; change nothing")
    bump = ap.add_mutually_exclusive_group()
    bump.add_argument("--patch", action="store_true", help="bump PATCH: X.Y.Z -> X.Y.Z+1")
    bump.add_argument("--minor", action="store_true", help="bump MINOR: X.Y.Z -> X.Y+1.0")
    bump.add_argument("--major", action="store_true", help="bump MAJOR: X.Y.Z -> X+1.0.0")
    ap.add_argument("-y", "--yes", action="store_true", help="don't prompt before tag+push")
    ap.add_argument("--no-publish", action="store_true",
                    help="release + watch, but leave the GitHub release a draft")
    args = ap.parse_args()

    # must be in a git repo to read the tags
    run(["git", "rev-parse", "--is-inside-work-tree"])
    cur = latest_version()
    curs = f"v{cur[0]}.{cur[1]}.{cur[2]}" if cur else None

    # No bump level chosen -> print the current version + usage and stop.  This keeps
    # a bare invocation from ever cutting a release by accident; you must pick a level.
    if not (args.patch or args.minor or args.major):
        print("current version: " + (curs if curs else "(none tagged yet)") + "\n")
        ap.print_help()
        return

    # from here on we actually cut a release -- gh is required
    if not shutil.which("gh"):
        sys.exit("error: GitHub CLI `gh` not found (needed to watch/publish).")
    run(["gh", "auth", "status"])

    x, y, z = cur or (0, 0, 0)
    if args.major:
        x, y, z = x + 1, 0, 0
    elif args.minor:
        x, y, z = x, y + 1, 0
    else:  # --patch
        z += 1
    newtag = f"v{x}.{y}.{z}"
    print(f"current: {curs or 'v0.0.0'}   ->   new: {newtag}")

    if run(["git", "tag", "--list", newtag]):
        sys.exit(f"error: tag {newtag} already exists locally.")
    if run(["git", "ls-remote", "--tags", "origin", newtag]):
        sys.exit(f"error: tag {newtag} already exists on origin.")

    branch = run(["git", "rev-parse", "--abbrev-ref", "HEAD"])
    head = run(["git", "rev-parse", "--short", "HEAD"])
    dirty = bool(run(["git", "status", "--porcelain"]))

    if args.dry_run:
        print(f"dry run -- would tag {branch}@{head} as {newtag}, push, watch CI, "
              + ("publish on green." if not args.no_publish else "leave it a draft."))
        return

    print(f"tagging {branch}@{head} as {newtag}"
          + ("  (WARNING: working tree is dirty; uncommitted changes are NOT in the "
             "release)" if dirty else ""))
    if not args.yes:
        try:
            if input(f"proceed -- push {newtag} and fire the release? [y/N] ").strip().lower() != "y":
                sys.exit("aborted.")
        except (EOFError, KeyboardInterrupt):
            sys.exit("\naborted.")

    run(["git", "tag", "-a", newtag, "-m", f"ReMoM {newtag}"])
    print(f"pushing {newtag} ...")
    run(["git", "push", "origin", newtag])

    # find the release.yml run this tag triggered (head_branch == tag for tag pushes)
    print("waiting for the Release workflow to start ...")
    run_id = url = None
    for _ in range(36):
        for r in gh_json(["run", "list", "--workflow", WORKFLOW, "-L", "12",
                          "--json", "databaseId,headBranch,event,url"]):
            if r["event"] == "push" and r["headBranch"] == newtag:
                run_id, url = r["databaseId"], r["url"]
                break
        if run_id:
            break
        time.sleep(5)
    if not run_id:
        finish(False, f"never saw a Release run for {newtag} -- check the Actions tab.")
    print(f"watching {url}")

    # block until the run completes; --exit-status is 0 only on success
    try:
        watch = subprocess.run(["gh", "run", "watch", str(run_id),
                                "--exit-status", "--interval", "20"])
    except KeyboardInterrupt:
        sys.exit(f"\nstopped watching -- the run is still going: {url}")
    if watch.returncode != 0:
        finish(False, f"Release {newtag} FAILED -- draft not published.  {url}")

    if args.no_publish:
        finish(True, f"Release {newtag} built green (left as a draft).  {url}")

    # publish the draft the workflow created (retry briefly for eventual consistency)
    print(f"CI green -- publishing {newtag} ...")
    last = ""
    for attempt in range(6):
        r = subprocess.run(["gh", "release", "edit", newtag,
                            "--draft=false", "--latest=true"],
                           text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if r.returncode == 0:
            finish(True, f"Published {newtag}.  "
                         f"https://github.com/{gh_repo()}/releases/tag/{newtag}")
        last = (r.stderr or "").strip()
        time.sleep(5)
    finish(False, f"CI was green but publishing {newtag} failed: {last}\n"
                  f"Publish it by hand: gh release edit {newtag} --draft=false --latest")


def gh_repo():
    try:
        return gh_json(["repo", "view", "--json", "nameWithOwner"])["nameWithOwner"]
    except Exception:
        return "OWNER/REPO"


if __name__ == "__main__":
    main()
