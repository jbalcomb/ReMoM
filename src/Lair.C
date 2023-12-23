/*
    Lair

    WIZARDS.EXE
        ovr083

BACKGRND.LBX
    LCONFIRM    lair confirm borde
    LCONFIRM    lair bottom w/butt
    LCONFIRM    lair bottom wo/but

NOTE: IDGI, but the FanDom MoM Wiki uses the term "Encounter Zone"
*/

#include "MoX.H"


// WZD o83p01
// drake178: TILE_HasEncounter()
int16_t Square_Has_Lair(int16_t world_x, int16_t world_y, int16_t map_plane)
{
    int16_t square_has_lair;
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Square_Has_Lair(world_x = %d, world_y = %d, map_plane = %d)\n", __FILE__, __LINE__, world_x, world_y, map_plane);
#endif

    square_has_lair = -1;

    for(itr = 0; itr < NUM_LAIRS && square_has_lair == -1; itr++)
    {
        if( (_LAIRS[itr].wp == map_plane) && (_LAIRS[itr].Intact == ST_TRUE) && (_LAIRS[itr].wy = world_y) && (_LAIRS[itr].wx = world_x) )
        {
            square_has_lair = itr;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Square_Has_Lair(world_x = %d, world_y = %d, map_plane = %d)\n", __FILE__, __LINE__, world_x, world_y, map_plane);
#endif
    return square_has_lair;
}

// WZD o83p02
// EZ_Clear            ();
// WZD o83p03
// sub_6D490           ();
// WZD o83p04
// EZ_Resolve          ();
// WZD o83p05
// OVL_EZ_Confirm_Entry();
// WZD o83p06
// EZ_TreasureDialog   ();
// WZD o83p07
// EZ_GoldManaReward   ();

// WZD o83p08
// drake178: STR_ListSeparator()
void STR_ListSeparator(int16_t * size, int16_t count, char * list)
{
    *size++;

    if(*size == count)
    {
        if(*size > 1)
        {
            strcat(list, " and ");
        }
    }
    else
    {
        if(*size > 1)
        {
            strcat(list, ", ");
        }
    }

}


// WZD o83p09
// STR_GetIndefinite   ();
// WZD o83p10
// EZ_GenerateRewards  ();
// WZD o83p11
// OVL_EZ_Confirm_Dlg  ();
// WZD o83p12
// OVL_DrawEZConfirm   ();
// WZD o83p13
// EZ_SpecialTreasure  ();
// WZD o83p14
// EZ_GetBookRealm     ();

