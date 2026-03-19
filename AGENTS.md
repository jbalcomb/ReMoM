---
applyTo: "**"
---

Ignore the Register annotations. (e.g., `_DI_`, `_SI_`, `_CX_`, etc.) They are just for reference when disassembling in IDA Pro 5.5.

# Project general coding standards

## Style Guide
- Use Allman style braces
- Wrap block comments at 180 characters
- Use "square", not "tile"

## Naming Conventions
- Use lower_snake_case for variable names  (e.g., )
- Use Proper_Snake_Case for function names  (e.g., Simulate_World_Map_Generation)
- Use UPPER_SNAKE_CASE for manifest constants and macro names

## Error Handling
- Always log errors with contextual information

## Glossary of Terms
- Dasm stands for Disassembly  (the disassembley of the Borland C++ 3.0 code from IDA Pro 5.5)
- DBG stands for Debug
- DEDU stands for Deduce  (like a to-do list item where some code or logic needs figured out or validated and then documented)
- DNE stands for Does Not Exist  (usually mean it seems like it should based on some similar code elsewhere or a bug or SimTex's coding style)
- IDK stands for I don't know
- IDGI stands for I don't get it
- NIU stands for Not In Use
- PFL stands for Platform
- OGBUG stands for Original Game Bug  (do not try to fix these in the base-line ReMoM code-base)
