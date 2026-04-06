/*
    WIZARDS.EXE
    seg050
    
    MoO2: Module: LOADSAVE
*/

// #ifdef STU_DEBUG
#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_TST.h"
// #endif

#include "../../ext/stu_compat.h"

#include "DOS.h"
#include "MOM_DAT.h"
#include "MOX_DAT.h"  /* _players[]; _screen_seg; */
#include "MOX_SET.h"  /* magic_set */
#include "Util.h"

#include "../../platform/include/Platform.h"

#include <assert.h>     /* assert() */
#include <math.h>       /* sqrt() */
#include <stddef.h>     /* NULL */
#include <stdio.h>      /* FILE; fclose(), fread(), fseek(); */
#include <stdlib.h>
#include <string.h>     /* memset(), stu_strcat(), stu_strcpy(); */
#include "LOADSAVE.h"



uint8_t _save_gam[LEN_SAVE_GAM_FILE];



// WZD o50p01
void Save_SAVE_GAM(int16_t save_gam_idx)
{
    char file_name[LEN_FILE_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FILE * file_pointer = NULL;
    long file_pointer_position = 0;

    if(save_gam_idx == ST_UNDEFINED)
    {
        stu_strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
        stu_itoa((save_gam_idx + 1), temp_string, 10);
        stu_strcpy(file_name, "SAVE");
        stu_strcat(file_name, temp_string);
        stu_strcat(file_name,".GAM");
    }

    file_pointer = stu_fopen_ci(file_name, "wb");
    assert(file_pointer != NULL);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 0);

    stu_fwrite(_HEROES2[0], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fwrite(_HEROES2[1], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fwrite(_HEROES2[2], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fwrite(_HEROES2[3], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fwrite(_HEROES2[4], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fwrite(_HEROES2[5], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 2520);

    stu_fwrite(&_num_players, 1, 2, file_pointer);
    stu_fwrite(&_landsize , 1, 2, file_pointer);
    stu_fwrite(&_magic, 1, 2, file_pointer);
    stu_fwrite(&_difficulty, 1, 2, file_pointer);
    stu_fwrite(&_cities, 1, 2, file_pointer);
    stu_fwrite(&_units, 1, 2, file_pointer);
    stu_fwrite(&_turn, 1, 2, file_pointer);
    stu_fwrite(&_unit, 1, 2, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 2536);

    stu_fwrite(_players, NUM_PLAYERS, 1224, file_pointer);
    // stu_fwrite(_players, PLAYER_COUNT_MAX, sizeof(struct s_WIZARD), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 9880);

    stu_fwrite(_world_maps, NUM_PLANES, 4800, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 19480);

    stu_fwrite(UU_TBL_1, NUM_PLANES, 96, file_pointer);
    stu_fwrite(UU_TBL_2, NUM_PLANES, 96, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 19864);

    stu_fwrite(_landmasses, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 24664);

    stu_fwrite(_NODES, NUM_NODES, 48, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 26104);

    stu_fwrite(_FORTRESSES, NUM_FORTRESSES, 4, file_pointer);
    // stu_fwrite(_FORTRESSES, FORTRESS_COUNT_MAX, sizeof(struct s_FORTRESS), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 26128);

    stu_fwrite(_TOWERS, NUM_TOWERS, 4, file_pointer);
    // stu_fwrite(_TOWERS, TOWER_COUNT_MAX, sizeof(struct s_TOWER), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 26152);

    // stu_fwrite(_LAIRS, NUM_LAIRS, 24, file_pointer);
    /* HACK */  stu_fwrite(_LAIRS, 102, 24, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 28600);

    stu_fwrite(_ITEMS, 138, 50, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 35500);

    stu_fwrite(_CITIES, NUM_CITIES, 114, file_pointer);
    // stu_fwrite(_CITIES, CITY_COUNT_MAX, sizeof(struct s_CITY), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 46900);

    stu_fwrite(_UNITS, NUM_UNITS, 32, file_pointer);
    // stu_fwrite(_UNITS, UNIT_COUNT_MAX, sizeof(struct s_UNIT), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 79188);

    stu_fwrite(_map_square_terrain_specials, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 83988);

    stu_fwrite(_square_explored, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 88788);

    // TODO  sizeof(struct s_MOVE_MODE_COST_MAPS)
    stu_fwrite(movement_mode_cost_maps, NUM_PLANES, 14400, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 117588);

    stu_fwrite(events_table, 1, 100, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 117688);

    stu_fwrite(_map_square_flags, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 122488);

    stu_fwrite(&grand_vizier, 1, 2, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 122490);

    stu_fwrite(TBL_Premade_Items, 250, 1, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 122740);

    stu_fwrite(hero_names_table, sizeof(struct s_INACTV_HERO), NUM_HERO_TYPES, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == LEN_SAVE_GAM_FILE);

    fclose(file_pointer);

    if(save_gam_idx != ST_UNDEFINED)  /* HACK */
    {
        // � don't save settings for continue/auto-save ?
        if(save_gam_idx < 8)
        {
            magic_set.Have_Save[save_gam_idx] = ST_TRUE;
            file_pointer = stu_fopen_ci("MAGIC.SET", "wb");
            stu_fwrite(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_pointer);
            fclose(file_pointer);
        }
    }

}


// WZD o50p02
void Load_SAVE_GAM(int16_t save_gam_idx)
{
    char file_name[LEN_FILE_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FILE * file_pointer = NULL;
    int32_t file_size = 0;
    int16_t file_size_flag = 0;
    long file_pointer_position = 0;

#ifdef STU_DEBUG
    printf("DEBUG: [%s, %d]: BEGIN: Load_SAVE_GAM(%d)\n", __FILE__, __LINE__, save_gam_idx);
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_SAVE_GAM(%d)\n", __FILE__, __LINE__, save_gam_idx);
    trc_prn("DEBUG: [%s, %d]: BEGIN: Load_SAVE_GAM(%d)\n", __FILE__, __LINE__, save_gam_idx);
#endif

    if(save_gam_idx == ST_UNDEFINED)
    {
        stu_strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
        stu_itoa((save_gam_idx + 1), temp_string, 10);
        stu_strcpy(file_name, "SAVE");
        stu_strcat(file_name, temp_string);
        stu_strcat(file_name,".GAM");
    }

    file_size = LOF(file_name);
    // DONT  assert(file_size != 0);  crashes if there are no game-save files, like on first run
    
    // dx 1, ax 57764 === 65536 + 57764 = 123300
    if(file_size != LEN_SAVE_GAM_FILE)
    {
        file_size_flag = ST_TRUE;
    }


    // TODO  gfile_pointer = gfopen(file_name, "rb");
    file_pointer = stu_fopen_ci(file_name, "rb");
    assert(file_pointer != NULL);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 0);  // BoF

    stu_fread(_HEROES2[0], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fread(_HEROES2[1], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fread(_HEROES2[2], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fread(_HEROES2[3], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fread(_HEROES2[4], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    stu_fread(_HEROES2[5], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 2520);

    stu_fread(&_num_players, 1, 2, file_pointer);
    stu_fread(&_landsize, 1, 2, file_pointer);
    stu_fread(&_magic, 1, 2, file_pointer);
    stu_fread(&_difficulty, 1, 2, file_pointer);
    stu_fread(&_cities, 1, 2, file_pointer);
    stu_fread(&_units, 1, 2, file_pointer);
    stu_fread(&_turn, 1, 2, file_pointer);
    stu_fread(&_unit, 1, 2, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 2536);

    stu_fread(_players, NUM_PLAYERS, 1224, file_pointer);
    // stu_fread(_players, PLAYER_COUNT_MAX, sizeof(struct s_WIZARD), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 9880);

    stu_fread(_world_maps, NUM_PLANES, 4800, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 19480);

    stu_fread(UU_TBL_1, NUM_PLANES, 96, file_pointer);
    stu_fread(UU_TBL_2, NUM_PLANES, 96, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 19864);

    // 0x004d98	0x0960	2	Land mass IDs
    assert(file_pointer_position == 0x4D98);
    // stu_fread(_landmasses, NUM_PLANES, 2400, file_pointer);
    stu_fread(_landmasses, 2400, NUM_PLANES, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 24664);

    stu_fread(_NODES, NUM_NODES, 48, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 26104);

    stu_fread(_FORTRESSES, NUM_FORTRESSES, 4, file_pointer);
    // stu_fread(_FORTRESSES, FORTRESS_COUNT_MAX, sizeof(struct s_FORTRESS), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 26128);

    stu_fread(_TOWERS, NUM_TOWERS, 4, file_pointer);
    // stu_fread(_TOWERS, TOWER_COUNT_MAX, sizeof(struct s_TOWER), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 26152);

    // stu_fread(_LAIRS, NUM_LAIRS, 24, file_pointer);
    /* HACK */  stu_fread(_LAIRS, 102, 24, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 28600);

    stu_fread(_ITEMS, 138, 50, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 35500);

    stu_fread(_CITIES, NUM_CITIES, 114, file_pointer);
    // stu_fread(_CITIES, CITY_COUNT_MAX, sizeof(struct s_CITY), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 46900);

    stu_fread(_UNITS, NUM_UNITS, 32, file_pointer);
    // stu_fread(_UNITS, UNIT_COUNT_MAX, sizeof(struct s_UNIT), file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 79188);

    stu_fread(_map_square_terrain_specials, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 83988);

    stu_fread(_square_explored, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 88788);
    
    // TODO  sizeof(struct s_MOVE_MODE_COST_MAPS)
    stu_fread(movement_mode_cost_maps, NUM_PLANES, 14400, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 117588);

    stu_fread(events_table, 1, 100, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 117688);

    stu_fread(_map_square_flags, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 122488);

    stu_fread(&grand_vizier, 1, 2, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 122490);

    stu_fread(TBL_Premade_Items, 250, 1, file_pointer);

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == 122740);

    if(file_size_flag == ST_FALSE)
    {
        stu_fread(hero_names_table, sizeof(struct s_INACTV_HERO), NUM_HERO_TYPES, file_pointer);
    }
    else
    {
        Clear_Memory((int8_t *)hero_names_table, 545);
    }

    file_pointer_position = stu_ftell(file_pointer);
    assert(file_pointer_position == LEN_SAVE_GAM_FILE);

    stu_fclose(file_pointer);

#ifdef STU_DEBUG
    printf("DEBUG: [%s, %d]: END: Load_SAVE_GAM(%d)\n", __FILE__, __LINE__, save_gam_idx);
    dbg_prn("DEBUG: [%s, %d]: END: Load_SAVE_GAM(%d)\n", __FILE__, __LINE__, save_gam_idx);
    trc_prn("DEBUG: [%s, %d]: END: Load_SAVE_GAM(%d)\n", __FILE__, __LINE__, save_gam_idx);
#endif

}



void Read_SAVE_GAM(void)
{
    FILE * file_pointer = NULL;
    file_pointer = stu_fopen_ci("SAVETEST.GAM", "rb");
    stu_fread(&_save_gam, LEN_SAVE_GAM_FILE, 1, file_pointer);
    stu_fclose(file_pointer);
}

void Write_SAVE_GAM(void)
{
    FILE * file_pointer = NULL;
    file_pointer = stu_fopen_ci("SAVETEST.GAM", "wb");
    stu_fwrite(&_save_gam, LEN_SAVE_GAM_FILE, 1, file_pointer);
    stu_fclose(file_pointer);
}
