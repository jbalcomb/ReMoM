Combat-Homeless.md

---

# What this list is

Every combat-overlay function that has **not** been taken to done-done **and has no home**.

**The list is empty.** All 237 `// WZD` overlay slots in the combat sources are done-done, nothing is claimed-but-unjudged, and no slot lacks a name. Miscellaneous means homeless, and nothing is homeless any more.

This file is kept because the classification rules and the tooling notes below are the record of *how* that was established — and because the list can reopen the moment a new slot is reconstructed.

The list is generated, not hand-kept. Regenerate it with:

```
python3 tools/review_coverage.py --homeless
```

Run the tool with no arguments for the full reconciliation report, or `--csv out.csv` for the per-slot matrix.

## How a function is classified

[tools/review_coverage.py](../../tools/review_coverage.py) compares four independent inventories and never guesses — where they disagree it reports the difference rather than picking a winner.

| inventory | what it is |
| --- | --- |
| **markers** | every `// WZD <overlay><part>` in [Combat.c](../../MoM/src/Combat.c), [COMBINIT.c](../../MoM/src/COMBINIT.c), [CMBMAGIC.c](../../MoM/src/CMBMAGIC.c), [CMBTAI.c](../../MoM/src/CMBTAI.c), [CMBTMVPT.c](../../MoM/src/CMBTMVPT.c), resolved to the name that follows it |
| **intent** | the `ovrNNN/Name.asm` paths in a review doc's header block, bridged to production names through the doc's rename ledger and its coverage table |
| **verdict** | the rows of a review doc's coverage table, each rendering a fidelity result on one function |
| **tracker** | the checked boxes in [stub_wip_todo.md](../@TODO/stub_wip_todo.md) |

That yields five states. **`CLAIMED` and `NOT DONE` qualify for this list; attribution then removes any of them that a finished review can be held responsible for.**

| state | count | meaning |
| --- | --- | --- |
| `DONE` | 237 | a DONE-DONE doc gives it a coverage-table row, or a tracker box is checked |
| `WALKED` | 0 | a DONE-DONE doc walks it under its own heading and declares it faithful, but never added a table row — a bookkeeping hole in the doc, not an unreviewed function |
| `CLAIMED` | 0 | named in a DONE-DONE doc's header block and never adjudicated anywhere |
| `NOT DONE` | 0 | no doc renders a verdict on it |
| `NO NAME` | 0 | an overlay slot nothing in the sources names |

### The Combat boundary, and what is deliberately outside it

`CMBTTST.c` and `CMBTTSTU.c` look like combat sources and are **not** scanned. That is correct: they hold only project test harnesses (`Combat_Screen_TST_00N`, `TST_Prepare_All_Battle_Units`), no OG reconstructions. Their `// WZD` lines are `dseg:` **data** markers on extern declarations, not `ovrNNNpNN` overlay-slot markers, so they carry nothing a fidelity review could adjudicate. Overlay markers elsewhere in the tree belong to other areas of concern — `o76` is the Advisor screen, not combat.

## Coverage is one axis; `--debt` is the other

A verdict says the C matches the listing. It says nothing about whether the C is clean, and a function can be perfectly faithful and still be a mess. `--debt` reports the second axis per function, cross-referenced with the DONE set:

```
python3 tools/review_coverage.py --debt          DONE-marked functions only
python3 tools/review_coverage.py --debt-all      every function
```

| column | what it counts |
| --- | --- |
| `CLAUDE` | `/* CLAUDE */` edits to original game code |
| `BUG` | raw `; BUG:` / `TODO` markers not in `OGBUG` form |
| `loc` | a local whose name is not `lower_snake_case` |
| `prm` | a **parameter** whose name is not `lower_snake_case` |
| `DBG` | `DBG_` leftovers |
| `cmt` | commented-out code with no live replacement beside it |
| `blank` | runs of 2+ blank lines |

It also reports **header drift** — prototypes in `Combat.h` and friends whose parameter names disagree with the `.c` definition.

### Three blind spots this tool had, and what they cost

All were found on 2026-08-27/28 and all are closed. They are recorded because the shape of the mistake matters more than the instances.

**Parameters were never scanned at all.** The `loc` pattern anchors on `^\s+` — body indentation — and requires the line to end in `=` or `;`. A function signature sits at column 0 and ends in `)`, so it could never match. `loc` had been reporting body declarations only while presenting itself as the whole naming check. Eleven non-compliant parameters across seven functions had passed every review, `Max_X` / `Max_Y` in `Auto_Move_Unit` among them.

**`cmt` counted the pattern AGENTS.md requires.** Faithful code that must change is commented out with its replacement below it, so scoring that shape as debt made `cmt` a number that could never reach zero — and a metric nobody can clear is a metric nobody reads. It stood at 39, of which roughly 34 were lines that had to stay. That noise hid a corrupted half-typed comment inside `Combat_Cache_Read` for a month. The first attempted fix keyed on the identifier matching between the commented line and the live one, which is wrong in the same direction: the platform substitutions **rename as they replace** (`gfopen` → `stu_fopen_ci`, `gfread` → `fread`), so an identifier test scores those as reviewable while passing the genuinely dead ones. The rule that works is positional — a live statement within a couple of lines below, looking past any further comment lines in the same stack.

