MoX-NewGame-MapGen-Landmass.md





https://masterofmagic.fandom.com/wiki/Terrain_Special#Chances_of_Specials

Terrain Special

Terrain Specials are special properties that can appear on a land tile. Terrain Specials are part of the World Creation that takes place at the start of a New Game. Many Terrain Specials are Minerals and will benefit any Town that can claim the tile where the mineral is found. Some Terrain Specials are bonuses that can be claimed by a Wizard and may contain wondrous Treasures. Magical Nodes are Terrain Specials that can enhance both the Town and the Wizard.

Contents
1.	Types of Specials
1. 1.	Encounter Zones
1. 2.	Nodes
1. 3.	Towers of Wizardry
1. 4.	Minerals

Types of Specials
Terrain Specials are generated as part of the World Creation at the start of a new game; most cannot be created later in the game.

Encounter Zones
Encounter Zones contain Treasure and, usually, defending fantastic creatures; in order to claim the Treasure you must move into the zone and defeat any defenders. For a Lair, Cave, Keep, Ancient Temple, Fallen Temple, Ruins, or Dungeon, the zone is simply removed when the Treasure is claimed; Nodes and Towers of Wizardry continue to exist and have ongoing uses. Encounter Zones can create Rampaging Monsters, making them a hazard to nearby towns. The only Encounter zone that can be created after the start of a game is Ruins.

Nodes
There are three types of magical Nodes (Nature Nodes, Sorcery Nodes, and Chaos Nodes) found in the game. These nodes can benefit a Town in several ways based on the Terrain where they are found and by how much magic they generate. All nodes are initially Encounter Zones, guarded by creatures of the same type as the node, but once conquered, they are not removed, and may be claimed for further benefits.

Towers of Wizardry
A Tower of Wizardry is a link between Myrror and Arcanus; a unit on the same tile as a Tower of Wizardry may change planes. Like Nodes, they are initially Encounter zones, and must be cleared before being used, but they cannot be claimed.

Minerals
Minerals are prized for the benefits they provide and can be claimed by building a Town nearby. It is possible, but rare, for Minerals to be discovered near an existing Town.



https://masterofmagic.fandom.com/wiki/Mineral#Chances_of_Minerals

Chances of Minerals
6% of Desert, Forest, Hill, Mountain, and Swamp tile on Arcanus, and 10% on Myrror, will have a Special Terrain Feature. While Forest and Swamp tiles always get TerrainSpecial Wild Game Wild Game and TerrainSpecial Nightshade Nightshade respectively, Desert, Hill and Mountain tiles can have a bunch of different mineral deposits; the total odds of each type are:

Chances for Terrain Specials
Terrain	            Iron	Coal	Silver	Gold	Mithril	Adamantium	Gems	Quork	Crysx
Arcanus Hill	    2 %	    1 %	1.33 %	1.33 %	0.33 %	---	---	---	---
Myrror Hill	        1 %	    1 %	1 %	4 %	2 %	1 %	---	---	---
Arcanus Mountain	1.33 %	1.67 %	1 %	1 %	1 %	---	---	---	---
Myrror Mountain	    1 %	    1 %	1 %	2 %	3 %	2 %	---	---	---
Arcanus Desert	    ---	    ---	---	---	---	---	4 %	2 %	---
Myrror Desert	    ---	    ---	---	---	---	---	2 %	6 %	2 %








// MGC o51p10
void NEWG_CreateLands__WIP(int16_t wp)


while(Generated_Land_Tiles <= Desired_Land_Tiles)


Used_Map_Sections is not actually useful?

loops eight times, generates wx,wy, 

    Section_Width = 16;
    Section_Height = 11;
    Origin_X = (6 + Random(46));
    Origin_Y = (6 + Random(26));
    if(Used_Map_Sections[(Origin_Y / Section_Height)][(Origin_X / Section_Width)] != 1)
        Used_Map_Sections[(Origin_Y / Section_Height)][(Origin_X / Section_Width)] = 1;

Used_Map_Sections[(7 / 11)][(7 / 16)]
