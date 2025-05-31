


FIGUREX

used to cache figure pictures from FIGURE(S).LBX's
8 directional figures per unit type

¿ operates in half logical pages (8 KB) ?
    ...offset = 0: 512
    ...logical_page = ((figure_set_idx * 3) / 2) === (figure_set_idx * 1.5)
    ...{ 0: 0, 1: 1.5, 2: 3, 3: 4.5, 4: 6, 5: 7.5, 6: 9, ... }



## FIGUREX

Allocate_Data_Space()
    // TODO  EmmHndl_FIGUREX = EMM_GetHandle(28, EmmHndlNm_FIGUREX, 1);
    EmmHndl_FIGUREX = Allocate_Space(28672);  // 28 * 16384 EMM Page Size = 458752 B / 16 = 28672 PR
EMM_FIGUREX_Init__HACK()
    EMM_Map4Pages(logical_page, EmmHndl_FIGUREX)


## EMM_FIGUREX_Init__HACK()

XREF:
    USELESS_Combat_Figure_Load_Compose()
    Combat_Figure_Compose_USEFULL()
    j_EMM_FIGUREX_Init__HACK()
        BU_CreateImage__SEGRAX()
        Combat_Figure_Load()
