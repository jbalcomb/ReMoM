


Main Screen  
    Status Window  
        Gold Reserve  (Total Gold)  
        Mana Reserve  (Total Mana)  
    Summary Window  
        Gold Income  
        Food Reserve  (Excess Food)  
        Mana Income  

Main_Screen()  
    Main_Screen_Draw()  
        Main_Screen_Draw_Do_Draw()  
            Main_Screen_Draw_Summary_Window()  
                Player_Resource_Income_Total()  
                    Get_Power_Incomes()  
                    WIZ_TotalUpkeep_Mana()  
                    Player_Armies_Gold_Upkeep()  
                        WIZ_GetFame()  
                        UNIT_GetGoldUpkeep()  
                    WIZ_ArmyUpkeep_Food()  
            Main_Screen_Draw_Status_Window()  
                _players[_human_player_idx].gold_reserve  
                _players[_human_player_idx].mana_reserve  

Gold Income
    WIZARDS.EXE  
        unit_types_tables  .Cost  .Upkeep  
    main()  
        |-> Load_WZD_Resources()  
            |-> Units_Upkeeps()  
    Loaded_Game_Update()  
        |-> Patch_Units_Upkeep_And_Sound()  
    ...  
    Player_Armies_Gold_Upkeep()  
        WIZ_GetFame()  
            Legendary Heroes  
            Just_Cause Overland Enchantment  
            _players[player_idx].Fame  
        UNIT_GetGoldUpkeep()  
            unit_gold_upkeep += _unit_type_table[_UNITS[unit_idx].type].Upkeep;  
        SUM[units upkeeps - fames]  

Player_Resource_Income_Total()  





Spell Casting Skill
    Total
    Base





food  
production  
gold  
mana  
research  

production  
maintenance  

income  
reserves  



## ¿ Per City ?

~ Current  
    Population  
    Population Growth  
    Farmers  
    Workers  
    Rebels  
    Food  
    Production  
    Gold  
    Mana  
    Research  

'Resource Window' - Food, Production, Gold, Mana, Research  


Gold Maintenance  


Food Required  (Minimum Amount to support Population)  
Gold Required  (Upkeed Cost of Buildings)  ("[net gold income (income - maintenance)]")  


Food Per Farmer  
Production Per Farmer  
Production Per Worker  

Race Modifier for Unrest  
Race Modifier for Food Units  
Race Modifier for Production Units  
Race Modifier for Gold Units  
Race Modifier for Mana Units  
Race Modifier for Research Units  

Unrest From Buildings  
Food From Buildings  
Production From Buildings  
Gold From Buildings  
Mana From Buildings  
Research From Buildings  

Building Modifier for Unrest  
Building Modifier for Food Units  
Building Modifier for Production Units  
Building Modifier for Gold Units  
Building Modifier for Mana Units  
Building Modifier for Research Units  

Building Contribution for Unrest  
Building Contribution for Food Units  
Building Contribution for Production Units  
Building Contribution for Gold Units  
Building Contribution for Mana Units  
Building Contribution for Research Units  



## ||==>> Reserves <<==||
amount of gold and mana in the reserves  

## ||==>> Upkeeps <<==||
net amount of gold, food, and mana being produced per turn  
surplus/deficit  
net gold and mana are added to the reserves each turn  
surplus food is sold on the world market at a indefinitely contracted rate of one gold piece per two surplus food units  

## Next Turn

WIZ_GoldIncomes()  
    _players[] s_WIZARD.gold_reserve  

_players[] s_WIZARD.mana_reserve  

Next_Turn_Proc()  
    |-> Next_Turn_Calc()  
    |-> All_City_Calculations()  
        |-> Do_City_Calculations()  
            _CITIES[] s_CITY.food_units  
            _CITIES[] s_CITY.production_units  
            _CITIES[] s_CITY.gold_units  
            _CITIES[] s_CITY.building_maintenance  
            _CITIES[] s_CITY.research_units  
            _CITIES[] s_CITY.Power  

        |-> Update_Players_Gold_Reserve()  
            |-> Players_Normal_Units()  

        |-> Players_Update_Magic_Power()  
        |-> Players_Apply_Magic_Power()  
            |-> Get_Power_Incomes()
                |-> Get_Power_Incomes_Base()
                |-> Spell_Research_Bonus()
                

