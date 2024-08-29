


# bldg_data_table

CTY_GrandVizier__STUB()
ovr157:0FB0 26 8B 47 32                                     mov     ax, [es:bx+s_BLDG_DATA.Category]
26 
8B 
47 
32  off of 'Category' in 'struct s_BLDG_DATA'
Occurrances of binary: 26 8B 47 32
ovr157:056C AI_CTY_SetProduction  mov     ax, [es:bx+s_BLDG_DATA.Category]
ovr157:0FB0 CTY_GrandVizier__STUB mov     ax, [es:bx+s_BLDG_DATA.Category]



MoX_Data.C
// WZD dseg:938C
struct s_BLDG_DATA * bldg_data_table;

MoX_Data.H
// drake178: struc BLD_REC
// sizeof:  34h  52d
struct s_BLDG_DATA
{
    /* 00 */  char name[LEN_BUILDING_NAME];
    /* 14 */  // int16_t reqd_bldg_1;       // bldg_idx  >= 100 is Terrain Type
    union
    {
        int16_t reqd_bldg_1;                // bldg_idx
        int16_t reqd_terrain;
    };
    /* 16 */  int16_t reqd_bldg_2;          // bldg_idx
    /* 18 */  int16_t replace_bldg;         // bldg_idx
    /* 1A */  int16_t Grants20XP;
    /* 1C */  int16_t Grants60XP;
    /* 1E */  int16_t Alchemist;
    /* 20 */  int16_t maintenance_cost;  // in gold units  AKA upkeep cost
    /* 22 */  int16_t AIVal_Growth;
    /* 24 */  int16_t Unknown_24h;
    /* 26 */  int16_t Unknown_26h;
    /* 28 */  int16_t AIVal_Religion;
    /* 2A */  int16_t AIVal_Research;
    /* 2C */  int16_t construction_cost;  // in production units
    /* 2E */  int16_t Unknown_2Eh;
    /* 30 */  int16_t G_Animation;  // ¿ 'size & position' for Cityscape ?
    /* 32 */  int16_t Category;
    /* 34 */
};

### "Category"
0:  None, Trade, Housing
1:  Marketplace, Bank, Merchants Guild, Maritime Guild
2:  Shrine, Temple, Parthenon, Cathedral
3:  Library, Sages Guild, Oracle, Alchemists Guild, University, Wizards Guild
4:  Barracks, Armory, Fighters Guild, Armorers Guild, War College, Smithy, Stables, Fantastic Stable, Mechanicians Guild, City Walls
5:  Sawmill, Foresters Guild, Builders Hall, Miners Guild
6:  Granary, Farmers Market, Animists Guild
7:  Ship Wrights Guild, Ship Yard
¿ `effect` ?

gold
mana
food
production
research
unrest
military
boats


1:  Marketplace, Bank, Merchants Guild, Maritime Guild
Marketplace         + 50% gold
Bank                Gold + 50%
Merchants Guild     + 100% gold
Maritime Guild      ¿ BUGBUG ?

2:  Shrine, Temple, Parthenon, Cathedral
Shrine              +1 mana     decreases unrest by 1
Temple              +2 mana     Decreases unrest by 1
Parthenon           +3 mana     Decreases unrest by 1
Cathedral           + 4 mana    Decreases unrest by 1

3:  Library, Sages Guild, Oracle, Alchemists Guild, University, Wizards Guild
Library             + 2 research points
Sages Guild         +3 research points
Oracle              -2 unrest
Alchemists Guild    + 3 mana
University          +5 research points
Wizards Guild       -3 mana; +8 research points

5:  Sawmill, Foresters Guild, Builders Hall, Miners Guild
Sawmill             +25% to production from forest square
Foresters Guild     food harvest + 2; 25% production
Builders Hall       N/A
Miners Guild        Increases bonus for minerals by 50%, including magic power; +50% production

6:  Granary, Farmers Market, Animists Guild
Granary             +2 food harvest; +20 people per turn
Farmers Market      +3 food; + 30 people per turn
Animists Guild      Food harvest + 1; unrest -1

¿ `codification` ?
Animists Guild would suggestion food > unrest
Foresters Guild would suggest prod > food
Oracle would suggest research > mana/unrest
Alchemists Guild would suggest category 2 is about unrest not mana  (¿ religion vs. magic ?)



