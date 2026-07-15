"""tools/lib_run.py — Shared launchers for HeMoM and ReMoMber.

Scope (deliberately narrow): run a binary, capture stderr, verify the
expected SAVE*.GAM was rewritten this run. Both binaries accept either
`--scenario HMS` or `--replay RMR` as the input-driver (mutually
exclusive). Headless by default; pass `headless=False` to run windowed.

Out of scope — stays in parity_check.py:
    - snapshotting bin/Debug → analysis/{hemom,remom}/
    - filtering [RNG-CALL] lines out of stderr.log
    - comparison, struct decode, reporting

Public API:
    REPO_ROOT, ASSETS_DIR, BUILD_DIR, HEMOM_EXE, REMOMBER_EXE
    read_seed_from_ini(ini_path) -> int
    slot_to_save_filename(slot)  -> "SAVE<N+1>.GAM"
    RunResult                                          (dataclass)
    run_hemom(*, ini, seed, hms=None, rmr=None, save_slot=None, ...)
    run_remomber(*, seed, hms=None, rmr=None, save_slot=None, ...)
"""

import os
import re
import subprocess
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Optional


# ----------------------------------------------------------------------
# Canonical paths
# ----------------------------------------------------------------------
REPO_ROOT    = Path(__file__).resolve().parent.parent
ASSETS_DIR   = REPO_ROOT / "assets"
BUILD_DIR    = REPO_ROOT / "out/build/clang-debug/bin/Debug"
HEMOM_EXE    = BUILD_DIR / "HeMoM"
REMOMBER_EXE = BUILD_DIR / "ReMoMber"


# ----------------------------------------------------------------------
# INI seed reader
# ----------------------------------------------------------------------
_SECTION_RE = re.compile(r"^\s*\[([^\]]+)\]\s*$")
_SEED_RE    = re.compile(r"^\s*seed\s*=\s*([^\s#;]+)")


def read_seed_from_ini(ini_path: Path) -> int:
    """Return the int value of `seed = N` under `[Game]` in `ini_path`.

    Raises:
        FileNotFoundError: ini_path doesn't exist.
        ValueError: no seed line under [Game], or value isn't an int.
    """
    if not ini_path.is_file():
        raise FileNotFoundError(f"INI not found: {ini_path}")
    in_game = False
    for raw in ini_path.read_text(encoding="utf-8").splitlines():
        sect = _SECTION_RE.match(raw)
        if sect:
            in_game = (sect.group(1).strip().lower() == "game")
            continue
        if not in_game:
            continue
        m = _SEED_RE.match(raw)
        if m:
            return int(m.group(1))
    raise ValueError(f"no `seed = ...` under [Game] in {ini_path}")


# ----------------------------------------------------------------------
# MoM save-slot convention
# ----------------------------------------------------------------------
def slot_to_save_filename(slot: int) -> str:
    """Map MoM save_slot (0-based) to its on-disk SAVE*.GAM filename.

    `save_slot = 0 → SAVE1.GAM`,  `1 → SAVE2.GAM`,  …,  `8 → SAVE9.GAM`.
    """
    if not (0 <= slot <= 8):
        raise ValueError(f"save_slot out of range [0..8]: {slot}")
    return f"SAVE{slot + 1}.GAM"


# ----------------------------------------------------------------------
# Run result
# ----------------------------------------------------------------------
@dataclass
class RunResult:
    """What a launcher reports back to its caller."""
    exit_status: int
    cmd: list[str]                  # for logging / debugging
    cwd: Path
    stderr_path: Path
    expected_save: Optional[Path]   # None ⇒ caller didn't ask to verify a save
    save_written_fresh: bool        # True ⇔ expected_save mtime ≥ run start
    t_start: float                  # for downstream artifact collection (since_ts)
    t_end: float

    @property
    def success(self) -> bool:
        """exit 0 AND (no save expected OR save was rewritten this run)."""
        if self.exit_status != 0:
            return False
        if self.expected_save is None:
            return True
        return self.save_written_fresh


# ----------------------------------------------------------------------
# Save freshness check
# ----------------------------------------------------------------------
def verify_save_fresh(path: Path, since_ts: float) -> bool:
    """True iff `path` exists and was written at-or-after `since_ts`.

    Tolerates up to 1 s of mtime-resolution loss (filesystems that round
    to whole seconds; t_start has sub-second precision)."""
    if not path.is_file():
        return False
    return path.stat().st_mtime >= (since_ts - 1.0)


# ----------------------------------------------------------------------
# Internals
# ----------------------------------------------------------------------
def _driver_args(hms: Optional[Path], rmr: Optional[Path]) -> list[str]:
    """Return CLI args for the input driver. HMS and RMR are mutually
    exclusive — pick one or pass neither (binary runs interactively)."""
    if hms is not None and rmr is not None:
        raise ValueError("hms= and rmr= are mutually exclusive (pick one).")
    if hms is not None:
        return ["--scenario", str(hms)]
    if rmr is not None:
        return ["--replay", str(rmr)]
    return []


