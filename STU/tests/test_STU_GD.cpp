// STU_GD value formatting -- the [GD] text is a contract with the OG-side probe and compare-gd, so these pin the exact output,
// including the truncation guards carried over from the original per-array dumpers:
//   GD_STR    stops copying once q reaches val_size - 2, so the closing quote and NUL always fit
//   GD_BYTES  stops once q reaches val_size - 3
//   numerics  stop once q passes val_size - 16
// A change here changes what compare-gd diffs, so a failure means "check the OG side too", not "update the expectation".

#include <gtest/gtest.h>

#include <cstdint>
#include <cstring>
#include <string>

extern "C" {
#include "../src/STU_GD.h"
}

namespace
{
    std::string format_field(const void * data, uint8_t kind, uint16_t count, int val_size = 1100)
    {
        char val[1100];
        gd_field_t field = { 0, kind, count, "f" };
        std::memset(val, 'X', sizeof(val));
        STU_GD_Format_Field(val, val_size, static_cast<const uint8_t *>(data), &field);
        return std::string(val);
    }

    std::string format_csv(const void * data, int kind, int first, int end, int buf_size = 1100)
    {
        char buf[1100];
        std::memset(buf, 'X', sizeof(buf));
        STU_GD_Format_Csv(buf, buf_size, data, kind, first, end);
        return std::string(buf);
    }
}

TEST(STU_GD, FieldUnsignedAndSignedBytes)
{
    const uint8_t bytes[] = { 0xFF, 0x7F, 0x80, 0x00 };
    EXPECT_EQ(format_field(bytes, GD_U8, 4), "255,127,128,0");
    EXPECT_EQ(format_field(bytes, GD_I8, 4), "-1,127,-128,0");
}

TEST(STU_GD, FieldWordsAreLittleEndian)
{
    const uint8_t bytes[] = { 0x34, 0x12, 0xFF, 0xFF, 0x00, 0x80 };
    EXPECT_EQ(format_field(bytes, GD_U16, 3), "4660,65535,32768");
    EXPECT_EQ(format_field(bytes, GD_I16, 3), "4660,-1,-32768");
}

TEST(STU_GD, FieldDwordsAreLittleEndian)
{
    // GD_U32 with bit 31 set prints unsigned on every platform -- the OG probe is a Linux build (64-bit long), so that is what it prints
    const uint8_t bytes[] = { 0x78, 0x56, 0x34, 0x12, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xD0 };
    EXPECT_EQ(format_field(bytes, GD_U32, 3), "305419896,4294967295,3489660928");
    EXPECT_EQ(format_field(bytes, GD_I32, 3), "305419896,-1,-805306368");
}

TEST(STU_GD, FieldStringStopsAtNulAndMasksNonPrintables)
{
    const uint8_t text[] = { 'R', 'j', 0x01, 'k', 0x00, 'Z', 'Z' };
    EXPECT_EQ(format_field(text, GD_STR, 7), "\"Rj.k\"");
}

TEST(STU_GD, FieldStringWithoutNulStopsAtCount)
{
    const uint8_t text[] = { 'A', 'B', 'C', 'D' };
    EXPECT_EQ(format_field(text, GD_STR, 3), "\"ABC\"");
}

TEST(STU_GD, FieldBytesAreUppercaseHex)
{
    const uint8_t bytes[] = { 0x00, 0xAB, 0x0F, 0xF0 };
    EXPECT_EQ(format_field(bytes, GD_BYTES, 4), "00AB0FF0");
}

TEST(STU_GD, FieldStringTruncationGuard)
{
    // val_size 6: copy while q < 4 -> opening quote + 3 chars, then the closing quote and NUL
    const uint8_t text[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };
    EXPECT_EQ(format_field(text, GD_STR, 7, 6), "\"ABC\"");
}

TEST(STU_GD, FieldBytesTruncationGuard)
{
    // val_size 8: append while q < 5 -> q = 0, 2, 4 each append; 6 stops -> three bytes
    const uint8_t bytes[] = { 0x01, 0x02, 0x03, 0x04 };
    EXPECT_EQ(format_field(bytes, GD_BYTES, 4, 8), "010203");
}

TEST(STU_GD, FieldNumericTruncationGuard)
{
    // val_size 20: stop once q > 4 -> "1000" (q = 4, continue), ",1000" (q = 9, stop)
    const int16_t words[] = { 1000, 1000, 1000, 1000 };
    EXPECT_EQ(format_field(words, GD_I16, 4, 20), "1000,1000");
}

TEST(STU_GD, CsvRangeAndKinds)
{
    const int8_t signed_bytes[] = { -1, 2, -3, 4 };
    const uint16_t words[] = { 0, 32768, 65535 };
    EXPECT_EQ(format_csv(signed_bytes, GD_I8, 0, 4), "-1,2,-3,4");
    EXPECT_EQ(format_csv(signed_bytes, GD_U8, 1, 3), "2,253");
    EXPECT_EQ(format_csv(words, GD_U16, 0, 3), "0,32768,65535");
}

TEST(STU_GD, CsvEmptyRangeIsEmptyString)
{
    const int16_t words[] = { 7 };
    EXPECT_EQ(format_csv(words, GD_I16, 0, 0), "");
}

TEST(STU_GD, CsvTruncatesAtWholeValues)
{
    // buf_size 8: "1000" (4), ",1000" would need 5 more + NUL -> does not fit, so the row stops at a value boundary
    const int16_t words[] = { 1000, 1000, 1000 };
    EXPECT_EQ(format_csv(words, GD_I16, 0, 3, 8), "1000");
}

TEST(STU_GD, FirstTimeIsKeyedByPointContent)
{
    char same_text[] = "test_STU_GD_point_A";
    EXPECT_EQ(STU_GD_First_Time("test_STU_GD_point_A"), 1);
    EXPECT_EQ(STU_GD_First_Time("test_STU_GD_point_A"), 0);
    EXPECT_EQ(STU_GD_First_Time(same_text), 0);   // a different pointer to the same text shares the gate
    EXPECT_EQ(STU_GD_First_Time("test_STU_GD_point_B"), 1);
    EXPECT_EQ(STU_GD_First_Time(nullptr), 0);
}
