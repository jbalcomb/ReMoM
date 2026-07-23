#!/usr/bin/env python3
"""
tools/fresh_install.py -- wipe ReMoM's per-user files and reinstall from scratch.

Clears the files ReMoM copies/creates for *itself* -- the seeded CONFIG.MOM,
MAGIC.SET, save games, backups, logs, config, and cache in your per-user
directories -- so the next launch goes through a clean first run.  Then runs the
full autotools install exactly as you would by hand:

    sh autogen.sh
    ./configure
    make -j<N>
    sudo make install

It NEVER touches your original game data (assets/, *.LBX, your real saves) --
only ReMoM's own per-user directories under your home.  Because first-run seeding
re-copies CONFIG.MOM/MAGIC.SET/SAVE1-9.GAM from your game data, wiping the
per-user copies is safe: the fresh launch repopulates them.

Usage:
    python3 tools/fresh_install.py [options] [-- <extra ./configure args>]

Options:
    -n, --dry-run     show what would be removed / run; change nothing
    -j, --jobs N      parallel make jobs (default: all CPUs)
        --no-install  build but skip 'sudo make install'
        --distclean   'make distclean' before bootstrapping (fully fresh build)
        --keep-data   don't wipe the per-user files (just rebuild + reinstall)
        --run         launch ReMoM at the end
"""

import argparse
import os
import platform
import shutil
import subprocess
import sys
from pathlib import Path


def _c(text, code):
    """Wrap text in an ANSI color when stdout is a terminal."""
    return "\033[%sm%s\033[0m" % (code, text) if sys.stdout.isatty() else text


def step(msg):
    print("\n" + _c("==> " + msg, "1;36"))


def info(msg):
    print("    " + msg)


def die(msg):
    print(_c("error: " + msg, "1;31"), file=sys.stderr)
    sys.exit(1)


def remom_user_dirs():
    """The per-user ReMoM directories, resolved the way STU_GRAF does."""
    home = Path.home()
    sysname = platform.system()

    if sysname == "Darwin":
        app = home / "Library" / "Application Support" / "ReMoM"   # data + config
        dirs = [app,
                home / "Library" / "Caches" / "ReMoM",             # cache
                home / "Library" / "Logs" / "ReMoM"]               # logs
    elif sysname == "Windows":
        appdata = Path(os.environ.get("APPDATA", home / "AppData" / "Roaming"))
        local = Path(os.environ.get("LOCALAPPDATA", home / "AppData" / "Local"))
        dirs = [appdata / "ReMoM",            # data + config
                local / "ReMoM" / "cache",    # cache
                local / "ReMoM" / "logs"]     # logs
    else:  # Linux / XDG
        def xdg(env, default):
            val = os.environ.get(env)
            return Path(val) if val else home / default
        dirs = [xdg("XDG_DATA_HOME",   ".local/share") / "ReMoM",   # CONFIG.MOM, MAGIC.SET, saves, backups
                xdg("XDG_CONFIG_HOME", ".config")       / "ReMoM",   # ReMoM.ini
                xdg("XDG_CACHE_HOME",  ".cache")        / "ReMoM",   # modified copies
                xdg("XDG_STATE_HOME",  ".local/state")  / "ReMoM"]   # logs

    seen, out = set(), []
    for d in dirs:
        d = d.expanduser()
        if d not in seen:
            seen.add(d)
            out.append(d)
    return out


def safe_rmtree(path, dry_run):
    """Remove a per-user ReMoM directory, with guards so we can never wipe $HOME
    or anything that isn't clearly a ReMoM directory."""
    p = path.expanduser()
    if not p.exists():
        info(_c("skip (absent)  ", "2;37") + str(p))
        return

    rp = p.resolve()
    home = Path.home().resolve()
    if "ReMoM" not in p.parts:
        die("refusing to remove '%s' (not a ReMoM path)" % p)
    if rp == home or home not in rp.parents:
        die("refusing to remove '%s' (must be a subdir of %s)" % (p, home))

    if dry_run:
        info(_c("would remove   ", "1;33") + str(p))
        return
    shutil.rmtree(p)
    info(_c("removed        ", "1;32") + str(p))


def run(cmd, cwd, dry_run):
    printable = " ".join(str(x) for x in cmd)
    info(_c("$ " + printable, "1;37"))
    if dry_run:
        return
    result = subprocess.run(cmd, cwd=str(cwd))
    if result.returncode != 0:
        die("command failed (exit %d): %s" % (result.returncode, printable))


def main():
    ap = argparse.ArgumentParser(
        description="Wipe ReMoM's per-user files and reinstall from scratch.")
    ap.add_argument("-n", "--dry-run", action="store_true",
                    help="show actions, change nothing")
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 1,
                    help="parallel make jobs (default: all CPUs)")
    ap.add_argument("--no-install", action="store_true",
                    help="build but skip 'sudo make install'")
    ap.add_argument("--distclean", action="store_true",
                    help="'make distclean' before bootstrapping (fully fresh build)")
    ap.add_argument("--keep-data", action="store_true",
                    help="don't wipe the per-user files")
    ap.add_argument("--run", action="store_true",
                    help="launch ReMoM at the end")
    ap.add_argument("configure_args", nargs=argparse.REMAINDER,
                    help="extra args passed to ./configure after a literal --")
    args = ap.parse_args()

    extra = args.configure_args
    if extra and extra[0] == "--":
        extra = extra[1:]

    repo = Path(__file__).resolve().parent.parent
    if not (repo / "configure.ac").exists() or not (repo / "autogen.sh").exists():
        die("can't find configure.ac / autogen.sh at %s" % repo)

    if args.dry_run:
        print(_c("DRY RUN -- nothing will be changed.", "1;35"))

    # 1) wipe ReMoM's per-user files
    if args.keep_data:
        step("Keeping ReMoM's per-user files (--keep-data)")
    else:
        step("Clearing ReMoM's per-user files")
        info(_c("(your original game data in assets/ is untouched)", "2;37"))
        for d in remom_user_dirs():
            safe_rmtree(d, args.dry_run)

    # 2) optional full-clean of the build tree
    if args.distclean and (repo / "Makefile").exists():
        step("make distclean")
        run(["make", "distclean"], repo, args.dry_run)

    # 3) the full autotools install
    step("sh autogen.sh")
    run(["sh", "autogen.sh"], repo, args.dry_run)

    step("./configure" + (("  " + " ".join(extra)) if extra else ""))
    run(["./configure"] + list(extra), repo, args.dry_run)

    step("make -j%d" % args.jobs)
    run(["make", "-j%d" % args.jobs], repo, args.dry_run)

    if args.no_install:
        step("Skipping install (--no-install)")
    else:
        step("sudo make install")
        info(_c("(sudo may prompt for your password)", "2;37"))
        run(["sudo", "make", "install"], repo, args.dry_run)

    step("Done")
    if args.run:
        info(_c("$ ReMoM", "1;37"))
        if not args.dry_run:
            subprocess.run(["ReMoM"])
    else:
        info("Next:  " + _c("ReMoM", "1;37") + "   (goes through the fresh first-run)")


if __name__ == "__main__":
    main()