j_WIZ_ResearchProgress()  
j_WIZ_ProcessUpkeep()  
    food, gold, mana  
j_CTY_OutpostGrowth()  
j_CTY_ProgressTurn()  
    population growth  
j_WIZ_ProcessGlobals()  
...  
j_WIZ_RecordHistory()  
...  
All_City_Calculations()  





~ Contributions  



Unrest |-> Rebels  
Subtract Rebels from Population  
Food Produced = Farmer Count * Food Per Farmer  
Food Consumed =   

City - Food - Generated, Consumed, Leftovers  
Base Food Level  


## Calculations, Formulas, Equations, Algorithms



Base Food Level  
    Factors  
        Square's Value  
            Terrain Type  
                +0.0  Desert, Mountain, Ocean, Tundra, Volcano; Chaos Node;  
                +0.5  Forst, Hills, Shore  
                +1.0  ¿N/A?; ¿N/A?;  
                +1.5  Grasslands  
                +2.0  River, River Mouth¹; Sorcery Node (¿Grassland+0.5?)  
                +2.5  ¿N/A?; Nature Node¹ (¿Forst+2?);  
        Square's Status  
            Has Cooruption  
                value = 0  
            Is Shared  
                value = 50%  
        City Enchantments  
        Overland Enchantments  
¹ 'Surveyor' provided bad information - shows 0.5 for River Mour and 0.5 for Nature Node  

'Master of Magic' Fandom Wiki  
https://masterofmagic.fandom.com/wiki/Food  
Base Food Level  
Each city has a base food level of Food which can be efficiently produced with enough farmers, as follows:  
    ½ for each Forest, Hill, or Shore in its catchment area.  
    1½ for each Grassland in its catchment area.  
    2 for each River, River Mouth¹, or Sorcery Node¹ in its catchment area.  
    2½ for each Nature Node¹ in its catchment area.  
    The food generated is halved if the terrain tile is shared with another city.  
    Terrain tiles with Corruption do not count. However, tiles with unbroken nodes or lairs do count.  
    +50% to the above quantities if the city has Gaia's Blessing cast on it.  
¹ Reported incorrectly by Surveyor.  



## Food Per Farmer
Do_City_Calculations()  
    |-> City_Food_Production()  
        |-> City_Food_Terrain()  
            |-> Get_Useable_City_Area()  
            |-> Map_Square_Food2()  
                |-> City_Map_Square_Is_Shared()  
        |-> City_Food_WildGame()  

s_CITY.Food  
So, s_CITY.Food is 1-byte value ... signed or unsigned?  

MoO2  
Do_Player_Colony_Calculations_()  
    loops over all colonies, checks owner and outpost status  
        the loop over colonies is done by testing that the ptr_colony is not greater than _colony[_NUM_COLONIES]  
    calls Pre_Import_Computing_() with pointer to colony  
    then Colony_Food2_Per_Farmer_() sets _colony s_COLONY.food2_per_farmer  
    after it updates all of the player's colonies  
        it calls Do_Player_Colony_Post_Production_()  


Get_Useable_City_Area()  
    j_Get_Useable_City_Area()  
    CTY_GetTileFood()  
    CTY_GetWildGameFood()  
    CTY_GetProd()  
    CTY_GetGold()  
    CTY_GetPower()  
    CTY_GetProdCost()  
    CTY_OutpostGrowth()  
    CTY_Gold_Breakdown()  
    CTY_Prod_Breakdown()  
    CTY_Pwr_Breakdown()  

j_Get_Useable_City_Area()  
    CTY_CheckTerrainReq()  
    CTY_GetWeaponQuality()  
    CTY_CountNightshades()  







### Race Modifier for Food Units
    Food Units Per Farmer
        2 Units
        Halflings   3 Units

