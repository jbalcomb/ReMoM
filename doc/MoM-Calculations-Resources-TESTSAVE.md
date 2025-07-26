
430 GP
315 MP
142 Gold
  5 Food
 35 Mana

Player_Resource_Income_Total()
    Player_Magic_Power_Income_Total(&mana_income, &food_income, &gold_income, player_idx);
    mana_expense = Player_Armies_And_Enchantments_Mana_Upkeep(player_idx);
    gold_expense = Player_Armies_Gold_Upkeep(player_idx);
    food_expense = Player_Armies_Food_Upkeep(player_idx);
mana_income:   39
mana_expense:  95
gold_expense:  70
food_expense:  64
gold_income:  210
food_income:   69
...
food_total: 5
gold_income:  212
gold_total:   142
mana_total:   -56



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
the 44 here is what shows on the CityList Screen, which is the Net Gold Income on the City Screen  
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
