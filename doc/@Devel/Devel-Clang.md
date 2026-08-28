
clang
clang-format
clangd
clang-tidy





## Clang  (LLVM)
https://github.com/llvm/llvm-project/releases
https://github.com/llvm/llvm-project/releases/download/llvmorg-20.1.4/LLVM-20.1.4-win64.exe

LLVM-20.1.4-win64.exe

C:\Program Files\LLVM\bin\
clang.exe
clang++.exe



## ClangFormat
https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.ClangFormat
http://clang.llvm.org/docs/ClangFormat.html

https://github.com/xaverh/vscode-clang-format

### House style & version pin

Formatting is governed by `.clang-format` at the repo root, pinned to **clang-format 20 (LLVM 20)**. Two rules were measured from the tree, not guessed:

- `PointerAlignment: Middle` → `Type * name` (~2800 sites vs ~400 the other way)
- `SpaceBeforeParens: Never` → `if( for( while(` (~15700 sites vs ~90 spaced)

The config also uses `ColumnLimit: 0` (never reflow), `MaxEmptyLinesToKeep: 3`, `SortIncludes: Never`, and `AlignTrailingComments: {Kind: Leave}` so hand-alignment is preserved.

### Do NOT bulk-format the tree

The codebase is heavily hand-aligned (column-aligned declarations, offset-annotated `#pragma pack` structs). Even a perfectly-tuned config rewrites ~90% of files, and much of that is unrecoverable alignment loss. So clang-format is applied **per file, as files are touched** — never `clang-format -i` across the whole repo. Guard byte-layout tables with `// clang-format off` / `// clang-format on`. Vendored/mirror trees (`ext/`, `IDE/`, `build/`, `out/`) are excluded via `.clang-format-ignore`.

### Workflow

| Action | Command |
| --- | --- |
| Format the file you're editing | VS Code task **format: fix current file**, or `clang-format -i --style=file <path>` |
| Check what a commit will need | `tools/format.sh check` (staged files) |
| Format staged files in place | `tools/format.sh fix` |
| Check everything this branch changed | `tools/format.sh check --since origin/main` |

`tools/format.sh` prefers `clang-format-20`, warns on any other major version, and honors `.clang-format-ignore`.

### Enforcement

- **Pre-push hook** (`tools/git-hooks/pre-push`): on main-bound pushes, checks only the files changed vs `origin/main`; skips gracefully if no clang-format is installed.
- **CI** (`.github/workflows/release-check.yml`, `format-check` job): on pull requests, installs pinned clang-format 20 and checks only the PR's changed files.
- **`.git-blame-ignore-revs`**: enable per-clone with `git config blame.ignoreRevsFile .git-blame-ignore-revs` (list any future pure-reformat commits there).
