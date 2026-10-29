/*
    SAVEFILE -- ReMoM's own save-game format.  See SAVEFILE.h.

    The sections are the same state, in the same order, that Save_SAVE_GAM() wrote in 1.31; only the container
    changed.  Adding state later means adding a row to Save_File_Sections() (and bumping nothing): old files simply
    lack the section, and Save_File_Read() names it.
*/

#include "SAVEFILE.h"

#include "../../STU/src/STU_LOG.h"

#include "MOM_DAT.h"
#include "MOM_DEF.h"
#include "MOX_DAT.h"  /* _players[] */

#include <stddef.h>
#include <string.h>

#define SAVEFILE_MAX_SECTIONS 40
#define SAVEFILE_MAX_EXTRA_SECTIONS 8

struct s_SAVE_SECTION
{
    const char * tag;
    void * data;
    uint32_t record_size;
    uint32_t record_count;
    int found;
    int optional;     /* registered by a mod: a save without it zeroes it instead of failing */
};

/* Sections added with Save_File_Register_Section(). */
static struct s_SAVE_SECTION save_file_extra_sections[SAVEFILE_MAX_EXTRA_SECTIONS];
static int save_file_extra_count;

/* The game's scalar globals, saved as one section of eight int16_t (1.31 wrote them individually at 0x9D8). */
static int16_t save_file_game_vars[8];

static void Save_File_Pack_Game_Vars(void)
{
    save_file_game_vars[0] = _num_players;
    save_file_game_vars[1] = _landsize;
    save_file_game_vars[2] = _magic;
    save_file_game_vars[3] = _difficulty;
    save_file_game_vars[4] = _cities;
    save_file_game_vars[5] = _units;
    save_file_game_vars[6] = _turn;
    save_file_game_vars[7] = _unit;
}

static void Save_File_Unpack_Game_Vars(void)
{
    _num_players = save_file_game_vars[0];
    _landsize    = save_file_game_vars[1];
    _magic       = save_file_game_vars[2];
    _difficulty  = save_file_game_vars[3];
    _cities      = save_file_game_vars[4];
    _units       = save_file_game_vars[5];
    _turn        = save_file_game_vars[6];
    _unit        = save_file_game_vars[7];
}

/* Every section of a save: tag, where it lives in memory, record size, record count. */
static int Save_File_Sections(struct s_SAVE_SECTION * sections)
{
    int count = 0;
    int itr;

#define SAVE_SECTION(_tag_, _data_, _size_, _count_)  \
    sections[count].tag = (_tag_);                    \
    sections[count].data = (void *)(_data_);          \
    sections[count].record_size = (uint32_t)(_size_); \
    sections[count].record_count = (uint32_t)(_count_); \
    sections[count].found = 0;                        \
    sections[count].optional = 0;                     \
    count++;

    SAVE_SECTION("HEROES_0", _HEROES2[0], sizeof(struct s_HERO), NUM_HERO_TYPES)
    SAVE_SECTION("HEROES_1", _HEROES2[1], sizeof(struct s_HERO), NUM_HERO_TYPES)
    SAVE_SECTION("HEROES_2", _HEROES2[2], sizeof(struct s_HERO), NUM_HERO_TYPES)
    SAVE_SECTION("HEROES_3", _HEROES2[3], sizeof(struct s_HERO), NUM_HERO_TYPES)
    SAVE_SECTION("HEROES_4", _HEROES2[4], sizeof(struct s_HERO), NUM_HERO_TYPES)
    SAVE_SECTION("HEROES_5", _HEROES2[5], sizeof(struct s_HERO), NUM_HERO_TYPES)
    SAVE_SECTION("GAMEVARS", save_file_game_vars, sizeof(int16_t), 8)
    SAVE_SECTION("PLAYERS ", _players, sizeof(struct s_WIZARD), NUM_PLAYERS)
    SAVE_SECTION("WORLDMAP", _world_maps, WORLD_SIZE * sizeof(int16_t), NUM_PLANES)
    SAVE_SECTION("CONNLAND", connectivity_grid_land, 96, NUM_PLANES)
    SAVE_SECTION("CONNSEA ", connectivity_grid_sea, 96, NUM_PLANES)
    SAVE_SECTION("LANDMASS", _landmasses, WORLD_SIZE, NUM_PLANES)
    SAVE_SECTION("NODES   ", _NODES, sizeof(struct s_NODE), NUM_NODES)
    SAVE_SECTION("FORTRESS", _FORTRESSES, sizeof(struct s_FORTRESS), NUM_FORTRESSES)
    SAVE_SECTION("TOWERS  ", _TOWERS, sizeof(struct s_TOWER), NUM_TOWERS)
    SAVE_SECTION("LAIRS   ", _LAIRS, sizeof(struct s_LAIR), NUM_LAIRS)
    SAVE_SECTION("ITEMS   ", _ITEMS, sizeof(struct s_ITEM), NUM_ITEMS)
    SAVE_SECTION("CITIES  ", _CITIES, sizeof(struct s_CITY), NUM_CITIES)
    SAVE_SECTION("UNITS   ", _UNITS, sizeof(struct s_UNIT), NUM_UNITS)
    SAVE_SECTION("TSPECIAL", _map_square_terrain_specials, WORLD_SIZE, NUM_PLANES)
    SAVE_SECTION("EXPLORED", _square_explored, WORLD_SIZE, NUM_PLANES)
    SAVE_SECTION("MOVECOST", movement_mode_cost_maps, 14400, NUM_PLANES)
    SAVE_SECTION("EVENTS  ", events_table, 100, 1)
    SAVE_SECTION("SQFLAGS ", _map_square_flags, WORLD_SIZE, NUM_PLANES)
    SAVE_SECTION("VIZIER  ", &grand_vizier, sizeof(grand_vizier), 1)
    SAVE_SECTION("PREFABS ", _prefab_item_table, 1, 250)
    SAVE_SECTION("HERONAME", hero_names_table, sizeof(struct s_INACTV_HERO), NUM_HERO_TYPES)

#undef SAVE_SECTION

    for(itr = 0; itr < save_file_extra_count; itr++)
    {
        sections[count] = save_file_extra_sections[itr];
        sections[count].found = 0;
        count++;
    }

    return count;
}

