-- Rules profile "original": Master of Magic 1.31, as reconstructed in ReMoM.
-- ReMoM's default profile.  Every rule here reproduces the C original exactly (see each file's header).

rules.load("terrain.lua")

rules.label       = "Master of Magic 1.31"
rules.description = "1.31 as reconstructed in ReMoM; ReMoM's default."
rules.about("square_production_bonus", { status = "confirmed", source = "MoM/src/Terrain.c Square_Production_Bonus_Original() (WZD s161p04); equal for every input (MoM/tests/test_Square_Production_Bonus_Rules.cpp)" })
