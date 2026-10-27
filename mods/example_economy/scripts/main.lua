-- Example ReMoM mod: the five city-economy hooks.
--
-- Each handler receives (city_idx, value) -- value is what the original game computed (or what an earlier mod
-- returned) -- and returns the new value.  Return `value` unchanged to leave the game alone.
-- The values pass through before the AI difficulty modifier, so every player's city starts from the same number.
--
-- Building indexes are ReMoM's (MoX/src/MOM_DAT.h); 3 is the Barracks in the original game.

local BARRACKS = 3

-- +1 gold in every city of size 5 or more.
game.on("city_gold", function(city, gold)
    if game.citysize(city) >= 5 then
        return gold + 1
    end
    return gold
end)

-- Cities with Barracks get +1 production.
game.on("city_production", function(city, production)
    return production + game.isbuilt(city, BARRACKS)
end)

-- Food, research and power are left as the game computed them.
game.on("city_food",     function(city, food)     return food     end)
game.on("city_research", function(city, research) return research end)
game.on("city_power",    function(city, power)    return power    end)

print("Example Economy loaded")
