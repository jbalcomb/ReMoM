#!/usr/bin/env python3

from __future__ import annotations

import argparse
from pathlib import Path

from extract_stub_wip_functions import Collect_Stub_Wip_Functions, Write_Stub_Wip_Todo_Markdown


def Parse_Command_Line() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate a Markdown TODO list for __STUB and __WIP functions in MoM/src and MoX/src.",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("doc/#TODO/stub_wip_todo.md"),
        help="Markdown document to create.",
    )

    return parser.parse_args()


def Main() -> int:
    arguments = Parse_Command_Line()
    repository_root = Path(__file__).resolve().parent.parent
    source_paths = [repository_root / "MoM" / "src", repository_root / "MoX" / "src"]
    function_entries = Collect_Stub_Wip_Functions(source_paths)
    output_path = arguments.output

    if not output_path.is_absolute():
        output_path = repository_root / output_path

    try:
        Write_Stub_Wip_Todo_Markdown(output_path, function_entries)
    except OSError:
        return 1

    print(f"Wrote {len(function_entries)} TODO entries to {output_path}")

    return 0


if __name__ == "__main__":
    raise SystemExit(Main())
