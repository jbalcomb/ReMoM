
STU::Debug::GameData



***SAVETEST.GAM***
SAVETEST--NewGame-ShareeKlackon.GAM
2024-07-21-05-12




'Continue'
...|-> Name Starting City
"Fa-rul"
(2) Klackon Swordsmen
unit_idx 0
unit_idx 2
wx,wy,wp 42,16,0

NameStartingCity_Dialog_Popup(0);
...so, city_idx 0

immediately crashes on changing city name from "Keltok" to "Fa-rul"
    race                    9
    wx,wy,wp                42,16,0
    size                    1
    population              4
    farmer_count            3
    sold_building       1
    pad2B_17h               0
    Pop_10s                 0
    PlayerBits              0
    pad2B_1Bh               0
    construction            2
    bldg_cnt                0
    bldg_status[]           {  0, -1, -1,  1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1 }
    enchantments            { 0 }
    production_units        5
    Prod_Accu               0
    gold_units              4
    building_maintenance    2
    mana_units              10
    research_units          0
    food_units              6
    road_connections[]      { 0 }

Magic
MP  3
RP  No Spell
SP  3
Casting Skill   20(20)
Magic Reserve   0
Power Base      10
Casting         None
Researching     None
Summon To       Shen-khi
Overland Enchantments   Eternal Night

25 gP
 0 MP
 1 Gold
0 Food
-12 Mana


Main_Screen_Draw_Summary_Window()
Player_Resource_Income_Total()
Player_Magic_Power_Income_Total(&mana_income, &food_income, &gold_income, player_idx);

mana_expense = Player_Armies_And_Enchantments_Mana_Upkeep(player_idx);
    for(itr_units = 0; itr_units < _units; itr_units++)
        if(_UNITS[itr_units].owner_idx == player_idx)
            mana_upkeep_cost += Unit_Mana_Upkeep(itr_units);
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        mana_upkeep_cost += Player_City_Enchantments_Upkeep(itr_cities, player_idx);
    mana_upkeep_cost += Player_Overland_Enchantments_Upkeep(player_idx);
    if(_players[player_idx].channeler > 0)
        mana_upkeep_cost = (mana_upkeep_cost / 2);  /* -50% */

*mana_total = mana_income - mana_expense;




Capture, Check, Release
    Cities
    Units
    Heroes
    Fortresses
    Towers
    Nodes
    Lairs






// Nathan Baggs
writer
reader
