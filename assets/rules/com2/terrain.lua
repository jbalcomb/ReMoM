-- Terrain rules: Caster of Magic for Windows (CoM2) 1.05.11.
--
-- CoM2 gives each kind of tile a production bonus in Data/TERRAIN.INI ("Prod=", sections [0] .. [16], one per
-- overland tile kind) instead of 1.31's hard-coded list.  The values below are CoM2's defaults; a CoM2 mod such as
-- Warlord ships its own TERRAIN.INI, which a loader will read into this table later.
--
-- Unconfirmed, to check against CoM2's own engine (Casapi.dll, Map.tileproductionbonus):
--   * shared squares: kept 1.31's halving; the CoM2 manual does not say.
--   * Gaia's Blessing: CoM2's [Special] ForestGaiaProduction=9 names forests only, so Nature Nodes keep 3.
--   * volcanoes: CoM2 gives the volcano's creator VolcanoOwnerProduction=7; that needs the volcano's owner and the
--     city's owner, which this rule is not given yet, so volcanoes give 0 as for everyone else.

local kind = {
    ocean = 0, shore = 1, lake = 2, river_mouth = 3, grassland = 4, hill = 5, mountain = 6, volcano = 7, desert = 8,
    swamp = 9, tundra = 10, river = 11, forest = 12, chaos_node = 13, nature_node = 14, sorcery_node = 15,
    river_start = 16,
}

-- Data/TERRAIN.INI, Prod= per tile kind.
local production = {
    [kind.ocean] = 0, [kind.shore] = 0, [kind.lake] = 0, [kind.river_mouth] = 0, [kind.grassland] = 0,
    [kind.hill] = 3, [kind.mountain] = 7, [kind.volcano] = 0, [kind.desert] = 3, [kind.swamp] = 0,
    [kind.tundra] = 0, [kind.river] = 0, [kind.forest] = 3, [kind.chaos_node] = 7, [kind.nature_node] = 3,
    [kind.sorcery_node] = 0, [kind.river_start] = 0,
}
local forest_gaia_production = 9  -- [Special] ForestGaiaProduction

local function between(t, first, last)
    return t >= first and t <= last
end

-- CoM2 tile kind of a 1.31 terrain type (the same mapping as com2_tile_kind() in MOD/src/CAS_Game.c).
local function tile_kind(t)
    local tt = terrain
    if t == tt.Ocean or t == tt.OceanAnim then return kind.ocean end
    if t == tt.BugGrass then return kind.grassland end
    if t == tt.Lake or between(t, tt.Lake1, tt.Lake4) then return kind.lake end
    if between(t, tt.Shore1_Fst, tt.Shore1_Lst) then return kind.shore end
    if t == tt.Grasslands1 or t == tt.Grasslands2 or t == tt.Grasslands3 or t == tt.Grasslands4 then return kind.grassland end
    if t == tt.Forest1 or t == tt.Forest2 or t == tt.Forest3 then return kind.forest end
    if t == tt.Mountain1 or between(t, tt.Mountains_Fst, tt.Mountains_Lst) then return kind.mountain end
    if t == tt.Hills1 or between(t, tt.Hills_Fst, tt.Hills_Lst) then return kind.hill end
    if t == tt.Desert1 or between(t, tt.Desert2, tt.Desert4) or between(t, tt.Desert_Fst, tt.Desert_Lst) then return kind.desert end
    if t == tt.Swamp1 or t == tt.Swamp2 or t == tt.Swamp3 then return kind.swamp end
    if t == tt.Tundra1 or t == tt.Tundra2 or t == tt.Tundra3 or between(t, tt.Tundra_1st, tt.Tundra_Last) then return kind.tundra end
    if t == tt.SorceryNode then return kind.sorcery_node end
    if t == tt.NatureNode then return kind.nature_node end
    if t == tt.ChaosNode then return kind.chaos_node end
    if t == tt.Volcano then return kind.volcano end
    if between(t, tt.RiverM_1st, tt.RiverM_end) then return kind.river_mouth end
    if between(t, tt.Rivers_1st, tt.Rivers_end) or between(t, tt["4WRiver1"], tt["4WRiver5"]) then return kind.river end
    if t < tt.TerType_Count then return kind.shore end  -- the remaining shore ranges
    return nil                                          -- not a terrain type
end

-- Same contract as rules/original/terrain.lua.
function rules.square_production_bonus(terrain_type, gaias_blessing, shared)
    local k = tile_kind(terrain_type)
    local production_bonus

    if k == nil then
        return 0
    end
    production_bonus = production[k]
    if k == kind.forest and gaias_blessing == 1 then
        production_bonus = forest_gaia_production
    end
    if shared ~= 0 then
        production_bonus = production_bonus // 2
    end
    return production_bonus
end
