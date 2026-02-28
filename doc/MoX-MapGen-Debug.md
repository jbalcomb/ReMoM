
OSG pg 182
Table 14.4
What Special Terrain Feature Appears?
...
The number is the percent chance of that particular special appearing when its parent terrain square receives a special terrain feature.

OSG pg 183
Special Terr aitt
Feature Placemettt
After the basic land squares are created, they
are indjvidual ly examjned on both worlds to
see if a special terrain featlu·e is added. Each
desert, hill, moun tain. fo rest, and swamp
square on Arcanus has a 1 in 17 chance of
receiving a special terrain feature. These same
squares on lp-ror have a 1 in 10 chance of
bearing goodies. The chance of a particu lar
type of pecial terrain feature appearing on
that square is sho\\11 in Table 14.4.

count all  of desert, hill, mountain, forest, swamp
count each of desert, hill, mountain, forest, swamp
count all specials for  desert, hill, mountain, forest, swamp
count specials for each of desert, hill, mountain, forest, swamp
arcanus - check for 1:17  0.05882352941176470588235294117647  5.88%
myrror  - check for 1:10  0.10  10%
Arcanus
Desert      Gems, Quark
Hills       Iron, Coal, Silver, Gold, Mithril
Mountains   Iron, Coal, Silver, Gold, Mithril
Forests     Wild Game
Swamp       Nightshade
Myrror
Desert      Gems, Quark, Crysx
Hills       Iron, Coal, Silver, Gold, Mithril, Adamantium
Mountains   Iron, Coal, Silver, Gold, Mithril, Adamantium
Forests     Wild Game
Swamp       Nightshade

Add another function just for the distribution specials on Desert squares. On Arcanus, they should be 66.7% Gems and 33.3% Quark, 0% Crysx. On Myrror, they should be 20% Gems, 40% Quark, 20% Crysx.

Add another function just for the distribution specials on Hills squares. On Arcanus, they should be 33.3% Iron, 16.7% Coal, 22.2% Silver, 22.2% Gold, 5.6% Mithril, 0% Adamantium. On Myrror, they should be 10% Iron, 10% Coal, 10% Silver, 40% Gold, 20% Mithril, 10% Adamantium.

Add another function just for the distribution specials on Mountain squares. On Arcanus, they should be 22.2% Iron, 27.7% Coal, 16.7% Silver, 16.7% Gold, 16.7% Mithril, 0% Adamantium. On Myrror, they should be 10% Iron, 10% Coal, 10% Silver, 20% Gold, 30% Mithril, 20% Adamantium.

Update the evaluation functions for Desert, Hills, Mountain to show all of their terrain specific specials even if they have a count of zero.
Update the evaluation functions for Desert, Hills, Mountain to show all of their terrain specific specials even if they have an expected count of zero.

Update the evaluation functions for Desert, Hills, Mountain. Remove the total row from the table, since it's meaningless.





ask Claude Opus/Sonnet about the random function's LFSR assembly code
ask GPT
ask Gemini





Add a function to collect statistics for #_landmasses so we can assess how well the drunkards walk algorithm is work. Include trial count, hit count, and sum so we can calculate the average probability after the simulation completes. It's OK if we need to save to a local file. Suggest other ideas for validating the algorithm or heightmap.


Here's a summary of the changes and the math behind them, followed by suggested validation ideas.

