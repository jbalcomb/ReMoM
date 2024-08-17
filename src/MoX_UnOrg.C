/*
    WIZARDS.EXE
        ...
        seg001
        ...
        seg009
        ...
        ovr063
        ...
        ovr067
        ...
*/

/*
    Meh.
    ...was part of MoM_Init.C
    __not sure what to do with ovr052__
    ...or ovr51
    or how they relate to Init, Loader, Loaded_Game_Update, etc.

*/



// WZD s01p03
// PageFlip_FX()

// WZD o60p07
// TILE_VisibilityUpdt()



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
        Get_Weighted_Choice    
        RNG_WeightedPick32    
        UU_RNG_HighestPick16  
        UU_RNG_HighestPick32  
        RP_Empty_Dialog_Fn1   
        RP_Empty_Dialog_Fn2   
        SND_LeftClickSound    
        SND_PlayClickSound    
        RP_SND_LeftClickSound2
        s01p15_Empty_pFxn     

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
// 
/*
    Default: Apply_Palette(); Toggle_Pages();
    Main Screen sets it to 4 before calling Outpost_Screen()
    NameStartingCity_Dialog_Popup sets it to 3 before returning to Main Screen
    Load Screen sets it to 2 when preparing to loop back to Main Screen
    Production Screen sets it to 3 before returning to City Screen / CityList Screen
*/
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
            // TODO  RP_VGA_CutRight();
        } break;
        case 2:
        {
            Toggle_Pages();  // |-> Page_Flip()
            Fade_In();
        } break;
        case 3:
        {
            Apply_Palette();
            // TODO  VGA_MosaicFlip();  // |-> Toggle_Pages() |-> Page_Flip()
            Toggle_Pages();  // |-> Page_Flip()
        } break;
        case 4:
        {
            PageFlip_GrowOut__WIP(GrowOutLeft, GrowOutTop, GrowOutFrames, (_screen_seg + (20 * SCREEN_WIDTH)));
        } break;
        default:
        {
            // Apply_Palette();
        } break;
    }

    PageFlipEffect = 0;  // DEDUCE: PageFlipEffect 0 is the default PageFlipEffect

}



// WZD s01p04
void Fade_Out(void)
{
    int16_t fade_percent;

    for(fade_percent = 25; fade_percent < 101; fade_percent += 25)
    {
        Mark_Time();
        Set_Palette_Changes(0, 255);
        Cycle_Palette(fade_percent);
        Release_Time(1);
    }

}


// WZD s01p05
void Fade_In(void)
{
    int16_t fade_percent;
    
    for(fade_percent = 75; fade_percent >= 0; fade_percent -= 25)
    {
        Mark_Time();
        Set_Palette_Changes(0, 255);
        Cycle_Palette(fade_percent);
        Release_Time(1);
    }

}



/*
    WIZARDS.EXE  seg009
*/

// WZD s09p07
void String_Copy_Far__STUB(unsigned short int dst_ofst, unsigned short int dst_sgmt, unsigned short int src_ofst, unsigned short int src_sgmt)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: String_Copy_Far__STUB()\n", __FILE__, __LINE__);
#endif

//     if(dst_sgmt == 0)
//     {
//         dst_sgmt = _DS_;
//     }
// 
//     if(src_sgmt == 0)
//     {
//         src_sgmt = _DS_;
//     }
// 
//     _ES_ = dst_sgmt;
//     _SI_ = src_ofst;
//     _DI_ = dst_ofst;
//     _DS_ = src_sgmt;
// 
//     
//     do {
//         *(dst_sgmt + dst_ofst++) = *(src_sgmt + src_ofst++);
// 
//     } while(*(src_sgmt + src_ofst) != '\0')

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: String_Copy_Far__STUB()\n", __FILE__, __LINE__);
#endif
}
