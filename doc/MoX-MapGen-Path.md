


Don't see anything like this in MoO2.

MGC
    ovr054
        CRP_SPATH_Arbitrary()
        UU_SPATH_Segmented()
        UU_SPATH_15by15()
        UU_SPATH_Segments()
        UU_SPATH_GetBorderTiles()
    ovr055
        CRP_NEWG_CreatePathGrid()




## movement_mode_cost_maps

// WZD dseg:9CAC
// drake178: TBL_MoveMaps_EMS
// AKA gfp_TBL_MoveMaps_EMM
struct s_MOVE_MODE_COST_MAPS * movement_mode_cost_maps;

MGC
dseg:89A6 00 00 00 00                                     gfp_CRP_TBL_OvlMovePathsEMM dd 0        ; DATA XREF: Allocate_Data_Space+1FFw ...
dseg:89A6                                                                                         ; 409h EMMData paragraphs
dseg:89A6                                                                                         ; can hold an array of up to 140 MovePath structures
dseg:89AA 00 00 00 00                                     UU_EMM_PageFrame_3840 dd 0
dseg:89AE 00 00 00 00                                     UU_EMM_PageFrame_5DC0 dd 0
dseg:89B2 00 00 00 00                                     gfp_CRP_TBL_Catchments_EMS dd 0         ; DATA XREF: Allocate_Data_Space+1E7w ...
dseg:89B2                                                                                         ; bitflag table of the map tiles that are in the
dseg:89B2                                                                                         ; catchment area of a city
dseg:89B2                                                                                         ; Myrror starts at 960h as on the byte maps, but this
dseg:89B2                                                                                         ; is not actually a byte map (coding oversight)
dseg:89B2                                                                                         ; allocated: 12Dh EMMData paragraphs even though it
dseg:89B2                                                                                         ; would only need 601 bytes if implemented properly
dseg:89B6 00 00 00 00                                     gfp_CRP_TBL_SharedTiles_EMS dd 0        ; DATA XREF: Allocate_Data_Space+1CFw ...
dseg:89B6                                                                                         ; bitflag table of the map tiles that are in the
dseg:89B6                                                                                         ; catchment area of two cities (shared tiles)
dseg:89B6                                                                                         ; Myrror starts at 960h as on the byte maps, but this
dseg:89B6                                                                                         ; is not actually a byte map (coding oversight)
dseg:89B6                                                                                         ; allocated: 12Dh EMMData paragraphs even though it
dseg:89B6                                                                                         ; would only need 601 bytes if implemented properly
dseg:89BA 00 00 00 00                                     gfp_CRP_TBL_TempMoveMap_EMS dd 0        ; DATA XREF: Allocate_Data_Space+19Fw ...
dseg:89BA                                                                                         ; 278h EMMData paragraphs
dseg:89BE 00 00 00 00                                     UU_COL_MinimapTiles@ dd 0
dseg:89C2 00 00 00 00                                     gfp_TBL_MoveMaps_EMM dd 0               ; DATA XREF: Save_SAVE_GAM+322r ...
dseg:89C2                                                                                         ; 70Ah EMMData paragraphs
dseg:89C2                                                                                         ; contains 2 HMP_MAPS structures, one per Plane
dseg:89C6 00 00 00 00                                     TBL_Scouting dd 0                       ; DATA XREF: Save_SAVE_GAM+304r ...
dseg:89C6                                                                                         ; 12Eh paragraphs in World_Data@
dseg:89CA 00 00 00 00                                     _map_square_flags dd 0                  ; DATA XREF: Save_SAVE_GAM+354r ...
dseg:89CA                                                                                         ; 12Eh paragraphs in World_Data@
dseg:89CE 00 00 00 00                                     _map_square_terrain_specials dd 0       ; DATA XREF: Save_SAVE_GAM+2EBr ...
dseg:89CE                                                                                         ; 12Eh paragraphs in World_Data@
dseg:89CE                                                                                         ; 1 byte per map tile (max 1 TER_SPEC on each tile)
dseg:89D2 00 00 00 00                                     _CITIES dd 0                            ; DATA XREF: Save_SAVE_GAM+2B9r ...





CRP_NEWG_CreatePathGrids__STUB()
    |-> CRP_NEWG_CreatePathGrid()
        5x5
        60/5=12 horizontal
        40/5=8 vertical


WZD
ovr147
    MovePath.c/.h
        Line 204
        void Move_Path_Find(int16_t arg_wx, int16_t arg_wy, struct s_MOVE_PATH * arg_movepath_cost_map)

ovr168
