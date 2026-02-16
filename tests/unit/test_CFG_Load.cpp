#include <gtest/gtest.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../../MoX/src/CFG.h"

// Helper to write CONFIG.MOM with 10 uint16_t values (20 bytes)
static void write_config_file(const char *path, const uint16_t vals[10])
{
    FILE *f = fopen(path, "wb");
    ASSERT_NE(f, nullptr);
    for (int i = 0; i < 10; ++i) {
        uint16_t v = vals[i];
        // write little-endian
        uint8_t b[2] = { (uint8_t)(v & 0xFF), (uint8_t)((v >> 8) & 0xFF) };
        fwrite(b, 1, 2, f);
    }
    fclose(f);
}

TEST(CFG, Load_CONFIG_MOM_reads_first_18_bytes)
{
    const char *fname = "CONFIG.MOM";
    // prepare values 0x0101..0x0A0A
    uint16_t vals[10];
    for (int i = 0; i < 10; ++i) vals[i] = (uint16_t)((i+1) | ((i+1) << 8));

    // ensure initial sentinel in the last field
    config_mom.NIU_12 = 0xBEEF;

    // write file containing 20 bytes, but Load_CONFIG_MOM reads only 18
    write_config_file(fname, vals);

    // call the loader
    Load_CONFIG_MOM();

    // first 9 fields (9*2 = 18 bytes) should be updated
    EXPECT_EQ(config_mom.MIDI_IO, vals[0]);
    EXPECT_EQ(config_mom.MIDI_ID, vals[1]);
    EXPECT_EQ(config_mom.MIDI_IRQ, vals[2]);
    EXPECT_EQ(config_mom.DIGI_IO, vals[3]);
    EXPECT_EQ(config_mom.DIGI_ID, vals[4]);
    EXPECT_EQ(config_mom.DIGI_IRQ, vals[5]);
    EXPECT_EQ(config_mom.DIGI_DMA, vals[6]);
    EXPECT_EQ(config_mom.NIU_0E, vals[7]);
    EXPECT_EQ(config_mom.NIU_10, vals[8]);

    // last field should remain unchanged (Load_CONFIG_MOM reads only 18 bytes)
    EXPECT_EQ(config_mom.NIU_12, (uint16_t)0xBEEF);

    // cleanup
    remove(fname);
}
