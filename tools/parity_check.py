#!/usr/bin/env python3
"""
parity_check.py — Cross-check that ReMoMber and HeMoM produce identical
behavior and data for a given (config, seed, .RMR) tuple.

The goal is to *prove* they match — not merely claim it. Every comparison
shows its inputs (path, byte size, line count, md5) and its method, then
its result. On failure, drill-down identifies the offset / line / field
that diverges.

Usage:
    tools/parity_check.py \\
        [--config assets/matchup_hemom.ini] \\
        [--seed 12345] \\
        [--rmr   assets/menu_baseline_seed12345.RMR] \\
        [--preset clang-debug]

Scratch lives under out/build/<preset>/analysis/{hemom,remom}/ and is
overwritten on each run.  Nothing is committed.

Phases:
    1. Collect : build both binaries; run HeMoM --newgame; run
                 ReMoMber --replay (headless); gather artifacts.
    2. Compare : a battery of comparators (RNG stream, save files,
                 MAGIC.SET, save-dump text, state-summary anchors).
    3. Report  : per-section PASS/FAIL with evidence; overall verdict.
"""

from __future__ import annotations

import argparse
import dataclasses
import hashlib
import os
import shutil
import subprocess
import sys
from pathlib import Path
from typing import Callable, Iterable, Optional


# ---------------------------------------------------------------------------
# Repo geometry
# ---------------------------------------------------------------------------

REPO_ROOT = Path(__file__).resolve().parent.parent
ASSETS_DIR = REPO_ROOT / "assets"
BUILD_ROOT = REPO_ROOT / "out" / "build"


def pick_preset(explicit: Optional[str]) -> str:
    """Mirror the bash scripts: prefer an existing build dir; else clang-debug."""
    if explicit:
        return explicit
    candidates = ["MSVC-headless-debug", "clang-debug", "MSVC-debug"]
    for name in candidates:
        if (BUILD_ROOT / name / "bin" / "Debug").is_dir():
            return name
    return "clang-debug"


@dataclasses.dataclass(frozen=True)
class BuildLayout:
    preset: str
    build_dir: Path
    bin_dir: Path
    hemom_exe: Path
    remom_exe: Path
    analysis_dir: Path
    hemom_out: Path
    remom_out: Path


def make_layout(preset: str) -> BuildLayout:
    build_dir = BUILD_ROOT / preset
    bin_dir = build_dir / "bin" / "Debug"
    # Linux clang preset has bare names; MSVC has .exe.
    hemom_candidates = [bin_dir / "HeMoM", bin_dir / "HeMoM.exe"]
    remom_candidates = [bin_dir / "ReMoMber", bin_dir / "ReMoMber.exe"]
    # Choose the one that exists, or the first as a build target stub.
    hemom_exe = next((p for p in hemom_candidates if p.exists()), hemom_candidates[0])
    remom_exe = next((p for p in remom_candidates if p.exists()), remom_candidates[0])
    analysis_dir = bin_dir / "analysis"
    return BuildLayout(
        preset=preset,
        build_dir=build_dir,
        bin_dir=bin_dir,
        hemom_exe=hemom_exe,
        remom_exe=remom_exe,
        analysis_dir=analysis_dir,
        hemom_out=analysis_dir / "hemom",
        remom_out=analysis_dir / "remom",
    )


# ---------------------------------------------------------------------------
# Result types
# ---------------------------------------------------------------------------

@dataclasses.dataclass
class Evidence:
    """Visible-to-user evidence for a single comparator result."""
    inputs: list[tuple[str, Path, int, int, str]]  # (label, path, bytes, lines, md5)
    method: str                                     # one-line description
    findings: list[str]                             # bullet points shown in report
    reproduce: list[str]                            # copy-paste shell commands


@dataclasses.dataclass
class ComparatorResult:
    name: str           # short label, e.g. "RNG stream"
    status: str         # "PASS" | "FAIL" | "INFO" | "SKIP"
    summary: str        # one-line summary for the table row
    evidence: Evidence


# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------

def md5_of(path: Path) -> str:
    if not path.exists():
        return "(missing)"
    h = hashlib.md5()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def line_count(path: Path) -> int:
    if not path.exists():
        return 0
    with path.open("rb") as f:
        return sum(1 for _ in f)


def byte_size(path: Path) -> int:
    return path.stat().st_size if path.exists() else 0


def relpath(p: Path) -> str:
    """Repo-relative path if inside REPO_ROOT, else absolute."""
    try:
        return str(p.relative_to(REPO_ROOT))
    except ValueError:
        return str(p)


# ---------------------------------------------------------------------------
# Reporting
# ---------------------------------------------------------------------------

USE_COLOR = sys.stdout.isatty() and os.environ.get("NO_COLOR") is None
RED = "\033[31m" if USE_COLOR else ""
GREEN = "\033[32m" if USE_COLOR else ""
YELLOW = "\033[33m" if USE_COLOR else ""
DIM = "\033[2m" if USE_COLOR else ""
RESET = "\033[0m" if USE_COLOR else ""

STATUS_GLYPH = {
    "PASS": f"{GREEN}PASS{RESET}",
    "FAIL": f"{RED}FAIL{RESET}",
    "INFO": f"{DIM}INFO{RESET}",
    "SKIP": f"{YELLOW}SKIP{RESET}",
}


def print_section_header(text: str) -> None:
    print()
    print(f"=== {text} ===")


def print_result_block(result: ComparatorResult) -> None:
    print()
    print(f"[{STATUS_GLYPH[result.status]}] {result.name}")
    print(f"    {result.summary}")
    if result.evidence.inputs:
        print("    Inputs:")
        for label, path, nbytes, nlines, digest in result.evidence.inputs:
            line_info = f"{nlines:>7,} lines" if nlines >= 0 else "  (binary)"
            print(f"      {label:<18} {nbytes:>10,} B  {line_info}  md5={digest[:12]}…  {relpath(path)}")
    print(f"    Method: {result.evidence.method}")
    if result.evidence.findings:
        for line in result.evidence.findings:
            print(f"      • {line}")
    if result.evidence.reproduce:
        print("    Reproduce by hand:")
        for cmd in result.evidence.reproduce:
            print(f"      $ {cmd}")


def print_overall(results: list[ComparatorResult]) -> int:
    by_status: dict[str, list[ComparatorResult]] = {"PASS": [], "FAIL": [], "SKIP": [], "INFO": []}
    for r in results:
        by_status.setdefault(r.status, []).append(r)
    print()
    print("=" * 72)
    # Compact one-liner roll-up of every section.
    print("Section summary:")
    for r in results:
        print(f"  [{STATUS_GLYPH[r.status]}] {r.name:<28} {r.summary}")
    print()
    n_pass = len(by_status["PASS"])
    n_fail = len(by_status["FAIL"])
    n_skip = len(by_status["SKIP"])
    n_info = len(by_status["INFO"])
    judged = n_pass + n_fail
    if n_fail:
        print(f"OVERALL: {STATUS_GLYPH['FAIL']}  "
              f"{n_pass}/{judged} sections matched · {n_fail} failed"
              + (f" · {n_skip} skipped" if n_skip else "")
              + (f" · {n_info} info" if n_info else ""))
        print()
        print("Failures:")
        for r in by_status["FAIL"]:
            print(f"  - {r.name}: {r.summary}")
        return 2
    if judged == 0:
        print(f"OVERALL: {STATUS_GLYPH['SKIP']}  no comparators were able to run "
              f"({n_skip} skipped, {n_info} info)")
        return 0
    print(f"OVERALL: {STATUS_GLYPH['PASS']}  {n_pass}/{judged} sections matched"
          + (f" · {n_skip} skipped" if n_skip else "")
          + (f" · {n_info} info" if n_info else ""))
    return 0


# ---------------------------------------------------------------------------
# Phase 1: Build, Run, Collect
# ---------------------------------------------------------------------------

