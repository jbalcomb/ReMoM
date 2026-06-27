AISPELL-AITP_Change_Terrain.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_ChangeTerrain__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_ChangeTerrain__WIP.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Change_Terrain()

Cast_Spell_Overland()
    // map-square target lookup at cast time
    |-> Get_Map_Square_Target_For_Spell()
        switch(spell_idx)
            case spl_Change_Terrain:
                |-> AITP_Change_Terrain()

---

# `AITP_Change_Terrain` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Change_Terrain` | [AISPELL.c:2958-3030](../../MoM/src/AISPELL.c#L2958-L3030) | AI target picker for **Change Terrain**: of the caster's own cities, find the **highest AI-valued** one that has a **desert or swamp** tile in its 5×5 catchment, and return that map square (`*targeted_wx`,`*targeted_wy`,`*targeted_wp`). Returns `ST_TRUE` if a target was found, `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:2958](../../MoM/src/AISPELL.c#L2958)) is a direct, faithful reconstruction of `AITP_ChangeTerrain__WIP.asm`; the production symbol dropped the `__WIP` tag (the asm/`.c` filename keeps it). It compiles and links as part of AISPELL.c / momlib. Same catchment-scan shape as [`AITP_Transmute`](AISPELL-AITP_Transmute.md), but it tests **terrain type** (desert/swamp) instead of mineral specials. One preserved OG quirk ([B1](#b1-ogquirk--55-catchment-includes-corners)). The asm is the authority.

## Purpose

The `AITP_*` target picker for **Change Terrain**, which the AI uses to improve a bad tile in one of its own cities' catchments. It scans each owned city's catchment for a desert or swamp square and aims at the one belonging to its most valuable city. "Value" is the precomputed AI city-valuation table `_ai_all_own_city_values[]` (asm `AI_Own_City_Values`), keyed by city index. Like `AITP_Transmute`, it returns a **map square** (x, y, plane), not a city index.