Barracks              30
Armory                80
Fighter’s Guild      200
Armorer’s Guild      350
War College          500
Smithy                40
Stable                80
Fantastic Stable     600
Mechanician’s Guild  600
City Walls           150



*Fantastic Stable*
Barracks  Armory  Fighter’s Guild  Armorer’s Guild  Fantastic Stable
                                   Animist’s Guild
                                   Stable

*Animist’s Guild*
    Stable & Temple 

*Mechanician’s Guild*
category 4 because is allows 'Catapults'

7:  Ship Wrights Guild, Ship Yard
Maritime Guild

Ship Wrights Guild  100   Trirems
Ship Yard           200   Galleys, Air Ships
Maritime Guild      400   Warships

A ship wright's guild brings together skilled craftsmen to build small boats.
A shipyard provides a city with the necessary equipment and workers to produce medium-sized ships.
A maritime guild produces large ships by training, organizing, and supporting experienced ship builders.





BUILDDAT.LBX
Entries:    1
Type:       5
record count    36
record size     52



LOADER.C

Load_BUILDDAT()
    bldg_data_table = (struct s_BLDG_DATA *)LBX_Load_Data(builddat_lbx_file, 0, 0, 36, 52);




## MoO2 Building

```c
Load_Colony_Screen_Seg_()
    |-> Load_Build_Anims_()
        for(itr = 0; itr < 7; itr++>)
        {
            if(itr == 0)
            {
                _anims__COLBLDG[itr] = Far_Reload("COLBLDG.LBX", itr, _screen_seg);
            }
            else
            {
                _anims__COLBLDG[itr] = Far_Reload_Next("COLBLDG.LBX", itr, _screen_seg);
            }
        }
        _bldg_bitm = Allocate_NexT_Block(_screen_seg, 8100)
    _bldg_bitm_n = ST_UNDEFINED
    _bldg_bitm = Allocate_NexT_Block(_screen_seg, 14432)
```

Module: COLBLDG


Module: COLBCACH
Allocate_Bldg_Cache_()
_building_indexes
_n_building_indexes
_active_prod
_scanned_prod
_colony_auto_building



## 1oom

C:\STU\developp\1oom\src\game\game_turn.c
    game_turn_build_ind()
        cost = e->factory_adj_cost;
        prod = game_adjust_prod_by_special((p->slider[PLANET_SLIDER_IND] * p->prod_after_maint) / 100, p->special);
        prod += p->bc_to_factory;
        game_add_planet_to_build_finished(g, i, owner, FINISHED_FACT);

¿¿¿
C:\STU\developp\1oom\src\game\game_ai_classic.c
game_ai_classic_turn_p3()
ai_p3_tbl_w[]
???

¿ 'Objective' is built into the race, in MoO1 ?




Manual
(PDF Page 127)
TABLE G
BUILDING TYPES
BUILDING           COST OF       REQUIRES:     ALLOWS          ALLOWS  EFFECTS
TYPE               PRODUCTION /                BUILDING:       TROOPS
                   UPKEEP                      
Alchemist’s Guild  250/3         Sage’s Guild  Wizard’s Guild          + 3 mana; All new troops get + 1 to hit, magic weapons

1. Alchemist’s guilds allow troops to be equipped with superior magic weapons (with attack strength bonuses) if city  
   resources include mithril or adamantium.  
2. Animist’s guilds increase the rate of food production, not total food produced by a city (i.e., farmers can harvest more  
   than normal). The healing rate for having an animist’s guild in a city is 50% higher than normal. Normally, units heal 5% of  
   their hit points per game turn when they are outside of cities and 10% when they are on city squares. The presence of an  
   animist’s guild raises the healing rate to 15% of a unit’s total hit points per game turn.  
3. Barracks are required if a city is to maintain any normal troops. The only troops that can be maintained without barracks  
   are engineers, galleys, magicians, priests, shamans, spearmen, triremes, warlocks and warships.  
4. Growth is a percentage bonus based on the proportion of workers to total population. if a city has one worker in a population  
   of four, the housing bonus is 25%. Beyond this, sawmills add 10% growth, and builder’s halls add 15% growth.  
