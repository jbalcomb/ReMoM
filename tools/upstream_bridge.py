#!/usr/bin/env python3
"""tools/upstream_bridge.py -- keep a branch that is someone else's main + ONE bridge commit of ours.

The branch is their main, plus a single commit on top holding every change we make to bridge their project
to ReMoM.  We never push to them: the script sets their remote's push URL to DISABLED.

Every subcommand is idempotent -- run it as often as you like and it converges on the same state:

  python3 tools/upstream_bridge.py              # status only: what is set up, ahead/behind; changes nothing
  python3 tools/upstream_bridge.py setup        # add/repair the remote (+ push DISABLED), fetch, create the branch
  python3 tools/upstream_bridge.py sync         # setup, switch to the branch, rebase the bridge commit onto their new main
  python3 tools/upstream_bridge.py commit [-m MSG] [PATH ...]
                                                # fold everything into the ONE bridge commit (create it, or replace it)

`commit` stages modified TRACKED files, plus any PATHs you name (use that for new files).  It never runs
`git add -A`: on their branch, ReMoM's .gitignore does not apply, so `out/` and other ReMoM leftovers in the
working tree would show up as untracked files and be swept into the bridge commit.

Defaults are for binaryalgorithm/mom131-source-map; --remote / --url / --upstream-branch / --branch override them.

Pushing the bridge branch to OUR origin is not done here.  `commit` and `sync` rewrite the bridge commit, so a
branch already on origin needs `git push --force-with-lease origin <branch>`.  Their repo has no license; pushing
it to a public origin republishes their work.
"""

import argparse
import os
import subprocess
import sys

DEFAULT_REMOTE = "binaryalgorithm"
DEFAULT_URL = "https://github.com/binaryalgorithm/mom131-source-map.git"
DEFAULT_UPSTREAM_BRANCH = "main"
DEFAULT_BRANCH = "binaryalgorithm-bridge"
DEFAULT_MESSAGE = "Bridge mom131-source-map to ReMoM"
PUSH_DISABLED = "DISABLED"


# --- git helpers -----------------------------------------------------------
def git(*args, check=True, capture=True):
    r = subprocess.run(["git"] + list(args), text=True,
                       stdout=subprocess.PIPE if capture else None,
                       stderr=subprocess.PIPE if capture else None)
    if check and r.returncode != 0:
        sys.exit("error: git " + " ".join(args) + "\n" + ((r.stderr or "").strip()))
    return (r.stdout or "").strip()


