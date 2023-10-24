



food
production
gold
mana
research

production
maintenance

income
reserves




Get_Incomes()

    Get_Power_Incomes(&Mana, &City_Food_Surplus, &City_Gold_Balance, player_idx);

    Mana_Upkeep = WIZ_TotalUpkeep_Mana(player_idx);
    Gold_Upkeep = WIZ_ArmyUpkeep_Gold(player_idx);
    Food_Upkeep = WIZ_ArmyUpkeep_Food(player_idx);

Per City
    City_Gold_Balance = (_CITIES[itr_cities].Gold - _CITIES[itr_cities].Upkeep);
    City_Food_Surplus = (_CITIES[itr_cities].Food - _CITIES[itr_cities].Pop_K);

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