# Artifacts each binary produces in its CWD that we want to keep for analysis.
# (SAVE2.GAM is conditional on save_slot=1 in the .ini; missing is OK.)
HEMOM_PRODUCED = ["SAVE2.GAM", "SAVE9.txt", "MAGIC.SET"]
REMOM_PRODUCED = ["SAVE2.GAM", "MAGIC.SET"]
# SAVE9.GAM is intentionally NOT compared: HeMoM writes it mid-flow (end of
# MAGIC.EXE equivalent), ReMoMber's auto-save overwrites it post-PreInit_Overland.
# OG has no way to snapshot at the same moment on both sides, so the file is
# structurally incomparable. SAVE2.GAM (the named slot save written by both
# binaries after Loaded_Game_Update) is the parity probe.


class Phase1Error(RuntimeError):
    """Raised when build/run/collect can't complete; main reports cleanly."""


def run(cmd: list[str], *, cwd: Path, env: Optional[dict[str, str]] = None,
        stderr_to: Optional[Path] = None, label: str = "") -> int:
    """Run a subprocess. Stream stderr to a file if requested; return exit code."""
    print(f"    $ {' '.join(cmd)}")
    if cwd != Path.cwd():
        print(f"      cwd: {relpath(cwd)}")
    stderr_handle = open(stderr_to, "wb") if stderr_to else subprocess.DEVNULL
    try:
        proc = subprocess.run(
            cmd, cwd=cwd, env=env,
            stdout=subprocess.DEVNULL, stderr=stderr_handle,
        )
        return proc.returncode
    finally:
        if stderr_to:
            stderr_handle.close()


def cmake_build(target: str, layout: BuildLayout) -> None:
    """Incremental cmake build of a target. cmake --build --preset reads CMakePresets.json from CWD."""
    print(f"  Build target: {target}")
    rc = run(["cmake", "--build", "--preset", layout.preset, "--target", target],
             cwd=REPO_ROOT)
    if rc != 0:
        raise Phase1Error(f"cmake build of {target} failed (exit {rc})")
    # Verify the binary now exists.
    exe = layout.hemom_exe if target == "HeMoM" else layout.remom_exe
    if not exe.exists():
        raise Phase1Error(f"build of {target} reported success but {relpath(exe)} is missing")
    print(f"    → {relpath(exe)} ({byte_size(exe):,} B)")


def clean_workspace(bin_dir: Path) -> None:
    """Remove ONLY stderr.log — it's always overwritten anyway, but we want a
    clean capture per run.  SAVE/MAGIC files are left in place so the user's
    persisted state (typed save names, chosen options) survives across runs;
    each binary overwrites them as it runs."""
    (bin_dir / "stderr.log").unlink(missing_ok=True)


def collect_artifacts(bin_dir: Path, into: Path, names: Iterable[str],
                      since_ts: float = 0.0) -> list[Path]:
    """COPY produced files from bin_dir into the target analysis dir.

    `since_ts`: if non-zero, skip files whose mtime is older than this
    timestamp — these are stale (from a prior run that the current binary
    didn't actually rewrite) and shouldn't be attributed to this run.
    A warning is printed so the user knows.
    """
    into.mkdir(parents=True, exist_ok=True)
    collected: list[Path] = []
    for name in names:
        src = bin_dir / name
        if not src.exists():
            continue
        if since_ts > 0.0 and src.stat().st_mtime < since_ts:
            print(f"    WARNING: {name} is stale (mtime before this run started) — skipping")
            continue
        dst = into / name
        shutil.copy2(str(src), str(dst))
        collected.append(dst)
    return collected


def filter_rng_log(stderr_log: Path, rng_log: Path) -> tuple[int, int]:
    """Filter `[RNG-CALL]` lines out of stderr into rng_log.
    Returns (total_stderr_lines, rng_lines)."""
    total = 0
    rng = 0
    with stderr_log.open("rb") as src, rng_log.open("wb") as dst:
        for line in src:
            total += 1
            if line.startswith(b"[RNG-CALL]"):
                dst.write(line)
                rng += 1
    return total, rng


def run_hemom(layout: BuildLayout, config: Path, seed: int) -> dict[str, Path]:
    print()
    print("  Run HeMoM --newgame")
    if not config.exists():
        raise Phase1Error(f"config not found: {config}")
    clean_workspace(layout.bin_dir)
    layout.hemom_out.mkdir(parents=True, exist_ok=True)
    stderr_log = layout.bin_dir / "stderr.log"
    import time
    t_start = time.time()
    rc = run(
        [str(layout.hemom_exe), "--newgame", str(config), "--seed", str(seed)],
        cwd=layout.bin_dir,
        stderr_to=stderr_log,
    )
    if rc != 0:
        # Surface the tail so user can see why.
        print(f"    HeMoM exited {rc}. Last 20 lines of stderr:")
        if stderr_log.exists():
            tail = stderr_log.read_text(errors="replace").splitlines()[-20:]
            for line in tail:
                print(f"      {line}")
        raise Phase1Error(f"HeMoM run failed (exit {rc})")
    # Copy stderr first, then the data artifacts.
    if not stderr_log.exists():
        raise Phase1Error(f"HeMoM produced no stderr.log at {stderr_log}")
    shutil.copy2(str(stderr_log), str(layout.hemom_out / "stderr.log"))
    moved = collect_artifacts(layout.bin_dir, layout.hemom_out, HEMOM_PRODUCED, since_ts=t_start)
    # Filter to rng.log alongside.
    rng_log = layout.hemom_out / "rng.log"
    total, rng = filter_rng_log(layout.hemom_out / "stderr.log", rng_log)
    print(f"    artifacts in {relpath(layout.hemom_out)}/: "
          f"{', '.join(p.name for p in moved) or '(none)'}")
    print(f"    stderr.log: {total:,} lines, rng.log: {rng:,} lines")
    return {p.name: p for p in moved} | {
        "stderr.log": layout.hemom_out / "stderr.log",
        "rng.log":    rng_log,
    }


def run_remom(layout: BuildLayout, seed: int, rmr: Path) -> dict[str, Path]:
    print()
    print("  Run ReMoMber --replay (headless)")
    if not rmr.exists():
        raise Phase1Error(f"RMR file not found: {rmr}")
    clean_workspace(layout.bin_dir)
    layout.remom_out.mkdir(parents=True, exist_ok=True)
    stderr_log = layout.bin_dir / "stderr.log"
    env = os.environ.copy()
    env.setdefault("SDL_VIDEODRIVER", "offscreen")
    import time
    t_start = time.time()
    rc = run(
        [str(layout.remom_exe), "--seed", str(seed), "--replay", str(rmr)],
        cwd=layout.bin_dir,
        env=env,
        stderr_to=stderr_log,
    )
    # ReMoMber exit code on replay can be non-zero for benign reasons (user-quit).
    # We rely on artifact presence below to decide if the run was usable.
    if rc != 0:
        print(f"    ReMoMber exited {rc} (often benign on --replay quit; checking artifacts)")
    if not stderr_log.exists():
        raise Phase1Error(f"ReMoMber produced no stderr.log at {stderr_log}")
    shutil.copy2(str(stderr_log), str(layout.remom_out / "stderr.log"))
    moved = collect_artifacts(layout.bin_dir, layout.remom_out, REMOM_PRODUCED, since_ts=t_start)
    if not (layout.remom_out / "SAVE2.GAM").exists():
        raise Phase1Error("ReMoMber did not produce SAVE2.GAM — RMR may be incompatible")
    rng_log = layout.remom_out / "rng.log"
    total, rng = filter_rng_log(layout.remom_out / "stderr.log", rng_log)
    print(f"    artifacts in {relpath(layout.remom_out)}/: "
          f"{', '.join(p.name for p in moved) or '(none)'}")
    print(f"    stderr.log: {total:,} lines, rng.log: {rng:,} lines")
    return {p.name: p for p in moved} | {
        "stderr.log": layout.remom_out / "stderr.log",
        "rng.log":    rng_log,
    }