int Save_File_Register_Section(const char * tag, void * data, uint32_t record_size, uint32_t record_count)
{
    int itr;
    if((tag == NULL) || (strlen(tag) != SAVEFILE_TAG_LEN) || (data == NULL))
    {
        LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] Save_File_Register_Section: tag must be %d characters", SAVEFILE_TAG_LEN);
        return -1;
    }
    for(itr = 0; itr < save_file_extra_count; itr++)
    {
        if(strcmp(save_file_extra_sections[itr].tag, tag) == 0)
        {
            save_file_extra_sections[itr].data = data;                  /* re-registering replaces */
            save_file_extra_sections[itr].record_size = record_size;
            save_file_extra_sections[itr].record_count = record_count;
            return 0;
        }
    }
    if(save_file_extra_count >= SAVEFILE_MAX_EXTRA_SECTIONS)
    {
        LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] Save_File_Register_Section: more than %d extra sections; \"%s\" not added", SAVEFILE_MAX_EXTRA_SECTIONS, tag);
        return -1;
    }
    save_file_extra_sections[save_file_extra_count].tag = tag;
    save_file_extra_sections[save_file_extra_count].data = data;
    save_file_extra_sections[save_file_extra_count].record_size = record_size;
    save_file_extra_sections[save_file_extra_count].record_count = record_count;
    save_file_extra_sections[save_file_extra_count].found = 0;
    save_file_extra_sections[save_file_extra_count].optional = 1;
    save_file_extra_count++;
    return 0;
}

void Save_File_Unregister_Sections(void)
{
    save_file_extra_count = 0;
}

int Save_File_Is_ReMoM(FILE * fp)
{
    char magic[SAVEFILE_MAGIC_LEN];
    long position = ftell(fp);
    size_t got = fread(magic, 1, SAVEFILE_MAGIC_LEN, fp);

    fseek(fp, position, SEEK_SET);
    return (got == SAVEFILE_MAGIC_LEN) && (memcmp(magic, SAVEFILE_MAGIC, SAVEFILE_MAGIC_LEN) == 0);
}

int Save_File_Write(FILE * fp)
{
    struct s_SAVE_SECTION sections[SAVEFILE_MAX_SECTIONS];
    int count;
    int itr;
    uint32_t header[2];
    char tag[SAVEFILE_TAG_LEN];

    Save_File_Pack_Game_Vars();
    count = Save_File_Sections(sections);

    header[0] = SAVEFILE_VERSION;
    header[1] = (uint32_t)count;
    if((fwrite(SAVEFILE_MAGIC, 1, SAVEFILE_MAGIC_LEN, fp) != SAVEFILE_MAGIC_LEN) || (fwrite(header, sizeof(header), 1, fp) != 1))
    {
        LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] write failed in the header");
        return -1;
    }

    for(itr = 0; itr < count; itr++)
    {
        uint32_t sizes[2];
        memset(tag, ' ', sizeof(tag));
        memcpy(tag, sections[itr].tag, strlen(sections[itr].tag));
        sizes[0] = sections[itr].record_size;
        sizes[1] = sections[itr].record_count;
        if(
            (fwrite(tag, 1, SAVEFILE_TAG_LEN, fp) != SAVEFILE_TAG_LEN)
            ||
            (fwrite(sizes, sizeof(sizes), 1, fp) != 1)
            ||
            (fwrite(sections[itr].data, sections[itr].record_size, sections[itr].record_count, fp) != sections[itr].record_count)
        )
        {
            LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] write failed in section \"%s\"", sections[itr].tag);
            return -1;
        }
    }

    return 0;
}

