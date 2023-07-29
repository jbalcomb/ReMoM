
/*
    Meh.
    ...was part of MoM_Init.C
    not sure what to do with ovr052
    ...or ovr51
    or how they relate to Init, Loader, Loaded_Game_Update, etc.

*/



// WZD s01p03
// PageFlip_FX()

// WZD o60p07
// TILE_VisibilityUpdt()

// WZD o63p15
// Check_Planar_Seal()

// WZD o67p15
// IsPassableTower()



/*
    // ~== MGC & WZD main()
    // MGC  s01p01
    // WZD  s01p01
    void MoM_Init(void);
*/
/*
    ~== {MGC main(), WZD main()}
    *.* |-> Screen_Control()

    MAGIC.EXE
    seg001
    s01p01
    WIZARDS.EXE
    seg001
    s01p01

    MoO2
        Module: INITGAME
        Module: init
*/

/*
Init_STGE()
Init_MoX()
Init_MoM()
Init_MGC()
Init_WZD()
*/

/*
    MGC
        s01p04  Load_MGC_Resources
        s01p07  Load_TERRSTAT
        s01p08  Load_SPELLDAT
    WZD
        o52p01  Load_WZD_Resources

OON XREF:

*/

/*
    ~== MAGIC/WIZARDS.EXE |-> main()
*/
// MGC s01p01
// WZD s01p01
// MoM_Init:  void MoM_main(void)

/*
    MAGIC.EXE
    seg001
        _main                
        SND_Empty_Fn         
        Menu_Screen_Control  
        Load_MGC_Resources   
        Main_Menu_Screen     
        Main_Menu_Screen_Draw
        Load_TERRSTAT        
        Load_SPELLDAT        
        UU_HLP_Load_MainMenu 

    WIZARDS.EXE
    seg001
        _main                 
        Screen_Control        
        PageFlip_FX           
        VGA_Fade_Out          
        Fade_In               
        Loaded_Game_Update_WZD
        RNG_WeightedPick16    
        RNG_WeightedPick32    
        UU_RNG_HighestPick16  
        UU_RNG_HighestPick32  
        RP_Empty_Dialog_Fn1   
        RP_Empty_Dialog_Fn2   
        SND_LeftClickSound    
        SND_PlayClickSound    
        RP_SND_LeftClickSound2
        s01p15_Empty_pFxn     
    
    ovr051
    ovr052
    ovr060
    ovr063
    ovr067

*/



#include "MoX.H"



// WZD dseg:9C8E
// ...unitialized data...after MSG's, before TBL's...
int16_t PageFlipEffect;



/*
    WIZARDS.EXE seg001
*/

// WZD s01p03
// ; executes a VGA page flip using a special effect for
// ; screen transition set through VGA_PageFlipEffect,
// ; which is zeroed out afterward:
// ;   0 - simple flip, no extra effect
// ;   1 - left to right cut (not used in the game)
// ;   2 - black cut with the new image fading in
// ;   3 - mosaic effect
// ;   4 - grow out from a tile (not used in the game)
// ;   5 - writes out the DAC, but DOES NOT do a page flip
// Default: Apply_Palette(); Toggle_Pages();
// WZD s01p03
// ¿ MoO2: Set_Palette_Changes() ?
void PageFlip_FX(void)
{

    switch(PageFlipEffect)
    {
        case 0:
        {
            Apply_Palette();
            Toggle_Pages();  // |-> Page_Flip()
        } break;
        case 1:
        {
            // RP_VGA_CutRight();
        } break;
        case 2:
        {
            // Toggle_Pages();  // |-> Page_Flip()
            // Fade_In();
        } break;
        case 3:
        {
            // Apply_Palette();
            // VGA_MosaicFlip();  // |-> GUI_PageFlip() |-> Page_Flip()
        } break;
        case 4:
        {
            // RP_VGA_GrowOutFlip(RP_GUI_GrowOutLeft, RP_GUI_GrowOutTop, RP_GUI_GrowOutFrames, _screen_seg + 400)             
        } break;
        default:
        {
            // Apply_Palette();
        } break;
    }

    PageFlipEffect = 0;  // DEDUCE: PageFlipEffect 0 is the default PageFlipEffect

}


/*
    WIZARDS.EXE ovr060
*/

// WZD o60p07
void TILE_VisibilityUpdt(void)
{
    int16_t itr_players;
    int16_t itr_units;
    int16_t tmp_unit_enchantments_hiword;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        // TODO(JimBalcomb,2023075): figure out the indexing in the Dasm - doesn't look like array of struct  also, this'll set the neutral player?
        _players[itr_players + 1].Dipl.Contacted[0] = 1;

        if(_players[itr_players + 1].Globals.Nature_Awareness != ST_FALSE)
        {
            for(itr_units = 0; itr_units < _units; itr_units++)
            {
                if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
                {
                    if(_UNITS[itr_units].owner_idx != itr_players)
                    {
                        if(_UNITS[itr_units].owner_idx != NEUTRAL_PLAYER_IDX)
                        {
                            // BUG: only checks enchantment, not ability or item
                            tmp_unit_enchantments_hiword = _UNITS[itr_units].Enchants_HI;  // // ; enum UE_FLAGS_H
                            // UE_Invisibility 0x8000
                            if( (tmp_unit_enchantments_hiword & 0x8000) != 0 )
                            {
                                _players[itr_players].Dipl.Contacted[_UNITS[itr_units].owner_idx] = 1;
                                _players[_UNITS[itr_units].owner_idx].Dipl.Contacted[itr_players] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    Update_Scouted_And_Contacted();
    // TST  Validate_Square_Scouted(18,11,0);

}

/*
    WIZARDS.EXE ovr063
*/

// WZD o63p15
int16_t Check_Planar_Seal(void)
{
    int16_t itr_players;
    int16_t active_planar_seal;

    active_planar_seal = ST_FALSE;

    itr_players = 0;
    while(itr_players++ < _num_players)
    {

        if(active_planar_seal == ST_FALSE)
        {
            if(_players[itr_players].Globals.Planar_Seal == ST_TRUE)
            {
                active_planar_seal = ST_TRUE;
            }
        }

    }

    return active_planar_seal;
}


/*
    WIZARDS.EXE ovr067
*/

// WZD o67p15
int16_t IsPassableTower(int16_t world_x, int16_t world_y)
{
    int16_t itr_towers;
    int16_t is_passible_tower;
    int16_t active_planar_seal;

    is_passible_tower = ST_FALSE;

    itr_towers = 0;
    while(itr_towers++ < TOWER_COUNT_MAX)
    {
        if(world_x == _TOWERS[itr_towers].world_x && world_y == _TOWERS[itr_towers].world_y)
        {
            is_passible_tower = ST_TRUE;
        }
    }

    if(is_passible_tower == ST_TRUE)
    {
        if(Check_Planar_Seal() == ST_TRUE)
        {
            is_passible_tower = ST_FALSE;
        }
    }

    return is_passible_tower;
}
