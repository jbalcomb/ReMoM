MoX-MapGen.md
SEEALSO: MoX-Landmasses.md
SEEALSO: MoX-WorldMap.md



MapGen uses 1000 as temporary River terrain type





## River_Path()
1. pick a random starting location
2. validate location
    * square does not have a Terrain Special
    * square is not Ocean
    * adjacent squares are not Ocean
3. initialize path finding
    * random cardinal direction
4. make 30 attempts at finding a valid path

direction
directions array does nothing?

...minimum length of 4
...maximum length of 29

not if the next square has a special or is Mountain, Hills, Node, or Desert
if the next square is Square_Is_River_NewGame()
if the next square or an adjacent is Square_Is_Ocean_NewGame()

uses dir_chg_tbl_wx__1[], dir_chg_tbl_wy__1[]

dir_chg_tbl_wx[]
...1 or 2
...indexed twice sometimes?
...indexed by direction, dir_chg

Generate_Landmasses() looks to have some similar usage of a direction variable that can be ST_UNDEFINED
    new_direction = dir_chg;
    next_wx = (curr_wx + dir_chg_tbl_wx[dir_chg]); next_wy = (curr_wy + dir_chg_tbl_wy[dir_chg]);
    if next_wx,wy invalid then new_direction = ST_UNDEFINED;

curr_wx vs. next_wx in a dir_chg loop
    Generate_Climate_Terrain_Types()  ...does all five directions ...takes
    Generate_Landmasses()             ...does 3-5 directions      ...takes gol steps 5,10,20
    Make_Aura()
    River_Path()

Generate_Climate_Terrain_Types()
...new_direction = ST_UNDEFINED;  // Eh? c&p error?
            curr_wx = (base_wx + dir_chg_tbl_wx[direction]);
                next_wx = (curr_wx + dir_chg_tbl_wx[dir_chg]);

Generate_Landmasses()
~ dir chg cnt, stp cnt
            curr_wx = (base_wx + dir_chg_tbl_wx[direction]);
            curr_wy = (base_wy + dir_chg_tbl_wy[direction]);
                    new_direction = dir_chg;
                    next_wx = (curr_wx + dir_chg_tbl_wx[dir_chg]);
                    next_wy = (curr_wy + dir_chg_tbl_wy[dir_chg]);

Make_Aura()
            curr_wx = (wx + dir_chg_tbl_wx[dir_chg_wx]);
            curr_wy = (wy + dir_chg_tbl_wy[dir_chg_wy]);

River_Path()
        next_ws = Tile_Xs[(length - 1)] + dir_chg_tbl_wx[next_dir];
        next_wy = Tile_Ys[(length - 1)] + dir_chg_tbl_wy[next_dir];



Prev_Direction
set to ST_UNDEFINED
test for ST_UNDEFINED
assigned curr dir, immediately following selection of curr dir
assigned curr dir, right before looping
Statement of Purpose?
...seems like all it does is ensure ST_TRUE on the first choice of direction change ... Selected_Direction = Main_Direction;
Yep, that's it.
Shoulda just been a 'first run' init true and set false after first loop iteration.


flow downhill due to gravity, taking the path of least resistance towards a larger body of water (ocean, lake, or another river). 
Instead of a specific cardinal direction, geographers and navigators use these terms to describe the direction of river flow:
Downstream: The primary direction of flow, moving from higher elevation (source/headwaters) to lower elevation (mouth).
Upstream: The direction against the current, moving toward the source.
River Right / River Left: Defined by looking in the direction of the downstream flow.
Drainage Basin/Watershed: The area that collects water for a river system. 





https://masterofmagic.fandom.com/wiki/River

Geography
River tiles are never isolated. When the world is created, the game will "stretch" several Rivers across the land. Each River begins inland, usually next to a Mountain or Hill tile, and runs across several tiles. The end of the River always meets the Shore of an Ocean or inland lake. The final tile in this series is always a River Mouth tile.

The game will generate a random number of Rivers on each Plane, though the total maximum number is 10 Rivers per plane (according to the official Strategy Guide). Each of these Rivers can be several tiles long.

Common Minerals
River tiles will never contain any Terrain Specials, whether Minerals, Nodes, or Encounter zones. The game explicitly prohibits this.



https://masterofmagic.fandom.com/wiki/River_Mouth

Geography
River Mouth tiles are placed by the game in every location where a River meets a Shore, and will not be found anywhere else. This happens both on the shores of Oceans, as well as the shores of inland lakes. Because of this, River Mouth tiles never appear on their own. There is always a River pointing inland from this meeting place, and a Shore tile on the other side.