What was added
STU_WRLD.h
Fixed the header guard (STU_VLD_H → STU_WRLD_H)
Added struct s_Landmass_Walk_Stats with trial_count, hit_count, sum
Declared Collect_Landmass_Walk_Stats and Write_Landmass_Walk_Stats
STU_WRLD.c
Added includes for TerrType.h (for tt_Ocean1), MOM_DEF.h (dimensions), MOX_DEF.h (GET_TERRAIN_TYPE), stdio.h, and STU_WRLD.h
Collect_Landmass_Walk_Stats — scans the finished plane and derives all three fields from the heightmap
Write_Landmass_Walk_Stats — appends a TSV row to landmass_walk_stats.txt; writes a header if the file is new
Calls both from Simulate_World_Map_Generation right after Generate_Landmasses
Why trial_count == sum
Generate_Landmasses starts every square at tt_Ocean1 = 0 and does:
SET_TERRAIN_TYPE(wx, wy, wp, GET_TERRAIN_TYPE(wx, wy, wp) + 1);
unconditionally on every step. Each step adds exactly +1 to exactly one square, so summing all terrain values gives the total step count. This is a useful sanity check: if trial_count != sum, something is wrong.
The output TSV columns are:
column	meaning
trial_count	total walker steps (= sum)
hit_count	distinct land squares created
sum	sum of per-square visit counts
avg_prob	hit_count / trial_count — efficiency: how many steps produced new land
avg_visits	sum / hit_count — mean revisit depth (heightmap mean over land)
Other validation ideas for the walk / heightmap
Land-to-target ratio — compare hit_count against n_needed (360 / 480 / 720). They should be very close; a big gap signals the termination condition is off.
Heightmap histogram — bucket terrain-type values (1, 2, 3, …) and plot the frequency. A classic random walk produces a roughly Poisson distribution; a heavy tail means walkers are revisiting the same corridors (possibly a direction-bias bug).
Distinct landmass count and size CDF — after Build_Landmass, count how many unique landmass IDs exist and sort their sizes. Should produce ~8 landmasses of varying area; a count of 1 means everything merged into one blob.
Coast-to-interior ratio — for each land square, check whether any of its 4 cardinal neighbours is ocean. The fraction of "coastal" squares over total land is a shape indicator: high = thin archipelago, low = compact continent.
Direction-bias histogram — instrument the direction-choice loop to tally North / South / East / West picks. Over thousands of steps the distribution should be near-uniform (25 % each); skew indicates the Random(4) - 1 mapping is wrong.
Bounding-box compactness per landmass — for each landmass, record (land_squares / (bbox_width * bbox_height)). Compact blobs score near 1; thin, wandering fingers score near 0.
Land / ocean adjacency ratio — measure what fraction of ocean squares are adjacent to land (the "shoreline density"). Helps tune how jagged coastlines look.
Reproducibility with fixed seed — run Generate_Landmasses twice with the same PRNG seed and assert the heightmaps are identical. Confirms no hidden global state mutates between calls.





...
Want me to also add a third test for the randomized polar strip behavior (asserting only allowed outcomes, not exact RNG layout)?

It seeds the inner polar rows (WORLD_YMIN + 1, WORLD_YMAX - 1) with a deterministic pre-conversion value (6 -> mountain).
After calling NEWG_SetBaseLands__WIP, it asserts each inner-polar cell is only tt_Mountain1 or tt_Tundra1, which captures valid randomized strip behavior without depending on exact RNG layout.


use #e_TERRAIN_TYPES to build a new symbol mapping string in #Print_Worldmap_Map, collapse the ranges that are the same type group (e.g., tt_Shore1_Fst thru tt_Shore1_Lst should just be Shore).

as an expert oldschool C programmer, add a comment for #Get_Worldmap_Terrain_Type describing in minute detail what is doing and why it is doing it



as an expert oldschool C programmer, add a comment for #NEWG_Clicked_Race describing in minute detail what is doing and why it is doing it

Done. The comment at NewGame.c:1070 now documents:
What it is -- the enum_RACE_BYTE value of the race the human player clicked on the "Select Race" screen (New Game Screen 6).
How it gets written -- from Arcanus_Races[itr] for indices 0-8, or Myrran_Races[itr-9] for indices 9-13 (guarded by the Myrran retort check, with a warning dialog on rejection).
The blink animation -- how the value is immediately read back to drive the four-pass highlight/dim redraw loop that gives visual click feedback.
Downstream consumption -- how MAPGEN uses it to assign the race of the human player's starting city (_CITIES[_cities].race), and that AI players ignore it.
Why it's int16_t -- the original executable stores it as a 16-bit word at dseg:8EA4, even though enum_RACE_BYTE values fit in a byte.
Lifetime -- set once during new-game setup, read during map generation, not meaningful after gameplay begins.





e.g.,
Codex TODO prompt
Please implement the code as described by the comment on STU/src/STU_WRLD.c:357

The comment is:
Worldmap_Statistics();

Replace the comment with your implementation.
Optimize for speed. Don't over-explore or over-check your work other than what is absolutely necessary to implement the comment.
Once you are done, remove the comment.




As an expert oldschool C programmer, add a comment describing, in minute detail, the difference between #Get_Useable_City_Area() in #CITYCALC.c and #Get_Useable_City_Area_NewGame in #MAPGEN.c.
They are used for the same reason, so start with a short summary of their purpose, then, in the detail, focus only on their differences.
This should be document in #MoM-CityArea.md in the doc directory.

CITYCALC.c
// WZD o142p08
int16_t Get_Useable_City_Area(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t *wx_array, int16_t *wy_array)
MAPGEN.c
// MGC o51p53
int16_t Get_Useable_City_Area_NewGame(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t * wx_array, int16_t * wy_array)





Random Seed?
this is overwritten by Randomize() in Init_Drivers() in ReMoM.c, which sets the seed to the system clock timer value.

commented out the call Randomize() in Init_Drivers, in Init.c
    // MAP_TRACE
    // Randomize();
