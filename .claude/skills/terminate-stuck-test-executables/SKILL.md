---
name: terminate-stuck-test-executables
description: Safely terminate stuck test executables that block rebuilds or CTest runs on Windows. Use when a test binary is locked or still running, or when the user says a test executable is stuck, hung, blocking the linker, or needs to be killed. Read the executable list from the generated CTest config and always ask the user for permission before use.
---

# Terminate Stuck Test Executables

Use this skill when a Windows test executable is still running and blocking a rebuild, test discovery, or CTest execution.

## Safety Rules

- Ask the user for permission immediately before using this skill.
- Only target executables discovered from the generated CTest config unless the user explicitly approves a wider action.
- Prefer listing candidate processes first when the environment allows it.
- After termination, rerun the narrowest blocked validation step.

## Executable Source

The helper reads executables from generated CTest files such as:

- `out/build/<preset>/CTestTestfile.cmake`
- `out/build/<preset>/**/*_tests.cmake`

## Script Asset

Use the bundled script:

- `./kill-stuck-test-processes.py`

## Suggested Invocation

Python example:

```powershell
python .\.claude\skills\terminate-stuck-test-executables\kill-stuck-test-processes.py --build-dir .\out\build\MSVC-release --name MOM_tests.exe
```

List discovered test executables without killing anything:

```powershell
python .\.claude\skills\terminate-stuck-test-executables\kill-stuck-test-processes.py --build-dir .\out\build\MSVC-release --list
```

Kill every currently running executable discovered from that CTest config:

```powershell
python .\.claude\skills\terminate-stuck-test-executables\kill-stuck-test-processes.py --build-dir .\out\build\MSVC-release --all
```

## Response Pattern

Before use, say:

`I found a stuck test executable. I can terminate it with the terminate-stuck-test-executables skill and then rerun the blocked validation. Please confirm if you want me to use it.`

After use, report:

- which processes were terminated
- which names were not running
- which names were discovered from CTest
- which validation step was rerun