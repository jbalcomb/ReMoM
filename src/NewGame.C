
/*

    MoO2
        Module: NEWGAME
            Newgame_Screen_
                Address: 01:000CD435
            Reload_Newgame_Screen_
                Address: 01:000CCA1C
            Draw_Newgame_Screen_
                Address: 01:000CCD3C
            Draw_Pictures_
                Address: 01:000CCC3D
            Add_Fields_
                Address: 01:000CCE2E
            Update_Fields_
                Address: 01:000CCE24
            data (0 bytes) _fields
                Address: 02:0019933C
            data (0 bytes) _background_seg
                Address: 02:00199354

        Module: MAPGEN
    
Reload_Newgame_Screen_
is where the loading happens
i.e., (Re-)Load

*/
/*

terrain_stats_table
770 records
6 bytes
half movement cost per movement type
0 unknwon
1 walking
2 Forster
3 Mountaineer
4 Swimming
5 Sailing
But, also, why 770? world_map terrain_type offset is 762 ?

*/

/*
Load_TERRSTAT()
terrain_stats_table = SA_MK_FP0( LBXR_LoadSingle("TERRSTAT", 0, 0, 770, 6) )

    XREF NEWG_SetMoveMaps+61  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+94  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+C4  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+F7  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+12A les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+15D les     si, [terrain_stats_table]
*/

/*
Load_SPELLDAT()
spell_data_table    = SA_MK_FP0( LBXR_LoadSingle("SPELLDAT", 0, 0, 215, 36) )

*/