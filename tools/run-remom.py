#!/usr/bin/env python3
"""run-remom.py — drive ReMoM non-interactively.

Defaults to the matchup workflow:
    --ini assets/matchup_hemom.ini    (seed comes from [Game] unless --seed)
    --hms assets/matchup_hemom.hms    (drives menus → save → quit)
    cwd   bin/Debug/                  (SAVE*.GAM writes land here)
    headless                          (SDL_VIDEODRIVER=offscreen)

Output:
    bin/Debug/SAVE<save_slot+1>.GAM   (verified rewritten this run)
    bin/Debug/stderr.log              (full ReMoM stderr)

Exit:
    0  ReMoM exited 0 AND expected SAVE was rewritten
    2  setup / prerequisite error (missing binary / INI / driver file)
    3  ReMoM exited non-zero OR expected SAVE was not rewritten
"""

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import lib_run as L


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                  formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ini", type=Path,
                    default=L.ASSETS_DIR / "matchup_hemom.ini",
                    help="ReMoM .ini; seed is read from [Game] seed unless --seed overrides")
    drv = ap.add_mutually_exclusive_group()
    drv.add_argument("--hms", type=Path,
                     default=L.ASSETS_DIR / "matchup_hemom.hms",
                     help="HMS scenario file (Artificial Human Player)")
    drv.add_argument("--rmr", type=Path,
                     help="RMR recorded-input file (replay mode)")
    ap.add_argument("--seed", type=int,
                    help="override seed (default: read from --ini [Game] seed)")
    ap.add_argument("--save-slot", type=int, default=1,
                    help="MoM save_slot (0..8) the HMS/RMR writes to; "
                         "used to verify SAVE<N+1>.GAM was rewritten (default: 1)")
    ap.add_argument("--gui", action="store_true",
                    help="run windowed (default: headless via SDL_VIDEODRIVER=offscreen)")
    ap.add_argument("remom_args", nargs="*",
                    help="extra args forwarded to ReMoM after --")
    args = ap.parse_args(argv)

    # --rmr wins if both somehow set; otherwise default --hms applies.
    hms = None if args.rmr else args.hms
    rmr = args.rmr

    try:
        seed = args.seed if args.seed is not None else L.read_seed_from_ini(args.ini)
    except (FileNotFoundError, ValueError) as e:
        print(f"ERROR: {e}", file=sys.stderr)
        return 2

    print(f"  ini       : {args.ini}")
    print(f"  driver    : {'--scenario ' + str(hms) if hms else '--replay ' + str(rmr)}")
    print(f"  seed      : {seed}")
    print(f"  save_slot : {args.save_slot}  (expects {L.slot_to_save_filename(args.save_slot)})")
    print(f"  cwd       : {L.BUILD_DIR}")
    print(f"  headless  : {not args.gui}")
    print()

    try:
        result = L.run_remom(
            seed=seed,
            hms=hms,
            rmr=rmr,
            save_slot=args.save_slot,
            extra_args=args.remom_args,
            headless=not args.gui,
        )
    except FileNotFoundError as e:
        print(f"ERROR: {e}", file=sys.stderr)
        return 2

    duration = result.t_end - result.t_start
    print(f"  exit_status        = {result.exit_status}")
    print(f"  stderr_path        = {result.stderr_path}  "
          f"({result.stderr_path.stat().st_size:,} B)")
    print(f"  expected_save      = {result.expected_save}")
    print(f"  save_written_fresh = {result.save_written_fresh}")
    print(f"  duration           = {duration:.2f}s")

    return 0 if result.success else 3


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
