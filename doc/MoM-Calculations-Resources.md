



Main Screen
ArmyList Screen
Magic Screen



## Usages

Main Screen - Status Window
    no stored value, calculated on every draw




## Resources

Reserves:
    Gold
    Mana

Upkeep:
    Gold
    Food
    Mana



Help: "TOTAL GOLD": "This is the total amount of gold in your treasury."
Help: "TOTAL MANA": "This is the total number of mana crystals in your reserve that you can draw from to cast spells."

Help: "GOLD INCOME": "The amount of gold you have as profitable income each turn."
Help: "FOOD RESERVE": "The amount of food in excess of that needed to support yours troops. If negative, units will starve to death."
Help: "MANA INCOME": "The number of mana crystals that are being saved/spent each turn."

Help: "UPKEEP": "This is the total that you are spending on food, gold, and mana each turn to support your armies."


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

¿ Where is the code to calculate and update the values stored in gold_reserve and mana_reserve ?


## Main_Screen_Draw_Summary_Window()

Get_Incomes(_human_player_idx, &gold, &food, &mana);
CITYCALC.C
// WZD o120p07
void Get_Incomes(int16_t player_idx, int16_t * gold, int16_t * food, int16_t * mana)

    Get_Power_Incomes(&Mana, &City_Food_Surplus, &City_Gold_Balance, player_idx);
    City_Gold_Balance = 0;
    City_Food_Surplus = 0;
    ...
    // ¿ exactly as presented on the 'ArmyList Screen' ?
    // ¿ "total" vs. "army" upkeep - equivalent for gold and food, but mana has non-armies Overland and City Enchantments ?
    Mana_Upkeep = WIZ_TotalUpkeep_Mana(player_idx);
    Gold_Upkeep = WIZ_ArmyUpkeep_Gold(player_idx);
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

## Food Reserve
    Sum of (Food - Pop_K) for each City minus the Count of Normal Units
    Surplus Food = Food Harvested - Food Eaten
    Excess Food = Surplus Food - Army Rations (Unit Upkeep)
    Food Income = Excess Food * 0.5 Gold


Income_Food[] - Normal_Unit_Counts[]


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
