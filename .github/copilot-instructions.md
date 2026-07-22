
## Vetting third-party dependencies
Before recommending, installing, or adding ANY external binary, library, package, or tool, follow
`doc/#Devel/Dependency-Vetting.md` and run `python tools/vet_dependency.py`. Never produce a trust
score (no 0-100, no letter grade); report verified facts, flags, and the questions you could not
answer. An unchecked item is a gap, never a pass. Classify blast-radius tier first: A ships to users,
B builds the product, C is dev-only scratch — scrutiny must be proportional.
