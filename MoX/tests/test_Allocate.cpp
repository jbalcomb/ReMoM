/*
 * Unit tests for Allocate.c
 * Tests the Space Allocation Memory Block (SAMB) system
 */

#include <gtest/gtest.h>
#include <cstring>
#include "../src/Allocate.h"
#include "../src/MOX_BASE.h"
#include "../src/MOX_TYPE.h"

// Test fixture for Allocate tests
class AllocateTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset near buffer state before each test
        Near_Allocate_Reset();
    }

    void TearDown() override {
        // Cleanup is handled by test-specific logic
    }
};

// ==============================================================================
// Allocate_Space Tests
// ==============================================================================

TEST_F(AllocateTest, Allocate_Space_NormalAllocation)
{
    uint16_t size = 100;
    SAMB_ptr block = Allocate_Space(size);
    
    // Verify allocation succeeded
    ASSERT_NE(block, nullptr);
    
    // Verify block header fields
    EXPECT_EQ(GET_2B_OFS((block), SAMB_RESERVED1), _AAAA);
    EXPECT_EQ(GET_2B_OFS((block), SAMB_RESERVED2), _AAAA);
    EXPECT_EQ(SA_GET_MEMSIG1(block), _SA_MEMSIG1);
    EXPECT_EQ(SA_GET_MEMSIG2(block), _SA_MEMSIG2);
    EXPECT_EQ(SA_GET_SIZE(block), size);
    EXPECT_EQ(SA_GET_USED(block), 1);
    EXPECT_EQ(GET_2B_OFS((block), SAMB_unknown), _AAAA);
    EXPECT_EQ(GET_2B_OFS((block), SAMB_MARK), _AAAA);
    
    free(block);
}

TEST_F(AllocateTest, Allocate_Space_MinimumSize) {
    uint16_t size = 0;
    SAMB_ptr block = Allocate_Space(size);
    
    ASSERT_NE(block, nullptr);
    EXPECT_EQ(SA_GET_SIZE(block), size);
    EXPECT_EQ(SA_GET_MEMSIG1(block), _SA_MEMSIG1);
    EXPECT_EQ(SA_GET_MEMSIG2(block), _SA_MEMSIG2);
    
    free(block);
}

TEST_F(AllocateTest, Allocate_Space_LargeSize) {
    uint16_t size = 1000;
    SAMB_ptr block = Allocate_Space(size);
    
    ASSERT_NE(block, nullptr);
    EXPECT_EQ(SA_GET_SIZE(block), size);
    EXPECT_EQ(SA_GET_USED(block), 1);
    
    free(block);
}

TEST_F(AllocateTest, Allocate_Space_MaximumSize) {
    uint16_t size = 4000;  // Large but reasonable size
    SAMB_ptr block = Allocate_Space(size);
    
    ASSERT_NE(block, nullptr);
    EXPECT_EQ(SA_GET_SIZE(block), size);
    
    free(block);
}

TEST_F(AllocateTest, Allocate_Space_CalculatesCorrectByteSize) {
    uint16_t size = 10;
    int32_t expected_bytes = (size + 1) * 16;
    
    SAMB_ptr block = Allocate_Space(size);
    ASSERT_NE(block, nullptr);
    
    // Verify we can access memory up to the expected size
    // Write pattern to verify allocation size
    memset(block, 0xFF, expected_bytes);
    
    free(block);
}

// ==============================================================================
// Check_Allocation Tests
// ==============================================================================

TEST_F(AllocateTest, Check_Allocation_ValidBlock) {
    SAMB_ptr block = Allocate_Space(50);
    ASSERT_NE(block, nullptr);
    
    int16_t result = Check_Allocation(block);
    EXPECT_EQ(result, ST_SUCCESS);
    
    free(block);
}

TEST_F(AllocateTest, Check_Allocation_InvalidSignature1) {
    SAMB_ptr block = Allocate_Space(50);
    ASSERT_NE(block, nullptr);
    
    // Corrupt first signature
    SET_2B_OFS(block, SAMB_MEMSIG1, 0xDEAD);
    
    int16_t result = Check_Allocation(block);
    EXPECT_EQ(result, ST_FAILURE);
    
    free(block);
}

TEST_F(AllocateTest, Check_Allocation_InvalidSignature2) {
    SAMB_ptr block = Allocate_Space(50);
    ASSERT_NE(block, nullptr);
    
    // Corrupt second signature
    SET_2B_OFS(block, SAMB_MEMSIG2, 0xBEEF);
    
    int16_t result = Check_Allocation(block);
    EXPECT_EQ(result, ST_FAILURE);
    
    free(block);
}

