MoX-MapGen.md
SEEALSO: MoX-Landmasses.md
SEEALSO: MoX-WorldMap.md



MapGen uses 1000 as temporary terrain type



https://masterofmagic.fandom.com/wiki/Node#_Known_Bugs
Drake178
6/13/2017
I read about the sorcery node count check in the strategy guide but frankly I'm not sure it was correct with the numbers. It also said that neighbouring terrain affects the node type. I'll try to verify these to whatever extent I can.
Magic Intensity is taken directly from the ingame tooltip.
Page 183  (PDF Page 192)
// drake178: NEWG_SetNodeType()
; before v1.1, this function used to consider the types
; of terrain in the node's aura, but now the calculated
; bias values are simply re-randomized instead



NEWG_SetBaseLands__WIP()  ==>  Translate_Heightmap_To_Base_Terrain_Types() += Add_Tundra()
NEWG_SetSpecLands__WIP()  ==>  ¿ ?



Init_Landmasses()




## Translate Drunkard's Walk Hight-Map to Terrain Types
Translate_Heightmap_To_Base_Terrain_Types() += Add_Tundra()
Ocean, Grasslands, Forest, Hills, Mountain
-INF - 0.00  Ocean
0.00 - 0.75  Grasslands
0.75 - 3.00  Forest
4.00 - 5.00  Hills
6.00 -  INF  Mountain

## then, ...
NEWG_SetSpecLands__WIP()
Swamp, Desert
NOT Terrain Specials





document the process that populates the map squares in @_world_maps with the terrain types
@Init_Landmasses()

Idx  Name            Squares  % Total   % Land
---  ------------  ---------  -------  -------
  0  Ocean              1868   77.83%      ---
162  Grasslands          120    5.00%    0.00%
163  Forest              188    7.83%    0.00%
164  Mountain             10    0.42%    0.00%
167  Tundra              171    7.12%    0.00%
171  Hills                43    1.79%    0.00%
---  ------------  ---------  -------  -------
Total Count:             2400

Idx  Name            Squares  % Total   % Land
---  ------------  ---------  -------  -------
  0  Ocean              1868   77.83%      ---
162  Grasslands          102    4.25%    0.00%
163  Forest              156    6.50%    0.00%
164  Mountain              8    0.33%    0.00%
165  Desert               38    1.58%    0.00%
166  Swamp                11    0.46%    0.00%
167  Tundra              185    7.71%    0.00%
171  Hills                32    1.33%    0.00%
---  ------------  ---------  -------  -------
Total Count:             2400

Idx  Name            Squares  % Total   % Land
---  ------------  ---------  -------  -------
  0  Ocean              1866   77.75%      ---
162  Grasslands           98    4.08%    0.00%
163  Forest              148    6.17%    0.00%
164  Mountain              8    0.33%    0.00%
165  Desert               36    1.50%    0.00%
166  Swamp                11    0.46%    0.00%
167  Tundra              185    7.71%    0.00%
168  SorceryNode           9    0.38%    0.00%
169  NatureNode            5    0.21%    0.00%
170  ChaosNode             2    0.08%    0.00%
171  Hills                32    1.33%    0.00%
---  ------------  ---------  -------  -------
Total Count:             2400

what changes Grasslands after the hightmap translation?
...going from 

--Grasslands    18
--Forest        32
--Mountain       2
--Hills         11
                ==
--              63

++Tundra        14
++Desert        38  ...starts from Forest, then walks over any non-Ocean square
++Swamp         11  ...starts from Forest, then only walks over Forest squares
                ==
++              63

--OCean          2
--Grasslands     4
--Forest         8
--Desert         2
                ==
--              16

++SorceryNode    9
++NatureNode     5
++ChaosNode      2
                ==
++              16

## code that use world map sections
NEWG_CreateLands__WIP()




## Geographical Limitations
void Generate_Towers(void)
            wx = (2 + Random(54));
            wy = (2 + Random(34));

Desert
        base_wx = ( 2 + Random( (WORLD_WIDTH  - ( 3 * 2)) ) );  // {  3, ..., 55 }
        base_wy = ( 8 + Random( (WORLD_HEIGHT - ( 8 * 2)) ) );  // {  9, ..., 25 }
Swamp
        base_wx = ( 1 + Random( (WORLD_WIDTH  - ( 2 * 2)) ) );  // {  2, ..., 57 }
        base_wy = (10 + Random( (WORLD_HEIGHT - (10 * 2)) ) );  // { 11, ..., 30 }
        if(
            (base_wy >= 35)
            &&
            (base_wy <= 45)  // BUGBUG  out of bounds of the world map
        )
        {
            continue;
        }

Rivers
    base_wx = (4 + Random((WORLD_WIDTH - 8)));
    base_wy = (4 + Random((WORLD_HEIGHT - 8)));


