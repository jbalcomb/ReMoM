
standard roulette wheel selection



relies on an individual’s fitness proportionate to the population’s total fitness



// WZD s01p08
// MoO2  Module: KEN  Get_Weighted_Choice_Long_()
/* GEMINI */
/* Using 'long' because in 16-bit DOS C, 'int' was 16-bit and 'long' was 32-bit */
int16_t Get_Weighted_Choice_Long(int32_t * weight_array, int16_t weight_count)
{
    int16_t Condense_Loop_Var;
    int16_t choice;
    int16_t weight_remainder;
    int16_t itr;
    int32_t weight_total;

restart_sum:
    weight_total = 0;
    
    /* 1. Sum up all the 32-bit weights */
    for (itr = 0; itr < weight_count; itr++) {
        weight_total += weight_array[itr];
        
        /* 2. THE NORMALIZATION HACK */
        if (weight_total >= 512) {
            /* If total hits 512, divide every 32-bit weight by 2 */
            for (Condense_Loop_Var = 0; Condense_Loop_Var < weight_count; Condense_Loop_Var++) {
                weight_array[Condense_Loop_Var] /= 2; 
            }
            goto restart_sum;
        }
    }

    /* 3. Safety Check */
    if (weight_total == 0) {
        return 0;
    }

    /* --- THE GENIUS TRUNCATION OPTIMIZATION --- */
    
    /* Because the loop above guarantees weight_total is strictly less than 512,
       the upper 16-bits of the total AND the upper 16-bits of every remaining 
       weight in the array are guaranteed to be exactly ZERO! 
       
       So, the compiler completely abandons 32-bit math for the rest of the function
       and just uses standard 16-bit integers! */

    /* Pass ONLY the lower 16-bits of weight_total to the random generator */
    weight_remainder = Random(weight_total);
    
    /* Subtract ONLY the lower 16-bits of the first weight */
    weight_remainder -= weight_array[0];
    
    choice = 0;

    /* Subtract ONLY the lower 16-bits of the remaining weights */
    while (weight_remainder > 0 && choice < weight_count - 1) {
        choice++;
        weight_remainder -= weight_array[choice];
    }

    return choice;

}



It is literally the "remainder of the random roll" after subtracting the weights of the items you've already checked.
By using subtraction instead of addition, the engine saves having to allocate an extra register in the CPU for a running_total. It just modifies the random roll variable in place until it dies!






https://algorithmafternoon.com/books/genetic_algorithm/chapter04/


https://en.wikipedia.org/wiki/Fitness_proportionate_selection


http://lipowski.home.amu.edu.pl/homepage/roulette.html


https://www.educative.io/answers/what-is-roulette-wheel-selection

