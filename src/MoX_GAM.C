/*
    WIZARDS.EXE
    seg050
    
    MoO2: Module: LOADSAVE
*/

#include "MoX.H"

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* memset(), strcat(), strcpy() */

#include <assert.h>



uint8_t _save_gam[123300];


void TST_Validate_GameData(void)
{
    int16_t itr;
    int16_t is_bjorked;
    int8_t * ptr_TBL_Terr_Specials;
    int8_t terrain_special;
    int8_t terrain_special_min;
    int8_t terrain_special_max;

    int8_t * ptr_TBL_Scouting;
    int8_t unexplored;
    int8_t unexplored_min;
    int8_t unexplored_max;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: TST_Validate_GameData()\n", __FILE__, __LINE__);
#endif


// terrain_special_min: -128
// terrain_special_max: 64
    is_bjorked = ST_FALSE;
    ptr_TBL_Terr_Specials = (int8_t *)TBL_Terr_Specials;
    terrain_special_min = 0;
    terrain_special_max = 0;
    for(itr = 0; itr < 4800; itr++)
    {
        terrain_special = *(ptr_TBL_Terr_Specials + itr);
#ifdef STU_DEBUG
        dbg_prn("terrain_special: 0x%02X  %d\n", terrain_special, terrain_special);
#endif
        if(terrain_special < terrain_special_min) { terrain_special_min = terrain_special; }
        if(terrain_special > terrain_special_max) { terrain_special_max = terrain_special; }
        // if( !(terrain_special >= 0) || !(terrain_special <= 9) )
        // {
        //     is_bjorked = ST_TRUE;
        // }
    }
#ifdef STU_DEBUG
        dbg_prn("terrain_special_min: %d\n", terrain_special_min);
        dbg_prn("terrain_special_max: %d\n", terrain_special_max);
#endif
//     if(is_bjorked == ST_TRUE)
//     {
// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d]: TBL_Terr_Specials is bjorked!!\n", __FILE__, __LINE__);
// #endif
//     }


// unexplored_min: 0
// unexplored_max: 15
    is_bjorked = ST_FALSE;
    ptr_TBL_Scouting = (int8_t *)(TBL_Scouting);
    unexplored_min = 0;
    unexplored_max = 0;
    for(itr = 0; itr < 4800; itr++)
    {
        unexplored = *(ptr_TBL_Scouting + itr);
#ifdef STU_DEBUG
        dbg_prn("unexplored: 0x%02X  %d\n", unexplored, unexplored);
#endif
        if(unexplored < unexplored_min) { unexplored_min = unexplored; }
        if(unexplored > unexplored_max) { unexplored_max = unexplored; }
        // if( !(unexplored == 0) || !(unexplored == 1) )
        // {
        //     is_bjorked = ST_TRUE;
        // }
    }
#ifdef STU_DEBUG
        dbg_prn("unexplored_min: %d\n", unexplored_min);
        dbg_prn("unexplored_max: %d\n", unexplored_max);
#endif
//     if(is_bjorked == ST_TRUE)
//     {
// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d]: TBL_Scounting is bjorked!!\n", __FILE__, __LINE__);
// #endif
//     }



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: TST_Validate_GameData()\n", __FILE__, __LINE__);
#endif
}

