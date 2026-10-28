#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

extern "C" {
#include "../src/MOM_DAT.h"
#include "../src/MOM_DEF.h"
#include "../src/MOX_DAT.h"
#include "../src/SAVEFILE.h"
}

namespace
{

/* Points every saved global at a buffer owned by the test, so the save format can be exercised without a game. */
class SAVEFILE_test : public ::testing::Test
{
protected:
    std::vector<std::vector<uint8_t>> buffers;

    uint8_t * Buffer(size_t size)
    {
        buffers.emplace_back(size);
        return buffers.back().data();
    }

    /* The globals as they were before the test; restored in TearDown so other tests never see the test's buffers. */
    struct s_HEROES * saved_heroes[NUM_PLAYERS];
    void * saved_ptrs[18];
    struct s_WIZARD saved_players[NUM_PLAYERS];

    void ** Globals(int i)
    {
        void ** globals[18] = {
            (void **)&_world_maps, (void **)&connectivity_grid_land, (void **)&connectivity_grid_sea, (void **)&_landmasses,
            (void **)&_NODES, (void **)&_FORTRESSES, (void **)&_TOWERS, (void **)&_LAIRS, (void **)&_ITEMS, (void **)&_CITIES,
            (void **)&_UNITS, (void **)&_map_square_terrain_specials, (void **)&_square_explored, (void **)&movement_mode_cost_maps,
            (void **)&events_table, (void **)&_map_square_flags, (void **)&_prefab_item_table, (void **)&hero_names_table };
        return globals[i];
    }

    void TearDown() override
    {
        for(int p = 0; p < NUM_PLAYERS; p++) { _HEROES2[p] = saved_heroes[p]; }
        for(int i = 0; i < 18; i++) { *Globals(i) = saved_ptrs[i]; }
        std::memcpy(_players, saved_players, sizeof(saved_players));
    }

    void SetUp() override
    {
        for(int p = 0; p < NUM_PLAYERS; p++) { saved_heroes[p] = _HEROES2[p]; }
        for(int i = 0; i < 18; i++) { saved_ptrs[i] = *Globals(i); }
        std::memcpy(saved_players, _players, sizeof(saved_players));
        for(int p = 0; p < NUM_PLAYERS; p++)
        {
            _HEROES2[p] = (struct s_HEROES *)Buffer(sizeof(struct s_HERO) * NUM_HERO_TYPES);
        }
        _world_maps = Buffer(WORLD_SIZE * 2 * NUM_PLANES);
        connectivity_grid_land = (int8_t *)Buffer(96 * NUM_PLANES);
        connectivity_grid_sea = (int8_t *)Buffer(96 * NUM_PLANES);
        _landmasses = Buffer(WORLD_SIZE * NUM_PLANES);
        _NODES = (struct s_NODE *)Buffer(sizeof(struct s_NODE) * NUM_NODES);
        _FORTRESSES = (struct s_FORTRESS *)Buffer(sizeof(struct s_FORTRESS) * NUM_FORTRESSES);
        _TOWERS = (struct s_TOWER *)Buffer(sizeof(struct s_TOWER) * NUM_TOWERS);
        _LAIRS = (struct s_LAIR *)Buffer(sizeof(struct s_LAIR) * NUM_LAIRS);
        _ITEMS = (struct s_ITEM *)Buffer(sizeof(struct s_ITEM) * NUM_ITEMS);
        _CITIES = (struct s_CITY *)Buffer(sizeof(struct s_CITY) * NUM_CITIES);
        _UNITS = (struct s_UNIT *)Buffer(sizeof(struct s_UNIT) * NUM_UNITS);
        _map_square_terrain_specials = Buffer(WORLD_SIZE * NUM_PLANES);
        _square_explored = Buffer(WORLD_SIZE * NUM_PLANES);
        movement_mode_cost_maps = (struct s_MOVE_MODE_COST_MAPS *)Buffer(14400 * NUM_PLANES);
        events_table = (struct s_EVENT_DATA *)Buffer(100);
        _map_square_flags = Buffer(WORLD_SIZE * NUM_PLANES);
        _prefab_item_table = Buffer(250);
        hero_names_table = (struct s_INACTV_HERO *)Buffer(sizeof(struct s_INACTV_HERO) * NUM_HERO_TYPES);
    }

