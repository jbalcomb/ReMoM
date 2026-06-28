AISPELL-AITP_Floating_Island.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\Pick_Target_For_Floating_Island.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\Pick_Target_For_Floating_Island.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                |-> Get_Map_Square_Target_For_Spell()
                    switch(spell_idx)
                        case spl_Floating_Island:
                            |-> AITP_Floating_Island()

---

# `AITP_Floating_Island` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Floating_Island` | [AISPELL.c:5178-5288](../../MoM/src/AISPELL.c#L5178-L5288) | AI target picker for **Floating Island**: pick a random owned city, then scan outward (up to 10 squares) in the four cardinal directions for the first square legal for a Floating Island, and return it. Returns `ST_TRUE` with `*targeted_wx/_wy/_wp` set, `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:5178](../../MoM/src/AISPELL.c#L5178)) is a faithful reconstruction of `Pick_Target_For_Floating_Island.asm` (production renamed `Pick_Target_For_Floating_Island` → `AITP_Floating_Island`; drake178 `AITP_FloatingIsland`; the asm/`.c` filename keeps the OG `Pick_Target_For_Floating_Island`). It compiles and links as part of AISPELL.c / momlib. The random-city pick, the four-direction scan, and the state transitions all match. One preserved OG quirk ([B1](#og-quirks-preserve-when-reconstructing)). The asm is the authority.

## Purpose

`Get_Map_Square_Target_For_Spell` routes **Floating Island** here ([case spl_Floating_Island](../../MoM/src/AISPELL.c#L5027)). It looks for a legal square near one of the AI's cities to spawn the island: it picks a **random** owned city, then probes the four straight cardinal rays (≤10 squares) and returns the first legal square found.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:5027](../../MoM/src/AISPELL.c#L5027) | `switch(spell_idx) case spl_Floating_Island` → here. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `Pick_Target_For_Floating_Island.asm`. The scan direction is chosen by two state flags — **`is_vertical_axis`** (FALSE = x/E-W, TRUE = y/N-S) and **`is_negative_offset`** (FALSE = `+itr` → East/South, TRUE = `−itr` → West/North):

| `is_negative_offset` | `is_vertical_axis` | offset | direction |
|---|---|---|---|
| `ST_FALSE` | `ST_FALSE` | `city_wx + itr` | EAST |
| `ST_FALSE` | `ST_TRUE` | `city_wy + itr` | SOUTH |
| `ST_TRUE` | `ST_FALSE` | `city_wx − itr` | WEST |
| `ST_TRUE` | `ST_TRUE` | `city_wy − itr` | NORTH |

```c
/* Phase 1 — up to 200 tries to land on a random OWNED city */
found_target = ST_FALSE;
for(tries = 0; tries < 200 && found_target == ST_FALSE; tries++)       // asm  Tries<200 && check_value==0
{
    city_idx = Random(_cities) - 1;                                    // asm  Random(_cities); dec
    if(_CITIES[city_idx].owner_idx == player_idx)                      // asm  owner_idx (cbw) == plyer_idx
    {
        found_target = ST_TRUE;
        city_wp = _CITIES[city_idx].wp;  city_wx = …wx;  city_wy = …wy;
    }
}
if(found_target == ST_FALSE) return ST_FALSE;                          // asm  no owned city → ax=0

/* Phase 2 — scan the four cardinals (≤10 squares) for a legal square */
itr_squares = 0;  is_negative_offset = ST_TRUE;  is_vertical_axis = ST_FALSE;  found_target = ST_FALSE;
while(found_target == ST_FALSE && itr_squares < 10)
{
    if(is_negative_offset == ST_FALSE)                /* EAST / SOUTH (down-right) */
        if(is_vertical_axis == ST_FALSE) { test(city_wx + itr, city_wy); is_vertical_axis = ST_TRUE; }                      // EAST
        else                             { test(city_wx, city_wy + itr); is_vertical_axis = ST_FALSE; is_negative_offset = ST_TRUE; }  // SOUTH
    else /* is_negative_offset == ST_TRUE */          /* WEST / NORTH (up-left) */
        if(is_vertical_axis == ST_FALSE) { test(city_wx - itr, city_wy); is_vertical_axis = ST_TRUE; }                      // WEST
        else                             { test(city_wx, city_wy - itr); is_vertical_axis = ST_FALSE; itr_squares++; is_negative_offset = ST_FALSE; }  // NORTH
    // "test(x,y)" = if Square_Is_Legal_For_Floating_Island(x, y, city_wp) == ST_TRUE
    //                  { found_target = ST_TRUE; target_wx = x; target_wy = y; }
}

if(found_target == ST_FALSE) return ST_FALSE;                         // asm  cmp check_value,0 / xor ax,ax
*targeted_wx = target_wx; *targeted_wy = target_wy; *targeted_wp = city_wp; return ST_TRUE;   // asm store / ax=1
```

**Faithful 1:1** — the 200-try random-city loop, the `found_target`/`itr_squares` while guard, the four direction tests with their exact `city_wx±itr` / `city_wy±itr` arguments, the per-branch state transitions (the south branch ends `is_vertical_axis=ST_FALSE; is_negative_offset=ST_TRUE`, matching asm `loc_E9755`), `itr_squares++` only in the north branch, and the dual-output return all match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `Square_Is_Legal_For_Floating_Island(wx,wy,wp)` | `Map_Square_Is_Legal_For_Floating_Island` | production dropped the `Map_` prefix ([AISPELL.h:283](../../MoM/src/AISPELL.h#L283)); same function. |
| `Random(_cities)` | `call Random` (arg `_cities`) | `city_idx = Random(_cities) - 1`. |
| `is_vertical_axis` / `is_negative_offset` | `Vertical` / `UpLeft` | renamed locals; same `ST_TRUE`/`ST_FALSE` state machine. |
| `found_target` | `check_value` (di) | renamed local. |
| `niu_player_idx` | `uu_player_idx` | assigned from `player_idx`, never read — a dead local, preserved from the asm. |
| `_CITIES[i]` stride | `imul size s_CITY` | `sizeof(s_CITY) = 114`. |

## OG quirks (preserve when reconstructing)

- **B1 — "utterly useless targeting"** (drake178): the picker chooses a **random** owned city (not the highest-value) and only probes the four straight cardinal rays out to 10 squares, taking the first legal square. Poor placement, but it's the original behavior — keep it.
- **Dead local**: `niu_player_idx` (asm `uu_player_idx`) is set from `player_idx` and never used — faithfully preserved.

## Sub-functions / external calls

- **`Square_Is_Legal_For_Floating_Island(wx, wy, wp)`** — legality test for a Floating Island square (asm `Map_Square_Is_Legal_For_Floating_Island`).
- **`Random(n)`** — game RNG, for the random city pick.
- Reads `_CITIES[]`, `_cities`; writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

## Related references

- `…\ovr156\Pick_Target_For_Floating_Island.asm` — IDA Pro 5.5 disassembly (the authority; OG name `Pick_Target_For_Floating_Island`).
- `…\ovr156\Pick_Target_For_Floating_Island.c` — Piethawn IDA-C.
- `…\ovr156\Map_Square_Is_Legal_For_Floating_Island.asm` — the per-square legality helper.
- `Get_Map_Square_Target_For_Spell` ([AISPELL.c:5002](../../MoM/src/AISPELL.c#L5002)) — the map-square target router that calls this for Floating Island.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
