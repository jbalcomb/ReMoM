/*
    MAGIC.EXE ovr058
*/

#include "INTRO.h"

#include "MOX/Allocate.h"
#include "MOX/MOX_DAT.h"
#include "MOX/LBX_Load.h"
#include "MOX/MOX_DEF.h"
#include "MOX/MOX_SET.h"
#include "MOX/MOX_TYPE.h"
#include "MOX/sdl2_Audio.h"
#include "MOX/SOUND.h"

#include "MGC_DATA.h"



// MGC dseg:385E                                                 BEGIN:  ovr058 - Initialized Data  (Intro/Logos)

// MGC dseg:385E
int16_t IntroAnim_FrameCounts__MGC_ovr058[13] = { 15,   12,   30,   18,   25,   32,   24,   32,   30,   37,   30,   11,   32 };

// MGC dseg:3878
char music_lbx_file__MGC_ovr058[] = "music.lbx";

// MGC dseg:3881
char str_empty_string__MGC_ovr058[] = "";

// MGC dseg:3882
char soundfx_lbx_file__MGC_ovr058[] = "soundfx.lbx";

// MGC dseg:388E
char str_ESC__MGC_ovr058[] = "\x1B";

// MGC dseg:3890
char intro_lbx_file__MGC_ovr058[] = "INTRO.LBX";

// MGC dseg:389A
char str_SIMTEX_COPYRIGHT_1995_V131__MGC_ovr058[] = "Copyright  Simtex Software, 1995   V1.31";

// MGC dseg:389A                                                 END:  ovr058 - Initialized Data  (Intro/Logos)



