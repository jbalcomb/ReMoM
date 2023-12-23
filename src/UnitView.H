/*
    Unit View

    WIZARDS.EXE
        ovr072
        ovr078
        ovr089
*/

#ifndef UNITVIEW_H
#define UNITVIEW_H

enum e_VIEW_TYPE
{
    uvt_Prod,
    uvt_Stat,
    uvt_Cmbt
};



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr072
*/



/*
    WIZARDS.EXE  ovr078
*/

// WZD o78p01
// USW_Build_Effect_List

// WZD o78p02
/*
    only called from 'Production Screen'

*/
void Prod_Build_Specials_List(int16_t unit_type, struct s_UV_List * list_buffer, int16_t * list_count);

// WZD o78p03
// drake178: ITEM_GetPowerNames()
// ITEM_GetPowerNames()



/*
    WIZARDS.EXE  ovr089
*/



#ifdef __cplusplus
}
#endif


#endif  /* UNITVIEW_H */
