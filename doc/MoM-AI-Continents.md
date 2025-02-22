
Continent
Continents
Land Mass
Land Masses
Landmass
Landmasses
Game Options: "Land Size"



¿ structures and data behind AISTAGE ?

ALLOC.C
Allocate_Data_Space()
    AI_MainWarConts[0] = ( (int16_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x17E8) );  // 12 bytes
    AI_MainWarConts[1] = ( (int16_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x17F4) );  // 12 bytes
    AI_CONTX_Reevals   = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1800) );  // 16 bytes
    AI_NewColConts[0]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1860) );  // 20 bytes
    AI_NewColConts[1]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1874) );  // 20 bytes
    AI_NewColTgtXs[0]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x1888) );  // 20 bytes
    AI_NewColTgtXs[1]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x189C) );  // 20 bytes
    AI_NewColTgtYs[0]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18B0) );  // 12 bytes
    AI_NewColTgtYs[1]  = ( (uint8_t *) ( (void *) (&_players[5].spells_list[0]) ) + (0x17E8 - 0x18BC) );  // 12 bytes



EMM_Map_CONTXXX__WIP()
EMM_Map_DataH()



CONTX_FirstTiles[2][60]
CONTX_TileChain[2][1600]

CONTX_1stLoadTs[2][60]
CONTX_LoadTChain[2][]

AI_NewColTgtYs[2]
AI_NewColTgtXs[2]
_ai_reevaluate_continents_countdown
AI_NewColConts[2]
AI_MainWarConts[2]



AIDUDES.C
EMM_Map_CONTXXX__WIP()
    CONTX_TileXs[0]           =  (int8_t *)(&EMM_PageFrame[0] + (   0 * SZ_PARAGRAPH_B));  //  100 -    0 = 100 * 1 PR = 1600 B
    CONTX_TileXs[1]           =  (int8_t *)(&EMM_PageFrame[0] + ( 100 * SZ_PARAGRAPH_B));  //  200 -  100 = 100 * 1 PR = 1600 B
    CONTX_TileYs[0]           =  (int8_t *)(&EMM_PageFrame[0] + ( 200 * SZ_PARAGRAPH_B));  //  300 -  200 = 100 * 1 PR = 1600 B
    CONTX_TileYs[1]           =  (int8_t *)(&EMM_PageFrame[0] + ( 300 * SZ_PARAGRAPH_B));  //  400 -  300 = 100 * 1 PR = 1600 B
    CONTX_TileChain[0]        = (int16_t *)(&EMM_PageFrame[0] + ( 400 * SZ_PARAGRAPH_B));  //  600 -  400 = 200 * 1 PR = 3200 B
    CONTX_TileChain[1]        = (int16_t *)(&EMM_PageFrame[0] + ( 600 * SZ_PARAGRAPH_B));  //  800 -  600 = 200 * 1 PR = 3200 B
    CONTX_FirstTiles[0]       = (int16_t *)(&EMM_PageFrame[0] + ( 800 * SZ_PARAGRAPH_B));  //  825 -  800 =  25 * 1 PR =  400 B
    CONTX_FirstTiles[1]       = (int16_t *)(&EMM_PageFrame[0] + ( 825 * SZ_PARAGRAPH_B));  //  850 -  825 =  25 * 1 PR =  400 B
    _ai_own_stack_wx          = (&EMM_PageFrame[0] + ( 850 * SZ_PARAGRAPH_B));  //  860 -  850 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_wy          = (&EMM_PageFrame[0] + ( 860 * SZ_PARAGRAPH_B));  //  870 -  860 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_wp          = (&EMM_PageFrame[0] + ( 870 * SZ_PARAGRAPH_B));  //  880 -  870 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_type        = (&EMM_PageFrame[0] + ( 880 * SZ_PARAGRAPH_B));  //  890 -  880 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_unit_count  = (&EMM_PageFrame[0] + ( 890 * SZ_PARAGRAPH_B));  //  900 -  890 =  10 * 1 PR =  160 B  80 2-byte values
    CONTX_NmeStrMap[0]        = (&EMM_PageFrame[0] + ( 900 * SZ_PARAGRAPH_B));  // 1500 -  900 = 600 * 1 PR = 9600 B
    CONTX_NmeStrMap[1]        = (&EMM_PageFrame[0] + (1500 * SZ_PARAGRAPH_B));  // 2100 - 1500 = 600 * 1 PR = 9600 B
    CONTX_LoadTileXs[0]       =  (int8_t *)(&EMM_PageFrame[0] + (2100 * SZ_PARAGRAPH_B));  // 2200 - 2100 = 100 * 1 PR = 1600 B
    CONTX_LoadTileXs[1]       =  (int8_t *)(&EMM_PageFrame[0] + (2200 * SZ_PARAGRAPH_B));  // 2300 - 2200 = 100 * 1 PR = 1600 B
    CONTX_LoadTileYs[0]       =  (int8_t *)(&EMM_PageFrame[0] + (2300 * SZ_PARAGRAPH_B));  // 2400 - 2300 = 100 * 1 PR = 1600 B
    CONTX_LoadTileYs[1]       =  (int8_t *)(&EMM_PageFrame[0] + (2400 * SZ_PARAGRAPH_B));  // 2500 - 2400 = 100 * 1 PR = 1600 B
    CONTX_LoadTChain[0]       = (int16_t *)(&EMM_PageFrame[0] + (2500 * SZ_PARAGRAPH_B));  // 2700 - 2500 = 200 * 1 PR = 3200 B
    CONTX_LoadTChain[1]       = (int16_t *)(&EMM_PageFrame[0] + (2700 * SZ_PARAGRAPH_B));  // 2900 - 2700 = 200 * 1 PR = 3200 B
    CONTX_1stLoadTs[0]        = (int16_t *)(&EMM_PageFrame[0] + (2900 * SZ_PARAGRAPH_B));  // 3000 - 2900 = 100 * 1 PR = 1600 B
    CONTX_1stLoadTs[1]        = (int16_t *)(&EMM_PageFrame[0] + (3000 * SZ_PARAGRAPH_B));  // 3200 - 3000 = 200 * 1 PR = 3200 B