    /* Every saved byte, in section order, for comparing whole-state snapshots. */
    std::vector<uint8_t> Snapshot()
    {
        std::vector<uint8_t> all;
        for(auto & b : buffers)
        {
            all.insert(all.end(), b.begin(), b.end());
        }
        const uint8_t * players = (const uint8_t *)_players;
        all.insert(all.end(), players, players + sizeof(struct s_WIZARD) * NUM_PLAYERS);
        int16_t vars[] = { _num_players, _landsize, _magic, _difficulty, _cities, _units, _turn, _unit };
        all.insert(all.end(), (uint8_t *)vars, (uint8_t *)vars + sizeof(vars));
        all.insert(all.end(), (uint8_t *)&grand_vizier, (uint8_t *)&grand_vizier + sizeof(grand_vizier));
        return all;
    }

    void Fill(unsigned seed)
    {
        std::srand(seed);
        for(auto & b : buffers)
        {
            for(auto & byte : b) { byte = (uint8_t)std::rand(); }
        }
        uint8_t * players = (uint8_t *)_players;
        for(size_t i = 0; i < sizeof(struct s_WIZARD) * NUM_PLAYERS; i++) { players[i] = (uint8_t)std::rand(); }
        _num_players = 5; _landsize = 1; _magic = 2; _difficulty = 3; _cities = 42; _units = 300; _turn = 77; _unit = 12;
        grand_vizier = 1;
    }

    void Clear()
    {
        for(auto & b : buffers) { std::fill(b.begin(), b.end(), 0); }
        std::memset(_players, 0, sizeof(struct s_WIZARD) * NUM_PLAYERS);
        _num_players = _landsize = _magic = _difficulty = _cities = _units = _turn = _unit = 0;
        grand_vizier = 0;
    }

    std::vector<uint8_t> Write()
    {
        FILE * fp = std::tmpfile();
        EXPECT_NE(fp, nullptr);
        EXPECT_EQ(Save_File_Write(fp), 0);
        long length = std::ftell(fp);
        std::vector<uint8_t> bytes((size_t)length);
        std::rewind(fp);
        EXPECT_EQ(std::fread(bytes.data(), 1, bytes.size(), fp), bytes.size());
        std::fclose(fp);
        return bytes;
    }

    int Read(const std::vector<uint8_t> & bytes)
    {
        FILE * fp = std::tmpfile();
        std::fwrite(bytes.data(), 1, bytes.size(), fp);
        std::rewind(fp);
        int is_remom = Save_File_Is_ReMoM(fp);
        int result = is_remom ? Save_File_Read(fp, "test.GAM") : -2;
        std::fclose(fp);
        return result;
    }

    /* Byte offset of a section's (record_size, record_count) header inside a written save. */
    static size_t Section_Header(const std::vector<uint8_t> & bytes, const char * tag)
    {
        size_t pos = 16;
        while(pos + 16 <= bytes.size())
        {
            uint32_t size, count;
            std::memcpy(&size, &bytes[pos + 8], 4);
            std::memcpy(&count, &bytes[pos + 12], 4);
            if(std::memcmp(&bytes[pos], tag, 8) == 0) { return pos; }
            pos += 16 + (size_t)size * count;
        }
        return 0;
    }
};

}  // namespace

TEST_F(SAVEFILE_test, RoundTrip_RestoresEverySavedByte)
{
    Fill(1234);
    std::vector<uint8_t> before = Snapshot();
    std::vector<uint8_t> file = Write();
    Clear();
    ASSERT_EQ(Read(file), 0);
    EXPECT_EQ(Snapshot(), before);
}

TEST_F(SAVEFILE_test, CitiesAreWrittenAtTheirInMemorySize)
{
    Fill(1);
    _CITIES[7].gold_units = 100000;
    _CITIES[7].food_units = -5000;
    std::vector<uint8_t> file = Write();
    size_t header = Section_Header(file, "CITIES  ");
    ASSERT_NE(header, 0u);
    uint32_t size;
    std::memcpy(&size, &file[header + 8], 4);
    EXPECT_EQ(size, sizeof(struct s_CITY));
    Clear();
    ASSERT_EQ(Read(file), 0);
    EXPECT_EQ(_CITIES[7].gold_units, 100000);
    EXPECT_EQ(_CITIES[7].food_units, -5000);
}

