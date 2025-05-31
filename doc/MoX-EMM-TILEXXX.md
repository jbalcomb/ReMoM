


TILEXXX



## TILEXXX

Allocate_Data_Space()
    // TODO  EmmHndl_TILEXXX = EMM_GetHandle( 3, EmmHndlNm_TILEXXX, 1);
    EmmHndl_TILEXXX = Allocate_Space(3072);   //  3 * 16384 EMM Page Size =  49152 B / 16 =  3072 PR
EMM_TILEX_Init__HACK()
    EMM_Map4Pages(0, EmmHndl_TILEXXX)