// ==============================================================================
// Allocate_First_Block Tests
// ==============================================================================

TEST_F(AllocateTest, Allocate_First_Block_Success)
{
    uint16_t main_size = 100;
    uint16_t sub_size = 20;
    
    SAMB_ptr main_block = Allocate_Space(main_size);

    ASSERT_NE(main_block, nullptr);
    
    SAMB_ptr sub_block = Allocate_First_Block(main_block, sub_size);

    ASSERT_NE(sub_block, nullptr);
    
    // Verify main block was updated
    EXPECT_EQ(SA_GET_USED(main_block), 1 + (1 + sub_size));
    
    // Verify sub-block header (16 bytes before sub_block)
    SAMB_ptr sub_header = sub_block - SZ_PARAGRAPH_B;
    EXPECT_EQ(GET_2B_OFS((sub_header), SAMB_RESERVED1), _BBBB);
    EXPECT_EQ(GET_2B_OFS((sub_header), SAMB_RESERVED2), _BBBB);
    EXPECT_EQ(SA_GET_MEMSIG1(sub_header), _SA_MEMSIG1);
    EXPECT_EQ(SA_GET_MEMSIG2(sub_header), _SA_MEMSIG2);
    EXPECT_EQ(SA_GET_SIZE(sub_header), sub_size);
    EXPECT_EQ(SA_GET_USED(sub_header), 1);
    EXPECT_EQ(GET_2B_OFS((sub_header), SAMB_unknown), _BBBB);
    EXPECT_EQ(SA_GET_MARK(sub_header), 1);

    free(main_block);
}

TEST_F(AllocateTest, Allocate_First_Block_SubBlockOffsetCorrect) {
    uint16_t main_size = 100;
    uint16_t sub_size = 20;
    
    SAMB_ptr main_block = Allocate_Space(main_size);
    ASSERT_NE(main_block, nullptr);
    
    SAMB_ptr sub_block = Allocate_First_Block(main_block, sub_size);
    ASSERT_NE(sub_block, nullptr);
    
    // Sub-block should be at main_block + 16 (main header) + 16 (sub header)
    ptrdiff_t expected_offset = 32;
    ptrdiff_t actual_offset = sub_block - main_block;
    EXPECT_EQ(actual_offset, expected_offset);
    
    free(main_block);
}

// ==============================================================================
// Allocate_Next_Block Tests
// ==============================================================================

TEST_F(AllocateTest, Allocate_Next_Block_AfterFirst) {
    uint16_t main_size = 200;
    uint16_t first_size = 20;
    uint16_t next_size = 30;
    
    SAMB_ptr main_block = Allocate_Space(main_size);
    ASSERT_NE(main_block, nullptr);
    
    SAMB_ptr first_block = Allocate_First_Block(main_block, first_size);
    ASSERT_NE(first_block, nullptr);
    
    uint16_t used_before = SA_GET_USED(main_block);
    
    SAMB_ptr next_block = Allocate_Next_Block(main_block, next_size);
    ASSERT_NE(next_block, nullptr);
    
    // Verify main block used increased
    EXPECT_EQ(SA_GET_USED(main_block), used_before + next_size + 1);
    
    // Verify next block header
    SAMB_ptr next_header = next_block - 16;
    EXPECT_EQ(GET_2B_OFS((next_header), SAMB_RESERVED1), _CCCC);
    EXPECT_EQ(GET_2B_OFS((next_header), SAMB_RESERVED2), _CCCC);
    EXPECT_EQ(SA_GET_MEMSIG1(next_header), _SA_MEMSIG1);
    EXPECT_EQ(SA_GET_MEMSIG2(next_header), _SA_MEMSIG2);
    EXPECT_EQ(SA_GET_SIZE(next_header), next_size);
    EXPECT_EQ(SA_GET_USED(next_header), 1);
    EXPECT_EQ(GET_2B_OFS((next_header), SAMB_unknown), _CCCC);
    EXPECT_EQ(GET_2B_OFS((next_header), SAMB_MARK), _CCCC);

    free(main_block);
}