int Save_File_Read(FILE * fp, const char * file_name)
{
    struct s_SAVE_SECTION sections[SAVEFILE_MAX_SECTIONS];
    int count;
    int itr;
    int section_itr;
    char magic[SAVEFILE_MAGIC_LEN];
    uint32_t header[2];

    count = Save_File_Sections(sections);

    if((fread(magic, 1, SAVEFILE_MAGIC_LEN, fp) != SAVEFILE_MAGIC_LEN) || (memcmp(magic, SAVEFILE_MAGIC, SAVEFILE_MAGIC_LEN) != 0) || (fread(header, sizeof(header), 1, fp) != 1))
    {
        LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] %s: not a ReMoM save (bad header)", file_name);
        return -1;
    }
    if(header[0] != SAVEFILE_VERSION)
    {
        LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] %s: format version %lu, this build reads version %d", file_name, (unsigned long)header[0], SAVEFILE_VERSION);
        return -1;
    }

    for(section_itr = 0; section_itr < (int)header[1]; section_itr++)
    {
        char tag[SAVEFILE_TAG_LEN + 1];
        uint32_t sizes[2];
        struct s_SAVE_SECTION * section = NULL;

        if((fread(tag, 1, SAVEFILE_TAG_LEN, fp) != SAVEFILE_TAG_LEN) || (fread(sizes, sizeof(sizes), 1, fp) != 1))
        {
            LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] %s: truncated at section %d", file_name, section_itr);
            return -1;
        }
        tag[SAVEFILE_TAG_LEN] = '\0';

        for(itr = 0; itr < count; itr++)
        {
            if(strncmp(tag, sections[itr].tag, strlen(sections[itr].tag)) == 0)
            {
                section = &sections[itr];
                break;
            }
        }

        if(section == NULL)
        {
            LOG_INFO(LOG_CAT_LOADSAVE, "[SAVEFILE] %s: skipping unknown section \"%s\" (%lu x %lu bytes)", file_name, tag, (unsigned long)sizes[1], (unsigned long)sizes[0]);
            fseek(fp, (long)sizes[0] * (long)sizes[1], SEEK_CUR);
            continue;
        }

        if((sizes[0] != section->record_size) || (sizes[1] != section->record_count))
        {
            LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] %s: section \"%s\" holds %lu records of %lu bytes; this build uses %lu records of %lu bytes (saved by a different ReMoM build)",
                file_name, tag, (unsigned long)sizes[1], (unsigned long)sizes[0], (unsigned long)section->record_count, (unsigned long)section->record_size);
            return -1;
        }

        if(fread(section->data, section->record_size, section->record_count, fp) != section->record_count)
        {
            LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] %s: truncated in section \"%s\"", file_name, tag);
            return -1;
        }
        section->found = 1;
    }

    for(itr = 0; itr < count; itr++)
    {
        if(!sections[itr].found && sections[itr].optional)
        {
            LOG_INFO(LOG_CAT_LOADSAVE, "[SAVEFILE] %s: no section \"%s\" (saved without the mod); starting it empty", file_name, sections[itr].tag);
            memset(sections[itr].data, 0, (size_t)sections[itr].record_size * sections[itr].record_count);
        }
        else if(!sections[itr].found)
        {
            LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] %s: missing section \"%s\"", file_name, sections[itr].tag);
            return -1;
        }
    }

    Save_File_Unpack_Game_Vars();
    return 0;
}

/* Byte offsets inside a 1.31 city record (114 bytes); everything before 0x5D has the same layout as struct s_CITY. */
#define V131_CITY_PRODUCTION    0x5D
#define V131_CITY_PROD_ACCU     0x5E
#define V131_CITY_GOLD          0x60
#define V131_CITY_MAINTENANCE   0x61
#define V131_CITY_MANA          0x62
#define V131_CITY_RESEARCH      0x63
#define V131_CITY_FOOD          0x64
#define V131_CITY_ROADS         0x65

int Save_File_Import_Cities_V131(FILE * fp)
{
    uint8_t record[CITY_RECORD_SIZE_V131];
    int itr;

    for(itr = 0; itr < NUM_CITIES; itr++)
    {
        struct s_CITY * city = &_CITIES[itr];

        if(fread(record, CITY_RECORD_SIZE_V131, 1, fp) != 1)
        {
            LOG_ERROR(LOG_CAT_LOADSAVE, "[SAVEFILE] 1.31 save truncated in city %d", itr);
            return -1;
        }

        memset(city, 0, sizeof(*city));
        memcpy(city, record, V131_CITY_PRODUCTION);                                       /* name .. enchantments: same layout */
        city->production_units     = (int8_t)record[V131_CITY_PRODUCTION];
        city->Prod_Accu            = (int16_t)(record[V131_CITY_PROD_ACCU] | (record[V131_CITY_PROD_ACCU + 1] << 8));
        city->gold_units           = record[V131_CITY_GOLD];
        city->building_maintenance = (int8_t)record[V131_CITY_MAINTENANCE];
        city->mana_units           = record[V131_CITY_MANA];
        city->research_units       = (int8_t)record[V131_CITY_RESEARCH];
        city->food_units           = (int8_t)record[V131_CITY_FOOD];
        memcpy(city->road_connections, &record[V131_CITY_ROADS], sizeof(city->road_connections));
    }

    return 0;
}
