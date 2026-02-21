
//ClaudeOpus46

Here is a step-by-step breakdown of NEWG_CreateLands__WIP() (MAPGEN.c:2070):

Overview
This function generates the base landmasses for a single world plane (Arcanus or Myrror) using a "Drunkard's Walk" (random walk) algorithm.
It fills the 60x40 tile map with ocean and then grows organic continent shapes by walking randomly from scattered origin points.

Step-by-Step Breakdown
1. Initialize the entire plane to ocean (lines 2095-2101)
Every tile on the plane is set to tt_Ocean1. This is the blank slate before any land is placed.

2. Clear the map section tracking grid (lines 2107-2113)
A 5x5 array map_sections is zeroed out. This grid is used to spread continent origins across the map so they don't all cluster in one area. Despite being 5x5, only a 4x3 portion is actually reachable given the coordinate math (the comment at line 2106 notes this inconsistency).

3. Set section dimensions and count (lines 2115-2118)
n_sections = 8 — up to 8 different map sections can host an origin point.
section_width = 16 — each section column spans 16 tiles wide.
section_height = 11 — each section row spans 11 tiles tall.
These values don't cleanly divide the 60x40 world (60/16 ~3.75, 40/11 ~3.6), which the code comments flag as a possible artifact of a different map size at some point in development.

4. Determine the target land tile count based on _landsize (lines 2123-2128)
Land Size	n_needed	% of 2400 tiles
Small	360	15%
Medium	480	20%
Large	720	30%
The main loop runs until n_generated (tiles converted from ocean to land) exceeds n_needed.

5. Main generation loop — repeat until enough land exists (line 2131)
while(n_generated <= n_needed) drives the whole process. Each iteration of this outer loop creates one "continent blob."

5a. Pick a random origin point with section spreading (lines 2138-2154)
A random base coordinate is chosen: base_wx in {7..52}, base_wy in {7..32} — kept well within the map borders.
The section grid is consulted: if the section containing this point hasn't been used yet, it's marked as used and n_sections is decremented.
This ensures that the first 8 continents seed into 8 different regions of the map.
Once all 8 sections are claimed (n_sections <= 0), any origin point is accepted without the section check.

5b. Determine how many direction changes (arms) this continent has (line 2156)
direction_change_count = random value in {3, 4, 5}.
This creates multiple "arms" radiating from the origin, giving the continent an irregular, multi-lobed shape.

5c. For each arm, determine walk length (lines 2161-2168)
Land Size	Steps per arm
Small	6..15
Medium	11..20
Large	21..30

5d. Set the arm's starting position (lines 2172-2173)
The arm starts offset from the base origin by one cardinal step using TILE_Cardinal_XMod2[itr_direction_change_count].
Since the direction arrays are {S, W, N, E, none} and the loop index goes 0..4, each arm radiates outward from the origin in a different cardinal direction.

5e. Walk the arm — the inner step loop (lines 2179-2259)
For each step of the walk:

Count new land (lines 2182-2187): If the current tile is still ocean, increment n_generated (this is how progress toward n_needed is tracked).

Increment terrain hit counter (line 2189): The terrain type value at (itr_wx, itr_wy) is incremented by 1. The intent is to build a density/heightmap where repeated hits create higher terrain types.

Assign landmass ID (line 2191): Build_Landmass(wp, curr_wx, curr_wy) is called. This function (MAPGEN.c:2854) checks all 9 neighboring tiles for existing landmass IDs. If none exist, a new landmass ID is created. If multiple different landmass IDs are adjacent, they are merged (all tiles of the secondary landmass are reassigned to the primary). This is how separate walker paths that touch each other get unified into a single continent.

Choose next direction with anti-straight-line bias (lines 2199-2254): A random cardinal direction (0-3 = S/W/N/E) is picked. If it's the same direction as last time, the walk is biased against continuing straight — it only accepts the same direction with probability 1 / (Same_Dir_Steps * 2), which decreases the longer the walker has gone in one direction. This prevents long straight coastlines and encourages organic, blobby shapes. If a new direction is chosen, Same_Dir_Steps resets to 1.

Boundary check (lines 2233-2252): The candidate next position is checked against map borders with a margin (2 tiles on X edges, 4 tiles on Y edges). If out of bounds, the direction is rejected (Last_Direction = ST_UNDEFINED) and the direction-picking loop retries. This keeps land away from the map edges.

Advance (lines 2256-2257): curr_wx/curr_wy move to the accepted next_wx/next_wy.

6. Termination
The function returns once n_generated > n_needed. At that point, the plane has the target amount of land distributed across several organic, irregularly-shaped continents, each tracked by a unique landmass ID.

Summary of Key Mechanisms
Mechanism	Purpose
Section grid	Spreads continent origins across the map
Multiple arms per origin	Creates multi-lobed continent shapes
Same-direction decay	Prevents straight lines, creates organic blobs
Border margin rejection	Keeps land away from map edges
Build_Landmass merging	Unifies adjacent walker paths into single continents
Terrain hit counting	Builds a density map (higher counts = heavier terrain later)
