#include <gtest/gtest.h>

#include "../src/MOX_BASE.h"
#include "../src/Util.h"

#include <cstring>

// ============================================================================
// String_To_Upper
// ============================================================================

TEST(String_To_Upper_test, LowercaseString)
{
    char buffer[] = "hello";
    String_To_Upper(buffer);
    EXPECT_STREQ("HELLO", buffer);
}

TEST(String_To_Upper_test, MixedCaseString)
{
    char buffer[] = "HeLLo WoRLd";
    String_To_Upper(buffer);
    EXPECT_STREQ("HELLO WORLD", buffer);
}

TEST(String_To_Upper_test, AlreadyUppercase)
{
    char buffer[] = "HELLO";
    String_To_Upper(buffer);
    EXPECT_STREQ("HELLO", buffer);
}

TEST(String_To_Upper_test, EmptyString)
{
    char buffer[] = "";
    String_To_Upper(buffer);
    EXPECT_STREQ("", buffer);
}

TEST(String_To_Upper_test, WithDigitsAndSymbols)
{
    char buffer[] = "abc123!@#";
    String_To_Upper(buffer);
    EXPECT_STREQ("ABC123!@#", buffer);
}


// ============================================================================
// String_To_Lower
// ============================================================================

TEST(String_To_Lower_test, UppercaseString)
{
    char buffer[] = "HELLO";
    String_To_Lower(buffer);
    EXPECT_STREQ("hello", buffer);
}

TEST(String_To_Lower_test, MixedCaseString)
{
    char buffer[] = "HeLLo WoRLd";
    String_To_Lower(buffer);
    EXPECT_STREQ("hello world", buffer);
}

TEST(String_To_Lower_test, AlreadyLowercase)
{
    char buffer[] = "hello";
    String_To_Lower(buffer);
    EXPECT_STREQ("hello", buffer);
}

TEST(String_To_Lower_test, EmptyString)
{
    char buffer[] = "";
    String_To_Lower(buffer);
    EXPECT_STREQ("", buffer);
}


// ============================================================================
// Copy_Memory_Near
// ============================================================================

TEST(Copy_Memory_Near_test, BasicCopy)
{
    uint8_t src[4] = { 0xAA, 0xBB, 0xCC, 0xDD };
    uint8_t dst[4] = { 0 };

    Copy_Memory_Near(dst, src, 4);

    EXPECT_EQ(dst[0], 0xAA);
    EXPECT_EQ(dst[1], 0xBB);
    EXPECT_EQ(dst[2], 0xCC);
    EXPECT_EQ(dst[3], 0xDD);
}

TEST(Copy_Memory_Near_test, ZeroCount)
{
    uint8_t src[4] = { 0xAA, 0xBB, 0xCC, 0xDD };
    uint8_t dst[4] = { 0 };

    Copy_Memory_Near(dst, src, 0);

    EXPECT_EQ(dst[0], 0x00);
    EXPECT_EQ(dst[1], 0x00);
}

TEST(Copy_Memory_Near_test, PartialCopy)
{
    uint8_t src[4] = { 0x11, 0x22, 0x33, 0x44 };
    uint8_t dst[4] = { 0 };

    Copy_Memory_Near(dst, src, 2);

    EXPECT_EQ(dst[0], 0x11);
    EXPECT_EQ(dst[1], 0x22);
    EXPECT_EQ(dst[2], 0x00);
    EXPECT_EQ(dst[3], 0x00);
}


// ============================================================================
// Clear_Memory
// ============================================================================