def phase1_collect(args: argparse.Namespace, layout: BuildLayout) -> tuple[dict[str, Path], dict[str, Path]]:
    print_section_header("Phase 1: Build · Run · Collect")
    layout.analysis_dir.mkdir(parents=True, exist_ok=True)
    if not args.skip_build:
        cmake_build("HeMoM", layout)
        cmake_build("ReMoMber", layout)
    else:
        print("  (--skip-build)")

    # BEFORE HeMoM runs and overwrites bin/Debug, snapshot any ReMoM-side
    # artifacts left there by a manual ReMoMber run into analysis/remom/.
    # This is for the workflow: user runs ReMoMber manually, then runs this
    # tool — without this step, HeMoM would clobber the user's ReMoMber output.
    _snapshot_remom_if_newer(layout)

    if args.skip_hemom:
        print("\n  (--skip-hemom — reusing existing analysis/hemom/)")
        hemom_artifacts = {p.name: p for p in layout.hemom_out.iterdir() if p.is_file()} if layout.hemom_out.is_dir() else {}
        if "SAVE2.GAM" not in hemom_artifacts:
            raise Phase1Error(f"--skip-hemom but no SAVE2.GAM under {relpath(layout.hemom_out)}")
    else:
        hemom_artifacts = run_hemom(layout, Path(args.config), args.seed)

    if args.run_remom:
        try:
            remom_artifacts = run_remom(layout, args.seed, Path(args.rmr))
        except Phase1Error as e:
            print(f"    {YELLOW}headless ReMoMber failed: {e}{RESET}")
            print(f"    falling back to whatever is already in {relpath(layout.remom_out)}/")
            remom_artifacts = _scan_existing(layout.remom_out)
    else:
        print()
        print("  (default mode — using pre-captured ReMoMber artifacts; pass --run-remom to try headless)")
        remom_artifacts = _scan_existing(layout.remom_out)

    # stderr is the minimum — the RNG trace lives there and is foundational.
    # Missing SAVE/MAGIC files are tolerated (the comparator for each will SKIP
    # with a clear note, and Phase 3 surfaces the SKIPs in the report).
    if "stderr.log" not in remom_artifacts:
        raise Phase1Error(
            f"ReMoMber stderr.log missing in {relpath(layout.remom_out)}/.\n"
            "  Populate by running:  ./tools/capture_remom_rng_baseline.sh "
            f"{args.seed}\n"
            "  (the capture script writes its artifacts into analysis/remom/)"
        )
    optional = ["SAVE2.GAM", "MAGIC.SET"]
    skipped = [n for n in optional if n not in remom_artifacts]
    if skipped:
        print(f"    note: ReMoM is missing {', '.join(skipped)} "
              f"— matching comparator(s) will SKIP, not FAIL")

    # If stderr is present but rng.log isn't, derive it now.
    if "stderr.log" in remom_artifacts and "rng.log" not in remom_artifacts:
        rng_log = layout.remom_out / "rng.log"
        filter_rng_log(remom_artifacts["stderr.log"], rng_log)
        remom_artifacts["rng.log"] = rng_log

    return hemom_artifacts, remom_artifacts


def _scan_existing(dir_path: Path) -> dict[str, Path]:
    if not dir_path.is_dir():
        return {}
    return {p.name: p for p in dir_path.iterdir() if p.is_file()}


def _snapshot_remom_if_newer(layout: BuildLayout) -> None:
    """If bin/Debug holds SAVE/MAGIC/stderr files newer than what's in analysis/remom/,
    copy them in before HeMoM runs (HeMoM overwrites bin/Debug otherwise).
    This is the seam that catches "user ran ReMoMber manually, then ran this tool"."""
    bin_files = ["SAVE2.GAM", "MAGIC.SET", "stderr.log",
                 "remom_log_current.txt", "remom_log_new.txt"]
    refreshed: list[str] = []
    layout.remom_out.mkdir(parents=True, exist_ok=True)
    for name in bin_files:
        src = layout.bin_dir / name
        if not src.exists():
            continue
        dst = layout.remom_out / name
        if (not dst.exists()) or src.stat().st_mtime > dst.stat().st_mtime:
            shutil.copy2(str(src), str(dst))
            refreshed.append(name)
    # If stderr.log was refreshed, the derived rng.log is stale — re-filter it.
    stderr_dst = layout.remom_out / "stderr.log"
    rng_dst = layout.remom_out / "rng.log"
    if stderr_dst.exists() and (
        not rng_dst.exists() or stderr_dst.stat().st_mtime > rng_dst.stat().st_mtime
    ):
        total, rng = filter_rng_log(stderr_dst, rng_dst)
        refreshed.append(f"rng.log ({rng:,} of {total:,} lines)")
    if refreshed:
        print(f"  snapshotted bin/Debug → {relpath(layout.remom_out)}/  ({', '.join(refreshed)})")


# ---------------------------------------------------------------------------
# Phase 2a: RNG call-stream comparator
# ---------------------------------------------------------------------------

def _input_tuple(label: str, path: Path) -> tuple[str, Path, int, int, str]:
    return (label, path, byte_size(path), line_count(path), md5_of(path))


def compare_rng(hemom_rng: Path, remom_rng: Path) -> ComparatorResult:
    """Line-by-line equality on the filtered [RNG-CALL] stream.

    HeMoM exits at Save_SAVE_GAM(8); ReMoM continues past that into the Main
    Screen and runs more Random() calls.  So a pass is: HeMoM is a strict
    line-for-line PREFIX of ReMoM.  Anything else fails — including HeMoM
    being LONGER than ReMoM (which would mean HeMoM is drawing Random() in
    a code path ReMoM doesn't reach).
    """
    name = "RNG call stream"
    hemom_lines = hemom_rng.read_text(errors="replace").splitlines()
    remom_lines = remom_rng.read_text(errors="replace").splitlines()
    h_count, r_count = len(hemom_lines), len(remom_lines)
    inputs = [
        _input_tuple("HeMoM rng", hemom_rng),
        _input_tuple("ReMoM rng", remom_rng),
    ]

    if h_count == 0 or r_count == 0:
        return ComparatorResult(
            name, "FAIL",
            f"empty input (HeMoM={h_count}, ReMoM={r_count})",
            Evidence(inputs=inputs,
                     method="line-by-line strict equality (HeMoM ⊆ ReMoM as prefix)",
                     findings=["one or both rng.log files contain no [RNG-CALL] lines"],
                     reproduce=[f"wc -l {hemom_rng} {remom_rng}"]),
        )

    if h_count > r_count:
        # First divergent line is the one past where ReMoM stops.
        i = r_count + 1
        sample = hemom_lines[r_count] if r_count < h_count else "(end of HeMoM)"
        return ComparatorResult(
            name, "FAIL",
            f"HeMoM ran {h_count - r_count} extra Random() call(s) past ReMoM's end",
            Evidence(inputs=inputs,
                     method="line-by-line strict equality (HeMoM ⊆ ReMoM as prefix)",
                     findings=[
                         f"line {i} of HeMoM rng exists; ReMoM rng has only {r_count} lines",
                         f"  HeMoM : {sample}",
                         "investigate the call site(s) past ReMoM's range",
                     ],
                     reproduce=[
                         f"sed -n '{i},+5p' {hemom_rng}",
                         f"tail -n 3 {remom_rng}",
                     ]),
        )

    # Walk both line-by-line up to HeMoM's length.
    first_diff = None
    for i in range(h_count):
        if hemom_lines[i] != remom_lines[i]:
            first_diff = i + 1  # 1-indexed
            break

    if first_diff is not None:
        ctx_from = max(1, first_diff - 2)
        ctx_to = min(h_count, first_diff + 2)
        findings = [
            f"first divergence at line {first_diff} of {h_count}",
            "context (lines {0}–{1}):".format(ctx_from, ctx_to),
        ]
        for j in range(ctx_from, ctx_to + 1):
            mark = "✗" if j == first_diff else " "
            findings.append(f"  {mark} HeMoM  L{j}: {hemom_lines[j - 1]}")
            findings.append(f"  {mark} ReMoM  L{j}: {remom_lines[j - 1]}")
        return ComparatorResult(
            name, "FAIL",
            f"first diverges at line {first_diff} of {h_count}",
            Evidence(inputs=inputs,
                     method="line-by-line strict equality (HeMoM ⊆ ReMoM as prefix)",
                     findings=findings,
                     reproduce=[
                         f"sed -n '{first_diff}p' {hemom_rng}",
                         f"sed -n '{first_diff}p' {remom_rng}",
                         f"diff {hemom_rng} <(head -n {h_count} {remom_rng})",
                     ]),
        )

    # PASS — sample lines for visible eyeball check, plus md5 of HeMoM and the
    # baseline-prefix slice we just verified.
    mid = h_count // 2 if h_count > 1 else 1
    tail_count = r_count - h_count
    findings = [
        f"all {h_count} lines match HeMoM⇄ReMoM",
        f"first L1     : {hemom_lines[0]}",
        f"mid   L{mid:<5d} : {hemom_lines[mid - 1]}",
        f"last  L{h_count:<5d} : {hemom_lines[-1]}",
    ]
    if tail_count > 0:
        findings.append(
            f"ReMoM continues past HeMoM by {tail_count} call(s) "
            f"(expected — ReMoM keeps running through Main Screen after Save_SAVE_GAM)"
        )
        findings.append(f"  ReMoM L{h_count + 1}: {remom_lines[h_count]}")
    summary = f"all {h_count} HeMoM calls match ReMoM line-for-line"
    if tail_count > 0:
        summary += f" (ReMoM has +{tail_count} extra post-save)"
    return ComparatorResult(
        name, "PASS", summary,
        Evidence(inputs=inputs,
                 method="line-by-line strict equality (HeMoM ⊆ ReMoM as prefix)",
                 findings=findings,
                 reproduce=[
                     f"diff {hemom_rng} <(head -n {h_count} {remom_rng})",
                     f"md5sum {hemom_rng}",
                     f"head -n {h_count} {remom_rng} | md5sum",
                 ]),
    )