TEST_F(AllocateTest, Allocate_Next_Block_MultipleAllocations) {
    uint16_t main_size = 500;
    SAMB_ptr main_block = Allocate_Space(main_size);
    ASSERT_NE(main_block, nullptr);
    
    SAMB_ptr block1 = Allocate_First_Block(main_block, 20);
    ASSERT_NE(block1, nullptr);
    
    SAMB_ptr block2 = Allocate_Next_Block(main_block, 30);
    ASSERT_NE(block2, nullptr);
    
    SAMB_ptr block3 = Allocate_Next_Block(main_block, 40);
    ASSERT_NE(block3, nullptr);
    
    // Total used should be: 1 + (1+20) + (1+30) + (1+40) = 94
    EXPECT_EQ(SA_GET_USED(main_block), 1 + (1+20) + (1+30) + (1+40));
    
    // Verify all blocks are different and properly spaced
    EXPECT_NE(block1, block2);
    EXPECT_NE(block2, block3);
    EXPECT_LT(block1, block2);
    EXPECT_LT(block2, block3);
    
    free(main_block);
}

// ==============================================================================
// Get_Free_Blocks Tests
// ==============================================================================

TEST_F(AllocateTest, Get_Free_Blocks_InitialState) {
    uint16_t size = 100;
    SAMB_ptr block = Allocate_Space(size);
    ASSERT_NE(block, nullptr);
    
    // Initial free blocks should be size - 1 (used starts at 1)
    uint16_t free_blocks = Get_Free_Blocks(block);
    EXPECT_EQ(free_blocks, size - 1);
    
    free(block);
}

TEST_F(AllocateTest, Get_Free_Blocks_AfterSubAllocations) {
    uint16_t main_size = 200;
    SAMB_ptr main_block = Allocate_Space(main_size);
    ASSERT_NE(main_block, nullptr);
    
    Allocate_First_Block(main_block, 20);
    uint16_t free_after_first = Get_Free_Blocks(main_block);
    
    Allocate_Next_Block(main_block, 30);
    uint16_t free_after_second = Get_Free_Blocks(main_block);
    
    // Each allocation consumes size + 1 blocks
    EXPECT_LT(free_after_second, free_after_first);
    EXPECT_EQ(free_after_first - free_after_second, 31); // 30 + 1
    
    free(main_block);
}

// ==============================================================================
// Mark_Block and Release_Block Tests
// ==============================================================================

TEST_F(AllocateTest, Mark_And_Release_Block) {
    uint16_t main_size = 100;
    SAMB_ptr main_block = Allocate_Space(main_size);
    ASSERT_NE(main_block, nullptr);
    
    Allocate_First_Block(main_block, 20);
    uint16_t used_after_first = SA_GET_USED(main_block);
    
    // Mark current state
    Mark_Block(main_block);
    EXPECT_EQ(SA_GET_MARK(main_block), used_after_first);
    
    // Allocate more
    Allocate_Next_Block(main_block, 30);
    uint16_t used_after_second = SA_GET_USED(main_block);
    EXPECT_GT(used_after_second, used_after_first);
    
    // Release back to mark
    Release_Block(main_block);
    EXPECT_EQ(SA_GET_USED(main_block), used_after_first);
    
    free(main_block);
}

TEST_F(AllocateTest, Reset_First_Block) {
    uint16_t main_size = 100;
    SAMB_ptr main_block = Allocate_Space(main_size);
    ASSERT_NE(main_block, nullptr);
    
    Allocate_First_Block(main_block, 20);
    Allocate_Next_Block(main_block, 30);
    
    // Reset should set used back to 1
    Reset_First_Block(main_block);
    EXPECT_EQ(SA_GET_USED(main_block), 1);
    
    free(main_block);
}

// ==============================================================================
// Near Buffer Allocation Tests
// ==============================================================================

TEST_F(AllocateTest, Near_Allocate_Reset) {
    Near_Allocate_Reset();
    EXPECT_EQ(near_buffer_used, 256);
    EXPECT_EQ(near_buffer_mark, 256);
}

TEST_F(AllocateTest, Near_Allocate_First) {
    int16_t size = 100;
    byte_ptr ptr = Near_Allocate_First(size);
    
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(near_buffer_used, size + 256);
    EXPECT_EQ(near_buffer_mark, size + 256);
    
    // Verify pointer is in near_buffer range
    EXPECT_GE(ptr, (byte_ptr)near_buffer);
    EXPECT_LT(ptr, (byte_ptr)(near_buffer + 4400));
}

TEST_F(AllocateTest, Near_Allocate_Next) {
    int16_t first_size = 100;
    int16_t next_size = 50;
    
    Near_Allocate_First(first_size);
    int16_t used_after_first = near_buffer_used;
    
    byte_ptr next_ptr = Near_Allocate_Next(next_size);
    ASSERT_NE(next_ptr, nullptr);
    
    EXPECT_EQ(near_buffer_used, used_after_first + next_size);
}