**Function bodies overshot, and multi-line signatures were invisible.** A definition was only recognised when its opening line ended in `)` or `{`, so a signature spanning lines was skipped entirely — and the body end was found by scanning *backward* from the next recognised definition to the last `}`, which then let the **preceding** function swallow it. `Generate_Combat_Map` was never scanned at all, and `Build_Battlefield` reported a blank-line run that sits 400 lines and three functions further down the file. Both `blank` hits in the whole area were this artifact. The fix is to join continuation lines to recover the signature, and to find the body end by scanning *forward* to the first column-0 `}` — which is the real close of a column-0 definition and does not depend on correctly spotting whatever comes next.

The general lesson: a metric that cannot reach zero trains you to ignore the column, and a metric that quietly checks less than its name implies is worse than no metric, because it reads as a pass. Two of these three did not report a wrong number — they reported a confidently *right-looking* number over a set that silently excluded the failing cases.

## Anchor drift

`--anchors` reports `#L` targets past the end of a file. That is the *detectable* subset. An anchor that drifted but still lands **inside** the file resolves to the wrong line while reading as authoritative, which is worse than an obviously broken link, and nothing caught those until [tools/reanchor_reviews.py](../../tools/reanchor_reviews.py):

```
python3 tools/reanchor_reviews.py            report only
python3 tools/reanchor_reviews.py --apply    rewrite the provable ones
```

It only rewrites anchors the doc itself can prove. Three shapes qualify, because in each the doc names the function whose definition the anchor should point at:

| shape | example |
| --- | --- |
| link text names the function | `[Make_Missiles](../../MoM/src/Combat.c#L22318)` |
| coverage-table row | a row whose first cell is a backticked function name, with a `File.c:123` link in the next cell |
| walk heading | a `##` heading naming the function, followed by its `File.c:123` link and an asm line count |

Everything else points into the middle of a function — a statement, a guard, a short range — and nothing in the doc says which one. Those are **not** rewritten; guessing would turn a wrong-but-checkable link into a wrong-and-plausible one. They are reported when past EOF and fixed by hand.

### The audit, and the 38 anchors it leaves for a human

One shape looks resolvable and is not: a backticked identifier immediately followed by its own link. The same shape carries at least three meanings —

| written as | means |
| --- | --- |
| a **bold** function name, its link, then an asm line count | the **definition** |
| "…has a second caller in `Fn`", then a link | a **call site** inside `Fn` |
| "per the `switch` in `Fn`", then a link | a **statement** inside `Fn` |

— and on a line naming two functions the pairing can mis-attribute. So the tool only *audits* it: if the target line does not mention the identifier within a couple of lines, the anchor has drifted, which is true whichever meaning was intended. It reports the function's real definition line as a hint and rewrites nothing.

**38 such anchors are outstanding.** They were invisible before — every one lands inside the file, so `--anchors` never saw them. Resolving each needs someone to read the sentence and decide which of the three meanings applies; a blanket rewrite to the definition would corrupt the call-site and statement cases.

484 anchors were repaired across the Combat reviews on 2026-08-28, every one of them in a doc already marked DONE-DONE, with drift as large as 5,700 lines. The docs' own `Production [Function_Name](…)` convention was the worst affected, because one stale line was shared by every bullet in a section.

**Re-run `--apply` after any edit that shifts line numbers in the combat sources.** Anchors rot silently, and a review that points at the wrong line is worth less than one that points at nothing.

## The `WALKED` bucket is empty

`Load_Combat_Terrain_Pictures` and `Map_Tile_EMS_Page_As_Sandbox` were the two entries here; both now carry coverage-table rows in [Combat-Combat_Screen.md](Combat-Combat_Screen.md), so nothing is walked-but-unrecorded any more.

## The `CLAIMED` bucket is empty

`Calc_Battlefield_Bonuses` was the last entry. It now carries a verdict in [Combat-Combat.md](Combat-Combat.md), alongside `Check_For_Winner`, which that review's header block had also named without adjudicating. Nothing is claimed-but-unjudged any more.

## Which review each one belongs to

A function belongs with whatever review covered the code that calls it. `--attribution` walks every call site of an uncovered function, finds the enclosing function, and reports which DONE-DONE review adjudicated *that*, so an uncovered slot becomes some finished review's backlog rather than a loose end.

```
python3 tools/review_coverage.py --attribution        full form, every candidate home
python3 tools/review_coverage.py --attribution-md     the table, as markdown
```

**Both produce nothing right now**, because no slot is uncovered. The rule is kept here because it is the policy for placing the next reconstructed slot, not because there is anything currently to place.

## Cross-doc referrals

A claim of the form "X is covered in <other doc>" is a coverage claim like any other, and nothing was checking them. `--referrals` resolves each one against the actual verdict inventory.