LoadScr.C
Loaded_Game_Update()
    /*
        BEGIN:  AI CONT / MOVE
    */
    for(itr = 0; itr < 100; itr++)
        TBL_OvlMovePaths_EMS[itr] = ST_UNDEFINED;

    CRP_UNIT_OverlandPath = ST_UNDEFINED;

    CONTX_CreateChains__WIP();

    CONTX_CreateLChains__WIP();

    for(itr = 0; itr < _num_players; itr++)
        _ai_reevaluate_continents_countdown[itr] = ST_FALSE;



## CONTX_CreateChains__WIP()

...two arrays, per plane, one with 60 elements - one per continent - and one with 1600 elements...
...starts with setting all elements to ST_UNDEFINED...
...loops over wp and wy and wx...
...loops until it finds a landmass...
...sets that as the map square index in the array that only has 60 elements...
...sets the value in the landmasses array to the current count of matched map squares...

    for(wp = 0; wp < NUM_PLANES; wp++)
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
            for(wx = 0; wx < WORLD_WIDTH; wx++)


Q: What are we trying to accomplish?
A: 

Q: What do we get out of this?
A: My best guess is that we end up with an array for each continent that holds the value we need to index another array that holds runs of world map indices, running from L-R and T-B, where that array's indices can be used to index into two other arrays that hold the wx and wy of said world map squares.



## AI_Continent_Eval__WIP()

AIDUDES.C
// WZD o145p03

    for(itr = 0; itr < NUM_LANDMASSES; itr++)
        AI_Cont_Own_Str[0][itr] = 0;
        AI_Cont_Own_Str[1][itr] = 0;
        CRP_AI_Cont_Nme_Str[0][itr] = 0;
        CRP_AI_Cont_Nme_Str[1][itr] = 0;
        AI_Cont_Own_Val[0][itr] = 0;
        AI_Cont_Own_Val[1][itr] = 0;
        AI_Cont_Nme_Val[0][itr] = 0;
        AI_Cont_Nme_Val[1][itr] = 0;

AI_Own_Stack_Count
AI_Enemy_Stack_Count
...set in AI_Player_Calculate_Target_Values()
AI_Own_Stacks[]
AI_Enemy_Stacks[]

AI_Cont_Own_Str[][]
CRP_AI_Cont_Nme_Str[][]
...set in AI_Continent_Eval__WIP()


...set in AI_Player_Calculate_Target_Values()
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        AI_NME_Garrison_Strs[itr_cities] = 0;
        AI_Own_Garr_Strs[itr_cities] = 0;
        AI_Enemy_City_Values[itr_cities] = 0;
        AI_Own_City_Values[itr_cities] = 0;