def git_ok(*args):
    return subprocess.run(["git"] + list(args), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode == 0


def git_dir():
    return git("rev-parse", "--git-dir")


def rebase_in_progress():
    d = git_dir()
    return os.path.isdir(os.path.join(d, "rebase-merge")) or os.path.isdir(os.path.join(d, "rebase-apply"))


def current_branch():
    return git("rev-parse", "--abbrev-ref", "HEAD")


def branch_exists(name):
    return git_ok("show-ref", "--verify", "--quiet", "refs/heads/" + name)


def ref_exists(ref):
    return git_ok("rev-parse", "--verify", "--quiet", ref + "^{commit}")


def tracked_changes():
    """Modified/staged tracked files (untracked ones are ignored on purpose)."""
    return git("status", "--porcelain", "--untracked-files=no")


# --- steps -----------------------------------------------------------------
def ensure_remote(a):
    remotes = git("remote").split()
    if a.remote not in remotes:
        git("remote", "add", a.remote, a.url)
        print(f"added remote {a.remote} -> {a.url}")
    elif git("remote", "get-url", a.remote) != a.url:
        git("remote", "set-url", a.remote, a.url)
        print(f"fixed remote {a.remote} URL -> {a.url}")
    if git("remote", "get-url", "--push", a.remote) != PUSH_DISABLED:
        git("remote", "set-url", "--push", a.remote, PUSH_DISABLED)
        print(f"push to {a.remote} disabled")


def fetch(a):
    print(f"fetching {a.remote} ...")
    git("fetch", "--prune", a.remote, capture=False)
    if not ref_exists(upstream_ref(a)):
        sys.exit(f"error: {upstream_ref(a)} not found after fetch.")


def upstream_ref(a):
    return f"{a.remote}/{a.upstream_branch}"


def ensure_branch(a):
    up = upstream_ref(a)
    if not branch_exists(a.branch):
        git("branch", "--track", a.branch, up)
        print(f"created branch {a.branch} from {up}")
    elif git("rev-parse", "--abbrev-ref", a.branch + "@{upstream}", check=False) != up:
        if not git_ok("merge-base", a.branch, up):
            sys.exit(f"error: branch {a.branch} already exists and shares no history with {up}; "
                     f"refusing to re-point it. Pass --branch NAME to use another branch.")
        git("branch", "--set-upstream-to", up, a.branch)
        print(f"branch {a.branch} now tracks {up}")


def setup(a):
    ensure_remote(a)
    fetch(a)
    ensure_branch(a)


def require_on_branch(a, switch):
    if rebase_in_progress():
        sys.exit("error: a rebase is in progress. Fix the conflicts, `git add` them, `git rebase --continue` "
                 "(or `git rebase --abort`), then run this again.")
    if current_branch() == a.branch:
        return
    if not switch:
        sys.exit(f"error: not on {a.branch}. Run `python3 tools/upstream_bridge.py sync` first.")
    if tracked_changes():
        sys.exit(f"error: uncommitted changes to tracked files on {current_branch()}; commit or stash them before "
                 f"switching to {a.branch}.")
    git("switch", a.branch, capture=False)


def bridge_commits(a):
    """Commits on the branch that are not in their main."""
    out = git("rev-list", "--reverse", f"{upstream_ref(a)}..{a.branch}")
    return out.split() if out else []


def sync(a):
    setup(a)
    require_on_branch(a, switch=True)
    up = upstream_ref(a)
    if git("merge-base", "HEAD", up) == git("rev-parse", up):
        print(f"{a.branch} is already on top of {up}.")
    else:
        if tracked_changes():
            sys.exit("error: uncommitted changes to tracked files; run `commit` (or stash) before rebasing.")
        print(f"rebasing {a.branch} onto {up} ...")
        if subprocess.run(["git", "rebase", up]).returncode != 0:
            sys.exit("rebase stopped on a conflict. Fix the files, `git add` them, `git rebase --continue`, "
                     "then run `sync` again.")
    report(a)


def commit(a):
    require_on_branch(a, switch=False)
    up = upstream_ref(a)
    if not ref_exists(up):
        sys.exit(f"error: {up} not found; run `setup` first.")
    git("add", "--update")
    if a.paths:
        git("add", "--", *a.paths)

    base = git("merge-base", "HEAD", up)
    old = bridge_commits(a)
    old_message = git("log", "-1", "--format=%B", old[0]) if old else ""
    message = a.message or old_message or DEFAULT_MESSAGE

    # The index now holds the whole bridge. Fold it into ONE commit on top of the merge base:
    # one commit with the same tree and message as before -> nothing to do (idempotent).
    staged_tree = git("write-tree")
    if len(old) == 1 and git("rev-parse", old[0] + "^{tree}") == staged_tree and message.strip() == old_message.strip():
        print("bridge commit already holds exactly these changes; nothing to do.")
        report(a)
        return
    if staged_tree == git("rev-parse", base + "^{tree}"):
        if old:
            git("reset", "--soft", base)
            print("no differences from their main left; bridge commit removed.")
        else:
            print("no differences from their main; nothing to commit.")
        report(a)
        return

    git("reset", "--soft", base)
    git("commit", "--quiet", "-m", message)
    print(("replaced" if old else "created") + f" the bridge commit ({len(old)} old commit(s) folded in).")
    report(a)


def report(a):
    up = upstream_ref(a)
    print()
    print(f"remote   {a.remote}: " + (git("remote", "get-url", a.remote, check=False) or "(missing)")
          + "   push: " + (git("remote", "get-url", "--push", a.remote, check=False) or "(missing)"))
    if not ref_exists(up):
        print(f"upstream {up}: not fetched")
        return
    print(f"upstream {up}: " + git("log", "-1", "--format=%h %ad %s", "--date=short", up))
    if not branch_exists(a.branch):
        print(f"branch   {a.branch}: not created")
        return
    if not git_ok("merge-base", a.branch, up):
        print(f"branch   {a.branch}: shares no history with {up} (not a bridge branch)")
        return
    ahead = bridge_commits(a)
    behind = git("rev-list", "--count", f"{a.branch}..{up}")
    print(f"branch   {a.branch}: {len(ahead)} bridge commit(s), {behind} behind {up}"
          + ("   <- current" if current_branch() == a.branch else ""))
    for c in ahead:
        print("         " + git("log", "-1", "--format=%h %s", c))
    if len(ahead) > 1:
        print("         more than one bridge commit: run `commit` to fold them into one.")
    if ahead:
        print(git("diff", "--stat", f"{up}...{a.branch}"))


def main():
    ap = argparse.ArgumentParser(description="Keep a branch = their main + one bridge commit.")
    ap.add_argument("--remote", default=DEFAULT_REMOTE)
    ap.add_argument("--url", default=DEFAULT_URL)
    ap.add_argument("--upstream-branch", default=DEFAULT_UPSTREAM_BRANCH)
    ap.add_argument("--branch", default=DEFAULT_BRANCH)
    sub = ap.add_subparsers(dest="cmd")
    sub.add_parser("setup", help="add/repair the remote (push disabled), fetch, create the branch")
    sub.add_parser("sync", help="setup, switch to the branch, rebase onto their new main")
    c = sub.add_parser("commit", help="fold all changes into the one bridge commit")
    c.add_argument("-m", "--message", help="commit message (default: keep the existing one)")
    c.add_argument("paths", nargs="*", help="new files to include (modified tracked files are always included)")
    a = ap.parse_args()

    git("rev-parse", "--is-inside-work-tree")
    if a.cmd == "setup":
        setup(a)
        report(a)
    elif a.cmd == "sync":
        sync(a)
    elif a.cmd == "commit":
        commit(a)
    else:
        report(a)
        print()
        ap.print_help()


if __name__ == "__main__":
    main()
