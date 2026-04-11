#!/usr/bin/env python3

# Based on https://wiki.wxwidgets.org/Parse_valgrind_suppressions.sh
# Read from stdin (supports: cat log.txt | python3 parse_suppressions.py)
# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all ./ReMoMber 2> ~/STU/devel/ReMoM/valgrind-gen-suppressions-all.npp
# Z:\home\jbalcomb\STU\devel\ReMoM\valgrind-gen-suppressions-all.npp
# cat valgrind-gen-suppressions-all.npp | python3 scripts/valgrind_suppressions_parser.py
# python3 scripts/valgrind_suppressions_parser.py --input-file valgrind-gen-suppressions-all.npp --output-file ReMoM.supp --summary

# Piped (same as before)
# cat valgrind-gen-suppressions-all.npp | python3 scripts/valgrind_suppressions_parser.py --summary
# File args
# python3 scripts/valgrind_suppressions_parser.py -i valgrind-gen-suppressions-all.npp -o ReMoM.supp --summary

import sys
import re
import hashlib
import argparse

SUPPRESSION_TOOL_KINDS = re.compile(
    r"^(Memcheck|Helgrind|DRD|exp-sgcheck|exp-bbv):"
)


def parse_valgrind_suppressions(input_stream, output_stream, show_summary):
    """
    Extracts unique suppression blocks from Valgrind log output.
    """
    seen_hashes = set()
    current_block = []
    in_block = False
    total_count = 0
    unique_count = 0
    skipped_count = 0

    for line in input_stream:
        if line.strip() == "{":
            in_block = True
            current_block = [line]
            continue

        if in_block:
            current_block.append(line)

            if len(current_block) == 2:
                # Validate: line after the name must be Tool:Kind (e.g. Memcheck:Cond)
                if not SUPPRESSION_TOOL_KINDS.match(line.strip()):
                    in_block = False
                    current_block = []
                    skipped_count += 1
                    continue

            if line.strip() == "}":
                in_block = False
                total_count += 1

                block_content = "".join(current_block)
                block_hash = hashlib.sha256(block_content.encode("utf-8")).hexdigest()

                if block_hash not in seen_hashes:
                    seen_hashes.add(block_hash)
                    unique_count += 1
                    output_stream.write(block_content + "\n")

    if show_summary:
        sys.stderr.write(
            f"Suppressions: {unique_count} unique out of {total_count} total"
        )
        if skipped_count > 0:
            sys.stderr.write(f" ({skipped_count} malformed blocks skipped)")
        sys.stderr.write("\n")


def main():
    parser = argparse.ArgumentParser(
        description="Extract unique Valgrind suppression blocks from log output."
    )
    parser.add_argument(
        "--input-file", "-i",
        default=None,
        help="Input file (default: stdin)",
    )
    parser.add_argument(
        "--output-file", "-o",
        default=None,
        help="Output file (default: stdout)",
    )
    parser.add_argument(
        "--summary", "-s",
        action="store_true",
        help="Print summary counts to stderr",
    )
    args = parser.parse_args()

    input_stream = open(args.input_file, "r") if args.input_file else sys.stdin
    output_stream = open(args.output_file, "w") if args.output_file else sys.stdout

    try:
        parse_valgrind_suppressions(input_stream, output_stream, args.summary)
    finally:
        if args.input_file:
            input_stream.close()
        if args.output_file:
            output_stream.close()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        sys.exit(0)