...node aura maker wont place on first or last on both axis
...node range: 0 distance, 1 map square; 1 distance, 9 map squares; 3 distance, 25 map squares
Generate_Nodes()
    ARCANUS_PLANE, 16 nodes
            ~ jittered placement?
            base_wx = ((itr % 5) * 12);
            base_wy = ((itr / 5) * 10);
            wx = (base_wx + (Random(24) - 1));
            wy = (base_wy + (Random(20) - 1));
    MYRROR_PLANE
            base_wx = (((itr - 20) % 5) * 12);
            base_wy = (((itr - 20) / 5) * 10);
            wx = (base_wx + (Random(24) - 1));
            wy = (base_wy + (Random(40) - 1));
    ...both have
        // BUGBUG  should be consistent? 2 or 3 from poles? 2 or 3 from edges?
        if(
            (wx < 3)
            ||
            (wy < 2)
            ||
            (wx >= (WORLD_WIDTH - 3))
            ||
            (wy >= (WORLD_HEIGHT - 3))
        )
        {
            continue;
        }

...ensure a minimum distance of 1 from top and bottom, and 3 from the left and right map edges
...one map squares from top and bottom means no nodes in the poles

//Gemini3
This assembly code is a classic example of procedural generation. Specifically, it is placing 16 "Arcanus Nodes" onto a world map.
The structure is a nested loop with a "try-until-valid" (rejection sampling) pattern. If a generated coordinate is invalid (out of bounds, too close to the edge, on water, or too close to another node), the code jumps back to @@Attempt_Arcanus to try again for that specific node.
The Logic Flow
1. Outer Loop: Iterates from itr = 0 to 15 (16 nodes total).
2. Inner "Attempt" Loop: - Calculate Grid Base: Uses the math we discussed to pick a sector based on itr.Randomize: Picks a coordinate and applies World Wrapping (if $x < 0$ add width, if $x > 60$ subtract width).Boundary Check: Rejects the point if it is too close to the map edges (e.g., $x < 3$ or $x > 57$).Terrain Check: Checks the tile at [wx, wy]. If it is Ocean1, there is only a 1-in-40 chance it will be accepted; otherwise, it jumps back to try again.Distance Check: Loops through all previously placed nodes (itr2 < itr) and uses Delta_XY_With_Wrap to ensure the new node is at least 3 units away from existing ones.Uniqueness Check: Calls NODE_IsAuraUnique. If it's not unique, start over.
3. Success: Once all conditions pass, it initializes the node data and increments itr.
```c
void Generate_Arcanus_Nodes() {
    short itr, itr2;
    short base_wx, base_wy, wx, wy;
    struct s_NODE *node_ptr;
    struct s_NODE *other_node;

    for (itr = 0; itr < 16; ) {
        /* @@Attempt_Arcanus */
        int accepted = 0;
        
        do {
            /* 1. Calculate Grid Sector */
            base_wx = (itr % 5) * 12;
            base_wy = (itr / 5) * 10;

            /* 2. Randomize within sector (Random(n) is 1..n) */
            wx = base_wx + (Random(24) - 1);
            wy = base_wy + (Random(20) - 1);

            /* 3. Handle World Wrap */
            if (wx >= WORLD_WIDTH)  wx -= WORLD_WIDTH;
            if (wy >= WORLD_HEIGHT) wy -= WORLD_HEIGHT;
            if (wx < 0)             wx += WORLD_WIDTH;
            if (wy < 0)             wy += WORLD_HEIGHT;

            /* 4. Edge Padding Checks */
            if (wx < 3 || wy < 2 || wx >= 57 || wy >= 37) continue;

            /* 5. Terrain Check (Ocean has 1/40 chance) */
            if (world_maps[wy][wx] == tt_Ocean1) {
                if (Random(40) != 1) continue;
            }

            /* 6. Distance Check against existing nodes */
            accepted = 1; /* Assume valid until proven otherwise */
            for (itr2 = 0; itr2 < itr; itr2++) {
                if (Delta_XY_With_Wrap(wx, wy, _NODES[itr2].wx, _NODES[itr2].wy, WORLD_WIDTH) < 3) {
                    accepted = 0;
                    break;
                }
            }
            if (!accepted) continue;

            /* 7. Initialize Node Data */
            node_ptr = &_NODES[itr];
            node_ptr->wx = (char)wx;
            node_ptr->wy = (char)wy;
            node_ptr->wp = 0;
            node_ptr->flags = 0;
            node_ptr->owner_idx = e_ST_UNDEFINED_DB;
            node_ptr->power = Random(6) + 4;

            /* 8. Aura Unique Check */
            NEWG_CreateNodeAura(node_ptr->power, node_ptr->Aura_Xs, node_ptr->Aura_Ys, wx, wy);
            if (NODE_IsAuraUnique(itr) == e_ST_TRUE) {
                accepted = 0; /* Assembly jmps to @@Attempt_Arcanus if True */
                continue;
            }

            /* Finalize */
            NEWG_SetNodeType(node_ptr->power, node_ptr->Aura_Xs, node_ptr->Aura_Ys, node_ptr->wp, node_ptr->type);
            accepted = 1;

        } while (!accepted);

        itr++; /* Move to next node */
    }
}
```








Module: MAPGEN

    data (0 bytes) _MAPGEN_TRACE
        Address: 02:0017A247

## _MAPGEN_TRACE
OON XREF:
Module: ADVCIV
    Twiddle_Selected_Adv_Civ_Planets_()
        Address: 01:000638A9
OON XREF:
Module: ADVCIV
    Advanced_Civilization_Colonies_()
        Address: 01:00062C70