// MGC o58p1
/*
plays the intro cinematic, can be interrupted by
pressing the Escape key or clicking anywhere on the
screen
*/
/*

*/
void Draw_Logos(void)
{
    uint8_t colors[2] = { 0, 0 };
    int16_t hotkey_idx_ESC = 0;
    SAMB_ptr digi_sound_buffer2 = 0;
    SAMB_ptr digi_sound_buffer1 = 0;
    SAMB_ptr digi_sound_buffer3 = 0;
    SAMB_ptr midi_sound_buffer = 0;
    int16_t leave_screen = 0;
    int16_t input_idx = 0;
    int16_t fullscreen_field = 0;
    int16_t itr_scenes = 0;  // _SI_
    int16_t itr_frames = 0;  // _DI_
    /* HACK */  uint32_t midi_sound_buffer_size = 0;  // DNE in Dasm
    /* HACK */  uint32_t sound_buffer_size1 = 0;  // DNE in Dasm
    /* HACK */  uint32_t sound_buffer_size2 = 0;  // DNE in Dasm
    /* HACK */  uint32_t sound_buffer_size3 = 0;  // DNE in Dasm

    colors[0] = 79;
    colors[1] = 163;
    
    // midi_sound_buffer = LBX_Reload(music_lbx_file__MGC_ovr058, MUSIC_Intro, _screen_seg);  // MOM49.XMI  "Intro music"
    Allocate_Next_Block(_screen_seg, 1);
    midi_sound_buffer = LBX_Reload_Next(music_lbx_file__MGC_ovr058, MUSIC_Intro, _screen_seg);  // MOM49.XMI  "Intro music"
    midi_sound_buffer_size = lbxload_entry_length;

    Mark_Block(_screen_seg);

    digi_sound_buffer1 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_IntroT1, _screen_seg);
    sound_buffer_size1 = lbxload_entry_length;

    if(magic_set.background_music == ST_TRUE)
    {

        // DOMSDOS  Play_Sound__WIP(midi_sound_buffer);
        sdl2_Play_Sound__WIP(midi_sound_buffer, midi_sound_buffer_size);
    }

    Set_Mouse_List(1, mouse_list_none);  // in MGC, second set of mouse list definitions?  mouse_list__MGC_HoF
    
    Clear_Fields();

    fullscreen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_empty_string__MGC_ovr058[0], ST_UNDEFINED);

    hotkey_idx_ESC = Add_Hot_Key(str_ESC__MGC_ovr058[0]);

    // 13 SCENE's, in INTRO.LBX
    for(itr_scenes = 0; itr_scenes < 13; itr_scenes++)
    {
        Open_File_Animation__HACK(intro_lbx_file__MGC_ovr058, itr_scenes);

        leave_screen = ST_FALSE;

        /*
            Dasm looks jumpy
            Could be a switch?
            ...with a default?
            ¿ only matches { 0, 1, 2, 3 } ?
        */
        if(itr_scenes == 0)
        {
            Intro_Fade_Out_2();
        }
        else if(itr_scenes == 1)
        {
            Intro_Fade_Out_20();
        }
        else if(itr_scenes == 2)
        {
            Intro_Fade_Out_10();
        }
        else if(itr_scenes == 3)
        {
            Intro_Fade_Out_20();
        }
        else
        {
            Intro_Fade_Out_2();
        }

        for(itr_frames = 0; IntroAnim_FrameCounts__MGC_ovr058[itr_scenes] > itr_frames; itr_frames++)
        {

            Mark_Time();

            input_idx = Get_Input();

            if(
                (input_idx == fullscreen_field)
                ||
                (input_idx == hotkey_idx_ESC)
            )
            {
                leave_screen = ST_TRUE;
            }

            Set_Page_Off();

            if(itr_frames == 0)
            {
                CLS();
            }

            Draw_File_Animation__HACK();

            if(
                (itr_scenes == 2)
                &&
                (itr_frames > 6)
            )
            {
                Set_Font_Colors_15(0, &colors[0]);
                Print_Centered(160, 192, str_SIMTEX_COPYRIGHT_1995_V131__MGC_ovr058);
            }

            if(
                (itr_scenes == 4)
                &&
                (itr_frames == 0)
            )
            {
                if(magic_set.sound_effects == ST_TRUE)
                {
                    // DOMSDOS  Play_Sound__WIP(digi_sound_buffer1);
                    sdl2_Play_Sound__WIP(digi_sound_buffer1, sound_buffer_size1);
                }
            }

            if(
                (itr_scenes == 5)
                &&
                (itr_frames == 10)
            )
            {
                Release_Block(_screen_seg);
                Mark_Block(_screen_seg);

                digi_sound_buffer1 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_IntroM, _screen_seg);  // [Merlin] "You have come too late."
                sound_buffer_size1 = lbxload_entry_length;

                digi_sound_buffer2 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_Unknown116, _screen_seg);  // ; [Merlin] "My work has already met with success."
                sound_buffer_size2 = lbxload_entry_length;

                if(magic_set.sound_effects == ST_TRUE)
                {
                    // DOMSDOS  Play_Sound__WIP(digi_sound_buffer1);
                    sdl2_Play_Sound__WIP(digi_sound_buffer1, sound_buffer_size1);
                }
            }

            if(
                (itr_scenes == 6)
                &&
                (itr_frames == 0)
            )
            {
                if(magic_set.sound_effects == ST_TRUE)
                {
                    // DOMSDOS  Play_Sound__WIP(digi_sound_buffer2);
                    sdl2_Play_Sound__WIP(digi_sound_buffer2, sound_buffer_size2);
                }
            }

            if(
                (itr_scenes == 7)
                &&
                (itr_frames == 2)
            )
            {
                Release_Block(_screen_seg);
                Mark_Block(_screen_seg);

                digi_sound_buffer1 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_IntroT2, _screen_seg);
                sound_buffer_size1 = lbxload_entry_length;

                digi_sound_buffer3 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_ATK_LightningBolt, _screen_seg);
                sound_buffer_size3 = lbxload_entry_length;

                if(magic_set.sound_effects == ST_TRUE)
                {
                    // DOMSDOS  Play_Sound__WIP(digi_sound_buffer1);
                    sdl2_Play_Sound__WIP(digi_sound_buffer1, sound_buffer_size1);
                }
            }

            if(
                (itr_scenes == 9)
                &&
                (itr_frames == 0)
            )
            {
                Release_Block(_screen_seg);
                Mark_Block(_screen_seg);

                digi_sound_buffer1 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_Unknown118, _screen_seg);
                sound_buffer_size1 = lbxload_entry_length;

                digi_sound_buffer3 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_ATK_LightningBolt, _screen_seg);
                sound_buffer_size3 = lbxload_entry_length;

                if(magic_set.sound_effects == ST_TRUE)
                {
                    // DOMSDOS  Play_Sound__WIP(digi_sound_buffer1);
                    sdl2_Play_Sound__WIP(digi_sound_buffer1, sound_buffer_size1);
                }
            }

            if(
                (itr_scenes == 11)
                &&
                (itr_frames == 2)
            )
            {
                // BUGBUG  release digi_sound_buffer3 here, but uses it below ... the load above should probably be in this block
                Release_Block(_screen_seg);
                Mark_Block(_screen_seg);

                digi_sound_buffer1 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_IntroT3, _screen_seg);
                sound_buffer_size1 = lbxload_entry_length;

                if(magic_set.sound_effects == ST_TRUE)
                {
                    // DOMSDOS  Play_Sound__WIP(digi_sound_buffer1);
                    sdl2_Play_Sound__WIP(digi_sound_buffer1, sound_buffer_size1);
                }
            }

            if(
                (itr_scenes == 8)
                &&
                (itr_frames == 3)
            )
            {
                if(magic_set.sound_effects == ST_TRUE)
                {
                    // DOMSDOS  Play_Sound__WIP(digi_sound_buffer3);
                    sdl2_Play_Sound__WIP(digi_sound_buffer3, sound_buffer_size3);
                }
            }

            if(
                (itr_scenes == 8)
                &&
                (itr_frames == 9)
            )
            {
                if(magic_set.sound_effects == ST_TRUE)
                {
                    // DOMSDOS  Play_Sound__WIP(digi_sound_buffer3);
                    sdl2_Play_Sound__WIP(digi_sound_buffer3, sound_buffer_size3);
                }
            }

            if(
                (itr_scenes == 10)
                &&
                (itr_frames == 2)
            )
            {
                if(magic_set.sound_effects == ST_TRUE)
                {
                    // DOMSDOS  Play_Sound__WIP(digi_sound_buffer3);
                    sdl2_Play_Sound__WIP(digi_sound_buffer3, sound_buffer_size3);
                }
            }

            Toggle_Pages();

            if((itr_frames == 0) && (itr_scenes > 2))
            {
                Intro_Fade_In_2();
            }
            else if((itr_frames == 0) && ((itr_scenes == 0) || (itr_scenes == 1) || (itr_scenes == 2)))
            {
                Intro_Fade_In_50();
            }

            Copy_On_To_Off_Page();

            Release_Time(3);

            if(leave_screen == ST_TRUE)
            {
                itr_frames = IntroAnim_FrameCounts__MGC_ovr058[itr_scenes];
            }

        }

        if(leave_screen == ST_TRUE)
        {
            itr_scenes = 13;
        }

    }
    
}


