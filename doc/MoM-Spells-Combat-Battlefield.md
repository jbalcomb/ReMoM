
Combat Battlefield Instant
Combat Battlefield Enchantment

¿ Combat Battlefield Instant ?
    Flame Strike, Holy Word, Death Spell, Call Chaos, Mass Healing
¿ Combat Battlefield Enchantment ?
    Black Prayer, Blur, Call Lightning, Darkness, Entangle, High Prayer, Mana Leak, Mass Invisibility, Metal Fires, Prayer, Terror, True Light, Warp Reality, Wrack



NOTE: 'Counter Magic' uses count, rather than {F,T}

'Counter Magic' is in its own SCC, but takes the same code-path as the battlefield spells

Seems mostly just 'Combat Enchantment', but ...
...includes 'Counter Magic'

all call Combat_Spell_Animation__WIP()
excludes 
    spl_Flame_Strike
    spl_Holy_Word
    spl_Death_Spell
    spl_Call_Chaos
    spl_Mass_Healing
from setting combat_enchantments[]
