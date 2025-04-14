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
        s01p11_empty_function
        s01p12_empty_function
        Play_Left_Click
        Play_Standard_Click
        Play_Left_Click__DUPE
        s01p16_empty_function

*/



#include "Fonts.H"
#include "Input.H"
#include "MOM_Data.H"
#include "MOX_DAT.H"  /* _screen_seg */
#include "MOX_SET.H"  /* magic_set */
#include "MOX_SND.H"
#include "MOX_TYPE.H"
#include "SOUND.H"
#include "Timer.H"
#include "Util.H"




int16_t current_screen;
int16_t previous_screen;



/*
only used twice, from 'Main Screen'

PageFlip_FX()
    PageFlipEffect == 4
    RP_VGA_GrowOutFlip(GrowOutLeft, GrowOutTop, GrowOutFrames, (_screen_seg + 400));
*/
// WZD dseg:9992
int16_t GrowOutFrames;
// WZD dseg:9994
int16_t GrowOutTop;
// WZD dseg:9996
int16_t GrowOutLeft;



// WZD dseg:9C8E
// ...unitialized data...after MSG's, before TBL's...
int16_t PageFlipEffect;



// MGC dseg:27FF                                                 BEGIN: seg001 - Initialized Data  (main)

// MGC dseg:27FF 43 4F 4E 46 49 47 2E 4D 4F 4D 00                CONFIG_MOM db 'CONFIG.MOM',0            ; DATA XREF: _main+12o ...
// MGC dseg:280A 52 75 6E 20 49 4E 53 54 41 4C 4C 20 74 6F 20 63+cnst_ConfigErr db 'Run INSTALL to configure MASTER OF MAGIC.',0Ah
// MGC dseg:280A 4F 46 20 4D 41 47 49 43 2E 0A 0A 00             db 0Ah,0
// MGC dseg:2836 72 62 00                                        cnst_ReadBinary db 'rb',0               ; DATA XREF: _main:@@Found_ConfigMomo ...
// MGC dseg:2836                                                                                         ; "read binary" open mode constant

// MGC dseg:2839
// drake178: cnst_MIDI_Init
// MOM_Data  char str_Initializing_Roland_Drivers[] = "Initializing Roland Drivers...$";

// MGC dseg:2859 53 41 56 45 00                                  cnst_SAVE db 'SAVE',0                   ; DATA XREF: _main+1D9o ...
// MGC dseg:285E 2E 47 41 4D 00                                  cnst_SAVE_ext db '.GAM',0               ; DATA XREF: _main+1F7o ...
// MGC dseg:2863 4D 41 47 49 43 2E 53 45 54 00                   cnst_Set_File db 'MAGIC.SET',0          ; DATA XREF: _main+241o
// MGC dseg:286D 77 62 00                                        cnst_WB db 'wb',0                       ; DATA XREF: _main+23Do
// MGC dseg:2870 46 4F 4E 54 53 2E 4C 42 58 00                   fonts_lbx_file__main db 'FONTS.LBX',0   ; DATA XREF: _main+2AFo
// MGC dseg:287A
char music_lbx__main[] = "music";
// MGC dseg:2880 54 68 61 6E 6B 20 79 6F 75 20 66 6F 72 20 70 6C+str_quit_message__main db 'Thank you for playing Master of Magic!',0
// MGC dseg:2880 61 79 69 6E 67 20 4D 61 73 74 65 72 20 6F 66 20+                                        ; DATA XREF: Menu_Screen_Control+D1o

// MGC dseg:2880                                                 END: seg001 - Initialized Data  (main)



/*
    WIZARDS.EXE seg001
*/

// WZD s01p01
// WZD s01p02

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


// WZD s01p06
// WZD s01p07
// WZD s01p08
// WZD s01p09
// WZD s01p10


// WZD s01p11
// drake178: RP_Empty_Dialog_Fn1()
void s01p11_empty_function(void)
{
// push    bp
// mov     bp, sp
// pop     bp
// retf
}


// WZD s01p12
// drake178: RP_Empty_Dialog_Fn2
void s01p12_empty_function(void)
{
// push    bp
// mov     bp, sp
// pop     bp
// retf
}


// WZD s01p13
// drake178: SND_LeftClickSound()
/*
; if sound effects are enabled, plays the left mouse click sound
*/
/*
Load_Button_Sounds()

SOUNDFX.LBX, 000    SLX19 VO    Standard Button click
SOUNDFX.LBX, 002    OKBUTTN     left button click

*/
void Play_Left_Click__STUB(void)
{
    if(magic_set.sound_effects == ST_TRUE)
    {
        Play_Sound__STUB(snd_left_button_click);
    }
}


// WZD s01p14
// drake178: SND_PlayClickSound()
/*
; if sound effects are enabled, plays the standard mouse click sound
*/
void Play_Standard_Click__STUB(void)
{
    if(magic_set.sound_effects == ST_TRUE)
    {
        Play_Sound__STUB(snd_standard_button_click);
    }
}


// WZD s01p15
// drake178: RP_SND_LeftClickSound2()
/*
; if sound effects are enabled, plays the left mouse click sound
; byte-identical to SND_LeftClickSound, should not exist
*/
/*

*/
void Play_Left_Click__DUPE(void)
{
    if(magic_set.sound_effects == ST_TRUE)
    {
        Play_Sound__STUB(snd_left_button_click);
    }
}


// WZD s01p16
void s01p16_empty_function(void)
{
// push    bp
// mov     bp, sp
// pop     bp
// retf
}



/*
    WIZARDS.EXE  seg009
*/

// WZD s09p01
// lbx_open()
// WZD s09p02
// lbx_close()
// WZD s09p03
// lbx_seek()
// WZD s09p04
// drake178: UU_DISK_GetFileSize()
// WZD s09p05
// lbx_read_sgmt()
// WZD s09p06
// lbx_read_ofst()

// WZD s09p07
/*
    Meh.
*/ 
// void String_Copy_Far(uint16_t dst_ofst, char * dst_sgmt, uint16_t src_ofst, char * src_sgmt)
void String_Copy_Far(char * dst, char * src)
{
    char character;

    while(1)
    {
        character = *src++;
        *dst++ = character;
        if(character == '\0')
        {
            break;
        }
    }

}

void String_Copy_Far__ASM(unsigned short int dst_ofst, unsigned short int dst_sgmt, unsigned short int src_ofst, unsigned short int src_sgmt)
{

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

}