TEST_F(AllocateTest, Near_Allocate_Mark_And_Undo) {
    Near_Allocate_First(100);
    int16_t used_after_first = near_buffer_used;
    
    Near_Allocate_Mark();
    EXPECT_EQ(near_buffer_mark, used_after_first);
    
    Near_Allocate_Next(50);
    EXPECT_GT(near_buffer_used, used_after_first);
    
    Near_Allocate_Undo();
    EXPECT_EQ(near_buffer_used, used_after_first);
}

TEST_F(AllocateTest, Near_Allocate_MultipleAllocations) {
    byte_ptr ptr1 = Near_Allocate_First(100);
    byte_ptr ptr2 = Near_Allocate_Next(200);
    byte_ptr ptr3 = Near_Allocate_Next(300);
    
    ASSERT_NE(ptr1, nullptr);
    ASSERT_NE(ptr2, nullptr);
    ASSERT_NE(ptr3, nullptr);
    
    // All pointers should be different
    EXPECT_NE(ptr1, ptr2);
    EXPECT_NE(ptr2, ptr3);
    EXPECT_NE(ptr1, ptr3);
    
    // Pointers should be in sequence
    EXPECT_LT(ptr1, ptr2);
    EXPECT_LT(ptr2, ptr3);
}

// ==============================================================================
// Allocate_Space_No_Header Tests
// ==============================================================================

TEST_F(AllocateTest, Allocate_Space_No_Header_Success)
{
    uint16_t size = 50;

    SAMB_ptr block = Allocate_Space_No_Header(size);
    
    ASSERT_NE(block, nullptr);
    
    // Capture the base pointer immediately after allocation
    // before tmp_SAMB_head could be overwritten by another call
    SAMB_ptr base_ptr = tmp_SAMB_head;
    
    // This version returns data pointer at offset +12 from the allocated base
    // Verify the offset is correct
    EXPECT_EQ(block, base_ptr + 12);
    
    // Free from the actual malloc'd address
    free(base_ptr);
}

// ==============================================================================
// Memory Signature Constants Tests
// ==============================================================================

TEST(AllocateConstantsTest, MemorySignatures) {
    EXPECT_EQ(_SA_MEMSIG1, 0x12FA);
    EXPECT_EQ(_SA_MEMSIG2, 0x4ECF);
}

TEST(AllocateConstantsTest, MarkerConstants) {
    EXPECT_EQ(_AAAA, 0xAAAA);
    EXPECT_EQ(_BBBB, 0xBBBB);
    EXPECT_EQ(_CCCC, 0xCCCC);
}

// ==============================================================================
// Integration Tests
// ==============================================================================

TEST_F(AllocateTest, Integration_CompleteAllocationCycle) {
    // Allocate main block
    uint16_t main_size = 500;
    SAMB_ptr main = Allocate_Space(main_size);
    ASSERT_NE(main, nullptr);
    
    // Allocate first sub-block
    SAMB_ptr sub1 = Allocate_First_Block(main, 50);
    ASSERT_NE(sub1, nullptr);
    
    // Mark this state
    Mark_Block(main);
    uint16_t marked_used = SA_GET_USED(main);
    
    // Allocate more sub-blocks
    SAMB_ptr sub2 = Allocate_Next_Block(main, 60);
    SAMB_ptr sub3 = Allocate_Next_Block(main, 70);
    ASSERT_NE(sub2, nullptr);
    ASSERT_NE(sub3, nullptr);
    
    // Check free space
    uint16_t free_space = Get_Free_Blocks(main);
    EXPECT_GT(free_space, 0);
    
    // Release back to mark (undoes sub2 and sub3)
    Release_Block(main);
    EXPECT_EQ(SA_GET_USED(main), marked_used);
    
    // Verify we can allocate again
    SAMB_ptr sub4 = Allocate_Next_Block(main, 40);
    ASSERT_NE(sub4, nullptr);
    
    free(main);
}

TEST_F(AllocateTest, Integration_NearAndFarAllocation) {
    // Test that near buffer and far allocation don't interfere
    
    // Near allocation
    byte_ptr near_ptr = Near_Allocate_First(100);
    ASSERT_NE(near_ptr, nullptr);
    
    // Far allocation
    SAMB_ptr far_ptr = Allocate_Space(100);
    ASSERT_NE(far_ptr, nullptr);
    
    // Should be completely different memory regions
    EXPECT_NE((void*)near_ptr, (void*)far_ptr);
    
    // Both should be usable
    memset(near_ptr, 0xAA, 100);
    memset(far_ptr, 0xBB, 16);
    
    free(far_ptr);
}
