#!/usr/bin/env python3

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


CANDIDATE_PATTERN = re.compile(
    r"""
    ^[ \t]*
    (?P<prefix>[^\n]*?)
    \b(?P<function_name>[A-Za-z_]\w*__(?:STUB|WIP))
    [ \t\r\n]*
    \(
        (?P<parameters>
            [^;{}()]*
            (?:\([^()]*\)[^;{}()]*)*
        )
    \)
    [ \t\r\n]*
    (?P<terminator>;|\{)
    """,
    re.MULTILINE | re.VERBOSE | re.DOTALL,
)

QUALIFIER_TOKENS = {
    "const",
    "extern",
    "inline",
    "register",
    "signed",
    "static",
    "typedef",
    "unsigned",
    "volatile",
}

STATEMENT_TOKENS = {
    "break",
    "case",
    "continue",
    "for",
    "goto",
    "if",
    "return",
    "sizeof",
    "switch",
    "while",
}

SOURCE_EXTENSIONS = {".c", ".h"}


def Natural_Sort_Key(value: str) -> list[object]:
    return [int(part) if part.isdigit() else part.casefold() for part in re.split(r"(\d+)", value)]


def Strip_C_Comments_And_Strings(source_text: str) -> str:
    output = []
    index = 0
    text_length = len(source_text)

    while index < text_length:
        current = source_text[index]
        following = source_text[index + 1] if (index + 1) < text_length else ""

        if current == "/" and following == "/":
            output.append("  ")
            index += 2

            while index < text_length and source_text[index] != "\n":
                output.append(" ")
                index += 1

            continue

        if current == "/" and following == "*":
            output.append("  ")
            index += 2

            while index < text_length:
                if source_text[index] == "*" and (index + 1) < text_length and source_text[index + 1] == "/":
                    output.append("  ")
                    index += 2
                    break

                output.append("\n" if source_text[index] == "\n" else " ")
                index += 1

            continue

        if current in {'"', "'"}:
            quote = current
            output.append(" ")
            index += 1

            while index < text_length:
                if source_text[index] == "\\" and (index + 1) < text_length:
                    output.append("  ")
                    index += 2
                    continue

                if source_text[index] == quote:
                    output.append(" ")
                    index += 1
                    break

                output.append("\n" if source_text[index] == "\n" else " ")
                index += 1

            continue

        output.append(current)
        index += 1

    return "".join(output)


def Is_Function_Declaration_Prefix(prefix_text: str) -> bool:
    if not prefix_text.strip():
        return False

    if "=" in prefix_text:
        return False

    token_list = re.findall(r"[A-Za-z_]\w*", prefix_text)

    if not token_list:
        return False

    lowered_tokens = [token.casefold() for token in token_list]

    if any(token in STATEMENT_TOKENS for token in lowered_tokens):
        return False

    non_qualifier_tokens = [token for token in lowered_tokens if token not in QUALIFIER_TOKENS]

    return len(non_qualifier_tokens) > 0


def Extract_Stub_Wip_Functions_From_Text(source_text: str, module_name: str) -> list[tuple[str, str]]:
    stripped_text = Strip_C_Comments_And_Strings(source_text)
    function_entries = set()

    for match in CANDIDATE_PATTERN.finditer(stripped_text):
        prefix_text = match.group("prefix")

        if not Is_Function_Declaration_Prefix(prefix_text):
            continue

        function_entries.add((module_name, match.group("function_name")))

    return sorted(function_entries, key=lambda entry: (Natural_Sort_Key(entry[0]), Natural_Sort_Key(entry[1])))


def Extract_Stub_Wip_Functions_From_File(file_path: Path) -> list[tuple[str, str]]:
    module_name = file_path.stem

    try:
        source_text = file_path.read_text(encoding="utf-8", errors="ignore")
    except OSError as error:
        print(f"Error reading {file_path}: {error}", file=sys.stderr)
        return []

    return Extract_Stub_Wip_Functions_From_Text(source_text, module_name)


def Collect_Stub_Wip_Functions(source_paths: list[Path]) -> list[tuple[str, str]]:
    function_entries = set()

    for source_path in source_paths:
        if source_path.is_dir():
            file_paths = sorted(
                (
                    path
                    for path in source_path.rglob("*")
                    if path.is_file() and path.suffix.casefold() in SOURCE_EXTENSIONS
                ),
                key=lambda path: (Natural_Sort_Key(path.stem), Natural_Sort_Key(path.name)),
            )
        else:
            file_paths = [source_path]

        for file_path in file_paths:
            for entry in Extract_Stub_Wip_Functions_From_File(file_path):
                function_entries.add(entry)

    return sorted(function_entries, key=lambda entry: (Natural_Sort_Key(entry[0]), Natural_Sort_Key(entry[1])))


def Create_Stub_Wip_Todo_Markdown(function_entries: list[tuple[str, str]], title: str = "STUB and WIP Function TODO List") -> str:
    output_lines = [f"# {title}", ""]

    if not function_entries:
        output_lines.append("No `__STUB` or `__WIP` functions were found.")
        output_lines.append("")
        return "\n".join(output_lines)

    current_module = None

    for module_name, function_name in function_entries:
        if module_name != current_module:
            if current_module is not None:
                output_lines.append("")

            output_lines.append(f"## {module_name}")
            current_module = module_name

        output_lines.append(f"- [ ] {function_name}")

    output_lines.append("")

    return "\n".join(output_lines)


def Write_Stub_Wip_Todo_Markdown(
    output_path: Path,
    function_entries: list[tuple[str, str]],
    title: str = "STUB and WIP Function TODO List",
) -> None:
    markdown_text = Create_Stub_Wip_Todo_Markdown(function_entries, title=title)

    try:
        output_path.parent.mkdir(parents=True, exist_ok=True)
        output_path.write_text(markdown_text, encoding="utf-8")
    except OSError as error:
        print(f"Error writing {output_path}: {error}", file=sys.stderr)
        raise


def Parse_Command_Line() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Extract function declarations and definitions whose names end in __STUB or __WIP.",
    )
    parser.add_argument("source_paths", nargs="+", type=Path, help="Source files or directories to scan.")

    return parser.parse_args()


def Main() -> int:
    arguments = Parse_Command_Line()
    function_entries = Collect_Stub_Wip_Functions(arguments.source_paths)

    for module_name, function_name in function_entries:
        print(f"{module_name}\t{function_name}")

    return 0


if __name__ == "__main__":
    raise SystemExit(Main())