// MGC o58p2
void Intro_Fade_Out_2(void)
{
    int16_t itr;  // _SI_

    for(itr = 50; itr < 101; itr += 50)
    {

        Mark_Time();

        Set_Palette_Changes(0, 255);

        Cycle_Palette(itr);

        Release_Time(1);

    }

}


// MGC o58p3
void Intro_Fade_Out_10(void)
{
    int16_t itr;  // _SI_

    for(itr = 10; itr < 101; itr += 10)
    {

        Mark_Time();

        Set_Palette_Changes(0, 255);

        Cycle_Palette(itr);

        Release_Time(1);

    }

}


// MGC o58p4
void Intro_Fade_Out_20(void)
{
    int16_t itr;  // _SI_

    for(itr = 5; itr < 101; itr += 5)
    {

        Mark_Time();

        Set_Palette_Changes(0, 255);

        Cycle_Palette(itr);

        Release_Time(1);

    }

}


// MGC o58p5
void Intro_Fade_In_2(void)
{
    int16_t itr;  // _SI_

    for(itr = 100; itr >= 0; itr -= 50)
    {

        Mark_Time();

        Set_Palette_Changes(0, 255);

        Cycle_Palette(itr);

        Release_Time(1);

    }

}


// MGC o58p6
void Intro_Fade_In_50(void)
{
    int16_t itr;  // _SI_

    for(itr = 10; itr >= 0; itr -= 2)
    {

        Mark_Time();

        Set_Palette_Changes(0, 255);

        Cycle_Palette(itr);

        Release_Time(1);

    }

}

