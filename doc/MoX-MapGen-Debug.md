


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
