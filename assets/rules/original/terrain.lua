-- Terrain rules: Master of Magic 1.31.
--
-- A line-for-line translation of the reconstructed 1.31 code, so ReMoM's default game plays exactly as 1.31 does.
-- MoM/tests/test_Square_Production_Bonus_Rules.cpp runs every input through this script and through the C original
-- and requires the same answer, so a change here that alters 1.31 behaviour fails the test.


-- Production bonus (%) one square of a city's area gives that city.
-- C original: Square_Production_Bonus_Original() in MoM/src/Terrain.c (WZD s161p04, drake178: TILE_GetProd()).
--   terrain_type    1.31 terrain type of the square (0 .. 65535; the game stores it as an unsigned 16-bit number)
--   gaias_blessing  1 if the city has Gaia's Blessing
--   shared          1 if the square is also in another city's area
function rules.square_production_bonus(terrain_type, gaias_blessing, shared)
    local production_bonus

    if terrain_type <= terrain.Desert_Lst then
        if terrain_type <= terrain.Mountains_Lst then
            if terrain_type <= terrain.Rivers_end then
                if terrain_type == terrain.Mountain1 or terrain_type == terrain.ChaosNode then
                    -- base/super-type Mountain, or Chaos Node
                    production_bonus = 5
                elseif terrain_type == terrain.Forest1 or terrain_type == terrain.Forest2
                    or terrain_type == terrain.Forest3 or terrain_type == terrain.NatureNode then
                    -- the C code tests `!= ST_TRUE`, so only exactly 1 counts as having Gaia's Blessing
                    if gaias_blessing ~= 1 then
                        production_bonus = 3
                    else
                        production_bonus = 6
                    end
                elseif terrain_type == terrain.Hills1 or terrain_type == terrain.Desert2
                    or terrain_type == terrain.Desert3 or terrain_type == terrain.Desert4 then
                    -- OGBUG?  terrain.Desert1, the base desert type, is not in this list and gets 0.
                    production_bonus = 3
                else
                    production_bonus = 0
                end
            else
                -- Mountain range
                production_bonus = 5
            end
        else
            -- Hills range and Desert range
            production_bonus = 3
        end

        if shared ~= 0 then
            production_bonus = production_bonus // 2
        end
    else
        -- above the last range that has any production bonus
        production_bonus = 0
    end

    return production_bonus
end