// WZD o50p01
void Save_SAVE_GAM(int16_t save_gam_idx)
{
    char file_name[20];
    char a_idx[20];
    FILE * file_pointer;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Save_SAVE_GAM( save_gam_idx = %d )\n", __FILE__, __LINE__, save_gam_idx);
#endif


    if(save_gam_idx == ST_UNDEFINED)
    {
        strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
#pragma warning(suppress : 4996)
        itoa(save_gam_idx, a_idx, 10);
        strcpy(file_name, "SAVE");
        strcat(file_name, a_idx);
        strcat(file_name,".GAM");
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: file_name: %s\n", __FILE__, __LINE__, file_name);
#endif

    file_pointer = fopen(file_name, "wb");


// DELETE      fwrite(p0_heroes, 12, 35, file_pointer);
// DELETE      fwrite(p1_heroes, 12, 35, file_pointer);
// DELETE      fwrite(p2_heroes, 12, 35, file_pointer);
// DELETE      fwrite(p3_heroes, 12, 35, file_pointer);
// DELETE      fwrite(p4_heroes, 12, 35, file_pointer);
// DELETE      fwrite(p5_heroes, 12, 35, file_pointer);
    fwrite(_HEROES2[0], sizeof(struct s_HERO), 35, file_pointer);
    fwrite(_HEROES2[1], sizeof(struct s_HERO), 35, file_pointer);
    fwrite(_HEROES2[2], sizeof(struct s_HERO), 35, file_pointer);
    fwrite(_HEROES2[3], sizeof(struct s_HERO), 35, file_pointer);
    fwrite(_HEROES2[4], sizeof(struct s_HERO), 35, file_pointer);
    fwrite(_HEROES2[5], sizeof(struct s_HERO), 35, file_pointer);


    fwrite(&_num_players, 1, 2, file_pointer);
    fwrite(&_landsize , 1, 2, file_pointer);
    fwrite(&_magic, 1, 2, file_pointer);
    fwrite(&_difficulty, 1, 2, file_pointer);
    fwrite(&_cities, 1, 2, file_pointer);
    fwrite(&_units, 1, 2, file_pointer);
    fwrite(&_turn, 1, 2, file_pointer);
    fwrite(&_unit, 1, 2, file_pointer);


    fwrite(_players, 6, 1224, file_pointer);
    // fwrite(_players, PLAYER_COUNT_MAX, sizeof(struct s_WIZARD), file_pointer);

    fwrite(_world_maps, 2, 4800, file_pointer);

    fwrite(UU_TBL_1, 2, 96, file_pointer);
    fwrite(UU_TBL_2, 2, 96, file_pointer);

    fwrite(TBL_Landmasses, 2, 2400, file_pointer);

    fwrite(_NODES, 30, 48, file_pointer);

    fwrite(_FORTRESSES, 6, 4, file_pointer);
    // fwrite(_FORTRESSES, FORTRESS_COUNT_MAX, sizeof(struct s_FORTRESS), file_pointer);

    fwrite(_TOWERS, 6, 4, file_pointer);
    // fwrite(_TOWERS, TOWER_COUNT_MAX, sizeof(struct s_TOWER), file_pointer);

    fwrite(_LAIRS, 102, 24, file_pointer);

    fwrite(_ITEMS, 138, 50, file_pointer);

    fwrite(_CITIES, 100, 114, file_pointer);
    // fwrite(_CITIES, CITY_COUNT_MAX, sizeof(struct s_CITY), file_pointer);

    fwrite(_UNITS, 1009, 32, file_pointer);
    // fwrite(_UNITS, UNIT_COUNT_MAX, sizeof(struct s_UNIT), file_pointer);

    fwrite(TBL_Terr_Specials, 2, 2400, file_pointer);

    fwrite(TBL_Scouting, 2, 2400, file_pointer);

    fwrite(movement_mode_cost_maps, 2, 14400, file_pointer);

    fwrite(events_table, 1, 100, file_pointer);

    fwrite(TBL_Terrain_Flags, 2, 2400, file_pointer);

    fwrite(&grand_vizier, 1, 2, file_pointer);

    fwrite(TBL_Premade_Items, 250, 1, file_pointer);

    fwrite(hero_names_table, 12, 35, file_pointer);


    fclose(file_pointer);

    if( (save_gam_idx > 0) && (save_gam_idx < 9) )
    {
        magic_set.Have_Save[save_gam_idx] = ST_TRUE;
        file_pointer = fopen("MAGIC.SET", "wb");
        fwrite(&magic_set, 466, 1, file_pointer);
        fclose(file_pointer);
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Save_SAVE_GAM( save_gam_idx = %d )\n", __FILE__, __LINE__, save_gam_idx);
#endif

}


// WZD o50p02
void Load_SAVE_GAM(int16_t save_gam_idx)
{
    char file_name[LEN_FILE_NAME];
    char a_idx[LEN_TEMP_STRING];
    FILE * file_pointer;
    int32_t file_size;
    int16_t file_size_flag;
    long file_pointer_position;

    if(save_gam_idx == ST_UNDEFINED)
    {
        strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
#pragma warning(suppress : 4996)
        itoa(save_gam_idx, a_idx, 10);
        strcpy(file_name, "SAVE");
        strcat(file_name, a_idx);
        strcat(file_name,".GAM");
    }

    file_size = LOF(file_name);
    assert(file_size != 0);

    if(file_size != 57764)
    {
        file_size_flag = ST_TRUE;
    }

    file_pointer = fopen(file_name, "rb");
    assert(file_pointer != NULL);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 0);  // BoF

    fread(_HEROES2[0], sizeof(struct s_HERO), 35, file_pointer);  // 12 * 35 = 420
    fread(_HEROES2[1], sizeof(struct s_HERO), 35, file_pointer);  // 12 * 35 = 420
    fread(_HEROES2[2], sizeof(struct s_HERO), 35, file_pointer);  // 12 * 35 = 420
    fread(_HEROES2[3], sizeof(struct s_HERO), 35, file_pointer);  // 12 * 35 = 420
    fread(_HEROES2[4], sizeof(struct s_HERO), 35, file_pointer);  // 12 * 35 = 420
    fread(_HEROES2[5], sizeof(struct s_HERO), 35, file_pointer);  // 12 * 35 = 420


    file_pointer_position = ftell(file_pointer);  // 12 * 35 * 6 = 420 * 6 = 2520
    assert(file_pointer_position == 2520);  // BoF + (12 * 35 * 6)

    fread(&_num_players, 1, 2, file_pointer);
    fread(&_landsize, 1, 2, file_pointer);
    fread(&_magic, 1, 2, file_pointer);
    fread(&_difficulty, 1, 2, file_pointer);
    fread(&_cities, 1, 2, file_pointer);
    fread(&_units, 1, 2, file_pointer);
    fread(&_turn, 1, 2, file_pointer);
    fread(&_unit, 1, 2, file_pointer);

    file_pointer_position = ftell(file_pointer);  // 2 + 2 + 2 + 2 + 2 + 2 + 2 + 2 = 16
    assert(file_pointer_position == 2536);  // 7344

    fread(_players, 6, 1224, file_pointer);
    // fread(_players, PLAYER_COUNT_MAX, sizeof(struct s_WIZARD), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 9880);

    // memset(_world_maps, 0, 9600); // 602 * 16 = 9632
    // � unhandled exception: invalid parameter ?
    fread(_world_maps, 2, 4800, file_pointer); // 9600

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 19480);

    fread(UU_TBL_1, 2, 96, file_pointer);
    fread(UU_TBL_2, 2, 96, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 19864);

    fread(TBL_Landmasses, 2, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 24664);

    fread(_NODES, 30, 48, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 26104);

    fread(_FORTRESSES, 6, 4, file_pointer);
    // fread(_FORTRESSES, FORTRESS_COUNT_MAX, sizeof(struct s_FORTRESS), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 26128);

    fread(_TOWERS, 6, 4, file_pointer);
    // fread(_TOWERS, TOWER_COUNT_MAX, sizeof(struct s_TOWER), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 26152);

    fread(_LAIRS, 102, 24, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 28600);

    fread(_ITEMS, 138, 50, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 35500);

    fread(_CITIES, 100, 114, file_pointer);
    // fread(_CITIES, CITY_COUNT_MAX, sizeof(struct s_CITY), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 46900);

    fread(_UNITS, 1009, 32, file_pointer);
    // fread(_UNITS, UNIT_COUNT_MAX, sizeof(struct s_UNIT), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 79188);

    fread(TBL_Terr_Specials, 2, 2400, file_pointer);  // 1 byte per world map square per plane; 60*40 world map * 2 planes

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 83988);

    fread(TBL_Scouting, 2, 2400, file_pointer);  // 1 byte per world map square per plane; 60*40 world map * 2 planes

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 88788);

    fread(movement_mode_cost_maps, 2, 14400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 117588);

    fread(events_table, 1, 100, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 117688);

    fread(TBL_Terrain_Flags, 2, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 122488);

    fread(&grand_vizier, 1, 2, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 122490);

    fread(TBL_Premade_Items, 250, 1, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 122740);

    // if (file_size_flag == ST_TRUE) { MEM_Clear_Far(hero_names_table, 545); } else { ... }
    fread(hero_names_table, 16, 35, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 123300);

    fclose(file_pointer);

}



void Read_SAVE_GAM(void)
{
    FILE * file_pointer;
    file_pointer = fopen("SAVETEST.GAM", "rb");
    fread(&_save_gam, 123300, 1, file_pointer);
    fclose(file_pointer);
}

void Write_SAVE_GAM(void)
{
    FILE * file_pointer;
    file_pointer = fopen("SAVETEST.GAM", "wb");
    fwrite(&_save_gam, 123300, 1, file_pointer);
    fclose(file_pointer);
}