now, should get same map every time?







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



Ocean, Grasslands, Forest, Mountain, Desert, Swamp, Tundra, Hills
SorceryNode = 0x0A8,
NatureNode  = 0x0A9,
ChaosNode   = 0x0AA,

...
Would you like me to add a "Seed" input so you can recreate specific world layouts by entering a number, similar to how Civilization or SimCity handled world gen on DOS?
...
Adding Coordinate Wrapping (a Toroidal world) is a classic trick used in games like Civilization or Ultima.
...


As an old school C programmer working in MS-DOS, plan a world generation processes that  takes a parameter for size, randomly builds 8 landmasses made of that many map squares. During the landmasses generation function, keep track of the number of times a map squares gets randomly selected and then use that count to choose the terrain type that map square will have in the world map - Ocean, Grasslands, Forest, Mountain, Desert, Swamp, Tundra, Hills.
The target platform is IBM-PC, MS-DOS, VGA Mode Y (320x200 pixels,256 colors).
The world map is 60 x 40 maps squares and is allowed to wrap on the x-axis.
Use a map window show a 12 x 10 portion of the world map.


Listen, if we’re targetting MS-DOS, we need to be mindful of that 640K barrier and the speed of an 8086. We aren't going to waste cycles on complex noise functions; we’re going to use a "Random Walk" or "Drunkard’s Walk" algorithm. It’s memory-efficient and creates organic-looking blobs.Since you want the "stacking" effect (counting hits per tile) to determine terrain, we’ll use a 2D array to store "hit counts."The StrategyThe Grid: Define a maximum map size (e.g., $100 \times 100$). Each byte in our world_map array starts at 0.The Seed: Use time.h or an interrupt call to seed srand().The Walk: For each of the 8 landmasses, pick a random starting $x, y$. For $N$ steps (your size parameter), move one square in a random cardinal direction.The Accumulation: Every time a walker lands on a square, increment its value in the array.The Render: Iterate through the array. Low counts stay Ocean; high counts become Mountains or Deserts.Proposed C Implementation (K&R Style)

To implement this in a way that feels like true 1980s/90s PC game development, we’ll introduce a Momentum variable. Instead of picking a new random direction every single step, we’ll use a "persistence" check. If a random roll fails (e.g., 70% chance), we keep the previous direction; otherwise, we "stumble" into a new one.

This prevents the "static noise" look and creates long, winding "peninsulas" or "mountain ranges" that look much more like real geography.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WIDTH  80
#define MAX_HEIGHT 25 /* Standard DOS text mode */

unsigned char world[MAX_WIDTH][MAX_HEIGHT];

void generate_land(int size) {
    int i, j, x, y, dir;
    
    /* Initialize the sea of zeros */
    for(x = 0; x < MAX_WIDTH; x++) {
        for(y = 0; y < MAX_HEIGHT; y++) {
            world[x][y] = 0;
        }
    }

    /* Create 8 landmasses */
    for(i = 0; i < 8; i++) {
        x = rand() % MAX_WIDTH;
        y = rand() % MAX_HEIGHT;

        for(j = 0; j < size; j++) {
            /* Increment "hit count" for this square */
            if(world[x][y] < 255) world[x][y]++;

            /* Random Walk: 0=Up, 1=Down, 2=Left, 3=Right */
            dir = rand() % 4;
            if(dir == 0 && y > 0) y--;
            else if(dir == 1 && y < MAX_HEIGHT - 1) y++;
            else if(dir == 2 && x > 0) x--;
            else if(dir == 3 && x < MAX_WIDTH - 1) x++;
        }
    }
}

char get_terrain(int hits) {
    if (hits == 0) return ' '; /* Ocean (Blue) */
    if (hits == 1) return '.'; /* Grasslands */
    if (hits == 2) return 'f'; /* Forest */
    if (hits == 3) return 'h'; /* Hills */
    if (hits == 4) return 'S'; /* Swamp */
    if (hits == 5) return 'T'; /* Tundra */
    if (hits == 6) return 'D'; /* Desert */
    return '^';                /* Mountain (Highest hits) */
}

Terrain Logic TableBy tracking the number of "hits," we are essentially creating a heightmap or a "density map." The more a walker revisits a spot, the more "significant" that terrain becomes.HitsTerrainDOS CharacterLogic0Ocean     Never touched by a walker.1Grasslands.Single pass; coastal or fringe areas.2ForestfLight overlap; moderate vegetation.3HillsnFrequent overlap; rising elevation.4SwampsHigh overlap; low-lying wet basins.5Tundra*Heavy overlap; cold or sparse heights.6Desert~Intense overlap; rain-shadow or extremes.7+Mountains^The "Heart" of the landmass.
