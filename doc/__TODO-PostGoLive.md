


¿ Deep-Dives / Reviews / Scrubdowns ?

angle.c
I tried, gave up, later, borrowed from 1oom, much later used the gemini-3-pro AI model



Generate_Combat_Map()
CMB_TileGen__WIP(cts);
CMB_TreeGen(cts, location_type);
CMB_RockGen(cts, location_type);
[] CMB_RiverGen(rivers_array);
[] CMB_RoadGen(location_type, roads_array);
[] CMB_RemoveRough(location_type);
[] CMB_MergeDirt();

                        // ; decides whether the AI will raze the selected city or not
                        // ; returns 1 if yes, 0 if no
                        // ; INCONSISTENT: Lawful wizards disproportionately
                        // ;  adjust based on whether hidden relation is 0 or not
                        // ; BUG? surviving units are checked without owners
                        // TODO  GAME_RazeCity = Raze_Check(player_idx, OVL_Action_Structure);
                        GAME_RazeCity = ST_FALSE;

