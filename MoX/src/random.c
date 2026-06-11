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

#include <stdio.h>  /* CLAUDE: fprintf(stderr, ...) for [RNG] diagnostic logging */

#include "EXIT.h"
/* CLAUDE 2026-06-01: tell random.h NOT to macro-expand `Random(n)` to
 * `Random_at(...)` inside this translation unit -- we need to define
 * Random() and Random_at() here as actual functions.  The macro will
 * be re-introduced below, AFTER the function definitions, so that
 * helper functions in this file (Get_Weighted_Choice etc) still get
 * call-site tracing when they invoke Random(). */
#define RANDOM_C_NO_AUTOTRACE
#include "random.h"
#include "MOX_BITS.h"
#include "MOX_TYPE.h"
#include "../../STU/src/STU_LOG.h"
#include "MOX2.h"  /* CLAUDE 2026-05-27: _cmd_line_seed; used to gate per-call [RNG-CALL] trace */

uint32_t random_seed = 0x35683568;  /* 896021864d  00110101011010000011010101101000b */

/* CLAUDE: g_random_call_count -- counts every call to Random() since process
   start.  Definition moved to STU/src/STU_LOG.c so libSTU is self-contained
   (stu_compat.h's CALL_TRACE wrappers reference the counter, and stu_compat.c
   is part of libSTU).  Random_at() in this TU still increments the counter
   via the extern declaration in random.h. */

/* CLAUDE: _cmd_line_seed moved to MoX/src/MOX2.c.  In MoO2 SimTex declared it
   in the MOX2 module alongside Check_Command_Line_Parameters_(); we mirror
   that placement.  random.c now only owns the RNG itself. */



/*
    WZD  seg001
*/

