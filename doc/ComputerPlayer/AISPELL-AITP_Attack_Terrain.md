AISPELL-AITP_Attack_Terrain.md

SEEALSO:  AISPELL-AITP_Attack_Terrain.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_HarmTerrain.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_HarmTerrain.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                |-> Get_Map_Square_Target_For_Spell()
                    switch(spell_idx)
                        case spl_Corruption:
                        case spl_Raise_Volcano:
                            |-> AITP_Attack_Terrain()

---

# `AITP_Attack_Terrain` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Attack_Terrain` | [AISPELL.c:5477-5614](../../MoM/src/AISPELL.c#L5477-L5614) | AI target picker for **Corruption** and **Raise Volcano**: find a contacted enemy (non-neutral) city of highest value, then randomly probe its catchment for a valid land square to attack, and return that square. Returns `ST_TRUE` with `*targeted_wx/_wy/_wp`, `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1), carrying one preserved OG bug ([B1](#b1-ogbug--returns-the-last-citys-plane)).** The body ([AISPELL.c:5477](../../MoM/src/AISPELL.c#L5477)) is a faithful reconstruction of `AITP_HarmTerrain.asm` (production renamed `AITP_HarmTerrain` → `AITP_Attack_Terrain`; the asm/`.c` filename keeps the OG `AITP_HarmTerrain`). It compiles and links as part of AISPELL.c / momlib, and the cast-time dispatch ([AISPELL.c:5041](../../MoM/src/AISPELL.c#L5041)) is live. The control flow, city gates, random probe, terrain filters, and return all match. The asm is the authority.

## Purpose

`Get_Map_Square_Target_For_Spell` routes **Corruption** and **Raise Volcano** here. The AI wants to wreck the terrain around a strong enemy city it has met: it scans for the highest-value **contacted, non-neutral, non-own** city, then makes up to 25 random probes of that city's catchment for a square that is land, not already a volcano, and not already corrupted (plus, for non-Corruption spells, not mountain/hills/river/node). The `spell_idx` argument selects whether those extra terrain-type filters apply.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:5041](../../MoM/src/AISPELL.c#L5041) | `switch(spell_idx)` cases `spl_Corruption` / `spl_Raise_Volcano` → here (active). |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_HarmTerrain.asm`.

```c
target_city_idx = ST_UNDEFINED;  highest_value = 0;                     // asm  Target_City=-1, Highest_Value=0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)                 // asm  si loop
{
    if(_CITIES[itr].owner_idx == player_idx)          continue;         // asm  owner==Player_Index → skip
    if(_CITIES[itr].owner_idx == NEUTRAL_PLAYER_IDX)  continue;         // asm  owner==5 (neutral)  → skip
    if(_ai_all_enemy_city_values[itr] <= highest_value) continue;       // asm  jg  (value > highest)
    if(Test_Bit_Field(player_idx, &_CITIES[itr].contacts) == 0) continue;  // asm  must be contacted

    city_wp = _CITIES[itr].wp;
    for(tries = 0; tries < 25 && found_target == 0; tries++)            // asm  ≤25 tries
    {
        wx_offset = Random(5) - 3;  wy_offset = Random(5) - 3;          // asm  Random(5)+(-3) → -2..+2 (Random is 1..5)
        reject = (one of the four ±2 diagonal corners) ? ST_TRUE : ST_FALSE;
        if(reject != ST_FALSE) continue;

        city_area_wx = _CITIES[itr].wx + wx_offset;                     // asm  di
        city_area_wy = _CITIES[itr].wy + wy_offset;                     // asm  Tile_Y
        if(city_area_wx <= WORLD_XMIN || city_area_wx >= WORLD_WIDTH)  continue;   // asm  di>0 && di<60
        if(city_area_wy <= WORLD_YMIN || city_area_wy >= WORLD_HEIGHT) continue;   // asm  Tile_Y>0 && <40

        if(Square_Is_Land(...)        != ST_TRUE)  continue;            // asm  j_Map_Square_Is_Land
        if(Square_Is_Volcano(...)     != ST_FALSE) continue;
        if(Square_Has_Corruption(...) != ST_FALSE) continue;
        if(spell_idx != spl_Corruption)                                // asm  cmp Spell_Index, spl_Corruption
        {
            if(Square_Is_Mountain(...) != ST_FALSE) continue;
            if(Square_Is_Hills(...)    != ST_FALSE) continue;
            if(Square_Is_River(...)    != ST_FALSE) continue;
            if(Square_Is_Node(...)     != ST_FALSE) continue;          // asm  j_TILE_IsNode
        }

        target_city_idx = itr; highest_value = …value;
        target_wx = city_area_wx; target_wy = city_area_wy; target_wp = city_wp;  // target_wp dead [B1]
        found_target = ST_TRUE;
    }
}

if(target_city_idx == ST_UNDEFINED) return ST_FALSE;                   // asm  cmp Target_City,-1 / xor ax,ax
*targeted_wx = target_wx; *targeted_wy = target_wy; *targeted_wp = city_wp; return ST_TRUE;   // [B1] city_wp, not target_wp
```

