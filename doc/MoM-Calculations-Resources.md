


TESTSAVE.GAM  

Banbury     HM  21  44  17  
Bremen      HM  21  90  95  
Birmingham  HM  17  10  24  
New Haven   HM  22  57  17  
York        HM  13   7  62  
Cantebury   HM   6   2  21  

44+90+10+57+7+2=210
+2 excess food  
-70 upkeep  
210+2-70=142  
430 GR + 142 GI = 572 GR

Banbury     HM  21  44  17  
Bremen      HM  21  90  95  
Birmingham  HM  17  10  24  
New Haven   HM  22  57  17  
York        HM  13   7  62  
Cantebury   HM   6   2  21  

44+90+10+57+7+2=210  
+2 excess food  
-70 upkeep  
210+2-70=142  

_CITIES[0]  Banbury  
_CITIES[13] Bremen  
_CITIES[33] Birmingham  
_CITIES[48] New Haven  
_CITIES[52] York  
_CITIES[54] Cantebury  

_CITIES[0]  Banbury  
[x] 96 - 52 = 44  

_CITIES[13] Bremen  
159 - 69 = 90

got 169 - 69 = 100  
probably should be 159  
g2g  Surveyor says +263%  
g2g  pop * tax = 31  
g2g  map_square_gold_income: 0  
Trade Goods
got production_units 115, should be 95  

Surveyor shows +173% Prod Bonus

21 pop
5 farmers
16 workers
16 * 4 / 2 = 32
5 / 2 + 1 = 3

8 terrain
8 sawmill
8 foresters
17 miners
17 mechanicians

city screen shows 95
but, break-down shows 93
32+3+8+8+8+17+17=93




_CITIES[33] Birmingham  
[x] 77 - 67 = 10

_CITIES[48] New Haven  
[x] 102 - 45 = 57

_CITIES[52] York  
[x] 64 - 57 = 7  

_CITIES[54] Cantebury  
[x] 22 - 20 = 2



_CITIES[52] York  
64 - 57 = 7  
19 + 7 + 9 + 9 + 19 = 63  

Surveyor: Gold Bonus + 239%  
100 + 50 + 50 = 200  
need 39 from city_road_trade_bonus + map_square_gold_bonus  
has 4 shore  
1 road, connected to enemy city, with pop ~24  


```
_CITIES[52].road_connections[0]:     4  
_CITIES[52].road_connections[1]:    32  
_CITIES[52].road_connections[2]:     0  
_CITIES[52].road_connections[3]:    48  
_CITIES[52].road_connections[4]:    64  
_CITIES[52].road_connections[5]:    36  
_CITIES[52].road_connections[6]:     0  
_CITIES[52].road_connections[7]:     0  
_CITIES[52].road_connections[8]:     0  
_CITIES[52].road_connections[9]:     0  
_CITIES[52].road_connections[10]:    0  
_CITIES[52].road_connections[11]:    0  
_CITIES[52].road_connections[12]:    0  
```









_CITIES[0]
 65 - 52 =  13
101 - 21 =  80
_CITIES[13]
113 - 69 =  44
 29 - 21 =   8
_CITIES[33]
 52 - 67 = -15
 83 - 17 =  66
_CITIES[48]
 69 - 45 =  14
106 - 22 =  84
_CITIES[52]
 42 - 57 = -15
 16 - 13 =   3
_CITIES[54]
 14 - 20 =  -6
  7 - 6 =    1

13+44-15+14-15-6=35

Banbury (capital)
Food: 21 + 29 = 50
Gold: 52 + 44 = 96
(31 + 3 + 15 + 15 + 31 = 95)
the 44 here is what shows on the CityList Screen, which is the Net Goild Income on the City Screen  
So, the 52 for mainenance looks right, but gold units should be ~96?

_CITIES[0].gold_units: 65  
_CITIES[0].building_maintenance: 52  
_CITIES[0].production_units: 22  
_CITIES[0].food_units: 101  
_CITIES[0].population: 21  

_CITIES[13].gold_units: 113  
_CITIES[13].building_maintenance: 69  
_CITIES[13].production_units: 115  
_CITIES[13].food_units: 29  
_CITIES[13].population: 21  

_CITIES[33].gold_units: 52  
_CITIES[33].building_maintenance: 67  
_CITIES[33].production_units: 29  
_CITIES[33].food_units: 83  
_CITIES[33].population: 17  

_CITIES[48].gold_units: 69  
_CITIES[48].building_maintenance: 45  
_CITIES[48].production_units: 24  
_CITIES[48].food_units: 106  
_CITIES[48].population: 22  

_CITIES[52].gold_units: 42  
_CITIES[52].building_maintenance: 57  
_CITIES[52].production_units: 72  
_CITIES[52].food_units: 16  
_CITIES[52].population: 13  

_CITIES[54].gold_units: 14  
_CITIES[54].building_maintenance: 20  
_CITIES[54].production_units: 26  
_CITIES[54].food_units: 7  
_CITIES[54].population: 6  





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
