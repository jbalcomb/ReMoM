


¿ Deep-Dives / Reviews / Scrubdowns ?

angle.c
I tried, gave up, later, borrowed from 1oom, much later used the gemini-3-pro AI model



Generate_Combat_Map()
Set_Terrain_Tile_Types(cts);
Scatter_Tree_Scenery(cts, location_type);
Scatter_Rock_Scenery(cts, location_type);
[] Carve_River_Terrain(rivers_array);
[] Build_Road_Network(location_type, roads_array);
[] CMB_RemoveRough(location_type);
[] Merge_Dirt_Patches();

                        // ; decides whether the AI will raze the selected city or not
                        // ; returns 1 if yes, 0 if no
                        // ; INCONSISTENT: Lawful wizards disproportionately
                        // ;  adjust based on whether hidden relation is 0 or not
                        // ; BUG? surviving units are checked without owners
                        // TODO  GAME_RazeCity = Raze_Check(player_idx, OVL_Action_Structure);
                        GAME_RazeCity = ST_FALSE;