TEST(Clear_Memory_test, ClearsBuffer)
{
    int8_t buffer[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    Clear_Memory(buffer, 8);

    for (int i = 0; i < 8; i++)
    {
        EXPECT_EQ(buffer[i], 0);
    }
}

TEST(Clear_Memory_test, ZeroCount)
{
    int8_t buffer[4] = { 0x7F, 0x7F, 0x7F, 0x7F };

    Clear_Memory(buffer, 0);

    EXPECT_EQ(buffer[0], 0x7F);
    EXPECT_EQ(buffer[1], 0x7F);
}

TEST(Clear_Memory_test, PartialClear)
{
    int8_t buffer[4] = { 1, 2, 3, 4 };

    Clear_Memory(buffer, 2);

    EXPECT_EQ(buffer[0], 0);
    EXPECT_EQ(buffer[1], 0);
    EXPECT_EQ(buffer[2], 3);
    EXPECT_EQ(buffer[3], 4);
}


// ============================================================================
// Set_Memory
// ============================================================================

TEST(Set_Memory_test, FillsBuffer)
{
    int8_t buffer[4] = { 0 };

    Set_Memory(buffer, 4, 0x42);

    EXPECT_EQ(buffer[0], 0x42);
    EXPECT_EQ(buffer[1], 0x42);
    EXPECT_EQ(buffer[2], 0x42);
    EXPECT_EQ(buffer[3], 0x42);
}

TEST(Set_Memory_test, ZeroCount)
{
    int8_t buffer[4] = { 1, 2, 3, 4 };

    Set_Memory(buffer, 0, 0xFF);

    EXPECT_EQ(buffer[0], 1);
    EXPECT_EQ(buffer[1], 2);
}

TEST(Set_Memory_test, PartialFill)
{
    int8_t buffer[4] = { 0 };

    Set_Memory(buffer, 2, 0x55);

    EXPECT_EQ(buffer[0], 0x55);
    EXPECT_EQ(buffer[1], 0x55);
    EXPECT_EQ(buffer[2], 0x00);
    EXPECT_EQ(buffer[3], 0x00);
}


// ============================================================================
// Swap_Short
// ============================================================================

TEST(Swap_Short_test, BasicSwap)
{
    int16_t a = 10;
    int16_t b = 20;

    Swap_Short(&a, &b);

    EXPECT_EQ(a, 20);
    EXPECT_EQ(b, 10);
}

TEST(Swap_Short_test, SwapSameValue)
{
    int16_t a = 42;
    int16_t b = 42;

    Swap_Short(&a, &b);

    EXPECT_EQ(a, 42);
    EXPECT_EQ(b, 42);
}

TEST(Swap_Short_test, SwapNegativeValues)
{
    int16_t a = -100;
    int16_t b = 200;

    Swap_Short(&a, &b);

    EXPECT_EQ(a, 200);
    EXPECT_EQ(b, -100);
}

TEST(Swap_Short_test, SwapZero)
{
    int16_t a = 0;
    int16_t b = 32767;

    Swap_Short(&a, &b);

    EXPECT_EQ(a, 32767);
    EXPECT_EQ(b, 0);
}


// ============================================================================
// Test_Bit_Field
// ============================================================================

TEST(Test_Bit_Field_test, BitZeroSet)
{
    // Arrange
    uint8_t field[2] = { 0x01, 0x00 };

    // Act & Assert
    EXPECT_EQ(Test_Bit_Field(0, field), ST_TRUE);
    EXPECT_EQ(Test_Bit_Field(1, field), ST_FALSE);
}

TEST(Test_Bit_Field_test, BitSevenSet)
{
    // Arrange
    uint8_t field[2] = { 0x80, 0x00 };

    // Act & Assert
    EXPECT_EQ(Test_Bit_Field(7, field), ST_TRUE);
    EXPECT_EQ(Test_Bit_Field(6, field), ST_FALSE);
}

TEST(Test_Bit_Field_test, BitInSecondByte)
{
    // Arrange
    uint8_t field[2] = { 0x00, 0x04 };  // bit 10 (byte 1, bit 2)

    // Act & Assert
    EXPECT_EQ(Test_Bit_Field(10, field), ST_TRUE);
    EXPECT_EQ(Test_Bit_Field(9, field), ST_FALSE);
    EXPECT_EQ(Test_Bit_Field(11, field), ST_FALSE);
}

TEST(Test_Bit_Field_test, AllBitsCleared)
{
    // Arrange
    uint8_t field[2] = { 0x00, 0x00 };

    // Act & Assert
    for (int16_t i = 0; i < 16; i++)
    {
        EXPECT_EQ(Test_Bit_Field(i, field), ST_FALSE);
    }
}

TEST(Test_Bit_Field_test, AllBitsSet)
{
    // Arrange
    uint8_t field[2] = { 0xFF, 0xFF };

    // Act & Assert
    for (int16_t i = 0; i < 16; i++)
    {
        EXPECT_EQ(Test_Bit_Field(i, field), ST_TRUE);
    }
}


// ============================================================================
// Set_Bit_Field
// ============================================================================

TEST(Set_Bit_Field_test, SetBitZero)
{
    // Arrange
    uint8_t field[2] = { 0x00, 0x00 };

    // Act
    Set_Bit_Field(0, field);

    // Assert
    EXPECT_EQ(field[0], 0x01);
    EXPECT_EQ(field[1], 0x00);
}

TEST(Set_Bit_Field_test, SetBitSeven)
{
    // Arrange
    uint8_t field[2] = { 0x00, 0x00 };

    // Act
    Set_Bit_Field(7, field);

    // Assert
    EXPECT_EQ(field[0], 0x80);
}

TEST(Set_Bit_Field_test, SetBitInSecondByte)
{
    // Arrange
    uint8_t field[2] = { 0x00, 0x00 };

    // Act
    Set_Bit_Field(10, field);

    // Assert
    EXPECT_EQ(field[0], 0x00);
    EXPECT_EQ(field[1], 0x04);
}

TEST(Set_Bit_Field_test, SetBitPreservesExisting)
{
    // Arrange
    uint8_t field[2] = { 0x01, 0x00 };  // bit 0 already set

    // Act
    Set_Bit_Field(3, field);

    // Assert
    EXPECT_EQ(field[0], 0x09);  // bits 0 and 3
}

TEST(Set_Bit_Field_test, SetAlreadySetBit)
{
    // Arrange
    uint8_t field[2] = { 0xFF, 0xFF };

    // Act
    Set_Bit_Field(5, field);

    // Assert
    EXPECT_EQ(field[0], 0xFF);
    EXPECT_EQ(field[1], 0xFF);
}


// ============================================================================
// Clear_Bit_Field
// ============================================================================

TEST(Clear_Bit_Field_test, ClearBitZero)
{
    // Arrange
    uint8_t field[2] = { 0xFF, 0xFF };

    // Act
    Clear_Bit_Field(0, field);

    // Assert
    EXPECT_EQ(field[0], 0xFE);
    EXPECT_EQ(field[1], 0xFF);
}

TEST(Clear_Bit_Field_test, ClearBitSeven)
{
    // Arrange
    uint8_t field[2] = { 0xFF, 0xFF };

    // Act
    Clear_Bit_Field(7, field);

    // Assert
    EXPECT_EQ(field[0], 0x7F);
}

TEST(Clear_Bit_Field_test, ClearBitInSecondByte)
{
    // Arrange
    uint8_t field[2] = { 0xFF, 0xFF };

    // Act
    Clear_Bit_Field(10, field);

    // Assert
    EXPECT_EQ(field[0], 0xFF);
    EXPECT_EQ(field[1], 0xFB);
}

TEST(Clear_Bit_Field_test, ClearAlreadyClearedBit)
{
    // Arrange
    uint8_t field[2] = { 0x00, 0x00 };

    // Act
    Clear_Bit_Field(5, field);

    // Assert
    EXPECT_EQ(field[0], 0x00);
    EXPECT_EQ(field[1], 0x00);
}


// ============================================================================
// Set_Bit_Field + Test_Bit_Field + Clear_Bit_Field  (Roundtrip)
// ============================================================================

TEST(Bit_Field_Roundtrip_test, SetTestClear)
{
    // Arrange
    uint8_t field[2] = { 0x00, 0x00 };

    // Act & Assert - Set and verify
    Set_Bit_Field(5, field);
    EXPECT_EQ(Test_Bit_Field(5, field), ST_TRUE);

    // Act & Assert - Clear and verify
    Clear_Bit_Field(5, field);
    EXPECT_EQ(Test_Bit_Field(5, field), ST_FALSE);
}

TEST(Bit_Field_Roundtrip_test, SetMultipleBits)
{
    // Arrange
    uint8_t field[2] = { 0x00, 0x00 };

    // Act
    Set_Bit_Field(0, field);
    Set_Bit_Field(3, field);
    Set_Bit_Field(8, field);
    Set_Bit_Field(15, field);

    // Assert
    EXPECT_EQ(Test_Bit_Field(0, field), ST_TRUE);
    EXPECT_EQ(Test_Bit_Field(1, field), ST_FALSE);
    EXPECT_EQ(Test_Bit_Field(2, field), ST_FALSE);
    EXPECT_EQ(Test_Bit_Field(3, field), ST_TRUE);
    EXPECT_EQ(Test_Bit_Field(7, field), ST_FALSE);
    EXPECT_EQ(Test_Bit_Field(8, field), ST_TRUE);
    EXPECT_EQ(Test_Bit_Field(15, field), ST_TRUE);
}


// ============================================================================
// Copy_Memory_Less_Than
// ============================================================================

TEST(Copy_Memory_Less_Than_test, CopiesSmallerValues)
{
    // Arrange
    int8_t src[4] = { 1, 2, 3, 4 };
    int8_t dst[4] = { 5, 5, 5, 5 };

    // Act
    Copy_Memory_Less_Than(src, dst, 4);

    // Assert - all src values are less than dst, so all should be copied
    EXPECT_EQ(dst[0], 1);
    EXPECT_EQ(dst[1], 2);
    EXPECT_EQ(dst[2], 3);
    EXPECT_EQ(dst[3], 4);
}

TEST(Copy_Memory_Less_Than_test, DoesNotCopyLargerValues)
{
    // Arrange
    int8_t src[4] = { 10, 10, 10, 10 };
    int8_t dst[4] = { 1, 2, 3, 4 };

    // Act
    Copy_Memory_Less_Than(src, dst, 4);

    // Assert - src values are larger, dst should be unchanged
    EXPECT_EQ(dst[0], 1);
    EXPECT_EQ(dst[1], 2);
    EXPECT_EQ(dst[2], 3);
    EXPECT_EQ(dst[3], 4);
}

TEST(Copy_Memory_Less_Than_test, ZeroCount)
{
    // Arrange
    int8_t src[4] = { 0, 0, 0, 0 };
    int8_t dst[4] = { 5, 5, 5, 5 };

    // Act
    Copy_Memory_Less_Than(src, dst, 0);

    // Assert
    EXPECT_EQ(dst[0], 5);
}


// ============================================================================
// Clear_Structure
// ============================================================================

TEST(Clear_Structure_test, RemoveLastRecord)
{
    // Arrange - 3 records of 4 bytes each
    uint8_t records[12] = {
        0x11, 0x22, 0x33, 0x44,  // record 0
        0x55, 0x66, 0x77, 0x88,  // record 1
        0xAA, 0xBB, 0xCC, 0xDD   // record 2
    };

    // Act - remove last record (index 2)
    Clear_Structure(2, records, 4, 3);

    // Assert - last record should be zeroed
    EXPECT_EQ(records[0], 0x11);
    EXPECT_EQ(records[4], 0x55);
    EXPECT_EQ(records[8], 0x00);
    EXPECT_EQ(records[9], 0x00);
    EXPECT_EQ(records[10], 0x00);
    EXPECT_EQ(records[11], 0x00);
}

TEST(Clear_Structure_test, RemoveFirstRecord)
{
    // Arrange - 3 records of 4 bytes each
    uint8_t records[12] = {
        0x11, 0x22, 0x33, 0x44,  // record 0
        0x55, 0x66, 0x77, 0x88,  // record 1
        0xAA, 0xBB, 0xCC, 0xDD   // record 2
    };

    // Act - remove first record (index 0)
    Clear_Structure(0, records, 4, 3);

    // Assert - records shifted down, last record zeroed
    EXPECT_EQ(records[0], 0x55);
    EXPECT_EQ(records[1], 0x66);
    EXPECT_EQ(records[2], 0x77);
    EXPECT_EQ(records[3], 0x88);
    EXPECT_EQ(records[4], 0xAA);
    EXPECT_EQ(records[5], 0xBB);
    EXPECT_EQ(records[6], 0xCC);
    EXPECT_EQ(records[7], 0xDD);
    EXPECT_EQ(records[8], 0x00);
    EXPECT_EQ(records[9], 0x00);
    EXPECT_EQ(records[10], 0x00);
    EXPECT_EQ(records[11], 0x00);
}

TEST(Clear_Structure_test, RemoveMiddleRecord)
{
    // Arrange - 3 records of 4 bytes each
    uint8_t records[12] = {
        0x11, 0x22, 0x33, 0x44,  // record 0
        0x55, 0x66, 0x77, 0x88,  // record 1
        0xAA, 0xBB, 0xCC, 0xDD   // record 2
    };

    // Act - remove middle record (index 1)
    Clear_Structure(1, records, 4, 3);

    // Assert - record 2 shifted down to record 1, last record zeroed
    EXPECT_EQ(records[0], 0x11);
    EXPECT_EQ(records[1], 0x22);
    EXPECT_EQ(records[2], 0x33);
    EXPECT_EQ(records[3], 0x44);
    EXPECT_EQ(records[4], 0xAA);
    EXPECT_EQ(records[5], 0xBB);
    EXPECT_EQ(records[6], 0xCC);
    EXPECT_EQ(records[7], 0xDD);
    EXPECT_EQ(records[8], 0x00);
    EXPECT_EQ(records[9], 0x00);
    EXPECT_EQ(records[10], 0x00);
    EXPECT_EQ(records[11], 0x00);
}


// ============================================================================
// Delete_Structure
// ============================================================================

TEST(Delete_Structure_test, RemoveFirstRecord)
{
    // Arrange - 3 records of 4 bytes each
    uint8_t records[12] = {
        0x11, 0x22, 0x33, 0x44,  // record 0
        0x55, 0x66, 0x77, 0x88,  // record 1
        0xAA, 0xBB, 0xCC, 0xDD   // record 2
    };

    // Act - remove first record (index 0)
    Delete_Structure(0, records, 4, 3);

    // Assert - records shifted down (last record NOT zeroed, unlike Clear_Structure)
    EXPECT_EQ(records[0], 0x55);
    EXPECT_EQ(records[1], 0x66);
    EXPECT_EQ(records[2], 0x77);
    EXPECT_EQ(records[3], 0x88);
    EXPECT_EQ(records[4], 0xAA);
    EXPECT_EQ(records[5], 0xBB);
    EXPECT_EQ(records[6], 0xCC);
    EXPECT_EQ(records[7], 0xDD);
}

TEST(Delete_Structure_test, RemoveMiddleRecord)
{
    // Arrange - 3 records of 4 bytes each
    uint8_t records[12] = {
        0x11, 0x22, 0x33, 0x44,  // record 0
        0x55, 0x66, 0x77, 0x88,  // record 1
        0xAA, 0xBB, 0xCC, 0xDD   // record 2
    };

    // Act - remove middle record (index 1)
    Delete_Structure(1, records, 4, 3);

    // Assert - record 2 shifted into record 1 position
    EXPECT_EQ(records[0], 0x11);
    EXPECT_EQ(records[1], 0x22);
    EXPECT_EQ(records[2], 0x33);
    EXPECT_EQ(records[3], 0x44);
    EXPECT_EQ(records[4], 0xAA);
    EXPECT_EQ(records[5], 0xBB);
    EXPECT_EQ(records[6], 0xCC);
    EXPECT_EQ(records[7], 0xDD);
}

TEST(Delete_Structure_test, RemoveLastRecord_NoShift)
{
    // Arrange - 3 records of 4 bytes each
    uint8_t records[12] = {
        0x11, 0x22, 0x33, 0x44,  // record 0
        0x55, 0x66, 0x77, 0x88,  // record 1
        0xAA, 0xBB, 0xCC, 0xDD   // record 2
    };

    // Act - remove last record (byte_count == 0, no shift)
    Delete_Structure(2, records, 4, 3);

    // Assert - nothing moved (byte_count is 0)
    EXPECT_EQ(records[0], 0x11);
    EXPECT_EQ(records[4], 0x55);
    EXPECT_EQ(records[8], 0xAA);
}


// WTF  // ============================================================================
// WTF  // bit_field_test_bits (global array verification)
// WTF  // ============================================================================
// WTF  
// WTF  TEST(bit_field_test_bits_test, CorrectValues)
// WTF  {
// WTF      EXPECT_EQ(bit_field_test_bits[0], 0x0001);
// WTF      EXPECT_EQ(bit_field_test_bits[1], 0x0002);
// WTF      EXPECT_EQ(bit_field_test_bits[2], 0x0004);
// WTF      EXPECT_EQ(bit_field_test_bits[3], 0x0008);
// WTF      EXPECT_EQ(bit_field_test_bits[4], 0x0010);
// WTF      EXPECT_EQ(bit_field_test_bits[5], 0x0020);
// WTF      EXPECT_EQ(bit_field_test_bits[6], 0x0040);
// WTF      EXPECT_EQ(bit_field_test_bits[7], 0x0080);
// WTF  }
