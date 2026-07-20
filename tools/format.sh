#!/usr/bin/env bash
#
# format.sh — house-style clang-format wrapper for ReMoM.
#
# ReMoM's tree is heavily hand-aligned, so we do NOT bulk-format it. This
# wrapper formats a *scoped* set of files (staged, or changed vs a base ref,
# or an explicit list) so freshly-touched code converges to house style
# without rewriting the whole codebase. Vendored/mirror trees are excluded
# via .clang-format-ignore.
#
# Usage:
#     tools/format.sh check              # dry-run the STAGED source files (CI/hook mode)
#     tools/format.sh fix                # format the STAGED source files in place
#     tools/format.sh check --since REF  # dry-run files changed vs REF (default origin/main)
#     tools/format.sh fix   --since REF  # format files changed vs REF
#     tools/format.sh check FILE...      # dry-run explicit files
#     tools/format.sh fix   FILE...      # format explicit files in place
#
# `check` exits non-zero if any file would change (suitable for a gate).
# clang-format 20 (LLVM 20) is the pinned version; see doc/#Devel/Devel-Clang.md.

set -euo pipefail

REQUIRED_MAJOR=20
SRC_RE='\.(c|h|cpp|hpp)$'

die() { echo "format.sh: $*" >&2; exit 2; }

# --- locate a clang-format, prefer the pinned major -------------------------
find_cf() {
    local c
    for c in clang-format-${REQUIRED_MAJOR} clang-format; do
        if command -v "$c" >/dev/null 2>&1; then echo "$c"; return 0; fi
    done
    return 1
}
CF="$(find_cf)" || die "clang-format not found (need LLVM ${REQUIRED_MAJOR}: apt install clang-format-${REQUIRED_MAJOR})"
CF_VER="$("$CF" --version | grep -oE '[0-9]+' | head -1)"
if [ "${CF_VER:-0}" != "$REQUIRED_MAJOR" ]; then
    echo "format.sh: warning: using $CF (major ${CF_VER:-?}); house style is pinned to LLVM ${REQUIRED_MAJOR}" >&2
fi

# --- parse args -------------------------------------------------------------
[ $# -ge 1 ] || die "usage: format.sh {check|fix} [--since REF | FILE...]"
mode="$1"; shift
case "$mode" in check|fix) ;; *) die "first arg must be 'check' or 'fix', got '$mode'";; esac

collect_staged() {
    git diff --cached --name-only --diff-filter=ACM
}
collect_since() {
    local base="${1:-origin/main}"
    if ! git rev-parse --verify --quiet "$base" >/dev/null; then
        echo "format.sh: base ref '$base' not found; nothing to check." >&2
        return 0
    fi
    git diff --name-only --diff-filter=ACM "$base"...
}

files=()
base=""   # non-empty => diff-scoped: enforce house style on freshly-changed LINES only
if [ $# -eq 0 ]; then
    mapfile -t files < <(collect_staged | grep -E "$SRC_RE" || true)
    scope="staged"; base="HEAD"
elif [ "$1" = "--since" ]; then
    base="${2:-origin/main}"
    mapfile -t files < <(collect_since "$base" | grep -E "$SRC_RE" || true)
    scope="changed vs $base"
else
    for f in "$@"; do [[ "$f" =~ $SRC_RE ]] && files+=("$f"); done
    scope="explicit"
fi

# Keep only files that still exist on disk.
existing=()
for f in "${files[@]:-}"; do [ -n "$f" ] && [ -f "$f" ] && existing+=("$f"); done
files=("${existing[@]:-}")

if [ ${#files[@]} -eq 0 ] || [ -z "${files[0]:-}" ]; then
    echo "format.sh: no source files in scope ($scope); nothing to do."
    exit 0
fi

echo "format.sh: $mode ${#files[@]} file(s) [$scope] with $CF ($($CF --version | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1))"

# The tree is heavily hand-aligned, so for diff-scoped runs (staged or --since)
# we enforce house style only on the LINES that actually changed, via
# git-clang-format. Explicit FILE arguments are still whole-file (you asked for
# it). This keeps freshly-touched code converging to house style without ever
# flagging the pre-existing hand-alignment of a file just because it was touched.
if [ -n "$base" ]; then
    if [ "$mode" = "fix" ]; then
        git clang-format --binary "$CF" "$base" -- "${files[@]}"
        echo "format.sh: formatted changed lines in place. Review with 'git diff'."
        exit 0
    fi
    out="$(git clang-format --binary "$CF" --diff "$base" -- "${files[@]}" 2>&1 || true)"
    case "$out" in
        "" | "no modified files to format" | "clang-format did not modify any files")
            echo "format.sh: OK — changed lines already match house style."; exit 0 ;;
        *)
            printf '%s\n' "$out"
            echo "format.sh: FAIL — changed lines need formatting. Run: tools/format.sh fix ${1:-} ${2:-}" >&2
            exit 1 ;;
    esac
fi

if [ "$mode" = "fix" ]; then
    "$CF" -i --style=file "${files[@]}"
    echo "format.sh: formatted in place. Review with 'git diff'."
    exit 0
fi

# check mode: --dry-run --Werror turns any needed change into a non-zero exit.
if "$CF" --dry-run --Werror --style=file "${files[@]}"; then
    echo "format.sh: OK — all in-scope files already match house style."
    exit 0
else
    echo "format.sh: FAIL — files above need formatting. Run: tools/format.sh fix" >&2
    exit 1
fi