Restricting targets to desert/swamp is the spell's **intended** behavior (Change Terrain upgrades poor terrain), not a defect — drake178 marks it a *WARNING*, not a *BUG*. The production header comment `¿ OGBUG only targets deserts and swamps ?` is answered here: **by design, not a bug.**

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2332](../../MoM/src/AISPELL.c#L2332) | **Probe**: if a target exists, Change Terrain is given a fixed priority weight (`AI_OVL_SplPriorities[3] = 50`). |
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:4105-4108](../../MoM/src/AISPELL.c#L4105-L4108) | Cast-time lookup (`switch(spell_idx) case spl_Change_Terrain`) — fills the caller's `wx`/`wy`/`wp` for the actual cast (now fully enabled, like the Transmute case). |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_ChangeTerrain__WIP.asm`. World dims: `WORLD_WIDTH=60`, `WORLD_HEIGHT=40`, `WORLD_YMIN=0` ([MOM_DEF.h:271-272](../../MoX/src/MOM_DEF.h#L271-L272), [320](../../MoX/src/MOM_DEF.h#L320)).

```c
target_city_idx = ST_UNDEFINED;                                       // asm  Target_City = -1
highest_value = 0;                                                    // asm  Highest_Value = 0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)              // asm  si loop
{
    if(_CITIES[itr_cities].owner_idx == player_idx)                  // asm  owner_idx (cbw) == player_idx
    {
        city_wp = _CITIES[itr_cities].wp;                            // asm  Plane = wp (cbw)

        for(wy_offset = -2; wy_offset < 3; wy_offset++)              // asm  Y_Modifier -2..2
        {
            city_area_wy = _CITIES[itr_cities].wy + wy_offset;       // asm  Tile_Y = wy + Y_Modifier
            if(city_area_wy >= WORLD_YMIN && city_area_wy < WORLD_HEIGHT)  // asm  Tile_Y>=WORLD_YMIN && <WORLD_HEIGHT
            {
                for(wx_offset = -2; wx_offset < 3; wx_offset++)      // asm  X_Modifier -2..2
                {
                    city_area_wx = _CITIES[itr_cities].wx + wx_offset;           // asm  di = wx + X_Modifier
                    if(city_area_wx < 0)            city_area_wx += WORLD_WIDTH;  // asm  di<0:  di+=60  (E/W wrap)
                    if(city_area_wx >= WORLD_WIDTH) city_area_wx -= WORLD_WIDTH;  // asm  di>=60: di-=60

                    if( Square_Is_Desert(city_area_wx, city_area_wy, city_wp) == ST_TRUE   // asm  call Square_Is_Desert / jz match
                     || Square_Is_Swamp (city_area_wx, city_area_wy, city_wp) == ST_TRUE ) // asm  call Square_Is_Swamp  / jnz skip
                    {
                        if(_ai_all_own_city_values[itr_cities] > highest_value)  // asm  jle skip  =>  strictly greater
                        {
                            target_city_idx = itr_cities;             // asm  Target_City = si
                            highest_value   = _ai_all_own_city_values[itr_cities];
                            target_wx = city_area_wx;                 // asm  Target_X = di
                            target_wy = city_area_wy;                 // asm  Target_Y = Tile_Y
                            target_wp = city_wp;                      // asm  Target_Plane = Plane
                        }
                    }
                }
            }
        }
    }
}

if(target_city_idx == ST_UNDEFINED)  return ST_FALSE;                                            // asm  cmp Target_City,-1 / xor ax,ax
*targeted_wx = target_wx; *targeted_wy = target_wy; *targeted_wp = target_wp;  return ST_TRUE;   // asm  store Target_* / ax = 1
```

**Faithful 1:1** — loop bounds, the `WORLD_YMIN` lower bound, E/W wrap, the desert/swamp predicate pair (`Square_Is_Desert` OR `Square_Is_Swamp`, each compared `== ST_TRUE`), the strict-greater value compare, the record order, and both returns all match the asm. The asm pushes `(Plane, Tile_Y, di)` for each predicate — i.e. `Square_Is_*(di, Tile_Y, Plane)` = `Square_Is_*(wx, wy, wp)` — matching the production argument order.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_city_values[]` | `AI_Own_City_Values` | `extern int16_t *` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)); word-indexed (`si*2`). |
| `Square_Is_Desert` / `Square_Is_Swamp` | `j_Square_Is_Desert` / `j_Square_Is_Swamp` | `(wx, wy, wp)` ([Terrain.h:173](../../MoM/src/Terrain.h#L173), [179](../../MoM/src/Terrain.h#L179)); compared `== ST_TRUE`. |
| `WORLD_YMIN` | `e_WORLD_YMIN` | `0` ([MOM_DEF.h:320](../../MoX/src/MOM_DEF.h#L320)). Note the asm spells the Y-min as a named constant here, vs the literal `>= 0` (`or di,di`) in `AITP_Transmute`. |
| `_CITIES[i]` stride | `imul size s_CITY` | `sizeof(s_CITY) = 114`. |

## B1 (OGQUIRK) — 5×5 catchment includes corners

Both the `wy_offset` and `wx_offset` loops run `-2..2`, so the scan covers the full **5×5 = 25-tile square** around the city. The real MoM city catchment is the 5×5 area **minus the four corners** (21 tiles). drake178 flags this directly (*"BUG: treats catchment corners as affecting the city"*), and the production header repeats it (`OGBUG treats catchment corners as affecting the city, should skip corners`). Faithfully preserved — do not "fix" by skipping corners. (The sibling [`AITP_Transmute`](AISPELL-AITP_Transmute.md) carries the identical quirk.)

## OG quirks (preserve when reconstructing)

- **B1** — 5×5 catchment scan includes the four corner tiles.
- **Tie-break**: the `> highest_value` test is strictly-greater, so among equal-value cities the **lowest city index** wins; within a city, the **last** matching tile in `(wy,wx)` scan order is the one returned (each match for the new best city overwrites `target_*`).
- **Desert/swamp-only is by design** — not a bug (see Purpose).

## Sub-functions / external calls

- **`Square_Is_Desert(wx, wy, wp)`** / **`Square_Is_Swamp(wx, wy, wp)`** — terrain-type predicates ([Terrain.c:1387](../../MoM/src/Terrain.c#L1387), [1455](../../MoM/src/Terrain.c#L1455)).
- Otherwise reads `_CITIES[]`, `_ai_all_own_city_values[]`, and `_cities`, and writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

## Related references

- `…\ovr156\AITP_ChangeTerrain__WIP.asm` — IDA Pro 5.5 disassembly (the authority; the OG filename keeps the `__WIP` tag).
- `…\ovr156\AITP_ChangeTerrain__WIP.c` — Piethawn IDA-C.
- [AISPELL-AITP_Transmute.md](AISPELL-AITP_Transmute.md) — sibling `AITP_*` catchment-scan picker (mineral specials instead of terrain type); shares B1.
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Change Terrain.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
