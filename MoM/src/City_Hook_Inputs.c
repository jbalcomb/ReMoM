/*
    City_Hook_Inputs -- the city-economy mod hooks, with the inputs Caster of Magic's economy scripts expect.

    CoM2 hands each economy script a few values the game already worked out (MODDING.INI / the scripts' own headers):
        EconomyFood      FARMERS, MAXPOP
        EconomyProd      WORKERS, FARMERS, PRODPER (production % from terrain)
        EconomyGold      ROADBONUS (road trade %), TERRAINGOLD (shore/river trade %), BASEGOLD (tax gold)
    They are computed here with the same functions CITYCALC.c uses, and only when the hook has a handler, so the game
    without mods does no extra work.
*/

#include "City_Hook_Inputs.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"

#include "CITYCALC.h"
#include "Terrain.h"

int32_t City_Hook_Value(int16_t hook_id, int16_t city_idx, int32_t value)
{
    struct s_MOD_HOOK_INPUT inputs[MOD_HOOK_MAX_INPUTS];
    int16_t wx_array[CITY_AREA_SIZE];
    int16_t wy_array[CITY_AREA_SIZE];
    int16_t useable_map_squares;
    int16_t rebels;
    int32_t percent;
    int count = 0;
    int itr;

    if(MOD_Hook_Count(hook_id) == 0)
    {
        return value;
    }

    rebels = City_Rebel_Count(city_idx);
    switch(hook_id)
    {
        case MOD_HOOK_CITY_FOOD:
        {
            inputs[count].name = "FARMERS"; inputs[count].value = _CITIES[city_idx].farmer_count; count++;
            inputs[count].name = "MAXPOP";  inputs[count].value = City_Maximum_Size(city_idx);    count++;
        } break;
        case MOD_HOOK_CITY_PRODUCTION:
        {
            percent = 0;
            useable_map_squares = Get_Useable_City_Area(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, &wx_array[0], &wy_array[0]);
            for(itr = 0; itr < useable_map_squares; itr++)
            {
                percent += Square_Production_Bonus(wx_array[itr], wy_array[itr], _CITIES[city_idx].wp, (_CITIES[city_idx].enchantments[GAIAS_BLESSING] != ST_FALSE));
            }
            inputs[count].name = "WORKERS"; inputs[count].value = _CITIES[city_idx].population - _CITIES[city_idx].farmer_count - rebels; count++;
            inputs[count].name = "FARMERS"; inputs[count].value = _CITIES[city_idx].farmer_count; count++;
            inputs[count].name = "PRODPER"; inputs[count].value = percent; count++;
        } break;
        case MOD_HOOK_CITY_GOLD:
        {
            inputs[count].name = "ROADBONUS";   inputs[count].value = City_Road_Trade_Bonus(city_idx); count++;
            inputs[count].name = "TERRAINGOLD"; inputs[count].value = Square_Gold_Bonus(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp); count++;
            inputs[count].name = "BASEGOLD";
            if(_CITIES[city_idx].owner_idx != NEUTRAL_PLAYER_IDX)
            {
                inputs[count].value = ((_CITIES[city_idx].population - rebels) * _players[_CITIES[city_idx].owner_idx].tax_rate) / 2;   /* as City_Gold_Production() */
            }
            else
            {
                inputs[count].value = _CITIES[city_idx].population - rebels;
            }
            count++;
        } break;
        default:
        {
        } break;
    }

    return MOD_Hook_City_Value_Inputs(hook_id, city_idx, value, inputs, count);
}