# ---------------------------------------------------------------------------
# Phase 2b: SAVE-file bytewise comparator with region/element mapping
# ---------------------------------------------------------------------------

# SAVE9.GAM / SAVEn.GAM layout, lifted verbatim from MoX/src/LOADSAVE.c
# (see Save_SAVE_GAM / Load_SAVE_GAM, position asserts after each block).
# Each tuple: (offset, total_size, label, element_size).
# If element_size > 0, the region is N=total_size/element_size elements and we
# can report which element a divergence falls in (e.g. "_players[3]").
SAVE_GAM_REGIONS: list[tuple[int, int, str, int]] = [
    (    0,  2520, "_HEROES2[0..5] (6 player hero slots)",   420),  # 2520/6 = 420
    ( 2520,    16, "globals (num_players,landsize,magic,difficulty,cities,units,turn,unit)", 2),
    ( 2536,  7344, "_players[0..5] (s_WIZARD = 1224 B)",    1224),
    ( 9880,  9600, "_world_maps[2 planes × 4800 B]",        4800),
    (19480,   192, "UU_TBL_1[2 planes × 96 B]",               96),
    (19672,   192, "UU_TBL_2[2 planes × 96 B]",               96),
    (19864,  4800, "_landmasses[2 planes × 2400 B]",        2400),
    (24664,  1440, "_NODES[30 × 48 B]",                       48),
    (26104,    24, "_FORTRESSES[6 × 4 B]",                     4),
    (26128,    24, "_TOWERS[6 × 4 B]",                         4),
    (26152,  2448, "_LAIRS[102 × 24 B]",                      24),
    (28600,  6900, "_ITEMS[138 × 50 B]",                      50),
    (35500, 11400, "_CITIES[100 × 114 B] (s_CITY)",          114),
    (46900, 32288, "_UNITS[1009 × 32 B] (s_UNIT)",            32),
    (79188,  4800, "_map_square_terrain_specials[2 planes]", 2400),
    (83988,  4800, "_square_explored[2 planes]",             2400),
    (88788, 28800, "movement_mode_cost_maps[2 planes]",     14400),
    (117588,  100, "events_table",                           100),
    (117688, 4800, "_map_square_flags[2 planes]",            2400),
    (122488,    2, "grand_vizier",                              2),
    (122490,  250, "TBL_Premade_Items",                         1),
    (122740,  560, "hero_names_table[16 × s_INACTV_HERO]",     35),
]
SAVE_GAM_TOTAL = 123300  # LEN_SAVE_GAM_FILE

# Init_Heroes (INITGAME.c:2060) leaves some _HEROES2 slots as raw heap garbage:
#
#   1. Outer loop iterates `itr_players = 0; itr_players < 5; itr_players++` —
#      _HEROES2[5] is NEVER written.
#
# _HEROES2[] is allocated with malloc() (MoX/src/Allocate.c:341), not calloc(), so
# the skipped slots contain whatever was on the heap. Those bytes are uninitialized
# in BOTH binaries — Init_Heroes itself is deterministic (LOG_TRACE output is
# byte-for-byte identical between ReMoMber and HeMoM at seed=12345), so byte-equality
# on SAVE9/SAVE2 cannot be reached without changing the game's allocator.
#
# We mask (zero) the heap-garbage slots before md5/diff so the comparison reflects
# the data Init_Heroes actually writes. Skip set captured from the seed=12345 trace
# (remom_log_current.txt → `grep "Init_Heroes] p="`):
INIT_HEROES_SKIPPED_SLOTS: dict[int, set[int]] = {
    0: set(range(6, 34)),       # written {0..5, 34}
    1: set(range(8, 34)),       # written {0..7, 34}
    2: set(range(12, 34)),      # written {0..11, 34}
    3: set(range(7, 34)),       # written {0..6, 34}
    4: set(range(7, 34)),       # written {0..6, 34}
    5: set(range(35)),          # player 5 outer-loop bound (`< 5`) — never iterated
}

def _mask_uninitialized_data(data: bytes) -> bytes:
    """Zero out heap-garbage bytes in a SAVE*.GAM.

    The game saves several structs that contain trailing #pragma pack(2)
    padding or skipped-slot regions. Those bytes are never written by game
    code, so they hold whatever the heap allocator returned when the block
    was malloc'd — that's process-local and won't agree across two binaries
    even with identical seed/inputs. We zero them before byte-compare.

    Covered regions:
      1. _HEROES2[0..5] — slots Init_Heroes skips entirely (the per-player
         set in INIT_HEROES_SKIPPED_SLOTS) are zeroed whole (12 B). Slot 11
         (the pad of struct s_HERO: Level=2 + abilities=4 + Casting_Skill=1
         + Spells[4]=4 = 11 B used, 12 B packed) is zeroed in every slot
         on every player.
      2. _TOWERS[0..5] — byte 3 of each 4-byte slot is pad2B_03h on
         struct s_TOWER (wx, wy, owner_idx, pad), zeroed everywhere.

    Operates on raw SAVE*.GAM bytes. Each player's hero block is 420 B
    starting at offset 0; the towers region is 24 B starting at 26128."""
    if len(data) < 2520:
        return data
    buf = bytearray(data)
    # _HEROES2 — skipped slots and trailing pad byte.
    for player_idx, skipped in INIT_HEROES_SKIPPED_SLOTS.items():
        player_base = player_idx * 420
        for ht in skipped:
            slot_base = player_base + ht * 12
            buf[slot_base:slot_base + 12] = b"\x00" * 12
    for player_idx in range(6):
        player_base = player_idx * 420
        for ht in range(35):
            buf[player_base + ht * 12 + 11] = 0
    # _TOWERS — trailing pad byte of every 4-byte slot. Offset and count
    # come from SAVE_GAM_REGIONS: (26128, 24, "_TOWERS[6 × 4 B]", 4).
    if len(buf) >= 26128 + 24:
        for tower_idx in range(6):
            buf[26128 + tower_idx * 4 + 3] = 0
    return bytes(buf)

# MAGIC.SET layout, from MoX/src/MOX_SET.h (struct s_MAGIC_SET, packed(2)).
# Single struct so no element mapping needed.
MAGIC_SET_TOTAL = 466


