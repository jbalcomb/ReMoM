# Rules catalog

Generated from `doc/rules-catalog/rules_catalog.ttl` by `tools/rules_catalog/ttl_to_table.py`; do not edit.
A **bold** value differs from the first profile's.

## Profiles

| Profile | Name | Starts from | Description |
|---|---|---|---|
| `original` | Master of Magic 1.31 |  | 1.31 as reconstructed in ReMoM; ReMoM's default. |
| `community_patch` | Community Patch | `original` | 1.31 with drake178's Community Patch. |
| `com2` | Caster of Magic for Windows 1.05.11 | `original` | Seravy's Caster of Magic for Windows, with the values of its own Data/ folder. |

## Square production bonus (`rules.square_production_bonus`)

Production bonus (%) one square of a city's area gives that city.

Asked for by `Square_Production_Bonus()` in `MoM/src/Terrain.c`; 1.31 original `Square_Production_Bonus_Original()` (WZD s161p04, drake178: `TILE_GetProd()`). Unit: percent.

- `terrain_type`: 1.31 terrain type of the square
- `gaias_blessing`: 1 if the city has Gaia's Blessing
- `shared`: 1 if the square is also in another city's area

| Data point | Arguments | Master of Magic 1.31 | Community Patch | Caster of Magic for Windows 1.05.11 |
|---|---|---|---|---|
| Grassland | `terrain_type=162, gaias_blessing=0, shared=0` | 0 | 0 | 0 |
| Forest | `terrain_type=163, gaias_blessing=0, shared=0` | 3 | 3 | 3 |
| Forest, Gaia's Blessing | `terrain_type=163, gaias_blessing=1, shared=0` | 6 | 6 | **9** |
| Hills | `terrain_type=171, gaias_blessing=0, shared=0` | 3 | 3 | 3 |
| Hills (range tile) | `terrain_type=275, gaias_blessing=0, shared=0` | 3 | 3 | 3 |
| Mountain | `terrain_type=164, gaias_blessing=0, shared=0` | 5 | 5 | **7** |
| Mountain (range tile) | `terrain_type=259, gaias_blessing=0, shared=0` | 5 | 5 | **7** |
| Mountain, shared square | `terrain_type=164, gaias_blessing=0, shared=1` | 2 | 2 | **3** |
| Desert (base type Desert1) | `terrain_type=165, gaias_blessing=0, shared=0` | 0 | 0 | **3** |
| Desert (Desert2) | `terrain_type=174, gaias_blessing=0, shared=0` | 3 | 3 | 3 |
| Desert (range tile) | `terrain_type=292, gaias_blessing=0, shared=0` | 3 | 3 | 3 |
| Swamp | `terrain_type=166, gaias_blessing=0, shared=0` | 0 | 0 | 0 |
| Tundra | `terrain_type=167, gaias_blessing=0, shared=0` | 0 | 0 | 0 |
| Volcano | `terrain_type=179, gaias_blessing=0, shared=0` | 0 | 0 | 0 |
| Chaos Node | `terrain_type=170, gaias_blessing=0, shared=0` | 5 | 5 | **7** |
| Nature Node | `terrain_type=169, gaias_blessing=0, shared=0` | 3 | 3 | 3 |
| Nature Node, Gaia's Blessing | `terrain_type=169, gaias_blessing=1, shared=0` | 6 | 6 | **3** |
| Sorcery Node | `terrain_type=168, gaias_blessing=0, shared=0` | 0 | 0 | 0 |
| River | `terrain_type=233, gaias_blessing=0, shared=0` | 0 | 0 | 0 |
| Ocean | `terrain_type=0, gaias_blessing=0, shared=0` | 0 | 0 | 0 |

| Profile | Status | Source | Note |
|---|---|---|---|
| Master of Magic 1.31 | confirmed | MoM/src/Terrain.c Square_Production_Bonus_Original() (WZD s161p04); equal for every input (MoM/tests/test_Square_Production_Bonus_Rules.cpp) |  |
| Community Patch | unconfirmed |  | No Community Patch change confirmed yet; same as 1.31 until drake178's change list is checked. |
| Caster of Magic for Windows 1.05.11 | partly confirmed | CoM2 Data/TERRAIN.INI Prod= and [Special] ForestGaiaProduction | Unconfirmed: halving on shared squares, Gaia's Blessing on Nature Nodes, volcano owner production (VolcanoOwnerProduction=7, not modelled). |
