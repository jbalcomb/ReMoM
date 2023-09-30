/*
    Terrain
    Terrain Types
    Terrain Type Attributes


    WIZARDS.EXE
        ovr161
    
*/

#include "MoX.H"

// TerType_Count 0x2FA 762



// WZD s161p01
// TILE_ClearCorruption 
// WZD s161p02
// TILE_IsNode          
// WZD s161p03
// TILE_GetFood         
// WZD s161p04
// TILE_GetProd         
// WZD s161p05
// TILE_GetWaterGold    
// WZD s161p06
// CTY_GetRoadGold      
// WZD s161p07
// TILE_IsRiver         
// WZD s161p08
// TILE_GetSpecialGold  
// WZD s161p08
// TILE_GetSilverGold   
// WZD s161p09
// TILE_GetGoldOreGold  
// WZD s161p10
// TILE_GetGemGold      
// WZD s161p11
// TILE_GetSpecialPower 
// WZD s161p12
// TILE_GetMithrilPower 
// WZD s161p13
// TILE_GetAdamntmPower 
// WZD s161p14
// TILE_GetQuorkPower   
// WZD s161p15
// TILE_GetCrysxPower   
// WZD s161p16
// TILE_HasMithril      
// WZD s161p17
// TILE_HasAdamantium   
// WZD s161p18
// TILE_GetNUCostReduce 
// WZD s161p19
// CTY_GetWeaponQuality 
// WZD s161p20
// TILE_GetRoadBldTime  
// WZD s161p21
// UU_TILE_GetUnsdMPCost
// WZD s161p22
// TILE_IsAISailable    
// WZD s161p23
// TILE_IsVisibleForest 

// WZD s161p24
// drake178: TILE_IsSailable()
int16_t Terrain_Is_Sailable(int16_t world_x, int16_t world_y, int16_t map_plane)
{
    int16_t return_value;
    uint8_t * src_sgmt;
    uint16_t src_ofst;
    uint16_t world_map_value;
    uint16_t terrain_type;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Terrain_Is_Sailable(world_x = %d, world_y = %d, map_plane = %d)\n", __FILE__, __LINE__, world_x, world_y, map_plane);
#endif

    // les  bx, [_world_maps]
    // ~== ES = (&_world_maps[0] / 16); BX = (&_world_maps[0] % 16);
    src_sgmt = _world_maps;
    src_ofst = 0;

    src_ofst = (map_plane * 4800) + (world_y * 120) + (world_x * 2);
    world_map_value = GET_2B_OFS(src_sgmt, src_ofst);

    terrain_type = world_map_value % TERRAIN_TYPE_COUNT;

    if(terrain_type > 0x25A)  /* _Tundra00001000 */
    {
        goto Return_False;
    }
    else
    {
        if(terrain_type > 0x1D8)  /* _River1111_5 */
        {
            goto Return_True;
        }
        else
        {
            if(terrain_type > 0x1D3)  /* _Shore111R1110 */
            {
                goto Return_False;
            }
            else
            {
                if(terrain_type > 0x1C3)  /* _Desert10101111 */
                {
                    goto Return_True;
                }
                else
                {
                    if(terrain_type > 0xE8)  /* _Shore000R0000 */
                    {
                        goto Return_False;
                    }
                    else
                    {
                        if(terrain_type > 0xC4)  /* _River1001_2 */
                        {
                            goto Return_True;
                        }
                        else
                        {
                            if(terrain_type > 0xA1)  /* _Shore10101111 */
                            {
                                goto Return_False;
                            }
                            else
                            {
                                if(terrain_type == 0x1)  /* TT_BugGrass */
                                {
                                    goto Return_False;
                                }
                                else
                                {
                                    goto Return_True;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

Return_False:
    return_value = ST_FALSE;
    goto Done;

Return_True:
    return_value = ST_TRUE;
    goto Done;

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Terrain_Is_Sailable(world_x = %d, world_y = %d, map_plane = %d)\n", __FILE__, __LINE__, world_x, world_y, map_plane);
#endif
    return return_value;
}

// WZD s161p25
// TILE_IsDeepOcean     
// WZD s161p26
// G_TILE_IsAIEmbarkable
// WZD s161p27
// TILE_BuildingReqType 
// WZD s161p28
// TILE_IsHills         
// WZD s161p29
// TILE_IsMountains     
// WZD s161p30
// TILE_IsDesert        
// WZD s161p31
// TILE_IsTundra        
// WZD s161p32
// TILE_IsSwamp         
// WZD s161p33
// TILE_IsGrasslands    
// WZD s161p34
// TILE_IsVolcano       
// WZD s161p35
// TILE_IsCorrupted     
// WZD s161p36
// UU_TILE_IsRes_40h    
// WZD s161p37
// CTY_CountNightshades 
// WZD s161p38
// TILE_IsLand          
// WZD s161p39
// TILE_IsAISailable2   
// WZD s161p40
// UU_TILE_BotchedResFn 
