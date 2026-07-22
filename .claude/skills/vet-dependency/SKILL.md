---
name: vet-dependency
description: Vet a third-party dependency before recommending, installing, or adding it. Use whenever a new external binary, library, package, or tool is proposed - including anything installed via winget/choco/vcpkg/FetchContent, any DLL committed to the repo, and any tool suggested to the user. Also use when the user challenges a dependency's pedigree, provenance, trustworthiness, or supply-chain risk.
---

# Vet a Dependency

**This skill is an adapter. The source of truth is [`doc/#Devel/Dependency-Vetting.md`](../../../doc/%23Devel/Dependency-Vetting.md) — read it and follow it.** Do not restate its policy here; it is shared with the other AI agents working this repo and must not drift.

## Trigger

Use this **before** the words "just install X" leave your mouth — not only when challenged. Recommending a dependency *is* a vetting decision, whether or not anyone asks.

## The two rules you will be tempted to break

1. **Never produce a trust score.** No 0-100, no letter grade, no "low/medium/high risk". A number implies rigour you did not apply and invites the user to stop thinking. Produce verified facts, flags, and explicitly-recorded gaps.
2. **Check the tier before anything else.** Scrutiny must be proportional to blast radius. The classic failure is a rigorous-looking analysis of a dev-only tool while binaries that ship to users sit unexamined.

## Procedure

1. **Classify the tier** (A ships to users / B builds the product / C dev-only scratch) — see the policy doc.
2. **Run the tool:**
   ```
   python tools/vet_dependency.py --repo OWNER/NAME \
       [--upstream OWNER/NAME --author "Exact Name"] \
       [--manifest-url RAW_MANIFEST_URL] [--tier A|B|C]
   ```
   It reports publisher age, activity, **maintainer tenure**, **authority lag** (time between first upstream contribution and taking on publishing authority), hash pinning, and the questions it structurally cannot answer.
3. **Verify upstream endorsement by hand** — does the upstream project's own site link this distributor?
4. **Work the manual checks** in the policy doc: build transparency/reproducibility, key-compromise exposure, sanctions screening where applicable, runtime behaviour.
5. **For Tier C, argue blast radius instead of trust** — vendor it, keep it off `PATH`, no network, scratch directory only.
6. **Report** facts + flags + gaps. State plainly that you cannot hold responsibility for the decision; the human decides and records it.

## Failure modes this exists to prevent

- Inventing criteria on the spot and rendering them as a score.
- Treating "is listed in MAINTAINERS" as a signal. Without a date it is nearly content-free — tenure, and tenure *relative to the artifact's lifetime and to when authority was acquired*, is the signal.
- Assuming the checks are sufficient. **They would all have passed xz-utils (CVE-2024-3094).** Only build transparency would have caught it: the backdoor was in the release tarball, not the git tree.
- Silently dropping a check whose lookup failed. An unchecked item is a **gap**, never a pass.
- Sloppy exact-name matching. GitHub's commit search degrades an unquoted `author-name:First Last`; the tool quotes it, you should not hand-roll the query.