**Faithful 1:1** — the four city gates (own / neutral / value / contacted), the 25-try random probe, the corner rejection, the X/Y bounds (`> 0` and `< WORLD_WIDTH`/`WORLD_HEIGHT`), the land/volcano/corruption filters, the Corruption-only skip of the mountain/hills/river/node filters, and the return all match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_enemy_city_values[]` | `AI_Enemy_City_Values` | `int16_t *` ([MOM_DAT.h:3037](../../MoX/src/MOM_DAT.h#L3037)). |
| `NEUTRAL_PLAYER_IDX` | `5` | the skipped owner ([MOM_DEF.h:393](../../MoX/src/MOM_DEF.h#L393)). |
| bounds `WORLD_WIDTH` / `WORLD_HEIGHT` | `60` / `40` | upper exclusive (asm `cmp …,60/40 / jl`); `WORLD_XMIN`/`WORLD_YMIN = 0` lower exclusive. |
| `Square_Is_Land / _Volcano / Has_Corruption / _Mountain / _Hills / _River / _Node` | `j_Map_Square_Is_Land / …Volcano / …Corrruption / …Mountain / …Hills / …River / j_TILE_IsNode` | production names per [Terrain.h](../../MoM/src/Terrain.h). |
| `Test_Bit_Field(player_idx, &city.contacts)` | `Test_Bit_Field` | contact-bitfield membership test. |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 114`. |

## B1 (OGBUG) — returns the *last city's* plane

On success the asm writes `RetP = Plane` (the wp of the **last contacted enemy city iterated**), **not** `Target_Plane` (the target square's plane, which it sets but never reads). Production mirrors this: it sets `target_wp = city_wp` but the return uses `*targeted_wp = city_wp` ([5611](../../MoM/src/AISPELL.c#L5611)), leaving `target_wp` dead. If a higher-value enemy city is processed *after* the target locks in but finds no square, `city_wp` ends on that later city's plane — so the returned `(wx, wy)` can pair with the wrong `wp`. Faithfully preserved.

## OG quirks (preserve when reconstructing)

- **B1** — returns the last-iterated contacted city's plane (`city_wp`), not the target square's plane.
- **Catchment offset is symmetric (not a bug).** `Random(5)` returns **1..5** ([random.c:296](../../MoX/src/random.c#L296)), so `Random(5) - 3` is **-2..+2** — the standard 5×5 grid; all four `±2` corners are reachable and explicitly rejected, leaving the 21-tile catchment. (An earlier revision wrongly flagged this as an asymmetric-offset bug, assuming `Random` was 0-based.)

## Sub-functions / external calls

- **`Random(5)`**, **`Test_Bit_Field(player_idx, &city.contacts)`**.
- Terrain predicates: `Square_Is_Land`, `Square_Is_Volcano`, `Square_Has_Corruption`, `Square_Is_Mountain`, `Square_Is_Hills`, `Square_Is_River`, `Square_Is_Node` ([Terrain.h](../../MoM/src/Terrain.h)).
- Reads `_CITIES[]`, `_ai_all_enemy_city_values[]`, `_cities`; writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

## Related references

- `…\ovr156\AITP_HarmTerrain.asm` — IDA Pro 5.5 disassembly (the authority; OG name `AITP_HarmTerrain`).
- `…\ovr156\AITP_HarmTerrain.c` — Piethawn IDA-C.
- `Get_Map_Square_Target_For_Spell` ([AISPELL.c:5002](../../MoM/src/AISPELL.c#L5002)) — the map-square router (Corruption / Raise Volcano case).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
