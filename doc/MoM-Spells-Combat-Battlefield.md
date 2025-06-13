
NOTE: 'Counter Magic' uses count, rather than {F,T}

'Counter Magic' is in its own SCC, but takes the same code-path as the battlefield spells

Seems mostly just 'Combat Enchantment', but ...
...includes 'Counter Magic'
...includes 'Flame Strike'

all call Combat_Spell_Animation__WIP()
excludes 
    spl_Flame_Strike
    spl_Holy_Word
    spl_Death_Spell
    spl_Call_Chaos
    spl_Mass_Healing
from setting combat_enchantments[]
