from __future__ import annotations

import argparse
import pathlib
import re
import subprocess
import sys


ADD_TEST_EXE_PATTERN = re.compile(r"add_test\([^\n]*?\s((?:[A-Za-z]:)?[^\s\)]*?\.exe)\b", re.IGNORECASE)


def collect_ctest_files(build_dir: pathlib.Path) -> list[pathlib.Path]:
    ctest_files = []

    root_testfile = build_dir / "CTestTestfile.cmake"
    if root_testfile.is_file():
        ctest_files.append(root_testfile)

    ctest_files.extend(sorted(build_dir.rglob("*_tests.cmake")))
    ctest_files.extend(sorted(build_dir.rglob("CTestTestfile.cmake")))

    unique_files = []
    seen_paths: set[pathlib.Path] = set()

    for file_path in ctest_files:
        resolved_path = file_path.resolve()
        if resolved_path in seen_paths:
            continue

        seen_paths.add(resolved_path)
        unique_files.append(file_path)

    return unique_files


def discover_test_executables(build_dir: pathlib.Path) -> dict[str, pathlib.Path]:
    executables: dict[str, pathlib.Path] = {}

    for ctest_file in collect_ctest_files(build_dir):
        try:
            content = ctest_file.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            content = ctest_file.read_text(encoding="latin-1")

        for match in ADD_TEST_EXE_PATTERN.finditer(content):
            exe_token = match.group(1).strip().strip('"')
            exe_path = pathlib.Path(exe_token)
            executables[exe_path.name.lower()] = exe_path

    return dict(sorted(executables.items()))


def list_running_pids(executable_name: str) -> list[int]:
    process_name = pathlib.Path(executable_name).stem
    completed = subprocess.run(
        [
            "tasklist",
            "/FI",
            f"IMAGENAME eq {process_name}.exe",
            "/FO",
            "CSV",
            "/NH",
        ],
        capture_output=True,
        text=True,
        check=True,
    )

    pids = []
    for line in completed.stdout.splitlines():
        line = line.strip()
        if not line or line.startswith("INFO:"):
            continue

        parts = [part.strip().strip('"') for part in line.split('","')]
        if len(parts) < 2:
            continue

        image_name = parts[0]
        pid_text = parts[1]

        if image_name.lower() != executable_name.lower():
            continue

        try:
            pids.append(int(pid_text))
        except ValueError:
            continue

    return pids


def terminate_pid(pid: int) -> None:
    subprocess.run(
        ["taskkill", "/PID", str(pid), "/F"],
        capture_output=True,
        text=True,
        check=True,
    )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Terminate stuck test executables discovered from generated CTest config."
    )
    parser.add_argument(
        "--build-dir",
        default="out/build/MSVC-release",
        help="Build directory containing generated CTest files.",
    )
    parser.add_argument(
        "--name",
        action="append",
        default=[],
        help="Executable name to terminate. Can be provided more than once.",
    )
    parser.add_argument(
        "--all",
        action="store_true",
        help="Terminate all running executables discovered from the CTest config.",
    )
    parser.add_argument(
        "--list",
        action="store_true",
        help="List discovered executables and exit without terminating anything.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    build_dir = pathlib.Path(args.build_dir)

    if not build_dir.is_dir():
        print(f"Build directory not found: {build_dir}", file=sys.stderr)
        return 1

    discovered = discover_test_executables(build_dir)
    if not discovered:
        print(f"No test executables found in generated CTest config under: {build_dir}", file=sys.stderr)
        return 1

    discovered_names = [path.name for path in discovered.values()]
    print("Discovered from CTest: " + ", ".join(discovered_names))

    if args.list:
        return 0

    requested_names = [name.strip() for name in args.name if name.strip()]
    if args.all:
        target_names = discovered_names
    else:
        target_names = requested_names

    if not target_names:
        print("Nothing to do. Provide --name or use --all.", file=sys.stderr)
        return 1

    unknown_names = [name for name in target_names if name.lower() not in discovered]
    if unknown_names:
        print("Refusing to terminate names not present in generated CTest config: " + ", ".join(unknown_names), file=sys.stderr)
        return 1

    terminated = []
    not_running = []

    for target_name in target_names:
        pids = list_running_pids(target_name)

        if not pids:
            not_running.append(target_name)
            continue

        for pid in pids:
            terminate_pid(pid)

        terminated.append(f"{target_name} (PIDs: {', '.join(str(pid) for pid in pids)})")

    if terminated:
        print("Terminated: " + ", ".join(terminated))

    if not_running:
        print("Not running: " + ", ".join(not_running))

    return 0


if __name__ == "__main__":
    sys.exit(main())