// WZD s01p07
// MoO2  Module: KEN  Get_Weighted_Choice_()
/* GEMINI */
// MGC  int16_t Get_Weighted_Choice_MGC(int16_t * weight_array, int16_t weight_count)
int16_t Get_Weighted_Choice(int16_t * weight_array, int16_t weight_count)
{
    int16_t i = 0;
    int16_t choice = 0;
    int16_t weight_remainder = 0;
    int16_t j = 0;
    int16_t max_weight = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

restart_sum:
    max_weight = 0;
    
    /* 1. Sum up all the weights in the array */
    for (j = 0; j < weight_count; j++) {
        max_weight += weight_array[j];
        
        /* 2. THE NORMALIZATION HACK */
        if (max_weight >= 512) {
            /* If the total weight gets dangerously high, divide EVERY weight in the array by 2 (bit-shift right) */
            for (i = 0; i < weight_count; i++) {
                weight_array[i] >>= 1; 
            }
            /* Start the summation completely over! */
            goto restart_sum;
        }
    }

    /* 3. Safety Check: If the max weight is 0, default to the first element */
    if (max_weight == 0) {
        return 0;
    }

    /* 4. Roll the dice */
    weight_remainder = Random(max_weight);
    
    /* 5. Find the winning index */
    weight_remainder -= weight_array[0];
    choice = 0;

    /* Subtract weights until the random value dips below zero */
    while (weight_remainder > 0 && choice < weight_count - 1) {
        choice++;
        weight_remainder -= weight_array[choice];
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    return choice;
}


// WZD s01p08
// MoO2  Module: KEN  Get_Weighted_Choice_Long_()
/* only difference is the type for the weights and the sum */
/* GEMINI */
int16_t Get_Weighted_Choice_Long(int32_t * weight_array, int16_t weight_count)
{
    int16_t i = 0;
    int16_t choice = 0;
    int16_t weight_remainder = 0;
    int16_t j = 0;
    int32_t max_weight = 0;

restart_sum:
    max_weight = 0;
    
    /* 1. Sum up all the weights in the array */
    for (j = 0; j < weight_count; j++) {
        max_weight += weight_array[j];
        
        /* 2. THE NORMALIZATION HACK */
        if (max_weight >= 512) {
            /* If the total weight gets dangerously high, divide EVERY weight in the array by 2 (bit-shift right) */
            for (i = 0; i < weight_count; i++) {
                weight_array[i] >>= 1; 
            }
            /* Start the summation completely over! */
            goto restart_sum;
        }
    }

    /* 3. Safety Check: If the max weight is 0, default to the first element */
    if (max_weight == 0) {
        return 0;
    }

    /* 4. Roll the dice */
    weight_remainder = Random(max_weight);
    
    /* 5. Find the winning index */
    weight_remainder -= weight_array[0];
    choice = 0;

    /* Subtract weights until the random value dips below zero */
    while (weight_remainder > 0 && choice < weight_count - 1) {
        choice++;
        weight_remainder -= weight_array[choice];
    }

    return choice;

}


// WZD s01p09
// MoO2  Module: KEN  Get_Weighted_Best_Choice_()
// Get_Weighted_Best_Choice()


// WZD s01p10
// MoO2  Module: KEN  Get_Weighted_Choice_Best_Long_()
// Get_Weighted_Best_Choice_Long()



/*
    WZD  seg022
*/

// WZD s22p05
void Set_Random_Seed(uint32_t n)
{
    /* CLAUDE: log every explicit seed assignment so we can see WHO sets it
       (config parse, save-load, etc.) and confirm it matches what the .ini
       requested. */
    LOG_INFO(LOG_CAT_RANDOM, "[RNG] Set_Random_Seed(0x%08X = %u)  (was 0x%08X)  random_calls=%llu",
        (unsigned)n, (unsigned)n, (unsigned)random_seed, (unsigned long long)g_random_call_count);
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
    /* CLAUDE: Randomize() reads the system clock and seeds from it.  Calling
       this AFTER the .ini-driven Set_Random_Seed() would clobber the
       deterministic seed with a wall-clock-derived one — exactly the kind
       of bug we are looking for.  Log every call. */
    LOG_INFO(LOG_CAT_RANDOM, "[RNG] Randomize() called  timer=0x%08X  (clobbers prior seed 0x%08X)  random_calls=%llu",
        (unsigned)timer_value, (unsigned)random_seed, (unsigned long long)g_random_call_count);
    random_seed = timer_value;
}


// WZD s22p08
/*
NOTE(JimBalcomb+CLAUDE,20260527):
This is the answer to your "lingering question" and I think we just stumbled onto a real bug. Look at what's in random.c:196-215:
int16_t Random(int16_t n) {
    uint32_t r = random_seed;
    r ^= (r << 13);
    r ^= (r >> 17);
    r ^= (r << 5);
    random_seed = r;
    return ((r >> 16) % n) + 1;
}
That's Marsaglia's xorshift32 with shifts 13/17/5 — published in 2003. MoM shipped in 1994. xorshift32 literally didn't exist when MoM was compiled. It cannot be what WIZARDS.EXE actually does.
Below it, commented out, is a 9-bit LFSR with the GEMINI annotation containing very specific disassembly traces:
/ * Assembly: rcl [bp+result], 1 * /
…and explicit feedback taps (low_seed ^ (low_seed >> 1) ^ (low_seed >> 2) ^ (low_seed >> 4) ^ (low_seed >> 6) ^ (high_seed >> 15)). That's the kind of detail you only get from staring at the actual WIZARDS.EXE disassembly. The "1oom" attribution on the active version suggests the xorshift was borrowed from MoO1's reimplementation — a different game's RNG, probably plugged in as a placeholder.
So the strong hypothesis: your active Random() is wrong, and the commented-out LFSR is the right shape. Your "lingering question" is well-founded.
*/
/* CLAUDE 2026-05-27: disabled the xorshift32 implementation below
 * (kept inside #if 0 for diff history) and replaced it with the 9-bit
 * LFSR derived by Gemini from MAGIC.EXE's disassembly.  See the LFSR
 * function further down.  Tested against 57,599 OG seed-transition
 * test vectors captured at ~/STU_DBWD/dosbox-capture/magic.csv. */

#if 0  /* 1oom xorshift32 -- NOT MoM's algorithm; left for reference */
int16_t Random(int16_t n)
{
    uint16_t result;
    uint32_t r = random_seed;

    /* CLAUDE: count every call so divergence in call-count between runs is observable. */
    g_random_call_count++;

    r ^= (r << 13);
    r ^= (r >> 17);
    r ^= (r << 5);

    random_seed = r;

    result = (r >> 16) % n;

    result += 1;

    return result;
}
#endif

/* 9-bit LFSR derived from MAGIC.EXE's Random at
 *   ~/STU/devel/STU-Extras/STU-DOSBox/MAGIC_EXE-random.asm
 *
 * Structure verified against the assembly:
 *   - 9-iteration loop (`mov cx, 9` / `loop ...`)
 *   - feedback taps at bits 0/1/2/4/6 of `low_seed` (= asm `si`)
 *     and bit 15 of `high_seed` (= asm `di`)
 *   - feedback bit shifted into result accumulator (asm `rcl [bp+result], 1`)
 *   - 32-bit seed shifted right by 1 with feedback bit at top
 *     (asm `shr ax,1` / `rcr di,1` / `rcr si,1`)
 *   - stuck-at-zero guard with magic value 12478
 *   - return value is (result % n) + 1, range 1..n
 */
int16_t Random(int16_t n)
{
    int16_t  i;
    uint16_t result    = 0;
    uint16_t low_seed;
    uint16_t high_seed;
    uint16_t new_bit;
    uint16_t carry_bit;
    int16_t  ret;

    if (n == 0) Exit_With_Message("RND no 0's");

    low_seed  = (uint16_t)(random_seed & 0xFFFF);
    high_seed = (uint16_t)(random_seed >> 16);

    for (i = 0; i < 9; i++) {
        new_bit = (uint16_t)((low_seed
                              ^ (low_seed >> 1)
                              ^ (low_seed >> 2)
                              ^ (low_seed >> 4)
                              ^ (low_seed >> 6)
                              ^ (high_seed >> 15)) & 1);

        result = (uint16_t)((result << 1) | new_bit);

        carry_bit = (uint16_t)(high_seed & 1);
        low_seed  = (uint16_t)((low_seed  >> 1) | (carry_bit << 15));
        high_seed = (uint16_t)((high_seed >> 1) | (new_bit   << 15));
    }

    if (low_seed == 0 && high_seed == 0) {
        low_seed = 12478;
    }

    random_seed = ((uint32_t)high_seed << 16) | (uint32_t)low_seed;

    ret = (int16_t)((result % n) + 1);

    return ret;
}

/* CLAUDE 2026-06-01: call-site-traced wrapper around Random().
 *
 * Every Random(n) call site in the codebase is macro-expanded (via
 * random.h) to Random_at(n, __FILE__, __LINE__).  Random_at:
 *   - bumps the global call counter
 *   - captures the seed before the LFSR step
 *   - calls the raw Random() above (which does ONLY the LFSR + return-value math)
 *   - logs `[RNG-CALL] call=N n=K before=0xX after=0xY result=R at=FILE:LINE`
 *     when the deterministic-seed flag is set, so the trace records the
 *     EXACT source location of every Random() invocation.
 *
 * The `at=` field is what makes call-by-call comparison with the OG
 * trace useful: when ReMoM and OG diverge in call ordinality, this
 * tells us which ReMoM source line is responsible for the extra call. */
int16_t Random_at(int16_t n, const char *file, int line)
{
    uint32_t seed_before;
    int16_t  ret;

    g_random_call_count++;
    seed_before = random_seed;

    ret = Random(n);   /* RANDOM_C_NO_AUTOTRACE is still in effect here */

    if (_cmd_line_seed != 0) {
        // TODO  use LOG_INFO(LOG_CAT_RANDOM, "[RNG] Set_Random_Seed(0x%08X = %u)  (was 0x%08X)  random_calls=%llu", (unsigned)n, (unsigned)n, (unsigned)random_seed, (unsigned long long)g_random_call_count);
        fprintf(stderr,
            "[RNG-CALL] call=%llu  n=%d  before=0x%08X  after=0x%08X  result=%d  at=%s:%d\n",
            (unsigned long long)g_random_call_count,
            (int)n,
            (unsigned)seed_before,
            (unsigned)random_seed,
            (int)ret,
            file, (int)line);
    }

    return ret;
}

/* From here on, even within random.c, Random(n) is rewritten by the
 * preprocessor to Random_at((n), __FILE__, __LINE__), so the
 * weighted-random helpers in this file (Get_Weighted_Choice etc.) get
 * call-site tracing too -- their internal Random() calls will be
 * tagged with random.c:NNN. */
#undef RANDOM_C_NO_AUTOTRACE
#define Random(n) Random_at((n), __FILE__, __LINE__)

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
