-- Rules profile "community_patch": 1.31 with drake178's Community Patch.
--
-- Starts from the original rules and replaces only the rules the Community Patch changes.

rules.load("original", "terrain.lua")
rules.load("terrain.lua")

rules.label       = "Community Patch"
rules.description = "1.31 with drake178's Community Patch."
rules.base        = "original"
rules.about("square_production_bonus", { status = "unconfirmed", note = "No Community Patch change confirmed yet; same as 1.31 until drake178's change list is checked." })
