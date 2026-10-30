-- Catalog of the game calculations that rules profiles decide.
--
-- Each entry names a calculation, where the C code asks for it, its inputs, and a set of data points: named inputs
-- whose results are worth comparing.  tools/rules_catalog runs every data point through every rules profile and
-- writes the results as RDF triples (doc/rules-catalog/rules_catalog.ttl); tools/rules_catalog/ttl_to_table.py turns
-- those into the comparison table (doc/rules-catalog/rules_catalog.md).  The values are never written by hand, so
-- the catalog always says what the code does.
--
-- Runs in the same sandbox as the profiles, after a profile is loaded, so `terrain` and the other constant tables
-- are available.

catalog.calculation {
    rule        = "square_production_bonus",
    label       = "Square production bonus",
    description = "Production bonus (%) one square of a city's area gives that city.",
    unit        = "percent",
    c_function  = "Square_Production_Bonus",
    c_original  = "Square_Production_Bonus_Original",
    c_file      = "MoM/src/Terrain.c",
    wzd         = "s161p04",
    drake178    = "TILE_GetProd()",
    inputs = {
        { name = "terrain_type",   description = "1.31 terrain type of the square" },
        { name = "gaias_blessing", description = "1 if the city has Gaia's Blessing" },
        { name = "shared",         description = "1 if the square is also in another city's area" },
    },
    data_points = {
        { id = "grassland",          label = "Grassland",                         args = { terrain.Grasslands1, 0, 0 } },
        { id = "forest",             label = "Forest",                            args = { terrain.Forest1, 0, 0 } },
        { id = "forest_gaia",        label = "Forest, Gaia's Blessing",           args = { terrain.Forest1, 1, 0 } },
        { id = "hills",              label = "Hills",                             args = { terrain.Hills1, 0, 0 } },
        { id = "hills_range",        label = "Hills (range tile)",                args = { terrain.Hills_Fst, 0, 0 } },
        { id = "mountain",           label = "Mountain",                          args = { terrain.Mountain1, 0, 0 } },
        { id = "mountain_range",     label = "Mountain (range tile)",             args = { terrain.Mountains_Fst, 0, 0 } },
        { id = "mountain_shared",    label = "Mountain, shared square",           args = { terrain.Mountain1, 0, 1 } },
        { id = "desert_base",        label = "Desert (base type Desert1)",        args = { terrain.Desert1, 0, 0 } },
        { id = "desert",             label = "Desert (Desert2)",                  args = { terrain.Desert2, 0, 0 } },
        { id = "desert_range",       label = "Desert (range tile)",               args = { terrain.Desert_Fst, 0, 0 } },
        { id = "swamp",              label = "Swamp",                             args = { terrain.Swamp1, 0, 0 } },
        { id = "tundra",             label = "Tundra",                            args = { terrain.Tundra1, 0, 0 } },
        { id = "volcano",            label = "Volcano",                           args = { terrain.Volcano, 0, 0 } },
        { id = "chaos_node",         label = "Chaos Node",                        args = { terrain.ChaosNode, 0, 0 } },
        { id = "nature_node",        label = "Nature Node",                       args = { terrain.NatureNode, 0, 0 } },
        { id = "nature_node_gaia",   label = "Nature Node, Gaia's Blessing",      args = { terrain.NatureNode, 1, 0 } },
        { id = "sorcery_node",       label = "Sorcery Node",                      args = { terrain.SorceryNode, 0, 0 } },
        { id = "river",              label = "River",                             args = { terrain.Rivers_1st, 0, 0 } },
        { id = "ocean",              label = "Ocean",                             args = { terrain.Ocean, 0, 0 } },
    },
}
