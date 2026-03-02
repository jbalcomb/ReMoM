


NOTE(JimBalcomb,20260302): moved notes from TerrType.h






terrain order pattern, some relation to autotiling
per drake178's naming scheme

Simtex_Autotiling()
mask value
 1, 2, 4, 8,16,32,64,128
NW, N,NE, E,SE, S,SW,  W

    _Shore00001000  = 0x2,  Land to the East
    _Desert00001000 = 0x124,
    _Tundra00001000 = 0x25A,  /* start of range for Tundra autotiling */
...these three ranges seem to have the same pattern

Mountain and Hills have the same order/pattern/bits; 16 each
...both have 1111 twice
...must be 4 cardinal directions, N,E,S,W, and not diagonals
...extra _1Hills2 and range is wrong?






/*
Terrain Type Range Conditions

...
...
...
something like, ...
is not 0
is not 1
then, <= the upper boundary of each range of terrain types

not sure about this specific animated ocean terrain
everything after this is Tundra
<= tt_AnimOcean  = 259h
~ < 'Tundra Range'

>= tt_Grasslands1  0xA2
>= tt_Shore1_Fst  = 2
<  tt_Shore1_Lst  = 0A1h
~ > 'Shore1 Range'

~ < 'Shore3 Range'

~ < '4WRiver Range'



*/


// DELETEME  enum Terrain_Type
// DELETEME  {
// DELETEME      e_TT_BugGrass       =   1,  /* 0x0001 */
// DELETEME      e__Grasslands1      = 162,  /* 0x00A2 */
// DELETEME      e_TT_Grass1         = 162,  /* 0x00A2 */
// DELETEME      e_TT_Forest3        = 184,  /* 0x00B8 */
// DELETEME      e_TT_Rivers_end     = 258,  /* 0x0102 */
// DELETEME      e__Mount1001        = 274,  /* 0x0112 */
// DELETEME      e__1Hills2          = 291,  /* 0x0123 */
// DELETEME      e__Desert10101111   = 451,  /* 0x01C3 */
// DELETEME      e__Shore111R1110    = 467,  /* 0x01D3 */
// DELETEME      e__River1111_5      = 472,  /* 0x01D8 */
// DELETEME      e_TT_Ocean2         = 601,  /* 0x0259 */
// DELETEME      e_TT_Count          = 762   /* 0x02FA */
// DELETEME  };

/*
something with base/basic types?
used in NEWG_SetBaseLands__WIP()

    tt_Grasslands1 = 0x0A2,
    tt_Forest1     = 0x0A3,
    tt_Mountain1   = 0x0A4,
    tt_Hills1      = 0x0AB,
    tt_Tundra1     = 0x0A7,

NOT
    tt_Desert1     = 0x0A5,
    tt_Swamp1      = 0x0A6,
    tt_Volcano     = 0x0B3,
    tt_SorceryNode = 0x0A8,
    tt_NatureNode  = 0x0A9,
    tt_ChaosNode   = 0x0AA,

used in NEWG_SetSpecLands()

*/



/*

    tte_Mountain   = 0xA4,  type used by Set_Terrain_Type_Mountain()
    tte_Volcano      = 0xB3,  type used by Set_Terrain_Type_Volcano()
    tte_AnimOcean      = 0x259,  type used by Square_Is_Ocean()

Square_Is_Sailable()
    if(terrain_type > 0x25A  tt_Tundra_1st AKA _Tundra00001000
    if(terrain_type > 0x1D8)    / * _River1111_5 * /
    if(terrain_type > 0x1D3)    / * _Shore111R1110 * /
    if(terrain_type > 0x1C3)    / * _Desert10101111 * /
    if(terrain_type > 0xE8)     / * _Shore000R0000 * /
    if(terrain_type > 0xC4)     / * _River1001_2 * /
    if(terrain_type > 0xA1)     / * _Shore10101111 * /
    if(terrain_type == 0x1  e_TT_BugGrass AKA tt_BugGrass


enum OVL_Tiles_Extended

    _Ocean          = 0x0,
    _Land           = 0x1,

    _Shore00001000  = 0x2,
    _Shore10101111  = 0xA1,

    tte_Grasslands    = 0xA2,
    _Forest1        = 0xA3,

    tte_Mountain   = 0xA4,  type used by Set_Terrain_Type_Mountain()

    _AllDesert1     = 0xA5,
    _Swamp1         = 0xA6,
    _AllTundra1     = 0xA7,
    _SorceryLake    = 0xA8,
    _NatureForest   = 0xA9,
    _ChaosVolcano   = 0xAA,
    _1Hills1        = 0xAB,
    _Grasslands2    = 0xAC,
    _Grasslands3    = 0xAD,
    _AllDesert2     = 0xAE,
    _AllDesert3     = 0xAF,
    _AllDesert4     = 0xB0,
    _Swamp2         = 0xB1,
    _Swamp3         = 0xB2,
    tte_Volcano      = 0xB3,  type used by Set_Terrain_Type_Volcano()
    _Grasslands4    = 0xB4,
    _AllTundra2     = 0xB5,
    _AllTundra3     = 0xB6,
    _Forest2        = 0xB7,
    _Forest3        = 0xB8,

    _River0010      = 0xB9,
    _River1011_4    = 0x102,

    _Mount0010      = 0x103,
    _Mount1001      = 0x112,

    _Hills_0010     = 0x113,
    _1Hills2        = 0x123,

    _Desert00001000 = 0x124,
    _Desert10101111 = 0x1C3,

    _Shore00011R11  = 0x1C4,
    _Shore001R1001  = 0x258,

    tte_AnimOcean      = 0x259,  type used by Square_Is_Ocean()

    _Tundra00001000 = 0x25A,
    _Tundra10101111 = 0x2F9,
*/