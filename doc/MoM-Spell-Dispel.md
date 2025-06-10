


NOTE: also call from Disenchant



Calculate_Dispel_Difficulty()
||
5 * Calculate_Dispel_Difficulty()

strength of dispel




strength + difficult



IDGI
How are they all `if(Random(250) <= Dispel_Chance)`?

e.g., TSCC == 25
Potency = ((500 / (500 + TSCC)) * 100)
Potency = ((500 / (500 +   25)) * 100)
Potency = ((500 / (525       )) * 100)
Potency = ((500 / (525       )) * 100)
Potency = ((0.952380952380952)) * 100)
Potency = ((0.952380952380952)) * 100)
Potency = ((95.2380952380952380952380)


https://masterofmagic.fandom.com/wiki/Dispel
Dispel Chance (%) = Dispel Strength / (Dispel Strength + Spell Cost) × 100
Note that the game does not actually use a percentage for dispel chance. Internally, the multiplier at the end of the formula is 250, and the result is correspondingly checked against a random value between 1 and 250 to determine success or failure. While this does give the function a slightly increased statistical precision, it is still conceptually identical.
https://masterofmagic.fandom.com/wiki/Dispel_Magic
Examples
Below are some examples of Dispel Magic and Dispel Magic True at work.
Example #1
A unit has two Unit Enchantments affecting it, one with a Casting Cost of  20, and another with  50.
Dispel Magic
Dispel Magic is cast on this unit, with no additional  Mana.
Total Casting Cost:  10
Dispel Strength:  10
Dispel chance against the first spell:
(10 ÷ (10 + 20)) × 100
= (10 ÷ 30) × 100
= 0.33 × 100
= 33%




Cast_Spell_Overland__WIP()
        // "Tries to prevent, with a strength 500 dispelling force, all enemy overland spells and enchantments."

        // To calculate Suppress Magic's dispelling potency against the target spell, the game uses the following formula:
        // Potency = (500 / (500 + TSCC)) * 100
        // Where TSCC is the total Casting Cost of the targeted enemy spell.

        for(itr_players = 0; ((itr_players < _num_players) && (Cast_Successful == ST_TRUE)); itr_players++)
        {

            if(
                (_players[itr_players].Globals[SUPPRESS_MAGIC] > 0)
                &&
                (itr_players != player_idx)
            )
            {

                Dispel_Chance = (500 + Calculate_Dispel_Difficulty(_players[player_idx].casting_cost_original, player_idx, spell_data_table[spell_idx].magic_realm));

                Dispel_Chance = (125000 / Dispel_Chance);  // 250 * 500 = 125000

                if(Random(250) <= Dispel_Chance)
                {

                    Fizzle_Notification(player_idx, itr_players, spell_idx, str_SuppressMagic);

                    Cast_Successful = ST_FALSE;

                }

            }

        }
