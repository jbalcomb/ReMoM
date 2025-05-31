
SEEALSO:  MoM-EMS-EMMDATAH.md
SEEALSO:  MoM-EMS-VGAFILEH.md
SEEALSO:  MoX-EMM-CONTXXX.md
SEEALSO:  MoX-EMM-FIGUREX.md
SEEALSO:  MoX-EMM-TILEXXX.md



EMS / EMM



EMMDATAH

VGAFILEH


FIGUREX
TILEXXX
CONTXXX

void Allocate_Data_Space(int16_t gfx_buff_nparas)
    // TODO  EmmHndl_FIGUREX = EMM_GetHandle(28, EmmHndlNm_FIGUREX, 1);
    EmmHndl_FIGUREX = Allocate_Space(28672);  // 28 * 16384 EMM Page Size = 458752 B / 16 = 28672 PR
    // TODO  EmmHndl_TILEXXX = EMM_GetHandle( 3, EmmHndlNm_TILEXXX, 1);
    EmmHndl_TILEXXX = Allocate_Space(3072);   //  3 * 16384 EMM Page Size =  49152 B / 16 =  3072 PR
    // TODO  EMM_ContXXX_H = EMM_GetHandle(4, cnst_EMM_ContH_Name, 1)
    EmmHndl_CONTXXX = Allocate_Space(4096);   //  4 * 16384 EMM Page Size = 65536 B / 16 = 4096 PR

// drake178: IMG_CMB_TerrTiles
// ; array of 48 images appended into the EMM TILEX handle
// WZD  dseg:939A
/*
definitely just 48, but looks like 55
¿ fits combat terrain type check of >55/>=56 ?
*/
// SAMB_ptr _combat_terrain_pict_segs[48];
SAMB_ptr _combat_terrain_pict_segs[55];


## FIGUREX

Allocate_Data_Space()
    // TODO  EmmHndl_FIGUREX = EMM_GetHandle(28, EmmHndlNm_FIGUREX, 1);
    EmmHndl_FIGUREX = Allocate_Space(28672);  // 28 * 16384 EMM Page Size = 458752 B / 16 = 28672 PR
EMM_FIGUREX_Init__HACK()
    EMM_Map4Pages(logical_page, EmmHndl_FIGUREX)



## TILEXXX

Allocate_Data_Space()
    // TODO  EmmHndl_TILEXXX = EMM_GetHandle( 3, EmmHndlNm_TILEXXX, 1);
    EmmHndl_TILEXXX = Allocate_Space(3072);   //  3 * 16384 EMM Page Size =  49152 B / 16 =  3072 PR
EMM_TILEX_Init__HACK()
    EMM_Map4Pages(0, EmmHndl_TILEXXX)

## CONTXXX

Allocate_Data_Space()
    // TODO  EMM_ContXXX_H = EMM_GetHandle(4, cnst_EMM_ContH_Name, 1)
    EmmHndl_CONTXXX = Allocate_Space(4096);   //  4 * 16384 EMM Page Size = 65536 B / 16 = 4096 PR





### EMMDATAH
EMM_EMMDATAH_AllocFirst()
EMM_EMMDATAH_AllocNext()

EMM_EMMDATAH_AllocFirst()
`OON XREF`  ALLOC  Allocate_Data_Space()
EMM_EMMDATAH_AllocNext()
`OON XREF`  ALLOC  Allocate_Data_Space()

all wrapped in SA_MK_FP0()

movepath_cost_map
nmovement_mode_cost_maps
city_area_shared_bits
city_area_bits
TBL_OvlMovePathsEMS@

dseg:9C90 00 00 00 00                                     TBL_OvlMovePathsEMS@ dd 0
dseg:9C94 ...
dseg:9C98 ...
dseg:9C9C 00 00 00 00                                     city_area_bits dd 0
dseg:9CA0 00 00 00 00                                     city_area_shared_bits dd 0
dseg:9CA4 00 00 00 00                                     movepath_cost_map dd 0
dseg:9CA8 ...
dseg:9CAC 00 00 00 00                                     movement_mode_cost_maps dd 0





### VGAFILEH