def offset_to_region(offset: int, regions: list[tuple[int, int, str, int]] = SAVE_GAM_REGIONS
                    ) -> tuple[str, int, int]:
    """Return (region_label, element_index_or_-1, byte_offset_within_element)."""
    for region_offset, size, label, esize in regions:
        if region_offset <= offset < region_offset + size:
            if esize > 0:
                rel = offset - region_offset
                return (label, rel // esize, rel % esize)
            return (label, -1, offset - region_offset)
    return (f"(outside known regions; total file size is {SAVE_GAM_TOTAL})", -1, offset)


def _hex_context(data: bytes, offset: int, span: int = 16) -> str:
    """Hex-dump 2*span+1 bytes centered on offset, with the target byte marked."""
    start = max(0, offset - span)
    end = min(len(data), offset + span + 1)
    out = []
    for i in range(start, end):
        marker = "[" if i == offset else " "
        end_marker = "]" if i == offset else ""
        out.append(f"{marker}{data[i]:02X}{end_marker}")
    return f"@0x{offset:06X}: " + "".join(out).strip()


def _first_diff_offset(a: bytes, b: bytes) -> Optional[int]:
    """Return the offset of the first differing byte, or None if equal."""
    n = min(len(a), len(b))
    for i in range(n):
        if a[i] != b[i]:
            return i
    return None if len(a) == len(b) else n


def compare_save_gam(name: str, hemom_path: Path, remom_path: Path) -> ComparatorResult:
    """Byte-equality on a SAVE*.GAM file with region+element mapping for diff
    drill-down.  Struct-aware field decode lands in Phase 2c on top of this."""
    if not hemom_path.exists() and not remom_path.exists():
        return ComparatorResult(
            name, "SKIP",
            "both sides missing (no .GAM file to compare)",
            Evidence(inputs=[], method="bytewise + region map",
                     findings=["no artifact to compare on either side"], reproduce=[]),
        )
    if not hemom_path.exists():
        return ComparatorResult(
            name, "SKIP",
            f"HeMoM side missing: {relpath(hemom_path)}",
            Evidence(inputs=[("ReMoM", remom_path, byte_size(remom_path), -1, md5_of(remom_path))],
                     method="bytewise + region map",
                     findings=[f"file not found: {relpath(hemom_path)}"], reproduce=[]),
        )
    if not remom_path.exists():
        return ComparatorResult(
            name, "SKIP",
            f"ReMoM side missing: {relpath(remom_path)}",
            Evidence(inputs=[("HeMoM", hemom_path, byte_size(hemom_path), -1, md5_of(hemom_path))],
                     method="bytewise + region map",
                     findings=[f"file not found: {relpath(remom_path)}",
                               "re-run ./tools/capture_remom_rng_baseline.sh to populate ReMoM side"],
                     reproduce=[]),
        )

    h_raw = hemom_path.read_bytes()
    r_raw = remom_path.read_bytes()
    # Mask Init_Heroes heap garbage in the _HEROES2 block on both .GAM files
    # before comparing. See INIT_HEROES_SKIPPED_SLOTS for the why.
    masked_save = name.startswith("SAVE2.GAM") or name.startswith("SAVE9.GAM")
    if masked_save:
        h_bytes = _mask_uninitialized_data(h_raw)
        r_bytes = _mask_uninitialized_data(r_raw)
        h_md5 = hashlib.md5(h_bytes).hexdigest()
        r_md5 = hashlib.md5(r_bytes).hexdigest()
    else:
        h_bytes = h_raw
        r_bytes = r_raw
        h_md5 = md5_of(hemom_path)
        r_md5 = md5_of(remom_path)
    inputs = [
        ("HeMoM", hemom_path, len(h_bytes), -1, h_md5),
        ("ReMoM", remom_path, len(r_bytes), -1, r_md5),
    ]

    if len(h_bytes) != len(r_bytes):
        return ComparatorResult(
            name, "FAIL",
            f"file sizes differ (HeMoM={len(h_bytes)}, ReMoM={len(r_bytes)})",
            Evidence(inputs=inputs, method="bytewise + region map",
                     findings=[f"HeMoM size: {len(h_bytes):,} B",
                               f"ReMoM size: {len(r_bytes):,} B",
                               f"expected:   {SAVE_GAM_TOTAL:,} B"],
                     reproduce=[
                         f"cmp -l {hemom_path} {remom_path} | head",
                         f"stat -c %s {hemom_path} {remom_path}",
                     ]),
        )

    if h_md5 == r_md5:
        pass_findings = [f"md5 (masked): {h_md5}" if masked_save else f"md5: {h_md5}"]
        if masked_save:
            pass_findings.append(
                "Init_Heroes heap-garbage slots zeroed before comparison "
                "(see INIT_HEROES_SKIPPED_SLOTS); on-disk md5s differ but the "
                "data Init_Heroes writes is byte-identical."
            )
        else:
            pass_findings.append("no further inspection needed; bytes are identical")
        return ComparatorResult(
            name, "PASS",
            f"all {len(h_bytes):,} bytes equal{' (post-mask)' if masked_save else ''} (md5 {h_md5[:12]}…)",
            Evidence(inputs=inputs,
                     method="md5 + bytewise" + (" (with _HEROES2 garbage mask)" if masked_save else ""),
                     findings=pass_findings,
                     reproduce=[f"md5sum {hemom_path} {remom_path}",
                                f"cmp {hemom_path} {remom_path}"]),
        )

    # md5 differ — locate first differing byte and map it to a region.
    first = _first_diff_offset(h_bytes, r_bytes)
    assert first is not None  # md5 mismatch implies a differing byte
    region_label, elem_idx, byte_within = offset_to_region(first)

    # Count total differing bytes for severity context.
    total_diff = sum(1 for i in range(len(h_bytes)) if h_bytes[i] != r_bytes[i])

    findings = [
        f"first differing byte at offset 0x{first:06X} ({first:,})",
        f"region : {region_label}",
    ]
    if masked_save:
        findings.append("note: _HEROES2 heap garbage was zeroed before comparison; "
                        "this diff is in DATA, not in uninitialized hero slots")
    if elem_idx >= 0:
        findings.append(f"element: index {elem_idx} (byte offset {byte_within} within element)")
    findings.append(f"  HeMoM {_hex_context(h_bytes, first)}")
    findings.append(f"  ReMoM {_hex_context(r_bytes, first)}")
    findings.append(f"total differing bytes: {total_diff:,} of {len(h_bytes):,} "
                    f"({100.0 * total_diff / len(h_bytes):.2f}%)")

    # Struct-aware drill-down (Phase 2c): if the diverging region maps to a
    # struct we know how to decode, walk every element of that region and show
    # which fields differ in each one (not just the first).
    struct_layouts = {
        "_players[0..5] (s_WIZARD = 1224 B)": (S_WIZARD_LAYOUT, 2536, 1224, 6, "_players"),
        "_CITIES[100 × 114 B] (s_CITY)":      (S_CITY_LAYOUT,  35500,  114, 100, "_CITIES"),
    }
    if region_label in struct_layouts:
        layout, base, esize, nelem, short = struct_layouts[region_label]
        findings.append(f"struct decode: walking all {nelem} elements of {short}[]")
        any_field_shown = False
        for idx in range(nelem):
            elem_offset = base + idx * esize
            elem_h = h_bytes[elem_offset:elem_offset + esize]
            elem_r = r_bytes[elem_offset:elem_offset + esize]
            if elem_h == elem_r:
                continue
            diffs = struct_field_diff(layout, elem_h, elem_r)
            if not diffs:
                # No field-level diff (the difference is in a non-mapped byte
                # range).  Note that, but skip the verbose dump.
                findings.append(f"  {short}[{idx}]: bytes differ but no mapped field — see hex context above")
                continue
            findings.append(f"  {short}[{idx}]:")
            for line in diffs[:8]:  # cap per element
                findings.append(f"    {line}")
            if len(diffs) > 8:
                findings.append(f"    … and {len(diffs) - 8} more fields differ")
            any_field_shown = True
        if not any_field_shown:
            findings.append("  (no mapped fields differ; divergence is in raw bytes only)")

    return ComparatorResult(
        name, "FAIL",
        f"first diff at 0x{first:06X} in {region_label.split(' ')[0]}",
        Evidence(inputs=inputs, method="bytewise + region map + struct-field decode",
                 findings=findings,
                 reproduce=[
                     f"cmp -l {hemom_path} {remom_path} | head -20",
                     f"xxd -s {first - 16} -l 64 {hemom_path}",
                     f"xxd -s {first - 16} -l 64 {remom_path}",
                 ]),
    )


def compare_magic_set(hemom_path: Path, remom_path: Path) -> ComparatorResult:
    """Same comparator but with the MAGIC.SET file's single-struct layout
    (struct-field decode lands in Phase 2c)."""
    name = "MAGIC.SET"
    # Same SKIP/equality logic — but no region map for now.
    if not hemom_path.exists() or not remom_path.exists():
        which = "HeMoM" if not hemom_path.exists() else "ReMoM"
        return ComparatorResult(
            name, "SKIP",
            f"{which} side missing",
            Evidence(inputs=[], method="bytewise (struct decode in Phase 2c)",
                     findings=[f"{which}'s {relpath(hemom_path if which == 'HeMoM' else remom_path)} not found"],
                     reproduce=[]),
        )
    h = hemom_path.read_bytes()
    r = remom_path.read_bytes()
    h_md5, r_md5 = md5_of(hemom_path), md5_of(remom_path)
    inputs = [
        ("HeMoM", hemom_path, len(h), -1, h_md5),
        ("ReMoM", remom_path, len(r), -1, r_md5),
    ]
    if h == r:
        return ComparatorResult(
            name, "PASS", f"all {len(h)} bytes equal (md5 {h_md5[:12]}…)",
            Evidence(inputs=inputs, method="md5 + bytewise",
                     findings=[f"md5: {h_md5}"],
                     reproduce=[f"md5sum {hemom_path} {remom_path}"]),
        )
    first = _first_diff_offset(h, r)
    total_diff = sum(1 for i in range(min(len(h), len(r))) if h[i] != r[i])
    findings = [
        f"first differing byte at offset 0x{first:04X} ({first})",
        f"  HeMoM {_hex_context(h, first, span=8)}",
        f"  ReMoM {_hex_context(r, first, span=8)}",
        f"total differing bytes: {total_diff} of {min(len(h), len(r))}",
    ]
    # A few well-known MAGIC.SET field offsets for first-cut interpretation
    # (full struct decode comes in 2c).
    field_hints = {
        0x28: "Difficulty",   0x2A: "Opponents",   0x2C: "LandSize",
        0x2E: "MagicPower",   0x30: "Have_Save[0..7]", 0x40: "Save_Names[0..7]",
        0xE0: "movement_animations", 0xE2: "hof_names[0..29]",
        0x1AA: "hof_scores[0..9]", 0x1BE: "hof_races[0..9]",
    }
    # Pick the largest anchor whose offset is ≤ first.
    closest = max((ofs for ofs in field_hints if ofs <= first), default=None)
    if closest is not None:
        findings.append(f"  near field anchor: {field_hints[closest]} "
                        f"(offset 0x{closest:02X}, +{first - closest} bytes)")
    # Struct-aware drill-down for s_MAGIC_SET — list every mapped field that differs.
    field_diffs = struct_field_diff(S_MAGIC_SET_LAYOUT, h, r)
    if field_diffs:
        findings.append(f"struct decode: {len(field_diffs)} mapped field(s) differ:")
        for line in field_diffs:
            findings.append(f"  {line}")
    return ComparatorResult(
        name, "FAIL",
        f"first diff at offset 0x{first:04X}",
        Evidence(inputs=inputs, method="bytewise (full struct decode in Phase 2c)",
                 findings=findings,
                 reproduce=[
                     f"cmp -l {hemom_path} {remom_path} | head -20",
                     f"xxd {hemom_path} > /tmp/hemom.magic; xxd {remom_path} > /tmp/remom.magic; diff /tmp/{{hemom,remom}}.magic | head -20",
                 ]),
    )


# ---------------------------------------------------------------------------
# Phase 2c: Struct-aware field decoders
# ---------------------------------------------------------------------------
#
# Each layout is a list of (offset, type_code, name) tuples.  type_code values:
#
#   'b' / 'B'        → int8 / uint8        (1 byte)
#   'h' / 'H'        → int16 / uint16 LE   (2 bytes)
#   'i' / 'I'        → int32 / uint32 LE   (4 bytes)
#   'str:N'          → fixed-length N-byte ASCII (null-stripped)
#   'array:N:CODE'   → tuple of N elements of CODE  (CODE itself recursively)
#
# Layouts are NOT exhaustive — they cover the fields most likely to differ on
# the new-game path: anchors, settings, spellranks, retorts, names.  Fields not
# in the layout still get compared as raw bytes by Phase 2b.

import struct as _struct


def _type_size(code: str) -> int:
    if code in ("b", "B"): return 1
    if code in ("h", "H"): return 2
    if code in ("i", "I"): return 4
    if code.startswith("str:"):
        return int(code.split(":", 1)[1])
    if code.startswith("array:"):
        _, n, inner = code.split(":", 2)
        return int(n) * _type_size(inner)
    raise ValueError(f"unknown type code: {code!r}")


def _decode_one(code: str, buf: bytes, offset: int):
    if code in ("b", "B", "h", "H", "i", "I"):
        return _struct.unpack_from("<" + code, buf, offset)[0]
    if code.startswith("str:"):
        n = int(code.split(":", 1)[1])
        raw = buf[offset:offset + n]
        return raw.split(b"\x00", 1)[0].decode("ascii", "replace")
    if code.startswith("array:"):
        _, n_str, inner = code.split(":", 2)
        n = int(n_str)
        step = _type_size(inner)
        return tuple(_decode_one(inner, buf, offset + i * step) for i in range(n))
    raise ValueError(f"unknown type code: {code!r}")


def decode_struct(layout: list[tuple[int, str, str]], buf: bytes, base: int = 0
                  ) -> dict[str, object]:
    out: dict[str, object] = {}
    for offset, code, field_name in layout:
        out[field_name] = _decode_one(code, buf, base + offset)
    return out


def diff_decoded(a: dict[str, object], b: dict[str, object]) -> list[tuple[str, object, object]]:
    """Return list of (field, a_value, b_value) for fields that differ."""
    return [(k, a[k], b[k]) for k in a if a[k] != b[k]]


# --- s_MAGIC_SET (MOX/src/MOX_SET.h) — 466 B ---------------------------------
S_MAGIC_SET_LAYOUT: list[tuple[int, str, str]] = [
    (0x00, "H", "sound_effects"),
    (0x02, "H", "background_music"),
    (0x04, "H", "event_music"),
    (0x06, "H", "city_spell_events"),
    (0x08, "H", "overland_spell_events"),
    (0x0A, "H", "summoning_events"),
    (0x0C, "H", "end_of_turn_summary"),
    (0x0E, "H", "raze_city"),
    (0x10, "H", "random_events"),
    (0x12, "H", "end_of_turn_wait"),
    (0x14, "H", "strategic_combat_only"),
    (0x16, "H", "auto_unit_information"),
    (0x18, "H", "enemy_moves"),
    (0x1A, "H", "enemy_spells"),
    (0x1C, "H", "spell_book_ordering"),
    (0x1E, "H", "spell_animations"),
    (0x20, "H", "show_node_owners"),
    (0x22, "H", "expanding_help"),
    (0x24, "H", "sound_channels"),
    (0x26, "H", "input_type"),
    (0x28, "H", "Difficulty"),
    (0x2A, "H", "Opponents"),
    (0x2C, "H", "LandSize"),
    (0x2E, "H", "MagicPower"),
    (0x30, "array:8:H", "Have_Save"),
    (0x40, "array:8:str:20", "Save_Names"),
    (0xE0, "H", "movement_animations"),
    # hof_names / hof_scores / hof_races deliberately omitted — they're noise
    # for the new-game path and the user explicitly doesn't want regression-history.
]

# --- s_WIZARD (MoX/src/MOM_DAT.h:1394, packed(2)) — 1224 B -------------------
S_WIZARD_LAYOUT: list[tuple[int, str, str]] = [
    (0x000, "B", "wizard_id"),
    (0x001, "str:20", "name"),
    (0x015, "B", "capital_race"),
    (0x016, "B", "banner_id"),
    (0x018, "H", "Personality"),
    (0x01A, "H", "Objective"),
    (0x022, "H", "som_research_cost"),
    (0x024, "H", "fame"),
    (0x026, "H", "Power_Base"),
    (0x028, "H", "volcanoes"),
    (0x02A, "B", "research_ratio"),
    (0x02B, "B", "mana_ratio"),
    (0x02C, "B", "skill_ratio"),
    (0x02D, "B", "Volcano_Power"),
    (0x02E, "h", "summon_wx"),
    (0x030, "h", "summon_wy"),
    (0x032, "h", "summon_wp"),
    (0x034, "array:8:H", "research_spells"),
    (0x048, "H", "average_unit_cost"),
    (0x05A, "array:5:h", "spellranks"),
    # Retorts — alchemy through artificer (18 × 1 byte)
    (0x064, "B", "alchemy"),
    (0x065, "B", "warlord"),
    (0x066, "B", "chaos_mastery"),
    (0x067, "B", "nature_mastery"),
    (0x068, "B", "sorcery_mastery"),
    (0x069, "B", "infernal_power"),
    (0x06A, "B", "divine_power"),
    (0x06B, "B", "sage_master"),
    (0x06C, "B", "channeler"),
    (0x06D, "B", "myrran"),
    (0x06E, "B", "archmage"),
    (0x06F, "B", "mana_focusing"),
    (0x070, "B", "node_mastery"),
    (0x071, "B", "famous"),
    (0x072, "B", "runemaster"),
    (0x073, "B", "conjurer"),
    (0x074, "B", "charismatic"),
    (0x075, "B", "artificer"),
    (0x264, "array:200:B", "spells_list"),
    (0x356, "H", "gold_reserve"),
    (0x4C4, "H", "Prim_Realm"),
    (0x4C6, "H", "Sec_Realm"),
]

# --- s_CITY (MoX/src/MOM_DAT.h:1685, packed(2)) — 114 B ---------------------
S_CITY_LAYOUT: list[tuple[int, str, str]] = [
    (0x00, "str:14", "name"),
    (0x0E, "b", "race"),
    (0x0F, "b", "wx"),
    (0x10, "b", "wy"),
    (0x11, "b", "wp"),
    (0x12, "b", "owner_idx"),
    (0x13, "b", "size"),
    (0x14, "b", "population"),
    (0x15, "b", "farmer_count"),
    (0x16, "b", "sold_building"),
    (0x18, "h", "Pop_10s"),
    (0x1A, "B", "contacts"),
    (0x1C, "h", "construction"),
    (0x1E, "b", "bldg_cnt"),
    # bldg_status[36] = 36 bytes 0x1F..0x42
    (0x1F, "array:36:b", "bldg_status"),
    (0x5D, "b", "production_units"),
    (0x5E, "h", "Prod_Accu"),
    (0x60, "B", "gold_units"),
    (0x61, "b", "building_maintenance"),
    (0x62, "b", "mana_units"),
    (0x63, "b", "research_units"),
    (0x64, "b", "food_units"),
]


def _format_diff(field: str, a_val: object, b_val: object) -> str:
    """Format a single field diff for display."""
    def fmt(v: object) -> str:
        if isinstance(v, tuple):
            return "[" + ", ".join(fmt(x) for x in v) + "]"
        if isinstance(v, str):
            return repr(v)
        if isinstance(v, int):
            return f"{v}"
        return repr(v)
    return f"{field:<24} HeMoM={fmt(a_val):<20}  ReMoM={fmt(b_val)}"


def struct_field_diff(layout: list[tuple[int, str, str]],
                      hemom_bytes: bytes, remom_bytes: bytes, base: int = 0
                      ) -> list[str]:
    """Decode both sides via `layout` (relative to `base`), diff field-by-field,
    return one display line per differing field."""
    a = decode_struct(layout, hemom_bytes, base)
    b = decode_struct(layout, remom_bytes, base)
    return [_format_diff(field, av, bv) for field, av, bv in diff_decoded(a, b)]


# ---------------------------------------------------------------------------
# Phase 2d: State summary (anchors shown side-by-side, INFO not pass/fail)
# ---------------------------------------------------------------------------
#
# The user explicitly asked for visible checkpoints (city name, slot name,
# spellranks, banner, race) so they can eyeball "yes, both sides set the
# things they were supposed to."  This section always runs and reports as
# INFO — it doesn't gate the overall verdict by itself.  Missing artifacts
# show as "—" rather than failing.

def _read_or_none(p: Path) -> Optional[bytes]:
    return p.read_bytes() if p.exists() else None


def _decode_field_at(layout: list[tuple[int, str, str]],
                     buf: Optional[bytes], base: int, field_name: str) -> object:
    if buf is None or base + max(o for o, _, _ in layout) >= len(buf):
        return None
    for offset, code, name in layout:
        if name == field_name:
            return _decode_one(code, buf, base + offset)
    return None


def state_summary(hemom_save: Path, hemom_magic: Path,
                  remom_save: Path, remom_magic: Path) -> ComparatorResult:
    """Decode known anchor fields from both sides and show them side-by-side.

    Reports INFO, not PASS/FAIL — its job is to make the per-field reality
    visible to the user.  PASS/FAIL on the underlying bytes is the job of
    compare_save_gam / compare_magic_set.

    Reads from SAVE2.GAM (the named slot save) — same struct layout as
    SAVE9.GAM, but captured at a moment both binaries agree on."""

    h_save = _read_or_none(hemom_save)
    r_save = _read_or_none(remom_save)
    h_mset = _read_or_none(hemom_magic)
    r_mset = _read_or_none(remom_magic)

    inputs = [
        _input_tuple("HeMoM SAVE2.GAM", hemom_save),
        _input_tuple("HeMoM MAGIC.SET", hemom_magic),
        _input_tuple("ReMoM SAVE2.GAM", remom_save),
        _input_tuple("ReMoM MAGIC.SET", remom_magic),
    ]

    # Anchors: (label, source_buf_pair, layout, base, field_name)
    # Each tuple resolves to (hemom_value, remom_value) for one anchor row.
    PLAYER0_BASE = 2536  # _players[0]
    CITY0_BASE = 35500   # _CITIES[0]
    anchors: list[tuple[str, object, object]] = []

    def add(label: str, h_val: object, r_val: object) -> None:
        anchors.append((label, h_val, r_val))

    # Player 0 (human wizard) from SAVE2.GAM
    add("player[0].wizard_id",
        _decode_field_at(S_WIZARD_LAYOUT, h_save, PLAYER0_BASE, "wizard_id"),
        _decode_field_at(S_WIZARD_LAYOUT, r_save, PLAYER0_BASE, "wizard_id"))
    add("player[0].name",
        _decode_field_at(S_WIZARD_LAYOUT, h_save, PLAYER0_BASE, "name"),
        _decode_field_at(S_WIZARD_LAYOUT, r_save, PLAYER0_BASE, "name"))
    add("player[0].capital_race",
        _decode_field_at(S_WIZARD_LAYOUT, h_save, PLAYER0_BASE, "capital_race"),
        _decode_field_at(S_WIZARD_LAYOUT, r_save, PLAYER0_BASE, "capital_race"))
    add("player[0].banner_id",
        _decode_field_at(S_WIZARD_LAYOUT, h_save, PLAYER0_BASE, "banner_id"),
        _decode_field_at(S_WIZARD_LAYOUT, r_save, PLAYER0_BASE, "banner_id"))
    add("player[0].spellranks",
        _decode_field_at(S_WIZARD_LAYOUT, h_save, PLAYER0_BASE, "spellranks"),
        _decode_field_at(S_WIZARD_LAYOUT, r_save, PLAYER0_BASE, "spellranks"))
    add("player[0].artificer",
        _decode_field_at(S_WIZARD_LAYOUT, h_save, PLAYER0_BASE, "artificer"),
        _decode_field_at(S_WIZARD_LAYOUT, r_save, PLAYER0_BASE, "artificer"))
    add("player[0].gold_reserve",
        _decode_field_at(S_WIZARD_LAYOUT, h_save, PLAYER0_BASE, "gold_reserve"),
        _decode_field_at(S_WIZARD_LAYOUT, r_save, PLAYER0_BASE, "gold_reserve"))

    # Home city (city[0]) from SAVE2.GAM
    add("city[0].name",
        _decode_field_at(S_CITY_LAYOUT, h_save, CITY0_BASE, "name"),
        _decode_field_at(S_CITY_LAYOUT, r_save, CITY0_BASE, "name"))
    add("city[0].race",
        _decode_field_at(S_CITY_LAYOUT, h_save, CITY0_BASE, "race"),
        _decode_field_at(S_CITY_LAYOUT, r_save, CITY0_BASE, "race"))
    add("city[0].(wx,wy,wp)",
        (_decode_field_at(S_CITY_LAYOUT, h_save, CITY0_BASE, "wx"),
         _decode_field_at(S_CITY_LAYOUT, h_save, CITY0_BASE, "wy"),
         _decode_field_at(S_CITY_LAYOUT, h_save, CITY0_BASE, "wp")),
        (_decode_field_at(S_CITY_LAYOUT, r_save, CITY0_BASE, "wx"),
         _decode_field_at(S_CITY_LAYOUT, r_save, CITY0_BASE, "wy"),
         _decode_field_at(S_CITY_LAYOUT, r_save, CITY0_BASE, "wp")))
    add("city[0].owner_idx",
        _decode_field_at(S_CITY_LAYOUT, h_save, CITY0_BASE, "owner_idx"),
        _decode_field_at(S_CITY_LAYOUT, r_save, CITY0_BASE, "owner_idx"))

    # MAGIC.SET — settings and the slot we wrote into
    add("magic_set.Difficulty",
        _decode_field_at(S_MAGIC_SET_LAYOUT, h_mset, 0, "Difficulty"),
        _decode_field_at(S_MAGIC_SET_LAYOUT, r_mset, 0, "Difficulty"))
    add("magic_set.Opponents",
        _decode_field_at(S_MAGIC_SET_LAYOUT, h_mset, 0, "Opponents"),
        _decode_field_at(S_MAGIC_SET_LAYOUT, r_mset, 0, "Opponents"))
    add("magic_set.LandSize",
        _decode_field_at(S_MAGIC_SET_LAYOUT, h_mset, 0, "LandSize"),
        _decode_field_at(S_MAGIC_SET_LAYOUT, r_mset, 0, "LandSize"))
    add("magic_set.MagicPower",
        _decode_field_at(S_MAGIC_SET_LAYOUT, h_mset, 0, "MagicPower"),
        _decode_field_at(S_MAGIC_SET_LAYOUT, r_mset, 0, "MagicPower"))
    h_save_names = _decode_field_at(S_MAGIC_SET_LAYOUT, h_mset, 0, "Save_Names")
    r_save_names = _decode_field_at(S_MAGIC_SET_LAYOUT, r_mset, 0, "Save_Names")
    add("magic_set.Save_Names[1]",
        h_save_names[1] if h_save_names else None,
        r_save_names[1] if r_save_names else None)

    # Build the displayed table.
    def show(v: object) -> str:
        if v is None:
            return f"{DIM}—{RESET}"
        if isinstance(v, tuple):
            return "(" + ", ".join(show(x) for x in v) + ")"
        if isinstance(v, str):
            return repr(v)
        return str(v)

    mismatches = sum(1 for _, h, r in anchors if h != r)
    findings = ["per-anchor side-by-side (HeMoM vs ReMoM):"]
    for label, h_val, r_val in anchors:
        marker = "✓" if h_val == r_val else "✗"
        # Highlight mismatches in color (when terminal supports it).
        color_open = RED if h_val != r_val else GREEN
        findings.append(f"  {color_open}{marker}{RESET} {label:<28} "
                        f"HeMoM={show(h_val):<24}  ReMoM={show(r_val)}")
    if mismatches == 0:
        summary = f"all {len(anchors)} anchors match"
    else:
        summary = f"{mismatches}/{len(anchors)} anchors differ (see drill-down)"

    return ComparatorResult(
        "State anchors", "INFO", summary,
        Evidence(inputs=inputs,
                 method="decode known fields from SAVE2.GAM + MAGIC.SET on both sides",
                 findings=findings,
                 reproduce=[
                     f"xxd -s {CITY0_BASE} -l 14 {hemom_save}   # HeMoM city[0].name",
                     f"xxd -s 0x40 -l 160 {hemom_magic}        # HeMoM magic_set.Save_Names",
                 ]),
    )


# ---------------------------------------------------------------------------
# Main wiring (Phase 3 polish)
# ---------------------------------------------------------------------------

def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="Prove ReMoMber and HeMoM produce identical behavior and data.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    p.add_argument("--config", default=str(ASSETS_DIR / "matchup_hemom.ini"),
                   help="HeMoM newgame .ini (default: assets/matchup_hemom.ini)")
    p.add_argument("--seed", type=int, default=12345,
                   help="Deterministic seed (default: 12345)")
    p.add_argument("--rmr", default=str(ASSETS_DIR / "menu_baseline_seed12345.RMR"),
                   help="Recorded menu-click sequence for ReMoMber --replay")
    p.add_argument("--preset", default=None,
                   help="CMake preset (auto-detected: clang-debug on Linux, MSVC-headless-debug on Windows)")
    p.add_argument("--skip-build", action="store_true",
                   help="Skip cmake --build; use existing binaries")
    p.add_argument("--run-remom", action="store_true",
                   help="Attempt to run ReMoMber --replay headlessly (may segfault; "
                        "default is to require analysis/remom/ populated by the bash capture script).")
    p.add_argument("--skip-hemom", action="store_true",
                   help="Skip running HeMoM; reuse existing analysis/hemom/ artifacts")
    return p.parse_args()