```
python3 tools/review_coverage.py --referrals
```

**Ten are unsupported.** Five name a function that no doc renders a verdict on:

| doc making the claim | function | claimed covered in |
| --- | --- | --- |
| [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md) | `Draw_Active_Unit_Stats_And_Icons` | [Combat-Combat_Screen.md](Combat-Combat_Screen.md) |
| [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md) | `Draw_Active_Unit_Damage_Bar` | [Combat-Combat_Screen.md](Combat-Combat_Screen.md) |
| [Combat-Combat_Screen.md](Combat-Combat_Screen.md) | `Combat_Grid_Screen_Coordinates` | [Combat-Miscellaneous.md](Combat-Miscellaneous.md) |
| [Combat-Combat_Screen.md](Combat-Combat_Screen.md) | `Calc_Battlefield_Bonuses` | [Combat-Miscellaneous.md](Combat-Miscellaneous.md) |
| [Combat-Generate_Combat_Map.md](Combat-Generate_Combat_Map.md) | `Combat_Grid_Screen_Coordinates` | [Combat-Combat_Screen.md](Combat-Combat_Screen.md), [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) |

The last three of those are self-referential in effect: two reviews point at *this* file for `Combat_Grid_Screen_Coordinates` and `Calc_Battlefield_Bonuses`, and this file is a homeless list, not an adjudication. Both are also in the `CLAIMED` bucket above - the same two functions, reached by a different route.

Five more point at the wrong doc for a function that *is* covered somewhere: `Spell_Resistance_Modifier`, `Draw_Active_Unit_Window` (twice), `Build_Battlefield` and `Load_Battle_Unit`. Those are cheap to fix - the referral just needs to name the doc that actually holds the verdict.

## Known parsing edges

- Overlay prefixes are written three ways in the sources — `s163`, `o163`, `ovr163` — so marker keys are normalised to (overlay number, part number). Marker text in this list is reproduced exactly as it appears in the source, so a `grep` on any line finds it.
- Two slots have no production body, only a commented-out name: `s96p04` and `s96p06`. This bullet previously listed four, adding `111p06` and `o112p11` - **that was wrong**. Both of those have real bodies and are now walked in [Combat-Spell_Cast_AI_Target.md](Combat-Spell_Cast_AI_Target.md); the tool mis-reported them while they still carried drake178's `UU15_` / `UU_` prefixes, and started resolving them once they were renamed to `AITP_Disintegrate` and `AITP_Word_Of_Recall`. Treat a `notimpl` on a `UU`-prefixed slot as a prompt to open the file, not as a fact.
- The two functions that used to sit in the `WALKED` bucket are cited in [Combat-Combat_Screen.md](Combat-Combat_Screen.md) at production lines that no longer exist - 20387, 21605, 21616 and 22885 against a 23376-line [Combat.c](../../MoM/src/Combat.c). The true definitions are [Map_Tile_EMS_Page_As_Sandbox](../../MoM/src/Combat.c#L19595) at 20215 and [Load_Combat_Terrain_Pictures](../../MoM/src/Combat.c#L22042) at 22713. The line numbers in that doc drift every time `Combat.c` moves; the walks themselves are still good.
- Two header-block listings name `.asm` files that are not on disk: `ovr163/Allocate_Combat_Base_Blocks.asm` in [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md) and `ovr098/Prepare_All_Battle_Units.asm` in [Combat-Init_Prep_Etc.md](Combat-Init_Prep_Etc.md). Both are production names written where the IDA name belongs.

## Adjudicated, but owned by nobody

Functions whose callers span three or more reviews land here by the `MISC_DOC_SPREAD` rule in [tools/review_coverage.py](../../tools/review_coverage.py) — no single review can own them, so their verdicts are recorded in this doc.

**`Combat_Grid_Screen_Coordinates`** ([Combat.c:21862](../../MoM/src/Combat.c#L21862), `ovr154/Combat_Grid_Screen_Coordinates.asm`, 44 asm lines) — **faithful**. Twelve call sites across four reviews: [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) (8), [Combat-Generate_Combat_Map.md](Combat-Generate_Combat_Map.md) (2), [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) (1), [Combat-Combat_Screen.md](Combat-Combat_Screen.md) (1).

`sx = ((cgx - cgy) * 16) + 158` is `sub` / `shl ax, 4` / `add 158` (asm:17-22); `sy = ((cgx + cgy) * 8) - 80` is `add` / `shl ax, 3` / `add -80` (asm:23-28). The sub-cell offsets follow — `(ox - oy) * 2` into `sx` via `shl ax, 1`, a bare `(ox + oy)` into `sy` — then both store through the out-pointers. Both accumulators are register locals with no stack slots, so their declaration order carries no constraint.

The asm parameter names invert what a reader expects: `cgc2` at `bp+6` is the **x** cell and `cgc1` at `bp+8` is the **y** cell, which is why production's `cgx, cgy` order is correct.
