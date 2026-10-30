-- Rules profile "com2": Caster of Magic for Windows 1.05.11.
--
-- Starts from the original rules (so anything not ported yet behaves as 1.31) and replaces the rules CoM2 changes.

rules.load("original", "terrain.lua")
rules.load("terrain.lua")

rules.label       = "Caster of Magic for Windows 1.05.11"
rules.description = "Seravy's Caster of Magic for Windows, with the values of its own Data/ folder."
rules.base        = "original"
rules.about("square_production_bonus", { status = "partly confirmed", source = "CoM2 Data/TERRAIN.INI Prod= and [Special] ForestGaiaProduction", note = "Unconfirmed: halving on shared squares, Gaia's Blessing on Nature Nodes, volcano owner production (VolcanoOwnerProduction=7, not modelled)." })
