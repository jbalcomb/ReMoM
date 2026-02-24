MoX-MapGen.md
SEEALSO: MoX-Landmasses.md
SEEALSO: MoX-WorldMap.md



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






## code that use world map sections
NEWG_CreateLands__WIP()










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