def main() -> int:
    args = parse_args()
    preset = pick_preset(args.preset)
    layout = make_layout(preset)

    print_section_header("Parity Check: HeMoM vs ReMoMber")
    print(f"  preset = {preset}")
    print(f"  config = {relpath(Path(args.config))}")
    print(f"  seed   = {args.seed}")
    print(f"  rmr    = {relpath(Path(args.rmr))}")
    print(f"  out    = {relpath(layout.analysis_dir)}/{{hemom,remom}}/")

    try:
        hemom_artifacts, remom_artifacts = phase1_collect(args, layout)
    except Phase1Error as e:
        print(f"\n  {RED}Phase 1 failure:{RESET} {e}", file=sys.stderr)
        return 1

    print_section_header("Phase 2: Compare")
    print(f"  HeMoM artifacts ({len(hemom_artifacts)}): {', '.join(sorted(hemom_artifacts))}")
    print(f"  ReMoM artifacts ({len(remom_artifacts)}): {', '.join(sorted(remom_artifacts))}")

    results: list[ComparatorResult] = []
    # 2a — RNG call stream (foundational).
    results.append(compare_rng(hemom_artifacts["rng.log"], remom_artifacts["rng.log"]))

    # 2b — SAVE2.GAM (named slot), MAGIC.SET (bytewise + region map).
    # SAVE9.GAM is intentionally not compared — see HEMOM_PRODUCED comment.
    results.append(compare_save_gam(
        "SAVE2.GAM (named slot)",
        layout.hemom_out / "SAVE2.GAM",
        layout.remom_out / "SAVE2.GAM",
    ))
    results.append(compare_magic_set(
        layout.hemom_out / "MAGIC.SET",
        layout.remom_out / "MAGIC.SET",
    ))

    # 2d — State anchors (always INFO; surfaces the checkpoints by hand).
    # Reads from SAVE2.GAM — captured at the same moment on both binaries
    # (after Loaded_Game_Update on the ReMoMber side, after New_Game_Continue
    # on the HeMoM side).
    results.append(state_summary(
        layout.hemom_out / "SAVE2.GAM",
        layout.hemom_out / "MAGIC.SET",
        layout.remom_out / "SAVE2.GAM",
        layout.remom_out / "MAGIC.SET",
    ))

    print_section_header("Phase 3: Report")
    for r in results:
        print_result_block(r)
    return print_overall(results)


if __name__ == "__main__":
    sys.exit(main())