TEST_F(SAVEFILE_test, Write_HasHeaderAndExpectedSize)
{
    Fill(2);
    std::vector<uint8_t> file = Write();
    ASSERT_GE(file.size(), 16u);
    EXPECT_EQ(std::memcmp(file.data(), SAVEFILE_MAGIC, SAVEFILE_MAGIC_LEN), 0);
    /* 1.31 state 123300 + cities (sizeof - 114) * 100 + header 16 + 27 section headers * 16 */
    EXPECT_EQ(file.size(), 123300u + (sizeof(struct s_CITY) - 114) * NUM_CITIES + 16u + 27u * 16u);
}

TEST_F(SAVEFILE_test, Read_RefusesARecordSizeFromAnotherBuild)
{
    Fill(3);
    std::vector<uint8_t> file = Write();
    size_t header = Section_Header(file, "UNITS   ");
    ASSERT_NE(header, 0u);
    uint32_t wrong = sizeof(struct s_UNIT) + 4;
    std::memcpy(&file[header + 8], &wrong, 4);
    EXPECT_EQ(Read(file), -1);
}

TEST_F(SAVEFILE_test, Read_RefusesAMissingSection)
{
    Fill(4);
    std::vector<uint8_t> file = Write();
    size_t header = Section_Header(file, "EVENTS  ");
    ASSERT_NE(header, 0u);
    std::memcpy(&file[header], "IGNOREME", 8);                    /* EVENTS becomes an unknown section */
    EXPECT_EQ(Read(file), -1);
}

TEST_F(SAVEFILE_test, Read_SkipsUnknownSections)
{
    Fill(5);
    std::vector<uint8_t> before = Snapshot();
    std::vector<uint8_t> file = Write();
    /* Insert a mod's section after the header and bump the section count. */
    std::vector<uint8_t> extra = { 'M', 'O', 'D', 'D', 'A', 'T', 'A', ' ', 4, 0, 0, 0, 2, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    file.insert(file.begin() + 16, extra.begin(), extra.end());
    uint32_t sections;
    std::memcpy(&sections, &file[12], 4);
    sections += 1;
    std::memcpy(&file[12], &sections, 4);
    Clear();
    ASSERT_EQ(Read(file), 0);
    EXPECT_EQ(Snapshot(), before);
}

TEST_F(SAVEFILE_test, OldLayoutIsNotMistakenForReMoM)
{
    std::vector<uint8_t> legacy(123300, 0x11);
    EXPECT_EQ(Read(legacy), -2);
}

TEST_F(SAVEFILE_test, ImportV131Cities_ReadsTheOneByteFields)
{
    std::vector<uint8_t> records(114 * NUM_CITIES, 0);
    uint8_t * r = &records[114 * 3];
    std::strcpy((char *)r, "Testburg");
    r[0x0E] = 4;                                                  /* race */
    r[0x13] = 7;                                                  /* size */
    r[0x1F + 5] = 1;                                              /* bldg_status[5] */
    r[0x5D] = 0xFE;                                               /* production -2 */
    r[0x5E] = 0x34; r[0x5F] = 0x12;                               /* Prod_Accu 0x1234 */
    r[0x60] = 200;                                                /* gold (unsigned) */
    r[0x61] = 0xFD;                                               /* maintenance -3 */
    r[0x62] = 250;                                                /* mana (unsigned) */
    r[0x63] = 9;                                                  /* research */
    r[0x64] = 0x81;                                               /* food -127 */
    r[0x65] = 0x55; r[0x65 + 12] = 0xAA;                          /* road_connections */
    FILE * fp = std::tmpfile();
    std::fwrite(records.data(), 1, records.size(), fp);
    std::rewind(fp);
    ASSERT_EQ(Save_File_Import_Cities_V131(fp), 0);
    std::fclose(fp);
    const struct s_CITY & c = _CITIES[3];
    EXPECT_STREQ(c.name, "Testburg");
    EXPECT_EQ(c.race, 4);
    EXPECT_EQ(c.size, 7);
    EXPECT_EQ(c.bldg_status[5], 1);
    EXPECT_EQ(c.production_units, -2);
    EXPECT_EQ(c.Prod_Accu, 0x1234);
    EXPECT_EQ(c.gold_units, 200);
    EXPECT_EQ(c.building_maintenance, -3);
    EXPECT_EQ(c.mana_units, 250);
    EXPECT_EQ(c.research_units, 9);
    EXPECT_EQ(c.food_units, -127);
    EXPECT_EQ((uint8_t)c.road_connections[0], 0x55);
    EXPECT_EQ((uint8_t)c.road_connections[12], 0xAA);
}
