
AISPELL-AITP_Transmute.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_Transmute.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_Transmute.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Transmute()

Cast_Spell_Overland()
    // map-square target lookup at cast time
    |-> Get_Map_Square_Target_For_Spell()
        switch(spell_idx)
            case spl_Transmute:
                |-> AITP_Transmute()

---

# `AITP_Transmute` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Transmute` | [AISPELL.c:2767-2841](../../MoM/src/AISPELL.c#L2767-L2841) | AI target picker for **Transmute**: of the caster's own cities, find the **highest AI-valued** one that has a coal/iron/silver-class mineral special in its 5×5 catchment, and return the map square (`*targeted_wx`,`*targeted_wy`,`*targeted_wp`) of the qualifying tile. Returns `ST_TRUE` if a target was found, `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:2767](../../MoM/src/AISPELL.c#L2767)) is a direct, faithful reconstruction of `AITP_Transmute.asm` (the earlier `return 0;` stub is gone). It compiles and links as part of AISPELL.c / momlib. Two original-game quirks are preserved verbatim ([B1](#b1-ogquirk--55-catchment-includes-corners), [B2](#b2-ogquirk--bitwise-terrain-special-test)). The asm is the authority.

## Purpose

The `AITP_*` target picker for **Transmute**, which converts a mineral resource on a map square. The AI scans each of its own cities' catchments for a transmutable mineral and aims at the one belonging to its most valuable city. "Value" is the precomputed AI city-valuation table `_ai_all_own_city_values[]` (asm `AI_Own_City_Values`), keyed by city index. Unlike the city-enchantment pickers, this returns a **map square** (x, y, plane), not a city index.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2326](../../MoM/src/AISPELL.c#L2326) | **Probe**: if a target exists, Transmute is given a fixed priority weight (`AI_OVL_SplPriorities[2] = 200`). |
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:3962-3965](../../MoM/src/AISPELL.c#L3962-L3965) | Cast-time lookup (`switch(spell_idx) case spl_Transmute`) — fills the caller's `wx`/`wy`/`wp` for the actual cast. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_Transmute.asm`. World dims: `WORLD_WIDTH=60`, `WORLD_HEIGHT=40` ([MOM_DEF.h:271-272](../../MoX/src/MOM_DEF.h#L271-L272)).

```c
city_idx = ST_UNDEFINED;                                              // asm  City_Index = -1
highest_value = 0;                                                    // asm  Highest_Value = 0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)              // asm  cx loop
{
    if(_CITIES[itr_cities].owner_idx == player_idx)                  // asm  owner_idx (cbw) == Player_Index
    {
        city_wp = _CITIES[itr_cities].wp;                            // asm  City_Plane = wp (cbw)

        for(wy_offset = -2; wy_offset < 3; wy_offset++)              // asm  Y_Modifier -2..2
        {
            city_area_wy = _CITIES[itr_cities].wy + wy_offset;       // asm  di = wy + Y_Modifier
            if(city_area_wy >= 0 && city_area_wy < WORLD_HEIGHT)     // asm  di>=0 && di<40   (no N/S wrap)
            {
                for(wx_offset = -2; wx_offset < 3; wx_offset++)      // asm  X_Modifier -2..2
                {
                    city_area_wx = _CITIES[itr_cities].wx + wx_offset;           // asm  si = wx + X_Modifier
                    if(city_area_wx < 0)            city_area_wx += WORLD_WIDTH;  // asm  si<0:  si+=60  (E/W wrap)
                    if(city_area_wx >= WORLD_WIDTH) city_area_wx -= WORLD_WIDTH;  // asm  si>=60: si-=60

                    if( (GET_TERRAIN_SPECIAL(city_area_wx, city_area_wy, city_wp) & TS_COAL)   != 0   // asm  test ...,TS_COAL  / jnz
                     || (GET_TERRAIN_SPECIAL(city_area_wx, city_area_wy, city_wp) & TS_IRON)   != 0   // asm  test ...,TS_IRON  / jnz
                     || (GET_TERRAIN_SPECIAL(city_area_wx, city_area_wy, city_wp) & TS_SILVER) != 0 ) // asm  test ...,TS_SILVER/ jz skip
                    {
                        if(_ai_all_own_city_values[itr_cities] > highest_value)  // asm  jle skip  =>  strictly greater
                        {
                            city_idx      = itr_cities;               // asm  City_Index = cx
                            highest_value = _ai_all_own_city_values[itr_cities];
                            target_wx = city_area_wx;                 // asm  Tile_X = si
                            target_wy = city_area_wy;                 // asm  Tile_Y = di
                            target_wp = city_wp;                      // asm  Tile_Plane = City_Plane
                        }
                    }
                }
            }
        }
    }
}

if(city_idx == ST_UNDEFINED)  return ST_FALSE;                                       // asm  cmp City_Index,-1 / xor ax,ax
*targeted_wx = target_wx; *targeted_wy = target_wy; *targeted_wp = target_wp;  return ST_TRUE;   // asm  store Tile_* / ax = 1
```

**Faithful 1:1** — the loop bounds, the wrap handling, the terrain-offset arithmetic, the mineral test sense (qualify when a bit is *set*), the strict-greater value compare, the record order, and both returns all match the asm. The catchment tile is read through the `GET_TERRAIN_SPECIAL(wx, wy, wp)` macro, which expands to `_map_square_terrain_specials[wp*WORLD_SIZE + wy*WORLD_WIDTH + wx]` — the same address the asm builds with `imul 0960h` / `imul 3Ch`.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_city_values[]` | `AI_Own_City_Values` | `extern int16_t *` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)); word-indexed (`cx*2`). |
| `GET_TERRAIN_SPECIAL(x,y,p)` | `wp*0960h + wy*3Ch + wx` into `_map_square_terrain_specials` | macro ([MOX_DEF.h:464](../../MoX/src/MOX_DEF.h#L464)); `WORLD_SIZE = 40*60 = 2400 = 0x960`, `WORLD_WIDTH = 60 = 0x3C`. Array is `extern uint8_t *` ([MOM_DAT.h:4079](../../MoX/src/MOM_DAT.h#L4079)). |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `TS_COAL / TS_IRON / TS_SILVER` | same | `2 / 1 / 3` ([Terrain.h:49-51](../../MoM/src/Terrain.h#L49-L51)). |

## B1 (OGQUIRK) — 5×5 catchment includes corners

Both the `wy_offset` and `wx_offset` loops run `-2..2`, so the scan covers the full **5×5 = 25-tile square** around the city. The real MoM city catchment is the 5×5 area **minus the four corners** (21 tiles). drake178 flags exactly this on the sibling picker (`AITP_ChangeTerrain`: *"BUG: treats catchment corners as affecting the city"*); the same square-scan applies here. Faithfully preserved — do not "fix" by skipping corners.

## B2 (OGQUIRK) — bitwise terrain-special test

The terrain-special byte holds a single **enum value** per tile (`TS_NONE=0, TS_IRON=1, TS_COAL=2, TS_SILVER=3, TS_GOLD=4, TS_GEMS=5, TS_MITHRIL=6, TS_ADAMANTIUM=7, …`), but the asm tests it with `test` (bitwise AND) and the C uses `& … != 0`. The three OR'd tests reduce to **"low two bits set"** (`value & 3 != 0`) — the `TS_SILVER` (3) test is fully redundant with `TS_COAL` (2) `|` `TS_IRON` (1).

So the match set is **not** just coal/iron/silver; it is every special whose value ANDs nonzero with `3`:

| Matches (`value & 3 != 0`) | Does **not** match |
|---|---|
| IRON (1), COAL (2), SILVER (3), GEMS (5), MITHRIL (6), ADAMANTIUM (7), CRYSX (9) | GOLD (4), QUORK (8), HUNTERS_LODGE (0x10), WILD_GAME (0x40), NIGHTSHADE (0x80) |

This is the original behavior (bitwise test over enum-valued data); faithfully preserved. Reconstruction note: switching to value-equality (`== TS_COAL`, etc.) would change which cities the AI considers — don't.

## OG quirks (preserve when reconstructing)

- **B1** — 5×5 catchment scan includes the four corner tiles.
- **B2** — bitwise terrain-special test broadens the match beyond coal/iron/silver and makes the `TS_SILVER` test redundant.
- **Tie-break**: the `> highest_value` test is strictly-greater, so among equal-value cities the **lowest city index** wins; within a city, the **last** matching tile in `(wy,wx)` scan order is the one returned (each match for the new best city overwrites `target_*`).

## Sub-functions / external calls

- **`GET_TERRAIN_SPECIAL(wx, wy, wp)`** — macro read of `_map_square_terrain_specials[]` ([MOX_DEF.h:464](../../MoX/src/MOX_DEF.h#L464)).
- Otherwise reads `_CITIES[]`, `_ai_all_own_city_values[]`, and `_cities`, and writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

## Related references

- `…\ovr156\AITP_Transmute.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_Transmute.c` — Piethawn IDA-C.
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Transmute.
- [AISPELL-AITP_Wall_Of_Stone.md](AISPELL-AITP_Wall_Of_Stone.md) — sibling `AITP_*` city target picker (simpler, no catchment scan).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
