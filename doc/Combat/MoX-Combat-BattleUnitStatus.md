


Elsewhere, ...
TILE_FleeTo() checks for (battle_units[itr_battle_units].unit_idx != -1)



```c
enum e_BATTLE_UNIT_STATUS
{
    bus_Active      = 0,  /* ~== Alive */
    bus_Recalled    = 1,  /* ¿ ? */
    bus_Fleeing     = 2,  /* ¿ ? */
    bus_Uninvolved  = 3,  /* ¿ ? */
    bus_Dead        = 4,  /* ¿ ? */
    bus_Drained     = 5,  /* ¿ ? */
    bus_Gone        = 6   /* ¿ ? ¿ ~== _UNITS[].wp == 9 ? */
};
```

When and where do these get set?

What makes use of them?



In `End_Of_Combat()`, ...
...*Undead* code checks (battle_units[itr_battle_units].Status == bus_Drained)
...*Zombie* code checks (battle_units[itr_battle_units].Status == bus_Dead)



### Strategic_Combat()



### End_Of_Combat()



### WIZ_FleeCombat()

```c
if(_UNITS[battle_units[itr_battle_units].unit_idx].wp == 9)
{
    battle_units[itr_battle_units].Status = bus_Dead;
}
```

```c
if(battle_units[itr_battle_units].Status == bus_Uninvolved)
{
    battle_units[itr_battle_units].Status = bus_Fleeing;
}
```

if Black Sleep, Confusion, or Web
    sets status back to bus_Fleeing
