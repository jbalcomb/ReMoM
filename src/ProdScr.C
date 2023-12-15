/*
    Production Screen

    WIZARDS.EXE
        ovr056

*/

#include "MoM.H"
#include "ProdScr.H"



/*
    WIZARDS.EXE  ovr056
*/

// WZD o56p01
// WZD o56p02
// WZD o56p03
// WZD o56p04
// WZD o56p05
// WZD o56p06
// WZD o56p07
// WZD o56p08
// WZD o56p09


// WZD o56p10
// drake178: CTY_GetProduceCost()
int16_t City_Production_Cost(int16_t production_idx, int16_t city_idx)
{
    int16_t tmp_production_idx;

    int16_t production_cost;  // _SI_

    production_cost = 0;

    tmp_production_idx = _CITIES[city_idx].construction;

    _CITIES[city_idx].construction = production_idx;

    production_cost = City_Current_Product_Cost(city_idx);

    production_cost -= _CITIES[city_idx].Prod_Accu;

    if(production_cost < 0)
    {
        production_cost = 0;
    }

    _CITIES[city_idx].construction = tmp_production_idx;

    return production_cost;
}


// WZD o56p11
// drake178: N/A
// MoO2  Module: COLCALC  Colony_N_Turns_To_Produce_()
int16_t City_N_Turns_To_Produce(int16_t production_cost, int16_t city_idx)
{

    int16_t turns_to_produce;  // _SI_

    turns_to_produce = 0;

    if(_CITIES[city_idx].production_units <= 0)
    {
        if(City_Production_Cost(_CITIES[city_idx].construction, city_idx) == 0)
        {
            turns_to_produce = 1;
        }
        else
        {
            turns_to_produce = 999;
        }
    }
    else
    {
        turns_to_produce = (production_cost / _CITIES[city_idx].production_units);
        
        if((production_cost % _CITIES[city_idx].production_units) != 0)
        {
            turns_to_produce += 1;
        }

        if(turns_to_produce == 0)
        {
            turns_to_produce = 1;
        }
    }

    return turns_to_produce;
}


// WZD o56p12
