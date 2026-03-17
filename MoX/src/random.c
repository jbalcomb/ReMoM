/*
    WIZARDS.EXE
    seg001
    seg022

MoO2
    Module: random
        unsigned integer (4 bytes) random_seed
        Random()
        Set_Random_Seed()
        Get_Random_Seed()
        Randomize()

*/

#include "EXIT.h"
#include "random.h"
#include "MOX_BITS.h"
#include "MOX_TYPE.h"

uint32_t random_seed = 0x35683568;



/*
    WZD  seg001
*/

// WZD s01p07
// MoO2  Module: KEN  Get_Weighted_Choice_()
int16_t Get_Weighted_Choice(int16_t * weight_array, int16_t weight_count)
{
    int16_t Condense_Loop_Var = 0;
    int16_t Picked_List_Item = 0;
    int16_t Weights_Remainder = 0;
    int16_t itr = 0;
    int16_t tmp_pick = 0;
    int16_t return_value = 0;
    do
    {
        tmp_pick = 0;
        tmp_pick += weight_array[itr];
        if(tmp_pick < 512)
        {

            itr++;

        }
        else
        {
            for(Condense_Loop_Var = 0; Condense_Loop_Var < weight_count; Condense_Loop_Var++)
            {
                weight_array[Condense_Loop_Var] = (weight_array[Condense_Loop_Var] / 2);
            }
        }
    } while (itr < weight_count);
    if(tmp_pick == 0)
    {
        return_value = 0;

    }
    else
    {
        Weights_Remainder = (Random(tmp_pick) - weight_array[0]);
        Picked_List_Item = 0;
        while(
            (Weights_Remainder > 0)
            &&
            (weight_count - 1) > Picked_List_Item)
        {
            Picked_List_Item++;
            Weights_Remainder -= weight_array[Picked_List_Item];
        }
        return_value = Picked_List_Item;
    }
    return return_value;
}


// WZD s01p08
// drake178: RNG_WeightedPick32()
// MoO2  Module: KEN  Get_Weighted_Choice_Long_()


// WZD s01p09
// drake178: UU_RNG_HighestPick16()
// MoO2  Module: KEN  Get_Weighted_Best_Choice_()

// WZD s01p10
// drake178: UU_RNG_HighestPick32()
// MoO2  Module: KEN  Get_Weighted_Choice_Best_Long_()



/*
    WZD  seg022
*/

// WZD s22p05
void Set_Random_Seed(uint32_t n)
{
    random_seed = n;
}


// WZD s22p06
uint32_t Get_Random_Seed(void)
{
    return random_seed;
}


// WZD s22p07
void Randomize(void)
{
    uint32_t timer_value;
    timer_value = (uint32_t)Read_System_Clock_Timer();
    random_seed = timer_value;
}


// WZD s22p08
// 1oom
int16_t Random(int16_t n)
{
    uint16_t result;
    uint32_t r = random_seed;

    r ^= (r << 13);
    r ^= (r >> 17);
    r ^= (r << 5);
    
    random_seed = r;

    result = (r >> 16) % n;

    result += 1;

    return result;
}
// GEMINI
// int16_t Random(int n) {
//     int16_t i = 0;
//     uint16_t result = 0;
//     uint16_t low_seed = 0;
//     uint16_t high_seed = 0;
//     uint16_t new_bit = 0;
//     uint16_t carry_bit = 0;
// 
//     low_seed = (uint16_t)(random_seed & 0xFFFF);
//     high_seed = (uint16_t)(random_seed >> 16);
// 
//     if (n == 0) Exit_With_Message("RND no 0's");
// 
//     /* 9-bit LFSR bit-shifting loop */
//     for (i = 0; i < 9; i++) {
//         /* 1. Calculate the Feedback Bit
//          * We XOR the specific tapped bits together. 
//          * By shifting them all down, the feedback result ends up in bit 0.
//          * We use '& 1' at the end to mask off everything except that final bit.
//          */
//         new_bit = (low_seed ^ 
//                   (low_seed >> 1) ^ 
//                   (low_seed >> 2) ^ 
//                   (low_seed >> 4) ^ 
//                   (low_seed >> 6) ^ 
//                   (high_seed >> 15)) & 1;
// 
//         /* 2. Push the generated bit into the 9-bit result queue */
//         /* Assembly: rcl [bp+result], 1 */
//         result = (result << 1) | new_bit;
//         
//         /* 3. The 32-bit Shift Right
//          * Because C89 in DOS uses 16-bit integers, we have to manually 
//          * carry the bit that falls off the bottom of high_seed into 
//          * the top of low_seed, just like the assembly's RCR instructions.
//          */
//         
//         /* Grab the lowest bit of high_seed before we shift it */
//         carry_bit = high_seed & 1; 
//         
//         /* Shift low_seed right, and drop the carry_bit into its 16th slot (bit 15) */
//         low_seed = (low_seed >> 1) | (carry_bit << 15);
//         
//         /* Shift high_seed right, and drop our calculated new_bit into ITS top slot */
//         high_seed = (high_seed >> 1) | (new_bit << 15);
//     }
// 
//     if (low_seed == 0 && high_seed == 0) {
//         low_seed = 12478; /* Prevent the LFSR from getting stuck at 0 */
//     }
// 
//     random_seed = ((uint32_t)high_seed << 16) | low_seed;
// 
//     /* Modulo division to fit the requested range, returns 1 to N */
//     return (result % n) + 1;
// }
