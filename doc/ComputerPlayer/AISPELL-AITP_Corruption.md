AISPELL-AITP_Corruption.md

SEEALSO:  AISPELL-AITP_Attack_Terrain.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\sub_E9FA9.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\sub_E9FA9.c

; OG: reached via the far-call thunk `sub_3667F` ( `jmp sub_E9FA9`, WIZARDS.asm:79800 ).
; Production: NO caller yet — defined + prototyped but unwired.

(no production caller)
    |-> AITP_Corruption(player_idx, &wx, &wy, &wp)

---

# `AITP_Corruption` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Corruption` | [AISPELL.c:5914-6064](../../MoM/src/AISPELL.c#L5914-L6064) | AI terrain-square picker. Scans for the highest-value **contacted, non-neutral, non-own** enemy city, then makes up to 25 random probes of that city's catchment for a square that is **land, not volcano, not corrupted, not hills, not mountain, not node** — and returns that square. Returns `ST_TRUE` with `*targeted_wx/_wy/_wp`, `ST_FALSE` otherwise. A near-twin of [`AITP_Attack_Terrain`](AISPELL-AITP_Attack_Terrain.md). |

> **Status: BUILDS CLEAN — faithful to the asm (1:1), carrying one preserved OGBUG ([B1](#b1-ogbug--returns-the-last-citys-plane)).** The body ([AISPELL.c:5914](../../MoM/src/AISPELL.c#L5914)) is a faithful reconstruction of `sub_E9FA9.asm` (drake178/IDA name `sub_E9FA9`; production renamed → `AITP_Corruption`; the asm/`.c` filename keeps the OG name). It compiles and links as part of AISPELL.c / momlib. The control flow, city gates, 25-try catchment probe, terrain filters, and return all match the asm. The asm is the authority.

## Purpose

An AI "attack the terrain near an enemy city" target picker for **Corruption** — structurally the same as `AITP_Attack_Terrain` (`AITP_HarmTerrain`), but with **no `spell_idx` argument** and a fixed, stricter terrain filter: it always rejects hills, mountains, and nodes (and, unlike `AITP_HarmTerrain`, never checks rivers). The net effect is "find a clear, **flat** land tile in a strong enemy city's catchment" — a sensible target for corrupting terrain.

Note the overlap: in the live overland dispatch, `Get_Map_Square_Target_For_Spell`'s `spl_Corruption` case routes to `AITP_Attack_Terrain`, not here ([AISPELL-Get_Map_Square_Target_For_Spell.md](AISPELL-Get_Map_Square_Target_For_Spell.md)). This function is the OG's separate, stricter Corruption picker, reached through its own thunk and currently **unwired** in the reconstruction.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| *(none in production)* | — | `AITP_Corruption` is defined ([AISPELL.c:5914](../../MoM/src/AISPELL.c#L5914)) and prototyped ([AISPELL.h:264](../../MoM/src/AISPELL.h#L264)) but **not called anywhere** in the reconstructed code. |
| OG thunk `sub_3667F` | `WIZARDS.asm:79800` | the OG far-call thunk (`proc sub_3667F far { jmp sub_E9FA9 }`), in the overlay thunk table next to `j_AITP_HarmTerrain`. No symbolic `call sub_3667F` was found in `WIZARDS.asm`, so the dispatching caller is not yet identified. |

Because it has its own far-call thunk it was a real callable function in `WIZARDS.EXE`; in the reconstruction it is currently an **orphan** (no dispatch wired up).

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `sub_E9FA9.asm`.

```c
best_city_idx = ST_UNDEFINED;  best_city_value = 0;                     // asm  var_4 = -1, var_2 = 0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)                 // asm  si loop
{
    if(_CITIES[itr].owner_idx == player_idx)            continue;       // 5937  owner (+12h) == player → skip
    if(_CITIES[itr].owner_idx == NEUTRAL_PLAYER_IDX)    continue;       // 5942  owner == 5 → skip
    if(Test_Bit_Field(player_idx, &_CITIES[itr].contacts) == 0) continue;  // 5947  must be contacted
    if(_ai_all_enemy_city_values[itr] <= best_city_value) continue;     // 5952  asm jg (value > best)

    city_wp = _CITIES[itr].wp;  square_ctr = 0;  found_target = 0;      // 5957-5959
    while(square_ctr < 25 && found_target == 0)                         // 5961  ≤25 tries
    {
        wx_offset = Random(5) - 3;  wy_offset = Random(5) - 3;          // 5963-5964  → -2..+2 (Random is 1..5)
        var_18 = (one of the four ±2 corners) ? ST_TRUE : ST_FALSE;     // reject corners
        city_area_wx = _CITIES[itr].wx + wx_offset;
        city_area_wy = _CITIES[itr].wy + wy_offset;
        if(var_18 != ST_FALSE)                          { square_ctr++; continue; }
        if(city_area_wx <= 0 || city_area_wx >= WORLD_WIDTH)  { square_ctr++; continue; }   // asm  >0 && <60
        if(city_area_wy <= 0 || city_area_wy >= WORLD_HEIGHT) { square_ctr++; continue; }   // asm  >0 && <40
        if(Square_Is_Land(...)        != ST_TRUE)        { square_ctr++; continue; }   // 6005  asm j_Map_Square_Is_Land
        if(Square_Is_Volcano(...)     != ST_FALSE)       { square_ctr++; continue; }   // 6011
        if(Square_Has_Corruption(...) != ST_FALSE)       { square_ctr++; continue; }   // 6017  asm j_Square_Has_Corrruption
        if(Square_Is_Hills(...)       != ST_FALSE)       { square_ctr++; continue; }   // 6023
        if(Square_Is_Mountain(...)    != ST_FALSE)       { square_ctr++; continue; }   // 6029
        if(Square_Is_Node(...)        != ST_FALSE)       { square_ctr++; continue; }   // 6035  asm j_TILE_IsNode

        best_city_idx = itr;  best_city_value = …value;                 // 6041-6042
        target_wx = city_area_wx;  target_wy = city_area_wy;  target_wp = city_wp;  // 6043-6045  target_wp dead [B1]
        found_target = ST_TRUE;  square_ctr++;                          // 6046-6048
    }
}

if(best_city_idx == ST_UNDEFINED) return ST_FALSE;                      // 6052-6054  asm cmp var_4,-1 / xor ax,ax
*targeted_wx = target_wx; *targeted_wy = target_wy; *targeted_wp = city_wp; return 1;   // 6058-6061  [B1] city_wp, not target_wp
```

**Faithful 1:1** — the four city gates (own / neutral / contacted / value), the 25-try probe, the corner rejection, the `> 0` / `< WORLD_WIDTH`/`WORLD_HEIGHT` bounds, the six terrain filters, and the return all match the asm.

### Catchment offset is symmetric — no asymmetry/corner bug

`Random(5)` returns **1..5** ([random.c:296](../../MoX/src/random.c#L296)), so `Random(5) - 3` is **-2..+2** — a symmetric 5×5 grid. All four `±2` corners are reachable and are explicitly rejected, leaving the standard 21-tile city catchment. There is **no** "offset skewed to −3..+1" and **no** "only −2 corners reachable" effect.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_CITIES[i]` | `imul 72h` | stride `0x72`; `owner_idx +12h`, `wx +0Fh`, `wy +10h`, `wp +11h`, `contacts +1Ah`. |
| `_ai_all_enemy_city_values[]` | `AI_Enemy_City_Values` | `int16_t *` ([MOM_DAT.h:3037](../../MoX/src/MOM_DAT.h#L3037)); word-indexed. |
| `NEUTRAL_PLAYER_IDX` | `5` | the skipped owner ([MOM_DEF.h:393](../../MoX/src/MOM_DEF.h#L393)). |
| bounds `WORLD_WIDTH` / `WORLD_HEIGHT` | `60` / `40` | upper exclusive (asm `cmp …,3Ch/28h / jl`); lower exclusive `> 0`. |
| try cap | `cmp var_C,19h` | 25. |
| `Random(5)` | `call Random; add ax,0FFFDh` | `+(-3)`; `Random ∈ 1..5`. |
| terrain predicates | `j_Map_Square_Is_Land` / `j_Square_Has_Corrruption` / `j_TILE_IsNode` | production `Square_Is_Land` / `Square_Has_Corruption` / `Square_Is_Node` ([Terrain.h](../../MoM/src/Terrain.h)). |

## B1 (OGBUG) — returns the *last city's* plane

On success the asm writes `*arg_6 = Plane` (the wp of the **last contacted enemy city iterated**), **not** `var_14` (the target square's plane, which it sets but never reads). Production mirrors this: it sets `target_wp = city_wp` ([6045](../../MoM/src/AISPELL.c#L6045)) but the return uses `*targeted_wp = city_wp` ([6060](../../MoM/src/AISPELL.c#L6060), tagged `OGBUG`), leaving `target_wp` dead. If a higher-value enemy city is iterated *after* the target locks in but finds no square, `city_wp` ends on that later city's plane — so the returned `(wx, wy)` can pair with the wrong `wp`. (Same bug as `AITP_Attack_Terrain`'s B1.) Faithfully preserved.

## Sub-functions / external calls

- **`Random(5)`**, **`Test_Bit_Field(player_idx, &city.contacts)`**.
- Terrain predicates: `Square_Is_Land`, `Square_Is_Volcano`, `Square_Has_Corruption`, `Square_Is_Hills`, `Square_Is_Mountain`, `Square_Is_Node` ([Terrain.h](../../MoM/src/Terrain.h)).
- Reads `_CITIES[]`, `_ai_all_enemy_city_values[]`, `_cities`; writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

## Related references

- `…\ovr156\sub_E9FA9.asm` — IDA Pro 5.5 disassembly (the authority; OG name `sub_E9FA9`, reached via the `sub_3667F` thunk).
- `…\ovr156\sub_E9FA9.c` — Piethawn IDA-C.
- [AISPELL-AITP_Attack_Terrain.md](AISPELL-AITP_Attack_Terrain.md) — the spell-gated twin (`AITP_HarmTerrain`, Corruption / Raise Volcano via `Get_Map_Square_Target_For_Spell`); shares the catchment-probe shape and the B1 plane bug.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent overland spell-AI dispatcher.