def _spawn(cmd: list[str], cwd: Path, stderr_path: Path,
           headless: bool) -> tuple[int, float, float]:
    """subprocess wrapper: capture stderr to file, time the run,
    set SDL_VIDEODRIVER=offscreen iff `headless`."""
    env = os.environ.copy()
    if headless:
        env.setdefault("SDL_VIDEODRIVER", "offscreen")
    stderr_path.parent.mkdir(parents=True, exist_ok=True)
    t_start = time.time()
    with stderr_path.open("wb") as errf:
        proc = subprocess.run(
            cmd,
            cwd=str(cwd),
            stdout=subprocess.DEVNULL,
            stderr=errf,
            env=env,
            check=False,
        )
    t_end = time.time()
    return proc.returncode, t_start, t_end


# ----------------------------------------------------------------------
# Launchers
# ----------------------------------------------------------------------
def run_hemom(*,
              ini: Path,
              seed: int,
              hms: Optional[Path] = None,
              rmr: Optional[Path] = None,
              cwd: Path = BUILD_DIR,
              stderr_path: Optional[Path] = None,
              save_slot: Optional[int] = None,
              extra_args: Iterable[str] = (),
              headless: bool = True) -> RunResult:
    """Drive HeMoM --newgame against `ini`, with an optional input driver.

    Args:
        ini: HeMoM .ini (the [Game] seed is informational only; the actual
            seed used is the `seed=` argument).
        seed: RNG seed; passed as `--seed N`.
        hms / rmr: input driver, mutually exclusive. Pass either, or neither
            (HeMoM --newgame exits after writing the save anyway).
        cwd: working directory for HeMoM (defaults to bin/Debug/; SAVE*.GAM
            files are written here).
        stderr_path: where to capture HeMoM's stderr (defaults to cwd/stderr.log).
        save_slot: if set, the launcher derives the expected SAVE*.GAM path
            and reports whether it was rewritten this run via
            `RunResult.save_written_fresh`.
        extra_args: forwarded to HeMoM verbatim.
        headless: when True, sets SDL_VIDEODRIVER=offscreen.
    """
    if not HEMOM_EXE.is_file():
        raise FileNotFoundError(f"HeMoM binary missing: {HEMOM_EXE}")
    if not ini.is_file():
        raise FileNotFoundError(f"INI not found: {ini}")
    if stderr_path is None:
        stderr_path = cwd / "stderr.log"

    cmd = [
        str(HEMOM_EXE),
        "--newgame", str(ini),
        "--seed1", str(seed),
        *_driver_args(hms, rmr),
        *list(extra_args),
    ]
    expected_save = (cwd / slot_to_save_filename(save_slot)
                     if save_slot is not None else None)

    rc, t_start, t_end = _spawn(cmd, cwd, stderr_path, headless=headless)
    fresh = verify_save_fresh(expected_save, t_start) if expected_save else False

    return RunResult(
        exit_status=rc,
        cmd=cmd,
        cwd=cwd,
        stderr_path=stderr_path,
        expected_save=expected_save,
        save_written_fresh=fresh,
        t_start=t_start,
        t_end=t_end,
    )


def run_remomber(*,
                 seed: int,
                 hms: Optional[Path] = None,
                 rmr: Optional[Path] = None,
                 cwd: Path = BUILD_DIR,
                 stderr_path: Optional[Path] = None,
                 save_slot: Optional[int] = None,
                 extra_args: Iterable[str] = (),
                 headless: bool = True) -> RunResult:
    """Drive ReMoMber with `--seed N` and an input driver.

    ReMoMber needs an HMS or RMR to be useful in automation (without one
    it sits on the title screen waiting for input), so the launcher
    requires at least one of `hms=` or `rmr=`.
    """
    if not REMOMBER_EXE.is_file():
        raise FileNotFoundError(f"ReMoMber binary missing: {REMOMBER_EXE}")
    if hms is None and rmr is None:
        raise ValueError("ReMoMber needs an input driver: pass hms= or rmr=.")
    if stderr_path is None:
        stderr_path = cwd / "stderr.log"

    cmd = [
        str(REMOMBER_EXE),
        "--seed1", str(seed),
        *_driver_args(hms, rmr),
        *list(extra_args),
    ]
    expected_save = (cwd / slot_to_save_filename(save_slot)
                     if save_slot is not None else None)

    rc, t_start, t_end = _spawn(cmd, cwd, stderr_path, headless=headless)
    fresh = verify_save_fresh(expected_save, t_start) if expected_save else False

    return RunResult(
        exit_status=rc,
        cmd=cmd,
        cwd=cwd,
        stderr_path=stderr_path,
        expected_save=expected_save,
        save_written_fresh=fresh,
        t_start=t_start,
        t_end=t_end,
    )
