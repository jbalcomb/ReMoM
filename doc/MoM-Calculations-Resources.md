


Main Screen
ArmyList Screen
CityList Screen
Magic Screen



## Usages

Main Screen - Status Window
    _players[].gold_reserve, _players[].mana_reserve
Main Screen - Summary Window
    no stored value, calculated on every draw



## Resources

Reserves:
    Gold
    Mana

Upkeep:
    Gold
    Food
    Mana



main()
    |-> Screen_Control()
    |-> Main_Screen()
        |-> Main_Screen_Draw()
            |-> Main_Screen_Draw_Do_Draw()
                |-> Main_Screen_Draw_Summary_Window()
                |-> Main_Screen_Draw_Status_Window()


## Main_Screen_Draw_Status_Window()
    _players[_human_player_idx].gold_reserve
    _players[_human_player_idx].mana_reserve
    Print_Integer_Right(265, 68, _players[_human_player_idx].gold_reserve);
    Print_Integer_Right(303, 68, _players[_human_player_idx].mana_reserve);
    Print(268, 68, cstr_GP);
    Print(306, 68, cstr_MP);

¿ Where is the code to calculate and update the values stored in gold_reserve and mana_reserve ?


## Main_Screen_Draw_Summary_Window()

Player_Resource_Income_Total(_human_player_idx, &gold, &food, &mana);
CITYCALC.C
// WZD o120p07
void Player_Resource_Income_Total(int16_t player_idx, int16_t * gold, int16_t * food, int16_t * mana)

    Get_Power_Incomes(&Mana, &City_Food_Surplus, &City_Gold_Balance, player_idx);
    City_Gold_Balance = 0;
    City_Food_Surplus = 0;
    ...
    // ¿ exactly as presented on the 'ArmyList Screen' ?
    // ¿ "total" vs. "army" upkeep - equivalent for gold and food, but mana has non-armies Overland and City Enchantments ?
    Mana_Upkeep = WIZ_TotalUpkeep_Mana(player_idx);
    Gold_Upkeep = Player_Armies_Gold_Upkeep(player_idx);
    Food_Upkeep = WIZ_ArmyUpkeep_Food(player_idx);
    ...
    adjusted for Computer-Players by Difficulty Modifier
    ...
    City_Gold_Balance += (_CITIES[itr_cities].Gold - _CITIES[itr_cities].Upkeep);
    City_Food_Surplus += (_CITIES[itr_cities].Food - _CITIES[itr_cities].Pop_K);
    ...



CITYCALC.C
// WZD o120p18
void Get_Power_Incomes(int16_t * Mana, int16_t * Research, int16_t * Skill, int16_t player_idx)
    if( (_players[player_idx].Spell_Cast & 0xD6 /* Spell_Of_Return */) != 0 )
    {
        *Mana = 0;
        *Research = 0;
        *Skill = 0;
    }
    else
    {






## Total Gold / Gold Reserve Treasury

WIZ_GoldIncomes()
    array of 6, for each wizard
    iterates over all cities
    sets Income_Gold[]
    s_CITY.Gold
    s_CITY.Upkeep
    sets Income_Food[], with 
        s_CITY.Food
        s_CITY.Pop_K

## Gold Income

_CITIES[].gold_units  
    set in Do_City_Calculations() using City_Gold_Production()  
_CITIES[].building_maintenance  
    set in Do_City_Calculations() using City_Gold_Mainanence()  


## Food Reserve
    Sum of (Food - Pop_K) for each City minus the Count of Normal Units
    Surplus Food = Food Harvested - Food Eaten
    Excess Food = Surplus Food - Army Rations (Unit Upkeep)
    Food Income = Excess Food * 0.5 Gold


Income_Food[] - Normal_Unit_Counts[]


## Accumulate Gold Income
NOTE: happens after city calculations  
'Next Turn': add 'Gold Income' to 'Gold Reserve'  
Next_Turn_Proc()  
Next_Turn_Calc()  
Update_Players_Gold_Reserve()  
gold_reserve + gold_income + food_income  
gold_income  
    Per Hero, +10 for each Hero Noble  
    Per City, gold_units - _CITIES[itr_cities].building_maintenance  

gold_incomes[_CITIES[itr_cities].owner_idx] += (_CITIES[itr_cities].gold_units - _CITIES[itr_cities].building_maintenance);  
Excess_Food = _CITIES[itr_cities].food_units - _CITIES[itr_cities].population;  
food_incomes[_CITIES[itr_cities].owner_idx] += (Excess_Food / 2);  
food_incomes[itr_players] -= normal_units[itr_players];  
_players[itr_players].gold_reserve += (gold_incomes[itr_players] + food_incomes[itr_players]);  

Do_City_Calculations()
    _CITIES[city_idx].food_units            = City_Food_Production(city_idx);
    _CITIES[city_idx].gold_units            = City_Gold_Production(city_idx);
    _CITIES[city_idx].building_maintenance  = City_Gold_Mainanence(city_idx);





## Magic Screen
Mana, Research, Skill  
Casting Skill  
Magic Reserve  
Power Base  
...  
"Magic Reserve" here is the same as "Total Mana" in 'Status Window' on the   'Main Screen'
...  
Power Distribution  
Mana Points Ratio  
Research Ratio  
Casting Skill Ratio  


Where does the 'Power Base' get set?  
    Players_Update_Magic_Power()  

Where does _CITIES[].mana_units get set?  


```
_players[player_idx].Power_Base;
_players[player_idx].Research_Pcnt
_players[player_idx].Mana_Pnct
_players[player_idx].Skill_Pcnt
```
```
mana_portion   = (((magic_power * 100) + 50) / _players[player_idx].Mana_Pnct)
skill_portion  = (((magic_power * 100) + 50) / _players[player_idx].Skill_Pcnt)
research_portion = magic_power - mana_portion - skill_portion;
```
```
_players[player_idx].mana_focusing
```
```
city_research_points += _CITIES[itr_cities].research_units
```
```
if(_players[player_idx].Heroes[itr_heroes].Unit_Index > -1)
_UNITS[].type
Abilities_HI & 0x20 != 0  /* Ab_Sage */
Abilities_HI & 0x40 != 0  /* Ab_Sage2 */
```