### Race Modifier for Production Units
    Production Units Per Worker
        1 Unit
        Klackons    1.5 Units

### Race Modifier for Gold Units
    Dwarw workers produce 200%
    Nomads produce 150%
    Tax Rate * Population ("figures")

### Race Modifier for Mana Units
    Magic Power Units Per Population ("person")
        Beastmen, Draconiams, High Elves    0.5 Units
        Dark Elves  1.0 Units



### Building Modifier for Food Units
    Animist's Guild     +50% per Farmer

### Building Contribution for Food Units
    Famrer's Market     +3 Food Units
    Forester's Guild    +2 Food Units
    Granary             +2 Food Units

### Terrain Specials Contribution for Food Units
    Wild Game   +2 Food Units

### Terrain Specials Contribution for Maximum Food Units  ¿Base Food Level? ¿Maximum Population?
    Wild Game   +2 Food Units


### Building Contribution for Population Growth
    Famrer's Market     +30 People Per Turn
    Granary             +20 People Per Turn









Surveyor  
City Resources  
Maximum Population  
Production Bonus  
Gold Bonus  
MoM-Manual:  
    "terrain type"  
    "special terrain features"  
    "bonus effects on food, gold, power, and production."  
    "Terrain Types"  
    "Terrain Specials"  
    "specific terrain types and features"  





Player_Resource_Income_Total()  

    Get_Power_Incomes(&Mana, &City_Food_Surplus, &City_Gold_Balance, player_idx);  

    Mana_Upkeep = WIZ_TotalUpkeep_Mana(player_idx);  
    Gold_Upkeep = Player_Armies_Gold_Upkeep(player_idx);  
    Food_Upkeep = WIZ_ArmyUpkeep_Food(player_idx);  

Per City  
    City_Gold_Balance = (_CITIES[itr_cities].Gold - _CITIES[itr_cities].Upkeep);  
    City_Food_Surplus = (_CITIES[itr_cities].Food - _CITIES[itr_cities].population);  

add 10 gold per noble hero

add food / 2, if food positive

        *food = City_Food_Surplus - Food_Upkeep;

        // mov bx, [bp+food]; mov ax, [bx]; cwd; sub ax, dx; sar ax, 1; add [bp+City_Gold_Balance], ax
        City_Gold_Balance += (*food > 0) ? 0 : *food / 2;
        *gold = City_Gold_Balance + Gold_Upkeep;

        *mana = Mana - Mana_Upkeep;



MoX_Data.H
```c
// Game Option - Difficulty
enum e_Difficulty
{
    god_Intro,
    god_Easy,
    god_Normal,
    god_Hard,
    god_Impossible
};
```

MoX_Data.C
```c
/* Intro, Easy, Normal, Hard, Impossible */
/* population_growth, outpost_growth, production, gold, mana, research, food, maintenance */
struct s_DIFFICULTY_MODIFIERS difficulty_modifiers_table[NUM_DIFFICULTY_LEVEL] =
{
    {100, 100, 100, 100, 100, 100, 100, 100},
    {100, 100, 125, 125, 125, 100, 125,  90},
    {150, 150, 150, 150, 150, 100, 150,  75},
    {200, 200, 200, 200, 200, 110, 200,  60},
    {400, 400, 400, 400, 400, 150, 400,  30}
};
```

MoX_Data.H
```c
/*
    computer player wizards - difficulty level modifiers
*/
// sizeof:  10h  16d)
struct s_DIFFICULTY_MODIFIERS
{
    /* 00 */  int16_t population_growth;
    /* 02 */  int16_t outpost_growth;
    /* 04 */  int16_t production;
    /* 06 */  int16_t gold;
    /* 08 */  int16_t mana;
    /* 0A */  int16_t research;
    /* 0C */  int16_t food;
    /* 0E */  int16_t maintenance;
    /* 10 */
};
```


https://masterofmagic.fandom.com/wiki/Forum:Infos_from_Official_Strategy_Guide#Game_difficulty
