#!/usr/bin/env python3
"""Bundle a ReMoM bug report: the log files plus system info, into one zip.

Cross-platform (Windows / Linux / macOS), Python 3, standard library only -- no
pip installs.  Optional convenience for testers; ReMoM itself never requires Python.

Usage:
    python3 tools/collect_report.py [LOG_DIR]

With no argument it finds ReMoM's log folder automatically (honoring REMOM_LOG_DIR,
then the per-OS default).  Writes remom_report_<timestamp>.zip in the current folder.
See TESTING.md.
"""

import datetime
import os
import platform
import sys
import zipfile

APP = "ReMoM"
LOG_NAMES = ("remom_log_current.txt", "remom_log_previous.txt", "remom_log_new.txt")


def default_log_dir():
    """Mirror STU_GRAF_User_State_Dir() so this matches where ReMoM actually writes."""
    env = os.environ.get("REMOM_LOG_DIR")
    if env:
        return env
    if sys.platform.startswith("win"):
        base = os.environ.get("LOCALAPPDATA")
        return os.path.join(base, APP, "logs") if base else None
    if sys.platform == "darwin":
        home = os.path.expanduser("~")
        return os.path.join(home, "Library", "Logs", APP)
    # Linux / other: XDG_STATE_HOME or ~/.local/state
    base = os.environ.get("XDG_STATE_HOME") or os.path.join(os.path.expanduser("~"), ".local", "state")
    return os.path.join(base, APP)


def system_info(log_dir):
    return (
        "ReMoM bug-report bundle\n"
        f"collected:    {datetime.datetime.now().isoformat(timespec='seconds')}\n"
        f"os:           {platform.platform()}\n"
        f"machine:      {platform.machine()}\n"
        f"python:       {platform.python_version()}\n"
        f"log_dir:      {log_dir}\n"
        "\nAttach this zip to your report. The ReMoM version and any crash trace\n"
        "([stu-brak] lines) are inside remom_log_current.txt.\n"
    )


def main(argv):
    log_dir = argv[1] if len(argv) > 1 else default_log_dir()
    if not log_dir or not os.path.isdir(log_dir):
        print(f"ERROR: log folder not found: {log_dir!r}", file=sys.stderr)
        print("Run ReMoM_diagnostic.exe at least once, or pass the folder as an argument.", file=sys.stderr)
        return 1

    found = [os.path.join(log_dir, n) for n in LOG_NAMES if os.path.isfile(os.path.join(log_dir, n))]
    # A perf capture (if any) lands next to the exe / in the CWD, not the log dir.
    found += [f for f in ("remom_input_metrics.fwv",) if os.path.isfile(f)]
    if not found:
        print(f"ERROR: no log files in {log_dir}", file=sys.stderr)
        return 1

    stamp = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
    out = f"remom_report_{stamp}.zip"
    with zipfile.ZipFile(out, "w", zipfile.ZIP_DEFLATED) as z:
        for path in found:
            z.write(path, arcname=os.path.basename(path))
        z.writestr("system_info.txt", system_info(log_dir))

    print(f"Wrote {out} ({len(found)} log file(s) + system_info.txt)")
    print(f"  from: {log_dir}")
    print("Attach it to your report -- see TESTING.md.")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
